#include <core/containers/optional.h>

GTEST_TEST(optional, equal_operator) {

    
    using type = i32;

    // constexpr bool operator==(const hud::optional<Left>& left, const hud::optional<Right>& right) noexcept
    {
        const auto test = []() {
            hud::optional<type> empty;
            hud::optional<type> empty_2;
            hud::optional<type> non_empty{ 1 };
            hud::optional<type> non_empty_1{ 1 };
            hud::optional<type> non_empty_2{ 2 };

            const std::tuple test_empty{
                empty == empty,
                empty == empty_2,
                empty == non_empty,
                empty == non_empty_1,
                empty == non_empty_2,
            };

            const std::tuple test_non_empty{
                non_empty == empty,
                non_empty == empty_2,
                non_empty == non_empty,
                non_empty == non_empty_1,
                non_empty == non_empty_2
            };

            const std::tuple test_non_empty_1{
                non_empty_1 == empty,
                non_empty_1 == empty_2,
                non_empty_1 == non_empty,
                non_empty_1 == non_empty_1,
                non_empty_1 == non_empty_2
            };

            const std::tuple test_non_empty_2{
                non_empty_2 == empty,
                non_empty_2 == empty_2,
                non_empty_2 == non_empty,
                non_empty_2 == non_empty_1,
                non_empty_2 == non_empty_2
            };

            return std::tuple{
                test_empty,
                test_non_empty,
                test_non_empty_1,
                test_non_empty_2
            };
        };


        // Non constant
        {
            const auto result = test();

            const auto& test_empty = std::get<0>(result);
            ASSERT_TRUE(std::get<0>(test_empty));
            ASSERT_TRUE(std::get<1>(test_empty));
            ASSERT_FALSE(std::get<2>(test_empty));
            ASSERT_FALSE(std::get<3>(test_empty));
            ASSERT_FALSE(std::get<4>(test_empty));

            const auto& test_non_empty = std::get<1>(result);
            ASSERT_FALSE(std::get<0>(test_non_empty));
            ASSERT_FALSE(std::get<1>(test_non_empty));
            ASSERT_TRUE(std::get<2>(test_non_empty));
            ASSERT_TRUE(std::get<3>(test_non_empty));
            ASSERT_FALSE(std::get<4>(test_non_empty));

            const auto& test_non_empty_1 = std::get<2>(result);
            ASSERT_FALSE(std::get<0>(test_non_empty_1));
            ASSERT_FALSE(std::get<1>(test_non_empty_1));
            ASSERT_TRUE(std::get<2>(test_non_empty_1));
            ASSERT_TRUE(std::get<3>(test_non_empty_1));
            ASSERT_FALSE(std::get<4>(test_non_empty_1));

            const auto& test_non_empty_2 = std::get<3>(result);
            ASSERT_FALSE(std::get<0>(test_non_empty_2));
            ASSERT_FALSE(std::get<1>(test_non_empty_2));
            ASSERT_FALSE(std::get<2>(test_non_empty_2));
            ASSERT_FALSE(std::get<3>(test_non_empty_2));
            ASSERT_TRUE(std::get<4>(test_non_empty_2));
        }

        // Constant
        {
            constexpr auto result = test();

            const auto& test_empty = std::get<0>(result);
            ASSERT_TRUE(std::get<0>(test_empty));
            ASSERT_TRUE(std::get<1>(test_empty));
            ASSERT_FALSE(std::get<2>(test_empty));
            ASSERT_FALSE(std::get<3>(test_empty));
            ASSERT_FALSE(std::get<4>(test_empty));

            const auto& test_non_empty = std::get<1>(result);
            ASSERT_FALSE(std::get<0>(test_non_empty));
            ASSERT_FALSE(std::get<1>(test_non_empty));
            ASSERT_TRUE(std::get<2>(test_non_empty));
            ASSERT_TRUE(std::get<3>(test_non_empty));
            ASSERT_FALSE(std::get<4>(test_non_empty));

            const auto& test_non_empty_1 = std::get<2>(result);
            ASSERT_FALSE(std::get<0>(test_non_empty_1));
            ASSERT_FALSE(std::get<1>(test_non_empty_1));
            ASSERT_TRUE(std::get<2>(test_non_empty_1));
            ASSERT_TRUE(std::get<3>(test_non_empty_1));
            ASSERT_FALSE(std::get<4>(test_non_empty_1));

            const auto& test_non_empty_2 = std::get<3>(result);
            ASSERT_FALSE(std::get<0>(test_non_empty_2));
            ASSERT_FALSE(std::get<1>(test_non_empty_2));
            ASSERT_FALSE(std::get<2>(test_non_empty_2));
            ASSERT_FALSE(std::get<3>(test_non_empty_2));
            ASSERT_TRUE(std::get<4>(test_non_empty_2));
        }
    }


    // constexpr bool operator==(const hud::optional<TOption>& option, const TValue& value) noexcept
    {
        const auto test = []() {
            hud::optional<type> empty;
            hud::optional<type> non_empty{ 1 };

            const auto test_empty = std::tuple{
                empty == 0,
                empty == 1,
                empty == 2,
            };

            const auto test_non_empty = std::tuple{
                non_empty == 0,
                non_empty == 1,
                non_empty == 2,
            };

            return std::tuple{
                test_empty,
                test_non_empty
            };
        };

        // Non constant
        {
            const auto result = test();

            const auto& test_empty = std::get<0>(result);
            ASSERT_FALSE(std::get<0>(test_empty));
            ASSERT_FALSE(std::get<1>(test_empty));
            ASSERT_FALSE(std::get<2>(test_empty));

            const auto& test_non_empty = std::get<1>(result);
            ASSERT_FALSE(std::get<0>(test_non_empty));
            ASSERT_TRUE(std::get<1>(test_non_empty));
            ASSERT_FALSE(std::get<2>(test_non_empty));
        }

        // Non constant
        {
            constexpr auto result = test();

            const auto& test_empty = std::get<0>(result);
            ASSERT_FALSE(std::get<0>(test_empty));
            ASSERT_FALSE(std::get<1>(test_empty));
            ASSERT_FALSE(std::get<2>(test_empty));

            const auto& test_non_empty = std::get<1>(result);
            ASSERT_FALSE(std::get<0>(test_non_empty));
            ASSERT_TRUE(std::get<1>(test_non_empty));
            ASSERT_FALSE(std::get<2>(test_non_empty));
        }
    }

    // constexpr bool operator==(const TValue& value, const hud::optional<TOption>& option) noexcept
    {
        const auto test = []() {
            hud::optional<type> empty;
            hud::optional<type> non_empty{ 1 };

            const auto test_empty = std::tuple{
                0 == empty,
                1 == empty,
                2 == empty,
            };

            const auto test_non_empty = std::tuple{
                0 == non_empty,
                1 == non_empty,
                2 == non_empty,
            };

            return std::tuple{
                test_empty,
                test_non_empty
            };
        };

        // Non constant
        {
            const auto result = test();

            const auto& test_empty = std::get<0>(result);
            ASSERT_FALSE(std::get<0>(test_empty));
            ASSERT_FALSE(std::get<1>(test_empty));
            ASSERT_FALSE(std::get<2>(test_empty));

            const auto& test_non_empty = std::get<1>(result);
            ASSERT_FALSE(std::get<0>(test_non_empty));
            ASSERT_TRUE(std::get<1>(test_non_empty));
            ASSERT_FALSE(std::get<2>(test_non_empty));
        }

        // Non constant
        {
            constexpr auto result = test();

            const auto& test_empty = std::get<0>(result);
            ASSERT_FALSE(std::get<0>(test_empty));
            ASSERT_FALSE(std::get<1>(test_empty));
            ASSERT_FALSE(std::get<2>(test_empty));

            const auto& test_non_empty = std::get<1>(result);
            ASSERT_FALSE(std::get<0>(test_non_empty));
            ASSERT_TRUE(std::get<1>(test_non_empty));
            ASSERT_FALSE(std::get<2>(test_non_empty));
        }
    }
}

