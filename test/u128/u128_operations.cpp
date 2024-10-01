#include <core/i128.h>

GTEST_TEST(u128, addition)
{
    hud_assert_eq(hud::u128 {1} + hud::u128 {2}, hud::u128 {3});
    // Check for overflow of u128; works if it's a two's complement representation.
    hud_assert_eq(hud::u128 {1} + hud::u128_max, hud::u128 {0});
}

GTEST_TEST(u128, subtraction)
{
    hud_assert_eq(hud::u128 {3} - hud::u128 {2}, hud::u128 {1});
    // Check for underflow of u128; works if it's a two's complement representation.
    hud_assert_eq(hud::u128_min - hud::u128 {1}, hud::u128_max);
}

GTEST_TEST(u128, subtraction_equal)
{
    hud::u128 a {3};
    a -= hud::u128 {2};
    hud_assert_eq(a, hud::u128 {1});
// Check for underflow of u128; works if it's a two's complement representation.
#if !USE_ADDRESS_SANITIZER
    hud::u128 b {hud::u128_min};
    b -= hud::u128 {1};
    hud_assert_eq(b, hud::u128_max); // Signed integer overflow is undefined behaviour
#endif
}

GTEST_TEST(u128, division)
{
    hud_assert_eq(hud::u128 {2} / hud::u128 {2}, hud::u128 {1});
    hud_assert_eq(hud::u128 {3} / hud::u128 {2}, hud::u128 {1});
    hud_assert_eq(hud::u128 {10} / hud::u128 {2}, hud::u128 {5});
    hud_assert_eq(hud::u128 {-6} / hud::u128 {3}, (hud::u128 {0x5555555555555555, 0x5555555555555553}));
    hud_assert_eq(hud::u128 {9} / hud::u128 {3}, hud::u128 {3});

    hud_assert_eq((hud::u128 {0x7FFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF} / hud::u128 {0, 2}), (hud::u128 {0x3FFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF}));
    hud_assert_eq((hud::u128 {0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF} / hud::u128 {0, 1}), (hud::u128 {0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF}));
    hud_assert_eq((hud::u128 {0x7FFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF} / hud::u128 {0, 3}), (hud::u128 {0x2AAAAAAAAAAAAAAA, 0xAAAAAAAAAAAAAAAA}));
}

GTEST_TEST(u128, division_equal)
{
    hud::u128 a {2};
    a /= hud::u128 {2};
    hud_assert_eq(a, hud::u128 {1});
    hud::u128 b {3};
    b /= hud::u128 {2};
    hud_assert_eq(b, hud::u128 {1});
    hud::u128 c {10};
    c /= hud::u128 {2};
    hud_assert_eq(c, hud::u128 {5});
    hud::u128 d {-6};
    d /= hud::u128 {3};
    hud_assert_eq(d, (hud::u128 {0x5555555555555555, 0x5555555555555553}));
    hud::u128 e {9};
    e /= hud::u128 {3};
    hud_assert_eq(e, hud::u128 {3});

    hud::u128 f {0x7FFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF};
    f /= hud::u128 {0, 2};
    hud_assert_eq(f, (hud::u128 {0x3FFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF}));
    hud::u128 g {0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF};
    g /= hud::u128 {0, 1};
    hud_assert_eq(g, (hud::u128 {0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF}));
    hud::u128 h {0x7FFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF};
    h /= hud::u128 {0, 3};
    hud_assert_eq(h, (hud::u128 {0x2AAAAAAAAAAAAAAA, 0xAAAAAAAAAAAAAAAA}));
}

GTEST_TEST(u128, multiplication)
{
    hud_assert_eq(hud::u128 {2} * hud::u128 {2}, hud::u128 {4});
    hud_assert_eq(hud::u128 {3} * hud::u128 {0}, hud::u128 {0});
    hud_assert_eq(hud::u128 {3} * hud::u128 {2}, hud::u128 {6});
    hud_assert_eq(hud::u128 {0} * hud::u128 {10}, hud::u128 {0});
    hud_assert_eq(hud::u128 {-3} * hud::u128 {4}, hud::u128 {-12});
    hud_assert_eq(hud::u128 {7} * hud::u128 {5}, hud::u128 {35});
    // Check for overflow of u128; works if it's a two's complement representation.
    hud_assert_eq(hud::u128_max * hud::u128 {2}, hud::u128_max - 1);
}

