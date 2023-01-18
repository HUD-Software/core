#include <core/containers/optional.h>
#include <core/traits/is_trivially_default_constructible.h>
#include <optional>

GTEST_TEST(optional, default_constructor_trivially_default_constructible_type)
{

    using type = i32;
    static_assert(hud::is_trivially_default_constructible_v<type>);

    static_assert(!hud::is_trivially_default_constructible_v<hud::optional<type>>);
    static_assert(hud::is_trivially_default_constructible_v<hud::optional<type>> == hud::is_trivially_default_constructible_v<std::optional<type>>);

    auto test = []()
    {
        hud::optional<type> option;
        return option.has_value();
    };

    // Non constant
    {
        GTEST_ASSERT_FALSE(test());
    }

    // Constant
    {
        constexpr bool has_value = test();
        GTEST_ASSERT_FALSE(has_value);
    }
}

GTEST_TEST(optional, default_constructor_non_trivially_default_constructible_type)
{

    using type = hud_test::non_bitwise_type;

    static_assert(!hud::is_trivially_default_constructible_v<type>);

    // If hud::optional<type> is not trivially default constructible
    // std::optional<type> is also non trivially default constructible
    static_assert(!hud::is_trivially_default_constructible_v<hud::optional<type>>);
    static_assert(hud::is_trivially_default_constructible_v<hud::optional<type>> == hud::is_trivially_default_constructible_v<std::optional<type>>);

    auto test = []()
    {
        hud::optional<type> option;
        return option.has_value();
    };

    // Non constant
    {
        GTEST_ASSERT_FALSE(test());
    }

    // Constant
    {
        constexpr bool has_value = test();
        GTEST_ASSERT_FALSE(has_value);
    }
}

GTEST_TEST(optional, nullopt_constructor)
{

    auto test = []()
    {
        hud::optional<hud_test::non_bitwise_type> option(hud::nullopt);
        return option.has_value();
    };

    // Non constant
    {
        GTEST_ASSERT_FALSE(test());
    }

    // Constant
    {
        constexpr bool has_value = test();
        GTEST_ASSERT_FALSE(has_value);
    }
}

GTEST_TEST(optional, constructor_in_place_of_non_trivially_constructible_type)
{

    using type = hud_test::non_bitwise_type;
    static_assert(!hud::is_trivially_constructible_v<type>);

    auto test = []()
    {
        i32 destructor_counter = 0;
        hud::optional<type> option(hud::in_place, 123, &destructor_counter);
        return std::tuple {
            option.has_value(),
            option.value().id() == 123,
            option.value().destructor_counter() == &destructor_counter,
            *option.value().destructor_counter(),
        };
    };

    // Non constant
    {
        const auto result = test();
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_TRUE(std::get<2>(result));
        GTEST_ASSERT_EQ(std::get<3>(result), 0);
    }

    // Constant
    {
        constexpr auto result = test();
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_TRUE(std::get<2>(result));
        GTEST_ASSERT_EQ(std::get<3>(result), 0);
    }
}

GTEST_TEST(optional, constructor_in_place_of_trivially_constructible_type)
{

    using type = i32;
    static_assert(hud::is_trivially_constructible_v<type>);

    auto test = []()
    {
        i32 destructor_counter = 42;
        hud::optional<type> option(hud::in_place, destructor_counter);
        return std::tuple {
            option.has_value(),
            option.value() == 42};
    };

    // Non constant
    {
        const auto result = test();
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
    }
}

GTEST_TEST(optional, copy_construct_bitwise_copy_constructible_same_type)
{

    using type = i32;
    static_assert(hud::is_bitwise_copy_constructible_v<type>);

    // If type is bitwise copy constructible, hud::optional<type> must be bitwise copy constructible
    static_assert(hud::is_bitwise_copy_constructible_v<hud::optional<type>>);

    auto test = [](const hud::optional<type> &copied)
    {
        hud::optional<type> option(copied);
        return std::tuple {
            option.has_value(),
            copied.has_value(),
            option.value() == copied.value(),
            option.value(),
        };
    };

    // Non constant
    {

        const auto result = test(hud::optional<type>(123));
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_TRUE(std::get<2>(result));
        GTEST_ASSERT_EQ(std::get<3>(result), 123);
    }

    // Constant
    {
        constexpr auto result = test(hud::optional<type>(123));
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_TRUE(std::get<2>(result));
        GTEST_ASSERT_EQ(std::get<3>(result), 123);
    }
}

