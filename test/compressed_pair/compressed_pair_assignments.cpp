#include <core/containers/compressed_pair.h>
#include <core/traits/is_trivially_copy_assignable.h>
#include <core/traits/is_copy_assignable.h>

GTEST_TEST(compressed_pair, copy_assign_trivially_copy_assignable_same_type)
{

    using type = i32;
    using pair_type = hud::compressed_pair<type, type>;

    static_assert(hud::is_trivially_copy_assignable_v<type>);

    const auto test = [](const type t1, const type t2)
    {
        pair_type pair {0, 0};

        const i32 first_before = pair.first();
        const i32 second_before = pair.second();
        const pair_type other {t1, t2};

        pair = other;

        return std::tuple {
            first_before,
            second_before,
            pair.first(),
            pair.second()
        };
    };

    // Non constant
    {
        const auto result = test(123, 456);
        hud_assert_eq(std::get<0>(result), 0);
        hud_assert_eq(std::get<1>(result), 0);
        hud_assert_eq(std::get<2>(result), 123);
        hud_assert_eq(std::get<3>(result), 456);
    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        hud_assert_eq(std::get<0>(result), 0);
        hud_assert_eq(std::get<1>(result), 0);
        hud_assert_eq(std::get<2>(result), 123);
        hud_assert_eq(std::get<3>(result), 456);
    }
}

GTEST_TEST(compressed_pair, copy_assign_non_trivially_copy_assignable_same_type)
{

    using type = hud_test::non_bitwise_copy_assignable_type;
    using pair_type = hud::compressed_pair<type, type>;

    static_assert(hud::is_copy_assignable_v<type>);
    static_assert(!hud::is_trivially_copy_assignable_v<type>);

    const auto test = [](const i32 t1, const i32 t2)
    {
        pair_type pair {0, 0};

        const i32 first_id_before = pair.first().id();
        const u32 first_copy_assign_count_before = pair.first().copy_assign_count();
        const u32 first_copy_ctor_count_before = pair.first().copy_constructor_count();
        const i32 second_id_before = pair.second().id();
        const u32 second_copy_assign_count_before = pair.second().copy_assign_count();
        const u32 second_copy_ctor_count_before = pair.second().copy_constructor_count();

        const pair_type other {t1, t2};

        pair = other;

        return std::tuple {
            first_id_before,
            first_copy_assign_count_before,
            first_copy_ctor_count_before,
            second_id_before,
            second_copy_assign_count_before,
            second_copy_ctor_count_before,
            pair.first().id(),
            pair.first().copy_assign_count(),
            pair.first().copy_constructor_count(),
            pair.second().id(),
            pair.second().copy_assign_count(),
            pair.second().copy_constructor_count(),
        };
    };

    // Non constant
    {
        const auto result = test(123, 456);
        hud_assert_eq(std::get<0>(result), 0);
        hud_assert_eq(std::get<1>(result), 0u);
        hud_assert_eq(std::get<2>(result), 0u);
        hud_assert_eq(std::get<3>(result), 0);
        hud_assert_eq(std::get<4>(result), 0u);
        hud_assert_eq(std::get<5>(result), 0u);

        hud_assert_eq(std::get<6>(result), 123);
        hud_assert_eq(std::get<7>(result), 1u);
        hud_assert_eq(std::get<8>(result), 0u);
        hud_assert_eq(std::get<9>(result), 456);
        hud_assert_eq(std::get<10>(result), 1u);
        hud_assert_eq(std::get<11>(result), 0u);
    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        hud_assert_eq(std::get<0>(result), 0);
        hud_assert_eq(std::get<1>(result), 0u);
        hud_assert_eq(std::get<2>(result), 0u);
        hud_assert_eq(std::get<3>(result), 0);
        hud_assert_eq(std::get<4>(result), 0u);
        hud_assert_eq(std::get<5>(result), 0u);

        hud_assert_eq(std::get<6>(result), 123);
        hud_assert_eq(std::get<7>(result), 1u);
        hud_assert_eq(std::get<8>(result), 0u);
        hud_assert_eq(std::get<9>(result), 456);
        hud_assert_eq(std::get<10>(result), 1u);
        hud_assert_eq(std::get<11>(result), 0u);
    }
}

