#include <core/containers/compressed_tuple.h>
#include <core/traits/is_same.h>

namespace hud_test
{
    struct empty
    {
    };
} // namespace hud_test

GTEST_TEST(compressed_tuple, sizeof_is_correct)
{
    hud_assert_eq(sizeof(hud::compressed_tuple<>), 1u);
    hud_assert_eq(sizeof(hud::compressed_tuple<hud_test::empty, hud_test::empty, hud_test::empty>), 3u);
    hud_assert_eq(sizeof(hud::compressed_tuple<i32, hud_test::empty, hud_test::empty>), 8u);
    hud_assert_eq(sizeof(hud::compressed_tuple<hud_test::empty, i32, hud_test::empty>), 8u);
    hud_assert_eq(sizeof(hud::compressed_tuple<hud_test::empty, hud_test::empty, i32>), 4u);
    hud_assert_eq(sizeof(hud::compressed_tuple<i32>), 4u);
    hud_assert_eq(sizeof(hud::compressed_tuple<i32, i32>), 8u);
    hud_assert_eq(sizeof(hud::compressed_tuple<i32, i8, i32>), 12u);
}

GTEST_TEST(compressed_tuple, make_tuple)
{
    auto compressed_tuple = hud::make_compressed_tuple(12, 15.0f, L'w');
    hud_assert_true((hud::is_same_v<decltype(compressed_tuple), hud::compressed_tuple<i32, f32, wchar>>));
    hud_assert_eq(hud::get<0>(compressed_tuple), 12);
    hud_assert_eq(hud::get<1>(compressed_tuple), 15.0f);
    hud_assert_eq(hud::get<2>(compressed_tuple), L'w');

    const auto tuple2 = hud::make_compressed_tuple(12, 15.0f, L'w');
    hud_assert_true((hud::is_same_v<decltype(tuple2), const hud::compressed_tuple<i32, f32, wchar>>));
    hud_assert_eq(hud::get<0>(tuple2), 12);
    hud_assert_eq(hud::get<1>(tuple2), 15.0f);
    hud_assert_eq(hud::get<2>(tuple2), L'w');

    constexpr auto tuple3 = hud::make_compressed_tuple(12, 15.0f, L'w');
    hud_assert_true((hud::is_same_v<decltype(tuple3), const hud::compressed_tuple<i32, f32, wchar>>));
    hud_assert_eq(hud::get<0>(tuple3), 12);
    hud_assert_eq(hud::get<1>(tuple3), 15.0f);
    hud_assert_eq(hud::get<2>(tuple3), L'w');
}

GTEST_TEST(compressed_tuple, get)
{
    const auto test = []()
    {
        auto compressed_tuple = hud::make_compressed_tuple(12, 15.0f, L'w');
        return std::tuple {
            hud::get<0>(compressed_tuple) == 12,
            hud::get<1>(compressed_tuple) == 15.0f,
            hud::get<2>(compressed_tuple) == L'w',
            hud::get<0>(hud::move(compressed_tuple)) == 12,
            hud::get<1>(hud::move(compressed_tuple)) == 15.0f,
            hud::get<2>(hud::move(compressed_tuple)) == L'w'
        };
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
    }
}

