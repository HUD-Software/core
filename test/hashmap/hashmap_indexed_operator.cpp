#include <core/containers/hashmap.h>

GTEST_TEST(hashmap, indexed_operator_trivial_same_type)
{
    const auto test = []()
    {
        hud::hashmap<const char *, const char *> map;
        const char key_0[] = "key";
        const char value_0[] = "value";
        const char value_1[] = "value_1";
        // Should add default value ""
        map[key_0];
        const bool ok_default = hud::cstring::equals(map.find(key_0)->value(), "");
        // Should add "key_0", "value_0"
        map[key_0] = value_0;
        const bool ok = map.find(key_0)->value() = value_0;
        // Should replace value of "key_0" to "value_1"
        map[key_0] = value_1;
        const bool ok_replace = map.find(key_0)->value() = value_1;

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
