#include <core/string/ascii_string_view.h>

GTEST_TEST(ascii_string_view, make_ascii_string_view_checked)
{
    auto test = []() {
        const char8 *ptr = "hello world hello world hello world";
        auto cstring_view_opt = hud::make_ascii_string_view_checked(ptr);
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

GTEST_TEST(ascii_string_view, inner_type)
{
    static_assert(hud::is_same_v<hud::ascii_string_view<const char8>::char_type, const char8>);
    static_assert(hud::is_same_v<hud::ascii_string_view<char8>::char_type, char8>);

    hud::ascii_string_view v {"Hello world"};
    static_assert(hud::is_same_v<decltype(v)::char_type, const char8>);

    char8 ptr[] = "Hello world";
    hud::ascii_string_view v1 {ptr};
    static_assert(hud::is_same_v<decltype(v1)::char_type, char8>);
}

GTEST_TEST(ascii_string_view, length)
{
    hud::ascii_string_view v {"Hello world"};
    hud_assert_true(v.length() == 11);

    hud::ascii_string_view v2 {"Hello \0world"};
    hud_assert_true(v2.length() == 6);
}

GTEST_TEST(ascii_string_view, data)
{
    const char8 *ptr = "Hello world";
    hud::ascii_string_view v {ptr};
    hud_assert_true(v.data() == ptr);
}

GTEST_TEST(ascii_string_view, is_empty)
{
    hud::ascii_string_view v {"Hello world"};
    hud_assert_false(v.is_empty());

    hud::ascii_string_view v_null {""};
    hud_assert_true(v_null.is_empty());
}

GTEST_TEST(ascii_string_view, equals)
{
    auto test = []() {
        char8 ptr[] = "hello world hello world hello world";
        char8 ptr2[] = "hello world hello world2 hello world";
        char8 ptr3[] = "hello world hello world hello world";
        char8 ptr4[] = "hello world hello world hello";
        hud::ascii_string_view v(ptr);
        hud::ascii_string_view v2(ptr2);
        hud::ascii_string_view v3(ptr3);
        hud::ascii_string_view v4(ptr4);

        return std::tuple {
            static_cast<char8 *>(ptr) != static_cast<char8 *>(ptr2),  // 0
            static_cast<char8 *>(ptr2) != static_cast<char8 *>(ptr3), // 1
            v.equals(v),                                              // 2
            !v.equals(v2),                                            // 3
            v.equals(v3),                                             // 4
            !v2.equals(v3),                                           // 5
            !v4.equals(v),                                            // 6
            !v.equals(v4),                                            // 7
        };
    };

    // Non Constant
    {
        auto result = test();
        hud_test::for_each_value(std::make_index_sequence<std::tuple_size_v<decltype(result)>>(), [&]<usize value>() {
            hud_assert_true(std::get<value>(result));
        });
    }

    // Constant
    {
        constexpr auto result = test();
        hud_test::for_each_value(std::make_index_sequence<std::tuple_size_v<decltype(result)>>(), [&]<usize value>() {
            hud_assert_true(std::get<value>(result));
        });
    }
}

GTEST_TEST(ascii_string_view, equals_partial)
{
    auto test = []() {
        char8 ptr[] = "hello world hello world hello world";
        char8 ptr2[] = "hello world hello world2 hello world";
        char8 ptr3[] = "hello world hello world hello world";
        char8 ptr4[] = "hello world hello world hello";
        hud::ascii_string_view v(ptr);
        hud::ascii_string_view v2(ptr2);
        hud::ascii_string_view v3(ptr3);
        hud::ascii_string_view v4(ptr4);

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

    // Non Constant
    {
        auto result = test();
        hud_test::for_each_value(std::make_index_sequence<std::tuple_size_v<decltype(result)>>(), [&]<usize value>() {
            hud_assert_true(std::get<value>(result));
        });
    }

    // Constant
    {
        constexpr auto result = test();
        hud_test::for_each_value(std::make_index_sequence<std::tuple_size_v<decltype(result)>>(), [&]<usize value>() {
            hud_assert_true(std::get<value>(result));
        });
    }
}

GTEST_TEST(ascii_string_view, find_first)
{
    auto test = []() {
        char8 ptr[] = "abcdefghijklmnopqrstuvwxyz";
        char8 ptr2[] = "123456789abcdefghijklmnopqrstuvwxyz123456789_";
        hud::ascii_string_view v(ptr);
        hud::ascii_string_view v2(ptr2);

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

        bool find_12 = v.find_first(hud::ascii_string_view {"abc"}) == 0;
        bool find_13 = v.find_first(hud::ascii_string_view {"def"}) == 3;
        bool find_14 = v.find_first(hud::ascii_string_view {"ghijkl"}) == 6;
        bool find_15 = v.find_first(hud::ascii_string_view {"mnopqrstuvwxyz"}) == 12;
        bool find_16 = v.find_first(hud::ascii_string_view {"_"}) == -1;
        bool find_17 = v.find_first(hud::ascii_string_view {"123456789"}) == -1;
        bool find_18 = v2.find_first(hud::ascii_string_view {"abc"}) == 9;
        bool find_19 = v2.find_first(hud::ascii_string_view {"def"}) == 12;
        bool find_20 = v2.find_first(hud::ascii_string_view {"ghijkl"}) == 15;
        bool find_21 = v2.find_first(hud::ascii_string_view {"mnopqrstuvwxyz"}) == 21;
        bool find_22 = v2.find_first(hud::ascii_string_view {"_"}) == 44 && v2[44] == '_';
        bool find_23 = v2.find_first(hud::ascii_string_view {"123456789"}) == 0;

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
            find_12,
            find_13,
            find_14,
            find_15,
            find_16,
            find_17,
            find_18,
            find_19,
            find_20,
            find_21,
            find_22,
            find_23,
        };
    };
    // Non Constant
    {
        auto result = test();
        hud_test::for_each_value(std::make_index_sequence<std::tuple_size_v<decltype(result)>>(), [&]<usize value>() {
            hud_assert_true(std::get<value>(result));
        });
    }

    // Constant
    {
        constexpr auto result = test();
        hud_test::for_each_value(std::make_index_sequence<std::tuple_size_v<decltype(result)>>(), [&]<usize value>() {
            hud_assert_true(std::get<value>(result));
        });
    }
}

GTEST_TEST(ascii_string_view, find_first_character)
{
    auto test = []() {
        char8 ptr[] = "abcdefghijklmnopqrstuvwxyz";
        char8 ptr2[] = "123456789abcdefghijklmnopqrstuvwxyz123456789_";
        hud::ascii_string_view v(ptr);
        hud::ascii_string_view v2(ptr2);

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
    // Non Constant
    {
        auto result = test();
        hud_test::for_each_value(std::make_index_sequence<std::tuple_size_v<decltype(result)>>(), [&]<usize value>() {
            hud_assert_true(std::get<value>(result));
        });
    }

    // Constant
    {
        constexpr auto result = test();
        hud_test::for_each_value(std::make_index_sequence<std::tuple_size_v<decltype(result)>>(), [&]<usize value>() {
            hud_assert_true(std::get<value>(result));
        });
    }
}

GTEST_TEST(ascii_string_view, contains_string)
{
    auto test = []() {
        char8 ptr[] = "abcdefghijklmnopqrstuvwxyz";
        char8 ptr2[] = "123456789abcdefghijklmnopqrstuvwxyz123456789_";
        hud::ascii_string_view v(ptr);
        hud::ascii_string_view v2(ptr2);

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

        bool find_12 = v.contains(hud::ascii_string_view {"abc"});
        bool find_13 = v.contains(hud::ascii_string_view {"def"});
        bool find_14 = v.contains(hud::ascii_string_view {"ghijkl"});
        bool find_15 = v.contains(hud::ascii_string_view {"mnopqrstuvwxyz"});
        bool find_16 = !v.contains(hud::ascii_string_view {"_"});
        bool find_17 = !v.contains(hud::ascii_string_view {"123456789"});
        bool find_18 = v2.contains(hud::ascii_string_view {"abc"});
        bool find_19 = v2.contains(hud::ascii_string_view {"def"});
        bool find_20 = v2.contains(hud::ascii_string_view {"ghijkl"});
        bool find_21 = v2.contains(hud::ascii_string_view {"mnopqrstuvwxyz"});
        bool find_22 = v2.contains(hud::ascii_string_view {"_"});
        bool find_23 = v2.contains(hud::ascii_string_view {"123456789"});

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
            find_12,
            find_13,
            find_14,
            find_15,
            find_16,
            find_17,
            find_18,
            find_19,
            find_20,
            find_21,
            find_22,
            find_23,
        };
    };
    // Non Constant
    {
        auto result = test();
        hud_test::for_each_value(std::make_index_sequence<std::tuple_size_v<decltype(result)>>(), [&]<usize value>() {
            hud_assert_true(std::get<value>(result));
        });
    }

    // Constant
    {
        constexpr auto result = test();
        hud_test::for_each_value(std::make_index_sequence<std::tuple_size_v<decltype(result)>>(), [&]<usize value>() {
            hud_assert_true(std::get<value>(result));
        });
    }
}

GTEST_TEST(ascii_string_view, contains_character)
{
    auto test = []() {
        char8 ptr[] = "abcdefghijklmnopqrstuvwxyz";
        char8 ptr2[] = "123456789abcdefghijklmnopqrstuvwxyz123456789_";
        hud::ascii_string_view v(ptr);
        hud::ascii_string_view v2(ptr2);

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
    // Non Constant
    {
        auto result = test();
        hud_test::for_each_value(std::make_index_sequence<std::tuple_size_v<decltype(result)>>(), [&]<usize value>() {
            hud_assert_true(std::get<value>(result));
        });
    }

    // Constant
    {
        constexpr auto result = test();
        hud_test::for_each_value(std::make_index_sequence<std::tuple_size_v<decltype(result)>>(), [&]<usize value>() {
            hud_assert_true(std::get<value>(result));
        });
    }
}

GTEST_TEST(ascii_string_view, to_uppercase)
{
    char8 txt[] = "abc123,;:!";
    hud::ascii_string_view v(txt);
    v.to_uppercase();
    hud_assert_true(v[0] == 'A' && v[1] == 'B' && v[2] == 'C' && v[3] == '1' && v[4] == '2' && v[5] == '3' && v[6] == ',' && v[7] == ';' && v[8] == ':' && v[9] == '!' && v[10] == '\0');
}

GTEST_TEST(ascii_string_view, to_uppercase_partial)
{
    char8 txt[] = "abc123,;:!";
    hud::ascii_string_view v(txt);
    v.to_uppercase_partial(2);
    hud_assert_true(v[0] == 'A' && v[1] == 'B' && v[2] == 'c' && v[3] == '1' && v[4] == '2' && v[5] == '3' && v[6] == ',' && v[7] == ';' && v[8] == ':' && v[9] == '!' && v[10] == '\0');
}

GTEST_TEST(ascii_string_view, to_lowercase)
{
    char8 txt[] = "ABC123,;:!";
    hud::ascii_string_view v(txt);
    v.to_lowercase();
    hud_assert_true(txt[0] == 'a' && txt[1] == 'b' && txt[2] == 'c' && txt[3] == '1' && txt[4] == '2' && txt[5] == '3' && txt[6] == ',' && txt[7] == ';' && txt[8] == ':' && txt[9] == '!' && txt[10] == '\0');
}

GTEST_TEST(ascii_string_view, to_lowercase_partial)
{
    char8 txt[] = "ABC123,;:!";
    hud::ascii_string_view v(txt);
    v.to_lowercase_partial(2);
    hud_assert_true(txt[0] == 'a' && txt[1] == 'b' && txt[2] == 'C' && txt[3] == '1' && txt[4] == '2' && txt[5] == '3' && txt[6] == ',' && txt[7] == ';' && txt[8] == ':' && txt[9] == '!' && txt[10] == '\0');
}

GTEST_TEST(ascii_string_view, is_valid)
{
    auto test = []() {
        const char8 *ptr = "hello world hello world hello world";
        hud::ascii_string_view v(ptr);
        const char8 *ptr_2 = "hello world\x80 hello world hello world";
        hud::ascii_string_view v2(ptr_2);
        const char8 *ptr_3 = "hello world\x85 hello world hello world";
        hud::ascii_string_view v3(ptr_3);
        return std::tuple {
            v.is_valid(),   // 0
            !v2.is_valid(), // 1
            !v3.is_valid()  // 2
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

GTEST_TEST(ascii_string_view, slice)
{
    hud::ascii_string_view const_v {"ABC"};
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
    hud::ascii_string_view v {txt};
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