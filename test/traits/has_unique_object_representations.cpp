#include <core/traits/has_unique_object_representations.h>

namespace hud_test {
    struct empty {};
    struct a {
        i32 a;
    };
    struct b {
        i32 a;
        f32 f;
    };
    struct c {
        i32 a;
        i16 f;
        i16 g;
    };
    struct padded {
        u8 p;
        // Padding of 3 bytes
        u32 a;
    };
    struct derived : a {};

    struct derived2 : a {
        virtual void fn() {}
    };
}

GTEST_TEST(traits, HasUniqueObjectRepresentations) {


    GTEST_ASSERT_TRUE(hud::has_unique_object_representations_v<i32>);
    GTEST_ASSERT_TRUE(hud::has_unique_object_representations_v<i32*>);
    GTEST_ASSERT_FALSE(hud::has_unique_object_representations_v<i32&>);
    GTEST_ASSERT_TRUE(hud::has_unique_object_representations_v<i32[]>);
    GTEST_ASSERT_TRUE(hud::has_unique_object_representations_v<i32[][2]>);
    GTEST_ASSERT_TRUE(hud::has_unique_object_representations_v<i32[2]>);
    GTEST_ASSERT_TRUE(hud::has_unique_object_representations_v<i32[2][2]>);
    GTEST_ASSERT_FALSE(hud::has_unique_object_representations_v<hud::void_t<>>);
    GTEST_ASSERT_FALSE(hud::has_unique_object_representations_v<f32>);
    GTEST_ASSERT_FALSE(hud::has_unique_object_representations_v<f64>);

    
    GTEST_ASSERT_FALSE(hud::has_unique_object_representations_v<hud_test::empty>);
    GTEST_ASSERT_TRUE(hud::has_unique_object_representations_v<hud_test::a>);
    GTEST_ASSERT_FALSE(hud::has_unique_object_representations_v<hud_test::b>);
    GTEST_ASSERT_TRUE(hud::has_unique_object_representations_v<hud_test::c>);

    
    GTEST_ASSERT_FALSE(hud::has_unique_object_representations_v<hud_test::padded>);

    
    GTEST_ASSERT_TRUE(hud::has_unique_object_representations_v<hud_test::derived>);

    
    GTEST_ASSERT_FALSE(hud::has_unique_object_representations_v<hud_test::derived2>);
}