GTEST_TEST(optional, not_equal_operator) {


    using type = i32;

    // constexpr bool operator!=(const hud::optional<Left>& left, const hud::optional<Right>& right) noexcept
    {
        const auto test = []() {
            hud::optional<type> empty;
            hud::optional<type> empty_2;
            hud::optional<type> non_empty{ 1 };
            hud::optional<type> non_empty_1{ 1 };
            hud::optional<type> non_empty_2{ 2 };

            const std::tuple test_empty{
                empty != empty,
                empty != empty_2,
                empty != non_empty,
                empty != non_empty_1,
                empty != non_empty_2,
            };

            const std::tuple test_non_empty{
                non_empty != empty,
                non_empty != empty_2,
                non_empty != non_empty,
                non_empty != non_empty_1,
                non_empty != non_empty_2
            };

            const std::tuple test_non_empty_1{
                non_empty_1 != empty,
                non_empty_1 != empty_2,
                non_empty_1 != non_empty,
                non_empty_1 != non_empty_1,
                non_empty_1 != non_empty_2
            };

            const std::tuple test_non_empty_2{
                non_empty_2 != empty,
                non_empty_2 != empty_2,
                non_empty_2 != non_empty,
                non_empty_2 != non_empty_1,
                non_empty_2 != non_empty_2
            };

            return std::tuple{
                test_empty,
                test_non_empty,
                test_non_empty_1,
                test_non_empty_2
            };
        };


        // Non constant
        {
            const auto result = test();

            const auto& test_empty = std::get<0>(result);
            ASSERT_FALSE(std::get<0>(test_empty));
            ASSERT_FALSE(std::get<1>(test_empty));
            ASSERT_TRUE(std::get<2>(test_empty));
            ASSERT_TRUE(std::get<3>(test_empty));
            ASSERT_TRUE(std::get<4>(test_empty));

            const auto& test_non_empty = std::get<1>(result);
            ASSERT_TRUE(std::get<0>(test_non_empty));
            ASSERT_TRUE(std::get<1>(test_non_empty));
            ASSERT_FALSE(std::get<2>(test_non_empty));
            ASSERT_FALSE(std::get<3>(test_non_empty));
            ASSERT_TRUE(std::get<4>(test_non_empty));

            const auto& test_non_empty_1 = std::get<2>(result);
            ASSERT_TRUE(std::get<0>(test_non_empty_1));
            ASSERT_TRUE(std::get<1>(test_non_empty_1));
            ASSERT_FALSE(std::get<2>(test_non_empty_1));
            ASSERT_FALSE(std::get<3>(test_non_empty_1));
            ASSERT_TRUE(std::get<4>(test_non_empty_1));

            const auto& test_non_empty_2 = std::get<3>(result);
            ASSERT_TRUE(std::get<0>(test_non_empty_2));
            ASSERT_TRUE(std::get<1>(test_non_empty_2));
            ASSERT_TRUE(std::get<2>(test_non_empty_2));
            ASSERT_TRUE(std::get<3>(test_non_empty_2));
            ASSERT_FALSE(std::get<4>(test_non_empty_2));
        }

        // Constant
        {
            constexpr auto result = test();

            const auto& test_empty = std::get<0>(result);
            ASSERT_FALSE(std::get<0>(test_empty));
            ASSERT_FALSE(std::get<1>(test_empty));
            ASSERT_TRUE(std::get<2>(test_empty));
            ASSERT_TRUE(std::get<3>(test_empty));
            ASSERT_TRUE(std::get<4>(test_empty));

            const auto& test_non_empty = std::get<1>(result);
            ASSERT_TRUE(std::get<0>(test_non_empty));
            ASSERT_TRUE(std::get<1>(test_non_empty));
            ASSERT_FALSE(std::get<2>(test_non_empty));
            ASSERT_FALSE(std::get<3>(test_non_empty));
            ASSERT_TRUE(std::get<4>(test_non_empty));

            const auto& test_non_empty_1 = std::get<2>(result);
            ASSERT_TRUE(std::get<0>(test_non_empty_1));
            ASSERT_TRUE(std::get<1>(test_non_empty_1));
            ASSERT_FALSE(std::get<3>(test_non_empty_1));
            ASSERT_FALSE(std::get<2>(test_non_empty_1));
            ASSERT_TRUE(std::get<4>(test_non_empty_1));

            const auto& test_non_empty_2 = std::get<3>(result);
            ASSERT_TRUE(std::get<0>(test_non_empty_2));
            ASSERT_TRUE(std::get<1>(test_non_empty_2));
            ASSERT_TRUE(std::get<2>(test_non_empty_2));
            ASSERT_TRUE(std::get<3>(test_non_empty_2));
            ASSERT_FALSE(std::get<4>(test_non_empty_2));
        }
    }


    // constexpr bool operator!=(const hud::optional<TOption>& option, const TValue& value) noexcept
    {
        const auto test = []() {
            hud::optional<type> empty;
            hud::optional<type> non_empty{ 1 };

            const auto test_empty = std::tuple{
                empty != 0,
                empty != 1,
                empty != 2,
            };

            const auto test_non_empty = std::tuple{
                non_empty != 0,
                non_empty != 1,
                non_empty != 2,
            };

            return std::tuple{
                test_empty,
                test_non_empty
            };
        };

        // Non constant
        {
            const auto result = test();

            const auto& test_empty = std::get<0>(result);
            ASSERT_TRUE(std::get<0>(test_empty));
            ASSERT_TRUE(std::get<1>(test_empty));
            ASSERT_TRUE(std::get<2>(test_empty));

            const auto& test_non_empty = std::get<1>(result);
            ASSERT_TRUE(std::get<0>(test_non_empty));
            ASSERT_FALSE(std::get<1>(test_non_empty));
            ASSERT_TRUE(std::get<2>(test_non_empty));
        }

        // Non constant
        {
            constexpr auto result = test();

            const auto& test_empty = std::get<0>(result);
            ASSERT_TRUE(std::get<0>(test_empty));
            ASSERT_TRUE(std::get<1>(test_empty));
            ASSERT_TRUE(std::get<2>(test_empty));

            const auto& test_non_empty = std::get<1>(result);
            ASSERT_TRUE(std::get<0>(test_non_empty));
            ASSERT_FALSE(std::get<1>(test_non_empty));
            ASSERT_TRUE(std::get<2>(test_non_empty));
        }
    }

    // constexpr bool operator!=(const TValue& value, const hud::optional<TOption>& option) noexcept
    {
        const auto test = []() {
            hud::optional<type> empty;
            hud::optional<type> non_empty{ 1 };

            const auto test_empty = std::tuple{
                0 != empty,
                1 != empty,
                2 != empty,
            };

            const auto test_non_empty = std::tuple{
                0 != non_empty,
                1 != non_empty,
                2 != non_empty,
            };

            return std::tuple{
                test_empty,
                test_non_empty
            };
        };

        // Non constant
        {
            const auto result = test();

            const auto& test_empty = std::get<0>(result);
            ASSERT_TRUE(std::get<0>(test_empty));
            ASSERT_TRUE(std::get<1>(test_empty));
            ASSERT_TRUE(std::get<2>(test_empty));

            const auto& test_non_empty = std::get<1>(result);
            ASSERT_TRUE(std::get<0>(test_non_empty));
            ASSERT_FALSE(std::get<1>(test_non_empty));
            ASSERT_TRUE(std::get<2>(test_non_empty));
        }

        // Non constant
        {
            constexpr auto result = test();

            const auto& test_empty = std::get<0>(result);
            ASSERT_TRUE(std::get<0>(test_empty));
            ASSERT_TRUE(std::get<1>(test_empty));
            ASSERT_TRUE(std::get<2>(test_empty));

            const auto& test_non_empty = std::get<1>(result);
            ASSERT_TRUE(std::get<0>(test_non_empty));
            ASSERT_FALSE(std::get<1>(test_non_empty));
            ASSERT_TRUE(std::get<2>(test_non_empty));
        }
    }
}

