#include <core/containers/hashmap.h>
#include "../misc/allocator_watcher.h"

GTEST_TEST(hashmap, clear_shrink_trivially_destructible_empty_map)
{
    const auto test = []()
    {
        using key_type = i32;
        using value_type = i64;
        hud::hashmap<key_type, value_type, hud::hashmap_default_hasher, hud::hashmap_default_key_equal<key_type>, hud_test::allocator_watcher<1>> map;
        map.clear_shrink();

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

GTEST_TEST(hashmap, clear_shrink_trivially_destructible_non_empty_map)
{

    const auto test = []()
    {
        using key_type = i32;
        using value_type = i64;
        hud::hashmap<key_type, value_type, hud::hashmap_default_hasher, hud::hashmap_default_key_equal<key_type>, hud_test::allocator_watcher<1>> map;
        map.reserve(256);
        for (u32 index = 0; index < 256; index++)
        {
            map.add({index, index * 10});
        }

        map.clear_shrink();
        i32 count {0};
        for (const auto &_ : map)
        {
            count++;
        }
        return std::tuple {
            map.count() == 0,                                                             // 0
            map.max_count() == 0,                                                         // 1
            map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 2
            map.allocator().free_count() == (hud::is_constant_evaluated() ? 2 : 1),       // 3
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

GTEST_TEST(hashmap, clear_shrink_non_trivially_destructible_empty_map)
{

    // No memory allocated
    {
        const auto test = []()
        {
            using key_type = hud_test::non_bitwise_type;
            using value_type = hud_test::non_bitwise_type;
            hud::hashmap<key_type, value_type, hud::hashmap_default_hasher, hud::hashmap_default_key_equal<key_type>, hud_test::allocator_watcher<1>> map;
            map.clear_shrink();

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

    // Memory reserved
    {
        const auto test = []()
        {
            hud::hashmap<hud_test::non_bitwise_type, hud_test::non_bitwise_type, hud::hashmap_default_hasher, hud::hashmap_default_key_equal<hud_test::non_bitwise_type>, hud_test::allocator_watcher<1>> map;
            constexpr usize COUNT = 256;
            map.reserve(COUNT);
            map.clear_shrink();

            i32 count {0};
            for (const auto &_ : map)
            {
                count++;
            }
            return std::tuple {
                map.count() == 0,                                                             // 0
                map.max_count() == 0,                                                         // 1
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 2
                map.allocator().free_count() == (hud::is_constant_evaluated() ? 2 : 1),       // 3
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
}

GTEST_TEST(hashmap, clear_shrink_non_trivially_destructible_non_empty_map)
{

    const auto test = []()
    {
        using key_type = hud_test::non_bitwise_type;
        using value_type = hud_test::non_bitwise_type;
        hud::hashmap<key_type, value_type, hud::hashmap_default_hasher, hud::hashmap_default_key_equal<key_type>, hud_test::allocator_watcher<1>> map;

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
        map.clear_shrink();

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
            map.max_count() == 0,                                                         // 1
            map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 2
            map.allocator().free_count() == (hud::is_constant_evaluated() ? 2 : 1),       // 3
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

GTEST_TEST(hashmap, clear_shrink_then_add_trivially_destructible_empty_map)
{

    // No reserve
    {
        const auto test = []()
        {
            using key_type = i32;
            using value_type = i64;
            hud::hashmap<key_type, value_type, hud::hashmap_default_hasher, hud::hashmap_default_key_equal<key_type>, hud_test::allocator_watcher<1>> map;
            constexpr usize COUNT = 256;
            map.clear_shrink();

            for (u32 index = 0; index < COUNT; index++)
            {
                map.add({index, index * 10});
            }

            // Check that we can iterate over all elements
            i32 count {0};
            for (const auto &_ : map)
            {
                count++;
            }

            return std::tuple {
                map.count() == COUNT,                   // 0
                map.max_count() > COUNT,                // 1
                map.allocator().allocation_count() > 0, // 2
                map.allocator().free_count() > 0,       // 3
                count == COUNT                          // 4
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

    // With reserve
    {
        const auto test = []()
        {
            using key_type = i32;
            using value_type = i64;
            hud::hashmap<key_type, value_type, hud::hashmap_default_hasher, hud::hashmap_default_key_equal<key_type>, hud_test::allocator_watcher<1>> map;
            constexpr usize COUNT = 256;
            map.reserve(COUNT);
            map.clear_shrink();

            for (u32 index = 0; index < COUNT; index++)
            {
                map.add({index, index * 10});
            }

            // Check that we can iterate over all elements
            i32 count {0};
            for (const auto &_ : map)
            {
                count++;
            }

            return std::tuple {
                map.count() == COUNT,                   // 0
                map.max_count() > COUNT,                // 1
                map.allocator().allocation_count() > 0, // 2
                map.allocator().free_count() > 0,       // 3
                count == COUNT                          // 4
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
}

GTEST_TEST(hashmap, clear_shrink_then_add_trivially_destructible_non_empty_map)
{
    // No reserve
    {
        const auto test = []()
        {
            using key_type = i32;
            using value_type = i64;
            hud::hashmap<key_type, value_type, hud::hashmap_default_hasher, hud::hashmap_default_key_equal<key_type>, hud_test::allocator_watcher<1>> map;
            // Add elements then clear the map
            constexpr usize COUNT = 128;
            for (u32 index = 0; index < COUNT; index++)
            {
                map.add({index, index * 10});
            }
            map.clear_shrink();

            // Add elements again
            for (u32 index = 0; index < COUNT * 2; index++)
            {
                map.add({index, index * 10});
            }

            // Check that we can iterate over all elements
            i32 element_count {0};
            for (const auto &_ : map)
            {
                element_count++;
            }

            return std::tuple {
                map.count() == COUNT * 2,               // 0
                map.max_count() > map.count(),          // 1
                map.allocator().allocation_count() > 0, // 2
                map.allocator().free_count() > 0,       // 3
                element_count == COUNT * 2              // 4
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

    // Reserve
    {
        const auto test = []()
        {
            using key_type = i32;
            using value_type = i64;
            hud::hashmap<key_type, value_type, hud::hashmap_default_hasher, hud::hashmap_default_key_equal<key_type>, hud_test::allocator_watcher<1>> map;
            // Add elements then clear the map
            constexpr usize COUNT = 128;
            map.reserve(COUNT * 2);
            for (u32 index = 0; index < COUNT; index++)
            {
                map.add({index, index * 10});
            }
            map.clear_shrink();

            // Add elements again
            for (u32 index = 0; index < COUNT * 2; index++)
            {
                map.add({index, index * 10});
            }

            // Check that we can iterate over all elements
            i32 element_count {0};
            for (const auto &_ : map)
            {
                element_count++;
            }

            return std::tuple {
                map.count() == COUNT * 2,               // 0
                map.max_count() > map.count(),          // 1
                map.allocator().allocation_count() > 0, // 2
                map.allocator().free_count() > 0,       // 3
                element_count == COUNT * 2              // 4
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
}

GTEST_TEST(hashmap, clear_shrink_then_add_non_trivially_destructible_empty_map)
{

    // No reserve
    {
        const auto test = []()
        {
            using key_type = hud_test::non_bitwise_type;
            using value_type = hud_test::non_bitwise_type;
            hud::hashmap<key_type, value_type, hud::hashmap_default_hasher, hud::hashmap_default_key_equal<key_type>, hud_test::allocator_watcher<1>> map;
            constexpr usize COUNT = 128;
            map.clear_shrink();

            // Add all elements
            for (i32 index = 0; index < COUNT * 2; index++)
            {
                map.add(
                    key_type {index},
                    value_type {index * 10}
                );
            }

            // Check that we can iterate over all elements
            i32 count {0};
            for (const auto &_ : map)
            {
                count++;
            }

            return std::tuple {
                map.count() == COUNT * 2,               // 0
                map.max_count() > COUNT,                // 1
                map.allocator().allocation_count() > 0, // 2
                map.allocator().free_count() > 0,       // 3
                count == COUNT * 2                      // 4
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

    // With reserve
    {
        const auto test = []()
        {
            using key_type = hud_test::non_bitwise_type;
            using value_type = hud_test::non_bitwise_type;
            hud::hashmap<key_type, value_type, hud::hashmap_default_hasher, hud::hashmap_default_key_equal<key_type>, hud_test::allocator_watcher<1>> map;
            constexpr usize COUNT = 128;
            map.reserve(COUNT * 2);
            map.clear_shrink();

            // Add all elements
            for (i32 index = 0; index < COUNT * 2; index++)
            {
                map.add(
                    key_type {index},
                    value_type {index * 10}
                );
            }

            // Check that we can iterate over all elements
            i32 count {0};
            for (const auto &_ : map)
            {
                count++;
            }

            return std::tuple {
                map.count() == COUNT * 2,               // 0
                map.max_count() > COUNT,                // 1
                map.allocator().allocation_count() > 0, // 2
                map.allocator().free_count() > 0,       // 3
                count == COUNT * 2                      // 4
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
}

GTEST_TEST(hashmap, clear_shrink_then_add_non_trivially_destructible_non_empty_map)
{

    // No reserve
    {
        const auto test = []()
        {
            using key_type = hud_test::non_bitwise_type;
            using value_type = hud_test::non_bitwise_type;
            hud::hashmap<key_type, value_type, hud::hashmap_default_hasher, hud::hashmap_default_key_equal<key_type>, hud_test::allocator_watcher<1>> map;
            constexpr usize COUNT = 128;

            // Add all elements
            for (i32 index = 0; index < COUNT; index++)
            {
                map.add(
                    key_type {index},
                    value_type {index * 10}
                );
            }

            map.clear_shrink();

            // Add all elements
            for (i32 index = 0; index < COUNT * 2; index++)
            {
                map.add(
                    key_type {index},
                    value_type {index * 10}
                );
            }

            // Check that we can iterate over all elements
            i32 count {0};
            for (const auto &_ : map)
            {
                count++;
            }

            return std::tuple {
                map.count() == COUNT * 2,               // 0
                map.max_count() > COUNT,                // 1
                map.allocator().allocation_count() > 0, // 2
                map.allocator().free_count() > 0,       // 3
                count == COUNT * 2                      // 4
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

    // With reserve
    {
        const auto test = []()
        {
            using key_type = hud_test::non_bitwise_type;
            using value_type = hud_test::non_bitwise_type;
            hud::hashmap<key_type, value_type, hud::hashmap_default_hasher, hud::hashmap_default_key_equal<key_type>, hud_test::allocator_watcher<1>> map;
            constexpr usize COUNT = 128;
            map.reserve(COUNT * 2);

            // Add all elements
            for (i32 index = 0; index < COUNT; index++)
            {
                map.add(
                    key_type {index},
                    value_type {index * 10}
                );
            }

            map.clear_shrink();

            // Add all elements
            for (i32 index = 0; index < COUNT * 2; index++)
            {
                map.add(
                    key_type {index},
                    value_type {index * 10}
                );
            }

            // Check that we can iterate over all elements
            i32 count {0};
            for (const auto &_ : map)
            {
                count++;
            }

            return std::tuple {
                map.count() == COUNT * 2,               // 0
                map.max_count() > COUNT,                // 1
                map.allocator().allocation_count() > 0, // 2
                map.allocator().free_count() > 0,       // 3
                count == COUNT * 2                      // 4
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
}