
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
    struct hash_64<hud_test::custom>
    {
        [[nodiscard]] constexpr u64 operator()(const hud_test::custom &custom) const
        {
            return hud::hash_64<f32> {}(custom.value_);
        }
    };

} // namespace hud

GTEST_TEST(hash_64, hash_can_hash_integral)
{
    hud_assert_eq(hud::hash_64<i8> {}(hud::i8_max), static_cast<u64>(hud::i8_max));
    hud_assert_eq(hud::hash_64<u8> {}(hud::u8_max), static_cast<u64>(hud::u8_max));

    hud_assert_eq(hud::hash_64<i16> {}(hud::i16_max), static_cast<u64>(hud::i16_max));
    hud_assert_eq(hud::hash_64<u16> {}(hud::u16_max), static_cast<u64>(hud::u16_max));

    hud_assert_eq(hud::hash_64<i32> {}(hud::i32_max), static_cast<u64>(hud::i32_max));
    hud_assert_eq(hud::hash_64<u32> {}(hud::u32_max), static_cast<u64>(hud::u32_max));

    hud_assert_eq(hud::hash_64<i64> {}(hud::i64_max), 0x7FFFFFFFFFFFFFFFu);
    hud_assert_eq(hud::hash_64<u64> {}(hud::u64_max), 0xFFFFFFFFFFFFFFFFu);
}

GTEST_TEST(hash_64, hash_of_integral_are_usable_in_constexpr)
{
    constexpr u64 hash_vi8 = hud::hash_64<i8> {}(hud::i8_max);
    hud_assert_eq(hash_vi8, static_cast<u64>(hud::i8_max));

    constexpr u64 hash_vu8 = hud::hash_64<u8> {}(hud::u8_max);
    hud_assert_eq(hash_vu8, static_cast<u64>(hud::u8_max));

    constexpr u64 hash_vi16 = hud::hash_64<i16> {}(hud::i16_max);
    hud_assert_eq(hash_vi16, static_cast<u64>(hud::i16_max));

    constexpr u64 hash_vu16 = hud::hash_64<u16> {}(hud::u16_max);
    hud_assert_eq(hash_vu16, static_cast<u64>(hud::u16_max));

    constexpr u64 hash_vi32 = hud::hash_64<i32> {}(hud::i32_max);
    hud_assert_eq(hash_vi32, static_cast<u64>(hud::i32_max));

    constexpr u64 hash_vu32 = hud::hash_64<u32> {}(hud::u32_max);
    hud_assert_eq(hash_vu32, static_cast<u64>(hud::u32_max));

    constexpr u64 hash_vi64 = hud::hash_64<i64> {}(hud::i64_max);
    hud_assert_eq(hash_vi64, 0x7FFFFFFFFFFFFFFFu);

    constexpr u64 hash_vu64 = hud::hash_64<u64> {}(hud::u64_max);
    hud_assert_eq(hash_vu64, 0xFFFFFFFFFFFFFFFFu);
}

GTEST_TEST(hash_64, hash_can_hash_floating_point)
{
    f32 flt32 = 12345.6789f;
    hud_assert_eq(hud::hash_64<f32> {}(flt32), 0x000000004640E6B7);

    f64 flt64 = 12345.6789;
    hud_assert_eq(hud::hash_64<f64> {}(flt64), 0x40C81CD6E631F8A1u);
}

GTEST_TEST(hash_64, hash_of_floating_point_are_usable_in_constexpr)
{
    constexpr f32 flt32 = 12345.6789f;
    constexpr u64 flt32_hash = hud::hash_64<f32> {}(flt32);
    hud_assert_eq(flt32_hash, 0x000000004640E6B7);

    constexpr f64 flt64 = 12345.6789;
    constexpr u64 flt64_hash = hud::hash_64<f64> {}(flt64);
    hud_assert_eq(flt64_hash, 0x40C81CD6E631F8A1u);
}

GTEST_TEST(hash_64, hash_can_hash_c_string)
{
    static constexpr const ansichar txt[] = "abcdefghijklmnopqrstuvwxyz";
    hud_assert_eq(hud::hash_64<const ansichar *> {}(txt, hud::cstring::length(txt)), 0x5EAD741CE7AC31BDu);

    static constexpr const wchar *wtxt = L"abcdefghijklmnopqrstuvwxyz";
    if constexpr (sizeof(wchar) == 2)
    {
        hud_assert_eq(hud::hash_64<const wchar *> {}(wtxt, hud::cstring::length(wtxt)), 0xE66EDE359E692EBBu);
    }
    else if constexpr (sizeof(wchar) == 4)
    {
        hud_assert_eq(hud::hash_64<const wchar *> {}(wtxt, hud::cstring::length(wtxt)), 0xF09AD8D8223EE63Bu);
    }
    else
    {
        FAIL();
    }
}