GTEST_TEST(optional, less_operator) {


    using type = i32;

    // constexpr bool operator<(const hud::optional<Left>& left, const hud::optional<Right>& right) noexcept
    {
        const auto test = []() {
            hud::optional<type> empty;
            hud::optional<type> empty_2;
            hud::optional<type> non_empty{ 1 };
            hud::optional<type> non_empty_1{ 1 };
            hud::optional<type> non_empty_2{ 2 };

            const std::tuple test_empty{
                empty < empty,
                empty < empty_2,
                empty < non_empty,
                empty < non_empty_1,
                empty < non_empty_2,
            };

            const std::tuple test_non_empty{
                non_empty < empty,
                non_empty < empty_2,
                non_empty < non_empty,
                non_empty < non_empty_1,
                non_empty < non_empty_2
            };

            const std::tuple test_non_empty_1{
                non_empty_1 < empty,
                non_empty_1 < empty_2,
                non_empty_1 < non_empty,
                non_empty_1 < non_empty_1,
                non_empty_1 < non_empty_2
            };

            const std::tuple test_non_empty_2{
                non_empty_2 < empty,
                non_empty_2 < empty_2,
                non_empty_2 < non_empty,
                non_empty_2 < non_empty_1,
                non_empty_2 < non_empty_2
            };

            return std::tuple{
                test_empty,
                test_non_empty,
                test_non_empty_1,
                test_non_empty_2
            };
        };


        // Non constant
        {
            const auto result = test();

            const auto& test_empty = std::get<0>(result);
            ASSERT_FALSE(std::get<0>(test_empty));
            ASSERT_FALSE(std::get<1>(test_empty));
            ASSERT_TRUE(std::get<2>(test_empty));
            ASSERT_TRUE(std::get<3>(test_empty));
            ASSERT_TRUE(std::get<4>(test_empty));

            const auto& test_non_empty = std::get<1>(result);
            ASSERT_FALSE(std::get<0>(test_non_empty));
            ASSERT_FALSE(std::get<1>(test_non_empty));
            ASSERT_FALSE(std::get<2>(test_non_empty));
            ASSERT_FALSE(std::get<3>(test_non_empty));
            ASSERT_TRUE(std::get<4>(test_non_empty));

            const auto& test_non_empty_1 = std::get<2>(result);
            ASSERT_FALSE(std::get<0>(test_non_empty_1));
            ASSERT_FALSE(std::get<1>(test_non_empty_1));
            ASSERT_FALSE(std::get<2>(test_non_empty_1));
            ASSERT_FALSE(std::get<3>(test_non_empty_1));
            ASSERT_TRUE(std::get<4>(test_non_empty_1));

            const auto& test_non_empty_2 = std::get<3>(result);
            ASSERT_FALSE(std::get<0>(test_non_empty_2));
            ASSERT_FALSE(std::get<1>(test_non_empty_2));
            ASSERT_FALSE(std::get<2>(test_non_empty_2));
            ASSERT_FALSE(std::get<3>(test_non_empty_2));
            ASSERT_FALSE(std::get<4>(test_non_empty_2));
        }

        // Constant
        {
            constexpr auto result = test();

            const auto& test_empty = std::get<0>(result);
            ASSERT_FALSE(std::get<0>(test_empty));
            ASSERT_FALSE(std::get<1>(test_empty));
            ASSERT_TRUE(std::get<2>(test_empty));
            ASSERT_TRUE(std::get<3>(test_empty));
            ASSERT_TRUE(std::get<4>(test_empty));

            const auto& test_non_empty = std::get<1>(result);
            ASSERT_FALSE(std::get<0>(test_non_empty));
            ASSERT_FALSE(std::get<1>(test_non_empty));
            ASSERT_FALSE(std::get<2>(test_non_empty));
            ASSERT_FALSE(std::get<3>(test_non_empty));
            ASSERT_TRUE(std::get<4>(test_non_empty));

            const auto& test_non_empty_1 = std::get<2>(result);
            ASSERT_FALSE(std::get<0>(test_non_empty_1));
            ASSERT_FALSE(std::get<1>(test_non_empty_1));
            ASSERT_FALSE(std::get<2>(test_non_empty_1));
            ASSERT_FALSE(std::get<3>(test_non_empty_1));
            ASSERT_TRUE(std::get<4>(test_non_empty_1));

            const auto& test_non_empty_2 = std::get<3>(result);
            ASSERT_FALSE(std::get<0>(test_non_empty_2));
            ASSERT_FALSE(std::get<1>(test_non_empty_2));
            ASSERT_FALSE(std::get<2>(test_non_empty_2));
            ASSERT_FALSE(std::get<3>(test_non_empty_2));
            ASSERT_FALSE(std::get<4>(test_non_empty_2));
        }
    }


    // constexpr bool operator<(const hud::optional<TOption>& option, const TValue& value) noexcept
    {
        const auto test = []() {
            hud::optional<type> empty;
            hud::optional<type> non_empty{ 1 };

            const auto test_empty = std::tuple{
                empty < 0,
                empty < 1,
                empty < 2,
            };

            const auto test_non_empty = std::tuple{
                non_empty < 0,
                non_empty < 1,
                non_empty < 2,
            };

            return std::tuple{
                test_empty,
                test_non_empty
            };
        };

        // Non constant
        {
            const auto result = test();

            const auto& test_empty = std::get<0>(result);
            ASSERT_TRUE(std::get<0>(test_empty));
            ASSERT_TRUE(std::get<1>(test_empty));
            ASSERT_TRUE(std::get<2>(test_empty));

            const auto& test_non_empty = std::get<1>(result);
            ASSERT_FALSE(std::get<0>(test_non_empty));
            ASSERT_FALSE(std::get<1>(test_non_empty));
            ASSERT_TRUE(std::get<2>(test_non_empty));
        }

        // Non constant
        {
            constexpr auto result = test();

            const auto& test_empty = std::get<0>(result);
            ASSERT_TRUE(std::get<0>(test_empty));
            ASSERT_TRUE(std::get<1>(test_empty));
            ASSERT_TRUE(std::get<2>(test_empty));

            const auto& test_non_empty = std::get<1>(result);
            ASSERT_FALSE(std::get<0>(test_non_empty));
            ASSERT_FALSE(std::get<1>(test_non_empty));
            ASSERT_TRUE(std::get<2>(test_non_empty));
        }
    }

    // constexpr bool operator<(const TValue& value, const hud::optional<TOption>& option) noexcept
    {
        const auto test = []() {
            hud::optional<type> empty;
            hud::optional<type> non_empty{ 1 };

            const auto test_empty = std::tuple{
                0 < empty,
                1 < empty,
                2 < empty,
            };

            const auto test_non_empty = std::tuple{
                0 < non_empty,
                1 < non_empty,
                2 < non_empty,
            };

            return std::tuple{
                test_empty,
                test_non_empty
            };
        };

        // Non constant
        {
            const auto result = test();

            const auto& test_empty = std::get<0>(result);
            ASSERT_FALSE(std::get<0>(test_empty));
            ASSERT_FALSE(std::get<1>(test_empty));
            ASSERT_FALSE(std::get<2>(test_empty));

            const auto& test_non_empty = std::get<1>(result);
            ASSERT_TRUE(std::get<0>(test_non_empty));
            ASSERT_FALSE(std::get<1>(test_non_empty));
            ASSERT_FALSE(std::get<2>(test_non_empty));
        }

        // Non constant
        {
            constexpr auto result = test();

            const auto& test_empty = std::get<0>(result);
            ASSERT_FALSE(std::get<0>(test_empty));
            ASSERT_FALSE(std::get<1>(test_empty));
            ASSERT_FALSE(std::get<2>(test_empty));

            const auto& test_non_empty = std::get<1>(result);
            ASSERT_TRUE(std::get<0>(test_non_empty));
            ASSERT_FALSE(std::get<1>(test_non_empty));
            ASSERT_FALSE(std::get<2>(test_non_empty));
        }
    }
}

