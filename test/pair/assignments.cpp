#include <core/containers/pair.h>
#include <core/traits/is_trivially_copy_assignable.h>
#include <core/traits/is_copy_assignable.h>

TEST(Pair, copy_assign_trivially_copy_assignable_same_type) {


    using type = i32;
    using PairType = Pair<type, type>;

    static_assert(hud::is_trivially_copy_assignable_v<type>);

    const auto test = [](const type t1, const type t2) {
        PairType pair{ 0,0 };

        const i32 first_before = pair.first;
        const i32 second_before = pair.second;
        const PairType other{ t1, t2 };

        pair = other;

        return std::tuple{
            first_before,
            second_before,
            pair.first,
            pair.second
        };
    };


    // Non constant
    {
        const auto result = test(123, 456);
        ASSERT_EQ(hud::get<0>(result), 0);
        ASSERT_EQ(hud::get<1>(result), 0);
        ASSERT_EQ(hud::get<2>(result), 123);
        ASSERT_EQ(hud::get<3>(result), 456);
    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        ASSERT_EQ(hud::get<0>(result), 0);
        ASSERT_EQ(hud::get<1>(result), 0);
        ASSERT_EQ(hud::get<2>(result), 123);
        ASSERT_EQ(hud::get<3>(result), 456);
    }
}

TEST(Pair, copy_assign_non_trivially_copy_assignable_same_type) {


    using type = hud::test::NonBitwiseCopyAssignableType;
    using PairType = Pair<type, type>;

    static_assert(hud::is_copy_assignable_v<type>);
    static_assert(!hud::is_trivially_copy_assignable_v<type>);

    const auto test = [](const i32 t1, const i32 t2) {
        PairType pair{ 0,0 };

        const i32 first_id_before = pair.first.id();
        const u32 first_copy_assign_count_before = pair.first.copy_assign_count();
        const u32 first_copy_ctor_count_before = pair.first.copy_constructor_count();
        const i32 second_id_before = pair.second.id();
        const u32 second_copy_assign_count_before = pair.second.copy_assign_count();
        const u32 second_copy_ctor_count_before = pair.second.copy_constructor_count();

        const PairType other{ t1, t2 };

        pair = other;

        return std::tuple{
            first_id_before,
            first_copy_assign_count_before,
            first_copy_ctor_count_before,
            second_id_before,
            second_copy_assign_count_before,
            second_copy_ctor_count_before,
            pair.first.id(),
            pair.first.copy_assign_count(),
            pair.first.copy_constructor_count(),
            pair.second.id(),
            pair.second.copy_assign_count(),
            pair.second.copy_constructor_count(),
        };
    };


    // Non constant
    {
        const auto result = test(123, 456);
        ASSERT_EQ(hud::get<0>(result), 0);
        ASSERT_EQ(hud::get<1>(result), 0u);
        ASSERT_EQ(hud::get<2>(result), 0u);
        ASSERT_EQ(hud::get<3>(result), 0);
        ASSERT_EQ(hud::get<4>(result), 0u);
        ASSERT_EQ(hud::get<5>(result), 0u);

        ASSERT_EQ(hud::get<6>(result), 123);
        ASSERT_EQ(hud::get<7>(result), 1u);
        ASSERT_EQ(hud::get<8>(result), 0u);
        ASSERT_EQ(hud::get<9>(result), 456);
        ASSERT_EQ(hud::get<10>(result), 1u);
        ASSERT_EQ(hud::get<11>(result), 0u);

    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        ASSERT_EQ(hud::get<0>(result), 0);
        ASSERT_EQ(hud::get<1>(result), 0u);
        ASSERT_EQ(hud::get<2>(result), 0u);
        ASSERT_EQ(hud::get<3>(result), 0);
        ASSERT_EQ(hud::get<4>(result), 0u);
        ASSERT_EQ(hud::get<5>(result), 0u);

        ASSERT_EQ(hud::get<6>(result), 123);
        ASSERT_EQ(hud::get<7>(result), 1u);
        ASSERT_EQ(hud::get<8>(result), 0u);
        ASSERT_EQ(hud::get<9>(result), 456);
        ASSERT_EQ(hud::get<10>(result), 1u);
        ASSERT_EQ(hud::get<11>(result), 0u);
    }
}

