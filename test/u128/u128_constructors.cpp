#include <core/i128.h>
#include <cmath> // std::nextafter,std::ldexp

GTEST_TEST(u128, default_constructor)
{
    u8 memory[sizeof(u128)];
    hud::memory::set_memory_safe(memory, 0xFF);
    hud_assert_eq(memory[0], 0xFF);
    hud_assert_eq(memory[sizeof(i128) - 1], 0xFF);
    u128 *v = new (memory) u128;
#if defined(HD_COMPILER_MSVC)
    hud_assert_eq(*v, 0);
#else
    hud_assert_ne(*v, 0);
#endif
}

GTEST_TEST(u128, constructor_init)
{
    u128 v {};
    hud_assert_eq(v, 0);
}

GTEST_TEST(u128, constructor_i8)
{
    // Init to 0
    {
        hud::u128 v {i8 {}};
        hud_assert_eq(v, (i8 {}));
    }

    // Init to min value
    {
        hud::u128 v {hud::i8_min};
        hud_assert_eq(v, hud::i8_min);
    }

    // Init to max value
    {
        hud::u128 v {hud::i8_max};
        hud_assert_eq(v, hud::i8_max);
    }
}

GTEST_TEST(u128, constructor_u8)
{
    // Init to 0
    {
        hud::u128 v {u8 {}};
        hud_assert_eq(v, (u8 {}));
    }

    // Init to min value
    {
        hud::u128 v {hud::u8_min};
        hud_assert_eq(v, hud::u8_min);
    }

    // Init to max value
    {
        hud::u128 v {hud::u8_max};
        hud_assert_eq(v, hud::u8_max);
    }
}

GTEST_TEST(u128, constructor_i16)
{
    // Init to 0
    {
        hud::u128 v {i16 {}};
        hud_assert_eq(v, i16 {});
    }

    // Init to min value
    {
        hud::u128 v {hud::i16_min};
        hud_assert_eq(v, hud::i16_min);
    }

    // Init to max value
    {
        hud::u128 v {hud::i16_max};
        hud_assert_eq(v, hud::i16_max);
    }
}

GTEST_TEST(u128, constructor_u16)
{
    // Init to 0
    {
        hud::u128 v {u16 {}};
        hud_assert_eq(v, u16 {});
    }

    // Init to min value
    {
        hud::u128 v {hud::u16_min};
        hud_assert_eq(v, hud::u16_min);
    }

    // Init to max value
    {
        hud::u128 v {hud::u16_max};
        hud_assert_eq(v, hud::u16_max);
    }
}

GTEST_TEST(u128, constructor_i32)
{
    // Init to 0
    {
        hud::u128 v {i32 {}};
        hud_assert_eq(v, i32 {});
    }

    // Init to min value
    {
        hud::u128 v {hud::i32_min};
        hud_assert_eq(v, hud::i32_min);
    }

    // Init to max value
    {
        hud::u128 v {hud::i32_max};
        hud_assert_eq(v, hud::i32_max);
    }
}

GTEST_TEST(u128, constructor_u32)
{
    // Init to 0
    {
        hud::u128 v {u32 {}};
        hud_assert_eq(v, u32 {});
    }

    // Init to min value
    {
        hud::u128 v {hud::u32_min};
        hud_assert_eq(v, hud::u32_min);
    }

    // Init to max value
    {
        hud::u128 v {hud::u32_max};
        hud_assert_eq(v, hud::u32_max);
    }
}

GTEST_TEST(u128, constructor_i64)
{
    // Init to 0
    {
        hud::u128 v {i64 {}};
        hud_assert_eq(v, i64 {});
    }

    // Init to min value
    {
        hud::u128 v {hud::i64_min};
        hud_assert_eq(v, hud::i64_min);
    }

    // Init to max value
    {
        hud::u128 v {hud::i64_max};
        hud_assert_eq(v, hud::i64_max);
    }
}

GTEST_TEST(u128, constructor_u64)
{
    // Init to 0
    {
        hud::u128 v {u64 {}};
        hud_assert_eq(v, u64 {});
    }

    // Init to min value
    {
        hud::u128 v {hud::u64_min};
        hud_assert_eq(v, hud::u64_min);
    }

    // Init to max value
    {
        hud::u128 v {hud::u64_max};
        hud_assert_eq(v, hud::u64_max);
    }
}

