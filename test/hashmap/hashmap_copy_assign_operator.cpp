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
    {10, 100}          \
 }

GTEST_TEST(hashmap, copy_assign_hashmap_of_bitwise_copy_constructible_same_type_same_allocator)
{
    using key_type = i32;
    using value_type = u64;

    static_assert(hud::is_bitwise_copy_assignable_v<key_type>);
    static_assert(hud::is_bitwise_copy_assignable_v<value_type>);

    // Test without extra
    {
        static const auto test = [](std::initializer_list<hud::pair<key_type, value_type>> elements_in_assigned, std::initializer_list<hud::pair<key_type, value_type>> elements_to_assign) {
            using AllocatorType = hud_test::allocator_watcher<1>;
            using AssignedType = hud::hashmap<key_type, value_type, hud::hash_64<key_type>, hud::equal<key_type>, AllocatorType>;
            using ToAssignType = hud::hashmap<key_type, value_type, hud::hash_64<key_type>, hud::equal<key_type>, AllocatorType>;
            AssignedType assigned(elements_in_assigned);
            ToAssignType to_assign(elements_to_assign);

            assigned = to_assign;

            // Ensure we copy all elements
            bool all_keys_and_values_copied = true;
            for (usize index = 0; index < elements_to_assign.size(); index++) {
                const auto &init_elem = (elements_to_assign.begin() + index);
                const auto it = assigned.find(init_elem->first);
                if (it == assigned.end()) {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->key() != init_elem->first) {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->value() != init_elem->second) {
                    all_keys_and_values_copied = false;
                    break;
                }
            }
            // Allocation count
            u32 expected_allocation_count = 0;
            bool assigned_allocate = elements_in_assigned.size() > 0;
            bool assigned_should_grow = elements_in_assigned.size() < elements_to_assign.size();
            if (assigned_allocate) {
                expected_allocation_count++;
            }
            if (assigned_should_grow) {
                expected_allocation_count++;
            }
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if consteval {
                expected_allocation_count *= 2;
            }

            u32 expected_free_count = 0;
            if (assigned_allocate && assigned_should_grow) {
                expected_free_count++;
            }
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if consteval {
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
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {}, std::initializer_list<hud::pair<key_type, value_type>> {});
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, std::initializer_list<hud::pair<key_type, value_type>> {});
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {}, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES);
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
        static const auto test = [](std::initializer_list<hud::pair<key_type, value_type>> elements_in_assigned, usize extra_assigned, std::initializer_list<hud::pair<key_type, value_type>> elements_to_assign, usize extra_to_assign) {
            using AllocatorType = hud_test::allocator_watcher<1>;
            using AssignedType = hud::hashmap<key_type, value_type, hud::hash_64<key_type>, hud::equal<key_type>, AllocatorType>;
            using ToAssignType = hud::hashmap<key_type, value_type, hud::hash_64<key_type>, hud::equal<key_type>, AllocatorType>;
            AssignedType assigned(elements_in_assigned, extra_assigned);
            ToAssignType to_assign(elements_to_assign, extra_to_assign);

            assigned = to_assign;

            // Ensure we copy all elements
            bool all_keys_and_values_copied = true;
            for (usize index = 0; index < elements_to_assign.size(); index++) {
                const auto &init_elem = (elements_to_assign.begin() + index);
                const auto it = assigned.find(init_elem->first);
                if (it == assigned.end()) {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->key() != init_elem->first) {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->value() != init_elem->second) {
                    all_keys_and_values_copied = false;
                    break;
                }
            }
            // Allocation count
            u32 expected_allocation_count = 0;
            bool assigned_allocate = elements_in_assigned.size() > 0 || extra_assigned > 0;
            bool assigned_should_grow = elements_in_assigned.size() < elements_to_assign.size();
            if (assigned_allocate) {
                expected_allocation_count++;
            }
            if (assigned_should_grow) {
                expected_allocation_count++;
            }
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if consteval {
                expected_allocation_count *= 2;
            }

            u32 expected_free_count = 0;
            if (assigned_allocate && assigned_should_grow) {
                expected_free_count++;
            }
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if consteval {
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
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {}, 0, std::initializer_list<hud::pair<key_type, value_type>> {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {}, 0, std::initializer_list<hud::pair<key_type, value_type>> {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {}, 1, std::initializer_list<hud::pair<key_type, value_type>> {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {}, 1, std::initializer_list<hud::pair<key_type, value_type>> {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 0, std::initializer_list<hud::pair<key_type, value_type>> {}, 0);

                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 0, std::initializer_list<hud::pair<key_type, value_type>> {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 1, std::initializer_list<hud::pair<key_type, value_type>> {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 1, std::initializer_list<hud::pair<key_type, value_type>> {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {}, 0, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {}, 0, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {}, 1, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {}, 1, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 0, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 0, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 1, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 1, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 0, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 0, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 1, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 1, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2, 0, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2, 0, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2, 1, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2, 1, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 1);
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
        static const auto test = [](std::initializer_list<hud::pair<key_type, value_type>> elements_in_assigned, std::initializer_list<hud::pair<key_type, value_type>> elements_to_assign) {
            using AllocatorType = hud_test::allocator_watcher<1>;
            using AllocatorType2 = hud_test::allocator_watcher_2<1>;
            using AssignedType = hud::hashmap<key_type, value_type, hud::hash_64<key_type>, hud::equal<key_type>, AllocatorType>;
            using ToAssignType = hud::hashmap<key_type, value_type, hud::hash_64<key_type>, hud::equal<key_type>, AllocatorType2>;
            AssignedType assigned(elements_in_assigned);
            ToAssignType to_assign(elements_to_assign);

            assigned = to_assign;

            // Ensure we copy all elements
            bool all_keys_and_values_copied = true;
            for (usize index = 0; index < elements_to_assign.size(); index++) {
                const auto &init_elem = (elements_to_assign.begin() + index);
                const auto it = assigned.find(init_elem->first);
                if (it == assigned.end()) {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->key() != init_elem->first) {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->value() != init_elem->second) {
                    all_keys_and_values_copied = false;
                    break;
                }
            }

            // Allocation count
            u32 expected_allocation_count = 0;
            bool to_assign_allocate = elements_to_assign.size() > 0;
            bool assigned_should_grow = elements_in_assigned.size() < elements_to_assign.size();
            if (to_assign_allocate) {
                expected_allocation_count++;
            }
            if (assigned_should_grow) {
                expected_allocation_count++;
            }
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if consteval {
                expected_allocation_count *= 2;
            }

            u32 expected_free_count = 0;
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if consteval {
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
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {}, std::initializer_list<hud::pair<key_type, value_type>> {});
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, std::initializer_list<hud::pair<key_type, value_type>> {});
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {}, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES);
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
        static const auto test = [](std::initializer_list<hud::pair<key_type, value_type>> elements_in_assigned, usize extra_assigned, std::initializer_list<hud::pair<key_type, value_type>> elements_to_assign, usize extra_to_assign) {
            using AllocatorType = hud_test::allocator_watcher<1>;
            using AllocatorType2 = hud_test::allocator_watcher_2<1>;
            using AssignedType = hud::hashmap<key_type, value_type, hud::hash_64<key_type>, hud::equal<key_type>, AllocatorType>;
            using ToAssignType = hud::hashmap<key_type, value_type, hud::hash_64<key_type>, hud::equal<key_type>, AllocatorType2>;
            AssignedType assigned(elements_in_assigned, extra_assigned);
            ToAssignType to_assign(elements_to_assign, extra_to_assign);

            assigned = to_assign;

            // Ensure we copy all elements
            bool all_keys_and_values_copied = true;
            for (usize index = 0; index < elements_to_assign.size(); index++) {
                const auto &init_elem = (elements_to_assign.begin() + index);
                const auto it = assigned.find(init_elem->first);
                if (it == assigned.end()) {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->key() != init_elem->first) {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->value() != init_elem->second) {
                    all_keys_and_values_copied = false;
                    break;
                }
            }
            // Allocation count
            u32 expected_allocation_count = 0;
            bool to_assign_allocate = elements_to_assign.size() > 0 || extra_to_assign > 0;
            bool assigned_should_grow = elements_in_assigned.size() < elements_to_assign.size();
            if (to_assign_allocate) {
                expected_allocation_count++;
            }
            if (assigned_should_grow) {
                expected_allocation_count++;
            }
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if consteval {
                expected_allocation_count *= 2;
            }

            u32 expected_free_count = 0;
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if consteval {
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
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {}, 0, std::initializer_list<hud::pair<key_type, value_type>> {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {}, 0, std::initializer_list<hud::pair<key_type, value_type>> {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {}, 1, std::initializer_list<hud::pair<key_type, value_type>> {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {}, 1, std::initializer_list<hud::pair<key_type, value_type>> {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 0, std::initializer_list<hud::pair<key_type, value_type>> {}, 0);

                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 0, std::initializer_list<hud::pair<key_type, value_type>> {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 1, std::initializer_list<hud::pair<key_type, value_type>> {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 1, std::initializer_list<hud::pair<key_type, value_type>> {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {}, 0, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {}, 0, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {}, 1, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {}, 1, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 0, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 0, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 1, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 1, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 0, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 0, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 1, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 1, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2, 0, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2, 0, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2, 1, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2, 1, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 1);
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
        static const auto test = [](std::initializer_list<hud::pair<key_type, value_type>> elements_in_assigned, std::initializer_list<hud::pair<key_type_2, value_type_2>> elements_to_assign) {
            using AllocatorType = hud_test::allocator_watcher<1>;
            using AssignedType = hud::hashmap<key_type, value_type, hud::hash_64<key_type>, hud::equal<key_type>, AllocatorType>;
            using ToAssignType = hud::hashmap<key_type_2, value_type_2, hud::hash_64<key_type_2>, hud::equal<key_type_2>, AllocatorType>;
            AssignedType assigned(elements_in_assigned);
            ToAssignType to_assign(elements_to_assign);

            assigned = to_assign;

            // Ensure we copy all elements
            bool all_keys_and_values_copied = true;
            for (usize index = 0; index < elements_to_assign.size(); index++) {
                const auto &init_elem = (elements_to_assign.begin() + index);
                const auto it = assigned.find(static_cast<key_type>(init_elem->first));
                if (it == assigned.end()) {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->key() != static_cast<key_type>(init_elem->first)) {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->value() != static_cast<key_type>(init_elem->second)) {
                    all_keys_and_values_copied = false;
                    break;
                }
            }

            // Allocation count
            u32 expected_allocation_count = 0;
            bool assigned_allocate = elements_in_assigned.size() > 0;
            bool assigned_should_grow = elements_in_assigned.size() < elements_to_assign.size();
            if (assigned_allocate) {
                expected_allocation_count++;
            }
            if (assigned_should_grow) {
                expected_allocation_count++;
            }
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if consteval {
                expected_allocation_count *= 2;
            }

            u32 expected_free_count = 0;
            if (assigned_allocate && assigned_should_grow) {
                expected_free_count++;
            }
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if consteval {
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
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {}, std::initializer_list<hud::pair<key_type_2, value_type_2>> {});
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, std::initializer_list<hud::pair<key_type_2, value_type_2>> {});
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {}, std::initializer_list<hud::pair<key_type_2, value_type_2>> TEST_VALUES2);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, std::initializer_list<hud::pair<key_type_2, value_type_2>> TEST_VALUES2);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, std::initializer_list<hud::pair<key_type_2, value_type_2>> TEST_VALUES);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2, std::initializer_list<hud::pair<key_type_2, value_type_2>> TEST_VALUES);
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
        static const auto test = [](std::initializer_list<hud::pair<key_type, value_type>> elements_in_assigned, usize extra_assigned, std::initializer_list<hud::pair<key_type_2, value_type_2>> elements_to_assign, usize extra_to_assign) {
            using AllocatorType = hud_test::allocator_watcher<1>;
            using AssignedType = hud::hashmap<key_type, value_type, hud::hash_64<key_type>, hud::equal<key_type>, AllocatorType>;
            using ToAssignType = hud::hashmap<key_type_2, value_type_2, hud::hash_64<key_type_2>, hud::equal<key_type_2>, AllocatorType>;
            AssignedType assigned(elements_in_assigned, extra_assigned);
            ToAssignType to_assign(elements_to_assign, extra_to_assign);

            assigned = to_assign;

            // Ensure we copy all elements
            bool all_keys_and_values_copied = true;
            for (usize index = 0; index < elements_to_assign.size(); index++) {
                const auto &init_elem = (elements_to_assign.begin() + index);
                const auto it = assigned.find(static_cast<key_type>(init_elem->first));
                if (it == assigned.end()) {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->key() != static_cast<key_type>(init_elem->first)) {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->value() != static_cast<key_type>(init_elem->second)) {
                    all_keys_and_values_copied = false;
                    break;
                }
            }
            // Allocation count
            u32 expected_allocation_count = 0;
            bool assigned_allocate = elements_in_assigned.size() > 0 || extra_assigned > 0;
            bool assigned_should_grow = elements_in_assigned.size() < elements_to_assign.size();
            if (assigned_allocate) {
                expected_allocation_count++;
            }
            if (assigned_should_grow) {
                expected_allocation_count++;
            }
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if consteval {
                expected_allocation_count *= 2;
            }

            u32 expected_free_count = 0;
            if (assigned_allocate && assigned_should_grow) {
                expected_free_count++;
            }
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if consteval {
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
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {}, 0, std::initializer_list<hud::pair<key_type_2, value_type_2>> {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {}, 0, std::initializer_list<hud::pair<key_type_2, value_type_2>> {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {}, 1, std::initializer_list<hud::pair<key_type_2, value_type_2>> {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {}, 1, std::initializer_list<hud::pair<key_type_2, value_type_2>> {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 0, std::initializer_list<hud::pair<key_type_2, value_type_2>> {}, 0);

                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 0, std::initializer_list<hud::pair<key_type_2, value_type_2>> {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 1, std::initializer_list<hud::pair<key_type_2, value_type_2>> {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 1, std::initializer_list<hud::pair<key_type_2, value_type_2>> {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {}, 0, std::initializer_list<hud::pair<key_type_2, value_type_2>> TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {}, 0, std::initializer_list<hud::pair<key_type_2, value_type_2>> TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {}, 1, std::initializer_list<hud::pair<key_type_2, value_type_2>> TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {}, 1, std::initializer_list<hud::pair<key_type_2, value_type_2>> TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 0, std::initializer_list<hud::pair<key_type_2, value_type_2>> TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 0, std::initializer_list<hud::pair<key_type_2, value_type_2>> TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 1, std::initializer_list<hud::pair<key_type_2, value_type_2>> TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 1, std::initializer_list<hud::pair<key_type_2, value_type_2>> TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 0, std::initializer_list<hud::pair<key_type_2, value_type_2>> TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 0, std::initializer_list<hud::pair<key_type_2, value_type_2>> TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 1, std::initializer_list<hud::pair<key_type_2, value_type_2>> TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 1, std::initializer_list<hud::pair<key_type_2, value_type_2>> TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2, 0, std::initializer_list<hud::pair<key_type_2, value_type_2>> TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2, 0, std::initializer_list<hud::pair<key_type_2, value_type_2>> TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2, 1, std::initializer_list<hud::pair<key_type_2, value_type_2>> TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2, 1, std::initializer_list<hud::pair<key_type_2, value_type_2>> TEST_VALUES, 1);
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
        static const auto test = [](std::initializer_list<hud::pair<key_type, value_type>> elements_in_assigned, std::initializer_list<hud::pair<key_type_2, value_type_2>> elements_to_assign) {
            using AllocatorType = hud_test::allocator_watcher<1>;
            using AllocatorType2 = hud_test::allocator_watcher_2<1>;
            using AssignedType = hud::hashmap<key_type, value_type, hud::hash_64<key_type>, hud::equal<key_type>, AllocatorType>;
            using ToAssignType = hud::hashmap<key_type_2, value_type_2, hud::hash_64<key_type_2>, hud::equal<key_type_2>, AllocatorType2>;
            AssignedType assigned(elements_in_assigned);
            ToAssignType to_assign(elements_to_assign);

            assigned = to_assign;

            // Ensure we copy all elements
            bool all_keys_and_values_copied = true;
            for (usize index = 0; index < elements_to_assign.size(); index++) {
                const auto &init_elem = (elements_to_assign.begin() + index);
                const auto it = assigned.find(static_cast<key_type>(init_elem->first));
                if (it == assigned.end()) {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->key() != static_cast<key_type>(init_elem->first)) {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->value() != static_cast<key_type>(init_elem->second)) {
                    all_keys_and_values_copied = false;
                    break;
                }
            }
            // Allocation count
            u32 expected_allocation_count = 0;
            bool to_assign_allocate = elements_to_assign.size() > 0;
            bool assigned_should_grow = elements_in_assigned.size() < elements_to_assign.size();
            if (to_assign_allocate) {
                expected_allocation_count++;
            }
            if (assigned_should_grow) {
                expected_allocation_count++;
            }
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if consteval {
                expected_allocation_count *= 2;
            }

            u32 expected_free_count = 0;
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if consteval {
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
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {}, std::initializer_list<hud::pair<key_type_2, value_type_2>> {});
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, std::initializer_list<hud::pair<key_type_2, value_type_2>> {});
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {}, std::initializer_list<hud::pair<key_type_2, value_type_2>> TEST_VALUES2);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, std::initializer_list<hud::pair<key_type_2, value_type_2>> TEST_VALUES2);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, std::initializer_list<hud::pair<key_type_2, value_type_2>> TEST_VALUES);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2, std::initializer_list<hud::pair<key_type_2, value_type_2>> TEST_VALUES);
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
        static const auto test = [](std::initializer_list<hud::pair<key_type, value_type>> elements_in_assigned, usize extra_assigned, std::initializer_list<hud::pair<key_type_2, value_type_2>> elements_to_assign, usize extra_to_assign) {
            using AllocatorType = hud_test::allocator_watcher<1>;
            using AllocatorType2 = hud_test::allocator_watcher_2<1>;
            using AssignedType = hud::hashmap<key_type, value_type, hud::hash_64<key_type>, hud::equal<key_type>, AllocatorType>;
            using ToAssignType = hud::hashmap<key_type_2, value_type_2, hud::hash_64<key_type_2>, hud::equal<key_type_2>, AllocatorType2>;
            AssignedType assigned(elements_in_assigned, extra_assigned);
            ToAssignType to_assign(elements_to_assign, extra_to_assign);

            assigned = to_assign;

            // Ensure we copy all elements
            bool all_keys_and_values_copied = true;
            for (usize index = 0; index < elements_to_assign.size(); index++) {
                const auto &init_elem = (elements_to_assign.begin() + index);
                const auto it = assigned.find(static_cast<key_type>(init_elem->first));
                if (it == assigned.end()) {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->key() != static_cast<key_type>(init_elem->first)) {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->value() != static_cast<key_type>(init_elem->second)) {
                    all_keys_and_values_copied = false;
                    break;
                }
            }
            // Allocation count
            u32 expected_allocation_count = 0;
            bool to_assign_allocate = elements_to_assign.size() > 0 || extra_to_assign > 0;
            bool assigned_should_grow = elements_in_assigned.size() < elements_to_assign.size();
            if (to_assign_allocate) {
                expected_allocation_count++;
            }
            if (assigned_should_grow) {
                expected_allocation_count++;
            }
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if consteval {
                expected_allocation_count *= 2;
            }

            u32 expected_free_count = 0;
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if consteval {
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
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {}, 0, std::initializer_list<hud::pair<key_type_2, value_type_2>> {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {}, 0, std::initializer_list<hud::pair<key_type_2, value_type_2>> {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {}, 1, std::initializer_list<hud::pair<key_type_2, value_type_2>> {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {}, 1, std::initializer_list<hud::pair<key_type_2, value_type_2>> {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 0, std::initializer_list<hud::pair<key_type_2, value_type_2>> {}, 0);

                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 0, std::initializer_list<hud::pair<key_type_2, value_type_2>> {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 1, std::initializer_list<hud::pair<key_type_2, value_type_2>> {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 1, std::initializer_list<hud::pair<key_type_2, value_type_2>> {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {}, 0, std::initializer_list<hud::pair<key_type_2, value_type_2>> TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {}, 0, std::initializer_list<hud::pair<key_type_2, value_type_2>> TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {}, 1, std::initializer_list<hud::pair<key_type_2, value_type_2>> TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {}, 1, std::initializer_list<hud::pair<key_type_2, value_type_2>> TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 0, std::initializer_list<hud::pair<key_type_2, value_type_2>> TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 0, std::initializer_list<hud::pair<key_type_2, value_type_2>> TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 1, std::initializer_list<hud::pair<key_type_2, value_type_2>> TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 1, std::initializer_list<hud::pair<key_type_2, value_type_2>> TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 0, std::initializer_list<hud::pair<key_type_2, value_type_2>> TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 0, std::initializer_list<hud::pair<key_type_2, value_type_2>> TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 1, std::initializer_list<hud::pair<key_type_2, value_type_2>> TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 1, std::initializer_list<hud::pair<key_type_2, value_type_2>> TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2, 0, std::initializer_list<hud::pair<key_type_2, value_type_2>> TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2, 0, std::initializer_list<hud::pair<key_type_2, value_type_2>> TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2, 1, std::initializer_list<hud::pair<key_type_2, value_type_2>> TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2, 1, std::initializer_list<hud::pair<key_type_2, value_type_2>> TEST_VALUES, 1);
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
        static const auto test = [](std::initializer_list<hud::pair<key_type, value_type>> elements_in_assigned, std::initializer_list<hud::pair<key_type, value_type>> elements_to_assign) {
            using AllocatorType = hud_test::allocator_watcher<1>;
            using AssignedType = hud::hashmap<key_type, value_type, hud::hash_64<key_type>, hud::equal<key_type>, AllocatorType>;
            using ToAssignType = hud::hashmap<key_type, value_type, hud::hash_64<key_type>, hud::equal<key_type>, AllocatorType>;
            AssignedType assigned(elements_in_assigned);
            ToAssignType to_assign(elements_to_assign);

            assigned = to_assign;

            // Ensure we copy all elements
            bool all_keys_and_values_copied = true;
            for (usize index = 0; index < elements_to_assign.size(); index++) {
                const auto &init_elem = (elements_to_assign.begin() + index);
                const auto it = assigned.find(init_elem->first);
                if (it == assigned.end()) {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->key() != init_elem->first) {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->key().copy_constructor_count() != 2) {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->value() != init_elem->second) {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->value().copy_constructor_count() != 2) {
                    all_keys_and_values_copied = false;
                    break;
                }
            }
            // Allocation count
            u32 expected_allocation_count = 0;
            bool assigned_allocate = elements_in_assigned.size() > 0;
            bool assigned_should_grow = elements_in_assigned.size() < elements_to_assign.size();
            if (assigned_allocate) {
                expected_allocation_count++;
            }
            if (assigned_should_grow) {
                expected_allocation_count++;
            }
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if consteval {
                expected_allocation_count *= 2;
            }

            u32 expected_free_count = 0;
            if (assigned_allocate && assigned_should_grow) {
                expected_free_count++;
            }
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if consteval {
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
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {}, std::initializer_list<hud::pair<key_type, value_type>> {});
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, std::initializer_list<hud::pair<key_type, value_type>> {});
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {}, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES);
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
        static const auto test = [](std::initializer_list<hud::pair<key_type, value_type>> elements_in_assigned, usize extra_assigned, std::initializer_list<hud::pair<key_type, value_type>> elements_to_assign, usize extra_to_assign) {
            using AllocatorType = hud_test::allocator_watcher<1>;
            using AssignedType = hud::hashmap<key_type, value_type, hud::hash_64<key_type>, hud::equal<key_type>, AllocatorType>;
            using ToAssignType = hud::hashmap<key_type, value_type, hud::hash_64<key_type>, hud::equal<key_type>, AllocatorType>;
            AssignedType assigned(elements_in_assigned, extra_assigned);
            ToAssignType to_assign(elements_to_assign, extra_to_assign);

            assigned = to_assign;

            // Ensure we copy all elements
            bool all_keys_and_values_copied = true;
            for (usize index = 0; index < elements_to_assign.size(); index++) {
                const auto &init_elem = (elements_to_assign.begin() + index);
                const auto it = assigned.find(init_elem->first);
                if (it == assigned.end()) {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->key() != init_elem->first) {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->key().copy_constructor_count() != 2) {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->value() != init_elem->second) {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->value().copy_constructor_count() != 2) {
                    all_keys_and_values_copied = false;
                    break;
                }
            }
            // Allocation count
            u32 expected_allocation_count = 0;
            bool assigned_allocate = elements_in_assigned.size() > 0 || extra_assigned > 0;
            bool assigned_should_grow = elements_in_assigned.size() < elements_to_assign.size();
            if (assigned_allocate) {
                expected_allocation_count++;
            }
            if (assigned_should_grow) {
                expected_allocation_count++;
            }
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if consteval {
                expected_allocation_count *= 2;
            }

            u32 expected_free_count = 0;
            if (assigned_allocate && assigned_should_grow) {
                expected_free_count++;
            }
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if consteval {
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
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {}, 0, std::initializer_list<hud::pair<key_type, value_type>> {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {}, 0, std::initializer_list<hud::pair<key_type, value_type>> {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {}, 1, std::initializer_list<hud::pair<key_type, value_type>> {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {}, 1, std::initializer_list<hud::pair<key_type, value_type>> {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 0, std::initializer_list<hud::pair<key_type, value_type>> {}, 0);

                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 0, std::initializer_list<hud::pair<key_type, value_type>> {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 1, std::initializer_list<hud::pair<key_type, value_type>> {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 1, std::initializer_list<hud::pair<key_type, value_type>> {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {}, 0, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {}, 0, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {}, 1, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {}, 1, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 0, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 0, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 1, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 1, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 0, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 0, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 1, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 1, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2, 0, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2, 0, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2, 1, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2, 1, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 1);
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
        static const auto test = [](std::initializer_list<hud::pair<key_type, value_type>> elements_in_assigned, std::initializer_list<hud::pair<key_type, value_type>> elements_to_assign) {
            using AllocatorType = hud_test::allocator_watcher<1>;
            using AllocatorType2 = hud_test::allocator_watcher_2<1>;
            using AssignedType = hud::hashmap<key_type, value_type, hud::hash_64<key_type>, hud::equal<key_type>, AllocatorType>;
            using ToAssignType = hud::hashmap<key_type, value_type, hud::hash_64<key_type>, hud::equal<key_type>, AllocatorType2>;
            AssignedType assigned(elements_in_assigned);
            ToAssignType to_assign(elements_to_assign);

            assigned = to_assign;

            // Ensure we copy all elements
            bool all_keys_and_values_copied = true;
            for (usize index = 0; index < elements_to_assign.size(); index++) {
                const auto &init_elem = (elements_to_assign.begin() + index);
                const auto it = assigned.find(init_elem->first);
                if (it == assigned.end()) {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->key() != init_elem->first) {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->key().copy_constructor_count() != 2) {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->value() != init_elem->second) {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->value().copy_constructor_count() != 2) {
                    all_keys_and_values_copied = false;
                    break;
                }
            }

            // Allocation count
            u32 expected_allocation_count = 0;
            bool to_assign_allocate = elements_to_assign.size() > 0;
            bool assigned_should_grow = elements_in_assigned.size() < elements_to_assign.size();
            if (to_assign_allocate) {
                expected_allocation_count++;
            }
            if (assigned_should_grow) {
                expected_allocation_count++;
            }
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if consteval {
                expected_allocation_count *= 2;
            }

            u32 expected_free_count = 0;
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if consteval {
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
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {}, std::initializer_list<hud::pair<key_type, value_type>> {});
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, std::initializer_list<hud::pair<key_type, value_type>> {});
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {}, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES);
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
        static const auto test = [](std::initializer_list<hud::pair<key_type, value_type>> elements_in_assigned, usize extra_assigned, std::initializer_list<hud::pair<key_type, value_type>> elements_to_assign, usize extra_to_assign) {
            using AllocatorType = hud_test::allocator_watcher<1>;
            using AllocatorType2 = hud_test::allocator_watcher_2<1>;
            using AssignedType = hud::hashmap<key_type, value_type, hud::hash_64<key_type>, hud::equal<key_type>, AllocatorType>;
            using ToAssignType = hud::hashmap<key_type, value_type, hud::hash_64<key_type>, hud::equal<key_type>, AllocatorType2>;
            AssignedType assigned(elements_in_assigned, extra_assigned);
            ToAssignType to_assign(elements_to_assign, extra_to_assign);

            assigned = to_assign;

            // Ensure we copy all elements
            bool all_keys_and_values_copied = true;
            for (usize index = 0; index < elements_to_assign.size(); index++) {
                const auto &init_elem = (elements_to_assign.begin() + index);
                const auto it = assigned.find(init_elem->first);
                if (it == assigned.end()) {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->key() != init_elem->first) {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->key().copy_constructor_count() != 2) {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->value() != init_elem->second) {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->value().copy_constructor_count() != 2) {
                    all_keys_and_values_copied = false;
                    break;
                }
            }
            // Allocation count
            u32 expected_allocation_count = 0;
            bool to_assign_allocate = elements_to_assign.size() > 0 || extra_to_assign > 0;
            bool assigned_should_grow = elements_in_assigned.size() < elements_to_assign.size();
            if (to_assign_allocate) {
                expected_allocation_count++;
            }
            if (assigned_should_grow) {
                expected_allocation_count++;
            }
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if consteval {
                expected_allocation_count *= 2;
            }

            u32 expected_free_count = 0;
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if consteval {
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
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {}, 0, std::initializer_list<hud::pair<key_type, value_type>> {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {}, 0, std::initializer_list<hud::pair<key_type, value_type>> {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {}, 1, std::initializer_list<hud::pair<key_type, value_type>> {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {}, 1, std::initializer_list<hud::pair<key_type, value_type>> {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 0, std::initializer_list<hud::pair<key_type, value_type>> {}, 0);

                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 0, std::initializer_list<hud::pair<key_type, value_type>> {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 1, std::initializer_list<hud::pair<key_type, value_type>> {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 1, std::initializer_list<hud::pair<key_type, value_type>> {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {}, 0, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {}, 0, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {}, 1, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {}, 1, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 0, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 0, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 1, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 1, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 0, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 0, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 1, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 1, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2, 0, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2, 0, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2, 1, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2, 1, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 1);
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
        static const auto test = [](std::initializer_list<hud::pair<key_type, value_type>> elements_in_assigned, std::initializer_list<hud::pair<key_type_2, value_type_2>> elements_to_assign) {
            using AllocatorType = hud_test::allocator_watcher<1>;
            using AssignedType = hud::hashmap<key_type, value_type, hud::hash_64<key_type>, hud::equal<key_type>, AllocatorType>;
            using ToAssignType = hud::hashmap<key_type_2, value_type_2, hud::hash_64<key_type>, hud::equal<key_type_2>, AllocatorType>;
            AssignedType assigned(elements_in_assigned);
            ToAssignType to_assign(elements_to_assign);

            assigned = to_assign;

            // Ensure we copy all elements
            bool all_keys_and_values_copied = true;
            for (usize index = 0; index < elements_to_assign.size(); index++) {
                const auto &init_elem = (elements_to_assign.begin() + index);
                const auto it = assigned.find(static_cast<key_type>(init_elem->first));
                if (it == assigned.end()) {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->key() != init_elem->first) {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->key().copy_constructor_count() != 2) {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->value() != init_elem->second) {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->value().copy_constructor_count() != 2) {
                    all_keys_and_values_copied = false;
                    break;
                }
            }

            // Allocation count
            u32 expected_allocation_count = 0;
            bool assigned_allocate = elements_in_assigned.size() > 0;
            bool assigned_should_grow = elements_in_assigned.size() < elements_to_assign.size();
            if (assigned_allocate) {
                expected_allocation_count++;
            }
            if (assigned_should_grow) {
                expected_allocation_count++;
            }
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if consteval {
                expected_allocation_count *= 2;
            }

            u32 expected_free_count = 0;
            if (assigned_allocate && assigned_should_grow) {
                expected_free_count++;
            }
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if consteval {
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
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {}, std::initializer_list<hud::pair<key_type_2, value_type_2>> {});
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, std::initializer_list<hud::pair<key_type_2, value_type_2>> {});
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {}, std::initializer_list<hud::pair<key_type_2, value_type_2>> TEST_VALUES2);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, std::initializer_list<hud::pair<key_type_2, value_type_2>> TEST_VALUES2);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, std::initializer_list<hud::pair<key_type_2, value_type_2>> TEST_VALUES);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2, std::initializer_list<hud::pair<key_type_2, value_type_2>> TEST_VALUES);
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
        static const auto test = [](std::initializer_list<hud::pair<key_type, value_type>> elements_in_assigned, usize extra_assigned, std::initializer_list<hud::pair<key_type_2, value_type_2>> elements_to_assign, usize extra_to_assign) {
            using AllocatorType = hud_test::allocator_watcher<1>;
            using AssignedType = hud::hashmap<key_type, value_type, hud::hash_64<key_type>, hud::equal<key_type>, AllocatorType>;
            using ToAssignType = hud::hashmap<key_type_2, value_type_2, hud::hash_64<key_type>, hud::equal<key_type_2>, AllocatorType>;
            AssignedType assigned(elements_in_assigned, extra_assigned);
            ToAssignType to_assign(elements_to_assign, extra_to_assign);

            assigned = to_assign;

            // Ensure we copy all elements
            bool all_keys_and_values_copied = true;
            for (usize index = 0; index < elements_to_assign.size(); index++) {
                const auto &init_elem = (elements_to_assign.begin() + index);
                const auto it = assigned.find(static_cast<key_type>(init_elem->first));
                if (it == assigned.end()) {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->key() != init_elem->first) {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->key().copy_constructor_count() != 2) {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->value() != init_elem->second) {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->value().copy_constructor_count() != 2) {
                    all_keys_and_values_copied = false;
                    break;
                }
            }
            // Allocation count
            u32 expected_allocation_count = 0;
            bool assigned_allocate = elements_in_assigned.size() > 0 || extra_assigned > 0;
            bool assigned_should_grow = elements_in_assigned.size() < elements_to_assign.size();
            if (assigned_allocate) {
                expected_allocation_count++;
            }
            if (assigned_should_grow) {
                expected_allocation_count++;
            }
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if consteval {
                expected_allocation_count *= 2;
            }

            u32 expected_free_count = 0;
            if (assigned_allocate && assigned_should_grow) {
                expected_free_count++;
            }
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if consteval {
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
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {}, 0, std::initializer_list<hud::pair<key_type_2, value_type_2>> {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {}, 0, std::initializer_list<hud::pair<key_type_2, value_type_2>> {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {}, 1, std::initializer_list<hud::pair<key_type_2, value_type_2>> {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {}, 1, std::initializer_list<hud::pair<key_type_2, value_type_2>> {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 0, std::initializer_list<hud::pair<key_type_2, value_type_2>> {}, 0);

                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 0, std::initializer_list<hud::pair<key_type_2, value_type_2>> {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 1, std::initializer_list<hud::pair<key_type_2, value_type_2>> {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 1, std::initializer_list<hud::pair<key_type_2, value_type_2>> {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {}, 0, std::initializer_list<hud::pair<key_type_2, value_type_2>> TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {}, 0, std::initializer_list<hud::pair<key_type_2, value_type_2>> TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {}, 1, std::initializer_list<hud::pair<key_type_2, value_type_2>> TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {}, 1, std::initializer_list<hud::pair<key_type_2, value_type_2>> TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 0, std::initializer_list<hud::pair<key_type_2, value_type_2>> TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 0, std::initializer_list<hud::pair<key_type_2, value_type_2>> TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 1, std::initializer_list<hud::pair<key_type_2, value_type_2>> TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 1, std::initializer_list<hud::pair<key_type_2, value_type_2>> TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 0, std::initializer_list<hud::pair<key_type_2, value_type_2>> TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 0, std::initializer_list<hud::pair<key_type_2, value_type_2>> TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 1, std::initializer_list<hud::pair<key_type_2, value_type_2>> TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 1, std::initializer_list<hud::pair<key_type_2, value_type_2>> TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2, 0, std::initializer_list<hud::pair<key_type_2, value_type_2>> TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2, 0, std::initializer_list<hud::pair<key_type_2, value_type_2>> TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2, 1, std::initializer_list<hud::pair<key_type_2, value_type_2>> TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2, 1, std::initializer_list<hud::pair<key_type_2, value_type_2>> TEST_VALUES, 1);
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
        static const auto test = [](std::initializer_list<hud::pair<key_type, value_type>> elements_in_assigned, std::initializer_list<hud::pair<key_type_2, value_type_2>> elements_to_assign) {
            using AllocatorType = hud_test::allocator_watcher<1>;
            using AllocatorType2 = hud_test::allocator_watcher_2<1>;
            using AssignedType = hud::hashmap<key_type, value_type, hud::hash_64<key_type>, hud::equal<key_type>, AllocatorType>;
            using ToAssignType = hud::hashmap<key_type_2, value_type_2, hud::hash_64<key_type>, hud::equal<key_type_2>, AllocatorType2>;
            AssignedType assigned(elements_in_assigned);
            ToAssignType to_assign(elements_to_assign);

            assigned = to_assign;

            // Ensure we copy all elements
            bool all_keys_and_values_copied = true;
            for (usize index = 0; index < elements_to_assign.size(); index++) {
                const auto &init_elem = (elements_to_assign.begin() + index);
                const auto it = assigned.find(static_cast<key_type>(init_elem->first));
                if (it == assigned.end()) {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->key() != init_elem->first) {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->key().copy_constructor_count() != 2) {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->value() != init_elem->second) {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->value().copy_constructor_count() != 2) {
                    all_keys_and_values_copied = false;
                    break;
                }
            }
            // Allocation count
            u32 expected_allocation_count = 0;
            bool to_assign_allocate = elements_to_assign.size() > 0;
            bool assigned_should_grow = elements_in_assigned.size() < elements_to_assign.size();
            if (to_assign_allocate) {
                expected_allocation_count++;
            }
            if (assigned_should_grow) {
                expected_allocation_count++;
            }
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if consteval {
                expected_allocation_count *= 2;
            }

            u32 expected_free_count = 0;
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if consteval {
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
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {}, std::initializer_list<hud::pair<key_type_2, value_type_2>> {});
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, std::initializer_list<hud::pair<key_type_2, value_type_2>> {});
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {}, std::initializer_list<hud::pair<key_type_2, value_type_2>> TEST_VALUES2);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, std::initializer_list<hud::pair<key_type_2, value_type_2>> TEST_VALUES2);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, std::initializer_list<hud::pair<key_type_2, value_type_2>> TEST_VALUES);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2, std::initializer_list<hud::pair<key_type_2, value_type_2>> TEST_VALUES);
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
        static const auto test = [](std::initializer_list<hud::pair<key_type, value_type>> elements_in_assigned, usize extra_assigned, std::initializer_list<hud::pair<key_type_2, value_type_2>> elements_to_assign, usize extra_to_assign) {
            using AllocatorType = hud_test::allocator_watcher<1>;
            using AllocatorType2 = hud_test::allocator_watcher_2<1>;
            using AssignedType = hud::hashmap<key_type, value_type, hud::hash_64<key_type>, hud::equal<key_type>, AllocatorType>;
            using ToAssignType = hud::hashmap<key_type_2, value_type_2, hud::hash_64<key_type>, hud::equal<key_type_2>, AllocatorType2>;
            AssignedType assigned(elements_in_assigned, extra_assigned);
            ToAssignType to_assign(elements_to_assign, extra_to_assign);

            assigned = to_assign;

            // Ensure we copy all elements
            bool all_keys_and_values_copied = true;
            for (usize index = 0; index < elements_to_assign.size(); index++) {
                const auto &init_elem = (elements_to_assign.begin() + index);
                const auto it = assigned.find(static_cast<key_type>(init_elem->first));
                if (it == assigned.end()) {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->key() != init_elem->first) {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->key().copy_constructor_count() != 2) {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->value() != init_elem->second) {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->value().copy_constructor_count() != 2) {
                    all_keys_and_values_copied = false;
                    break;
                }
            }
            // Allocation count
            u32 expected_allocation_count = 0;
            bool to_assign_allocate = elements_to_assign.size() > 0 || extra_to_assign > 0;
            bool assigned_should_grow = elements_in_assigned.size() < elements_to_assign.size();
            if (to_assign_allocate) {
                expected_allocation_count++;
            }
            if (assigned_should_grow) {
                expected_allocation_count++;
            }
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if consteval {
                expected_allocation_count *= 2;
            }

            u32 expected_free_count = 0;
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if consteval {
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
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {}, 0, std::initializer_list<hud::pair<key_type_2, value_type_2>> {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {}, 0, std::initializer_list<hud::pair<key_type_2, value_type_2>> {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {}, 1, std::initializer_list<hud::pair<key_type_2, value_type_2>> {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {}, 1, std::initializer_list<hud::pair<key_type_2, value_type_2>> {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 0, std::initializer_list<hud::pair<key_type_2, value_type_2>> {}, 0);

                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 0, std::initializer_list<hud::pair<key_type_2, value_type_2>> {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 1, std::initializer_list<hud::pair<key_type_2, value_type_2>> {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 1, std::initializer_list<hud::pair<key_type_2, value_type_2>> {}, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {}, 0, std::initializer_list<hud::pair<key_type_2, value_type_2>> TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {}, 0, std::initializer_list<hud::pair<key_type_2, value_type_2>> TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {}, 1, std::initializer_list<hud::pair<key_type_2, value_type_2>> TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {}, 1, std::initializer_list<hud::pair<key_type_2, value_type_2>> TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 0, std::initializer_list<hud::pair<key_type_2, value_type_2>> TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 0, std::initializer_list<hud::pair<key_type_2, value_type_2>> TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 1, std::initializer_list<hud::pair<key_type_2, value_type_2>> TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 1, std::initializer_list<hud::pair<key_type_2, value_type_2>> TEST_VALUES2, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 0, std::initializer_list<hud::pair<key_type_2, value_type_2>> TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 0, std::initializer_list<hud::pair<key_type_2, value_type_2>> TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 1, std::initializer_list<hud::pair<key_type_2, value_type_2>> TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 1, std::initializer_list<hud::pair<key_type_2, value_type_2>> TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }

            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2, 0, std::initializer_list<hud::pair<key_type_2, value_type_2>> TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2, 0, std::initializer_list<hud::pair<key_type_2, value_type_2>> TEST_VALUES, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2, 1, std::initializer_list<hud::pair<key_type_2, value_type_2>> TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
                hud_assert_true(std::get<4>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2, 1, std::initializer_list<hud::pair<key_type_2, value_type_2>> TEST_VALUES, 1);
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

GTEST_TEST(hashmap, copy_assign_hashmap_same_allocator_call_destructor_of_elements)
{
    using key_type = hud_test::SetBoolToTrueWhenDestroyed;
    using value_type = hud_test::SetBoolToTrueWhenDestroyed;

    // Test without extra
    {
        static const auto test = [](const usize count_in_assigned, const usize count_to_assigned) {
            i32 *dtor_assigned_key_counter = nullptr;
            i32 *dtor_assigned_value_counter = nullptr;

            if (count_in_assigned > 0) {
                dtor_assigned_key_counter = hud::memory::allocate_array<i32>(count_in_assigned);
                hud::memory::set_memory_zero_safe(dtor_assigned_key_counter, count_in_assigned * sizeof(i32));
                dtor_assigned_value_counter = hud::memory::allocate_array<i32>(count_in_assigned);
                hud::memory::set_memory_zero_safe(dtor_assigned_value_counter, count_in_assigned * sizeof(i32));
            }
            hud_test::LeakArrayGuard guard_assigned_key_counter(dtor_assigned_key_counter, count_in_assigned);
            hud_test::LeakArrayGuard guard_assigned_value_counter(dtor_assigned_value_counter, count_in_assigned);

            i32 *dtor_to_assigned_key_counter = nullptr;
            i32 *dtor_to_assigned_value_counter = nullptr;
            if (count_to_assigned > 0) {
                dtor_to_assigned_key_counter = hud::memory::allocate_array<i32>(count_to_assigned);
                hud::memory::set_memory_zero_safe(dtor_to_assigned_key_counter, count_to_assigned * sizeof(i32));
                dtor_to_assigned_value_counter = hud::memory::allocate_array<i32>(count_to_assigned);
                hud::memory::set_memory_zero_safe(dtor_to_assigned_value_counter, count_to_assigned * sizeof(i32));
            }
            hud_test::LeakArrayGuard guard_to_assigned_key_counter(dtor_to_assigned_key_counter, count_to_assigned);
            hud_test::LeakArrayGuard guard_to_assigned_value_counter(dtor_to_assigned_value_counter, count_to_assigned);

            using AllocatorType = hud_test::allocator_watcher<alignof(hud_test::SetBoolToTrueWhenDestroyed)>;
            using AssignedType = hud::hashmap<key_type, value_type, hud::hash_64<key_type>, hud::equal<key_type>, AllocatorType>;
            using ToAssignType = hud::hashmap<key_type, value_type, hud::hash_64<key_type>, hud::equal<key_type>, AllocatorType>;
            AssignedType assigned;
            assigned.reserve(count_in_assigned);
            for (i32 i = 0; i < count_in_assigned; i++) {
                assigned.add({i, dtor_assigned_key_counter + i}, {i, dtor_assigned_value_counter + i});
            }

            ToAssignType to_assign;
            to_assign.reserve(count_to_assigned);
            for (i32 i = 0; i < count_to_assigned; i++) {
                to_assign.add({i, dtor_to_assigned_key_counter + i}, {i, dtor_to_assigned_value_counter + i});
            }
            // Set all destructor to zero, to not count destruction that appears during add of temporary key and value type
            if (count_in_assigned > 0) {
                hud::memory::set_memory_zero_safe(dtor_assigned_key_counter, count_in_assigned * sizeof(i32));
                hud::memory::set_memory_zero_safe(dtor_assigned_value_counter, count_in_assigned * sizeof(i32));
            }
            if (count_to_assigned > 0) {
                hud::memory::set_memory_zero_safe(dtor_to_assigned_key_counter, count_to_assigned * sizeof(i32));
                hud::memory::set_memory_zero_safe(dtor_to_assigned_value_counter, count_to_assigned * sizeof(i32));
            }
            assigned = to_assign;

            // Ensure we destroy all elements that is in assigned
            bool all_destructors_are_called = true;
            for (usize index = 0; index < count_in_assigned; index++) {
                if (dtor_assigned_key_counter[index] != 1u) {
                    all_destructors_are_called = false;
                    break;
                }
                if (dtor_assigned_value_counter[index] != 1u) {
                    all_destructors_are_called = false;
                    break;
                }
            }

            // Ensure we don't destroy elements in to_assign
            bool to_assign_destructors_are_not_called = true;
            for (usize index = 0; index < count_to_assigned; index++) {
                if (dtor_to_assigned_key_counter[index] != 0u) {
                    to_assign_destructors_are_not_called = false;
                    break;
                }
                if (dtor_to_assigned_value_counter[index] != 0u) {
                    to_assign_destructors_are_not_called = false;
                    break;
                }
            }
            u32 expected_allocation_count = 0;
            bool in_assign_allocate = count_in_assigned > 0;
            bool assigned_should_grow = count_in_assigned < count_to_assigned;
            if (in_assign_allocate) {
                expected_allocation_count++;
            }
            if (assigned_should_grow) {
                expected_allocation_count++;
            }
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if consteval {
                expected_allocation_count *= 2;
            }

            u32 expected_free_count = 0;
            if (in_assign_allocate && assigned_should_grow) {
                expected_free_count++;
            }
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if consteval {
                expected_free_count *= 2;
            }

            return std::tuple {
                all_destructors_are_called,                                           // 0
                to_assign_destructors_are_not_called,                                 // 1
                assigned.allocator().allocation_count() == expected_allocation_count, // 2
                assigned.allocator().free_count() == expected_free_count              // 3
            };
        };

        // Non constant
        {
            {
                const auto result = runtime_test(test, 0, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 1, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 0, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 1, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 0, 16);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 16, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 16, 16);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 16, 10);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 10, 16);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
        }

        // Constant
        {
            {
                constexpr auto result = test(0, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                constexpr auto result = test(1, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                constexpr auto result = test(0, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                constexpr auto result = test(1, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                constexpr auto result = test(0, 16);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                constexpr auto result = test(16, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                constexpr auto result = test(0, 16);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                constexpr auto result = test(16, 16);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                constexpr auto result = test(16, 10);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                constexpr auto result = test(10, 16);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
        }
    }

    // Test with extra
    {
        static const auto test = [](const usize count_in_assigned, usize extra_in_assigned, const usize count_to_assigned, usize extra_to_assigned) {
            i32 *dtor_assigned_key_counter = nullptr;
            i32 *dtor_assigned_value_counter = nullptr;

            if (count_in_assigned > 0) {
                dtor_assigned_key_counter = hud::memory::allocate_array<i32>(count_in_assigned);
                hud::memory::set_memory_zero_safe(dtor_assigned_key_counter, count_in_assigned * sizeof(i32));
                dtor_assigned_value_counter = hud::memory::allocate_array<i32>(count_in_assigned);
                hud::memory::set_memory_zero_safe(dtor_assigned_value_counter, count_in_assigned * sizeof(i32));
            }
            hud_test::LeakArrayGuard guard_assigned_key_counter(dtor_assigned_key_counter, count_in_assigned);
            hud_test::LeakArrayGuard guard_assigned_value_counter(dtor_assigned_value_counter, count_in_assigned);

            i32 *dtor_to_assigned_key_counter = nullptr;
            i32 *dtor_to_assigned_value_counter = nullptr;
            if (count_to_assigned > 0) {
                dtor_to_assigned_key_counter = hud::memory::allocate_array<i32>(count_to_assigned);
                hud::memory::set_memory_zero_safe(dtor_to_assigned_key_counter, count_to_assigned * sizeof(i32));
                dtor_to_assigned_value_counter = hud::memory::allocate_array<i32>(count_to_assigned);
                hud::memory::set_memory_zero_safe(dtor_to_assigned_value_counter, count_to_assigned * sizeof(i32));
            }
            hud_test::LeakArrayGuard guard_to_assigned_key_counter(dtor_to_assigned_key_counter, count_to_assigned);
            hud_test::LeakArrayGuard guard_to_assigned_value_counter(dtor_to_assigned_value_counter, count_to_assigned);

            using AllocatorType = hud_test::allocator_watcher<alignof(hud_test::SetBoolToTrueWhenDestroyed)>;
            using AssignedType = hud::hashmap<key_type, value_type, hud::hash_64<key_type>, hud::equal<key_type>, AllocatorType>;
            using ToAssignType = hud::hashmap<key_type, value_type, hud::hash_64<key_type>, hud::equal<key_type>, AllocatorType>;
            AssignedType assigned;
            assigned.reserve(count_in_assigned + extra_in_assigned);
            for (i32 i = 0; i < count_in_assigned; i++) {
                assigned.add({i, dtor_assigned_key_counter + i}, {i, dtor_assigned_value_counter + i});
            }

            ToAssignType to_assign;
            to_assign.reserve(count_to_assigned + extra_to_assigned);
            for (i32 i = 0; i < count_to_assigned; i++) {
                to_assign.add({i, dtor_to_assigned_key_counter + i}, {i, dtor_to_assigned_value_counter + i});
            }
            // Set all destructor to zero, to not count destruction that appears during add of temporary key and value type
            if (count_in_assigned > 0) {
                hud::memory::set_memory_zero_safe(dtor_assigned_key_counter, count_in_assigned * sizeof(i32));
                hud::memory::set_memory_zero_safe(dtor_assigned_value_counter, count_in_assigned * sizeof(i32));
            }
            if (count_to_assigned > 0) {
                hud::memory::set_memory_zero_safe(dtor_to_assigned_key_counter, count_to_assigned * sizeof(i32));
                hud::memory::set_memory_zero_safe(dtor_to_assigned_value_counter, count_to_assigned * sizeof(i32));
            }

            assigned = to_assign;

            // Ensure we destroy all elements that is in assigned
            bool all_destructors_are_called = true;
            for (usize index = 0; index < count_in_assigned; index++) {
                if (dtor_assigned_key_counter[index] != 1u) {
                    all_destructors_are_called = false;
                    break;
                }
                if (dtor_assigned_value_counter[index] != 1u) {
                    all_destructors_are_called = false;
                    break;
                }
            }

            // Ensure we don't destroy elements in to_assign
            bool to_assign_destructors_are_not_called = true;
            for (usize index = 0; index < count_to_assigned; index++) {
                if (dtor_to_assigned_key_counter[index] != 0u) {
                    to_assign_destructors_are_not_called = false;
                    break;
                }
                if (dtor_to_assigned_value_counter[index] != 0u) {
                    to_assign_destructors_are_not_called = false;
                    break;
                }
            }
            u32 expected_allocation_count = 0;
            bool in_assign_allocate = count_in_assigned > 0 || extra_in_assigned > 0;
            bool assigned_should_grow = count_in_assigned + extra_in_assigned < count_to_assigned;
            if (in_assign_allocate) {
                expected_allocation_count++;
            }
            if (assigned_should_grow) {
                expected_allocation_count++;
            }
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if consteval {
                expected_allocation_count *= 2;
            }

            u32 expected_free_count = 0;
            if (in_assign_allocate && assigned_should_grow) {
                expected_free_count++;
            }
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if consteval {
                expected_free_count *= 2;
            }

            return std::tuple {
                all_destructors_are_called,                                           // 0
                to_assign_destructors_are_not_called,                                 // 1
                assigned.allocator().allocation_count() == expected_allocation_count, // 2
                assigned.allocator().free_count() == expected_free_count              // 3
            };
        };

        // Non constant
        {
            {
                const auto result = runtime_test(test, 0, 0, 0, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 0, 10, 0, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 0, 0, 0, 10);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 0, 10, 0, 10);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 1, 0, 0, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 1, 10, 0, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 1, 0, 0, 10);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 1, 10, 0, 10);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 0, 0, 1, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 0, 10, 1, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 0, 0, 1, 10);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 0, 10, 1, 10);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 1, 0, 1, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 1, 0, 1, 10);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 1, 10, 1, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 1, 10, 1, 10);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 0, 0, 16, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 0, 10, 16, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 0, 0, 16, 10);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 0, 10, 16, 10);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 16, 0, 0, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 16, 10, 0, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 16, 0, 0, 10);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 16, 10, 0, 10);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 16, 0, 16, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 16, 10, 16, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 16, 0, 16, 10);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 16, 10, 16, 10);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 16, 0, 10, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 16, 10, 10, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 16, 0, 10, 10);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 16, 10, 10, 10);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 10, 0, 16, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 10, 10, 16, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 10, 0, 16, 10);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 10, 10, 16, 10);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
        }

        // Constant
        {
            {
                constexpr auto result = test(0, 0, 0, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                constexpr auto result = test(0, 10, 0, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                constexpr auto result = test(0, 0, 0, 10);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                constexpr auto result = test(0, 10, 0, 10);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }

            {
                constexpr auto result = test(1, 0, 0, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                constexpr auto result = test(1, 10, 0, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                constexpr auto result = test(1, 0, 0, 10);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                constexpr auto result = test(1, 10, 0, 10);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }

            {
                constexpr auto result = test(0, 0, 1, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                constexpr auto result = test(0, 10, 1, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                constexpr auto result = test(0, 0, 1, 10);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                constexpr auto result = test(0, 10, 1, 10);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }

            {
                constexpr auto result = test(1, 0, 1, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                constexpr auto result = test(1, 0, 1, 10);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                constexpr auto result = test(1, 10, 1, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                constexpr auto result = test(1, 10, 1, 10);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }

            {
                constexpr auto result = test(0, 0, 16, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                constexpr auto result = test(0, 10, 16, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }

            {
                constexpr auto result = test(0, 0, 16, 10);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                constexpr auto result = test(0, 10, 16, 10);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }

            {
                constexpr auto result = test(16, 0, 0, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                constexpr auto result = test(16, 10, 0, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                constexpr auto result = test(16, 0, 0, 10);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                constexpr auto result = test(16, 10, 0, 10);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }

            {
                constexpr auto result = test(0, 0, 16, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                constexpr auto result = test(0, 10, 16, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                constexpr auto result = test(0, 0, 16, 10);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                constexpr auto result = test(0, 10, 16, 10);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }

            {
                constexpr auto result = test(16, 0, 16, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                constexpr auto result = test(16, 10, 16, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                constexpr auto result = test(16, 0, 16, 10);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                constexpr auto result = test(16, 10, 16, 10);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }

            {
                constexpr auto result = test(16, 0, 10, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                constexpr auto result = test(16, 10, 10, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                constexpr auto result = test(16, 0, 10, 10);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                constexpr auto result = test(16, 10, 10, 10);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }

            {
                constexpr auto result = test(10, 0, 16, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                constexpr auto result = test(10, 10, 16, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                constexpr auto result = test(10, 0, 16, 10);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                constexpr auto result = test(10, 10, 16, 10);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
        }
    }
}

GTEST_TEST(hashmap, copy_assign_hashmap_different_allocator_call_destructor_of_elements)
{
    using key_type = hud_test::SetBoolToTrueWhenDestroyed;
    using value_type = hud_test::SetBoolToTrueWhenDestroyed;

    // Test without extra
    {
        static const auto test = [](const usize count_in_assigned, const usize count_to_assigned) {
            i32 *dtor_assigned_key_counter = nullptr;
            i32 *dtor_assigned_value_counter = nullptr;

            if (count_in_assigned > 0) {
                dtor_assigned_key_counter = hud::memory::allocate_array<i32>(count_in_assigned);
                hud::memory::set_memory_zero_safe(dtor_assigned_key_counter, count_in_assigned * sizeof(i32));
                dtor_assigned_value_counter = hud::memory::allocate_array<i32>(count_in_assigned);
                hud::memory::set_memory_zero_safe(dtor_assigned_value_counter, count_in_assigned * sizeof(i32));
            }
            hud_test::LeakArrayGuard guard_assigned_key_counter(dtor_assigned_key_counter, count_in_assigned);
            hud_test::LeakArrayGuard guard_assigned_value_counter(dtor_assigned_value_counter, count_in_assigned);

            i32 *dtor_to_assigned_key_counter = nullptr;
            i32 *dtor_to_assigned_value_counter = nullptr;
            if (count_to_assigned > 0) {
                dtor_to_assigned_key_counter = hud::memory::allocate_array<i32>(count_to_assigned);
                hud::memory::set_memory_zero_safe(dtor_to_assigned_key_counter, count_to_assigned * sizeof(i32));
                dtor_to_assigned_value_counter = hud::memory::allocate_array<i32>(count_to_assigned);
                hud::memory::set_memory_zero_safe(dtor_to_assigned_value_counter, count_to_assigned * sizeof(i32));
            }
            hud_test::LeakArrayGuard guard_to_assigned_key_counter(dtor_to_assigned_key_counter, count_to_assigned);
            hud_test::LeakArrayGuard guard_to_assigned_value_counter(dtor_to_assigned_value_counter, count_to_assigned);

            using AllocatorType = hud_test::allocator_watcher<alignof(hud_test::SetBoolToTrueWhenDestroyed)>;
            using AllocatorType2 = hud_test::allocator_watcher_2<alignof(hud_test::SetBoolToTrueWhenDestroyed)>;
            using AssignedType = hud::hashmap<key_type, value_type, hud::hash_64<key_type>, hud::equal<key_type>, AllocatorType>;
            using ToAssignType = hud::hashmap<key_type, value_type, hud::hash_64<key_type>, hud::equal<key_type>, AllocatorType2>;
            AssignedType assigned;
            assigned.reserve(count_in_assigned);
            for (i32 i = 0; i < count_in_assigned; i++) {
                assigned.add({i, dtor_assigned_key_counter + i}, {i, dtor_assigned_value_counter + i});
            }

            ToAssignType to_assign;
            to_assign.reserve(count_to_assigned);
            for (i32 i = 0; i < count_to_assigned; i++) {
                to_assign.add({i, dtor_to_assigned_key_counter + i}, {i, dtor_to_assigned_value_counter + i});
            }
            // Set all destructor to zero, to not count destruction that appears during add of temporary key and value type
            if (count_in_assigned > 0) {
                hud::memory::set_memory_zero_safe(dtor_assigned_key_counter, count_in_assigned * sizeof(i32));
                hud::memory::set_memory_zero_safe(dtor_assigned_value_counter, count_in_assigned * sizeof(i32));
            }
            if (count_to_assigned > 0) {
                hud::memory::set_memory_zero_safe(dtor_to_assigned_key_counter, count_to_assigned * sizeof(i32));
                hud::memory::set_memory_zero_safe(dtor_to_assigned_value_counter, count_to_assigned * sizeof(i32));
            }

            assigned = to_assign;

            // Ensure we destroy all elements that is in assigned
            bool all_destructors_are_called = true;
            for (usize index = 0; index < count_in_assigned; index++) {
                if (dtor_assigned_key_counter[index] != 1u) {
                    all_destructors_are_called = false;
                    break;
                }
                if (dtor_assigned_value_counter[index] != 1u) {
                    all_destructors_are_called = false;
                    break;
                }
            }

            // Ensure we don't destroy elements in to_assign
            bool to_assign_destructors_are_not_called = true;
            for (usize index = 0; index < count_to_assigned; index++) {
                if (dtor_to_assigned_key_counter[index] != 0u) {
                    to_assign_destructors_are_not_called = false;
                    break;
                }
                if (dtor_to_assigned_value_counter[index] != 0u) {
                    to_assign_destructors_are_not_called = false;
                    break;
                }
            }

            u32 expected_allocation_count = 0;
            bool to_assign_allocate = count_to_assigned > 0;
            bool assigned_should_grow = count_in_assigned < count_to_assigned;
            if (to_assign_allocate) {
                expected_allocation_count++;
            }
            if (assigned_should_grow) {
                expected_allocation_count++;
            }
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if consteval {
                expected_allocation_count *= 2;
            }

            u32 expected_free_count = 0;
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if consteval {
                expected_free_count *= 2;
            }

            return std::tuple {
                all_destructors_are_called,                                           // 0
                to_assign_destructors_are_not_called,                                 // 1
                assigned.allocator().allocation_count() == expected_allocation_count, // 2
                assigned.allocator().free_count() == expected_free_count              // 3
            };
        };

        // Non constant
        {
            {
                const auto result = runtime_test(test, 0, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 1, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 0, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 1, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 0, 16);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 16, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 0, 16);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 16, 16);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 16, 10);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 10, 16);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
        }

        // Constant
        {
            {
                constexpr auto result = test(0, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                constexpr auto result = test(1, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                constexpr auto result = test(0, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                constexpr auto result = test(1, 1);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                constexpr auto result = test(0, 16);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                constexpr auto result = test(16, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                constexpr auto result = test(0, 16);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                constexpr auto result = test(16, 16);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                constexpr auto result = test(16, 10);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                constexpr auto result = test(10, 16);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
        }
    }

    // Test with extra
    {
        static const auto test = [](const usize count_in_assigned, usize extra_in_assigned, const usize count_to_assigned, usize extra_to_assigned) {
            i32 *dtor_assigned_key_counter = nullptr;
            i32 *dtor_assigned_value_counter = nullptr;

            if (count_in_assigned > 0) {
                dtor_assigned_key_counter = hud::memory::allocate_array<i32>(count_in_assigned);
                hud::memory::set_memory_zero_safe(dtor_assigned_key_counter, count_in_assigned * sizeof(i32));
                dtor_assigned_value_counter = hud::memory::allocate_array<i32>(count_in_assigned);
                hud::memory::set_memory_zero_safe(dtor_assigned_value_counter, count_in_assigned * sizeof(i32));
            }
            hud_test::LeakArrayGuard guard_assigned_key_counter(dtor_assigned_key_counter, count_in_assigned);
            hud_test::LeakArrayGuard guard_assigned_value_counter(dtor_assigned_value_counter, count_in_assigned);

            i32 *dtor_to_assigned_key_counter = nullptr;
            i32 *dtor_to_assigned_value_counter = nullptr;
            if (count_to_assigned > 0) {
                dtor_to_assigned_key_counter = hud::memory::allocate_array<i32>(count_to_assigned);
                hud::memory::set_memory_zero_safe(dtor_to_assigned_key_counter, count_to_assigned * sizeof(i32));
                dtor_to_assigned_value_counter = hud::memory::allocate_array<i32>(count_to_assigned);
                hud::memory::set_memory_zero_safe(dtor_to_assigned_value_counter, count_to_assigned * sizeof(i32));
            }
            hud_test::LeakArrayGuard guard_to_assigned_key_counter(dtor_to_assigned_key_counter, count_to_assigned);
            hud_test::LeakArrayGuard guard_to_assigned_value_counter(dtor_to_assigned_value_counter, count_to_assigned);

            using AllocatorType = hud_test::allocator_watcher<alignof(hud_test::SetBoolToTrueWhenDestroyed)>;
            using AllocatorType2 = hud_test::allocator_watcher_2<alignof(hud_test::SetBoolToTrueWhenDestroyed)>;
            using AssignedType = hud::hashmap<key_type, value_type, hud::hash_64<key_type>, hud::equal<key_type>, AllocatorType>;
            using ToAssignType = hud::hashmap<key_type, value_type, hud::hash_64<key_type>, hud::equal<key_type>, AllocatorType2>;
            AssignedType assigned;
            assigned.reserve(count_in_assigned + extra_in_assigned);
            for (i32 i = 0; i < count_in_assigned; i++) {
                assigned.add({i, dtor_assigned_key_counter + i}, {i, dtor_assigned_value_counter + i});
            }

            ToAssignType to_assign;
            to_assign.reserve(count_to_assigned + extra_to_assigned);
            for (i32 i = 0; i < count_to_assigned; i++) {
                to_assign.add({i, dtor_to_assigned_key_counter + i}, {i, dtor_to_assigned_value_counter + i});
            }
            // Set all destructor to zero, to not count destruction that appears during add of temporary key and value type
            if (count_in_assigned > 0) {
                hud::memory::set_memory_zero_safe(dtor_assigned_key_counter, count_in_assigned * sizeof(i32));
                hud::memory::set_memory_zero_safe(dtor_assigned_value_counter, count_in_assigned * sizeof(i32));
            }
            if (count_to_assigned > 0) {
                hud::memory::set_memory_zero_safe(dtor_to_assigned_key_counter, count_to_assigned * sizeof(i32));
                hud::memory::set_memory_zero_safe(dtor_to_assigned_value_counter, count_to_assigned * sizeof(i32));
            }

            assigned = to_assign;

            // Ensure we destroy all elements that is in assigned
            bool all_destructors_are_called = true;
            for (usize index = 0; index < count_in_assigned; index++) {
                if (dtor_assigned_key_counter[index] != 1u) {
                    all_destructors_are_called = false;
                    break;
                }
                if (dtor_assigned_value_counter[index] != 1u) {
                    all_destructors_are_called = false;
                    break;
                }
            }

            // Ensure we don't destroy elements in to_assign
            bool to_assign_destructors_are_not_called = true;
            for (usize index = 0; index < count_to_assigned; index++) {
                if (dtor_to_assigned_key_counter[index] != 0u) {
                    to_assign_destructors_are_not_called = false;
                    break;
                }
                if (dtor_to_assigned_value_counter[index] != 0u) {
                    to_assign_destructors_are_not_called = false;
                    break;
                }
            }

            // Allocation count
            u32 expected_allocation_count = 0;
            bool to_assign_allocate = count_to_assigned > 0 || extra_to_assigned > 0;
            bool assigned_should_grow = (count_in_assigned + extra_in_assigned) < count_to_assigned;
            if (to_assign_allocate) {
                expected_allocation_count++;
            }
            if (assigned_should_grow) {
                expected_allocation_count++;
            }
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if consteval {
                expected_allocation_count *= 2;
            }
            u32 expected_free_count = 0;
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if consteval {
                expected_free_count *= 2;
            }

            return std::tuple {
                all_destructors_are_called,                                           // 0
                to_assign_destructors_are_not_called,                                 // 1
                assigned.allocator().allocation_count() == expected_allocation_count, // 2
                assigned.allocator().free_count() == expected_free_count              // 3
            };
        };

        // Non constant
        {
            {
                const auto result = runtime_test(test, 0, 0, 0, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 0, 10, 0, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 0, 0, 0, 10);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 0, 10, 0, 10);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 1, 0, 0, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 1, 10, 0, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 1, 0, 0, 10);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 1, 10, 0, 10);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 0, 0, 1, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 0, 10, 1, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 0, 0, 1, 10);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 0, 10, 1, 10);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 1, 0, 1, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 1, 0, 1, 10);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 1, 10, 1, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 1, 10, 1, 10);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 0, 0, 16, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 0, 10, 16, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 0, 0, 16, 10);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 0, 10, 16, 10);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 16, 0, 0, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 16, 10, 0, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 16, 0, 0, 10);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 16, 10, 0, 10);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 0, 0, 16, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 0, 10, 16, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 0, 0, 16, 10);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 0, 10, 16, 10);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 16, 0, 16, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 16, 10, 16, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 16, 0, 16, 10);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 16, 10, 16, 10);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 16, 0, 10, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 16, 10, 10, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 16, 0, 10, 10);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 16, 10, 10, 10);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 10, 0, 16, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 10, 10, 16, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 10, 0, 16, 10);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                const auto result = runtime_test(test, 10, 10, 16, 10);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
        }

        // Constant
        {
            {
                constexpr auto result = test(0, 0, 0, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                constexpr auto result = test(0, 10, 0, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                constexpr auto result = test(0, 0, 0, 10);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                constexpr auto result = test(0, 10, 0, 10);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }

            {
                constexpr auto result = test(1, 0, 0, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                constexpr auto result = test(1, 10, 0, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                constexpr auto result = test(1, 0, 0, 10);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                constexpr auto result = test(1, 10, 0, 10);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }

            {
                constexpr auto result = test(0, 0, 1, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                constexpr auto result = test(0, 10, 1, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                constexpr auto result = test(0, 0, 1, 10);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                constexpr auto result = test(0, 10, 1, 10);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }

            {
                constexpr auto result = test(1, 0, 1, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                constexpr auto result = test(1, 0, 1, 10);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                constexpr auto result = test(1, 10, 1, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                constexpr auto result = test(1, 10, 1, 10);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }

            {
                constexpr auto result = test(0, 0, 16, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                constexpr auto result = test(0, 10, 16, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }

            {
                constexpr auto result = test(0, 0, 16, 10);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                constexpr auto result = test(0, 10, 16, 10);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }

            {
                constexpr auto result = test(16, 0, 0, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                constexpr auto result = test(16, 10, 0, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                constexpr auto result = test(16, 0, 0, 10);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                constexpr auto result = test(16, 10, 0, 10);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }

            {
                constexpr auto result = test(0, 0, 16, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                constexpr auto result = test(0, 10, 16, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                constexpr auto result = test(0, 0, 16, 10);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                constexpr auto result = test(0, 10, 16, 10);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }

            {
                constexpr auto result = test(16, 0, 16, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                constexpr auto result = test(16, 10, 16, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                constexpr auto result = test(16, 0, 16, 10);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                constexpr auto result = test(16, 10, 16, 10);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }

            {
                constexpr auto result = test(16, 0, 10, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                constexpr auto result = test(16, 10, 10, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                constexpr auto result = test(16, 0, 10, 10);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                constexpr auto result = test(16, 10, 10, 10);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }

            {
                constexpr auto result = test(10, 0, 16, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                constexpr auto result = test(10, 10, 16, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                constexpr auto result = test(10, 0, 16, 10);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
            {
                constexpr auto result = test(10, 10, 16, 10);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
                hud_assert_true(std::get<3>(result));
            }
        }
    }
}

GTEST_TEST(vector, copy_assign_hashmap_of_bitwise_copy_assignable_to_self)
{
    using key_type = i32;
    using value_type = u64;

    static_assert(hud::is_bitwise_copy_assignable_v<key_type>);
    static_assert(hud::is_bitwise_copy_assignable_v<value_type>);

    // Test without extra
    {
        static const auto test = [](std::initializer_list<hud::pair<key_type, value_type>> elements_in_assigned) {
            using AllocatorType = hud_test::allocator_watcher<1>;
            using AssignedType = hud::hashmap<key_type, value_type, hud::hash_64<key_type>, hud::equal<key_type>, AllocatorType>;
            AssignedType assigned(elements_in_assigned);

            // assigned = assigned;

            // Ensure we copy all elements
            bool all_keys_and_values_copied = true;
            for (usize index = 0; index < elements_in_assigned.size(); index++) {
                const auto &init_elem = (elements_in_assigned.begin() + index);
                const auto it = assigned.find(init_elem->first);
                if (it == assigned.end()) {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->key() != init_elem->first) {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->value() != init_elem->second) {
                    all_keys_and_values_copied = false;
                    break;
                }
            }
            // Allocation count
            u32 expected_allocation_count = 0;
            bool assigned_allocate = elements_in_assigned.size() > 0;
            if (assigned_allocate) {
                expected_allocation_count++;
            }

            // If we are in constant evaluated the allocation is done in 2 separated memory
            if consteval {
                expected_allocation_count *= 2;
            }

            u32 expected_free_count = 0;
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if consteval {
                expected_free_count *= 2;
            }
            return std::tuple {
                all_keys_and_values_copied,                                           // 0
                assigned.allocator().allocation_count() == expected_allocation_count, // 1
                assigned.allocator().free_count() == expected_free_count              // 2
            };
        };

        // Non constant
        {
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {});
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
            }
        }

        // Constant
        {
            {
                constexpr auto result = test({});
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES2);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
            }
        }
    }

    // Test with extra
    {
        static const auto test = [](std::initializer_list<hud::pair<key_type, value_type>> elements_in_assigned, usize extra_assigned) {
            using AllocatorType = hud_test::allocator_watcher<1>;
            using AssignedType = hud::hashmap<key_type, value_type, hud::hash_64<key_type>, hud::equal<key_type>, AllocatorType>;
            AssignedType assigned(elements_in_assigned, extra_assigned);

            assigned = assigned;

            // Ensure we copy all elements
            bool all_keys_and_values_copied = true;
            for (usize index = 0; index < elements_in_assigned.size(); index++) {
                const auto &init_elem = (elements_in_assigned.begin() + index);
                const auto it = assigned.find(init_elem->first);
                if (it == assigned.end()) {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->key() != init_elem->first) {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->value() != init_elem->second) {
                    all_keys_and_values_copied = false;
                    break;
                }
            }
            // Allocation count
            u32 expected_allocation_count = 0;
            bool assigned_allocate = elements_in_assigned.size() > 0 || extra_assigned > 0;
            if (assigned_allocate) {
                expected_allocation_count++;
            }

            // If we are in constant evaluated the allocation is done in 2 separated memory
            if consteval {
                expected_allocation_count *= 2;
            }

            u32 expected_free_count = 0;
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if consteval {
                expected_free_count *= 2;
            }
            return std::tuple {
                all_keys_and_values_copied,                                           // 0
                assigned.allocator().allocation_count() == expected_allocation_count, // 1
                assigned.allocator().free_count() == expected_free_count              // 2
            };
        };

        // Non constant
        {
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {}, 15);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES, 15);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2, 15);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
            }
        }

        // Constant
        {
            {
                constexpr auto result = test({}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
            }
            {
                constexpr auto result = test({}, 15);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, 15);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
            }

            {
                constexpr auto result = test(TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES2, 15);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
            }
        }
    }
}

GTEST_TEST(vector, copy_assign_hashmap_of_non_bitwise_copy_assignable_to_self)
{
    using key_type = hud_test::non_bitwise_type;
    using value_type = hud_test::non_bitwise_type;

    static_assert(!hud::is_bitwise_copy_assignable_v<key_type>);
    static_assert(!hud::is_bitwise_copy_assignable_v<value_type>);

    // Test without extra
    {
        static const auto test = [](std::initializer_list<hud::pair<key_type, value_type>> elements_in_assigned) {
            using AllocatorType = hud_test::allocator_watcher<1>;
            using AssignedType = hud::hashmap<key_type, value_type, hud::hash_64<key_type>, hud::equal<key_type>, AllocatorType>;
            AssignedType assigned(elements_in_assigned);

            assigned = assigned;

            // Ensure we copy all elements
            bool all_keys_and_values_copied = true;
            for (usize index = 0; index < elements_in_assigned.size(); index++) {
                const auto &init_elem = (elements_in_assigned.begin() + index);
                const auto it = assigned.find(init_elem->first);
                if (it == assigned.end()) {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->key() != init_elem->first) {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->key().constructor_count() != 1) {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->key().copy_constructor_count() != 1) {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->key().copy_assign_count() != 0) {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->key().move_constructor_count() != 0) {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->key().move_assign_count() != 0) {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->value() != init_elem->second) {
                    all_keys_and_values_copied = false;
                    break;
                }
            }
            // Allocation count
            u32 expected_allocation_count = 0;
            bool assigned_allocate = elements_in_assigned.size() > 0;
            if (assigned_allocate) {
                expected_allocation_count++;
            }

            // If we are in constant evaluated the allocation is done in 2 separated memory
            if consteval {
                expected_allocation_count *= 2;
            }

            u32 expected_free_count = 0;
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if consteval {
                expected_free_count *= 2;
            }
            return std::tuple {
                all_keys_and_values_copied,                                           // 0
                assigned.allocator().allocation_count() == expected_allocation_count, // 1
                assigned.allocator().free_count() == expected_free_count              // 2
            };
        };

        // Non constant
        {
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> {});
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
            }
            {
                const auto result = runtime_test(test, std::initializer_list<hud::pair<key_type, value_type>> TEST_VALUES2);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
            }
        }

        // Constant
        {
            {
                constexpr auto result = test({});
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES2);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
            }
        }
    }

    // Test with extra
    {
        static const auto test = [](std::initializer_list<hud::pair<key_type, value_type>> elements_in_assigned, usize extra_assigned) {
            using AllocatorType = hud_test::allocator_watcher<1>;
            using AssignedType = hud::hashmap<key_type, value_type, hud::hash_64<key_type>, hud::equal<key_type>, AllocatorType>;
            AssignedType assigned(elements_in_assigned, extra_assigned);

            assigned = assigned;

            // Ensure we copy all elements
            bool all_keys_and_values_copied = true;
            for (usize index = 0; index < elements_in_assigned.size(); index++) {
                const auto &init_elem = (elements_in_assigned.begin() + index);
                const auto it = assigned.find(init_elem->first);
                if (it == assigned.end()) {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->key() != init_elem->first) {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->key().constructor_count() != 1) {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->key().copy_constructor_count() != 1) {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->key().copy_assign_count() != 0) {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->key().move_constructor_count() != 0) {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->key().move_assign_count() != 0) {
                    all_keys_and_values_copied = false;
                    break;
                }
                if (it->value() != init_elem->second) {
                    all_keys_and_values_copied = false;
                    break;
                }
            }
            // Allocation count
            u32 expected_allocation_count = 0;
            bool assigned_allocate = elements_in_assigned.size() > 0 || extra_assigned > 0;
            if (assigned_allocate) {
                expected_allocation_count++;
            }

            // If we are in constant evaluated the allocation is done in 2 separated memory
            if consteval {
                expected_allocation_count *= 2;
            }

            u32 expected_free_count = 0;
            // If we are in constant evaluated the allocation is done in 2 separated memory
            if consteval {
                expected_free_count *= 2;
            }
            return std::tuple {
                all_keys_and_values_copied,                                           // 0
                assigned.allocator().allocation_count() == expected_allocation_count, // 1
                assigned.allocator().free_count() == expected_free_count              // 2
            };
        };

        // Non constant
        {
            {
                const auto result = test({}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
            }
            {
                const auto result = test({}, 15);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
            }
            {
                const auto result = test(TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
            }
            {
                const auto result = test(TEST_VALUES, 15);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
            }

            {
                const auto result = test(TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
            }
            {
                const auto result = test(TEST_VALUES2, 15);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
            }
        }

        // Constant
        {
            {
                constexpr auto result = test({}, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
            }
            {
                constexpr auto result = test({}, 15);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES, 15);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
            }

            {
                constexpr auto result = test(TEST_VALUES2, 0);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
            }
            {
                constexpr auto result = test(TEST_VALUES2, 15);
                hud_assert_true(std::get<0>(result));
                hud_assert_true(std::get<1>(result));
                hud_assert_true(std::get<2>(result));
            }
        }
    }
}