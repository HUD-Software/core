#include <core/templates/greater_equal.h>

namespace {
    struct S {
        constexpr S(i32 val) noexcept
            : value(val) {
        }

        constexpr bool operator>=(const S& other) const noexcept {
            return value >= other.value;
        }

        i32 value;
    };
}


TEST(templates, greater_equal) {


    hud::greater_equal<i32> gr;
    ASSERT_TRUE(gr(2, 1));
    ASSERT_TRUE(gr(1, 1));
    ASSERT_FALSE(gr(1, 2));

    hud::greater_equal<S> gr_s;
    ASSERT_TRUE(gr_s(S{ 2 }, S{ 1 }));
    ASSERT_TRUE(gr_s(S{ 1 }, S{ 1 }));
    ASSERT_FALSE(gr_s(S{ 1 }, S{ 2 }));

    hud::greater_equal<i32*> gr_ptr;
    ASSERT_TRUE(gr_ptr(nullptr, nullptr));
    ASSERT_FALSE(gr_ptr(nullptr, reinterpret_cast<i32*>(0x1)));
    ASSERT_TRUE(gr_ptr(reinterpret_cast<i32*>(0x1), nullptr));
}