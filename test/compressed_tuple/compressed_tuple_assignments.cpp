#include <core/containers/compressed_tuple.h>

GTEST_TEST(compressed_tuple, copy_assign_trivially_copy_assignable_same_types)
{
    using tuple_type = hud::compressed_tuple<f32, i32, wchar>;

    const auto test = []()
    {
        tuple_type compressed_tuple {hud::tag_init};
        const tuple_type other_tuple {12.0f, 123, L'h'};
        compressed_tuple = other_tuple;
        return std::compressed_tuple {
            hud::get<0>(compressed_tuple) == 12.f,
            hud::get<1>(compressed_tuple) == 123,
            hud::get<2>(compressed_tuple) == L'h',
        };
    };

    // Constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
    }

    // Non constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
    }
}

GTEST_TEST(compressed_tuple, copy_assign_trivially_copy_assignable_different_types)
{
    using tuple_type = hud::compressed_tuple<u64, i32, wchar>;
    using other_tuple_type = hud::compressed_tuple<u16, i16, char16>;

    const auto test = []()
    {
        tuple_type compressed_tuple {hud::tag_init};
        const other_tuple_type other_tuple {(u16)12, (i16)123, u'h'};
        compressed_tuple = other_tuple;
        return std::compressed_tuple {
            hud::get<0>(compressed_tuple) == 12,
            hud::get<1>(compressed_tuple) == 123,
            hud::get<2>(compressed_tuple) == L'h',
        };
    };

    // Constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
    }

    // Non constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
    }
}

GTEST_TEST(compressed_tuple, copy_assign_non_trivially_copy_assignable_same_types)
{
    using tuple_type = hud::compressed_tuple<hud_test::non_bitwise_copy_assignable_type, f32, i32, wchar>;

    const auto test = []()
    {
        tuple_type compressed_tuple {hud::tag_init};
        const tuple_type other_tuple {1, 12.0f, 123, L'h'};
        compressed_tuple = other_tuple;
        return std::compressed_tuple {
            hud::get<0>(compressed_tuple).id() == 1,
            hud::get<0>(compressed_tuple).copy_constructor_count() == 0u,
            hud::get<0>(compressed_tuple).copy_assign_count() == 1u,
            hud::get<1>(compressed_tuple) == 12.f,
            hud::get<2>(compressed_tuple) == 123,
            hud::get<3>(compressed_tuple) == L'h',
        };
    };

    // Constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
    }

    // Non constant
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

GTEST_TEST(compressed_tuple, copy_assign_non_trivially_copy_assignable_different_types)
{
    using tuple_type = hud::compressed_tuple<hud_test::non_bitwise_copy_assignable_type_2, f32, i32, wchar>;
    using other_tuple_type = hud::compressed_tuple<hud_test::non_bitwise_copy_assignable_type, u16, i16, char16>;

    const auto test = []()
    {
        tuple_type compressed_tuple {hud::tag_init};
        const other_tuple_type other_tuple {1, (u16)12, (i16)123, u'h'};
        compressed_tuple = other_tuple;
        return std::compressed_tuple {
            hud::get<0>(compressed_tuple).id() == 1,
            hud::get<0>(compressed_tuple).copy_constructor_count() == 0u,
            hud::get<0>(compressed_tuple).copy_assign_count() == 1u,
            hud::get<1>(compressed_tuple) == 12.f,
            hud::get<2>(compressed_tuple) == 123,
            hud::get<3>(compressed_tuple) == L'h',
        };
    };

    // Constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
    }

    // Non constant
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

GTEST_TEST(compressed_tuple, move_assign_trivially_copy_assignable_same_types)
{
    using tuple_type = hud::compressed_tuple<f32, i32, wchar>;

    const auto test = []()
    {
        tuple_type compressed_tuple {hud::tag_init};
        tuple_type other_tuple {12.0f, 123, L'h'};
        compressed_tuple = hud::move(other_tuple);
        return std::compressed_tuple {
            hud::get<0>(compressed_tuple) == 12.f,
            hud::get<1>(compressed_tuple) == 123,
            hud::get<2>(compressed_tuple) == L'h',
        };
    };

    // Constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
    }

    // Non constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
    }
}

GTEST_TEST(compressed_tuple, move_assign_trivially_copy_assignable_different_types)
{
    using tuple_type = hud::compressed_tuple<u64, i32, wchar>;
    using other_tuple_type = hud::compressed_tuple<u16, i16, char16>;

    const auto test = []()
    {
        tuple_type compressed_tuple {hud::tag_init};
        other_tuple_type other_tuple {(u16)12, (i16)123, u'h'};
        compressed_tuple = hud::move(other_tuple);
        return std::compressed_tuple {
            hud::get<0>(compressed_tuple) == 12,
            hud::get<1>(compressed_tuple) == 123,
            hud::get<2>(compressed_tuple) == L'h',
        };
    };

    // Constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
    }

    // Non constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
    }
}

GTEST_TEST(compressed_tuple, move_assign_non_trivially_copy_assignable_same_types)
{
    using tuple_type = hud::compressed_tuple<hud_test::non_bitwise_copy_assignable_type, f32, i32, wchar>;

    const auto test = []()
    {
        tuple_type compressed_tuple {hud::tag_init};
        tuple_type other_tuple {1, 12.0f, 123, L'h'};
        compressed_tuple = hud::move(other_tuple);
        return std::compressed_tuple {
            hud::get<0>(compressed_tuple).id() == 1,
            hud::get<0>(compressed_tuple).copy_constructor_count() == 0u,
            hud::get<0>(compressed_tuple).copy_assign_count() == 1u,
            hud::get<1>(compressed_tuple) == 12.f,
            hud::get<2>(compressed_tuple) == 123,
            hud::get<3>(compressed_tuple) == L'h',
        };
    };

    // Constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
    }

    // Non constant
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

