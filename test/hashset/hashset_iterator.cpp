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
//     hud::is_same_v<decltype((it_begin->key())), const i32 &>,
//     hud::is_same_v<decltype((const_it_begin->key())), const i32 &>
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
//         auto [key] = *set.find(1);
//         hud_assert_eq(key, 1);
//         const auto [key_c] = *set.find(1);
//         hud_assert_eq(key_c, 1);
//     }

// // Const Array
// {
//     const hud::hashset<i32> set {1, 2, 3, 4};
//     hud_assert_eq(set.count(), 4u);
//     hud_assert_ge(set.max_count(), 4u);
//     auto [key] = *set.find(1);
//     hud_assert_eq(key, 1);
//     const auto [key_c] = *set.find(1);
//     hud_assert_eq(key_c, 1);
// }

// // auto is a copy
// {
//     hud::hashset<hud_test::non_bitwise_type> set;
//     set.add(1);
//     set.add(2);
//     set.add(3);
//     set.add(4);
//     hud_assert_eq(set.count(), 4u);
//     hud_assert_ge(set.max_count(), 4u);
//     auto [key] = *set.find(4);
//     hud_assert_eq(key, 4);
//     // Check key is a copy
//     hud_assert_eq(key.constructor_count(), 1u);
//     hud_assert_eq(key.copy_assign_count(), 0u);
//     hud_assert_eq(key.copy_constructor_count(), 1u);
//     hud_assert_eq(key.move_assign_count(), 0u);
//     hud_assert_eq(key.move_constructor_count(), 1u);
// }

// // auto& is a reference
// {
//     hud::hashset<hud_test::non_bitwise_type> set;
//     set.add(1);
//     set.add(2);
//     set.add(3);
//     set.add(4);
//     hud_assert_eq(set.count(), 4u);
//     hud_assert_ge(set.max_count(), 4u);
//     auto &[key] = *set.find(4);
//     hud_assert_eq(key, 4);
//     // Check key is a reference (Not copie when binded)
//     hud_assert_eq(key.constructor_count(), 1u);
//     hud_assert_eq(key.copy_assign_count(), 0u);
//     hud_assert_eq(key.copy_constructor_count(), 0u);
//     hud_assert_eq(key.move_assign_count(), 0u);
//     hud_assert_eq(key.move_constructor_count(), 1u);
// }

// // // auto&& is a reference
// {
//     hud::hashset<hud_test::non_bitwise_type> set;
//     set.add(1);
//     set.add(2);
//     set.add(3);
//     set.add(4);
//     hud_assert_eq(set.count(), 4u);
//     hud_assert_ge(set.max_count(), 4u);
//     auto &&[key] = *set.find(4);
//     hud_assert_eq(key, 4);
//     // Check key is a reference (Not copie when binded)
//     hud_assert_eq(key.constructor_count(), 1u);
//     hud_assert_eq(key.copy_assign_count(), 0u);
//     hud_assert_eq(key.copy_constructor_count(), 0u);
//     hud_assert_eq(key.move_assign_count(), 0u);
//     hud_assert_eq(key.move_constructor_count(), 1u);
// }

// // Loop
// {
//     const auto test = []()
//     {
//         using set_type = hud::hashset<hud_test::non_bitwise_type>;
//         set_type set {1, 2, 3, 4};
//         const set_type const_set {1, 2, 3, 4};

// set_type::key_type result[4];
// usize index = 0;
// for (const auto &[key] : set)
// {
//     result[index++] = key;
// }

// set_type::key_type const_result[4];
// index = 0;
// for (const auto &[key] : const_set)
// {
//     const_result[index++] = key;
// }
// i32 elem_1_index = hud::find_index_if(result, 4, [](const auto &element)
//                                       { return element == 1; });
// i32 elem_2_index = hud::find_index_if(result, 4, [](const auto &element)
//                                       { return element == 2; });
// i32 elem_3_index = hud::find_index_if(result, 4, [](const auto &element)
//                                       { return element == 3; });
// i32 elem_4_index = hud::find_index_if(result, 4, [](const auto &element)
//                                       { return element == 4; });

// i32 const_elem_1_index = hud::find_index_if(const_result, 4, [](const auto &element)
//                                             { return element == 1; });
// i32 const_elem_2_index = hud::find_index_if(const_result, 4, [](const auto &element)
//                                             { return element == 2; });
// i32 const_elem_3_index = hud::find_index_if(const_result, 4, [](const auto &element)
//                                             { return element == 3; });
// i32 const_elem_4_index = hud::find_index_if(const_result, 4, [](const auto &element)
//                                             { return element == 4; });