GTEST_TEST(compressed_pair, copy_assign_trivially_copy_assignable_different_type)
{

    using Type1 = i32;
    using Type2 = i16;
    using pair_type = hud::compressed_pair<Type1, Type1>;
    using OtherPairType = hud::compressed_pair<Type2, Type2>;

    static_assert(hud::is_trivially_copy_assignable_v<Type1, Type2>);

    const auto test = [](const Type2 t1, const Type2 t2)
    {
        pair_type pair(0, 0);

        const Type1 first_before = pair.first();
        const Type1 second_before = pair.second();
        const OtherPairType other {t1, t2};

        pair = other;

        return std::tuple {
            first_before,
            second_before,
            pair.first(),
            pair.second()
        };
    };

    // Non constant
    {
        const auto result = test(123, 456);
        hud_assert_eq(std::get<0>(result), 0);
        hud_assert_eq(std::get<1>(result), 0);
        hud_assert_eq(std::get<2>(result), 123);
        hud_assert_eq(std::get<3>(result), 456);
    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        hud_assert_eq(std::get<0>(result), 0);
        hud_assert_eq(std::get<1>(result), 0);
        hud_assert_eq(std::get<2>(result), 123);
        hud_assert_eq(std::get<3>(result), 456);
    }
}

GTEST_TEST(compressed_pair, copy_assign_non_trivially_copy_assignable_different_type)
{

    using Type1 = hud_test::non_bitwise_copy_assignable_type_2;
    using Type2 = hud_test::non_bitwise_copy_assignable_type;
    using pair_type = hud::compressed_pair<Type1, Type1>;
    using OtherPairType = hud::compressed_pair<Type2, Type2>;

    static_assert(hud::is_copy_assignable_v<Type1, Type2>);
    static_assert(!hud::is_trivially_copy_assignable_v<Type1, Type2>);

    const auto test = [](const i32 t1, const i32 t2)
    {
        pair_type pair {0, 0};

        const i32 first_id_before = pair.first().id();
        const u32 first_copy_assign_count_before = pair.first().copy_assign_count();
        const u32 first_copy_ctor_count_before = pair.first().copy_constructor_count();
        const i32 second_id_before = pair.second().id();
        const u32 second_copy_assign_count_before = pair.second().copy_assign_count();
        const u32 second_copy_ctor_count_before = pair.second().copy_constructor_count();

        const OtherPairType other {t1, t2};

        pair = other;

        return std::tuple {
            first_id_before,
            first_copy_assign_count_before,
            first_copy_ctor_count_before,
            second_id_before,
            second_copy_assign_count_before,
            second_copy_ctor_count_before,
            pair.first().id(),
            pair.first().copy_assign_count(),
            pair.first().copy_constructor_count(),
            pair.second().id(),
            pair.second().copy_assign_count(),
            pair.second().copy_constructor_count(),
        };
    };

    // Non constant
    {
        const auto result = test(123, 456);
        hud_assert_eq(std::get<0>(result), 0);
        hud_assert_eq(std::get<1>(result), 0u);
        hud_assert_eq(std::get<2>(result), 0u);
        hud_assert_eq(std::get<3>(result), 0);
        hud_assert_eq(std::get<4>(result), 0u);
        hud_assert_eq(std::get<5>(result), 0u);

        hud_assert_eq(std::get<6>(result), 123);
        hud_assert_eq(std::get<7>(result), 1u);
        hud_assert_eq(std::get<8>(result), 0u);
        hud_assert_eq(std::get<9>(result), 456);
        hud_assert_eq(std::get<10>(result), 1u);
        hud_assert_eq(std::get<11>(result), 0u);
    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        hud_assert_eq(std::get<0>(result), 0);
        hud_assert_eq(std::get<1>(result), 0u);
        hud_assert_eq(std::get<2>(result), 0u);
        hud_assert_eq(std::get<3>(result), 0);
        hud_assert_eq(std::get<4>(result), 0u);
        hud_assert_eq(std::get<5>(result), 0u);

        hud_assert_eq(std::get<6>(result), 123);
        hud_assert_eq(std::get<7>(result), 1u);
        hud_assert_eq(std::get<8>(result), 0u);
        hud_assert_eq(std::get<9>(result), 456);
        hud_assert_eq(std::get<10>(result), 1u);
        hud_assert_eq(std::get<11>(result), 0u);
    }
}

