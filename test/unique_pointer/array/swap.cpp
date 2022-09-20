#include <core/containers/unique_pointer.h>

TEST(UniquePointer_array, swap) {

    // UniquePointer::swap
    {
        const auto test = []() {
            hud::UniquePointer<i32[]> p(new i32[2]{ 1, 2 });
            hud::UniquePointer<i32[]> p2(new i32[2]{ 3, 4});
            p.swap(p2);
            return std::tuple{
                p[0] == 3,
                p[1] == 4,
                p2[0] == 1,
                p2[1] == 2,
            };
        };

        // Non constant
        {
            const auto result = test();
            ASSERT_TRUE(std::get<0>(result));
            ASSERT_TRUE(std::get<1>(result));
            ASSERT_TRUE(std::get<2>(result));
            ASSERT_TRUE(std::get<3>(result));
        }

        // Constant
        {
            constexpr auto result = test();
            ASSERT_TRUE(std::get<0>(result));
            ASSERT_TRUE(std::get<1>(result));
            ASSERT_TRUE(std::get<2>(result));
            ASSERT_TRUE(std::get<3>(result));
        }
    }


    // hud::swap
    {
        const auto test = []() {
            hud::UniquePointer<i32[]> p(new i32[2]{ 1, 2 });
            hud::UniquePointer<i32[]> p2(new i32[2]{ 3, 4 });
            hud::swap(p, p2);
            return std::tuple{
                p[0] == 3,
                p[1] == 4,
                p2[0] == 1,
                p2[1] == 2,
            };
        };

        // Non constant
        {
            const auto result = test();
            ASSERT_TRUE(std::get<0>(result));
            ASSERT_TRUE(std::get<1>(result));
            ASSERT_TRUE(std::get<2>(result));
            ASSERT_TRUE(std::get<3>(result));
        }

        // Constant
        {
            constexpr auto result = test();
            ASSERT_TRUE(std::get<0>(result));
            ASSERT_TRUE(std::get<1>(result));
            ASSERT_TRUE(std::get<2>(result));
            ASSERT_TRUE(std::get<3>(result));
        }
    }

}