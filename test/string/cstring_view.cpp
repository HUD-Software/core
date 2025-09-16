#include <core/string/cstring_view.h>

GTEST_TEST(cstring_view, constructors)
{
    auto test = []() {
        const ansichar *ptr = "hello world hello world hello world";
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
    static_assert(hud::is_same_v<hud::cstring_view<const ansichar>::char_type, const ansichar>);
    static_assert(hud::is_same_v<hud::cstring_view<ansichar>::char_type, ansichar>);

    hud::cstring_view v {"Hello world"};
    static_assert(hud::is_same_v<decltype(v)::char_type, const ansichar>);

    ansichar ptr[] = "Hello world";
    hud::cstring_view v1 {ptr};
    static_assert(hud::is_same_v<decltype(v1)::char_type, ansichar>);
}

GTEST_TEST(cstring_view, is_ascii)
{
    auto test = []() {
        const ansichar *ptr = "hello world hello world hello world";
        hud::cstring_view v(ptr);
        const ansichar *ptr_2 = "hello world\x80 hello world hello world";
        hud::cstring_view v2(ptr_2);
        const ansichar *ptr_3 = "hello world\x85 hello world hello world";
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

GTEST_TEST(cstring_view, equals)
{
    auto test = []() {
        ansichar ptr[] = "hello world hello world hello world";
        ansichar ptr2[] = "hello world hello world2 hello world";
        ansichar ptr3[] = "hello world hello world hello world";
        ansichar ptr4[] = "hello world hello world hello";
        hud::cstring_view v(ptr);
        hud::cstring_view v2(ptr2);
        hud::cstring_view v3(ptr3);
        hud::cstring_view v4(ptr4);

        return std::tuple {
            static_cast<ansichar *>(ptr) != static_cast<ansichar *>(ptr2),  // 0
            static_cast<ansichar *>(ptr2) != static_cast<ansichar *>(ptr3), // 1
            v.equals(v),                                                    // 2
            !v.equals(v2),                                                  // 3
            v.equals(v3),                                                   // 4
            !v2.equals(v3),                                                 // 5
            !v4.equals(v),                                                  // 6
            !v.equals(v4),                                                  // 7
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

GTEST_TEST(cstring_view, equals_partial)
{
    auto test = []() {
        ansichar ptr[] = "hello world hello world hello world";
        ansichar ptr2[] = "hello world hello world2 hello world";
        ansichar ptr3[] = "hello world hello world hello world";
        ansichar ptr4[] = "hello world hello world hello";
        hud::cstring_view v(ptr);
        hud::cstring_view v2(ptr2);
        hud::cstring_view v3(ptr3);
        hud::cstring_view v4(ptr4);

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

GTEST_TEST(cstring_view, find_first)
{
    auto test = []() {
        ansichar ptr[] = "abcdefghijklmnopqrstuvwxyz";
        ansichar ptr2[] = "123456789abcdefghijklmnopqrstuvwxyz123456789_";
        hud::cstring_view v(ptr);
        hud::cstring_view v2(ptr2);

        bool find_0 = v.find_first("abc") == 0;
        bool find_1 = v.find_first("def") == 3;
        bool find_2 = v.find_first("ghijkl") == 6;
        bool find_3 = v.find_first("mnopqrstuvwxyz") == 12;
        bool find_4 = v.find_first("_") == -1;
        bool find_5 = v.find_first("123456789") == -1;
        bool find_6 = v2.find_first("abc") == 9;
        bool find_7 = v2.find_first("def") == 12;
        bool find_8 = v2.find_first("ghijkl") == 15;
        bool find_9 = v2.find_first("mnopqrstuvwxyz") == 21;
        bool find_10 = v2.find_first("_") == 44 && v2[44] == '_';
        bool find_11 = v2.find_first("123456789") == 0;
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

GTEST_TEST(cstring_view, find_first_character)
{
    auto test = []() {
        ansichar ptr[] = "abcdefghijklmnopqrstuvwxyz";
        ansichar ptr2[] = "123456789abcdefghijklmnopqrstuvwxyz123456789_";
        hud::cstring_view v(ptr);
        hud::cstring_view v2(ptr2);

        bool find_0 = v.find_first_character('a') == 0;
        bool find_1 = v.find_first_character('d') == 3;
        bool find_2 = v.find_first_character('g') == 6;
        bool find_3 = v.find_first_character('m') == 12;
        bool find_4 = v.find_first_character('_') == -1;
        bool find_5 = v.find_first_character('1') == -1;
        bool find_6 = v2.find_first_character('a') == 9;
        bool find_7 = v2.find_first_character('d') == 12;
        bool find_8 = v2.find_first_character('g') == 15;
        bool find_9 = v2.find_first_character('m') == 21;
        bool find_10 = v2.find_first_character('_') == 44 && v2[44] == '_';
        bool find_11 = v2.find_first_character('1') == 0;
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

GTEST_TEST(cstring_view, contains_string)
{
    auto test = []() {
        ansichar ptr[] = "abcdefghijklmnopqrstuvwxyz";
        ansichar ptr2[] = "123456789abcdefghijklmnopqrstuvwxyz123456789_";
        hud::cstring_view v(ptr);
        hud::cstring_view v2(ptr2);

        bool find_0 = v.contains("abc");
        bool find_1 = v.contains("def");
        bool find_2 = v.contains("ghijkl");
        bool find_3 = v.contains("mnopqrstuvwxyz");
        bool find_4 = !v.contains("_");
        bool find_5 = !v.contains("123456789");
        bool find_6 = v2.contains("abc");
        bool find_7 = v2.contains("def");
        bool find_8 = v2.contains("ghijkl");
        bool find_9 = v2.contains("mnopqrstuvwxyz");
        bool find_10 = v2.contains("_");
        bool find_11 = v2.contains("123456789");
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

GTEST_TEST(cstring_view, contains_character)
{
    auto test = []() {
        ansichar ptr[] = "abcdefghijklmnopqrstuvwxyz";
        ansichar ptr2[] = "123456789abcdefghijklmnopqrstuvwxyz123456789_";
        hud::cstring_view v(ptr);
        hud::cstring_view v2(ptr2);

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

GTEST_TEST(cstring_view, to_uppercase)
{
    ansichar txt[] = "abc123,;:!";
    hud::cstring_view v(txt);
    v.to_uppercase();
    hud_assert_true(v[0] == 'A' && v[1] == 'B' && v[2] == 'C' && v[3] == '1' && v[4] == '2' && v[5] == '3' && v[6] == ',' && v[7] == ';' && v[8] == ':' && v[9] == '!' && v[10] == '\0');
}

GTEST_TEST(cstring_view, to_uppercase_partial)
{
    ansichar txt[] = "abc123,;:!";
    hud::cstring_view v(txt);
    v.to_uppercase_partial(2);
    hud_assert_true(v[0] == 'A' && v[1] == 'B' && v[2] == 'c' && v[3] == '1' && v[4] == '2' && v[5] == '3' && v[6] == ',' && v[7] == ';' && v[8] == ':' && v[9] == '!' && v[10] == '\0');
}

GTEST_TEST(cstring_view, to_lowercase)
{
    ansichar txt[] = "ABC123,;:!";
    hud::cstring_view v(txt);
    v.to_lowercase();
    hud_assert_true(txt[0] == 'a' && txt[1] == 'b' && txt[2] == 'c' && txt[3] == '1' && txt[4] == '2' && txt[5] == '3' && txt[6] == ',' && txt[7] == ';' && txt[8] == ':' && txt[9] == '!' && txt[10] == '\0');
}

GTEST_TEST(cstring_view, to_lowercase_partial)
{
    ansichar txt[] = "ABC123,;:!";
    hud::cstring_view v(txt);
    v.to_lowercase_partial(2);
    hud_assert_true(txt[0] == 'a' && txt[1] == 'b' && txt[2] == 'C' && txt[3] == '1' && txt[4] == '2' && txt[5] == '3' && txt[6] == ',' && txt[7] == ';' && txt[8] == ':' && txt[9] == '!' && txt[10] == '\0');
}
