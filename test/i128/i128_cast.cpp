#include <core/i128.h>
#include <cmath> // std::nextafter, std::ldexp

GTEST_TEST(i128, cast)
{
    const auto lambda = []<typename type_t>()
    {
        i128 v {hud::limits<type_t>::max};
        type_t result = static_cast<type_t>(v);
        hud_assert_eq(result, hud::limits<type_t>::max);
    };
    hud_test::for_each_type<bool, i8, u8, i16, u16, i32, u32, i64, u64, ansichar, wchar, char16, char32, i128, i128>()(lambda);

#if HD_INTRINSIC_INT128_SUPPORTED
    hud_test::for_each_type<__int128, unsigned __int128>()(lambda);
#endif

    // Test cast max f32
    f32 max_f32_fit_in_i128 = std::nextafter(static_cast<f32>(hud::i128_max), 0.0f);
    i128 v_f32 {max_f32_fit_in_i128};
    f32 result_f32 = static_cast<f64>(v_f32);
    hud_assert_eq(result_f32, max_f32_fit_in_i128);

    // Test cast max f64
    f64 max_f64_fit_in_i128 = std::nextafter(static_cast<f64>(hud::i128_max), 0.0f);
    i128 v_f64 {max_f64_fit_in_i128};
    f64 result_f64 = static_cast<f64>(v_f64);
    hud_assert_eq(result_f64, max_f64_fit_in_i128);
}