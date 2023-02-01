#include <core/containers/optional.h>

GTEST_TEST(optional, swap_empty_with_empty_trivial_type)
{

    using type = i32;

    const auto test = []()
    {
        hud::optional<type> option;
        hud::optional<type> option2;
        option.swap(option2);
        return std::tuple {
            option.has_value(),
            option2.has_value()};
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_false(std::get<0>(result));
        hud_assert_false(std::get<1>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_false(std::get<0>(result));
        hud_assert_false(std::get<1>(result));
    }
}

GTEST_TEST(optional, swap_empty_with_empty_non_trivial_type)
{

    using type = hud_test::non_bitwise_type;

    const auto test = []()
    {
        hud::optional<type> option;
        hud::optional<type> option2;
        option.swap(option2);
        return std::tuple {
            option.has_value(),
            option2.has_value()};
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_false(std::get<0>(result));
        hud_assert_false(std::get<1>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_false(std::get<0>(result));
        hud_assert_false(std::get<1>(result));
    }
}

GTEST_TEST(optional, swap_empty_with_non_empty_trivial_type)
{

    using type = i32;

    const auto test = []()
    {
        hud::optional<type> option;
        hud::optional<type> option2 {123};
        option.swap(option2);
        return std::tuple {
            option.has_value(),
            option.value(),
            option2.has_value()};
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 123);
        hud_assert_false(std::get<2>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 123);
        hud_assert_false(std::get<2>(result));
    }
}

GTEST_TEST(optional, swap_empty_with_non_empty_non_trivial_type)
{

    using type = hud_test::non_bitwise_type;

    const auto test = []()
    {
        hud::optional<type> option;
        hud::optional<type> option2 {hud::in_place, 123, nullptr};
        option.swap(option2);
        return std::tuple {
            option.has_value(),
            option2.has_value()};
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_false(std::get<1>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_false(std::get<1>(result));
    }
}

GTEST_TEST(optional, swap_non_empty_with_non_empty_trivial_type)
{

    using type = i32;

    const auto test = []()
    {
        hud::optional<type> option {456};
        hud::optional<type> option2 {123};
        option.swap(option2);
        return std::tuple {
            option.has_value(),
            option.value(),
            option2.has_value(),
            option2.value()};
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 123);
        hud_assert_true(std::get<2>(result));
        hud_assert_eq(std::get<3>(result), 456);
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 123);
        hud_assert_true(std::get<2>(result));
        hud_assert_eq(std::get<3>(result), 456);
    }
}

GTEST_TEST(optional, swap_non_empty_with_non_empty_non_trivial_type)
{

    using type = hud_test::non_bitwise_type;

    const auto test = []()
    {
        hud::optional<type> option {hud::in_place, 456, nullptr};
        hud::optional<type> option2 {hud::in_place, 123, nullptr};
        option.swap(option2);
        return std::tuple {
            option.has_value(),
            option.value().id(),
            option2.has_value(),
            option2.value().id()};
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 123);
        hud_assert_true(std::get<2>(result));
        hud_assert_eq(std::get<3>(result), 456);
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 123);
        hud_assert_true(std::get<2>(result));
        hud_assert_eq(std::get<3>(result), 456);
    }
}

GTEST_TEST(optional, hud_swap_empty_with_empty_trivial_type)
{

    using type = i32;

    const auto test = []()
    {
        hud::optional<type> option;
        hud::optional<type> option2;
        hud::swap(option, option2);
        return std::tuple {
            option.has_value(),
            option2.has_value()};
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_false(std::get<0>(result));
        hud_assert_false(std::get<1>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_false(std::get<0>(result));
        hud_assert_false(std::get<1>(result));
    }
}

GTEST_TEST(optional, hud_swap_empty_with_empty_non_trivial_type)
{

    using type = hud_test::non_bitwise_type;

    const auto test = []()
    {
        hud::optional<type> option;
        hud::optional<type> option2;
        hud::swap(option, option2);
        return std::tuple {
            option.has_value(),
            option2.has_value()};
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_false(std::get<0>(result));
        hud_assert_false(std::get<1>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_false(std::get<0>(result));
        hud_assert_false(std::get<1>(result));
    }
}

GTEST_TEST(optional, hud_swap_empty_with_non_empty_trivial_type)
{

    using type = i32;

    const auto test = []()
    {
        hud::optional<type> option;
        hud::optional<type> option2 {123};
        hud::swap(option, option2);
        return std::tuple {
            option.has_value(),
            option.value(),
            option2.has_value()};
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 123);
        hud_assert_false(std::get<2>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 123);
        hud_assert_false(std::get<2>(result));
    }
}

GTEST_TEST(optional, hud_swap_empty_with_non_empty_non_trivial_type)
{

    using type = hud_test::non_bitwise_type;

    const auto test = []()
    {
        hud::optional<type> option;
        hud::optional<type> option2 {hud::in_place, 123, nullptr};
        hud::swap(option, option2);
        return std::tuple {
            option.has_value(),
            option2.has_value()};
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_false(std::get<1>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_false(std::get<1>(result));
    }
}

GTEST_TEST(optional, hud_swap_non_empty_with_non_empty_trivial_type)
{

    using type = i32;

    const auto test = []()
    {
        hud::optional<type> option {456};
        hud::optional<type> option2 {123};
        hud::swap(option, option2);
        return std::tuple {
            option.has_value(),
            option.value(),
            option2.has_value(),
            option2.value()};
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 123);
        hud_assert_true(std::get<2>(result));
        hud_assert_eq(std::get<3>(result), 456);
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 123);
        hud_assert_true(std::get<2>(result));
        hud_assert_eq(std::get<3>(result), 456);
    }
}

GTEST_TEST(optional, hud_swap_non_empty_with_non_empty_non_trivial_type)
{

    using type = hud_test::non_bitwise_type;

    const auto test = []()
    {
        hud::optional<type> option {hud::in_place, 456, nullptr};
        hud::optional<type> option2 {hud::in_place, 123, nullptr};
        hud::swap(option, option2);
        return std::tuple {
            option.has_value(),
            option.value().id(),
            option2.has_value(),
            option2.value().id()};
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 123);
        hud_assert_true(std::get<2>(result));
        hud_assert_eq(std::get<3>(result), 456);
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 123);
        hud_assert_true(std::get<2>(result));
        hud_assert_eq(std::get<3>(result), 456);
    }
}
