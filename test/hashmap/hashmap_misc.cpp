#include <core/containers/hashmap.h>
#include <core/traits/is_same.h>

GTEST_TEST(hashmap, hashmap_value_type_is_correct)
{
    hud_assert_true((hud::is_same_v<i64, hud::hashmap<i64, i32>::key_type>));
    hud_assert_true((hud::is_same_v<i32, hud::hashmap<i64, i32>::value_type>));

    hud_assert_true((hud::is_same_v<const i64, hud::hashmap<const i64, i32>::key_type>));
    hud_assert_true((hud::is_same_v<const i32, hud::hashmap<i64, const i32>::value_type>));
}

GTEST_TEST(hashmap, count_return_count_of_element)
{
    using type = usize;
    using HashMapType = hud::hashmap<type, type>;

    const auto test = []()
    {
        HashMapType map;

        // Count return 0 if array is empty
        bool empty_ok = map.count() == 0;

        // Count return number of element if not empty
        constexpr usize COUNT = 256;
        for (usize value = 0; value < COUNT; value++)
        {
            map.add(value, value);
        }
        bool return_correct_count = map.count() == COUNT;

        // Count return number of element after remove
        // Remove half
        for (usize value = 0; value < COUNT; value += 2)
        {
            map.remove(value);
        }
        bool return_correct_count_after_erase_some = (map.count() == COUNT / 2);
        return std::tuple {empty_ok, return_correct_count, return_correct_count_after_erase_some};
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
    }
}

GTEST_TEST(hashmap, sizeof_map_is_correct)
{
    // Size of hashmap do not depends of the key and value types
    hud_assert_true(sizeof(hud::hashmap<i32, i32>) == sizeof(hud::hashmap<i64, i64>));

    // hashmp is compressed, all empty element like allocator, comparator, hasher must be 0 size
    // This test must be updated if the member of hashmap is changed
    constexpr usize control_ptr_size = sizeof(void *);
    constexpr usize slot_ptr_size = sizeof(void *);
    constexpr usize count_size = sizeof(usize);
    constexpr usize max_count_size = sizeof(usize);
    constexpr usize free_slot_before_grow_size = sizeof(usize);

    constexpr usize sizeof_map = sizeof(hud::hashmap<i32, i32>);
    hud_assert_true(sizeof_map == control_ptr_size + slot_ptr_size + count_size + max_count_size + free_slot_before_grow_size);
}
