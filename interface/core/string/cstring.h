#ifndef HD_INC_CORE_STRING_CSTRING_H
#define HD_INC_CORE_STRING_CSTRING_H
#include "../character.h"
#include "../memory.h"
#include <stdarg.h> // va_start, va_end
// For is_ascii check : https://quick-bench.com/q/P_adhBeQdvHLTBB8EZCtLyrPRsM
namespace hud
{

    struct cstring
    {
        // RSIZE_MAX_STR defines the maximum allowed size for strings or buffers
        // handled by our code.
        //
        // We limit it to 4 KB (4 * 1024 bytes) to prevent:
        // 1. Buffer overflows.
        // 2. Excessive allocations or copies that could stress memory.
        //
        // This constant is mainly used to check string lengths in safe copy/move
        // or construction operations, ensuring all operations stay within a
        // reasonable and safe limit.
        //
        // Declared constexpr so it is known at compile-time and static to limit
        // its scope to this file or class.
        static constexpr u32 RSIZE_MAX_STR {4UL << 10}; // 4KB

        /**
         * Test whether null-terminated string contains only pure ansi characters.
         * @param string The null-terminated string
         * @return Always return true
         */
        [[nodiscard]] static constexpr bool is_ascii(const char8 *string) noexcept
        {
            if (string == nullptr) {
                return false;
            }

            while (!character::is_null(*string)) {
                if (!character::is_ascii(*string)) {
                    return false;
                }
                string++;
            }
            return true;
        }

        /**
         * Test whether wide null-terminated string contains only pure ansi characters.
         * @param string The null-terminated string
         * @return true if the string contains only char8, false otherwise
         */
        [[nodiscard]] static constexpr bool is_ascii(const wchar *string) noexcept
        {
            if (string == nullptr) {
                return false;
            }

            while (!character::is_null(*string)) {
                if (!character::is_ascii(*string)) {
                    return false;
                }
                string++;
            }
            return true;
        }

        /**
         * Test whether wide null-terminated string contains only pure ansi characters, checking string_size is not bigger than length of the string.
         * @param string The null-terminated string
         * @param string_size Size of the string in characters to test
         * @return true if the string contains only char8 and reach null-terminator character or the string_size character.
         *         false if the string contains non char8 character
         */
        [[nodiscard]] static constexpr bool is_ascii_safe(const char8 *string, usize string_size) noexcept
        {
            if (string == nullptr) {
                return false;
            }

            while (string_size-- > 0) {
                char8 cur = *string;
                if (character::is_null(cur)) {
                    return true;
                }
                if (!character::is_ascii(cur)) {
                    return false;
                }
                string++;
            }
            return true;
        }

        /**
         * Test whether wide null-terminated string contains only pure ansi characters, checking string_size is not bigger than length of the string.
         * @param string The null-terminated string
         * @param string_size Size of the string in characters to test
         * @return true if the string contains only char8 and reach null-terminator character or the string_size character.
         *         false if the string contains non char8 character
         */
        [[nodiscard]] static constexpr bool is_ascii_safe(const wchar *string, usize string_size) noexcept
        {
            if (string == nullptr) {
                return false;
            }

            while (string_size-- > 0) {
                wchar cur = *string;
                if (character::is_null(cur)) {
                    return true;
                }
                if (!character::is_ascii(cur)) {
                    return false;
                }
                string++;
            }
            return true;
        }

        /**
         * Test if a null-terminated string is null or empty.
         * @param string The null-terminated string
         * @return true if the string is null or empty, false otherwise
         */
        [[nodiscard]] static HD_FORCEINLINE bool is_null_or_empty(const char8 *const string) noexcept
        {
            return (string == nullptr) || character::is_null(*string);
        }

        /**
         * Test if a wide null-terminated string is null or empty.
         * @param string The null-terminated string
         * @return true if the string is null or empty, false otherwise
         */
        [[nodiscard]] static HD_FORCEINLINE bool is_null_or_empty(const wchar *const string) noexcept
        {
            return (string == nullptr) || character::is_null(*string);
        }

