#include <core/containers/pair.h>
#include <core/traits/is_same.h>

GTEST_TEST(pair, inner_is_correct)
{
    // Non constant
    {
        using pair_type = hud::pair<i32, u32>;

        bool is_first_same = hud::is_same_v<pair_type::first_type, i32>;
        hud_assert_true(is_first_same);

        bool is_second_same = hud::is_same_v<pair_type::second_type, u32>;
        hud_assert_true(is_second_same);
    }

    // Constant
    {
        using pair_type = std::pair<i32, u32>;

        bool is_first_same = hud::is_same_v<pair_type::first_type, i32>;
        hud_assert_true(is_first_same);

        bool is_second_same = hud::is_same_v<pair_type::second_type, u32>;
        hud_assert_true(is_second_same);
    }
}

GTEST_TEST(pair, sizeof_pair_is_sizeof_inner_types)
{

    using pair_type = hud::pair<i32, u64>;

    struct shoud_be
    {
        i32 f;
        u64 s;
    };

    hud_assert_eq(sizeof(pair_type), sizeof(shoud_be));
}

GTEST_TEST(pair, make_pair_create_pair_trivially_constructible)
{

    using type = i32;
    static_assert(hud::is_trivially_constructible_v<type, type &&>);

    const auto test = [](type &&t1, type &&t2)
    {
        const auto pair = hud::make_pair(t1, t2);
        return std::tuple {
            pair.first,
            pair.second
        };
    };

    // Non constant
    {
        const auto result = test(123, 456);

        hud_assert_eq(std::get<0>(result), 123);
        hud_assert_eq(std::get<1>(result), 456);
    }

    // Constant
    {
        constexpr auto result = test(123, 456);

        hud_assert_eq(std::get<0>(result), 123);
        hud_assert_eq(std::get<1>(result), 456);
    }
}

