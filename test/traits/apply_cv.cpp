#include <core/traits/apply_cv.h>

namespace hud_test
{
    struct a
    {
    };
} // namespace hud_test

GTEST_TEST(traits, apply_cv)
{
    GTEST_ASSERT_TRUE((hud::is_same_v<hud_test::a, hud::apply_cv_t<hud_test::a, false, false>>));
    GTEST_ASSERT_TRUE((hud::is_same_v<const hud_test::a, hud::apply_cv_t<hud_test::a, true, false>>));
    GTEST_ASSERT_TRUE((hud::is_same_v<volatile hud_test::a, hud::apply_cv_t<hud_test::a, false, true>>));
    GTEST_ASSERT_TRUE((hud::is_same_v<const volatile hud_test::a, hud::apply_cv_t<hud_test::a, true, true>>));

    GTEST_ASSERT_TRUE((hud::is_same_v<const hud_test::a, hud::apply_cv_t<const hud_test::a, false, false>>));
    GTEST_ASSERT_TRUE((hud::is_same_v<const hud_test::a, hud::apply_cv_t<const hud_test::a, true, false>>));
    GTEST_ASSERT_TRUE((hud::is_same_v<const volatile hud_test::a, hud::apply_cv_t<const hud_test::a, false, true>>));
    GTEST_ASSERT_TRUE((hud::is_same_v<const volatile hud_test::a, hud::apply_cv_t<const hud_test::a, true, true>>));

    GTEST_ASSERT_TRUE((hud::is_same_v<volatile hud_test::a, hud::apply_cv_t<volatile hud_test::a, false, false>>));
    GTEST_ASSERT_TRUE((hud::is_same_v<const volatile hud_test::a, hud::apply_cv_t<volatile hud_test::a, true, false>>));
    GTEST_ASSERT_TRUE((hud::is_same_v<volatile hud_test::a, hud::apply_cv_t<volatile hud_test::a, false, true>>));
    GTEST_ASSERT_TRUE((hud::is_same_v<const volatile hud_test::a, hud::apply_cv_t<volatile hud_test::a, true, true>>));

    GTEST_ASSERT_TRUE((hud::is_same_v<const volatile hud_test::a, hud::apply_cv_t<const volatile hud_test::a, false, false>>));
    GTEST_ASSERT_TRUE((hud::is_same_v<const volatile hud_test::a, hud::apply_cv_t<const volatile hud_test::a, true, false>>));
    GTEST_ASSERT_TRUE((hud::is_same_v<const volatile hud_test::a, hud::apply_cv_t<const volatile hud_test::a, false, true>>));
    GTEST_ASSERT_TRUE((hud::is_same_v<const volatile hud_test::a, hud::apply_cv_t<const volatile hud_test::a, true, true>>));
}