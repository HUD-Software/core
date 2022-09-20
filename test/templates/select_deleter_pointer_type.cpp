#include <core/templates/select_deleter_pointer_type.h>
#include <core/traits/is_same.h>

namespace {
    struct deleter_no_pointer_type {
    };
    struct deleter_pointer_type {
        using pointer_type = float*;
    };
}

TEST(templates, selectDeleterPointerType)
{
    ASSERT_TRUE((hud::is_same_v<hud::select_deleter_pointer_type_t<i32, deleter_no_pointer_type>, i32*>) );
    ASSERT_FALSE((hud::is_same_v<hud::select_deleter_pointer_type_t<i32, deleter_no_pointer_type>, float*>));
    ASSERT_TRUE((hud::is_same_v<hud::select_deleter_pointer_type_t<i32, deleter_pointer_type>, float*>));
    ASSERT_FALSE((hud::is_same_v<hud::select_deleter_pointer_type_t<i32, deleter_pointer_type>, i32*>));
}