TEST(Pair, copy_assign_trivially_copy_assignable_different_type) {


    using Type1 = i32;
    using Type2 = i16;
    using PairType = Pair<Type1, Type1>;
    using OtherPairType = Pair<Type2, Type2>;

    static_assert(hud::is_trivially_copy_assignable_v<Type1, Type2>);

    const auto test = [](const Type2 t1, const Type2 t2) {
        PairType pair(0, 0) ;

        const Type1 first_before = pair.first;
        const Type1 second_before = pair.second;
        const OtherPairType other{ t1, t2 };

        pair = other;

        return std::tuple{
            first_before,
            second_before,
            pair.first,
            pair.second
        };
    };


    // Non constant
    {
        const auto result = test(123, 456);
        ASSERT_EQ(hud::get<0>(result), 0);
        ASSERT_EQ(hud::get<1>(result), 0);
        ASSERT_EQ(hud::get<2>(result), 123);
        ASSERT_EQ(hud::get<3>(result), 456);
    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        ASSERT_EQ(hud::get<0>(result), 0);
        ASSERT_EQ(hud::get<1>(result), 0);
        ASSERT_EQ(hud::get<2>(result), 123);
        ASSERT_EQ(hud::get<3>(result), 456);
    }
}

TEST(Pair, copy_assign_non_trivially_copy_assignable_different_type) {


    using Type1 = hud::test::NonBitwiseCopyAssignableType2;
    using Type2 = hud::test::NonBitwiseCopyAssignableType;
    using PairType = Pair<Type1, Type1>;
    using OtherPairType = Pair< Type2, Type2>;

    static_assert(hud::is_copy_assignable_v<Type1, Type2>);
    static_assert(!hud::is_trivially_copy_assignable_v<Type1, Type2>);

    const auto test = [](const i32 t1, const i32 t2) {
        PairType pair{ 0,0 };

        const i32 first_id_before = pair.first.id();
        const u32 first_copy_assign_count_before = pair.first.copy_assign_count();
        const u32 first_copy_ctor_count_before = pair.first.copy_constructor_count();
        const i32 second_id_before = pair.second.id();
        const u32 second_copy_assign_count_before = pair.second.copy_assign_count();
        const u32 second_copy_ctor_count_before = pair.second.copy_constructor_count();

        const OtherPairType other{ t1, t2 };

        pair = other;

        return std::tuple{
            first_id_before,
            first_copy_assign_count_before,
            first_copy_ctor_count_before,
            second_id_before,
            second_copy_assign_count_before,
            second_copy_ctor_count_before,
            pair.first.id(),
            pair.first.copy_assign_count(),
            pair.first.copy_constructor_count(),
            pair.second.id(),
            pair.second.copy_assign_count(),
            pair.second.copy_constructor_count(),
        };
    };


    // Non constant
    {
        const auto result = test(123, 456);
        ASSERT_EQ(hud::get<0>(result), 0);
        ASSERT_EQ(hud::get<1>(result), 0u);
        ASSERT_EQ(hud::get<2>(result), 0u);
        ASSERT_EQ(hud::get<3>(result), 0);
        ASSERT_EQ(hud::get<4>(result), 0u);
        ASSERT_EQ(hud::get<5>(result), 0u);

        ASSERT_EQ(hud::get<6>(result), 123);
        ASSERT_EQ(hud::get<7>(result), 1u);
        ASSERT_EQ(hud::get<8>(result), 0u);
        ASSERT_EQ(hud::get<9>(result), 456);
        ASSERT_EQ(hud::get<10>(result), 1u);
        ASSERT_EQ(hud::get<11>(result), 0u);

    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        ASSERT_EQ(hud::get<0>(result), 0);
        ASSERT_EQ(hud::get<1>(result), 0u);
        ASSERT_EQ(hud::get<2>(result), 0u);
        ASSERT_EQ(hud::get<3>(result), 0);
        ASSERT_EQ(hud::get<4>(result), 0u);
        ASSERT_EQ(hud::get<5>(result), 0u);

        ASSERT_EQ(hud::get<6>(result), 123);
        ASSERT_EQ(hud::get<7>(result), 1u);
        ASSERT_EQ(hud::get<8>(result), 0u);
        ASSERT_EQ(hud::get<9>(result), 456);
        ASSERT_EQ(hud::get<10>(result), 1u);
        ASSERT_EQ(hud::get<11>(result), 0u);
    }
}

