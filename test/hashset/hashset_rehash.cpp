#include <core/containers/hashset.h>

GTEST_TEST(hashset, rehash_0_on_empty_do_nothing)
{
    using key_type = hud_test::non_bitwise_type;
    using hashset_type = hud::hashset<key_type>;

    const auto test = []()
    {
        hashset_type set;
        bool ok = set.count() == 0;
        ok &= set.max_count() == 0;
        ok &= set.slack() == 0;
        set.rehash(0);
        ok &= set.count() == 0;
        ok &= set.max_count() == 0;
        ok &= set.slack() == 0;

        return ok;
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(result);
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(result);
    }
}

GTEST_TEST(hashset, rehash_0_empty_but_reserved_shrink_to_fit)
{
    using key_type = hud_test::non_bitwise_type;
    using hashset_type = hud::hashset<key_type>;

    const auto test = []()
    {
        hashset_type set;
        set.reserve(25);
        bool ok = set.count() == 0;
        ok &= set.max_count() >= 25;
        ok &= set.slack() >= 25;

        // Rehash ( Similar to shrink_to_fit)
        set.rehash(0);

        ok &= set.count() == 0;
        ok &= set.max_count() == 0;
        ok &= set.slack() == 0;

        return ok;
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(result);
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(result);
    }
}

GTEST_TEST(hashset, rehash_0_non_empty_and_reserved_shrink_to_fit)
{
    using key_type = hud_test::non_bitwise_type;
    using hashset_type = hud::hashset<key_type>;

    const auto test = []()
    {
        hashset_type set;
        set.reserve(25);
        set.add(1);
        set.add(2);
        set.add(3);
        bool ok = set.count() == 3;
        ok &= set.max_count() >= 25;
        ok &= set.slack() >= 25;

        // Check we can iterate over values
        usize elem_count = 0;
        for (const auto &_ : set)
        {
            elem_count++;
        }
        ok &= elem_count == 3;
        auto find_1 = set.find(1);
        ok &= find_1->key() == 1;
        auto find_2 = set.find(2);
        ok &= find_2->key() == 2;
        auto find_3 = set.find(3);
        ok &= find_3->key() == 3;

        // Rehash ( Similar to shrink_to_fit)
        set.rehash(0);
        ok &= set.count() == 3;
        ok &= set.max_count() == 3;
        ok &= set.slack() == 0;
        // Check we can iterate over values
        elem_count = 0;
        for (const auto &_ : set)
        {
            elem_count++;
        }
        ok &= elem_count == 3;
        find_1 = set.find(1);
        ok &= find_1->key() == 1;
        find_2 = set.find(2);
        ok &= find_2->key() == 2;
        find_3 = set.find(3);
        ok &= find_3->key() == 3;

        return ok;
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(result);
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(result);
    }
}

GTEST_TEST(hashset, rehash_0_non_empty_shrink_to_fit)
{
    using key_type = hud_test::non_bitwise_type;
    using hashset_type = hud::hashset<key_type>;

    const auto test = []()
    {
        hashset_type set;

        set.add(1);
        set.add(2);
        set.add(3);
        set.add(4);
        set.remove(3);
        bool ok = set.count() == 3;
        ok &= set.max_count() == 7;
        ok &= set.slack() == 3;

        // Check we can iterate over values
        usize elem_count = 0;
        for (const auto &_ : set)
        {
            elem_count++;
        }
        ok &= elem_count == 3;
        auto find_1 = set.find(1);
        ok &= find_1->key() == 1;
        auto find_2 = set.find(2);
        ok &= find_2->key() == 2;
        auto find_4 = set.find(4);
        ok &= find_4->key() == 4;
        // Rehash ( Similar to shrink_to_fit)
        set.rehash(0);

        // Check we have shrink_to_fit
        ok &= set.count() == 3;
        ok &= set.max_count() == 3;
        ok &= set.slack() == 0;
        // Check we can iterate over values
        elem_count = 0;
        for (const auto &_ : set)
        {
            elem_count++;
        }
        ok &= elem_count == 3;
        find_1 = set.find(1);
        ok &= find_1->key() == 1;
        find_2 = set.find(2);
        ok &= find_2->key() == 2;
        find_4 = set.find(4);
        ok &= find_4->key() == 4;

        return ok;
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(result);
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(result);
    }
}

