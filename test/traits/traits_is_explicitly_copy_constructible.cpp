#include <core/traits/is_explicitly_copy_constructible.h>

namespace hud_test
{
    // Implicit copy constructor same type
    struct no_explicit_ctor
    {
        no_explicit_ctor(const no_explicit_ctor &) noexcept {};
    };

    struct no_explicit_ctor_2
    {
        no_explicit_ctor_2(const no_explicit_ctor_2 &) noexcept {};
        explicit no_explicit_ctor_2(no_explicit_ctor_2 &&) noexcept {};
    };

    struct no_explicit_ctor_3
    {
        no_explicit_ctor_3(const no_explicit_ctor_3 &) noexcept {};
        no_explicit_ctor_3(no_explicit_ctor_3 &&) noexcept {};
    };

    // Explicit copy constructor same type
    struct explicit_ctor
    {
        explicit explicit_ctor(const explicit_ctor &) noexcept {};
    };

    struct explicit_ctor_2
    {
        explicit explicit_ctor_2(const explicit_ctor_2 &) noexcept {};
        explicit explicit_ctor_2(explicit_ctor_2 &&) noexcept {};
    };

    struct explicit_ctor_3
    {
        explicit explicit_ctor_3(const explicit_ctor_3 &) noexcept {};
        explicit_ctor_3(explicit_ctor_3 &&) noexcept {};
    };

    // Implicit copy constructor other explicit copy ctor type
    struct no_explicit_cross_explicit_ctor
    {
        no_explicit_cross_explicit_ctor(const explicit_ctor &) noexcept {};
    };

    struct no_explicit_cross_explicit_ctor_2
    {
        no_explicit_cross_explicit_ctor_2(const explicit_ctor &) noexcept {};
        no_explicit_cross_explicit_ctor_2(explicit_ctor &&) noexcept {};
    };

    struct no_explicit_cross_explicit_ctor_3
    {
        no_explicit_cross_explicit_ctor_3(const explicit_ctor &) noexcept {};
        explicit no_explicit_cross_explicit_ctor_3(explicit_ctor &&) noexcept {};
    };

    // Explicit copy constructor other explicit copy ctor type
    struct explicit_cross_explicit_ctor
    {
        explicit explicit_cross_explicit_ctor(const explicit_ctor &) noexcept {};
    };

    struct explicit_cross_explicit_ctor_2
    {
        explicit explicit_cross_explicit_ctor_2(const explicit_ctor &) noexcept {};
        explicit explicit_cross_explicit_ctor_2(explicit_ctor &&) noexcept {};
    };

    struct explicit_cross_explicit_ctor_3
    {
        explicit explicit_cross_explicit_ctor_3(const explicit_ctor &) noexcept {};
        explicit explicit_cross_explicit_ctor_3(explicit_ctor &&) noexcept {};
    };

    // Implicit copy constructor other implicit copy ctor type
    struct no_explicit_cross_no_explicit_ctor
    {
        no_explicit_cross_no_explicit_ctor(const no_explicit_ctor &) noexcept {};
    };

    struct no_explicit_cross_no_explicit_ctor_2
    {
        no_explicit_cross_no_explicit_ctor_2(const no_explicit_ctor &) noexcept {};
        no_explicit_cross_no_explicit_ctor_2(no_explicit_ctor &&) noexcept {};
    };

    struct no_explicit_cross_no_explicit_ctor_3
    {
        no_explicit_cross_no_explicit_ctor_3(const no_explicit_ctor &) noexcept {};
        explicit no_explicit_cross_no_explicit_ctor_3(no_explicit_ctor &&) noexcept {};
    };

    // Implicit copy constructor other implicit copy ctor type
    struct explicit_cross_implicit_ctor
    {
        explicit explicit_cross_implicit_ctor(const no_explicit_ctor &) noexcept {};
    };

