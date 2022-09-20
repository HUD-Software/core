#include <core/traits/is_member_pointer.h>

namespace {
    [[maybe_unused]] int i = 0;
    [[maybe_unused]] const int i1 = 0;
    [[maybe_unused]] volatile int i2 = 0;
    [[maybe_unused]] const volatile int i3 = 0;
    struct a {
        void fn_a() {};
        void fn_a2() const {};
        void fn_a3() volatile {};
        void fn_a4() const volatile {};
        int a0 = 0;
        const int a1 = 0;
        volatile int a2 = 0;
        const volatile int a3 = 0;
    };
    [[maybe_unused]] void(a::* pt)() = &a::fn_a;
    [[maybe_unused]] void(a::* pt2)() const = &a::fn_a2;
    [[maybe_unused]] void(a::* pt3)() volatile = &a::fn_a3;
    [[maybe_unused]] void(a::* pt4)() const volatile = &a::fn_a4;

    [[maybe_unused]] int IsFunction_a() { return 0; }
}

TEST(traits, is_member_pointer) {
    ASSERT_FALSE((hud::is_member_pointer_v<a*>));
    ASSERT_FALSE((hud::is_member_pointer_v<decltype(&i)>));
    ASSERT_FALSE((hud::is_member_pointer_v<decltype(&i1)>));
    ASSERT_FALSE((hud::is_member_pointer_v<decltype(&i2)>));
    ASSERT_FALSE((hud::is_member_pointer_v<decltype(&i3)>));
    ASSERT_TRUE((hud::is_member_pointer_v<void(a::*)()>));
    ASSERT_TRUE((hud::is_member_pointer_v<void(a::*)()const>));
    ASSERT_TRUE((hud::is_member_pointer_v<void(a::*)()volatile>));
    ASSERT_TRUE((hud::is_member_pointer_v<void(a::*)()const volatile>));
    ASSERT_FALSE((hud::is_member_pointer_v<decltype(&IsFunction_a)>));
    ASSERT_TRUE((hud::is_member_pointer_v<decltype(&a::fn_a)>));
    ASSERT_TRUE((hud::is_member_pointer_v<decltype(&a::fn_a2)>));
    ASSERT_TRUE((hud::is_member_pointer_v<decltype(&a::fn_a3)>));
    ASSERT_TRUE((hud::is_member_pointer_v<decltype(&a::fn_a4)>));
    ASSERT_TRUE((hud::is_member_pointer_v<decltype(&a::a0)>));
    ASSERT_TRUE((hud::is_member_pointer_v<decltype(&a::a1)>));
    ASSERT_TRUE((hud::is_member_pointer_v<decltype(&a::a2)>));
    ASSERT_TRUE((hud::is_member_pointer_v<decltype(&a::a3)>));
    ASSERT_TRUE((hud::is_member_pointer_v<decltype(pt)>));
    ASSERT_TRUE((hud::is_member_pointer_v<decltype(pt2)>));
    ASSERT_TRUE((hud::is_member_pointer_v<decltype(pt3)>));
    ASSERT_TRUE((hud::is_member_pointer_v<decltype(pt4)>));
}