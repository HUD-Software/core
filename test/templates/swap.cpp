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
}

GTEST_TEST(templates, swap_trivial_type)
{
    i32 a = 5;
    i32 b = 32;
    hud::swap(a, b);

    GTEST_ASSERT_EQ(a, 32);
    GTEST_ASSERT_EQ(b, 5);
}

GTEST_TEST(templates, swap_non_rivial_type)
{
    hud_test::type t(2);
    hud_test::type t1(3);
    hud::swap(t, t1);
    GTEST_ASSERT_EQ(t.inner, 3);
    GTEST_ASSERT_EQ(t1.inner, 2);
}

GTEST_TEST(memory, swap_array_trivial_type)
{
    i32 c[3] = {1, 2, 3};
    i32 d[3] = {4, 5, 6};
    hud::swap(c, d);
    GTEST_ASSERT_EQ(c[0], 4);
    GTEST_ASSERT_EQ(c[1], 5);
    GTEST_ASSERT_EQ(c[2], 6);
    GTEST_ASSERT_EQ(d[0], 1);
    GTEST_ASSERT_EQ(d[1], 2);
    GTEST_ASSERT_EQ(d[2], 3);
}

GTEST_TEST(memory, swap_array_non_trivial_type)
{
    hud_test::type c[3] = {1, 2, 3};
    hud_test::type d[3] = {4, 5, 6};
    hud::swap(c, d);
    GTEST_ASSERT_EQ(c[0].inner, 4);
    GTEST_ASSERT_EQ(c[1].inner, 5);
    GTEST_ASSERT_EQ(c[2].inner, 6);
    GTEST_ASSERT_EQ(d[0].inner, 1);
    GTEST_ASSERT_EQ(d[1].inner, 2);
    GTEST_ASSERT_EQ(d[2].inner, 3);
}