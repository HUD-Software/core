#include <core/templates/less.h>

// LCOV_EXCL_START
namespace hud_test {
    struct S{
        constexpr S(i32 val) noexcept
            : value(val) {
        }

        constexpr bool operator<(const S& other) const noexcept {
            return value < other.value;
        }

        i32 value;
    };
}
// LCOV_EXCL_STOP

GTEST_TEST(templates, less) {


    hud::less<i32> ls;
    ASSERT_FALSE(ls(2, 1));
    ASSERT_FALSE(ls(1, 1));
    ASSERT_TRUE(ls(1, 2));

    hud::less<hud_test::S> ls_s;
    ASSERT_FALSE(ls_s(hud_test::S{ 2 }, hud_test::S{ 1 }));
    ASSERT_FALSE(ls_s(hud_test::S{ 1 }, hud_test::S{ 1 }));
    ASSERT_TRUE(ls_s(hud_test::S{ 1 }, hud_test::S{ 2 }));

    hud::less<i32*> ls_ptr;
    ASSERT_FALSE(ls_ptr(nullptr, nullptr));
    ASSERT_TRUE(ls_ptr(nullptr, reinterpret_cast<i32*>(0x1)));
    ASSERT_FALSE(ls_ptr(reinterpret_cast<i32*>(0x1), nullptr));
}