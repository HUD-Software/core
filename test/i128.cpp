#include <cmath>

GTEST_TEST(i128, default_constructor)
{
    i128 v;
    hud_assert_ne(v, 0);
}

GTEST_TEST(i128, constructor_init)
{
    i128 v {};
    hud_assert_eq(v, 0);
}

GTEST_TEST(i128, constructor_i8)
{
    // Init to 0
    {
        hud::i128 v {i8 {}};
        hud_assert_eq(v, (i8 {}));
    }

    // Init to min value
    {
        hud::i128 v {hud::i8_min};
        hud_assert_eq(v, hud::i8_min);
    }

    // Init to max value
    {
        hud::i128 v {hud::i8_max};
        hud_assert_eq(v, hud::i8_max);
    }
}

GTEST_TEST(i128, constructor_i16)
{
    // Init to 0
    {
        hud::i128 v {i16 {}};
        hud_assert_eq(v, i16 {});
    }

    // Init to min value
    {
        hud::i128 v {hud::i16_min};
        hud_assert_eq(v, hud::i16_min);
    }

    // Init to max value
    {
        hud::i128 v {hud::i16_max};
        hud_assert_eq(v, hud::i16_max);
    }
}

GTEST_TEST(i128, constructor_i32)
{
    // Init to 0
    {
        hud::i128 v {i32 {}};
        hud_assert_eq(v, i32 {});
    }

    // Init to min value
    {
        hud::i128 v {hud::i32_min};
        hud_assert_eq(v, hud::i32_min);
    }

    // Init to max value
    {
        hud::i128 v {hud::i32_max};
        hud_assert_eq(v, hud::i32_max);
    }
}

GTEST_TEST(i128, constructor_i64)
{
    // Init to 0
    {
        hud::i128 v {i64 {}};
        hud_assert_eq(v, i64 {});
    }

    // Init to min value
    {
        hud::i128 v {hud::i64_min};
        hud_assert_eq(v, hud::i64_min);
    }

    // Init to max value
    {
        hud::i128 v {hud::i64_max};
        hud_assert_eq(v, hud::i64_max);
    }
}

GTEST_TEST(i128, constructor_f32)
{
    // Init to 0
    {
        hud::i128 v {f32 {}};
        hud_assert_eq((float)v, f32 {});
    }

    // Init to min value
    {
        hud::i128 v {hud::f32_min};
        hud_assert_eq(static_cast<u32>(v), static_cast<u32>(hud::f32_min));
    }

    // Init to max value
    {
        hud::i128 v {hud::f32_max};
        hud_assert_eq(static_cast<u32>(v), static_cast<u32>(hud::f32_max));
    }
}

GTEST_TEST(i128, constructor_f64)
{
    // Init to 0
    {
        hud::i128 v {f64 {}};
        hud_assert_eq((float)v, f64 {});
    }

    // Init to low value
    {
        hud::i128 v {hud::f64_min};
        hud_assert_eq(static_cast<u64>(v), static_cast<u64>(hud::f64_min));
    }

    // Init to big value
    {
        // Compute 2^127
        f64 max_value = std::ldexp(1.0, 127);
        // Compute the greatest value just before 2^127
        f64 value = std::nextafter(std::ldexp(1.0, 127), 0);
        hud_assert_true(std::isfinite(value));
        hud_assert_true((std::numeric_limits<f64>::max_exponent <= 127) || (value >= -std::ldexp(static_cast<f64>(1), 127)));
        hud_assert_true(value < std::ldexp(static_cast<f64>(1), 127));

        hud::i128 v {value};
        hud_assert_eq(static_cast<f64>(v), value);
    }
}

GTEST_TEST(i128, constructor_u128)
{
}

GTEST_TEST(i128, constructor_i128)
{
}

GTEST_TEST(i128, constructor_high_low)
{
}

#if defined(HD_INTRINSIC_INT128_SUPPORTED)
GTEST_TEST(i128, constructor__int128)
{
}

GTEST_TEST(i128, constructor_unsigned__int128)
{
}

#endif