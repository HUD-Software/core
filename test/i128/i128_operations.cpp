#include <core/i128.h>

GTEST_TEST(i128, addition)
{
    hud_assert_eq(hud::i128 {1} + hud::i128 {2}, hud::i128 {3});
// Check for overflow of i128; works if it's a two's complement representation.
#if !USE_ADDRESS_SANITIZER
    hud_assert_eq(hud::i128 {1} + hud::i128_max, hud::i128_min); // Signed integer overflow is undefined behaviour
#endif
}

GTEST_TEST(i128, addition_equal)
{
    hud::i128 a {1};
    a += hud::i128 {2};
    hud_assert_eq(a, hud::i128 {3});
// Check for overflow of i128; works if it's a two's complement representation.
#if !USE_ADDRESS_SANITIZER
    hud::i128 b {1};
    b += hud::i128_max;
    hud_assert_eq(b, hud::i128_min); // Signed integer overflow is undefined behaviour
#endif
}

GTEST_TEST(i128, subtraction)
{
    hud_assert_eq(hud::i128 {3} - hud::i128 {2}, hud::i128 {1});
// Check for underflow of i128; works if it's a two's complement representation.
#if !USE_ADDRESS_SANITIZER
    hud_assert_eq(hud::i128_min - hud::i128 {1}, hud::i128_max); // Signed integer overflow is undefined behaviour
#endif
}

GTEST_TEST(i128, subtraction_equal)
{
    hud::i128 a {3};
    a -= hud::i128 {2};
    hud_assert_eq(a, hud::i128 {1});
// Check for underflow of i128; works if it's a two's complement representation.
#if !USE_ADDRESS_SANITIZER
    hud::i128 b {hud::i128_min};
    b -= hud::i128 {1};
    hud_assert_eq(b, hud::i128_max); // Signed integer overflow is undefined behaviour
#endif
}

GTEST_TEST(i128, division)
{
    hud_assert_eq(hud::i128 {2} / hud::i128 {2}, hud::i128 {1});
    hud_assert_eq(hud::i128 {3} / hud::i128 {2}, hud::i128 {1});
    hud_assert_eq(hud::i128 {10} / hud::i128 {2}, hud::i128 {5});
    hud_assert_eq(hud::i128 {-6} / hud::i128 {3}, hud::i128 {-2});
    hud_assert_eq(hud::i128 {9} / hud::i128 {3}, hud::i128 {3});

    hud_assert_eq((hud::i128 {0x7FFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF} / hud::i128 {0, 2}), (hud::i128 {0x3FFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF}));
    hud_assert_eq((hud::i128 {(i64)0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF} / hud::i128 {0, 1}), (hud::i128 {(i64)0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF}));
    hud_assert_eq((hud::i128 {0x7FFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF} / hud::i128 {0, 3}), (hud::i128 {0x2AAAAAAAAAAAAAAA, 0xAAAAAAAAAAAAAAAA}));
}

GTEST_TEST(i128, division_equal)
{
    hud::i128 a {2};
    a /= hud::i128 {2};
    hud_assert_eq(a, hud::i128 {1});
    hud::i128 b {3};
    b /= hud::i128 {2};
    hud_assert_eq(b, hud::i128 {1});
    hud::i128 c {10};
    c /= hud::i128 {2};
    hud_assert_eq(c, hud::i128 {5});
    hud::i128 d {-6};
    d /= hud::i128 {3};
    hud_assert_eq(d, hud::i128 {-2});
    hud::i128 e {9};
    e /= hud::i128 {3};
    hud_assert_eq(e, hud::i128 {3});

    hud::i128 f {0x7FFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF};
    f /= hud::i128 {0, 2};
    hud_assert_eq(f, (hud::i128 {0x3FFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF}));
    hud::i128 g {(i64)0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF};
    g /= hud::i128 {0, 1};
    hud_assert_eq(g, (hud::i128 {(i64)0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF}));
    hud::i128 h {0x7FFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF};
    h /= hud::i128 {0, 3};
    hud_assert_eq(h, (hud::i128 {0x2AAAAAAAAAAAAAAA, 0xAAAAAAAAAAAAAAAA}));
}

GTEST_TEST(i128, multiplication)
{
    hud_assert_eq(hud::i128 {2} * hud::i128 {2}, hud::i128 {4});
    hud_assert_eq(hud::i128 {3} * hud::i128 {0}, hud::i128 {0});
    hud_assert_eq(hud::i128 {3} * hud::i128 {2}, hud::i128 {6});
    hud_assert_eq(hud::i128 {0} * hud::i128 {10}, hud::i128 {0});
    hud_assert_eq(hud::i128 {-3} * hud::i128 {4}, hud::i128 {-12});
    hud_assert_eq(hud::i128 {7} * hud::i128 {5}, hud::i128 {35});
// Check for overflow of i128; works if it's a two's complement representation.
#if !USE_ADDRESS_SANITIZER
    hud_assert_eq(hud::i128_max * hud::i128 {2}, i128 {-2}); // Signed integer overflow is undefined behaviour
#endif
}

