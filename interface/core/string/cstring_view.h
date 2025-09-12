#ifndef HD_INC_CORE_STRING_CSTRING_VIEW_H
#define HD_INC_CORE_STRING_CSTRING_VIEW_H
#include "cstring.h"

namespace hud
{
    /**
     * A view of a C-compatible, null-terminated string with no null-terminated bytes in the middle.
     */
    struct cstring_view
    {
        constexpr cstring_view(const ansichar *str) noexcept
            : ptr_(str)
            , len_(cstring::length(str))
        {
            HUD_CHECK(ptr_ != nullptr && "Invalid null pointer");
        }

        constexpr cstring_view(const ansichar *str, usize length) noexcept
            : ptr_(str)
            , len_(length)
        {
            HUD_CHECK(ptr_ != nullptr && "Invalid null pointer");
            HUD_CHECK(cstring::length_safe(str, cstring::RSIZE_MAX_STR) == length && "length must match string length");
        }

        [[nodiscard]]
        constexpr usize length() const noexcept
        {
            return len_;
        }

        [[nodiscard]]
        constexpr const ansichar *data() const noexcept
        {
            return ptr_;
        }

        [[nodiscard]]
        constexpr bool is_ascii() const noexcept
        {
            usize i {0};
// In non constexpr context and if HD_SSE2 is on, we do the check with SSE2
#if HD_SSE2
            if consteval {
            }
            else {
                // Mask 16 bytes of 0x80 with the ansichars
                const __m128i mask = _mm_set1_epi8(i8(0x80));
                for (; i + 16 <= len_; i += 16) {
                    __m128i chunk = _mm_loadu_si128(reinterpret_cast<const __m128i *>(ptr_ + i));
                    if (_mm_movemask_epi8(_mm_and_si128(chunk, mask)) != 0)
                        return false;
                }
            }
#endif
            // Do the rest 1 byte at a time
            for (; i < len_; ++i) {
                if (ptr_[i] & 0x80) {
                    return false;
                }
            }

            return true;
        }

    private:
        const ansichar *ptr_;

        usize len_;
    };

} // namespace hud

#endif // HD_INC_CORE_STRING_CSTRING_VIEW_H