GTEST_TEST(u128, constructor_f32)
{
    // Init to 0
    {
        hud::u128 v {f32 {}};
        hud_assert_eq((float)v, f32 {});
    }

    // Init to min positive value
    {
        f32 value = hud::f32_min_positive;
        hud::u128 v {value};

        // Cast to u32 because f32_min is the lowest positive value of an f32
        hud_assert_eq(static_cast<u32>(v), static_cast<u32>(value));
    }

    // Init to max value
    {
        // Compute the greatest value just before 2^128
        f32 value = std::nextafterf(std::numeric_limits<f32>::max(), 0.0);
        hud::u128 v {value};

        hud_assert_eq(static_cast<f32>(v), value);
    }
}

GTEST_TEST(u128, constructor_f64)
{
    // Init to 0
    {
        hud::u128 v {f64 {}};
        hud_assert_eq((float)v, f64 {});
    }

    // Init to min positive value
    {
        f64 value = hud::f64_min_positive;
        hud::u128 v {value};
        hud_assert_eq(static_cast<u64>(v), static_cast<u64>(hud::f64_min_positive));
    }

    // Init to big value
    {
        // Compute the greatest value just before 2^128
        f64 value = std::nextafter(std::ldexp(1.0, 128), 0.0);
        hud::u128 v {value};
        hud_assert_eq(static_cast<f64>(v), value);
    }
}

GTEST_TEST(u128, constructor_u128)
{
    // Init to 0
    {
        hud::u128 v {hud::u128 {0}};
        hud_assert_eq(v, hud::u128 {0});
    }

    // Init to min value
    {
        hud::u128 v {hud::u128_min};
        hud_assert_eq(v, hud::u128_min);
    }

    // Init to max value
    {
        hud::u128 v {hud::u128_max};
        hud_assert_eq(v, hud::u128_max);
    }
}

GTEST_TEST(u128, constructor_i128)
{
    // Init to 0
    {
        hud::u128 v {hud::i128 {0}};
        hud_assert_eq(v, static_cast<hud::u128>(hud::i128 {0}));
    }

    // Init to min value
    {
        hud::u128 v {hud::i128_min};
        hud_assert_eq(v, static_cast<hud::u128>(hud::i128_min));
    }

    // Init to max value
    {
        hud::u128 v {hud::i128_max};
        hud_assert_eq(v, static_cast<hud::u128>(hud::i128_max));
    }
}

GTEST_TEST(u128, constructor_high_low)
{

    {
        hud::u128 v {0, 0};
        hud_assert_eq(v.high(), 0);
        hud_assert_eq(v.low(), 0);
    }

    {
        hud::u128 v {hud::u64_max, 0};
        hud_assert_eq(v.high(), hud::u64_max);
        hud_assert_eq(v.low(), 0);
    }

    {
        hud::u128 v {0, hud::u64_max};
        hud_assert_eq(v.high(), 0);
        hud_assert_eq(v.low(), hud::u64_max);
    }
}

#if defined(HD_INTRINSIC_INT128_SUPPORTED)
GTEST_TEST(u128, constructor__int128)
{
    // Init to 0
    {
        hud::u128 v {__int128 {0}};
        hud_assert_eq(v, static_cast<hud::u128>(hud::i128 {0}));
    }

    // Init to min value
    {
        hud::u128 v {hud::i128_min};
        hud_assert_eq(v, static_cast<hud::u128>(hud::i128_min));
    }

    // Init to max value
    {
        hud::u128 v {hud::i128_max};
        hud_assert_eq(v, static_cast<hud::u128>(hud::i128_max));
    }
}

GTEST_TEST(u128, constructor_unsigned__int128)
{
    // Init to 0
    {
        hud::u128 v {(unsigned __int128) {0}};
        hud_assert_eq(v, static_cast<hud::u128>(hud::i128 {0}));
    }

    // Init to min value
    {
        hud::u128 v {hud::i128_min};
        hud_assert_eq(v, static_cast<hud::u128>(hud::i128_min));
    }

    // Init to max value
    {
        hud::u128 v {hud::i128_max};
        hud_assert_eq(v, static_cast<hud::u128>(hud::i128_max));
    }
}

#endif