GTEST_TEST(u128, multiplication_equal)
{
    hud::u128 a {2};
    a *= hud::u128 {2};
    hud_assert_eq(a, hud::u128 {4});
    hud::u128 b {3};
    b *= hud::u128 {0};
    hud_assert_eq(b, hud::u128 {0});
    hud::u128 c {3};
    c *= hud::u128 {2};
    hud_assert_eq(c, hud::u128 {6});
    hud::u128 d {0};
    d *= hud::u128 {10};
    hud_assert_eq(d, hud::u128 {0});
    hud::u128 e {-3};
    e *= hud::u128 {4};
    hud_assert_eq(e, hud::u128 {-12});
    hud::u128 f {7};
    f *= hud::u128 {5};
    hud_assert_eq(f, hud::u128 {35});
// Check for overflow of u128; works if it's a two's complement representation.
#if !USE_ADDRESS_SANITIZER
    hud::u128 g {hud::u128_max};
    g *= hud::u128 {2};
    hud_assert_eq(g, hud::u128_max - 1); // Signed integer overflow is undefined behaviour
#endif
}

GTEST_TEST(u128, modulus)
{
    hud_assert_eq(hud::u128 {2} % hud::u128 {2}, hud::u128 {0});
    hud_assert_eq(hud::u128 {3} % hud::u128 {2}, hud::u128 {1});
    hud_assert_eq(hud::u128 {10} % hud::u128 {4}, hud::u128 {2});

    hud_assert_eq(hud::u128 {0} % hud::u128_max, hud::u128 {0});
    hud_assert_eq(hud::u128_max % hud::u128_max, hud::u128 {0});
}

GTEST_TEST(u128, modulus_equal)
{
    hud::u128 a {2};
    a %= hud::u128 {2};
    hud_assert_eq(a, hud::u128 {0});
    hud::u128 b {3};
    b %= hud::u128 {2};
    hud_assert_eq(b, hud::u128 {1});
    hud::u128 c {10};
    c %= hud::u128 {4};
    hud_assert_eq(c, hud::u128 {2});
    hud::u128 d {0};
    d %= hud::u128 {hud::u128_max};
    hud_assert_eq(d, hud::u128 {0});

    hud::u128 e {hud::u128_max};
    e %= hud::u128 {hud::u128_max};
    hud_assert_eq(e, hud::u128 {0});
}

GTEST_TEST(u128, negate)
{
    hud_assert_eq(-hud::u128 {0}, hud::u128 {0});
    hud_assert_eq(-hud::u128 {2}, hud::u128 {-2});
    hud_assert_eq(-hud::u128 {-2}, hud::u128 {2});
}

GTEST_TEST(u128, not )
{
    hud_assert_true(!hud::u128 {0});
    hud_assert_false(!hud::u128 {1});
    hud_assert_false(!hud::u128_max);
}

GTEST_TEST(u128, invert_bits)
{
    hud_assert_eq(~hud::u128 {0}, hud::u128_max);
    hud_assert_eq(~hud::u128_max, hud::u128 {0});
    hud_assert_eq((~hud::u128 {hud::u64_max, 0}), (hud::u128 {0, hud::u64_max}));
}

GTEST_TEST(u128, logical_or)
{
    hud_assert_eq(hud::u128 {0} | hud::u128 {1}, hud::u128 {1});
    hud_assert_eq(hud::u128 {2} | hud::u128 {1}, hud::u128 {3});
    hud_assert_eq(hud::u128 {0} | hud::u128_max, hud::u128_max);
}

GTEST_TEST(u128, logical_and)
{
    hud::u128 a {0};
    a &= hud::u128 {1};
    hud_assert_eq(a, hud::u128 {0});
    hud::u128 b {2};
    b &= hud::u128 {3};
    hud_assert_eq(b, hud::u128 {2});
    hud::u128 c {0};
    c &= hud::u128 {hud::u128_max};
    hud_assert_eq(c, hud::u128 {0});
}

GTEST_TEST(u128, logical_xor)
{
    hud_assert_eq(hud::u128 {0} ^ hud::u128 {1}, hud::u128 {1});
    hud_assert_eq(hud::u128 {2} ^ hud::u128 {3}, hud::u128 {1});
    hud_assert_eq(hud::u128 {0} ^ hud::u128_max, hud::u128_max);
    hud_assert_eq(hud::u128_max ^ hud::u128_max, hud::u128 {0});
}

