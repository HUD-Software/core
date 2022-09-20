#include <core/containers/tuple.h>

TEST(Tuple, copy_assign_trivially_copy_assignable_same_types) {
    using TupleType = hud::Tuple<f32, i32, wchar>;

    const auto test = []() {
        TupleType tuple{ hud::tag_init };
        const TupleType other_tuple{ 12.0f, 123, L'h' };
        tuple = other_tuple;
        return std::tuple{
            hud::hud::get<0>(tuple) == 12.f,
            hud::hud::get<1>(tuple) == 123,
            hud::hud::get<2>(tuple) == L'h',
        };
    };


    // Constant
    {
        const auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));

    }

    // Non constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
    }
}

TEST(Tuple, copy_assign_trivially_copy_assignable_different_types) {
    using TupleType = hud::Tuple<u64, i32, wchar>;
    using OtherTupleType = hud::Tuple<u16, i16, char16>;

    const auto test = []() {
        TupleType tuple{ hud::tag_init };
        const OtherTupleType other_tuple{ (u16)12, (i16)123, u'h' };
        tuple = other_tuple;
        return std::tuple{
            hud::hud::get<0>(tuple) == 12,
            hud::hud::get<1>(tuple) == 123,
            hud::hud::get<2>(tuple) == L'h',
        };
    };


    // Constant
    {
        const auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));

    }

    // Non constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
    }
}

TEST(Tuple, copy_assign_non_trivially_copy_assignable_same_types) {
    using TupleType = hud::Tuple<hud::test::NonBitwiseCopyAssignableType ,f32, i32, wchar>;

    const auto test = []() {
        TupleType tuple{ hud::tag_init };
        const TupleType other_tuple{ 1, 12.0f, 123, L'h' };
        tuple = other_tuple;
        return std::tuple{
            hud::hud::get<0>(tuple).id() == 1,
            hud::hud::get<0>(tuple).copy_constructor_count() == 0u,
            hud::hud::get<0>(tuple).copy_assign_count() == 1u,
            hud::hud::get<1>(tuple) == 12.f,
            hud::hud::get<2>(tuple) == 123,
            hud::hud::get<3>(tuple) == L'h',
        };
    };


    // Constant
    {
        const auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
        ASSERT_TRUE(hud::get<4>(result));
        ASSERT_TRUE(hud::get<5>(result));

    }

    // Non constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
        ASSERT_TRUE(hud::get<4>(result));
        ASSERT_TRUE(hud::get<5>(result));
    }
}

TEST(Tuple, copy_assign_non_trivially_copy_assignable_different_types) {
    using TupleType = hud::Tuple<hud::test::NonBitwiseCopyAssignableType2, f32, i32, wchar>;
    using OtherTupleType = hud::Tuple<hud::test::NonBitwiseCopyAssignableType, u16, i16, char16>;

    const auto test = []() {
        TupleType tuple{ hud::tag_init };
        const OtherTupleType other_tuple{ 1, (u16)12, (i16)123, u'h' };
        tuple = other_tuple;
        return std::tuple{
            hud::hud::get<0>(tuple).id() == 1,
            hud::hud::get<0>(tuple).copy_constructor_count() == 0u,
            hud::hud::get<0>(tuple).copy_assign_count() == 1u,
            hud::hud::get<1>(tuple) == 12.f,
            hud::hud::get<2>(tuple) == 123,
            hud::hud::get<3>(tuple) == L'h',
        };
    };


    // Constant
    {
        const auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
        ASSERT_TRUE(hud::get<4>(result));
        ASSERT_TRUE(hud::get<5>(result));

    }

    // Non constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
        ASSERT_TRUE(hud::get<4>(result));
        ASSERT_TRUE(hud::get<5>(result));
    }
}

