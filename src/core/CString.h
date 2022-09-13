#pragma once
#ifndef HD_INC_OSLAYER_CSTRING_H
#define HD_INC_OSLAYER_CSTRING_H

#if defined(HD_OS_WINDOWS)
#include "OSWindows/WindowsCString.h"
#elif defined(HD_OS_LINUX)
#include "OSLinux/LinuxCString.h"
#else
#error Targeted OS not supported
#endif

namespace hud {

    /** Provides raw C String operations. */
    struct CString :
        #if defined(HD_OS_WINDOWS)
        OS::Windows::CString
        #elif defined(HD_OS_LINUX)
        OS::Linux::CString
        #endif
    {
        /**
        * Write a formatted ansichar to a ansichar buffer (like printf does).
        * @param buffer The ansichar buffer receiving the formatted string
        * @praam buffer_size The maximum number of character to store in buffer, null-terminator character included
        * @param format The ansichar containing the format of the string
        * @param args Depending of the format, list of arguments
        * @return Number of character written, -1 if an error occurred.
        */
        static i32 format(ansichar* buffer, u32 buffer_size, const ansichar* format, ...) noexcept {
            va_list args;
            va_start(args, format);
            i32 count = format_vargs(buffer, buffer_size, format, args);
            va_end(args);
            return count;
        }

        /**
        * Write a formatted wchar to a wchar buffer (like printf does).
        * @param buffer The wchar buffer receiving the formatted string
        * @praam buffer_size The maximum number of character to store in buffer, null-terminator character included
        * @param format The wchar containing the format of the string
        * @param args Depending of the format, list of arguments
        * @return Number of character written, -1 if an error occurred.
        */
        static i32 format(wchar* buffer, u32 buffer_size, const wchar* format, ...) noexcept {
            va_list args;
            va_start(args, format);
            i32 count = format_vargs(buffer, buffer_size, format, args);
            va_end(args);
            return count;
        }
    };
 
} // namespace hud

#endif // HD_INC_OSLAYER_CSTRING_H