#include <core/string/cstring_view.h>

GTEST_TEST(cstring_view, is_ascii)
{

    auto test = []() {
        const ansichar *ptr = "hello world";
        hud::cstring_view v(ptr);
        const ansichar *ptr_2 = "hello world\x80";
        hud::cstring_view v2(ptr_2);
        const ansichar *ptr_3 = "hello world\x85";
        hud::cstring_view v3(ptr_3);

        return std::tuple {
            v.is_ascii(),
            !v2.is_ascii(),
            !v3.is_ascii()
        };
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
    }
}