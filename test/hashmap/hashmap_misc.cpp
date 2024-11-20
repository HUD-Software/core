#include <core/containers/hashmap.h>
#include <core/traits/is_same.h>

GTEST_TEST(hashmap, hashmap_value_type_is_correct)
{
    hud_assert_true((hud::is_same_v<i64, hud::hashmap<i64, i32>::key_type>));
    hud_assert_true((hud::is_same_v<i32, hud::hashmap<i64, i32>::value_type>));

    hud_assert_true((hud::is_same_v<const i64, hud::hashmap<const i64, i32>::key_type>));
    hud_assert_true((hud::is_same_v<const i32, hud::hashmap<i64, const i32>::value_type>));
}

// Create a metadata with 2 posible groups
// alignas(16) static constexpr hud::details::hashmap::metadata::byte_type metadata_array[hud::details::hashmap::metadata::group_type::SLOT_PER_GROUP * 2] {
//     // First group
//     hud::details::hashmap::metadata::byte_type {(hud::details::hashmap::metadata::byte_type)0x6D}, // 0 => LSB of the mask
//     hud::details::hashmap::metadata::byte_type {(hud::details::hashmap::metadata::byte_type)0x2A},
//     hud::details::hashmap::metadata::byte_type {0},
//     hud::details::hashmap::metadata::deleted_byte,
//     hud::details::hashmap::metadata::byte_type {(hud::details::hashmap::metadata::byte_type)0x6D}, // 4
//     hud::details::hashmap::metadata::empty_byte,
//     hud::details::hashmap::metadata::byte_type {0},
//     hud::details::hashmap::metadata::byte_type {(hud::details::hashmap::metadata::byte_type)0x7F}, // 7 => MSB of the mask
//     // Second group
//     hud::details::hashmap::metadata::deleted_byte, // 8 => LSB of the mask
//     hud::details::hashmap::metadata::byte_type {0},
//     hud::details::hashmap::metadata::byte_type {0},
//     hud::details::hashmap::metadata::empty_byte,
//     hud::details::hashmap::metadata::byte_type {0}, // 12
//     hud::details::hashmap::metadata::byte_type {(hud::details::hashmap::metadata::byte_type)0x7B},
//     hud::details::hashmap::metadata::empty_byte,
//     hud::details::hashmap::metadata::sentinel_byte // 16 => MSB of the mask
// };

GTEST_TEST(hashmap, metadata)
{
    // Testing metadata byte filtering
    using metadata_type = hud::details::hashmap::metadata;
    hud_assert_true(metadata_type::is_byte_empty(metadata_type::empty_byte));
    hud_assert_false(metadata_type::is_byte_empty(metadata_type::deleted_byte));
    hud_assert_false(metadata_type::is_byte_empty(metadata_type::sentinel_byte));
    hud_assert_false(metadata_type::is_byte_empty(0x7F));

    hud_assert_false(metadata_type::is_byte_deleted(metadata_type::empty_byte));
    hud_assert_true(metadata_type::is_byte_deleted(metadata_type::deleted_byte));
    hud_assert_false(metadata_type::is_byte_deleted(metadata_type::sentinel_byte));
    hud_assert_false(metadata_type::is_byte_deleted(0x7F));

    hud_assert_true(metadata_type::is_byte_empty_or_deleted(metadata_type::empty_byte));
    hud_assert_true(metadata_type::is_byte_empty_or_deleted(metadata_type::deleted_byte));
    hud_assert_false(metadata_type::is_byte_empty_or_deleted(metadata_type::sentinel_byte));
    hud_assert_false(metadata_type::is_byte_empty_or_deleted(0x7F));

    hud_assert_false(metadata_type::is_byte_full(metadata_type::empty_byte));
    hud_assert_false(metadata_type::is_byte_full(metadata_type::deleted_byte));
    hud_assert_false(metadata_type::is_byte_full(metadata_type::sentinel_byte));
    hud_assert_true(metadata_type::is_byte_full(0x7F));

    // Testing metadata group
    using group_type = metadata_type::group_type;
    using mask_type = group_type::mask;
    using mask_empty_type = group_type::empty_mask;
    using mask_empty_or_deleted_type = group_type::empty_or_deleted_mask;

    u64 group_value = 0x80FEFF7F80FEFF7F;
    group_type g {reinterpret_cast<metadata_type::byte_type *>(&group_value)};
    hud_assert_eq(g.match(0x7F), mask_type {0x0000008000000080});
    hud_assert_eq(g.mask_of_empty_or_deleted_slot(), mask_empty_or_deleted_type {0x8080000080800000});
    hud_assert_eq(g.mask_of_empty_slot(), mask_empty_type {0x8000000080000000});
    hud_assert_eq(g.mask_of_full_slot(), 0x0000008000000080);

    // Test group at index
    u64 two_group[2] = {0x7F00806DFE002A6D, 0x807B00800000FEFF};
    metadata_type meta(reinterpret_cast<metadata_type::byte_type *>(&two_group) /*const_cast<metadata_type::byte_type *>(&metadata_array[0])*/);
    group_type g0 = meta.group_of_slot_index(group_type::SLOT_PER_GROUP * 0);
    // Read first group
    hud_assert_eq(g0.match(0x7F), mask_type {0x8000000000000000});
    hud_assert_eq(g0.match(0x2A), mask_type {0x0000000000008000});
    hud_assert_eq(g0.match(0x6D), mask_type {0x0000008000000000});
    hud_assert_eq(g0.mask_of_empty_or_deleted_slot(), mask_empty_or_deleted_type {0x0000800080000000});
    hud_assert_eq(g0.mask_of_empty_slot(), mask_empty_type {0x0000800000000000});
    hud_assert_eq(g0.mask_of_full_slot(), 0x8080008000808080);

    group_type g1 = meta.group_of_slot_index(group_type::SLOT_PER_GROUP * 1);
    // Read second group
    hud_assert_eq(g1.match(0x7B), mask_type {0x0080000000000000});
    hud_assert_eq(g1.mask_of_empty_or_deleted_slot(), mask_empty_or_deleted_type {0x8000008000008000});
    hud_assert_eq(g1.mask_of_empty_slot(), mask_empty_type {0x8000008000000000});
    hud_assert_eq(g1.mask_of_full_slot(), 0x0080800080800000);

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