GTEST_TEST(compressed_pair, move_assign_trivially_copy_assignable_same_type)
{

    using type = i32;
    using pair_type = hud::compressed_pair<type, type>;

    static_assert(hud::is_trivially_copy_assignable_v<type>);

    const auto test = [](const type t1, const type t2)
    {
        pair_type pair {0, 0};

        const i32 first_before = pair.first();
        const i32 second_before = pair.second();
        pair_type other {t1, t2};

        pair = hud::move(other);

        return std::tuple {
            first_before,
            second_before,
            pair.first(),
            pair.second()
        };
    };

    // Non constant
    {
        const auto result = test(123, 456);
        hud_assert_eq(std::get<0>(result), 0);
        hud_assert_eq(std::get<1>(result), 0);
        hud_assert_eq(std::get<2>(result), 123);
        hud_assert_eq(std::get<3>(result), 456);
    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        hud_assert_eq(std::get<0>(result), 0);
        hud_assert_eq(std::get<1>(result), 0);
        hud_assert_eq(std::get<2>(result), 123);
        hud_assert_eq(std::get<3>(result), 456);
    }
}

GTEST_TEST(compressed_pair, move_assign_non_trivially_copy_assignable_same_type)
{

    using type = hud_test::non_bitwise_copy_assignable_type;
    using pair_type = hud::compressed_pair<type, type>;

    static_assert(hud::is_copy_assignable_v<type>);
    static_assert(!hud::is_trivially_copy_assignable_v<type>);

    const auto test = [](const i32 t1, const i32 t2)
    {
        pair_type pair {0, 0};

        const i32 first_id_before = pair.first().id();
        const u32 first_copy_assign_count_before = pair.first().copy_assign_count();
        const u32 first_copy_ctor_count_before = pair.first().copy_constructor_count();
        const i32 second_id_before = pair.second().id();
        const u32 second_copy_assign_count_before = pair.second().copy_assign_count();
        const u32 second_copy_ctor_count_before = pair.second().copy_constructor_count();

        pair_type other {t1, t2};

        pair = hud::move(other);

        return std::tuple {
            first_id_before,
            first_copy_assign_count_before,
            first_copy_ctor_count_before,
            second_id_before,
            second_copy_assign_count_before,
            second_copy_ctor_count_before,
            pair.first().id(),
            pair.first().copy_assign_count(),
            pair.first().copy_constructor_count(),
            pair.second().id(),
            pair.second().copy_assign_count(),
            pair.second().copy_constructor_count(),
        };
    };

    // Non constant
    {
        const auto result = test(123, 456);
        hud_assert_eq(std::get<0>(result), 0);
        hud_assert_eq(std::get<1>(result), 0u);
        hud_assert_eq(std::get<2>(result), 0u);
        hud_assert_eq(std::get<3>(result), 0);
        hud_assert_eq(std::get<4>(result), 0u);
        hud_assert_eq(std::get<5>(result), 0u);

        hud_assert_eq(std::get<6>(result), 123);
        hud_assert_eq(std::get<7>(result), 1u);
        hud_assert_eq(std::get<8>(result), 0u);
        hud_assert_eq(std::get<9>(result), 456);
        hud_assert_eq(std::get<10>(result), 1u);
        hud_assert_eq(std::get<11>(result), 0u);
    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        hud_assert_eq(std::get<0>(result), 0);
        hud_assert_eq(std::get<1>(result), 0u);
        hud_assert_eq(std::get<2>(result), 0u);
        hud_assert_eq(std::get<3>(result), 0);
        hud_assert_eq(std::get<4>(result), 0u);
        hud_assert_eq(std::get<5>(result), 0u);

        hud_assert_eq(std::get<6>(result), 123);
        hud_assert_eq(std::get<7>(result), 1u);
        hud_assert_eq(std::get<8>(result), 0u);
        hud_assert_eq(std::get<9>(result), 456);
        hud_assert_eq(std::get<10>(result), 1u);
        hud_assert_eq(std::get<11>(result), 0u);
    }
}

