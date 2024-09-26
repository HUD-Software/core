GTEST_TEST(i128, addition)
{
    hud_assert_eq(hud::i128 {1} + hud::i128 {2}, hud::i128 {3});
    // Check for overflow of i128; works if it's a two's complement representation.
    hud_assert_eq(hud::i128 {1} + hud::i128_max, hud::i128_min);
}

GTEST_TEST(i128, subtraction)
{
    hud_assert_eq(hud::i128 {3} - hud::i128 {2}, hud::i128 {1});

    // Check for underflow of i128; works if it's a two's complement representation.
    hud_assert_eq(hud::i128_min - hud::i128 {1}, hud::i128_max);
}

GTEST_TEST(i128, division)
{
    hud_assert_eq(hud::i128 {2} / hud::i128 {2}, hud::i128 {1});
    hud_assert_eq(hud::i128 {3} / hud::i128 {2}, hud::i128 {1});
}

GTEST_TEST(i128, multiplication)
{
    hud_assert_eq(hud::i128 {2} * hud::i128 {2}, hud::i128 {4});
    hud_assert_eq(hud::i128 {3} * hud::i128 {0}, hud::i128 {0});
    // Check for overflow of i128; works if it's a two's complement representation.
    hud_assert_eq(hud::i128_max * hud::i128 {2}, i128 {-2});
}

GTEST_TEST(i128, modulus)
{
    hud_assert_eq(hud::i128 {2} % hud::i128 {2}, hud::i128 {0});
    hud_assert_eq(hud::i128 {3} % hud::i128 {2}, hud::i128 {1});
    hud_assert_eq(hud::i128 {10} % hud::i128 {4}, hud::i128 {2});

    hud_assert_eq(hud::i128 {0} % hud::i128_max, hud::i128 {0});
    hud_assert_eq(hud::i128_max % hud::i128_max, hud::i128 {0});
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
    hud_assert_eq(hud::i128 {0} & hud::i128 {1}, hud::i128 {0});
    hud_assert_eq(hud::i128 {2} & hud::i128 {3}, hud::i128 {2});
    hud_assert_eq(hud::i128 {0} & hud::i128_max, hud::i128 {0});
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
    hud::i128 v0 {0};
    v0 |= hud::i128 {1};
    hud_assert_eq(v0, hud::i128 {1});

    hud::i128 v1 {2};
    v1 |= hud::i128 {1};
    hud_assert_eq(v1, hud::i128 {3});

    hud::i128 v2 {0};
    v2 |= hud::i128_max;
    hud_assert_eq(v2, hud::i128_max);
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