#pragma once

#if defined(WIN32) || defined(_WINDOWS)
#define windows
#endif
#if defined(__linux__)
#define linux 
#endif

#ifdef windows
#define NOMINMAX
#include <Windows.h>
#endif

#ifdef linux
#include <unistd.h>
#endif

#include "ProcessHandle.h"

#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <inttypes.h>
#include <memory>

#include "Event.h"

class Pipe {
public:

#ifdef windows
	typedef HANDLE handle_t;
#endif
#ifdef linux
	typedef int32_t handle_t;
#endif
	
	struct io_op_result {
		io_op_result(bool result = false, size_t bytes_read_or_written = 0);
		io_op_result(bool result, size_t bytes_read_or_written, size_t bytes_available_in_pipe);

		bool success = false;
		size_t io_bytes = 0;
		size_t total_bytes = 0;
		bool is_total_bytes_learned = false;
	};

	Pipe(Pipe& other) = delete;
	Pipe(size_t buffer_size_recommendation = 0, std::shared_ptr<Event> external_io_event = nullptr);
	//Pipe(size_t buffer_size_recommendation, Event::handle_t external_io_event);
	//Pipe(size_t buffer_size_recommendation, size_t external_io_event_int);
	Pipe(Pipe::handle_t write_handle, Pipe::handle_t read_handle, std::shared_ptr<Event> external_io_event = nullptr);
	Pipe(Pipe::handle_t write_handle, Pipe::handle_t read_handle, Event::handle_t external_io_event);
	Pipe(Pipe::handle_t write_handle, Pipe::handle_t read_handle, size_t external_io_event_int);
	Pipe(size_t write_handle_int, size_t read_handle_int, std::shared_ptr<Event> external_io_event /* = nullptr */); // default value clashes with other constructors
	Pipe(size_t write_handle_int, size_t read_handle_int, Event::handle_t external_io_event);
	Pipe(size_t write_handle_int, size_t read_handle_int, size_t external_io_event_int);

	~Pipe();

	void set_ownership(bool is_read_handle_owned, bool is_write_handle_owned);
	bool close();
	bool close_read();
	bool close_write();

	io_op_result clear();
	bool is_empty();
	size_t get_size();

	const Pipe::handle_t get_read_handle();
	const Pipe::handle_t get_write_handle();
	const size_t get_read_handle_int();
	const size_t get_write_handle_int();

	const Pipe::handle_t get_read_handle_duplicate();
	const Pipe::handle_t get_write_handle_duplicate();
	const size_t get_read_handle_duplicate_int();
	const size_t get_write_handle_duplicate_int();

	io_op_result write(const void* source_buffer, size_t buffer_size_in_byte, size_t offset_in_byte = 0, const char* endline_character = "");
	io_op_result write(const std::string& source_data, const char* endline_character = "");
	io_op_result write(const std::string& source_data, size_t string_size_in_bytes, size_t offset_in_byte = 0, const char* endline_character = "");
	io_op_result write(const std::wstring& source_data, const char* endline_character = "");
	io_op_result write(const std::wstring& source_data, size_t string_size_in_bytes, size_t offset_in_byte = 0, const char* endline_character = "");
	template<typename T>
	io_op_result write(const std::vector<T>& source_vector, size_t offset_in_bytes = 0, const char* endline_character = "");
	template<typename T, int32_t N>
	io_op_result write(const std::array<T, N>& source_vector, size_t offset_in_bytes = 0, const char* endline_character = "");

	io_op_result read(void* target_buffer, size_t buffer_size_in_byte, size_t offset_in_byte = 0, const char* endline_character = "");
	io_op_result read(void** target_buffer, size_t* buffer_size = nullptr, const char* endline_character = "");
	io_op_result read(std::string& target_data, const char* endline_character = "");
	io_op_result read(std::string& target_data, size_t string_size_in_bytes, size_t offset_in_byte = 0, const char* endline_character = "");
	io_op_result read(std::wstring& target_data, const char* endline_character = "");
	io_op_result read(std::wstring& target_data, size_t string_size_in_bytes, size_t offset_in_byte = 0, const char* endline_character = "");
	template<typename T>
	io_op_result read(std::vector<T>& target_vector, size_t offset_in_bytes = 0, const char* endline_character = "");
	template<typename T, int32_t N>
	io_op_result read(std::array<T, N>& target_vector, size_t offset_in_bytes = 0, const char* endline_character = "");

	io_op_result peek(void* target_buffer, size_t buffer_size_in_byte, size_t offset_in_byte = 0, const char* endline_character = "");
	io_op_result peek(void** target_buffer, size_t* buffer_size = nullptr, const char* endline_character = "");
	io_op_result peek(std::string& target_data, const char* endline_character = "");
	io_op_result peek(std::string& target_data, size_t string_size_in_bytes, size_t offset_in_byte = 0, const char* endline_character = "");
	io_op_result peek(std::wstring& target_data, const char* endline_character = "");
	io_op_result peek(std::wstring& target_data, size_t string_size_in_bytes, size_t offset_in_byte = 0, const char* endline_character = "");
	template<typename T>
	io_op_result peek(std::vector<T>& target_vector, size_t offset_in_bytes = 0, const char* endline_character = "");
	template<typename T, int32_t N>
	io_op_result peek(std::array<T, N>& target_vector, size_t offset_in_bytes = 0, const char* endline_character = "");

