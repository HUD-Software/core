#include <core/containers/unique_pointer.h>

GTEST_TEST(unique_pointer, destructor)
{

    const auto test = []()
    {
        i32 dtor_count = 0;
        {
            hud::unique_pointer<hud_test::non_bitwise_type> ptr(new hud_test::non_bitwise_type(123, &dtor_count));
        }
        return dtor_count == 1;
    };

    // Non constant
    {
        const auto result = test();
        GTEST_ASSERT_TRUE(result);
    }

    // Constant
    {
        constexpr auto result = test();
        GTEST_ASSERT_TRUE(result);
    }
}

GTEST_TEST(unique_pointer, destructor_after_move)
{

    const auto test = []()
    {
        i32 dtor_count = 0;
        {
            hud::unique_pointer<hud_test::non_bitwise_type> ptr(new hud_test::non_bitwise_type(123, &dtor_count));
            hud::unique_pointer<hud_test::non_bitwise_type> other(hud::move(ptr));
        }

        return dtor_count == 1;
    };

    // Non constant
    {
        const auto result = test();
        GTEST_ASSERT_TRUE(result);
    }

    // Constant
    {
        constexpr auto result = test();
        GTEST_ASSERT_TRUE(result);
    }
}