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
} // namespace hud_test

GTEST_TEST(templates, selectDeleterPointerType)
{
    hud_assert_true((hud::is_same_v<hud::select_deleter_pointer_type_t<i32, hud_test::deleter_no_pointer_type>, i32 *>));
    hud_assert_false((hud::is_same_v<hud::select_deleter_pointer_type_t<i32, hud_test::deleter_no_pointer_type>, float *>));
    hud_assert_true((hud::is_same_v<hud::select_deleter_pointer_type_t<i32, hud_test::deleter_pointer_type>, float *>));
    hud_assert_false((hud::is_same_v<hud::select_deleter_pointer_type_t<i32, hud_test::deleter_pointer_type>, i32 *>));
}