#include <core/containers/shared_pointer.h>

GTEST_TEST(shared_pointer_not_safe, destructor)
{

    const auto test = []()
    {
        i32 dtor_count = 0;
        {
            hud::shared_pointer<hud_test::non_bitwise_type> ptr(new hud_test::non_bitwise_type(123, &dtor_count));
            {
                hud::shared_pointer<hud_test::non_bitwise_type> ptr_2(ptr);
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