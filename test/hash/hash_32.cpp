
#include <core/cstring.h>
#include <core/hash.h>

namespace hud_test
{
    struct custom
    {
        f32 value_ = 12345.6789f;
    };
} // namespace hud_test

namespace hud
{
    template<>
    struct hash_32<hud_test::custom>
    {
        [[nodiscard]] constexpr u32 operator()(const hud_test::custom &custom) const
        {
            return hash_32<f32> {}(custom.value_);
        }
    };
} // namespace hud

GTEST_TEST(hash_32, hash_can_hash_integral)
{
    // Consider compiler just truncate value that are bigger than u32
    hud_assert_eq(hud::hash_32<i8> {}(hud::i8_max), static_cast<u32>(hud::i8_max));
    hud_assert_eq(hud::hash_32<u8> {}(hud::u8_max), static_cast<u32>(hud::u8_max));

    hud_assert_eq(hud::hash_32<i16> {}(hud::i16_max), static_cast<u32>(hud::i16_max));
    hud_assert_eq(hud::hash_32<u16> {}(hud::u16_max), static_cast<u32>(hud::u16_max));

    hud_assert_eq(hud::hash_32<i32> {}(hud::i32_max), static_cast<u32>(hud::i32_max));
    hud_assert_eq(hud::hash_32<u32> {}(hud::u32_max), static_cast<u32>(hud::u32_max));

    hud_assert_eq(hud::hash_32<i64> {}(hud::i64_max), 0x7FFFFFE8u);
    hud_assert_eq(hud::hash_32<u64> {}(hud::u64_max), 0xFFFFFFE8u);
}

GTEST_TEST(hash_32, hash_of_integral_are_usable_in_constexpr)
{
    constexpr u32 hash_vi8 = hud::hash_32<i8> {}(hud::i8_max);
    hud_assert_eq(hash_vi8, static_cast<u32>(hud::i8_max));

    constexpr u32 hash_vu8 = hud::hash_32<u8> {}(hud::u8_max);
    hud_assert_eq(hash_vu8, static_cast<u32>(hud::u8_max));

    constexpr u32 hash_vi16 = hud::hash_32<i16> {}(hud::i16_max);
    hud_assert_eq(hash_vi16, static_cast<u32>(hud::i16_max));

    constexpr u32 hash_vu16 = hud::hash_32<u16> {}(hud::u16_max);
    hud_assert_eq(hash_vu16, static_cast<u32>(hud::u16_max));

    constexpr u32 hash_vi32 = hud::hash_32<i32> {}(hud::i32_max);
    hud_assert_eq(hash_vi32, static_cast<u32>(hud::i32_max));

    constexpr u32 hash_vu32 = hud::hash_32<u32> {}(hud::u32_max);
    hud_assert_eq(hash_vu32, static_cast<u32>(hud::u32_max));

    constexpr u32 hash_vi64 = hud::hash_32<i64> {}(hud::i64_max);
    hud_assert_eq(hash_vi64, 0x7FFFFFE8u);

    constexpr u32 hash_vu64 = hud::hash_32<u64> {}(hud::u64_max);
    hud_assert_eq(hash_vu64, 0xFFFFFFE8u);
}

GTEST_TEST(hash_32, hash_can_hash_floating_point)
{
    f32 flt32 = 12345.6789f;
    hud_assert_eq(hud::hash_32<f32> {}(flt32), static_cast<u32>(0x4640e6b7));
    f64 flt64 = 12345.6789;
    hud_assert_eq(hud::hash_32<f64> {}(flt64), static_cast<u32>(0xb82c8fdb));
}

GTEST_TEST(hash_32, hash_of_floating_point_are_usable_in_constexpr)
{
    constexpr f32 flt32 = 12345.6789f;
    constexpr u32 flt32_hash = hud::hash_32<f32> {}(flt32);
    hud_assert_eq(flt32_hash, static_cast<u32>(0x4640e6b7));

    constexpr f64 flt64 = 12345.6789;
    constexpr u32 flt64_hash = hud::hash_32<f64> {}(flt64);
    hud_assert_eq(flt64_hash, static_cast<u32>(0xb82c8fdb));
}

GTEST_TEST(hash_32, hash_can_hash_c_string)
{
    static constexpr const ansichar txt[] = "abcdefghijklmnopqrstuvwxyz";
    hud_assert_eq(hud::hash_32<const ansichar *> {}(txt, hud::cstring::length(txt)), 0xaa02c5c1);

    static constexpr const wchar *wtxt = L"abcdefghijklmnopqrstuvwxyz";
    if constexpr (sizeof(wchar) == 2)
    {
        hud_assert_eq(hud::hash_32<const wchar *> {}(wtxt, hud::cstring::length(wtxt)), 0x891d95cf);
    }
    else if constexpr (sizeof(wchar) == 4)
    {
        hud_assert_eq(hud::hash_32<const wchar *> {}(wtxt, hud::cstring::length(wtxt)), 0x71002A00);
    }
    else
    {
        FAIL();
    }
}

