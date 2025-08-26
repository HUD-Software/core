#include <core/containers/compressed_tuple.h>

namespace hud_test
{
    struct comparable
    {
        constexpr comparable(i32 j) noexcept
            : i(j)
        {
        }

        [[nodiscard]] constexpr bool operator==(const comparable &o) const noexcept
        {
            return i == o.i;
        }

        [[nodiscard]] constexpr bool operator==(const i32 &o) const noexcept
        {
            return i == o;
        }

        [[nodiscard]] constexpr bool operator<(const comparable &o) const noexcept
        {
            return i < o.i;
        }

        [[nodiscard]] constexpr bool operator<(const i32 &o) const noexcept
        {
            return i < o;
        }

        i32 i;
    };

    constexpr bool operator<(const i32 &left, const comparable &right) noexcept
    {
        return left < right.i;
    }
} // namespace hud_test

GTEST_TEST(compressed_tuple, equal_operator_same_types)
{
    using tuple_type = hud::compressed_tuple<i32, wchar, hud_test::comparable>;

    const auto test = []()
    {
        return std::tuple {
            hud::compressed_tuple<> {}
                == hud::compressed_tuple<> {},
            tuple_type {22, L'a', hud_test::comparable(33)}
                == tuple_type {22, L'a', hud_test::comparable(33)},
            tuple_type {22, L'a', hud_test::comparable(33)}
                == tuple_type {22, L'a', hud_test::comparable(44)},
            tuple_type {22, L'a', hud_test::comparable(33)}
                == tuple_type {22, L'b', hud_test::comparable(33)},
            tuple_type {22, L'a', hud_test::comparable(33)}
                == tuple_type {11, L'a', hud_test::comparable(33)}
        };
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_false(std::get<2>(result));
        hud_assert_false(std::get<3>(result));
        hud_assert_false(std::get<4>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_false(std::get<2>(result));
        hud_assert_false(std::get<3>(result));
        hud_assert_false(std::get<4>(result));
    }
}

GTEST_TEST(compressed_tuple, equal_operator_different_types)
{
    using tuple_type = hud::compressed_tuple<i32, wchar, hud_test::comparable>;

    const auto test = []()
    {
        return std::tuple {
            tuple_type {     22, L'a', hud_test::comparable(33)}
                == hud::compressed_tuple<i32, wchar, i32> {     22, L'a',                       33},
            tuple_type {     22, L'a', hud_test::comparable(33)}
                == hud::compressed_tuple<i32, wchar, i32> {     22, L'a',                       44},
            tuple_type {     22, L'a', hud_test::comparable(33)}
                == hud::compressed_tuple<i32, char16, hud_test::comparable> {     22, u'a', hud_test::comparable(33)},
            tuple_type {     22, L'a', hud_test::comparable(33)}
                == hud::compressed_tuple<i32, char16, hud_test::comparable> {     22, u'b', hud_test::comparable(33)},
            tuple_type {     22, L'a', hud_test::comparable(33)}
                == hud::compressed_tuple<i16, wchar, hud_test::comparable> {(i16)22, L'a', hud_test::comparable(33)},
            tuple_type {     22, L'a', hud_test::comparable(33)}
                == hud::compressed_tuple<i16, wchar, hud_test::comparable> {(i16)11, L'a', hud_test::comparable(33)}
        };
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_false(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_false(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_false(std::get<5>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_false(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_false(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_false(std::get<5>(result));
    }
}

GTEST_TEST(compressed_tuple, not_equal_operator_same_types)
{
    using tuple_type = hud::compressed_tuple<i32, wchar, hud_test::comparable>;

    const auto test = []()
    {
        return std::tuple {
            hud::compressed_tuple<> {}
                != hud::compressed_tuple<> {},
            tuple_type {22, L'a', hud_test::comparable(33)}
                != tuple_type {22, L'a', hud_test::comparable(33)},
            tuple_type {22, L'a', hud_test::comparable(33)}
                != tuple_type {22, L'a', hud_test::comparable(44)},
            tuple_type {22, L'a', hud_test::comparable(33)}
                != tuple_type {22, L'b', hud_test::comparable(33)},
            tuple_type {22, L'a', hud_test::comparable(33)}
                != tuple_type {11, L'a', hud_test::comparable(33)}
        };
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_false(std::get<0>(result));
        hud_assert_false(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_false(std::get<0>(result));
        hud_assert_false(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
    }
}

GTEST_TEST(compressed_tuple, not_equal_operator_different_types)
{
    using tuple_type = hud::compressed_tuple<i32, wchar, hud_test::comparable>;

    const auto test = []()
    {
        return std::tuple {
            tuple_type {     22, L'a', hud_test::comparable(33)}
                != hud::compressed_tuple<i32, wchar, i32> {     22, L'a',                       33},
            tuple_type {     22, L'a', hud_test::comparable(33)}
                != hud::compressed_tuple<i32, wchar, i32> {     22, L'a',                       44},
            tuple_type {     22, L'a', hud_test::comparable(33)}
                != hud::compressed_tuple<i32, char16, hud_test::comparable> {     22, u'a', hud_test::comparable(33)},
            tuple_type {     22, L'a', hud_test::comparable(33)}
                != hud::compressed_tuple<i32, char16, hud_test::comparable> {     22, u'b', hud_test::comparable(33)},
            tuple_type {     22, L'a', hud_test::comparable(33)}
                != hud::compressed_tuple<i16, wchar, hud_test::comparable> {(i16)22, L'a', hud_test::comparable(33)},
            tuple_type {     22, L'a', hud_test::comparable(33)}
                != hud::compressed_tuple<i16, wchar, hud_test::comparable> {(i16)11, L'a', hud_test::comparable(33)}
        };
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_false(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_false(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_false(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_false(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_false(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_false(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
    }
}

GTEST_TEST(compressed_tuple, less_operator_same_types)
{
    using tuple_type = hud::compressed_tuple<i32, wchar, hud_test::comparable>;

    const auto test = []()
    {
        return std::tuple {
            hud::compressed_tuple<> {}
                < hud::compressed_tuple<> {},
            tuple_type {22, L'a', hud_test::comparable(33)}
                < tuple_type {22, L'a', hud_test::comparable(33)},
            tuple_type {22, L'a', hud_test::comparable(33)}
                < tuple_type {22, L'a', hud_test::comparable(44)},
            tuple_type {22, L'a', hud_test::comparable(33)}
                < tuple_type {22, L'a', hud_test::comparable(22)},
            tuple_type {22, L'a', hud_test::comparable(33)}
                < tuple_type {22, L'b', hud_test::comparable(33)},
            tuple_type {22, L'a', hud_test::comparable(33)}
                < tuple_type {22, L'\0', hud_test::comparable(33)},
            tuple_type {22, L'a', hud_test::comparable(33)}
                < tuple_type {33, L'a', hud_test::comparable(33)},
            tuple_type {22, L'a', hud_test::comparable(33)}
                < tuple_type {11, L'a', hud_test::comparable(33)}
        };
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_false(std::get<0>(result));
        hud_assert_false(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_false(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_false(std::get<5>(result));
        hud_assert_true(std::get<6>(result));
        hud_assert_false(std::get<7>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_false(std::get<0>(result));
        hud_assert_false(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_false(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_false(std::get<5>(result));
        hud_assert_true(std::get<6>(result));
        hud_assert_false(std::get<7>(result));
    }
}

GTEST_TEST(compressed_tuple, less_operator_different_types)
{
    using tuple_type = hud::compressed_tuple<i32, wchar, hud_test::comparable>;

    const auto test = []()
    {
        return std::tuple {

            hud::compressed_tuple<> {}
                < hud::compressed_tuple<> {},
            tuple_type {22, L'a', hud_test::comparable(33)}
                < hud::compressed_tuple<i32, wchar, i32> {22, L'a', 33},
            tuple_type {22, L'a', hud_test::comparable(33)}
                < hud::compressed_tuple<i32, wchar, i32> {22, L'a', 44},
            tuple_type {22, L'a', hud_test::comparable(33)}
                < hud::compressed_tuple<i32, wchar, i32> {22, L'a', 22},
            tuple_type {22, L'a', hud_test::comparable(33)}
                < hud::compressed_tuple<i32, char16, hud_test::comparable> {22, u'a', hud_test::comparable(33)},
            tuple_type {22, L'a', hud_test::comparable(33)}
                < hud::compressed_tuple<i32, char16, hud_test::comparable> {22, u'b', hud_test::comparable(33)},
            tuple_type {22, L'a', hud_test::comparable(33)}
                < hud::compressed_tuple<i32, char16, hud_test::comparable> {22, u'\0', hud_test::comparable(33)},
            tuple_type {22, L'a', hud_test::comparable(33)}
                < hud::compressed_tuple<i16, wchar, hud_test::comparable> {(i16)22, u'a', hud_test::comparable(33)},
            tuple_type {22, L'a', hud_test::comparable(33)}
                < hud::compressed_tuple<i16, wchar, hud_test::comparable> {(i16)33, u'b', hud_test::comparable(33)},
            tuple_type {22, L'a', hud_test::comparable(33)}
                < hud::compressed_tuple<i16, wchar, hud_test::comparable> {(i16)11, u'\0', hud_test::comparable(33)}
        };
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_false(std::get<0>(result));
        hud_assert_false(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_false(std::get<3>(result));
        hud_assert_false(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_false(std::get<6>(result));
        hud_assert_false(std::get<7>(result));
        hud_assert_true(std::get<8>(result));
        hud_assert_false(std::get<9>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_false(std::get<0>(result));
        hud_assert_false(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_false(std::get<3>(result));
        hud_assert_false(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_false(std::get<6>(result));
        hud_assert_false(std::get<7>(result));
        hud_assert_true(std::get<8>(result));
        hud_assert_false(std::get<9>(result));
    }
}

GTEST_TEST(compressed_tuple, less_equal_operator_same_types)
{
    using tuple_type = hud::compressed_tuple<i32, wchar, hud_test::comparable>;

    const auto test = []()
    {
        return std::tuple {
            hud::compressed_tuple<> {}
                <= hud::compressed_tuple<> {},
            tuple_type {22, L'a', hud_test::comparable(33)}
                <= tuple_type {22, L'a', hud_test::comparable(33)},
            tuple_type {22, L'a', hud_test::comparable(33)}
                <= tuple_type {22, L'a', hud_test::comparable(44)},
            tuple_type {22, L'a', hud_test::comparable(33)}
                <= tuple_type {22, L'a', hud_test::comparable(22)},
            tuple_type {22, L'a', hud_test::comparable(33)}
                <= tuple_type {22, L'b', hud_test::comparable(33)},
            tuple_type {22, L'a', hud_test::comparable(33)}
                <= tuple_type {22, L'\0', hud_test::comparable(33)},
            tuple_type {22, L'a', hud_test::comparable(33)}
                <= tuple_type {33, L'a', hud_test::comparable(33)},
            tuple_type {22, L'a', hud_test::comparable(33)}
                <= tuple_type {11, L'a', hud_test::comparable(33)}
        };
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_false(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_false(std::get<5>(result));
        hud_assert_true(std::get<6>(result));
        hud_assert_false(std::get<7>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_false(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_false(std::get<5>(result));
        hud_assert_true(std::get<6>(result));
        hud_assert_false(std::get<7>(result));
    }
}

GTEST_TEST(compressed_tuple, less_equal_operator_different_types)
{
    using tuple_type = hud::compressed_tuple<i32, wchar, hud_test::comparable>;

    const auto test = []()
    {
        return std::tuple {

            hud::compressed_tuple<> {}
                <= hud::compressed_tuple<> {},
            tuple_type {22, L'a', hud_test::comparable(33)}
                <= hud::compressed_tuple<i32, wchar, i32> {22, L'a', 33},
            tuple_type {22, L'a', hud_test::comparable(33)}
                <= hud::compressed_tuple<i32, wchar, i32> {22, L'a', 44},
            tuple_type {22, L'a', hud_test::comparable(33)}
                <= hud::compressed_tuple<i32, wchar, i32> {22, L'a', 22},
            tuple_type {22, L'a', hud_test::comparable(33)}
                <= hud::compressed_tuple<i32, char16, hud_test::comparable> {22, u'a', hud_test::comparable(33)},
            tuple_type {22, L'a', hud_test::comparable(33)}
                <= hud::compressed_tuple<i32, char16, hud_test::comparable> {22, u'b', hud_test::comparable(33)},
            tuple_type {22, L'a', hud_test::comparable(33)}
                <= hud::compressed_tuple<i32, char16, hud_test::comparable> {22, u'\0', hud_test::comparable(33)},
            tuple_type {22, L'a', hud_test::comparable(33)}
                <= hud::compressed_tuple<i16, wchar, hud_test::comparable> {(i16)22, L'a', hud_test::comparable(33)},
            tuple_type {22, L'a', hud_test::comparable(33)}
                <= hud::compressed_tuple<i16, wchar, hud_test::comparable> {(i16)33, L'a', hud_test::comparable(33)},
            tuple_type {22, L'a', hud_test::comparable(33)}
                <= hud::compressed_tuple<i16, wchar, hud_test::comparable> {(i16)11, L'a', hud_test::comparable(33)}
        };
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_false(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_false(std::get<6>(result));
        hud_assert_true(std::get<7>(result));
        hud_assert_true(std::get<8>(result));
        hud_assert_false(std::get<9>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_false(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_false(std::get<6>(result));
        hud_assert_true(std::get<7>(result));
        hud_assert_true(std::get<8>(result));
        hud_assert_false(std::get<9>(result));
    }
}

GTEST_TEST(compressed_tuple, greater_operator_same_types)
{
    using tuple_type = hud::compressed_tuple<i32, wchar, hud_test::comparable>;

    const auto test = []()
    {
        return std::tuple {
            hud::compressed_tuple<> {}
                > hud::compressed_tuple<> {},
            tuple_type {22, L'a', hud_test::comparable(33)}
                > tuple_type {22, L'a', hud_test::comparable(33)},
            tuple_type {22, L'a', hud_test::comparable(33)}
                > tuple_type {22, L'a', hud_test::comparable(44)},
            tuple_type {22, L'a', hud_test::comparable(33)}
                > tuple_type {22, L'a', hud_test::comparable(22)},
            tuple_type {22, L'a', hud_test::comparable(33)}
                > tuple_type {22, L'b', hud_test::comparable(33)},
            tuple_type {22, L'a', hud_test::comparable(33)}
                > tuple_type {22, L'\0', hud_test::comparable(33)},
            tuple_type {22, L'a', hud_test::comparable(33)}
                > tuple_type {33, L'a', hud_test::comparable(33)},
            tuple_type {22, L'a', hud_test::comparable(33)}
                > tuple_type {11, L'a', hud_test::comparable(33)}
        };
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_false(std::get<0>(result));
        hud_assert_false(std::get<1>(result));
        hud_assert_false(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_false(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_false(std::get<6>(result));
        hud_assert_true(std::get<7>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_false(std::get<0>(result));
        hud_assert_false(std::get<1>(result));
        hud_assert_false(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_false(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_false(std::get<6>(result));
        hud_assert_true(std::get<7>(result));
    }
}

GTEST_TEST(compressed_tuple, greater_equal_operator_different_types)
{
    using tuple_type = hud::compressed_tuple<i32, wchar, hud_test::comparable>;

    const auto test = []()
    {
        return std::tuple {

            hud::compressed_tuple<> {}
                >= hud::compressed_tuple<> {},
            tuple_type {22, L'a', hud_test::comparable(33)}
                >= hud::compressed_tuple<i32, wchar, i32> {22, L'a', 33},
            tuple_type {22, L'a', hud_test::comparable(33)}
                >= hud::compressed_tuple<i32, wchar, i32> {22, L'a', 44},
            tuple_type {22, L'a', hud_test::comparable(33)}
                >= hud::compressed_tuple<i32, wchar, i32> {22, L'a', 22},
            tuple_type {22, L'a', hud_test::comparable(33)}
                >= hud::compressed_tuple<i32, char16, hud_test::comparable> {22, u'a', hud_test::comparable(33)},
            tuple_type {22, L'a', hud_test::comparable(33)}
                >= hud::compressed_tuple<i32, char16, hud_test::comparable> {22, u'b', hud_test::comparable(33)},
            tuple_type {22, L'a', hud_test::comparable(33)}
                >= hud::compressed_tuple<i32, char16, hud_test::comparable> {22, u'\0', hud_test::comparable(33)},
            tuple_type {22, L'a', hud_test::comparable(33)}
                >= hud::compressed_tuple<i16, wchar, hud_test::comparable> {(i16)22, L'a', hud_test::comparable(33)},
            tuple_type {22, L'a', hud_test::comparable(33)}
                >= hud::compressed_tuple<i16, wchar, hud_test::comparable> {(i16)33, L'a', hud_test::comparable(33)},
            tuple_type {22, L'a', hud_test::comparable(33)}
                >= hud::compressed_tuple<i16, wchar, hud_test::comparable> {(i16)11, L'a', hud_test::comparable(33)}
        };
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_false(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_false(std::get<5>(result));
        hud_assert_true(std::get<6>(result));
        hud_assert_true(std::get<7>(result));
        hud_assert_false(std::get<8>(result));
        hud_assert_true(std::get<9>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_false(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_false(std::get<5>(result));
        hud_assert_true(std::get<6>(result));
        hud_assert_true(std::get<7>(result));
        hud_assert_false(std::get<8>(result));
        hud_assert_true(std::get<9>(result));
    }
}