GTEST_TEST(compressed_tuple, tuple_cat)
{
    using tuple_type1 = hud::compressed_tuple<i32, f32>;
    using tuple_type2 = hud::compressed_tuple<ansichar, wchar, char16, hud_test::non_bitwise_type>;
    using tuple_type3 = hud::compressed_tuple<u64, f64, hud_test::non_bitwise_type>;

    using CatType = decltype(hud::tuple_cat(tuple_type1 {}, tuple_type2 {}, tuple_type3 {}));

    CatType compressed_tuple = CatType {
        123,
        12.0f,
        'h',
        L'u',
        u'd',
        hud_test::non_bitwise_type {456, nullptr},
        789u,
        159.,
        hud_test::non_bitwise_type {753, nullptr}
    };

    hud_assert_eq(hud::tuple_size_v<CatType>, hud::tuple_size_v<tuple_type1> + hud::tuple_size_v<tuple_type2> + hud::tuple_size_v<tuple_type3>);
    hud_assert_true((hud::is_same_v<decltype(hud::get<0>(compressed_tuple)), i32 &>));
    hud_assert_true((hud::is_same_v<decltype(hud::get<1>(compressed_tuple)), f32 &>));
    hud_assert_true((hud::is_same_v<decltype(hud::get<2>(compressed_tuple)), ansichar &>));
    hud_assert_true((hud::is_same_v<decltype(hud::get<3>(compressed_tuple)), wchar &>));
    hud_assert_true((hud::is_same_v<decltype(hud::get<4>(compressed_tuple)), char16 &>));
    hud_assert_true((hud::is_same_v<decltype(hud::get<5>(compressed_tuple)), hud_test::non_bitwise_type &>));
    hud_assert_true((hud::is_same_v<decltype(hud::get<6>(compressed_tuple)), u64 &>));
    hud_assert_true((hud::is_same_v<decltype(hud::get<7>(compressed_tuple)), f64 &>));
    hud_assert_true((hud::is_same_v<decltype(hud::get<8>(compressed_tuple)), hud_test::non_bitwise_type &>));

    hud_assert_eq(hud::get<0>(compressed_tuple), 123);
    hud_assert_eq(hud::get<1>(compressed_tuple), 12.0f);
    hud_assert_eq(hud::get<2>(compressed_tuple), 'h');
    hud_assert_eq(hud::get<3>(compressed_tuple), L'u');
    hud_assert_true(hud::get<4>(compressed_tuple) == u'd');
    hud_assert_eq(hud::get<5>(compressed_tuple).id(), 456);
    hud_assert_eq(hud::get<5>(compressed_tuple).destructor_counter(), nullptr);
    hud_assert_eq(hud::get<5>(compressed_tuple).constructor_count(), 1u);
    hud_assert_eq(hud::get<5>(compressed_tuple).copy_constructor_count(), 0u);
    hud_assert_eq(hud::get<5>(compressed_tuple).move_constructor_count(), 1u);
    hud_assert_eq(hud::get<5>(compressed_tuple).copy_assign_count(), 0u);
    hud_assert_eq(hud::get<5>(compressed_tuple).move_assign_count(), 0u);
    hud_assert_eq(hud::get<6>(compressed_tuple), 789u);
    hud_assert_eq(hud::get<7>(compressed_tuple), 159.);
    hud_assert_eq(hud::get<8>(compressed_tuple).id(), 753);
    hud_assert_eq(hud::get<8>(compressed_tuple).destructor_counter(), nullptr);
    hud_assert_eq(hud::get<8>(compressed_tuple).constructor_count(), 1u);
    hud_assert_eq(hud::get<8>(compressed_tuple).copy_constructor_count(), 0u);
    hud_assert_eq(hud::get<8>(compressed_tuple).move_constructor_count(), 1u);
    hud_assert_eq(hud::get<8>(compressed_tuple).copy_assign_count(), 0u);
    hud_assert_eq(hud::get<8>(compressed_tuple).move_assign_count(), 0u);

    using EmptyCatType = decltype(hud::compressed_tuple_cat());
    hud_assert_true((hud::is_same_v<EmptyCatType, hud::compressed_tuple<>>));
    hud_assert_eq(hud::tuple_size_v<EmptyCatType>, 0u);
}

GTEST_TEST(compressed_tuple, structure_binding_by_copy)
{
    using type = hud_test::non_bitwise_type;
    static_assert(hud::is_constructible_v<type, type &&>);
    static_assert(!hud::is_trivially_constructible_v<type, type &&>);

    const auto test = [](type &&t1, type &&t2, type &&t3)
    {
        auto compressed_tuple = hud::make_tuple(hud::move(t1), hud::move(t2), hud::move(t3));
        auto [first_cpy, second_cpy, third_cpy] = compressed_tuple;
        first_cpy = type {321, nullptr};
        second_cpy = type {654, nullptr};
        third_cpy = type {987, nullptr};
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
            third_cpy.constructor_count(),
            third_cpy.move_constructor_count(),
            third_cpy.copy_constructor_count(),
            third_cpy.move_assign_count(),
            third_cpy.copy_assign_count(),
            hud::get<0>(compressed_tuple).id() == 123,
            hud::get<1>(compressed_tuple).id() == 456,
            hud::get<2>(compressed_tuple).id() == 789
        };
    };
    // Non constant
    {
        const auto result = test(type {123, nullptr}, type {456, nullptr}, type {789, nullptr});
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
        hud_assert_eq(std::get<10>(result), 1);
        hud_assert_eq(std::get<11>(result), 0);
        hud_assert_eq(std::get<12>(result), 0);
        hud_assert_eq(std::get<13>(result), 1);
        hud_assert_eq(std::get<14>(result), 0);
        hud_assert_true(std::get<15>(result));
        hud_assert_true(std::get<16>(result));
        hud_assert_true(std::get<17>(result));
    }

    // Non constant
    {
        constexpr auto result = test(type {123, nullptr}, type {456, nullptr}, type {789, nullptr});
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
        hud_assert_eq(std::get<10>(result), 1);
        hud_assert_eq(std::get<11>(result), 0);
        hud_assert_eq(std::get<12>(result), 0);
        hud_assert_eq(std::get<13>(result), 1);
        hud_assert_eq(std::get<14>(result), 0);
        hud_assert_true(std::get<15>(result));
        hud_assert_true(std::get<16>(result));
        hud_assert_true(std::get<17>(result));
    }
}

