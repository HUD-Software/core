#include <core/traits/is_trivially_constructible.h>

namespace hud_test
{
    struct a
    {
    };

    struct b
    {
        b()
        {
        }
    }; // non-trivial default ctor.

    struct c : a
    {
    };
} // namespace hud_test

GTEST_TEST(traits, is_trivially_constructible)
{
    hud_assert_true((hud::is_trivially_constructible_v<i32, i32>));
    hud_assert_false((hud::is_trivially_constructible_v<i32 &, i32>));
    hud_assert_false((hud::is_trivially_constructible_v<i32[], i32[]>));
    hud_assert_false((hud::is_trivially_constructible_v<i32[32], i32[32]>));
    hud_assert_true((hud::is_trivially_constructible_v<i32 *, i32 *>));

    hud_assert_true((hud::is_trivially_constructible_v<hud_test::a>));
    hud_assert_true((hud::is_trivially_constructible_v<hud_test::a, hud_test::a>));
    hud_assert_true((hud::is_trivially_constructible_v<hud_test::a, hud_test::a &>));
    hud_assert_true((hud::is_trivially_constructible_v<hud_test::a, hud_test::a &&>));
    hud_assert_false((hud::is_trivially_constructible_v<hud_test::a, hud_test::b>));
    hud_assert_true((hud::is_trivially_constructible_v<hud_test::a, hud_test::c>));
    hud_assert_false((hud::is_trivially_constructible_v<hud_test::b>));
    hud_assert_false((hud::is_trivially_constructible_v<hud_test::b, hud_test::a>));
    hud_assert_false((hud::is_trivially_constructible_v<hud_test::b, hud_test::a &>));
    hud_assert_false((hud::is_trivially_constructible_v<hud_test::b, hud_test::a &&>));
    hud_assert_true((hud::is_trivially_constructible_v<hud_test::b, hud_test::b>));
    hud_assert_false((hud::is_trivially_constructible_v<hud_test::b, hud_test::c>));
    hud_assert_true((hud::is_trivially_constructible_v<hud_test::c>));
#if defined(HD_COMPILER_CLANG_CL) || defined(HD_COMPILER_CLANG)
    // Bug: Waiting for clang anwser https://github.com/llvm/llvm-project/issues/55974
    // Clang do not implement the Parenthesized initialization of aggregates feature yet
    hud_assert_false((hud::is_trivially_constructible_v<hud_test::c, hud_test::a>));
#else
    hud_assert_true((hud::is_trivially_constructible_v<hud_test::c, hud_test::a>));
#endif
    hud_assert_false((hud::is_trivially_constructible_v<hud_test::c, hud_test::b>));
    hud_assert_true((hud::is_trivially_constructible_v<hud_test::c, hud_test::c>));
}