#include <core/containers/unique_pointer.h>

GTEST_TEST(unique_pointer_array, swap)
{

    // UniquePointer::swap
    {
        const auto test = []()
        {
            hud::unique_pointer<i32[]> p(new i32[2] {1, 2});
            hud::unique_pointer<i32[]> p2(new i32[2] {3, 4});
            p.swap(p2);
            return std::tuple {
                p[0] == 3,
                p[1] == 4,
                p2[0] == 1,
                p2[1] == 2,
            };
        };

        // Non constant
        {
            const auto result = test();
            hud_assert_true(std::get<0>(result));
            hud_assert_true(std::get<1>(result));
            hud_assert_true(std::get<2>(result));
            hud_assert_true(std::get<3>(result));
        }

        // Constant
        {
            constexpr auto result = test();
            hud_assert_true(std::get<0>(result));
            hud_assert_true(std::get<1>(result));
            hud_assert_true(std::get<2>(result));
            hud_assert_true(std::get<3>(result));
        }
    }

    // hud::swap
    {
        const auto test = []()
        {
            hud::unique_pointer<i32[]> p(new i32[2] {1, 2});
            hud::unique_pointer<i32[]> p2(new i32[2] {3, 4});
            hud::swap(p, p2);
            return std::tuple {
                p[0] == 3,
                p[1] == 4,
                p2[0] == 1,
                p2[1] == 2,
            };
        };

        // Non constant
        {
            const auto result = test();
            hud_assert_true(std::get<0>(result));
            hud_assert_true(std::get<1>(result));
            hud_assert_true(std::get<2>(result));
            hud_assert_true(std::get<3>(result));
        }

        // Constant
        {
            constexpr auto result = test();
            hud_assert_true(std::get<0>(result));
            hud_assert_true(std::get<1>(result));
            hud_assert_true(std::get<2>(result));
            hud_assert_true(std::get<3>(result));
        }
    }
}