    struct explicit_cross_implicit_ctor_2
    {
        explicit explicit_cross_implicit_ctor_2(const no_explicit_ctor &) noexcept {};
        explicit explicit_cross_implicit_ctor_2(no_explicit_ctor &&) noexcept {};
    };

    struct explicit_cross_implicit_ctor_3
    {
        explicit explicit_cross_implicit_ctor_3(const no_explicit_ctor &) noexcept {};
        explicit explicit_cross_implicit_ctor_3(no_explicit_ctor &&) noexcept {};
    };

} // namespace hud_test

GTEST_TEST(traits, is_explicitly_copy_constructible)
{
    // Implicit copy constructor same type
    hud_assert_false((hud::is_explicitly_copy_constructible_v<hud_test::no_explicit_ctor>));
    hud_assert_false((hud::is_explicitly_copy_constructible_v<hud_test::no_explicit_ctor_2>));
    hud_assert_false((hud::is_explicitly_copy_constructible_v<hud_test::no_explicit_ctor_3>));

    // Explicit copy constructor same type
    hud_assert_true((hud::is_explicitly_copy_constructible_v<hud_test::explicit_ctor>));
    hud_assert_true((hud::is_explicitly_copy_constructible_v<hud_test::explicit_ctor_2>));
    hud_assert_true((hud::is_explicitly_copy_constructible_v<hud_test::explicit_ctor_3>));

    // Implicit copy constructor other explicit copy ctor type
    hud_assert_false((hud::is_explicitly_copy_constructible_v<hud_test::no_explicit_cross_explicit_ctor, hud_test::explicit_ctor>));
    hud_assert_false((hud::is_explicitly_copy_constructible_v<hud_test::no_explicit_cross_explicit_ctor_2, hud_test::explicit_ctor>));
    hud_assert_false((hud::is_explicitly_copy_constructible_v<hud_test::no_explicit_cross_explicit_ctor_3, hud_test::explicit_ctor>));

    // Explicit copy constructor other explicit copy ctor type
    hud_assert_true((hud::is_explicitly_copy_constructible_v<hud_test::explicit_cross_explicit_ctor, hud_test::explicit_ctor>));
    hud_assert_true((hud::is_explicitly_copy_constructible_v<hud_test::explicit_cross_explicit_ctor_2, hud_test::explicit_ctor>));
    hud_assert_true((hud::is_explicitly_copy_constructible_v<hud_test::explicit_cross_explicit_ctor_3, hud_test::explicit_ctor>));

    // Implicit copy constructor other implicit copy ctor type
    hud_assert_false((hud::is_explicitly_copy_constructible_v<hud_test::no_explicit_cross_no_explicit_ctor, hud_test::no_explicit_ctor>));
    hud_assert_false((hud::is_explicitly_copy_constructible_v<hud_test::no_explicit_cross_no_explicit_ctor_2, hud_test::no_explicit_ctor>));
    hud_assert_false((hud::is_explicitly_copy_constructible_v<hud_test::no_explicit_cross_no_explicit_ctor_3, hud_test::no_explicit_ctor>));

    // explicit copy constructor other implicit copy ctor type
    hud_assert_true((hud::is_explicitly_copy_constructible_v<hud_test::explicit_cross_implicit_ctor, hud_test::no_explicit_ctor>));
    hud_assert_true((hud::is_explicitly_copy_constructible_v<hud_test::explicit_cross_implicit_ctor_2, hud_test::no_explicit_ctor>));
    hud_assert_true((hud::is_explicitly_copy_constructible_v<hud_test::explicit_cross_implicit_ctor_3, hud_test::no_explicit_ctor>));

    // cv-qualified trivial types
    hud_assert_false((hud::is_explicitly_copy_constructible_v<i32>));
    hud_assert_false((hud::is_explicitly_copy_constructible_v<const i32>));
    hud_assert_true((hud::is_explicitly_copy_constructible_v<volatile i32>));
    hud_assert_true((hud::is_explicitly_copy_constructible_v<const volatile i32>));
}