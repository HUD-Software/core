#include <core/hash.h>

#include <core/cstring.h>

// #include <core/templates/bit_cast.h>

GTEST_TEST(hash, hash_can_hash_integral)
{
    // Consider compiler just truncate value that are bigger than u32
    hud_assert_eq(hud::hash(hud::i8_max), static_cast<u32>(hud::i8_max));
    hud_assert_eq(hud::hash(hud::u8_max), static_cast<u32>(hud::u8_max));

    hud_assert_eq(hud::hash(hud::i16_max), static_cast<u32>(hud::i16_max));
    hud_assert_eq(hud::hash(hud::u16_max), static_cast<u32>(hud::u16_max));

    hud_assert_eq(hud::hash(hud::i32_max), static_cast<u32>(hud::i32_max));
    hud_assert_eq(hud::hash(hud::u32_max), static_cast<u32>(hud::u32_max));

    hud_assert_eq(hud::hash(hud::i64_max), 0x7FFFFFE8u);
    hud_assert_eq(hud::hash(hud::u64_max), 0xFFFFFFE8u);
}

GTEST_TEST(hash, hash_of_integral_are_usable_in_constexpr)
{
    constexpr u32 hash_vi8 = hud::hash(hud::i8_max);
    hud_assert_eq(hash_vi8, static_cast<u32>(hud::i8_max));

    constexpr u32 hash_vu8 = hud::hash(hud::u8_max);
    hud_assert_eq(hash_vu8, static_cast<u32>(hud::u8_max));

    constexpr u32 hash_vi16 = hud::hash(hud::i16_max);
    hud_assert_eq(hash_vi16, static_cast<u32>(hud::i16_max));

    constexpr u32 hash_vu16 = hud::hash(hud::u16_max);
    hud_assert_eq(hash_vu16, static_cast<u32>(hud::u16_max));

    constexpr u32 hash_vi32 = hud::hash(hud::i32_max);
    hud_assert_eq(hash_vi32, static_cast<u32>(hud::i32_max));

    constexpr u32 hash_vu32 = hud::hash(hud::u32_max);
    hud_assert_eq(hash_vu32, static_cast<u32>(hud::u32_max));

    constexpr u32 hash_vi64 = hud::hash(hud::i64_max);
    hud_assert_eq(hash_vi64, 0x7FFFFFE8u);

    constexpr u32 hash_vu64 = hud::hash(hud::u64_max);
    hud_assert_eq(hash_vu64, 0xFFFFFFE8u);
}

GTEST_TEST(hash, hash_can_hash_floating_point)
{
    f32 flt32 = 12345.6789f;
    hud_assert_eq(hud::hash(flt32), static_cast<u32>(0x4640e6b7));
    f64 flt64 = 12345.6789;
    hud_assert_eq(hud::hash(flt64), static_cast<u32>(0xb82c8fdb));
}

GTEST_TEST(hash, hash_of_floating_point_are_usable_in_constexpr)
{
    constexpr f32 flt32 = 12345.6789f;
    constexpr u32 flt32_hash = hud::bit_cast<u32>(flt32);
    hud_assert_eq(flt32_hash, static_cast<u32>(0x4640e6b7));

    constexpr f64 flt64 = 12345.6789;
    constexpr u32 flt64_hash = hud::hash(hud::bit_cast<u64>(flt64));
    hud_assert_eq(flt64_hash, static_cast<u32>(0xb82c8fdb));
}

GTEST_TEST(hash, hash_can_hash_c_string)
{
    static constexpr const ansichar txt[] = "abcdefghijklmnopqrstuvwxyz";
    hud_assert_eq(hud::hash(txt, hud::cstring::length(txt)), 0xaa02c5c1);

    static constexpr const wchar *wtxt = L"abcdefghijklmnopqrstuvwxyz";
    if constexpr (sizeof(wchar) == 2)
    {
        hud_assert_eq(hud::hash(wtxt, hud::cstring::length(wtxt)), 0x891d95cf);
    }
    else if constexpr (sizeof(wchar) == 4)
    {
        hud_assert_eq(hud::hash(wtxt, hud::cstring::length(wtxt)), 0x71002A00);
    }
    else
    {
        FAIL();
    }
}

GTEST_TEST(hash, hash_can_hash_enumeration)
{
    enum class E : u32
    {
        a = hud::u32_max,
        b = 'a'
    };
    enum class E2 : u64
    {
        a = hud::u64_max,
        b = 'a'
    };

    hud_assert_eq(hud::hash(E::a), hud::hash(hud::u32_max));
    hud_assert_eq(hud::hash(E::b), hud::hash(static_cast<u32>('a')));
    hud_assert_eq(hud::hash(E2::a), hud::hash(hud::u64_max));
    hud_assert_eq(hud::hash(E2::b), hud::hash(static_cast<u64>('a')));
}

GTEST_TEST(hash, hash_enumeration_are_usable_in_constexpr)
{
    enum class E : u32
    {
        a = hud::u32_max,
        b = 'a'
    };
    enum class E2 : u64
    {
        a = hud::u64_max,
        b = 'a'
    };

    constexpr u32 hash_e1_a = hud::hash(E::a);
    hud_assert_eq(hash_e1_a, hud::hash(hud::u32_max));
    constexpr u32 hash_e1_b = hud::hash(E::b);
    hud_assert_eq(hash_e1_b, hud::hash(static_cast<u32>('a')));
    constexpr u32 hash_e2_a = hud::hash(E2::a);
    hud_assert_eq(hash_e2_a, hud::hash(hud::u64_max));
    constexpr u32 hash_e2_ab = hud::hash(E2::b);
    hud_assert_eq(hash_e2_ab, hud::hash(static_cast<u64>('a')));
}

GTEST_TEST(hash, hash_can_hash_pointers)
{
    const u32 *ptr = nullptr;
    hud_assert_eq(hud::hash(ptr), reinterpret_cast<uptr>(ptr));
}
