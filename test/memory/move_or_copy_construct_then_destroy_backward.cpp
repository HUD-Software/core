#include <core/memory.h>
#include "../misc/leak_guard.h"

TEST(Memory, move_or_copy_construct_array_then_destroy_backward_trivial_type)
{


    using type = u32;
    using ResultType = std::tuple<u32, u32, u32, u32>;

    static_assert(std::is_trivially_copy_constructible_v<type>);
    static_assert(std::is_trivially_destructible_v<type>);

    auto test = []() -> ResultType {
        type dest[2];
        type src[2] = { 15, 32 };
        hud::memory::move_or_copy_construct_array_then_destroy_backward(dest, src, 2);
        return { dest[0], dest[1], src[0], src[1] };
    };

    // Non constant
    {
        ResultType result = test();
        ASSERT_EQ(hud::get<0>(result), 15u);
        ASSERT_EQ(hud::get<1>(result), 32u);
        ASSERT_EQ(hud::get<2>(result), 15u);
        ASSERT_EQ(hud::get<3>(result), 32u);
    }

    // Constant
    {
        constexpr ResultType result = test();
        ASSERT_EQ(hud::get<0>(result), 15u);
        ASSERT_EQ(hud::get<1>(result), 32u);
        ASSERT_EQ(hud::get<2>(result), 15u);
        ASSERT_EQ(hud::get<3>(result), 32u);
    }
}

TEST(Memory, move_or_copy_construct_array_then_destroy_backward_bitwise_constructible_and_destructible_type)
{


    struct c {
        i32 i;
    };
    using type = c;
    using ResultType = std::tuple<i32, i32, i32, i32>;

    static_assert(hud::is_bitwise_copy_constructible_v<c>);
    static_assert(hud::is_bitwise_move_constructible_v<c>);
    static_assert(hud::is_trivially_destructible_v<c>);

    auto test = []() -> ResultType {
        type dest[2], src[2];
        dest[0].i = 0;
        dest[1].i = 0;
        src[0].i = 15;
        src[1].i = 32;
        hud::memory::move_or_copy_construct_array_then_destroy_backward(dest,src, 2);
        return { dest[0].i, dest[1].i, src[0].i, src[1].i };
    };

    // Non constant
    {
        ResultType result = test();
        ASSERT_EQ(hud::get<0>(result), 15);
        ASSERT_EQ(hud::get<1>(result), 32);
        ASSERT_EQ(hud::get<2>(result), 15);
        ASSERT_EQ(hud::get<3>(result), 32);
    }

    // Constant
    {
        constexpr ResultType result = test();
        ASSERT_EQ(hud::get<0>(result), 15);
        ASSERT_EQ(hud::get<1>(result), 32);
        ASSERT_EQ(hud::get<2>(result), 15);
        ASSERT_EQ(hud::get<3>(result), 32);
    }
}

