#include <core/containers/hashset.h>
#include "../misc/allocator_watcher.h"

GTEST_TEST(hashset, swap_empty_hashset_do_nothing)
{
    using key_type = hud_test::non_bitwise_type;
    using AllocatorType = hud_test::allocator_watcher<1>;
    using hashset_type = hud::hashset<key_type, hud::hash_64<key_type>, hud::equal<key_type>, AllocatorType>;

    // hud::hashset::swap
    {
        const auto test = []()
        {
            hashset_type set;
            hashset_type other_set;
            set.swap(other_set);
            return std::tuple {
                set.count() == 0,                        // 0
                other_set.count() == 0,                  // 1
                set.max_count() == 0,                    // 2
                other_set.max_count() == 0,              // 3
                set.slack() == 0,                        // 4
                other_set.slack() == 0,                  // 5
                set.allocator().allocation_count() == 0, // 6
                set.allocator().free_count() == 0,       // 7
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
            hashset_type set;
            hashset_type other_set;
            hud::swap(set, other_set);
            return std::tuple {
                set.count() == 0,                        // 0
                other_set.count() == 0,                  // 1
                set.max_count() == 0,                    // 2
                other_set.max_count() == 0,              // 3
                set.slack() == 0,                        // 4
                other_set.slack() == 0,                  // 5
                set.allocator().allocation_count() == 0, // 6
                set.allocator().free_count() == 0,       // 7
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

GTEST_TEST(hashset, swap_non_trivial_type)
{
    using key_type = hud_test::non_bitwise_type;
    using AllocatorType = hud_test::allocator_watcher<1>;
    using hashset_type = hud::hashset<key_type, hud::hash_64<key_type>, hud::equal<key_type>, AllocatorType>;

    // hud::hashset::swap
    {
        const auto test = []()
        {
            hashset_type set;
            set.reserve(255);
            for (i32 i = 0; i < 255; i++)
            {
                set.add(i);
            }
            hashset_type other_set;
            set.swap(other_set);

            return std::tuple {
                set.count() == 0,                                                             // 0
                other_set.count() == 255,                                                     // 1
                set.max_count() == 0,                                                         // 2
                other_set.max_count() >= 255,                                                 // 3
                set.slack() == 0,                                                             // 4
                other_set.slack() < (other_set.max_count() - other_set.count()),              // 5
                set.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 6
                set.allocator().free_count() == 0,                                            // 7
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
            hashset_type set;
            set.reserve(255);
            for (i32 i = 0; i < 255; i++)
            {
                set.add(i);
            }
            hashset_type other_set;
            hud::swap(set, other_set);

            return std::tuple {
                set.count() == 0,                                                             // 0
                other_set.count() == 255,                                                     // 1
                set.max_count() == 0,                                                         // 2
                other_set.max_count() >= 255,                                                 // 3
                set.slack() == 0,                                                             // 4
                other_set.slack() < (other_set.max_count() - other_set.count()),              // 5
                set.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 6
                set.allocator().free_count() == 0,                                            // 7
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

GTEST_TEST(hashset, swap_trivial_type)
{
    using key_type = i32;
    using AllocatorType = hud_test::allocator_watcher<1>;
    using hashset_type = hud::hashset<key_type, hud::hash_64<key_type>, hud::equal<key_type>, AllocatorType>;

    // hud::hashset::swap
    {
        const auto test = []()
        {
            hashset_type set;
            set.reserve(255);
            for (i32 i = 0; i < 255; i++)
            {
                set.add(i);
            }
            hashset_type other_set;
            set.swap(other_set);

            return std::tuple {
                set.count() == 0,                                                             // 0
                other_set.count() == 255,                                                     // 1
                set.max_count() == 0,                                                         // 2
                other_set.max_count() >= 255,                                                 // 3
                set.slack() == 0,                                                             // 4
                other_set.slack() < (other_set.max_count() - other_set.count()),              // 5
                set.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 6
                set.allocator().free_count() == 0,                                            // 7
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
            hashset_type set;
            set.reserve(255);
            for (i32 i = 0; i < 255; i++)
            {
                set.add(i);
            }
            hashset_type other_set;
            hud::swap(set, other_set);

            return std::tuple {
                set.count() == 0,                                                             // 0
                other_set.count() == 255,                                                     // 1
                set.max_count() == 0,                                                         // 2
                other_set.max_count() >= 255,                                                 // 3
                set.slack() == 0,                                                             // 4
                other_set.slack() < (other_set.max_count() - other_set.count()),              // 5
                set.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1), // 6
                set.allocator().free_count() == 0,                                            // 7
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