	io_op_result update(const void* source_buffer, size_t buffer_size_in_byte, size_t offset_in_byte = 0, const char* endline_character = "");
	io_op_result update(const std::string& source_data, const char* endline_character = "");
	io_op_result update(const std::string& source_data, size_t string_size_in_bytes, size_t offset_in_byte = 0, const char* endline_character = "");
	io_op_result update(const std::wstring& source_data, const char* endline_character = "");
	io_op_result update(const std::wstring& source_data, size_t string_size_in_bytes, size_t offset_in_byte = 0, const char* endline_character = "");
	template<typename T>
	io_op_result update(const std::vector<T>& source_vector, size_t offset_in_bytes = 0, const char* endline_character = "");
	template<typename T, int32_t N>
	io_op_result update(const std::array<T, N>& source_vector, size_t offset_in_bytes = 0, const char* endline_character = "");

	io_op_result read_and_parse(std::vector<std::string>& out_vector, const char* endline_character = "|");

	// event system
	enum IOEventTriggerCondition {
		NEVER = 0,
		WRITE = 1,
		READ = 2,
		PEEK = 4,
		
		R = READ,
		W = WRITE,
		P = PEEK,
		RW = READ | WRITE,
		ALL = WRITE | READ | PEEK,
	};

	std::shared_ptr<Event> get_io_event();
	void set_io_event(std::shared_ptr<Event> external_event);
	void set_event_trigger_condition(int condition_flags);

private:

	Pipe::handle_t  _handle_read = 0;
	Pipe::handle_t  _handle_write = 0;

	bool _is_read_open = false;
	bool _is_write_open = false;
	bool _is_read_owned = false;
	bool _is_write_owned = false;

	int32_t _event_trigger_flag = IOEventTriggerCondition::NEVER;
	std::shared_ptr<Event> _io_event = nullptr;

	const bool _is_inheritable = true;

	bool _open(size_t buffer_size_recommendation = 0);
	bool _open_io_event();
	bool _trigger_io_event();
};

/// /////////////////////////////////////

template<typename T>
inline Pipe::io_op_result Pipe::write(const std::vector<T>& source_vector, size_t offset_in_bytes, const char* endline_character)
{
	return write((const char*)&(source_vector[0]), source_vector.size() * sizeof(T), offset_in_bytes, endline_character);
}

template<typename T, int32_t N>
inline Pipe::io_op_result Pipe::write(const std::array<T, N>& source_vector, size_t offset_in_bytes, const char* endline_character)
{
	return write((const char*)&(source_vector[0]), N * sizeof(T), offset_in_bytes, endline_character);
}

template<typename T>
inline Pipe::io_op_result Pipe::read(std::vector<T>& target_vector, size_t offset_in_bytes, const char* endline_character)
{
	return read((const char*)&(target_vector[0]), target_vector.size() * sizeof(T), offset_in_bytes, endline_character);
}

template<typename T, int32_t N>
inline Pipe::io_op_result Pipe::read(std::array<T, N>& target_vector, size_t offset_in_bytes, const char* endline_character)
{
	return read((const char*)&(target_vector[0]), N * sizeof(T), offset_in_bytes, endline_character);
}

template<typename T>
inline Pipe::io_op_result Pipe::peek(std::vector<T>& target_vector, size_t offset_in_bytes, const char* endline_character)
{
	return peek((const char*)&(target_vector[0]), target_vector.size() * sizeof(T), offset_in_bytes, endline_character);
}

template<typename T, int32_t N>
inline Pipe::io_op_result Pipe::peek(std::array<T, N>& target_vector, size_t offset_in_bytes, const char* endline_character)
{
	return peek((const char*)&(target_vector[0]), N * sizeof(T), offset_in_bytes, endline_character);
}

template<typename T>
inline Pipe::io_op_result Pipe::update(const std::vector<T>& source_vector, size_t offset_in_bytes, const char* endline_character)
{
	return update((const char*)&(source_vector[0]), source_vector.size() * sizeof(T), offset_in_bytes, endline_character);
}

template<typename T, int32_t N>
inline Pipe::io_op_result Pipe::update(const std::array<T, N>& source_vector, size_t offset_in_bytes, const char* endline_character)
{
	return update((const char*)&(source_vector[0]), N * sizeof(T), offset_in_bytes, endline_character);
}

std::ostream& operator<<(std::ostream& o, Pipe& pipe);
std::wostream& operator<<(std::wostream& o, Pipe& pipe);