GTEST_TEST(optional, greater_operator) {


    using type = i32;

    // constexpr bool operator>(const hud::optional<Left>& left, const hud::optional<Right>& right) noexcept
    {
        const auto test = []() {
            hud::optional<type> empty;
            hud::optional<type> empty_2;
            hud::optional<type> non_empty{ 1 };
            hud::optional<type> non_empty_1{ 1 };
            hud::optional<type> non_empty_2{ 2 };

            const std::tuple test_empty{
                empty > empty,
                empty > empty_2,
                empty > non_empty,
                empty > non_empty_1,
                empty > non_empty_2,
            };

            const std::tuple test_non_empty{
                non_empty > empty,
                non_empty > empty_2,
                non_empty > non_empty,
                non_empty > non_empty_1,
                non_empty > non_empty_2
            };

            const std::tuple test_non_empty_1{
                non_empty_1 > empty,
                non_empty_1 > empty_2,
                non_empty_1 > non_empty,
                non_empty_1 > non_empty_1,
                non_empty_1 > non_empty_2
            };

            const std::tuple test_non_empty_2{
                non_empty_2 > empty,
                non_empty_2 > empty_2,
                non_empty_2 > non_empty,
                non_empty_2 > non_empty_1,
                non_empty_2 > non_empty_2
            };

            return std::tuple{
                test_empty,
                test_non_empty,
                test_non_empty_1,
                test_non_empty_2
            };
        };


        // Non constant
        {
            const auto result = test();

            const auto& test_empty = std::get<0>(result);
            ASSERT_FALSE(std::get<0>(test_empty));
            ASSERT_FALSE(std::get<1>(test_empty));
            ASSERT_FALSE(std::get<2>(test_empty));
            ASSERT_FALSE(std::get<3>(test_empty));
            ASSERT_FALSE(std::get<4>(test_empty));

            const auto& test_non_empty = std::get<1>(result);
            ASSERT_TRUE(std::get<0>(test_non_empty));
            ASSERT_TRUE(std::get<1>(test_non_empty));
            ASSERT_FALSE(std::get<2>(test_non_empty));
            ASSERT_FALSE(std::get<3>(test_non_empty));
            ASSERT_FALSE(std::get<4>(test_non_empty));

            const auto& test_non_empty_1 = std::get<2>(result);
            ASSERT_TRUE(std::get<0>(test_non_empty_1));
            ASSERT_TRUE(std::get<1>(test_non_empty_1));
            ASSERT_FALSE(std::get<2>(test_non_empty_1));
            ASSERT_FALSE(std::get<3>(test_non_empty_1));
            ASSERT_FALSE(std::get<4>(test_non_empty_1));

            const auto& test_non_empty_2 = std::get<3>(result);
            ASSERT_TRUE(std::get<0>(test_non_empty_2));
            ASSERT_TRUE(std::get<1>(test_non_empty_2));
            ASSERT_TRUE(std::get<2>(test_non_empty_2));
            ASSERT_TRUE(std::get<3>(test_non_empty_2));
            ASSERT_FALSE(std::get<4>(test_non_empty_2));
        }

        // Constant
        {
            constexpr auto result = test();

            const auto& test_empty = std::get<0>(result);
            ASSERT_FALSE(std::get<0>(test_empty));
            ASSERT_FALSE(std::get<1>(test_empty));
            ASSERT_FALSE(std::get<2>(test_empty));
            ASSERT_FALSE(std::get<3>(test_empty));
            ASSERT_FALSE(std::get<4>(test_empty));

            const auto& test_non_empty = std::get<1>(result);
            ASSERT_TRUE(std::get<0>(test_non_empty));
            ASSERT_TRUE(std::get<1>(test_non_empty));
            ASSERT_FALSE(std::get<2>(test_non_empty));
            ASSERT_FALSE(std::get<3>(test_non_empty));
            ASSERT_FALSE(std::get<4>(test_non_empty));

            const auto& test_non_empty_1 = std::get<2>(result);
            ASSERT_TRUE(std::get<0>(test_non_empty_1));
            ASSERT_TRUE(std::get<1>(test_non_empty_1));
            ASSERT_FALSE(std::get<2>(test_non_empty_1));
            ASSERT_FALSE(std::get<3>(test_non_empty_1));
            ASSERT_FALSE(std::get<4>(test_non_empty_1));

            const auto& test_non_empty_2 = std::get<3>(result);
            ASSERT_TRUE(std::get<0>(test_non_empty_2));
            ASSERT_TRUE(std::get<1>(test_non_empty_2));
            ASSERT_TRUE(std::get<2>(test_non_empty_2));
            ASSERT_TRUE(std::get<3>(test_non_empty_2));
            ASSERT_FALSE(std::get<4>(test_non_empty_2));
        }
    }


    // constexpr bool operator>(const hud::optional<TOption>& option, const TValue& value) noexcept
    {
        const auto test = []() {
            hud::optional<type> empty;
            hud::optional<type> non_empty{ 1 };

            const auto test_empty = std::tuple{
                empty > 0,
                empty > 1,
                empty > 2,
            };

            const auto test_non_empty = std::tuple{
                non_empty > 0,
                non_empty > 1,
                non_empty > 2,
            };

            return std::tuple{
                test_empty,
                test_non_empty
            };
        };

        // Non constant
        {
            const auto result = test();

            const auto& test_empty = std::get<0>(result);
            ASSERT_FALSE(std::get<0>(test_empty));
            ASSERT_FALSE(std::get<1>(test_empty));
            ASSERT_FALSE(std::get<2>(test_empty));

            const auto& test_non_empty = std::get<1>(result);
            ASSERT_TRUE(std::get<0>(test_non_empty));
            ASSERT_FALSE(std::get<1>(test_non_empty));
            ASSERT_FALSE(std::get<2>(test_non_empty));
        }

        // Non constant
        {
            constexpr auto result = test();

            const auto& test_empty = std::get<0>(result);
            ASSERT_FALSE(std::get<0>(test_empty));
            ASSERT_FALSE(std::get<1>(test_empty));
            ASSERT_FALSE(std::get<2>(test_empty));

            const auto& test_non_empty = std::get<1>(result);
            ASSERT_TRUE(std::get<0>(test_non_empty));
            ASSERT_FALSE(std::get<1>(test_non_empty));
            ASSERT_FALSE(std::get<2>(test_non_empty));
        }
    }

    // constexpr bool operator>(const TValue& value, const hud::optional<TOption>& option) noexcept
    {
        const auto test = []() {
            hud::optional<type> empty;
            hud::optional<type> non_empty{ 1 };

            const auto test_empty = std::tuple{
                0 > empty,
                1 > empty,
                2 > empty,
            };

            const auto test_non_empty = std::tuple{
                0 > non_empty,
                1 > non_empty,
                2 > non_empty,
            };

            return std::tuple{
                test_empty,
                test_non_empty
            };
        };

        // Non constant
        {
            const auto result = test();

            const auto& test_empty = std::get<0>(result);
            ASSERT_TRUE(std::get<0>(test_empty));
            ASSERT_TRUE(std::get<1>(test_empty));
            ASSERT_TRUE(std::get<2>(test_empty));

            const auto& test_non_empty = std::get<1>(result);
            ASSERT_FALSE(std::get<0>(test_non_empty));
            ASSERT_FALSE(std::get<1>(test_non_empty));
            ASSERT_TRUE(std::get<2>(test_non_empty));
        }

        // Non constant
        {
            constexpr auto result = test();

            const auto& test_empty = std::get<0>(result);
            ASSERT_TRUE(std::get<0>(test_empty));
            ASSERT_TRUE(std::get<1>(test_empty));
            ASSERT_TRUE(std::get<2>(test_empty));

            const auto& test_non_empty = std::get<1>(result);
            ASSERT_FALSE(std::get<0>(test_non_empty));
            ASSERT_FALSE(std::get<1>(test_non_empty));
            ASSERT_TRUE(std::get<2>(test_non_empty));
        }
    }
}