TEST(Tuple, move_assign_trivially_copy_assignable_same_types) {
    using TupleType = hud::Tuple<f32, i32, wchar>;

    const auto test = []() {
        TupleType tuple{ hud::tag_init };
        TupleType other_tuple{ 12.0f, 123, L'h' };
        tuple = hud::move(other_tuple);
        return std::tuple{
            hud::hud::get<0>(tuple) == 12.f,
            hud::hud::get<1>(tuple) == 123,
            hud::hud::get<2>(tuple) == L'h',
        };
    };


    // Constant
    {
        const auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));

    }

    // Non constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
    }
}

TEST(Tuple, move_assign_trivially_copy_assignable_different_types) {
    using TupleType = hud::Tuple<u64, i32, wchar>;
    using OtherTupleType = hud::Tuple<u16, i16, char16>;

    const auto test = []() {
        TupleType tuple{ hud::tag_init };
        OtherTupleType other_tuple{ (u16)12, (i16)123, u'h' };
        tuple = hud::move(other_tuple);
        return std::tuple{
            hud::hud::get<0>(tuple) == 12,
            hud::hud::get<1>(tuple) == 123,
            hud::hud::get<2>(tuple) == L'h',
        };
    };


    // Constant
    {
        const auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));

    }

    // Non constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
    }
}

TEST(Tuple, move_assign_non_trivially_copy_assignable_same_types) {
    using TupleType = hud::Tuple<hud::test::NonBitwiseCopyAssignableType, f32, i32, wchar>;

    const auto test = []() {
        TupleType tuple{ hud::tag_init };
        TupleType other_tuple{ 1, 12.0f, 123, L'h' };
        tuple = hud::move(other_tuple);
        return std::tuple{
            hud::hud::get<0>(tuple).id() == 1,
            hud::hud::get<0>(tuple).copy_constructor_count() == 0u,
            hud::hud::get<0>(tuple).copy_assign_count() == 1u,
            hud::hud::get<1>(tuple) == 12.f,
            hud::hud::get<2>(tuple) == 123,
            hud::hud::get<3>(tuple) == L'h',
        };
    };


    // Constant
    {
        const auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
        ASSERT_TRUE(hud::get<4>(result));
        ASSERT_TRUE(hud::get<5>(result));

    }

    // Non constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
        ASSERT_TRUE(hud::get<4>(result));
        ASSERT_TRUE(hud::get<5>(result));
    }
}

TEST(Tuple, move_assign_non_trivially_copy_assignable_different_types) {
    using TupleType = hud::Tuple<hud::test::NonBitwiseCopyAssignableType2, f32, i32, wchar>;
    using OtherTupleType = hud::Tuple<hud::test::NonBitwiseCopyAssignableType, u16, i16, char16>;

    const auto test = []() {
        TupleType tuple{ hud::tag_init };
        OtherTupleType other_tuple{ 1, (u16)12, (i16)123, u'h' };
        tuple = hud::move(other_tuple);
        return std::tuple{
            hud::hud::get<0>(tuple).id() == 1,
            hud::hud::get<0>(tuple).copy_constructor_count() == 0u,
            hud::hud::get<0>(tuple).copy_assign_count() == 1u,
            hud::hud::get<1>(tuple) == 12.f,
            hud::hud::get<2>(tuple) == 123,
            hud::hud::get<3>(tuple) == L'h',
        };
    };


    // Constant
    {
        const auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
        ASSERT_TRUE(hud::get<4>(result));
        ASSERT_TRUE(hud::get<5>(result));

    }

    // Non constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
        ASSERT_TRUE(hud::get<4>(result));
        ASSERT_TRUE(hud::get<5>(result));
    }
}

TEST(Tuple, move_assign_trivially_move_assignable_same_types) {
    using TupleType = hud::Tuple<f32, i32, wchar>;

    const auto test = []() {
        TupleType tuple{ hud::tag_init };
        TupleType other_tuple{ 12.0f, 123, L'h' };
        tuple = hud::move(other_tuple);
        return std::tuple{
            hud::hud::get<0>(tuple) == 12.f,
            hud::hud::get<1>(tuple) == 123,
            hud::hud::get<2>(tuple) == L'h',
        };
    };


    // Constant
    {
        const auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));

    }

    // Non constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
    }
}

