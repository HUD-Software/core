#include <core/containers/tuple_size.h>
#include <core/containers/tuple.h>

namespace {
    struct tuple_like_class
    {};
}

namespace hud {
    template<>
    struct tuple_size < tuple_like_class> : hud::integral_constant<usize, 666>
    {};
}

TEST(Tuple, tuple_size) {

    ASSERT_EQ(hud::tuple_size_v<hud::tuple<>>, 0u);
    ASSERT_EQ((hud::tuple_size_v<hud::tuple<u32, u64, hud::tuple<i32>>>), 3u);
    ASSERT_EQ(hud::tuple_size_v<tuple_like_class>, 666u);
    ASSERT_EQ(hud::tuple_size_v<const tuple_like_class>, 666u);
    ASSERT_EQ(hud::tuple_size_v<volatile tuple_like_class>, 666u);
    ASSERT_EQ(hud::tuple_size_v<const volatile tuple_like_class>, 666u);
};