#include <core/containers/hashmap.h>
#include "../misc/allocator_watcher.h"

GTEST_TEST(hashmap, swap_empty_hashmap_do_nothing)
{
    using key_type = hud_test::non_bitwise_type;
    using value_type = hud_test::non_bitwise_type;
    using AllocatorType = hud_test::allocator_watcher<1>;
    using hashmap_type = hud::hashmap<key_type, value_type, hud::hashmap_default_hasher, hud::hashmap_default_key_equal<key_type>, AllocatorType>;

    // hud::hashmap::swap
    {
        const auto test = []()
        {
            hashmap_type map;
            hashmap_type other_map;
            map.swap(other_map);
            return std::tuple {
                map.count() == 0,                        // 0
                other_map.count() == 0,                  // 1
                map.max_count() == 0,                    // 2
                other_map.max_count() == 0,              // 3
                map.slack() == 0,                        // 4
                other_map.slack() == 0,                  // 5
                map.allocator().allocation_count() == 0, // 6
                map.allocator().free_count() == 0,       // 7
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
            hud_assert_true(std::get<6>(result));
            hud_assert_true(std::get<7>(result));
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
            hud_assert_true(std::get<6>(result));
            hud_assert_true(std::get<7>(result));
        }
    }

    // hud::swap
    {
        const auto test = []()
        {
            hashmap_type map;
            hashmap_type other_map;
            hud::swap(map, other_map);
            return std::tuple {
                map.count() == 0,                        // 0
                other_map.count() == 0,                  // 1
                map.max_count() == 0,                    // 2
                other_map.max_count() == 0,              // 3
                map.slack() == 0,                        // 4
                other_map.slack() == 0,                  // 5
                map.allocator().allocation_count() == 0, // 6
                map.allocator().free_count() == 0,       // 7
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
            hud_assert_true(std::get<6>(result));
            hud_assert_true(std::get<7>(result));
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
            hud_assert_true(std::get<6>(result));
            hud_assert_true(std::get<7>(result));
        }
    }
}

GTEST_TEST(hashmap, swap_non_trivial_type)
{
    using key_type = hud_test::non_bitwise_type;
    using value_type = hud_test::non_bitwise_type;
    using AllocatorType = hud_test::allocator_watcher<1>;
    using hashmap_type = hud::hashmap<key_type, value_type, hud::hashmap_default_hasher, hud::hashmap_default_key_equal<key_type>, AllocatorType>;

    // hud::hashmap::swap
    {
        const auto test = []()
        {
            hashmap_type map;
            map.reserve(255);
            for (i32 i = 0; i < 255; i++)
            {
                map.add({i, i * i});
            }
            hashmap_type other_map;
            map.swap(other_map);

            return std::tuple {
                map.count() == 0,                                                             // 0
                other_map.count() == 255,                                                     // 1
                map.max_count() == 0,                                                         // 2
                other_map.max_count() >= 255,                                                 // 3
                map.slack() == 0,                                                             // 4
                other_map.slack() < (other_map.max_count() - other_map.count()),              // 5
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 6
                map.allocator().free_count() == 0,                                            // 7
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
            hud_assert_true(std::get<6>(result));
            hud_assert_true(std::get<7>(result));
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
            hud_assert_true(std::get<6>(result));
            hud_assert_true(std::get<7>(result));
        }
    }

    // hud::swap
    {
        const auto test = []()
        {
            hashmap_type map;
            map.reserve(255);
            for (i32 i = 0; i < 255; i++)
            {
                map.add({i, i * i});
            }
            hashmap_type other_map;
            hud::swap(map, other_map);

            return std::tuple {
                map.count() == 0,                                                             // 0
                other_map.count() == 255,                                                     // 1
                map.max_count() == 0,                                                         // 2
                other_map.max_count() >= 255,                                                 // 3
                map.slack() == 0,                                                             // 4
                other_map.slack() < (other_map.max_count() - other_map.count()),              // 5
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 6
                map.allocator().free_count() == 0,                                            // 7
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
            hud_assert_true(std::get<6>(result));
            hud_assert_true(std::get<7>(result));
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
            hud_assert_true(std::get<6>(result));
            hud_assert_true(std::get<7>(result));
        }
    }
}

GTEST_TEST(hashmap, swap_trivial_type)
{
    using key_type = i32;
    using value_type = i64;
    using AllocatorType = hud_test::allocator_watcher<1>;
    using hashmap_type = hud::hashmap<key_type, value_type, hud::hashmap_default_hasher, hud::hashmap_default_key_equal<key_type>, AllocatorType>;

    // hud::hashmap::swap
    {
        const auto test = []()
        {
            hashmap_type map;
            map.reserve(255);
            for (i32 i = 0; i < 255; i++)
            {
                map.add({i, i * i});
            }
            hashmap_type other_map;
            map.swap(other_map);

            return std::tuple {
                map.count() == 0,                                                             // 0
                other_map.count() == 255,                                                     // 1
                map.max_count() == 0,                                                         // 2
                other_map.max_count() >= 255,                                                 // 3
                map.slack() == 0,                                                             // 4
                other_map.slack() < (other_map.max_count() - other_map.count()),              // 5
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 6
                map.allocator().free_count() == 0,                                            // 7
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
            hud_assert_true(std::get<6>(result));
            hud_assert_true(std::get<7>(result));
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
            hud_assert_true(std::get<6>(result));
            hud_assert_true(std::get<7>(result));
        }
    }

    // hud::swap
    {
        const auto test = []()
        {
            hashmap_type map;
            map.reserve(255);
            for (i32 i = 0; i < 255; i++)
            {
                map.add({i, i * i});
            }
            hashmap_type other_map;
            hud::swap(map, other_map);

            return std::tuple {
                map.count() == 0,                                                             // 0
                other_map.count() == 255,                                                     // 1
                map.max_count() == 0,                                                         // 2
                other_map.max_count() >= 255,                                                 // 3
                map.slack() == 0,                                                             // 4
                other_map.slack() < (other_map.max_count() - other_map.count()),              // 5
                map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 6
                map.allocator().free_count() == 0,                                            // 7
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
            hud_assert_true(std::get<6>(result));
            hud_assert_true(std::get<7>(result));
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
            hud_assert_true(std::get<6>(result));
            hud_assert_true(std::get<7>(result));
        }
    }
}