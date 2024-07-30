global using pipe_handle_t = System.IntPtr;
global using pipe_t = System.UIntPtr;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace ipclibcs
{
    class Pipe
    {
        public struct io_op_result
        {
            public io_op_result() { }

            public bool success = false;
            public size_t io_bytes = 0;
            public size_t total_bytes = 0;
            public bool is_total_bytes_learned = false;
        };

        #region C API
        [DllImport("ipclib.dll")]
        private static extern pipe_t pipe_create();
        [DllImport("ipclib.dll")]
        private static extern pipe_t pipe_create_hh(pipe_handle_t write_handle, pipe_handle_t read_handle);
        [DllImport("ipclib.dll")]
        private static extern pipe_t pipe_create_hhh(pipe_handle_t write_handle, pipe_handle_t read_handle, event_handle_t external_io_event);
        [DllImport("ipclib.dll")]
        private static extern pipe_t pipe_create_hhs(pipe_handle_t write_handle, pipe_handle_t read_handle, size_t external_io_event_int);
        [DllImport("ipclib.dll")]
        private static extern pipe_t pipe_create_ssh(size_t write_handle_int, size_t read_handle_int, event_handle_t external_io_event);
        [DllImport("ipclib.dll")]
        private static extern pipe_t pipe_create_sss(size_t write_handle_int, size_t read_handle_int, size_t external_io_event_int);

        [DllImport("ipclib.dll")]
        private static extern void pipe_destroy(pipe_t pipe);

        [DllImport("ipclib.dll")]
        private static extern void pipe_set_ownership(pipe_t pipe, bool is_read_handle_owned, bool is_write_handle_owned);
        [DllImport("ipclib.dll")]
        private static extern bool pipe_close(pipe_t pipe);
        [DllImport("ipclib.dll")]
        private static extern bool pipe_close_read(pipe_t pipe);
        [DllImport("ipclib.dll")]
        private static extern bool pipe_close_write(pipe_t pipe);

        [DllImport("ipclib.dll")]
        private static extern io_op_result pipe_clear(pipe_t pipe);
        [DllImport("ipclib.dll")]
        private static extern bool pipe_is_empty(pipe_t pipe);
        [DllImport("ipclib.dll")]
        private static extern size_t pipe_get_size(pipe_t pipe);

        [DllImport("ipclib.dll")]
        private static extern pipe_handle_t pipe_get_read_handle(pipe_t pipe);
        [DllImport("ipclib.dll")]
        private static extern pipe_handle_t pipe_get_write_handle(pipe_t pipe);
        [DllImport("ipclib.dll")]
        private static extern size_t pipe_get_read_handle_int(pipe_t pipe);
        [DllImport("ipclib.dll")]
        private static extern size_t pipe_get_write_handle_int(pipe_t pipe);

        //const Pipe::handle_t ipc_api pipe_get_read_handle_duplicate(void* pipe);
        //const Pipe::handle_t ipc_api pipe_get_write_handle_duplicate(void* pipe);
        //const size_t ipc_api pipe_get_read_handle_duplicate_int(void* pipe);
        //const size_t ipc_api pipe_get_write_handle_duplicate_int(void* pipe);

        [DllImport("ipclib.dll")]
        private static extern io_op_result pipe_write(pipe_t pipe, byte[] source_buffer, size_t buffer_size_in_byte, size_t offset_in_byte, string endline_character);
        [DllImport("ipclib.dll")]
        private static extern io_op_result pipe_read(pipe_t pipe, byte[] target_buffer, size_t buffer_size_in_byte, size_t offset_in_byte, string endline_character);
        [DllImport("ipclib.dll")]
        private static extern io_op_result pipe_peek(pipe_t pipe, byte[] target_buffer, size_t buffer_size_in_byte, size_t offset_in_byte, string endline_character);
        [DllImport("ipclib.dll")]
        private static extern io_op_result pipe_update(pipe_t pipe, byte[] source_buffer, size_t buffer_size_in_byte, size_t offset_in_byte, string endline_character);

        //Pipe::io_op_result ipc_api pipe_read_and_parse(std::vector<std::string>& out_vector, char* endline_character = "|");

        [DllImport("ipclib.dll")]
        private static extern event_t pipe_get_io_event(pipe_t pipe);
        [DllImport("ipclib.dll")]
        private static extern void pipe_set_io_event(pipe_t pipe, event_t external_event);
        [DllImport("ipclib.dll")]
        private static extern void pipe_set_event_trigger_condition(pipe_t pipe, int condition_flags);
        #endregion

        private pipe_t pipe_ptr = (pipe_t)0;

        public Pipe(pipe_t pipe_ptr)
        {
            this.pipe_ptr = pipe_ptr;
        }

        public Pipe()
        {
            pipe_ptr = pipe_create();
        }
        public Pipe(pipe_handle_t write_handle, pipe_handle_t read_handle)
        {
            pipe_ptr = pipe_create_hh(write_handle, read_handle);
        }
        public Pipe(pipe_handle_t write_handle, pipe_handle_t read_handle, event_handle_t external_io_event)
        {
            pipe_ptr = pipe_create_hhh(write_handle, read_handle, external_io_event);
        }
        public Pipe(pipe_handle_t write_handle, pipe_handle_t read_handle, size_t external_io_event_int)
        {
            pipe_ptr = pipe_create_hhs(write_handle, read_handle, external_io_event_int);
        }
        public Pipe(size_t write_handle_int, size_t read_handle_int, event_handle_t external_io_event)
        {
            pipe_ptr = pipe_create_ssh(write_handle_int, read_handle_int, external_io_event);
        }
        public Pipe(size_t write_handle_int, size_t read_handle_int, size_t external_io_event_int)
        {
            pipe_ptr = pipe_create_sss(write_handle_int, read_handle_int, external_io_event_int);
        }

        ~Pipe()
        {
            pipe_destroy(pipe_ptr);
            pipe_ptr = (pipe_t)0;
        }

        public void set_ownership(bool is_read_handle_owned, bool is_write_handle_owned)
        {
            pipe_set_ownership(pipe_ptr, is_read_handle_owned, is_write_handle_owned);
        }
        public bool close()
        {
            return pipe_close(pipe_ptr);
        }
        public bool close_read()
        {
            return pipe_close_read(pipe_ptr);
        }
        public bool close_write()
        {
            return pipe_close_write(pipe_ptr);
        }

        public io_op_result clear()
        {
            return pipe_clear(pipe_ptr);
        }
        public bool is_empty()
        {
            return pipe_is_empty(pipe_ptr);
        }
        public size_t get_size()
        {
            return pipe_get_size(pipe_ptr);
        }

        public pipe_handle_t get_read_handle()
        {
            return pipe_get_read_handle(pipe_ptr);
        }
        public pipe_handle_t get_write_handle()
        {
            return pipe_get_write_handle(pipe_ptr);

        }
        public size_t get_read_handle_int()
        {
            return pipe_get_read_handle_int(pipe_ptr);
        }
        public size_t get_write_handle_int()
        {
            return pipe_get_write_handle_int(pipe_ptr);
        }

        public io_op_result write<T>(T[] source_buffer, size_t buffer_size_in_byte, size_t offset_in_byte = 0, string endline_character = "")
        {
            byte[] source_buffer_byte = Unsafe.As<byte[]>(source_buffer);
            return pipe_write(pipe_ptr, source_buffer_byte, buffer_size_in_byte, offset_in_byte, endline_character);
        }
        public io_op_result read<T>(T[] target_buffer, size_t buffer_size_in_byte, size_t offset_in_byte = 0, string endline_character = "")
        {
            byte[] target_buffer_byte = Unsafe.As<byte[]>(target_buffer);
            return pipe_read(pipe_ptr, target_buffer_byte, buffer_size_in_byte, offset_in_byte, endline_character);
        }
        public io_op_result peek<T>(T[] target_buffer, size_t buffer_size_in_byte, size_t offset_in_byte = 0, string endline_character = "")
        {
            byte[] target_buffer_byte = Unsafe.As<byte[]>(target_buffer);
            return pipe_peek(pipe_ptr, target_buffer_byte, buffer_size_in_byte, offset_in_byte, endline_character);
        }
        public io_op_result update<T>(T[] source_buffer, size_t buffer_size_in_byte, size_t offset_in_byte = 0, string endline_character = "")
        {
            byte[] source_buffer_byte = Unsafe.As<byte[]>(source_buffer);
            return pipe_update(pipe_ptr, source_buffer_byte, buffer_size_in_byte, offset_in_byte, endline_character);
        }

        // event system
        public enum IOEventTriggerCondition
        {
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

        public Event get_io_event()
        {
            return new Event(pipe_get_io_event(pipe_ptr));
        }
        public void set_io_event(Event external_event)
        {
            pipe_set_io_event(pipe_ptr, external_event.event_ptr);
        }
        public void set_event_trigger_condition(IOEventTriggerCondition condition_flags)
        {
            pipe_set_event_trigger_condition(pipe_ptr, (int)condition_flags);
        }

        public override string ToString()
        {
            return "r: " + get_read_handle_int().ToString() + "w: " + get_write_handle_int().ToString();
        }
    }
}
