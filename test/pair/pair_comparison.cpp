#include <core/containers/pair.h>

GTEST_TEST(pair, operator_equal)
{

    const auto test = [](hud::pair<i32, wchar> r, hud::pair<i32, wchar> l)
    {
        return r == l;
    };

    // Non constant
    {
        hud_assert_true(test({1, L'a'}, {1, L'a'}));
        hud_assert_false(test({1, L'a'}, {2, L'a'}));
        hud_assert_false(test({1, L'a'}, {1, L'b'}));
    }

    // Constant
    {
        constexpr auto eq_1 = test({1, L'a'}, {1, L'a'});
        hud_assert_true(eq_1);
        constexpr auto eq_2 = test({1, L'a'}, {2, L'a'});
        hud_assert_false(eq_2);
        constexpr auto eq_3 = test({1, L'a'}, {1, L'b'});
        hud_assert_false(eq_3);
    }
}

GTEST_TEST(pair, operator_not_equal)
{

    const auto test = [](hud::pair<i32, wchar> r, hud::pair<i32, wchar> l)
    {
        return r != l;
    };

    // Non constant
    {
        hud_assert_false(test({1, L'a'}, {1, L'a'}));
        hud_assert_true(test({1, L'a'}, {2, L'a'}));
        hud_assert_true(test({1, L'a'}, {1, L'b'}));
    }

    // Constant
    {
        constexpr auto ne_1 = test({1, L'a'}, {1, L'a'});
        hud_assert_false(ne_1);
        constexpr auto ne_2 = test({1, L'a'}, {2, L'a'});
        hud_assert_true(ne_2);
        constexpr auto ne_3 = test({1, L'a'}, {1, L'b'});
        hud_assert_true(ne_3);
    }
}

GTEST_TEST(pair, operator_less)
{

    const auto test = [](hud::pair<i32, wchar> r, hud::pair<i32, wchar> l)
    {
        return r < l;
    };

    // Non constant
    {
        hud_assert_false(test({1, L'a'}, {1, L'a'}));
        hud_assert_true(test({1, L'a'}, {2, L'a'}));
        hud_assert_true(test({1, L'a'}, {1, L'b'}));
        hud_assert_false(test({1, L'b'}, {1, L'a'}));
    }

    // Constant
    {
        constexpr auto le_1 = test({1, L'a'}, {1, L'a'});
        hud_assert_false(le_1);
        constexpr auto le_2 = test({1, L'a'}, {2, L'a'});
        hud_assert_true(le_2);
        constexpr auto le_3 = test({1, L'a'}, {1, L'b'});
        hud_assert_true(le_3);
        constexpr auto le_4 = test({1, L'b'}, {1, L'a'});
        hud_assert_false(le_4);
    }
}

GTEST_TEST(pair, operator_less_equal)
{

    const auto test = [](hud::pair<i32, wchar> r, hud::pair<i32, wchar> l)
    {
        return r <= l;
    };

    // Non constant
    {
        hud_assert_true(test({1, L'a'}, {1, L'a'}));
        hud_assert_true(test({1, L'a'}, {2, L'a'}));
        hud_assert_true(test({1, L'a'}, {1, L'b'}));
        hud_assert_false(test({1, L'b'}, {1, L'a'}));
    }

    // Constant
    {
        constexpr auto le_1 = test({1, L'a'}, {1, L'a'});
        hud_assert_true(le_1);
        constexpr auto le_2 = test({1, L'a'}, {2, L'a'});
        hud_assert_true(le_2);
        constexpr auto le_3 = test({1, L'a'}, {1, L'b'});
        hud_assert_true(le_3);
        constexpr auto le_4 = test({1, L'b'}, {1, L'a'});
        hud_assert_false(le_4);
    }
}

GTEST_TEST(pair, operator_greater)
{

    const auto test = [](hud::pair<i32, wchar> r, hud::pair<i32, wchar> l)
    {
        return r > l;
    };

    // Non constant
    {
        hud_assert_false(test({1, L'a'}, {1, L'a'}));
        hud_assert_false(test({1, L'a'}, {2, L'a'}));
        hud_assert_false(test({1, L'a'}, {1, L'b'}));
        hud_assert_true(test({1, L'b'}, {1, L'a'}));
    }

    // Constant
    {
        constexpr auto gr_1 = test({1, L'a'}, {1, L'a'});
        hud_assert_false(gr_1);
        constexpr auto gr_2 = test({1, L'a'}, {2, L'a'});
        hud_assert_false(gr_2);
        constexpr auto gr_3 = test({1, L'a'}, {1, L'b'});
        hud_assert_false(gr_3);
        constexpr auto gr_4 = test({1, L'b'}, {1, L'a'});
        hud_assert_true(gr_4);
    }
}

GTEST_TEST(pair, operator_greater_equal)
{

    const auto test = [](hud::pair<i32, wchar> r, hud::pair<i32, wchar> l)
    {
        return r >= l;
    };

    // Non constant
    {
        hud_assert_true(test({1, L'a'}, {1, L'a'}));
        hud_assert_false(test({1, L'a'}, {2, L'a'}));
        hud_assert_false(test({1, L'a'}, {1, L'b'}));
        hud_assert_true(test({1, L'b'}, {1, L'a'}));
    }

    // Constant
    {
        constexpr auto ge_1 = test({1, L'a'}, {1, L'a'});
        hud_assert_true(ge_1);
        constexpr auto ge_2 = test({1, L'a'}, {2, L'a'});
        hud_assert_false(ge_2);
        constexpr auto ge_3 = test({1, L'a'}, {1, L'b'});
        hud_assert_false(ge_3);
        constexpr auto ge_4 = test({1, L'b'}, {1, L'a'});
        hud_assert_true(ge_4);
    }
}