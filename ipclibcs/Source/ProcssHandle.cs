global using intptr_t = System.IntPtr;
global using size_t = System.UInt64;

global using process_handle_t =  System.IntPtr;

using System.Runtime.InteropServices;

namespace ipclibcs
{
    class ProcessHandle
    {
        #region C API
        [DllImport("ipclib.dll")]
        private static extern process_handle_t process_get_current_process_handle();

        [DllImport("ipclib.dll")]
        private static extern size_t process_get_current_process_handle_int();

        [DllImport("ipclib.dll")] private static extern process_handle_t process_get_current_process_handle_duplicate_h(process_handle_t target_process);

        [DllImport("ipclib.dll")] private static extern process_handle_t process_get_current_process_handle_duplicate_s(size_t target_process);

        [DllImport("ipclib.dll")] 
        private static extern size_t process_get_current_process_handle_duplicate_int_h(process_handle_t target_process);
        
        [DllImport("ipclib.dll")]
        private static extern size_t process_get_current_process_handle_duplicate_int_s(size_t target_process);
        #endregion

        
        public static process_handle_t get_current_process_handle()
        {
            return process_get_current_process_handle();
        }
        public static size_t get_current_process_handle_int()
        {
            return process_get_current_process_handle_int();
        }
        public static process_handle_t get_current_process_handle_duplicate(process_handle_t target_process)
        {
            return process_get_current_process_handle_duplicate_h(target_process);
        }
        public static process_handle_t get_current_process_handle_duplicate(size_t target_process)
        {
            return process_get_current_process_handle_duplicate_s(target_process);
        }
        public static size_t get_current_process_handle_duplicate_int(process_handle_t target_process)
        {
            return process_get_current_process_handle_duplicate_int_h(target_process);
        }
        public static size_t get_current_process_handle_duplicate_int(size_t target_process)
        {
            return process_get_current_process_handle_duplicate_int_s(target_process);

        }


    }
}
