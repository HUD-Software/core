#ifndef HD_INC_CORE_STRING_ENCODING_UTF16_H
#define HD_INC_CORE_STRING_ENCODING_UTF16_H
#include "../../slice.h"

namespace hud::encoding
{
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
     * @tparam char_t Expected character type (must be 2 bytes, e.g., char16).
     * @param string UTF-16 code unit sequence to validate.
     * @return true if the input is valid UTF-16, false otherwise.
     */
    template<typename char_t>
    requires(sizeof(char_t) == 2)
    [[nodiscard]] static constexpr bool is_valid_utf16_portable(const hud::slice<char_t> string) noexcept
    {
        usize pos = 0;
        usize len = string.count();
        char_t *ptr = string.begin_ptr();
        while (pos < len) {
            char16 word = ptr[pos];
            if ((word & 0xF800) == 0xD800) {
                if (pos + 1 >= len) {
                    return false;
                }
                char16 diff = char16(word - 0xD800);
                if (diff > 0x3FF) {
                    return false;
                }
                char16 next_word = ptr[pos + 1];
                char16 diff2 = char16(next_word - 0xDC00);
                if (diff2 > 0x3FF) {
                    return false;
                }
                pos += 2;
            }
            else {
                pos++;
            }
        }
        return true;
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
     * @tparam char_t Expected character type (must be 2 bytes, e.g., char16).
     * @param string UTF-16 code unit sequence to validate.
     * @return true if the input is valid UTF-16, false otherwise.
     */
    template<typename char_t>
    requires(sizeof(char_t) == 2)
    [[nodiscard]] static constexpr bool is_valid_utf16(const hud::slice<char_t> string) noexcept
    {
        return is_valid_utf16_portable(string);
    }
} // namespace hud::encoding

#endif // HD_INC_CORE_STRING_ENCODING_UTF16_H