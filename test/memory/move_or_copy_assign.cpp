#include <core/memory.h>
#include <core/traits/is_trivial.h>
#include <core/traits/is_bitwise_copy_assignable.h>
#include <core/traits/is_bitwise_move_assignable.h>
#include "../misc/leak_guard.h"

GTEST_TEST(memory, move_or_copy_assign_trivial_type)
{

    using type = u32;
    using ResultType = std::tuple<u32, u32>;

    static_assert(hud::is_trivial_v<type>);

    auto test = []() -> ResultType
    {
        type dest;
        type src = 15;
        hud::memory::move_or_copy_assign(&dest, hud::move(src));
        return {dest, src};
    };

    // Non constant
    {
        ResultType result = test();
        hud_assert_eq(std::get<0>(result), 15u);
        hud_assert_eq(std::get<1>(result), 15u);
    }

    // Constant
    {
        constexpr ResultType result = test();
        hud_assert_eq(std::get<0>(result), 15u);
        hud_assert_eq(std::get<1>(result), 15u);
    }
}

GTEST_TEST(memory, move_or_copy_assign_bitwise_assignable_type)
{

    struct c
    {
        i32 i;
    };

    using type = c;
    using ResultType = std::tuple<i32, i32>;

    static_assert(hud::is_bitwise_copy_assignable_v<type>);
    static_assert(hud::is_bitwise_move_assignable_v<type>);

    auto test = []() -> ResultType
    {
        type dest, src;
        dest.i = 0;
        src.i = 15;
        hud::memory::move_or_copy_assign(&dest, hud::move(src));
        return {dest.i, src.i};
    };

    // Non constant
    {
        ResultType result = test();
        hud_assert_eq(std::get<0>(result), 15);
        hud_assert_eq(std::get<1>(result), 15);
    }

    // Constant
    {
        constexpr ResultType result = test();
        hud_assert_eq(std::get<0>(result), 15);
        hud_assert_eq(std::get<1>(result), 15);
    }
}

GTEST_TEST(memory, move_or_copy_assign_non_bitwise_copy_assignable_different_type)
{

    using source_type = hud_test::non_bitwise_copy_assignable_type_3;
    using destination_type = hud_test::non_bitwise_copy_assignable_type_4;
    using ResultType = std::tuple<u32, u32, i32>;

    static_assert(hud::is_copy_assignable_v<destination_type, source_type>);
    static_assert(!hud::is_bitwise_copy_assignable_v<destination_type, source_type>);

    auto test = []() -> ResultType
    {
        i32 assign_counter = 0;
        source_type *src = hud::memory::allocate_array<source_type>(1);
        hud_test::LeakArrayGuard guard_src(src, 1);
        hud::memory::construct_at(src, &assign_counter);
        destination_type *dest = hud::memory::allocate_array<destination_type>(1);
        hud_test::LeakArrayGuard guard_dest(dest, 1);
        hud::memory::construct_at(dest);

        hud::memory::move_or_copy_assign(dest, hud::move(*src));
        return {dest->copy_constructor_count(), dest->copy_assign_count(), dest->copy_order()};
    };

    // Non constant
    {
        ResultType result = test();
        hud_assert_eq(std::get<0>(result), 0u);
        hud_assert_eq(std::get<1>(result), 1u);
        hud_assert_eq(std::get<2>(result), 1);
    }

    // Constant
    {
        constexpr ResultType result = test();
        hud_assert_eq(std::get<0>(result), 0u);
        hud_assert_eq(std::get<1>(result), 1u);
        hud_assert_eq(std::get<2>(result), 1);
    }
}

GTEST_TEST(memory, move_or_copy_assign_non_bitwise_move_assignable_different_type)
{

    using source_type = hud_test::non_bitwise_move_assignable_type3;
    using destination_type = hud_test::non_bitwise_move_assignable_type4;
    using ResultType = std::tuple<u32, u32, u32, u32, i32>;

    static_assert(hud::is_move_assignable_v<destination_type, source_type>);
    static_assert(!hud::is_bitwise_move_assignable_v<destination_type, source_type>);

    auto test = []() -> ResultType
    {
        i32 assign_counter = 0;
        source_type *src = hud::memory::allocate_array<source_type>(1);
        hud_test::LeakArrayGuard guard_src(src, 1);
        hud::memory::construct_at(src, &assign_counter);
        destination_type *dest = hud::memory::allocate_array<destination_type>(1);
        hud_test::LeakArrayGuard guard_dest(dest, 1);
        hud::memory::construct_at(dest);

        hud::memory::move_or_copy_assign(dest, hud::move(*src));
        return {dest->copy_constructor_count(), dest->move_constructor_count(), dest->move_assign_count(), dest->copy_assign_count(), dest->move_order()};
    };

    // Non constant
    {
        ResultType result = test();
        hud_assert_eq(std::get<0>(result), 0u);
        hud_assert_eq(std::get<1>(result), 0u);
        hud_assert_eq(std::get<2>(result), 1u);
        hud_assert_eq(std::get<3>(result), 0u);
        hud_assert_eq(std::get<4>(result), 1);
    }

    // Constant
    {
        constexpr ResultType result = test();
        hud_assert_eq(std::get<0>(result), 0u);
        hud_assert_eq(std::get<1>(result), 0u);
        hud_assert_eq(std::get<2>(result), 1u);
        hud_assert_eq(std::get<3>(result), 0u);
        hud_assert_eq(std::get<4>(result), 1);
    }
}

