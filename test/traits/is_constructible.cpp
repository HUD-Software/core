#include <core/traits/is_constructible.h>

namespace hud_test
{
    struct a
    {
        i32 a;
    };

    struct b
    {
        b(i32 b)
            : mB(b)
        {
        }

        i32 mB;
    };

    struct c
    {
        a a;
    };

    struct d
    {
        d(b b)
            : mB(b)
        {
        }

        b mB;
    };

    struct e
    {
        e() = default;

        e(a a)
            : mA(a)
        {
        }

        a mA;
    };
} // namespace hud_test

GTEST_TEST(traits, is_constructible)
{
    hud_assert_true((hud::is_constructible_v<hud_test::a>));
#if defined(HD_COMPILER_CLANG_CL) || defined(HD_COMPILER_CLANG)
    // Bug: Waiting for clang anwser https://github.com/llvm/llvm-project/issues/55974
    // Clang do not implement the Parenthesized initialization of aggregates feature yet
    hud_assert_false((hud::is_constructible_v<hud_test::a, i32>));
#else
    hud_assert_true((hud::is_constructible_v<hud_test::a, i32>));
#endif
    hud_assert_false((hud::is_constructible_v<hud_test::b>));
    hud_assert_true((hud::is_constructible_v<hud_test::b, i32>));
    hud_assert_true((hud::is_constructible_v<hud_test::b, const i32>));
    hud_assert_true((hud::is_constructible_v<hud_test::b, const volatile i32>));
    hud_assert_true((hud::is_constructible_v<hud_test::b, f32>)); // Implicit conversion f32 to i32
    hud_assert_true((hud::is_constructible_v<hud_test::c>));
#if defined(HD_COMPILER_CLANG_CL) || defined(HD_COMPILER_CLANG)
    // Bug: Waiting for clang anwser https://github.com/llvm/llvm-project/issues/55974
    // Clang do not implement the Parenthesized initialization of aggregates feature yet
    hud_assert_false((hud::is_constructible_v<hud_test::c, hud_test::a>));
#else
    hud_assert_true((hud::is_constructible_v<hud_test::c, hud_test::a>));
#endif
    hud_assert_false((hud::is_constructible_v<hud_test::d>)); // b Don't have default constructor
    hud_assert_true((hud::is_constructible_v<hud_test::d, hud_test::b>));
    hud_assert_true((hud::is_constructible_v<hud_test::d, i32>)); // b is create with i32
    hud_assert_true((hud::is_constructible_v<hud_test::e>));
    hud_assert_true((hud::is_constructible_v<hud_test::e, hud_test::a>));
}
