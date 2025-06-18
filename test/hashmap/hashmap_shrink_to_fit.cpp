#include <core/containers/hashmap.h>

GTEST_TEST(hashmap, shrink_to_fit_on_empty_do_nothing)
{
    using key_type = hud_test::non_bitwise_type;
    using value_type = hud_test::non_bitwise_type;
    using hashmap_type = hud::hashmap<key_type, value_type>;

    const auto test = []()
    {
        hashmap_type map;
        bool ok = map.count() == 0;
        ok &= map.max_count() == 0;
        ok &= map.slack() == 0;
        map.shrink_to_fit();
        ok &= map.count() == 0;
        ok &= map.max_count() == 0;
        ok &= map.slack() == 0;

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

GTEST_TEST(hashmap, shrink_to_fit_empty_but_reserved_is_ok)
{
    using key_type = hud_test::non_bitwise_type;
    using value_type = hud_test::non_bitwise_type;
    using hashmap_type = hud::hashmap<key_type, value_type>;

    const auto test = []()
    {
        hashmap_type map;
        map.reserve(25);
        bool ok = map.count() == 0;
        ok &= map.max_count() >= 25;
        ok &= map.slack() >= 25;

        // Rehash ( Similar to shrink_to_fit)
        map.shrink_to_fit();

        ok &= map.count() == 0;
        ok &= map.max_count() == 0;
        ok &= map.slack() == 0;

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

GTEST_TEST(hashmap, shrink_to_fit_non_empty_and_reserved_is_ok)
{
    using key_type = hud_test::non_bitwise_type;
    using value_type = hud_test::non_bitwise_type;
    using hashmap_type = hud::hashmap<key_type, value_type>;

    const auto test = []()
    {
        hashmap_type map;
        map.reserve(25);
        map.add(1, 11);
        map.add(2, 22);
        map.add(3, 33);
        bool ok = map.count() == 3;
        ok &= map.max_count() >= 25;
        ok &= map.slack() >= 25;

        // Check we can iterate over values
        usize elem_count = 0;
        for (const auto &_ : map)
        {
            elem_count++;
        }
        ok &= elem_count == 3;
        auto find_1 = map.find(1);
        ok &= find_1->key() == 1;
        ok &= find_1->value() == 11;
        auto find_2 = map.find(2);
        ok &= find_2->key() == 2;
        ok &= find_2->value() == 22;
        auto find_3 = map.find(3);
        ok &= find_3->key() == 3;
        ok &= find_3->value() == 33;

        // Rehash ( Similar to shrink_to_fit)
        map.shrink_to_fit();
        ok &= map.count() == 3;
        ok &= map.max_count() == 3;
        ok &= map.slack() == 0;
        // Check we can iterate over values
        elem_count = 0;
        for (const auto &_ : map)
        {
            elem_count++;
        }
        ok &= elem_count == 3;
        find_1 = map.find(1);
        ok &= find_1->key() == 1;
        ok &= find_1->value() == 11;
        find_2 = map.find(2);
        ok &= find_2->key() == 2;
        ok &= find_2->value() == 22;
        find_3 = map.find(3);
        ok &= find_3->key() == 3;
        ok &= find_3->value() == 33;

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

GTEST_TEST(hashmap, shrink_to_fit_non_empty_is_ok)
{
    using key_type = hud_test::non_bitwise_type;
    using value_type = hud_test::non_bitwise_type;
    using hashmap_type = hud::hashmap<key_type, value_type>;

    const auto test = []()
    {
        hashmap_type map;

        map.add(1, 11);
        map.add(2, 22);
        map.add(3, 33);
        map.add(4, 44);
        map.remove(3);
        bool ok = map.count() == 3;
        ok &= map.max_count() == 7;
        ok &= map.slack() == 3;

        // Check we can iterate over values
        usize elem_count = 0;
        for (const auto &_ : map)
        {
            elem_count++;
        }
        ok &= elem_count == 3;
        auto find_1 = map.find(1);
        ok &= find_1->key() == 1;
        ok &= find_1->value() == 11;
        auto find_2 = map.find(2);
        ok &= find_2->key() == 2;
        ok &= find_2->value() == 22;
        auto find_4 = map.find(4);
        ok &= find_4->key() == 4;
        ok &= find_4->value() == 44;
        // Rehash ( Similar to shrink_to_fit)
        map.shrink_to_fit();

        // Check we have shrink_to_fit
        ok &= map.count() == 3;
        ok &= map.max_count() == 3;
        ok &= map.slack() == 0;
        // Check we can iterate over values
        elem_count = 0;
        for (const auto &_ : map)
        {
            elem_count++;
        }
        ok &= elem_count == 3;
        find_1 = map.find(1);
        ok &= find_1->key() == 1;
        ok &= find_1->value() == 11;
        find_2 = map.find(2);
        ok &= find_2->key() == 2;
        ok &= find_2->value() == 22;
        find_4 = map.find(4);
        ok &= find_4->key() == 4;
        ok &= find_4->value() == 44;

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