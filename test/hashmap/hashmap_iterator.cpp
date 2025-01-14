#include <core/containers/hashmap.h>

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
            hud::is_same_v<decltype(it_begin->first), i32>,
            hud::is_same_v<decltype(it_begin->second), i64>,
            hud::is_same_v<decltype((const_it_begin->first)), const i32 &>,
            hud::is_same_v<decltype((const_it_begin->second)), const i64 &>,
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
        auto [first, second] = map.find(1);
        hud_assert_eq(first, 1);
        hud_assert_eq(second, 11);
        const auto [first_c, second_c] = map.find(1);
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
        auto [first, second] = map.find(1);
        hud_assert_eq(first, 1);
        hud_assert_eq(second, 11);
        const auto [first_c, second_c] = map.find(1);
        hud_assert_eq(first_c, 1);
        hud_assert_eq(second_c, 11);
    }

    // Copy do not modify hash value
    {
        hud::hashmap<i32, i64> map;
        map.add({1, 11});
        map.add({2, 22});
        map.add({3, 33});
        map.add({4, 44});
        hud_assert_eq(map.count(), 4u);
        hud_assert_ge(map.max_count(), 4u);
        auto it = map.find(1);
        auto [first, second] = it;
        hud_assert_eq(first, 1);
        second = 111;
        hud_assert_eq(second, 111);
        auto [first_1, second_1] = map.find(1);
        hud_assert_eq(first_1, 1);
        hud_assert_eq(second_1, 11);
    }

    // Ref do modify hash value
    {
        hud::hashmap<i32, hud_test::non_bitwise_copy_assignable_type> map;
        map.add({1, 11});
        map.add({2, 22});
        map.add({3, 33});
        map.add({4, 44});
        hud_assert_eq(map.count(), 4u);
        hud_assert_ge(map.max_count(), 4u);
        auto it = map.find(4);
        auto &[first, second] = *it;
        hud_assert_eq(first, 4);
        second = 444;
        hud_assert_eq(second, 444);
        auto it_1 = map.find(1);
        auto &[first_1, second_1] = *it_1;
        hud_assert_eq(first_1, 1);
        hud_assert_eq(second_1, 11);
    }
}

// GTEST_TEST(hashmap, range_for_loop)
// {
//     const auto test = []()
//     {
//         using map_type = hud::hashmap<i32, i64>;
//         map_type map {
//             {1, 11},
//             {2, 22},
//             {3, 33},
//             {4, 44}
//         };
//         const map_type const_map {
//             {1, 11},
//             {2, 22},
//             {3, 33},
//             {4, 44}
//         };

// map_type::type result[4];
// usize index = 0;
// for (const auto &value : map)
// {
//     result[index++] = value;
// }
// map_type::type const_result[4];
// index = 0;
// for (const auto &value : const_map)
// {
//     const_result[index++] = value;
// }

// return std::tuple {
//     result[0] == {1, 11},
//     result[1] == {2, 22},
//     result[2] == {3, 33},
//     result[3] == {4, 44},
//     const_result[0] == {1, 11},
//     const_result[1] == {2, 22},
//     const_result[2] == {3, 33},
//     const_result[3] == {4, 44},
// };
// };

// // Non constant
// {
// }

// // Constant
// {
// }
// }