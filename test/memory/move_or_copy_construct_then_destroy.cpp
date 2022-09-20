#include <core/memory.h>
#include "../misc/leak_guard.h"

TEST(Memory, move_or_copy_construct_then_destroy_trivial_type)
{


    using type = u32;
    using ResultType = std::tuple<u32, u32>;

    static_assert(std::is_trivially_copy_constructible_v<type>);
    static_assert(std::is_trivially_destructible_v<type>);

    auto test = []() -> ResultType {
        type dest;
        type src = 15;
        hud::memory::move_or_copy_construct_then_destroy(&dest, hud::move(src));
        return { dest, src };
    };

    // Non constant
    {
        ResultType result = test();
        ASSERT_EQ(hud::get<0>(result), 15u);
        ASSERT_EQ(hud::get<1>(result), 15u);
    }

    // Constant
    {
        constexpr ResultType result = test();
        ASSERT_EQ(hud::get<0>(result), 15u);
        ASSERT_EQ(hud::get<1>(result), 15u);
    }
}

TEST(Memory, move_or_copy_construct_then_destroy_bitwise_constructible_type)
{


    struct c {
        i32 i;
    };

    using type = c;
    using ResultType = std::tuple<i32, i32>;

    static_assert(hud::is_bitwise_copy_constructible_v<c>);
    static_assert(hud::is_bitwise_move_constructible_v<c>);

    auto test = []() -> ResultType {
        type dest, src;
        dest.i = 0;
        src.i = 15;
        hud::memory::move_or_copy_construct_then_destroy(&dest, hud::move(src));
        return { dest.i, src.i };
    };

    // Non constant
    {
        ResultType result = test();
        ASSERT_EQ(hud::get<0>(result), 15);
        ASSERT_EQ(hud::get<1>(result), 15);
    }

    // Constant
    {
        constexpr ResultType result = test();
        ASSERT_EQ(hud::get<0>(result), 15);
        ASSERT_EQ(hud::get<1>(result), 15);
    }
}

TEST(Memory, move_or_copy_construct_then_destroy_non_bitwise_copy_constructible_and_destructible_different_type)
{


    struct SourceType : public hud::test::NonBitwiseCopyConstructibleType3, hud::test::SetBoolToTrueWhenDestroyed {
        constexpr SourceType(i32* ctor_order, i32* dtor_order) noexcept
            : hud::test::NonBitwiseCopyConstructibleType3(ctor_order)
            , hud::test::SetBoolToTrueWhenDestroyed(dtor_order){
        }

        constexpr ~SourceType() = default;
    };

    struct DestinationType : public hud::test::NonBitwiseCopyConstructibleType4 {
        constexpr DestinationType(const SourceType& other) noexcept
            : hud::test::NonBitwiseCopyConstructibleType4(other) {
        }
    };

    using ResultType = std::tuple<i32, i32>;
    
    static_assert(hud::is_copy_constructible_v<DestinationType, SourceType>);
    static_assert(!hud::is_bitwise_copy_constructible_v<DestinationType, SourceType>);

    auto test = []() -> ResultType {
        i32 ctor_order = 0;
        i32 dtor_order = 0;

        SourceType* src = hud::memory::allocate_array<SourceType>(1);
        hud::test::LeakArrayGuard guard_src(src, 1);
        hud::memory::construct_at(src, &ctor_order, &dtor_order);

        DestinationType* dest = hud::memory::allocate_array<DestinationType>(1);
        hud::test::LeakArrayGuard guard_dest(dest, 1);

        hud::memory::move_or_copy_construct_then_destroy(dest, hud::move(*src));
        return { dest->copy_constructor_count(), 
                 dtor_order };
    };

    // Non constant
    {
        ResultType result = test();
        ASSERT_EQ(hud::get<0>(result), 1);
        ASSERT_EQ(hud::get<1>(result), 1);
    }

    // Constant
    {
        constexpr ResultType result = test();
        ASSERT_EQ(hud::get<0>(result), 1);
        ASSERT_EQ(hud::get<1>(result), 1);
    }
}

