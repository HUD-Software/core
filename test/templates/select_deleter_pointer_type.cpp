#include <core/templates/select_deleter_pointer_type.h>
#include <core/traits/is_same.h>

namespace hud_test
{
    struct deleter_no_pointer_type
    {
    };
    struct deleter_pointer_type
    {
        using pointer_type = float *;
    };
}

GTEST_TEST(templates, selectDeleterPointerType)
{
    GTEST_ASSERT_TRUE((hud::is_same_v<hud::select_deleter_pointer_type_t<i32, hud_test::deleter_no_pointer_type>, i32 *>));
    GTEST_ASSERT_FALSE((hud::is_same_v<hud::select_deleter_pointer_type_t<i32, hud_test::deleter_no_pointer_type>, float *>));
    GTEST_ASSERT_TRUE((hud::is_same_v<hud::select_deleter_pointer_type_t<i32, hud_test::deleter_pointer_type>, float *>));
    GTEST_ASSERT_FALSE((hud::is_same_v<hud::select_deleter_pointer_type_t<i32, hud_test::deleter_pointer_type>, i32 *>));
}