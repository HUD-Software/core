#include <core/containers/tuple_size.h>
#include <core/containers/tuple.h>

namespace hud_test {
    struct tuple_like_class
    {};
}

namespace hud {
    template<>
    struct tuple_size < hud_test::tuple_like_class> : hud::integral_constant<usize, 666>
    {};
}

GTEST_TEST(tuple, tuple_size) {

    ASSERT_EQ(hud::tuple_size_v<hud::tuple<>>, 0u);
    ASSERT_EQ((hud::tuple_size_v<hud::tuple<u32, u64, hud::tuple<i32>>>), 3u);
    ASSERT_EQ(hud::tuple_size_v<hud_test::tuple_like_class>, 666u);
    ASSERT_EQ(hud::tuple_size_v<const hud_test::tuple_like_class>, 666u);
    ASSERT_EQ(hud::tuple_size_v<volatile hud_test::tuple_like_class>, 666u);
    ASSERT_EQ(hud::tuple_size_v<const volatile hud_test::tuple_like_class>, 666u);
};