GTEST_TEST(memory, move_or_copy_assign_non_bitwise_copy_assignable_same_type)
{

    using type = hud_test::non_bitwise_copy_assignable_type_4;
    using ResultType = std::tuple<u32, u32, i32>;

    static_assert(hud::is_copy_constructible_v<type>);
    static_assert(!hud::is_bitwise_copy_constructible_v<type>);

    auto test = []() -> ResultType
    {
        i32 assign_counter = 0;
        type *src = hud::memory::allocate_array<type>(1);
        hud_test::LeakArrayGuard guard_src(src, 1);
        hud::memory::construct_at(src, &assign_counter);
        type *dest = hud::memory::allocate_array<type>(1);
        hud_test::LeakArrayGuard guard_dest(dest, 1);
        hud::memory::construct_at(dest);

        hud::memory::move_or_copy_assign(dest, hud::move(*src));
        return {dest->copy_constructor_count(), dest->copy_assign_count(), dest->copy_order()};
    };

    // Non constant
    {
        ResultType result = test();
        hud_assert_eq(std::get<0>(result), 0u);
        hud_assert_eq(std::get<1>(result), 1u);
        hud_assert_eq(std::get<2>(result), 1);
    }

    // Constant
    {
        constexpr ResultType result = test();
        hud_assert_eq(std::get<0>(result), 0u);
        hud_assert_eq(std::get<1>(result), 1u);
        hud_assert_eq(std::get<2>(result), 1);
    }
}

GTEST_TEST(memory, move_or_copy_assign_array_trivial_type)
{

    using type = u32;
    using ResultType = std::tuple<u32, u32, u32, u32>;

    static_assert(std::is_trivial_v<type>);

    auto test = []() -> ResultType
    {
        type dest[2];
        type src[2] = {15, 32};
        hud::memory::move_or_copy_assign_array(dest, src, src + 2);
        return {dest[0], dest[1], src[0], src[1]};
    };

    // Non constant
    {
        ResultType result = test();
        hud_assert_eq(std::get<0>(result), 15u);
        hud_assert_eq(std::get<1>(result), 32u);
        hud_assert_eq(std::get<2>(result), 15u);
        hud_assert_eq(std::get<3>(result), 32u);
    }

    // Constant
    {
        constexpr ResultType result = test();
        hud_assert_eq(std::get<0>(result), 15u);
        hud_assert_eq(std::get<1>(result), 32u);
        hud_assert_eq(std::get<2>(result), 15u);
        hud_assert_eq(std::get<3>(result), 32u);
    }
}

GTEST_TEST(memory, move_or_copy_assign_array_bitwise_assignable_type)
{

    struct c
    {
        i32 i;
    };

    using type = c;
    using ResultType = std::tuple<i32, i32>;

    static_assert(hud::is_bitwise_copy_assignable_v<type>);
    static_assert(hud::is_bitwise_move_assignable_v<type>);

    auto test = []() -> ResultType
    {
        type dest[2], src[2];
        dest[0].i = 0;
        dest[1].i = 0;
        src[0].i = 15;
        src[1].i = 32;
        hud::memory::move_or_copy_assign_array(dest, src, src + 2);
        return {dest[0].i, dest[1].i};
    };

    // Non constant
    {
        ResultType result = test();
        hud_assert_eq(std::get<0>(result), 15);
        hud_assert_eq(std::get<1>(result), 32);
    }

    // Constant
    {
        constexpr ResultType result = test();
        hud_assert_eq(std::get<0>(result), 15);
        hud_assert_eq(std::get<1>(result), 32);
    }
}

