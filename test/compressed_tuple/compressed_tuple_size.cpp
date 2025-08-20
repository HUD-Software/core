#include <core/containers/tuple_size.h>
#include <core/containers/compressed_tuple.h>

namespace hud_test
{
    struct tuple_like_class
    {
    };
} // namespace hud_test

namespace hud
{
    template<>
    struct tuple_size<hud_test::tuple_like_class> : hud::integral_constant<usize, 666>
    {
    };
} // namespace hud

GTEST_TEST(compressed_tuple, tuple_size)
{

    hud_assert_eq(hud::tuple_size_v<hud::compressed_tuple<>>, 0u);
    hud_assert_eq((hud::tuple_size_v<hud::compressed_tuple<u32, u64, hud::compressed_tuple<i32>>>), 3u);
    hud_assert_eq(hud::tuple_size_v<hud_test::tuple_like_class>, 666u);
    hud_assert_eq(hud::tuple_size_v<const hud_test::tuple_like_class>, 666u);
    hud_assert_eq(hud::tuple_size_v<volatile hud_test::tuple_like_class>, 666u);
    hud_assert_eq(hud::tuple_size_v<const volatile hud_test::tuple_like_class>, 666u);
};