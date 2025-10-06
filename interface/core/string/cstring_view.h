#ifndef HD_INC_CORE_STRING_CSTRING_VIEW_H
#define HD_INC_CORE_STRING_CSTRING_VIEW_H
#include "cstring.h"
#include "../slice.h"
#include "unicode/utf8.h"
#include "unicode/utf16.h"
#include "unicode/utf32.h"

namespace hud
{
    /**
     * An immutable view of a C-style, null-terminated string.
     * This view does not own the underlying string and does not allow modification
     * of its length. It provides utility functions for querying and accessing
     * the string content.
     */
    template<typename char_t>
    struct cstring_view
    {
        /** Type of the underlying character. */
        using char_type = char_t;

        /**
         * Constructs a cstring_view from a C-style string pointer.
         * @param str Pointer to a null-terminated string. Must not be null.
         */
        constexpr cstring_view(char_t *str) noexcept
            : ptr_(str)
        {
            HUD_CHECK(ptr_ != nullptr && "Invalid null pointer");
        }

        /**
         * Returns the length of the string (number of characters before the null terminator).
         * @return The length of the string in characters.
         */
        [[nodiscard]]
        constexpr usize length() const noexcept
        {
            return hud::cstring::length(ptr_);
        }

        /**
         * Returns a pointer to the underlying C-style string.
         * @return A pointer to the string's first character (null-terminated).
         */
        [[nodiscard]]
        constexpr char_t *data() const noexcept
        {
            return ptr_;
        }

        /**
         * Checks if the string is empty.
         * @return true if the string is empty, false otherwise.
         */
        [[nodiscard]]
        constexpr bool is_empty() const noexcept
        {
            return hud::character::is_null(ptr_[0]);
        }

        /**
         * Checks if all characters in the string are ASCII characters.
         * @return true if the string contains only ASCII characters, false otherwise.
         */
        [[nodiscard]]
        constexpr bool is_ascii() const noexcept
        requires(sizeof(char_t) == 1)
        {
            usize i {0};
// In non constexpr context and if HD_SSE2 is on, we do the check with SSE2
#if HD_SSE2
            if consteval {
                // Constexpr context: fallback to scalar check
            }
            else {
                // 16 bytes mask of 0x80
                const __m128i mask = _mm_set1_epi8(char(0x80));
                // For each 16 bytes block in ptr_
                for (; i + 16 <= length(); i += 16) {
                    // Load the 16 bytes block
                    __m128i chunk = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ptr_ + i));
#if HD_SSE4_1
                    // Check if chunk AND mask have all bits is set to 0 or at least one is set to 1.
                    // _mm_testz_si128 return 1 if all bits is set to 0, 0 otherwise.
                    if (!_mm_testz_si128(chunk, mask)) {
                        return false;
                    }
#else
                    // Compute the bitwise AND of 128 bits with _mm_and_si128
                    // Then create mask from the most significant bit of each 8-bit element with _mm_movemask_epi8
                    if (_mm_movemask_epi8(_mm_or_si128(chunk, mask)) != 0)
                        return false;
#endif
                }
            }
#endif
            // Do the rest 1 byte at a time
            const char_t *p = ptr_ + i;
            while (*p != char_t {}) {
                if (!character::is_ascii(*p)) {
                    return false;
                }
                p++;
            }
            return true;
        }
        /**
         * Checks if all characters in the string are ASCII characters.
         * @return true if the string contains only ASCII characters, false otherwise.
         */
        [[nodiscard]]
        constexpr bool is_ascii() const noexcept
        requires(sizeof(char_t) == 2)
        {
            usize i = 0;
            usize len = length();
#if HD_SSE2
            if consteval {
                // Constexpr context: fallback to scalar check
            }
            else {
                // 16 bytes mask of 0xFF80
                const __m128i mask = _mm_set1_epi16(short(0xFF80));
                // For each 8 bytes block in ptr_ ( 8 char_t can fit in __m128i if sizeof(char_t) == 2 )
                for (; i + 8 <= len; i += 8) {
                    // Load the 16 bytes block
                    __m128i chunk = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ptr_ + i));
#if HD_SSE4_1
                    // Check if chunk AND mask have all bits is set to 0 or at least one is set to 1.
                    // _mm_testz_si128 return 1 if all bits is set to 0, 0 otherwise.
                    if (!_mm_testz_si128(chunk, mask)) {
                        return false;
                    }
#else
                    // Compute the bitwise AND of 128 bits with _mm_and_si128
                    // Then create mask from the most significant bit of each 8-bit element with _mm_movemask_epi8
                    if (_mm_movemask_epi8(_mm_or_si128(chunk, mask)) != 0)
                        return false;
#endif
                }
            }