GTEST_TEST(u128, logical_or_equal)
{
    hud::u128 a {0};
    a |= hud::u128 {1};
    hud_assert_eq(a, hud::u128 {1});
    hud::u128 b {2};
    b |= hud::u128 {1};
    hud_assert_eq(b, hud::u128 {3});
    hud::u128 c {0};
    c |= hud::u128_max;
    hud_assert_eq(c, hud::u128_max);
}

GTEST_TEST(u128, logical_and_equal)
{
    hud::u128 v0 {0};
    v0 &= hud::u128 {1};
    hud_assert_eq(v0, hud::u128 {0});

    hud::u128 v1 {3};
    v1 &= hud::u128 {1};
    hud_assert_eq(v1, hud::u128 {1});

    hud::u128 v2 {0};
    v2 &= hud::u128_max;
    hud_assert_eq(v2, hud::u128 {0});
}

GTEST_TEST(u128, logical_xor_equal)
{
    hud::u128 v0 {0};
    v0 ^= hud::u128 {1};
    hud_assert_eq(v0, hud::u128 {1});

    hud::u128 v1 {3};
    v1 ^= hud::u128 {2};
    hud_assert_eq(v1, hud::u128 {1});

    hud::u128 v2 {0};
    v2 ^= hud::u128_max;
    hud_assert_eq(v2, hud::u128 {hud::u128_max});

    hud::u128 v3 {hud::u128_max};
    v3 ^= hud::u128_max;
    hud_assert_eq(v3, hud::u128 {0});
}

GTEST_TEST(u128, post_increment)
{
    hud::u128 v0 {0};
    hud::u128 res = v0++;
    hud_assert_eq(v0, hud::u128 {1});
    hud_assert_eq(res, hud::u128 {0});
}

GTEST_TEST(u128, pre_increment)
{
    hud::u128 v0 {0};
    hud::u128 res = ++v0;
    hud_assert_eq(v0, hud::u128 {1});
    hud_assert_eq(res, hud::u128 {1});
}

GTEST_TEST(u128, post_decrement)
{
    hud::u128 v0 {hud::u128_max};
    hud::u128 res = v0--;
    hud_assert_eq(v0, hud::u128 {hud::u128_max - 1});
    hud_assert_eq(res, hud::u128 {hud::u128_max});
}

GTEST_TEST(u128, pre_decrement)
{
    hud::u128 v0 {hud::u128_max};
    hud::u128 res = --v0;
    hud_assert_eq(v0, hud::u128 {hud::u128_max - 1});
    hud_assert_eq(res, hud::u128 {hud::u128_max - 1});
}

GTEST_TEST(u128, left_shit)
{
    {
        hud::u128 v0 {1};
        hud::u128 res = v0 << 1;
        hud_assert_eq(v0, hud::u128 {1});
        hud_assert_eq(res, hud::u128 {2});
    }
    {
        hud::u128 v0 {hud::u64_max};
        hud::u128 res = v0 << 1;
        hud_assert_eq(v0, hud::u128 {hud::u64_max});
        hud_assert_eq(res, hud::u128(1u, hud::u64_max << 1));
    }
}

GTEST_TEST(u128, right_shift)
{
    {
        hud::u128 v0 {2};
        hud::u128 res = v0 >> 1;
        hud_assert_eq(v0, hud::u128 {2});
        hud_assert_eq(res, hud::u128 {1});
    }
    {
        hud::u128 v0 {hud::u64_max};
        hud::u128 res = v0 >> 1;
        hud_assert_eq(v0, hud::u128 {hud::u64_max});
        hud_assert_eq(res, hud::u128(0u, hud::u64_max >> 1));
    }
}

GTEST_TEST(u128, left_shit_equal)
{
    {
        hud::u128 v0 {1};
        v0 <<= 1;
        hud_assert_eq(v0, hud::u128 {2});
    }
    {
        hud::u128 v0 {hud::u64_max};
        v0 <<= 1;
        hud_assert_eq(v0, hud::u128(1u, hud::u64_max << 1));
    }
}

GTEST_TEST(u128, right_shift_equal)
{
    {
        hud::u128 v0 {2};
        v0 >>= 1;
        hud_assert_eq(v0, hud::u128 {1});
    }
    {
        hud::u128 v0 {hud::u64_max};
        v0 >>= 1;
        hud_assert_eq(v0, hud::u128(0u, hud::u64_max >> 1));
    }
}