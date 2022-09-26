#include <core/containers/tuple.h>

namespace {
    struct Comparable {
        constexpr Comparable(i32 j) noexcept
            : i(j) {
        }

        [[nodiscard]]
        constexpr bool operator==(const Comparable& o) const noexcept{
            return i == o.i;
        }
        [[nodiscard]]
        constexpr bool operator==(const i32& o) const noexcept {
            return i == o;
        }
        [[nodiscard]]
        constexpr bool operator<(const Comparable& o) const noexcept {
            return i < o.i;
        }
        [[nodiscard]]
        constexpr bool operator<(const i32& o) const noexcept {
            return i < o;
        }
        i32 i;
    };

    constexpr bool operator<(const i32& left, const Comparable& right) noexcept {
        return left < right.i;
    }
}

TEST(tuple, equal_operator_same_types) {
    using tuple_type = hud::tuple<i32, wchar, Comparable>;

    const auto test = []() {
        return std::tuple{
            hud::tuple<>{} == hud::tuple<>{},
            tuple_type{22, L'a', Comparable(33)} == tuple_type{22, L'a', Comparable(33)},
            tuple_type{ 22, L'a', Comparable(33) } == tuple_type{ 22, L'a', Comparable(44) },
            tuple_type{ 22, L'a', Comparable(33) } == tuple_type{ 22, L'b', Comparable(33) },
            tuple_type{ 22, L'a', Comparable(33) } == tuple_type{ 11, L'a', Comparable(33) }
        };
    };


    // Non constant 
    {
        const auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_FALSE(std::get<2>(result));
        ASSERT_FALSE(std::get<3>(result));
        ASSERT_FALSE(std::get<4>(result));
    }

    // Constant 
    {
        constexpr auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_FALSE(std::get<2>(result));
        ASSERT_FALSE(std::get<3>(result));
        ASSERT_FALSE(std::get<4>(result));
    }
}

TEST(tuple, equal_operator_different_types) {
    using tuple_type = hud::tuple<i32, wchar, Comparable>;

    const auto test = []() {
        return std::tuple{
            tuple_type{22, L'a', Comparable(33)} == hud::tuple<i32, wchar, i32>{22, L'a', 33},
            tuple_type{ 22, L'a', Comparable(33) } == hud::tuple<i32, wchar, i32>{22, L'a', 44},
            tuple_type{ 22, L'a', Comparable(33) } == hud::tuple<i32, char16, Comparable>{ 22, u'a', Comparable(33) },
            tuple_type{ 22, L'a', Comparable(33) } == hud::tuple<i32, char16, Comparable>{ 22, u'b', Comparable(33) },
            tuple_type{ 22, L'a', Comparable(33) } == hud::tuple<i16, wchar, Comparable>{ (i16)22, L'a', Comparable(33) },
            tuple_type{ 22, L'a', Comparable(33) } == hud::tuple<i16, wchar, Comparable>{ (i16)11, L'a', Comparable(33) }
        };
    };

    // Non constant 
    {
        const auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_FALSE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_FALSE(std::get<3>(result));
        ASSERT_TRUE(std::get<4>(result));
        ASSERT_FALSE(std::get<5>(result));
    }

    // Constant 
    {
        constexpr auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_FALSE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_FALSE(std::get<3>(result));
        ASSERT_TRUE(std::get<4>(result));
        ASSERT_FALSE(std::get<5>(result));
    }
}

TEST(tuple, not_equal_operator_same_types) {
    using tuple_type = hud::tuple<i32, wchar, Comparable>;

    const auto test = []() {
        return std::tuple{
            hud::tuple<>{} != hud::tuple<>{},
            tuple_type{22, L'a', Comparable(33)} != tuple_type{22, L'a', Comparable(33)},
            tuple_type{ 22, L'a', Comparable(33) } != tuple_type{ 22, L'a', Comparable(44) },
            tuple_type{ 22, L'a', Comparable(33) } != tuple_type{ 22, L'b', Comparable(33) },
            tuple_type{ 22, L'a', Comparable(33) } != tuple_type{ 11, L'a', Comparable(33) }
        };
    };


    // Non constant 
    {
        const auto result = test();
        ASSERT_FALSE(std::get<0>(result));
        ASSERT_FALSE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
        ASSERT_TRUE(std::get<4>(result));
    }

    // Constant 
    {
        constexpr auto result = test();
        ASSERT_FALSE(std::get<0>(result));
        ASSERT_FALSE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
        ASSERT_TRUE(std::get<4>(result));
    }
}