GTEST_TEST(optional, less_equal_operator) {


    using type = i32;

    // constexpr bool operator<=(const hud::optional<Left>& left, const hud::optional<Right>& right) noexcept
    {
        const auto test = []() {
            hud::optional<type> empty;
            hud::optional<type> empty_2;
            hud::optional<type> non_empty{ 1 };
            hud::optional<type> non_empty_1{ 1 };
            hud::optional<type> non_empty_2{ 2 };

            const std::tuple test_empty{
                empty <= empty,
                empty <= empty_2,
                empty <= non_empty,
                empty <= non_empty_1,
                empty <= non_empty_2,
            };

            const std::tuple test_non_empty{
                non_empty <= empty,
                non_empty <= empty_2,
                non_empty <= non_empty,
                non_empty <= non_empty_1,
                non_empty <= non_empty_2
            };

            const std::tuple test_non_empty_1{
                non_empty_1 <= empty,
                non_empty_1 <= empty_2,
                non_empty_1 <= non_empty,
                non_empty_1 <= non_empty_1,
                non_empty_1 <= non_empty_2
            };

            const std::tuple test_non_empty_2{
                non_empty_2 <= empty,
                non_empty_2 <= empty_2,
                non_empty_2 <= non_empty,
                non_empty_2 <= non_empty_1,
                non_empty_2 <= non_empty_2
            };

            return std::tuple{
                test_empty,
                test_non_empty,
                test_non_empty_1,
                test_non_empty_2
            };
        };


        // Non constant
        {
            const auto result = test();

            const auto& test_empty = std::get<0>(result);
            ASSERT_TRUE(std::get<0>(test_empty));
            ASSERT_TRUE(std::get<1>(test_empty));
            ASSERT_TRUE(std::get<2>(test_empty));
            ASSERT_TRUE(std::get<3>(test_empty));
            ASSERT_TRUE(std::get<4>(test_empty));

            const auto& test_non_empty = std::get<1>(result);
            ASSERT_FALSE(std::get<0>(test_non_empty));
            ASSERT_FALSE(std::get<1>(test_non_empty));
            ASSERT_TRUE(std::get<2>(test_non_empty));
            ASSERT_TRUE(std::get<3>(test_non_empty));
            ASSERT_TRUE(std::get<4>(test_non_empty));

            const auto& test_non_empty_1 = std::get<2>(result);
            ASSERT_FALSE(std::get<0>(test_non_empty_1));
            ASSERT_FALSE(std::get<1>(test_non_empty_1));
            ASSERT_TRUE(std::get<2>(test_non_empty_1));
            ASSERT_TRUE(std::get<3>(test_non_empty_1));
            ASSERT_TRUE(std::get<4>(test_non_empty_1));

            const auto& test_non_empty_2 = std::get<3>(result);
            ASSERT_FALSE(std::get<0>(test_non_empty_2));
            ASSERT_FALSE(std::get<1>(test_non_empty_2));
            ASSERT_FALSE(std::get<2>(test_non_empty_2));
            ASSERT_FALSE(std::get<3>(test_non_empty_2));
            ASSERT_TRUE(std::get<4>(test_non_empty_2));
        }

        // Constant
        {
            constexpr auto result = test();

            const auto& test_empty = std::get<0>(result);
            ASSERT_TRUE(std::get<0>(test_empty));
            ASSERT_TRUE(std::get<1>(test_empty));
            ASSERT_TRUE(std::get<2>(test_empty));
            ASSERT_TRUE(std::get<3>(test_empty));
            ASSERT_TRUE(std::get<4>(test_empty));

            const auto& test_non_empty = std::get<1>(result);
            ASSERT_FALSE(std::get<0>(test_non_empty));
            ASSERT_FALSE(std::get<1>(test_non_empty));
            ASSERT_TRUE(std::get<2>(test_non_empty));
            ASSERT_TRUE(std::get<3>(test_non_empty));
            ASSERT_TRUE(std::get<4>(test_non_empty));

            const auto& test_non_empty_1 = std::get<2>(result);
            ASSERT_FALSE(std::get<0>(test_non_empty_1));
            ASSERT_FALSE(std::get<1>(test_non_empty_1));
            ASSERT_TRUE(std::get<2>(test_non_empty_1));
            ASSERT_TRUE(std::get<3>(test_non_empty_1));
            ASSERT_TRUE(std::get<4>(test_non_empty_1));

            const auto& test_non_empty_2 = std::get<3>(result);
            ASSERT_FALSE(std::get<0>(test_non_empty_2));
            ASSERT_FALSE(std::get<1>(test_non_empty_2));
            ASSERT_FALSE(std::get<2>(test_non_empty_2));
            ASSERT_FALSE(std::get<3>(test_non_empty_2));
            ASSERT_TRUE(std::get<4>(test_non_empty_2));
        }
    }


    // constexpr bool operator<=(const hud::optional<TOption>& option, const TValue& value) noexcept
    {
        const auto test = []() {
            hud::optional<type> empty;
            hud::optional<type> non_empty{ 1 };

            const auto test_empty = std::tuple{
                empty <= 0,
                empty <= 1,
                empty <= 2,
            };

            const auto test_non_empty = std::tuple{
                non_empty <= 0,
                non_empty <= 1,
                non_empty <= 2,
            };

            return std::tuple{
                test_empty,
                test_non_empty
            };
        };

        // Non constant
        {
            const auto result = test();

            const auto& test_empty = std::get<0>(result);
            ASSERT_TRUE(std::get<0>(test_empty));
            ASSERT_TRUE(std::get<1>(test_empty));
            ASSERT_TRUE(std::get<2>(test_empty));

            const auto& test_non_empty = std::get<1>(result);
            ASSERT_FALSE(std::get<0>(test_non_empty));
            ASSERT_TRUE(std::get<1>(test_non_empty));
            ASSERT_TRUE(std::get<2>(test_non_empty));
        }

        // Non constant
        {
            constexpr auto result = test();

            const auto& test_empty = std::get<0>(result);
            ASSERT_TRUE(std::get<0>(test_empty));
            ASSERT_TRUE(std::get<1>(test_empty));
            ASSERT_TRUE(std::get<2>(test_empty));

            const auto& test_non_empty = std::get<1>(result);
            ASSERT_FALSE(std::get<0>(test_non_empty));
            ASSERT_TRUE(std::get<1>(test_non_empty));
            ASSERT_TRUE(std::get<2>(test_non_empty));
        }
    }

    // constexpr bool operator<=(const TValue& value, const hud::optional<TOption>& option) noexcept
    {
        const auto test = []() {
            hud::optional<type> empty;
            hud::optional<type> non_empty{ 1 };

            const auto test_empty = std::tuple{
                0 <= empty,
                1 <= empty,
                2 <= empty,
            };

            const auto test_non_empty = std::tuple{
                0 <= non_empty,
                1 <= non_empty,
                2 <= non_empty,
            };

            return std::tuple{
                test_empty,
                test_non_empty
            };
        };

        // Non constant
        {
            const auto result = test();

            const auto& test_empty = std::get<0>(result);
            ASSERT_FALSE(std::get<0>(test_empty));
            ASSERT_FALSE(std::get<1>(test_empty));
            ASSERT_FALSE(std::get<2>(test_empty));

            const auto& test_non_empty = std::get<1>(result);
            ASSERT_TRUE(std::get<0>(test_non_empty));
            ASSERT_TRUE(std::get<1>(test_non_empty));
            ASSERT_FALSE(std::get<2>(test_non_empty));
        }

        // Non constant
        {
            constexpr auto result = test();

            const auto& test_empty = std::get<0>(result);
            ASSERT_FALSE(std::get<0>(test_empty));
            ASSERT_FALSE(std::get<1>(test_empty));
            ASSERT_FALSE(std::get<2>(test_empty));

            const auto& test_non_empty = std::get<1>(result);
            ASSERT_TRUE(std::get<0>(test_non_empty));
            ASSERT_TRUE(std::get<1>(test_non_empty));
            ASSERT_FALSE(std::get<2>(test_non_empty));
        }
    }
}