GTEST_TEST(compressed_pair, move_assign_trivially_copy_assignable_different_type)
{

    using Type1 = i32;
    using Type2 = i16;
    using pair_type = hud::compressed_pair<Type1, Type1>;
    using OtherPairType = hud::compressed_pair<Type2, Type2>;

    static_assert(hud::is_trivially_copy_assignable_v<Type1, Type2>);

    const auto test = [](const Type2 t1, const Type2 t2)
    {
        pair_type pair(0, 0);

        const Type1 first_before = pair.first();
        const Type1 second_before = pair.second();
        OtherPairType other {t1, t2};

        pair = hud::move(other);

        return std::tuple {
            first_before,
            second_before,
            pair.first(),
            pair.second()
        };
    };

    // Non constant
    {
        const auto result = test(123, 456);
        hud_assert_eq(std::get<0>(result), 0);
        hud_assert_eq(std::get<1>(result), 0);
        hud_assert_eq(std::get<2>(result), 123);
        hud_assert_eq(std::get<3>(result), 456);
    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        hud_assert_eq(std::get<0>(result), 0);
        hud_assert_eq(std::get<1>(result), 0);
        hud_assert_eq(std::get<2>(result), 123);
        hud_assert_eq(std::get<3>(result), 456);
    }
}

GTEST_TEST(compressed_pair, move_assign_non_trivially_copy_assignable_different_type)
{

    using Type1 = hud_test::non_bitwise_copy_assignable_type_2;
    using Type2 = hud_test::non_bitwise_copy_assignable_type;
    using pair_type = hud::compressed_pair<Type1, Type1>;
    using OtherPairType = hud::compressed_pair<Type2, Type2>;

    static_assert(hud::is_copy_assignable_v<Type1, Type2>);
    static_assert(!hud::is_trivially_copy_assignable_v<Type1, Type2>);

    const auto test = [](const i32 t1, const i32 t2)
    {
        pair_type pair {0, 0};

        const i32 first_id_before = pair.first().id();
        const u32 first_copy_assign_count_before = pair.first().copy_assign_count();
        const u32 first_copy_ctor_count_before = pair.first().copy_constructor_count();
        const i32 second_id_before = pair.second().id();
        const u32 second_copy_assign_count_before = pair.second().copy_assign_count();
        const u32 second_copy_ctor_count_before = pair.second().copy_constructor_count();

        OtherPairType other {t1, t2};

        pair = hud::move(other);

        return std::tuple {
            first_id_before,
            first_copy_assign_count_before,
            first_copy_ctor_count_before,
            second_id_before,
            second_copy_assign_count_before,
            second_copy_ctor_count_before,
            pair.first().id(),
            pair.first().copy_assign_count(),
            pair.first().copy_constructor_count(),
            pair.second().id(),
            pair.second().copy_assign_count(),
            pair.second().copy_constructor_count(),
        };
    };

    // Non constant
    {
        const auto result = test(123, 456);
        hud_assert_eq(std::get<0>(result), 0);
        hud_assert_eq(std::get<1>(result), 0u);
        hud_assert_eq(std::get<2>(result), 0u);
        hud_assert_eq(std::get<3>(result), 0);
        hud_assert_eq(std::get<4>(result), 0u);
        hud_assert_eq(std::get<5>(result), 0u);

        hud_assert_eq(std::get<6>(result), 123);
        hud_assert_eq(std::get<7>(result), 1u);
        hud_assert_eq(std::get<8>(result), 0u);
        hud_assert_eq(std::get<9>(result), 456);
        hud_assert_eq(std::get<10>(result), 1u);
        hud_assert_eq(std::get<11>(result), 0u);
    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        hud_assert_eq(std::get<0>(result), 0);
        hud_assert_eq(std::get<1>(result), 0u);
        hud_assert_eq(std::get<2>(result), 0u);
        hud_assert_eq(std::get<3>(result), 0);
        hud_assert_eq(std::get<4>(result), 0u);
        hud_assert_eq(std::get<5>(result), 0u);

        hud_assert_eq(std::get<6>(result), 123);
        hud_assert_eq(std::get<7>(result), 1u);
        hud_assert_eq(std::get<8>(result), 0u);
        hud_assert_eq(std::get<9>(result), 456);
        hud_assert_eq(std::get<10>(result), 1u);
        hud_assert_eq(std::get<11>(result), 0u);
    }
}

