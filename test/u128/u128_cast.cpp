
GTEST_TEST(u128, cast)
{
    const auto lambda = []<typename type_t>()
    {
        u128 v {hud::math::limits<type_t>::max};
        type_t result = static_cast<type_t>(v);
        hud_assert_eq(result, hud::math::limits<type_t>::max);
    };
    hud_test::for_each_type<bool, i8, u8, i16, u16, i32, u32, i64, u64, i128, u128>()(lambda);
}

// GTEST_TEST(u128, cast_to_u8)
// {
//     u128 v {hud::u8_max};
//     u8 result = static_cast<u8>(v);
//     hud_assert_eq(result, hud::u8_max);
// }

// GTEST_TEST(u128, cast_to_i16)
// {
//     u128 v {hud::i16_max};
//     i16 result = static_cast<i16>(v);
//     hud_assert_eq(result, hud::i16_max);
// }

// GTEST_TEST(u128, cast_to_u16)
// {
//     u128 v {hud::u16_max};
//     u16 result = static_cast<u16>(v);
//     hud_assert_eq(result, hud::u16_max);
// }

// GTEST_TEST(u128, cast_to_i32)
// {
//     u128 v {hud::i32_max};
//     i32 result = static_cast<i32>(v);
//     hud_assert_eq(result, hud::i32_max);
// }

// GTEST_TEST(u128, cast_to_u32)
// {
//     u128 v {hud::u32_max};
//     u32 result = static_cast<u32>(v);
//     hud_assert_eq(result, hud::u32_max);
// }