TEST(tuple, not_equal_operator_different_types) {
    using tuple_type = hud::tuple<i32, wchar, Comparable>;

    const auto test = []() {
        return std::tuple{
            tuple_type{22, L'a', Comparable(33)} != hud::tuple<i32, wchar, i32>{22, L'a', 33},
            tuple_type{ 22, L'a', Comparable(33) } != hud::tuple<i32, wchar, i32>{22, L'a', 44},
            tuple_type{ 22, L'a', Comparable(33) } != hud::tuple<i32, char16, Comparable>{ 22, u'a', Comparable(33) },
            tuple_type{ 22, L'a', Comparable(33) } != hud::tuple<i32, char16, Comparable>{ 22, u'b', Comparable(33) },
            tuple_type{ 22, L'a', Comparable(33) } != hud::tuple<i16, wchar, Comparable>{ (i16)22, L'a', Comparable(33) },
            tuple_type{ 22, L'a', Comparable(33) } != hud::tuple<i16, wchar, Comparable>{ (i16)11, L'a', Comparable(33) }
        };
    };

    // Non constant 
    {
        const auto result = test();
        ASSERT_FALSE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_FALSE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
        ASSERT_FALSE(std::get<4>(result));
        ASSERT_TRUE(std::get<5>(result));
    }

    // Constant 
    {
        constexpr auto result = test();
        ASSERT_FALSE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_FALSE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
        ASSERT_FALSE(std::get<4>(result));
        ASSERT_TRUE(std::get<5>(result));
    }
}


TEST(tuple, less_operator_same_types) {
    using tuple_type = hud::tuple<i32, wchar, Comparable>;

    const auto test = []() {
        return std::tuple{
            hud::tuple<>{} < hud::tuple<>{},
            tuple_type{ 22, L'a', Comparable(33) } < tuple_type{ 22, L'a', Comparable(33) },
            tuple_type{ 22, L'a', Comparable(33) } < tuple_type{ 22, L'a', Comparable(44) },
            tuple_type{ 22, L'a', Comparable(33) } < tuple_type{ 22, L'a', Comparable(22) },
            tuple_type{ 22, L'a', Comparable(33) } < tuple_type{ 22, L'b', Comparable(33) },
            tuple_type{ 22, L'a', Comparable(33) } < tuple_type{ 22, L'\0', Comparable(33) },
            tuple_type{ 22, L'a', Comparable(33) } < tuple_type{ 33, L'a', Comparable(33) },
            tuple_type{ 22, L'a', Comparable(33) } < tuple_type{ 11, L'a', Comparable(33) }
        };
    };


    // Non constant 
    {
        const auto result = test();
        ASSERT_FALSE(std::get<0>(result));
        ASSERT_FALSE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_FALSE(std::get<3>(result));
        ASSERT_TRUE(std::get<4>(result));
        ASSERT_FALSE(std::get<5>(result));
        ASSERT_TRUE(std::get<6>(result));
        ASSERT_FALSE(std::get<7>(result));
    }

    // Constant 
    {
        constexpr auto result = test();
        ASSERT_FALSE(std::get<0>(result));
        ASSERT_FALSE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_FALSE(std::get<3>(result));
        ASSERT_TRUE(std::get<4>(result));
        ASSERT_FALSE(std::get<5>(result));
        ASSERT_TRUE(std::get<6>(result));
        ASSERT_FALSE(std::get<7>(result));
    }
}


TEST(tuple, less_operator_different_types) {
    using tuple_type = hud::tuple<i32, wchar, Comparable>;

    const auto test = []() {
        return std::tuple{

            hud::tuple<>{} < hud::tuple<>{},
            tuple_type{ 22, L'a', Comparable(33) } < hud::tuple<i32, wchar, i32>{ 22, L'a', 33 },
            tuple_type{ 22, L'a', Comparable(33) } < hud::tuple<i32, wchar, i32>{ 22, L'a', 44 },
            tuple_type{ 22, L'a', Comparable(33) } < hud::tuple<i32, wchar, i32>{ 22, L'a', 22 },
            tuple_type{ 22, L'a', Comparable(33) } < hud::tuple<i32, char16, Comparable>{ 22, u'a', Comparable(33) },
            tuple_type{ 22, L'a', Comparable(33) } < hud::tuple<i32, char16, Comparable>{ 22, u'b', Comparable(33) },
            tuple_type{ 22, L'a', Comparable(33) } < hud::tuple<i32, char16, Comparable>{ 22, u'\0', Comparable(33) },
            tuple_type{ 22, L'a', Comparable(33) } < hud::tuple<i16, wchar, Comparable>{ (i16)22, u'a', Comparable(33) },
            tuple_type{ 22, L'a', Comparable(33) } < hud::tuple<i16, wchar, Comparable>{ (i16)33, u'b', Comparable(33) },
            tuple_type{ 22, L'a', Comparable(33) } < hud::tuple<i16, wchar, Comparable>{ (i16)11, u'\0', Comparable(33) }
           
        };
    };

    // Non constant 
    {
        const auto result = test();
        ASSERT_FALSE(std::get<0>(result));
        ASSERT_FALSE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_FALSE(std::get<3>(result));
        ASSERT_FALSE(std::get<4>(result));
        ASSERT_TRUE(std::get<5>(result));
        ASSERT_FALSE(std::get<6>(result));
        ASSERT_FALSE(std::get<7>(result));
        ASSERT_TRUE(std::get<8>(result));
        ASSERT_FALSE(std::get<9>(result));
    }

    // Constant 
    {
        constexpr auto result = test();
        ASSERT_FALSE(std::get<0>(result));
        ASSERT_FALSE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_FALSE(std::get<3>(result));
        ASSERT_FALSE(std::get<4>(result));
        ASSERT_TRUE(std::get<5>(result));
        ASSERT_FALSE(std::get<6>(result));
        ASSERT_FALSE(std::get<7>(result));
        ASSERT_TRUE(std::get<8>(result));
        ASSERT_FALSE(std::get<9>(result));
    }
}

