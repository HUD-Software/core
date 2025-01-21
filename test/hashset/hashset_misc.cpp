#include <core/containers/hashset.h>
#include <core/traits/is_same.h>

GTEST_TEST(hashset, hashset_value_type_is_correct)
{
    hud_assert_true((hud::is_same_v<i64, hud::hashset<i64>::key_type>));
    hud_assert_true((hud::is_same_v<const i64, hud::hashset<const i64>::key_type>));
}