GTEST_TEST(pair, make_pair_create_pair_non_trivial)
{
    using type = hud_test::non_bitwise_type;
    static_assert(hud::is_constructible_v<type, type &&>);
    static_assert(!hud::is_trivially_constructible_v<type, type &&>);

    const auto test = [](type &&t1, type &&t2)
    {
        const auto pair = hud::make_pair(hud::move(t1), hud::move(t2));
        return std::tuple {
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
        const auto result = test(type {123, nullptr}, type {456, nullptr});

        hud_assert_eq(std::get<0>(result), 123);
        hud_assert_eq(std::get<1>(result), 0u);
        hud_assert_eq(std::get<2>(result), 0u);
        hud_assert_eq(std::get<3>(result), 1u);
        hud_assert_eq(std::get<4>(result), 1u);
        hud_assert_eq(std::get<5>(result), 0u);
        hud_assert_eq(std::get<6>(result), 456);
        hud_assert_eq(std::get<7>(result), 0u);
        hud_assert_eq(std::get<8>(result), 0u);
        hud_assert_eq(std::get<9>(result), 1u);
        hud_assert_eq(std::get<10>(result), 1u);
        hud_assert_eq(std::get<11>(result), 0u);
    }

    // Constant
    {
        constexpr auto result = test(type {123, nullptr}, type {456, nullptr});

        hud_assert_eq(std::get<0>(result), 123);
        hud_assert_eq(std::get<1>(result), 0u);
        hud_assert_eq(std::get<2>(result), 0u);
        hud_assert_eq(std::get<3>(result), 1u);
        hud_assert_eq(std::get<4>(result), 1u);
        hud_assert_eq(std::get<5>(result), 0u);
        hud_assert_eq(std::get<6>(result), 456);
        hud_assert_eq(std::get<7>(result), 0u);
        hud_assert_eq(std::get<8>(result), 0u);
        hud_assert_eq(std::get<9>(result), 1u);
        hud_assert_eq(std::get<10>(result), 1u);
        hud_assert_eq(std::get<11>(result), 0u);
    }
}

GTEST_TEST(pair, get_is_usable_with_pair)
{

    const auto test = [](i32 t1, i32 t2)
    {
        const auto pair = hud::make_pair(t1, t2);
        return std::tuple {
            hud::get<0>(pair),
            hud::get<1>(pair)
        };
    };

    // Non constant
    {
        const auto result = test(123, 456);
        hud_assert_eq(std::get<0>(result), 123);
        hud_assert_eq(std::get<1>(result), 456);
    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        hud_assert_eq(std::get<0>(result), 123);
        hud_assert_eq(std::get<1>(result), 456);
    }
}

GTEST_TEST(pair, tuple_size)
{

    constexpr usize tuple_size = hud::tuple_size_v<hud::pair<u32, u64>>;
    hud_assert_eq(tuple_size, 2u);
}

GTEST_TEST(pair, tuple_element)
{

    constexpr bool is_tuple_element_0_same = hud::is_same_v<hud::tuple_element_t<0, hud::pair<u32, u64>>, u32>;
    constexpr bool is_tuple_element_1_same = hud::is_same_v<hud::tuple_element_t<1, hud::pair<u32, u64>>, u64>;
    hud_assert_true(is_tuple_element_0_same);
    hud_assert_true(is_tuple_element_1_same);
}

GTEST_TEST(pair, structure_binding_by_copy)
{
    using type = hud_test::non_bitwise_type;
    static_assert(hud::is_constructible_v<type, type &&>);
    static_assert(!hud::is_trivially_constructible_v<type, type &&>);

    const auto test = [](type &&t1, type &&t2)
    {
        auto pair = hud::make_pair(hud::move(t1), hud::move(t2));
        auto [first_cpy, second_cpy] = pair;
        first_cpy = type {321, nullptr};
        second_cpy = type {654, nullptr};
        return std::tuple {
            first_cpy.constructor_count(),
            first_cpy.move_constructor_count(),
            first_cpy.copy_constructor_count(),
            first_cpy.move_assign_count(),
            first_cpy.copy_assign_count(),
            second_cpy.constructor_count(),
            second_cpy.move_constructor_count(),
            second_cpy.copy_constructor_count(),
            second_cpy.move_assign_count(),
            second_cpy.copy_assign_count(),
            hud::get<0>(pair).id() == 123,
            hud::get<1>(pair).id() == 456
        };
    };
    // Non constant
    {
        const auto result = test(type {123, nullptr}, type {456, nullptr});
        hud_assert_eq(std::get<0>(result), 1);
        hud_assert_eq(std::get<1>(result), 0);
        hud_assert_eq(std::get<2>(result), 0);
        hud_assert_eq(std::get<3>(result), 1);
        hud_assert_eq(std::get<4>(result), 0);
        hud_assert_eq(std::get<5>(result), 1);
        hud_assert_eq(std::get<6>(result), 0);
        hud_assert_eq(std::get<7>(result), 0);
        hud_assert_eq(std::get<8>(result), 1);
        hud_assert_eq(std::get<9>(result), 0);
        hud_assert_true(std::get<10>(result));
        hud_assert_true(std::get<11>(result));
    }

    // Non constant
    {
        constexpr auto result = test(type {123, nullptr}, type {456, nullptr});
        hud_assert_eq(std::get<0>(result), 1);
        hud_assert_eq(std::get<1>(result), 0);
        hud_assert_eq(std::get<2>(result), 0);
        hud_assert_eq(std::get<3>(result), 1);
        hud_assert_eq(std::get<4>(result), 0);
        hud_assert_eq(std::get<5>(result), 1);
        hud_assert_eq(std::get<6>(result), 0);
        hud_assert_eq(std::get<7>(result), 0);
        hud_assert_eq(std::get<8>(result), 1);
        hud_assert_eq(std::get<9>(result), 0);
        hud_assert_true(std::get<10>(result));
        hud_assert_true(std::get<11>(result));
    }
}

GTEST_TEST(pair, structure_binding_rvalue)
{
    using type = hud_test::non_bitwise_type;
    static_assert(hud::is_constructible_v<type, type &&>);
    static_assert(!hud::is_trivially_constructible_v<type, type &&>);

    const auto test = [](type &&t1, type &&t2)
    {
        auto pair = hud::make_pair(hud::move(t1), hud::move(t2));
        auto &[first_ref, second_ref] = pair;
        first_ref = type {951, nullptr};
        second_ref = type {357, nullptr};
        return std::tuple {
            first_ref.constructor_count(),
            first_ref.move_constructor_count(),
            first_ref.copy_constructor_count(),
            first_ref.move_assign_count(),
            first_ref.copy_assign_count(),
            second_ref.constructor_count(),
            second_ref.move_constructor_count(),
            second_ref.copy_constructor_count(),
            second_ref.move_assign_count(),
            second_ref.copy_assign_count(),
            hud::get<0>(pair).id() == 951,
            hud::get<1>(pair).id() == 357
        };
    };
    // Non constant
    {
        const auto result = test(type {123, nullptr}, type {456, nullptr});
        hud_assert_eq(std::get<0>(result), 1);
        hud_assert_eq(std::get<1>(result), 0);
        hud_assert_eq(std::get<2>(result), 0);
        hud_assert_eq(std::get<3>(result), 1);
        hud_assert_eq(std::get<4>(result), 0);
        hud_assert_eq(std::get<5>(result), 1);
        hud_assert_eq(std::get<6>(result), 0);
        hud_assert_eq(std::get<7>(result), 0);
        hud_assert_eq(std::get<8>(result), 1);
        hud_assert_eq(std::get<9>(result), 0);
        hud_assert_true(std::get<10>(result));
        hud_assert_true(std::get<11>(result));
    }

    // Non constant
    {
        constexpr auto result = test(type {123, nullptr}, type {456, nullptr});
        hud_assert_eq(std::get<0>(result), 1);
        hud_assert_eq(std::get<1>(result), 0);
        hud_assert_eq(std::get<2>(result), 0);
        hud_assert_eq(std::get<3>(result), 1);
        hud_assert_eq(std::get<4>(result), 0);
        hud_assert_eq(std::get<5>(result), 1);
        hud_assert_eq(std::get<6>(result), 0);
        hud_assert_eq(std::get<7>(result), 0);
        hud_assert_eq(std::get<8>(result), 1);
        hud_assert_eq(std::get<9>(result), 0);
        hud_assert_true(std::get<10>(result));
        hud_assert_true(std::get<11>(result));
    }
}

GTEST_TEST(pair, structure_binding_const_rvalue)
{
    using type = hud_test::non_bitwise_type;
    static_assert(hud::is_constructible_v<type, type &&>);
    static_assert(!hud::is_trivially_constructible_v<type, type &&>);

    const auto test = [](type &&t1, type &&t2)
    {
        auto pair = hud::make_pair(hud::move(t1), hud::move(t2));
        const auto &[first_ref, second_ref] = pair;
        return std::tuple {
            first_ref.constructor_count(),
            first_ref.move_constructor_count(),
            first_ref.copy_constructor_count(),
            first_ref.move_assign_count(),
            first_ref.copy_assign_count(),
            second_ref.constructor_count(),
            second_ref.move_constructor_count(),
            second_ref.copy_constructor_count(),
            second_ref.move_assign_count(),
            second_ref.copy_assign_count(),
        };
    };
    // Non constant
    {
        const auto result = test(type {123, nullptr}, type {456, nullptr});
        hud_assert_eq(std::get<0>(result), 1);
        hud_assert_eq(std::get<1>(result), 1);
        hud_assert_eq(std::get<2>(result), 0);
        hud_assert_eq(std::get<3>(result), 0);
        hud_assert_eq(std::get<4>(result), 0);
        hud_assert_eq(std::get<5>(result), 1);
        hud_assert_eq(std::get<6>(result), 1);
        hud_assert_eq(std::get<7>(result), 0);
        hud_assert_eq(std::get<8>(result), 0);
        hud_assert_eq(std::get<9>(result), 0);
    }

    // Non constant
    {
        constexpr auto result = test(type {123, nullptr}, type {456, nullptr});
        hud_assert_eq(std::get<0>(result), 1);
        hud_assert_eq(std::get<1>(result), 1);
        hud_assert_eq(std::get<2>(result), 0);
        hud_assert_eq(std::get<3>(result), 0);
        hud_assert_eq(std::get<4>(result), 0);
        hud_assert_eq(std::get<5>(result), 1);
        hud_assert_eq(std::get<6>(result), 1);
        hud_assert_eq(std::get<7>(result), 0);
        hud_assert_eq(std::get<8>(result), 0);
        hud_assert_eq(std::get<9>(result), 0);
    }
}

GTEST_TEST(pair, structure_binding_pair_lvalue)
{
    using type = hud_test::non_bitwise_type;
    static_assert(hud::is_constructible_v<type, type &&>);
    static_assert(!hud::is_trivially_constructible_v<type, type &&>);

    const auto test = [](type &&t1, type &&t2)
    {
        auto pair = hud::make_pair(hud::move(t1), hud::move(t2));
        auto &&[first_ref, second_ref] = pair;
        first_ref = type {951, nullptr};
        second_ref = type {357, nullptr};
        return std::tuple {
            first_ref.constructor_count(),
            first_ref.move_constructor_count(),
            first_ref.copy_constructor_count(),
            first_ref.move_assign_count(),
            first_ref.copy_assign_count(),
            second_ref.constructor_count(),
            second_ref.move_constructor_count(),
            second_ref.copy_constructor_count(),
            second_ref.move_assign_count(),
            second_ref.copy_assign_count(),
            hud::get<0>(pair).id() == 951,
            hud::get<1>(pair).id() == 357
        };
    };
    // Non constant
    {
        const auto result = test(type {123, nullptr}, type {456, nullptr});
        hud_assert_eq(std::get<0>(result), 1);
        hud_assert_eq(std::get<1>(result), 0);
        hud_assert_eq(std::get<2>(result), 0);
        hud_assert_eq(std::get<3>(result), 1);
        hud_assert_eq(std::get<4>(result), 0);
        hud_assert_eq(std::get<5>(result), 1);
        hud_assert_eq(std::get<6>(result), 0);
        hud_assert_eq(std::get<7>(result), 0);
        hud_assert_eq(std::get<8>(result), 1);
        hud_assert_eq(std::get<9>(result), 0);
        hud_assert_true(std::get<10>(result));
        hud_assert_true(std::get<11>(result));
    }

    // Non constant
    {
        constexpr auto result = test(type {123, nullptr}, type {456, nullptr});
        hud_assert_eq(std::get<0>(result), 1);
        hud_assert_eq(std::get<1>(result), 0);
        hud_assert_eq(std::get<2>(result), 0);
        hud_assert_eq(std::get<3>(result), 1);
        hud_assert_eq(std::get<4>(result), 0);
        hud_assert_eq(std::get<5>(result), 1);
        hud_assert_eq(std::get<6>(result), 0);
        hud_assert_eq(std::get<7>(result), 0);
        hud_assert_eq(std::get<8>(result), 1);
        hud_assert_eq(std::get<9>(result), 0);
        hud_assert_true(std::get<10>(result));
        hud_assert_true(std::get<11>(result));
    }
}

GTEST_TEST(pair, structure_binding_const_pair_lvalue)
{
    using type = hud_test::non_bitwise_type;
    static_assert(hud::is_constructible_v<type, type &&>);
    static_assert(!hud::is_trivially_constructible_v<type, type &&>);

    const auto test = [](type &&t1, type &&t2)
    {
        const auto pair = hud::make_pair(hud::move(t1), hud::move(t2));
        auto &&[first_ref, second_ref] = pair;
        // Not possible, pair is const
        // first_ref = type {951, nullptr};
        // second_ref = type {357, nullptr};
        return std::tuple {
            first_ref.constructor_count(),
            first_ref.move_constructor_count(),
            first_ref.copy_constructor_count(),
            first_ref.move_assign_count(),
            first_ref.copy_assign_count(),
            second_ref.constructor_count(),
            second_ref.move_constructor_count(),
            second_ref.copy_constructor_count(),
            second_ref.move_assign_count(),
            second_ref.copy_assign_count(),
        };
    };
    // Non constant
    {
        const auto result = test(type {123, nullptr}, type {456, nullptr});
        hud_assert_eq(std::get<0>(result), 1);
        hud_assert_eq(std::get<1>(result), 1);
        hud_assert_eq(std::get<2>(result), 0);
        hud_assert_eq(std::get<3>(result), 0);
        hud_assert_eq(std::get<4>(result), 0);
        hud_assert_eq(std::get<5>(result), 1);
        hud_assert_eq(std::get<6>(result), 1);
        hud_assert_eq(std::get<7>(result), 0);
        hud_assert_eq(std::get<8>(result), 0);
        hud_assert_eq(std::get<9>(result), 0);
    }

    // Non constant
    {
        constexpr auto result = test(type {123, nullptr}, type {456, nullptr});
        hud_assert_eq(std::get<0>(result), 1);
        hud_assert_eq(std::get<1>(result), 1);
        hud_assert_eq(std::get<2>(result), 0);
        hud_assert_eq(std::get<3>(result), 0);
        hud_assert_eq(std::get<4>(result), 0);
        hud_assert_eq(std::get<5>(result), 1);
        hud_assert_eq(std::get<6>(result), 1);
        hud_assert_eq(std::get<7>(result), 0);
        hud_assert_eq(std::get<8>(result), 0);
        hud_assert_eq(std::get<9>(result), 0);
    }
}