#include "PipeInterface.h"

intptr_t ipc_api pipe_create()
{
    return (intptr_t) new Pipe();
}

intptr_t ipc_api pipe_create_hh(Pipe::handle_t write_handle, Pipe::handle_t read_handle)
{
    return (intptr_t) new Pipe(write_handle, read_handle);
}

intptr_t ipc_api pipe_create_hhh(Pipe::handle_t write_handle, Pipe::handle_t read_handle, Event::handle_t external_io_event)
{
    return (intptr_t) new Pipe(write_handle, read_handle, external_io_event);
}

intptr_t ipc_api pipe_create_hhs(Pipe::handle_t write_handle, Pipe::handle_t read_handle, size_t external_io_event_int)
{
    return (intptr_t) new Pipe(write_handle, read_handle, external_io_event_int);
}

intptr_t ipc_api pipe_create_ssh(size_t write_handle_int, size_t read_handle_int, Event::handle_t external_io_event)
{
    return (intptr_t) new Pipe(write_handle_int, read_handle_int, external_io_event);
}

intptr_t ipc_api pipe_create_sss(size_t write_handle_int, size_t read_handle_int, size_t external_io_event_int)
{
    return (intptr_t) new Pipe(write_handle_int, read_handle_int, external_io_event_int);
}

void ipc_api pipe_destroy(void* pipe)
{
    delete pipe;
}

void ipc_api pipe_set_ownership(void* pipe, bool is_read_handle_owned, bool is_write_handle_owned)
{
    ((Pipe*)pipe)->set_ownership(is_read_handle_owned, is_write_handle_owned);
}

bool ipc_api pipe_close(void* pipe)
{
    return ((Pipe*)pipe)->close();
}

bool ipc_api pipe_close_read(void* pipe)
{
    return ((Pipe*)pipe)->close_read();
}

bool ipc_api pipe_close_write(void* pipe)
{
    return ((Pipe*)pipe)->close_write();
}

pipe_io_op_result ipc_api pipe_clear(void* pipe)
{
    Pipe::io_op_result r = ((Pipe*)pipe)->clear();
    return  *(pipe_io_op_result*)&r;
}

bool ipc_api pipe_is_empty(void* pipe)
{
    return ((Pipe*)pipe)->is_empty();
}

size_t ipc_api pipe_get_size(void* pipe)
{
    return ((Pipe*)pipe)->get_size();
}

Pipe::handle_t ipc_api pipe_get_read_handle(void* pipe)
{
    return ((Pipe*)pipe)->get_read_handle();
}

Pipe::handle_t ipc_api pipe_get_write_handle(void* pipe)
{
    return ((Pipe*)pipe)->get_write_handle();
}

size_t ipc_api pipe_get_read_handle_int(void* pipe)
{
    return ((Pipe*)pipe)->get_read_handle_int();
}

size_t ipc_api pipe_get_write_handle_int(void* pipe)
{
    return ((Pipe*)pipe)->get_write_handle_int();
}


//const Pipe::handle_t ipc_api pipe_get_read_handle_duplicate(void* pipe)
//{
//    return ((Pipe*)pipe)->get_read_handle_duplicate();
//}
//
//const Pipe::handle_t ipc_api pipe_get_write_handle_duplicate(void* pipe)
//{
//    return ((Pipe*)pipe)->get_write_handle_duplicate();
//}
//
//const size_t ipc_api pipe_get_read_handle_duplicate_int(void* pipe)
//{
//    return ((Pipe*)pipe)->get_read_handle_duplicate_int();
//}
//
//const size_t ipc_api pipe_get_write_handle_duplicate_int(void* pipe)
//{
//    return ((Pipe*)pipe)->get_write_handle_duplicate_int();
//}

pipe_io_op_result ipc_api pipe_write(void* pipe, const void* source_buffer, size_t buffer_size_in_byte, size_t offset_in_byte, const char* endline_character)
{
    Pipe::io_op_result r = ((Pipe*)pipe)->write(source_buffer, buffer_size_in_byte, offset_in_byte, endline_character);
    return *(pipe_io_op_result*)&r;
}

pipe_io_op_result ipc_api pipe_read(void* pipe, void* target_buffer, size_t buffer_size_in_byte, size_t offset_in_byte, const char* endline_character)
{
    Pipe::io_op_result r = ((Pipe*)pipe)->read(target_buffer, buffer_size_in_byte, offset_in_byte, endline_character);
    return *(pipe_io_op_result*)&r;
}

pipe_io_op_result ipc_api pipe_peek(void* pipe, void* target_buffer, size_t buffer_size_in_byte, size_t offset_in_byte, const char* endline_character)
{
    Pipe::io_op_result r = ((Pipe*)pipe)->read(target_buffer, buffer_size_in_byte, offset_in_byte, endline_character);
    return *(pipe_io_op_result*)&r;
}

pipe_io_op_result ipc_api pipe_update(void* pipe, const void* source_buffer, size_t buffer_size_in_byte, size_t offset_in_byte, const char* endline_character)
{
    Pipe::io_op_result r = ((Pipe*)pipe)->update(source_buffer, buffer_size_in_byte, offset_in_byte, endline_character);
    return *(pipe_io_op_result*)&r;
}

intptr_t ipc_api pipe_get_io_event(void* pipe)
{
    return (intptr_t)((Pipe*)pipe)->get_io_event().get();
}

void ipc_api pipe_set_io_event(void* pipe, void* external_event)
{
    ((Pipe*)pipe)->set_io_event(std::shared_ptr<Event>((Event*)external_event));
}

void ipc_api pipe_set_event_trigger_condition(void* pipe, int condition_flags)
{
    ((Pipe*)pipe)->set_event_trigger_condition(condition_flags);
}
