global using event_handle_t = System.IntPtr;
global using event_t = System.UIntPtr;

using System.Runtime.InteropServices;

namespace ipclibcs
{
    public class Event
    {
        public enum reset_type
        {
            manual = 0,
            auto_reset = 1,
        };

        public enum wait_op_return
        {
            FAILED = 0,
            SIGNALED = 1,
            TIMEOUT = 2,
            ABANDONED = 3,
        };

        #region C API
        [DllImport("ipclib.dll")]
        private static extern event_t event_create();
        [DllImport("ipclib.dll")]
        private static extern event_t event_create_r(reset_type type);
        [DllImport("ipclib.dll")]
        private static extern event_t event_create_h(event_handle_t handle);
        [DllImport("ipclib.dll")]
        private static extern event_t event_create_s(size_t handle);

        [DllImport("ipclib.dll")]
        private static extern void event_destroy(event_t event_variable);

        [DllImport("ipclib.dll")]
        private static extern void event_set_ownership(event_t event_variable, bool is_event_handle_owned);
        [DllImport("ipclib.dll")]
        private static extern bool event_close(event_t event_variable);

        [DllImport("ipclib.dll")]
        private static extern bool event_set(event_t event_variable);
        [DllImport("ipclib.dll")]
        private static extern bool event_reset(event_t event_variable);

        [DllImport("ipclib.dll")]
        private static extern wait_op_return event_wait(event_t event_variable);
        [DllImport("ipclib.dll")]
        private static extern wait_op_return event_wait_timeout(event_t event_variable, int milliseconds);

        [DllImport("ipclib.dll")]
        private static extern void event_clear_listeners(event_t event_variable);

        [DllImport("ipclib.dll")]
        private static extern event_handle_t event_get_handle(event_t event_variable);
        [DllImport("ipclib.dll")]
        private static extern event_handle_t event_get_handle_duplicate(event_t event_variable, process_handle_t target_process);

        [DllImport("ipclib.dll")]
        private static extern size_t event_get_handle_int(event_t event_variable);
        [DllImport("ipclib.dll")]
        private static extern size_t event_get_handle_duplicate_int(event_t event_variable, process_handle_t target_process);
        #endregion

        public event_t event_ptr = (event_t)0;

        //Event(Event& other) = delete;
        public Event(event_t event_ptr)
        {
            this.event_ptr = event_ptr;
        }
        public Event()
        {
            event_ptr = event_create();
        }
        public Event(reset_type type)
        {
            event_ptr = event_create_r(type);
        }
        public Event(event_handle_t handle)
        {
            event_ptr = event_create_h(handle);
        }

        public Event(size_t handle)
        {
            event_ptr = event_create_s(handle);
        }

        ~Event()
        {
            event_destroy(event_ptr);
            event_ptr = (event_t)0;
        }

        public void set_ownership(bool is_event_handle_owned)
        {
            event_set_ownership(event_ptr, is_event_handle_owned);
        }

        public bool close()
        {
            return event_close(event_ptr);
        }

        public bool set()
        {
            return event_set(event_ptr);
        }

        public bool reset()
        {
            return event_reset(event_ptr);
        }

        public wait_op_return wait()
        {
            return event_wait(event_ptr);
        }
        public wait_op_return wait_timeout(int milliseconds)
        {
            return event_wait_timeout(event_ptr, milliseconds);
        }

        //void add_listener(std::function<void()> lambda);
        //void clear_listeners();

        public event_handle_t get_handle(){
            return event_get_handle(event_ptr);
        }

        public event_handle_t get_handle_duplicate(process_handle_t target_process)
        {
            return event_get_handle_duplicate(event_ptr, target_process);
        }

        public size_t get_handle_int()
        {
            return event_get_handle_int(event_ptr);
        }

        public size_t get_handle_duplicate_int(process_handle_t target_process)
        {
            return event_get_handle_duplicate_int(event_ptr, target_process);
        }

        public override string ToString()
        {
            return get_handle_int().ToString();
        }
    }
}