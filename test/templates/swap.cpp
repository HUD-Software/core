#include <core/templates/swap.h>

namespace {
    struct type_t {
        type_t() noexcept = default;
        type_t(const i32 i) noexcept
            : inner(i) {
        }
        type_t(type_t&& other) noexcept
            : inner(other.inner) {
            other.inner = 0;
        }
        type_t& operator=(const type_t& other) noexcept {
            inner = other.inner;
            return *this;
        }
        i32 inner;
    };
}

TEST(templates, swap_trivial_type) {
    i32 a = 5;
    i32 b = 32;
    hud::swap(a, b);

    ASSERT_EQ(a, 32);
    ASSERT_EQ(b, 5);
}

TEST(templates, swap_non_rivial_type) {
    type_t t(2);
    type_t t1(3);
    hud::swap(t, t1);
    ASSERT_EQ(t.inner, 3);
    ASSERT_EQ(t1.inner, 2);
}


TEST(Memory, swap_array_trivial_type) {
    i32 c[3] = { 1,2,3 };
    i32 d[3] = { 4,5,6 };
    hud::swap(c, d);
    ASSERT_EQ(c[0], 4);
    ASSERT_EQ(c[1], 5);
    ASSERT_EQ(c[2], 6);
    ASSERT_EQ(d[0], 1);
    ASSERT_EQ(d[1], 2);
    ASSERT_EQ(d[2], 3);


}

TEST(Memory, swap_array_non_trivial_type) {
    type_t c[3] = { 1,2,3 };
    type_t d[3] = { 4,5,6 };
    hud::swap(c, d);
    ASSERT_EQ(c[0].inner, 4);
    ASSERT_EQ(c[1].inner, 5);
    ASSERT_EQ(c[2].inner, 6);
    ASSERT_EQ(d[0].inner, 1);
    ASSERT_EQ(d[1].inner, 2);
    ASSERT_EQ(d[2].inner, 3);
}