#include <core/containers/hashmap.h>
#include <core/traits/is_same.h>

GTEST_TEST(hashmap, hashmap_value_type_is_correct)
{
    hud_assert_true((hud::is_same_v<i64, hud::hashmap<i64, i32>::key_type>));
    hud_assert_true((hud::is_same_v<i32, hud::hashmap<i64, i32>::value_type>));

    hud_assert_true((hud::is_same_v<const i64, hud::hashmap<const i64, i32>::key_type>));
    hud_assert_true((hud::is_same_v<const i32, hud::hashmap<i64, const i32>::value_type>));
}