GTEST_TEST(compressed_tuple, move_assign_non_trivially_copy_assignable_different_types)
{
    using tuple_type = hud::compressed_tuple<hud_test::non_bitwise_copy_assignable_type_2, f32, i32, wchar>;
    using other_tuple_type = hud::compressed_tuple<hud_test::non_bitwise_copy_assignable_type, u16, i16, char16>;

    const auto test = []()
    {
        tuple_type compressed_tuple {hud::tag_init};
        other_tuple_type other_tuple {1, (u16)12, (i16)123, u'h'};
        compressed_tuple = hud::move(other_tuple);
        return std::compressed_tuple {
            hud::get<0>(compressed_tuple).id() == 1,
            hud::get<0>(compressed_tuple).copy_constructor_count() == 0u,
            hud::get<0>(compressed_tuple).copy_assign_count() == 1u,
            hud::get<1>(compressed_tuple) == 12.f,
            hud::get<2>(compressed_tuple) == 123,
            hud::get<3>(compressed_tuple) == L'h',
        };
    };

    // Constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
    }

    // Non constant
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

GTEST_TEST(compressed_tuple, move_assign_trivially_move_assignable_same_types)
{
    using tuple_type = hud::compressed_tuple<f32, i32, wchar>;

    const auto test = []()
    {
        tuple_type compressed_tuple {hud::tag_init};
        tuple_type other_tuple {12.0f, 123, L'h'};
        compressed_tuple = hud::move(other_tuple);
        return std::compressed_tuple {
            hud::get<0>(compressed_tuple) == 12.f,
            hud::get<1>(compressed_tuple) == 123,
            hud::get<2>(compressed_tuple) == L'h',
        };
    };

    // Constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
    }

    // Non constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
    }
}

GTEST_TEST(compressed_tuple, move_assign_trivially_move_assignable_different_types)
{
    using tuple_type = hud::compressed_tuple<u64, i32, wchar>;
    using other_tuple_type = hud::compressed_tuple<u16, i16, char16>;

    const auto test = []()
    {
        tuple_type compressed_tuple {hud::tag_init};
        other_tuple_type other_tuple {(u16)12, (i16)123, u'h'};
        compressed_tuple = hud::move(other_tuple);
        return std::compressed_tuple {
            hud::get<0>(compressed_tuple) == 12,
            hud::get<1>(compressed_tuple) == 123,
            hud::get<2>(compressed_tuple) == L'h',
        };
    };

    // Constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
    }

    // Non constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
    }
}

GTEST_TEST(compressed_tuple, move_assign_non_trivially_move_assignable_same_types)
{
    using tuple_type = hud::compressed_tuple<hud_test::non_bitwise_move_assignable_type, f32, i32, wchar>;

    const auto test = []()
    {
        tuple_type compressed_tuple {hud::tag_init};
        tuple_type other_tuple {1, 12.0f, 123, L'h'};
        compressed_tuple = hud::move(other_tuple);
        return std::compressed_tuple {
            hud::get<0>(compressed_tuple).id() == 1,
            hud::get<0>(compressed_tuple).copy_constructor_count() == 0u,
            hud::get<0>(compressed_tuple).move_constructor_count() == 0u,
            hud::get<0>(compressed_tuple).copy_assign_count() == 0u,
            hud::get<0>(compressed_tuple).move_assign_count() == 1u,
            hud::get<1>(compressed_tuple) == 12.f,
            hud::get<2>(compressed_tuple) == 123,
            hud::get<3>(compressed_tuple) == L'h',
        };
    };

    // Constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_true(std::get<6>(result));
        hud_assert_true(std::get<7>(result));
    }

    // Non constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_true(std::get<6>(result));
        hud_assert_true(std::get<7>(result));
    }
}

GTEST_TEST(compressed_tuple, move_assign_non_trivially_move_assignable_different_types)
{
    using tuple_type = hud::compressed_tuple<hud_test::non_bitwise_move_assignable_type2, f32, i32, wchar>;
    using other_tuple_type = hud::compressed_tuple<hud_test::non_bitwise_move_assignable_type, u16, i16, char16>;

    const auto test = []()
    {
        tuple_type compressed_tuple {hud::tag_init};
        other_tuple_type other_tuple {1, (u16)12, (i16)123, u'h'};
        compressed_tuple = hud::move(other_tuple);
        return std::compressed_tuple {
            hud::get<0>(compressed_tuple).id() == 1,
            hud::get<0>(compressed_tuple).copy_constructor_count() == 0u,
            hud::get<0>(compressed_tuple).move_constructor_count() == 0u,
            hud::get<0>(compressed_tuple).copy_assign_count() == 0u,
            hud::get<0>(compressed_tuple).move_assign_count() == 1u,
            hud::get<1>(compressed_tuple) == 12.f,
            hud::get<2>(compressed_tuple) == 123,
            hud::get<3>(compressed_tuple) == L'h',
        };
    };

    // Constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_true(std::get<6>(result));
        hud_assert_true(std::get<7>(result));
    }

    // Non constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_true(std::get<6>(result));
        hud_assert_true(std::get<7>(result));
    }
}