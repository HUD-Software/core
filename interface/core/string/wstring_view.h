#ifndef HD_INC_CORE_STRING_CWSTRING_VIEW_H
#define HD_INC_CORE_STRING_CWSTRING_VIEW_H
#include "cstring.h"
#include "../slice.h"
namespace hud
{
    /**
     * An immutable view of a C-style, null-terminated wide string (`wchar`).
     * This view does not own the underlying string and does not allow modification
     * of its length. It provides utility functions for querying and accessing
     * the string content.
     */
    template<typename char_t>
    struct wstring_view
    {
        /** Type of the underlying wide character. */
        using char_type = char_t;

        static_assert(hud::is_same_v<hud::remove_cv_t<char_type>, wchar>);

        /**
         * Constructs a wstring_view from a C-style string pointer.
         * @param str Pointer to a null-terminated string. Must not be null.
         */
        constexpr wstring_view(char_type *str) noexcept
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
            return cstring::length(ptr_);
        }

        /**
         * Returns a pointer to the underlying C-style string.
         * @return A pointer to the string's first character (null-terminated).
         */
        [[nodiscard]]
        constexpr const char_type *data() const noexcept
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
        {
            usize i {0};
// In non constexpr context and if HD_SSE2 is on, we do the check with SSE2
#if HD_SSE2
            if consteval {
            }
            else {
                static_assert((sizeof(wchar) == 2 || sizeof(wchar) == 4) && "wchar must be 2 or 4 bytes");
                if constexpr (sizeof(wchar) == 2) // UTF-16 / Windows
                {
                    // Mask 8 bytes of 0xFF80 with the ansichars
                    const __m128i mask = _mm_set1_epi16(0xFF80);
                    for (; i + 8 <= length(); i += 8) {
                        __m128i chunk = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ptr_ + i));
                        __m128i tmp = _mm_and_si128(chunk, mask);
                        __m128i zero = _mm_setzero_si128();
                        __m128i cmp = _mm_cmpeq_epi32(tmp, zero);
                        if (_mm_movemask_epi8(cmp) != 0xFFFF)
                            return false;
                    }
                }
                else if constexpr (sizeof(wchar) == 4) // UTF-32 / Linux
                {
                    const __m128i mask = _mm_set1_epi32(0xFFFFFF80);
                    for (; i + 4 <= length(); i += 4) { // 4 wchar_t dans 128 bits
                        __m128i chunk = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ptr_ + i));
                        __m128i tmp = _mm_and_si128(chunk, mask);
                        __m128i zero = _mm_setzero_si128();
                        __m128i cmp = _mm_cmpeq_epi32(tmp, zero);
                        if (_mm_movemask_epi8(cmp) != 0xFFFF)
                            return false;
                    }
                }
            }