TEST(tuple, less_equal_operator_same_types) {
    using tuple_type = hud::tuple<i32, wchar, Comparable>;

    const auto test = []() {
        return std::tuple{
            hud::tuple<>{} <= hud::tuple<>{},
            tuple_type{ 22, L'a', Comparable(33) } <= tuple_type{ 22, L'a', Comparable(33) },
            tuple_type{ 22, L'a', Comparable(33) } <= tuple_type{ 22, L'a', Comparable(44) },
            tuple_type{ 22, L'a', Comparable(33) } <= tuple_type{ 22, L'a', Comparable(22) },
            tuple_type{ 22, L'a', Comparable(33) } <= tuple_type{ 22, L'b', Comparable(33) },
            tuple_type{ 22, L'a', Comparable(33) } <= tuple_type{ 22, L'\0', Comparable(33) },
            tuple_type{ 22, L'a', Comparable(33) } <= tuple_type{ 33, L'a', Comparable(33) },
            tuple_type{ 22, L'a', Comparable(33) } <= tuple_type{ 11, L'a', Comparable(33) }
        };
    };


    // Non constant 
    {
        const auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_FALSE(std::get<3>(result));
        ASSERT_TRUE(std::get<4>(result));
        ASSERT_FALSE(std::get<5>(result));
        ASSERT_TRUE(std::get<6>(result));
        ASSERT_FALSE(std::get<7>(result));
    }

    // Constant 
    {
        constexpr auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_FALSE(std::get<3>(result));
        ASSERT_TRUE(std::get<4>(result));
        ASSERT_FALSE(std::get<5>(result));
        ASSERT_TRUE(std::get<6>(result));
        ASSERT_FALSE(std::get<7>(result));
    }
}

TEST(tuple, less_equal_operator_different_types) {
    using tuple_type = hud::tuple<i32, wchar, Comparable>;

    const auto test = []() {
        return std::tuple{

            hud::tuple<>{} <= hud::tuple<>{},
            tuple_type{ 22, L'a', Comparable(33) } <= hud::tuple<i32, wchar, i32>{ 22, L'a', 33 },
            tuple_type{ 22, L'a', Comparable(33) } <= hud::tuple<i32, wchar, i32>{ 22, L'a', 44 },
            tuple_type{ 22, L'a', Comparable(33) } <= hud::tuple<i32, wchar, i32>{ 22, L'a', 22 },
            tuple_type{ 22, L'a', Comparable(33) } <= hud::tuple<i32, char16, Comparable>{ 22, u'a', Comparable(33) },
            tuple_type{ 22, L'a', Comparable(33) } <= hud::tuple<i32, char16, Comparable>{ 22, u'b', Comparable(33) },
            tuple_type{ 22, L'a', Comparable(33) } <= hud::tuple<i32, char16, Comparable>{ 22, u'\0', Comparable(33) },
            tuple_type{ 22, L'a', Comparable(33) } <= hud::tuple<i16, wchar, Comparable>{ (i16)22, L'a', Comparable(33) },
            tuple_type{ 22, L'a', Comparable(33) } <= hud::tuple<i16, wchar, Comparable>{ (i16)33, L'a', Comparable(33) },
            tuple_type{ 22, L'a', Comparable(33) } <= hud::tuple<i16, wchar, Comparable>{ (i16)11, L'a', Comparable(33) }
        };
    };

    // Non constant 
    {
        const auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_FALSE(std::get<3>(result));
        ASSERT_TRUE(std::get<4>(result));
        ASSERT_TRUE(std::get<5>(result));
        ASSERT_FALSE(std::get<6>(result));
        ASSERT_TRUE(std::get<7>(result));
        ASSERT_TRUE(std::get<8>(result));
        ASSERT_FALSE(std::get<9>(result));
    }

    // Constant 
    {
        constexpr auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_FALSE(std::get<3>(result));
        ASSERT_TRUE(std::get<4>(result));
        ASSERT_TRUE(std::get<5>(result));
        ASSERT_FALSE(std::get<6>(result));
        ASSERT_TRUE(std::get<7>(result));
        ASSERT_TRUE(std::get<8>(result));
        ASSERT_FALSE(std::get<9>(result));
    }
}

