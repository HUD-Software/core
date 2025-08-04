#include <core/containers/hashmap.h>

GTEST_TEST(hashmap, indexed_operator_trivial_same_type)
{
    const auto test = []()
    {
        hud::hashmap<usize, usize> map;
        map[1] = 2;
        hud::hashmap<const char *, const char *> map2;
        const char key_0[] = "key";
        const char value_0[] = "value";
        const char value_1[] = "value_1";
        map2[key_0] = value_0;
        const bool ok = map2.find(key_0)->value() = value_0;
        map2[key_0] = value_1;
        const bool ok2 = map2.find(key_0)->value() = value_1;
        return std::tuple {
            map.find(1)->value() = 2,
            ok,
            ok2
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
        // constexpr auto result = test();
        // hud_assert_true(std::get<0>(result));
        // hud_assert_true(std::get<1>(result));
        // hud_assert_true(std::get<2>(result));
    }
}
