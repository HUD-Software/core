#ifndef HD_INC_CORE_STRING_CSTRING_VIEW_H
#define HD_INC_CORE_STRING_CSTRING_VIEW_H
#include "cstring.h"
#include "../slice.h"
#include "encoding/utf8.h"
#include "encoding/utf16.h"
#include "encoding/utf32.h"
#include "encoding/ascii.h"

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
            return hud::encoding::is_valid_ascii(as_slice());
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
         * Provides read-only access to the code unit at a given index.
         * @param i Index of the code unit to access.
         * @return A const reference to the code unit at position i.
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
            return sizeof(char_type) == 1 ? hud::encoding::is_valid_utf8(as_slice()) : false;
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
            return sizeof(char_type) == 2 ? hud::encoding::is_valid_utf16(as_slice()) : false;
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
            return sizeof(char_type) == 4 ? hud::encoding::is_valid_utf32(as_slice()) : false;
        }

    private:
        /**Pointer to the null-terminated C-style string. */
        char_type *ptr_;
    };

} // namespace hud

#endif // HD_INC_CORE_STRING_CSTRING_VIEW_H