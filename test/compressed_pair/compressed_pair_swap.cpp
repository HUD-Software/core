#include <core/containers/compressed_pair.h>

GTEST_TEST(compressed_pair, swap_empty_trivial_type)
{

    // compressed_pair::swap
    {

        const auto test = [](hud::compressed_pair<i32, wchar> a, hud::compressed_pair<i32, wchar> b)
        {
            static_assert(hud::is_swappable_v<usize, usize>);
            a.swap(b);
            return std::tuple {
                a.first(),
                a.second(),
                b.first(),
                b.second()
            };
        };

        // Non constant
        {
            const auto result = test({1, L'a'}, {2, L'b'});

            hud_assert_eq(std::get<0>(result), 2);
            hud_assert_eq(std::get<1>(result), L'b');
            hud_assert_eq(std::get<2>(result), 1);
            hud_assert_eq(std::get<3>(result), L'a');
        }

        // Constant
        {
            constexpr auto result = test({1, L'a'}, {2, L'b'});

            hud_assert_eq(std::get<0>(result), 2);
            hud_assert_eq(std::get<1>(result), L'b');
            hud_assert_eq(std::get<2>(result), 1);
            hud_assert_eq(std::get<3>(result), L'a');
        }
    }

    // hud::swap
    {
        const auto test = [](hud::compressed_pair<i32, wchar> a, hud::compressed_pair<i32, wchar> b)
        {
            static_assert(hud::is_swappable_v<usize, usize>);
            swap(a, b);
            return std::tuple {
                a.first(),
                a.second(),
                b.first(),
                b.second()
            };
        };

        // Non constant
        {
            const auto result = test({1, L'a'}, {2, L'b'});

            hud_assert_eq(std::get<0>(result), 2);
            hud_assert_eq(std::get<1>(result), L'b');
            hud_assert_eq(std::get<2>(result), 1);
            hud_assert_eq(std::get<3>(result), L'a');
        }

        // Constant
        {
            constexpr auto result = test({1, L'a'}, {2, L'b'});

            hud_assert_eq(std::get<0>(result), 2);
            hud_assert_eq(std::get<1>(result), L'b');
            hud_assert_eq(std::get<2>(result), 1);
            hud_assert_eq(std::get<3>(result), L'a');
        }
    }
}