TEST(tuple, greater_operator_same_types) {
    using tuple_type = hud::tuple<i32, wchar, Comparable>;

    const auto test = []() {
        return std::tuple{
            hud::tuple<>{} > hud::tuple<>{},
            tuple_type{ 22, L'a', Comparable(33) } > tuple_type{ 22, L'a', Comparable(33) },
            tuple_type{ 22, L'a', Comparable(33) } > tuple_type{ 22, L'a', Comparable(44) },
            tuple_type{ 22, L'a', Comparable(33) } > tuple_type{ 22, L'a', Comparable(22) },
            tuple_type{ 22, L'a', Comparable(33) } > tuple_type{ 22, L'b', Comparable(33) },
            tuple_type{ 22, L'a', Comparable(33) } > tuple_type{ 22, L'\0', Comparable(33) },
            tuple_type{ 22, L'a', Comparable(33) } > tuple_type{ 33, L'a', Comparable(33) },
            tuple_type{ 22, L'a', Comparable(33) } > tuple_type{ 11, L'a', Comparable(33) }
        };
    };


    // Non constant 
    {
        const auto result = test();
        ASSERT_FALSE(std::get<0>(result));
        ASSERT_FALSE(std::get<1>(result));
        ASSERT_FALSE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
        ASSERT_FALSE(std::get<4>(result));
        ASSERT_TRUE(std::get<5>(result));
        ASSERT_FALSE(std::get<6>(result));
        ASSERT_TRUE(std::get<7>(result));
    }

    // Constant 
    {
        constexpr auto result = test();
        ASSERT_FALSE(std::get<0>(result));
        ASSERT_FALSE(std::get<1>(result));
        ASSERT_FALSE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
        ASSERT_FALSE(std::get<4>(result));
        ASSERT_TRUE(std::get<5>(result));
        ASSERT_FALSE(std::get<6>(result));
        ASSERT_TRUE(std::get<7>(result));
    }
}

TEST(tuple, greater_equal_operator_different_types) {
    using tuple_type = hud::tuple<i32, wchar, Comparable>;

    const auto test = []() {
        return std::tuple{

            hud::tuple<>{} >= hud::tuple<>{},
            tuple_type{ 22, L'a', Comparable(33) } >= hud::tuple<i32, wchar, i32>{ 22, L'a', 33 },
            tuple_type{ 22, L'a', Comparable(33) } >= hud::tuple<i32, wchar, i32>{ 22, L'a', 44 },
            tuple_type{ 22, L'a', Comparable(33) } >= hud::tuple<i32, wchar, i32>{ 22, L'a', 22 },
            tuple_type{ 22, L'a', Comparable(33) } >= hud::tuple<i32, char16, Comparable>{ 22, u'a', Comparable(33) },
            tuple_type{ 22, L'a', Comparable(33) } >= hud::tuple<i32, char16, Comparable>{ 22, u'b', Comparable(33) },
            tuple_type{ 22, L'a', Comparable(33) } >= hud::tuple<i32, char16, Comparable>{ 22, u'\0', Comparable(33) },
            tuple_type{ 22, L'a', Comparable(33) } >= hud::tuple<i16, wchar, Comparable>{ (i16)22, L'a', Comparable(33) },
            tuple_type{ 22, L'a', Comparable(33) } >= hud::tuple<i16, wchar, Comparable>{ (i16)33, L'a', Comparable(33) },
            tuple_type{ 22, L'a', Comparable(33) } >= hud::tuple<i16, wchar, Comparable>{ (i16)11, L'a', Comparable(33) }
        };
    };

    // Non constant 
    {
        const auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_FALSE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
        ASSERT_TRUE(std::get<4>(result));
        ASSERT_FALSE(std::get<5>(result));
        ASSERT_TRUE(std::get<6>(result));
        ASSERT_TRUE(std::get<7>(result));
        ASSERT_FALSE(std::get<8>(result));
        ASSERT_TRUE(std::get<9>(result));
    }

    // Constant 
    {
        constexpr auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_FALSE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
        ASSERT_TRUE(std::get<4>(result));
        ASSERT_FALSE(std::get<5>(result));
        ASSERT_TRUE(std::get<6>(result));
        ASSERT_TRUE(std::get<7>(result));
        ASSERT_FALSE(std::get<8>(result));
        ASSERT_TRUE(std::get<9>(result));
    }
}