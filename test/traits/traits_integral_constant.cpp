#include <core/traits/integral_constant.h>

GTEST_TEST(traits, integral_constant)
{

    using ict = hud::integral_constant<bool, true>;
    ict ic_bool_true;
    hud_assert_true(ic_bool_true.value);
    hud_assert_true(ic_bool_true());
    hud_assert_true(static_cast<bool>(ic_bool_true));
    hud_assert_true((hud::is_same_v<ict::value_type, bool>));
    hud_assert_true((hud::is_same_v<ict::type, ict>));

    using icf = hud::integral_constant<bool, false>;
    icf ic_bool_false;
    hud_assert_false(ic_bool_false.value);
    hud_assert_false(ic_bool_false());
    hud_assert_false(static_cast<bool>(ic_bool_false));
    hud_assert_true((hud::is_same_v<icf::value_type, bool>));
    hud_assert_true((hud::is_same_v<icf::type, icf>));

    using icone = hud::integral_constant<i32, 1>;
    icone ic_int32_one;
    hud_assert_true(ic_int32_one.value);
    hud_assert_true(ic_int32_one());
    hud_assert_true(static_cast<bool>(ic_int32_one));
    hud_assert_true((hud::is_same_v<icone::value_type, i32>));
    hud_assert_true((hud::is_same_v<icone::type, icone>));

    using iczero = hud::integral_constant<i32, 0>;
    iczero ic_int32_zero;
    hud_assert_false(ic_int32_zero.value);
    hud_assert_false(ic_int32_zero());
    hud_assert_false(static_cast<bool>(ic_int32_zero));
    hud_assert_true((hud::is_same_v<iczero::value_type, i32>));
    hud_assert_true((hud::is_same_v<iczero::type, iczero>));

    hud_assert_true(hud::bool_constant<true>::value);
    hud_assert_false(hud::bool_constant<false>::value);
    hud_assert_false(hud::false_type::value);
    hud_assert_true(hud::true_type::value);
}