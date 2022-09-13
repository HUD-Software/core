#pragma once
#ifndef HD_INC_OSLAYER_OS_WINDOWS_CSTRING_H
#define HD_INC_OSLAYER_OS_WINDOWS_CSTRING_H
#include "../OSCommon/CommonCString.h"

#if !defined(HD_OS_WINDOWS)
#error This file must be included only when targetting Windows OS
#endif 

namespace hud::OS::Windows{

    struct CString 
        : public OS::Common::CString
    {
        /**
        * Copy ansi string and assert the given parameters.
        * @param destination The destination ansichar buffer
        * @param destination_size Size of destination buffer in bytes, including the null-terminator character
        * @param source Null-terminated ansichar to copy
        * @return true if copy success, false if an error occured
        */
        static HD_FORCEINLINE bool copy_safe(ansichar* destination, const usize destination_size, const ansichar* source) noexcept {
            check(destination != nullptr);
            check(source != nullptr);
            check(destination_size >= (length(source) + 1));
            return strcpy_s(destination, destination_size, source) == 0;
        }

        /**
        * Copy wide string and assert the given parameters.
        * @param destination The destination string buffer
        * @param destination_size Size of destination buffer in bytes, including the null-terminator character
        * @param source Null-terminated string to copy
        * @return true if copy success, false if an error occured
        */
        static HD_FORCEINLINE bool copy_safe(wchar* destination, const usize destination_size, const wchar* source) noexcept {
            check(destination != nullptr);
            check(source != nullptr);
            check(destination_size >= (length(source) + 1));
            return wcscpy_s(destination, destination_size, source) == 0;
        }

        /**
        * Copy characters from ansi string and assert the given parameters.
        * Contrary to the unsafe version if source has more character than count the null character is appended in the destination buffer.
        * @param destination The destination ansichar buffer
        * @param destination_size Size of destination buffer in bytes, including the null-terminator character
        * @param source Null-terminated ansichar to copy
        * @param count Number of character to copy
        * @return true if copy success, false if an error occured
        */
        static HD_FORCEINLINE bool copy_partial_safe(ansichar* destination, const usize destination_size, const ansichar* source, const usize count) noexcept {
            check(destination != nullptr);
            check(source != nullptr);
            check(destination_size >= (length(source) + 1));
            return strncpy_s(destination, destination_size, source, count) == 0;
        }

        /**
        * Copy characters from wide string and assert the given parameters.
        * Contrary to the unsafe version if source has more character than count the null character is appended in the destination buffer.
        * @param destination The destination string buffer
        * @param destination_size Size of destination buffer in bytes, including the null-terminator character
        * @param source Null-terminated string to copy
        * @param count Number of character to copy
        * @return true if copy success, false if an error occured
        */
        static HD_FORCEINLINE bool copy_partial_safe(wchar* destination, const usize destination_size, const wchar* source, const usize count) noexcept {
            check(destination != nullptr);
            check(source != nullptr);
            check(destination_size >= (length(source) + 1));
            return wcsncpy_s(destination, destination_size, source, count) == 0;
        }

        /**
        * Appends a ansi string to another ansi string and assert the given parameters.
        * @param destination The destination string buffer
        * @param destination_size Size of destination buffer in bytes
        * @param source Null-terminated string to append
        * @return true if appends success, false if an error occured
        */
        static HD_FORCEINLINE bool append_safe(ansichar* destination, const usize destination_size, const ansichar* source) noexcept {
            check(destination != nullptr);
            check(source != nullptr);
            check(destination_size >= (length(source) + 1));
            return strcat_s(destination, destination_size, source) == 0;
        }

        /**
        * Appends a wide string to another wide string and check the given parameters.
        * @param destination The destination string buffer
        * @param destination_size Size of destination buffer in bytes
        * @param source Null-terminated string to append
        * @return true if appends success, false if an error occured (destination_size is too small or destination or source is nullptr)
        */
        static HD_FORCEINLINE bool append_safe(wchar* destination, const usize destination_size, const wchar* source) noexcept {
            check(destination != nullptr);
            check(source != nullptr);
            check(destination_size >= (length(source) + 1));
            return wcscat_s(destination, destination_size, source) == 0;
        }

        /**
        * Appends a ansi string to another ansi string and assert the given parameters.
        * @param destination The destination string buffer
        * @param destination_size Size of destination buffer in bytes
        * @param source Null-terminated string to append
        * @param count Number of character to append
        * @return true if appends success, false if an error occured (destination_size is too small or destination or source is nullptr)
        */
        static HD_FORCEINLINE bool append_partial_safe(ansichar* destination, const usize destination_size, const ansichar* source, const usize count) noexcept {
            check(destination != nullptr);
            check(source != nullptr);
            check(destination_size >= (length(destination) + 1 + count));
            return strncat_s(destination, destination_size, source, count) == 0;
        }

        /**
        * Appends a wide string to another wide string and assert the given parameters.
        * @param destination The destination string buffer
        * @param destination_size Size of destination buffer in bytes
        * @param source Null-terminated string to append
        * @param count Number of character to append
        * @return true if appends success, false if an error occured (destination_size is too small or destination or source is nullptr)
        */
        static HD_FORCEINLINE bool append_partial_safe(wchar* destination, const usize destination_size, const wchar* source, const usize count) noexcept {
            check(destination != nullptr);
            check(source != nullptr);
            check(destination_size >= (length(destination) + 1 + count));
            return wcsncat_s(destination, destination_size, source, count) == 0;
        }

        /**
        * Retrieve the length of a ansi string and check the given parameters.
        * @param string Null-terminated string
        * @param max_length Maximum number of character to count
        * @return Length of the string, 0 if string is null pointer, max_length if null-terminator character was not found
        */
        static HD_FORCEINLINE usize length_safe(const ansichar* string, const usize max_length) noexcept {
            return strnlen_s(string, max_length);
        }

        /**
        * Retrieve the length of a wide string and check the given parameters.
        * @param string Null-terminated string
        * @param max_length Maximum number of character to count
        * @return Length of the string, 0 if string is null pointer, max_length if null-terminator character was not found
        */
        static HD_FORCEINLINE usize length_safe(const wchar* string, const usize max_length) noexcept {
            return wcsnlen_s(string, max_length);
        }
        
        /**
        * Write a formatted ansichar to a ansichar buffer (like printf does).
        * @param buffer The ansichar buffer receiving the formatted string
        * @praam buffer_size The maximum number of character to store in buffer, null-terminator character included
        * @param format The ansichar containing the format of the string
        * @param args Depending of the format, list of arguments
        * @return Number of character written, -1 if an error occurred.
        */
        static HD_FORCEINLINE i32 format_vargs(ansichar* buffer, u32 buffer_size, const ansichar* format, va_list args) noexcept {
            return Common::CString::format_vargs(buffer, buffer_size, format, args);
        }

        /**
        * Write a formatted wchar to a wchar buffer (like printf does).
        * @param buffer The wchar buffer receiving the formatted string
        * @praam buffer_size The maximum number of character to store in buffer, null-terminator character included
        * @param format The wchar containing the format of the string
        * @param args Depending of the format, list of arguments
        * @return Number of character written, -1 if an error occurred.
        */
        static HD_FORCEINLINE i32 format_vargs(wchar* buffer, u32 buffer_size, const wchar* format, va_list args) noexcept {
            return _vsnwprintf(buffer, buffer_size, format, args);
        }
    };

} // namespace hud::OS::Windows

#endif // HD_INC_OSLAYER_OS_WINDOWS_CSTRING_H
