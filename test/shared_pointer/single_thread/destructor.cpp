#include <core/containers/shared_pointer.h>

TEST(SharedPointer_not_safe, destructor) {

    const auto test = []() {
        i32 dtor_count = 0;
        {
            hud::SharedPointer<hud::test::NonBitwiseType> ptr(new hud::test::NonBitwiseType(123, &dtor_count));
            {
                hud::SharedPointer<hud::test::NonBitwiseType> ptr_2(ptr);
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