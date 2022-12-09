#include <core/containers/weak_pointer.h>

#include <memory>

GTEST_TEST(weak_pointer_not_safe, default_constructor) {
    std::shared_ptr<int> shared;
    std::weak_ptr<int> weak(shared);
    hud::shared_pointer<int> hud_shared;
    hud::weak_pointer<int> hud_weak(hud_shared);
    hud::shared_pointer<int> locked = hud_weak.lock();
//     const auto test = []()
//     {
//         hud::weak_pointer<hud_test::non_bitwise_type> weak_ptr;
//         return std::tuple{
//             weak_ptr.pointer() == nullptr,
//             weak_ptr.shared_count()
//         };
//     };

//     // Non constant
//     {
//         const auto result = test();
//         GTEST_ASSERT_TRUE(std::get<0>(result));
//         GTEST_ASSERT_EQ(std::get<1>(result), 0u);
//     }

//     // Constant
//     {
//         constexpr auto result = test();
//         GTEST_ASSERT_TRUE(std::get<0>(result));
//         GTEST_ASSERT_EQ(std::get<1>(result), 0u);
//     }
}