GTEST_TEST(compressed_pair, move_assign_trivially_move_assignable_same_type)
{

    using type = i32;
    using pair_type = hud::compressed_pair<type, type>;

    static_assert(hud::is_trivially_move_assignable_v<type>);

    const auto test = [](const type t1, const type t2)
    {
        pair_type pair {0, 0};

        const i32 first_before = pair.first();
        const i32 second_before = pair.second();
        pair_type other {t1, t2};

        pair = hud::move(other);

        return std::tuple {
            first_before,
            second_before,
            pair.first(),
            pair.second()
        };
    };

    // Non constant
    {
        const auto result = test(123, 456);
        hud_assert_eq(std::get<0>(result), 0);
        hud_assert_eq(std::get<1>(result), 0);
        hud_assert_eq(std::get<2>(result), 123);
        hud_assert_eq(std::get<3>(result), 456);
    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        hud_assert_eq(std::get<0>(result), 0);
        hud_assert_eq(std::get<1>(result), 0);
        hud_assert_eq(std::get<2>(result), 123);
        hud_assert_eq(std::get<3>(result), 456);
    }
}

GTEST_TEST(compressed_pair, move_assign_non_trivially_move_assignable_same_type)
{

    using type = hud_test::non_bitwise_move_assignable_type;
    using pair_type = hud::compressed_pair<type, type>;

    static_assert(hud::is_move_assignable_v<type>);
    static_assert(!hud::is_trivially_move_assignable_v<type>);

    const auto test = [](const i32 t1, const i32 t2)
    {
        pair_type pair {0, 0};

        const i32 first_id_before = pair.first().id();
        const u32 first_copy_assign_count_before = pair.first().copy_assign_count();
        const u32 first_copy_ctor_count_before = pair.first().copy_constructor_count();
        const u32 first_move_assign_count_before = pair.first().move_assign_count();
        const u32 first_move_ctor_count_before = pair.first().move_constructor_count();
        const i32 second_id_before = pair.second().id();
        const u32 second_copy_assign_count_before = pair.second().copy_assign_count();
        const u32 second_copy_ctor_count_before = pair.second().copy_constructor_count();
        const u32 second_move_assign_count_before = pair.second().move_assign_count();
        const u32 second_move_ctor_count_before = pair.second().move_constructor_count();

        pair_type other {t1, t2};

        pair = hud::move(other);

        return std::tuple {
            first_id_before,
            first_copy_assign_count_before,
            first_copy_ctor_count_before,
            first_move_assign_count_before,
            first_move_ctor_count_before,
            second_id_before,
            second_copy_assign_count_before,
            second_copy_ctor_count_before,
            second_move_assign_count_before,
            second_move_ctor_count_before,
            pair.first().id(),
            pair.first().copy_assign_count(),
            pair.first().copy_constructor_count(),
            pair.first().move_assign_count(),
            pair.first().move_constructor_count(),
            pair.second().id(),
            pair.second().copy_assign_count(),
            pair.second().copy_constructor_count(),
            pair.second().move_assign_count(),
            pair.second().move_constructor_count(),
        };
    };

    // Non constant
    {
        const auto result = test(123, 456);
        hud_assert_eq(std::get<0>(result), 0);
        hud_assert_eq(std::get<1>(result), 0u);
        hud_assert_eq(std::get<2>(result), 0u);
        hud_assert_eq(std::get<3>(result), 0u);
        hud_assert_eq(std::get<4>(result), 0u);
        hud_assert_eq(std::get<5>(result), 0);
        hud_assert_eq(std::get<6>(result), 0u);
        hud_assert_eq(std::get<7>(result), 0u);
        hud_assert_eq(std::get<8>(result), 0u);
        hud_assert_eq(std::get<9>(result), 0u);

        hud_assert_eq(std::get<10>(result), 123);
        hud_assert_eq(std::get<11>(result), 0u);
        hud_assert_eq(std::get<12>(result), 0u);
        hud_assert_eq(std::get<13>(result), 1u);
        hud_assert_eq(std::get<14>(result), 0u);
        hud_assert_eq(std::get<15>(result), 456);
        hud_assert_eq(std::get<16>(result), 0u);
        hud_assert_eq(std::get<17>(result), 0u);
        hud_assert_eq(std::get<18>(result), 1u);
        hud_assert_eq(std::get<19>(result), 0u);
    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        hud_assert_eq(std::get<0>(result), 0);
        hud_assert_eq(std::get<1>(result), 0u);
        hud_assert_eq(std::get<2>(result), 0u);
        hud_assert_eq(std::get<3>(result), 0u);
        hud_assert_eq(std::get<4>(result), 0u);
        hud_assert_eq(std::get<5>(result), 0);
        hud_assert_eq(std::get<6>(result), 0u);
        hud_assert_eq(std::get<7>(result), 0u);
        hud_assert_eq(std::get<8>(result), 0u);
        hud_assert_eq(std::get<9>(result), 0u);

        hud_assert_eq(std::get<10>(result), 123);
        hud_assert_eq(std::get<11>(result), 0u);
        hud_assert_eq(std::get<12>(result), 0u);
        hud_assert_eq(std::get<13>(result), 1u);
        hud_assert_eq(std::get<14>(result), 0u);
        hud_assert_eq(std::get<15>(result), 456);
        hud_assert_eq(std::get<16>(result), 0u);
        hud_assert_eq(std::get<17>(result), 0u);
        hud_assert_eq(std::get<18>(result), 1u);
        hud_assert_eq(std::get<19>(result), 0u);
    }
}

