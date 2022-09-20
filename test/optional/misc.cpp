#include <core/containers/optional.h>
#include <optional>

TEST(optional, less_or_equal_size_as_std_optional) {
    ASSERT_LE(sizeof(hud::optional<i32>), sizeof(std::optional<i32>));
}

TEST(optional, trivially_copy_constructible_if_type_is_trivially_copy_constructible) {


    using type = i32;

    ASSERT_TRUE(hud::is_trivially_copy_constructible_v<type>);
    ASSERT_TRUE(hud::is_trivially_copy_constructible_v<hud::optional<type>>);
}

TEST(optional, not_trivially_copy_constructible_if_type_is_not_trivially_copy_constructible) {


    using type = hud::test::NonBitwiseType;

    ASSERT_FALSE(hud::is_trivially_copy_constructible_v<type>);
    ASSERT_FALSE(hud::is_trivially_copy_constructible_v<hud::optional<type>>);
}

TEST(optional, trivially_destructible_if_type_is_trivially_destructible) {


    using type = i32;

    ASSERT_TRUE(hud::is_trivially_destructible_v<type>);
    ASSERT_TRUE(hud::is_trivially_destructible_v<hud::optional<type>>);
}

TEST(optional, not_trivially_destructible_if_type_is_not_trivially_destructible) {


    using type = hud::test::NonBitwiseType;

    ASSERT_FALSE(hud::is_trivially_destructible_v<type>);
    ASSERT_FALSE(hud::is_trivially_destructible_v<hud::optional<type>>);
}

TEST(optional, cast_bool) {


    using type = hud::test::NonBitwiseType;

    const auto test = []() {
        hud::optional<type> option_empty;
        hud::optional<type> option_non_empty{ hud::in_place, 123, nullptr };
        return std::tuple{ static_cast<bool>(option_empty), static_cast<bool>(option_non_empty) };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_FALSE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_FALSE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
    }
}


TEST(optional, has_value) {


    using type = hud::test::NonBitwiseType;

    const auto test = []() {
        hud::optional<type> option_empty;
        hud::optional<type> option_non_empty{ hud::in_place, 123, nullptr };
        return std::tuple{ option_empty.has_value(), option_non_empty.has_value() };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_FALSE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_FALSE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
    }
}

TEST(optional, value) {


    using type = hud::test::NonBitwiseType;

    const auto test = []() {
        
        hud::optional<type> option_non_empty{ hud::in_place, 123, nullptr };
        const hud::optional<type> const_option_non_empty{ hud::in_place, 123, nullptr };

        return std::tuple{
            // Value should return Lvalue reference
            hud::is_lvalue_reference_v<decltype(option_non_empty.value())> && 
            option_non_empty.value().id() == 123,
            // Value should return Lvalue reference to const
            hud::is_lvalue_reference_v<decltype(const_option_non_empty.value())> && 
            hud::is_const_v<hud::remove_reference_t<decltype(const_option_non_empty.value())>> && 
            const_option_non_empty.value().id() == 123,
            // Value should return Rvalue reference
            hud::is_rvalue_reference_v<decltype(hud::forward<hud::optional<type>>(option_non_empty).value())> &&
            hud::move(option_non_empty).value().id() == 123,
            // Value should return Rvalue reference to const
            hud::is_rvalue_reference_v<decltype(hud::forward<const hud::optional<type>>(const_option_non_empty).value())> && 
            hud::is_const_v<hud::remove_reference_t<decltype(hud::forward<const hud::optional<type>>(const_option_non_empty).value())>> &&
            hud::move(const_option_non_empty).value().id() == 123,
        };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
    }
}

