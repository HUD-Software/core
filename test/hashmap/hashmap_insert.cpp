#include <core/containers/hashmap.h>
#include "../misc/allocator_watcher.h"

GTEST_TEST(hashmap, add_by_copy_construct_bitwise_copy_constructible_type)
{
    using key_type = usize;
    using value_type = usize;
    using hashmap_type = hud::hashmap<key_type, value_type, hud::hashmap_default_hasher<key_type>, hud::hashmap_default_key_equal<key_type>, hud_test::allocator_watcher<1>>;

    static_assert(hud::is_bitwise_copy_constructible_v<key_type, value_type>);

    const auto test = []()
    {
        hashmap_type map;

        const auto it_0 = map.add(1, 11);

        const auto first_element_result = std::tuple {
            *it_0,
            map.count(),
            map.max_count(),
            map.allocator().allocation_count(),
            map.allocator().free_count(),
        };

        // // Add same element
        // const u64 value_0 = map.add(1, 11);
        // const auto second_element_result = std::tuple {
        //     index_1,
        //     array.data() != nullptr,
        //     array.count(),
        //     array.max_count(),
        //     array[0].copy_assign_count(),
        //     array[0].copy_constructor_count(),
        //     array[1].copy_assign_count(),
        //     array[1].copy_constructor_count(),
        //     array.allocator().allocation_count(),
        //     array.allocator().free_count()
        // };

        // // Add 2nd element
        // const u64 value_0 = map.add(1, 22);
        // const auto second_element_result = std::tuple {
        //     index_1,
        //     array.data() != nullptr,
        //     array.count(),
        //     array.max_count(),
        //     array[0].copy_assign_count(),
        //     array[0].copy_constructor_count(),
        //     array[1].copy_assign_count(),
        //     array[1].copy_constructor_count(),
        //     array.allocator().allocation_count(),
        //     array.allocator().free_count()
        // };

        return std::tuple {
            first_element_result
        };
    };

    constexpr auto result = test();

    // Non Constant
    {
    }
    // Constant
    {
    }
    // hud::hashmap<i32, u64> map;
    // hud_assert_eq(map.insert_to_ref(1, 11), 11);
    // u64 res1 = map.insert_to_ref(2, 22);
    // res = map.insert_to_ref(3, 33);
    // }