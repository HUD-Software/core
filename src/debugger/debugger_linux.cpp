#if defined(HD_COMPILER_CLANG) || defined(HD_COMPILER_GCC)
#include <core/debugger/debugger_linux.h>
#include <fcntl.h>  // open
#include <unistd.h> // read
#include <string.h> // strstr

namespace hud::linux
{
    bool debugger::is_present() noexcept
    {
        // /proc/self/status contained TracerPid which is the PID of the process tracing this process ( 0 if not being traced )
        // https://man7.org/linux/man-pages/man5/procfs.5.html
        i32 file_descriptor = open("/proc/self/status", O_RDONLY);
        if (file_descriptor == -1)
        // LCOV_EXCL_START
        {
            return false;
        }
        // LCOV_EXCL_STOP

        constexpr usize BUFFER_BYTES_COUNT = 256;
        char8 buffer[BUFFER_BYTES_COUNT];
        isize read_count = read(file_descriptor, buffer, BUFFER_BYTES_COUNT);
        // LCOV_EXCL_START
        if (read_count == -1) {
            return false;
        }
        // LCOV_EXCL_STOP
        buffer[read_count - 1] = '\0';
        constexpr char8 TRACERPID[] = "TracerPid:\t";
        char8 *found = strstr(buffer, TRACERPID);
        if (found == nullptr)
        // LCOV_EXCL_START
        {
            return false;
        }
        // LCOV_EXCL_STOP
        return found[sizeof(TRACERPID) - 1] != '0';
    }
} // namespace hud::linux

#endif