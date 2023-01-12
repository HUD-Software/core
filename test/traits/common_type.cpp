#include <core/traits/common_type.h>

namespace hud_test
{
    struct base
    {
    };
    struct derived : base
    {
    };

    template <typename = void, typename... type_t>
    struct has_common_type_impl
        : hud::false_type
    {
    };

    template <typename... type_t>
    struct has_common_type_impl<hud::void_t<hud::common_type_t<type_t...>>, type_t...>
        : hud::true_type
    {
    };

    template <typename... type_t>
    struct has_common_type
        : has_common_type_impl<void, type_t...>
    {
    };

    template <typename... type_t>
    inline constexpr bool has_common_type_v = has_common_type<type_t...>::value;
}

GTEST_TEST(traits, common_type)
{
    GTEST_ASSERT_TRUE((hud::is_same_v<hud::common_type_t<i8, i16, i32>, i32>));
    GTEST_ASSERT_TRUE((hud::is_same_v<hud::common_type_t<f32, f64>, f64>));
    GTEST_ASSERT_TRUE((hud::is_same_v<hud::common_type_t<hud_test::derived, hud_test::base>, hud_test::base>));
    GTEST_ASSERT_TRUE((hud::is_same_v<hud::common_type_t<const hud_test::derived, hud_test::base>, hud_test::base>));
    GTEST_ASSERT_TRUE((hud::is_same_v<hud::common_type_t<const hud_test::derived, const hud_test::base>, hud_test::base>));
    GTEST_ASSERT_TRUE((hud::is_same_v<hud::common_type_t<hud_test::derived *, hud_test::base *>, hud_test::base *>));
    GTEST_ASSERT_TRUE((hud::is_same_v<hud::common_type_t<const hud_test::derived *, hud_test::base *>, const hud_test::base *>));
    GTEST_ASSERT_TRUE((hud::is_same_v<hud::common_type_t<const hud_test::derived *, hud_test::derived *, const hud_test::base *>, const hud_test::base *>));
    GTEST_ASSERT_TRUE((hud::is_same_v<hud::common_type_t<hud_test::derived *, const hud_test::base *>, const hud_test::base *>));
    GTEST_ASSERT_TRUE((hud::is_same_v<hud::common_type_t<const i32, volatile i32>, i32>));
    GTEST_ASSERT_TRUE((hud::is_same_v<hud::common_type_t<i32[], i32 *>, i32 *>));
    GTEST_ASSERT_TRUE((hud::is_same_v<hud::common_type_t<i32[], const i32[32], i32 *>, const i32 *>));

    GTEST_ASSERT_TRUE((hud_test::has_common_type_v<i8, i16, i32>));
    GTEST_ASSERT_TRUE((hud_test::has_common_type_v<f32, f64>));
    GTEST_ASSERT_TRUE((hud_test::has_common_type_v<hud_test::derived, hud_test::base>));
    GTEST_ASSERT_TRUE((hud_test::has_common_type_v<const hud_test::derived, hud_test::base>));
    GTEST_ASSERT_TRUE((hud_test::has_common_type_v<const hud_test::derived, const hud_test::base>));
    GTEST_ASSERT_TRUE((hud_test::has_common_type_v<hud_test::derived *, hud_test::base *>));
    GTEST_ASSERT_TRUE((hud_test::has_common_type_v<const hud_test::derived *, hud_test::base *>));
    GTEST_ASSERT_TRUE((hud_test::has_common_type_v<const hud_test::derived *, hud_test::derived *, const hud_test::base *>));
    GTEST_ASSERT_TRUE((hud_test::has_common_type_v<hud_test::derived *, const hud_test::base *>));
    GTEST_ASSERT_TRUE((hud_test::has_common_type_v<const i32, volatile i32>));
    GTEST_ASSERT_TRUE((hud_test::has_common_type_v<i32[], i32 *>));
    GTEST_ASSERT_TRUE((hud_test::has_common_type_v<i32[], const i32[32], i32 *>));

    GTEST_ASSERT_FALSE((hud_test::has_common_type_v<i8, i16, i32, hud_test::derived>));
}