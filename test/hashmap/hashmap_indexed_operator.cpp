#include <core/containers/hashmap.h>

GTEST_TEST(hashmap, indexed_operator_trivial_same_type)
{
    const auto test = []()
    {
        using key_type = u32;
        using value_type = u64;
        hud::hashmap<key_type, value_type> map;

        const key_type key = 123;
        // Should add default value 0
        map[key];
        const bool ok_default = map.find(key)->value() == 0;
        // Should add key, 0
        map[key] = value_type {1};
        const bool ok = map.find(key)->value() == 1;
        // Should replace value of key to 2
        map[key] = value_type {2};
        const bool ok_replace = map.find(key)->value() == 2;

        return std::tuple {
            ok_default,
            ok,
            ok_replace
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

GTEST_TEST(hashmap, indexed_operator_trivial_different_type)
{
    const auto test = []()
    {
        using key_type = u32;
        using value_type = u64;

        using other_key_type = i16;
        using other_value_type = i8;
        static_assert(!hud::is_same_v<key_type, other_key_type>);
        static_assert(!hud::is_same_v<value_type, other_value_type>);

        hud::hashmap<key_type, value_type> map;
        const other_key_type key = 123;

        // Should add default value 0
        map[key];
        const bool ok_default = map.find(key)->value() == 0;
        // Should add key, 0
        map[key] = other_value_type {1};
        const bool ok = map.find(key)->value() == 1;
        // Should replace value of key to 2
        map[key] = other_value_type {2};
        const bool ok_replace = map.find(key)->value() == 2;

        return std::tuple {
            ok_default,
            ok,
            ok_replace
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

GTEST_TEST(hashmap, indexed_operator_non_trivial_copy_same_type)
{
    const auto test = []()
    {
        using key_type = hud_test::non_bitwise_type;
        using value_type = hud_test::non_bitwise_type;
        hud::hashmap<key_type, value_type> map;

        const key_type key = 123;

        // Should add default value 0
        map[key];
        auto &value = map.find(key)->value();
        bool ok_default = value.id() == 0;
        ok_default &= value.constructor_count() == 0;
        ok_default &= value.copy_constructor_count() == 0;
        ok_default &= value.move_constructor_count() == 0;
        ok_default &= value.copy_assign_count() == 0;
        ok_default &= value.move_assign_count() == 0;

        // Should add key, 0
        const value_type to_copy = value_type {1};
        map[key] = to_copy;
        auto &value_ok = map.find(key)->value();
        bool ok = value_ok.id() == 1;
        ok &= value_ok.constructor_count() == 1;
        ok &= value_ok.copy_constructor_count() == 0;
        ok &= value_ok.move_constructor_count() == 0;
        ok &= value_ok.copy_assign_count() == 1;
        ok &= value_ok.move_assign_count() == 0;

        // Should replace value of key to 2
        const value_type to_copy_2 = value_type {2};
        map[key] = to_copy_2;
        auto &value_replace = map.find(key)->value();
        bool ok_replace = value_replace.id() == 2;
        ok_replace &= value_replace.constructor_count() == 1;
        ok_replace &= value_replace.copy_constructor_count() == 0;
        ok_replace &= value_replace.move_constructor_count() == 0;
        ok_replace &= value_replace.copy_assign_count() == 1;
        ok_replace &= value_replace.move_assign_count() == 0;

        return std::tuple {
            ok_default,
            ok,
            ok_replace
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

GTEST_TEST(hashmap, indexed_operator_non_trivial_copy_different_type)
{
    const auto test = []()
    {
        using key_type = hud_test::non_bitwise_type;
        using value_type = hud_test::non_bitwise_type;

        using other_key_type = hud_test::non_bitwise_type2;
        using other_value_type = hud_test::non_bitwise_type2;
        static_assert(!hud::is_same_v<key_type, other_key_type>);
        static_assert(!hud::is_same_v<value_type, other_value_type>);

        hud::hashmap<key_type, value_type> map;
        const other_key_type key = other_key_type {123};

        // Should add default value 0
        map[key];
        auto &value = map.find(key)->value();
        bool ok_default = value.id() == 0;
        ok_default &= value.constructor_count() == 0;
        ok_default &= value.copy_constructor_count() == 0;
        ok_default &= value.move_constructor_count() == 0;
        ok_default &= value.copy_assign_count() == 0;
        ok_default &= value.move_assign_count() == 0;

        // Should add key, 0
        const other_value_type to_copy = other_value_type {1};
        map[key] = to_copy;
        auto &value_ok = map.find(key)->value();
        bool ok = value_ok.id() == 1;
        ok &= value_ok.constructor_count() == 1;
        ok &= value_ok.copy_constructor_count() == 0;
        ok &= value_ok.move_constructor_count() == 0;
        ok &= value_ok.copy_assign_count() == 1;
        ok &= value_ok.move_assign_count() == 0;

        // Should replace value of key to 2
        const other_value_type to_copy_2 = other_value_type {2};
        map[key] = to_copy_2;
        auto &value_replace = map.find(key)->value();
        bool ok_replace = value_replace.id() == 2;
        ok_replace &= value_replace.constructor_count() == 1;
        ok_replace &= value_replace.copy_constructor_count() == 0;
        ok_replace &= value_replace.move_constructor_count() == 0;
        ok_replace &= value_replace.copy_assign_count() == 1;
        ok_replace &= value_replace.move_assign_count() == 0;

        return std::tuple {
            ok_default,
            ok,
            ok_replace
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

GTEST_TEST(hashmap, indexed_operator_non_trivial_move_same_type)
{
    const auto test = []()
    {
        using key_type = hud_test::non_bitwise_type;
        using value_type = hud_test::non_bitwise_type;
        hud::hashmap<key_type, value_type> map;

        const key_type key = 123;

        // Should add default value 0
        map[key];
        auto &value = map.find(key)->value();
        bool ok_default = value.id() == 0;
        ok_default &= value.constructor_count() == 0;
        ok_default &= value.copy_constructor_count() == 0;
        ok_default &= value.move_constructor_count() == 0;
        ok_default &= value.copy_assign_count() == 0;
        ok_default &= value.move_assign_count() == 0;

        // Should add key, 0
        map[key] = value_type {1};
        auto &value_ok = map.find(key)->value();
        bool ok = value_ok.id() == 1;
        ok &= value_ok.constructor_count() == 1;
        ok &= value_ok.copy_constructor_count() == 0;
        ok &= value_ok.move_constructor_count() == 0;
        ok &= value_ok.copy_assign_count() == 0;
        ok &= value_ok.move_assign_count() == 1;

        // Should replace value of key to 2
        map[key] = value_type {2};
        auto &value_replace = map.find(key)->value();
        bool ok_replace = value_replace.id() == 2;
        ok_replace &= value_replace.constructor_count() == 1;
        ok_replace &= value_replace.copy_constructor_count() == 0;
        ok_replace &= value_replace.move_constructor_count() == 0;
        ok_replace &= value_replace.copy_assign_count() == 0;
        ok_replace &= value_replace.move_assign_count() == 1;

        return std::tuple {
            ok_default,
            ok,
            ok_replace
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

GTEST_TEST(hashmap, indexed_operator_non_trivial_move_different_type)
{
    const auto test = []()
    {
        using key_type = hud_test::non_bitwise_type;
        using value_type = hud_test::non_bitwise_type;

        using other_key_type = hud_test::non_bitwise_type2;
        using other_value_type = hud_test::non_bitwise_type2;
        static_assert(!hud::is_same_v<key_type, other_key_type>);
        static_assert(!hud::is_same_v<value_type, other_value_type>);

        hud::hashmap<key_type, value_type> map;
        const other_key_type key = other_key_type {123};

        // Should add default value 0
        map[key];
        auto &value = map.find(key)->value();
        bool ok_default = value.id() == 0;
        ok_default &= value.constructor_count() == 0;
        ok_default &= value.copy_constructor_count() == 0;
        ok_default &= value.move_constructor_count() == 0;
        ok_default &= value.copy_assign_count() == 0;
        ok_default &= value.move_assign_count() == 0;

        // Should add key, 0
        map[key] = other_value_type {1};
        auto &value_ok = map.find(key)->value();
        bool ok = value_ok.id() == 1;
        ok &= value_ok.constructor_count() == 1;
        ok &= value_ok.copy_constructor_count() == 0;
        ok &= value_ok.move_constructor_count() == 0;
        ok &= value_ok.copy_assign_count() == 0;
        ok &= value_ok.move_assign_count() == 1;

        // Should replace value of key to 2
        map[key] = other_value_type {2};
        auto &value_replace = map.find(key)->value();
        bool ok_replace = value_replace.id() == 2;
        ok_replace &= value_replace.constructor_count() == 1;
        ok_replace &= value_replace.copy_constructor_count() == 0;
        ok_replace &= value_replace.move_constructor_count() == 0;
        ok_replace &= value_replace.copy_assign_count() == 0;
        ok_replace &= value_replace.move_assign_count() == 1;

        return std::tuple {
            ok_default,
            ok,
            ok_replace
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