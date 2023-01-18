#include <core/containers/pair.h>

GTEST_TEST(optional, swap_empty_trivial_type)
{

    // Pair::swap
    {

        const auto test = [](hud::pair<i32, wchar> a, hud::pair<i32, wchar> b)
        {
            static_assert(hud::is_swappable_v<usize, usize>);
            a.swap(b);
            return std::tuple {
                a.first,
                a.second,
                b.first,
                b.second};
        };

        // Non constant
        {
            const auto result = test({1, L'a'}, {2, L'b'});

            GTEST_ASSERT_EQ(std::get<0>(result), 2);
            GTEST_ASSERT_EQ(std::get<1>(result), L'b');
            GTEST_ASSERT_EQ(std::get<2>(result), 1);
            GTEST_ASSERT_EQ(std::get<3>(result), L'a');
        }

        // Constant
        {
            constexpr auto result = test({1, L'a'}, {2, L'b'});

            GTEST_ASSERT_EQ(std::get<0>(result), 2);
            GTEST_ASSERT_EQ(std::get<1>(result), L'b');
            GTEST_ASSERT_EQ(std::get<2>(result), 1);
            GTEST_ASSERT_EQ(std::get<3>(result), L'a');
        }
    }

    // hud::swap
    {
        const auto test = [](hud::pair<i32, wchar> a, hud::pair<i32, wchar> b)
        {
            static_assert(hud::is_swappable_v<usize, usize>);
            swap(a, b);
            return std::tuple {
                a.first,
                a.second,
                b.first,
                b.second};
        };

        // Non constant
        {
            const auto result = test({1, L'a'}, {2, L'b'});

            GTEST_ASSERT_EQ(std::get<0>(result), 2);
            GTEST_ASSERT_EQ(std::get<1>(result), L'b');
            GTEST_ASSERT_EQ(std::get<2>(result), 1);
            GTEST_ASSERT_EQ(std::get<3>(result), L'a');
        }

        // Constant
        {
            constexpr auto result = test({1, L'a'}, {2, L'b'});

            GTEST_ASSERT_EQ(std::get<0>(result), 2);
            GTEST_ASSERT_EQ(std::get<1>(result), L'b');
            GTEST_ASSERT_EQ(std::get<2>(result), 1);
            GTEST_ASSERT_EQ(std::get<3>(result), L'a');
        }
    }
}