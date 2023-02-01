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
            void fn_a() {};
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

        [[maybe_unused]] int is_function_a()
        {
            return 0;
        }
    } // namespace
} // namespace hud_test

GTEST_TEST(traits, is_member_pointer)
{
    hud_assert_false((hud::is_member_pointer_v<hud_test::a *>));
    hud_assert_false((hud::is_member_pointer_v<decltype(&hud_test::i)>));
    hud_assert_false((hud::is_member_pointer_v<decltype(&hud_test::i1)>));
    hud_assert_false((hud::is_member_pointer_v<decltype(&hud_test::i2)>));
    hud_assert_false((hud::is_member_pointer_v<decltype(&hud_test::i3)>));
    hud_assert_true((hud::is_member_pointer_v<void (hud_test::a::*)()>));
    hud_assert_true((hud::is_member_pointer_v<void (hud_test::a::*)() const>));
    hud_assert_true((hud::is_member_pointer_v<void (hud_test::a::*)() volatile>));
    hud_assert_true((hud::is_member_pointer_v<void (hud_test::a::*)() const volatile>));
    hud_assert_false((hud::is_member_pointer_v<decltype(&hud_test::is_function_a)>));
    hud_assert_true((hud::is_member_pointer_v<decltype(&hud_test::a::fn_a)>));
    hud_assert_true((hud::is_member_pointer_v<decltype(&hud_test::a::fn_a2)>));
    hud_assert_true((hud::is_member_pointer_v<decltype(&hud_test::a::fn_a3)>));
    hud_assert_true((hud::is_member_pointer_v<decltype(&hud_test::a::fn_a4)>));
    hud_assert_true((hud::is_member_pointer_v<decltype(&hud_test::a::a0)>));
    hud_assert_true((hud::is_member_pointer_v<decltype(&hud_test::a::a1)>));
    hud_assert_true((hud::is_member_pointer_v<decltype(&hud_test::a::a2)>));
    hud_assert_true((hud::is_member_pointer_v<decltype(&hud_test::a::a3)>));
    hud_assert_true((hud::is_member_pointer_v<decltype(hud_test::pt)>));
    hud_assert_true((hud::is_member_pointer_v<decltype(hud_test::pt2)>));
    hud_assert_true((hud::is_member_pointer_v<decltype(hud_test::pt3)>));
    hud_assert_true((hud::is_member_pointer_v<decltype(hud_test::pt4)>));
}