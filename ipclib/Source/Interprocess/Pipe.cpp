#include "Pipe.h"
#include "IPCAssert.h"

#include <iostream>

#if defined(WIN32) || defined(_WINDOWS)
#define windows
#endif

#if defined(__linux__)
#define linux 
#endif

Pipe::io_op_result::io_op_result(bool result, size_t bytes_read_or_written) :
	success(result), io_bytes(bytes_read_or_written) { }

Pipe::io_op_result::io_op_result(bool result, size_t bytes_read_or_written, size_t bytes_available_in_pipe) : 
	success(result), io_bytes(bytes_read_or_written), total_bytes(bytes_available_in_pipe), is_total_bytes_learned(true) { }


Pipe::Pipe(size_t buffer_size_recommendation, std::shared_ptr<Event> external_io_event) : 
	_io_event(external_io_event)
{
	_open(buffer_size_recommendation);
}

//Pipe::Pipe(size_t buffer_size_recommendation, Event::handle_t external_io_event) :
//	Pipe(buffer_size_recommendation, std::make_shared<Event>(external_io_event)) { }
//
//Pipe::Pipe(size_t buffer_size_recommendation, size_t external_io_event) :
//	Pipe(buffer_size_recommendation, std::make_shared<Event>(external_io_event)) { }


Pipe::Pipe(Pipe::handle_t write_handle, Pipe::handle_t read_handle, std::shared_ptr<Event> external_io_event)
{
	_handle_write = write_handle;
	_handle_read = read_handle;
	
	_is_write_open = _handle_write != nullptr;
	_is_read_open = _handle_read != nullptr;

	_io_event = external_io_event;
	set_ownership(true, true);	// handles are always owned by default!
}

Pipe::Pipe(Pipe::handle_t write_handle, Pipe::handle_t read_handle, Event::handle_t external_io_event)
	: Pipe(reinterpret_cast<Pipe::handle_t>(write_handle), reinterpret_cast<Pipe::handle_t>(read_handle), std::make_shared<Event>(external_io_event)) {}

Pipe::Pipe(Pipe::handle_t write_handle, Pipe::handle_t read_handle, size_t external_io_event)
	: Pipe(reinterpret_cast<Pipe::handle_t>(write_handle), reinterpret_cast<Pipe::handle_t>(read_handle), std::make_shared<Event>(external_io_event)) {}

Pipe::Pipe(size_t write_handle_int, size_t read_handle_int, std::shared_ptr<Event> external_io_event)
	: Pipe(reinterpret_cast<Pipe::handle_t>(write_handle_int), reinterpret_cast<Pipe::handle_t>(read_handle_int), external_io_event) {}

Pipe::Pipe(size_t write_handle_int, size_t read_handle_int, Event::handle_t external_io_event)
	: Pipe(reinterpret_cast<Pipe::handle_t>(write_handle_int), reinterpret_cast<Pipe::handle_t>(read_handle_int), std::make_shared<Event>(external_io_event)) {}

Pipe::Pipe(size_t write_handle_int, size_t read_handle_int, size_t external_io_event_int)
	: Pipe(reinterpret_cast<Pipe::handle_t>(write_handle_int), reinterpret_cast<Pipe::handle_t>(read_handle_int), std::make_shared<Event>(external_io_event_int)) {}

Pipe::~Pipe()
{
	close();
}


std::shared_ptr<Event> Pipe::get_io_event()
{
	if (_io_event == nullptr) _open_io_event();

	return _io_event;
}

void Pipe::set_io_event(std::shared_ptr<Event> external_event)
{
	_io_event = external_event;
}

void Pipe::set_event_trigger_condition(int condition_flags)
{
	_event_trigger_flag = condition_flags;
}

bool Pipe::_open(size_t buffer_size_recommendation)
{
	bool result;
	
#ifdef windows

	SECURITY_ATTRIBUTES security = {
			sizeof(security), nullptr, true
	};

	result = CreatePipe(&_handle_read, &_handle_write, &security, buffer_size_recommendation);

#endif 
#ifdef linux

	result = false;
	std::cout << "[OS Error] Pipes in linux based systems are not supported" << std::endl;
	ASSERT(false);

#endif

	_is_read_open = result;
	_is_write_open = result;
	set_ownership(result, result /*false, false*/);	// handles are always owned by default!

	return result;
}