GTEST_TEST(i128, multiplication_equal)
{
    hud::i128 a {2};
    a *= hud::i128 {2};
    hud_assert_eq(a, hud::i128 {4});
    hud::i128 b {3};
    b *= hud::i128 {0};
    hud_assert_eq(b, hud::i128 {0});
    hud::i128 c {3};
    c *= hud::i128 {2};
    hud_assert_eq(c, hud::i128 {6});
    hud::i128 d {0};
    d *= hud::i128 {10};
    hud_assert_eq(d, hud::i128 {0});
    hud::i128 e {-3};
    e *= hud::i128 {4};
    hud_assert_eq(e, hud::i128 {-12});
    hud::i128 f {7};
    f *= hud::i128 {5};
    hud_assert_eq(f, hud::i128 {35});
// Check for overflow of i128; works if it's a two's complement representation.
#if !USE_ADDRESS_SANITIZER
    hud::i128 g {hud::i128_max};
    g *= hud::i128 {2};
    hud_assert_eq(g, i128 {-2}); // Signed integer overflow is undefined behaviour
#endif
}

GTEST_TEST(i128, modulus)
{
    hud_assert_eq(hud::i128 {2} % hud::i128 {2}, hud::i128 {0});
    hud_assert_eq(hud::i128 {3} % hud::i128 {2}, hud::i128 {1});
    hud_assert_eq(hud::i128 {10} % hud::i128 {4}, hud::i128 {2});

    hud_assert_eq(hud::i128 {0} % hud::i128_max, hud::i128 {0});
    hud_assert_eq(hud::i128_max % hud::i128_max, hud::i128 {0});
}

GTEST_TEST(i128, modulus_equal)
{
    hud::i128 a {2};
    a %= hud::i128 {2};
    hud_assert_eq(a, hud::i128 {0});
    hud::i128 b {3};
    b %= hud::i128 {2};
    hud_assert_eq(b, hud::i128 {1});
    hud::i128 c {10};
    c %= hud::i128 {4};
    hud_assert_eq(c, hud::i128 {2});
    hud::i128 d {0};
    d %= hud::i128 {hud::i128_max};
    hud_assert_eq(d, hud::i128 {0});

    hud::i128 e {hud::i128_max};
    e %= hud::i128 {hud::i128_max};
    hud_assert_eq(e, hud::i128 {0});
}

GTEST_TEST(i128, negate)
{
    hud_assert_eq(-hud::i128 {0}, hud::i128 {0});
    hud_assert_eq(-hud::i128 {2}, hud::i128 {-2});
    hud_assert_eq(-hud::i128 {-2}, hud::i128 {2});
}

GTEST_TEST(i128, not )
{
    hud_assert_true(!hud::i128 {0});
    hud_assert_false(!hud::i128 {1});
    hud_assert_false(!hud::i128_max);
}

GTEST_TEST(i128, invert_bits)
{
    hud_assert_eq(~hud::i128 {0}, i128 {-1});
    hud_assert_eq(~hud::i128_max, hud::i128_min);
    hud_assert_eq((~hud::i128 {hud::i64_max, 0}), (hud::i128 {hud::i64_min, hud::u64_max}));
}

GTEST_TEST(i128, logical_or)
{
    hud_assert_eq(hud::i128 {0} | hud::i128 {1}, hud::i128 {1});
    hud_assert_eq(hud::i128 {2} | hud::i128 {1}, hud::i128 {3});
    hud_assert_eq(hud::i128 {0} | hud::i128_max, hud::i128_max);
}

GTEST_TEST(i128, logical_and)
{
    hud::i128 a {0};
    a &= hud::i128 {1};
    hud_assert_eq(a, hud::i128 {0});
    hud::i128 b {2};
    b &= hud::i128 {3};
    hud_assert_eq(b, hud::i128 {2});
    hud::i128 c {0};
    c &= hud::i128 {hud::i128_max};
    hud_assert_eq(c, hud::i128 {0});
}

GTEST_TEST(i128, logical_xor)
{
    hud_assert_eq(hud::i128 {0} ^ hud::i128 {1}, hud::i128 {1});
    hud_assert_eq(hud::i128 {2} ^ hud::i128 {3}, hud::i128 {1});
    hud_assert_eq(hud::i128 {0} ^ hud::i128_max, hud::i128_max);
    hud_assert_eq(hud::i128_max ^ hud::i128_max, hud::i128 {0});
}

