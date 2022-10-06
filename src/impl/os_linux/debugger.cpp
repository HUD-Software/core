#include <core/minimal.h>
#include <core/os_linux/debugger.h>
#include <fcntl.h> // open
#include <unistd.h> // read
#include <string.h> // strstr

namespace hud::os::linux{

    bool debugger::is_present() noexcept {
        // /proc/self/status contained TracerPid which is the PID of the process tracing this process ( 0 if not being traced )
        // https://man7.org/linux/man-pages/man5/procfs.5.html
        i32 file_descriptor = open("/proc/self/status", O_RDONLY);
        if (file_descriptor == -1) {
            return false;
        }

        constexpr usize BUFFER_BYTES_COUNT = 256;
        ansichar buffer[BUFFER_BYTES_COUNT];
        isize read_count = read(file_descriptor, buffer, BUFFER_BYTES_COUNT );
        if(read_count == -1) {
            return false;
        }

        buffer[read_count] = '\0';
        constexpr ansichar TRACERPID[] = "TracerPid:\t";
        ansichar* found = strstr(buffer, TRACERPID);
        if(found == nullptr) {
            return false;
        }
        return found[sizeof(TRACERPID)] != 0;
    }
}