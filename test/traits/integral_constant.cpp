#include <core/traits/integral_constant.h>

GTEST_TEST(traits, integral_constant) {
    
    using ict = hud::integral_constant<bool, true>;
    ict ic_bool_true;
    GTEST_ASSERT_TRUE(ic_bool_true.value);
    GTEST_ASSERT_TRUE(ic_bool_true());
    GTEST_ASSERT_TRUE(static_cast<bool>(ic_bool_true));
    GTEST_ASSERT_TRUE((hud::is_same_v<ict::value_type, bool>));
    GTEST_ASSERT_TRUE((hud::is_same_v<ict::type, ict>));

    using icf = hud::integral_constant<bool, false>;
    icf ic_bool_false;
    GTEST_ASSERT_FALSE(ic_bool_false.value);
    GTEST_ASSERT_FALSE(ic_bool_false());
    GTEST_ASSERT_FALSE(static_cast<bool>(ic_bool_false));
    GTEST_ASSERT_TRUE((hud::is_same_v<icf::value_type, bool>));
    GTEST_ASSERT_TRUE((hud::is_same_v<icf::type, icf>));

    using icone = hud::integral_constant<i32, 1>;
    icone ic_int32_one;
    GTEST_ASSERT_TRUE(ic_int32_one.value);
    GTEST_ASSERT_TRUE(ic_int32_one());
    GTEST_ASSERT_TRUE(static_cast<bool>(ic_int32_one));
    GTEST_ASSERT_TRUE((hud::is_same_v<icone::value_type, i32>));
    GTEST_ASSERT_TRUE((hud::is_same_v<icone::type, icone>));

    using iczero = hud::integral_constant<i32, 0>;
    iczero ic_int32_zero;
    GTEST_ASSERT_FALSE(ic_int32_zero.value);
    GTEST_ASSERT_FALSE(ic_int32_zero());
    GTEST_ASSERT_FALSE(static_cast<bool>(ic_int32_zero));
    GTEST_ASSERT_TRUE((hud::is_same_v<iczero::value_type, i32>));
    GTEST_ASSERT_TRUE((hud::is_same_v<iczero::type, iczero>));

    GTEST_ASSERT_TRUE(hud::bool_constant<true>::value);
    GTEST_ASSERT_FALSE(hud::bool_constant<false>::value);
    GTEST_ASSERT_FALSE(hud::false_type::value);
    GTEST_ASSERT_TRUE(hud::true_type::value);
}