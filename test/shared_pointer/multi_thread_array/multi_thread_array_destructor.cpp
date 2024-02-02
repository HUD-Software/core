#include <core/containers/shared_pointer.h>
#include <array>

GTEST_TEST(shared_pointer_array_safe, destructor)
{

    const auto test = []()
    {
        i32 dtor_count[4] = {0};
        {
            hud::shared_pointer<hud_test::non_bitwise_type[4], hud::thread_safety_e::safe> ptr(new hud_test::non_bitwise_type[4] {
                {123, &dtor_count[0]},
                {123, &dtor_count[1]},
                {123, &dtor_count[2]},
                {123, &dtor_count[3]}
            });
            {
                hud::shared_pointer<hud_test::non_bitwise_type[4], hud::thread_safety_e::safe> ptr_2(ptr);
            } // dtor_count == 0

        } // dtor_count == 1
        return std::array {
            dtor_count[0] == 1,
            dtor_count[1] == 1,
            dtor_count[2] == 1,
            dtor_count[3] == 1,
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
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
    }
}