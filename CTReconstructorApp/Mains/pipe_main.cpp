#include "Interprocess/Pipe.h"
#include "Interprocess/Event.h"

#include <sstream>

int mainR() {
	
	HANDLE pipe_read, pipe_write;
	CreatePipe(&pipe_read, &pipe_write, nullptr, 0);
	const char* message1 = "1";
	const char* message2 = "\n";
	const char* message3 = "3";
	unsigned long bytes_written;

	WriteFile(pipe_write, message1, sizeof(char) * strlen(message1), &bytes_written, nullptr);
	WriteFile(pipe_write, message2, sizeof(char) * strlen(message2), &bytes_written, nullptr);
	WriteFile(pipe_write, message3, sizeof(char) * strlen(message3), &bytes_written, nullptr);
	
	//CloseHandle(pipe_read);
	CloseHandle(pipe_write);

	int read_buffer_size = 256;
	char* read_message = (char*)malloc(sizeof(char) * read_buffer_size);
	unsigned long bytes_read;
	
	if(!ReadFile(pipe_read, read_message, read_buffer_size, &bytes_read, nullptr)) 
		std::cout << "pipe closed\n";

	memset(read_message + bytes_read, 0, read_buffer_size - bytes_read);

	std::cout << read_message << std::endl;
	std::cin.get();

	return 0;
}

int mainP() {
	Pipe pipe;
	
	char message1[] = "Hello";
	char message2[] = "There";

	std::string a = "Hello_there";
	std::vector<char> b = {'a', 'b', 'c'};
	std::array<char, 4> c = {'x', 'y', 'z', 'w'};

	pipe.write(a, a.size() * sizeof(char), 4, "");
	pipe.write(b);
	pipe.write(c);

	pipe.write(message1, sizeof(char) * strlen(message1), 0, "");
	pipe.write(message2, sizeof(char) * strlen(message2), 0, "");

	std::cout << pipe.get_size() << std::endl;

	//std::string message_read;
	char buffer[512];
	pipe.read((void*)buffer, 512, 0, "");

	std::cout << buffer;
	std::cin.get();
}

int mainE() {
	Event event;

	event.add_listener([]() {
		std::cout << "listener triggered" << std::endl;
		});

	event.set();

	std::cin.get();
	return 0;
}

void main_parent() {

	Event event;
	Pipe pipe;

	std::wstringstream ss_command;
	ss_command << "../build/windows-Release-x86_64CTReconstructor/CTReconstructor.exe " << event.get_handle_int() << " " << pipe.get_write_handle_int();
	std::wstring command = ss_command.str();

	std::cout << "Parent:  Starting child process..." << std::endl;

	STARTUPINFO start_info = { sizeof(start_info) };
	PROCESS_INFORMATION proc_info = { 0 };
	::CreateProcessW(L"CTReconstructor.exe", &command[0], nullptr, nullptr, true, CREATE_NEW_CONSOLE, nullptr, nullptr, &start_info, &proc_info);
	::CloseHandle(proc_info.hThread);
	::CloseHandle(proc_info.hProcess);

	std::cout << "Parent:  Waiting for the child to signal the event." << std::endl;
	
	event.add_listener([&event, &pipe]() {
		std::string message;
		pipe.read(message);
		std::cout << "Parent: event is triggered, pipe has the value: \"" << message << "\"" << std::endl;
		});

	std::cin.get();
}

void main_child(char* handle_string[]) {
	std::stringstream eventss(handle_string[1]);
	size_t event_int;
	eventss >> event_int;

	std::stringstream pipess(handle_string[2]);
	size_t pipe_write_int;
	pipess >> pipe_write_int;

	Pipe pipe(pipe_write_int, 0, event_int);
	pipe.set_event_trigger_condition(Pipe::IOEventTriggerCondition::W);

	std::cout << "Child:  Event handle " <<  pipe.get_io_event()->get_handle_int() << " " << pipe.get_write_handle_int() << std::endl;
	::Sleep(2000);

	
	std::cout << "Child: Writing to pipe" << std::endl;
	pipe.write("hello");

	std::cout << "Child: Writing to pipe" << std::endl;
	pipe.write(" ");
	
	std::cout << "Child: Writing to pipe" << std::endl;
	pipe.write("parent");

	std::cout << "Child: Writing to pipe" << std::endl;
	pipe.write("How are you?");
	::Sleep(1);
	std::cout << "Child: Writing to pipe" << std::endl;
	pipe.write("I Hope everyting is alright.");
	::Sleep(1);
	std::cout << "Child: Writing to pipe" << std::endl;
	pipe.write("I feel great too, thanks!");

	std::cin.get();
}

int main(int argc, char* argv[]) {
	if (argc > 1) main_child(argv);
	else main_parent();
}