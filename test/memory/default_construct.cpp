#include <core/memory.h>
#include "../misc/leak_guard.h"

GTEST_TEST(memory, default_construct_trivial_type)
{

    using type = u32;
    using ResultType = u32;
    static_assert(std::is_trivial_v<type>);

    auto test = []() -> ResultType
    {
        type to_construct;
        hud::memory::default_construct(&to_construct);
        return to_construct;
    };

    // Non constant
    {
        ResultType result = test();
        GTEST_ASSERT_EQ(result, 0u);
    }

    // Constant
    {
        constexpr ResultType result = test();
        GTEST_ASSERT_EQ(result, 0u);
    }
}

GTEST_TEST(memory, default_construct_trivial_constructible_type)
{

    struct c
    {
        i32 value;
    };
    using type = c;
    using ResultType = i32;

    static_assert(std::is_trivial_v<type>);

    auto test = []() -> ResultType
    {
        type *to_construct = hud::memory::allocate_array<type>(1);
        hud_test::LeakArrayGuard guard(to_construct, 1);
        hud::memory::default_construct(to_construct);
        return to_construct->value;
    };

    // Non constant
    {
        ResultType result = test();
        GTEST_ASSERT_EQ(result, 0);
    }

    // Constant
    {
        constexpr ResultType result = test();
        GTEST_ASSERT_EQ(result, 0);
    }
}

GTEST_TEST(memory, default_construct_non_trivial_constructible_type)
{

    struct c
    {
        i32 value = 15;
    };
    using type = c;
    using ResultType = i32;

    static_assert(!std::is_trivial_v<type>);

    auto test = []() -> ResultType
    {
        type *to_construct = hud::memory::allocate_array<type>(1);
        hud_test::LeakArrayGuard guard(to_construct, 1);
        hud::memory::default_construct(to_construct);
        return to_construct->value;
    };

    // Non constant
    {
        ResultType result = test();
        GTEST_ASSERT_EQ(result, 15);
    }

    // Constant
    {
        constexpr ResultType result = test();
        GTEST_ASSERT_EQ(result, 15);
    }
}

GTEST_TEST(memory, default_construct_array_trivial_type)
{

    using type = u32;
    using ResultType = std::tuple<u32, u32>;

    static_assert(std::is_trivial_v<type>);

    auto test = []() -> ResultType
    {
        type to_construct[2];
        hud::memory::default_construct_array(to_construct, to_construct + 2);
        return {to_construct[0], to_construct[1]};
    };

    // Non constant
    {
        ResultType result = test();
        GTEST_ASSERT_EQ(std::get<0>(result), 0u);
        GTEST_ASSERT_EQ(std::get<0>(result), 0u);
    }

    // Constant
    {
        constexpr ResultType result = test();
        GTEST_ASSERT_EQ(std::get<0>(result), 0u);
        GTEST_ASSERT_EQ(std::get<0>(result), 0u);
    }
}

GTEST_TEST(memory, default_construct_array_trivial_constructible_type)
{

    struct c
    {
        i32 value;
    };
    using type = c;
    using ResultType = std::tuple<i32, i32>;

    static_assert(std::is_trivial_v<type>);

    auto test = []() -> ResultType
    {
        type *to_construct = hud::memory::allocate_array<type>(2);
        hud_test::LeakArrayGuard guard(to_construct, 2);
        hud::memory::default_construct_array(to_construct, to_construct + 2);
        return {to_construct[0].value, to_construct[1].value};
    };

    // Non constant
    {
        ResultType result = test();
        GTEST_ASSERT_EQ(std::get<0>(result), 0);
        GTEST_ASSERT_EQ(std::get<0>(result), 0);
    }

    // Constant
    {
        constexpr ResultType result = test();
        GTEST_ASSERT_EQ(std::get<0>(result), 0);
        GTEST_ASSERT_EQ(std::get<0>(result), 0);
    }
}

GTEST_TEST(memory, default_construct_array_non_trivial_constructible_type)
{

    struct c
    {
        i32 value = 15;
    };
    using type = c;
    using ResultType = std::tuple<i32, i32>;

    static_assert(!std::is_trivial_v<type>);

    auto test = []() -> ResultType
    {
        type *to_construct = hud::memory::allocate_array<type>(2);
        hud_test::LeakArrayGuard guard(to_construct, 2);
        hud::memory::default_construct_array(to_construct, to_construct + 2);
        return {to_construct[0].value, to_construct[1].value};
    };

    // Non constant
    {
        ResultType result = test();
        GTEST_ASSERT_EQ(std::get<0>(result), 15);
        GTEST_ASSERT_EQ(std::get<0>(result), 15);
    }

    // Constant
    {
        constexpr ResultType result = test();
        GTEST_ASSERT_EQ(std::get<0>(result), 15);
        GTEST_ASSERT_EQ(std::get<0>(result), 15);
    }
}