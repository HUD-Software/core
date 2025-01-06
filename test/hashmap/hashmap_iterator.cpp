#include <core/containers/hashmap.h>

GTEST_TEST(hashmap, iterators)
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
        hud::hashmap<i32, i64>::iterator it_begin = map.begin();
        hud_assert_true((hud::is_same_v<decltype(it_begin->key()), const i32 &>));
        hud_assert_true((hud::is_same_v<decltype(it_begin->value()), i64 &>));
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
        hud::hashmap<i32, i64>::const_iterator it_begin = map.begin();
        hud_assert_true((hud::is_same_v<decltype(it_begin->key()), const i32 &>));
        hud_assert_true((hud::is_same_v<decltype(it_begin->value()), const i64 &>));
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
        auto [first, second] = map.find(1);
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
//     // Non const Array
//     {
//         hud::array<i32> arr({10, 20, 30, 40});
//         hud_assert_ne(arr.data(), nullptr);
//         hud_assert_eq(arr.count(), 4u);
//         hud_assert_eq(arr.max_count(), 4u);
//         hud_assert_eq(arr[0], 10);
//         hud_assert_eq(arr[1], 20);
//         hud_assert_eq(arr[2], 30);
//         hud_assert_eq(arr[3], 40);

// i32 result[4];
// i32 index = 0;
// // constexpr Iterator begin() noexcept
// // constexpr Iterator end() noexcept
// for (auto &value : arr)
// {
//     result[index++] = value;
// }
// hud_assert_eq(result[0], arr[0]);
// hud_assert_eq(result[1], arr[1]);
// hud_assert_eq(result[2], arr[2]);
// hud_assert_eq(result[3], arr[3]);
// }

// // Const Array
// {
//     const hud::array<i32> arr({10, 20, 30, 40});
//     hud_assert_ne(arr.data(), nullptr);
//     hud_assert_eq(arr.count(), 4u);
//     hud_assert_eq(arr.max_count(), 4u);
//     hud_assert_eq(arr[0], 10);
//     hud_assert_eq(arr[1], 20);
//     hud_assert_eq(arr[2], 30);
//     hud_assert_eq(arr[3], 40);

// i32 result[4];
// i32 index = 0;
// // constexpr Iterator begin() noexcept
// // constexpr Iterator end() noexcept
// for (auto &value : arr)
// {
//     result[index++] = value;
// }
// hud_assert_eq(result[0], arr[0]);
// hud_assert_eq(result[1], arr[1]);
// hud_assert_eq(result[2], arr[2]);
// hud_assert_eq(result[3], arr[3]);
// }
// }