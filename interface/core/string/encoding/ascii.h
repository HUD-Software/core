#ifndef HD_INC_CORE_STRING_ENCODING_ASCII_H
#define HD_INC_CORE_STRING_ENCODING_ASCII_H
#include "../../slice.h"

namespace hud::encoding
{
    /**
     * Checks if all characters in the string are ASCII characters.
     * @param string slice of the string to check.
     * @return true if the string contains only ASCII characters, false otherwise.
     */
    template<typename char_t>
    requires(sizeof(char_t) == 1)
    [[nodiscard]] static constexpr bool is_valid_ascii(const hud::slice<char_t> string) noexcept
    {
        usize i {0};
        usize len = string.count();
        char_t *ptr = string.data();
// In non constexpr context and if HD_SSE2 is on, we do the check with SSE2
#if HD_SSE2
        if consteval {
            // Constexpr context: fallback to scalar check
        }
        else {
            // 16 bytes mask of 0x80
            const __m128i mask = _mm_set1_epi8(char(0x80));
            // For each 16 bytes block in ptr
            for (; i + 16 <= len; i += 16) {
                // Load the 16 bytes block
                __m128i chunk = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ptr + i));
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
        const char_t *p = ptr + i;
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
    template<typename char_t>
    requires(sizeof(char_t) == 2)
    [[nodiscard]]
    constexpr bool is_valid_ascii(const hud::slice<char_t> string) noexcept
    {
        usize i = 0;
        usize len = string.count();
        char_t *ptr = string.data();
#if HD_SSE2
        if consteval {
            // Constexpr context: fallback to scalar check
        }
        else {
            // 16 bytes mask of 0xFF80
            const __m128i mask = _mm_set1_epi16(short(0xFF80));
            // For each 8 bytes block in ptr ( 8 char_t can fit in __m128i if sizeof(char_t) == 2 )
            for (; i + 8 <= len; i += 8) {
                // Load the 16 bytes block
                __m128i chunk = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ptr + i));
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
            if (ptr[i] > 0x7F) // ASCII = 0..127
                return false;
        }
        return true;
    }

    template<typename char_t>
    requires(sizeof(char_t) == 4)
    [[nodiscard]]
    constexpr bool is_valid_ascii(const hud::slice<char_t> string) noexcept
    {
        usize i = 0;
        usize len = string.count();
        char_t *ptr = string.data();
#if HD_SSE2
        if consteval {
            // Constexpr context: fallback to scalar check
        }
        else {
            // 16 bytes mask of 0xFFFFFF80
            const __m128i mask = _mm_set1_epi32(0xFFFFFF80);
            // For each 4 bytes block in ptr ( 4 char_t can fit in __m128i if sizeof(char_t) == 4 )
            for (; i + 4 <= len; i += 4) {
                // Load the 16 bytes block
                __m128i chunk = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ptr + i));
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
            if (ptr[i] > 0x7F) // ASCII = 0..127
                return false;
        }
        return true;
    }

} // namespace hud::encoding

#endif // HD_INC_CORE_STRING_ENCODING_ASCII_H