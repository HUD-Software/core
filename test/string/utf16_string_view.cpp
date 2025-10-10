#include <core/string/utf16_string_view.h>
#include "lipsum/arabic_lipsum.h"

GTEST_TEST(utf16_string_view, make_utf16_string_view_checked)
{
    auto test = []() {
        const char16 *ptr = U16_ARABIC_LIPSUM;
        auto cstring_view_opt = hud::make_utf16_string_view_checked(ptr);
        auto v = *cstring_view_opt;
        return std::tuple {
            v.length() == hud::cstring::length(ptr), // 0
            v.data() == ptr,                         // 1

        };
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
    }
}

GTEST_TEST(utf16_string_view, inner_type)
{
    static_assert(hud::is_same_v<hud::utf16_string_view<const char16>::char_type, const char16>);
    static_assert(hud::is_same_v<hud::utf16_string_view<char16>::char_type, char16>);

    hud::utf16_string_view v {u"Hello world"};
    static_assert(hud::is_same_v<decltype(v)::char_type, const char16>);

#if defined(HD_HOST_WINDOWS)
    hud::utf16_string_view v2 {L"Hello world"};
    static_assert(hud::is_same_v<decltype(v2)::char_type, const wchar>);
#endif

    char16 ptr[] = u"Hello world";
    hud::utf16_string_view v1 {ptr};
    static_assert(hud::is_same_v<decltype(v1)::char_type, char16>);
}