        /**
         * Copy ansi string.
         * @param destination The destination char8 buffer
         * @param source Null-terminated char8 to copy
         * @return destination pointer
         */
        static HD_FORCEINLINE char8 *copy(char8 *destination, const char8 *source) noexcept
        {
            check_not_null(destination, source);
            return strcpy(destination, source);
        }

        /**
         * Copy wide string.
         * @param destination The destination string buffer
         * @param source Null-terminated string to copy
         * @return destination pointer
         */
        static HD_FORCEINLINE wchar *copy(wchar *destination, const wchar *source) noexcept
        {
            check_not_null(destination, source);
            return wcscpy(destination, source);
        }

        /**
         * Copy characters from ansi string.
         * Caution should be take when source has more character than count, in this case no null character is append in the destination buffer.
         * To append a null character use copy_partial_safe instead.
         * @param destination The destination char8 buffer
         * @param source Null-terminated char8 to copy
         * @param count Number of character to copy
         * @return Destination pointer
         */
        static HD_FORCEINLINE char8 *copy_partial(char8 *destination, const char8 *source, const usize count) noexcept
        {
            check_not_null(destination, source);
            return strncpy(destination, source, count);
        }

        /**
         * Copy characters from wide string.
         * Caution should be take when source has more character than count, in this case no null character is append in the destination buffer.
         * To append a null character use copy_partial_safe instead.
         * @param destination The destination string buffer
         * @param source Null-terminated string to copy
         * @param count Number of character to copy
         * @return destination pointer
         */
        static HD_FORCEINLINE wchar *copy_partial(wchar *destination, const wchar *source, const usize count) noexcept
        {
            check_not_null(destination, source);
            return wcsncpy(destination, source, count);
        }

        /**
         * Appends a ansi string to another ansi string.
         * @param destination The destination char8 buffer
         * @param source Null-terminated char8 to append
         * @return destination pointer
         */
        static HD_FORCEINLINE char8 *append(char8 *destination, const char8 *source) noexcept
        {
            check_not_null(destination, source);
            return strcat(destination, source);
        }

        /**
         * Appends a wide string to another wide string.
         * @param destination The destination string buffer
         * @param source Null-terminated string to append
         * @return destination pointer
         */
        static HD_FORCEINLINE wchar *append(wchar *destination, const wchar *source) noexcept
        {
            check_not_null(destination, source);
            return wcscat(destination, source);
        }

        /**
         * Appends a part of an ansi string to another ansi string.
         * @param destination The destination char8 buffer
         * @param source Null-terminated char8 to append
         * @param count Number of character to append
         * @return destination pointer
         */
        static HD_FORCEINLINE char8 *append_partial(char8 *destination, const char8 *source, const usize count) noexcept
        {
            check_not_null(destination, source);
            return strncat(destination, source, count);
        }

        /**
         * Appends a part of an wide string to another wide string.
         * @param destination The destination wchar buffer
         * @param source Null-terminated wchar to append
         * @param count Number of character to append
         * @return destination pointer
         */
        static HD_FORCEINLINE wchar *append_partial(wchar *destination, const wchar *source, const usize count) noexcept
        {
            return wcsncat(destination, source, count);
        }

        /**
         * Convert string to uppercase.
         * @param string The string buffer to capitalize
         * @return string pointer
         */
        template<typename type_t>
        requires(hud::is_one_of_types_v<type_t, char8, wchar>)
        static type_t *to_uppercase(type_t *string) noexcept
        {
            type_t *ptr = string;
            while (!character::is_null(*ptr)) {
                type_t *cur = ptr++;
                *cur = character::to_uppercase(*cur);
            }
            return string;
        }

