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
            hud::is_same_v<decltype(const_it_begin->first), const i32>,
            hud::is_same_v<decltype(const_it_begin->second), const i64>,
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