GTEST_TEST(compressed_tuple, structure_binding_rvalue)
{
    using type = hud_test::non_bitwise_type;
    static_assert(hud::is_constructible_v<type, type &&>);
    static_assert(!hud::is_trivially_constructible_v<type, type &&>);

    const auto test = [](type &&t1, type &&t2, type &&t3)
    {
        auto compressed_tuple = hud::make_tuple(hud::move(t1), hud::move(t2), hud::move(t3));
        auto &[first_ref, second_ref, third_ref] = compressed_tuple;
        first_ref = type {321, nullptr};
        second_ref = type {654, nullptr};
        third_ref = type {987, nullptr};
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
            third_ref.constructor_count(),
            third_ref.move_constructor_count(),
            third_ref.copy_constructor_count(),
            third_ref.move_assign_count(),
            third_ref.copy_assign_count(),
            hud::get<0>(compressed_tuple).id() == 321,
            hud::get<1>(compressed_tuple).id() == 654,
            hud::get<2>(compressed_tuple).id() == 987
        };
    };
    // Non constant
    {
        const auto result = test(type {123, nullptr}, type {456, nullptr}, type {789, nullptr});
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
        hud_assert_eq(std::get<10>(result), 1);
        hud_assert_eq(std::get<11>(result), 0);
        hud_assert_eq(std::get<12>(result), 0);
        hud_assert_eq(std::get<13>(result), 1);
        hud_assert_eq(std::get<14>(result), 0);
        hud_assert_true(std::get<15>(result));
        hud_assert_true(std::get<16>(result));
        hud_assert_true(std::get<17>(result));
    }

    // Non constant
    {
        constexpr auto result = test(type {123, nullptr}, type {456, nullptr}, type {789, nullptr});
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
        hud_assert_eq(std::get<10>(result), 1);
        hud_assert_eq(std::get<11>(result), 0);
        hud_assert_eq(std::get<12>(result), 0);
        hud_assert_eq(std::get<13>(result), 1);
        hud_assert_eq(std::get<14>(result), 0);
        hud_assert_true(std::get<15>(result));
        hud_assert_true(std::get<16>(result));
        hud_assert_true(std::get<17>(result));
    }
}

GTEST_TEST(compressed_tuple, structure_binding_const_rvalue)
{
    using type = hud_test::non_bitwise_type;
    static_assert(hud::is_constructible_v<type, type &&>);
    static_assert(!hud::is_trivially_constructible_v<type, type &&>);

    const auto test = [](type &&t1, type &&t2, type &&t3)
    {
        auto compressed_tuple = hud::make_tuple(hud::move(t1), hud::move(t2), hud::move(t3));
        const auto &[first_ref, second_ref, third_ref] = compressed_tuple;
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
            third_ref.constructor_count(),
            third_ref.move_constructor_count(),
            third_ref.copy_constructor_count(),
            third_ref.move_assign_count(),
            third_ref.copy_assign_count(),
            hud::get<0>(compressed_tuple).id() == 123,
            hud::get<1>(compressed_tuple).id() == 456,
            hud::get<2>(compressed_tuple).id() == 789
        };
    };

    // Non constant
    {
        const auto result = test(type {123, nullptr}, type {456, nullptr}, type {789, nullptr});
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
        hud_assert_eq(std::get<10>(result), 1);
        hud_assert_eq(std::get<11>(result), 1);
        hud_assert_eq(std::get<12>(result), 0);
        hud_assert_eq(std::get<13>(result), 0);
        hud_assert_eq(std::get<14>(result), 0);
        hud_assert_true(std::get<15>(result));
        hud_assert_true(std::get<16>(result));
        hud_assert_true(std::get<17>(result));
    }

    // Non constant
    {
        constexpr auto result = test(type {123, nullptr}, type {456, nullptr}, type {789, nullptr});
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
        hud_assert_eq(std::get<10>(result), 1);
        hud_assert_eq(std::get<11>(result), 1);
        hud_assert_eq(std::get<12>(result), 0);
        hud_assert_eq(std::get<13>(result), 0);
        hud_assert_eq(std::get<14>(result), 0);
        hud_assert_true(std::get<15>(result));
        hud_assert_true(std::get<16>(result));
        hud_assert_true(std::get<17>(result));
    }
}

