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
        i128 v {i8 {}};
        hud_assert_eq(v, (i8 {}));
    }

    // Init to min value
    {
        i128 v {hud::i8_min};
        hud_assert_eq(v, hud::i8_min);
    }

    // Init to max value
    {
        i128 v {hud::i8_max};
        hud_assert_eq(v, hud::i8_max);
    }
}

GTEST_TEST(i128, constructor_i16)
{
    // Init to 0
    {
        i128 v {i16 {}};
        hud_assert_eq(v, i16 {});
    }

    // Init to min value
    {
        i128 v {hud::i16_min};
        hud_assert_eq(v, hud::i16_min);
    }

    // Init to max value
    {
        i128 v {hud::i16_max};
        hud_assert_eq(v, hud::i16_max);
    }
}

GTEST_TEST(i128, constructor_i32)
{
    // Init to 0
    {
        i128 v {i32 {}};
        hud_assert_eq(v, i32 {});
    }

    // Init to min value
    {
        i128 v {hud::i32_min};
        hud_assert_eq(v, hud::i32_min);
    }

    // Init to max value
    {
        i128 v {hud::i32_max};
        hud_assert_eq(v, hud::i32_max);
    }
}

GTEST_TEST(i128, constructor_i64)
{
    // Init to 0
    {
        i128 v {i64 {}};
        hud_assert_eq(v, i64 {});
    }

    // Init to min value
    {
        i128 v {hud::i64_min};
        hud_assert_eq(v, hud::i64_min);
    }

    // Init to max value
    {
        i128 v {hud::i64_max};
        hud_assert_eq(v, hud::i64_max);
    }
}

GTEST_TEST(i128, constructor_i128)
{
    // Init to 0
    {
        i128 v {i128 {}};
        hud_assert_eq(v, i128 {});
    }

    // Init to min value
    {
        i128 v {hud::i128_min};
        hud_assert_eq(v, hud::i128_min);
    }

    // Init to max value
    {
        i128 v {hud::i128_max};
        hud_assert_eq(v, hud::i128_max);
    }
}

GTEST_TEST(i128, constructor_u8)
{
    // Init to 0
    {
        i128 v {u8 {}};
        hud_assert_eq(v, (u8 {}));
    }

    // Init to min value
    {
        i128 v {hud::u8_min};
        hud_assert_eq(v, hud::u8_min);
    }

    // Init to max value
    {
        i128 v {hud::u8_max};
        hud_assert_eq(v, hud::u8_max);
    }
}

GTEST_TEST(i128, constructor_u16)
{
    // Init to 0
    {
        i128 v {u16 {}};
        hud_assert_eq(v, u16 {});
    }

    // Init to min value
    {
        i128 v {hud::u16_min};
        hud_assert_eq(v, hud::u16_min);
    }

    // Init to max value
    {
        i128 v {hud::u16_max};
        hud_assert_eq(v, hud::u16_max);
    }
}

GTEST_TEST(i128, constructor_u32)
{
    // Init to 0
    {
        i128 v {u32 {}};
        hud_assert_eq(v, u32 {});
    }

    // Init to min value
    {
        i128 v {hud::u32_min};
        hud_assert_eq(v, hud::u32_min);
    }

    // Init to max value
    {
        i128 v {hud::u32_max};
        hud_assert_eq(v, hud::u32_max);
    }
}

GTEST_TEST(i128, constructor_u64)
{
    // Init to 0
    {
        i128 v {u64 {}};
        hud_assert_eq(v, u64 {});
    }

    // Init to min value
    {
        i128 v {hud::u64_min};
        hud_assert_eq(v, hud::u64_min);
    }

    // Init to max value
    {
        i128 v {hud::u64_max};
        hud_assert_eq(v, hud::u64_max);
    }
}

GTEST_TEST(i128, constructor_u128)
{
    // Init to 0
    {
        i128 v {u128 {}};
        hud_assert_eq(v, static_cast<i128>(u128 {}));
    }

    // Init to min value
    {
        i128 v {hud::u128_min};
        hud_assert_eq(v, static_cast<i128>(hud::u128_min));
    }

    // Init to max value
    {
        i128 v {hud::u128_max};
        hud_assert_eq(v, static_cast<i128>(hud::u128_max));
    }
}