bool Pipe::_open_io_event()
{
	bool result;

	_io_event = std::make_shared<Event>();
	result = _io_event != nullptr;

	return result;
}

bool Pipe::_trigger_io_event()
{
	if (_io_event == nullptr) return false;
	return _io_event->set();
}


bool Pipe::close_read()
{
	if (!_is_read_owned) return true;

	bool result;
#ifdef windows

	result = CloseHandle(_handle_read);

#endif 
#ifdef linux

	result = false;
	std::cout << "[OS Error] Pipes in linux based systems are not supported" << std::endl;
	ASSERT(false);

#endif

	if (result) _handle_read = nullptr;
	_is_read_open = false;
	return result;
}

bool Pipe::close_write()
{
	if (!_is_write_owned) return true;

	bool result;
#ifdef windows

	result = CloseHandle(_handle_write);

#endif 
#ifdef linux

	result = false;
	std::cout << "[OS Error] Pipes in linux based systems are not supported" << std::endl;
	ASSERT(false);

#endif

	if (result) _handle_write = nullptr;
	_is_write_open = false;
	return result;
}

void Pipe::set_ownership(bool is_read_handle_owned, bool is_write_handle_owned)
{
	if (_is_read_open) _is_read_owned = is_read_handle_owned;
	if (_is_write_open) _is_write_owned = is_write_handle_owned;
}

bool Pipe::close() {
	bool result = true;
	
	result &= close_read();
	result &= close_write();

	return result;
}

Pipe::io_op_result Pipe::clear()
{
	const int buffer_size = 256;

	io_op_result result(true, 0);

	char buffer[buffer_size];
	while (!is_empty()) {
		io_op_result result_of_iteration = read(buffer, buffer_size * sizeof(char), 0, "");
		result.success &= result_of_iteration.success;
		result.io_bytes += result_of_iteration.io_bytes;
	}

	return result;
}

bool Pipe::is_empty()
{
	return get_size() == 0;
}

size_t Pipe::get_size()
{
	io_op_result result;
	result = peek(nullptr, 0, 0, "");

	ASSERT(result.is_total_bytes_learned);
	return result.total_bytes;
}

const Pipe::handle_t Pipe::get_read_handle()
{
	return _handle_read;
}

const Pipe::handle_t Pipe::get_write_handle()
{
	return _handle_write;
}

const size_t Pipe::get_read_handle_int()
{
	return reinterpret_cast<size_t>(get_read_handle());
}

const size_t Pipe::get_write_handle_int()
{
	return reinterpret_cast<size_t>(get_write_handle());
}

Pipe::io_op_result Pipe::write(const void* source_buffer, size_t buffer_size_in_byte, size_t offset_in_byte, const char* endline_character)
{
	if (!_is_write_open) {
		std::cout << "[OS Error] Pipe::write() is called but write handle is closed" << std::endl;
		ASSERT(false);
	}

	io_op_result result;

#ifdef windows
	
	DWORD bytes_written;
	result.success = WriteFile(_handle_write , (unsigned char*)source_buffer + offset_in_byte, buffer_size_in_byte - offset_in_byte, &bytes_written, nullptr);
	result.io_bytes = bytes_written;

#endif

#ifdef linux

	result.success = false;
	result.bytes_written = 0;
	std::cout << "[OS Error] Pipes in linux based systems are not supported" << std::endl;
	ASSERT(false);

#endif

	if (_event_trigger_flag && IOEventTriggerCondition::WRITE) _trigger_io_event();
	return result;
}

Pipe::io_op_result Pipe::write(const std::string& source_data, const char* endline_character)
{
	return write(source_data.c_str(), source_data.size() * sizeof(char), 0, endline_character);
}

