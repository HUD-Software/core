#include <core/traits/apply_cv.h>

namespace {
    struct a {};
}

TEST(traits, apply_cv) {
    ASSERT_TRUE((hud::is_same_v<a, hud::apply_cv_t<a, false, false>>));
    ASSERT_TRUE((hud::is_same_v<const a, hud::apply_cv_t<a, true, false>>));
    ASSERT_TRUE((hud::is_same_v<volatile a, hud::apply_cv_t<a, false, true>>));
    ASSERT_TRUE((hud::is_same_v<const volatile a, hud::apply_cv_t<a, true, true>>));
    
    ASSERT_TRUE((hud::is_same_v<const a, hud::apply_cv_t<const a, false, false>>));
    ASSERT_TRUE((hud::is_same_v<const a, hud::apply_cv_t<const a, true, false>>));
    ASSERT_TRUE((hud::is_same_v<const volatile a, hud::apply_cv_t<const a, false, true>>));
    ASSERT_TRUE((hud::is_same_v<const volatile a, hud::apply_cv_t<const a, true, true>>));
    
    ASSERT_TRUE((hud::is_same_v<volatile a, hud::apply_cv_t<volatile a, false, false>>));
    ASSERT_TRUE((hud::is_same_v<const volatile a, hud::apply_cv_t<volatile a, true, false>>));
    ASSERT_TRUE((hud::is_same_v<volatile a, hud::apply_cv_t<volatile a, false, true>>));
    ASSERT_TRUE((hud::is_same_v<const volatile a, hud::apply_cv_t<volatile a, true, true>>));
    
    ASSERT_TRUE((hud::is_same_v<const volatile a, hud::apply_cv_t<const volatile a, false, false>>));
    ASSERT_TRUE((hud::is_same_v<const volatile a, hud::apply_cv_t<const volatile a, true, false>>));
    ASSERT_TRUE((hud::is_same_v<const volatile a, hud::apply_cv_t<const volatile a, false, true>>));
    ASSERT_TRUE((hud::is_same_v<const volatile a, hud::apply_cv_t<const volatile a, true, true>>));
}