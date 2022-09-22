#include <core/containers/pair.h>

TEST(pair, operator_equal) {


    const auto test = [](hud::pair<i32, wchar> r, hud::pair<i32, wchar> l) {
        return r == l;
    };

    // Non constant
    {
        ASSERT_TRUE(test({ 1,L'a' }, { 1,L'a' }));
        ASSERT_FALSE(test({ 1,L'a' }, { 2, L'a' }));
        ASSERT_FALSE(test({ 1,L'a' }, { 1, L'b' }));
    }

    // Constant
    {
        constexpr auto eq_1 = test({ 1,L'a' }, { 1,L'a' });
        ASSERT_TRUE(eq_1);
        constexpr auto eq_2 = test({ 1,L'a' }, { 2, L'a' });
        ASSERT_FALSE(eq_2);
        constexpr auto eq_3 = test({ 1,L'a' }, { 1, L'b' });
        ASSERT_FALSE(eq_3);
    }
}

TEST(pair, operator_not_equal) {


    const auto test = [](hud::pair<i32, wchar> r, hud::pair<i32, wchar> l) {
        return r != l;
    };

    // Non constant
    {
        ASSERT_FALSE(test({ 1,L'a' }, { 1,L'a' }));
        ASSERT_TRUE(test({ 1,L'a' }, { 2, L'a' }));
        ASSERT_TRUE(test({ 1,L'a' }, { 1, L'b' }));
    }

    // Constant
    {
        constexpr auto ne_1 = test({ 1,L'a' }, { 1,L'a' });
        ASSERT_FALSE(ne_1);
        constexpr auto ne_2 = test({ 1,L'a' }, { 2, L'a' });
        ASSERT_TRUE(ne_2);
        constexpr auto ne_3 = test({ 1,L'a' }, { 1, L'b' });
        ASSERT_TRUE(ne_3);
    }
}


TEST(pair, operator_less) {


    const auto test = [](hud::pair<i32, wchar> r, hud::pair<i32, wchar> l) {
        return r < l;
    };

    // Non constant
    {
        ASSERT_FALSE(test({ 1,L'a' }, { 1,L'a' }));
        ASSERT_TRUE(test({ 1,L'a' }, { 2, L'a' }));
        ASSERT_TRUE(test({ 1,L'a' }, { 1, L'b' }));
        ASSERT_FALSE(test({ 1,L'b' }, { 1, L'a' }));
    }

    // Constant
    {
        constexpr auto le_1 = test({ 1,L'a' }, { 1,L'a' });
        ASSERT_FALSE(le_1);
        constexpr auto le_2 = test({ 1,L'a' }, { 2, L'a' });
        ASSERT_TRUE(le_2);
        constexpr auto le_3 = test({ 1,L'a' }, { 1, L'b' });
        ASSERT_TRUE(le_3);
        constexpr auto le_4 = test({ 1,L'b' }, { 1, L'a' });
        ASSERT_FALSE(le_4);

    }
}

TEST(pair, operator_less_equal) {


    const auto test = [](hud::pair<i32, wchar> r, hud::pair<i32, wchar> l) {
        return r <= l;
    };

    // Non constant
    {
        ASSERT_TRUE(test({ 1,L'a' }, { 1,L'a' }));
        ASSERT_TRUE(test({ 1,L'a' }, { 2, L'a' }));
        ASSERT_TRUE(test({ 1,L'a' }, { 1, L'b' }));
        ASSERT_FALSE(test({ 1,L'b' }, { 1, L'a' }));
    }

    // Constant
    {
        constexpr auto le_1 = test({ 1,L'a' }, { 1,L'a' });
        ASSERT_TRUE(le_1);
        constexpr auto le_2 = test({ 1,L'a' }, { 2, L'a' });
        ASSERT_TRUE(le_2);
        constexpr auto le_3 = test({ 1,L'a' }, { 1, L'b' });
        ASSERT_TRUE(le_3);
        constexpr auto le_4 = test({ 1,L'b' }, { 1, L'a' });
        ASSERT_FALSE(le_4);

    }
}

TEST(pair, operator_greater) {


    const auto test = [](hud::pair<i32, wchar> r, hud::pair<i32, wchar> l) {
        return r > l;
    };

    // Non constant
    {
        ASSERT_FALSE(test({ 1,L'a' }, { 1,L'a' }));
        ASSERT_FALSE(test({ 1,L'a' }, { 2, L'a' }));
        ASSERT_FALSE(test({ 1,L'a' }, { 1, L'b' }));
        ASSERT_TRUE(test({ 1,L'b' }, { 1, L'a' }));
    }

    // Constant
    {
        constexpr auto gr_1 = test({ 1,L'a' }, { 1,L'a' });
        ASSERT_FALSE(gr_1);
        constexpr auto gr_2 = test({ 1,L'a' }, { 2, L'a' });
        ASSERT_FALSE(gr_2);
        constexpr auto gr_3 = test({ 1,L'a' }, { 1, L'b' });
        ASSERT_FALSE(gr_3);
        constexpr auto gr_4 = test({ 1,L'b' }, { 1, L'a' });
        ASSERT_TRUE(gr_4);

    }
}


TEST(pair, operator_greater_equal) {


    const auto test = [](hud::pair<i32, wchar> r, hud::pair<i32, wchar> l) {
        return r >= l;
    };

    // Non constant
    {
        ASSERT_TRUE(test({ 1,L'a' }, { 1,L'a' }));
        ASSERT_FALSE(test({ 1,L'a' }, { 2, L'a' }));
        ASSERT_FALSE(test({ 1,L'a' }, { 1, L'b' }));
        ASSERT_TRUE(test({ 1,L'b' }, { 1, L'a' }));
    }

    // Constant
    {
        constexpr auto ge_1 = test({ 1,L'a' }, { 1,L'a' });
        ASSERT_TRUE(ge_1);
        constexpr auto ge_2 = test({ 1,L'a' }, { 2, L'a' });
        ASSERT_FALSE(ge_2);
        constexpr auto ge_3 = test({ 1,L'a' }, { 1, L'b' });
        ASSERT_FALSE(ge_3);
        constexpr auto ge_4 = test({ 1,L'b' }, { 1, L'a' });
        ASSERT_TRUE(ge_4);

    }
}