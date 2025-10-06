#ifndef HD_INC_CORE_STRING_ENCODING_UTF8_H
#define HD_INC_CORE_STRING_ENCODING_UTF8_H
#include "../../slice.h"
#include "../../traits/is_same.h"
#include "../../traits/remove_cv.h"

namespace hud::encoding
{
    /**
     * Validates whether a given byte sequence is well-formed UTF-8 according to the Unicode specification.
     *
     * This function checks each sequence of bytes (ASCII, 2-, 3-, or 4-byte sequences) and ensures
     * the following rules are respected:
     * - ASCII bytes (< 0x80) are accepted directly.
     * - Multi-byte sequences must follow the correct pattern (10xxxxxx after a valid leading byte).
     * - Overlong encodings are rejected.
     * - Disallowed values (such as surrogates [U+D800, U+DFFF]) are rejected.
     * - Code points above U+10FFFF are rejected.
     *
     * An optimization is applied to quickly skip blocks of 16 consecutive ASCII bytes in a single operation.
     *
     * @tparam char_t Expected character type (must be `char8` or equivalent).
     * @param string UTF-8 byte sequence to validate.
     * @return true if the input is valid UTF-8, false otherwise.
     */
    template<typename char_t>
    requires(sizeof(char_t) == 1)
    [[nodiscard]] static constexpr bool is_valid_utf8_portable(const hud::slice<char_t> string) noexcept
    {
        usize pos = 0;
        u32 code_point = 0;
        usize byte_count = string.byte_count();
        const char8 *str = string.data();

        while (pos < byte_count) {
            // Optimization step:
            // If the next 16 bytes are guaranteed to be ASCII (all < 128),
            // we can skip them all at once instead of checking byte by byte.
            usize next_pos = pos + 16;
            if (next_pos <= byte_count) {                                        // Make sure we don't read past the buffer
                u64 v1 = hud::memory::unaligned_load64(str + pos);               // load first 8 bytes
                u64 v2 = hud::memory::unaligned_load64(str + pos + sizeof(u64)); // load next 8 bytes
                // Bitwise OR combines both 8-byte blocks so we only need a single mask test below.
                // If any byte in v1 or v2 has its high bit set (>= 0x80, non-ASCII),
                // the result will also have that bit set. This lets us quickly check
                // if all 16 bytes are ASCII with one comparison instead of two.
                u64 v {v1 | v2};
                if ((v & 0x8080808080808080) == 0) {
                    pos = next_pos; // all 16 bytes are ASCII → skip them at once
                    continue;
                }
            }

            // Now process byte by byte
            unsigned char byte = str[pos];

            // Consume consecutive ASCII bytes.
            // This inner loop skips multiple ASCII chars in a row efficiently.
            while ((byte & 0x80) == 0) {
                if (++pos == byte_count) {
                    return true;
                }
                byte = str[pos];
            }

            // Case: 2-byte sequence -> 110xxxxx 10xxxxxx
            // If we catch leading byte 110xxxxx
            if ((byte & 0b11100000) == 0b11000000) {

                // Jump to next supposed code point (after 110xxxxx 10xxxxxx)
                // If we go too far, then there is no continuous byte 10xxxxxx
                next_pos = pos + 2;
                if (next_pos > byte_count) {
                    return false;
                }
                // Ensure 1st continuous byte is 10xxxxxx
                if ((str[pos + 1] & 0b11000000) != 0b10000000) {
                    return false;
                }
                // Read the code point
                code_point = (byte & 0b00011111) << 6 | (str[pos + 1] & 0b00111111);
                // Ensure code point is [0x80, 0x7FF] aka [U+0080, U+07FF]
                if ((code_point < 0x80) || (0x7ff < code_point)) {
                    return false;
                }
            }
            // Case: 3-byte sequence -> 1110xxxx 10xxxxxx 10xxxxxx
            // If we catch leading byte 1110xxxx
            else if ((byte & 0b11110000) == 0b11100000) {

                // Jump to next supposed code point (after 1110xxxx 10xxxxxx 10xxxxxx)
                // If we go too far, then there is no continuous bytes 10xxxxxx 10xxxxxx
                next_pos = pos + 3;
                if (next_pos > byte_count) {
                    return false;
                }
                // Ensure 1st continuous byte is 10xxxxxx
                if ((str[pos + 1] & 0b11000000) != 0b10000000) {
                    return false;
                }
                // Ensure 2nd continuous byte is 10xxxxxx
                if ((str[pos + 2] & 0b11000000) != 0b10000000) {
                    return false;
                }
                // Read the code point
                code_point = (byte & 0b00001111) << 12 | (str[pos + 1] & 0b00111111) << 6 | (str[pos + 2] & 0b00111111);
                // Check code point valid value
                // - must not be overlong encoding (< 0x800 is invalid)
                // - must be [0x0800, 0xFFFF] aka [U+0800, U+FFFF]
                // - must not be in surrogate range [0xD800, 0xDFFF] aka [U+D800, U+DFFF]
                if ((code_point < 0x800) || (0xffff < code_point) || (0xd7ff < code_point && code_point < 0xe000)) {
                    return false;
                }
            }
            // Case: 4-byte sequence -> 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
            // If we catch leading byte 11110xxx
            else if ((byte & 0b11111000) == 0b11110000) {
                // Jump to next supposed code point (after 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx)
                // If we go too far, then there is no continuous bytes 10xxxxxx 10xxxxxx 10xxxxxx
                next_pos = pos + 4;
                if (next_pos > byte_count) {
                    return false;
                }
                // Ensure 1st continuous byte is 10xxxxxx
                if ((string[pos + 1] & 0b11000000) != 0b10000000) {
                    return false;
                }
                // Ensure 2nd continuous byte is 10xxxxxx
                if ((string[pos + 2] & 0b11000000) != 0b10000000) {
                    return false;
                }
                // Ensure 3rd continuous byte is 10xxxxxx
                if ((string[pos + 3] & 0b11000000) != 0b10000000) {
                    return false;
                }
                // Read the code point
                code_point = (byte & 0b00000111) << 18 | (string[pos + 1] & 0b00111111) << 12 | (string[pos + 2] & 0b00111111) << 6 | (string[pos + 3] & 0b00111111);
                // Check code point valid value
                // - must be > 0xFFFF (otherwise it's overlong)
                // - must not exceed Unicode max (0x10FFFF)
                if (code_point <= 0xffff || 0x10ffff < code_point) {
                    return false;
                }
            }
            else {
                // Any other pattern is invalid:
                // e.g. a continuation byte without a proper leading byte
                return false;
            }
            // Move to the next character after validating the current one
            pos = next_pos;
        }
        return true;
    }

    /**
     * Validates whether a given byte sequence is well-formed UTF-8 according to the Unicode specification.
     *
     * This function checks each sequence of bytes (ASCII, 2-, 3-, or 4-byte sequences) and ensures
     * the following rules are respected:
     * - ASCII bytes (< 0x80) are accepted directly.
     * - Multi-byte sequences must follow the correct pattern (10xxxxxx after a valid leading byte).
     * - Overlong encodings are rejected.
     * - Disallowed values (such as surrogates [U+D800, U+DFFF]) are rejected.
     * - Code points above U+10FFFF are rejected.
     *
     * An optimization is applied to quickly skip blocks of 16 consecutive ASCII bytes in a single operation.
     *
     * @tparam char_t Expected character type (must be `char8` or equivalent).
     * @param string UTF-8 byte sequence to validate.
     * @return true if the input is valid UTF-8, false otherwise.
     */
    template<typename char_t>
    requires(sizeof(char_t) == 1)
    [[nodiscard]] static constexpr bool is_valid_utf8(const hud::slice<char_t> string) noexcept
    {
        return is_valid_utf8_portable(string);
    }

} // namespace hud::encoding

#endif // HD_INC_CORE_STRING_ENCODING_UTF8_H