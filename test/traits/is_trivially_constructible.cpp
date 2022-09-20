#include <core/traits/is_trivially_constructible.h>

namespace {
    struct a { };
    struct b { b() {} };  // non-trivial default ctor.
    struct c : a { };
}

TEST(traits, is_trivially_constructible) {
    ASSERT_TRUE((hud::is_trivially_constructible_v<i32, i32>));
    ASSERT_FALSE((hud::is_trivially_constructible_v<i32&, i32>));
    ASSERT_FALSE((hud::is_trivially_constructible_v<i32[], i32[]>));
    ASSERT_FALSE((hud::is_trivially_constructible_v<i32[32], i32[32]>));
    ASSERT_TRUE((hud::is_trivially_constructible_v<i32*, i32*>));

    ASSERT_TRUE((hud::is_trivially_constructible_v<a>));
    ASSERT_TRUE((hud::is_trivially_constructible_v<a, a>));
    ASSERT_TRUE((hud::is_trivially_constructible_v<a, a&>));
    ASSERT_TRUE((hud::is_trivially_constructible_v<a, a&&>));
    ASSERT_FALSE((hud::is_trivially_constructible_v<a, b>));
    ASSERT_TRUE((hud::is_trivially_constructible_v<a, c>));
    ASSERT_FALSE((hud::is_trivially_constructible_v<b>));
    ASSERT_FALSE((hud::is_trivially_constructible_v<b, a>));
    ASSERT_FALSE((hud::is_trivially_constructible_v<b, a&>));
    ASSERT_FALSE((hud::is_trivially_constructible_v<b, a&&>));
    ASSERT_TRUE((hud::is_trivially_constructible_v<b, b>));
    ASSERT_FALSE((hud::is_trivially_constructible_v<b, c>));
    ASSERT_TRUE((hud::is_trivially_constructible_v<c>));
#if defined(HD_COMPILER_CLANG_CL) || defined(HD_COMPILER_CLANG)
    // Bug: Waiting for clang anwser https://github.com/llvm/llvm-project/issues/55974
    // Clang do not implement the Parenthesized initialization of aggregates feature yet
    ASSERT_FALSE((hud::is_trivially_constructible_v<c, a>));
#else
    ASSERT_TRUE((hud::is_trivially_constructible_v<c, a>));
#endif
    ASSERT_FALSE((hud::is_trivially_constructible_v<c, b>));
    ASSERT_TRUE((hud::is_trivially_constructible_v<c, c>));
}