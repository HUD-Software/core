#include <core/containers/unique_pointer.h>

GTEST_TEST(unique_pointer_array, destructor)
{

    const auto test = []()
    {
        i32 dtor_count[2] = {0, 0};
        {
            hud::unique_pointer<hud_test::non_bitwise_type[]> ptr(new hud_test::non_bitwise_type[2] {
                {123, &dtor_count[0]},
                {456, &dtor_count[1]}
            });
        }
        return std::tuple {
            dtor_count[0] == 1,
            dtor_count[1] == 1};
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
    }
}

GTEST_TEST(unique_pointer_array, destructor_after_move)
{

    const auto test = []()
    {
        i32 dtor_count[2] = {0, 0};
        {
            hud::unique_pointer<hud_test::non_bitwise_type[]> ptr(new hud_test::non_bitwise_type[2] {
                {123, &dtor_count[0]},
                {456, &dtor_count[1]}
            });
            hud::unique_pointer<hud_test::non_bitwise_type[]> other(hud::move(ptr));
        }
        return std::tuple {
            dtor_count[0] == 1,
            dtor_count[1] == 1};
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
    }
}