#include <core/containers/hashset.h>
#include "../misc/allocator_watcher.h"

GTEST_TEST(hashset, destructor_call_elements_destructors)
{

    using key_type = hud_test::non_bitwise_type;
    using hashset_type = hud::hashset<key_type>;

    const auto test = []()
    {
        hashset_type set;

        const usize COUNT = 4;
        i32 *dtor_assigned_key_counter = nullptr;

        dtor_assigned_key_counter = hud::memory::allocate_array<i32>(COUNT);
        hud::memory::set_memory_zero_safe(dtor_assigned_key_counter, COUNT * sizeof(i32));

        hud_test::LeakArrayGuard guard_assigned_key_counter(dtor_assigned_key_counter, COUNT);

        bool all_keys_destructor_are_not_called = true;

        {
            hashset_type set;
            set.reserve(COUNT);
            for (i32 i = 0; i < COUNT; i++)
            {
                set.add(hud::tag_piecewise_construct, hud::forward_as_tuple(i, dtor_assigned_key_counter + i));
            }

            // Ensure element's destructors are not called
            for (i32 i = 0; i < COUNT; i++)
            {
                const auto it = set.find(i);
                if (*it->key().destructor_counter() != 0)
                {
                    all_keys_destructor_are_not_called = false;
                }
            }
        }

        return std::tuple {
            all_keys_destructor_are_not_called, // 0
        };
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
    }
}