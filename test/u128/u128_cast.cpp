
#include <cmath> // std::nextafter, std::ldexp

GTEST_TEST(u128, cast)
{
    const auto lambda = []<typename type_t>()
    {
        u128 v {hud::limits<type_t>::max};
        type_t result = static_cast<type_t>(v);
        hud_assert_eq(result, hud::limits<type_t>::max);
    };
    hud_test::for_each_type<bool, i8, u8, i16, u16, i32, u32, i64, u64, ansichar, wchar, char16, char32, f32, i128, u128>()(lambda);

#if HD_INTRINSIC_INT128_SUPPORTED
    hud_test::for_each_type<__int128, unsigned __int128>()(lambda);
#endif

    // Test cast max f64
    f64 max_f64_fit_in_u128 = std::nextafter(std::ldexp(1.0, 128), 0.0);
    u128 v {max_f64_fit_in_u128};
    f64 result = static_cast<f64>(v);
    hud_assert_eq(result, max_f64_fit_in_u128);
}