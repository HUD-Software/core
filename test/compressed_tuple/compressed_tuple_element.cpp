#include <core/containers/tuple_element.h>
#include <core/containers/compressed_tuple.h>

namespace hud_test
{
    template<const usize at, typename type0_t, typename type1_t>
    struct tuple_like_class;

    template<typename type0_t, typename type1_t>
    struct tuple_like_class<0, type0_t, type1_t>
    {
        using type = type0_t;
        type element;
    };

    template<typename type0_t, typename type1_t>
    struct tuple_like_class<1, type0_t, type1_t>
    {
        using type = type1_t;
        type element;
    };
} // namespace hud_test

namespace hud
{
    /**
    Specialization to retrieve the type of the hud_test::tuple_like_class that correspond to the given template argument
    */
    template<typename type0_t, typename type1_t>
    struct tuple_element<0, hud_test::tuple_like_class<0, type0_t, type1_t>>
    {
        using type = typename hud_test::tuple_like_class<0, type0_t, type1_t>::type;
    };

    template<typename type0_t, typename type1_t>
    struct tuple_element<1, hud_test::tuple_like_class<1, type0_t, type1_t>>
    {
        using type = typename hud_test::tuple_like_class<1, type0_t, type1_t>::type;
    };
} // namespace hud

GTEST_TEST(compressed_tuple, tuple_element)
{

    hud_assert_true((hud::is_same_v<hud::tuple_element_t<0, hud::compressed_tuple<u32, u64, wchar>>, u32>));
    hud_assert_true((hud::is_same_v<hud::tuple_element_t<1, hud::compressed_tuple<u32, u64, wchar>>, u64>));
    hud_assert_true((hud::is_same_v<hud::tuple_element_t<2, hud::compressed_tuple<u32, u64, wchar>>, wchar>));

    hud_assert_true((hud::is_same_v<hud::tuple_element_t<0, hud_test::tuple_like_class<0, u32, u64>>, u32>));
    hud_assert_false((hud::is_same_v<hud::tuple_element_t<0, hud_test::tuple_like_class<0, u32, u64>>, u64>));
    hud_assert_false((hud::is_same_v<hud::tuple_element_t<1, hud_test::tuple_like_class<1, u32, u64>>, u32>));
    hud_assert_true((hud::is_same_v<hud::tuple_element_t<1, hud_test::tuple_like_class<1, u32, u64>>, u64>));

    hud_assert_false((hud::is_same_v<hud::tuple_element_t<0, const hud_test::tuple_like_class<0, u32, u64>>, u32>));
    hud_assert_true((hud::is_same_v<hud::tuple_element_t<0, const hud_test::tuple_like_class<0, u32, u64>>, const u32>));
    hud_assert_false((hud::is_same_v<hud::tuple_element_t<0, const hud_test::tuple_like_class<0, u32, u64>>, u64>));
    hud_assert_false((hud::is_same_v<hud::tuple_element_t<0, const hud_test::tuple_like_class<0, u32, u64>>, const u64>));
    hud_assert_false((hud::is_same_v<hud::tuple_element_t<1, const hud_test::tuple_like_class<1, u32, u64>>, u32>));
    hud_assert_false((hud::is_same_v<hud::tuple_element_t<1, const hud_test::tuple_like_class<1, u32, u64>>, const u32>));
    hud_assert_false((hud::is_same_v<hud::tuple_element_t<1, const hud_test::tuple_like_class<1, u32, u64>>, u64>));
    hud_assert_true((hud::is_same_v<hud::tuple_element_t<1, const hud_test::tuple_like_class<1, u32, u64>>, const u64>));

    hud_assert_false((hud::is_same_v<hud::tuple_element_t<0, volatile hud_test::tuple_like_class<0, u32, u64>>, u32>));
    hud_assert_true((hud::is_same_v<hud::tuple_element_t<0, volatile hud_test::tuple_like_class<0, u32, u64>>, volatile u32>));
    hud_assert_false((hud::is_same_v<hud::tuple_element_t<0, volatile hud_test::tuple_like_class<0, u32, u64>>, u64>));
    hud_assert_false((hud::is_same_v<hud::tuple_element_t<0, volatile hud_test::tuple_like_class<0, u32, u64>>, volatile u64>));
    hud_assert_false((hud::is_same_v<hud::tuple_element_t<1, volatile hud_test::tuple_like_class<1, u32, u64>>, u32>));
    hud_assert_false((hud::is_same_v<hud::tuple_element_t<1, volatile hud_test::tuple_like_class<1, u32, u64>>, volatile u32>));
    hud_assert_false((hud::is_same_v<hud::tuple_element_t<1, volatile hud_test::tuple_like_class<1, u32, u64>>, u64>));
    hud_assert_true((hud::is_same_v<hud::tuple_element_t<1, volatile hud_test::tuple_like_class<1, u32, u64>>, volatile u64>));

    hud_assert_false((hud::is_same_v<hud::tuple_element_t<0, const volatile hud_test::tuple_like_class<0, u32, u64>>, u32>));
    hud_assert_true((hud::is_same_v<hud::tuple_element_t<0, const volatile hud_test::tuple_like_class<0, u32, u64>>, const volatile u32>));
    hud_assert_false((hud::is_same_v<hud::tuple_element_t<0, const volatile hud_test::tuple_like_class<0, u32, u64>>, u64>));
    hud_assert_false((hud::is_same_v<hud::tuple_element_t<0, const volatile hud_test::tuple_like_class<0, u32, u64>>, const volatile u64>));
    hud_assert_false((hud::is_same_v<hud::tuple_element_t<1, const volatile hud_test::tuple_like_class<1, u32, u64>>, u32>));
    hud_assert_false((hud::is_same_v<hud::tuple_element_t<1, const volatile hud_test::tuple_like_class<1, u32, u64>>, const volatile u32>));
    hud_assert_false((hud::is_same_v<hud::tuple_element_t<1, const volatile hud_test::tuple_like_class<1, u32, u64>>, u64>));
    hud_assert_true((hud::is_same_v<hud::tuple_element_t<1, const volatile hud_test::tuple_like_class<1, u32, u64>>, const volatile u64>));
}