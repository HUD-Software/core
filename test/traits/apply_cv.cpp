#include <core/traits/apply_cv.h>

namespace hud_test
{
    struct a
    {
    };
} // namespace hud_test

GTEST_TEST(traits, apply_cv)
{
    hud_assert_true((hud::is_same_v<hud_test::a, hud::apply_cv_t<hud_test::a, false, false>>));
    hud_assert_true((hud::is_same_v<const hud_test::a, hud::apply_cv_t<hud_test::a, true, false>>));
    hud_assert_true((hud::is_same_v<volatile hud_test::a, hud::apply_cv_t<hud_test::a, false, true>>));
    hud_assert_true((hud::is_same_v<const volatile hud_test::a, hud::apply_cv_t<hud_test::a, true, true>>));

    hud_assert_true((hud::is_same_v<const hud_test::a, hud::apply_cv_t<const hud_test::a, false, false>>));
    hud_assert_true((hud::is_same_v<const hud_test::a, hud::apply_cv_t<const hud_test::a, true, false>>));
    hud_assert_true((hud::is_same_v<const volatile hud_test::a, hud::apply_cv_t<const hud_test::a, false, true>>));
    hud_assert_true((hud::is_same_v<const volatile hud_test::a, hud::apply_cv_t<const hud_test::a, true, true>>));

    hud_assert_true((hud::is_same_v<volatile hud_test::a, hud::apply_cv_t<volatile hud_test::a, false, false>>));
    hud_assert_true((hud::is_same_v<const volatile hud_test::a, hud::apply_cv_t<volatile hud_test::a, true, false>>));
    hud_assert_true((hud::is_same_v<volatile hud_test::a, hud::apply_cv_t<volatile hud_test::a, false, true>>));
    hud_assert_true((hud::is_same_v<const volatile hud_test::a, hud::apply_cv_t<volatile hud_test::a, true, true>>));

    hud_assert_true((hud::is_same_v<const volatile hud_test::a, hud::apply_cv_t<const volatile hud_test::a, false, false>>));
    hud_assert_true((hud::is_same_v<const volatile hud_test::a, hud::apply_cv_t<const volatile hud_test::a, true, false>>));
    hud_assert_true((hud::is_same_v<const volatile hud_test::a, hud::apply_cv_t<const volatile hud_test::a, false, true>>));
    hud_assert_true((hud::is_same_v<const volatile hud_test::a, hud::apply_cv_t<const volatile hud_test::a, true, true>>));
}