// return std::tuple {
//     elem_1_index != -1,                             // 0
//     elem_2_index != -1,                             // 1
//     elem_3_index != -1,                             // 2
//     elem_4_index != -1,                             // 3
//     const_elem_1_index != -1,                       // 4
//     const_elem_2_index != -1,                       // 5
//     const_elem_3_index != -1,                       // 6
//     const_elem_4_index != -1,                       // 7
//     result[0].constructor_count() == 1u,            // 8
//     result[0].copy_assign_count() == 1u,            // 9
//     result[0].copy_constructor_count() == 0u,       // 10
//     result[0].move_assign_count() == 0u,            // 11
//     result[0].move_constructor_count() == 1u,       // 12
//     const_result[0].constructor_count() == 1u,      // 13
//     const_result[0].copy_assign_count() == 1u,      // 14
//     const_result[0].copy_constructor_count() == 0u, // 15
//     const_result[0].move_assign_count() == 0u,      // 16
//     const_result[0].move_constructor_count() == 1u, // 17
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
//     hud_assert_true(std::get<8>(result));
//     hud_assert_true(std::get<9>(result));
//     hud_assert_true(std::get<10>(result));
//     hud_assert_true(std::get<11>(result));
//     hud_assert_true(std::get<12>(result));
//     hud_assert_true(std::get<13>(result));
//     hud_assert_true(std::get<14>(result));
//     hud_assert_true(std::get<15>(result));
//     hud_assert_true(std::get<16>(result));
//     hud_assert_true(std::get<17>(result));
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
//     hud_assert_true(std::get<8>(result));
//     hud_assert_true(std::get<9>(result));
//     hud_assert_true(std::get<10>(result));
//     hud_assert_true(std::get<11>(result));
//     hud_assert_true(std::get<12>(result));
//     hud_assert_true(std::get<13>(result));
//     hud_assert_true(std::get<14>(result));
//     hud_assert_true(std::get<15>(result));
//     hud_assert_true(std::get<16>(result));
//     hud_assert_true(std::get<17>(result));
// }
// }
// }

// GTEST_TEST(hashset, range_for_loop)
// {
//     const auto test = []()
//     {
//         using set_type = hud::hashset<hud_test::non_bitwise_type>;
//         set_type set {1, 2, 3, 4};
//         const set_type const_set {1, 2, 3, 4};

// set_type::key_type result[4];
// usize index = 0;
// for (const auto &value : set)
// {
//     result[index++] = value.key();
// }

// set_type::key_type const_result[4];
// index = 0;
// for (const auto &value : const_set)
// {
//     const_result[index++] = value.key();
// }
// i32 elem_1_index = hud::find_index_if(result, 4, [](const auto &element)
//                                       { return element == 1; });
// i32 elem_2_index = hud::find_index_if(result, 4, [](const auto &element)
//                                       { return element == 2; });
// i32 elem_3_index = hud::find_index_if(result, 4, [](const auto &element)
//                                       { return element == 3; });
// i32 elem_4_index = hud::find_index_if(result, 4, [](const auto &element)
//                                       { return element == 4; });

// i32 const_elem_1_index = hud::find_index_if(const_result, 4, [](const auto &element)
//                                             { return element == 1; });
// i32 const_elem_2_index = hud::find_index_if(const_result, 4, [](const auto &element)
//                                             { return element == 2; });
// i32 const_elem_3_index = hud::find_index_if(const_result, 4, [](const auto &element)
//                                             { return element == 3; });
// i32 const_elem_4_index = hud::find_index_if(const_result, 4, [](const auto &element)
//                                             { return element == 4; });

// return std::tuple {
//     elem_1_index != -1,                             // 0
//     elem_2_index != -1,                             // 1
//     elem_3_index != -1,                             // 2
//     elem_4_index != -1,                             // 3
//     const_elem_1_index != -1,                       // 4
//     const_elem_2_index != -1,                       // 5
//     const_elem_3_index != -1,                       // 6
//     const_elem_4_index != -1,                       // 7
//     result[0].constructor_count() == 1u,            // 8
//     result[0].copy_assign_count() == 1u,            // 9
//     result[0].copy_constructor_count() == 0u,       // 10
//     result[0].move_assign_count() == 0u,            // 11
//     result[0].move_constructor_count() == 1u,       // 12
//     const_result[0].constructor_count() == 1u,      // 13
//     const_result[0].copy_assign_count() == 1u,      // 14
//     const_result[0].copy_constructor_count() == 0u, // 15
//     const_result[0].move_assign_count() == 0u,      // 16
//     const_result[0].move_constructor_count() == 1u, // 17
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
//     hud_assert_true(std::get<8>(result));
//     hud_assert_true(std::get<9>(result));
//     hud_assert_true(std::get<10>(result));
//     hud_assert_true(std::get<11>(result));
//     hud_assert_true(std::get<12>(result));
//     hud_assert_true(std::get<13>(result));
//     hud_assert_true(std::get<14>(result));
//     hud_assert_true(std::get<15>(result));
//     hud_assert_true(std::get<16>(result));
//     hud_assert_true(std::get<17>(result));
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
//     hud_assert_true(std::get<8>(result));
//     hud_assert_true(std::get<9>(result));
//     hud_assert_true(std::get<10>(result));
//     hud_assert_true(std::get<11>(result));
//     hud_assert_true(std::get<12>(result));
//     hud_assert_true(std::get<13>(result));
//     hud_assert_true(std::get<14>(result));
//     hud_assert_true(std::get<15>(result));
//     hud_assert_true(std::get<16>(result));
//     hud_assert_true(std::get<17>(result));
// }
// }