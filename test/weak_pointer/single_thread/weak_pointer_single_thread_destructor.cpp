#include <core/containers/shared_pointer.h>

GTEST_TEST(weak_pointer_not_safe, destructor)
{
    const auto test = []()
    {
        i32 dtor_count = 0;
        hud::weak_pointer<hud_test::non_bitwise_type> weak_ptr;
        {
            hud::shared_pointer<hud_test::non_bitwise_type> shared_ptr(new hud_test::non_bitwise_type(123, &dtor_count));
            weak_ptr = shared_ptr;
            {
                hud::shared_pointer<hud_test::non_bitwise_type> shared_ptr_2(shared_ptr);
            } // dtor_count == 0

        } // dtor_count == 1
        return dtor_count == 1;
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