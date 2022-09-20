#include <core/containers/tuple_size.h>
#include <core/containers/tuple.h>

namespace {
    struct TupleLikeClass
    {};
}

namespace hud {
    template<>
    struct TupleSize < TupleLikeClass> : hud::integral_constant<usize, 666>
    {};
}

TEST(Tuple, TupleSize) {

    ASSERT_EQ(hud::TupleSizeV<hud::Tuple<>>, 0u);
    ASSERT_EQ((hud::TupleSizeV<hud::Tuple<u32, u64, hud::Tuple<i32>>>), 3u);
    ASSERT_EQ(hud::TupleSizeV<TupleLikeClass>, 666u);
    ASSERT_EQ(hud::TupleSizeV<const TupleLikeClass>, 666u);
    ASSERT_EQ(hud::TupleSizeV<volatile TupleLikeClass>, 666u);
    ASSERT_EQ(hud::TupleSizeV<const volatile TupleLikeClass>, 666u);
};