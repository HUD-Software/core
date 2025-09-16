#include <core/string/wstring_view.h>

GTEST_TEST(wstring_view, constructors)
{
    auto test = []() {
        const wchar *ptr = L"hello world hello world hello world";
        hud::wstring_view v(ptr);
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

GTEST_TEST(wstring_view, inner_type)
{
    static_assert(hud::is_same_v<hud::wstring_view<const wchar>::char_type, const wchar>);
    static_assert(hud::is_same_v<hud::wstring_view<wchar>::char_type, wchar>);

    hud::wstring_view v {L"Hello world"};
    static_assert(hud::is_same_v<decltype(v)::char_type, const wchar>);

    wchar ptr[] = L"Hello world";
    hud::wstring_view v1 {ptr};
    static_assert(hud::is_same_v<decltype(v1)::char_type, wchar>);
}

GTEST_TEST(wstring_view, is_ascii)
{
    auto test = []() {
        const wchar *ptr = L"hello world hello world hello world";
        hud::wstring_view v(ptr);
        const wchar *ptr_2 = L"hello world\x80 hello world hello world";
        hud::wstring_view v2(ptr_2);
        const wchar *ptr_3 = L"hello world\x85 hello world hello world";
        hud::wstring_view v3(ptr_3);

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

GTEST_TEST(wstring_view, equals)
{
    auto test = []() {
        wchar ptr[] = L"hello world hello world hello world";
        wchar ptr2[] = L"hello world hello world2 hello world";
        wchar ptr3[] = L"hello world hello world hello world";
        wchar ptr4[] = L"hello world hello world hello";
        hud::wstring_view v(ptr);
        hud::wstring_view v2(ptr2);
        hud::wstring_view v3(ptr3);
        hud::wstring_view v4(ptr4);

        return std::tuple {
            static_cast<wchar *>(ptr) != static_cast<wchar *>(ptr2),  // 0
            static_cast<wchar *>(ptr2) != static_cast<wchar *>(ptr3), // 1
            v.equals(v),                                              // 2
            !v.equals(v2),                                            // 3
            v.equals(v3),                                             // 4
            !v2.equals(v3),                                           // 5
            !v4.equals(v),                                            // 6
            !v.equals(v4),                                            // 7
        };
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_true(std::get<6>(result));
        hud_assert_true(std::get<7>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_true(std::get<6>(result));
        hud_assert_true(std::get<7>(result));
    }
}

GTEST_TEST(wstring_view, equals_partial)
{
    auto test = []() {
        wchar ptr[] = L"hello world hello world hello world";
        wchar ptr2[] = L"hello world hello world2 hello world";
        wchar ptr3[] = L"hello world hello world hello world";
        wchar ptr4[] = L"hello world hello world hello";
        hud::wstring_view v(ptr);
        hud::wstring_view v2(ptr2);
        hud::wstring_view v3(ptr3);
        hud::wstring_view v4(ptr4);

        bool equals_partial_0 = v.equals_partial(v, v.length());          // 0
        bool equals_partial_1 = v.equals_partial(v, v.length() - 1);      // 1
        bool equals_partial_2 = v.equals_partial(v, v.length() + 1);      // 2
        bool equals_partial_3 = !v.equals_partial(v2, v2.length());       // 3
        bool equals_partial_4 = !v.equals_partial(v2, v2.length() - 1);   // 4
        bool equals_partial_5 = !v.equals_partial(v2, v2.length() + 1);   // 5
        bool equals_partial_6 = v.equals_partial(v2, 8);                  // 6
        bool equals_partial_7 = v.equals_partial(v3, v3.length());        // 7
        bool equals_partial_8 = v.equals_partial(v3, v3.length() - 1);    // 8
        bool equals_partial_9 = v.equals_partial(v3, v3.length() + 1);    // 9
        bool equals_partial_10 = v.equals_partial(v4, v4.length());       // 10
        bool equals_partial_11 = v.equals_partial(v4, v4.length() - 1);   // 11
        bool equals_partial_12 = !v.equals_partial(v4, v4.length() + 1);  // 12
        bool equals_partial_13 = !v.equals_partial(v4, v.length());       // 13
        bool equals_partial_14 = !v2.equals_partial(v3, v3.length());     // 14
        bool equals_partial_15 = !v2.equals_partial(v3, v3.length() - 1); // 15
        bool equals_partial_16 = !v2.equals_partial(v3, v3.length() + 1); // 16
        bool equals_partial_17 = v2.equals_partial(v3, 8);                // 17
        bool equals_partial_18 = v4.equals_partial(v, v4.length());       // 18
        bool equals_partial_19 = v4.equals_partial(v, v4.length() - 1);   // 19
        bool equals_partial_20 = !v4.equals_partial(v, v4.length() + 1);  // 20
        bool equals_partial_21 = !v4.equals_partial(v2, v4.length());     // 21
        bool equals_partial_22 = !v4.equals_partial(v2, v4.length() - 1); // 22
        bool equals_partial_23 = !v4.equals_partial(v2, v4.length() + 1); // 23
        bool equals_partial_24 = v4.equals_partial(v2, 8);                // 24
        return std::tuple {
            equals_partial_0,
            equals_partial_1,
            equals_partial_2,
            equals_partial_3,
            equals_partial_4,
            equals_partial_5,
            equals_partial_6,
            equals_partial_7,
            equals_partial_8,
            equals_partial_9,
            equals_partial_10,
            equals_partial_11,
            equals_partial_12,
            equals_partial_13,
            equals_partial_14,
            equals_partial_15,
            equals_partial_16,
            equals_partial_17,
            equals_partial_18,
            equals_partial_19,
            equals_partial_20,
            equals_partial_21,
            equals_partial_22,
            equals_partial_23,
            equals_partial_24
        };
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_true(std::get<6>(result));
        hud_assert_true(std::get<7>(result));
        hud_assert_true(std::get<8>(result));
        hud_assert_true(std::get<9>(result));
        hud_assert_true(std::get<10>(result));
        hud_assert_true(std::get<11>(result));
        hud_assert_true(std::get<12>(result));
        hud_assert_true(std::get<13>(result));
        hud_assert_true(std::get<14>(result));
        hud_assert_true(std::get<15>(result));
        hud_assert_true(std::get<16>(result));
        hud_assert_true(std::get<17>(result));
        hud_assert_true(std::get<18>(result));
        hud_assert_true(std::get<19>(result));
        hud_assert_true(std::get<20>(result));
        hud_assert_true(std::get<21>(result));
        hud_assert_true(std::get<22>(result));
        hud_assert_true(std::get<23>(result));
        hud_assert_true(std::get<24>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_true(std::get<6>(result));
        hud_assert_true(std::get<7>(result));
        hud_assert_true(std::get<8>(result));
        hud_assert_true(std::get<9>(result));
        hud_assert_true(std::get<10>(result));
        hud_assert_true(std::get<11>(result));
        hud_assert_true(std::get<12>(result));
        hud_assert_true(std::get<13>(result));
        hud_assert_true(std::get<14>(result));
        hud_assert_true(std::get<15>(result));
        hud_assert_true(std::get<16>(result));
        hud_assert_true(std::get<17>(result));
        hud_assert_true(std::get<18>(result));
        hud_assert_true(std::get<19>(result));
        hud_assert_true(std::get<20>(result));
        hud_assert_true(std::get<21>(result));
        hud_assert_true(std::get<22>(result));
        hud_assert_true(std::get<23>(result));
        hud_assert_true(std::get<24>(result));
    }
}

GTEST_TEST(wstring_view, find_first)
{
    auto test = []() {
        wchar ptr[] = L"abcdefghijklmnopqrstuvwxyz";
        wchar ptr2[] = L"123456789abcdefghijklmnopqrstuvwxyz123456789_";
        hud::wstring_view v(ptr);
        hud::wstring_view v2(ptr2);

        bool find_0 = v.find_first(L"abc") == 0;
        bool find_1 = v.find_first(L"def") == 3;
        bool find_2 = v.find_first(L"ghijkl") == 6;
        bool find_3 = v.find_first(L"mnopqrstuvwxyz") == 12;
        bool find_4 = v.find_first(L"_") == -1;
        bool find_5 = v.find_first(L"123456789") == -1;
        bool find_6 = v2.find_first(L"abc") == 9;
        bool find_7 = v2.find_first(L"def") == 12;
        bool find_8 = v2.find_first(L"ghijkl") == 15;
        bool find_9 = v2.find_first(L"mnopqrstuvwxyz") == 21;
        bool find_10 = v2.find_first(L"_") == 44 && v2[44] == '_';
        bool find_11 = v2.find_first(L"123456789") == 0;
        return std::tuple {
            find_0,
            find_1,
            find_2,
            find_3,
            find_4,
            find_5,
            find_6,
            find_7,
            find_8,
            find_9,
            find_10,
            find_11,
        };
    };
    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_true(std::get<6>(result));
        hud_assert_true(std::get<7>(result));
        hud_assert_true(std::get<8>(result));
        hud_assert_true(std::get<9>(result));
        hud_assert_true(std::get<10>(result));
        hud_assert_true(std::get<11>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_true(std::get<6>(result));
        hud_assert_true(std::get<7>(result));
        hud_assert_true(std::get<8>(result));
        hud_assert_true(std::get<9>(result));
        hud_assert_true(std::get<10>(result));
        hud_assert_true(std::get<11>(result));
    }
}

GTEST_TEST(wstring_view, find_first_character)
{
    auto test = []() {
        wchar ptr[] = L"abcdefghijklmnopqrstuvwxyz";
        wchar ptr2[] = L"123456789abcdefghijklmnopqrstuvwxyz123456789_";
        hud::wstring_view v(ptr);
        hud::wstring_view v2(ptr2);

        bool find_0 = v.find_first_character(L'a') == 0;
        bool find_1 = v.find_first_character(L'd') == 3;
        bool find_2 = v.find_first_character(L'g') == 6;
        bool find_3 = v.find_first_character(L'm') == 12;
        bool find_4 = v.find_first_character(L'_') == -1;
        bool find_5 = v.find_first_character(L'1') == -1;
        bool find_6 = v2.find_first_character(L'a') == 9;
        bool find_7 = v2.find_first_character(L'd') == 12;
        bool find_8 = v2.find_first_character(L'g') == 15;
        bool find_9 = v2.find_first_character(L'm') == 21;
        bool find_10 = v2.find_first_character(L'_') == 44 && v2[44] == '_';
        bool find_11 = v2.find_first_character(L'1') == 0;
        return std::tuple {
            find_0,
            find_1,
            find_2,
            find_3,
            find_4,
            find_5,
            find_6,
            find_7,
            find_8,
            find_9,
            find_10,
            find_11,
        };
    };
    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_true(std::get<6>(result));
        hud_assert_true(std::get<7>(result));
        hud_assert_true(std::get<8>(result));
        hud_assert_true(std::get<9>(result));
        hud_assert_true(std::get<10>(result));
        hud_assert_true(std::get<11>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_true(std::get<6>(result));
        hud_assert_true(std::get<7>(result));
        hud_assert_true(std::get<8>(result));
        hud_assert_true(std::get<9>(result));
        hud_assert_true(std::get<10>(result));
        hud_assert_true(std::get<11>(result));
    }
}

GTEST_TEST(wstring_view, contains_string)
{
    auto test = []() {
        wchar ptr[] = L"abcdefghijklmnopqrstuvwxyz";
        wchar ptr2[] = L"123456789abcdefghijklmnopqrstuvwxyz123456789_";
        hud::wstring_view v(ptr);
        hud::wstring_view v2(ptr2);

        bool find_0 = v.contains(L"abc");
        bool find_1 = v.contains(L"def");
        bool find_2 = v.contains(L"ghijkl");
        bool find_3 = v.contains(L"mnopqrstuvwxyz");
        bool find_4 = !v.contains(L"_");
        bool find_5 = !v.contains(L"123456789");
        bool find_6 = v2.contains(L"abc");
        bool find_7 = v2.contains(L"def");
        bool find_8 = v2.contains(L"ghijkl");
        bool find_9 = v2.contains(L"mnopqrstuvwxyz");
        bool find_10 = v2.contains(L"_");
        bool find_11 = v2.contains(L"123456789");
        return std::tuple {
            find_0,
            find_1,
            find_2,
            find_3,
            find_4,
            find_5,
            find_6,
            find_7,
            find_8,
            find_9,
            find_10,
            find_11,
        };
    };
    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_true(std::get<6>(result));
        hud_assert_true(std::get<7>(result));
        hud_assert_true(std::get<8>(result));
        hud_assert_true(std::get<9>(result));
        hud_assert_true(std::get<10>(result));
        hud_assert_true(std::get<11>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_true(std::get<6>(result));
        hud_assert_true(std::get<7>(result));
        hud_assert_true(std::get<8>(result));
        hud_assert_true(std::get<9>(result));
        hud_assert_true(std::get<10>(result));
        hud_assert_true(std::get<11>(result));
    }
}

GTEST_TEST(wstring_view, contains_character)
{
    auto test = []() {
        wchar ptr[] = L"abcdefghijklmnopqrstuvwxyz";
        wchar ptr2[] = L"123456789abcdefghijklmnopqrstuvwxyz123456789_";
        hud::wstring_view v(ptr);
        hud::wstring_view v2(ptr2);

        bool find_0 = v.contains('a');
        bool find_1 = v.contains('d');
        bool find_2 = v.contains('g');
        bool find_3 = v.contains('m');
        bool find_4 = !v.contains('_');
        bool find_5 = !v.contains('1');
        bool find_6 = v2.contains('a');
        bool find_7 = v2.contains('d');
        bool find_8 = v2.contains('g');
        bool find_9 = v2.contains('m');
        bool find_10 = v2.contains('_');
        bool find_11 = v2.contains('1');
        return std::tuple {
            find_0,
            find_1,
            find_2,
            find_3,
            find_4,
            find_5,
            find_6,
            find_7,
            find_8,
            find_9,
            find_10,
            find_11,
        };
    };
    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_true(std::get<6>(result));
        hud_assert_true(std::get<7>(result));
        hud_assert_true(std::get<8>(result));
        hud_assert_true(std::get<9>(result));
        hud_assert_true(std::get<10>(result));
        hud_assert_true(std::get<11>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_true(std::get<6>(result));
        hud_assert_true(std::get<7>(result));
        hud_assert_true(std::get<8>(result));
        hud_assert_true(std::get<9>(result));
        hud_assert_true(std::get<10>(result));
        hud_assert_true(std::get<11>(result));
    }
}

GTEST_TEST(wstring_view, to_uppercase)
{
    wchar wide_txt[] = L"abc123,;:!";
    hud::wstring_view w(wide_txt);
    w.to_uppercase();
    hud_assert_true(w[0] == L'A' && w[1] == L'B' && w[2] == L'C' && w[3] == L'1' && w[4] == L'2' && w[5] == L'3' && w[6] == L',' && w[7] == L';' && w[8] == L':' && w[9] == L'!' && w[10] == L'\0');
}

GTEST_TEST(wstring_view, to_uppercase_partial)
{
    wchar txt[] = L"abc123,;:!";
    hud::wstring_view v(txt);
    v.to_uppercase_partial(2);
    hud_assert_true(v[0] == L'A' && v[1] == L'B' && v[2] == L'c' && v[3] == L'1' && v[4] == L'2' && v[5] == L'3' && v[6] == L',' && v[7] == L';' && v[8] == L':' && v[9] == L'!' && v[10] == L'\0');
}

GTEST_TEST(wstring_view, to_lowercase)
{
    wchar txt[] = L"ABC123,;:!";
    hud::wstring_view v(txt);
    v.to_lowercase();
    hud_assert_true(txt[0] == L'a' && txt[1] == L'b' && txt[2] == L'c' && txt[3] == L'1' && txt[4] == L'2' && txt[5] == L'3' && txt[6] == L',' && txt[7] == L';' && txt[8] == L':' && txt[9] == L'!' && txt[10] == '\0');
}

GTEST_TEST(wstring_view, to_lowercase_partial)
{
    wchar txt[] = L"ABC123,;:!";
    hud::wstring_view v(txt);
    v.to_lowercase_partial(2);
    hud_assert_true(txt[0] == L'a' && txt[1] == L'b' && txt[2] == L'C' && txt[3] == L'1' && txt[4] == L'2' && txt[5] == L'3' && txt[6] == L',' && txt[7] == L';' && txt[8] == L':' && txt[9] == L'!' && txt[10] == '\0');
}