GTEST_TEST(hash_64, hash_can_hash_c_string_are_usable_in_constexpr)
{
    static constexpr const ansichar txt[] = "abcdefghijklmnopqrstuvwxyz";
    constexpr u64 hash_a = hud::hash_64<const ansichar *> {}(txt, hud::cstring::length(txt));
    hud_assert_eq(hash_a, 0x5EAD741CE7AC31BDu);

    // Not Possible, wchar* cannot be cast to ansichar at compile time
    //
    // static constexpr const wchar *wtxt = L"abcdefghijklmnopqrstuvwxyz";
    // if constexpr (sizeof(wchar) == 2)
    // {
    //     constexpr u64 hash_w = hud::hash_64(wtxt, hud::cstring::length(wtxt));
    //     hud_assert_eq(hash_w, 0xE66EDE359E692EBBu);
    // }
    // else if constexpr (sizeof(wchar) == 4)
    // {
    //     constexpr u64 hash_w = hud::hash_64(wtxt, hud::cstring::length(wtxt));
    //     hud_assert_eq(hash_w, 0xF09AD8D8223EE63Bu);
    // }
    // else
    // {
    //     FAIL();
    // }
}

// GTEST_TEST(hash_64, hash_can_hash_enumeration)
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

// hud_assert_eq(hud::hash_64(E::a), hud::hash_64(hud::u32_max));
// hud_assert_eq(hud::hash_64(E::b), hud::hash_64(static_cast<u64>('a')));
// hud_assert_eq(hud::hash_64(E2::a), hud::hash_64(hud::u64_max));
// hud_assert_eq(hud::hash_64(E2::b), hud::hash_64(static_cast<u64>('a')));
// }

// GTEST_TEST(hash_64, hash_enumeration_are_usable_in_constexpr)
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

// constexpr u64 hash_e1_a = hud::hash_64(E::a);
// hud_assert_eq(hash_e1_a, hud::hash_64(hud::u32_max));
// constexpr u64 hash_e1_b = hud::hash_64(E::b);
// hud_assert_eq(hash_e1_b, hud::hash_64(static_cast<u32>('a')));
// constexpr u64 hash_e2_a = hud::hash_64(E2::a);
// hud_assert_eq(hash_e2_a, hud::hash_64(hud::u64_max));
// constexpr u64 hash_e2_ab = hud::hash_64(E2::b);
// hud_assert_eq(hash_e2_ab, hud::hash_64(static_cast<u64>('a')));
// }

GTEST_TEST(hash_64, hash_can_hash_pointers)
{
    const u64 *ptr = nullptr;
    hud_assert_eq(hud::hash_64<const void *> {}(static_cast<const void *>(ptr)), reinterpret_cast<uptr>(ptr));
}

GTEST_TEST(hash_64, hasher64)
{
    constexpr const usize len = hud::cstring::length("key");
    hud::hasher_64 hasher;
    hud_assert_eq(hasher("key", len).result(), 0xCEAAB8E77B74C2E7u);
    hud_assert_eq(hasher((const ansichar *)&len, sizeof(len)).result(), 0x746D68F6EB969EB7u);

    // Test that hasher can be called redundancy
    u64 value = hud::hasher_64 {}("key", len).hash((const ansichar *)&len, sizeof(len)).result();
    hud_assert_eq(value, 0x746D68F6EB969EB7u);
}

GTEST_TEST(hash_64, hash_custom_type)
{
    // Non constant
    {
        constexpr hud_test::custom custom_64 {12345.6789f};
        hud_assert_eq(hud::hash_64<hud_test::custom> {}(custom_64), 0x000000004640E6B7);
    }

    // Constant
    {
        constexpr hud_test::custom custom_64 {12345.6789f};
        constexpr u64 hash_64 = hud::hash_64<hud_test::custom> {}(custom_64);
        hud_assert_eq(hash_64, 0x000000004640E6B7);
    }
}

GTEST_TEST(hash_64, hasher_custom_type)
{
    // Non constant
    {
        hud::hasher_64 hasher;
        hud_assert_eq(hasher.hash(hud_test::custom {}).result(), hud::combine_64(0, hud::hash_64<hud_test::custom> {}(hud_test::custom {})));
    }

    // Constant
    {
        hud::hasher_64 hasher;
        constexpr hud_test::custom custom_64;
        hud_assert_eq(hasher.hash(custom_64).result(), hud::combine_64(0, hud::hash_64<hud_test::custom> {}(hud_test::custom {})));
    }
}