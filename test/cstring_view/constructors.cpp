#include <core/string/cstring_view.h>

GTEST_TEST(cstring_view, constructors) {

    auto test = []() {
        const ansichar *ptr = "hello world";
        hud::cstring_view v(ptr);
        hud::cstring_view v2(ptr, hud::cstring::length(ptr));
        return std::tuple {
            v.length() == hud::cstring::length(ptr),
            v.data() == ptr,
            v2.length() == hud::cstring::length(ptr),
            v2.data() == ptr,
        };
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
    }
}