TEST(Pair, move_assign_trivially_copy_assignable_same_type) {


    using type = i32;
    using PairType = Pair<type, type>;

    static_assert(hud::is_trivially_copy_assignable_v<type>);

    const auto test = [](const type t1, const type t2) {
        PairType pair{ 0,0 };

        const i32 first_before = pair.first;
        const i32 second_before = pair.second;
        PairType other{ t1, t2 };

        pair = hud::move(other);

        return std::tuple{
            first_before,
            second_before,
            pair.first,
            pair.second
        };
    };


    // Non constant
    {
        const auto result = test(123, 456);
        ASSERT_EQ(hud::get<0>(result), 0);
        ASSERT_EQ(hud::get<1>(result), 0);
        ASSERT_EQ(hud::get<2>(result), 123);
        ASSERT_EQ(hud::get<3>(result), 456);
    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        ASSERT_EQ(hud::get<0>(result), 0);
        ASSERT_EQ(hud::get<1>(result), 0);
        ASSERT_EQ(hud::get<2>(result), 123);
        ASSERT_EQ(hud::get<3>(result), 456);
    }

}

TEST(Pair, move_assign_non_trivially_copy_assignable_same_type) {


    using type = hud::test::NonBitwiseCopyAssignableType;
    using PairType = Pair<type, type>;

    static_assert(hud::is_copy_assignable_v<type>);
    static_assert(!hud::is_trivially_copy_assignable_v<type>);

    const auto test = [](const i32 t1, const i32 t2) {
        PairType pair{ 0,0 };

        const i32 first_id_before = pair.first.id();
        const u32 first_copy_assign_count_before = pair.first.copy_assign_count();
        const u32 first_copy_ctor_count_before = pair.first.copy_constructor_count();
        const i32 second_id_before = pair.second.id();
        const u32 second_copy_assign_count_before = pair.second.copy_assign_count();
        const u32 second_copy_ctor_count_before = pair.second.copy_constructor_count();

        PairType other{ t1, t2 };

        pair = hud::move(other);

        return std::tuple{
            first_id_before,
            first_copy_assign_count_before,
            first_copy_ctor_count_before,
            second_id_before,
            second_copy_assign_count_before,
            second_copy_ctor_count_before,
            pair.first.id(),
            pair.first.copy_assign_count(),
            pair.first.copy_constructor_count(),
            pair.second.id(),
            pair.second.copy_assign_count(),
            pair.second.copy_constructor_count(),
        };
    };


    // Non constant
    {
        const auto result = test(123, 456);
        ASSERT_EQ(hud::get<0>(result), 0);
        ASSERT_EQ(hud::get<1>(result), 0u);
        ASSERT_EQ(hud::get<2>(result), 0u);
        ASSERT_EQ(hud::get<3>(result), 0);
        ASSERT_EQ(hud::get<4>(result), 0u);
        ASSERT_EQ(hud::get<5>(result), 0u);

        ASSERT_EQ(hud::get<6>(result), 123);
        ASSERT_EQ(hud::get<7>(result), 1u);
        ASSERT_EQ(hud::get<8>(result), 0u);
        ASSERT_EQ(hud::get<9>(result), 456);
        ASSERT_EQ(hud::get<10>(result), 1u);
        ASSERT_EQ(hud::get<11>(result), 0u);

    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        ASSERT_EQ(hud::get<0>(result), 0);
        ASSERT_EQ(hud::get<1>(result), 0u);
        ASSERT_EQ(hud::get<2>(result), 0u);
        ASSERT_EQ(hud::get<3>(result), 0);
        ASSERT_EQ(hud::get<4>(result), 0u);
        ASSERT_EQ(hud::get<5>(result), 0u);

        ASSERT_EQ(hud::get<6>(result), 123);
        ASSERT_EQ(hud::get<7>(result), 1u);
        ASSERT_EQ(hud::get<8>(result), 0u);
        ASSERT_EQ(hud::get<9>(result), 456);
        ASSERT_EQ(hud::get<10>(result), 1u);
        ASSERT_EQ(hud::get<11>(result), 0u);
    }
}

