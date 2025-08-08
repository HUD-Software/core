#include <core/containers/hashset.h>
#include <core/algorithms/find.h>

GTEST_TEST(hashset, slot_storage_is_not_movable_but_copyable)
{
    hud::hashset<hud_test::non_bitwise_type> set {1, 2, 3, 4};

    auto it = set.begin();
    // Should not be possible
    // auto slot_moved = hud::move(*it);
    auto slot_copied = *it;
    hud_assert_ne(&slot_copied.key(), &it->key());
    auto &slot_referenced = *it;
    hud_assert_eq(slot_referenced.key(), it->key());
    const auto &slot_creferenced = *it;
    hud_assert_eq(slot_creferenced.key(), it->key());
}

GTEST_TEST(hashset, iterators)
{
    const auto test = []()
    {
        hud::hashset<i32> set {1, 2, 3, 4};
        const hud::hashset<i32> const_set {1, 2, 3, 4};

        hud::hashset<i32, i64>::iterator it_begin = set.begin();
        hud::hashset<i32, i64>::const_iterator const_it_begin = const_set.begin();

        return std::tuple {
            hud::is_same_v<decltype((it_begin->key())), const i32 &>,
            hud::is_same_v<decltype((const_it_begin->key())), const i32 &>,
        };
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
    }
    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
    }
}

