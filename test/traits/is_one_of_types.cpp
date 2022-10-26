#include <core/traits/is_one_of_types.h>

TEST(traits, is_one_of_types) {
    ASSERT_TRUE((hud::is_one_of_types_v<bool, int, long, float, short, bool, wchar_t>));
    ASSERT_TRUE((hud::is_one_of_types_v<int, int, long, float, short, bool, wchar_t>));
    ASSERT_TRUE((hud::is_one_of_types_v<wchar_t, int, long, float, short, bool, wchar_t>));
    ASSERT_FALSE((hud::is_one_of_types_v<bool, int, long, float, short, wchar_t>));
}