GTEST_TEST(i128, logical_or_equal)
{
    hud::i128 a {0};
    a |= hud::i128 {1};
    hud_assert_eq(a, hud::i128 {1});
    hud::i128 b {2};
    b |= hud::i128 {1};
    hud_assert_eq(b, hud::i128 {3});
    hud::i128 c {0};
    c |= hud::i128_max;
    hud_assert_eq(c, hud::i128_max);
}

GTEST_TEST(i128, logical_and_equal)
{
    hud::i128 v0 {0};
    v0 &= hud::i128 {1};
    hud_assert_eq(v0, hud::i128 {0});

    hud::i128 v1 {3};
    v1 &= hud::i128 {1};
    hud_assert_eq(v1, hud::i128 {1});

    hud::i128 v2 {0};
    v2 &= hud::i128_max;
    hud_assert_eq(v2, hud::i128 {0});
}

GTEST_TEST(i128, logical_xor_equal)
{
    hud::i128 v0 {0};
    v0 ^= hud::i128 {1};
    hud_assert_eq(v0, hud::i128 {1});

    hud::i128 v1 {3};
    v1 ^= hud::i128 {2};
    hud_assert_eq(v1, hud::i128 {1});

    hud::i128 v2 {0};
    v2 ^= hud::i128_max;
    hud_assert_eq(v2, hud::i128 {hud::i128_max});

    hud::i128 v3 {hud::i128_max};
    v3 ^= hud::i128_max;
    hud_assert_eq(v3, hud::i128 {0});
}

GTEST_TEST(i128, post_increment)
{
    hud::i128 v0 {0};
    hud::i128 res = v0++;
    hud_assert_eq(v0, hud::i128 {1});
    hud_assert_eq(res, hud::i128 {0});
}

GTEST_TEST(i128, pre_increment)
{
    hud::i128 v0 {0};
    hud::i128 res = ++v0;
    hud_assert_eq(v0, hud::i128 {1});
    hud_assert_eq(res, hud::i128 {1});
}

GTEST_TEST(i128, post_decrement)
{
    hud::i128 v0 {hud::i128_max};
    hud::i128 res = v0--;
    hud_assert_eq(v0, hud::i128 {hud::i128_max - 1});
    hud_assert_eq(res, hud::i128 {hud::i128_max});
}

GTEST_TEST(i128, pre_decrement)
{
    hud::i128 v0 {hud::i128_max};
    hud::i128 res = --v0;
    hud_assert_eq(v0, hud::i128 {hud::i128_max - 1});
    hud_assert_eq(res, hud::i128 {hud::i128_max - 1});
}

GTEST_TEST(i128, left_shit)
{
    {
        hud::i128 v0 {1};
        hud::i128 res = v0 << 1;
        hud_assert_eq(v0, hud::i128 {1});
        hud_assert_eq(res, hud::i128 {2});
    }
    {
        hud::i128 v0 {hud::u64_max};
        hud::i128 res = v0 << 1;
        hud_assert_eq(v0, hud::i128 {hud::u64_max});
        hud_assert_eq(res, hud::i128(1u, hud::u64_max << 1));
    }
}

GTEST_TEST(i128, right_shift)
{
    {
        hud::i128 v0 {2};
        hud::i128 res = v0 >> 1;
        hud_assert_eq(v0, hud::i128 {2});
        hud_assert_eq(res, hud::i128 {1});
    }
    {
        hud::i128 v0 {hud::u64_max};
        hud::i128 res = v0 >> 1;
        hud_assert_eq(v0, hud::i128 {hud::u64_max});
        hud_assert_eq(res, hud::i128(0u, hud::u64_max >> 1));
    }
}

GTEST_TEST(i128, left_shit_equal)
{
    {
        hud::i128 v0 {1};
        v0 <<= 1;
        hud_assert_eq(v0, hud::i128 {2});
    }
    {
        hud::i128 v0 {hud::u64_max};
        v0 <<= 1;
        hud_assert_eq(v0, hud::i128(1u, hud::u64_max << 1));
    }
}

GTEST_TEST(i128, right_shift_equal)
{
    {
        hud::i128 v0 {2};
        v0 >>= 1;
        hud_assert_eq(v0, hud::i128 {1});
    }
    {
        hud::i128 v0 {hud::u64_max};
        v0 >>= 1;
        hud_assert_eq(v0, hud::i128(0u, hud::u64_max >> 1));
    }
}