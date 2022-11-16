#include <core/containers/pair.h>
#include <core/traits/is_same.h>

GTEST_TEST(pair, inner_is_correct) {


    using pair_type = hud::pair<i32, u32>;
    
    bool is_first_same = hud::is_same_v<pair_type::first_type, i32>;
    ASSERT_TRUE(is_first_same);

    bool is_second_same = hud::is_same_v<pair_type::second_type, u32>;
    ASSERT_TRUE(is_second_same);
}

GTEST_TEST(pair, sizeof_pair_is_sizeof_inner_types) {


    using pair_type = hud::pair<i32, u64>;
    struct shoud_be { i32 f; u64 s; };
    ASSERT_EQ(sizeof(pair_type), sizeof(shoud_be) );
}

GTEST_TEST(pair, make_pair_create_pair_trivially_constructible) {


    using type = i32;
    static_assert(hud::is_trivially_constructible_v<type, type&&>);

    const auto test = [](type&& t1, type&& t2) {
        const auto pair = hud::make_pair(t1, t2);
        return std::tuple{
            pair.first,
            pair.second
        };
    };
    
    // Non constant
    {
        const auto result = test(123,456);

        ASSERT_EQ(std::get<0>(result), 123);
        ASSERT_EQ(std::get<1>(result), 456);
    }

    // Constant
    {
        constexpr auto result = test(123, 456);

        ASSERT_EQ(std::get<0>(result), 123);
        ASSERT_EQ(std::get<1>(result), 456);
    }
}

GTEST_TEST(pair, make_pair_create_pair_non_trivial) {

    using type = hud_test::non_bitwise_type;
    static_assert(hud::is_constructible_v<type, type&&>);
    static_assert(!hud::is_trivially_constructible_v<type, type&&>);

    const auto test = [](type&& t1, type&& t2) {
        const auto pair = hud::make_pair(hud::forward<type>(t1), hud::forward<type>(t2));
        return std::tuple{
            pair.first.id(),
            pair.first.move_assign_count(),
            pair.first.copy_assign_count(),
            pair.first.constructor_count(),
            pair.first.move_constructor_count(),
            pair.first.copy_constructor_count(),
            pair.second.id(),
            pair.second.move_assign_count(),
            pair.second.copy_assign_count(),
            pair.second.constructor_count(),
            pair.second.move_constructor_count(),
            pair.second.copy_constructor_count(),
        };
    };

    // Non constant
    {
        const auto result = test(type{ 123, nullptr }, type{ 456, nullptr });

        ASSERT_EQ(std::get<0>(result), 123);
        ASSERT_EQ(std::get<1>(result), 0u);
        ASSERT_EQ(std::get<2>(result), 0u);
        ASSERT_EQ(std::get<3>(result), 1u);
        ASSERT_EQ(std::get<4>(result), 1u);
        ASSERT_EQ(std::get<5>(result), 0u);
        ASSERT_EQ(std::get<6>(result), 456);
        ASSERT_EQ(std::get<7>(result), 0u);
        ASSERT_EQ(std::get<8>(result), 0u);
        ASSERT_EQ(std::get<9>(result), 1u);
        ASSERT_EQ(std::get<10>(result), 1u);
        ASSERT_EQ(std::get<11>(result), 0u);
    }

    // Constant
    {
        constexpr auto result = test(type{ 123, nullptr }, type{ 456, nullptr });

        ASSERT_EQ(std::get<0>(result), 123);
        ASSERT_EQ(std::get<1>(result), 0u);
        ASSERT_EQ(std::get<2>(result), 0u);
        ASSERT_EQ(std::get<3>(result), 1u);
        ASSERT_EQ(std::get<4>(result), 1u);
        ASSERT_EQ(std::get<5>(result), 0u);
        ASSERT_EQ(std::get<6>(result), 456);
        ASSERT_EQ(std::get<7>(result), 0u);
        ASSERT_EQ(std::get<8>(result), 0u);
        ASSERT_EQ(std::get<9>(result), 1u);
        ASSERT_EQ(std::get<10>(result), 1u);
        ASSERT_EQ(std::get<11>(result), 0u);
    }
}

GTEST_TEST(pair, get_is_usable_with_pair) {


    const auto test = [](i32 t1, i32 t2) {
        const auto pair = hud::make_pair(t1, t2);
        return std::tuple{
            hud::get<0>(pair),
            hud::get<1>(pair)
        };
    };

    // Non constant
    {
        const auto result = test(123, 456);
        ASSERT_EQ(std::get<0>(result), 123);
        ASSERT_EQ(std::get<1>(result), 456);
    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        ASSERT_EQ(std::get<0>(result), 123);
        ASSERT_EQ(std::get<1>(result), 456);
    }
}

GTEST_TEST(pair, tuple_size) {


    constexpr usize tuple_size = hud::tuple_size_v<hud::pair<u32, u64>>;
    ASSERT_EQ(tuple_size, 2u);
}

GTEST_TEST(pair, tuple_element) {

    constexpr bool is_tuple_element_0_same = hud::is_same_v<hud::tuple_element_t<0, hud::pair<u32, u64> >, u32>;
    constexpr bool is_tuple_element_1_same = hud::is_same_v<hud::tuple_element_t<1, hud::pair<u32, u64> >, u64>;
    ASSERT_TRUE(is_tuple_element_0_same);
    ASSERT_TRUE(is_tuple_element_1_same);
}