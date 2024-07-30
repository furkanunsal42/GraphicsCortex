#include "Event.h"
#include "Pipe.h"

#include <string>
#include <iostream>
#include <sstream>

void main_parent() {
	std::cout << "I'm C++" << std::endl;

	Pipe pipe_read;
	Pipe pipe_write;
	Event event_external;

	std::wstringstream ss;
	ss << L"IPClibcs.exe " <<
		pipe_write.get_io_event()->get_handle_int() << " " <<
		event_external.get_handle_int() << " " <<
		pipe_write.get_read_handle_int() << " " <<
		pipe_read.get_write_handle_int();

	std::wstring command = ss.str();

	pipe_write.write(L"Hello");

	STARTUPINFO start_info = { sizeof(start_info) };
	PROCESS_INFORMATION proc_info = { 0 };
	//::CreateProcessW(L"../build/windows-Release-x86_64CTReconstructor/CTReconstructor.exe", &command[0], nullptr, nullptr, true, CREATE_NEW_CONSOLE, nullptr, nullptr, &start_info, &proc_info);
	::CreateProcessW(L"IPClibcs.exe", &command[0], nullptr, nullptr, true, CREATE_NEW_CONSOLE, nullptr, nullptr, &start_info, &proc_info);
	::CloseHandle(proc_info.hThread);
	::CloseHandle(proc_info.hProcess);

	std::wcout << L"Parent: my rp:" << pipe_read << L" wp:" << pipe_write << " event:" << *pipe_write.get_io_event() << " event_external:" << event_external << std::endl;
	pipe_read.close_write();
	pipe_write.close_read();
	pipe_write.set_event_trigger_condition(Pipe::W);


	event_external.add_listener([&pipe_read, &pipe_write]() {
		
		std::wstring message;
		pipe_read.read(message);

		std::wcout << L"Parent: event is triggered, child sent: " << message << std::endl;

		if (message == L"Hello There") {
			::Sleep(1000);
			pipe_write.write(L"Nice to see you Boy!");
		}

		if (message == L"Its nice to talk to you too, Father!") {
			::Sleep(1000);
			pipe_write.write(L"Come join me Boy! We can rule the Universe as Father and Son!");
		}
		});

	std::cin.get();
}

void main_child(char* argv[]) {
	std::wcout << L"I'm C++" << std::endl;

	std::wstringstream ss;
	ss << argv[1] << " " << argv[2] << " " << argv[3] << " " << argv[4];
	
	size_t event_external_h;
	size_t event_h;
	size_t pipe_read_h;
	size_t pipe_write_h;

	ss >> event_external_h;
	ss >> event_h;
	ss >> pipe_read_h;
	ss >> pipe_write_h;

	Pipe pipe_read((size_t)0, pipe_read_h, nullptr);
	Pipe pipe_write(pipe_write_h, (size_t)0, event_h);
	Event event_external(event_external_h);

	pipe_write.set_event_trigger_condition(Pipe::WRITE);
	pipe_write.write(L"Hello");

	std::wcout << L"Child: my rp:" << pipe_read << L" wp:" << pipe_write << L" event:" << *pipe_write.get_io_event() << L" event_external:" << event_external << std::endl;

	event_external.add_listener([&pipe_read, &pipe_write]() {
		std::wstring message;
		pipe_read.read(message);

		std::wcout << L"Child: event is triggered, parent sent: " << message << std::endl;

		if (message == L"Nice to see you Boy!") {
			::Sleep(1000);
			pipe_write.write(L"Its nice to talk to you too, Father!");
		}

		if (message == L"Come join me Boy! We can rule the Universe as Father and Son!") {
			::Sleep(1000);
			pipe_write.write(L"No Father! You were supposed to bring balance to the Force not destroy it!");
		}
		});

	pipe_write.write(L"Hello There");

	std::cin.get();
}

int main(int argc, char* argv[]) {
	if (argc > 1) main_child(argv);
	else main_parent();

	std::cout << "Success" << std::endl;
	std::cin.get();
}