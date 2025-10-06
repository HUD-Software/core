#ifndef HD_INC_CORE_STRING_CSTRING_H
#define HD_INC_CORE_STRING_CSTRING_H
#include "../character.h"
#include "../memory.h"
#include <stdarg.h> // va_start, va_end
#include <string.h> // strncpy, wcsncpy, wcscat,
// For is_ascii check : https://quick-bench.com/q/P_adhBeQdvHLTBB8EZCtLyrPRsM
namespace hud
{
    namespace details::cstring
    {
        /**
         * Test whether null-terminated string contains only pure ascii characters.
         * @param string The null-terminated string
         * @return Always return true
         */
        template<typename char_t>
        requires(hud::is_one_of_types_v<char_t, char8, wchar, char16, char32>)
        [[nodiscard]] static constexpr bool is_ascii_portable(const char_t *string) noexcept
        {
            if (string == nullptr) [[unlikely]] {
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
         * Compare two strings with case sensitive comparison.
         * @param string_0 Null-terminated string
         * @param string_1 Null-terminated string
         * @return true if equal, false otherwise
         */
        template<typename type_t>
        requires(hud::is_one_of_types_v<type_t, char8, wchar, char16, char32>)
        [[nodiscard]] static constexpr bool equals_portable(const type_t *string_0, const type_t *string_1) noexcept
        {
            if (!string_0 || !string_1) [[unlikely]]
                return false;
            while (*string_0 && *string_1) {
                if (*string_0 != *string_1)
                    return false;
                ++string_0;
                ++string_1;
            }
            return *string_0 == *string_1;
        }

        /**
         * Compare partially two strings with case sensitive comparison.
         * @param string_0 Null-terminated string
         * @param string_1 Null-terminated string
         * @param count Number of character to compare
         * @return true if equal, false otherwise
         */
        template<typename type_t>
        requires(hud::is_one_of_types_v<type_t, char8, wchar, char16, char32>)
        [[nodiscard]] static constexpr bool equals_partial_portable(const type_t *string_0, const type_t *string_1, const usize count) noexcept
        {
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

        /**
         * Retrieve the length of a string.
         * @param string Null-terminated string
         * @return Length of the string
         */
        template<typename type_t>
        requires(hud::is_one_of_types_v<type_t, char8, wchar, char16, char32>)
        [[nodiscard]] static constexpr usize length_portable(const type_t *string) noexcept
        {
            usize string_length = 0;
            while (*string != type_t {}) {
                string_length++;
                string++;
            }
            return string_length;
        }

        /**
         * Find a string in another string.
         * @param string The string to be scanned
         * @param string_to_find The string to find
         * @return Pointer to the first occurrence of string in another string, nullptr if not found
         */
        template<typename type_t>
        requires(hud::is_one_of_types_v<type_t, char8, wchar, char16, char32>)
        [[nodiscard]] static constexpr const type_t *find_string_portable(const type_t *string, const type_t *const string_to_find) noexcept
        {
            if (!*string_to_find) [[unlikely]]
                return string;
            for (; *string; ++string) {
                const type_t *h = string;
                const type_t *n = string_to_find;
                while (*n && *h && *h == *n) {
                    ++h;
                    ++n;
                }
                if (!*n)
                    return string;
            }
            return nullptr;
        }

        /**
         * Find a character in a string.
         * @param string The string to be scanned
         * @param character_to_find The string to find
         * @return Pointer to the first occurrence of the character in the string, nullptr if not found
         */
        template<typename type_t>
        requires(hud::is_one_of_types_v<type_t, char8, wchar, char16, char32>)
        [[nodiscard]] static constexpr const type_t *find_character_portable(const type_t *string, const type_t character_to_find) noexcept
        {
            while (*string != type_t {}) {
                if (*string == character_to_find)
                    return string;
                string++;
            }
            return nullptr;
        }
        /**
         * Retrieve the length of a string and check the given parameters.
         * @param string Null-terminated string
         * @param max_length Maximum number of character to count. Max limit is hud::cstring::RSIZE_MAX_STR
         * @return Length of the string, 0 if string is null pointer, max_length if null-terminator character was not found
         */
        template<typename type_t>
        requires(hud::is_one_of_types_v<type_t, char8, wchar, char16, char32>)
        [[nodiscard]] static constexpr usize length_safe_portable(const type_t *string, const usize max_length) noexcept
        {
            usize string_length = 0;
            while (*string != type_t {} || string_length > max_length) {
                string_length++;
                string++;
            }
            return string_length;
        }

        /**
         * Copy string.
         * @param destination The destination char_t buffer
         * @param source Null-terminated char_t to copy
         * @return destination pointer
         */
        template<typename char_t>
        requires hud::is_one_of_types_v<char_t, char8, wchar, char16, char32>
        [[nodiscard]] static constexpr char_t *copy_portable(char_t *destination, const char_t *source) noexcept
        {
            char_t *dest = destination;
            while ((*dest++ = *source++) != 0) {}
            return destination;
        }

        /**
         * Copy characters from string.
         * Caution should be take when source has more character than count, in this case no null character is append in the destination buffer.
         * To append a null character use copy_partial_safe instead.
         * @param destination The destination char_t buffer
         * @param source Null-terminated char_t to copy
         * @param count Number of character to copy
         * @return Destination pointer
         */
        template<typename char_t>
        requires hud::is_one_of_types_v<char_t, char8, wchar, char16, char32>
        static constexpr char_t *copy_partial_portable(char_t *destination, const char_t *source, const usize count) noexcept
        {
            char_t *dest = destination;
            const char_t *src = source;
            usize i = 0;
            for (; i < count && *src != 0; ++i) {
                dest[i] = src[i];
            }
            for (; i < count; ++i) {
                dest[i] = 0;
            }
            return destination;
        }

        /**
         * Appends a string to another string.
         * @param destination The destination char_t buffer
         * @param source Null-terminated char_t to append
         * @return destination pointer
         */
        template<typename char_t>
        requires hud::is_one_of_types_v<char_t, char8, wchar, char16, char32>
        static constexpr char_t *append_portable(char_t *destination, const char_t *source) noexcept
        {
            char_t *dest = destination;
            while (*dest)
                ++dest;
            const char_t *src = source;
            while (*src) {
                *dest++ = *src++;
            }
            *dest = 0;
            return destination;
        }

        template<typename char_t>
        requires hud::is_one_of_types_v<char_t, char8, wchar, char16, char32>
        static constexpr char_t *append_partial_portable(char_t *destination, const char_t *source, const usize count) noexcept
        {
            char_t *dest = destination;
            // Se placer à la fin de destination
            while (*dest)
                ++dest;

            const char_t *src = source;
            usize i = 0;
            // Copier au plus 'count' caractères
            for (; i < count && *src != 0; ++i) {
                *dest++ = *src++;
            }
            // Null-terminer
            *dest = 0;

            return destination;
        }

    } // namespace details::cstring

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
         * Test whether null-terminated string contains only pure ascii characters.
         * @param string The null-terminated string
         * @return Always return true
         */
        template<typename char_t>
        requires(hud::is_one_of_types_v<char_t, char8, wchar, char16, char32>)
        [[nodiscard]] static constexpr bool is_ascii(const char_t *string) noexcept
        {
            return hud::details::cstring::is_ascii_portable(string);
        }

        /**
         * Test whether null-terminated string contains only pure ascii characters, checking string_size is not bigger than length of the string.
         * @param string The null-terminated string
         * @param string_size Size of the string in characters to test
         * @return true if the string contains only ascii and reach null-terminator character or the string_size character.
         *         false if the string contains non ascii character
         */
        template<typename char_t>
        requires(hud::is_one_of_types_v<char_t, char8, wchar, char16, char32>)
        [[nodiscard]] static constexpr bool is_ascii_safe(const char_t *string, usize string_size) noexcept
        {
            if (string == nullptr) [[unlikely]] {
                return false;
            }

            while (string_size-- > 0) {
                char_t cur = *string;
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
        template<typename char_t>
        requires(hud::is_one_of_types_v<char_t, char8, wchar, char16, char32>)
        [[nodiscard]] static HD_FORCEINLINE bool is_null_or_empty(const char_t *const string) noexcept
        {
            return (string == nullptr) || character::is_null(*string);
        }

        /**
         * Copy string.
         * @param destination The destination char_t buffer
         * @param source Null-terminated char_t to copy
         * @return destination pointer
         */
        template<typename char_t>
        requires hud::is_one_of_types_v<char_t, char8, wchar, char16, char32>
        [[nodiscard]] static constexpr char_t *copy(char_t *destination, const char_t *source) noexcept
        {
            if consteval {
                return hud::details::cstring::copy_portable(destination, source);
            }
            else {
                check_not_null(destination, source);
                if constexpr (hud::is_same_v<char_t, char8>) {
                    return strcpy(destination, source);
                }
                else if constexpr (hud::is_same_v<char_t, wchar>) {
                    return wcscpy(destination, source);
                }
                else if constexpr ((hud::is_same_v<char_t, char16> && sizeof(wchar) == 2) || // On Windows char16 and wchar is 2 bytes both
                                   (hud::is_same_v<char_t, char32> && sizeof(wchar) == 4))   // On Linux, MacOS or EMSCRIPTEN char32 and wchar is 4 bytes both
                {
                    return wcscpy(reinterpret_cast<wchar *>(destination), reinterpret_cast<const wchar *>(source));
                }
                else {
                    return hud::details::cstring::copy_portable(destination, source);
                }
            }
        }

        /**
         * Copy characters from string.
         * Caution should be take when source has more character than count, in this case no null character is append in the destination buffer.
         * To append a null character use copy_partial_safe instead.
         * @param destination The destination char_t buffer
         * @param source Null-terminated char_t to copy
         * @param count Number of character to copy
         * @return Destination pointer
         */
        template<typename char_t>
        requires hud::is_one_of_types_v<char_t, char8, wchar, char16, char32>
        static HD_FORCEINLINE char_t *copy_partial(char_t *destination, const char_t *source, const usize count) noexcept
        {
            if consteval {
                return hud::details::cstring::copy_partial_portable(destination, source, count);
            }
            else {
                check_not_null(destination, source);
                if constexpr (hud::is_same_v<char_t, char8>) {
                    return strncpy(destination, source, count);
                }
                else if constexpr (hud::is_same_v<char_t, wchar>) {
                    return wcsncpy(destination, source, count);
                }
                else if constexpr ((hud::is_same_v<char_t, char16> && sizeof(wchar) == 2) || // On Windows char16 and wchar is 2 bytes both
                                   (hud::is_same_v<char_t, char32> && sizeof(wchar) == 4))   // On Linux, MacOS or EMSCRIPTEN char32 and wchar is 4 bytes both
                {
                    return wcsncpy(reinterpret_cast<wchar *>(destination), reinterpret_cast<const wchar *>(source), count);
                }
                else {
                    return hud::details::cstring::copy_partial_portable(destination, source, count);
                }
            }
        }

        /**
         * Appends a string to another string.
         * @param destination The destination char_t buffer
         * @param source Null-terminated char_t to append
         * @return destination pointer
         */
        template<typename char_t>
        requires hud::is_one_of_types_v<char_t, char8, wchar, char16, char32>
        static HD_FORCEINLINE char_t *append(char_t *destination, const char_t *source) noexcept
        {
            if consteval {
                return hud::details::cstring::append_portable(destination, source);
            }
            else {
                check_not_null(destination, source);

                if constexpr (hud::is_same_v<char_t, char8>) {
                    return strcat(destination, source);
                }
                else if constexpr (hud::is_same_v<char_t, wchar>) {
                    return wcscat(destination, source);
                }
                else if constexpr ((hud::is_same_v<char_t, char16> && sizeof(wchar) == 2) || // On Windows char16 and wchar is 2 bytes both
                                   (hud::is_same_v<char_t, char32> && sizeof(wchar) == 4))   // On Linux, MacOS or EMSCRIPTEN char32 and wchar is 4 bytes both
                {
                    return wcscat(reinterpret_cast<wchar *>(destination), reinterpret_cast<const wchar *>(source));
                }
                else {
                    return hud::details::cstring::append_portable(destination, source);
                }
            }
        }

        /**
         * Appends a part of an string to another ansi.
         * @param destination The destination char_t buffer
         * @param source Null-terminated char_t to append
         * @param count Number of character to append
         * @return destination pointer
         */
        template<typename char_t>
        requires hud::is_one_of_types_v<char_t, char8, wchar, char16, char32>
        static HD_FORCEINLINE char_t *append_partial(char_t *destination, const char_t *source, const usize count) noexcept
        {
            if consteval {
                return hud::details::cstring::append_partial_portable(destination, source, count);
            }
            else {
                check_not_null(destination, source);
                if constexpr (hud::is_same_v<char_t, char8>) {
                    return strncat(destination, source, count);
                }
                else if constexpr (hud::is_same_v<char_t, wchar>) {
                    return wcsncat(destination, source, count);
                }
                else if constexpr ((hud::is_same_v<char_t, char16> && sizeof(wchar) == 2) || // On Windows char16 and wchar is 2 bytes both
                                   (hud::is_same_v<char_t, char32> && sizeof(wchar) == 4))   // On Linux, MacOS or EMSCRIPTEN char32 and wchar is 4 bytes both
                {
                    return wcsncat(reinterpret_cast<wchar *>(destination), reinterpret_cast<const wchar *>(source), count);
                }
                else {
                    return hud::details::cstring::append_partial_portable(destination, source, count);
                }
            }
        }

        /**
         * Convert string to uppercase.
         * @param string The string buffer to capitalize
         * @return string pointer
         */
        template<typename type_t>
        requires(hud::is_one_of_types_v<type_t, char8, wchar>)
        static type_t *ascii_to_uppercase(type_t *string) noexcept
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
        static bool ascii_to_uppercase_safe(type_t *string, usize string_size) noexcept
        {
            if (string == nullptr) [[unlikely]] {
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
        static type_t *ascii_to_uppercase_partial(type_t *string, usize count) noexcept
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
        static bool ascii_to_uppercase_partial_safe(type_t *string, usize string_size, usize count) noexcept
        {
            if (string == nullptr || string_size < count) [[unlikely]] {
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
        static HD_FORCEINLINE type_t *ascii_to_lowercase(type_t *string) noexcept
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
        static bool ascii_to_lowercase_safe(type_t *string, usize string_size) noexcept
        {
            if (string == nullptr) [[unlikely]] {
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
        static type_t *ascii_to_lowercase_partial(type_t *string, usize count) noexcept
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
        static bool ascii_to_lowercase_partial_safe(type_t *string, usize string_size, usize count) noexcept
        {
            if (string == nullptr || string_size < count) [[unlikely]] {
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
                return hud::details::cstring::equals_portable(string_0, string_1);
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
                return hud::details::cstring::equals_portable(string_0, string_1);
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
                return hud::details::cstring::equals_partial_portable(string_0, string_1, count);
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
                return hud::details::cstring::equals_partial_portable(string_0, string_1, count);
            }
            else {
                return wcsncmp(string_0, string_1, count) == 0;
            }
        }

        /**
         * Retrieve the length of a null terminated string.
         * @param string Null-terminated string
         * @return Length of the string
         */
        template<typename char_t>
        requires hud::is_one_of_types_v<char_t, char8, wchar, char16, char32>
        [[nodiscard]] static constexpr usize length(const char_t *string) noexcept
        {
            if consteval {
                return hud::details::cstring::length_portable(string);
            }
            else {
                if constexpr (hud::is_same_v<char_t, char8>) {
                    return strlen(string);
                }
                else if constexpr (hud::is_same_v<char_t, wchar>) {
                    return wcslen(string);
                }
                else if constexpr ((hud::is_same_v<char_t, char16> && sizeof(wchar) == 2) || // On Windows char16 and wchar is 2 bytes both
                                   (hud::is_same_v<char_t, char32> && sizeof(wchar) == 4))   // On Linux, MacOS or EMSCRIPTEN char32 and wchar is 4 bytes both
                {
                    return wcslen(reinterpret_cast<wchar *>(string));
                }
                else {
                    return hud::details::cstring::length_portable(string);
                }
            }
        }

        /**
         * Retrieve the length of a null terminated string and check the given parameters.
         * @param string Null-terminated string
         * @param max_length Maximum number of character to count. Max limit is hud::cstring::RSIZE_MAX_STR
         * @return Length of the string, 0 if string is null pointer, max_length if null-terminator character was not found
         */
        template<typename char_t>
        requires hud::is_one_of_types_v<char_t, char8, wchar, char16, char32>
        [[nodiscard]] static constexpr usize length_safe(const char_t *string, const usize max_length) noexcept
        {
            if consteval {
                return hud::details::cstring::length_safe_portable(string, max_length);
            }
            else {
                if constexpr (hud::is_same_v<char_t, char8>) {
                    return is_safe_length_parameters_valid(string, max_length) ? strnlen(string, max_length > RSIZE_MAX_STR ? RSIZE_MAX_STR : max_length) : 0u;
                }
                else if constexpr (hud::is_same_v<char_t, wchar>) {
                    return is_safe_length_parameters_valid(string, max_length) ? wcsnlen(string, max_length > RSIZE_MAX_STR ? RSIZE_MAX_STR : max_length) : 0u;
                }
                else if constexpr ((hud::is_same_v<char_t, char16> && sizeof(wchar) == 2) || // On Windows char16 and wchar is 2 bytes both
                                   (hud::is_same_v<char_t, char32> && sizeof(wchar) == 4))   // On Linux, MacOS or EMSCRIPTEN char32 and wchar is 4 bytes both
                {
                    return is_safe_length_parameters_valid(string, max_length) ? wcsnlen(string, max_length > RSIZE_MAX_STR ? RSIZE_MAX_STR : max_length) : 0u;
                }
                else {
                    return hud::details::cstring::length_safe_portable(string, max_length);
                }
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
                return hud::details::cstring::find_string_portable(string, string_to_find);
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
                return hud::details::cstring::find_string_portable(string, string_to_find);
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
                return hud::details::cstring::find_character_portable(string, character_to_find);
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
                return hud::details::cstring::find_character_portable(string, character_to_find);
            }
            else {
                return wcschr(string, character_to_find);
            }
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
        requires(hud::is_one_of_types_v<type_t, char8, wchar, char16, char32>)
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
        requires(hud::is_one_of_types_v<type_t, char8, wchar, char16, char32>)
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