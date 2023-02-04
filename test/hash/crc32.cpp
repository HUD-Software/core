#include <core/hash/crc32.h>
#include <slice_by_8/crc.h> // Intel Slice-by-8

#include <core/cstring.h>

static constexpr const ansichar *txt = "abcdefghijklmnopqrstuvwxyz";
static constexpr const ansichar *txt_unaligned = " abcdefghijklmnopqrstuvwxyz";

GTEST_TEST(hash, crc32_lookup_table_is_correct)
{
    hud_assert_true(hud::hash_algorithm::crc32::is_lookup_table_values_correct());
}

GTEST_TEST(hash, crc32_hash_aligned_buffer)
{

    const u32 result = hud::hash_algorithm::crc32::hash((const u8 *)(txt), hud::cstring::length(txt), 0);
    hud_assert_eq(result, 0x4C2750BDu);
}

GTEST_TEST(hash, crc32_hash_unaligned)
{
    const u32 result2 = hud::hash_algorithm::crc32::hash((const u8 *)(txt_unaligned + 1), hud::cstring::length(txt_unaligned + 1), 0);
    hud_assert_eq(result2, 0x4C2750BDu);
}

GTEST_TEST(hash, crc32_result_same_as_intel_implementation)
{
    u32 runningcrc = ~0u;
    hud_assert_eq(hud::hash_algorithm::crc32::hash((const u8 *)(txt), hud::cstring::length(txt), 0), ~crc32_sb8_64_bit(&runningcrc, (const u8 *)(txt), static_cast<uint32_t>(hud::cstring::length(txt)), 0, MODE_CONT));

    hud_assert_eq(hud::hash_algorithm::crc32::hash((const u8 *)(txt_unaligned + 1), hud::cstring::length(txt_unaligned + 1), 0), ~crc32_sb8_64_bit(&runningcrc, (const u8 *)(txt_unaligned + 1), static_cast<uint32_t>(hud::cstring::length(txt_unaligned + 1)), 0, MODE_CONT));
}