GTEST_TEST(optional, copy_construct_non_bitwise_copy_constructible_same_type)
{

    using type = hud_test::non_bitwise_type;
    static_assert(!hud::is_bitwise_copy_constructible_v<type>);

    // If type is non bitwise copy constructible, hud::optional<type> must be non bitwise copy constructible
    static_assert(!hud::is_bitwise_copy_constructible_v<hud::optional<type>>);

    auto test = [](const hud::optional<type> &copied)
    {
        hud::optional<type> option(copied);
        return std::tuple {
            option.has_value(),
            copied.has_value(),
            option.value() == copied.value(),
            option.value().constructor_count(),
            option.value().copy_constructor_count(),
            option.value().move_constructor_count(),
            option.value().copy_assign_count(),
            option.value().move_assign_count(),
        };
    };

    // Non constant
    {
        const auto result = test(hud::optional<type>(nullptr));
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_TRUE(std::get<2>(result));
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 1u);
        GTEST_ASSERT_EQ(std::get<5>(result), 0u);
        GTEST_ASSERT_EQ(std::get<6>(result), 0u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }

    // Constant
    {
        constexpr auto result = test(hud::optional<type>(nullptr));
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_TRUE(std::get<2>(result));
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 1u);
        GTEST_ASSERT_EQ(std::get<5>(result), 0u);
        GTEST_ASSERT_EQ(std::get<6>(result), 0u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
}

GTEST_TEST(optional, trivially_move_constructible_if_type_is_trivially_move_constructible)
{

    using type = i32;

    GTEST_ASSERT_TRUE(hud::is_trivially_move_constructible_v<type>);
    GTEST_ASSERT_TRUE(hud::is_trivially_move_constructible_v<hud::optional<type>>);
}

GTEST_TEST(optional, not_trivially_move_constructible_if_type_is_not_trivially_move_constructible)
{

    using type = hud_test::non_bitwise_type;

    GTEST_ASSERT_FALSE(hud::is_trivially_move_constructible_v<type>);
    GTEST_ASSERT_FALSE(hud::is_trivially_move_constructible_v<hud::optional<type>>);
}

GTEST_TEST(optional, move_construct_bitwise_copy_constructible_same_type)
{

    using type = i32;
    static_assert(hud::is_bitwise_copy_constructible_v<type>);
    static_assert(hud::is_bitwise_move_constructible_v<type>);

    // If type is bitwise copy constructible, hud::optional<type> must be bitwise copy constructible
    static_assert(hud::is_bitwise_copy_constructible_v<hud::optional<type>>);
    static_assert(hud::is_bitwise_move_constructible_v<hud::optional<type>>);

    auto test = [](hud::optional<type> &&copied)
    {
        hud::optional<type> option(hud::move(copied));
        return std::tuple {
            option.has_value(),
            copied.has_value(),
            option.value() == copied.value(),
            option.value(),
        };
    };

    // Non constant
    {

        const auto result = test(hud::optional<type>(123));
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_TRUE(std::get<2>(result));
        GTEST_ASSERT_EQ(std::get<3>(result), 123);
    }

    // Constant
    {
        constexpr auto result = test(hud::optional<type>(123));
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_TRUE(std::get<2>(result));
        GTEST_ASSERT_EQ(std::get<3>(result), 123);
    }
}

GTEST_TEST(optional, move_construct_non_bitwise_copy_constructible_same_type)
{

    using type = hud_test::NonBitwiseCopyConstructibleType;

    // Ensure the type is copyable but not moveable
    static_assert(!hud::is_bitwise_move_constructible_v<type>);
    static_assert(hud::is_copy_constructible_v<type>);
    static_assert(!hud::is_move_constructible_v<type>);

    // If type is copy constructible but not moveable,
    // hud::optional<type> is still move constructible but call the copy constructor of type
    static_assert(!hud::is_bitwise_copy_constructible_v<hud::optional<type>>);
    static_assert(hud::is_move_constructible_v<hud::optional<type>>);
    static_assert(hud::is_copy_constructible_v<hud::optional<type>>);

    auto test = [](hud::optional<type> &&copied)
    {
        hud::optional<type> option(hud::move(copied));
        return std::tuple {
            option.has_value(),
            copied.has_value(),
            option.value().id() == copied.value().id(),
            option.value().copy_constructor_count(),
            option.value().id()};
    };

    // Non constant
    {
        const auto result = test(hud::optional<type>(123));
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_TRUE(std::get<2>(result));
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 123);
    }

    // Constant
    // MSVC call copy constructor instead of move constructor
    // https://developercommunity.visualstudio.com/t/constexpr-stdconstruct-at-do-not-works/1545985
#if !defined(HD_COMPILER_MSVC)
    constexpr auto result = test(hud::optional<type>(123));
    GTEST_ASSERT_TRUE(std::get<0>(result));
    GTEST_ASSERT_TRUE(std::get<1>(result));
    GTEST_ASSERT_TRUE(std::get<2>(result));
    GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    GTEST_ASSERT_EQ(std::get<4>(result), 123);
#endif
}

