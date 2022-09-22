#include <core/templates/equal.h>

namespace {
    struct S {
        constexpr S(i32 val) noexcept 
            : value(val) {
        }

        constexpr bool operator==(const S& other) const noexcept {
            return value == other.value;
        }

        i32 value;
    };
}

TEST(templates, equal) {


    hud::equal<i32> eq;
    ASSERT_TRUE(eq(1, 1));
    ASSERT_FALSE(eq(-1, 1));

    hud::equal<S> eq_s;
    ASSERT_TRUE(eq_s(S{ 1, }, S{ 1 }));
    ASSERT_FALSE(eq_s(S{ -1 }, S{ 1 }));

    hud::equal<i32*> eq_ptr;
    ASSERT_TRUE(eq_ptr(nullptr, nullptr));
    ASSERT_FALSE(eq_ptr(nullptr, reinterpret_cast<i32*>(0x1)));
}