TEST(Memory, move_or_copy_construct_array_then_destroy_backward_non_bitwise_copy_constructible_and_destructible_different_type)
{


    struct SourceType : public hud::test::NonBitwiseCopyConstructibleType3, hud::test::SetBoolToTrueWhenDestroyed {
        constexpr SourceType(i32* ctor_order, i32* dtor_order) noexcept
            : hud::test::NonBitwiseCopyConstructibleType3(ctor_order)
            , hud::test::SetBoolToTrueWhenDestroyed(dtor_order) {
        }

        constexpr ~SourceType() = default;
    };

    struct DestinationType : public hud::test::NonBitwiseCopyConstructibleType4 {
        constexpr DestinationType(const SourceType& other) noexcept
            : hud::test::NonBitwiseCopyConstructibleType4(other) {
        }
    };

    using ResultType = std::tuple<u32, i32, u32, i32>;

    static_assert(hud::is_copy_constructible_v<DestinationType, SourceType>);
    static_assert(!hud::is_bitwise_copy_constructible_v<DestinationType, SourceType>);

    auto test = []() -> ResultType {
        i32 ctor_order = 0;
        i32 dtor_order = 0;

        SourceType* src = hud::memory::allocate_array<SourceType>(2);
        hud::test::LeakArrayGuard guard_src(src, 2);
        hud::memory::construct_array_at(src, src+2, & ctor_order, & dtor_order);

        DestinationType* dest = hud::memory::allocate_array<DestinationType>(2);
        hud::test::LeakArrayGuard guard_dest(dest, 2);

        hud::memory::move_or_copy_construct_array_then_destroy_backward(dest, src, 2);
        return { dest[0].copy_constructor_count(),
                 dest[0].ctor_order(),
                 dest[1].copy_constructor_count(),
                 dest[1].ctor_order() };
    };

    // Non constant
    {
        ResultType result = test();
        ASSERT_EQ(hud::get<0>(result), 1u);
        ASSERT_EQ(hud::get<1>(result), 2);
        ASSERT_EQ(hud::get<2>(result), 1u);
        ASSERT_EQ(hud::get<3>(result), 1);
    }

    // Constant
    {
        constexpr ResultType result = test();
        ASSERT_EQ(hud::get<0>(result), 1u);
        ASSERT_EQ(hud::get<1>(result), 2);
        ASSERT_EQ(hud::get<2>(result), 1u);
        ASSERT_EQ(hud::get<3>(result), 1);
    }
}

TEST(Memory, move_or_copy_construct_array_then_destroy_backward_non_bitwise_move_constructible_and_destructible_different_type)
{


    struct SourceType : public hud::test::NonBitwiseMoveConstructibleType3, hud::test::SetBoolToTrueWhenDestroyed {
        constexpr SourceType(i32* ctor_order, i32* dtor_order) noexcept
            : hud::test::NonBitwiseMoveConstructibleType3(ctor_order)
            , hud::test::SetBoolToTrueWhenDestroyed(dtor_order) {
        }

        constexpr ~SourceType() = default;
    };

    struct DestinationType : public hud::test::NonBitwiseMoveConstructibleType4 {
        constexpr DestinationType(SourceType&& other) noexcept
            : hud::test::NonBitwiseMoveConstructibleType4(std::forward<hud::test::NonBitwiseMoveConstructibleType3>(other)) {
        }

        // MSVC call copy constructor instead of move constructor 
        // https://developercommunity.visualstudio.com/t/constexpr-stdconstruct-at-do-not-works/1545985
        constexpr DestinationType(const SourceType& other) noexcept
            : hud::test::NonBitwiseMoveConstructibleType4(other) {
        }
    };

    using ResultType = std::tuple<u32, u32, i32, u32, u32, i32>;
    static_assert(hud::is_move_constructible_v<DestinationType, SourceType>);
    static_assert(!hud::is_bitwise_move_constructible_v<DestinationType, SourceType>);
    static_assert(hud::is_destructible_v<SourceType>);
    static_assert(!hud::is_trivially_destructible_v<SourceType>);

    auto test = []() -> ResultType {
        i32 ctor_order = 0;
        i32 dtor_order = 0;

        SourceType* src = hud::memory::allocate_array<SourceType>(2);
        hud::test::LeakArrayGuard guard_src(src, 2);
        hud::memory::construct_array_at(src, src+2, & ctor_order, & dtor_order);

        DestinationType* dest = hud::memory::allocate_array<DestinationType>(2);
        hud::test::LeakArrayGuard guard_dest(dest, 2);

        hud::memory::move_or_copy_construct_array_then_destroy_backward(dest, src, 2);
        return { dest[0].move_constructor_count(),
                 dest[0].copy_constructor_count(),
                 dest[0].ctor_order(),
                 dest[1].move_constructor_count(),
                 dest[1].copy_constructor_count(),
                 dest[1].ctor_order() };
    };

    // Non constant
    {
        ResultType result = test();
        ASSERT_EQ(hud::get<0>(result), 1u);
        ASSERT_EQ(hud::get<1>(result), 0u);
        ASSERT_EQ(hud::get<2>(result), 2);
        ASSERT_EQ(hud::get<3>(result), 1u);
        ASSERT_EQ(hud::get<4>(result), 0u);
        ASSERT_EQ(hud::get<5>(result), 1);
    }

    // Constant
    {
        constexpr ResultType result = test();

        // MSVC call copy constructor instead of move constructor 
        // https://developercommunity.visualstudio.com/t/constexpr-stdconstruct-at-do-not-works/1545985
        if constexpr (hud::compilation::is_compiler(hud::compiler_e::msvc)) {
            ASSERT_EQ(hud::get<0>(result), 0u);
            ASSERT_EQ(hud::get<1>(result), 1u);
        }
        else {
            ASSERT_EQ(hud::get<0>(result), 1u);
            ASSERT_EQ(hud::get<1>(result), 0u);
        }
        ASSERT_EQ(hud::get<2>(result), 2);

        // MSVC call copy constructor instead of move constructor 
        // https://developercommunity.visualstudio.com/t/constexpr-stdconstruct-at-do-not-works/1545985
        if constexpr (hud::compilation::is_compiler(hud::compiler_e::msvc)) {
            ASSERT_EQ(hud::get<3>(result), 0u);
            ASSERT_EQ(hud::get<4>(result), 1u);
        }
        else {
            ASSERT_EQ(hud::get<3>(result), 1u);
            ASSERT_EQ(hud::get<4>(result), 0u);
        }
        ASSERT_EQ(hud::get<5>(result), 1);
    }
}

