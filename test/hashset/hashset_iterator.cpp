// #include <core/containers/hashset.h>
// #include <core/algorithms/find.h>

// GTEST_TEST(hashset, iterators)
// {
//     const auto test = []()
//     {
//         hud::hashset<i32> set {1, 2, 3, 4};
//         const hud::hashset<i32> const_set {1, 2, 3, 4};

// hud::hashset<i32>::iterator it_begin = set.begin();
// hud::hashset<i32>::const_iterator const_it_begin = const_set.begin();

// return std::tuple {
//     hud::is_same_v<decltype((*it_begin)), i32 &>,
//     hud::is_same_v<decltype((*const_it_begin)), const i32 &>
// };
// };

// // Non constant
// {
//     const auto result = test();
//     hud_assert_true(std::get<0>(result));
//     hud_assert_true(std::get<1>(result));
// }
// // Constant
// {
//     constexpr auto result = test();
//     hud_assert_true(std::get<0>(result));
//     hud_assert_true(std::get<1>(result));
// }
// }

// GTEST_TEST(hashset, structure_binding)
// {
//     // Non const Array
//     {
//         hud::hashset<i32> set;
//         set.add(1);
//         set.add(2);
//         set.add(3);
//         set.add(4);
//         hud_assert_eq(set.count(), 4u);
//         hud_assert_ge(set.max_count(), 4u);
//         auto [value] = set.find(1);
//         hud_assert_eq(value, 1);
//         const auto [const_value] = set.find(1);
//         hud_assert_eq(const_value, 1);
//     }

// // Const Array
// {
//     const hud::hashset<i32> set {
//         1,
//         2,
//         3,
//         4
//     };
//     hud_assert_eq(set.count(), 4u);
//     hud_assert_ge(set.max_count(), 4u);
//     auto [value] = set.find(1);
//     hud_assert_eq(value, 1);
//     const auto [const_value] = set.find(1);
//     hud_assert_eq(const_value, 1);
// }

// // auto is a reference
// {
//     hud::hashset<i32> set;
//     set.add(1);
//     set.add(2);
//     set.add(3);
//     set.add(4);
//     hud_assert_eq(set.count(), 4u);
//     hud_assert_ge(set.max_count(), 4u);
//     auto it = set.find(4);
//     auto [value] = it;
//     hud_assert_eq(value, 4);
//     value = 444;
//     auto [value_1] = set.find(4);
//     hud_assert_eq(value_1, 444);
// }

// // Ref do modify value
// {
//     hud::hashset<hud_test::non_bitwise_copy_assignable_type> set;
//     set.add(1);
//     set.add(2);
//     set.add(3);
//     set.add(4);
//     hud_assert_eq(set.count(), 4u);
//     hud_assert_ge(set.max_count(), 4u);
//     auto it = set.find(4);
//     auto &[value] = it;
//     hud_assert_eq(value, 4);
//     value = 444;
//     auto it_1 = set.find(4);
//     auto &[value_1] = it_1;
//     hud_assert_eq(value_1, 444);
// }

// // Loop
// {
//     const auto test = []()
//     {
//         using set_type = hud::hashset<i32>;
//         set_type set {
//             1,
//             2,
//             3,
//             4
//         };
//         const set_type const_set {
//             1,
//             2,
//             3,
//             4
//         };

// set_type::type result[4];
// usize index = 0;
// for (const auto &value : set)
// {
//     result[index++] = value;
// }

// set_type::type const_result[4];
// index = 0;
// for (const auto &value : const_set)
// {
//     const_result[index++] = value;
// }
// i32 value_1_11_index = hud::find_index_if(result, 4, [](const auto &value)
//                                           { return value == 1; });
// i32 value_2_22_index = hud::find_index_if(result, 4, [](const auto &value)
//                                           { return value == 2; });
// i32 value_3_33_index = hud::find_index_if(result, 4, [](const auto &value)
//                                           { return value == 3; });
// i32 value_4_44_index = hud::find_index_if(result, 4, [](const auto &value)
//                                           { return value == 4; });

// i32 const_value_1_11_index = hud::find_index_if(const_result, 4, [](const auto &value)
//                                                 { return value == 1; });
// i32 const_value_2_22_index = hud::find_index_if(const_result, 4, [](const auto &value)
//                                                 { return value == 2; });
// i32 const_value_3_33_index = hud::find_index_if(const_result, 4, [](const auto &value)
//                                                 { return value == 3; });
// i32 const_value_4_44_index = hud::find_index_if(const_result, 4, [](const auto &value)
//                                                 { return value == 4; });

