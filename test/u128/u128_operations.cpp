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

GTEST_TEST(u128, division)
{
    hud_assert_eq(hud::u128 {2} / hud::u128 {2}, hud::u128 {1});
    hud_assert_eq(hud::u128 {3} / hud::u128 {2}, hud::u128 {1});
}

GTEST_TEST(u128, multiplication)
{
    hud_assert_eq(hud::u128 {2} * hud::u128 {2}, hud::u128 {4});
    hud_assert_eq(hud::u128 {3} * hud::u128 {0}, hud::u128 {0});
    // Check for overflow of u128; works if it's a two's complement representation.
    hud_assert_eq(hud::u128_max * hud::u128 {2}, hud::u128_max - 1);
}

GTEST_TEST(u128, modulus)
{
    hud_assert_eq(hud::u128 {2} % hud::u128 {2}, hud::u128 {0});
    hud_assert_eq(hud::u128 {3} % hud::u128 {2}, hud::u128 {1});
    hud_assert_eq(hud::u128 {10} % hud::u128 {4}, hud::u128 {2});

    hud_assert_eq(hud::u128 {0} % hud::u128_max, hud::u128 {0});
    hud_assert_eq(hud::u128_max % hud::u128_max, hud::u128 {0});
}

GTEST_TEST(u128, negate)
{
    hud_assert_eq(-hud::u128 {0}, hud::u128 {0});
    hud_assert_eq(-hud::u128 {2}, hud::u128_max - 1);
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
    hud_assert_eq(hud::u128 {0} & hud::u128 {1}, hud::u128 {0});
    hud_assert_eq(hud::u128 {2} & hud::u128 {3}, hud::u128 {2});
    hud_assert_eq(hud::u128 {0} & hud::u128_max, hud::u128 {0});
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
    hud::u128 v0 {0};
    v0 |= hud::u128 {1};
    hud_assert_eq(v0, hud::u128 {1});

    hud::u128 v1 {2};
    v1 |= hud::u128 {1};
    hud_assert_eq(v1, hud::u128 {3});

    hud::u128 v2 {0};
    v2 |= hud::u128_max;
    hud_assert_eq(v2, hud::u128_max);
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