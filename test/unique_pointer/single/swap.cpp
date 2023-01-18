#include <core/containers/unique_pointer.h>

GTEST_TEST(unique_pointer, swap)
{

    // UniquePointer::swap
    {
        const auto test = []()
        {
            hud::unique_pointer<i32> p(new i32(1));
            hud::unique_pointer<i32> p2(new i32(2));
            p.swap(p2);
            return std::tuple {
                *p == 2,
                *p2 == 1};
        };

        // Non constant
        {
            const auto result = test();
            GTEST_ASSERT_TRUE(std::get<0>(result));
            GTEST_ASSERT_TRUE(std::get<1>(result));
        }

        // Constant
        {
            constexpr auto result = test();
            GTEST_ASSERT_TRUE(std::get<0>(result));
            GTEST_ASSERT_TRUE(std::get<1>(result));
        }
    }

    // hud::swap
    {
        const auto test = []()
        {
            hud::unique_pointer<i32> p(new i32(1));
            hud::unique_pointer<i32> p2(new i32(2));
            hud::swap(p, p2);
            return std::tuple {
                *p == 2,
                *p2 == 1};
        };

        // Non constant
        {
            const auto result = test();
            GTEST_ASSERT_TRUE(std::get<0>(result));
            GTEST_ASSERT_TRUE(std::get<1>(result));
        }

        // Constant
        {
            constexpr auto result = test();
            GTEST_ASSERT_TRUE(std::get<0>(result));
            GTEST_ASSERT_TRUE(std::get<1>(result));
        }
    }
}