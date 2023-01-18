#ifndef HD_INC_CORE_OS_LINUX_CSTRING_H
#define HD_INC_CORE_OS_LINUX_CSTRING_H
#include "../os_common/cstring.h"

#if !defined(HD_OS_LINUX)
    #error This file must be included only when targetting Linux OS
#endif

namespace hud::os::linux
{

    struct cstring
        : public os::common::cstring
    {

        /**
         * Copy ansi string and assert the given parameters.
         * @param destination The destination ansichar buffer
         * @param destination_size Size of destination buffer in bytes, including the null-terminator character
         * @param source Null-terminated ansichar to copy
         * @return true if copy success, false if an error occured
         */
        static HD_FORCEINLINE bool copy_safe(ansichar *destination, const usize destination_size, const ansichar *source) noexcept
        {
            check_params(destination, destination_size, source, length(source) + 1);
            // if (!check(destination != nullptr) ||
            //     !check(destination_size > 0u) ||
            //     !check(source != nullptr) ||
            //     !check(destination_size > hud::isize_max))
            // {
            //     return false;
            // }

            strncpy(destination, source, length(source));
            return true;
        }

        /**
         * Copy wide string and assert the given parameters.
         * @param destination The destination string buffer
         * @param destination_size Size of destination buffer in bytes, including the null-terminator character
         * @param source Null-terminated string to copy
         * @return true if copy success, false if an error occured
         */
        static HD_FORCEINLINE bool copy_safe(wchar *destination, const usize destination_size, const wchar *source) noexcept
        {
            check_params(destination, destination_size, source, length(source) + 1);
            wcsncpy(destination, source, length(source) + 1);
            return true;
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
        static HD_FORCEINLINE bool copy_partial_safe(ansichar *destination, const usize destination_size, const ansichar *source, const usize count) noexcept
        {
            check_params(destination, destination_size, source, length(source) + 1);
            check(count <= length(source));
            strncpy(destination, source, count);
            destination[count] = character::ANSI_NULL_CHARACTER;
            return true;
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
        static HD_FORCEINLINE bool copy_partial_safe(wchar *destination, const usize destination_size, const wchar *source, const usize count) noexcept
        {
            check_params(destination, destination_size, source, length(source) + 1);
            check(destination_size >= count);
            wcsncpy(destination, source, count);
            destination[count] = character::WIDE_NULL_CHARACTER;
            return true;
        }

        /**
         * Appends a ansi string to another ansi string and assert the given parameters.
         * @param destination The destination string buffer
         * @param destination_size Size of destination buffer in bytes
         * @param source Null-terminated string to append
         * @return true if appends success, false if an error occured
         */
        static HD_FORCEINLINE bool append_safe(ansichar *destination, const usize destination_size, const ansichar *source) noexcept
        {
            check_params(destination, destination_size, source, length(source) + 1);
            strcat(destination, source);
            return true;
        }

        /**
         * Appends a wide string to another wide string and check the given parameters.
         * @param destination The destination string buffer
         * @param destination_size Size of destination buffer in bytes
         * @param source Null-terminated string to append
         * @return true if appends success, false if an error occured (destination_size is too small or destination or source is nullptr)
         */
        static HD_FORCEINLINE bool append_safe(wchar *destination, const usize destination_size, const wchar *source) noexcept
        {
            check_params(destination, destination_size, source, length(source) + 1);
            wcscat(destination, source);
            return true;
        }

        /**
         * Appends a ansi string to another ansi string and assert the given parameters.
         * @param destination The destination string buffer
         * @param destination_size Size of destination buffer in bytes
         * @param source Null-terminated string to append
         * @param count Number of character to append
         * @return true if appends success, false if an error occured (destination_size is too small or destination or source is nullptr)
         */
        static HD_FORCEINLINE bool append_partial_safe(ansichar *destination, const usize destination_size, const ansichar *source, const usize count) noexcept
        {
            check_params(destination, destination_size, source, length(destination) + 1 + count);
            strncat(destination, source, count);
            return true;
        }

        /**
         * Appends a wide string to another wide string and assert the given parameters.
         * @param destination The destination string buffer
         * @param destination_size Size of destination buffer in bytes
         * @param source Null-terminated string to append
         * @param count Number of character to append
         * @return true if appends success, false if an error occured (destination_size is too small or destination or source is nullptr)
         */
        static HD_FORCEINLINE bool append_partial_safe(wchar *destination, const usize destination_size, const wchar *source, const usize count) noexcept
        {
            check_params(destination, destination_size, source, length(destination) + 1 + count);
            wcsncat(destination, source, count);
            return true;
        }

        /**
         * Retrieve the length of a ansi string and check the given parameters.
         * @param string Null-terminated string
         * @param max_length Maximum number of character to count. Max limit is hud::cstring::RSIZE_MAX_STR
         * @return Length of the string, 0 if string is null pointer, max_length if null-terminator character was not found
         */
        static HD_FORCEINLINE usize length_safe(const ansichar *string, const usize max_length) noexcept
        {
            return is_safe_length_parameters_valid(string, max_length) ? strnlen(string, max_length > RSIZE_MAX_STR ? RSIZE_MAX_STR : max_length) : 0u;
        }

        /**
         * Retrieve the length of a wide string and check the given parameters.
         * @param string Null-terminated string
         * @param max_length Maximum number of character to count. Max limit is hud::cstring::RSIZE_MAX_STR
         * @return Length of the string, 0 if string is null pointer, max_length if null-terminator character was not found
         */
        static HD_FORCEINLINE usize length_safe(const wchar *string, const usize max_length) noexcept
        {
            return is_safe_length_parameters_valid(string, max_length) ? wcsnlen(string, max_length > RSIZE_MAX_STR ? RSIZE_MAX_STR : max_length) : 0u;
        }

        /**
         * Write a formatted ansichar to a ansichar buffer (like printf does).
         * @param buffer The ansichar buffer receiving the formatted string
         * @praam buffer_size The maximum number of character to store in buffer, null-terminator character included
         * @param format The ansichar containing the format of the string
         * @param args Depending of the format, list of arguments
         * @return Number of character written, -1 if an error occurred.
         */
        using hud::os::common::cstring::format_vargs;

        /**
         * Write a formatted wchar to a wchar buffer (like printf does).
         * @param buffer The wchar buffer receiving the formatted string
         * @praam buffer_size The maximum number of character to store in buffer, null-terminator character included
         * @param format The wchar containing the format of the string
         * @param args Depending of the format, list of arguments
         * @return Number of character written, -1 if an error occurred.
         */
        static HD_FORCEINLINE i32 format_vargs(wchar *buffer, u32 buffer_size, const wchar *format, va_list args) noexcept
        {
            return vswprintf(buffer, buffer_size, format, args);
        }

    private:
        /**
         * Check that parameters given to safe_length are valid
         * @param string Null-terminated string
         * @param max_length Maximum number of character to count
         * @return True if parameters are valid, false otherwise
         */
        static bool is_safe_length_parameters_valid(const void *string, const usize max_length)
        {
            const bool string_is_nullptr = string == nullptr;
            const bool max_length_is_zero = max_length == 0;
            const bool max_length_is_greater_than_RSIZE_MAX_STR = max_length > RSIZE_MAX_STR;
            if constexpr (hud::compilation::is_assertion_enabled())
            {
                hud::debugger::break_here_if(string_is_nullptr);
                hud::debugger::break_here_if(max_length_is_zero);
                hud::debugger::break_here_if(max_length_is_greater_than_RSIZE_MAX_STR);
            }
            // We do not add the max_length_is_greater_than_RSIZE_MAX_STR to condition,
            // the value will be clamped to RSIZE_MAX_STR
            return !string_is_nullptr && !max_length_is_zero;
        }
    };

} // namespace hud::os::linux

#endif /* HD_INC_CORE_OS_LINUX_CSTRING_H */