GTEST_TEST(optional, greater_equal_operator) {


    using type = i32;

    // constexpr bool operator>=(const hud::optional<Left>& left, const hud::optional<Right>& right) noexcept
    {
        const auto test = []() {
            hud::optional<type> empty;
            hud::optional<type> empty_2;
            hud::optional<type> non_empty{ 1 };
            hud::optional<type> non_empty_1{ 1 };
            hud::optional<type> non_empty_2{ 2 };

            const std::tuple test_empty{
                empty >= empty,
                empty >= empty_2,
                empty >= non_empty,
                empty >= non_empty_1,
                empty >= non_empty_2,
            };

            const std::tuple test_non_empty{
                non_empty >= empty,
                non_empty >= empty_2,
                non_empty >= non_empty,
                non_empty >= non_empty_1,
                non_empty >= non_empty_2
            };

            const std::tuple test_non_empty_1{
                non_empty_1 >= empty,
                non_empty_1 >= empty_2,
                non_empty_1 >= non_empty,
                non_empty_1 >= non_empty_1,
                non_empty_1 >= non_empty_2
            };

            const std::tuple test_non_empty_2{
                non_empty_2 >= empty,
                non_empty_2 >= empty_2,
                non_empty_2 >= non_empty,
                non_empty_2 >= non_empty_1,
                non_empty_2 >= non_empty_2
            };

            return std::tuple{
                test_empty,
                test_non_empty,
                test_non_empty_1,
                test_non_empty_2
            };
        };


        // Non constant
        {
            const auto result = test();

            const auto& test_empty = std::get<0>(result);
            ASSERT_TRUE(std::get<0>(test_empty));
            ASSERT_TRUE(std::get<1>(test_empty));
            ASSERT_FALSE(std::get<2>(test_empty));
            ASSERT_FALSE(std::get<3>(test_empty));
            ASSERT_FALSE(std::get<4>(test_empty));

            const auto& test_non_empty = std::get<1>(result);
            ASSERT_TRUE(std::get<0>(test_non_empty));
            ASSERT_TRUE(std::get<1>(test_non_empty));
            ASSERT_TRUE(std::get<2>(test_non_empty));
            ASSERT_TRUE(std::get<3>(test_non_empty));
            ASSERT_FALSE(std::get<4>(test_non_empty));

            const auto& test_non_empty_1 = std::get<2>(result);
            ASSERT_TRUE(std::get<0>(test_non_empty_1));
            ASSERT_TRUE(std::get<1>(test_non_empty_1));
            ASSERT_TRUE(std::get<2>(test_non_empty_1));
            ASSERT_TRUE(std::get<3>(test_non_empty_1));
            ASSERT_FALSE(std::get<4>(test_non_empty_1));

            const auto& test_non_empty_2 = std::get<3>(result);
            ASSERT_TRUE(std::get<0>(test_non_empty_2));
            ASSERT_TRUE(std::get<1>(test_non_empty_2));
            ASSERT_TRUE(std::get<2>(test_non_empty_2));
            ASSERT_TRUE(std::get<3>(test_non_empty_2));
            ASSERT_TRUE(std::get<4>(test_non_empty_2));
        }

        // Constant
        {
            constexpr auto result = test();

            const auto& test_empty = std::get<0>(result);
            ASSERT_TRUE(std::get<0>(test_empty));
            ASSERT_TRUE(std::get<1>(test_empty));
            ASSERT_FALSE(std::get<2>(test_empty));
            ASSERT_FALSE(std::get<3>(test_empty));
            ASSERT_FALSE(std::get<4>(test_empty));

            const auto& test_non_empty = std::get<1>(result);
            ASSERT_TRUE(std::get<0>(test_non_empty));
            ASSERT_TRUE(std::get<1>(test_non_empty));
            ASSERT_TRUE(std::get<2>(test_non_empty));
            ASSERT_TRUE(std::get<3>(test_non_empty));
            ASSERT_FALSE(std::get<4>(test_non_empty));

            const auto& test_non_empty_1 = std::get<2>(result);
            ASSERT_TRUE(std::get<0>(test_non_empty_1));
            ASSERT_TRUE(std::get<1>(test_non_empty_1));
            ASSERT_TRUE(std::get<2>(test_non_empty_1));
            ASSERT_TRUE(std::get<3>(test_non_empty_1));
            ASSERT_FALSE(std::get<4>(test_non_empty_1));

            const auto& test_non_empty_2 = std::get<3>(result);
            ASSERT_TRUE(std::get<0>(test_non_empty_2));
            ASSERT_TRUE(std::get<1>(test_non_empty_2));
            ASSERT_TRUE(std::get<2>(test_non_empty_2));
            ASSERT_TRUE(std::get<3>(test_non_empty_2));
            ASSERT_TRUE(std::get<4>(test_non_empty_2));
        }
    }


    // constexpr bool operator>=(const hud::optional<TOption>& option, const TValue& value) noexcept
    {
        const auto test = []() {
            hud::optional<type> empty;
            hud::optional<type> non_empty{ 1 };

            const auto test_empty = std::tuple{
                empty >= 0,
                empty >= 1,
                empty >= 2,
            };

            const auto test_non_empty = std::tuple{
                non_empty >= 0,
                non_empty >= 1,
                non_empty >= 2,
            };

            return std::tuple{
                test_empty,
                test_non_empty
            };
        };

        // Non constant
        {
            const auto result = test();

            const auto& test_empty = std::get<0>(result);
            ASSERT_FALSE(std::get<0>(test_empty));
            ASSERT_FALSE(std::get<1>(test_empty));
            ASSERT_FALSE(std::get<2>(test_empty));

            const auto& test_non_empty = std::get<1>(result);
            ASSERT_TRUE(std::get<0>(test_non_empty));
            ASSERT_TRUE(std::get<1>(test_non_empty));
            ASSERT_FALSE(std::get<2>(test_non_empty));
        }

        // Non constant
        {
            constexpr auto result = test();

            const auto& test_empty = std::get<0>(result);
            ASSERT_FALSE(std::get<0>(test_empty));
            ASSERT_FALSE(std::get<1>(test_empty));
            ASSERT_FALSE(std::get<2>(test_empty));

            const auto& test_non_empty = std::get<1>(result);
            ASSERT_TRUE(std::get<0>(test_non_empty));
            ASSERT_TRUE(std::get<1>(test_non_empty));
            ASSERT_FALSE(std::get<2>(test_non_empty));
        }
    }

    // constexpr bool operator>=(const TValue& value, const hud::optional<TOption>& option) noexcept
    {
        const auto test = []() {
            hud::optional<type> empty;
            hud::optional<type> non_empty{ 1 };

            const auto test_empty = std::tuple{
                0 >= empty,
                1 >= empty,
                2 >= empty,
            };

            const auto test_non_empty = std::tuple{
                0 >= non_empty,
                1 >= non_empty,
                2 >= non_empty,
            };

            return std::tuple{
                test_empty,
                test_non_empty
            };
        };

        // Non constant
        {
            const auto result = test();

            const auto& test_empty = std::get<0>(result);
            ASSERT_TRUE(std::get<0>(test_empty));
            ASSERT_TRUE(std::get<1>(test_empty));
            ASSERT_TRUE(std::get<2>(test_empty));

            const auto& test_non_empty = std::get<1>(result);
            ASSERT_FALSE(std::get<0>(test_non_empty));
            ASSERT_TRUE(std::get<1>(test_non_empty));
            ASSERT_TRUE(std::get<2>(test_non_empty));
        }

        // Non constant
        {
            constexpr auto result = test();

            const auto& test_empty = std::get<0>(result);
            ASSERT_TRUE(std::get<0>(test_empty));
            ASSERT_TRUE(std::get<1>(test_empty));
            ASSERT_TRUE(std::get<2>(test_empty));

            const auto& test_non_empty = std::get<1>(result);
            ASSERT_FALSE(std::get<0>(test_non_empty));
            ASSERT_TRUE(std::get<1>(test_non_empty));
            ASSERT_TRUE(std::get<2>(test_non_empty));
        }
    }
}