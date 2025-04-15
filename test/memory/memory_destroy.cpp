#include <core/memory.h>
#include "../misc/leak_guard.h"

GTEST_TEST(memory, destroy_trivial_type)
{

    using type = u32;
    using ResultType = u32;

    static_assert(std::is_trivial_v<type>);

    auto test = []() -> ResultType
    {
        type to_destroy = 15;
        hud::memory::destroy_object(&to_destroy);
        return to_destroy;
    };

    // Non constant
    {
        ResultType result = test();
        hud_assert_eq(result, 15u);
    }

    // Constant
    {
        constexpr ResultType result = test();
        hud_assert_eq(result, 15u);
    }
}

GTEST_TEST(memory, destroy_trivially_destructible_type)
{

    struct c
    {
        i32 i;
    };

    using type = c;
    using ResultType = i32;

    static_assert(std::is_trivially_destructible_v<type>);

    auto test = []() -> ResultType
    {
        type to_destroy;
        to_destroy.i = 15;
        hud::memory::destroy_object(&to_destroy);
        return to_destroy.i;
    };

    // Non constant
    {
        ResultType result = test();
        hud_assert_eq(result, 15);
    }

    // Constant
    {
        constexpr ResultType result = test();
        hud_assert_eq(result, 15);
    }
}

GTEST_TEST(memory, destroy_non_trivially_destructible_type)
{

    using type = hud_test::non_bitwise_type;
    using ResultType = i32;

    static_assert(!std::is_trivially_destructible_v<type>);

    auto test = []() -> ResultType
    {
        i32 is_destructor_called = 0;
        type *to_destroy = hud::memory::allocate_array<type>(1);
        hud_test::LeakArrayGuard guard(to_destroy, 1);
        hud::memory::construct_object_at(to_destroy, &is_destructor_called);
        hud::memory::destroy_object(to_destroy);
        return is_destructor_called;
    };

    // Non constant
    {
        ResultType result = test();
        hud_assert_eq(result, 1);
    }

    // Constant
    {
        constexpr ResultType result = test();
        hud_assert_eq(result, 1);
    }
}

GTEST_TEST(memory, destroy_array_trivial_type)
{

    using type = u32;
    using ResultType = std::tuple<u32, u32>;

    static_assert(std::is_trivially_destructible_v<type>);

    auto test = []() -> ResultType
    {
        type to_destroy[2] = {15, 30};
        hud::memory::destroy_object_array(to_destroy, 2);
        return {to_destroy[0], to_destroy[1]};
    };

    // Non constant
    {
        ResultType result = test();
        hud_assert_eq(std::get<0>(result), 15u);
        hud_assert_eq(std::get<1>(result), 30u);
    }

    // Constant
    {
        constexpr ResultType result = test();
        hud_assert_eq(std::get<0>(result), 15u);
        hud_assert_eq(std::get<1>(result), 30u);
    }
}

GTEST_TEST(memory, destroy_array_trivially_destructible_type)
{

    struct c
    {
        i32 i;
    };

    using type = c;
    using ResultType = std::tuple<i32, i32>;
    static_assert(std::is_trivially_destructible_v<type>);

    auto test = []() -> ResultType
    {
        type to_destroy[2];
        to_destroy[0].i = 15;
        to_destroy[1].i = 30;
        hud::memory::destroy_object_array(to_destroy, 2);
        return {to_destroy[0].i, to_destroy[1].i};
    };

    // Non constant
    {
        ResultType result = test();
        hud_assert_eq(std::get<0>(result), 15);
        hud_assert_eq(std::get<1>(result), 30);
    }

    // Constant
    {
        constexpr ResultType result = test();
        hud_assert_eq(std::get<0>(result), 15);
        hud_assert_eq(std::get<1>(result), 30);
    }
}

GTEST_TEST(memory, destroy_array_non_trivially_destructible_type)
{

    using type = hud_test::SetBoolToTrueWhenDestroyed;
    using ResultType = std::tuple<i32, i32>;

    static_assert(!std::is_trivially_destructible_v<type>);

    auto test = []() -> ResultType
    {
        i32 value[2] = {0, 0};
        type *to_destroy = hud::memory::allocate_array<type>(2);
        hud_test::LeakArrayGuard guard(to_destroy, 2);
        hud::memory::construct_array_at(to_destroy, to_destroy + 2);
        to_destroy[0].set_ptr(value);
        to_destroy[1].set_ptr(value + 1);
        hud::memory::destroy_object_array(to_destroy, 2);
        return {value[0], value[1]};
    };

    // Non constant
    {
        ResultType result = test();
        hud_assert_eq(std::get<0>(result), 1);
        hud_assert_eq(std::get<1>(result), 1);
    }

    // Constant
    {
        constexpr ResultType result = test();
        hud_assert_eq(std::get<0>(result), 1);
        hud_assert_eq(std::get<1>(result), 1);
    }
}