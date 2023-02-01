#include <core/templates/less.h>

// LCOV_EXCL_START
namespace hud_test
{
    struct S
    {
        constexpr S(i32 val) noexcept
            : value(val)
        {
        }

        constexpr bool operator<(const S &other) const noexcept
        {
            return value < other.value;
        }

        i32 value;
    };
} // namespace hud_test

// LCOV_EXCL_STOP

GTEST_TEST(templates, less)
{

    hud::less<i32> ls;
    hud_assert_false(ls(2, 1));
    hud_assert_false(ls(1, 1));
    hud_assert_true(ls(1, 2));

    hud::less<hud_test::S> ls_s;
    hud_assert_false(ls_s(hud_test::S {2}, hud_test::S {1}));
    hud_assert_false(ls_s(hud_test::S {1}, hud_test::S {1}));
    hud_assert_true(ls_s(hud_test::S {1}, hud_test::S {2}));

    hud::less<i32 *> ls_ptr;
    hud_assert_false(ls_ptr(nullptr, nullptr));
    hud_assert_true(ls_ptr(nullptr, reinterpret_cast<i32 *>(0x1)));
    hud_assert_false(ls_ptr(reinterpret_cast<i32 *>(0x1), nullptr));
}