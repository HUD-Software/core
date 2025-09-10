#include <core/i128.h>
#include <cmath> // std::nextafter,std::ldexp
#include <core/math.h>

GTEST_TEST(u128, assign_i8)
{
    // Init to 0
    {
        hud::u128 v {hud::u128_max};
        v = i8 {};
        hud_assert_eq(v, (i8 {}));
    }

    // Init to min value
    {
        hud::u128 v {hud::u128_max};
        v = hud::i8_min;
        hud_assert_eq(v, hud::i8_min);
    }

    // Init to max value
    {
        hud::u128 v {hud::u128_max};
        v = hud::i8_max;
        hud_assert_eq(v, hud::i8_max);
    }
}

GTEST_TEST(u128, assign_i16)
{
    // Init to 0
    {
        hud::u128 v {hud::u128_max};
        v = i16 {};
        hud_assert_eq(v, i16 {});
    }

    // Init to min value
    {
        hud::u128 v {hud::u128_max};
        v = hud::i16_min;
        hud_assert_eq(v, hud::i16_min);
    }

    // Init to max value
    {
        hud::u128 v {hud::u128_max};
        v = hud::i16_max;
        hud_assert_eq(v, hud::i16_max);
    }
}

GTEST_TEST(u128, assign_i32)
{
    // Init to 0
    {
        hud::u128 v {hud::u128_max};
        v = i32 {};
        hud_assert_eq(v, i32 {});
    }

    // Init to min value
    {
        hud::u128 v {hud::u128_max};
        v = hud::i32_min;
        hud_assert_eq(v, hud::i32_min);
    }

    // Init to max value
    {
        hud::u128 v {hud::u128_max};
        v = hud::i32_max;
        hud_assert_eq(v, hud::i32_max);
    }
}

GTEST_TEST(u128, assign_i64)
{
    // Init to 0
    {
        hud::u128 v {hud::u128_max};
        v = i64 {};
        hud_assert_eq(v, i64 {});
    }

    // Init to min value
    {
        hud::u128 v {hud::u128_max};
        v = hud::i64_min;
        hud_assert_eq(v, hud::i64_min);
    }

    // Init to max value
    {
        hud::u128 v {hud::u128_max};
        v = hud::i64_max;
        hud_assert_eq(v, hud::i64_max);
    }
}

GTEST_TEST(u128, assign_f32)
{
    // Init to 0
    {
        hud::u128 v {hud::u128_max};
        v = f32 {};
        hud_assert_eq((float)v, f32 {});
    }

    // Init to min positive value
    {
        f32 value = hud::f32_min_positive;
        hud::u128 v {hud::u128_max};
        v = value;
        // Cast to u32 because f32_min is the lowest positive value of an f32
        hud_assert_eq(static_cast<u32>(v), static_cast<u32>(value));
    }

    // Init to max value
    {
        // Find the largest possible value for an f32 that is just slightly smaller than the maximum value of f32 (3.4028235e+38).
        f32 value = std::nextafterf(std::numeric_limits<f32>::max(), 0.0);

        hud::u128 v {hud::u128_max};
        v = value;
        hud_assert_eq(static_cast<f32>(v), value);
    }
}

GTEST_TEST(u128, assign_f64)
{
    // Init to 0
    {
        hud::u128 v {hud::u128_max};
        v = f64 {};
        hud_assert_eq((float)v, f64 {});
    }

    // Init to min positive value
    {
        f64 value = hud::f64_min_positive;

        hud::u128 v {hud::u128_max};
        v = value;
        hud_assert_eq(static_cast<u64>(v), static_cast<u64>(hud::f64_min_positive));
    }

    // Init to big value
    {
        // Finds the largest representable f64 value that is just slightly smaller than 2^128 (approximately 3.402823669209385 \times 10^{38}), moving towards 0.0.
        // It gives you the closest f64 value before 2^128
        f64 value = std::nextafter(std::ldexp(1.0, 128), 0.0);

        hud::u128 v {hud::u128_max};
        v = value;
        hud_assert_eq(static_cast<f64>(v), value);
    }
}

GTEST_TEST(u128, assign_u128)
{
    // Init to 0
    {
        hud::u128 v {hud::u128_max};
        v = hud::u128 {0};
        hud_assert_eq(v, hud::u128 {0});
    }

    // Init to min value
    {
        hud::u128 v {0};
        v = hud::u128_min;
        hud_assert_eq(v, hud::u128_min);
    }

    // Init to max value
    {
        hud::u128 v {0};
        v = hud::u128_max;
        hud_assert_eq(v, hud::u128_max);
    }
}

GTEST_TEST(u128, assign_i128)
{
    // Init to 0
    {
        hud::u128 v {hud::u128_max};
        v = hud::i128 {0};
        hud_assert_eq(v, static_cast<hud::u128>(hud::i128 {0}));
    }

    // Init to min value
    {
        hud::u128 v {0};
        v = hud::i128 {hud::i128_min};
        hud_assert_eq(v, static_cast<hud::u128>(hud::i128_min));
    }

    // Init to max value
    {
        hud::u128 v {0};
        v = hud::i128 {hud::i128_max};
        hud_assert_eq(v, static_cast<hud::u128>(hud::i128_max));
    }
}

#if defined(HD_INTRINSIC_INT128_SUPPORTED)
GTEST_TEST(u128, assign__int128)
{
    // Init to 0
    {
        hud::u128 v {hud::u128_max};
        v = __int128 {0};
        hud_assert_eq(v, static_cast<hud::u128>(hud::i128 {0}));
    }

    // Init to min value
    {
        hud::u128 v {0};
        v = __int128 {hud::i128_min};
        hud_assert_eq(v, static_cast<hud::u128>(hud::i128_min));
    }

    // Init to max value
    {
        hud::u128 v {0};
        v = __int128 {hud::i128_max};
        hud_assert_eq(v, static_cast<hud::u128>(hud::i128_max));
    }
}

GTEST_TEST(u128, assign_unsigned__int128)
{
    // Init to 0
    {
        hud::u128 v {hud::u128_max};
        v = (unsigned __int128) {0};
        hud_assert_eq(v, static_cast<hud::u128>(hud::i128 {0}));
    }

    // Init to min value
    {
        hud::u128 v {0};
        v = (unsigned __int128) {hud::u128_min};
        hud_assert_eq(v, hud::u128_min);
    }

    // Init to max value
    {
        hud::u128 v {0};
        v = (unsigned __int128) {hud::u128_max};
        hud_assert_eq(v, hud::u128_max);
    }
}

#endif