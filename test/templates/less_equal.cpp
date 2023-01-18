#include <core/templates/less_equal.h>

// LCOV_EXCL_START
namespace hud_test
{
    struct S
    {
        constexpr S(i32 val) noexcept
            : value(val)
        {
        }

        constexpr bool operator<=(const S &other) const noexcept
        {
            return value <= other.value;
        }

        i32 value;
    };
} // namespace hud_test

// LCOV_EXCL_STOP

GTEST_TEST(templates, less_equal)
{

    hud::less_equal<i32> le;
    GTEST_ASSERT_FALSE(le(2, 1));
    GTEST_ASSERT_TRUE(le(1, 1));
    GTEST_ASSERT_TRUE(le(1, 2));

    hud::less_equal<hud_test::S> le_s;
    GTEST_ASSERT_FALSE(le_s(hud_test::S {2}, hud_test::S {1}));
    GTEST_ASSERT_TRUE(le_s(hud_test::S {1}, hud_test::S {1}));
    GTEST_ASSERT_TRUE(le_s(hud_test::S {1}, hud_test::S {2}));

    hud::less_equal<i32 *> le_ptr;
    GTEST_ASSERT_TRUE(le_ptr(nullptr, nullptr));
    GTEST_ASSERT_TRUE(le_ptr(nullptr, reinterpret_cast<i32 *>(0x1)));
    GTEST_ASSERT_FALSE(le_ptr(reinterpret_cast<i32 *>(0x1), nullptr));
}