#endif

            // Scalar fallback
            for (; i < len; ++i) {
                if (ptr_[i] > 0x7F) // ASCII = 0..127
                    return false;
            }
            return true;
        }
        [[nodiscard]]
        constexpr bool is_ascii() const noexcept
        requires(sizeof(char_t) == 4)
        {
            usize i = 0;
            usize len = length();
#if HD_SSE2
            if consteval {
                // Constexpr context: fallback to scalar check
            }
            else {
                // 16 bytes mask of 0xFFFFFF80
                const __m128i mask = _mm_set1_epi32(0xFFFFFF80);
                // For each 4 bytes block in ptr_ ( 4 char_t can fit in __m128i if sizeof(char_t) == 4 )
                for (; i + 4 <= len; i += 4) {
                    // Load the 16 bytes block
                    __m128i chunk = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ptr_ + i));
#if HD_SSE4_1
                    // Check if chunk AND mask have all bits is set to 0 or at least one is set to 1.
                    // _mm_testz_si128 return 1 if all bits is set to 0, 0 otherwise.
                    if (!_mm_testz_si128(chunk, mask)) {
                        return false;
                    }
#else
                    // Compute the bitwise AND of 128 bits with _mm_and_si128
                    // Then create mask from the most significant bit of each 8-bit element with _mm_movemask_epi8
                    if (_mm_movemask_epi8(_mm_or_si128(chunk, mask)) != 0)
                        return false;
#endif
                }
            }
