#include <core/traits/has_unique_object_representations.h>

namespace hud_test
{
    struct empty
    {
    };

    struct a
    {
        i32 a;
    };

    struct b
    {
        i32 a;
        f32 f;
    };

    struct c
    {
        i32 a;
        i16 f;
        i16 g;
    };

    struct padded
    {
        u8 p;
        // Padding of 3 bytes
        u32 a;
    };

    struct derived : a
    {
    };

    struct derived2 : a
    {
        virtual void fn()
        {
        }
    };
} // namespace hud_test

GTEST_TEST(traits, HasUniqueObjectRepresentations)
{

    hud_assert_true(hud::has_unique_object_representations_v<i32>);
    hud_assert_true(hud::has_unique_object_representations_v<i32 *>);
    hud_assert_false(hud::has_unique_object_representations_v<i32 &>);
    hud_assert_true(hud::has_unique_object_representations_v<i32[]>);
    hud_assert_true(hud::has_unique_object_representations_v<i32[][2]>);
    hud_assert_true(hud::has_unique_object_representations_v<i32[2]>);
    hud_assert_true(hud::has_unique_object_representations_v<i32[2][2]>);
    hud_assert_false(hud::has_unique_object_representations_v<hud::void_t<>>);
    hud_assert_false(hud::has_unique_object_representations_v<f32>);
    hud_assert_false(hud::has_unique_object_representations_v<f64>);

    hud_assert_false(hud::has_unique_object_representations_v<hud_test::empty>);
    hud_assert_true(hud::has_unique_object_representations_v<hud_test::a>);
    hud_assert_false(hud::has_unique_object_representations_v<hud_test::b>);
    hud_assert_true(hud::has_unique_object_representations_v<hud_test::c>);

    hud_assert_false(hud::has_unique_object_representations_v<hud_test::padded>);

    hud_assert_true(hud::has_unique_object_representations_v<hud_test::derived>);

    hud_assert_false(hud::has_unique_object_representations_v<hud_test::derived2>);
}