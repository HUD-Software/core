#include <core/templates/greater.h>

// LCOV_EXCL_START
namespace hud_test
{
    struct S
    {
        constexpr S(i32 val) noexcept
            : value(val)
        {
        }

        constexpr bool operator>(const S &other) const noexcept
        {
            return value > other.value;
        }

        i32 value;
    };
} // namespace hud_test

// LCOV_EXCL_STOP

GTEST_TEST(templates, greater)
{

    hud::greater<i32> gr;
    hud_assert_true(gr(2, 1));
    hud_assert_false(gr(1, 1));
    hud_assert_false(gr(1, 2));

    hud::greater<hud_test::S> gr_s;
    hud_assert_true(gr_s(hud_test::S {2}, hud_test::S {1}));
    hud_assert_false(gr_s(hud_test::S {1}, hud_test::S {1}));
    hud_assert_false(gr_s(hud_test::S {1}, hud_test::S {2}));

    hud::greater<i32 *> gr_ptr;
    hud_assert_false(gr_ptr(nullptr, nullptr));
    hud_assert_false(gr_ptr(nullptr, reinterpret_cast<i32 *>(0x1)));
    hud_assert_true(gr_ptr(reinterpret_cast<i32 *>(0x1), nullptr));
}