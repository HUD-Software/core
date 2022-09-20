#include <core/containers/unique_pointer.h>

TEST(UniquePointer_array, destructor) {

    const auto test = []() {
        i32 dtor_count[2] = { 0, 0 };
        {
            hud::UniquePointer<hud::test::NonBitwiseType[]> ptr(new hud::test::NonBitwiseType[2]{ {123, &dtor_count[0]}, {456, &dtor_count[1]} });
        }
        return std::tuple{
            dtor_count[0] == 1,
            dtor_count[1] == 1
        };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
    }
}

TEST(UniquePointer_array, destructor_after_move) {

    const auto test = []() {
        i32 dtor_count[2] = { 0, 0 };
        {
            hud::UniquePointer<hud::test::NonBitwiseType[]> ptr(new hud::test::NonBitwiseType[2]{ {123, &dtor_count[0]}, {456, &dtor_count[1]} });
            hud::UniquePointer<hud::test::NonBitwiseType[]> other(hud::move(ptr));
        }
        return std::tuple{
            dtor_count[0] == 1,
            dtor_count[1] == 1
        };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
    }
}