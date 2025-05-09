#include <core/containers/hashmap.h>
#include "../misc/allocator_watcher.h"

GTEST_TEST(hashmap, clear_trivially_destructible_empty_map)
{

    const auto test = []()
    {
        hud::hashmap<i32, i64, hud::hashmap_default_hasher<i32>, hud::hashmap_default_key_equal<i32>, hud_test::allocator_watcher<1>> map;
        map.clear();

        i32 count {0};
        for (const auto &_ : map)
        {
            count++;
        }
        return std::tuple {
            map.count() == 0,                        // 0
            map.max_count() == 0,                    // 1
            map.allocator().allocation_count() == 0, // 2
            map.allocator().free_count() == 0,       // 3
            count == 0                               // 4
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
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
    }
}

GTEST_TEST(hashmap, clear_trivially_destructible_non_empty_map)
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
        i32 count {0};
        for (const auto &_ : map)
        {
            count++;
        }
        return std::tuple {
            map.count() == 0,                                                             // 0
            map.max_count() > map.count(),                                                // 1
            map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 2
            map.allocator().free_count() == 0,                                            // 3
            count == 0                                                                    // 4
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
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
    }
}

GTEST_TEST(hashmap, clear_non_trivially_destructible_empty_map)
{

    const auto test = []()
    {
        hud::hashmap<hud_test::non_bitwise_type, hud_test::non_bitwise_type, hud::hashmap_default_hasher<hud_test::non_bitwise_type>, hud::hashmap_default_key_equal<hud_test::non_bitwise_type>, hud_test::allocator_watcher<1>> map;
        map.clear();

        i32 count {0};
        for (const auto &_ : map)
        {
            count++;
        }
        return std::tuple {
            map.count() == 0,                        // 0
            map.max_count() == 0,                    // 1
            map.allocator().allocation_count() == 0, // 2
            map.allocator().free_count() == 0,       // 3
            count == 0                               // 4
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
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
    }
}

GTEST_TEST(hashmap, clear_non_trivially_destructible_non_empty_map)
{

    const auto test = []()
    {
        using key_type = hud_test::non_bitwise_type;
        using value_type = hud_test::non_bitwise_type;
        hud::hashmap<key_type, value_type, hud::hashmap_default_hasher<key_type>, hud::hashmap_default_key_equal<key_type>, hud_test::allocator_watcher<1>> map;

        constexpr usize COUNT = 256;
        map.reserve(COUNT);
        i32 key_destructor_called_count[COUNT];
        i32 value_destructor_called_count[COUNT];

        // Add all elements
        for (i32 index = 0; index < COUNT; index++)
        {
            map.add(
                key_type {index, key_destructor_called_count + index},
                value_type {index * 10, value_destructor_called_count + index}
            );
        }
        hud::memory::set_memory_safe(key_destructor_called_count, 0);
        hud::memory::set_memory_safe(value_destructor_called_count, 0);

        // Clear the hashmap
        map.clear();

        // Ensure we have no element left in the map
        i32 count {0};
        for (const auto &_ : map)
        {
            count++;
        }

        // Ensure all destructor are called once
        bool all_destructor_called_after_clear = true;
        for (i32 index = 0; index < COUNT; index++)
        {
            if (key_destructor_called_count[index] != 1)
            {
                all_destructor_called_after_clear = false;
                break;
            }
            if (value_destructor_called_count[index] != 1)
            {
                all_destructor_called_after_clear = false;
                break;
            }
        }

        return std::tuple {
            map.count() == 0,                                                             // 0
            map.max_count() > map.count(),                                                // 1
            map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 2
            map.allocator().free_count() == 0,                                            // 3
            count == 0,                                                                   // 4
            all_destructor_called_after_clear                                             // 5
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
    }
}