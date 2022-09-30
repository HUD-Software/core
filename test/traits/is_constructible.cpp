#include <core/traits/is_constructible.h>

namespace hud_test {
    struct a {
        i32 a;
    };
    struct b {
        b(i32 b) : mB(b) {}
        i32 mB;
    };
    struct c {
        a a;
    };
    struct d {
        d(b b) : mB(b) {}
        b mB;
    };
    struct e {
        e() = default;
        e(a a) :mA(a) {}
        a mA;
    };
}
TEST(traits, is_constructible) {
    ASSERT_TRUE((hud::is_constructible_v<hud_test::a>));
#if defined(HD_COMPILER_CLANG_CL) || defined(HD_COMPILER_CLANG)
    // Bug: Waiting for clang anwser https://github.com/llvm/llvm-project/issues/55974
    // Clang do not implement the Parenthesized initialization of aggregates feature yet
    ASSERT_FALSE((hud::is_constructible_v < hud_test::a, i32 >));
#else
    ASSERT_TRUE((hud::is_constructible_v < hud_test::a, i32 >));
#endif
    ASSERT_FALSE((hud::is_constructible_v<hud_test::b>));
    ASSERT_TRUE((hud::is_constructible_v<hud_test::b, i32>));
    ASSERT_TRUE((hud::is_constructible_v<hud_test::b, const i32>));
    ASSERT_TRUE((hud::is_constructible_v<hud_test::b, const volatile i32>));
    ASSERT_TRUE((hud::is_constructible_v<hud_test::b, f32>)); // Implicit conversion f32 to i32
    ASSERT_TRUE((hud::is_constructible_v<hud_test::c>));
#if defined(HD_COMPILER_CLANG_CL) || defined(HD_COMPILER_CLANG)
// Bug: Waiting for clang anwser https://github.com/llvm/llvm-project/issues/55974
// Clang do not implement the Parenthesized initialization of aggregates feature yet
    ASSERT_FALSE((hud::is_constructible_v<hud_test::c, hud_test::a>));
#else
    ASSERT_TRUE((hud::is_constructible_v<hud_test::c, hud_test::a>));
#endif
    ASSERT_FALSE((hud::is_constructible_v<hud_test::d>)); // b Don't have default constructor
    ASSERT_TRUE((hud::is_constructible_v<hud_test::d, hud_test::b>));
    ASSERT_TRUE((hud::is_constructible_v<hud_test::d, i32>)); // b is create with i32
    ASSERT_TRUE((hud::is_constructible_v<hud_test::e>));
    ASSERT_TRUE((hud::is_constructible_v<hud_test::e, hud_test::a>));
}
