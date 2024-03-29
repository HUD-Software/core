#include <core/containers/shared_pointer.h>

GTEST_TEST(weak_pointer_array_safe, destructor)
{
    const auto test = []()
    {
        i32 dtor_count[2] = {0, 0};
        hud::weak_pointer<hud_test::non_bitwise_type[2], hud::thread_safety_e::safe> weak_ptr;
        {
            hud::shared_pointer<hud_test::non_bitwise_type[2], hud::thread_safety_e::safe> shared_ptr(new hud_test::non_bitwise_type[2] {
                {1, &dtor_count[0]},
                {2, &dtor_count[1]}
            });
            weak_ptr = shared_ptr;
            {
                hud::shared_pointer<hud_test::non_bitwise_type[2], hud::thread_safety_e::safe> shared_ptr_2(shared_ptr);
            } // dtor_count[0] == 0 && dtor_count[1] == 0

        } // dtor_count[0] == 1 && dtor_count[1] == 1
        return dtor_count[0] == 1 && dtor_count[1] == 1;
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(result);
    }

    // Constant
    {
        const auto result = test();
        hud_assert_true(result);
    }
}