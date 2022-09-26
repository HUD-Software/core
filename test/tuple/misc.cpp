#include <core/containers/tuple.h>
#include <core/traits/is_same.h>

TEST(tuple, sizeof_is_correct) {
    ASSERT_EQ(sizeof(hud::tuple<>), 1u);
    ASSERT_EQ(sizeof(hud::tuple<i32>), 4u);
    ASSERT_EQ(sizeof(hud::tuple<i32, i32>), 8u);
    ASSERT_EQ(sizeof(hud::tuple<i32, i8, i32>), 12u);
}

TEST(tuple, make_tuple) {
    auto tuple = hud::make_tuple(12, 15.0f, L'w');
    ASSERT_TRUE((hud::is_same_v<decltype(tuple), hud::tuple<i32, f32, wchar>>));
    ASSERT_EQ(hud::get<0>(tuple), 12);
    ASSERT_EQ(hud::get<1>(tuple), 15.0f);
    ASSERT_EQ(hud::get<2>(tuple), L'w');

    const auto tuple2 = hud::make_tuple(12, 15.0f, L'w');
    ASSERT_TRUE((hud::is_same_v<decltype(tuple2), const hud::tuple<i32, f32, wchar>>));
    ASSERT_EQ(hud::get<0>(tuple2), 12);
    ASSERT_EQ(hud::get<1>(tuple2), 15.0f);
    ASSERT_EQ(hud::get<2>(tuple2), L'w');

    constexpr auto tuple3 = hud::make_tuple(12, 15.0f, L'w');
    ASSERT_TRUE((hud::is_same_v<decltype(tuple3), const hud::tuple<i32, f32, wchar>>));
    ASSERT_EQ(hud::get<0>(tuple3), 12);
    ASSERT_EQ(hud::get<1>(tuple3), 15.0f);
    ASSERT_EQ(hud::get<2>(tuple3), L'w');
}

TEST(tuple, get) {
    const auto test = []() {
        auto tuple = hud::make_tuple(12, 15.0f, L'w');
        return std::tuple{
            hud::get<0>(tuple) == 12,
            hud::get<1>(tuple) == 15.0f,
            hud::get<2>(tuple) == L'w',
            hud::get<0>(hud::move(tuple)) == 12,
            hud::get<1>(hud::move(tuple)) == 15.0f,
            hud::get<2>(hud::move(tuple)) == L'w'
        };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
        ASSERT_TRUE(std::get<4>(result));
        ASSERT_TRUE(std::get<5>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
        ASSERT_TRUE(std::get<4>(result));
        ASSERT_TRUE(std::get<5>(result));
    }
}


TEST(tuple, tuple_cat) {
    using tuple_type1 = hud::tuple<i32, f32>;
    using tuple_type2 = hud::tuple<ansichar, wchar, char16, hud_test::non_bitwise_type>;
    using tuple_type3 = hud::tuple<u64, f64, hud_test::non_bitwise_type>;
    

    using CatType = decltype(hud::tuple_cat(tuple_type1{}, tuple_type2{}, tuple_type3{}));

    CatType tuple = CatType{ 123, 12.0f, 'h', L'u', u'd', hud_test::non_bitwise_type{456, nullptr}, 789u, 159., hud_test::non_bitwise_type{753, nullptr} };

    ASSERT_EQ(hud::tuple_size_v<CatType>, hud::tuple_size_v<tuple_type1> +hud::tuple_size_v<tuple_type2> +hud::tuple_size_v<tuple_type3>);
    ASSERT_TRUE((hud::is_same_v<decltype(hud::get<0>(tuple)), i32&>));
    ASSERT_TRUE((hud::is_same_v<decltype(hud::get<1>(tuple)), f32& > ));
    ASSERT_TRUE((hud::is_same_v<decltype(hud::get<2>(tuple)), ansichar& > ));
    ASSERT_TRUE((hud::is_same_v<decltype(hud::get<3>(tuple)), wchar& > ));
    ASSERT_TRUE((hud::is_same_v<decltype(hud::get<4>(tuple)), char16& >));
    ASSERT_TRUE((hud::is_same_v<decltype(hud::get<5>(tuple)), hud_test::non_bitwise_type& > ));
    ASSERT_TRUE((hud::is_same_v<decltype(hud::get<6>(tuple)), u64& > ));
    ASSERT_TRUE((hud::is_same_v<decltype(hud::get<7>(tuple)), f64& > ));
    ASSERT_TRUE((hud::is_same_v<decltype(hud::get<8>(tuple)), hud_test::non_bitwise_type& > ));

    ASSERT_EQ(hud::get<0>(tuple), 123);
    ASSERT_EQ(hud::get<1>(tuple), 12.0f);
    ASSERT_EQ(hud::get<2>(tuple), 'h');
    ASSERT_EQ(hud::get<3>(tuple), L'u');
    ASSERT_TRUE(hud::get<4>(tuple) == u'd');
    ASSERT_EQ(hud::get<5>(tuple).id(), 456);
    ASSERT_EQ(hud::get<5>(tuple).destructor_counter(), nullptr);
    ASSERT_EQ(hud::get<5>(tuple).constructor_count(), 1u);
    ASSERT_EQ(hud::get<5>(tuple).copy_constructor_count(), 0u);
    ASSERT_EQ(hud::get<5>(tuple).move_constructor_count(), 1u);
    ASSERT_EQ(hud::get<5>(tuple).copy_assign_count(), 0u);
    ASSERT_EQ(hud::get<5>(tuple).move_assign_count(), 0u);
    ASSERT_EQ(hud::get<6>(tuple), 789u);
    ASSERT_EQ(hud::get<7>(tuple), 159.);
    ASSERT_EQ(hud::get<8>(tuple).id(), 753);
    ASSERT_EQ(hud::get<8>(tuple).destructor_counter(), nullptr);
    ASSERT_EQ(hud::get<8>(tuple).constructor_count(), 1u);
    ASSERT_EQ(hud::get<8>(tuple).copy_constructor_count(), 0u);
    ASSERT_EQ(hud::get<8>(tuple).move_constructor_count(), 1u);
    ASSERT_EQ(hud::get<8>(tuple).copy_assign_count(), 0u);
    ASSERT_EQ(hud::get<8>(tuple).move_assign_count(), 0u);

    using EmptyCatType = decltype(hud::tuple_cat());
    ASSERT_EQ(hud::tuple_size_v<EmptyCatType>, 0u);
}