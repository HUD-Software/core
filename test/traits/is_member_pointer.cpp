#include <core/traits/is_member_pointer.h>

namespace hud_test
{
    namespace
    {
        [[maybe_unused]] int i = 0;
        [[maybe_unused]] const int i1 = 0;
        [[maybe_unused]] volatile int i2 = 0;
        [[maybe_unused]] const volatile int i3 = 0;
        struct a
        {
            void fn_a(){};
            void fn_a2() const {};
            void fn_a3() volatile {};
            void fn_a4() const volatile {};
            int a0 = 0;
            const int a1 = 0;
            volatile int a2 = 0;
            const volatile int a3 = 0;
        };
        [[maybe_unused]] void (a::*pt)() = &a::fn_a;
        [[maybe_unused]] void (a::*pt2)() const = &a::fn_a2;
        [[maybe_unused]] void (a::*pt3)() volatile = &a::fn_a3;
        [[maybe_unused]] void (a::*pt4)() const volatile = &a::fn_a4;

        [[maybe_unused]] int is_function_a() { return 0; }
    }
}

GTEST_TEST(traits, is_member_pointer)
{
    GTEST_ASSERT_FALSE((hud::is_member_pointer_v<hud_test::a *>));
    GTEST_ASSERT_FALSE((hud::is_member_pointer_v<decltype(&hud_test::i)>));
    GTEST_ASSERT_FALSE((hud::is_member_pointer_v<decltype(&hud_test::i1)>));
    GTEST_ASSERT_FALSE((hud::is_member_pointer_v<decltype(&hud_test::i2)>));
    GTEST_ASSERT_FALSE((hud::is_member_pointer_v<decltype(&hud_test::i3)>));
    GTEST_ASSERT_TRUE((hud::is_member_pointer_v<void (hud_test::a::*)()>));
    GTEST_ASSERT_TRUE((hud::is_member_pointer_v<void (hud_test::a::*)() const>));
    GTEST_ASSERT_TRUE((hud::is_member_pointer_v<void (hud_test::a::*)() volatile>));
    GTEST_ASSERT_TRUE((hud::is_member_pointer_v<void (hud_test::a::*)() const volatile>));
    GTEST_ASSERT_FALSE((hud::is_member_pointer_v<decltype(&hud_test::is_function_a)>));
    GTEST_ASSERT_TRUE((hud::is_member_pointer_v<decltype(&hud_test::a::fn_a)>));
    GTEST_ASSERT_TRUE((hud::is_member_pointer_v<decltype(&hud_test::a::fn_a2)>));
    GTEST_ASSERT_TRUE((hud::is_member_pointer_v<decltype(&hud_test::a::fn_a3)>));
    GTEST_ASSERT_TRUE((hud::is_member_pointer_v<decltype(&hud_test::a::fn_a4)>));
    GTEST_ASSERT_TRUE((hud::is_member_pointer_v<decltype(&hud_test::a::a0)>));
    GTEST_ASSERT_TRUE((hud::is_member_pointer_v<decltype(&hud_test::a::a1)>));
    GTEST_ASSERT_TRUE((hud::is_member_pointer_v<decltype(&hud_test::a::a2)>));
    GTEST_ASSERT_TRUE((hud::is_member_pointer_v<decltype(&hud_test::a::a3)>));
    GTEST_ASSERT_TRUE((hud::is_member_pointer_v<decltype(hud_test::pt)>));
    GTEST_ASSERT_TRUE((hud::is_member_pointer_v<decltype(hud_test::pt2)>));
    GTEST_ASSERT_TRUE((hud::is_member_pointer_v<decltype(hud_test::pt3)>));
    GTEST_ASSERT_TRUE((hud::is_member_pointer_v<decltype(hud_test::pt4)>));
}