#include <core/memory.h>
#include "../misc/leak_guard.h"

TEST(Memory, construct_at_trival_type)
{


    auto test_no_param = []() -> u32 {
        u32 to_construct;
        hud::memory::construct_at(&to_construct);
        return to_construct;
    }; 
    
    auto test_param = []() -> u32 {
        u32 to_construct;
        hud::memory::construct_at(&to_construct, 15);
        return to_construct;
    };
    // Non constant
    {
        u32 result = test_no_param();
        ASSERT_EQ(result, 0u);
        result = test_param();
        ASSERT_EQ(result, 15u);
    }

    // Constant
    {
        constexpr u32 result_no_param = test_no_param();
        ASSERT_EQ(result_no_param, 0u);
        constexpr u32 result_param = test_param();
        ASSERT_EQ(result_param, 15u);
    }
}

TEST(Memory, construct_at_non_trivially_constructible_type)
{


    struct c {
        i32 value = 0;
        c() = default;
        constexpr c(i32 val) noexcept
            : value(val) {
        }
    };
    using type = c;
    using ResultType = i32;
    static_assert(!hud::is_trivially_constructible_v<type>);
    static_assert(!hud::is_trivially_constructible_v<type, i32>);

    auto test_no_param = []() -> ResultType {
        type* to_construct = hud::memory::allocate_array<type>(1);
        hud::test::LeakArrayGuard guard(to_construct, 1);
        hud::memory::construct_at(to_construct);
        return to_construct->value;
    };

    auto test_param = []() -> ResultType {
        type* to_construct = hud::memory::allocate_array<type>(1);
        hud::test::LeakArrayGuard guard(to_construct, 1);
        hud::memory::construct_at(to_construct, 15);
        return to_construct->value;
    };
    // Non constant
    {
        ResultType result_no_param = test_no_param();
        ASSERT_EQ(result_no_param, 0);
        ResultType result_param = test_param();
        ASSERT_EQ(result_param, 15);
    }

    // Constant
    {
        constexpr ResultType result_no_param = test_no_param();
        ASSERT_EQ(result_no_param, 0);
        constexpr ResultType result_param = test_param();
        ASSERT_EQ(result_param, 15);
    }
}

TEST(Memory, construct_array_at_trival_type)
{


    using type = u32;
    using ResultType = std::tuple<u32, u32>;
    static_assert(hud::is_trivially_constructible_v<type>);
    static_assert(hud::is_trivially_constructible_v<type, u32>);

    auto test_no_param = []() -> ResultType {
        type to_construct[2];
        hud::memory::construct_array_at(to_construct, to_construct + 2);
        return { to_construct[0], to_construct[1] };
    };

    auto test_param = []() -> ResultType {
        type to_construct[2];
        hud::memory::construct_array_at(to_construct, to_construct+2, 15u);
        return { to_construct[0], to_construct[1] };
    };
    // Non constant
    {
        ResultType result_no_param = test_no_param();
        ASSERT_EQ(hud::get<0>(result_no_param), 0u);
        ASSERT_EQ(hud::get<1>(result_no_param), 0u);
        ResultType result_param = test_param();
        ASSERT_EQ(hud::get<0>(result_param), 15u);
        ASSERT_EQ(hud::get<1>(result_param), 15u);
    }

    // Constant
    {
        constexpr ResultType result_no_param = test_no_param();
        ASSERT_EQ(hud::get<0>(result_no_param), 0u);
        ASSERT_EQ(hud::get<1>(result_no_param), 0u);
        constexpr ResultType result_param = test_param();
        ASSERT_EQ(hud::get<0>(result_param), 15u);
        ASSERT_EQ(hud::get<1>(result_param), 15u);
    }
}

TEST(Memory, construct_array_at_non_trival_type)
{


    struct c {
        i32 value = 0;
        c() = default;
        constexpr c(i32 val) noexcept
            : value(val) {
        }
    };
    using type = c;
    using ResultType = std::tuple<i32, i32>;
    static_assert(!hud::is_trivially_constructible_v<type>);
    static_assert(!hud::is_trivially_constructible_v<type, i32>);

    auto test_no_param = []() -> ResultType {
        type* to_construct = hud::memory::allocate_array<type>(2);
        hud::test::LeakArrayGuard guard(to_construct, 2);
        hud::memory::construct_array_at(to_construct, to_construct +2);
        return { to_construct[0].value, to_construct[1].value };
    };
    
    auto test_param = []() -> ResultType {
        type* to_construct = hud::memory::allocate_array<type>(2);
        hud::test::LeakArrayGuard guard(to_construct, 2);
        hud::memory::construct_array_at(to_construct, to_construct+2, 15);
        return { to_construct[0].value, to_construct[1].value };
    };

    // Non constant
    {
        ResultType result_no_param = test_no_param();
        ASSERT_EQ(hud::get<0>(result_no_param), 0);
        ASSERT_EQ(hud::get<1>(result_no_param), 0);
        ResultType result_param = test_param();
        ASSERT_EQ(hud::get<0>(result_param), 15);
        ASSERT_EQ(hud::get<1>(result_param), 15);
    }

    // Constant
    {
        constexpr ResultType result_no_param = test_no_param();
        ASSERT_EQ(hud::get<0>(result_no_param), 0);
        ASSERT_EQ(hud::get<1>(result_no_param), 0);
        constexpr ResultType result_param = test_param();
        ASSERT_EQ(hud::get<0>(result_param), 15);
        ASSERT_EQ(hud::get<1>(result_param), 15);
    }
}