// return std::tuple {
//     value_1_11_index != -1,
//     value_2_22_index != -1,
//     value_3_33_index != -1,
//     value_4_44_index != -1,
//     const_value_1_11_index != -1,
//     const_value_2_22_index != -1,
//     const_value_3_33_index != -1,
//     const_value_4_44_index != -1,
// };
// };

// // Non constant
// {
//     const auto result = test();
//     hud_assert_true(std::get<0>(result));
//     hud_assert_true(std::get<1>(result));
//     hud_assert_true(std::get<2>(result));
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_true(std::get<6>(result));
//     hud_assert_true(std::get<7>(result));
// }

// // Constant
// {
//     constexpr auto result = test();
//     hud_assert_true(std::get<0>(result));
//     hud_assert_true(std::get<1>(result));
//     hud_assert_true(std::get<2>(result));
//     hud_assert_true(std::get<3>(result));
//     hud_assert_true(std::get<4>(result));
//     hud_assert_true(std::get<5>(result));
//     hud_assert_true(std::get<6>(result));
//     hud_assert_true(std::get<7>(result));
// }
// }
// }

// // GTEST_TEST(hashset, range_for_loop)
// // {
// //     const auto test = []()
// //     {
// //         using set_type = hud::hashset<i32, i64>;
// //         set_type map {
// //             {1, 11},
// //             {2, 22},
// //             {3, 33},
// //             {4, 44}
// //         };
// //         const set_type const_map {
// //             {1, 11},
// //             {2, 22},
// //             {3, 33},
// //             {4, 44}
// //         };

// // set_type::type result[4];
// // usize index = 0;
// // for (const auto &value : map)
// // {
// //     result[index++] = value;
// // }

// // set_type::type const_result[4];
// // index = 0;
// // for (const auto &value : const_map)
// // {
// //     const_result[index++] = value;
// // }
// // i32 value_1_11_index = hud::find_index_if(result, 4, [](const auto &value)
// //                                          { return value.first == 1 && value.second == 11; });
// // i32 value_2_22_index = hud::find_index_if(result, 4, [](const auto &value)
// //                                          { return value.first == 2 && value.second == 22; });
// // i32 value_3_33_index = hud::find_index_if(result, 4, [](const auto &value)
// //                                          { return value.first == 3 && value.second == 33; });
// // i32 value_4_44_index = hud::find_index_if(result, 4, [](const auto &value)
// //                                          { return value.first == 4 && value.second == 44; });

// // i32 const_value_1_11_index = hud::find_index_if(const_result, 4, [](const auto &value)
// //                                                { return value.first == 1 && value.second == 11; });
// // i32 const_value_2_22_index = hud::find_index_if(const_result, 4, [](const auto &value)
// //                                                { return value.first == 2 && value.second == 22; });
// // i32 const_value_3_33_index = hud::find_index_if(const_result, 4, [](const auto &value)
// //                                                { return value.first == 3 && value.second == 33; });
// // i32 const_value_4_44_index = hud::find_index_if(const_result, 4, [](const auto &value)
// //                                                { return value.first == 4 && value.second == 44; });

// // return std::tuple {
// //     value_1_11_index != -1,
// //     value_2_22_index != -1,
// //     value_3_33_index != -1,
// //     value_4_44_index != -1,
// //     const_value_1_11_index != -1,
// //     const_value_2_22_index != -1,
// //     const_value_3_33_index != -1,
// //     const_value_4_44_index != -1,
// // };
// // };

// // // Non constant
// // {
// //     const auto result = test();
// //     hud_assert_true(std::get<0>(result));
// //     hud_assert_true(std::get<1>(result));
// //     hud_assert_true(std::get<2>(result));
// //     hud_assert_true(std::get<3>(result));
// //     hud_assert_true(std::get<4>(result));
// //     hud_assert_true(std::get<5>(result));
// //     hud_assert_true(std::get<6>(result));
// //     hud_assert_true(std::get<7>(result));
// // }

// // // Constant
// // {
// //     constexpr auto result = test();
// //     hud_assert_true(std::get<0>(result));
// //     hud_assert_true(std::get<1>(result));
// //     hud_assert_true(std::get<2>(result));
// //     hud_assert_true(std::get<3>(result));
// //     hud_assert_true(std::get<4>(result));
// //     hud_assert_true(std::get<5>(result));
// //     hud_assert_true(std::get<6>(result));
// //     hud_assert_true(std::get<7>(result));
// // }
// // }