        /**
         * Convert a null-terminated string to uppercase.
         * @param string The null-terminated string buffer to capitalize
         * @param string_size Size of string buffer in characters.
         * @return true if capitalization success, false if an error occured (string is nullptr or null-terminated is reach before string_size characters)
         */
        template<typename type_t>
        requires(hud::is_one_of_types_v<type_t, char8, wchar>)
        static bool to_uppercase_safe(type_t *string, usize string_size) noexcept
        {
            if (string == nullptr) {
                return false;
            }

            while (string_size-- > 0) {
                type_t *cur = string++;
                if (character::is_null(*cur)) {
                    return false;
                }
                *cur = character::to_uppercase(*cur);
            }
            return character::is_null(*string);
        }

        /**
         * Convert string to uppercase.
         * @param string The string buffer to capitalize
         * @param count Number of character to capitalize
         * @return string pointer
         */
        template<typename type_t>
        requires(hud::is_one_of_types_v<type_t, char8, wchar>)
        static type_t *to_uppercase_partial(type_t *string, usize count) noexcept
        {
            type_t *ptr = string;
            while (count-- > 0) {
                *ptr = character::to_uppercase(*ptr);
                ptr++;
            }
            return string;
        }

        /**
         * Convert string to uppercase and check the given parameters.
         * @param string The string buffer to capitalize
         * @param string_size Size of string buffer in characters.
         * @param count Number of character to capitalize
         * @return true if capitalization success, false if an error occured (string is nullptr or null-terminated is reach before string_size characters)
         */
        template<typename type_t>
        requires(hud::is_one_of_types_v<type_t, char8, wchar>)
        static bool to_uppercase_partial_safe(type_t *string, usize string_size, usize count) noexcept
        {
            if (string == nullptr || string_size < count) {
                return false;
            }

            const usize not_capatilized_count = string_size - count;
            while (string_size-- > not_capatilized_count) {
                type_t *cur = string++;
                if (character::is_null(*cur)) {
                    return false;
                }
                *cur = character::to_uppercase(*cur);
            }

            return true;
        }

        /**
         * Convert string to lowercase.
         * @param string The string buffer to minimize
         * @return string pointer
         */
        template<typename type_t>
        requires(hud::is_one_of_types_v<type_t, char8, wchar>)
        static HD_FORCEINLINE type_t *to_lowercase(type_t *string) noexcept
        {
            type_t *ptr = string;
            while (!character::is_null(*ptr)) {
                *ptr = character::to_lowercase(*ptr);
                ptr++;
            }
            return string;
        }

        /**
         * Convert string to lowercase and check the given parameters.
         * @param string The string buffer to minimize
         * @param string_size Size of string buffer in characters.
         * @return true if minimization success, false if an error occured (string is nullptr or null-terminated is reach before string_size characters)
         */
        template<typename type_t>
        requires(hud::is_one_of_types_v<type_t, char8, wchar>)
        static bool to_lowercase_safe(type_t *string, usize string_size) noexcept
        {
            if (string == nullptr) {
                return false;
            }

            type_t *ptr = string;
            while (string_size-- > 0) {
                if (character::is_null(*ptr)) {
                    return false;
                }
                *ptr = character::to_lowercase(*ptr);
                ptr++;
            }
            return character::is_null(*ptr);
        }

        /**
         * Convert string to lowercase.
         * @param string The string buffer to minimize
         * @param count Number of character to minimize
         * @return string pointer
         */
        template<typename type_t>
        requires(hud::is_one_of_types_v<type_t, char8, wchar>)
        static type_t *to_lowercase_partial(type_t *string, usize count) noexcept
        {
            type_t *ptr = string;
            while (count-- > 0) {
                *ptr = character::to_lowercase(*ptr);
                ptr++;
            }
            return string;
        }

        /**
         * Convert wide to uppercase and check the given parameters.
         * @param string The string buffer to capitalize
         * @param string_size Size of string buffer in characters.
         * @param count Number of character to capitalize
         * @return true if capitalization success, false if an error occured (string is nullptr or null-terminated is reach before string_size characters)
         */
        template<typename type_t>
        requires(hud::is_one_of_types_v<type_t, char8, wchar>)
        static bool to_lowercase_partial_safe(type_t *string, usize string_size, usize count) noexcept
        {
            if (string == nullptr || string_size < count) {
                return false;
            }

            const usize not_minimized_count = string_size - count;
            while (string_size-- > not_minimized_count) {
                type_t *cur = string++;
                if (character::is_null(*cur)) {
                    return false;
                }
                *cur = character::to_lowercase(*cur);
            }

            return true;
        }