GTEST_TEST(memory, move_or_copy_assign_array_non_bitwise_copy_assignable_different_type)
{

    using source_type = hud_test::non_bitwise_copy_assignable_type_3;
    using destination_type = hud_test::non_bitwise_copy_assignable_type_4;
    using ResultType = std::tuple<u32, u32, i32, u32, u32, i32>;

    static_assert(hud::is_copy_constructible_v<destination_type, source_type>);
    static_assert(!hud::is_bitwise_copy_constructible_v<destination_type, source_type>);
    static_assert(!std::is_same_v<destination_type, source_type>);

    auto test = []() -> ResultType
    {
        i32 assign_counter = 0;
        source_type *src = hud::memory::allocate_array<source_type>(2);
        hud_test::LeakArrayGuard guard_src(src, 2);
        hud::memory::construct_array_at(src, src + 2, &assign_counter);
        destination_type *dest = hud::memory::allocate_array<destination_type>(2);
        hud_test::LeakArrayGuard guard_dest(dest, 2);
        hud::memory::construct_array_at(dest, dest + 2);

        hud::memory::move_or_copy_assign_array(dest, src, src + 2);
        return {dest[0].copy_constructor_count(), dest[0].copy_assign_count(), dest[0].copy_order(), dest[1].copy_constructor_count(), dest[1].copy_assign_count(), dest[1].copy_order()};
    };

    // Non constant
    {
        ResultType result = test();
        hud_assert_eq(std::get<0>(result), 0u);
        hud_assert_eq(std::get<1>(result), 1u);
        hud_assert_eq(std::get<2>(result), 1);
        hud_assert_eq(std::get<3>(result), 0u);
        hud_assert_eq(std::get<4>(result), 1u);
        hud_assert_eq(std::get<5>(result), 2);
    }

    // Constant
    {
        constexpr ResultType result = test();
        hud_assert_eq(std::get<0>(result), 0u);
        hud_assert_eq(std::get<1>(result), 1u);
        hud_assert_eq(std::get<2>(result), 1);
        hud_assert_eq(std::get<3>(result), 0u);
        hud_assert_eq(std::get<4>(result), 1u);
        hud_assert_eq(std::get<5>(result), 2);
    }
}

GTEST_TEST(memory, move_or_copy_assign_array_non_bitwise_move_assignable_different_type)
{

    using source_type = hud_test::non_bitwise_move_assignable_type3;
    using destination_type = hud_test::non_bitwise_move_assignable_type4;
    using ResultType = std::tuple<u32, u32, u32, u32, i32, u32, u32, u32, u32, i32>;

    static_assert(hud::is_move_constructible_v<destination_type, source_type>);
    static_assert(!hud::is_bitwise_move_constructible_v<destination_type, source_type>);
    static_assert(!std::is_same_v<destination_type, source_type>);

    auto test = []() -> ResultType
    {
        i32 assign_counter = 0;
        source_type *src = hud::memory::allocate_array<source_type>(2);
        hud_test::LeakArrayGuard guard_src(src, 2);
        hud::memory::construct_array_at(src, src + 2, &assign_counter);
        destination_type *dest = hud::memory::allocate_array<destination_type>(2);
        hud_test::LeakArrayGuard guard_dest(dest, 2);
        hud::memory::construct_array_at(dest, dest + 2);

        hud::memory::move_or_copy_assign_array(dest, src, src + 2);

        return {dest[0].copy_constructor_count(), dest[0].move_constructor_count(), dest[0].copy_assign_count(), dest[0].move_assign_count(), dest[0].move_order(), dest[1].copy_constructor_count(), dest[1].move_constructor_count(), dest[1].copy_assign_count(), dest[1].move_assign_count(), dest[1].move_order()};
    };

    // Non constant
    {
        ResultType result = test();
        hud_assert_eq(std::get<0>(result), 0u);
        hud_assert_eq(std::get<1>(result), 0u);
        hud_assert_eq(std::get<2>(result), 0u);
        hud_assert_eq(std::get<3>(result), 1u);
        hud_assert_eq(std::get<4>(result), 1);
        hud_assert_eq(std::get<5>(result), 0u);
        hud_assert_eq(std::get<6>(result), 0u);
        hud_assert_eq(std::get<7>(result), 0u);
        hud_assert_eq(std::get<8>(result), 1u);
        hud_assert_eq(std::get<9>(result), 2);
    }

    // Constant
    {
        constexpr ResultType result = test();
        hud_assert_eq(std::get<0>(result), 0u);
        hud_assert_eq(std::get<1>(result), 0u);
        hud_assert_eq(std::get<2>(result), 0u);
        hud_assert_eq(std::get<3>(result), 1u);
        hud_assert_eq(std::get<4>(result), 1);
        hud_assert_eq(std::get<5>(result), 0u);
        hud_assert_eq(std::get<6>(result), 0u);
        hud_assert_eq(std::get<7>(result), 0u);
        hud_assert_eq(std::get<8>(result), 1u);
        hud_assert_eq(std::get<9>(result), 2);
    }
}

