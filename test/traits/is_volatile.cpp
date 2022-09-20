#include <core/traits/is_volatile.h>

TEST(traits, is_volatile) {
    ASSERT_FALSE((hud::is_volatile_v<int>));
    ASSERT_TRUE((hud::is_volatile_v<volatile int>));
    ASSERT_FALSE((hud::is_volatile_v<const int>));
    ASSERT_TRUE((hud::is_volatile_v<const volatile int>));
    ASSERT_FALSE((hud::is_volatile_v<volatile int*>));
    ASSERT_TRUE((hud::is_volatile_v<volatile int* volatile>));
}