// GTEST_TEST(hash_32, hash_can_hash_enumeration)
// {
//     enum class E : u32
//     {
//         a = hud::u32_max,
//         b = 'a'
//     };
//     enum class E2 : u64
//     {
//         a = hud::u64_max,
//         b = 'a'
//     };

// hud_assert_eq(hud::hash_32 {}(E::a), hud::hash_32 {}(hud::u32_max));
// hud_assert_eq(hud::hash_32 {}(E::b), hud::hash_32 {}(static_cast<u32>('a')));
// hud_assert_eq(hud::hash_32 {}(E2::a), hud::hash_32 {}(hud::u64_max));
// hud_assert_eq(hud::hash_32 {}(E2::b), hud::hash_32 {}(static_cast<u64>('a')));
// }

// GTEST_TEST(hash_32, hash_enumeration_are_usable_in_constexpr)
// {
//     enum class E : u32
//     {
//         a = hud::u32_max,
//         b = 'a'
//     };
//     enum class E2 : u64
//     {
//         a = hud::u64_max,
//         b = 'a'
//     };

// constexpr u32 hash_e1_a = hud::hash_32 {}(E::a);
// hud_assert_eq(hash_e1_a, hud::hash_32 {}(hud::u32_max));
// constexpr u32 hash_e1_b = hud::hash_32 {}(E::b);
// hud_assert_eq(hash_e1_b, hud::hash_32 {}(static_cast<u32>('a')));
// constexpr u32 hash_e2_a = hud::hash_32 {}(E2::a);
// hud_assert_eq(hash_e2_a, hud::hash_32 {}(hud::u64_max));
// constexpr u32 hash_e2_ab = hud::hash_32 {}(E2::b);
// hud_assert_eq(hash_e2_ab, hud::hash_32 {}(static_cast<u64>('a')));
// }

GTEST_TEST(hash_32, hash_can_hash_pointers)
{
    const u32 *ptr = nullptr;
    hud_assert_eq(hud::hash_32<const void *> {}(static_cast<const void *>(ptr)), reinterpret_cast<uptr>(ptr));
}

GTEST_TEST(hash_32, hasher32)
{
    constexpr const usize len = hud::cstring::length("key");
    hud::hasher_32 hasher;
    hud_assert_eq(hasher("key", len).result(), 0x105695BEu);
#if defined(HD_TARGET_32_BITS)
    hud_assert_eq(hasher((const ansichar *)&len, static_cast<usize>(sizeof(len))).result(), 0xE66FB46Bu);
#else // HD_TARGET_64_BITS
    hud_assert_eq(hasher((const ansichar *)&len, static_cast<usize>(sizeof(len))).result(), 0xE638A9DBu);
#endif

    // Test that hasher can be called redundancy
    u32 value = hud::hasher_32 {}("key", len).hash((const ansichar *)&len, static_cast<usize>(sizeof(len))).result();
#if defined(HD_TARGET_32_BITS)
    hud_assert_eq(value, 0xE66FB46Bu);
#else // HD_TARGET_64_BITS
    hud_assert_eq(value, 0xE638A9DBu);
#endif
}

GTEST_TEST(hash_32, hash_custom_type)
{
    // Non constant
    {
        constexpr hud_test::custom custom_32 {12345.6789f};
        hud_assert_eq(hud::hash_32<hud_test::custom> {}(custom_32), 0x4640E6B7u);
    }

    // Constant
    {
        constexpr hud_test::custom custom_32 {12345.6789f};
        constexpr u32 hash_32 = hud::hash_32<hud_test::custom> {}(custom_32);
        hud_assert_eq(hash_32, 0x4640E6B7u);
    }
}

GTEST_TEST(hash_32, hasher_custom_type)
{
    // Non constant
    {
        hud::hasher_32 hasher;
        hud_assert_eq(hasher.hash(hud_test::custom {}).result(), hud::combine_32(0, hud::hash_32<hud_test::custom> {}(hud_test::custom {})));
    }

    // Constant
    {
        hud::hasher_32 hasher;
        constexpr hud_test::custom custom_32;
        hud_assert_eq(hasher.hash(custom_32).result(), hud::combine_32(0, hud::hash_32<hud_test::custom> {}(hud_test::custom {})));
    }
}