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
} // namespace hud_test

// LCOV_EXCL_STOP

GTEST_TEST(templates, equal)
{

    hud::equal<i32> eq;
    hud_assert_true(eq(1, 1));
    hud_assert_false(eq(-1, 1));

    hud::equal<hud_test::S> eq_s;
    hud_assert_true(eq_s(hud_test::S {
                               1,
                           },
                           hud_test::S {1}));
    hud_assert_false(eq_s(hud_test::S {-1}, hud_test::S {1}));

    hud::equal<i32 *> eq_ptr;
    hud_assert_true(eq_ptr(nullptr, nullptr));
    hud_assert_false(eq_ptr(nullptr, reinterpret_cast<i32 *>(0x1)));
}