TEST(Memory, move_or_copy_construct_then_destroy_non_bitwise_move_constructible_and_destructible_different_type)
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

    using ResultType = std::tuple<u32, u32, i32>;
    static_assert(hud::is_move_constructible_v<DestinationType, SourceType>);
    static_assert(!hud::is_bitwise_move_constructible_v<DestinationType, SourceType>);
    static_assert(hud::is_destructible_v<SourceType>);
    static_assert(!hud::is_trivially_destructible_v<SourceType>);

    auto test = []() -> ResultType {
        i32 ctor_order = 0;
        i32 dtor_order = 0;

        SourceType* src = hud::memory::allocate_array<SourceType>(1);
        hud::test::LeakArrayGuard guard_src(src, 1);
        hud::memory::construct_at(src, &ctor_order, &dtor_order);

        DestinationType* dest = hud::memory::allocate_array<DestinationType>(1);
        hud::test::LeakArrayGuard guard_dest(dest, 1);

        hud::memory::move_or_copy_construct_then_destroy(dest, hud::move(*src));
        return { dest->move_constructor_count(), 
                 dest->copy_constructor_count(),
                 dtor_order };
    };

    // Non constant
    {
        ResultType result = test();
        ASSERT_EQ(hud::get<0>(result), 1u);
        ASSERT_EQ(hud::get<1>(result), 0u);
        ASSERT_EQ(hud::get<2>(result), 1);
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
        ASSERT_EQ(hud::get<2>(result), 1);
    }
}

TEST(Memory, move_or_copy_construct_then_destroy_non_bitwise_copy_constructible_and_destructible_same_type)
{


    struct type : public hud::test::NonBitwiseCopyConstructibleType4, hud::test::SetBoolToTrueWhenDestroyed {
        constexpr type(i32* ctor_order, i32* dtor_order) noexcept
            : hud::test::NonBitwiseCopyConstructibleType4(ctor_order)
            , hud::test::SetBoolToTrueWhenDestroyed(dtor_order) {
        }

        constexpr ~type() = default;
    };

    using ResultType = std::tuple<u32, i32>;
    static_assert(hud::is_copy_constructible_v<type>);
    static_assert(!hud::is_bitwise_copy_constructible_v<type>);
    static_assert(hud::is_destructible_v<type>);
    static_assert(!hud::is_trivially_destructible_v<type>);


    auto test = []() -> ResultType {
        i32 ctor_order = 0;
        i32 dtor_order = 0;

        type* src = hud::memory::allocate_array<type>(1);
        hud::test::LeakArrayGuard guard_src(src, 1);
        hud::memory::construct_at(src, &ctor_order, &dtor_order);

        type* dest = hud::memory::allocate_array<type>(1);
        hud::test::LeakArrayGuard guard_dest(dest, 1);

        hud::memory::move_or_copy_construct_then_destroy(dest, hud::move(*src));
        return { dest->copy_constructor_count(), 
                 dest->ctor_order() };
    };

    // Non constant
    {
        ResultType result = test();
        ASSERT_EQ(hud::get<0>(result), 1u);
        ASSERT_EQ(hud::get<1>(result), 1);
    }

    // Constant
    {
        constexpr ResultType result = test();
        ASSERT_EQ(hud::get<0>(result), 1u);
        ASSERT_EQ(hud::get<1>(result), 1);
    }
}

TEST(Memory, move_or_copy_construct_then_destroy_non_bitwise_move_constructible_and_destructible_same_type)
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

    using ResultType = std::tuple<u32, u32, i32>;
    static_assert(hud::is_move_constructible_v<type>);
    static_assert(!hud::is_bitwise_move_constructible_v<type>);
    static_assert(hud::is_destructible_v<type>);
    static_assert(!hud::is_trivially_destructible_v<type>);


    auto test = []() -> ResultType {
        i32 ctor_order = 0;
        i32 dtor_order = 0;

        type* src = hud::memory::allocate_array<type>(1);
        hud::test::LeakArrayGuard guard_src(src, 1);
        hud::memory::construct_at(src, &ctor_order, &dtor_order);

        type* dest = hud::memory::allocate_array<type>(1);
        hud::test::LeakArrayGuard guard_dest(dest, 1);

        hud::memory::move_or_copy_construct_then_destroy(dest, hud::move(*src));
        return { dest->move_constructor_count(), 
                 dest->copy_constructor_count(), 
                 dest->ctor_order() };
    };

    // Non constant
    {
        ResultType result = test();
        ASSERT_EQ(hud::get<0>(result), 1u);
        ASSERT_EQ(hud::get<1>(result), 0u);
        ASSERT_EQ(hud::get<2>(result), 1);
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
        ASSERT_EQ(hud::get<2>(result), 1);
    }
}