GTEST_TEST(compressed_pair, move_assign_trivially_move_assignable_different_type)
{

    using Type1 = i32;
    using Type2 = i16;
    using pair_type = hud::compressed_pair<Type1, Type1>;
    using OtherPairType = hud::compressed_pair<Type2, Type2>;

    static_assert(hud::is_trivially_move_assignable_v<Type1, Type2>);

    const auto test = [](const Type2 t1, const Type2 t2)
    {
        pair_type pair(0, 0);

        const Type1 first_before = pair.first();
        const Type1 second_before = pair.second();
        OtherPairType other {t1, t2};

        pair = hud::move(other);

        return std::tuple {
            first_before,
            second_before,
            pair.first(),
            pair.second()
        };
    };

    // Non constant
    {
        const auto result = test(123, 456);
        hud_assert_eq(std::get<0>(result), 0);
        hud_assert_eq(std::get<1>(result), 0);
        hud_assert_eq(std::get<2>(result), 123);
        hud_assert_eq(std::get<3>(result), 456);
    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        hud_assert_eq(std::get<0>(result), 0);
        hud_assert_eq(std::get<1>(result), 0);
        hud_assert_eq(std::get<2>(result), 123);
        hud_assert_eq(std::get<3>(result), 456);
    }
}

GTEST_TEST(compressed_pair, move_assign_non_trivially_move_assignable_different_type)
{

    using Type1 = hud_test::non_bitwise_move_assignable_type2;
    using Type2 = hud_test::non_bitwise_move_assignable_type;
    using pair_type = hud::compressed_pair<Type1, Type1>;
    using OtherPairType = hud::compressed_pair<Type2, Type2>;

    static_assert(hud::is_move_assignable_v<Type1, Type2>);
    static_assert(!hud::is_trivially_move_assignable_v<Type1, Type2>);

    const auto test = [](const i32 t1, const i32 t2)
    {
        pair_type pair {0, 0};

        const i32 first_id_before = pair.first().id();
        const u32 first_copy_assign_count_before = pair.first().copy_assign_count();
        const u32 first_copy_ctor_count_before = pair.first().copy_constructor_count();
        const u32 first_move_assign_count_before = pair.first().move_assign_count();
        const u32 first_move_ctor_count_before = pair.first().move_constructor_count();
        const i32 second_id_before = pair.second().id();
        const u32 second_copy_assign_count_before = pair.second().copy_assign_count();
        const u32 second_copy_ctor_count_before = pair.second().copy_constructor_count();
        const u32 second_move_assign_count_before = pair.second().move_assign_count();
        const u32 second_move_ctor_count_before = pair.second().move_constructor_count();

        OtherPairType other {t1, t2};

        pair = hud::move(other);

        return std::tuple {
            first_id_before,
            first_copy_assign_count_before,
            first_copy_ctor_count_before,
            first_move_assign_count_before,
            first_move_ctor_count_before,
            second_id_before,
            second_copy_assign_count_before,
            second_copy_ctor_count_before,
            second_move_assign_count_before,
            second_move_ctor_count_before,
            pair.first().id(),
            pair.first().copy_assign_count(),
            pair.first().copy_constructor_count(),
            pair.first().move_assign_count(),
            pair.first().move_constructor_count(),
            pair.second().id(),
            pair.second().copy_assign_count(),
            pair.second().copy_constructor_count(),
            pair.second().move_assign_count(),
            pair.second().move_constructor_count(),
        };
    };

    // Non constant
    {
        const auto result = test(123, 456);
        hud_assert_eq(std::get<0>(result), 0);
        hud_assert_eq(std::get<1>(result), 0u);
        hud_assert_eq(std::get<2>(result), 0u);
        hud_assert_eq(std::get<3>(result), 0u);
        hud_assert_eq(std::get<4>(result), 0u);
        hud_assert_eq(std::get<5>(result), 0);
        hud_assert_eq(std::get<6>(result), 0u);
        hud_assert_eq(std::get<7>(result), 0u);
        hud_assert_eq(std::get<8>(result), 0u);
        hud_assert_eq(std::get<9>(result), 0u);

        hud_assert_eq(std::get<10>(result), 123);
        hud_assert_eq(std::get<11>(result), 0u);
        hud_assert_eq(std::get<12>(result), 0u);
        hud_assert_eq(std::get<13>(result), 1u);
        hud_assert_eq(std::get<14>(result), 0u);
        hud_assert_eq(std::get<15>(result), 456);
        hud_assert_eq(std::get<16>(result), 0u);
        hud_assert_eq(std::get<17>(result), 0u);
        hud_assert_eq(std::get<18>(result), 1u);
        hud_assert_eq(std::get<19>(result), 0u);
    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        hud_assert_eq(std::get<0>(result), 0);
        hud_assert_eq(std::get<1>(result), 0u);
        hud_assert_eq(std::get<2>(result), 0u);
        hud_assert_eq(std::get<3>(result), 0u);
        hud_assert_eq(std::get<4>(result), 0u);
        hud_assert_eq(std::get<5>(result), 0);
        hud_assert_eq(std::get<6>(result), 0u);
        hud_assert_eq(std::get<7>(result), 0u);
        hud_assert_eq(std::get<8>(result), 0u);
        hud_assert_eq(std::get<9>(result), 0u);

        hud_assert_eq(std::get<10>(result), 123);
        hud_assert_eq(std::get<11>(result), 0u);
        hud_assert_eq(std::get<12>(result), 0u);
        hud_assert_eq(std::get<13>(result), 1u);
        hud_assert_eq(std::get<14>(result), 0u);
        hud_assert_eq(std::get<15>(result), 456);
        hud_assert_eq(std::get<16>(result), 0u);
        hud_assert_eq(std::get<17>(result), 0u);
        hud_assert_eq(std::get<18>(result), 1u);
        hud_assert_eq(std::get<19>(result), 0u);
    }
}