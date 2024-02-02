#include <core/containers/optional.h>
#include <core/traits/is_trivially_copy_constructible.h>

GTEST_TEST(optional, emplace_move_inplace)
{

    using type = hud_test::non_bitwise_type;

    static_assert(!hud::is_trivially_copy_constructible_v<type>);
    static_assert(!hud::is_trivially_move_constructible_v<type>);
    static_assert(hud::is_constructible_v<type, i32, i32 *>);

    const auto test = [](const i32 value)
    {
        i32 destructor_count;
        hud::optional<type> option;

        const bool has_value_before = option.has_value();

        option.emplace(value, &destructor_count);

        return std::tuple {
            has_value_before,
            option.has_value(),
            option.value().id(),
            option.value().constructor_count(),
            option.value().move_assign_count(),
            option.value().copy_assign_count(),
            option.value().move_constructor_count(),
            option.value().copy_constructor_count(),
            destructor_count};
    };

    // Non constant
    {
        const auto result = test(123);
        hud_assert_false(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_eq(std::get<2>(result), 123);
        hud_assert_eq(std::get<3>(result), 1u);
        hud_assert_eq(std::get<4>(result), 0u);
        hud_assert_eq(std::get<5>(result), 0u);
        hud_assert_eq(std::get<6>(result), 0u);
        hud_assert_eq(std::get<7>(result), 0u);
        hud_assert_eq(std::get<8>(result), 0);
    }

    // Constant
    {
        constexpr auto result = test(123);
        hud_assert_false(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_eq(std::get<2>(result), 123);
        hud_assert_eq(std::get<3>(result), 1u);
        hud_assert_eq(std::get<4>(result), 0u);
        hud_assert_eq(std::get<5>(result), 0u);
        hud_assert_eq(std::get<6>(result), 0u);
        hud_assert_eq(std::get<7>(result), 0u);
        hud_assert_eq(std::get<8>(result), 0);
    }
}

GTEST_TEST(optional, emplace_call_destructor)
{

    using type = hud_test::non_bitwise_type;

    static_assert(!hud::is_trivially_copy_constructible_v<type>);
    static_assert(!hud::is_trivially_move_constructible_v<type>);
    static_assert(hud::is_constructible_v<type, i32, i32 *>);

    const auto test = [](const i32 value)
    {
        i32 destructor_count {0};
        i32 other_destructor_count {0};
        hud::optional<type> option {hud::in_place, value, &destructor_count};

        option.emplace(value, &other_destructor_count);

        return std::tuple {destructor_count, other_destructor_count};
    };

    // Non constant
    {
        const auto result = test(123);
        hud_assert_eq(std::get<0>(result), 1);
        hud_assert_eq(std::get<1>(result), 0);
    }

    // Constant
    {
        constexpr auto result = test(123);
        hud_assert_eq(std::get<0>(result), 1);
        hud_assert_eq(std::get<1>(result), 0);
    }
}

GTEST_TEST(optional, emplace_by_copy_in_empty_trivially_copy_constructible)
{

    using type = i32;

    static_assert(hud::is_trivially_copy_constructible_v<type>);

    const auto test = [](const type value)
    {
        hud::optional<type> option;

        const bool has_value_before = option.has_value();

        option.emplace(value);

        return std::tuple {
            has_value_before,
            option.has_value(),
            option.value()};
    };

    // Non constant
    {
        const auto result = test(123);
        hud_assert_false(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_eq(std::get<2>(result), 123);
    }

    // Constant
    {
        constexpr auto result = test(123);
        hud_assert_false(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_eq(std::get<2>(result), 123);
    }
}

GTEST_TEST(optional, emplace_by_copy_in_non_empty_trivially_copy_constructible)
{

    using type = i32;

    static_assert(hud::is_trivially_copy_constructible_v<type>);

    const auto test = [](const type before, const type after)
    {
        hud::optional<type> option {before};

        const bool has_value_before = option.has_value();
        const type value_before = option.value();

        option.emplace(after);

        return std::tuple {
            has_value_before,
            value_before,
            option.has_value(),
            option.value()};
    };

    // Non constant
    {
        const auto result = test(123, 456);
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 123);
        hud_assert_true(std::get<2>(result));
        hud_assert_eq(std::get<3>(result), 456);
    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 123);
        hud_assert_true(std::get<2>(result));
        hud_assert_eq(std::get<3>(result), 456);
    }
}

GTEST_TEST(optional, emplace_by_copy_in_empty_non_trivially_copy_constructible)
{

    using type = hud_test::NonBitwiseCopyConstructibleType;

    static_assert(!hud::is_trivially_copy_constructible_v<type>);

    const auto test = [](const i32 value)
    {
        hud::optional<type> option;

        const bool has_value_before = option.has_value();

        const type other {value};
        option.emplace(other);

        return std::tuple {
            has_value_before,
            option.has_value(),
            option.value().id(),
            option.value().copy_constructor_count(),
        };
    };

    // Non constant
    {
        const auto result = test(123);
        hud_assert_false(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_eq(std::get<2>(result), 123);
        hud_assert_eq(std::get<3>(result), 1u);
    }

    // Constant
    {
        constexpr auto result = test(123);
        hud_assert_false(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_eq(std::get<2>(result), 123);
        hud_assert_eq(std::get<3>(result), 1u);
    }
}

GTEST_TEST(optional, emplace_by_copy_in_non_empty_non_trivially_copy_constructible)
{

    using type = hud_test::NonBitwiseCopyConstructibleType;

    static_assert(!hud::is_trivially_copy_constructible_v<type>);

    const auto test = [](const i32 before, const i32 after)
    {
        hud::optional<type> option {hud::in_place, before};

        const bool has_value_before = option.has_value();
        const i32 value_before = option.value().id();

        const type other {after};
        option.emplace(other);

        return std::tuple {
            has_value_before,
            value_before,
            option.has_value(),
            option.value().id()};
    };

    // Non constant
    {
        const auto result = test(123, 456);
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 123);
        hud_assert_true(std::get<2>(result));
        hud_assert_eq(std::get<3>(result), 456);
    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 123);
        hud_assert_true(std::get<2>(result));
        hud_assert_eq(std::get<3>(result), 456);
    }
}

GTEST_TEST(optional, emplace_by_copy_in_empty_trivially_move_constructible)
{

    using type = i32;

    static_assert(hud::is_trivially_move_constructible_v<type>);

    const auto test = [](const type value)
    {
        hud::optional<type> option;

        const bool has_value_before = option.has_value();

        option.emplace(value);

        return std::tuple {
            has_value_before,
            option.has_value(),
            option.value()};
    };

    // Non constant
    {
        const auto result = test(123);
        hud_assert_false(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_eq(std::get<2>(result), 123);
    }

    // Constant
    {
        constexpr auto result = test(123);
        hud_assert_false(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_eq(std::get<2>(result), 123);
    }
}

GTEST_TEST(optional, emplace_by_copy_in_non_empty_trivially_move_constructible)
{

    using type = i32;

    static_assert(hud::is_trivially_move_constructible_v<type>);

    const auto test = [](const type before, const type after)
    {
        hud::optional<type> option {before};

        const bool has_value_before = option.has_value();
        const type value_before = option.value();

        option.emplace(after);

        return std::tuple {
            has_value_before,
            value_before,
            option.has_value(),
            option.value()};
    };

    // Non constant
    {
        const auto result = test(123, 456);
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 123);
        hud_assert_true(std::get<2>(result));
        hud_assert_eq(std::get<3>(result), 456);
    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 123);
        hud_assert_true(std::get<2>(result));
        hud_assert_eq(std::get<3>(result), 456);
    }
}

GTEST_TEST(optional, emplace_by_copy_in_empty_non_trivially_move_constructible)
{

    using type = hud_test::NonBitwiseMoveConstructibleType;

    static_assert(!hud::is_trivially_move_constructible_v<type>);

    const auto test = [](const i32 value)
    {
        hud::optional<type> option;

        const bool has_value_before = option.has_value();

        const type other {value};
        option.emplace(other);

        return std::tuple {
            has_value_before,
            option.has_value(),
            option.value().id(),
            option.value().copy_constructor_count(),
            option.value().move_constructor_count(),
        };
    };

    // Non constant
    {
        const auto result = test(123);
        hud_assert_false(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_eq(std::get<2>(result), 123);
        hud_assert_eq(std::get<3>(result), 1u);
        hud_assert_eq(std::get<4>(result), 0u);
    }

    // Constant
    {
        constexpr auto result = test(123);
        hud_assert_false(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_eq(std::get<2>(result), 123);
        hud_assert_eq(std::get<3>(result), 1u);
        hud_assert_eq(std::get<4>(result), 0u);
    }
}

GTEST_TEST(optional, emplace_by_copy_in_non_empty_non_trivially_move_constructible)
{

    using type = hud_test::NonBitwiseMoveConstructibleType;

    static_assert(!hud::is_trivially_move_constructible_v<type>);

    const auto test = [](const i32 before, const i32 after)
    {
        hud::optional<type> option {hud::in_place, before};

        const bool has_value_before = option.has_value();
        const i32 value_before = option.value().id();
        const u32 copy_ctor_count_before = option.value().copy_constructor_count();
        const u32 move_ctor_count_before = option.value().move_constructor_count();

        const type other {after};
        option.emplace(other);

        return std::tuple {
            has_value_before,
            value_before,
            copy_ctor_count_before,
            move_ctor_count_before,
            option.has_value(),
            option.value().id(),
            option.value().copy_constructor_count(),
            option.value().move_constructor_count()};
    };

    // Non constant
    {
        const auto result = test(123, 456);
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 123);
        hud_assert_eq(std::get<2>(result), 0u);
        hud_assert_eq(std::get<3>(result), 0u);
        hud_assert_true(std::get<4>(result));
        hud_assert_eq(std::get<5>(result), 456);
        hud_assert_eq(std::get<6>(result), 1u);
        hud_assert_eq(std::get<7>(result), 0u);
    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 123);
        hud_assert_eq(std::get<2>(result), 0u);
        hud_assert_eq(std::get<3>(result), 0u);
        hud_assert_true(std::get<4>(result));
        hud_assert_eq(std::get<5>(result), 456);
        hud_assert_eq(std::get<6>(result), 1u);
        hud_assert_eq(std::get<7>(result), 0u);
    }
}

GTEST_TEST(optional, emplace_by_move_in_empty_trivially_copy_constructible)
{

    using type = i32;

    static_assert(hud::is_trivially_copy_constructible_v<type>);

    const auto test = [](type value)
    {
        hud::optional<type> option;

        const bool has_value_before = option.has_value();

        option.emplace(hud::move(value));

        return std::tuple {
            has_value_before,
            option.has_value(),
            option.value()};
    };

    // Non constant
    {
        const auto result = test(123);
        hud_assert_false(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_eq(std::get<2>(result), 123);
    }

    // Constant
    {
        constexpr auto result = test(123);
        hud_assert_false(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_eq(std::get<2>(result), 123);
    }
}

GTEST_TEST(optional, emplace_by_move_in_non_empty_trivially_copy_constructible)
{

    using type = i32;

    static_assert(hud::is_trivially_copy_constructible_v<type>);

    const auto test = [](const type before, type after)
    {
        hud::optional<type> option {before};

        const bool has_value_before = option.has_value();
        const type value_before = option.value();

        option.emplace(hud::move(after));

        return std::tuple {
            has_value_before,
            value_before,
            option.has_value(),
            option.value()};
    };

    // Non constant
    {
        const auto result = test(123, 456);
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 123);
        hud_assert_true(std::get<2>(result));
        hud_assert_eq(std::get<3>(result), 456);
    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 123);
        hud_assert_true(std::get<2>(result));
        hud_assert_eq(std::get<3>(result), 456);
    }
}

GTEST_TEST(optional, emplace_by_move_in_empty_non_trivially_copy_constructible)
{

    using type = hud_test::NonBitwiseCopyConstructibleType;

    static_assert(!hud::is_trivially_copy_constructible_v<type>);
    const auto test = [](const i32 value)
    {
        hud::optional<type> option;

        const bool has_value_before = option.has_value();

        type other {value};
        option.emplace(hud::move(other));

        return std::tuple {
            has_value_before,
            option.has_value(),
            option.value().id(),
            option.value().copy_constructor_count(),
        };
    };

    // Non constant
    {
        const auto result = test(123);
        hud_assert_false(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_eq(std::get<2>(result), 123);
        hud_assert_eq(std::get<3>(result), 1u);
    }

    // Constant
    {
        constexpr auto result = test(123);
        hud_assert_false(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_eq(std::get<2>(result), 123);
        hud_assert_eq(std::get<3>(result), 1u);
    }
}

GTEST_TEST(optional, emplace_by_move_in_non_empty_non_trivially_copy_constructible)
{

    using type = hud_test::NonBitwiseCopyConstructibleType;

    static_assert(!hud::is_trivially_copy_constructible_v<type>);

    const auto test = [](const i32 before, const i32 after)
    {
        hud::optional<type> option {hud::in_place, before};

        const bool has_value_before = option.has_value();
        const i32 value_before = option.value().id();
        const u32 ctor_before = option.value().copy_constructor_count();

        type other {after};
        option.emplace(hud::move(other));

        return std::tuple {
            has_value_before,
            value_before,
            ctor_before,
            option.has_value(),
            option.value().id(),
            option.value().copy_constructor_count()};
    };

    // Non constant
    {
        const auto result = test(123, 456);
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 123);
        hud_assert_eq(std::get<2>(result), 0u);
        hud_assert_true(std::get<3>(result));
        hud_assert_eq(std::get<4>(result), 456);
        hud_assert_eq(std::get<5>(result), 1u);
    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 123);
        hud_assert_eq(std::get<2>(result), 0u);
        hud_assert_true(std::get<3>(result));
        hud_assert_eq(std::get<4>(result), 456);
        hud_assert_eq(std::get<5>(result), 1u);
    }
}

GTEST_TEST(optional, emplace_by_move_in_empty_trivially_move_constructible)
{

    using type = i32;

    static_assert(hud::is_trivially_move_constructible_v<type>);

    const auto test = [](const type value)
    {
        hud::optional<type> option;

        const bool has_value_before = option.has_value();

        option.emplace(hud::move(value));

        return std::tuple {
            has_value_before,
            option.has_value(),
            option.value()};
    };

    // Non constant
    {
        const auto result = test(123);
        hud_assert_false(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_eq(std::get<2>(result), 123);
    }

    // Constant
    {
        constexpr auto result = test(123);
        hud_assert_false(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_eq(std::get<2>(result), 123);
    }
}

GTEST_TEST(optional, emplace_by_move_in_non_empty_trivially_move_constructible)
{

    using type = i32;

    static_assert(hud::is_trivially_move_constructible_v<type>);

    const auto test = [](const type before, type after)
    {
        hud::optional<type> option {before};

        const bool has_value_before = option.has_value();
        const type value_before = option.value();

        option.emplace(hud::move(after));

        return std::tuple {
            has_value_before,
            value_before,
            option.has_value(),
            option.value()};
    };

    // Non constant
    {
        const auto result = test(123, 456);
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 123);
        hud_assert_true(std::get<2>(result));
        hud_assert_eq(std::get<3>(result), 456);
    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 123);
        hud_assert_true(std::get<2>(result));
        hud_assert_eq(std::get<3>(result), 456);
    }
}

GTEST_TEST(optional, emplace_by_move_in_empty_non_trivially_move_constructible)
{

    using type = hud_test::NonBitwiseMoveConstructibleType;

    static_assert(!hud::is_trivially_move_constructible_v<type>);

    const auto test = [](const i32 value)
    {
        hud::optional<type> option;

        const bool has_value_before = option.has_value();

        type other {value};
        option.emplace(hud::move(other));

        return std::tuple {
            has_value_before,
            option.has_value(),
            option.value().id(),
            option.value().copy_constructor_count(),
            option.value().move_constructor_count(),
        };
    };

    // Non constant
    {
        const auto result = test(123);
        hud_assert_false(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_eq(std::get<2>(result), 123);
        hud_assert_eq(std::get<3>(result), 0u);
        hud_assert_eq(std::get<4>(result), 1u);
    }

    // Constant
    {
        constexpr auto result = test(123);
        hud_assert_false(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_eq(std::get<2>(result), 123);
        hud_assert_eq(std::get<3>(result), 0u);
        hud_assert_eq(std::get<4>(result), 1u);
    }
}

GTEST_TEST(optional, emplace_by_move_in_non_empty_non_trivially_move_constructible)
{

    using type = hud_test::NonBitwiseMoveConstructibleType;

    static_assert(!hud::is_trivially_move_constructible_v<type>);

    const auto test = [](const i32 before, const i32 after)
    {
        hud::optional<type> option {hud::in_place, before};

        const bool has_value_before = option.has_value();
        const i32 value_before = option.value().id();
        const u32 copy_ctor_count_before = option.value().copy_constructor_count();
        const u32 move_ctor_count_before = option.value().move_constructor_count();

        type other {after};
        option.emplace(hud::move(other));

        return std::tuple {
            has_value_before,
            value_before,
            copy_ctor_count_before,
            move_ctor_count_before,
            option.has_value(),
            option.value().id(),
            option.value().copy_constructor_count(),
            option.value().move_constructor_count()};
    };

    // Non constant
    {
        const auto result = test(123, 456);
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 123);
        hud_assert_eq(std::get<2>(result), 0u);
        hud_assert_eq(std::get<3>(result), 0u);
        hud_assert_true(std::get<4>(result));
        hud_assert_eq(std::get<5>(result), 456);
        hud_assert_eq(std::get<6>(result), 0u);
        hud_assert_eq(std::get<7>(result), 1u);
    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 123);
        hud_assert_eq(std::get<2>(result), 0u);
        hud_assert_eq(std::get<3>(result), 0u);
        hud_assert_true(std::get<4>(result));
        hud_assert_eq(std::get<5>(result), 456);
        hud_assert_eq(std::get<6>(result), 0u);
        hud_assert_eq(std::get<7>(result), 1u);
    }
}