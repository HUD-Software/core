#include <core/string/cstring.h>
#include <core/memory.h>
namespace hud
{
    // [[nodiscard]] constexpr bool cstring::is_valid_utf8(const char8 *string, usize byte_count) noexcept
    // {
    //     u64 pos = 0;
    //     u32 code_point = 0;
    //     while (pos < byte_count) {
    //         // check of the next 16 bytes are ascii.
    //         u64 next_pos = pos + 16;
    //         if (next_pos <= byte_count) { // if it is safe to read 16 more bytes, check that they are ascii
    //             u64 v1 = hud::memory::unaligned_load64(string + pos);
    //             // std::memcpy(&v1, string + pos, sizeof(u64));
    //             u64 v2 = hud::memory::unaligned_load64(string + pos + sizeof(u64));
    //             // std::memcpy(&v2, string + pos + sizeof(u64), sizeof(u64));
    //             u64 v {v1 | v2};
    //             if ((v & 0x8080808080808080) == 0) {
    //                 pos = next_pos;
    //                 continue;
    //             }
    //         }
    //         unsigned char byte = string[pos];

    // while (byte < 0b10000000) {
    //     if (++pos == byte_count) {
    //         return true;
    //     }
    //     byte = string[pos];
    // }

    // if ((byte & 0b11100000) == 0b11000000) {
    //     next_pos = pos + 2;
    //     if (next_pos > byte_count) {
    //         return false;
    //     }
    //     if ((string[pos + 1] & 0b11000000) != 0b10000000) {
    //         return false;
    //     }
    //     // range check
    //     code_point = (byte & 0b00011111) << 6 | (string[pos + 1] & 0b00111111);
    //     if ((code_point < 0x80) || (0x7ff < code_point)) {
    //         return false;
    //     }
    // }
    // else if ((byte & 0b11110000) == 0b11100000) {
    //     next_pos = pos + 3;
    //     if (next_pos > byte_count) {
    //         return false;
    //     }
    //     if ((string[pos + 1] & 0b11000000) != 0b10000000) {
    //         return false;
    //     }
    //     if ((string[pos + 2] & 0b11000000) != 0b10000000) {
    //         return false;
    //     }
    //     // range check
    //     code_point = (byte & 0b00001111) << 12 | (string[pos + 1] & 0b00111111) << 6 | (string[pos + 2] & 0b00111111);
    //     if ((code_point < 0x800) || (0xffff < code_point) || (0xd7ff < code_point && code_point < 0xe000)) {
    //         return false;
    //     }
    // }
    // else if ((byte & 0b11111000) == 0b11110000) { // 0b11110000
    //     next_pos = pos + 4;
    //     if (next_pos > byte_count) {
    //         return false;
    //     }
    //     if ((string[pos + 1] & 0b11000000) != 0b10000000) {
    //         return false;
    //     }
    //     if ((string[pos + 2] & 0b11000000) != 0b10000000) {
    //         return false;
    //     }
    //     if ((string[pos + 3] & 0b11000000) != 0b10000000) {
    //         return false;
    //     }
    //     // range check
    //     code_point =
    //         (byte & 0b00000111) << 18 | (string[pos + 1] & 0b00111111) << 12 | (string[pos + 2] & 0b00111111) << 6 | (string[pos + 3] & 0b00111111);
    //     if (code_point <= 0xffff || 0x10ffff < code_point) {
    //         return false;
    //     }
    // }
    // else {
    //     // we may have a continuation
    //     return false;
    // }
    // pos = next_pos;
    // }
    // return true;
    // }
} // namespace hud
