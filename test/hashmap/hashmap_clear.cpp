#include <core/containers/hashmap.h>
#include "../misc/allocator_watcher.h"

GTEST_TEST(hashmap, clear_empty_map)
{

    const auto test = []()
    {
        hud::hashmap<i32, i64, hud::hashmap_default_hasher<i32>, hud::hashmap_default_key_equal<i32>, hud_test::allocator_watcher<1>> map;
        map.clear();
        return std::tuple {
            map.count() == 0,                        // 0
            map.max_count() == 0,                    // 1
            map.allocator().allocation_count() == 0, // 2
            map.allocator().free_count() == 0        // 3
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

GTEST_TEST(hashmap, clear_non_empty_map)
{

    const auto test = []()
    {
        hud::hashmap<i32, i64, hud::hashmap_default_hasher<i32>, hud::hashmap_default_key_equal<i32>, hud_test::allocator_watcher<1>> map;
        map.reserve(256);
        for (u32 index = 0; index < 256; index++)
        {
            map.add({index, index * 10});
        }

        map.clear();
        return std::tuple {
            map.count() == 0,                                                             // 0
            map.max_count() > map.count(),                                                // 1
            map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 2
            map.allocator().free_count() == 0                                             // 3
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