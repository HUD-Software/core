#include <core/containers/tuple.h>

GTEST_TEST(tuple, copy_assign_trivially_copy_assignable_same_types) {
    using tuple_type = hud::tuple<f32, i32, wchar>;

    const auto test = []() {
        tuple_type tuple{ hud::taginit };
        const tuple_type other_tuple{ 12.0f, 123, L'h' };
        tuple = other_tuple;
        return std::tuple{
            hud::get<0>(tuple) == 12.f,
            hud::get<1>(tuple) == 123,
            hud::get<2>(tuple) == L'h',
        };
    };


    // Constant
    {
        const auto result = test();
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_TRUE(std::get<2>(result));

    }

    // Non constant
    {
        constexpr auto result = test();
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_TRUE(std::get<2>(result));
    }
}

GTEST_TEST(tuple, copy_assign_trivially_copy_assignable_different_types) {
    using tuple_type = hud::tuple<u64, i32, wchar>;
    using other_tuple_type = hud::tuple<u16, i16, char16>;

    const auto test = []() {
        tuple_type tuple{ hud::taginit };
        const other_tuple_type other_tuple{ (u16)12, (i16)123, u'h' };
        tuple = other_tuple;
        return std::tuple{
            hud::get<0>(tuple) == 12,
            hud::get<1>(tuple) == 123,
            hud::get<2>(tuple) == L'h',
        };
    };


    // Constant
    {
        const auto result = test();
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_TRUE(std::get<2>(result));

    }

    // Non constant
    {
        constexpr auto result = test();
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_TRUE(std::get<2>(result));
    }
}

GTEST_TEST(tuple, copy_assign_non_trivially_copy_assignable_same_types) {
    using tuple_type = hud::tuple<hud_test::non_bitwise_copy_assignable_type ,f32, i32, wchar>;

    const auto test = []() {
        tuple_type tuple{ hud::taginit };
        const tuple_type other_tuple{ 1, 12.0f, 123, L'h' };
        tuple = other_tuple;
        return std::tuple{
            hud::get<0>(tuple).id() == 1,
            hud::get<0>(tuple).copy_constructor_count() == 0u,
            hud::get<0>(tuple).copy_assign_count() == 1u,
            hud::get<1>(tuple) == 12.f,
            hud::get<2>(tuple) == 123,
            hud::get<3>(tuple) == L'h',
        };
    };


    // Constant
    {
        const auto result = test();
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_TRUE(std::get<2>(result));
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));

    }

    // Non constant
    {
        constexpr auto result = test();
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_TRUE(std::get<2>(result));
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
    }
}

GTEST_TEST(tuple, copy_assign_non_trivially_copy_assignable_different_types) {
    using tuple_type = hud::tuple<hud_test::non_bitwise_copy_assignable_type_2, f32, i32, wchar>;
    using other_tuple_type = hud::tuple<hud_test::non_bitwise_copy_assignable_type, u16, i16, char16>;

    const auto test = []() {
        tuple_type tuple{ hud::taginit };
        const other_tuple_type other_tuple{ 1, (u16)12, (i16)123, u'h' };
        tuple = other_tuple;
        return std::tuple{
            hud::get<0>(tuple).id() == 1,
            hud::get<0>(tuple).copy_constructor_count() == 0u,
            hud::get<0>(tuple).copy_assign_count() == 1u,
            hud::get<1>(tuple) == 12.f,
            hud::get<2>(tuple) == 123,
            hud::get<3>(tuple) == L'h',
        };
    };


    // Constant
    {
        const auto result = test();
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_TRUE(std::get<2>(result));
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));

    }

    // Non constant
    {
        constexpr auto result = test();
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_TRUE(std::get<2>(result));
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
    }
}

