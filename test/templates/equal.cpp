#include <core/templates/equal.h>

// LCOV_EXCL_START
namespace hud_test
{
    struct S
    {
        constexpr S(i32 val) noexcept
            : value(val)
        {
        }

        constexpr bool operator==(const S &other) const noexcept
        {
            return value == other.value;
        }

        i32 value;
    };
}
// LCOV_EXCL_STOP

GTEST_TEST(templates, equal)
{

    hud::equal<i32> eq;
    GTEST_ASSERT_TRUE(eq(1, 1));
    GTEST_ASSERT_FALSE(eq(-1, 1));

    hud::equal<hud_test::S> eq_s;
    GTEST_ASSERT_TRUE(eq_s(hud_test::S{
                               1,
                           },
                           hud_test::S{1}));
    GTEST_ASSERT_FALSE(eq_s(hud_test::S{-1}, hud_test::S{1}));

    hud::equal<i32 *> eq_ptr;
    GTEST_ASSERT_TRUE(eq_ptr(nullptr, nullptr));
    GTEST_ASSERT_FALSE(eq_ptr(nullptr, reinterpret_cast<i32 *>(0x1)));
}