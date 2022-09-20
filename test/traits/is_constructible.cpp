#include <core/traits/is_constructible.h>

namespace {
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
    ASSERT_TRUE((hud::is_constructible_v<a>));
#if defined(HD_COMPILER_CLANG_CL) || defined(HD_COMPILER_CLANG)
    // Bug: Waiting for clang anwser https://github.com/llvm/llvm-project/issues/55974
    // Clang do not implement the Parenthesized initialization of aggregates feature yet
    ASSERT_FALSE((hud::is_constructible_v < a, i32 >));
#else
    ASSERT_TRUE((hud::is_constructible_v < a, i32 >));
#endif
    ASSERT_FALSE((hud::is_constructible_v<b>));
    ASSERT_TRUE((hud::is_constructible_v<b, i32>));
    ASSERT_TRUE((hud::is_constructible_v<b, const i32>));
    ASSERT_TRUE((hud::is_constructible_v<b, const volatile i32>));
    ASSERT_TRUE((hud::is_constructible_v<b, f32>)); // Implicit conversion f32 to i32
    ASSERT_TRUE((hud::is_constructible_v<c>));
#if defined(HD_COMPILER_CLANG_CL) || defined(HD_COMPILER_CLANG)
// Bug: Waiting for clang anwser https://github.com/llvm/llvm-project/issues/55974
// Clang do not implement the Parenthesized initialization of aggregates feature yet
    ASSERT_FALSE((hud::is_constructible_v<c, a>));
#else
    ASSERT_TRUE((hud::is_constructible_v<c, a>));
#endif
    ASSERT_FALSE((hud::is_constructible_v<d>)); // b Don't have default constructor
    ASSERT_TRUE((hud::is_constructible_v<d, b>));
    ASSERT_TRUE((hud::is_constructible_v<d, i32>)); // b is create with i32
    ASSERT_TRUE((hud::is_constructible_v<e>));
    ASSERT_TRUE((hud::is_constructible_v<e, a>));
}