GTEST_TEST(optional, move_construct_bitwise_move_constructible_same_type)
{

    using type = i32;
    static_assert(hud::is_bitwise_move_constructible_v<type>);

    // If type is bitwise move constructible, hud::optional<type> must be bitwise move constructible
    static_assert(hud::is_bitwise_move_constructible_v<hud::optional<type>>);

    auto test = [](hud::optional<type> &&copied)
    {
        hud::optional<type> option(hud::move(copied));
        return std::tuple {
            option.has_value(),
            copied.has_value(),
            option.value() == copied.value(),
            option.value(),
        };
    };

    // Non constant
    {

        const auto result = test(hud::optional<type>(123));
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_TRUE(std::get<2>(result));
        GTEST_ASSERT_EQ(std::get<3>(result), 123);
    }

    // Constant
    {
        constexpr auto result = test(hud::optional<type>(123));
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_TRUE(std::get<2>(result));
        GTEST_ASSERT_EQ(std::get<3>(result), 123);
    }
}

GTEST_TEST(optional, move_construct_non_bitwise_move_constructible_same_type)
{

    using type = hud_test::non_bitwise_type;
    static_assert(!hud::is_bitwise_move_constructible_v<type>);

    // If type is non bitwise copy constructible, hud::optional<type> must be non bitwise copy constructible
    static_assert(!hud::is_bitwise_move_constructible_v<hud::optional<type>>);

    auto test = [](hud::optional<type> &&copied)
    {
        hud::optional<type> option(hud::move(copied));
        return std::tuple {
            option.has_value(),
            copied.has_value(),
            option.value().id() == copied.value().id(),
            option.value().constructor_count(),
            option.value().copy_constructor_count(),
            option.value().move_constructor_count(),
            option.value().copy_assign_count(),
            option.value().move_assign_count()};
    };

    // Non constant
    {
        const auto result = test(hud::optional<type>(hud::in_place, 123, nullptr));
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_TRUE(std::get<2>(result));
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
        GTEST_ASSERT_EQ(std::get<5>(result), 1u);
        GTEST_ASSERT_EQ(std::get<6>(result), 0u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }

    // Constant
    {
        constexpr auto result = test(hud::optional<type>(hud::in_place, 123, nullptr));
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_TRUE(std::get<2>(result));
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
        GTEST_ASSERT_EQ(std::get<5>(result), 1u);
        GTEST_ASSERT_EQ(std::get<6>(result), 0u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    }
}

GTEST_TEST(optional, param_constructor_bitwise_copy_constructible_same_type_by_copy)
{

    using type = i32;
    static_assert(hud::is_bitwise_copy_constructible_v<type>);
    static_assert(hud::is_copy_constructible_v<hud::optional<type>, type>);
    static_assert(!hud::is_bitwise_copy_constructible_v<hud::optional<type>, type>);

    auto test = [](i32 value)
    {
        hud::optional<type> option(value);
        return std::tuple {
            option.has_value(),
            option.value() == value};
    };

    // Non constant
    {
        const auto result = test(123);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
    }

    // Constant
    {
        constexpr auto result = test(123);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
    }
}

GTEST_TEST(optional, param_constructor_non_bitwise_copy_constructible_same_type_by_copy)
{

    using type = hud_test::non_bitwise_type;
    static_assert(!hud::is_bitwise_copy_constructible_v<type>);
    static_assert(hud::is_copy_constructible_v<hud::optional<type>, type>);
    static_assert(!hud::is_bitwise_copy_constructible_v<hud::optional<type>, type>);

    auto test = [](const type &value)
    {
        hud::optional<type> option(value);
        return std::tuple {
            option.has_value(),
            option.value().id() == value.id(),
            option.value().constructor_count(),
            option.value().copy_constructor_count(),
            option.value().move_constructor_count(),
            option.value().copy_assign_count(),
            option.value().move_assign_count()

        };
    };

    // Non constant
    {
        const auto result = test(type(123, nullptr));
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_EQ(std::get<2>(result), 1u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
        GTEST_ASSERT_EQ(std::get<5>(result), 0u);
        GTEST_ASSERT_EQ(std::get<6>(result), 0u);
    }

    // Constant
    {
        constexpr auto result = test(type(123, nullptr));
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_EQ(std::get<2>(result), 1u);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
        GTEST_ASSERT_EQ(std::get<5>(result), 0u);
        GTEST_ASSERT_EQ(std::get<6>(result), 0u);
    }
}

GTEST_TEST(optional, param_constructor_bitwise_move_constructible_same_type_by_move)
{

    using type = i32;
    static_assert(hud::is_bitwise_move_constructible_v<type>);
    static_assert(hud::is_move_constructible_v<hud::optional<type>, type>);
    static_assert(!hud::is_bitwise_move_constructible_v<hud::optional<type>, type>);

    auto test = [](i32 value)
    {
        hud::optional<type> option(hud::move(value));
        return std::tuple {
            option.has_value(),
            option.value() == value};
    };

    // Non constant
    {
        const auto result = test(123);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
    }

    // Constant
    {
        constexpr auto result = test(123);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
    }
}

GTEST_TEST(optional, param_constructor_non_bitwise_move_constructible_same_type_by_move)
{

    using type = hud_test::non_bitwise_type;
    static_assert(!hud::is_bitwise_move_constructible_v<type>);
    static_assert(hud::is_move_constructible_v<hud::optional<type>, type>);
    static_assert(!hud::is_bitwise_move_constructible_v<hud::optional<type>, type>);

    auto test = [](type &&value)
    {
        hud::optional<type> option(hud::move(value));
        return std::tuple {
            option.has_value(),
            option.value().id() == value.id(),
            option.value().constructor_count(),
            option.value().copy_constructor_count(),
            option.value().move_constructor_count(),
            option.value().copy_assign_count(),
            option.value().move_assign_count()

        };
    };

    // Non constant
    {
        const auto result = test(type(123, nullptr));
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_EQ(std::get<2>(result), 1u);
        GTEST_ASSERT_EQ(std::get<3>(result), 0u);
        GTEST_ASSERT_EQ(std::get<4>(result), 1u);
        GTEST_ASSERT_EQ(std::get<5>(result), 0u);
        GTEST_ASSERT_EQ(std::get<6>(result), 0u);
    }

    // Constant
    {
        constexpr auto result = test(type(123, nullptr));
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_EQ(std::get<2>(result), 1u);
        GTEST_ASSERT_EQ(std::get<3>(result), 0u);
        GTEST_ASSERT_EQ(std::get<4>(result), 1u);
        GTEST_ASSERT_EQ(std::get<5>(result), 0u);
        GTEST_ASSERT_EQ(std::get<6>(result), 0u);
    }
}
