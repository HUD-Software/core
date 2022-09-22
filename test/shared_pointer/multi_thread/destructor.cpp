#include <core/containers/shared_pointer.h>

TEST(shared_pointer__safe, destructor) {

    const auto test = []() {
        i32 dtor_count = 0;
        {
            hud::shared_pointer<hud_test::non_bitwise_type, hud::thread_safety_e::safe> ptr(new hud_test::non_bitwise_type(123, &dtor_count));
            {
                hud::shared_pointer<hud_test::non_bitwise_type, hud::thread_safety_e::safe> ptr_2(ptr);
            } // dtor_count == 0

        } // dtor_count == 1
        return dtor_count;
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(result);
    }

    // Constant
    {
        const auto result = test();
        ASSERT_TRUE(result);
    }
}