#endif
            // Scalar fallback
            for (; i < len; ++i) {
                if (ptr_[i] > 0x7F) // ASCII = 0..127
                    return false;
            }
            return true;
        }

        /**
         * Checks if this string is equal to another cstring_view.
         * ⚠️ This function should be used only if the view is ascii
         * @param v Another cstring_view to compare.
         * @return true if both strings are identical, false otherwise.
         */
        [[nodiscard]]
        constexpr bool equals(const cstring_view<char_t> &v) const noexcept
        {
            return hud::cstring::equals(ptr_, v.ptr_);
        }

        /**
         * Checks if the first n characters of this string match another cstring_view.
         * ⚠️ This function should be used only if the view is ascii
         * @param v Another cstring_view to compare.
         * @param n Number of characters to compare.
         * @return true if the first n characters match, false otherwise.
         */
        [[nodiscard]]
        constexpr bool equals_partial(const cstring_view<char_t> &v, const usize n) const noexcept
        {
            return hud::cstring::equals_partial(ptr_, v.ptr_, n);
        }

        /**
         * Finds the first occurrence of a substring given as a C-style string.
         * ⚠️ This function should be used only if the view is ascii
         * @param to_find_ptr Pointer to a null-terminated string to search for.
         * @return Index of the first occurrence, or -1 if not found.
         */
        [[nodiscard]]
        constexpr isize find_first(const char_t *to_find_ptr) const noexcept
        {
            const char_t *result = hud::cstring::find_string(ptr_, to_find_ptr);
            return result == nullptr ? -1 : result - ptr_;
        }

        /**
         * Finds the first occurrence of a substring given as another cstring_view.
         * ⚠️ This function should be used only if the view is ascii
         * @param to_find Substring to search for.
         * @return Index of the first occurrence, or -1 if not found.
         */
        [[nodiscard]]
        constexpr isize find_first(const cstring_view<char_t> &to_find) const noexcept
        {
            return find_first(to_find.data());
        }

        /**
         * Finds the first occurrence of a character.
         * ⚠️ This function should be used only if the view is ascii
         * @param character_to_find The character to search for.
         * @return Index of the first occurrence, or -1 if not found.
         */
        [[nodiscard]]
        constexpr isize find_first_character(char_t character_to_find) const noexcept
        {
            const char_t *result = hud::cstring::find_character(ptr_, character_to_find);
            return result == nullptr ? -1 : result - ptr_;
        }

        /**
         * Checks if this string contains a given substring (C-style string).
         * ⚠️ This function should be used only if the view is ascii
         * @param to_find_str Substring to search for.
         * @return true if the substring is found, false otherwise.
         */
        [[nodiscard]]
        constexpr bool contains(const char_t *to_find_str) const noexcept
        {
            return hud::cstring::find_string(ptr_, to_find_str) != nullptr;
        }

        /**
         * Checks if this string contains a given substring (cstring_view).
         * ⚠️ This function should be used only if the view is ascii
         * @param to_find Substring to search for.
         * @return true if the substring is found, false otherwise.
         */
        [[nodiscard]]
        constexpr bool contains(const cstring_view<char_t> &to_find) const noexcept
        {
            return contains(to_find.data());
        }

        /**
         * Checks if this string contains a given character.
         * ⚠️ This function should be used only if the view is ascii
         * @param character_to_find The character to search for.
         * @return true if the character is found, false otherwise.
         */
        [[nodiscard]]
        constexpr bool contains(const char_t character_to_find) const noexcept
        {
            return hud::cstring::find_character(ptr_, character_to_find) != nullptr;
        }

        /**
         * Convert string to uppercase.
         * ⚠️ This function should be used only if the view is ascii
         * @param string The string buffer to capitalize
         * @return string pointer
         */
        constexpr void to_uppercase() noexcept
        {
            hud::cstring::ascii_to_uppercase(ptr_);
        }

        /**
         * Convert string to uppercase.
         * ⚠️ This function should be used only if the view is ascii
         * @param string The string buffer to capitalize
         * @param count Number of character to capitalize
         * @return string pointer
         */
        constexpr void to_uppercase_partial(usize count) noexcept
        {
            hud::cstring::ascii_to_uppercase_partial(ptr_, count);
        }

        /**
         * Convert string to lowercase.
         * ⚠️ This function should be used only if the view is ascii
         * @param string The string buffer to minimize
         * @return string pointer
         */
        constexpr void to_lowercase() noexcept
        {
            hud::cstring::ascii_to_lowercase(ptr_);
        }

        /**
         * Convert string to lowercase.
         * ⚠️ This function should be used only if the view is ascii
         * @param string The string buffer to minimize
         * @return string pointer
         */
        constexpr void to_lowercase_partial(usize count) noexcept
        {
            hud::cstring::ascii_to_lowercase_partial(ptr_, count);
        }

        /**
         * Returns a slice view of the string.
         * @return A slice representing all characters of this string.
         */
        [[nodiscard]]
        constexpr const hud::slice<char_t> as_slice() const noexcept
        {
            return hud::slice {ptr_, length()};
        }

        /**
         * Returns a slice view of the string.
         * @return A slice representing all characters of this string.
         */
        [[nodiscard]]
        constexpr hud::slice<char_t> as_slice() noexcept
        {
            return hud::slice {ptr_, length()};
        }

        /**
         * Provides read-only access to a character at a given index.
         * @param i Index of the character to access.
         * @return A const reference to the character at position i.
         */
        [[nodiscard]]
        constexpr char_t &operator[](const usize i) const noexcept
        {
            return ptr_[i];
        }

        /**
         * Validates whether the cstring_view is well-formed UTF-8 according to the Unicode specification.
         *
         * This function checks each sequence of bytes (ASCII, 2-, 3-, or 4-byte sequences) and ensures
         * the following rules are respected:
         * - ASCII bytes (< 0x80) are accepted directly.
         * - Multi-byte sequences must follow the correct pattern (10xxxxxx after a valid leading byte).
         * - Overlong encodings are rejected.
         * - Disallowed values (such as surrogates [U+D800, U+DFFF]) are rejected.
         * - Code points above U+10FFFF are rejected.
         *
         * @return true if the input is valid UTF-8, false otherwise.
         */
        [[nodiscard]]
        constexpr bool is_valid_utf8() const noexcept
        {
            return hud::unicode::is_valid_utf8(as_slice());
        }

        /**
         * Validates whether a given sequence of 16-bit units is well-formed UTF-16 according to the Unicode specification.
         *
         * This function checks each 16-bit unit (char16) and ensures the following rules are respected:
         * - Code units in the range [0x0000, 0xD7FF] and [0xE000, 0xFFFF] are valid single-unit characters.
         * - Surrogate pairs are correctly formed:
         *   - A high surrogate in the range [0xD800, 0xDBFF] must be followed by a low surrogate in the range [0xDC00, 0xDFFF].
         *   - Any high surrogate not followed by a valid low surrogate is rejected.
         *   - Low surrogates without a preceding high surrogate are rejected.
         * - Code points above U+10FFFF are implicitly invalid (handled by surrogate pair rules).
         *
         * The validation works in a portable, sequential manner, checking each 16-bit unit one by one.
         *
         * @return true if the input is valid UTF-16, false otherwise.
         */
        [[nodiscard]]
        constexpr bool is_valid_utf16() const noexcept
        {
            return hud::unicode::is_valid_utf16(as_slice());
        }

        /**
         * Validates whether a given sequence of 32-bit code points is well-formed UTF-32 according to the Unicode specification.
         *
         * This function checks each 32-bit unit (char32) and ensures the following rules are respected:
         * - Each code point must be in the valid Unicode range [0x0000, 0x10FFFF].
         * - Surrogate code points [0xD800, 0xDFFF] are explicitly invalid in UTF-32.
         * - No multi-unit sequences exist in UTF-32; each unit represents a complete code point.
         *
         * The validation is performed sequentially over each 32-bit code point.
         *
         * @return true if the input is valid UTF-32, false otherwise.
         */
        [[nodiscard]]
        constexpr bool is_valid_utf32() const noexcept
        {
            return hud::unicode::is_valid_utf32(as_slice());
        }

    private:
        /**Pointer to the null-terminated C-style string. */
        char_type *ptr_;
    };

} // namespace hud

#endif // HD_INC_CORE_STRING_CSTRING_VIEW_H