TEST(Pair, move_assign_trivially_copy_assignable_different_type) {


    using Type1 = i32;
    using Type2 = i16;
    using PairType = Pair<Type1, Type1>;
    using OtherPairType = Pair<Type2, Type2>;

    static_assert(hud::is_trivially_copy_assignable_v<Type1, Type2>);

    const auto test = [](const Type2 t1, const Type2 t2) {
        PairType pair(0, 0);

        const Type1 first_before = pair.first;
        const Type1 second_before = pair.second;
        OtherPairType other{ t1, t2 };

        pair = hud::move(other);

        return std::tuple{
            first_before,
            second_before,
            pair.first,
            pair.second
        };
    };


    // Non constant
    {
        const auto result = test(123, 456);
        ASSERT_EQ(hud::get<0>(result), 0);
        ASSERT_EQ(hud::get<1>(result), 0);
        ASSERT_EQ(hud::get<2>(result), 123);
        ASSERT_EQ(hud::get<3>(result), 456);
    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        ASSERT_EQ(hud::get<0>(result), 0);
        ASSERT_EQ(hud::get<1>(result), 0);
        ASSERT_EQ(hud::get<2>(result), 123);
        ASSERT_EQ(hud::get<3>(result), 456);
    }
}

TEST(Pair, move_assign_non_trivially_copy_assignable_different_type) {


    using Type1 = hud::test::NonBitwiseCopyAssignableType2;
    using Type2 = hud::test::NonBitwiseCopyAssignableType;
    using PairType = Pair<Type1, Type1>;
    using OtherPairType = Pair< Type2, Type2>;

    static_assert(hud::is_copy_assignable_v<Type1, Type2>);
    static_assert(!hud::is_trivially_copy_assignable_v<Type1, Type2>);

    const auto test = [](const i32 t1, const i32 t2) {
        PairType pair{ 0,0 };

        const i32 first_id_before = pair.first.id();
        const u32 first_copy_assign_count_before = pair.first.copy_assign_count();
        const u32 first_copy_ctor_count_before = pair.first.copy_constructor_count();
        const i32 second_id_before = pair.second.id();
        const u32 second_copy_assign_count_before = pair.second.copy_assign_count();
        const u32 second_copy_ctor_count_before = pair.second.copy_constructor_count();

        OtherPairType other{ t1, t2 };

        pair = hud::move(other);

        return std::tuple{
            first_id_before,
            first_copy_assign_count_before,
            first_copy_ctor_count_before,
            second_id_before,
            second_copy_assign_count_before,
            second_copy_ctor_count_before,
            pair.first.id(),
            pair.first.copy_assign_count(),
            pair.first.copy_constructor_count(),
            pair.second.id(),
            pair.second.copy_assign_count(),
            pair.second.copy_constructor_count(),
        };
    };


    // Non constant
    {
        const auto result = test(123, 456);
        ASSERT_EQ(hud::get<0>(result), 0);
        ASSERT_EQ(hud::get<1>(result), 0u);
        ASSERT_EQ(hud::get<2>(result), 0u);
        ASSERT_EQ(hud::get<3>(result), 0);
        ASSERT_EQ(hud::get<4>(result), 0u);
        ASSERT_EQ(hud::get<5>(result), 0u);

        ASSERT_EQ(hud::get<6>(result), 123);
        ASSERT_EQ(hud::get<7>(result), 1u);
        ASSERT_EQ(hud::get<8>(result), 0u);
        ASSERT_EQ(hud::get<9>(result), 456);
        ASSERT_EQ(hud::get<10>(result), 1u);
        ASSERT_EQ(hud::get<11>(result), 0u);

    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        ASSERT_EQ(hud::get<0>(result), 0);
        ASSERT_EQ(hud::get<1>(result), 0u);
        ASSERT_EQ(hud::get<2>(result), 0u);
        ASSERT_EQ(hud::get<3>(result), 0);
        ASSERT_EQ(hud::get<4>(result), 0u);
        ASSERT_EQ(hud::get<5>(result), 0u);

        ASSERT_EQ(hud::get<6>(result), 123);
        ASSERT_EQ(hud::get<7>(result), 1u);
        ASSERT_EQ(hud::get<8>(result), 0u);
        ASSERT_EQ(hud::get<9>(result), 456);
        ASSERT_EQ(hud::get<10>(result), 1u);
        ASSERT_EQ(hud::get<11>(result), 0u);
    }
}