        /**
         * Compare two ansi strings with case sensitive comparison.
         * @param string_0 Null-terminated string
         * @param string_1 Null-terminated string
         * @return true if equal, false otherwise
         */
        [[nodiscard]] static constexpr bool equals(const char8 *string_0, const char8 *string_1) noexcept
        {
            if consteval {
                if (!string_0 || !string_1)
                    return false;
                while (*string_0 && *string_1) {
                    if (*string_0 != *string_1)
                        return false;
                    ++string_0;
                    ++string_1;
                }
                return *string_0 == *string_1;
            }
            else {
                return strcmp(string_0, string_1) == 0;
            }
        }

        /**
         * Compare two wide strings with case sensitive comparison.
         * @param string_0 Null-terminated string
         * @param string_1 Null-terminated string
         * @return 1 if equal, 0 otherwise
         */
        [[nodiscard]] static constexpr bool equals(const wchar *string_0, const wchar *string_1) noexcept
        {
            if consteval {
                if (!string_0 || !string_1)
                    return false;
                while (*string_0 && *string_1) {
                    if (*string_0 != *string_1)
                        return false;
                    ++string_0;
                    ++string_1;
                }
                return *string_0 == *string_1;
            }
            else {
                return wcscmp(string_0, string_1) == 0;
            }
        }

        /**
         * Compare partially two ansi strings with case sensitive comparison.
         * @param string_0 Null-terminated string
         * @param string_1 Null-terminated string
         * @param count Number of character to compare
         * @return true if equal, false otherwise
         */
        [[nodiscard]] static constexpr bool equals_partial(const char8 *string_0, const char8 *string_1, const usize count) noexcept
        {
            if consteval {
                size_t i = 0;
                while (i < count) {
                    char8 c0 = string_0[i];
                    char8 c1 = string_1[i];
                    if (c0 != c1) {
                        return (c0 - c1) == 0;
                    }
                    if (c0 == '\0') {
                        return true;
                    }
                    i++;
                }
                return true;
            }
            else {
                return strncmp(string_0, string_1, count) == 0;
            }
        }

        /**
         * Compare partially two wide strings with case sensitive comparison.
         * @param string_0 Null-terminated string
         * @param string_1 Null-terminated string
         * @param count Number of character to compare
         * @return true if equal, false otherwise
         */
        [[nodiscard]] static constexpr bool equals_partial(const wchar *string_0, const wchar *string_1, const usize count) noexcept
        {
            if consteval {
                size_t i = 0;
                while (i < count) {
                    wchar c0 = string_0[i];
                    wchar c1 = string_1[i];
                    if (c0 != c1) {
                        return (c0 - c1) == 0;
                    }
                    if (c0 == L'\0') {
                        return true;
                    }
                    i++;
                }
                return true;
            }
            else {
                return wcsncmp(string_0, string_1, count) == 0;
            }
        }

        /**
         * Retrieve the length of a ansi string.
         * @param string Null-terminated string
         * @return Length of the string
         */
        [[nodiscard]] static constexpr usize length(const char8 *string) noexcept
        {
            if consteval {
                // LCOV_EXCL_START
                usize string_length = 0;
                while (*string != '\0') {
                    string_length++;
                    string++;
                }
                return string_length;
                // LCOV_EXCL_STOP
            }
            else {
                return strlen(string);
            }
        }

        /**
         * Retrieve the length of a wide string.
         * @param string Null-terminated string
         * @return Length of the string
         */
        [[nodiscard]] static constexpr usize length(const wchar *string) noexcept
        {
            if consteval {
                // LCOV_EXCL_START
                usize string_length = 0;
                while (*string != '\0') {
                    string_length++;
                    string++;
                }
                return string_length;
                // LCOV_EXCL_STOP
            }
            else {
                return wcslen(string);
            }
        }