TEST(Memory, move_or_copy_construct_array_then_destroy_backward_non_bitwise_copy_constructible_and_destructible_same_type)
{


    struct type : public hud::test::NonBitwiseCopyConstructibleType4, hud::test::SetBoolToTrueWhenDestroyed {
        constexpr type(i32* ctor_order, i32* dtor_order) noexcept
            : hud::test::NonBitwiseCopyConstructibleType4(ctor_order)
            , hud::test::SetBoolToTrueWhenDestroyed(dtor_order) {
        }

        constexpr ~type() = default;
    };

    using ResultType = std::tuple<u32, i32, u32, i32>;
    static_assert(hud::is_copy_constructible_v<type>);
    static_assert(!hud::is_bitwise_copy_constructible_v<type>);
    static_assert(hud::is_destructible_v<type>);
    static_assert(!hud::is_trivially_destructible_v<type>);


    auto test = []() -> ResultType {
        i32 ctor_order = 0;
        i32 dtor_order = 0;

        type* src = hud::memory::allocate_array<type>(2);
        hud::test::LeakArrayGuard guard_src(src, 2);
        hud::memory::construct_array_at(src, src+ 2, & ctor_order, & dtor_order);

        type* dest = hud::memory::allocate_array<type>(2);
        hud::test::LeakArrayGuard guard_dest(dest, 2);

        hud::memory::move_or_copy_construct_array_then_destroy_backward(dest, src, 2);
        return { dest[0].copy_constructor_count(),
                 dest[0].ctor_order(),
                 dest[1].copy_constructor_count(),
                 dest[1].ctor_order() };
    };

    // Non constant
    {
        ResultType result = test();
        ASSERT_EQ(hud::get<0>(result), 1u);
        ASSERT_EQ(hud::get<1>(result), 2);
        ASSERT_EQ(hud::get<2>(result), 1u);
        ASSERT_EQ(hud::get<3>(result), 1);
    }

    // Constant
    {
        constexpr ResultType result = test();
        ASSERT_EQ(hud::get<0>(result), 1u);
        ASSERT_EQ(hud::get<1>(result), 2);
        ASSERT_EQ(hud::get<2>(result), 1u);
        ASSERT_EQ(hud::get<3>(result), 1);
    }
}

