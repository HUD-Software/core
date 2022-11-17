#include <core/memory.h>
#include "../misc/leak_guard.h"

GTEST_TEST(memory, copy_assign_array_trivial_type)
{


    using type = u32;
    using ResultType = std::tuple<u32, u32, u32, u32>;

    static_assert(std::is_trivial_v<type>);

    auto test = []() -> ResultType {
        type dest[2];
        type src[2] = { 15, 32 };
        hud::memory::copy_assign_array(dest, src, 2);
        return { dest[0], dest[1], src[0], src[1] };
    };

    // Non constant
    {
        ResultType result = test();
        GTEST_ASSERT_EQ(std::get<0>(result), 15u);
        GTEST_ASSERT_EQ(std::get<1>(result), 32u);
        GTEST_ASSERT_EQ(std::get<2>(result), 15u);
        GTEST_ASSERT_EQ(std::get<3>(result), 32u);
    }

    // Constant
    {
        constexpr ResultType result = test();
        GTEST_ASSERT_EQ(std::get<0>(result), 15u);
        GTEST_ASSERT_EQ(std::get<1>(result), 32u);
        GTEST_ASSERT_EQ(std::get<2>(result), 15u);
        GTEST_ASSERT_EQ(std::get<3>(result), 32u);
    }
}

GTEST_TEST(memory, copy_assign_array_bitwise_assignable_type)
{


    struct c {
        i32 i;
    };

    using type = c;
    using ResultType = std::tuple<i32, i32, i32, i32>;

    static_assert(hud::is_bitwise_copy_constructible_v<c>);
    static_assert(hud::is_bitwise_move_constructible_v<c>);

    auto test = []() -> ResultType {
        type dest[2], src[2];
        dest[0].i = 0;
        dest[1].i = 0;
        src[0].i = 15;
        src[1].i = 32;
        hud::memory::copy_assign_array(dest, src, 2);
        return { dest[0].i, dest[1].i, src[0].i, src[1].i };
    };

    // Non constant
    {
        ResultType result = test();
        GTEST_ASSERT_EQ(std::get<0>(result), 15);
        GTEST_ASSERT_EQ(std::get<1>(result), 32);
        GTEST_ASSERT_EQ(std::get<2>(result), 15);
        GTEST_ASSERT_EQ(std::get<3>(result), 32);
    }

    // Constant
    {
        constexpr ResultType result = test();
        GTEST_ASSERT_EQ(std::get<0>(result), 15);
        GTEST_ASSERT_EQ(std::get<1>(result), 32);
        GTEST_ASSERT_EQ(std::get<2>(result), 15);
        GTEST_ASSERT_EQ(std::get<3>(result), 32);
    }
}

GTEST_TEST(memory, copy_assign_array_non_bitwise_copy_assignable_different_type)
{


    using source_type = hud_test::non_bitwise_copy_assignable_type_3;
    using destination_type = hud_test::non_bitwise_copy_assignable_type_4;
    using ResultType = std::tuple<u32, u32, i32, u32, u32, i32>;

    static_assert(hud::is_copy_constructible_v<destination_type, source_type>);
    static_assert(!hud::is_bitwise_copy_constructible_v<destination_type, source_type>);
    static_assert(!std::is_same_v<destination_type, source_type>);

    auto test = []() -> ResultType {
        i32 assign_counter = 0;
        source_type* src = hud::memory::allocate_array<source_type>(2);
        hud_test::LeakArrayGuard src_guard(src, 2);
        hud::memory::construct_array_at(src, src+2, &assign_counter);

        destination_type* dest = hud::memory::allocate_array<destination_type>(2);
        hud_test::LeakArrayGuard dest_guard(dest, 2);
        hud::memory::construct_array_at(dest, dest+2);

        hud::memory::copy_assign_array(dest, src, 2);
        return { dest[0].copy_constructor_count(), 
                 dest[0].copy_assign_count(),
                 dest[0].copy_order(),
                 dest[1].copy_constructor_count(),
                 dest[1].copy_assign_count(),
                 dest[1].copy_order() };
    };

    // Non constant
    {
        ResultType result = test();
        GTEST_ASSERT_EQ(std::get<0>(result), 0u);
        GTEST_ASSERT_EQ(std::get<1>(result), 1u);
        GTEST_ASSERT_EQ(std::get<2>(result), 1);
        GTEST_ASSERT_EQ(std::get<3>(result), 0u);
        GTEST_ASSERT_EQ(std::get<4>(result), 1u);
        GTEST_ASSERT_EQ(std::get<5>(result), 2);
    }

    // Constant
    {
        constexpr ResultType result = test();
        GTEST_ASSERT_EQ(std::get<0>(result), 0u);
        GTEST_ASSERT_EQ(std::get<1>(result), 1u);
        GTEST_ASSERT_EQ(std::get<2>(result), 1);
        GTEST_ASSERT_EQ(std::get<3>(result), 0u);
        GTEST_ASSERT_EQ(std::get<4>(result), 1u);
        GTEST_ASSERT_EQ(std::get<5>(result), 2);
    }
}

GTEST_TEST(memory, copy_assign_array_non_bitwise_copy_assignable_same_type)
{


    using type = hud_test::non_bitwise_copy_assignable_type_4;
    using ResultType = std::tuple<u32, u32, i32, u32, u32, i32>;
    
    static_assert(hud::is_copy_assignable_v<type>);
    static_assert(!hud::is_bitwise_copy_assignable_v<type>);

    auto test = []() -> ResultType {
        i32 assign_counter = 0;
        type* src = hud::memory::allocate_array<type>(2);
        hud_test::LeakArrayGuard guard_src(src, 2);
        hud::memory::construct_array_at(src, src+2, & assign_counter);
        type* dest = hud::memory::allocate_array<type>(2);
        hud_test::LeakArrayGuard guard_dest(dest, 2);
        hud::memory::construct_array_at(dest, dest+2);

        hud::memory::copy_assign_array(dest, src, 2);

        return { dest[0].copy_constructor_count(),
                 dest[0].copy_assign_count(),
                 dest[0].copy_order(),
                 dest[1].copy_constructor_count(),
                 dest[1].copy_assign_count(),
                 dest[1].copy_order() };
    };

    // Non constant
    {
        ResultType result = test();
        GTEST_ASSERT_EQ(std::get<0>(result), 0u);
        GTEST_ASSERT_EQ(std::get<1>(result), 1u);
        GTEST_ASSERT_EQ(std::get<2>(result), 1);
        GTEST_ASSERT_EQ(std::get<3>(result), 0u);
        GTEST_ASSERT_EQ(std::get<4>(result), 1u);
        GTEST_ASSERT_EQ(std::get<5>(result), 2);
    }

    // Constant
    {
        constexpr ResultType result = test();
        GTEST_ASSERT_EQ(std::get<0>(result), 0u);
        GTEST_ASSERT_EQ(std::get<1>(result), 1u);
        GTEST_ASSERT_EQ(std::get<2>(result), 1);
        GTEST_ASSERT_EQ(std::get<3>(result), 0u);
        GTEST_ASSERT_EQ(std::get<4>(result), 1u);
        GTEST_ASSERT_EQ(std::get<5>(result), 2);
    }
}
