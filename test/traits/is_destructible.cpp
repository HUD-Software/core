#include <core/traits/is_destructible.h>

#if defined(HD_OS_WINDOWS)
#pragma warning(disable:4624) // 'derived class' : destructor was implicitly defined as deleted because a base class destructor is inaccessible or deleted
#endif

namespace {
    struct a { };
    struct b { virtual ~b() = delete; };
    struct c : b { };
    struct d { ~d() noexcept {} };
}

TEST(traits, is_destructible) {
    ASSERT_TRUE((hud::is_destructible_v<i32>));
    ASSERT_TRUE((hud::is_destructible_v<i32&>));
    ASSERT_TRUE((hud::is_destructible_v<i32&&>));
    ASSERT_TRUE((hud::is_destructible_v<volatile i32>));
    ASSERT_TRUE((hud::is_destructible_v<const i32>));
    ASSERT_TRUE((hud::is_destructible_v<const volatile i32>));
    ASSERT_TRUE((hud::is_destructible_v<a>));
    ASSERT_FALSE((hud::is_destructible_v<b>));
    ASSERT_FALSE((hud::is_destructible_v<c>));
    ASSERT_FALSE((hud::is_destructible_v<void>));
    ASSERT_FALSE((hud::is_destructible_v<const void>));
    ASSERT_FALSE((hud::is_destructible_v<volatile void>));
    ASSERT_FALSE((hud::is_destructible_v<const volatile void>));
    ASSERT_FALSE((hud::is_destructible_v<i32[]>));
    ASSERT_FALSE((hud::is_destructible_v<const i32[]>));
    ASSERT_FALSE((hud::is_destructible_v<volatile i32[]>));
    ASSERT_FALSE((hud::is_destructible_v<const volatile i32[]>));

}