Pipe::io_op_result Pipe::write(const std::string& source_data, size_t string_size_in_bytes, size_t offset_in_byte, const char* endline_character)
{
	return write(source_data.c_str(), string_size_in_bytes, offset_in_byte, endline_character);
}

Pipe::io_op_result Pipe::write(const std::wstring& source_data, const char* endline_character)
{
	return write(source_data.c_str(), source_data.size() * sizeof(wchar_t), 0, endline_character);
}

Pipe::io_op_result Pipe::write(const std::wstring& source_data, size_t string_size_in_bytes, size_t offset_in_byte, const char* endline_character)
{
	return write(source_data.c_str(), source_data.size() * sizeof(wchar_t), 0, endline_character);
}

Pipe::io_op_result Pipe::read(void* target_buffer, size_t buffer_size_in_byte, size_t offset_in_byte, const char* endline_character)
{
	if (!_is_read_open) {
		std::cout << "[OS Error] Pipe::read() is called but read handle is closed" << std::endl;
		ASSERT(false);
	}

	if (buffer_size_in_byte == 0) return io_op_result(true, 0, 0);

	io_op_result result;

#ifdef windows

	DWORD bytes_read;
	result.success = ReadFile(_handle_read, (unsigned char*)target_buffer + offset_in_byte, buffer_size_in_byte - offset_in_byte, &bytes_read, nullptr);
	result.io_bytes = bytes_read;

#endif

#ifdef linux

	result.success = false;
	result.bytes_read = 0;
	std::cout << "[OS Error] Pipes in linux based systems are not supported" << std::endl;
	ASSERT(false);

#endif

	if (_event_trigger_flag && IOEventTriggerCondition::READ) _trigger_io_event();
	return result;
}

Pipe::io_op_result Pipe::read(void** target_buffer, size_t* buffer_size, const char* endline_character)
{
	if (*target_buffer != nullptr) {
		std::cout << "[OS Error] Pipe::read() is called but void** target_buffer must point to nullptr" << std::endl;
		ASSERT(false);
	}
	
	io_op_result result;

	size_t size = get_size();
	
	if (size == 0) {
		result.success = true;
		result.io_bytes = 0;
		result.total_bytes = size;
		return result;
	}

	*target_buffer = new char[get_size()];
	io_op_result read_result = read(*target_buffer, size, 0, endline_character);
	if (buffer_size != nullptr)
		*buffer_size = size;

	return read_result;
}

Pipe::io_op_result Pipe::read(std::string& target_data, const char* endline_character)
{
	target_data.resize(get_size() / sizeof(char), 0);
	return read(&(*target_data.begin()), target_data.size() * sizeof(char), 0, endline_character);
}

Pipe::io_op_result Pipe::read(std::string& target_data, size_t string_size_in_bytes, size_t offset_in_byte, const char* endline_character)
{
	target_data.resize(string_size_in_bytes, 0);
	return read(&(*target_data.begin()), string_size_in_bytes, offset_in_byte, endline_character);
}

Pipe::io_op_result Pipe::read(std::wstring& target_data, const char* endline_character)
{
	target_data.resize(get_size() / sizeof(wchar_t), 0);
	return read(&(*target_data.begin()), target_data.size() * sizeof(wchar_t), 0, endline_character);
}

Pipe::io_op_result Pipe::read(std::wstring& target_data, size_t string_size_in_bytes, size_t offset_in_byte, const char* endline_character)
{
	target_data.resize(string_size_in_bytes, 0);
	return read(&(*target_data.begin()), string_size_in_bytes, offset_in_byte, endline_character);
}

Pipe::io_op_result Pipe::peek(void* target_buffer, size_t buffer_size_in_byte, size_t offset_in_byte, const char* endline_character)
{
	if (!_is_read_open) {
		std::cout << "[OS Error] Pipe::peek() is called but read handle is closed" << std::endl;
		ASSERT(false);
	}

	io_op_result result;

#ifdef windows

	DWORD bytes_read;
	DWORD total_bytes;
	DWORD bytes_left_in_message;

	result.success = PeekNamedPipe(_handle_read, (unsigned char*)target_buffer + offset_in_byte, buffer_size_in_byte - offset_in_byte, &bytes_read, &total_bytes, &bytes_left_in_message);

	result.io_bytes = bytes_read;
	result.total_bytes = total_bytes;
	result.is_total_bytes_learned = true;

#endif

#ifdef linux

	result.success = false;
	result.bytes_read = 0;
	std::cout << "[OS Error] Pipes in linux based systems are not supported" << std::endl;
	ASSERT(false);

#endif

	if (_event_trigger_flag && IOEventTriggerCondition::PEEK) _trigger_io_event();
	return result;
}

