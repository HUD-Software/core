#include <core/containers/hashset.h>
#include "../misc/allocator_watcher.h"

GTEST_TEST(hashset, default_constructor_should_allocate_no_memory)
{
    hud::hashset<i32, hud::hash_64<i32>, hud::equal<i32>, hud_test::allocator_watcher<1>> set;
    hud_assert_eq(set.count(), 0u);
    hud_assert_eq(set.max_count(), 0u);
    hud_assert_eq(set.allocator().allocation_count(), 0u);
    hud_assert_eq(set.allocator().free_count(), 0u);
    hud_assert_eq(set.allocator().move_assign_count(), 0u);
    hud_assert_eq(set.allocator().move_construct_count(), 0u);
}

GTEST_TEST(hashset, construct_with_initializer_list_of_bitwise_copy_constructible_same_type)
{
    using key_type = i32;

    static_assert(hud::is_bitwise_copy_constructible_v<key_type>);

    // Test with default allocator and no extra
    {
        auto test_default_allocator = [](std::initializer_list<key_type> initializer)
        {
            hud::hashset<key_type, hud::hash_64<key_type>, hud::equal<key_type>> set {initializer};

            // Validate we have {1,11}
            const auto &it_1 = set.find(1);
            bool key_1_inserted = it_1 != set.end();
            bool key_1_is_correct = key_1_inserted ? it_1->key() == 1 : false;

            // Validate we have {2,22}
            const auto &it_2 = set.find(2);
            bool key_2_inserted = it_2 != set.end();
            bool key_2_is_correct = key_2_inserted ? it_2->key() == 2 : false;

            // Validate we have {3,33}
            const auto &it_3 = set.find(3);
            bool key_3_inserted = it_3 != set.end();
            bool key_3_is_correct = key_3_inserted ? it_3->key() == 3 : false;

            return std::tuple {
                set.count() == 3,               // 0
                set.max_count() >= set.count(), // 1
                key_1_inserted,                 // 2
                key_1_is_correct,               // 3
                key_2_inserted,                 // 4
                key_2_is_correct,               // 5
                key_3_inserted,                 // 6
                key_3_is_correct,               // 7
            };
        };

        // Non Constant
        {
            const auto result = test_default_allocator({1, 1, 2, 3});
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
            constexpr auto result = test_default_allocator({1, 1, 2, 3});
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

    // Test with default allocator with extra
    {
        auto test_default_allocator = [](std::initializer_list<key_type> initializer)
        {
            hud::hashset<key_type, hud::hash_64<key_type>, hud::equal<key_type>> set {initializer, 200};

            // Validate we have {1,11}
            const auto &it_1 = set.find(1);
            bool key_1_inserted = it_1 != set.end();
            bool key_1_is_correct = key_1_inserted ? it_1->key() == 1 : false;

            // Validate we have {2,22}
            const auto &it_2 = set.find(2);
            bool key_2_inserted = it_2 != set.end();
            bool key_2_is_correct = key_2_inserted ? it_2->key() == 2 : false;

            // Validate we have {3,33}
            const auto &it_3 = set.find(3);
            bool key_3_inserted = it_3 != set.end();
            bool key_3_is_correct = key_3_inserted ? it_3->key() == 3 : false;

            return std::tuple {
                set.count() == 3,                     // 0
                set.max_count() >= set.count() + 200, // 1
                key_1_inserted,                       // 2
                key_1_is_correct,                     // 3
                key_2_inserted,                       // 4
                key_2_is_correct,                     // 5
                key_3_inserted,                       // 6
                key_3_is_correct,                     // 7
            };
        };

        // Non Constant
        {
            const auto result = test_default_allocator({1, 1, 2, 3});
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
            constexpr auto result = test_default_allocator({1, 1, 2, 3});
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

GTEST_TEST(hashset, construct_with_initializer_list_of_bitwise_copy_constructible_different_type)
{

    using key_type = i32;
    using other_key_type = u32;
    static_assert(hud::is_bitwise_copy_constructible_v<key_type, other_key_type>);

    // Test with default allocator and no extra
    {
        auto test_default_allocator = [](std::initializer_list<other_key_type> initializer)
        {
            hud::hashset<key_type, hud::hash_64<key_type>, hud::equal<key_type>> set {initializer};

            // Validate we have {1,11}
            const auto &it_1 = set.find(1);
            bool key_1_inserted = it_1 != set.end();
            bool key_1_is_correct = key_1_inserted ? it_1->key() == 1 : false;

            // Validate we have {2,22}
            const auto &it_2 = set.find(2);
            bool key_2_inserted = it_2 != set.end();
            bool key_2_is_correct = key_2_inserted ? it_2->key() == 2 : false;

            // Validate we have {3,33}
            const auto &it_3 = set.find(3);
            bool key_3_inserted = it_3 != set.end();
            bool key_3_is_correct = key_3_inserted ? it_3->key() == 3 : false;

            return std::tuple {
                set.count() == 3,               // 0
                set.max_count() >= set.count(), // 1
                key_1_inserted,                 // 2
                key_1_is_correct,               // 3
                key_2_inserted,                 // 4
                key_2_is_correct,               // 5
                key_3_inserted,                 // 6
                key_3_is_correct,               // 7
            };
        };

        // Non Constant
        {
            const auto result = test_default_allocator({1, 1, 2, 3});
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
            constexpr auto result = test_default_allocator({1, 1, 2, 3});
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

    // Test with default allocator and with extra
    {
        auto test_default_allocator = [](std::initializer_list<other_key_type> initializer)
        {
            hud::hashset<key_type, hud::hash_64<key_type>, hud::equal<key_type>> set {initializer, 200};

            // Validate we have {1,11}
            const auto &it_1 = set.find(1);
            bool key_1_inserted = it_1 != set.end();
            bool key_1_is_correct = key_1_inserted ? it_1->key() == 1 : false;

            // Validate we have {2,22}
            const auto &it_2 = set.find(2);
            bool key_2_inserted = it_2 != set.end();
            bool key_2_is_correct = key_2_inserted ? it_2->key() == 2 : false;

            // Validate we have {3,33}
            const auto &it_3 = set.find(3);
            bool key_3_inserted = it_3 != set.end();
            bool key_3_is_correct = key_3_inserted ? it_3->key() == 3 : false;

            return std::tuple {
                set.count() == 3,                     // 0
                set.max_count() >= set.count() + 200, // 1
                key_1_inserted,                       // 2
                key_1_is_correct,                     // 3
                key_2_inserted,                       // 4
                key_2_is_correct,                     // 5
                key_3_inserted,                       // 6
                key_3_is_correct,                     // 7
            };
        };

        // Non Constant
        {
            const auto result = test_default_allocator({1, 1, 2, 3});
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
            constexpr auto result = test_default_allocator({1, 1, 2, 3});
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

GTEST_TEST(hashset, construct_with_initializer_list_of_non_bitwise_copy_constructible_same_type)
{

    using key_type = hud_test::non_bitwise_copy_constructible_type;

    static_assert(!hud::is_bitwise_copy_constructible_v<key_type>);

    // Test with default allocator and no extra
    {
        auto test_default_allocator = [](std::initializer_list<key_type> initializer)
        {
            hud::hashset<key_type, hud::hash_64<key_type>, hud::equal<key_type>> set {initializer};

            // Validate we have {1,11}
            const auto &it_1 = set.find(1);
            bool key_1_inserted = it_1 != set.end();
            bool key_1_is_correct = key_1_inserted;
            if (key_1_inserted)
            {
                // Validate key is correctly added
                key_1_is_correct &= it_1->key() == 1;
                key_1_is_correct &= it_1->key().copy_constructor_count() == 1;
            }

            // Validate we have {2,22}
            const auto &it_2 = set.find(2);
            bool key_2_inserted = it_2 != set.end();
            bool key_2_is_correct = key_2_inserted;
            if (key_2_inserted)
            {
                // Validate key is correctly added
                key_2_is_correct &= it_2->key() == 2;
                key_2_is_correct &= it_2->key().copy_constructor_count() == 1;
            }

            // Validate we have {3,33}
            const auto &it_3 = set.find(3);
            bool key_3_inserted = it_3 != set.end();
            bool key_3_is_correct = key_3_inserted;
            if (key_3_inserted)
            {
                // Validate key is correctly added
                key_3_is_correct &= it_3->key() == 3;
                key_3_is_correct &= it_3->key().copy_constructor_count() == 1;
            }

            return std::tuple {
                set.count() == 3,               // 0
                set.max_count() >= set.count(), // 1
                key_1_inserted,                 // 2
                key_1_is_correct,               // 3
                key_2_inserted,                 // 4
                key_2_is_correct,               // 5
                key_3_inserted,                 // 6
                key_3_is_correct,               // 7
            };
        };

        // Non Constant
        {
            const auto result = test_default_allocator({1, 1, 2, 3});
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
            constexpr auto result = test_default_allocator({1, 1, 2, 3});
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

    // Test with default allocator with extra
    {
        auto test_default_allocator = [](std::initializer_list<key_type> initializer)
        {
            hud::hashset<key_type, hud::hash_64<key_type>, hud::equal<key_type>> set {initializer, 200};

            // Validate we have {1,11}
            const auto &it_1 = set.find(1);
            bool key_1_inserted = it_1 != set.end();
            bool key_1_is_correct = key_1_inserted;
            if (key_1_inserted)
            {
                // Validate key is correctly added
                key_1_is_correct &= it_1->key() == 1;
                key_1_is_correct &= it_1->key().copy_constructor_count() == 1;
            }

            // Validate we have {2,22}
            const auto &it_2 = set.find(2);
            bool key_2_inserted = it_2 != set.end();
            bool key_2_is_correct = key_2_inserted;
            if (key_2_inserted)
            {
                // Validate key is correctly added
                key_2_is_correct &= it_2->key() == 2;
                key_2_is_correct &= it_2->key().copy_constructor_count() == 1;
            }

            // Validate we have {3,33}
            const auto &it_3 = set.find(3);
            bool key_3_inserted = it_3 != set.end();
            bool key_3_is_correct = key_3_inserted;
            if (key_3_inserted)
            {
                // Validate key is correctly added
                key_3_is_correct &= it_3->key() == 3;
                key_3_is_correct &= it_3->key().copy_constructor_count() == 1;
            }

            return std::tuple {
                set.count() == 3,                     // 0
                set.max_count() >= set.count() + 200, // 1
                key_1_inserted,                       // 2
                key_1_is_correct,                     // 3
                key_2_inserted,                       // 4
                key_2_is_correct,                     // 5
                key_3_inserted,                       // 6
                key_3_is_correct,                     // 7
            };
        };

        // Non Constant
        {
            const auto result = test_default_allocator({1, 1, 2, 3});
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
            constexpr auto result = test_default_allocator({1, 1, 2, 3});
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

GTEST_TEST(hashset, construct_with_initializer_list_of_non_bitwise_copy_constructible_different_type)
{
    using key_type = hud_test::non_bitwise_copy_constructible_type2;
    using other_key_type = hud_test::non_bitwise_copy_constructible_type;

    static_assert(!hud::is_bitwise_copy_constructible_v<key_type, other_key_type>);

    // Test with default allocator and no extra
    {
        auto test_default_allocator = [](std::initializer_list<other_key_type> initializer)
        {
            hud::hashset<key_type, hud::hash_64<key_type>, hud::equal<key_type>> set {initializer};

            // Validate we have {1,11}
            const auto &it_1 = set.find(1);
            bool key_1_inserted = it_1 != set.end();
            bool key_1_is_correct = key_1_inserted;
            if (key_1_inserted)
            {
                // Validate key is correctly added
                key_1_is_correct &= it_1->key() == 1;
                key_1_is_correct &= it_1->key().copy_constructor_count() == 1;
            }

            // Validate we have {2,22}
            const auto &it_2 = set.find(2);
            bool key_2_inserted = it_2 != set.end();
            bool key_2_is_correct = key_2_inserted;
            if (key_2_inserted)
            {
                // Validate key is correctly added
                key_2_is_correct &= it_2->key() == 2;
                key_2_is_correct &= it_2->key().copy_constructor_count() == 1;
            }

            // Validate we have {3,33}
            const auto &it_3 = set.find(3);
            bool key_3_inserted = it_3 != set.end();
            bool key_3_is_correct = key_3_inserted;
            if (key_3_inserted)
            {
                // Validate key is correctly added
                key_3_is_correct &= it_3->key() == 3;
                key_3_is_correct &= it_3->key().copy_constructor_count() == 1;
            }

            return std::tuple {
                set.count() == 3,               // 0
                set.max_count() >= set.count(), // 1
                key_1_inserted,                 // 2
                key_1_is_correct,               // 3
                key_2_inserted,                 // 4
                key_2_is_correct,               // 5
                key_3_inserted,                 // 6
                key_3_is_correct,               // 7
            };
        };

        // Non Constant
        {
            const auto result = test_default_allocator({1, 1, 2, 3});
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
            constexpr auto result = test_default_allocator({1, 1, 2, 3});
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

    // Test with default allocator with extra
    {
        auto test_default_allocator = [](std::initializer_list<other_key_type> initializer)
        {
            hud::hashset<key_type, hud::hash_64<key_type>, hud::equal<key_type>> set {initializer, 200};

            // Validate we have {1,11}
            const auto &it_1 = set.find(1);
            bool key_1_inserted = it_1 != set.end();
            bool key_1_is_correct = key_1_inserted;
            if (key_1_inserted)
            {
                // Validate key is correctly added
                key_1_is_correct &= it_1->key() == 1;
                key_1_is_correct &= it_1->key().copy_constructor_count() == 1;
            }

            // Validate we have {2,22}
            const auto &it_2 = set.find(2);
            bool key_2_inserted = it_2 != set.end();
            bool key_2_is_correct = key_2_inserted;
            if (key_2_inserted)
            {
                // Validate key is correctly added
                key_2_is_correct &= it_2->key() == 2;
                key_2_is_correct &= it_2->key().copy_constructor_count() == 1;
            }

            // Validate we have {3,33}
            const auto &it_3 = set.find(3);
            bool key_3_inserted = it_3 != set.end();
            bool key_3_is_correct = key_3_inserted;
            if (key_3_inserted)
            {
                // Validate key is correctly added
                key_3_is_correct &= it_3->key() == 3;
                key_3_is_correct &= it_3->key().copy_constructor_count() == 1;
            }

            return std::tuple {
                set.count() == 3,                     // 0
                set.max_count() >= set.count() + 200, // 1
                key_1_inserted,                       // 2
                key_1_is_correct,                     // 3
                key_2_inserted,                       // 4
                key_2_is_correct,                     // 5
                key_3_inserted,                       // 6
                key_3_is_correct,                     // 7
            };
        };

        // Non Constant
        {
            const auto result = test_default_allocator({1, 1, 2, 3});
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
            constexpr auto result = test_default_allocator({1, 1, 2, 3});
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

GTEST_TEST(hashset, construct_with_initializer_list_of_non_bitwise_move_constructible_same_type)
{

    using key_type = hud_test::non_bitwise_move_constructible_type;

    static_assert(!hud::is_bitwise_copy_constructible_v<key_type>);

    // Test with default allocator and no extra
    {
        auto test_default_allocator = [](std::initializer_list<key_type> initializer)
        {
            hud::hashset<key_type, hud::hash_64<key_type>, hud::equal<key_type>> set {initializer};

            // Validate we have {1,11}
            const auto &it_1 = set.find(1);
            bool key_1_inserted = it_1 != set.end();
            bool key_1_is_correct = key_1_inserted;
            if (key_1_inserted)
            {
                // Validate key is correctly added
                key_1_is_correct &= it_1->key() == 1;
                key_1_is_correct &= it_1->key().copy_constructor_count() == 1;
                key_1_is_correct &= it_1->key().move_constructor_count() == 0;
            }

            // Validate we have {2,22}
            const auto &it_2 = set.find(2);
            bool key_2_inserted = it_2 != set.end();
            bool key_2_is_correct = key_2_inserted;
            if (key_2_inserted)
            {
                // Validate key is correctly added
                key_2_is_correct &= it_2->key() == 2;
                key_2_is_correct &= it_2->key().copy_constructor_count() == 1;
                key_2_is_correct &= it_2->key().move_constructor_count() == 0;
            }

            // Validate we have {3,33}
            const auto &it_3 = set.find(3);
            bool key_3_inserted = it_3 != set.end();
            bool key_3_is_correct = key_3_inserted;
            if (key_3_inserted)
            {
                // Validate key is correctly added
                key_3_is_correct &= it_3->key() == 3;
                key_3_is_correct &= it_3->key().copy_constructor_count() == 1;
                key_3_is_correct &= it_3->key().move_constructor_count() == 0;
            }

            return std::tuple {
                set.count() == 3,               // 0
                set.max_count() >= set.count(), // 1
                key_1_inserted,                 // 2
                key_1_is_correct,               // 3
                key_2_inserted,                 // 4
                key_2_is_correct,               // 5
                key_3_inserted,                 // 6
                key_3_is_correct,               // 7
            };
        };

        // Non Constant
        {
            const auto result = test_default_allocator({1, 1, 2, 3});
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
            constexpr auto result = test_default_allocator({1, 1, 2, 3});
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

    // Test with default allocator with extra
    {
        auto test_default_allocator = [](std::initializer_list<key_type> initializer)
        {
            hud::hashset<key_type, hud::hash_64<key_type>, hud::equal<key_type>> set {initializer, 200};

            // Validate we have {1,11}
            const auto &it_1 = set.find(1);
            bool key_1_inserted = it_1 != set.end();
            bool key_1_is_correct = key_1_inserted;
            if (key_1_inserted)
            {
                // Validate key is correctly added
                key_1_is_correct &= it_1->key() == 1;
                key_1_is_correct &= it_1->key().copy_constructor_count() == 1;
                key_1_is_correct &= it_1->key().move_constructor_count() == 0;
            }

            // Validate we have {2,22}
            const auto &it_2 = set.find(2);
            bool key_2_inserted = it_2 != set.end();
            bool key_2_is_correct = key_2_inserted;
            if (key_2_inserted)
            {
                // Validate key is correctly added
                key_2_is_correct &= it_2->key() == 2;
                key_2_is_correct &= it_2->key().copy_constructor_count() == 1;
                key_2_is_correct &= it_2->key().move_constructor_count() == 0;
            }

            // Validate we have {3,33}
            const auto &it_3 = set.find(3);
            bool key_3_inserted = it_3 != set.end();
            bool key_3_is_correct = key_3_inserted;
            if (key_3_inserted)
            {
                // Validate key is correctly added
                key_3_is_correct &= it_3->key() == 3;
                key_3_is_correct &= it_3->key().copy_constructor_count() == 1;
                key_3_is_correct &= it_3->key().move_constructor_count() == 0;
            }

            return std::tuple {
                set.count() == 3,                     // 0
                set.max_count() >= set.count() + 200, // 1
                key_1_inserted,                       // 2
                key_1_is_correct,                     // 3
                key_2_inserted,                       // 4
                key_2_is_correct,                     // 5
                key_3_inserted,                       // 6
                key_3_is_correct,                     // 7
            };
        };

        // Non Constant
        {
            const auto result = test_default_allocator({1, 1, 2, 3});
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
            constexpr auto result = test_default_allocator({1, 1, 2, 3});
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

GTEST_TEST(hashset, construct_with_initializer_list_of_non_bitwise_move_constructible_different_type)
{
    using key_type = hud_test::non_bitwise_move_constructible_type2;
    using other_key_type = hud_test::non_bitwise_move_constructible_type;

    static_assert(!hud::is_bitwise_copy_constructible_v<key_type, other_key_type>);

    // Test with default allocator and no extra
    {
        auto test_default_allocator = [](std::initializer_list<other_key_type> initializer)
        {
            hud::hashset<key_type, hud::hash_64<key_type>, hud::equal<key_type>> set {initializer};

            // Validate we have {1,11}
            const auto &it_1 = set.find(1);
            bool key_1_inserted = it_1 != set.end();
            bool key_1_is_correct = key_1_inserted;
            if (key_1_inserted)
            {
                // Validate key is correctly added
                key_1_is_correct &= it_1->key() == 1;
                key_1_is_correct &= it_1->key().copy_constructor_count() == 1;
                key_1_is_correct &= it_1->key().move_constructor_count() == 0;
            }

            // Validate we have {2,22}
            const auto &it_2 = set.find(2);
            bool key_2_inserted = it_2 != set.end();
            bool key_2_is_correct = key_2_inserted;
            if (key_2_inserted)
            {
                // Validate key is correctly added
                key_2_is_correct &= it_2->key() == 2;
                key_2_is_correct &= it_2->key().copy_constructor_count() == 1;
                key_2_is_correct &= it_2->key().move_constructor_count() == 0;
            }

            // Validate we have {3,33}
            const auto &it_3 = set.find(3);
            bool key_3_inserted = it_3 != set.end();
            bool key_3_is_correct = key_3_inserted;
            if (key_3_inserted)
            {
                // Validate key is correctly added
                key_3_is_correct &= it_3->key() == 3;
                key_3_is_correct &= it_3->key().copy_constructor_count() == 1;
                key_3_is_correct &= it_3->key().move_constructor_count() == 0;
            }

            return std::tuple {
                set.count() == 3,               // 0
                set.max_count() >= set.count(), // 1
                key_1_inserted,                 // 2
                key_1_is_correct,               // 3
                key_2_inserted,                 // 4
                key_2_is_correct,               // 5
                key_3_inserted,                 // 6
                key_3_is_correct,               // 7
            };
        };

        // Non Constant
        {
            const auto result = test_default_allocator({1, 1, 2, 3});
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
            constexpr auto result = test_default_allocator({1, 1, 2, 3});
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

    // Test with default allocator with extra
    {
        auto test_default_allocator = [](std::initializer_list<other_key_type> initializer)
        {
            hud::hashset<key_type, hud::hash_64<key_type>, hud::equal<key_type>> set {initializer, 200};

            // Validate we have {1,11}
            const auto &it_1 = set.find(1);
            bool key_1_inserted = it_1 != set.end();
            bool key_1_is_correct = key_1_inserted;
            if (key_1_inserted)
            {
                // Validate key is correctly added
                key_1_is_correct &= it_1->key() == 1;
                key_1_is_correct &= it_1->key().copy_constructor_count() == 1;
                key_1_is_correct &= it_1->key().move_constructor_count() == 0;
            }

            // Validate we have {2,22}
            const auto &it_2 = set.find(2);
            bool key_2_inserted = it_2 != set.end();
            bool key_2_is_correct = key_2_inserted;
            if (key_2_inserted)
            {
                // Validate key is correctly added
                key_2_is_correct &= it_2->key() == 2;
                key_2_is_correct &= it_2->key().copy_constructor_count() == 1;
                key_2_is_correct &= it_2->key().move_constructor_count() == 0;
            }

            // Validate we have {3,33}
            const auto &it_3 = set.find(3);
            bool key_3_inserted = it_3 != set.end();
            bool key_3_is_correct = key_3_inserted;
            if (key_3_inserted)
            {
                // Validate key is correctly added
                key_3_is_correct &= it_3->key() == 3;
                key_3_is_correct &= it_3->key().copy_constructor_count() == 1;
                key_3_is_correct &= it_3->key().move_constructor_count() == 0;
            }

            return std::tuple {
                set.count() == 3,                     // 0
                set.max_count() >= set.count() + 200, // 1
                key_1_inserted,                       // 2
                key_1_is_correct,                     // 3
                key_2_inserted,                       // 4
                key_2_is_correct,                     // 5
                key_3_inserted,                       // 6
                key_3_is_correct,                     // 7
            };
        };

        // Non Constant
        {
            const auto result = test_default_allocator({1, 1, 2, 3});
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
            constexpr auto result = test_default_allocator({1, 1, 2, 3});
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
