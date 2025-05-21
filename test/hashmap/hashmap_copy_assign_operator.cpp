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

#define TEST_VALUES2 { \
    { 8,  88},           \
    { 9,  99},           \
    {10, 100},         \
    {11, 110},         \
    {12, 120},         \
    {13, 130},         \
 }

GTEST_TEST(hashmap, copy_assign_hashmap_of_bitwise_copy_constructible_same_type_same_allocator)
{
    using key_type = i32;
    using value_type = u64;

    static_assert(hud::is_bitwise_copy_assignable_v<key_type>);
    static_assert(hud::is_bitwise_copy_assignable_v<value_type>);

    // Test without extra
    {
        const auto test = [](std::initializer_list<hud::pair<key_type, value_type>> elements_in_assigned, std::initializer_list<hud::pair<key_type, value_type>> elements_to_assign)
        {
            using AllocatorType = hud_test::allocator_watcher<1>;
            using AssignedType = hud::hashmap<key_type, value_type, hud::hashmap_default_hasher, hud::hashmap_default_key_equal<key_type>, AllocatorType>;
            using ToAssignType = hud::hashmap<key_type, value_type, hud::hashmap_default_hasher, hud::hashmap_default_key_equal<key_type>, AllocatorType>;
            AssignedType assigned(elements_in_assigned);
            ToAssignType to_assign(elements_to_assign);

            assigned = to_assign;

            // Ensure we copy all elements
            bool all_keys_and_values_copied = true;
            for (usize index = 0; index < elements_to_assign.size(); index++)
            {
                const auto &init_elem = (elements_to_assign.begin() + index);
                const auto it = assigned.find(init_elem->first);
                if (it == assigned.end())
                {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->key() != init_elem->first)
                {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->value() != init_elem->second)
                {
                    all_keys_and_values_copied = false;
                    break;
                }
            }
            // Allocation count
            u32 expected_allocation_count = 0;
            bool assigned_allocate = elements_in_assigned.size() > 0;
            bool assigned_should_grow = elements_in_assigned.size() < elements_to_assign.size();
            if (assigned_allocate)
            {
                expected_allocation_count++;
            }
            if (assigned_should_grow)
            {
                expected_allocation_count++;
            }
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if (hud::is_constant_evaluated())
            {
                expected_allocation_count *= 2;
            }

            u32 expected_free_count = 0;
            if (assigned_allocate && assigned_should_grow)
            {
                expected_free_count++;
            }
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if (hud::is_constant_evaluated())
            {
                expected_free_count *= 2;
            }
            return std::tuple {
                all_keys_and_values_copied,                                           // 0
                assigned.count() == elements_to_assign.size(),                        // 1
                assigned.max_count() >= elements_to_assign.size(),                    // 2
                assigned.allocator().allocation_count() == expected_allocation_count, // 3
                assigned.allocator().free_count() == expected_free_count              // 4
            };
        };

        // Non constant
        {
            {
                const auto result = test({}, {});
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, {});
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test({}, TEST_VALUES2);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, TEST_VALUES2);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, TEST_VALUES);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES2, TEST_VALUES);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
        }

        // Constant
        {
            {
                constexpr auto result = test({}, {});
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, {});
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test({}, TEST_VALUES2);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, TEST_VALUES2);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, TEST_VALUES);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES2, TEST_VALUES);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
        }
    }

    // Test with extra
    {
        const auto test = [](std::initializer_list<hud::pair<key_type, value_type>> elements_in_assigned, usize extra_assigned, std::initializer_list<hud::pair<key_type, value_type>> elements_to_assign, usize extra_to_assign)
        {
            using AllocatorType = hud_test::allocator_watcher<1>;
            using AssignedType = hud::hashmap<key_type, value_type, hud::hashmap_default_hasher, hud::hashmap_default_key_equal<key_type>, AllocatorType>;
            using ToAssignType = hud::hashmap<key_type, value_type, hud::hashmap_default_hasher, hud::hashmap_default_key_equal<key_type>, AllocatorType>;
            AssignedType assigned(elements_in_assigned, extra_assigned);
            ToAssignType to_assign(elements_to_assign, extra_to_assign);

            assigned = to_assign;

            // Ensure we copy all elements
            bool all_keys_and_values_copied = true;
            for (usize index = 0; index < elements_to_assign.size(); index++)
            {
                const auto &init_elem = (elements_to_assign.begin() + index);
                const auto it = assigned.find(init_elem->first);
                if (it == assigned.end())
                {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->key() != init_elem->first)
                {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->value() != init_elem->second)
                {
                    all_keys_and_values_copied = false;
                    break;
                }
            }
            // Allocation count
            u32 expected_allocation_count = 0;
            bool assigned_allocate = elements_in_assigned.size() > 0 || extra_assigned > 0;
            bool assigned_should_grow = elements_in_assigned.size() < elements_to_assign.size();
            if (assigned_allocate)
            {
                expected_allocation_count++;
            }
            if (assigned_should_grow)
            {
                expected_allocation_count++;
            }
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if (hud::is_constant_evaluated())
            {
                expected_allocation_count *= 2;
            }

            u32 expected_free_count = 0;
            if (assigned_allocate && assigned_should_grow)
            {
                expected_free_count++;
            }
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if (hud::is_constant_evaluated())
            {
                expected_free_count *= 2;
            }
            return std::tuple {
                all_keys_and_values_copied,                                           // 0
                assigned.count() == elements_to_assign.size(),                        // 1
                assigned.max_count() >= elements_to_assign.size(),                    // 2
                assigned.allocator().allocation_count() == expected_allocation_count, // 3
                assigned.allocator().free_count() == expected_free_count              // 4
            };
        };

        // Non constant
        {
            {
                const auto result = test({}, 0, {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test({}, 0, {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test({}, 1, {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test({}, 1, {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                const auto result = test(TEST_VALUES, 0, {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, 0, {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, 1, {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, 1, {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                const auto result = test({}, 0, TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test({}, 0, TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test({}, 1, TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test({}, 1, TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                const auto result = test(TEST_VALUES, 0, TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, 0, TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, 1, TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, 1, TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                const auto result = test(TEST_VALUES, 0, TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, 0, TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, 1, TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, 1, TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                const auto result = test(TEST_VALUES2, 0, TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES2, 0, TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES2, 1, TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES2, 1, TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
        }

        // Constant
        {
            {
                constexpr auto result = test({}, 0, {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test({}, 0, {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test({}, 1, {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test({}, 1, {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                constexpr auto result = test(TEST_VALUES, 0, {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, 0, {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, 1, {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, 1, {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                constexpr auto result = test({}, 0, TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test({}, 0, TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test({}, 1, TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test({}, 1, TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                constexpr auto result = test(TEST_VALUES, 0, TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, 0, TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, 1, TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, 1, TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                constexpr auto result = test(TEST_VALUES, 0, TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, 0, TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, 1, TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, 1, TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                constexpr auto result = test(TEST_VALUES2, 0, TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES2, 0, TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES2, 1, TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES2, 1, TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
        }
    }
}

GTEST_TEST(hashmap, copy_assign_hashmap_of_bitwise_copy_constructible_same_type_different_allocator)
{
    using key_type = i32;
    using value_type = u64;

    static_assert(hud::is_bitwise_copy_assignable_v<key_type>);
    static_assert(hud::is_bitwise_copy_assignable_v<value_type>);

    // Test without extra
    {
        const auto test = [](std::initializer_list<hud::pair<key_type, value_type>> elements_in_assigned, std::initializer_list<hud::pair<key_type, value_type>> elements_to_assign)
        {
            using AllocatorType = hud_test::allocator_watcher<1>;
            using AllocatorType2 = hud_test::allocator_watcher_2<1>;
            using AssignedType = hud::hashmap<key_type, value_type, hud::hashmap_default_hasher, hud::hashmap_default_key_equal<key_type>, AllocatorType>;
            using ToAssignType = hud::hashmap<key_type, value_type, hud::hashmap_default_hasher, hud::hashmap_default_key_equal<key_type>, AllocatorType2>;
            AssignedType assigned(elements_in_assigned);
            ToAssignType to_assign(elements_to_assign);

            assigned = to_assign;

            // Ensure we copy all elements
            bool all_keys_and_values_copied = true;
            for (usize index = 0; index < elements_to_assign.size(); index++)
            {
                const auto &init_elem = (elements_to_assign.begin() + index);
                const auto it = assigned.find(init_elem->first);
                if (it == assigned.end())
                {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->key() != init_elem->first)
                {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->value() != init_elem->second)
                {
                    all_keys_and_values_copied = false;
                    break;
                }
            }

            // Allocation count
            u32 expected_allocation_count = 0;
            bool to_assign_allocate = elements_to_assign.size() > 0;
            bool assigned_should_grow = elements_in_assigned.size() < elements_to_assign.size();
            if (to_assign_allocate)
            {
                expected_allocation_count++;
            }
            if (assigned_should_grow)
            {
                expected_allocation_count++;
            }
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if (hud::is_constant_evaluated())
            {
                expected_allocation_count *= 2;
            }

            u32 expected_free_count = 0;
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if (hud::is_constant_evaluated())
            {
                expected_free_count *= 2;
            }
            return std::tuple {
                all_keys_and_values_copied,                                           // 0
                assigned.count() == elements_to_assign.size(),                        // 1
                assigned.max_count() >= elements_to_assign.size(),                    // 2
                assigned.allocator().allocation_count() == expected_allocation_count, // 3
                assigned.allocator().free_count() == expected_free_count              // 4
            };
        };

        // Non constant
        {
            {
                const auto result = test({}, {});
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, {});
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test({}, TEST_VALUES2);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, TEST_VALUES2);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, TEST_VALUES);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES2, TEST_VALUES);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
        }

        // Constant
        {
            {
                constexpr auto result = test({}, {});
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, {});
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test({}, TEST_VALUES2);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, TEST_VALUES2);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, TEST_VALUES);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES2, TEST_VALUES);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
        }
    }

    // Test with extra
    {
        const auto test = [](std::initializer_list<hud::pair<key_type, value_type>> elements_in_assigned, usize extra_assigned, std::initializer_list<hud::pair<key_type, value_type>> elements_to_assign, usize extra_to_assign)
        {
            using AllocatorType = hud_test::allocator_watcher<1>;
            using AllocatorType2 = hud_test::allocator_watcher_2<1>;
            using AssignedType = hud::hashmap<key_type, value_type, hud::hashmap_default_hasher, hud::hashmap_default_key_equal<key_type>, AllocatorType>;
            using ToAssignType = hud::hashmap<key_type, value_type, hud::hashmap_default_hasher, hud::hashmap_default_key_equal<key_type>, AllocatorType2>;
            AssignedType assigned(elements_in_assigned, extra_assigned);
            ToAssignType to_assign(elements_to_assign, extra_to_assign);

            assigned = to_assign;

            // Ensure we copy all elements
            bool all_keys_and_values_copied = true;
            for (usize index = 0; index < elements_to_assign.size(); index++)
            {
                const auto &init_elem = (elements_to_assign.begin() + index);
                const auto it = assigned.find(init_elem->first);
                if (it == assigned.end())
                {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->key() != init_elem->first)
                {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->value() != init_elem->second)
                {
                    all_keys_and_values_copied = false;
                    break;
                }
            }
            // Allocation count
            u32 expected_allocation_count = 0;
            bool to_assign_allocate = elements_to_assign.size() > 0 || extra_to_assign > 0;
            bool assigned_should_grow = elements_in_assigned.size() < elements_to_assign.size();
            if (to_assign_allocate)
            {
                expected_allocation_count++;
            }
            if (assigned_should_grow)
            {
                expected_allocation_count++;
            }
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if (hud::is_constant_evaluated())
            {
                expected_allocation_count *= 2;
            }

            u32 expected_free_count = 0;
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if (hud::is_constant_evaluated())
            {
                expected_free_count *= 2;
            }
            return std::tuple {
                all_keys_and_values_copied,                                           // 0
                assigned.count() == elements_to_assign.size(),                        // 1
                assigned.max_count() >= elements_to_assign.size(),                    // 2
                assigned.allocator().allocation_count() == expected_allocation_count, // 3
                assigned.allocator().free_count() == expected_free_count              // 4
            };
        };

        // Non constant
        {
            {
                const auto result = test({}, 0, {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test({}, 0, {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test({}, 1, {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test({}, 1, {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                const auto result = test(TEST_VALUES, 0, {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, 0, {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, 1, {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, 1, {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                const auto result = test({}, 0, TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test({}, 0, TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test({}, 1, TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test({}, 1, TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                const auto result = test(TEST_VALUES, 0, TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, 0, TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, 1, TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, 1, TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                const auto result = test(TEST_VALUES, 0, TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, 0, TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, 1, TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, 1, TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                const auto result = test(TEST_VALUES2, 0, TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES2, 0, TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES2, 1, TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES2, 1, TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
        }

        // Constant
        {
            {
                constexpr auto result = test({}, 0, {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test({}, 0, {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test({}, 1, {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test({}, 1, {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                constexpr auto result = test(TEST_VALUES, 0, {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, 0, {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, 1, {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, 1, {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                constexpr auto result = test({}, 0, TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test({}, 0, TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test({}, 1, TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test({}, 1, TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                constexpr auto result = test(TEST_VALUES, 0, TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, 0, TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, 1, TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, 1, TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                constexpr auto result = test(TEST_VALUES, 0, TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, 0, TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, 1, TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, 1, TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                constexpr auto result = test(TEST_VALUES2, 0, TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES2, 0, TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES2, 1, TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES2, 1, TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
        }
    }
}

GTEST_TEST(hashmap, copy_assign_hashmap_of_bitwise_copy_constructible_different_type_same_allocator)
{
    using key_type = i32;
    using value_type = u64;
    using key_type_2 = i128;
    using value_type_2 = u128;

    static_assert(hud::is_bitwise_copy_assignable_v<key_type>);
    static_assert(hud::is_bitwise_copy_assignable_v<value_type>);
    static_assert(hud::is_bitwise_copy_assignable_v<key_type_2>);
    static_assert(hud::is_bitwise_copy_assignable_v<value_type_2>);

    // Test without extra
    {
        const auto test = [](std::initializer_list<hud::pair<key_type, value_type>> elements_in_assigned, std::initializer_list<hud::pair<key_type_2, value_type_2>> elements_to_assign)
        {
            using AllocatorType = hud_test::allocator_watcher<1>;
            using AssignedType = hud::hashmap<key_type, value_type, hud::hashmap_default_hasher, hud::hashmap_default_key_equal<key_type>, AllocatorType>;
            using ToAssignType = hud::hashmap<key_type_2, value_type_2, hud::hashmap_default_hasher, hud::hashmap_default_key_equal<key_type_2>, AllocatorType>;
            AssignedType assigned(elements_in_assigned);
            ToAssignType to_assign(elements_to_assign);

            assigned = to_assign;

            // Ensure we copy all elements
            bool all_keys_and_values_copied = true;
            for (usize index = 0; index < elements_to_assign.size(); index++)
            {
                const auto &init_elem = (elements_to_assign.begin() + index);
                const auto it = assigned.find(static_cast<key_type>(init_elem->first));
                if (it == assigned.end())
                {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->key() != static_cast<key_type>(init_elem->first))
                {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->value() != static_cast<key_type>(init_elem->second))
                {
                    all_keys_and_values_copied = false;
                    break;
                }
            }

            // Allocation count
            u32 expected_allocation_count = 0;
            bool assigned_allocate = elements_in_assigned.size() > 0;
            bool assigned_should_grow = elements_in_assigned.size() < elements_to_assign.size();
            if (assigned_allocate)
            {
                expected_allocation_count++;
            }
            if (assigned_should_grow)
            {
                expected_allocation_count++;
            }
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if (hud::is_constant_evaluated())
            {
                expected_allocation_count *= 2;
            }

            u32 expected_free_count = 0;
            if (assigned_allocate && assigned_should_grow)
            {
                expected_free_count++;
            }
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if (hud::is_constant_evaluated())
            {
                expected_free_count *= 2;
            }
            return std::tuple {
                all_keys_and_values_copied,                                           // 0
                assigned.count() == elements_to_assign.size(),                        // 1
                assigned.max_count() >= elements_to_assign.size(),                    // 2
                assigned.allocator().allocation_count() == expected_allocation_count, // 3
                assigned.allocator().free_count() == expected_free_count              // 4
            };
        };

        // Non constant
        {
            {
                const auto result = test({}, {});
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, {});
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test({}, TEST_VALUES2);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, TEST_VALUES2);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, TEST_VALUES);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES2, TEST_VALUES);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
        }

        // Constant
        {
            {
                constexpr auto result = test({}, {});
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, {});
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test({}, TEST_VALUES2);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, TEST_VALUES2);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, TEST_VALUES);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES2, TEST_VALUES);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
        }
    }

    // Test with extra
    {
        const auto test = [](std::initializer_list<hud::pair<key_type, value_type>> elements_in_assigned, usize extra_assigned, std::initializer_list<hud::pair<key_type_2, value_type_2>> elements_to_assign, usize extra_to_assign)
        {
            using AllocatorType = hud_test::allocator_watcher<1>;
            using AssignedType = hud::hashmap<key_type, value_type, hud::hashmap_default_hasher, hud::hashmap_default_key_equal<key_type>, AllocatorType>;
            using ToAssignType = hud::hashmap<key_type_2, value_type_2, hud::hashmap_default_hasher, hud::hashmap_default_key_equal<key_type_2>, AllocatorType>;
            AssignedType assigned(elements_in_assigned, extra_assigned);
            ToAssignType to_assign(elements_to_assign, extra_to_assign);

            assigned = to_assign;

            // Ensure we copy all elements
            bool all_keys_and_values_copied = true;
            for (usize index = 0; index < elements_to_assign.size(); index++)
            {
                const auto &init_elem = (elements_to_assign.begin() + index);
                const auto it = assigned.find(static_cast<key_type>(init_elem->first));
                if (it == assigned.end())
                {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->key() != static_cast<key_type>(init_elem->first))
                {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->value() != static_cast<key_type>(init_elem->second))
                {
                    all_keys_and_values_copied = false;
                    break;
                }
            }
            // Allocation count
            u32 expected_allocation_count = 0;
            bool assigned_allocate = elements_in_assigned.size() > 0 || extra_assigned > 0;
            bool assigned_should_grow = elements_in_assigned.size() < elements_to_assign.size();
            if (assigned_allocate)
            {
                expected_allocation_count++;
            }
            if (assigned_should_grow)
            {
                expected_allocation_count++;
            }
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if (hud::is_constant_evaluated())
            {
                expected_allocation_count *= 2;
            }

            u32 expected_free_count = 0;
            if (assigned_allocate && assigned_should_grow)
            {
                expected_free_count++;
            }
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if (hud::is_constant_evaluated())
            {
                expected_free_count *= 2;
            }
            return std::tuple {
                all_keys_and_values_copied,                                           // 0
                assigned.count() == elements_to_assign.size(),                        // 1
                assigned.max_count() >= elements_to_assign.size(),                    // 2
                assigned.allocator().allocation_count() == expected_allocation_count, // 3
                assigned.allocator().free_count() == expected_free_count              // 4
            };
        };

        // Non constant
        {
            {
                const auto result = test({}, 0, {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test({}, 0, {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test({}, 1, {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test({}, 1, {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                const auto result = test(TEST_VALUES, 0, {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, 0, {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, 1, {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, 1, {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                const auto result = test({}, 0, TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test({}, 0, TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test({}, 1, TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test({}, 1, TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                const auto result = test(TEST_VALUES, 0, TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, 0, TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, 1, TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, 1, TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                const auto result = test(TEST_VALUES, 0, TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, 0, TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, 1, TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, 1, TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                const auto result = test(TEST_VALUES2, 0, TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES2, 0, TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES2, 1, TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES2, 1, TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
        }

        // Constant
        {
            {
                constexpr auto result = test({}, 0, {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test({}, 0, {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test({}, 1, {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test({}, 1, {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                constexpr auto result = test(TEST_VALUES, 0, {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, 0, {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, 1, {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, 1, {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                constexpr auto result = test({}, 0, TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test({}, 0, TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test({}, 1, TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test({}, 1, TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                constexpr auto result = test(TEST_VALUES, 0, TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, 0, TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, 1, TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, 1, TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                constexpr auto result = test(TEST_VALUES, 0, TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, 0, TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, 1, TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, 1, TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                constexpr auto result = test(TEST_VALUES2, 0, TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES2, 0, TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES2, 1, TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES2, 1, TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
        }
    }
}

GTEST_TEST(hashmap, copy_assign_hashmap_of_bitwise_copy_constructible_different_type_different_allocator)
{
    using key_type = i32;
    using value_type = u64;
    using key_type_2 = i128;
    using value_type_2 = u128;

    static_assert(hud::is_bitwise_copy_assignable_v<key_type>);
    static_assert(hud::is_bitwise_copy_assignable_v<value_type>);
    static_assert(hud::is_bitwise_copy_assignable_v<key_type_2>);
    static_assert(hud::is_bitwise_copy_assignable_v<value_type_2>);

    // Test without extra
    {
        const auto test = [](std::initializer_list<hud::pair<key_type, value_type>> elements_in_assigned, std::initializer_list<hud::pair<key_type_2, value_type_2>> elements_to_assign)
        {
            using AllocatorType = hud_test::allocator_watcher<1>;
            using AllocatorType2 = hud_test::allocator_watcher_2<1>;
            using AssignedType = hud::hashmap<key_type, value_type, hud::hashmap_default_hasher, hud::hashmap_default_key_equal<key_type>, AllocatorType>;
            using ToAssignType = hud::hashmap<key_type_2, value_type_2, hud::hashmap_default_hasher, hud::hashmap_default_key_equal<key_type_2>, AllocatorType2>;
            AssignedType assigned(elements_in_assigned);
            ToAssignType to_assign(elements_to_assign);

            assigned = to_assign;

            // Ensure we copy all elements
            bool all_keys_and_values_copied = true;
            for (usize index = 0; index < elements_to_assign.size(); index++)
            {
                const auto &init_elem = (elements_to_assign.begin() + index);
                const auto it = assigned.find(static_cast<key_type>(init_elem->first));
                if (it == assigned.end())
                {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->key() != static_cast<key_type>(init_elem->first))
                {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->value() != static_cast<key_type>(init_elem->second))
                {
                    all_keys_and_values_copied = false;
                    break;
                }
            }
            // Allocation count
            u32 expected_allocation_count = 0;
            bool to_assign_allocate = elements_to_assign.size() > 0;
            bool assigned_should_grow = elements_in_assigned.size() < elements_to_assign.size();
            if (to_assign_allocate)
            {
                expected_allocation_count++;
            }
            if (assigned_should_grow)
            {
                expected_allocation_count++;
            }
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if (hud::is_constant_evaluated())
            {
                expected_allocation_count *= 2;
            }

            u32 expected_free_count = 0;
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if (hud::is_constant_evaluated())
            {
                expected_free_count *= 2;
            }
            return std::tuple {
                all_keys_and_values_copied,                                           // 0
                assigned.count() == elements_to_assign.size(),                        // 1
                assigned.max_count() >= elements_to_assign.size(),                    // 2
                assigned.allocator().allocation_count() == expected_allocation_count, // 3
                assigned.allocator().free_count() == expected_free_count              // 4
            };
        };

        // Non constant
        {
            {
                const auto result = test({}, {});
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, {});
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test({}, TEST_VALUES2);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, TEST_VALUES2);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, TEST_VALUES);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES2, TEST_VALUES);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
        }

        // Constant
        {
            {
                constexpr auto result = test({}, {});
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, {});
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test({}, TEST_VALUES2);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, TEST_VALUES2);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, TEST_VALUES);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES2, TEST_VALUES);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
        }
    }

    // Test with extra
    {
        const auto test = [](std::initializer_list<hud::pair<key_type, value_type>> elements_in_assigned, usize extra_assigned, std::initializer_list<hud::pair<key_type_2, value_type_2>> elements_to_assign, usize extra_to_assign)
        {
            using AllocatorType = hud_test::allocator_watcher<1>;
            using AllocatorType2 = hud_test::allocator_watcher_2<1>;
            using AssignedType = hud::hashmap<key_type, value_type, hud::hashmap_default_hasher, hud::hashmap_default_key_equal<key_type>, AllocatorType>;
            using ToAssignType = hud::hashmap<key_type_2, value_type_2, hud::hashmap_default_hasher, hud::hashmap_default_key_equal<key_type_2>, AllocatorType2>;
            AssignedType assigned(elements_in_assigned, extra_assigned);
            ToAssignType to_assign(elements_to_assign, extra_to_assign);

            assigned = to_assign;

            // Ensure we copy all elements
            bool all_keys_and_values_copied = true;
            for (usize index = 0; index < elements_to_assign.size(); index++)
            {
                const auto &init_elem = (elements_to_assign.begin() + index);
                const auto it = assigned.find(static_cast<key_type>(init_elem->first));
                if (it == assigned.end())
                {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->key() != static_cast<key_type>(init_elem->first))
                {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->value() != static_cast<key_type>(init_elem->second))
                {
                    all_keys_and_values_copied = false;
                    break;
                }
            }
            // Allocation count
            u32 expected_allocation_count = 0;
            bool to_assign_allocate = elements_to_assign.size() > 0 || extra_to_assign > 0;
            bool assigned_should_grow = elements_in_assigned.size() < elements_to_assign.size();
            if (to_assign_allocate)
            {
                expected_allocation_count++;
            }
            if (assigned_should_grow)
            {
                expected_allocation_count++;
            }
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if (hud::is_constant_evaluated())
            {
                expected_allocation_count *= 2;
            }

            u32 expected_free_count = 0;
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if (hud::is_constant_evaluated())
            {
                expected_free_count *= 2;
            }
            return std::tuple {
                all_keys_and_values_copied,                                           // 0
                assigned.count() == elements_to_assign.size(),                        // 1
                assigned.max_count() >= elements_to_assign.size(),                    // 2
                assigned.allocator().allocation_count() == expected_allocation_count, // 3
                assigned.allocator().free_count() == expected_free_count              // 4
            };
        };

        // Non constant
        {
            {
                const auto result = test({}, 0, {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test({}, 0, {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test({}, 1, {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test({}, 1, {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                const auto result = test(TEST_VALUES, 0, {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, 0, {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, 1, {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, 1, {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                const auto result = test({}, 0, TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test({}, 0, TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test({}, 1, TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test({}, 1, TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                const auto result = test(TEST_VALUES, 0, TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, 0, TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, 1, TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, 1, TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                const auto result = test(TEST_VALUES, 0, TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, 0, TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, 1, TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, 1, TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                const auto result = test(TEST_VALUES2, 0, TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES2, 0, TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES2, 1, TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES2, 1, TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
        }

        // Constant
        {
            {
                constexpr auto result = test({}, 0, {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test({}, 0, {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test({}, 1, {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test({}, 1, {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                constexpr auto result = test(TEST_VALUES, 0, {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, 0, {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, 1, {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, 1, {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                constexpr auto result = test({}, 0, TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test({}, 0, TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test({}, 1, TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test({}, 1, TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                constexpr auto result = test(TEST_VALUES, 0, TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, 0, TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, 1, TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, 1, TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                constexpr auto result = test(TEST_VALUES, 0, TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, 0, TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, 1, TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, 1, TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                constexpr auto result = test(TEST_VALUES2, 0, TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES2, 0, TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES2, 1, TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES2, 1, TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
        }
    }
}

GTEST_TEST(hashmap, copy_assign_hashmap_of_non_bitwise_copy_constructible_same_type_same_allocator)
{
    using key_type = hud_test::non_bitwise_copy_constructible_type;
    using value_type = hud_test::non_bitwise_copy_constructible_type2;

    static_assert(!hud::is_bitwise_copy_constructible_v<key_type>);
    static_assert(!hud::is_bitwise_copy_constructible_v<value_type>);

    // Test without extra
    {
        const auto test = [](std::initializer_list<hud::pair<key_type, value_type>> elements_in_assigned, std::initializer_list<hud::pair<key_type, value_type>> elements_to_assign)
        {
            using AllocatorType = hud_test::allocator_watcher<1>;
            using AssignedType = hud::hashmap<key_type, value_type, hud::hashmap_default_hasher, hud::hashmap_default_key_equal<key_type>, AllocatorType>;
            using ToAssignType = hud::hashmap<key_type, value_type, hud::hashmap_default_hasher, hud::hashmap_default_key_equal<key_type>, AllocatorType>;
            AssignedType assigned(elements_in_assigned);
            ToAssignType to_assign(elements_to_assign);

            assigned = to_assign;

            // Ensure we copy all elements
            bool all_keys_and_values_copied = true;
            for (usize index = 0; index < elements_to_assign.size(); index++)
            {
                const auto &init_elem = (elements_to_assign.begin() + index);
                const auto it = assigned.find(init_elem->first);
                if (it == assigned.end())
                {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->key() != init_elem->first)
                {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->key().copy_constructor_count() != 2)
                {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->value() != init_elem->second)
                {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->value().copy_constructor_count() != 2)
                {
                    all_keys_and_values_copied = false;
                    break;
                }
            }
            // Allocation count
            u32 expected_allocation_count = 0;
            bool assigned_allocate = elements_in_assigned.size() > 0;
            bool assigned_should_grow = elements_in_assigned.size() < elements_to_assign.size();
            if (assigned_allocate)
            {
                expected_allocation_count++;
            }
            if (assigned_should_grow)
            {
                expected_allocation_count++;
            }
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if (hud::is_constant_evaluated())
            {
                expected_allocation_count *= 2;
            }

            u32 expected_free_count = 0;
            if (assigned_allocate && assigned_should_grow)
            {
                expected_free_count++;
            }
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if (hud::is_constant_evaluated())
            {
                expected_free_count *= 2;
            }
            return std::tuple {
                all_keys_and_values_copied,                                           // 0
                assigned.count() == elements_to_assign.size(),                        // 1
                assigned.max_count() >= elements_to_assign.size(),                    // 2
                assigned.allocator().allocation_count() == expected_allocation_count, // 3
                assigned.allocator().free_count() == expected_free_count              // 4
            };
        };

        // Non constant
        {
            {
                const auto result = test({}, {});
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, {});
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test({}, TEST_VALUES2);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, TEST_VALUES2);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, TEST_VALUES);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES2, TEST_VALUES);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
        }

        // Constant
        {
            {
                constexpr auto result = test({}, {});
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, {});
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test({}, TEST_VALUES2);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, TEST_VALUES2);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, TEST_VALUES);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES2, TEST_VALUES);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
        }
    }

    // Test with extra
    {
        const auto test = [](std::initializer_list<hud::pair<key_type, value_type>> elements_in_assigned, usize extra_assigned, std::initializer_list<hud::pair<key_type, value_type>> elements_to_assign, usize extra_to_assign)
        {
            using AllocatorType = hud_test::allocator_watcher<1>;
            using AssignedType = hud::hashmap<key_type, value_type, hud::hashmap_default_hasher, hud::hashmap_default_key_equal<key_type>, AllocatorType>;
            using ToAssignType = hud::hashmap<key_type, value_type, hud::hashmap_default_hasher, hud::hashmap_default_key_equal<key_type>, AllocatorType>;
            AssignedType assigned(elements_in_assigned, extra_assigned);
            ToAssignType to_assign(elements_to_assign, extra_to_assign);

            assigned = to_assign;

            // Ensure we copy all elements
            bool all_keys_and_values_copied = true;
            for (usize index = 0; index < elements_to_assign.size(); index++)
            {
                const auto &init_elem = (elements_to_assign.begin() + index);
                const auto it = assigned.find(init_elem->first);
                if (it == assigned.end())
                {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->key() != init_elem->first)
                {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->key().copy_constructor_count() != 2)
                {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->value() != init_elem->second)
                {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->value().copy_constructor_count() != 2)
                {
                    all_keys_and_values_copied = false;
                    break;
                }
            }
            // Allocation count
            u32 expected_allocation_count = 0;
            bool assigned_allocate = elements_in_assigned.size() > 0 || extra_assigned > 0;
            bool assigned_should_grow = elements_in_assigned.size() < elements_to_assign.size();
            if (assigned_allocate)
            {
                expected_allocation_count++;
            }
            if (assigned_should_grow)
            {
                expected_allocation_count++;
            }
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if (hud::is_constant_evaluated())
            {
                expected_allocation_count *= 2;
            }

            u32 expected_free_count = 0;
            if (assigned_allocate && assigned_should_grow)
            {
                expected_free_count++;
            }
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if (hud::is_constant_evaluated())
            {
                expected_free_count *= 2;
            }
            return std::tuple {
                all_keys_and_values_copied,                                           // 0
                assigned.count() == elements_to_assign.size(),                        // 1
                assigned.max_count() >= elements_to_assign.size(),                    // 2
                assigned.allocator().allocation_count() == expected_allocation_count, // 3
                assigned.allocator().free_count() == expected_free_count              // 4
            };
        };

        // Non constant
        {
            {
                const auto result = test({}, 0, {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test({}, 0, {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test({}, 1, {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test({}, 1, {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                const auto result = test(TEST_VALUES, 0, {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, 0, {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, 1, {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, 1, {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                const auto result = test({}, 0, TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test({}, 0, TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test({}, 1, TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test({}, 1, TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                const auto result = test(TEST_VALUES, 0, TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, 0, TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, 1, TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, 1, TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                const auto result = test(TEST_VALUES, 0, TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, 0, TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, 1, TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, 1, TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                const auto result = test(TEST_VALUES2, 0, TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES2, 0, TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES2, 1, TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES2, 1, TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
        }

        // Constant
        {
            {
                constexpr auto result = test({}, 0, {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test({}, 0, {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test({}, 1, {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test({}, 1, {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                constexpr auto result = test(TEST_VALUES, 0, {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, 0, {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, 1, {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, 1, {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                constexpr auto result = test({}, 0, TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test({}, 0, TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test({}, 1, TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test({}, 1, TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                constexpr auto result = test(TEST_VALUES, 0, TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, 0, TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, 1, TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, 1, TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                constexpr auto result = test(TEST_VALUES, 0, TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, 0, TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, 1, TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, 1, TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                constexpr auto result = test(TEST_VALUES2, 0, TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES2, 0, TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES2, 1, TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES2, 1, TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
        }
    }
}

GTEST_TEST(hashmap, copy_assign_hashmap_of_non_bitwise_copy_constructible_same_type_different_allocator)
{
    using key_type = hud_test::non_bitwise_copy_constructible_type;
    using value_type = hud_test::non_bitwise_copy_constructible_type2;

    static_assert(!hud::is_bitwise_copy_constructible_v<key_type>);
    static_assert(!hud::is_bitwise_copy_constructible_v<value_type>);

    // Test without extra
    {
        const auto test = [](std::initializer_list<hud::pair<key_type, value_type>> elements_in_assigned, std::initializer_list<hud::pair<key_type, value_type>> elements_to_assign)
        {
            using AllocatorType = hud_test::allocator_watcher<1>;
            using AllocatorType2 = hud_test::allocator_watcher_2<1>;
            using AssignedType = hud::hashmap<key_type, value_type, hud::hashmap_default_hasher, hud::hashmap_default_key_equal<key_type>, AllocatorType>;
            using ToAssignType = hud::hashmap<key_type, value_type, hud::hashmap_default_hasher, hud::hashmap_default_key_equal<key_type>, AllocatorType2>;
            AssignedType assigned(elements_in_assigned);
            ToAssignType to_assign(elements_to_assign);

            assigned = to_assign;

            // Ensure we copy all elements
            bool all_keys_and_values_copied = true;
            for (usize index = 0; index < elements_to_assign.size(); index++)
            {
                const auto &init_elem = (elements_to_assign.begin() + index);
                const auto it = assigned.find(init_elem->first);
                if (it == assigned.end())
                {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->key() != init_elem->first)
                {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->key().copy_constructor_count() != 2)
                {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->value() != init_elem->second)
                {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->value().copy_constructor_count() != 2)
                {
                    all_keys_and_values_copied = false;
                    break;
                }
            }

            // Allocation count
            u32 expected_allocation_count = 0;
            bool to_assign_allocate = elements_to_assign.size() > 0;
            bool assigned_should_grow = elements_in_assigned.size() < elements_to_assign.size();
            if (to_assign_allocate)
            {
                expected_allocation_count++;
            }
            if (assigned_should_grow)
            {
                expected_allocation_count++;
            }
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if (hud::is_constant_evaluated())
            {
                expected_allocation_count *= 2;
            }

            u32 expected_free_count = 0;
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if (hud::is_constant_evaluated())
            {
                expected_free_count *= 2;
            }
            return std::tuple {
                all_keys_and_values_copied,                                           // 0
                assigned.count() == elements_to_assign.size(),                        // 1
                assigned.max_count() >= elements_to_assign.size(),                    // 2
                assigned.allocator().allocation_count() == expected_allocation_count, // 3
                assigned.allocator().free_count() == expected_free_count              // 4
            };
        };

        // Non constant
        {
            {
                const auto result = test({}, {});
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, {});
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test({}, TEST_VALUES2);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, TEST_VALUES2);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, TEST_VALUES);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES2, TEST_VALUES);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
        }

        // Constant
        {
            {
                constexpr auto result = test({}, {});
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, {});
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test({}, TEST_VALUES2);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, TEST_VALUES2);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, TEST_VALUES);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES2, TEST_VALUES);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
        }
    }

    // Test with extra
    {
        const auto test = [](std::initializer_list<hud::pair<key_type, value_type>> elements_in_assigned, usize extra_assigned, std::initializer_list<hud::pair<key_type, value_type>> elements_to_assign, usize extra_to_assign)
        {
            using AllocatorType = hud_test::allocator_watcher<1>;
            using AllocatorType2 = hud_test::allocator_watcher_2<1>;
            using AssignedType = hud::hashmap<key_type, value_type, hud::hashmap_default_hasher, hud::hashmap_default_key_equal<key_type>, AllocatorType>;
            using ToAssignType = hud::hashmap<key_type, value_type, hud::hashmap_default_hasher, hud::hashmap_default_key_equal<key_type>, AllocatorType2>;
            AssignedType assigned(elements_in_assigned, extra_assigned);
            ToAssignType to_assign(elements_to_assign, extra_to_assign);

            assigned = to_assign;

            // Ensure we copy all elements
            bool all_keys_and_values_copied = true;
            for (usize index = 0; index < elements_to_assign.size(); index++)
            {
                const auto &init_elem = (elements_to_assign.begin() + index);
                const auto it = assigned.find(init_elem->first);
                if (it == assigned.end())
                {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->key() != init_elem->first)
                {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->key().copy_constructor_count() != 2)
                {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->value() != init_elem->second)
                {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->value().copy_constructor_count() != 2)
                {
                    all_keys_and_values_copied = false;
                    break;
                }
            }
            // Allocation count
            u32 expected_allocation_count = 0;
            bool to_assign_allocate = elements_to_assign.size() > 0 || extra_to_assign > 0;
            bool assigned_should_grow = elements_in_assigned.size() < elements_to_assign.size();
            if (to_assign_allocate)
            {
                expected_allocation_count++;
            }
            if (assigned_should_grow)
            {
                expected_allocation_count++;
            }
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if (hud::is_constant_evaluated())
            {
                expected_allocation_count *= 2;
            }

            u32 expected_free_count = 0;
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if (hud::is_constant_evaluated())
            {
                expected_free_count *= 2;
            }
            return std::tuple {
                all_keys_and_values_copied,                                           // 0
                assigned.count() == elements_to_assign.size(),                        // 1
                assigned.max_count() >= elements_to_assign.size(),                    // 2
                assigned.allocator().allocation_count() == expected_allocation_count, // 3
                assigned.allocator().free_count() == expected_free_count              // 4
            };
        };

        // Non constant
        {
            {
                const auto result = test({}, 0, {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test({}, 0, {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test({}, 1, {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test({}, 1, {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                const auto result = test(TEST_VALUES, 0, {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, 0, {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, 1, {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, 1, {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                const auto result = test({}, 0, TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test({}, 0, TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test({}, 1, TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test({}, 1, TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                const auto result = test(TEST_VALUES, 0, TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, 0, TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, 1, TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, 1, TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                const auto result = test(TEST_VALUES, 0, TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, 0, TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, 1, TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, 1, TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                const auto result = test(TEST_VALUES2, 0, TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES2, 0, TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES2, 1, TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES2, 1, TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
        }

        // Constant
        {
            {
                constexpr auto result = test({}, 0, {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test({}, 0, {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test({}, 1, {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test({}, 1, {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                constexpr auto result = test(TEST_VALUES, 0, {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, 0, {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, 1, {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, 1, {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                constexpr auto result = test({}, 0, TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test({}, 0, TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test({}, 1, TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test({}, 1, TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                constexpr auto result = test(TEST_VALUES, 0, TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, 0, TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, 1, TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, 1, TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                constexpr auto result = test(TEST_VALUES, 0, TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, 0, TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, 1, TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, 1, TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                constexpr auto result = test(TEST_VALUES2, 0, TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES2, 0, TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES2, 1, TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES2, 1, TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
        }
    }
}

GTEST_TEST(hashmap, copy_assign_hashmap_of_non_bitwise_copy_constructible_different_type_same_allocator)
{
    using key_type = hud_test::non_bitwise_copy_constructible_type;
    using value_type = hud_test::non_bitwise_copy_constructible_type2;
    using key_type_2 = hud_test::non_bitwise_copy_constructible_type3;
    using value_type_2 = hud_test::non_bitwise_copy_constructible_type4;

    static_assert(!hud::is_bitwise_copy_constructible_v<key_type>);
    static_assert(!hud::is_bitwise_copy_constructible_v<value_type>);
    static_assert(!hud::is_bitwise_copy_constructible_v<key_type_2>);
    static_assert(!hud::is_bitwise_copy_constructible_v<value_type_2>);

    // Test without extra
    {
        const auto test = [](std::initializer_list<hud::pair<key_type, value_type>> elements_in_assigned, std::initializer_list<hud::pair<key_type_2, value_type_2>> elements_to_assign)
        {
            using AllocatorType = hud_test::allocator_watcher<1>;
            using AssignedType = hud::hashmap<key_type, value_type, hud::hashmap_default_hasher, hud::hashmap_default_key_equal<key_type>, AllocatorType>;
            using ToAssignType = hud::hashmap<key_type_2, value_type_2, hud::hashmap_default_hasher, hud::hashmap_default_key_equal<key_type_2>, AllocatorType>;
            AssignedType assigned(elements_in_assigned);
            ToAssignType to_assign(elements_to_assign);

            assigned = to_assign;

            // Ensure we copy all elements
            bool all_keys_and_values_copied = true;
            for (usize index = 0; index < elements_to_assign.size(); index++)
            {
                const auto &init_elem = (elements_to_assign.begin() + index);
                const auto it = assigned.find(static_cast<key_type>(init_elem->first));
                if (it == assigned.end())
                {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->key() != init_elem->first)
                {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->key().copy_constructor_count() != 2)
                {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->value() != init_elem->second)
                {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->value().copy_constructor_count() != 2)
                {
                    all_keys_and_values_copied = false;
                    break;
                }
            }

            // Allocation count
            u32 expected_allocation_count = 0;
            bool assigned_allocate = elements_in_assigned.size() > 0;
            bool assigned_should_grow = elements_in_assigned.size() < elements_to_assign.size();
            if (assigned_allocate)
            {
                expected_allocation_count++;
            }
            if (assigned_should_grow)
            {
                expected_allocation_count++;
            }
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if (hud::is_constant_evaluated())
            {
                expected_allocation_count *= 2;
            }

            u32 expected_free_count = 0;
            if (assigned_allocate && assigned_should_grow)
            {
                expected_free_count++;
            }
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if (hud::is_constant_evaluated())
            {
                expected_free_count *= 2;
            }
            return std::tuple {
                all_keys_and_values_copied,                                           // 0
                assigned.count() == elements_to_assign.size(),                        // 1
                assigned.max_count() >= elements_to_assign.size(),                    // 2
                assigned.allocator().allocation_count() == expected_allocation_count, // 3
                assigned.allocator().free_count() == expected_free_count              // 4
            };
        };

        // Non constant
        {
            {
                const auto result = test({}, {});
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, {});
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test({}, TEST_VALUES2);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, TEST_VALUES2);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, TEST_VALUES);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES2, TEST_VALUES);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
        }

        // Constant
        {
            {
                constexpr auto result = test({}, {});
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, {});
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test({}, TEST_VALUES2);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, TEST_VALUES2);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, TEST_VALUES);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES2, TEST_VALUES);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
        }
    }

    // Test with extra
    {
        const auto test = [](std::initializer_list<hud::pair<key_type, value_type>> elements_in_assigned, usize extra_assigned, std::initializer_list<hud::pair<key_type_2, value_type_2>> elements_to_assign, usize extra_to_assign)
        {
            using AllocatorType = hud_test::allocator_watcher<1>;
            using AssignedType = hud::hashmap<key_type, value_type, hud::hashmap_default_hasher, hud::hashmap_default_key_equal<key_type>, AllocatorType>;
            using ToAssignType = hud::hashmap<key_type_2, value_type_2, hud::hashmap_default_hasher, hud::hashmap_default_key_equal<key_type_2>, AllocatorType>;
            AssignedType assigned(elements_in_assigned, extra_assigned);
            ToAssignType to_assign(elements_to_assign, extra_to_assign);

            assigned = to_assign;

            // Ensure we copy all elements
            bool all_keys_and_values_copied = true;
            for (usize index = 0; index < elements_to_assign.size(); index++)
            {
                const auto &init_elem = (elements_to_assign.begin() + index);
                const auto it = assigned.find(static_cast<key_type>(init_elem->first));
                if (it == assigned.end())
                {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->key() != init_elem->first)
                {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->key().copy_constructor_count() != 2)
                {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->value() != init_elem->second)
                {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->value().copy_constructor_count() != 2)
                {
                    all_keys_and_values_copied = false;
                    break;
                }
            }
            // Allocation count
            u32 expected_allocation_count = 0;
            bool assigned_allocate = elements_in_assigned.size() > 0 || extra_assigned > 0;
            bool assigned_should_grow = elements_in_assigned.size() < elements_to_assign.size();
            if (assigned_allocate)
            {
                expected_allocation_count++;
            }
            if (assigned_should_grow)
            {
                expected_allocation_count++;
            }
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if (hud::is_constant_evaluated())
            {
                expected_allocation_count *= 2;
            }

            u32 expected_free_count = 0;
            if (assigned_allocate && assigned_should_grow)
            {
                expected_free_count++;
            }
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if (hud::is_constant_evaluated())
            {
                expected_free_count *= 2;
            }
            return std::tuple {
                all_keys_and_values_copied,                                           // 0
                assigned.count() == elements_to_assign.size(),                        // 1
                assigned.max_count() >= elements_to_assign.size(),                    // 2
                assigned.allocator().allocation_count() == expected_allocation_count, // 3
                assigned.allocator().free_count() == expected_free_count              // 4
            };
        };

        // Non constant
        {
            {
                const auto result = test({}, 0, {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test({}, 0, {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test({}, 1, {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test({}, 1, {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                const auto result = test(TEST_VALUES, 0, {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, 0, {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, 1, {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, 1, {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                const auto result = test({}, 0, TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test({}, 0, TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test({}, 1, TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test({}, 1, TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                const auto result = test(TEST_VALUES, 0, TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, 0, TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, 1, TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, 1, TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                const auto result = test(TEST_VALUES, 0, TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, 0, TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, 1, TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, 1, TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                const auto result = test(TEST_VALUES2, 0, TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES2, 0, TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES2, 1, TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES2, 1, TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
        }

        // Constant
        {
            {
                constexpr auto result = test({}, 0, {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test({}, 0, {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test({}, 1, {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test({}, 1, {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                constexpr auto result = test(TEST_VALUES, 0, {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, 0, {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, 1, {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, 1, {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                constexpr auto result = test({}, 0, TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test({}, 0, TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test({}, 1, TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test({}, 1, TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                constexpr auto result = test(TEST_VALUES, 0, TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, 0, TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, 1, TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, 1, TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                constexpr auto result = test(TEST_VALUES, 0, TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, 0, TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, 1, TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, 1, TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                constexpr auto result = test(TEST_VALUES2, 0, TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES2, 0, TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES2, 1, TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES2, 1, TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
        }
    }
}

GTEST_TEST(hashmap, copy_assign_hashmap_of_non_bitwise_copy_constructible_different_type_different_allocator)
{
    using key_type = hud_test::non_bitwise_copy_constructible_type;
    using value_type = hud_test::non_bitwise_copy_constructible_type2;
    using key_type_2 = hud_test::non_bitwise_copy_constructible_type3;
    using value_type_2 = hud_test::non_bitwise_copy_constructible_type4;

    static_assert(!hud::is_bitwise_copy_constructible_v<key_type>);
    static_assert(!hud::is_bitwise_copy_constructible_v<value_type>);
    static_assert(!hud::is_bitwise_copy_constructible_v<key_type_2>);
    static_assert(!hud::is_bitwise_copy_constructible_v<value_type_2>);

    // Test without extra
    {
        const auto test = [](std::initializer_list<hud::pair<key_type, value_type>> elements_in_assigned, std::initializer_list<hud::pair<key_type_2, value_type_2>> elements_to_assign)
        {
            using AllocatorType = hud_test::allocator_watcher<1>;
            using AllocatorType2 = hud_test::allocator_watcher_2<1>;
            using AssignedType = hud::hashmap<key_type, value_type, hud::hashmap_default_hasher, hud::hashmap_default_key_equal<key_type>, AllocatorType>;
            using ToAssignType = hud::hashmap<key_type_2, value_type_2, hud::hashmap_default_hasher, hud::hashmap_default_key_equal<key_type_2>, AllocatorType2>;
            AssignedType assigned(elements_in_assigned);
            ToAssignType to_assign(elements_to_assign);

            assigned = to_assign;

            // Ensure we copy all elements
            bool all_keys_and_values_copied = true;
            for (usize index = 0; index < elements_to_assign.size(); index++)
            {
                const auto &init_elem = (elements_to_assign.begin() + index);
                const auto it = assigned.find(static_cast<key_type>(init_elem->first));
                if (it == assigned.end())
                {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->key() != init_elem->first)
                {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->key().copy_constructor_count() != 2)
                {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->value() != init_elem->second)
                {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->value().copy_constructor_count() != 2)
                {
                    all_keys_and_values_copied = false;
                    break;
                }
            }
            // Allocation count
            u32 expected_allocation_count = 0;
            bool to_assign_allocate = elements_to_assign.size() > 0;
            bool assigned_should_grow = elements_in_assigned.size() < elements_to_assign.size();
            if (to_assign_allocate)
            {
                expected_allocation_count++;
            }
            if (assigned_should_grow)
            {
                expected_allocation_count++;
            }
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if (hud::is_constant_evaluated())
            {
                expected_allocation_count *= 2;
            }

            u32 expected_free_count = 0;
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if (hud::is_constant_evaluated())
            {
                expected_free_count *= 2;
            }
            return std::tuple {
                all_keys_and_values_copied,                                           // 0
                assigned.count() == elements_to_assign.size(),                        // 1
                assigned.max_count() >= elements_to_assign.size(),                    // 2
                assigned.allocator().allocation_count() == expected_allocation_count, // 3
                assigned.allocator().free_count() == expected_free_count              // 4
            };
        };

        // Non constant
        {
            {
                const auto result = test({}, {});
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, {});
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test({}, TEST_VALUES2);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, TEST_VALUES2);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, TEST_VALUES);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES2, TEST_VALUES);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
        }

        // Constant
        {
            {
                constexpr auto result = test({}, {});
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, {});
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test({}, TEST_VALUES2);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, TEST_VALUES2);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, TEST_VALUES);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES2, TEST_VALUES);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
        }
    }

    // Test with extra
    {
        const auto test = [](std::initializer_list<hud::pair<key_type, value_type>> elements_in_assigned, usize extra_assigned, std::initializer_list<hud::pair<key_type_2, value_type_2>> elements_to_assign, usize extra_to_assign)
        {
            using AllocatorType = hud_test::allocator_watcher<1>;
            using AllocatorType2 = hud_test::allocator_watcher_2<1>;
            using AssignedType = hud::hashmap<key_type, value_type, hud::hashmap_default_hasher, hud::hashmap_default_key_equal<key_type>, AllocatorType>;
            using ToAssignType = hud::hashmap<key_type_2, value_type_2, hud::hashmap_default_hasher, hud::hashmap_default_key_equal<key_type_2>, AllocatorType2>;
            AssignedType assigned(elements_in_assigned, extra_assigned);
            ToAssignType to_assign(elements_to_assign, extra_to_assign);

            assigned = to_assign;

            // Ensure we copy all elements
            bool all_keys_and_values_copied = true;
            for (usize index = 0; index < elements_to_assign.size(); index++)
            {
                const auto &init_elem = (elements_to_assign.begin() + index);
                const auto it = assigned.find(static_cast<key_type>(init_elem->first));
                if (it == assigned.end())
                {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->key() != init_elem->first)
                {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->key().copy_constructor_count() != 2)
                {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->value() != init_elem->second)
                {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->value().copy_constructor_count() != 2)
                {
                    all_keys_and_values_copied = false;
                    break;
                }
            }
            // Allocation count
            u32 expected_allocation_count = 0;
            bool to_assign_allocate = elements_to_assign.size() > 0 || extra_to_assign > 0;
            bool assigned_should_grow = elements_in_assigned.size() < elements_to_assign.size();
            if (to_assign_allocate)
            {
                expected_allocation_count++;
            }
            if (assigned_should_grow)
            {
                expected_allocation_count++;
            }
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if (hud::is_constant_evaluated())
            {
                expected_allocation_count *= 2;
            }

            u32 expected_free_count = 0;
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if (hud::is_constant_evaluated())
            {
                expected_free_count *= 2;
            }
            return std::tuple {
                all_keys_and_values_copied,                                           // 0
                assigned.count() == elements_to_assign.size(),                        // 1
                assigned.max_count() >= elements_to_assign.size(),                    // 2
                assigned.allocator().allocation_count() == expected_allocation_count, // 3
                assigned.allocator().free_count() == expected_free_count              // 4
            };
        };

        // Non constant
        {
            {
                const auto result = test({}, 0, {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test({}, 0, {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test({}, 1, {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test({}, 1, {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                const auto result = test(TEST_VALUES, 0, {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, 0, {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, 1, {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, 1, {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                const auto result = test({}, 0, TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test({}, 0, TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test({}, 1, TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test({}, 1, TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                const auto result = test(TEST_VALUES, 0, TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, 0, TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, 1, TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, 1, TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                const auto result = test(TEST_VALUES, 0, TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, 0, TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, 1, TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES, 1, TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                const auto result = test(TEST_VALUES2, 0, TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES2, 0, TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES2, 1, TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = test(TEST_VALUES2, 1, TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
        }

        // Constant
        {
            {
                constexpr auto result = test({}, 0, {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test({}, 0, {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test({}, 1, {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test({}, 1, {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                constexpr auto result = test(TEST_VALUES, 0, {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, 0, {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, 1, {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, 1, {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                constexpr auto result = test({}, 0, TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test({}, 0, TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test({}, 1, TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test({}, 1, TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                constexpr auto result = test(TEST_VALUES, 0, TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, 0, TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, 1, TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, 1, TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                constexpr auto result = test(TEST_VALUES, 0, TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, 0, TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, 1, TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, 1, TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                constexpr auto result = test(TEST_VALUES2, 0, TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES2, 0, TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES2, 1, TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES2, 1, TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
        }
    }
}
