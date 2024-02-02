#include <core/templates/swap.h>

namespace hud_test
{
    struct type
    {
        type() noexcept = default;

        type(const i32 i) noexcept
            : inner(i)
        {
        }

        type(type &&other) noexcept
            : inner(other.inner)
        {
            other.inner = 0;
        }

        type &operator=(const type &other) noexcept
        {
            inner = other.inner;
            return *this;
        }

        i32 inner;
    };
} // namespace hud_test

GTEST_TEST(templates, swap_trivial_type)
{
    i32 a = 5;
    i32 b = 32;
    hud::swap(a, b);

    hud_assert_eq(a, 32);
    hud_assert_eq(b, 5);
}

GTEST_TEST(templates, swap_non_rivial_type)
{
    hud_test::type t(2);
    hud_test::type t1(3);
    hud::swap(t, t1);
    hud_assert_eq(t.inner, 3);
    hud_assert_eq(t1.inner, 2);
}

GTEST_TEST(memory, swap_array_trivial_type)
{
    i32 c[3] = {1, 2, 3};
    i32 d[3] = {4, 5, 6};
    hud::swap(c, d);
    hud_assert_eq(c[0], 4);
    hud_assert_eq(c[1], 5);
    hud_assert_eq(c[2], 6);
    hud_assert_eq(d[0], 1);
    hud_assert_eq(d[1], 2);
    hud_assert_eq(d[2], 3);
}

GTEST_TEST(memory, swap_array_non_trivial_type)
{
    hud_test::type c[3] = {1, 2, 3};
    hud_test::type d[3] = {4, 5, 6};
    hud::swap(c, d);
    hud_assert_eq(c[0].inner, 4);
    hud_assert_eq(c[1].inner, 5);
    hud_assert_eq(c[2].inner, 6);
    hud_assert_eq(d[0].inner, 1);
    hud_assert_eq(d[1].inner, 2);
    hud_assert_eq(d[2].inner, 3);
}