GTEST_TEST(hashset, rehash_less_than_count_do_nothing)
{
    using key_type = hud_test::non_bitwise_type;
    using hashset_type = hud::hashset<key_type>;

    const auto test = []()
    {
        hashset_type set;
        set.add(1);
        set.add(2);
        set.add(3);
        bool ok = set.count() == 3;
        ok &= set.max_count() == 3;
        ok &= set.slack() == 0;

        // Check we can iterate over values
        usize elem_count = 0;
        for (const auto &_ : set)
        {
            elem_count++;
        }
        ok &= elem_count == 3;
        auto find_1 = set.find(1);
        ok &= find_1->key() == 1;
        auto find_2 = set.find(2);
        ok &= find_2->key() == 2;
        auto find_3 = set.find(3);
        ok &= find_3->key() == 3;

        // Rehash ( Similar to shrink_to_fit)
        set.rehash(2);
        ok &= set.count() == 3;
        ok &= set.max_count() == 3;
        ok &= set.slack() == 0;
        // Check we can iterate over values
        elem_count = 0;
        for (const auto &_ : set)
        {
            elem_count++;
        }
        ok &= elem_count == 3;
        find_1 = set.find(1);
        ok &= find_1->key() == 1;
        find_2 = set.find(2);
        ok &= find_2->key() == 2;
        find_3 = set.find(3);
        ok &= find_3->key() == 3;

        return ok;
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(result);
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(result);
    }
}

GTEST_TEST(hashset, rehash_equal_than_count_do_nothing)
{
    using key_type = hud_test::non_bitwise_type;
    using hashset_type = hud::hashset<key_type>;

    const auto test = []()
    {
        hashset_type set;
        set.add(1);
        set.add(2);
        set.add(3);
        bool ok = set.count() == 3;
        ok &= set.max_count() == 3;
        ok &= set.slack() == 0;

        // Check we can iterate over values
        usize elem_count = 0;
        for (const auto &_ : set)
        {
            elem_count++;
        }
        ok &= elem_count == 3;
        auto find_1 = set.find(1);
        ok &= find_1->key() == 1;
        auto find_2 = set.find(2);
        ok &= find_2->key() == 2;
        auto find_3 = set.find(3);
        ok &= find_3->key() == 3;

        // Rehash ( Similar to shrink_to_fit)
        set.rehash(3);
        ok &= set.count() == 3;
        ok &= set.max_count() == 3;
        ok &= set.slack() == 0;
        // Check we can iterate over values
        elem_count = 0;
        for (const auto &_ : set)
        {
            elem_count++;
        }
        ok &= elem_count == 3;
        find_1 = set.find(1);
        ok &= find_1->key() == 1;
        find_2 = set.find(2);
        ok &= find_2->key() == 2;
        find_3 = set.find(3);
        ok &= find_3->key() == 3;

        return ok;
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(result);
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(result);
    }
}

GTEST_TEST(hashset, rehash_greater_than_count_do_nothing)
{
    using key_type = hud_test::non_bitwise_type;
    using hashset_type = hud::hashset<key_type>;

    const auto test = []()
    {
        hashset_type set;
        set.add(1);
        set.add(2);
        set.add(3);
        bool ok = set.count() == 3;
        ok &= set.max_count() == 3;
        ok &= set.slack() == 0;

        // Check we can iterate over values
        usize elem_count = 0;
        for (const auto &_ : set)
        {
            elem_count++;
        }
        ok &= elem_count == 3;
        auto find_1 = set.find(1);
        ok &= find_1->key() == 1;
        auto find_2 = set.find(2);
        ok &= find_2->key() == 2;
        auto find_3 = set.find(3);
        ok &= find_3->key() == 3;

        // Rehash ( Similar to shrink_to_fit)
        set.rehash(255);
        ok &= set.count() == 3;
        ok &= set.max_count() == 255;
        ok &= set.slack() == 221; // 7/8 of 255 - 3 elements
        // Check we can iterate over values
        elem_count = 0;
        for (const auto &_ : set)
        {
            elem_count++;
        }
        ok &= elem_count == 3;
        find_1 = set.find(1);
        ok &= find_1->key() == 1;
        find_2 = set.find(2);
        ok &= find_2->key() == 2;
        find_3 = set.find(3);
        ok &= find_3->key() == 3;

        return ok;
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(result);
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(result);
    }
}