GTEST_TEST(tuple, move_assign_trivially_copy_assignable_same_types) {
    using tuple_type = hud::tuple<f32, i32, wchar>;

    const auto test = []() {
        tuple_type tuple{ hud::taginit };
        tuple_type other_tuple{ 12.0f, 123, L'h' };
        tuple = hud::move(other_tuple);
        return std::tuple{
            hud::get<0>(tuple) == 12.f,
            hud::get<1>(tuple) == 123,
            hud::get<2>(tuple) == L'h',
        };
    };


    // Constant
    {
        const auto result = test();
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_TRUE(std::get<2>(result));

    }

    // Non constant
    {
        constexpr auto result = test();
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_TRUE(std::get<2>(result));
    }
}

GTEST_TEST(tuple, move_assign_trivially_copy_assignable_different_types) {
    using tuple_type = hud::tuple<u64, i32, wchar>;
    using other_tuple_type = hud::tuple<u16, i16, char16>;

    const auto test = []() {
        tuple_type tuple{ hud::taginit };
        other_tuple_type other_tuple{ (u16)12, (i16)123, u'h' };
        tuple = hud::move(other_tuple);
        return std::tuple{
            hud::get<0>(tuple) == 12,
            hud::get<1>(tuple) == 123,
            hud::get<2>(tuple) == L'h',
        };
    };


    // Constant
    {
        const auto result = test();
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_TRUE(std::get<2>(result));

    }

    // Non constant
    {
        constexpr auto result = test();
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_TRUE(std::get<2>(result));
    }
}

GTEST_TEST(tuple, move_assign_non_trivially_copy_assignable_same_types) {
    using tuple_type = hud::tuple<hud_test::non_bitwise_copy_assignable_type, f32, i32, wchar>;

    const auto test = []() {
        tuple_type tuple{ hud::taginit };
        tuple_type other_tuple{ 1, 12.0f, 123, L'h' };
        tuple = hud::move(other_tuple);
        return std::tuple{
            hud::get<0>(tuple).id() == 1,
            hud::get<0>(tuple).copy_constructor_count() == 0u,
            hud::get<0>(tuple).copy_assign_count() == 1u,
            hud::get<1>(tuple) == 12.f,
            hud::get<2>(tuple) == 123,
            hud::get<3>(tuple) == L'h',
        };
    };


    // Constant
    {
        const auto result = test();
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_TRUE(std::get<2>(result));
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));

    }

    // Non constant
    {
        constexpr auto result = test();
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_TRUE(std::get<2>(result));
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
    }
}

GTEST_TEST(tuple, move_assign_non_trivially_copy_assignable_different_types) {
    using tuple_type = hud::tuple<hud_test::non_bitwise_copy_assignable_type_2, f32, i32, wchar>;
    using other_tuple_type = hud::tuple<hud_test::non_bitwise_copy_assignable_type, u16, i16, char16>;

    const auto test = []() {
        tuple_type tuple{ hud::taginit };
        other_tuple_type other_tuple{ 1, (u16)12, (i16)123, u'h' };
        tuple = hud::move(other_tuple);
        return std::tuple{
            hud::get<0>(tuple).id() == 1,
            hud::get<0>(tuple).copy_constructor_count() == 0u,
            hud::get<0>(tuple).copy_assign_count() == 1u,
            hud::get<1>(tuple) == 12.f,
            hud::get<2>(tuple) == 123,
            hud::get<3>(tuple) == L'h',
        };
    };


    // Constant
    {
        const auto result = test();
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_TRUE(std::get<2>(result));
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));

    }

    // Non constant
    {
        constexpr auto result = test();
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_TRUE(std::get<2>(result));
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
    }
}

GTEST_TEST(tuple, move_assign_trivially_move_assignable_same_types) {
    using tuple_type = hud::tuple<f32, i32, wchar>;

    const auto test = []() {
        tuple_type tuple{ hud::taginit };
        tuple_type other_tuple{ 12.0f, 123, L'h' };
        tuple = hud::move(other_tuple);
        return std::tuple{
            hud::get<0>(tuple) == 12.f,
            hud::get<1>(tuple) == 123,
            hud::get<2>(tuple) == L'h',
        };
    };


    // Constant
    {
        const auto result = test();
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_TRUE(std::get<2>(result));

    }

    // Non constant
    {
        constexpr auto result = test();
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_TRUE(std::get<2>(result));
    }
}

