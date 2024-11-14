#include <core/hash/crc32.h>
#include <slice_by_8/crc.h> // Intel Slice-by-8
#include <core/cstring.h>

static constexpr const ansichar *txt = "abcdefghijklmnopqrstuvwxyz";
static constexpr const ansichar *txt_unaligned = " abcdefghijklmnopqrstuvwxyz";
static constexpr const ansichar *txt_test = "123456789";

GTEST_TEST(hash, crc32_lookup_table_is_correct)
{
    // Generate the Crc32 Lookup table with the correct polynomial
    u32 generated_crc32_lookup[8][256] = {};

    for (unsigned int i = 0; i <= 0xFF; i++)
    {
        u32 crc = i;
        for (unsigned int j = 0; j < 8; j++)
        {
            crc = (crc >> 1) ^ ((crc & 1) * hud::hash_algorithm::crc32::REFLECTED_POLYNOMIAL);
        }
        generated_crc32_lookup[0][i] = crc;
    }

    for (unsigned int i = 0; i <= 0xFF; i++)
    {
        generated_crc32_lookup[1][i] = (generated_crc32_lookup[0][i] >> 8) ^ generated_crc32_lookup[0][generated_crc32_lookup[0][i] & 0xFF];
        generated_crc32_lookup[2][i] = (generated_crc32_lookup[1][i] >> 8) ^ generated_crc32_lookup[0][generated_crc32_lookup[1][i] & 0xFF];
        generated_crc32_lookup[3][i] = (generated_crc32_lookup[2][i] >> 8) ^ generated_crc32_lookup[0][generated_crc32_lookup[2][i] & 0xFF];
        generated_crc32_lookup[4][i] = (generated_crc32_lookup[3][i] >> 8) ^ generated_crc32_lookup[0][generated_crc32_lookup[3][i] & 0xFF];
        generated_crc32_lookup[5][i] = (generated_crc32_lookup[4][i] >> 8) ^ generated_crc32_lookup[0][generated_crc32_lookup[4][i] & 0xFF];
        generated_crc32_lookup[6][i] = (generated_crc32_lookup[5][i] >> 8) ^ generated_crc32_lookup[0][generated_crc32_lookup[5][i] & 0xFF];
        generated_crc32_lookup[7][i] = (generated_crc32_lookup[6][i] >> 8) ^ generated_crc32_lookup[0][generated_crc32_lookup[6][i] & 0xFF];
    }

    // Assert that all values in the lookup table are corrects
    for (u32 i = 0; i < 8; i++)
    {
        for (u32 j = 0; j < 256; j++)
        {
            hud::check(generated_crc32_lookup[i][j] == hud::hash_algorithm::crc32::CRC32_LOOKUP[i][j]);
        }
    }
    // Compare with the crc32 lookup table
    hud_assert_true(hud::memory::compare_equal(hud::hash_algorithm::crc32::CRC32_LOOKUP, generated_crc32_lookup));
}

GTEST_TEST(hash, crc32_hash_aligned_buffer)
{
    const u32 result = hud::hash_algorithm::crc32::hash((const u8 *)(txt), hud::cstring::length(txt), 0);
    hud_assert_eq(result, 0x4C2750BDu);

    const u32 result_2 = hud::hash_algorithm::crc32::hash((const u8 *)(txt), 0, 0);
    hud_assert_eq(result_2, 0x0u);
}

GTEST_TEST(hash, crc32_hash_unaligned)
{
    const u32 result = hud::hash_algorithm::crc32::hash((const u8 *)(txt_unaligned + 1), hud::cstring::length(txt_unaligned + 1), 0);
    hud_assert_eq(result, 0x4C2750BDu);

    const u32 result_2 = hud::hash_algorithm::crc32::hash((const u8 *)(txt_unaligned + 1), 0, 0);
    hud_assert_eq(result_2, 0x0);
}

GTEST_TEST(hash, crc32_result_same_as_intel_implementation)
{
    u32 runningcrc = ~0u;
    hud_assert_eq(hud::hash_algorithm::crc32::hash((const u8 *)(txt), hud::cstring::length(txt), 0), ~crc32_sb8_64_bit(&runningcrc, (const u8 *)(txt), static_cast<u32>(hud::cstring::length(txt)), 0, MODE_CONT));
    hud_assert_eq(hud::hash_algorithm::crc32::hash((const u8 *)(txt_unaligned + 1), hud::cstring::length(txt_unaligned + 1), 0), ~crc32_sb8_64_bit(&runningcrc, (const u8 *)(txt_unaligned + 1), static_cast<u32>(hud::cstring::length(txt_unaligned + 1)), 0, MODE_CONT));
}

// From zlib crc32.c
static u32 multmodp(u32 a, u32 b)
{
    u32 m, p;

    m = (u32)1 << 31;
    p = 0;
    for (;;)
    {
        if (a & m)
        {
            p ^= b;
            if ((a & (m - 1)) == 0)
                break;
        }
        m >>= 1;
        b = b & 1 ? (b >> 1) ^ hud::hash_algorithm::crc32::REFLECTED_POLYNOMIAL : b >> 1;
    }
    return p;
}

GTEST_TEST(hash, crc32_lookup_x2ntable_is_correct)
{
    // Generate the x2n_table
    u32 p = (u32)1 << 30; /* x^1 */

    u32 x2n_table[32];
    x2n_table[0] = p;
    for (u32 n = 1; n < 32; n++)
    {
        x2n_table[n] = p = multmodp(p, p);
    }
    // Compare with the crc32 x2n_table
    hud_assert_true(hud::memory::compare_equal(hud::hash_algorithm::crc32::X2N_TABLE, x2n_table));
}

GTEST_TEST(hash, 123456789)
{
    // From https://github.com/Michaelangel007/crc32
    const u32 result = hud::hash_algorithm::crc32::hash((const u8 *)(txt_test), hud::cstring::length(txt_test), 0);
    hud_assert_eq(result, 0xCBF43926);
}

GTEST_TEST(hash, combine)
{
    const u32 result_1 = hud::hash_algorithm::crc32::hash((const u8 *)(txt), hud::cstring::length(txt), 0);
    const u32 result_2 = hud::hash_algorithm::crc32::hash((const u8 *)(txt_test), hud::cstring::length(txt_test), 0);
    const u32 result = hud::hash_algorithm::crc32::combine(result_1, result_2, hud::cstring::length(txt_test));
    hud_assert_eq(result, 0x5579202Fu);
}