        /**
         * Find a string in another string.
         * @param string The string to be scanned
         * @param string_to_find The string to find
         * @return Pointer to the first occurrence of string in another string, nullptr if not found
         */
        [[nodiscard]] static constexpr const char8 *find_string(const char8 *string, const char8 *const string_to_find) noexcept
        {
            if consteval {
                if (!*string_to_find)
                    return string;
                for (; *string; ++string) {
                    const char8 *h = string;
                    const char8 *n = string_to_find;
                    while (*n && *h && *h == *n) {
                        ++h;
                        ++n;
                    }

                    if (!*n)
                        return string;
                }
                return nullptr;
            }
            else {
                return strstr(string, string_to_find);
            }
        }

        /**
         * Find a string in another string.
         * @param string The string to be scanned
         * @param string_to_find The string to find
         * @return Pointer to the first occurrence of string in another string, nullptr if not found
         */
        [[nodiscard]] static constexpr const wchar *find_string(const wchar *string, const wchar *const string_to_find) noexcept
        {
            if consteval {
                if (!*string_to_find)
                    return string;
                for (; *string; ++string) {
                    const wchar *h = string;
                    const wchar *n = string_to_find;
                    while (*n && *h && *h == *n) {
                        ++h;
                        ++n;
                    }

                    if (!*n)
                        return string;
                }
                return nullptr;
            }
            else {
                return wcsstr(string, string_to_find);
            }
        }

        /**
         * Find a character in a string.
         * @param string The string to be scanned
         * @param character_to_find The string to find
         * @return Pointer to the first occurrence of the character in the string, nullptr if not found
         */
        [[nodiscard]] static constexpr const char8 *find_character(const char8 *string, const char8 character_to_find) noexcept
        {
            if consteval {
                while (*string != '\0') {
                    if (*string == character_to_find)
                        return string;
                    string++;
                }
                return nullptr;
            }
            else {
                return strchr(string, character_to_find);
            }
        }

        /**
         * Find a character in a string.
         * @param string The string to be scanned
         * @param character_to_find The string to find
         * @return Pointer to the first occurrence of the character in the string, nullptr if not found
         */
        [[nodiscard]] static constexpr const wchar *find_character(const wchar *string, const wchar character_to_find) noexcept
        {
            if consteval {
                while (*string != '\0') {
                    if (*string == character_to_find)
                        return string;
                    string++;
                }
                return nullptr;
            }
            else {
                return wcschr(string, character_to_find);
            }
        }

        /**
         * Write a formatted char8 to a char8 buffer (like printf does).
         * @param buffer The char8 buffer receiving the formatted string
         * @praam buffer_size The maximum number of character to store in buffer, null-terminator character included
         * @param format The char8 containing the format of the string
         * @param args Depending of the format, list of arguments
         * @return Number of character written, -1 if an error occurred.
         */
        static HD_FORCEINLINE i32 format_vargs(char8 *buffer, u32 buffer_size, const char8 *format, va_list args) noexcept
        {
            return vsnprintf(buffer, buffer_size, format, args);
        }

        /**
         * Copy ansi string and assert the given parameters.
         * @param destination The destination char8 buffer
         * @param destination_size Size of destination buffer in bytes, including the null-terminator character
         * @param source Null-terminated char8 to copy
         * @return true if copy success, false if an error occured
         */
        static HD_FORCEINLINE bool copy_safe(char8 *destination, const usize destination_size, const char8 *source) noexcept
        {
            check_params(destination, destination_size, source, length(source) + 1);
#if defined(HD_HOST_WINDOWS)
            return strcpy_s(destination, destination_size, source) == 0;
#else
            strncpy(destination, source, length(source) + 1);
            return true;
#endif
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
#if defined(HD_HOST_WINDOWS)
            return wcscpy_s(destination, destination_size, source) == 0;
#else
            wcsncpy(destination, source, length(source) + 1);
            return true;
#endif
        }