GTEST_TEST(tuple, move_assign_trivially_move_assignable_different_types) {
    using tuple_type = hud::tuple<u64, i32, wchar>;
    using other_tuple_type = hud::tuple<u16, i16, char16>;

    const auto test = []() {
        tuple_type tuple{ hud::taginit };
        other_tuple_type other_tuple{ (u16)12, (i16)123, u'h' };
        tuple = hud::move(other_tuple);
        return std::tuple{
            hud::get<0>(tuple) == 12,
            hud::get<1>(tuple) == 123,
            hud::get<2>(tuple) == L'h',
        };
    };


    // Constant
    {
        const auto result = test();
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_TRUE(std::get<2>(result));

    }

    // Non constant
    {
        constexpr auto result = test();
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_TRUE(std::get<2>(result));
    }
}

GTEST_TEST(tuple, move_assign_non_trivially_move_assignable_same_types) {
    using tuple_type = hud::tuple<hud_test::NonBitwiseMoveAssignableType, f32, i32, wchar>;

    const auto test = []() {
        tuple_type tuple{ hud::taginit };
        tuple_type other_tuple{ 1, 12.0f, 123, L'h' };
        tuple = hud::move(other_tuple);
        return std::tuple{
            hud::get<0>(tuple).id() == 1,
            hud::get<0>(tuple).copy_constructor_count() == 0u,
            hud::get<0>(tuple).move_constructor_count() == 0u,
            hud::get<0>(tuple).copy_assign_count() == 0u,
            hud::get<0>(tuple).move_assign_count() == 1u,
            hud::get<1>(tuple) == 12.f,
            hud::get<2>(tuple) == 123,
            hud::get<3>(tuple) == L'h',
        };
    };


    // Constant
    {
        const auto result = test();
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_TRUE(std::get<2>(result));
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_TRUE(std::get<6>(result));
        GTEST_ASSERT_TRUE(std::get<7>(result));

    }

    // Non constant
    {
        constexpr auto result = test();
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_TRUE(std::get<2>(result));
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_TRUE(std::get<6>(result));
        GTEST_ASSERT_TRUE(std::get<7>(result));
    }
}

GTEST_TEST(tuple, move_assign_non_trivially_move_assignable_different_types) {
    using tuple_type = hud::tuple<hud_test::NonBitwiseMoveAssignableType2, f32, i32, wchar>;
    using other_tuple_type = hud::tuple<hud_test::NonBitwiseMoveAssignableType, u16, i16, char16>;

    const auto test = []() {
        tuple_type tuple{ hud::taginit };
        other_tuple_type other_tuple{ 1, (u16)12, (i16)123, u'h' };
        tuple = hud::move(other_tuple);
        return std::tuple{
            hud::get<0>(tuple).id() == 1,
            hud::get<0>(tuple).copy_constructor_count() == 0u,
            hud::get<0>(tuple).move_constructor_count() == 0u,
            hud::get<0>(tuple).copy_assign_count() == 0u,
            hud::get<0>(tuple).move_assign_count() == 1u,
            hud::get<1>(tuple) == 12.f,
            hud::get<2>(tuple) == 123,
            hud::get<3>(tuple) == L'h',
        };
    };


    // Constant
    {
        const auto result = test();
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_TRUE(std::get<2>(result));
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_TRUE(std::get<6>(result));
        GTEST_ASSERT_TRUE(std::get<7>(result));

    }

    // Non constant
    {
        constexpr auto result = test();
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_TRUE(std::get<2>(result));
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_TRUE(std::get<6>(result));
        GTEST_ASSERT_TRUE(std::get<7>(result));
    }
}