GTEST_TEST(hashset, structure_binding)
{
    // Non const Array
    {
        hud::hashset<i32> set {1, 2, 3, 4};
        hud_assert_eq(set.count(), 4u);
        hud_assert_ge(set.max_count(), 4u);
        auto [key] = *set.find(1);
        hud_assert_eq(key, 1);
        const auto [key_c] = *set.find(1);
        hud_assert_eq(key_c, 1);
    }

    // Const Array
    {
        const hud::hashset<i32> set {1, 2, 3, 4};
        hud_assert_eq(set.count(), 4u);
        hud_assert_ge(set.max_count(), 4u);
        auto [key] = *set.find(1);
        hud_assert_eq(key, 1);
        const auto [key_c] = *set.find(1);
        hud_assert_eq(key_c, 1);
    }

    // auto is a copy
    {
        hud::hashset<hud_test::non_bitwise_type> set {1, 2, 3, 4};
        hud_assert_eq(set.count(), 4u);
        hud_assert_ge(set.max_count(), 4u);
        auto [key] = *set.find(4);
        hud_assert_eq(key, 4);
        // Check key is a copy
        hud_assert_eq(key.constructor_count(), 1u);
        hud_assert_eq(key.copy_assign_count(), 0u);
        hud_assert_eq(key.copy_constructor_count(), 1u);
        hud_assert_eq(key.move_assign_count(), 0u);
        hud_assert_eq(key.move_constructor_count(), 0u);
        hud_assert_eq(key, 4);
        auto [first_1] = *set.find(4);
        hud_assert_eq(first_1, 4);
    }

    // auto& is a reference
    {
        hud::hashset<hud_test::non_bitwise_type> set {1, 2, 3, 4};
        hud_assert_eq(set.count(), 4u);
        hud_assert_ge(set.max_count(), 4u);
        auto &[key] = *set.find(4);
        hud_assert_eq(key, 4);
        // Check key is a reference
        hud_assert_eq(key.constructor_count(), 1u);
        hud_assert_eq(key.copy_assign_count(), 0u);
        hud_assert_eq(key.copy_constructor_count(), 0u);
        hud_assert_eq(key.move_assign_count(), 0u);
        hud_assert_eq(key.move_constructor_count(), 0u);
        auto &[first_1] = *set.find(4);
        hud_assert_eq(first_1, 4);
    }

    // auto&& is a reference
    {
        hud::hashset<hud_test::non_bitwise_type> set {1, 2, 3, 4};
        hud_assert_eq(set.count(), 4u);
        hud_assert_ge(set.max_count(), 4u);
        auto &&[key] = *set.find(4);
        hud_assert_eq(key, 4);
        // Check key is a reference
        hud_assert_eq(key.constructor_count(), 1u);
        hud_assert_eq(key.copy_assign_count(), 0u);
        hud_assert_eq(key.copy_constructor_count(), 0u);
        hud_assert_eq(key.move_assign_count(), 0u);
        hud_assert_eq(key.move_constructor_count(), 0u);
        auto &&[first_1] = *set.find(4);
        hud_assert_eq(first_1, 4);
    }

    // Loop
    {
        const auto test = []()
        {
            using map_type = hud::hashset<i32>;
            map_type set {1, 2, 3, 4};

            const map_type const_set {1, 2, 3, 4};

            map_type::key_type result[4];
            usize index = 0;
            for (const auto &[key] : set)
            {
                result[index++] = key;
            }

            map_type::key_type const_result[4];
            index = 0;
            for (const auto &[key] : const_set)
            {
                const_result[index++] = key;
            }
            i32 pair_1_11_index = hud::find_index_if(result, 4, [](const auto &key)
                                                     { return key == 1; });
            i32 pair_2_22_index = hud::find_index_if(result, 4, [](const auto &key)
                                                     { return key == 2; });
            i32 pair_3_33_index = hud::find_index_if(result, 4, [](const auto &key)
                                                     { return key == 3; });
            i32 pair_4_44_index = hud::find_index_if(result, 4, [](const auto &key)
                                                     { return key == 4; });

            i32 const_pair_1_11_index = hud::find_index_if(const_result, 4, [](const auto &key)
                                                           { return key == 1; });
            i32 const_pair_2_22_index = hud::find_index_if(const_result, 4, [](const auto &key)
                                                           { return key == 2; });
            i32 const_pair_3_33_index = hud::find_index_if(const_result, 4, [](const auto &key)
                                                           { return key == 3; });
            i32 const_pair_4_44_index = hud::find_index_if(const_result, 4, [](const auto &key)
                                                           { return key == 4; });

            return std::tuple {
                pair_1_11_index != -1,
                pair_2_22_index != -1,
                pair_3_33_index != -1,
                pair_4_44_index != -1,
                const_pair_1_11_index != -1,
                const_pair_2_22_index != -1,
                const_pair_3_33_index != -1,
                const_pair_4_44_index != -1,
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

GTEST_TEST(hashset, range_for_loop)
{
    const auto test = []()
    {
        using map_type = hud::hashset<i32>;
        map_type set {1, 2, 3, 4};

        const map_type const_set {1, 2, 3, 4};

        i32 result[4];
        usize index = 0;
        for (const auto &value : set)
        {
            result[index++] = value.key();
        }

        i32 const_result[4];
        index = 0;
        for (const auto &value : const_set)
        {
            const_result[index++] = value.key();
        }
        i32 pair_1_11_index = hud::find_index_if(result, 4, [](const auto &key)
                                                 { return key == 1; });
        i32 pair_2_22_index = hud::find_index_if(result, 4, [](const auto &key)
                                                 { return key == 2; });
        i32 pair_3_33_index = hud::find_index_if(result, 4, [](const auto &key)
                                                 { return key == 3; });
        i32 pair_4_44_index = hud::find_index_if(result, 4, [](const auto &key)
                                                 { return key == 4; });

        i32 const_pair_1_11_index = hud::find_index_if(const_result, 4, [](const auto &key)
                                                       { return key == 1; });
        i32 const_pair_2_22_index = hud::find_index_if(const_result, 4, [](const auto &key)
                                                       { return key == 2; });
        i32 const_pair_3_33_index = hud::find_index_if(const_result, 4, [](const auto &key)
                                                       { return key == 3; });
        i32 const_pair_4_44_index = hud::find_index_if(const_result, 4, [](const auto &key)
                                                       { return key == 4; });

        return std::tuple {
            pair_1_11_index != -1,
            pair_2_22_index != -1,
            pair_3_33_index != -1,
            pair_4_44_index != -1,
            const_pair_1_11_index != -1,
            const_pair_2_22_index != -1,
            const_pair_3_33_index != -1,
            const_pair_4_44_index != -1,
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