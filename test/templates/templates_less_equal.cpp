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
    hud_assert_false(le(2, 1));
    hud_assert_true(le(1, 1));
    hud_assert_true(le(1, 2));

    hud::less_equal<hud_test::S> le_s;
    hud_assert_false(le_s(hud_test::S {2}, hud_test::S {1}));
    hud_assert_true(le_s(hud_test::S {1}, hud_test::S {1}));
    hud_assert_true(le_s(hud_test::S {1}, hud_test::S {2}));

    hud::less_equal<i32 *> le_ptr;
    hud_assert_true(le_ptr(nullptr, nullptr));
    hud_assert_true(le_ptr(nullptr, reinterpret_cast<i32 *>(0x1)));
    hud_assert_false(le_ptr(reinterpret_cast<i32 *>(0x1), nullptr));
}