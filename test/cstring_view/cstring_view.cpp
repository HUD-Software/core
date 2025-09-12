#include <core/string/cstring_view.h>

GTEST_TEST(cstring_view, constructors)
{
    auto test = []() {
        const ansichar *ptr = "hello world hello world hello world";
        hud::cstring_view v(ptr);
        hud::cstring_view v2(ptr, hud::cstring::length(ptr));
        return std::tuple {
            v.length() == hud::cstring::length(ptr),  // 0
            v.data() == ptr,                          // 1
            v2.length() == hud::cstring::length(ptr), // 2
            v2.data() == ptr,                         // 3
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

        return std::tuple {
            v.equals_partial(v, v.length()),         // 0
            v.equals_partial(v, v.length() - 1),     // 1
            v.equals_partial(v, v.length() + 1),     // 2
            !v.equals_partial(v2, v2.length()),      // 3
            !v.equals_partial(v2, v2.length() - 1),  // 4
            !v.equals_partial(v2, v2.length() + 1),  // 5
            v.equals_partial(v2, 8),                 // 6
            v.equals_partial(v3, v3.length()),       // 7
            v.equals_partial(v3, v3.length() - 1),   // 8
            v.equals_partial(v3, v3.length() + 1),   // 9
            v.equals_partial(v4, v4.length()),       // 10
            v.equals_partial(v4, v4.length() - 1),   // 11
            v.equals_partial(v4, v4.length() + 1),   // 12
            v.equals_partial(v4, v.length()),        // 13
            !v2.equals_partial(v3, v3.length()),     // 14
            !v2.equals_partial(v3, v3.length() - 1), // 15
            !v2.equals_partial(v3, v3.length() + 1), // 16
            v2.equals_partial(v3, 8),                // 17
            v4.equals_partial(v, v4.length()),       // 18
            v4.equals_partial(v, v4.length() - 1),   // 19
            v4.equals_partial(v, v4.length() + 1),   // 20
            !v4.equals_partial(v2, v4.length()),     // 21
            !v4.equals_partial(v2, v4.length() - 1), // 22
            !v4.equals_partial(v2, v4.length() + 1), // 23
            v4.equals_partial(v2, 8),                // 24
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
