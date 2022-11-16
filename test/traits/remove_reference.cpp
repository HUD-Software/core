#include <core/traits/remove_reference.h>


GTEST_TEST(traits, remove_reference) {
    ASSERT_TRUE((hud::is_same_v<hud::remove_reference_t<i32>, i32>));
    ASSERT_TRUE((hud::is_same_v<hud::remove_reference_t<i32[]>, i32[]>));
    ASSERT_TRUE((hud::is_same_v<hud::remove_reference_t<i32[10]>, i32[10]>));
    ASSERT_TRUE((hud::is_same_v<hud::remove_reference_t<i32*>, i32*>));

    ASSERT_TRUE((hud::is_same_v<hud::remove_reference_t<i32&>, i32>));
    ASSERT_TRUE((hud::is_same_v<hud::remove_reference_t<i32&&>, i32>));
    ASSERT_TRUE((hud::is_same_v<hud::remove_reference_t<const i32&>, const i32>));
    ASSERT_TRUE((hud::is_same_v<hud::remove_reference_t<volatile i32&>, volatile i32>));
    ASSERT_TRUE((hud::is_same_v<hud::remove_reference_t<const volatile i32&>, const volatile i32>));
    ASSERT_TRUE((hud::is_same_v<hud::remove_reference_t<const i32&&>, const i32>));
    ASSERT_TRUE((hud::is_same_v<hud::remove_reference_t<volatile i32&&>, volatile i32>));
    ASSERT_TRUE((hud::is_same_v<hud::remove_reference_t<const volatile i32&&>, const volatile i32>));
}