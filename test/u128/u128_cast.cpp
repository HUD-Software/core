
GTEST_TEST(u128, cast)
{
    const auto lambda = []<typename type_t>()
    {
        u128 v {hud::math::limits<type_t>::max};
        type_t result = static_cast<type_t>(v);
        hud_assert_eq(result, hud::math::limits<type_t>::max);
    };
    hud_test::for_each_type<bool, i8, u8, i16, u16, i32, u32, i64, u64, i128, u128>()(lambda);

#if HD_INTRINSIC_INT128_SUPPORTED
    hud_test::for_each_type<__int128, unsigned __int128>()(lambda);
#endif
}