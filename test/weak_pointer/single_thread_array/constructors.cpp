#include <core/containers/unique_pointer.h>

GTEST_TEST(weak_pointer_array_not_safe, default_constructor)
{

    const auto test = []()
    {
        hud::weak_pointer<hud_test::non_bitwise_type[2]> weak_ptr;
        return std::tuple{
            weak_ptr.lock().pointer() == nullptr,
            weak_ptr.lock().shared_count()};
    };

    // Non constant
    {
        const auto result = test();
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
    }

    // Constant
    {
        constexpr auto result = test();
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
    }
}
