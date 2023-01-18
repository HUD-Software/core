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
    GTEST_ASSERT_TRUE((hud::is_destructible_v<i32>));
    GTEST_ASSERT_TRUE((hud::is_destructible_v<i32 &>));
    GTEST_ASSERT_TRUE((hud::is_destructible_v<i32 &&>));
    GTEST_ASSERT_TRUE((hud::is_destructible_v<volatile i32>));
    GTEST_ASSERT_TRUE((hud::is_destructible_v<const i32>));
    GTEST_ASSERT_TRUE((hud::is_destructible_v<const volatile i32>));
    GTEST_ASSERT_TRUE((hud::is_destructible_v<hud_test::a>));
    GTEST_ASSERT_FALSE((hud::is_destructible_v<hud_test::b>));
    GTEST_ASSERT_FALSE((hud::is_destructible_v<hud_test::c>));
    GTEST_ASSERT_FALSE((hud::is_destructible_v<void>));
    GTEST_ASSERT_FALSE((hud::is_destructible_v<const void>));
    GTEST_ASSERT_FALSE((hud::is_destructible_v<volatile void>));
    GTEST_ASSERT_FALSE((hud::is_destructible_v<const volatile void>));
    GTEST_ASSERT_FALSE((hud::is_destructible_v<i32[]>));
    GTEST_ASSERT_FALSE((hud::is_destructible_v<const i32[]>));
    GTEST_ASSERT_FALSE((hud::is_destructible_v<volatile i32[]>));
    GTEST_ASSERT_FALSE((hud::is_destructible_v<const volatile i32[]>));
}