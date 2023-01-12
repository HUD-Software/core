#include <core/containers/tuple.h>

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
}

GTEST_TEST(tuple, equal_operator_same_types)
{
    using tuple_type = hud::tuple<i32, wchar, hud_test::comparable>;

    const auto test = []()
    {
        return std::tuple{
            hud::tuple<>{} == hud::tuple<>{},
            tuple_type{22, L'a', hud_test::comparable(33)} == tuple_type{22, L'a', hud_test::comparable(33)},
            tuple_type{22, L'a', hud_test::comparable(33)} == tuple_type{22, L'a', hud_test::comparable(44)},
            tuple_type{22, L'a', hud_test::comparable(33)} == tuple_type{22, L'b', hud_test::comparable(33)},
            tuple_type{22, L'a', hud_test::comparable(33)} == tuple_type{11, L'a', hud_test::comparable(33)}};
    };

    // Non constant
    {
        const auto result = test();
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_FALSE(std::get<2>(result));
        GTEST_ASSERT_FALSE(std::get<3>(result));
        GTEST_ASSERT_FALSE(std::get<4>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_FALSE(std::get<2>(result));
        GTEST_ASSERT_FALSE(std::get<3>(result));
        GTEST_ASSERT_FALSE(std::get<4>(result));
    }
}

GTEST_TEST(tuple, equal_operator_different_types)
{
    using tuple_type = hud::tuple<i32, wchar, hud_test::comparable>;

    const auto test = []()
    {
        return std::tuple{
            tuple_type{22, L'a', hud_test::comparable(33)} == hud::tuple<i32, wchar, i32>{22, L'a', 33},
            tuple_type{22, L'a', hud_test::comparable(33)} == hud::tuple<i32, wchar, i32>{22, L'a', 44},
            tuple_type{22, L'a', hud_test::comparable(33)} == hud::tuple<i32, char16, hud_test::comparable>{22, u'a', hud_test::comparable(33)},
            tuple_type{22, L'a', hud_test::comparable(33)} == hud::tuple<i32, char16, hud_test::comparable>{22, u'b', hud_test::comparable(33)},
            tuple_type{22, L'a', hud_test::comparable(33)} == hud::tuple<i16, wchar, hud_test::comparable>{(i16)22, L'a', hud_test::comparable(33)},
            tuple_type{22, L'a', hud_test::comparable(33)} == hud::tuple<i16, wchar, hud_test::comparable>{(i16)11, L'a', hud_test::comparable(33)}};
    };

    // Non constant
    {
        const auto result = test();
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_FALSE(std::get<1>(result));
        GTEST_ASSERT_TRUE(std::get<2>(result));
        GTEST_ASSERT_FALSE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_FALSE(std::get<5>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_FALSE(std::get<1>(result));
        GTEST_ASSERT_TRUE(std::get<2>(result));
        GTEST_ASSERT_FALSE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_FALSE(std::get<5>(result));
    }
}

GTEST_TEST(tuple, not_equal_operator_same_types)
{
    using tuple_type = hud::tuple<i32, wchar, hud_test::comparable>;

    const auto test = []()
    {
        return std::tuple{
            hud::tuple<>{} != hud::tuple<>{},
            tuple_type{22, L'a', hud_test::comparable(33)} != tuple_type{22, L'a', hud_test::comparable(33)},
            tuple_type{22, L'a', hud_test::comparable(33)} != tuple_type{22, L'a', hud_test::comparable(44)},
            tuple_type{22, L'a', hud_test::comparable(33)} != tuple_type{22, L'b', hud_test::comparable(33)},
            tuple_type{22, L'a', hud_test::comparable(33)} != tuple_type{11, L'a', hud_test::comparable(33)}};
    };

    // Non constant
    {
        const auto result = test();
        GTEST_ASSERT_FALSE(std::get<0>(result));
        GTEST_ASSERT_FALSE(std::get<1>(result));
        GTEST_ASSERT_TRUE(std::get<2>(result));
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        GTEST_ASSERT_FALSE(std::get<0>(result));
        GTEST_ASSERT_FALSE(std::get<1>(result));
        GTEST_ASSERT_TRUE(std::get<2>(result));
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
    }
}

GTEST_TEST(tuple, not_equal_operator_different_types)
{
    using tuple_type = hud::tuple<i32, wchar, hud_test::comparable>;

    const auto test = []()
    {
        return std::tuple{
            tuple_type{22, L'a', hud_test::comparable(33)} != hud::tuple<i32, wchar, i32>{22, L'a', 33},
            tuple_type{22, L'a', hud_test::comparable(33)} != hud::tuple<i32, wchar, i32>{22, L'a', 44},
            tuple_type{22, L'a', hud_test::comparable(33)} != hud::tuple<i32, char16, hud_test::comparable>{22, u'a', hud_test::comparable(33)},
            tuple_type{22, L'a', hud_test::comparable(33)} != hud::tuple<i32, char16, hud_test::comparable>{22, u'b', hud_test::comparable(33)},
            tuple_type{22, L'a', hud_test::comparable(33)} != hud::tuple<i16, wchar, hud_test::comparable>{(i16)22, L'a', hud_test::comparable(33)},
            tuple_type{22, L'a', hud_test::comparable(33)} != hud::tuple<i16, wchar, hud_test::comparable>{(i16)11, L'a', hud_test::comparable(33)}};
    };

    // Non constant
    {
        const auto result = test();
        GTEST_ASSERT_FALSE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_FALSE(std::get<2>(result));
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_FALSE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        GTEST_ASSERT_FALSE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_FALSE(std::get<2>(result));
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_FALSE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
    }
}

GTEST_TEST(tuple, less_operator_same_types)
{
    using tuple_type = hud::tuple<i32, wchar, hud_test::comparable>;

    const auto test = []()
    {
        return std::tuple{
            hud::tuple<>{} < hud::tuple<>{},
            tuple_type{22, L'a', hud_test::comparable(33)} < tuple_type{22, L'a', hud_test::comparable(33)},
            tuple_type{22, L'a', hud_test::comparable(33)} < tuple_type{22, L'a', hud_test::comparable(44)},
            tuple_type{22, L'a', hud_test::comparable(33)} < tuple_type{22, L'a', hud_test::comparable(22)},
            tuple_type{22, L'a', hud_test::comparable(33)} < tuple_type{22, L'b', hud_test::comparable(33)},
            tuple_type{22, L'a', hud_test::comparable(33)} < tuple_type{22, L'\0', hud_test::comparable(33)},
            tuple_type{22, L'a', hud_test::comparable(33)} < tuple_type{33, L'a', hud_test::comparable(33)},
            tuple_type{22, L'a', hud_test::comparable(33)} < tuple_type{11, L'a', hud_test::comparable(33)}};
    };

    // Non constant
    {
        const auto result = test();
        GTEST_ASSERT_FALSE(std::get<0>(result));
        GTEST_ASSERT_FALSE(std::get<1>(result));
        GTEST_ASSERT_TRUE(std::get<2>(result));
        GTEST_ASSERT_FALSE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_FALSE(std::get<5>(result));
        GTEST_ASSERT_TRUE(std::get<6>(result));
        GTEST_ASSERT_FALSE(std::get<7>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        GTEST_ASSERT_FALSE(std::get<0>(result));
        GTEST_ASSERT_FALSE(std::get<1>(result));
        GTEST_ASSERT_TRUE(std::get<2>(result));
        GTEST_ASSERT_FALSE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_FALSE(std::get<5>(result));
        GTEST_ASSERT_TRUE(std::get<6>(result));
        GTEST_ASSERT_FALSE(std::get<7>(result));
    }
}

GTEST_TEST(tuple, less_operator_different_types)
{
    using tuple_type = hud::tuple<i32, wchar, hud_test::comparable>;

    const auto test = []()
    {
        return std::tuple{

            hud::tuple<>{} < hud::tuple<>{},
            tuple_type{22, L'a', hud_test::comparable(33)} < hud::tuple<i32, wchar, i32>{22, L'a', 33},
            tuple_type{22, L'a', hud_test::comparable(33)} < hud::tuple<i32, wchar, i32>{22, L'a', 44},
            tuple_type{22, L'a', hud_test::comparable(33)} < hud::tuple<i32, wchar, i32>{22, L'a', 22},
            tuple_type{22, L'a', hud_test::comparable(33)} < hud::tuple<i32, char16, hud_test::comparable>{22, u'a', hud_test::comparable(33)},
            tuple_type{22, L'a', hud_test::comparable(33)} < hud::tuple<i32, char16, hud_test::comparable>{22, u'b', hud_test::comparable(33)},
            tuple_type{22, L'a', hud_test::comparable(33)} < hud::tuple<i32, char16, hud_test::comparable>{22, u'\0', hud_test::comparable(33)},
            tuple_type{22, L'a', hud_test::comparable(33)} < hud::tuple<i16, wchar, hud_test::comparable>{(i16)22, u'a', hud_test::comparable(33)},
            tuple_type{22, L'a', hud_test::comparable(33)} < hud::tuple<i16, wchar, hud_test::comparable>{(i16)33, u'b', hud_test::comparable(33)},
            tuple_type{22, L'a', hud_test::comparable(33)} < hud::tuple<i16, wchar, hud_test::comparable>{(i16)11, u'\0', hud_test::comparable(33)}

        };
    };

    // Non constant
    {
        const auto result = test();
        GTEST_ASSERT_FALSE(std::get<0>(result));
        GTEST_ASSERT_FALSE(std::get<1>(result));
        GTEST_ASSERT_TRUE(std::get<2>(result));
        GTEST_ASSERT_FALSE(std::get<3>(result));
        GTEST_ASSERT_FALSE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_FALSE(std::get<6>(result));
        GTEST_ASSERT_FALSE(std::get<7>(result));
        GTEST_ASSERT_TRUE(std::get<8>(result));
        GTEST_ASSERT_FALSE(std::get<9>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        GTEST_ASSERT_FALSE(std::get<0>(result));
        GTEST_ASSERT_FALSE(std::get<1>(result));
        GTEST_ASSERT_TRUE(std::get<2>(result));
        GTEST_ASSERT_FALSE(std::get<3>(result));
        GTEST_ASSERT_FALSE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_FALSE(std::get<6>(result));
        GTEST_ASSERT_FALSE(std::get<7>(result));
        GTEST_ASSERT_TRUE(std::get<8>(result));
        GTEST_ASSERT_FALSE(std::get<9>(result));
    }
}

GTEST_TEST(tuple, less_equal_operator_same_types)
{
    using tuple_type = hud::tuple<i32, wchar, hud_test::comparable>;

    const auto test = []()
    {
        return std::tuple{
            hud::tuple<>{} <= hud::tuple<>{},
            tuple_type{22, L'a', hud_test::comparable(33)} <= tuple_type{22, L'a', hud_test::comparable(33)},
            tuple_type{22, L'a', hud_test::comparable(33)} <= tuple_type{22, L'a', hud_test::comparable(44)},
            tuple_type{22, L'a', hud_test::comparable(33)} <= tuple_type{22, L'a', hud_test::comparable(22)},
            tuple_type{22, L'a', hud_test::comparable(33)} <= tuple_type{22, L'b', hud_test::comparable(33)},
            tuple_type{22, L'a', hud_test::comparable(33)} <= tuple_type{22, L'\0', hud_test::comparable(33)},
            tuple_type{22, L'a', hud_test::comparable(33)} <= tuple_type{33, L'a', hud_test::comparable(33)},
            tuple_type{22, L'a', hud_test::comparable(33)} <= tuple_type{11, L'a', hud_test::comparable(33)}};
    };

    // Non constant
    {
        const auto result = test();
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_TRUE(std::get<2>(result));
        GTEST_ASSERT_FALSE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_FALSE(std::get<5>(result));
        GTEST_ASSERT_TRUE(std::get<6>(result));
        GTEST_ASSERT_FALSE(std::get<7>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_TRUE(std::get<2>(result));
        GTEST_ASSERT_FALSE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_FALSE(std::get<5>(result));
        GTEST_ASSERT_TRUE(std::get<6>(result));
        GTEST_ASSERT_FALSE(std::get<7>(result));
    }
}

GTEST_TEST(tuple, less_equal_operator_different_types)
{
    using tuple_type = hud::tuple<i32, wchar, hud_test::comparable>;

    const auto test = []()
    {
        return std::tuple{

            hud::tuple<>{} <= hud::tuple<>{},
            tuple_type{22, L'a', hud_test::comparable(33)} <= hud::tuple<i32, wchar, i32>{22, L'a', 33},
            tuple_type{22, L'a', hud_test::comparable(33)} <= hud::tuple<i32, wchar, i32>{22, L'a', 44},
            tuple_type{22, L'a', hud_test::comparable(33)} <= hud::tuple<i32, wchar, i32>{22, L'a', 22},
            tuple_type{22, L'a', hud_test::comparable(33)} <= hud::tuple<i32, char16, hud_test::comparable>{22, u'a', hud_test::comparable(33)},
            tuple_type{22, L'a', hud_test::comparable(33)} <= hud::tuple<i32, char16, hud_test::comparable>{22, u'b', hud_test::comparable(33)},
            tuple_type{22, L'a', hud_test::comparable(33)} <= hud::tuple<i32, char16, hud_test::comparable>{22, u'\0', hud_test::comparable(33)},
            tuple_type{22, L'a', hud_test::comparable(33)} <= hud::tuple<i16, wchar, hud_test::comparable>{(i16)22, L'a', hud_test::comparable(33)},
            tuple_type{22, L'a', hud_test::comparable(33)} <= hud::tuple<i16, wchar, hud_test::comparable>{(i16)33, L'a', hud_test::comparable(33)},
            tuple_type{22, L'a', hud_test::comparable(33)} <= hud::tuple<i16, wchar, hud_test::comparable>{(i16)11, L'a', hud_test::comparable(33)}};
    };

    // Non constant
    {
        const auto result = test();
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_TRUE(std::get<2>(result));
        GTEST_ASSERT_FALSE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_FALSE(std::get<6>(result));
        GTEST_ASSERT_TRUE(std::get<7>(result));
        GTEST_ASSERT_TRUE(std::get<8>(result));
        GTEST_ASSERT_FALSE(std::get<9>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_TRUE(std::get<2>(result));
        GTEST_ASSERT_FALSE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_FALSE(std::get<6>(result));
        GTEST_ASSERT_TRUE(std::get<7>(result));
        GTEST_ASSERT_TRUE(std::get<8>(result));
        GTEST_ASSERT_FALSE(std::get<9>(result));
    }
}

GTEST_TEST(tuple, greater_operator_same_types)
{
    using tuple_type = hud::tuple<i32, wchar, hud_test::comparable>;

    const auto test = []()
    {
        return std::tuple{
            hud::tuple<>{} > hud::tuple<>{},
            tuple_type{22, L'a', hud_test::comparable(33)} > tuple_type{22, L'a', hud_test::comparable(33)},
            tuple_type{22, L'a', hud_test::comparable(33)} > tuple_type{22, L'a', hud_test::comparable(44)},
            tuple_type{22, L'a', hud_test::comparable(33)} > tuple_type{22, L'a', hud_test::comparable(22)},
            tuple_type{22, L'a', hud_test::comparable(33)} > tuple_type{22, L'b', hud_test::comparable(33)},
            tuple_type{22, L'a', hud_test::comparable(33)} > tuple_type{22, L'\0', hud_test::comparable(33)},
            tuple_type{22, L'a', hud_test::comparable(33)} > tuple_type{33, L'a', hud_test::comparable(33)},
            tuple_type{22, L'a', hud_test::comparable(33)} > tuple_type{11, L'a', hud_test::comparable(33)}};
    };

    // Non constant
    {
        const auto result = test();
        GTEST_ASSERT_FALSE(std::get<0>(result));
        GTEST_ASSERT_FALSE(std::get<1>(result));
        GTEST_ASSERT_FALSE(std::get<2>(result));
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_FALSE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_FALSE(std::get<6>(result));
        GTEST_ASSERT_TRUE(std::get<7>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        GTEST_ASSERT_FALSE(std::get<0>(result));
        GTEST_ASSERT_FALSE(std::get<1>(result));
        GTEST_ASSERT_FALSE(std::get<2>(result));
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_FALSE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_FALSE(std::get<6>(result));
        GTEST_ASSERT_TRUE(std::get<7>(result));
    }
}

GTEST_TEST(tuple, greater_equal_operator_different_types)
{
    using tuple_type = hud::tuple<i32, wchar, hud_test::comparable>;

    const auto test = []()
    {
        return std::tuple{

            hud::tuple<>{} >= hud::tuple<>{},
            tuple_type{22, L'a', hud_test::comparable(33)} >= hud::tuple<i32, wchar, i32>{22, L'a', 33},
            tuple_type{22, L'a', hud_test::comparable(33)} >= hud::tuple<i32, wchar, i32>{22, L'a', 44},
            tuple_type{22, L'a', hud_test::comparable(33)} >= hud::tuple<i32, wchar, i32>{22, L'a', 22},
            tuple_type{22, L'a', hud_test::comparable(33)} >= hud::tuple<i32, char16, hud_test::comparable>{22, u'a', hud_test::comparable(33)},
            tuple_type{22, L'a', hud_test::comparable(33)} >= hud::tuple<i32, char16, hud_test::comparable>{22, u'b', hud_test::comparable(33)},
            tuple_type{22, L'a', hud_test::comparable(33)} >= hud::tuple<i32, char16, hud_test::comparable>{22, u'\0', hud_test::comparable(33)},
            tuple_type{22, L'a', hud_test::comparable(33)} >= hud::tuple<i16, wchar, hud_test::comparable>{(i16)22, L'a', hud_test::comparable(33)},
            tuple_type{22, L'a', hud_test::comparable(33)} >= hud::tuple<i16, wchar, hud_test::comparable>{(i16)33, L'a', hud_test::comparable(33)},
            tuple_type{22, L'a', hud_test::comparable(33)} >= hud::tuple<i16, wchar, hud_test::comparable>{(i16)11, L'a', hud_test::comparable(33)}};
    };

    // Non constant
    {
        const auto result = test();
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_FALSE(std::get<2>(result));
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_FALSE(std::get<5>(result));
        GTEST_ASSERT_TRUE(std::get<6>(result));
        GTEST_ASSERT_TRUE(std::get<7>(result));
        GTEST_ASSERT_FALSE(std::get<8>(result));
        GTEST_ASSERT_TRUE(std::get<9>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_FALSE(std::get<2>(result));
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_FALSE(std::get<5>(result));
        GTEST_ASSERT_TRUE(std::get<6>(result));
        GTEST_ASSERT_TRUE(std::get<7>(result));
        GTEST_ASSERT_FALSE(std::get<8>(result));
        GTEST_ASSERT_TRUE(std::get<9>(result));
    }
}