TEST(optional, value_or) {


    using type = hud::test::NonBitwiseType;

    const auto test = []() {
        hud::optional<type> option_empty;
        hud::optional<type> option_non_empty{ hud::in_place, 123, nullptr };
        const hud::optional<type> const_option_empty;
        const hud::optional<type> const_option_non_empty{ hud::in_place, 123, nullptr };

        return std::tuple{
            option_empty.value_or(type{456, nullptr}).id(),
            option_non_empty.value_or(type{ 456, nullptr }).id(),
            const_option_empty.value_or(type{ 456, nullptr }).id(),
            const_option_non_empty.value_or(type{ 456, nullptr }).id(),
            hud::move(option_empty).value_or(type{456, nullptr}).id(),
            hud::move(option_non_empty).value_or(type{ 456, nullptr }).id(),
            hud::move(const_option_empty).value_or(type{ 456, nullptr }).id(),
            hud::move(const_option_non_empty).value_or(type{ 456, nullptr }).id()
        };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_EQ(std::get<0>(result), 456);
        ASSERT_EQ(std::get<1>(result), 123);
        ASSERT_EQ(std::get<2>(result), 456);
        ASSERT_EQ(std::get<3>(result), 123);
        ASSERT_EQ(std::get<4>(result), 456);
        ASSERT_EQ(std::get<5>(result), 123);
        ASSERT_EQ(std::get<6>(result), 456);
        ASSERT_EQ(std::get<7>(result), 123);
    }


    // Constant
    {
        constexpr auto result = test();
        ASSERT_EQ(std::get<0>(result), 456);
        ASSERT_EQ(std::get<1>(result), 123);
        ASSERT_EQ(std::get<2>(result), 456);
        ASSERT_EQ(std::get<3>(result), 123);
        ASSERT_EQ(std::get<4>(result), 456);
        ASSERT_EQ(std::get<5>(result), 123);
        ASSERT_EQ(std::get<6>(result), 456);
        ASSERT_EQ(std::get<7>(result), 123);
    }
}


TEST(optional, operator_arrow) {



    using type = hud::test::NonBitwiseType;

    const auto test = []() {
        hud::optional<type> option{ hud::in_place, 123, nullptr };
        const hud::optional<type> const_option{ hud::in_place, 456, nullptr };
        return std::tuple{
            option->id(),
            const_option->id()
        };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_EQ(std::get<0>(result), 123);
        ASSERT_EQ(std::get<1>(result), 456);
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_EQ(std::get<0>(result), 123);
        ASSERT_EQ(std::get<1>(result), 456);
    }
}

TEST(optional, operator_dereference) {


    using type = hud::test::NonBitwiseType;

    const auto test = []() {
        hud::optional<type> option{ hud::in_place, 123, nullptr };
        const hud::optional<type> const_option{ hud::in_place, 456, nullptr };
        return std::tuple{
            (*option).id(),
            (*const_option).id()
        };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_EQ(std::get<0>(result), 123);
        ASSERT_EQ(std::get<1>(result), 456);
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_EQ(std::get<0>(result), 123);
        ASSERT_EQ(std::get<1>(result), 456);
    }
}
TEST(optional, reset_on_empty_do_nothing) {


    using type = hud::test::NonBitwiseType;

    const auto test = []() {
        hud::optional<type> option_empty;
        const bool has_value_before = option_empty.has_value();

        option_empty.reset();

        return std::tuple{
            has_value_before,
            option_empty.has_value(),
        };
    };


    // Non constant
    {
        const auto result = test();
        ASSERT_FALSE(std::get<0>(result));
        ASSERT_FALSE(std::get<1>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_FALSE(std::get<0>(result));
        ASSERT_FALSE(std::get<1>(result));
    }
}

TEST(optional, reset_call_destructor_if_T_is_not_trivially_destructible) {


    using type = hud::test::NonBitwiseType;

    static_assert(!hud::is_trivially_destructible_v<type>);

    const auto test = []() {
        i32 destructor_count;
        hud::optional<type> option{ hud::in_place, 123, &destructor_count };
        const bool has_value_before = option.has_value();
        const i32 destructor_count_before = destructor_count;

        option.reset();

        return std::tuple{
            has_value_before,
            destructor_count_before,
            option.has_value(),
            destructor_count
        };
    };


    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_EQ(std::get<1>(result), 0);
        ASSERT_FALSE(std::get<2>(result));
        ASSERT_EQ(std::get<3>(result), 1);
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_EQ(std::get<1>(result), 0);
        ASSERT_FALSE(std::get<2>(result));
        ASSERT_EQ(std::get<3>(result), 1);
    }
}