TEST(Tuple, move_assign_trivially_move_assignable_different_types) {
    using TupleType = hud::Tuple<u64, i32, wchar>;
    using OtherTupleType = hud::Tuple<u16, i16, char16>;

    const auto test = []() {
        TupleType tuple{ hud::tag_init };
        OtherTupleType other_tuple{ (u16)12, (i16)123, u'h' };
        tuple = hud::move(other_tuple);
        return std::tuple{
            hud::hud::get<0>(tuple) == 12,
            hud::hud::get<1>(tuple) == 123,
            hud::hud::get<2>(tuple) == L'h',
        };
    };


    // Constant
    {
        const auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));

    }

    // Non constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
    }
}

TEST(Tuple, move_assign_non_trivially_move_assignable_same_types) {
    using TupleType = hud::Tuple<hud::test::NonBitwiseMoveAssignableType, f32, i32, wchar>;

    const auto test = []() {
        TupleType tuple{ hud::tag_init };
        TupleType other_tuple{ 1, 12.0f, 123, L'h' };
        tuple = hud::move(other_tuple);
        return std::tuple{
            hud::hud::get<0>(tuple).id() == 1,
            hud::hud::get<0>(tuple).copy_constructor_count() == 0u,
            hud::hud::get<0>(tuple).move_constructor_count() == 0u,
            hud::hud::get<0>(tuple).copy_assign_count() == 0u,
            hud::hud::get<0>(tuple).move_assign_count() == 1u,
            hud::hud::get<1>(tuple) == 12.f,
            hud::hud::get<2>(tuple) == 123,
            hud::hud::get<3>(tuple) == L'h',
        };
    };


    // Constant
    {
        const auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
        ASSERT_TRUE(hud::get<4>(result));
        ASSERT_TRUE(hud::get<5>(result));
        ASSERT_TRUE(hud::get<6>(result));
        ASSERT_TRUE(hud::get<7>(result));

    }

    // Non constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
        ASSERT_TRUE(hud::get<4>(result));
        ASSERT_TRUE(hud::get<5>(result));
        ASSERT_TRUE(hud::get<6>(result));
        ASSERT_TRUE(hud::get<7>(result));
    }
}

TEST(Tuple, move_assign_non_trivially_move_assignable_different_types) {
    using TupleType = hud::Tuple<hud::test::NonBitwiseMoveAssignableType2, f32, i32, wchar>;
    using OtherTupleType = hud::Tuple<hud::test::NonBitwiseMoveAssignableType, u16, i16, char16>;

    const auto test = []() {
        TupleType tuple{ hud::tag_init };
        OtherTupleType other_tuple{ 1, (u16)12, (i16)123, u'h' };
        tuple = hud::move(other_tuple);
        return std::tuple{
            hud::hud::get<0>(tuple).id() == 1,
            hud::hud::get<0>(tuple).copy_constructor_count() == 0u,
            hud::hud::get<0>(tuple).move_constructor_count() == 0u,
            hud::hud::get<0>(tuple).copy_assign_count() == 0u,
            hud::hud::get<0>(tuple).move_assign_count() == 1u,
            hud::hud::get<1>(tuple) == 12.f,
            hud::hud::get<2>(tuple) == 123,
            hud::hud::get<3>(tuple) == L'h',
        };
    };


    // Constant
    {
        const auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
        ASSERT_TRUE(hud::get<4>(result));
        ASSERT_TRUE(hud::get<5>(result));
        ASSERT_TRUE(hud::get<6>(result));
        ASSERT_TRUE(hud::get<7>(result));

    }

    // Non constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
        ASSERT_TRUE(hud::get<4>(result));
        ASSERT_TRUE(hud::get<5>(result));
        ASSERT_TRUE(hud::get<6>(result));
        ASSERT_TRUE(hud::get<7>(result));
    }
}