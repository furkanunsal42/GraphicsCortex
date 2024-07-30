#pragma once

#include "Pipe.h"

#define ipc_api __declspec(dllexport)

struct pipe_io_op_result {
	bool success = false;
	size_t io_bytes = 0;
	size_t total_bytes = 0;
	bool is_total_bytes_learned = false;
};

extern "C" {

	intptr_t ipc_api pipe_create();
	intptr_t ipc_api pipe_create_hh(Pipe::handle_t write_handle, Pipe::handle_t read_handle);
	intptr_t ipc_api pipe_create_hhh(Pipe::handle_t write_handle, Pipe::handle_t read_handle, Event::handle_t external_io_event);
	intptr_t ipc_api pipe_create_hhs(Pipe::handle_t write_handle, Pipe::handle_t read_handle, size_t external_io_event_int);
	intptr_t ipc_api pipe_create_ssh(size_t write_handle_int, size_t read_handle_int, Event::handle_t external_io_event);
	intptr_t ipc_api pipe_create_sss(size_t write_handle_int, size_t read_handle_int, size_t external_io_event_int);

	void ipc_api pipe_destroy(void* pipe);

	void ipc_api pipe_set_ownership(void* pipe, bool is_read_handle_owned, bool is_write_handle_owned);
	bool ipc_api pipe_close(void* pipe);
	bool ipc_api pipe_close_read(void* pipe);
	bool ipc_api pipe_close_write(void* pipe);

	pipe_io_op_result ipc_api pipe_clear(void* pipe);
	bool ipc_api pipe_is_empty(void* pipe);
	size_t ipc_api pipe_get_size(void* pipe);

	Pipe::handle_t ipc_api pipe_get_read_handle(void* pipe);
	Pipe::handle_t ipc_api pipe_get_write_handle(void* pipe);
	size_t ipc_api pipe_get_read_handle_int(void* pipe);
	size_t ipc_api pipe_get_write_handle_int(void* pipe);

	//const Pipe::handle_t ipc_api pipe_get_read_handle_duplicate(void* pipe);
	//const Pipe::handle_t ipc_api pipe_get_write_handle_duplicate(void* pipe);
	//const size_t ipc_api pipe_get_read_handle_duplicate_int(void* pipe);
	//const size_t ipc_api pipe_get_write_handle_duplicate_int(void* pipe);

	pipe_io_op_result ipc_api pipe_write(void* pipe, const void* source_buffer, size_t buffer_size_in_byte, size_t offset_in_byte, const char* endline_character);
	pipe_io_op_result ipc_api pipe_read(void* pipe, void* target_buffer, size_t buffer_size_in_byte, size_t offset_in_byte, const char* endline_character);
	pipe_io_op_result ipc_api pipe_peek(void* pipe, void* target_buffer, size_t buffer_size_in_byte, size_t offset_in_byte, const char* endline_character);
	pipe_io_op_result ipc_api pipe_update(void* pipe, const void* source_buffer, size_t buffer_size_in_byte, size_t offset_in_byte, const char* endline_character);

	//Pipe::io_op_result ipc_api pipe_read_and_parse(std::vector<std::string>& out_vector, char* endline_character = "|");

	intptr_t ipc_api pipe_get_io_event(void* pipe);
	void ipc_api pipe_set_io_event(void* pipe, void* external_event);
	void ipc_api pipe_set_event_trigger_condition(void* pipe, int condition_flags);

	//std::ostream& operator<<(std::ostream& o, Pipe& pipe);
}