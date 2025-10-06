#ifndef HD_INC_CORE_STRING_UNICODE_UTF32_H
#define HD_INC_CORE_STRING_UNICODE_UTF32_H
#include "../../slice.h"

namespace hud::unicode
{
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
     * @tparam char_t Expected character type (must be 4 bytes, e.g., char32).
     * @param string UTF-32 code point sequence to validate.
     * @return true if the input is valid UTF-32, false otherwise.
     */
    template<typename char_t>
    requires(sizeof(char_t) == 4)
    [[nodiscard]] static constexpr bool is_valid_utf32_portable(const hud::slice<char_t> string) noexcept
    {
        const u32 *ptr = reinterpret_cast<const u32 *>(string.data());
        usize len = string.count();
        usize pos = 0;
        for (; pos < len; pos++) {
            uint32_t word = ptr[pos];
            if (word > 0x10FFFF || (word >= 0xD800 && word <= 0xDFFF)) {
                return false;
            }
        }
        return true;
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
     * @tparam char_t Expected character type (must be 4 bytes, e.g., char32).
     * @param string UTF-32 code point sequence to validate.
     * @return true if the input is valid UTF-32, false otherwise.
     */
    template<typename char_t>
    requires(sizeof(char_t) == 4)
    [[nodiscard]] static constexpr bool is_valid_utf32(const hud::slice<char_t> string) noexcept
    {
        return is_valid_utf16_portable(string);
    }
} // namespace hud::unicode

#endif // HD_INC_CORE_STRING_UNICODE_UTF32_H