GTEST_TEST(memory, move_or_copy_assign_array_non_bitwise_copy_constructible_same_type)
{

    using type = hud_test::non_bitwise_copy_assignable_type_4;
    using ResultType = std::tuple<u32, u32, i32, u32, u32, i32>;

    static_assert(hud::is_copy_constructible_v<type>);
    static_assert(!hud::is_bitwise_copy_constructible_v<type>);

    auto test = []() -> ResultType
    {
        i32 assign_counter = 0;
        type *src = hud::memory::allocate_array<type>(2);
        hud_test::LeakArrayGuard guard_src(src, 2);
        hud::memory::construct_array_at(src, src + 2, &assign_counter);
        type *dest = hud::memory::allocate_array<type>(2);
        hud_test::LeakArrayGuard guard_dest(dest, 2);
        hud::memory::construct_array_at(dest, dest + 2);

        hud::memory::move_or_copy_assign_array(dest, src, src + 2);
        return {dest[0].copy_constructor_count(), dest[0].copy_assign_count(), dest[0].copy_order(), dest[1].copy_constructor_count(), dest[1].copy_assign_count(), dest[1].copy_order()};
    };

    // Non constant
    {
        ResultType result = test();
        hud_assert_eq(std::get<0>(result), 0u);
        hud_assert_eq(std::get<1>(result), 1u);
        hud_assert_eq(std::get<2>(result), 1);
        hud_assert_eq(std::get<3>(result), 0u);
        hud_assert_eq(std::get<4>(result), 1u);
        hud_assert_eq(std::get<5>(result), 2);
    }

    // Constant
    {
        constexpr ResultType result = test();
        hud_assert_eq(std::get<0>(result), 0u);
        hud_assert_eq(std::get<1>(result), 1u);
        hud_assert_eq(std::get<2>(result), 1);
        hud_assert_eq(std::get<3>(result), 0u);
        hud_assert_eq(std::get<4>(result), 1u);
        hud_assert_eq(std::get<5>(result), 2);
    }
}

GTEST_TEST(memory, move_or_copy_assign_array_non_bitwise_move_constructible_same_type)
{

    using type = hud_test::non_bitwise_move_assignable_type4;
    using ResultType = std::tuple<u32, u32, u32, u32, i32, u32, u32, u32, u32, i32>;

    static_assert(hud::is_move_constructible_v<type>);
    static_assert(!hud::is_bitwise_move_constructible_v<type>);

    auto test = []() -> ResultType
    {
        i32 assign_counter = 0;
        type *src = hud::memory::allocate_array<type>(2);
        hud_test::LeakArrayGuard guard_src(src, 2);
        hud::memory::construct_array_at(src, src + 2, &assign_counter);
        type *dest = hud::memory::allocate_array<type>(2);
        hud_test::LeakArrayGuard guard_dest(dest, 2);
        hud::memory::construct_array_at(dest, dest + 2);

        hud::memory::move_or_copy_assign_array(dest, src, src + 2);

        return {dest[0].copy_constructor_count(), dest[0].move_constructor_count(), dest[0].copy_assign_count(), dest[0].move_assign_count(), dest[0].move_order(), dest[1].copy_constructor_count(), dest[1].move_constructor_count(), dest[1].copy_assign_count(), dest[1].move_assign_count(), dest[1].move_order()};
    };

    // Non constant
    {
        ResultType result = test();
        hud_assert_eq(std::get<0>(result), 0u);
        hud_assert_eq(std::get<1>(result), 0u);
        hud_assert_eq(std::get<2>(result), 0u);
        hud_assert_eq(std::get<3>(result), 1u);
        hud_assert_eq(std::get<4>(result), 1);
        hud_assert_eq(std::get<5>(result), 0u);
        hud_assert_eq(std::get<6>(result), 0u);
        hud_assert_eq(std::get<7>(result), 0u);
        hud_assert_eq(std::get<8>(result), 1u);
        hud_assert_eq(std::get<9>(result), 2);
    }

    // Constant
    {
        constexpr ResultType result = test();
        hud_assert_eq(std::get<0>(result), 0u);
        hud_assert_eq(std::get<1>(result), 0u);
        hud_assert_eq(std::get<2>(result), 0u);
        hud_assert_eq(std::get<3>(result), 1u);
        hud_assert_eq(std::get<4>(result), 1);
        hud_assert_eq(std::get<5>(result), 0u);
        hud_assert_eq(std::get<6>(result), 0u);
        hud_assert_eq(std::get<7>(result), 0u);
        hud_assert_eq(std::get<8>(result), 1u);
        hud_assert_eq(std::get<9>(result), 2);
    }
}