        /**
         * Copy characters from ansi string and assert the given parameters.
         * Contrary to the unsafe version if source has more character than count the null character is appended in the destination buffer.
         * @param destination The destination char8 buffer
         * @param destination_size Size of destination buffer in bytes, including the null-terminator character
         * @param source Null-terminated char8 to copy
         * @param count Number of character to copy
         * @return true if copy success, false if an error occured
         */
        static HD_FORCEINLINE bool copy_partial_safe(char8 *destination, const usize destination_size, const char8 *source, const usize count) noexcept
        {
            check_params(destination, destination_size, source, length(source) + 1);
#if defined(HD_HOST_WINDOWS)
            return strncpy_s(destination, destination_size, source, count) == 0;
#else
            check(count <= length(source));
            strncpy(destination, source, count);
            destination[count] = character::ANSI_NULL_CHARACTER;
            return true;
#endif
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
#if defined(HD_HOST_WINDOWS)
            return wcsncpy_s(destination, destination_size, source, count) == 0;
#else
            check(destination_size >= count);
            wcsncpy(destination, source, count);
            destination[count] = character::WIDE_NULL_CHARACTER;
            return true;
#endif
        }

        /**
         * Appends a ansi string to another ansi string and assert the given parameters.
         * @param destination The destination string buffer
         * @param destination_size Size of destination buffer in bytes
         * @param source Null-terminated string to append
         * @return true if appends success, false if an error occured
         */
        static HD_FORCEINLINE bool append_safe(char8 *destination, const usize destination_size, const char8 *source) noexcept
        {
            check_params(destination, destination_size, source, length(source) + 1);
#if defined(HD_HOST_WINDOWS)
            return strcat_s(destination, destination_size, source) == 0;
#else
            strcat(destination, source);
            return true;
#endif
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
#if defined(HD_HOST_WINDOWS)
            return wcscat_s(destination, destination_size, source) == 0;
#else
            wcscat(destination, source);
            return true;
#endif
        }

        /**
         * Appends a ansi string to another ansi string and assert the given parameters.
         * @param destination The destination string buffer
         * @param destination_size Size of destination buffer in bytes
         * @param source Null-terminated string to append
         * @param count Number of character to append
         * @return true if appends success, false if an error occured (destination_size is too small or destination or source is nullptr)
         */
        static HD_FORCEINLINE bool append_partial_safe(char8 *destination, const usize destination_size, const char8 *source, const usize count) noexcept
        {
            check_params(destination, destination_size, source, length(destination) + 1 + count);
#if defined(HD_HOST_WINDOWS)
            return strncat_s(destination, destination_size, source, count) == 0;
#else
            strncat(destination, source, count);
            return true;
#endif
        }

        /**
         * Appends a wide string to another wide string and assert the given parameters.
         * @param destination The destination string buffer
         * @param destination_size Size of destination buffer in bytes
         * @param source Null-terminated string to append
         * @param count Number of character to append
         * @return true if appends success, false if an error occured (destination_size is too small or destination or source is nullptr)
         */
        [[nodiscard]] static HD_FORCEINLINE bool append_partial_safe(wchar *destination, const usize destination_size, const wchar *source, const usize count) noexcept
        {
            check_params(destination, destination_size, source, length(destination) + 1 + count);
#if defined(HD_HOST_WINDOWS)
            return wcsncat_s(destination, destination_size, source, count) == 0;
#else
            wcsncat(destination, source, count);
            return true;
#endif
        }

