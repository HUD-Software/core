#include <core/containers/unique_pointer.h>

TEST(UniquePointer, destructor) {

    const auto test = []() {
        i32 dtor_count = 0;
        {
            hud::UniquePointer<hud::test::NonBitwiseType> ptr(new hud::test::NonBitwiseType(123, &dtor_count));
        }
        return dtor_count == 1;
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(result);
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(result);
    }
}

TEST(UniquePointer, destructor_after_move) {

    const auto test = []() {
        i32 dtor_count = 0;
        {
            hud::UniquePointer<hud::test::NonBitwiseType> ptr(new hud::test::NonBitwiseType(123, &dtor_count));
            hud::UniquePointer<hud::test::NonBitwiseType> other(hud::move(ptr));
        }

        return dtor_count == 1;
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(result);
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(result);
    }
}