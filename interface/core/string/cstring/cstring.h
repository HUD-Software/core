#ifndef HD_INC_CORE_STRING_CSTRING_CSTRING_H
#define HD_INC_CORE_STRING_CSTRING_CSTRING_H

#if defined(HD_HOST_WINDOWS)
    #include "cstring_windows.h"
#elif defined(HD_HOST_LINUX) || defined(HD_HOST_EMSCRIPTEN)
    #include "cstring_linux.h"
#else
    #error Targeted OS not supported
#endif

namespace hud
{

    /** Provides raw C string operations. */
    struct cstring :
#if defined(HD_HOST_WINDOWS)
        hud::windows::cstring
#elif defined(HD_HOST_LINUX) || defined(HD_HOST_EMSCRIPTEN)
        hud::linux::cstring
#endif
    {
        /**
         * Write a formatted string to a string buffer (like printf does).
         * @param buffer The string buffer receiving the formatted string
         * @param buffer_size The maximum number of character to store in buffer, null-terminator character included
         * @param format The string containing the format of the string
         * @param args Depending of the format, list of arguments
         * @return Number of character written, -1 if an error occurred.
         */
        template<typename type_t>
        requires(hud::is_one_of_types_v<type_t, ansichar, wchar>)
        static i32 format(type_t *buffer, u32 buffer_size, const type_t *format, ...) noexcept
        {
            va_list args;
            va_start(args, format);
            i32 count = format_vargs(buffer, buffer_size, format, args);
            va_end(args);
            return count;
        }
    };

} // namespace hud

#endif // HD_INC_CORE_STRING_CSTRING_CSTRING_H