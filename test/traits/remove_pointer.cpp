#include <core/traits/remove_pointer.h>


TEST(traits, remove_pointer) {
    ASSERT_TRUE((hud::is_same_v<hud::remove_pointer_t<i32>, i32>));
    ASSERT_TRUE((hud::is_same_v<hud::remove_pointer_t<i32[]>, i32[]>));
    ASSERT_TRUE((hud::is_same_v<hud::remove_pointer_t<i32[10]>, i32[10]>));
    ASSERT_TRUE((hud::is_same_v<hud::remove_pointer_t<i32[][10]>, i32[][10]>));
    ASSERT_TRUE((hud::is_same_v<hud::remove_pointer_t<i32* []>, i32* []>));
    ASSERT_TRUE((hud::is_same_v<hud::remove_pointer_t<i32* [10]>, i32* [10]>));

    ASSERT_TRUE((hud::is_same_v<hud::remove_pointer_t<i32*>, i32>));
    ASSERT_TRUE((hud::is_same_v<hud::remove_pointer_t<i32**>, i32*>));
    ASSERT_TRUE((hud::is_same_v<hud::remove_pointer_t<i32* const>, i32>));
    ASSERT_TRUE((hud::is_same_v<hud::remove_pointer_t<i32* volatile>, i32>));
    ASSERT_TRUE((hud::is_same_v<hud::remove_pointer_t<i32* const volatile>, i32>));
    ASSERT_TRUE((hud::is_same_v<hud::remove_pointer_t<const i32*>, const i32>));
    ASSERT_TRUE((hud::is_same_v<hud::remove_pointer_t<volatile i32*>, volatile i32>));
    ASSERT_TRUE((hud::is_same_v<hud::remove_pointer_t<const volatile i32*>, const volatile i32>));
}