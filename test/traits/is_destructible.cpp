#include <core/traits/is_destructible.h>

#if defined(HD_OS_WINDOWS)
    #pragma warning(disable : 4624) // 'derived class' : destructor was implicitly defined as deleted because a base class destructor is inaccessible or deleted
#endif

namespace hud_test
{
    struct a
    {
    };

    struct b
    {
        virtual ~b() = delete;
    };

    struct c : b
    {
    };

    struct d
    {
        ~d() noexcept
        {
        }
    };
} // namespace hud_test

GTEST_TEST(traits, is_destructible)
{
    hud_assert_true((hud::is_destructible_v<i32>));
    hud_assert_true((hud::is_destructible_v<i32 &>));
    hud_assert_true((hud::is_destructible_v<i32 &&>));
    hud_assert_true((hud::is_destructible_v<volatile i32>));
    hud_assert_true((hud::is_destructible_v<const i32>));
    hud_assert_true((hud::is_destructible_v<const volatile i32>));
    hud_assert_true((hud::is_destructible_v<hud_test::a>));
    hud_assert_false((hud::is_destructible_v<hud_test::b>));
    hud_assert_false((hud::is_destructible_v<hud_test::c>));
    hud_assert_false((hud::is_destructible_v<void>));
    hud_assert_false((hud::is_destructible_v<const void>));
    hud_assert_false((hud::is_destructible_v<volatile void>));
    hud_assert_false((hud::is_destructible_v<const volatile void>));
    hud_assert_false((hud::is_destructible_v<i32[]>));
    hud_assert_false((hud::is_destructible_v<const i32[]>));
    hud_assert_false((hud::is_destructible_v<volatile i32[]>));
    hud_assert_false((hud::is_destructible_v<const volatile i32[]>));
}