Pipe:: io_op_result Pipe::peek(void** target_buffer, size_t* buffer_size, const char* endline_character)
{
	if (*target_buffer != nullptr) {
		std::cout << "[OS Error] Pipe::peek() is called but void** target_buffer must point to nullptr" << std::endl;
		ASSERT(false);
	}

	io_op_result result;

	size_t size = get_size();

	if (size == 0) {
		result.success = true;
		result.io_bytes = 0;
		result.total_bytes = size;
		return result;
	}

	*target_buffer = new char[get_size()];
	io_op_result read_result = peek(*target_buffer, size, 0, endline_character);
	if (buffer_size != nullptr)
		*buffer_size = size;

	return read_result;
}

Pipe::io_op_result Pipe::peek(std::string& target_data, const char* endline_character)
{
	return peek(&(target_data.at(0)), target_data.size() * sizeof(char), 0, endline_character);
}

Pipe::io_op_result Pipe::peek(std::string& target_data, size_t string_size_in_bytes, size_t offset_in_byte, const char* endline_character)
{
	return peek(&(target_data.at(0)), string_size_in_bytes, offset_in_byte, endline_character);
}

Pipe::io_op_result Pipe::peek(std::wstring& target_data, const char* endline_character)
{
	return peek(&(target_data.at(0)), target_data.size() * sizeof(wchar_t), 0, endline_character);
}

Pipe::io_op_result Pipe::peek(std::wstring& target_data, size_t string_size_in_bytes, size_t offset_in_byte, const char* endline_character)
{
	return peek(&(target_data.at(0)), string_size_in_bytes, offset_in_byte, endline_character);
}

Pipe::io_op_result Pipe::update(const void* source_buffer, size_t buffer_size_in_byte, size_t offset_in_byte, const char* endline_character)
{
	io_op_result result;
	
	io_op_result clear_result = clear();
	io_op_result write_result = write(source_buffer, buffer_size_in_byte, offset_in_byte, endline_character);
	
	result.success = clear_result.success && write_result.success;
	result.io_bytes = clear_result.io_bytes + write_result.io_bytes;
	result.total_bytes = write_result.io_bytes;
	result.is_total_bytes_learned = true;
	
	return result;
}

Pipe::io_op_result Pipe::update(const std::string& source_data, const char* endline_character)
{
	return update(source_data.c_str(), source_data.size() * sizeof(char), 0, endline_character);
}

Pipe::io_op_result Pipe::update(const std::string& source_data, size_t string_size_in_bytes, size_t offset_in_byte, const char* endline_character)
{
	return update(source_data.c_str(), string_size_in_bytes, offset_in_byte, endline_character);
}

Pipe::io_op_result Pipe::update(const std::wstring& source_data, const char* endline_character)
{
	return update(source_data.c_str(), source_data.size() * sizeof(wchar_t), 0, endline_character);
}

Pipe::io_op_result Pipe::update(const std::wstring& source_data, size_t string_size_in_bytes, size_t offset_in_byte, const char* endline_character)
{
	return update(source_data.c_str(), string_size_in_bytes, offset_in_byte, endline_character);
}

std::ostream& operator<<(std::ostream& o, Pipe& pipe)
{
	o << "r:" << pipe.get_read_handle_int() << " w:" << pipe.get_write_handle_int();
	return o;
}

std::wostream& operator<<(std::wostream& o, Pipe& pipe)
{
	o << L"r:" << pipe.get_read_handle_int() << L" w:" << pipe.get_write_handle_int();
	return o;
}

#undef windows
#undef linux