TEST(Pair, move_assign_trivially_move_assignable_same_type) {


    using type = i32;
    using PairType = Pair<type, type>;

    static_assert(hud::is_trivially_move_assignable_v<type>);

    const auto test = [](const type t1, const type t2) {
        PairType pair{ 0,0 };

        const i32 first_before = pair.first;
        const i32 second_before = pair.second;
        PairType other{ t1, t2 };

        pair = hud::move(other);

        return std::tuple{
            first_before,
            second_before,
            pair.first,
            pair.second
        };
    };


    // Non constant
    {
        const auto result = test(123, 456);
        ASSERT_EQ(hud::get<0>(result), 0);
        ASSERT_EQ(hud::get<1>(result), 0);
        ASSERT_EQ(hud::get<2>(result), 123);
        ASSERT_EQ(hud::get<3>(result), 456);
    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        ASSERT_EQ(hud::get<0>(result), 0);
        ASSERT_EQ(hud::get<1>(result), 0);
        ASSERT_EQ(hud::get<2>(result), 123);
        ASSERT_EQ(hud::get<3>(result), 456);
    }

}

TEST(Pair, move_assign_non_trivially_move_assignable_same_type) {


    using type = hud::test::NonBitwiseMoveAssignableType;
    using PairType = Pair<type, type>;

    static_assert(hud::is_move_assignable_v<type>);
    static_assert(!hud::is_trivially_move_assignable_v<type>);

    const auto test = [](const i32 t1, const i32 t2) {
        PairType pair{ 0,0 };

        const i32 first_id_before = pair.first.id();
        const u32 first_copy_assign_count_before = pair.first.copy_assign_count();
        const u32 first_copy_ctor_count_before = pair.first.copy_constructor_count();
        const u32 first_move_assign_count_before = pair.first.move_assign_count();
        const u32 first_move_ctor_count_before = pair.first.move_constructor_count();
        const i32 second_id_before = pair.second.id();
        const u32 second_copy_assign_count_before = pair.second.copy_assign_count();
        const u32 second_copy_ctor_count_before = pair.second.copy_constructor_count();
        const u32 second_move_assign_count_before = pair.second.move_assign_count();
        const u32 second_move_ctor_count_before = pair.second.move_constructor_count();

        PairType other{ t1, t2 };

        pair = hud::move(other);

        return std::tuple{
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
            pair.first.id(),
            pair.first.copy_assign_count(),
            pair.first.copy_constructor_count(),
            pair.first.move_assign_count(),
            pair.first.move_constructor_count(),
            pair.second.id(),
            pair.second.copy_assign_count(),
            pair.second.copy_constructor_count(),
            pair.second.move_assign_count(),
            pair.second.move_constructor_count(),
        };
    };


    // Non constant
    {
        const auto result = test(123, 456);
        ASSERT_EQ(hud::get<0>(result), 0);
        ASSERT_EQ(hud::get<1>(result), 0u);
        ASSERT_EQ(hud::get<2>(result), 0u);
        ASSERT_EQ(hud::get<3>(result), 0u);
        ASSERT_EQ(hud::get<4>(result), 0u);
        ASSERT_EQ(hud::get<5>(result), 0);
        ASSERT_EQ(hud::get<6>(result), 0u);
        ASSERT_EQ(hud::get<7>(result), 0u);
        ASSERT_EQ(hud::get<8>(result), 0u);
        ASSERT_EQ(hud::get<9>(result), 0u);

        ASSERT_EQ(hud::get<10>(result), 123);
        ASSERT_EQ(hud::get<11>(result), 0u);
        ASSERT_EQ(hud::get<12>(result), 0u);
        ASSERT_EQ(hud::get<13>(result), 1u);
        ASSERT_EQ(hud::get<14>(result), 0u);
        ASSERT_EQ(hud::get<15>(result), 456);
        ASSERT_EQ(hud::get<16>(result), 0u);
        ASSERT_EQ(hud::get<17>(result), 0u);
        ASSERT_EQ(hud::get<18>(result), 1u);
        ASSERT_EQ(hud::get<19>(result), 0u);
    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        ASSERT_EQ(hud::get<0>(result), 0);
        ASSERT_EQ(hud::get<1>(result), 0u);
        ASSERT_EQ(hud::get<2>(result), 0u);
        ASSERT_EQ(hud::get<3>(result), 0u);
        ASSERT_EQ(hud::get<4>(result), 0u);
        ASSERT_EQ(hud::get<5>(result), 0);
        ASSERT_EQ(hud::get<6>(result), 0u);
        ASSERT_EQ(hud::get<7>(result), 0u);
        ASSERT_EQ(hud::get<8>(result), 0u);
        ASSERT_EQ(hud::get<9>(result), 0u);

        ASSERT_EQ(hud::get<10>(result), 123);
        ASSERT_EQ(hud::get<11>(result), 0u);
        ASSERT_EQ(hud::get<12>(result), 0u);
        ASSERT_EQ(hud::get<13>(result), 1u);
        ASSERT_EQ(hud::get<14>(result), 0u);
        ASSERT_EQ(hud::get<15>(result), 456);
        ASSERT_EQ(hud::get<16>(result), 0u);
        ASSERT_EQ(hud::get<17>(result), 0u);
        ASSERT_EQ(hud::get<18>(result), 1u);
        ASSERT_EQ(hud::get<19>(result), 0u);
    }
}

