#include <core/containers/tuple.h>
#include <core/traits/is_same.h>

namespace hud_test
{
    struct empty
    {
    };
} // namespace hud_test

GTEST_TEST(tuple, sizeof_is_correct)
{
    GTEST_ASSERT_EQ(sizeof(hud::tuple<>), 1u);
    GTEST_ASSERT_EQ(sizeof(hud::tuple<hud_test::empty, hud_test::empty, hud_test::empty>), 3u);
    GTEST_ASSERT_EQ(sizeof(hud::tuple<i32>), 4u);
    GTEST_ASSERT_EQ(sizeof(hud::tuple<i32, i32>), 8u);
    GTEST_ASSERT_EQ(sizeof(hud::tuple<i32, i8, i32>), 12u);
}

GTEST_TEST(tuple, make_tuple)
{
    auto tuple = hud::make_tuple(12, 15.0f, L'w');
    GTEST_ASSERT_TRUE((hud::is_same_v<decltype(tuple), hud::tuple<i32, f32, wchar>>));
    GTEST_ASSERT_EQ(hud::get<0>(tuple), 12);
    GTEST_ASSERT_EQ(hud::get<1>(tuple), 15.0f);
    GTEST_ASSERT_EQ(hud::get<2>(tuple), L'w');

    const auto tuple2 = hud::make_tuple(12, 15.0f, L'w');
    GTEST_ASSERT_TRUE((hud::is_same_v<decltype(tuple2), const hud::tuple<i32, f32, wchar>>));
    GTEST_ASSERT_EQ(hud::get<0>(tuple2), 12);
    GTEST_ASSERT_EQ(hud::get<1>(tuple2), 15.0f);
    GTEST_ASSERT_EQ(hud::get<2>(tuple2), L'w');

    constexpr auto tuple3 = hud::make_tuple(12, 15.0f, L'w');
    GTEST_ASSERT_TRUE((hud::is_same_v<decltype(tuple3), const hud::tuple<i32, f32, wchar>>));
    GTEST_ASSERT_EQ(hud::get<0>(tuple3), 12);
    GTEST_ASSERT_EQ(hud::get<1>(tuple3), 15.0f);
    GTEST_ASSERT_EQ(hud::get<2>(tuple3), L'w');
}

GTEST_TEST(tuple, get)
{
    const auto test = []()
    {
        auto tuple = hud::make_tuple(12, 15.0f, L'w');
        return std::tuple {
            hud::get<0>(tuple) == 12,
            hud::get<1>(tuple) == 15.0f,
            hud::get<2>(tuple) == L'w',
            hud::get<0>(hud::move(tuple)) == 12,
            hud::get<1>(hud::move(tuple)) == 15.0f,
            hud::get<2>(hud::move(tuple)) == L'w'};
    };

    // Non constant
    {
        const auto result = test();
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_TRUE(std::get<2>(result));
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_TRUE(std::get<2>(result));
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
    }
}

GTEST_TEST(tuple, tuple_cat)
{
    using tuple_type1 = hud::tuple<i32, f32>;
    using tuple_type2 = hud::tuple<ansichar, wchar, char16, hud_test::non_bitwise_type>;
    using tuple_type3 = hud::tuple<u64, f64, hud_test::non_bitwise_type>;

    using CatType = decltype(hud::tuple_cat(tuple_type1 {}, tuple_type2 {}, tuple_type3 {}));

    CatType tuple = CatType {
        123,
        12.0f,
        'h',
        L'u',
        u'd',
        hud_test::non_bitwise_type {456, nullptr},
        789u,
        159.,
        hud_test::non_bitwise_type {753, nullptr}
    };

    GTEST_ASSERT_EQ(hud::tuple_size_v<CatType>, hud::tuple_size_v<tuple_type1> + hud::tuple_size_v<tuple_type2> + hud::tuple_size_v<tuple_type3>);
    GTEST_ASSERT_TRUE((hud::is_same_v<decltype(hud::get<0>(tuple)), i32 &>));
    GTEST_ASSERT_TRUE((hud::is_same_v<decltype(hud::get<1>(tuple)), f32 &>));
    GTEST_ASSERT_TRUE((hud::is_same_v<decltype(hud::get<2>(tuple)), ansichar &>));
    GTEST_ASSERT_TRUE((hud::is_same_v<decltype(hud::get<3>(tuple)), wchar &>));
    GTEST_ASSERT_TRUE((hud::is_same_v<decltype(hud::get<4>(tuple)), char16 &>));
    GTEST_ASSERT_TRUE((hud::is_same_v<decltype(hud::get<5>(tuple)), hud_test::non_bitwise_type &>));
    GTEST_ASSERT_TRUE((hud::is_same_v<decltype(hud::get<6>(tuple)), u64 &>));
    GTEST_ASSERT_TRUE((hud::is_same_v<decltype(hud::get<7>(tuple)), f64 &>));
    GTEST_ASSERT_TRUE((hud::is_same_v<decltype(hud::get<8>(tuple)), hud_test::non_bitwise_type &>));

    GTEST_ASSERT_EQ(hud::get<0>(tuple), 123);
    GTEST_ASSERT_EQ(hud::get<1>(tuple), 12.0f);
    GTEST_ASSERT_EQ(hud::get<2>(tuple), 'h');
    GTEST_ASSERT_EQ(hud::get<3>(tuple), L'u');
    GTEST_ASSERT_TRUE(hud::get<4>(tuple) == u'd');
    GTEST_ASSERT_EQ(hud::get<5>(tuple).id(), 456);
    GTEST_ASSERT_EQ(hud::get<5>(tuple).destructor_counter(), nullptr);
    GTEST_ASSERT_EQ(hud::get<5>(tuple).constructor_count(), 1u);
    GTEST_ASSERT_EQ(hud::get<5>(tuple).copy_constructor_count(), 0u);
    GTEST_ASSERT_EQ(hud::get<5>(tuple).move_constructor_count(), 1u);
    GTEST_ASSERT_EQ(hud::get<5>(tuple).copy_assign_count(), 0u);
    GTEST_ASSERT_EQ(hud::get<5>(tuple).move_assign_count(), 0u);
    GTEST_ASSERT_EQ(hud::get<6>(tuple), 789u);
    GTEST_ASSERT_EQ(hud::get<7>(tuple), 159.);
    GTEST_ASSERT_EQ(hud::get<8>(tuple).id(), 753);
    GTEST_ASSERT_EQ(hud::get<8>(tuple).destructor_counter(), nullptr);
    GTEST_ASSERT_EQ(hud::get<8>(tuple).constructor_count(), 1u);
    GTEST_ASSERT_EQ(hud::get<8>(tuple).copy_constructor_count(), 0u);
    GTEST_ASSERT_EQ(hud::get<8>(tuple).move_constructor_count(), 1u);
    GTEST_ASSERT_EQ(hud::get<8>(tuple).copy_assign_count(), 0u);
    GTEST_ASSERT_EQ(hud::get<8>(tuple).move_assign_count(), 0u);

    using EmptyCatType = decltype(hud::tuple_cat());
    GTEST_ASSERT_EQ(hud::tuple_size_v<EmptyCatType>, 0u);
}