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