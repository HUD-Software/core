#include <core/containers/hashmap.h>
#include "../misc/allocator_watcher.h"

GTEST_TEST(hashmap, destructor_call_elements_destructors)
{

    using key_type = hud_test::non_bitwise_type;
    using value_type = hud_test::non_bitwise_type;
    using hashmap_type = hud::hashmap<key_type, value_type>;

    const auto test = []() {
        hashmap_type map;

        const usize COUNT = 4;
        i32 *dtor_assigned_key_counter = nullptr;
        i32 *dtor_assigned_value_counter = nullptr;

        dtor_assigned_key_counter = hud::memory::allocate_array<i32>(COUNT);
        hud::memory::set_memory_zero_safe(dtor_assigned_key_counter, COUNT * sizeof(i32));
        dtor_assigned_value_counter = hud::memory::allocate_array<i32>(COUNT);
        hud::memory::set_memory_zero_safe(dtor_assigned_value_counter, COUNT * sizeof(i32));

        hud_test::LeakArrayGuard guard_assigned_key_counter(dtor_assigned_key_counter, COUNT);
        hud_test::LeakArrayGuard guard_assigned_value_counter(dtor_assigned_value_counter, COUNT);

        bool all_keys_destructor_are_not_called = true;
        bool all_values_destructor_are_not_called = true;
        {
            hashmap_type map;
            map.reserve(COUNT);
            for (i32 i = 0; i < COUNT; i++) {
                map.add(hud::tag_piecewise_construct, hud::forward_as_tuple(i, dtor_assigned_key_counter + i), hud::forward_as_tuple(i, dtor_assigned_value_counter + i));
            }

            // Ensure element's destructors are not called
            for (i32 i = 0; i < COUNT; i++) {
                const auto it = map.find(i);
                if (*it->key().destructor_counter() != 0) {
                    all_keys_destructor_are_not_called = false;
                }
                if (*it->value().destructor_counter() != 0) {
                    all_values_destructor_are_not_called = false;
                }
            }
        }

        return std::tuple {
            all_keys_destructor_are_not_called,  // 0
            all_values_destructor_are_not_called // 1
        };
    };

    // Non constant
    {
        const auto result = runtime_test(test);
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