TEST(Memory, move_or_copy_construct_array_then_destroy_backward_non_bitwise_move_constructible_and_destructible_same_type)
{


    struct type : public hud::test::NonBitwiseMoveConstructibleType4, hud::test::SetBoolToTrueWhenDestroyed {
        constexpr type(i32* ctor_order, i32* dtor_order) noexcept
            : hud::test::NonBitwiseMoveConstructibleType4(ctor_order)
            , hud::test::SetBoolToTrueWhenDestroyed(dtor_order) {
        }

        // MSVC call copy constructor instead of move constructor 
        // https://developercommunity.visualstudio.com/t/constexpr-stdconstruct-at-do-not-works/1545985
        constexpr type(const type& other) noexcept
            : hud::test::NonBitwiseMoveConstructibleType4(other)
            , hud::test::SetBoolToTrueWhenDestroyed(other) {
        }

        constexpr type(type&& other) noexcept
            : hud::test::NonBitwiseMoveConstructibleType4(std::forward<hud::test::NonBitwiseMoveConstructibleType4>(other))
            , hud::test::SetBoolToTrueWhenDestroyed(std::forward<hud::test::SetBoolToTrueWhenDestroyed>(other)) {
        }

        constexpr ~type() = default;
    };

    using ResultType = std::tuple<u32, u32, i32, u32, u32, i32>;
    static_assert(hud::is_move_constructible_v<type>);
    static_assert(!hud::is_bitwise_move_constructible_v<type>);
    static_assert(hud::is_destructible_v<type>);
    static_assert(!hud::is_trivially_destructible_v<type>);


    auto test = []() -> ResultType {
        i32 ctor_order = 0;
        i32 dtor_order = 0;

        type* src = hud::memory::allocate_array<type>(2);
        hud::test::LeakArrayGuard guard_src(src, 2);
        hud::memory::construct_array_at(src, src+2, & ctor_order, & dtor_order);

        type* dest = hud::memory::allocate_array<type>(2);
        hud::test::LeakArrayGuard guard_dest(dest, 2);

        hud::memory::move_or_copy_construct_array_then_destroy_backward(dest, src, 2);
        return { dest[0].move_constructor_count(),
                 dest[0].copy_constructor_count(),
                 dest[0].ctor_order(),
                 dest[1].move_constructor_count(),
                 dest[1].copy_constructor_count(),
                 dest[1].ctor_order() };
    };

    // Non constant
    {
        ResultType result = test();
        ASSERT_EQ(hud::get<0>(result), 1u);
        ASSERT_EQ(hud::get<1>(result), 0u);
        ASSERT_EQ(hud::get<2>(result), 2);
        ASSERT_EQ(hud::get<3>(result), 1u);
        ASSERT_EQ(hud::get<4>(result), 0u);
        ASSERT_EQ(hud::get<5>(result), 1);
    }

    // Constant
    {
        constexpr ResultType result = test();

        // MSVC call copy constructor instead of move constructor 
        // https://developercommunity.visualstudio.com/t/constexpr-stdconstruct-at-do-not-works/1545985
        if constexpr (hud::compilation::is_compiler(hud::compiler_e::msvc)) {
            ASSERT_EQ(hud::get<0>(result), 0u);
            ASSERT_EQ(hud::get<1>(result), 1u);
        }
        else {
            ASSERT_EQ(hud::get<0>(result), 1u);
            ASSERT_EQ(hud::get<1>(result), 0u);
        }
        ASSERT_EQ(hud::get<2>(result), 2);

        // MSVC call copy constructor instead of move constructor 
        // https://developercommunity.visualstudio.com/t/constexpr-stdconstruct-at-do-not-works/1545985
        if constexpr (hud::compilation::is_compiler(hud::compiler_e::msvc)) {
            ASSERT_EQ(hud::get<3>(result), 0u);
            ASSERT_EQ(hud::get<4>(result), 1u);
        }
        else {
            ASSERT_EQ(hud::get<3>(result), 1u);
            ASSERT_EQ(hud::get<4>(result), 0u);
        }
        ASSERT_EQ(hud::get<5>(result), 1);
    }
}