TEST(Pair, move_assign_trivially_move_assignable_different_type) {


    using Type1 = i32;
    using Type2 = i16;
    using PairType = Pair<Type1, Type1>;
    using OtherPairType = Pair<Type2, Type2>;

    static_assert(hud::is_trivially_move_assignable_v<Type1, Type2>);

    const auto test = [](const Type2 t1, const Type2 t2) {
        PairType pair(0, 0);

        const Type1 first_before = pair.first;
        const Type1 second_before = pair.second;
        OtherPairType other{ t1, t2 };

        pair = hud::move(other);

        return std::tuple{
            first_before,
            second_before,
            pair.first,
            pair.second
        };
    };


    // Non constant
    {
        const auto result = test(123, 456);
        ASSERT_EQ(hud::get<0>(result), 0);
        ASSERT_EQ(hud::get<1>(result), 0);
        ASSERT_EQ(hud::get<2>(result), 123);
        ASSERT_EQ(hud::get<3>(result), 456);
    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        ASSERT_EQ(hud::get<0>(result), 0);
        ASSERT_EQ(hud::get<1>(result), 0);
        ASSERT_EQ(hud::get<2>(result), 123);
        ASSERT_EQ(hud::get<3>(result), 456);
    }
}

TEST(Pair, move_assign_non_trivially_move_assignable_different_type) {


    using Type1 = hud::test::NonBitwiseMoveAssignableType2;
    using Type2 = hud::test::NonBitwiseMoveAssignableType;
    using PairType = Pair<Type1, Type1>;
    using OtherPairType = Pair< Type2, Type2>;

    static_assert(hud::is_move_assignable_v<Type1, Type2>);
    static_assert(!hud::is_trivially_move_assignable_v<Type1, Type2>);

    const auto test = [](const i32 t1, const i32 t2) {
        PairType pair{ 0,0 };

        const i32 first_id_before = pair.first.id();
        const u32 first_copy_assign_count_before = pair.first.copy_assign_count();
        const u32 first_copy_ctor_count_before = pair.first.copy_constructor_count();
        const u32 first_move_assign_count_before = pair.first.move_assign_count();
        const u32 first_move_ctor_count_before = pair.first.move_constructor_count();
        const i32 second_id_before = pair.second.id();
        const u32 second_copy_assign_count_before = pair.second.copy_assign_count();
        const u32 second_copy_ctor_count_before = pair.second.copy_constructor_count();
        const u32 second_move_assign_count_before = pair.second.move_assign_count();
        const u32 second_move_ctor_count_before = pair.second.move_constructor_count();

        OtherPairType other{ t1, t2 };

        pair = hud::move(other);

        return std::tuple{
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
            pair.first.id(),
            pair.first.copy_assign_count(),
            pair.first.copy_constructor_count(),
            pair.first.move_assign_count(),
            pair.first.move_constructor_count(),
            pair.second.id(),
            pair.second.copy_assign_count(),
            pair.second.copy_constructor_count(),
            pair.second.move_assign_count(),
            pair.second.move_constructor_count(),
        };
    };


    // Non constant
    {
        const auto result = test(123, 456);
        ASSERT_EQ(hud::get<0>(result), 0);
        ASSERT_EQ(hud::get<1>(result), 0u);
        ASSERT_EQ(hud::get<2>(result), 0u);
        ASSERT_EQ(hud::get<3>(result), 0u);
        ASSERT_EQ(hud::get<4>(result), 0u);
        ASSERT_EQ(hud::get<5>(result), 0);
        ASSERT_EQ(hud::get<6>(result), 0u);
        ASSERT_EQ(hud::get<7>(result), 0u);
        ASSERT_EQ(hud::get<8>(result), 0u);
        ASSERT_EQ(hud::get<9>(result), 0u);

        ASSERT_EQ(hud::get<10>(result), 123);
        ASSERT_EQ(hud::get<11>(result), 0u);
        ASSERT_EQ(hud::get<12>(result), 0u);
        ASSERT_EQ(hud::get<13>(result), 1u);
        ASSERT_EQ(hud::get<14>(result), 0u);
        ASSERT_EQ(hud::get<15>(result), 456);
        ASSERT_EQ(hud::get<16>(result), 0u);
        ASSERT_EQ(hud::get<17>(result), 0u);
        ASSERT_EQ(hud::get<18>(result), 1u);
        ASSERT_EQ(hud::get<19>(result), 0u);
    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        ASSERT_EQ(hud::get<0>(result), 0);
        ASSERT_EQ(hud::get<1>(result), 0u);
        ASSERT_EQ(hud::get<2>(result), 0u);
        ASSERT_EQ(hud::get<3>(result), 0u);
        ASSERT_EQ(hud::get<4>(result), 0u);
        ASSERT_EQ(hud::get<5>(result), 0);
        ASSERT_EQ(hud::get<6>(result), 0u);
        ASSERT_EQ(hud::get<7>(result), 0u);
        ASSERT_EQ(hud::get<8>(result), 0u);
        ASSERT_EQ(hud::get<9>(result), 0u);

        ASSERT_EQ(hud::get<10>(result), 123);
        ASSERT_EQ(hud::get<11>(result), 0u);
        ASSERT_EQ(hud::get<12>(result), 0u);
        ASSERT_EQ(hud::get<13>(result), 1u);
        ASSERT_EQ(hud::get<14>(result), 0u);
        ASSERT_EQ(hud::get<15>(result), 456);
        ASSERT_EQ(hud::get<16>(result), 0u);
        ASSERT_EQ(hud::get<17>(result), 0u);
        ASSERT_EQ(hud::get<18>(result), 1u);
        ASSERT_EQ(hud::get<19>(result), 0u);
    }
}