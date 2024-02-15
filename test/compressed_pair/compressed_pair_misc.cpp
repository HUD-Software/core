#include <core/containers/compressed_pair.h>

struct empty
{
};

[[nodiscard]] HD_FORCEINLINE constexpr bool operator==(const empty &, const empty &) noexcept
{
    return true;
}

struct empty2
{
};

GTEST_TEST(compressed_pair, inner_is_correct)
{
    // Non empty
    using compressed_pair_type = hud::compressed_pair<empty, u32>;

    bool is_first_same = hud::is_same_v<compressed_pair_type::first_type, empty>;
    hud_assert_true(is_first_same);

    bool is_second_same = hud::is_same_v<compressed_pair_type::second_type, u32>;
    hud_assert_true(is_second_same);
}

GTEST_TEST(compressed_pair, sizeof_pair_is_sizeof_inner_types)
{
    hud_assert_eq(sizeof(hud::compressed_pair<empty, u64>), sizeof(u64));
    hud_assert_eq(sizeof(hud::compressed_pair<u64, empty>), sizeof(u64));
    hud_assert_eq(sizeof(hud::compressed_pair<empty, empty>), sizeof(empty));
    hud_assert_eq(sizeof(hud::compressed_pair<empty, empty2>), sizeof(empty));
}

GTEST_TEST(compressed_pair, make_compressed_pair_create_compressed_pair_trivially_constructible)
{

    using type = i32;
    static_assert(hud::is_trivially_constructible_v<type, type &&>);

    const auto test = [](empty &&t1, type &&t2)
    {
        const auto pair = hud::make_compressed_pair(t1, t2);
        return std::tuple {
            pair.first(),
            pair.second()
        };
    };

    // Non constant
    {
        const auto result = test(empty {}, 456);

        hud_assert_eq(std::get<0>(result), empty {});
        hud_assert_eq(std::get<1>(result), 456);
    }

    // Constant
    {
        constexpr auto result = test(empty {}, 456);

        hud_assert_eq(std::get<0>(result), empty {});
        hud_assert_eq(std::get<1>(result), 456);
    }
}

GTEST_TEST(compressed_pair, make_compressed_pair_create_compressed_pair_non_trivial)
{

    using type = hud_test::non_bitwise_type;
    static_assert(hud::is_constructible_v<type, type &&>);
    static_assert(!hud::is_trivially_constructible_v<type, type &&>);

    const auto test = [](empty &&t1, type &&t2)
    {
        const auto pair = hud::make_compressed_pair(hud::forward<empty>(t1), hud::forward<type>(t2));
        return std::tuple {
            pair.first() == empty {},
            pair.second().id(),
            pair.second().move_assign_count(),
            pair.second().copy_assign_count(),
            pair.second().constructor_count(),
            pair.second().move_constructor_count(),
            pair.second().copy_constructor_count(),
        };
    };

    // Non constant
    {
        const auto result = test(empty {}, type {456, nullptr});

        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 456);
        hud_assert_eq(std::get<2>(result), 0u);
        hud_assert_eq(std::get<3>(result), 0u);
        hud_assert_eq(std::get<4>(result), 1u);
        hud_assert_eq(std::get<5>(result), 1u);
        hud_assert_eq(std::get<6>(result), 0u);
    }

    // Constant
    {
        constexpr auto result = test(empty {}, type {456, nullptr});

        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 456);
        hud_assert_eq(std::get<2>(result), 0u);
        hud_assert_eq(std::get<3>(result), 0u);
        hud_assert_eq(std::get<4>(result), 1u);
        hud_assert_eq(std::get<5>(result), 1u);
        hud_assert_eq(std::get<6>(result), 0u);
    }
}

GTEST_TEST(compressed_pair, get_is_usable_with_pair)
{

    const auto test = [](empty t1, i32 t2)
    {
        const auto pair = hud::make_compressed_pair(t1, t2);
        return std::tuple {
            hud::get<0>(pair),
            hud::get<1>(pair)
        };
    };

    // Non constant
    {
        const auto result = test(empty {}, 456);
        hud_assert_eq(std::get<0>(result), empty {});
        hud_assert_eq(std::get<1>(result), 456);
    }

    // Constant
    {
        constexpr auto result = test(empty {}, 456);
        hud_assert_eq(std::get<0>(result), empty {});
        hud_assert_eq(std::get<1>(result), 456);
    }
}

GTEST_TEST(compressed_pair, tuple_size)
{
    hud_assert_eq((hud::tuple_size_v<hud::compressed_pair<u32, u64>>), 2u);
    hud_assert_eq((hud::tuple_size_v<hud::compressed_pair<empty, u64>>), 2u);
    hud_assert_eq((hud::tuple_size_v<hud::compressed_pair<u32, empty>>), 2u);
    hud_assert_eq((hud::tuple_size_v<hud::compressed_pair<empty, empty>>), 2u);
}

GTEST_TEST(compressed_pair, tuple_element)
{
    hud_assert_true((hud::is_same_v<hud::tuple_element_t<0, hud::compressed_pair<u32, u64>>, u32>));
    hud_assert_true((hud::is_same_v<hud::tuple_element_t<0, hud::compressed_pair<empty, u64>>, empty>));
    hud_assert_true((hud::is_same_v<hud::tuple_element_t<0, hud::compressed_pair<empty, empty>>, empty>));
    hud_assert_true((hud::is_same_v<hud::tuple_element_t<1, hud::compressed_pair<u32, u64>>, u64>));
    hud_assert_true((hud::is_same_v<hud::tuple_element_t<1, hud::compressed_pair<u32, empty>>, empty>));
    hud_assert_true((hud::is_same_v<hud::tuple_element_t<1, hud::compressed_pair<empty, empty>>, empty>));
}