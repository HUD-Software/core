#ifndef HD_INC_CORE_STRING_CSTRING_CSTRING_COMMON_H
#define HD_INC_CORE_STRING_CSTRING_CSTRING_COMMON_H
#include "../../character.h"
#include "../../assert.h"
#include "../../traits/is_one_of_types.h"
#include <string.h> // strcpy, strcmp, etc...
#include <wchar.h>  // wcscpy, wcscat, etc...
#include <cstdio>   // vsnprintf
#include <cstdarg>  // va_list

namespace hud::common
{
    struct cstring
    {
        static constexpr u32 RSIZE_MAX_STR {4UL << 10}; // 4KB

        // static constexpr u64 RSIZE_MAX {hud::usize_max >> 1}; // Largest acceptable size for bounds-checked functions

        /**
         * Test whether null-terminated string contains only pure ansi characters.
         * @param string The null-terminated string
         * @return Always return true
         */
        static HD_FORCEINLINE bool is_pure_ansi(const ansichar *string) noexcept
        {
            if (string == nullptr)
            {
                return false;
            }

            while (!character::is_null(*string))
            {
                if (!character::is_pure_ansi(*string))
                {
                    return false;
                }
                string++;
            }
            return true;
        }

        /**
         * Test whether wide null-terminated string contains only pure ansi characters.
         * @param string The null-terminated string
         * @return true if the string contains only ansichar, false otherwise
         */
        static bool is_pure_ansi(const wchar *string) noexcept
        {
            if (string == nullptr)
            {
                return false;
            }

            while (!character::is_null(*string))
            {
                if (!character::is_pure_ansi(*string))
                {
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
         * @return true if the string contains only ansichar and reach null-terminator character or the string_size character.
         *         false if the string contains non ansichar character
         */
        static HD_FORCEINLINE bool is_pure_ansi_safe(const ansichar *string, usize string_size) noexcept
        {
            return string != nullptr;
        }

        /**
         * Test whether wide null-terminated string contains only pure ansi characters, checking string_size is not bigger than length of the string.
         * @param string The null-terminated string
         * @param string_size Size of the string in characters to test
         * @return true if the string contains only ansichar and reach null-terminator character or the string_size character.
         *         false if the string contains non ansichar character
         */
        static bool is_pure_ansi_safe(const wchar *string, usize string_size) noexcept
        {
            if (string == nullptr)
            {
                return false;
            }

            while (string_size-- > 0)
            {
                wchar cur = *string;
                if (character::is_null(cur))
                {
                    return true;
                }
                if (!character::is_pure_ansi(cur))
                {
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
        static HD_FORCEINLINE bool is_null_or_empty(const ansichar *const string) noexcept
        {
            return (string == nullptr) || character::is_null(*string);
        }

        /**
         * Test if a wide null-terminated string is null or empty.
         * @param string The null-terminated string
         * @return true if the string is null or empty, false otherwise
         */
        static HD_FORCEINLINE bool is_null_or_empty(const wchar *const string) noexcept
        {
            return (string == nullptr) || character::is_null(*string);
        }

        /**
         * Copy ansi string.
         * @param destination The destination ansichar buffer
         * @param source Null-terminated ansichar to copy
         * @return destination pointer
         */
        static HD_FORCEINLINE ansichar *copy(ansichar *destination, const ansichar *source) noexcept
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
         * @param destination The destination ansichar buffer
         * @param source Null-terminated ansichar to copy
         * @param count Number of character to copy
         * @return Destination pointer
         */
        static HD_FORCEINLINE ansichar *copy_partial(ansichar *destination, const ansichar *source, const usize count) noexcept
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
         * @param destination The destination ansichar buffer
         * @param source Null-terminated ansichar to append
         * @return destination pointer
         */
        static HD_FORCEINLINE ansichar *append(ansichar *destination, const ansichar *source) noexcept
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
         * @param destination The destination ansichar buffer
         * @param source Null-terminated ansichar to append
         * @param count Number of character to append
         * @return destination pointer
         */
        static HD_FORCEINLINE ansichar *append_partial(ansichar *destination, const ansichar *source, const usize count) noexcept
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
        requires(hud::is_one_of_types_v<type_t, ansichar, wchar>)
        static type_t *to_uppercase(type_t *string) noexcept
        {
            type_t *ptr = string;
            while (!character::is_null(*ptr))
            {
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
        requires(hud::is_one_of_types_v<type_t, ansichar, wchar>)
        static bool to_uppercase_safe(type_t *string, usize string_size) noexcept
        {
            if (string == nullptr)
            {
                return false;
            }

            while (string_size-- > 0)
            {
                type_t *cur = string++;
                if (character::is_null(*cur))
                {
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
        requires(hud::is_one_of_types_v<type_t, ansichar, wchar>)
        static type_t *to_uppercase_partial(type_t *string, usize count) noexcept
        {
            type_t *ptr = string;
            while (count-- > 0)
            {
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
        requires(hud::is_one_of_types_v<type_t, ansichar, wchar>)
        static bool to_uppercase_partial_safe(type_t *string, usize string_size, usize count) noexcept
        {
            if (string == nullptr || string_size < count)
            {
                return false;
            }

            const usize not_capatilized_count = string_size - count;
            while (string_size-- > not_capatilized_count)
            {
                type_t *cur = string++;
                if (character::is_null(*cur))
                {
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
        requires(hud::is_one_of_types_v<type_t, ansichar, wchar>)
        static HD_FORCEINLINE type_t *to_lowercase(type_t *string) noexcept
        {
            type_t *ptr = string;
            while (!character::is_null(*ptr))
            {
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
        requires(hud::is_one_of_types_v<type_t, ansichar, wchar>)
        static bool to_lowercase_safe(type_t *string, usize string_size) noexcept
        {
            if (string == nullptr)
            {
                return false;
            }

            type_t *ptr = string;
            while (string_size-- > 0)
            {
                if (character::is_null(*ptr))
                {
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
        requires(hud::is_one_of_types_v<type_t, ansichar, wchar>)
        static type_t *to_lowercase_partial(type_t *string, usize count) noexcept
        {
            type_t *ptr = string;
            while (count-- > 0)
            {
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
        requires(hud::is_one_of_types_v<type_t, ansichar, wchar>)
        static bool to_lowercase_partial_safe(type_t *string, usize string_size, usize count) noexcept
        {
            if (string == nullptr || string_size < count)
            {
                return false;
            }

            const usize not_minimized_count = string_size - count;
            while (string_size-- > not_minimized_count)
            {
                type_t *cur = string++;
                if (character::is_null(*cur))
                {
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
        static constexpr bool equals(const ansichar *string_0, const ansichar *string_1) noexcept
        {
            if consteval
            {
                if (!string_0 || !string_1)
                    return false;
                while (*string_0 && *string_1)
                {
                    if (*string_0 != *string_1)
                        return false;
                    ++string_0;
                    ++string_1;
                }
                return *string_0 == *string_1;
            }
            else
            {
                return strcmp(string_0, string_1) == 0;
            }
        }

        /**
         * Compare two wide strings with case sensitive comparison.
         * @param string_0 Null-terminated string
         * @param string_1 Null-terminated string
         * @return 1 if equal, 0 otherwise
         */
        static constexpr bool equals(const wchar *string_0, const wchar *string_1) noexcept
        {
            if consteval
            {
                if (!string_0 || !string_1)
                    return false;
                while (*string_0 && *string_1)
                {
                    if (*string_0 != *string_1)
                        return false;
                    ++string_0;
                    ++string_1;
                }
                return *string_0 == *string_1;
            }
            else
            {
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
        static HD_FORCEINLINE bool equals_partial(const ansichar *string_0, const ansichar *string_1, const usize count) noexcept
        {
            return strncmp(string_0, string_1, count) == 0;
        }

        /**
         * Compare partially two wide strings with case sensitive comparison.
         * @param string_0 Null-terminated string
         * @param string_1 Null-terminated string
         * @param count Number of character to compare
         * @return true if equal, false otherwise
         */
        static HD_FORCEINLINE bool equals_partial(const wchar *string_0, const wchar *string_1, const usize count) noexcept
        {
            return wcsncmp(string_0, string_1, count) == 0;
        }

        /**
         * Retrieve the length of a ansi string.
         * @param string Null-terminated string
         * @return Length of the string
         */
        [[nodiscard]] static constexpr usize length(const ansichar *string) noexcept
        {
            if consteval
            {
                // LCOV_EXCL_START
                usize string_length = 0;
                while (*string != '\0')
                {
                    string_length++;
                    string++;
                }
                return string_length;
                // LCOV_EXCL_STOP
            }
            else
            {
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
            if consteval
            {
                // LCOV_EXCL_START
                usize string_length = 0;
                while (*string != '\0')
                {
                    string_length++;
                    string++;
                }
                return string_length;
                // LCOV_EXCL_STOP
            }
            else
            {
                return wcslen(string);
            }
        }

        /**
         * Find a string in another string.
         * @param string The string to be scanned
         * @param string_to_find The string to find
         * @return Pointer to the first occurrence of string in another string, nullptr if not found
         */
        static HD_FORCEINLINE const ansichar *find_string(const ansichar *const string, const ansichar *const string_to_find) noexcept
        {
            return strstr(string, string_to_find);
        }

        /**
         * Find a string in another string.
         * @param string The string to be scanned
         * @param string_to_find The string to find
         * @return Pointer to the first occurrence of string in another string, nullptr if not found
         */
        static HD_FORCEINLINE const wchar *find_string(const wchar *const string, const wchar *const string_to_find) noexcept
        {
            return wcsstr(string, string_to_find);
        }

        /**
         * Find a character in a string.
         * @param string The string to be scanned
         * @param character_to_find The string to find
         * @return Pointer to the first occurrence of the character in the string, nullptr if not found
         */
        static HD_FORCEINLINE const ansichar *find_character(const ansichar *const string, const ansichar character_to_find) noexcept
        {
            return strchr(string, character_to_find);
        }

        /**
         * Find a character in a string.
         * @param string The string to be scanned
         * @param character_to_find The string to find
         * @return Pointer to the first occurrence of the character in the string, nullptr if not found
         */
        static HD_FORCEINLINE const wchar *find_character(const wchar *const string, const wchar character_to_find) noexcept
        {
            return wcschr(string, character_to_find);
        }

        /**
         * Write a formatted ansichar to a ansichar buffer (like printf does).
         * @param buffer The ansichar buffer receiving the formatted string
         * @praam buffer_size The maximum number of character to store in buffer, null-terminator character included
         * @param format The ansichar containing the format of the string
         * @param args Depending of the format, list of arguments
         * @return Number of character written, -1 if an error occurred.
         */
        static HD_FORCEINLINE i32 format_vargs(ansichar *buffer, u32 buffer_size, const ansichar *format, va_list args) noexcept
        {
            return vsnprintf(buffer, buffer_size, format, args);
        }

    protected:
        /**
         * Checks that destination and source pointer are not null
         * @tparam type_t Only ansichar or wchar
         * @param destination The destination pointer
         * @param source The source pointer
         */
        template<typename type_t>
        requires(hud::is_one_of_types_v<type_t, ansichar, wchar>)
        static HD_FORCEINLINE void check_not_null(type_t *destination, const type_t *source) noexcept
        {
            check(destination != nullptr);
            check(source != nullptr);
        }

        /**
         * Checks that destination and source pointer are not null, and checks that destination_size is large enough to receive source_size
         * @tparam type_t Only ansichar or wchar
         * @param destination The destination pointer
         * @param destination_size The size in bytes of the buffer pointed by destination
         * @param source The source pointer
         * @param source_size The size in bytes of the buffer pointed by source
         */
        template<typename type_t>
        requires(hud::is_one_of_types_v<type_t, ansichar, wchar>)
        static HD_FORCEINLINE void check_params(type_t *destination, const usize destination_size, const type_t *source, const usize source_size) noexcept
        {
            check_not_null(destination, source);
            check(destination_size >= source_size);
        }
    };

} // namespace hud::common

#endif // HD_INC_CORE_STRING_CSTRING_CSTRING_COMMON_H