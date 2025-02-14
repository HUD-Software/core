#include <core/containers/hashmap.h>
#include "../misc/allocator_watcher.h"

#define TEST_VALUES { \
    {0,  0},           \
    {1, 11},          \
    {2, 22},          \
    {3, 33},          \
    {4, 44},          \
    {5, 55},          \
    {6, 66},          \
    {7, 77}           \
}

GTEST_TEST(hashmap, copy_construct_bitwise_copy_constructible_same_type_same_allocator)
{
    /** The array we copy for test, we allocate also extra memory to test if we really copy the count(), not the max_count() elements */
    // using key_type = i32;
    // using value_type = i64;

    // static_assert(hud::is_bitwise_copy_constructible_v<key_type>);
    // static_assert(hud::is_bitwise_copy_constructible_v<value_type>);

    // using AllocatorType = hud_test::allocator_watcher<1>;
    // using NewType = hud::hashmap<key_type, value_type, hud::hashmap_default_hasher<key_type>, hud::hashmap_default_key_equal<key_type>, AllocatorType>;
    // using CopiedType = hud::hashmap<key_type, value_type, hud::hashmap_default_hasher<key_type>, hud::hashmap_default_key_equal<key_type>, AllocatorType>;

    // // No extra
    // {
    //     auto test_default_allocator = [](std::initializer_list<hud::pair<i32, i64>> initializer)
    //     {
    //         const CopiedType copied(initializer);

    // // Copy the map
    // NewType copy(copied);

    // // Ensure we copy all elements
    // bool all_keys_and_values_copied = true;
    // for (usize index = 0; index < initializer.size(); index++)
    // {
    //     const auto &init_elem = (initializer.begin() + index);
    //     const auto it = copy.find(init_elem->first);
    //     if (it == copy.end())
    //     {
    //         all_keys_and_values_copied = false;
    //         break;
    //     }
    //     if (it->key() != init_elem->first)
    //     {
    //         all_keys_and_values_copied = false;
    //         break;
    //     }
    //     if (it->value() != init_elem->second)
    //     {
    //         all_keys_and_values_copied = false;
    //         break;
    //     }
    // }

    // return std::tuple {
    //     copy.count() == copied.count(),         // 0
    //     copy.max_count() == copied.max_count(), // 1
    //     all_keys_and_values_copied,             // 2
    //     copy.allocator().allocation_count(),    // 3
    //     copy.allocator().free_count()           // 4
    // };
    // };

    // // Non constant
    // {
    //     const auto result = test_default_allocator(TEST_VALUES);
    //     hud_assert_true(std::get<0>(result));
    //     hud_assert_true(std::get<1>(result));
    //     hud_assert_true(std::get<2>(result));
    //     hud_assert_true(std::get<3>(result));
    //     hud_assert_true(std::get<4>(result));
    // }

    // Constant
    // {
    //     constexpr auto result = test_default_allocator(TEST_VALUES);
    //     hud_assert_true(std::get<0>(result));
    //     hud_assert_true(std::get<1>(result));
    //     hud_assert_true(std::get<2>(result));
    //     hud_assert_true(std::get<3>(result));
    //     hud_assert_true(std::get<4>(result));
    // }

    // auto test_with_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
    // {
    //     const CopiedType copied(initializer, copied_extra);

    // // Copy the array
    // hud::array<type, AllocatorType> copy(copied, AllocatorType {});

    // // Ensure we copy all datas in order
    // bool all_values_copied = true;
    // for (usize index = 0; index < initializer.size(); index++)
    // {
    //     if (copy[index] != static_cast<type>(index))
    //     {
    //         all_values_copied = false;
    //         break;
    //     }
    // }

    // return std::tuple {
    //     // Ensure we copy all datas in order
    //     copy.data() != nullptr,
    //     copy.count(),
    //     copy.max_count(),
    //     all_values_copied,

    // // Ensure the copy data is not the same memory of the copied data
    // copied.data() != copy.data(),

    // // Ensure we are allocating only one time
    // copy.allocator().allocation_count(),
    // copy.allocator().free_count()
    // };
    // };

    // // Non constant
    // {
    //     const auto result = test_with_allocator({0, 1, 2, 3}, 1u);
    //     // Ensure we copy all datas in order
    //     hud_assert_true(std::get<0>(result));
    //     hud_assert_eq(std::get<1>(result), 4u);
    //     hud_assert_eq(std::get<2>(result), 4u + 1u);
    //     hud_assert_true(std::get<3>(result));

    // // Ensure the copy data is not the same memory of the copied data
    // hud_assert_true(std::get<4>(result));

    // // Ensure we are allocating only one time
    // hud_assert_eq(std::get<5>(result), 1u);
    // hud_assert_eq(std::get<6>(result), 0u);
    // }

    // // Constant
    // {
    //     constexpr auto result = test_with_allocator({0, 1, 2, 3}, 1u);
    //     // Ensure we copy all datas in order
    //     hud_assert_true(std::get<0>(result));
    //     hud_assert_eq(std::get<1>(result), 4u);
    //     hud_assert_eq(std::get<2>(result), 4u + 1u);
    //     hud_assert_true(std::get<3>(result));

    // // Ensure the copy data is not the same memory of the copied data
    // hud_assert_true(std::get<4>(result));

    // // Ensure we are allocating only one time
    // hud_assert_eq(std::get<5>(result), 1u);
    // hud_assert_eq(std::get<6>(result), 0u);
    // }
    //}

    // With extra
    // {
    //     hud_test::for_each_value<std::make_integer_sequence<usize, 5>>()([]<usize extra>()
    //                                                                      {
    //              // Test with default allocator
    //              {
    //                  auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
    //                  {
    //                      const CopiedType copied(initializer, copied_extra);

    // // Copy the array
    // hud::array<type, AllocatorType> copy(copied, extra);

    // // Ensure we copy all datas in order
    // bool all_values_copied = true;
    // for (usize index = 0; index < initializer.size(); index++)
    // {
    //     if (copy[index] != static_cast<type>(index))
    //     {
    //         all_values_copied = false;
    //         break;
    //     }
    // }

    // return std::tuple {
    //     // Ensure we copy all datas in order
    //     copy.data() != nullptr,
    //     copy.count(),
    //     copy.max_count(),
    //     all_values_copied,

    // // Ensure the copy data is not the same memory of the copied data
    // copied.data() != copy.data(),

    // // Ensure we are allocating only one time
    // copy.allocator().allocation_count(),
    // copy.allocator().free_count()
    // };
    // };

    // // Non constant
    // {
    //     const auto result = test_default_allocator({0, 1, 2, 3}, 1u);
    //     // Ensure we copy all datas in order
    //     hud_assert_true(std::get<0>(result));
    //     hud_assert_eq(std::get<1>(result), 4u);
    //     hud_assert_eq(std::get<2>(result), 4u + 1u + extra);
    //     hud_assert_true(std::get<3>(result));

    // // Ensure the copy data is not the same memory of the copied data
    // hud_assert_true(std::get<4>(result));

    // // Ensure we are allocating only one time
    // hud_assert_eq(std::get<5>(result), 2u);
    // hud_assert_eq(std::get<6>(result), 0u);
    // }

    // // Constant
    // {
    //     constexpr auto result = test_default_allocator({0, 1, 2, 3}, 1u);
    //     // Ensure we copy all datas in order
    //     hud_assert_true(std::get<0>(result));
    //     hud_assert_eq(std::get<1>(result), 4u);
    //     hud_assert_eq(std::get<2>(result), 4u + 1u + extra);
    //     hud_assert_true(std::get<3>(result));

    // // Ensure the copy data is not the same memory of the copied data
    // hud_assert_true(std::get<4>(result));

    // // Ensure we are allocating only one time
    // hud_assert_eq(std::get<5>(result), 2u);
    // hud_assert_eq(std::get<6>(result), 0u);
    // }
    // }

    // // Test with allocator
    // {
    //     auto test_with_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
    //     {
    //         const CopiedType copied(initializer, copied_extra);

    // // Copy the array
    // hud::array<type, AllocatorType> copy(copied, extra, AllocatorType {});

    // // Ensure we copy all datas in order
    // bool all_values_copied = true;
    // for (usize index = 0; index < initializer.size(); index++)
    // {
    //     if (copy[index] != static_cast<type>(index))
    //     {
    //         all_values_copied = false;
    //         break;
    //     }
    // }

    // return std::tuple {
    //     // Ensure we copy all datas in order
    //     copy.data() != nullptr,
    //     copy.count(),
    //     copy.max_count(),
    //     all_values_copied,

    // // Ensure the copy data is not the same memory of the copied data
    // copied.data() != copy.data(),

    // // Ensure we are allocating only one time
    // copy.allocator().allocation_count(),
    // copy.allocator().free_count()
    // };
    // };

    // // Non constant
    // {
    //     const auto result = test_with_allocator({0, 1, 2, 3}, 1u);
    //     // Ensure we copy all datas in order
    //     hud_assert_true(std::get<0>(result));
    //     hud_assert_eq(std::get<1>(result), 4u);
    //     hud_assert_eq(std::get<2>(result), 4u + 1u + extra);
    //     hud_assert_true(std::get<3>(result));

    // // Ensure the copy data is not the same memory of the copied data
    // hud_assert_true(std::get<4>(result));

    // // Ensure we are allocating only one time
    // hud_assert_eq(std::get<5>(result), 1u);
    // hud_assert_eq(std::get<6>(result), 0u);
    // }

    // // Constant
    // {
    //     constexpr auto result = test_with_allocator({0, 1, 2, 3}, 1u);
    //     // Ensure we copy all datas in order
    //     hud_assert_true(std::get<0>(result));
    //     hud_assert_eq(std::get<1>(result), 4u);
    //     hud_assert_eq(std::get<2>(result), 4u + 1u + extra);
    //     hud_assert_true(std::get<3>(result));

    // // Ensure the copy data is not the same memory of the copied data
    // hud_assert_true(std::get<4>(result));

    // // Ensure we are allocating only one time
    // hud_assert_eq(std::get<5>(result), 1u);
    // hud_assert_eq(std::get<6>(result), 0u);
    // }
    // } });
    // }
}

GTEST_TEST(hashmap, copy_construct_bitwise_copy_constructible_same_type_different_allocator)
{
}

GTEST_TEST(hashmap, copy_construct_bitwise_copy_constructible_different_type_same_allocator)
{
}

GTEST_TEST(hashmap, copy_construct_bitwise_copy_constructible_different_type_different_allocator)
{
}

GTEST_TEST(hashmap, copy_construct_non_bitwise_copy_constructible_same_type_same_allocator)
{
}

GTEST_TEST(hashmap, copy_construct_non_bitwise_copy_constructible_same_type_different_allocator)
{
}

GTEST_TEST(hashmap, copy_construct_non_bitwise_copy_constructible_different_type_same_allocator)
{
}

GTEST_TEST(hashmap, copy_construct_non_bitwise_copy_constructible_different_type_different_allocator)
{
}