#endif
            // Do the rest 1 byte at a time
            const wchar *p = ptr_ + i;
            while (*p != '\0') {
                if (!character::is_pure_ascii(*p)) {
                    return false;
                }
                p++;
            }
            return true;
        }

        /**
         * Checks if this string is equal to another string.
         * @param v Another wstring_view to compare with.
         * @return true if both strings are identical, false otherwise.
         */

        [[nodiscard]]
        constexpr bool equals(const wstring_view &v) const noexcept
        {
            return hud::cstring::equals(ptr_, v.ptr_);
        }

        /**
         * Checks if the first n characters of this string match those of another string.
         * @param v Another wstring_view to compare with.
         * @param n Number of characters to compare.
         * @return true if the first n characters match, false otherwise.
         */
        [[nodiscard]]
        constexpr bool equals_partial(const wstring_view &v, const usize n) const noexcept
        {
            return hud::cstring::equals_partial(ptr_, v.ptr_, n);
        }

        /**
         * Finds the first occurrence of a substring given as a wide C-style string.
         * @param to_find_str Pointer to a null-terminated wide string to search for.
         * @return Index of the first occurrence, or -1 if not found.
         */
        [[nodiscard]]
        constexpr isize find_first(const wchar *to_find_str) const noexcept
        {
            const wchar *result = hud::cstring::find_string(ptr_, to_find_str);
            return result == nullptr ? -1 : result - ptr_;
        }

        /**
         * Finds the first occurrence of a substring given as another wstring_view.
         * @param to_find Substring to search for.
         * @return Index of the first occurrence, or -1 if not found.
         */
        template<typename uchar_t>
        [[nodiscard]]
        constexpr isize find_first(const wstring_view<uchar_t> &to_find) const noexcept
        {
            return find_first(to_find.data());
        }

        /**
         * Finds the first occurrence of a character.
         * @param character_to_find The character to search for.
         * @return Index of the first occurrence, or -1 if not found.
         */
        [[nodiscard]]
        constexpr isize find_first_character(const wchar character_to_find) const noexcept
        {
            const wchar *result = hud::cstring::find_character(ptr_, character_to_find);
            return result == nullptr ? -1 : result - ptr_;
        }

        /**
         * Checks if this string contains a given substring (C-style wide string).
         * @param to_find_str Substring to search for.
         * @return true if the substring is found, false otherwise.
         */
        [[nodiscard]]
        constexpr bool contains(const wchar *to_find_str) const noexcept
        {
            return hud::cstring::find_string(ptr_, to_find_str) != nullptr;
        }

        /**
         * Checks if this string contains a given substring (wstring_view).
         * @param to_find Substring to search for.
         * @return true if the substring is found, false otherwise.
         */
        template<typename uchar_t>
        [[nodiscard]]
        constexpr bool contains(const wstring_view<uchar_t> &to_find) const noexcept
        {
            return contains(to_find.data());
        }

        /**
         * Checks if this string contains a given character.
         * @param character_to_find The character to search for.
         * @return true if the character is found, false otherwise.
         */
        [[nodiscard]]
        constexpr bool contains(const wchar character_to_find) const noexcept
        {
            return hud::cstring::find_character(ptr_, character_to_find) != nullptr;
        }

        /**
         * Convert string to uppercase.
         * @param string The string buffer to capitalize
         * @return string pointer
         */
        constexpr void to_uppercase() noexcept
        {
            hud::cstring::to_uppercase(ptr_);
        }

        /**
         * Convert string to uppercase.
         * @param string The string buffer to capitalize
         * @param count Number of character to capitalize
         * @return string pointer
         */
        constexpr void to_uppercase_partial(usize count) noexcept
        {
            hud::cstring::to_uppercase_partial(ptr_, count);
        }

        /**
         * Convert string to lowercase.
         * @param string The string buffer to minimize
         * @return string pointer
         */
        constexpr void to_lowercase() noexcept
        {
            hud::cstring::to_lowercase(ptr_);
        }

        /**
         * Convert string to lowercase.
         * @param string The string buffer to minimize
         * @return string pointer
         */
        constexpr void to_lowercase_partial(usize count) noexcept
        {
            hud::cstring::to_lowercase_partial(ptr_, count);
        }

        /**
         * Returns a slice view of the string.
         * @return A slice representing all characters of this string.
         */
        [[nodiscard]]
        constexpr const hud::slice<char_type> as_slice() const noexcept
        {
            return hud::slice<char_type> {ptr_, length()};
        }

        /**
         * Returns a slice view of the string.
         * @return A slice representing all characters of this string.
         */
        [[nodiscard]]
        constexpr hud::slice<char_type> as_slice() noexcept
        {
            return hud::slice<char_type> {ptr_, length()};
        }

        /**
         * Provides read-only access to a character at a given index.
         * @param i Index of the character to access.
         * @return A const reference to the character at position i.
         */
        [[nodiscard]]
        constexpr char_type &operator[](const usize i) const noexcept
        {
            return ptr_[i];
        }

    private:
        /** Pointer to the null-terminated C-style wide string. */
        char_type *ptr_;
    };

} // namespace hud

#endif // HD_INC_CORE_STRING_CWSTRING_VIEW_H