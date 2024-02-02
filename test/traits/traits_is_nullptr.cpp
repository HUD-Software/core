#include <core/traits/is_nullptr.h>

namespace hud_test
{
    [[maybe_unused]] int *a = 0;
}

GTEST_TEST(traits, is_nullptr)
{
    hud_assert_false((hud::is_nullptr_v<decltype(hud_test::a)>));
    hud_assert_false((hud::is_nullptr_v<decltype(0)>));
    hud_assert_false((hud::is_nullptr_v<decltype(&(*hud_test::a))>));
    hud_assert_true((hud::is_nullptr_v<decltype(nullptr)>));
    hud_assert_true((hud::is_nullptr_v<std::nullptr_t>));
    hud_assert_true((hud::is_nullptr_v<hud::ptr::null>));
}