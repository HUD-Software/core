#include <core/containers/hashmap.h>
#include "../misc/allocator_watcher.h"

GTEST_TEST(hashmap, default_constructor_should_allocate_no_memory)
{
    hud::hashmap<i32, i64, hud::hashmap_default_hasher<i32>, hud::hashmap_default_key_equal<i32>, hud_test::allocator_watcher<1>> map;
    hud_assert_eq(map.count(), 0u);
    hud_assert_eq(map.max_count(), 0u);
    hud_assert_eq(map.allocator().allocation_count(), 0u);
    hud_assert_eq(map.allocator().free_count(), 0u);
    hud_assert_eq(map.allocator().move_assign_count(), 0u);
    hud_assert_eq(map.allocator().move_construct_count(), 0u);
}

GTEST_TEST(hashmap, construct_with_initializer_list_of_bitwise_copy_constructible_same_type)
{

    using key_type = i32;
    using value_type = i64;

    static_assert(hud::is_bitwise_copy_constructible_v<key_type>);
    static_assert(hud::is_bitwise_copy_constructible_v<value_type>);

    // Test with default allocator and no extra
    {
        auto test_default_allocator = [](std::initializer_list<hud::pair<key_type, value_type>> initializer)
        {
            hud::hashmap<key_type, value_type, hud::hashmap_default_hasher<key_type>, hud::hashmap_default_key_equal<key_type>> map {initializer};

            // Validate we have {1,11}
            const auto &it_1 = map.find(1);
            bool key_1_inserted = it_1 != map.end();
            bool key_1_is_correct = key_1_inserted ? it_1->key() == 1 && it_1->value() == 11 : false;

            // Validate we have {2,22}
            const auto &it_2 = map.find(2);
            bool key_2_inserted = it_2 != map.end();
            bool key_2_is_correct = key_2_inserted ? it_2->key() == 2 && it_2->value() == 22 : false;

            // Validate we have {3,33}
            const auto &it_3 = map.find(3);
            bool key_3_inserted = it_3 != map.end();
            bool key_3_is_correct = key_3_inserted ? it_3->key() == 3 && it_3->value() == 33 : false;

            return std::tuple {
                map.count() == 3,
                map.max_count() >= 3,
                key_1_inserted,
                key_1_is_correct,
                key_2_inserted,
                key_2_is_correct,
                key_3_inserted,
                key_3_is_correct,
            };
        };

        // Non Constant
        {
            const auto result = test_default_allocator({
                {1, 11},
                {1, 00},
                {2, 22},
                {3, 33}
            });
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
            constexpr auto result = test_default_allocator({
                {1, 11},
                {1, 00},
                {2, 22},
                {3, 33}
            });
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

GTEST_TEST(hashmap, construct_with_initializer_list_of_non_bitwise_copy_constructible_same_type)
{

    using key_type = hud_test::non_bitwise_copy_constructible_type;
    using value_type = hud_test::non_bitwise_copy_constructible_type;

    static_assert(!hud::is_bitwise_copy_constructible_v<key_type>);
    static_assert(!hud::is_bitwise_copy_constructible_v<value_type>);

    // Test with default allocator and no extra
    {
        auto test_default_allocator = [](std::initializer_list<hud::pair<key_type, value_type>> initializer)
        {
            hud::hashmap<key_type, value_type, hud::hashmap_default_hasher<key_type>, hud::hashmap_default_key_equal<key_type>> map {initializer};

            // Validate we have {1,11}
            const auto &it_1 = map.find(1);
            bool key_1_inserted = it_1 != map.end();
            bool key_1_is_correct = key_1_inserted;
            if (key_1_inserted)
            {
                // Validate key is correctly added
                key_1_is_correct &= it_1->key() == 1;

                // Validate value is correctly added
                key_1_is_correct &= it_1->value() == 11;
            }
            // bool key_1_is_correct = key_1_inserted ? it_1->key() == 1 && it_1->value() == 11 : false;

            // Validate we have {2,22}
            const auto &it_2 = map.find(2);
            bool key_2_inserted = it_2 != map.end();
            bool key_2_is_correct = key_2_inserted ? it_2->key() == 2 && it_2->value() == 22 : false;

            // Validate we have {3,33}
            const auto &it_3 = map.find(3);
            bool key_3_inserted = it_3 != map.end();
            bool key_3_is_correct = key_3_inserted ? it_3->key() == 3 && it_3->value() == 33 : false;

            return std::tuple {
                map.count() == 3,
                map.max_count() >= 3,
                key_1_inserted,
                key_1_is_correct,
                key_2_inserted,
                key_2_is_correct,
                key_3_inserted,
                key_3_is_correct,
            };
        };

        // Non Constant
        {
            const auto result = test_default_allocator({
                {1, 11},
                {1, 00},
                {2, 22},
                {3, 33}
            });
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
            constexpr auto result = test_default_allocator({
                {1, 11},
                {1, 00},
                {2, 22},
                {3, 33}
            });
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