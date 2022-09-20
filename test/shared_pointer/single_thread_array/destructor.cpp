#include <core/containers/shared_pointer.h>
#include <array>

TEST(SharedPointer_array_not_safe, destructor) {

    const auto test = []() {
        i32 dtor_count[4] = { 0 };
        {
            hud::SharedPointer<hud::test::NonBitwiseType[4]> ptr(new hud::test::NonBitwiseType[4]{ { 123, &dtor_count[0] }, { 123, &dtor_count[1] }, { 123, &dtor_count[2] }, { 123, &dtor_count[3] } });
            {
                hud::SharedPointer<hud::test::NonBitwiseType[4]> ptr_2(ptr);
            } // dtor_count == 0

        } // dtor_count == 1
        return std::array{
            dtor_count[0] == 1,
            dtor_count[1] == 1,
            dtor_count[2] == 1,
            dtor_count[3] == 1,
        };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
    }

    // Constant
    {
        const auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
    }
}