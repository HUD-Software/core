#include <core/string/cstring_view.h>

GTEST_TEST(cstring_view, constructors)
{
    auto test = []() {
        const char8 *ptr = "hello world hello world hello world";
        hud::cstring_view v(ptr);
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

GTEST_TEST(cstring_view, inner_type)
{
    static_assert(hud::is_same_v<hud::cstring_view<const char8>::char_type, const char8>);
    static_assert(hud::is_same_v<hud::cstring_view<char8>::char_type, char8>);

    hud::cstring_view v {"Hello world"};
    static_assert(hud::is_same_v<decltype(v)::char_type, const char8>);

    char8 ptr[] = "Hello world";
    hud::cstring_view v1 {ptr};
    static_assert(hud::is_same_v<decltype(v1)::char_type, char8>);
}

GTEST_TEST(cstring_view, is_ascii)
{
    auto test = []() {
        const char8 *ptr = "hello world hello world hello world";
        hud::cstring_view v(ptr);
        const char8 *ptr_2 = "hello world\x80 hello world hello world";
        hud::cstring_view v2(ptr_2);
        const char8 *ptr_3 = "hello world\x85 hello world hello world";
        hud::cstring_view v3(ptr_3);

        return std::tuple {
            v.is_ascii(),   // 0
            !v2.is_ascii(), // 1
            !v3.is_ascii()  // 2
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

GTEST_TEST(cstring_view, slice)
{
    hud::cstring_view const_v {"ABC"};
    auto const_s = const_v.as_slice();
    auto const_begin = const_s.begin();
    hud_assert_true(*const_begin == 'A');
    const_begin++;
    hud_assert_true(*const_begin == 'B');
    const_begin++;
    hud_assert_true(*const_begin == 'C');
    const_begin++;
    hud_assert_true(const_begin == const_s.end());

    char8 txt[] = "ABC";
    hud::cstring_view v {txt};
    auto s = v.as_slice();
    auto begin = s.begin();
    hud_assert_true(*begin == 'A');
    begin++;
    hud_assert_true(*begin == 'B');
    begin++;
    hud_assert_true(*begin == 'C');
    begin++;
    hud_assert_true(begin == s.end());

    auto const_s_1 = v.as_slice().sub_slice(0, 2);
    auto const_begin_1 = const_s_1.begin();
    hud_assert_true(*const_begin_1 == 'A');
    const_begin_1++;
    hud_assert_true(*const_begin_1 == 'B');
    const_begin_1++;
    hud_assert_true(const_begin_1 == const_s_1.end());

    s = v.as_slice().sub_slice(0, 2);
    begin = s.begin();
    hud_assert_true(*begin == 'A');
    begin++;
    hud_assert_true(*begin == 'B');
    begin++;
    hud_assert_true(begin == s.end());
}