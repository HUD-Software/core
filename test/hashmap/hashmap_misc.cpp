#include <core/containers/hashmap.h>
#include <core/traits/is_same.h>

GTEST_TEST(hashmap, hashmap_value_type_is_correct)
{
    hud_assert_true((hud::is_same_v<i64, hud::hashmap<i64, i32>::key_type>));
    hud_assert_true((hud::is_same_v<i32, hud::hashmap<i64, i32>::value_type>));

    hud_assert_true((hud::is_same_v<const i64, hud::hashmap<const i64, i32>::key_type>));
    hud_assert_true((hud::is_same_v<const i32, hud::hashmap<i64, const i32>::value_type>));
}

GTEST_TEST(hashmap, metadata)
{
    // Testing metadata byte filtering
    using control_type = hud::details::hashset::control_type;
    hud_assert_true(hud::details::hashset::control::is_byte_empty(hud::details::hashset::empty_byte));
    hud_assert_false(hud::details::hashset::control::is_byte_empty(hud::details::hashset::deleted_byte));
    hud_assert_false(hud::details::hashset::control::is_byte_empty(hud::details::hashset::sentinel_byte));
    hud_assert_false(hud::details::hashset::control::is_byte_empty(0x7F));

    hud_assert_false(hud::details::hashset::control::is_byte_deleted(hud::details::hashset::empty_byte));
    hud_assert_true(hud::details::hashset::control::is_byte_deleted(hud::details::hashset::deleted_byte));
    hud_assert_false(hud::details::hashset::control::is_byte_deleted(hud::details::hashset::sentinel_byte));
    hud_assert_false(hud::details::hashset::control::is_byte_deleted(0x7F));

    hud_assert_true(hud::details::hashset::control::is_byte_empty_or_deleted(hud::details::hashset::empty_byte));
    hud_assert_true(hud::details::hashset::control::is_byte_empty_or_deleted(hud::details::hashset::deleted_byte));
    hud_assert_false(hud::details::hashset::control::is_byte_empty_or_deleted(hud::details::hashset::sentinel_byte));
    hud_assert_false(hud::details::hashset::control::is_byte_empty_or_deleted(0x7F));

    hud_assert_false(hud::details::hashset::control::is_byte_full(hud::details::hashset::empty_byte));
    hud_assert_false(hud::details::hashset::control::is_byte_full(hud::details::hashset::deleted_byte));
    hud_assert_false(hud::details::hashset::control::is_byte_full(hud::details::hashset::sentinel_byte));
    hud_assert_true(hud::details::hashset::control::is_byte_full(0x7F));

    // Testing metadata group
    using group_type = hud::details::hashset::group_type;
    using mask_type = group_type::mask;
    using mask_empty_type = group_type::empty_mask;
    using mask_empty_or_deleted_type = group_type::empty_or_deleted_mask;
    using mask_full_type = group_type::full_mask;

    u64 group_value = 0x80FEFF7F80FEFF7F;
    group_type g {reinterpret_cast<control_type *>(&group_value)};
    hud_assert_eq(g.match(0x7F), mask_type {0x0000008000000080});
    hud_assert_eq(g.mask_of_empty_or_deleted_slot(), mask_empty_or_deleted_type {0x8080000080800000});
    hud_assert_eq(g.mask_of_empty_slot(), mask_empty_type {0x8000000080000000});
    hud_assert_eq(g.mask_of_full_slot(), mask_full_type {0x0000008000000080});

    // Test group at index
    // empty (0x80), deleted (0xFE), sentinel (0xFF)
    u64 two_group[2] = {0x7F00806DFE002A6D, 0x807B00800000FEFF};
    control_type *metadata_ptr(reinterpret_cast<control_type *>(&two_group));
    group_type g0 {metadata_ptr};
    // Read first group
    hud_assert_eq(g0.match(0x7F), mask_type {0x8000000000000000});
    hud_assert_eq(g0.match(0x2A), mask_type {0x0000000000008000});
    hud_assert_eq(g0.match(0x6D), mask_type {0x0000008000000080});
    hud_assert_eq(g0.mask_of_empty_or_deleted_slot(), mask_empty_or_deleted_type {0x0000800080000000});
    hud_assert_eq(g0.mask_of_empty_slot(), mask_empty_type {0x0000800000000000});
    hud_assert_eq(g0.mask_of_full_slot(), mask_full_type {0x8080008000808080});

    group_type g1 {metadata_ptr + group_type::SLOT_PER_GROUP * 1};
    // Read second group
    hud_assert_eq(g1.match(0x7B), mask_type {0x0080000000000000});
    hud_assert_eq(g1.mask_of_empty_or_deleted_slot(), mask_empty_or_deleted_type {0x8000008000008000});
    hud_assert_eq(g1.mask_of_empty_slot(), mask_empty_type {0x8000008000000000});
    hud_assert_eq(g1.mask_of_full_slot(), mask_full_type {0x0080800080800000});

    // Test find with group and iteration
    // Find the 2 indices of 0x6D in the group and iterate over it
    // Expect to have index 0 and 4 in the group 0x7F00806DFE002A6D
    group_type::mask h2_match_mask = g0.match(0x6D);
    u32 indices[2] = {hud::u32_max, hud::u32_max};
    u32 current_index = 0;
    for (u32 value : h2_match_mask)
    {
        hud_assert_ne(current_index, 2);
        indices[current_index] = value;
        ++current_index;
    }
    hud_assert_eq(indices[0], 0);
    hud_assert_eq(indices[1], 4);
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