        /**
         * Retrieve the length of a ansi string and check the given parameters.
         * @param string Null-terminated string
         * @param max_length Maximum number of character to count. Max limit is hud::cstring::RSIZE_MAX_STR
         * @return Length of the string, 0 if string is null pointer, max_length if null-terminator character was not found
         */
        [[nodiscard]] static constexpr HD_FORCEINLINE usize length_safe(const char8 *string, const usize max_length) noexcept
        {
            if consteval {
                // LCOV_EXCL_START
                usize string_length = 0;
                while (*string != '\0' || string_length > max_length) {
                    string_length++;
                    string++;
                }
                return string_length;
                // LCOV_EXCL_STOP
            }
            else {
                return is_safe_length_parameters_valid(string, max_length) ? strnlen(string, max_length > RSIZE_MAX_STR ? RSIZE_MAX_STR : max_length) : 0u;
            }
        }

        /**
         * Retrieve the length of a wide string and check the given parameters.
         * @param string Null-terminated string
         * @param max_length Maximum number of character to count. Max limit is hud::cstring::RSIZE_MAX_STR
         * @return Length of the string, 0 if string is null pointer, max_length if null-terminator character was not found
         */
        [[nodiscard]] static constexpr usize length_safe(const wchar *string, const usize max_length) noexcept
        {
            if consteval {
                // LCOV_EXCL_START
                usize string_length = 0;
                while (*string != '\0' || string_length > max_length) {
                    string_length++;
                    string++;
                }
                return string_length;
                // LCOV_EXCL_STOP
            }
            else {
                return is_safe_length_parameters_valid(string, max_length) ? wcsnlen(string, max_length > RSIZE_MAX_STR ? RSIZE_MAX_STR : max_length) : 0u;
            }
        }

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
#if defined(HD_HOST_WINDOWS)
            return _vsnwprintf(buffer, buffer_size, format, args);
#else
            return vswprintf(buffer, buffer_size, format, args);
#endif
        }

        /**
         * Write a formatted string to a string buffer (like printf does).
         * @param buffer The string buffer receiving the formatted string
         * @param buffer_size The maximum number of character to store in buffer, null-terminator character included
         * @param format The string containing the format of the string
         * @param args Depending of the format, list of arguments
         * @return Number of character written, -1 if an error occurred.
         */
        template<typename type_t>
        requires(hud::is_one_of_types_v<type_t, char8, wchar>)
        static i32 format(type_t *buffer, u32 buffer_size, const type_t *format, ...) noexcept
        {
            va_list args;
            va_start(args, format);
            i32 count = format_vargs(buffer, buffer_size, format, args);
            va_end(args);
            return count;
        }

    protected:
        /**
         * Checks that destination and source pointer are not null
         * @tparam type_t Only char8 or wchar
         * @param destination The destination pointer
         * @param source The source pointer
         */
        template<typename type_t>
        requires(hud::is_one_of_types_v<type_t, char8, wchar>)
        static HD_FORCEINLINE void check_not_null(type_t *destination, const type_t *source) noexcept
        {
            HUD_CHECK(destination != nullptr);
            HUD_CHECK(source != nullptr);
        }

        /**
         * Checks that destination and source pointer are not null, and checks that destination_size is large enough to receive source_size
         * @tparam type_t Only char8 or wchar
         * @param destination The destination pointer
         * @param destination_size The size in bytes of the buffer pointed by destination
         * @param source The source pointer
         * @param source_size The size in bytes of the buffer pointed by source
         */
        template<typename type_t>
        requires(hud::is_one_of_types_v<type_t, char8, wchar>)
        static HD_FORCEINLINE void check_params(type_t *destination, const usize destination_size, const type_t *source, const usize source_size) noexcept
        {
            check_not_null(destination, source);
            HUD_CHECK(destination_size >= source_size);
        }

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
            if constexpr (hud::compilation::is_assertion_enabled()) {
                // hud::debugger::break_here_if(string_is_nullptr);
                // hud::debugger::break_here_if(max_length_is_zero);
                // hud::debugger::break_here_if(max_length_is_greater_than_RSIZE_MAX_STR);
            }
            // We do not add the max_length_is_greater_than_RSIZE_MAX_STR to condition,
            // the value will be clamped to RSIZE_MAX_STR
            return !string_is_nullptr && !max_length_is_zero;
        }
    };

} // namespace hud

#endif // HD_INC_CORE_STRING_CSTRING_H