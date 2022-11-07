#include <core/templates/greater.h>

// LCOV_EXCL_START
namespace hud_test {
    struct S {
        constexpr S(i32 val) noexcept
            : value(val) {
        }

        constexpr bool operator>(const S& other) const noexcept {
            return value > other.value;
        }

        i32 value;
    };
}
// LCOV_EXCL_STOP


TEST(templates, greater) {


    hud::greater<i32> gr;
    ASSERT_TRUE(gr(2, 1));
    ASSERT_FALSE(gr(1, 1));
    ASSERT_FALSE(gr(1, 2));

    hud::greater<hud_test::S> gr_s;
    ASSERT_TRUE(gr_s(hud_test::S{ 2 }, hud_test::S{ 1 }));
    ASSERT_FALSE(gr_s(hud_test::S{ 1 }, hud_test::S{ 1 }));
    ASSERT_FALSE(gr_s(hud_test::S{ 1 }, hud_test::S{ 2 }));

    hud::greater<i32*> gr_ptr;
    ASSERT_FALSE(gr_ptr(nullptr, nullptr));
    ASSERT_FALSE(gr_ptr(nullptr, reinterpret_cast<i32*>(0x1)));
    ASSERT_TRUE(gr_ptr(reinterpret_cast<i32*>(0x1), nullptr));
}