GTEST_TEST(compressed_tuple, structure_binding_lvalue)
{
    using type = hud_test::non_bitwise_type;
    static_assert(hud::is_constructible_v<type, type &&>);
    static_assert(!hud::is_trivially_constructible_v<type, type &&>);

    const auto test = [](type &&t1, type &&t2, type &&t3)
    {
        auto compressed_tuple = hud::make_tuple(hud::move(t1), hud::move(t2), hud::move(t3));
        auto &[first_ref, second_ref, third_ref] = compressed_tuple;
        first_ref = type {321, nullptr};
        second_ref = type {654, nullptr};
        third_ref = type {987, nullptr};
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
            third_ref.constructor_count(),
            third_ref.move_constructor_count(),
            third_ref.copy_constructor_count(),
            third_ref.move_assign_count(),
            third_ref.copy_assign_count(),
            hud::get<0>(compressed_tuple).id() == 321,
            hud::get<1>(compressed_tuple).id() == 654,
            hud::get<2>(compressed_tuple).id() == 987
        };
    };
    // Non constant
    {
        const auto result = test(type {123, nullptr}, type {456, nullptr}, type {789, nullptr});
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
        hud_assert_eq(std::get<10>(result), 1);
        hud_assert_eq(std::get<11>(result), 0);
        hud_assert_eq(std::get<12>(result), 0);
        hud_assert_eq(std::get<13>(result), 1);
        hud_assert_eq(std::get<14>(result), 0);
        hud_assert_true(std::get<15>(result));
        hud_assert_true(std::get<16>(result));
        hud_assert_true(std::get<17>(result));
    }

    // Non constant
    {
        constexpr auto result = test(type {123, nullptr}, type {456, nullptr}, type {789, nullptr});
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
        hud_assert_eq(std::get<10>(result), 1);
        hud_assert_eq(std::get<11>(result), 0);
        hud_assert_eq(std::get<12>(result), 0);
        hud_assert_eq(std::get<13>(result), 1);
        hud_assert_eq(std::get<14>(result), 0);
        hud_assert_true(std::get<15>(result));
        hud_assert_true(std::get<16>(result));
        hud_assert_true(std::get<17>(result));
    }
}

GTEST_TEST(compressed_tuple, structure_binding_const_tuple_lvalue)
{
    using type = hud_test::non_bitwise_type;
    static_assert(hud::is_constructible_v<type, type &&>);
    static_assert(!hud::is_trivially_constructible_v<type, type &&>);

    const auto test = [](type &&t1, type &&t2, type &&t3)
    {
        const auto compressed_tuple = hud::make_tuple(hud::move(t1), hud::move(t2), hud::move(t3));
        auto &[first_ref, second_ref, third_ref] = compressed_tuple;
        // Not possible, compressed_tuple is const
        // first_ref = type {321, nullptr};
        // second_ref = type {654, nullptr};
        // third_ref = type {987, nullptr};
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
            third_ref.constructor_count(),
            third_ref.move_constructor_count(),
            third_ref.copy_constructor_count(),
            third_ref.move_assign_count(),
            third_ref.copy_assign_count(),
            hud::get<0>(compressed_tuple).id() == 123,
            hud::get<1>(compressed_tuple).id() == 456,
            hud::get<2>(compressed_tuple).id() == 789
        };
    };
    // Non constant
    {
        const auto result = test(type {123, nullptr}, type {456, nullptr}, type {789, nullptr});
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
        hud_assert_eq(std::get<10>(result), 1);
        hud_assert_eq(std::get<11>(result), 1);
        hud_assert_eq(std::get<12>(result), 0);
        hud_assert_eq(std::get<13>(result), 0);
        hud_assert_eq(std::get<14>(result), 0);
        hud_assert_true(std::get<15>(result));
        hud_assert_true(std::get<16>(result));
        hud_assert_true(std::get<17>(result));
    }

    // Non constant
    {
        constexpr auto result = test(type {123, nullptr}, type {456, nullptr}, type {789, nullptr});
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
        hud_assert_eq(std::get<10>(result), 1);
        hud_assert_eq(std::get<11>(result), 1);
        hud_assert_eq(std::get<12>(result), 0);
        hud_assert_eq(std::get<13>(result), 0);
        hud_assert_eq(std::get<14>(result), 0);
        hud_assert_true(std::get<15>(result));
        hud_assert_true(std::get<16>(result));
        hud_assert_true(std::get<17>(result));
    }
}
