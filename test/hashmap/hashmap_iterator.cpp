#include <core/containers/hashmap.h>
#include <core/algorithms/find.h>

GTEST_TEST(hashmap, iterators)
{
    const auto test = []()
    {
        hud::hashmap<i32, i64> map {
            {1, 11},
            {2, 22},
            {3, 33},
            {4, 44}
        };
        const hud::hashmap<i32, i64> const_map {
            {1, 11},
            {2, 22},
            {3, 33},
            {4, 44}
        };

        hud::hashmap<i32, i64>::iterator it_begin = map.begin();
        hud::hashmap<i32, i64>::const_iterator const_it_begin = const_map.begin();

        return std::tuple {
            hud::is_same_v<decltype((it_begin->key())), const i32 &>,
            hud::is_same_v<decltype((it_begin->value())), i64 &>,
            hud::is_same_v<decltype((const_it_begin->key())), const i32 &>,
            hud::is_same_v<decltype((const_it_begin->value())), const i64 &>,
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

GTEST_TEST(hashmap, structure_binding)
{
    // Non const Array
    {
        hud::hashmap<i32, i64> map;
        map.add({1, 11});
        map.add({2, 22});
        map.add({3, 33});
        map.add({4, 44});
        hud_assert_eq(map.count(), 4u);
        hud_assert_ge(map.max_count(), 4u);
        auto [first, second] = *map.find(1);
        hud_assert_eq(first, 1);
        hud_assert_eq(second, 11);
        const auto [first_c, second_c] = *map.find(1);
        hud_assert_eq(first_c, 1);
        hud_assert_eq(second_c, 11);
    }

    // Const Array
    {
        const hud::hashmap<i32, i64> map {
            {1, 11},
            {2, 22},
            {3, 33},
            {4, 44}
        };
        hud_assert_eq(map.count(), 4u);
        hud_assert_ge(map.max_count(), 4u);
        auto [first, second] = *map.find(1);
        hud_assert_eq(first, 1);
        hud_assert_eq(second, 11);
        const auto [first_c, second_c] = *map.find(1);
        hud_assert_eq(first_c, 1);
        hud_assert_eq(second_c, 11);
    }

    // auto is a copy
    {
        hud::hashmap<i32, i64> map;
        map.add({1, 11});
        map.add({2, 22});
        map.add({3, 33});
        map.add({4, 44});
        hud_assert_eq(map.count(), 4u);
        hud_assert_ge(map.max_count(), 4u);
        auto [first, second] = *map.find(4);
        hud_assert_eq(first, 4);
        second = 444;
        hud_assert_eq(first, 4);
        hud_assert_eq(second, 444);
        auto [first_1, second_1] = *map.find(4);
        hud_assert_eq(first_1, 4);
        hud_assert_eq(second_1, 44);
    }

    // auto& is a reference
    {
        hud::hashmap<i32, i64> map;
        map.add({1, 11});
        map.add({2, 22});
        map.add({3, 33});
        map.add({4, 44});
        hud_assert_eq(map.count(), 4u);
        hud_assert_ge(map.max_count(), 4u);
        auto &[first, second] = *map.find(4);
        hud_assert_eq(first, 4);
        second = 444;
        hud_assert_eq(second, 444);
        auto &[first_1, second_1] = *map.find(4);
        hud_assert_eq(first_1, 4);
        hud_assert_eq(second_1, 444);
    }

    // auto&& is a reference
    {
        hud::hashmap<i32, i64> map;
        map.add({1, 11});
        map.add({2, 22});
        map.add({3, 33});
        map.add({4, 44});
        hud_assert_eq(map.count(), 4u);
        hud_assert_ge(map.max_count(), 4u);
        auto &&[first, second] = *map.find(1);
        hud_assert_eq(first, 1);
        second = 111;
        hud_assert_eq(second, 111);
        auto &&[first_1, second_1] = *map.find(1);
        hud_assert_eq(first_1, 1);
        hud_assert_eq(second_1, 111);
    }

    // Loop
    {
        const auto test = []()
        {
            using map_type = hud::hashmap<i32, i64>;
            map_type map {
                {1, 11},
                {2, 22},
                {3, 33},
                {4, 44}
            };
            const map_type const_map {
                {1, 11},
                {2, 22},
                {3, 33},
                {4, 44}
            };

            hud::pair<map_type::key_type, map_type::value_type> result[4];
            usize index = 0;
            for (const auto &[first, second] : map)
            {
                result[index++] = {first, second};
            }

            hud::pair<map_type::key_type, map_type::value_type> const_result[4];
            index = 0;
            for (const auto &[first, second] : const_map)
            {
                const_result[index++] = {first, second};
            }
            i32 pair_1_11_index = hud::find_index_if(result, 4, [](const auto &pair)
                                                     { return pair.first == 1 && pair.second == 11; });
            i32 pair_2_22_index = hud::find_index_if(result, 4, [](const auto &pair)
                                                     { return pair.first == 2 && pair.second == 22; });
            i32 pair_3_33_index = hud::find_index_if(result, 4, [](const auto &pair)
                                                     { return pair.first == 3 && pair.second == 33; });
            i32 pair_4_44_index = hud::find_index_if(result, 4, [](const auto &pair)
                                                     { return pair.first == 4 && pair.second == 44; });

            i32 const_pair_1_11_index = hud::find_index_if(const_result, 4, [](const auto &pair)
                                                           { return pair.first == 1 && pair.second == 11; });
            i32 const_pair_2_22_index = hud::find_index_if(const_result, 4, [](const auto &pair)
                                                           { return pair.first == 2 && pair.second == 22; });
            i32 const_pair_3_33_index = hud::find_index_if(const_result, 4, [](const auto &pair)
                                                           { return pair.first == 3 && pair.second == 33; });
            i32 const_pair_4_44_index = hud::find_index_if(const_result, 4, [](const auto &pair)
                                                           { return pair.first == 4 && pair.second == 44; });

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

GTEST_TEST(hashmap, range_for_loop)
{
    const auto test = []()
    {
        using map_type = hud::hashmap<i32, i64>;
        map_type map {
            {1, 11},
            {2, 22},
            {3, 33},
            {4, 44}
        };
        const map_type const_map {
            {1, 11},
            {2, 22},
            {3, 33},
            {4, 44}
        };

        hud::pair<i32, i64> result[4];
        usize index = 0;
        for (const auto &value : map)
        {
            result[index++] = {value.key(), value.value()};
        }

        hud::pair<i32, i64> const_result[4];
        index = 0;
        for (const auto &value : const_map)
        {
            const_result[index++] = {value.key(), value.value()};
        }
        i32 pair_1_11_index = hud::find_index_if(result, 4, [](const auto &pair)
                                                 { return pair.first == 1 && pair.second == 11; });
        i32 pair_2_22_index = hud::find_index_if(result, 4, [](const auto &pair)
                                                 { return pair.first == 2 && pair.second == 22; });
        i32 pair_3_33_index = hud::find_index_if(result, 4, [](const auto &pair)
                                                 { return pair.first == 3 && pair.second == 33; });
        i32 pair_4_44_index = hud::find_index_if(result, 4, [](const auto &pair)
                                                 { return pair.first == 4 && pair.second == 44; });

        i32 const_pair_1_11_index = hud::find_index_if(const_result, 4, [](const auto &pair)
                                                       { return pair.first == 1 && pair.second == 11; });
        i32 const_pair_2_22_index = hud::find_index_if(const_result, 4, [](const auto &pair)
                                                       { return pair.first == 2 && pair.second == 22; });
        i32 const_pair_3_33_index = hud::find_index_if(const_result, 4, [](const auto &pair)
                                                       { return pair.first == 3 && pair.second == 33; });
        i32 const_pair_4_44_index = hud::find_index_if(const_result, 4, [](const auto &pair)
                                                       { return pair.first == 4 && pair.second == 44; });

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