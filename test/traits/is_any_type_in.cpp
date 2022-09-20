#include <core/traits/is_any_type_in.h>

TEST(traits, is_any_of) {
    ASSERT_TRUE((hud::is_any_type_in_v<bool, int, long, float, short, bool, wchar_t>));
    ASSERT_TRUE((hud::is_any_type_in_v<int, int, long, float, short, bool, wchar_t>));
    ASSERT_TRUE((hud::is_any_type_in_v<wchar_t, int, long, float, short, bool, wchar_t>));
    ASSERT_FALSE((hud::is_any_type_in_v<bool, int, long, float, short, wchar_t>));
}