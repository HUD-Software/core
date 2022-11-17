#include <core/templates/lexicographical_compare.h>

GTEST_TEST(templates, lexicographicalCompare) {

    i32 value[3] = { 1,2,3 };
    i32 value2[3] = { 1,2,4 };
    i32 value3[3] = { 1,2,5 };
    i32 value4[4] = { 1,2,3,5 };

    i8 value_i8[3] = { 1,2,3 };
    i8 value2_i8[3] = { 1,2,4 };
    i8 value3_i8[3] = { 1,2,5 };
    i8 value4_i8[4] = { 1,2,3,5 };

    // bool lexicographical_compare(It1 first_1, It1 last_1, It2 first_2, It2 last_2)
    GTEST_ASSERT_FALSE(hud::lexicographical_compare(value, value + 3, value, value + 3));
    GTEST_ASSERT_TRUE(hud::lexicographical_compare(value, value + 3, value2, value2 + 3));
    GTEST_ASSERT_TRUE(hud::lexicographical_compare(value, value + 3, value3, value3 + 3));
    GTEST_ASSERT_TRUE(hud::lexicographical_compare(value, value + 3, value4, value4 + 4));

    GTEST_ASSERT_FALSE(hud::lexicographical_compare(value2, value2 + 3, value, value + 3));
    GTEST_ASSERT_FALSE(hud::lexicographical_compare(value2, value2 + 3, value2, value2 + 3));
    GTEST_ASSERT_TRUE(hud::lexicographical_compare(value2, value2 + 3, value3, value3 + 3));
    GTEST_ASSERT_FALSE(hud::lexicographical_compare(value2, value2 + 3, value4, value4 + 4));

    GTEST_ASSERT_FALSE(hud::lexicographical_compare(value3, value3 + 3, value, value + 3));
    GTEST_ASSERT_FALSE(hud::lexicographical_compare(value3, value3 + 3, value2, value2 + 3));
    GTEST_ASSERT_FALSE(hud::lexicographical_compare(value3, value3 + 3, value3, value3 + 3));
    GTEST_ASSERT_FALSE(hud::lexicographical_compare(value3, value3 + 3, value4, value4 + 4));

    GTEST_ASSERT_FALSE(hud::lexicographical_compare(value4, value4 + 4, value, value + 3));
    GTEST_ASSERT_TRUE(hud::lexicographical_compare(value4, value4 + 4, value2, value2 + 3));
    GTEST_ASSERT_TRUE(hud::lexicographical_compare(value4, value4 + 4, value3, value3 + 3));
    GTEST_ASSERT_FALSE(hud::lexicographical_compare(value4, value4 + 4, value4, value4 + 4));

    // bool lexicographical_compare(It1* first_1, It1* last_1, It2* first_2, It2* last_2) noexcept requires(sizeof(It1) == 1 && sizeof(It2) == 1) 
    GTEST_ASSERT_FALSE(hud::lexicographical_compare(value_i8, value_i8 + 3, value_i8, value_i8 + 3));
    GTEST_ASSERT_TRUE(hud::lexicographical_compare(value_i8, value_i8 + 3, value2_i8, value2_i8 + 3));
    GTEST_ASSERT_TRUE(hud::lexicographical_compare(value_i8, value_i8 + 3, value3_i8, value3_i8 + 3));
    GTEST_ASSERT_TRUE(hud::lexicographical_compare(value_i8, value_i8 + 3, value4_i8, value4_i8 + 4));

    GTEST_ASSERT_FALSE(hud::lexicographical_compare(value2_i8, value2_i8 + 3, value_i8, value_i8 + 3));
    GTEST_ASSERT_FALSE(hud::lexicographical_compare(value2_i8, value2_i8 + 3, value2_i8, value2_i8 + 3));
    GTEST_ASSERT_TRUE(hud::lexicographical_compare(value2_i8, value2_i8 + 3, value3_i8, value3_i8 + 3));
    GTEST_ASSERT_FALSE(hud::lexicographical_compare(value2_i8, value2_i8 + 3, value4_i8, value4_i8 + 4));

    GTEST_ASSERT_FALSE(hud::lexicographical_compare(value3_i8, value3_i8 + 3, value_i8, value_i8 + 3));
    GTEST_ASSERT_FALSE(hud::lexicographical_compare(value3_i8, value3_i8 + 3, value2_i8, value2_i8 + 3));
    GTEST_ASSERT_FALSE(hud::lexicographical_compare(value3_i8, value3_i8 + 3, value3_i8, value3_i8 + 3));
    GTEST_ASSERT_FALSE(hud::lexicographical_compare(value3_i8, value3_i8 + 3, value4_i8, value4_i8 + 4));

    GTEST_ASSERT_FALSE(hud::lexicographical_compare(value4_i8, value4_i8 + 4, value_i8, value_i8 + 3));
    GTEST_ASSERT_TRUE(hud::lexicographical_compare(value4_i8, value4_i8 + 4, value2_i8, value2_i8 + 3));
    GTEST_ASSERT_TRUE(hud::lexicographical_compare(value4_i8, value4_i8 + 4, value3_i8, value3_i8 + 3));
    GTEST_ASSERT_FALSE(hud::lexicographical_compare(value4_i8, value4_i8 + 4, value4_i8, value4_i8 + 4));

    // bool lexicographical_compare(const Arr1(&arr1)[size1], const Arr2(&arr2)[size2]) noexcept
    GTEST_ASSERT_FALSE(hud::lexicographical_compare(value, value));
    GTEST_ASSERT_TRUE(hud::lexicographical_compare(value, value2));
    GTEST_ASSERT_TRUE(hud::lexicographical_compare(value, value3));
    GTEST_ASSERT_TRUE(hud::lexicographical_compare(value, value4));

    GTEST_ASSERT_FALSE(hud::lexicographical_compare(value2, value));
    GTEST_ASSERT_FALSE(hud::lexicographical_compare(value2, value2));
    GTEST_ASSERT_TRUE(hud::lexicographical_compare(value2, value3));
    GTEST_ASSERT_FALSE(hud::lexicographical_compare(value2, value4));

    GTEST_ASSERT_FALSE(hud::lexicographical_compare(value3, value));
    GTEST_ASSERT_FALSE(hud::lexicographical_compare(value3, value2));
    GTEST_ASSERT_FALSE(hud::lexicographical_compare(value3, value3));
    GTEST_ASSERT_FALSE(hud::lexicographical_compare(value3, value4));

    GTEST_ASSERT_FALSE(hud::lexicographical_compare(value4, value));
    GTEST_ASSERT_TRUE(hud::lexicographical_compare(value4, value2));
    GTEST_ASSERT_TRUE(hud::lexicographical_compare(value4, value3));
    GTEST_ASSERT_FALSE(hud::lexicographical_compare(value4, value4));

    // bool lexicographical_compare(const Arr1(&arr1)[size], const Arr2(&arr2)[size]) noexcept requires(sizeof(Arr1) == 1 && sizeof(Arr2) == 1)
    GTEST_ASSERT_FALSE(hud::lexicographical_compare(value_i8, value_i8));
    GTEST_ASSERT_TRUE(hud::lexicographical_compare(value_i8, value2_i8));
    GTEST_ASSERT_TRUE(hud::lexicographical_compare(value_i8, value3_i8));

    GTEST_ASSERT_FALSE(hud::lexicographical_compare(value2_i8, value_i8));
    GTEST_ASSERT_FALSE(hud::lexicographical_compare(value2_i8, value2_i8));
    GTEST_ASSERT_TRUE(hud::lexicographical_compare(value2_i8, value3_i8));


    GTEST_ASSERT_FALSE(hud::lexicographical_compare(value3_i8, value_i8));
    GTEST_ASSERT_FALSE(hud::lexicographical_compare(value3_i8, value2_i8));
    GTEST_ASSERT_FALSE(hud::lexicographical_compare(value3_i8, value3_i8));

    GTEST_ASSERT_FALSE(hud::lexicographical_compare(value4_i8, value4_i8));

    // bool lexicographical_compare(const Arr1(&arr1)[size1], const Arr2(&arr2)[size2]) noexcept
    GTEST_ASSERT_TRUE(hud::lexicographical_compare(value_i8, value4_i8));
    GTEST_ASSERT_FALSE(hud::lexicographical_compare(value2_i8, value4_i8));
    GTEST_ASSERT_FALSE(hud::lexicographical_compare(value3_i8, value4_i8));
    GTEST_ASSERT_FALSE(hud::lexicographical_compare(value4_i8, value_i8));
    GTEST_ASSERT_TRUE(hud::lexicographical_compare(value4_i8, value2_i8));
    GTEST_ASSERT_TRUE(hud::lexicographical_compare(value4_i8, value3_i8));

}