#include <core/containers/tuple.h>

#include <core/containers/pair.h>

#include <core/traits/is_same.h>
#include <core/traits/is_explicitly_constructible.h>
#include <core/traits/is_implicitly_constructible.h>
#include <core/traits/is_explicitly_copy_constructible.h>
#include <core/traits/is_implicitly_copy_constructible.h>
#include <core/traits/is_explicitly_move_constructible.h>
#include <core/traits/is_implicitly_move_constructible.h>

#include <math.h> // isnan

namespace {
    struct ExplicitDefaultConstructible
        : public hud::test::default_constructible_type
    {
        constexpr explicit ExplicitDefaultConstructible() noexcept
            : hud::test::default_constructible_type() {
        }
    };
    static_assert(hud::is_explicitly_default_constructible_v<ExplicitDefaultConstructible>);

    struct ImplicitDefaultConstructible
        : public hud::test::default_constructible_type
    {
        constexpr ImplicitDefaultConstructible() noexcept
            : hud::test::default_constructible_type() {
        }
    };

    static_assert(hud::is_implicitly_default_constructible_v<ImplicitDefaultConstructible>);

    struct ExplicitlyCopyConstructible
        : public hud::test::NonBitwiseCopyConstructibleType {

        constexpr ExplicitlyCopyConstructible(i32 id) noexcept
            : hud::test::NonBitwiseCopyConstructibleType(id) {
        }

        explicit constexpr ExplicitlyCopyConstructible(const ExplicitlyCopyConstructible& other) noexcept
            : hud::test::NonBitwiseCopyConstructibleType(other) {
        }
    };
    
    static_assert(hud::is_explicitly_copy_constructible_v<ExplicitlyCopyConstructible>);

    struct ExplicitlyCopyConstructible2
        : public hud::test::NonBitwiseCopyConstructibleType2 {

        constexpr ExplicitlyCopyConstructible2(i32 id) noexcept
            : hud::test::NonBitwiseCopyConstructibleType2(id) {
        }

        explicit constexpr ExplicitlyCopyConstructible2(const ExplicitlyCopyConstructible& other) noexcept
            : hud::test::NonBitwiseCopyConstructibleType2(other) {
        }
    };
    static_assert(hud::is_explicitly_copy_constructible_v<ExplicitlyCopyConstructible2, ExplicitlyCopyConstructible>);

    struct ImplicitlyCopyConstructible
        : public hud::test::NonBitwiseCopyConstructibleType {

        constexpr ImplicitlyCopyConstructible(i32 id) noexcept
            : hud::test::NonBitwiseCopyConstructibleType(id) {
        }

        constexpr ImplicitlyCopyConstructible(const ImplicitlyCopyConstructible& other) noexcept
            : hud::test::NonBitwiseCopyConstructibleType(other) {
        }
    };
    static_assert(hud::is_implicitly_copy_constructible_v<ImplicitlyCopyConstructible>);

    struct ImplicitlyCopyConstructible2
        : public hud::test::NonBitwiseCopyConstructibleType2 {

        constexpr ImplicitlyCopyConstructible2(i32 id) noexcept
            : hud::test::NonBitwiseCopyConstructibleType2(id) {
        }

        constexpr ImplicitlyCopyConstructible2(const ImplicitlyCopyConstructible& other) noexcept
            : hud::test::NonBitwiseCopyConstructibleType2(other) {
        }
    };
    static_assert(hud::is_implicitly_copy_constructible_v<ImplicitlyCopyConstructible2, ImplicitlyCopyConstructible>);

    struct ExplicitlyMoveConstructible
        : public hud::test::NonBitwiseMoveConstructibleType {

        constexpr ExplicitlyMoveConstructible(i32 id) noexcept
            : hud::test::NonBitwiseMoveConstructibleType(id) {
        }

        explicit constexpr ExplicitlyMoveConstructible(const ExplicitlyMoveConstructible& other) noexcept
            : hud::test::NonBitwiseMoveConstructibleType(other) {
        }

        explicit constexpr ExplicitlyMoveConstructible(ExplicitlyMoveConstructible&& other) noexcept
            : hud::test::NonBitwiseMoveConstructibleType(hud::move(other)) {
        }
    };
    static_assert(hud::is_explicitly_move_constructible_v<ExplicitlyMoveConstructible>);

    struct ExplicitlyMoveConstructible2
        : public hud::test::NonBitwiseMoveConstructibleType2 {

        constexpr ExplicitlyMoveConstructible2(i32 id) noexcept
            : hud::test::NonBitwiseMoveConstructibleType2(id) {
        }

        explicit constexpr ExplicitlyMoveConstructible2(const ExplicitlyMoveConstructible& other) noexcept
            : hud::test::NonBitwiseMoveConstructibleType2(other) {
        }

        explicit constexpr ExplicitlyMoveConstructible2(ExplicitlyMoveConstructible&& other) noexcept
            : hud::test::NonBitwiseMoveConstructibleType2(hud::move(other)) {
        }
    };
    static_assert(hud::is_explicitly_move_constructible_v<ExplicitlyMoveConstructible2, ExplicitlyMoveConstructible>);

    struct ImplicitlyMoveConstructible
        : public hud::test::NonBitwiseMoveConstructibleType {

        constexpr ImplicitlyMoveConstructible(i32 id) noexcept
            : hud::test::NonBitwiseMoveConstructibleType(id) {
        }

        constexpr ImplicitlyMoveConstructible(const ImplicitlyMoveConstructible& other) noexcept
            : hud::test::NonBitwiseMoveConstructibleType(other) {
        }

        constexpr ImplicitlyMoveConstructible(ImplicitlyMoveConstructible&& other) noexcept
            : hud::test::NonBitwiseMoveConstructibleType(hud::move(other)) {
        }
    };

    static_assert(hud::is_implicitly_move_constructible_v<ImplicitlyMoveConstructible>);

    struct ImplicitlyMoveConstructible2
        : public hud::test::NonBitwiseMoveConstructibleType2 {

        constexpr ImplicitlyMoveConstructible2(i32 id) noexcept
            : hud::test::NonBitwiseMoveConstructibleType2(id) {
        }

        constexpr ImplicitlyMoveConstructible2(const ImplicitlyMoveConstructible& other) noexcept
            : hud::test::NonBitwiseMoveConstructibleType2(other) {
        }

        constexpr ImplicitlyMoveConstructible2(ImplicitlyMoveConstructible&& other) noexcept
            : hud::test::NonBitwiseMoveConstructibleType2(hud::move(other)) {
        }
    };
    static_assert(hud::is_implicitly_move_constructible_v<ImplicitlyMoveConstructible2, ImplicitlyMoveConstructible>);
}


TEST(Tuple, default_constructor_uninit) {
    using TupleType = hud::Tuple<>;
    using TupleType2 = hud::Tuple<i32, i32, i32>;
    using TupleType3 = hud::Tuple<i32, f32, wchar>;
    using TupleType4 = hud::Tuple<ExplicitDefaultConstructible, ImplicitDefaultConstructible>;
    using TupleType5 = hud::Tuple<ExplicitDefaultConstructible, i32>;

    ASSERT_EQ(hud::TupleSizeV<TupleType>, 0u);
    ASSERT_EQ(hud::TupleSizeV<TupleType2>, 3u);
    ASSERT_EQ(hud::TupleSizeV<TupleType3>, 3u);
    ASSERT_EQ(hud::TupleSizeV<TupleType4>, 2u);
    ASSERT_EQ(hud::TupleSizeV<TupleType5>, 2u);

    // Non constant
    {
        [[maybe_unused]] TupleType tuple1;

        TupleType2* tuple2 = hud::memory::allocate_array<TupleType2>(1);
        hud::memory::set(tuple2, sizeof(TupleType2), 0xFF);
        hud::memory::construct_at(tuple2);
        ASSERT_TRUE((hud::is_same_v<decltype(hud::hud::get<0>(*tuple2)), i32&>));
        ASSERT_TRUE((hud::is_same_v<decltype(hud::hud::get<1>(*tuple2)), i32&>));
        ASSERT_TRUE((hud::is_same_v<decltype(hud::hud::get<2>(*tuple2)), i32&>));
        ASSERT_NE((hud::hud::get<0>(*tuple2)), i32{});
        ASSERT_NE((hud::hud::get<1>(*tuple2)), i32{});
        ASSERT_NE((hud::hud::get<2>(*tuple2)), i32{});
        hud::memory::destroy_array(tuple2, 1);
        hud::memory::free(tuple2);

        TupleType3* tuple3 = hud::memory::allocate_array<TupleType3>(1);
        hud::memory::set(tuple3, sizeof(TupleType3), 0xFF);
        hud::memory::construct_at(tuple3);
        ASSERT_TRUE((hud::is_same_v<decltype(hud::hud::get<0>(*tuple3)), i32&>));
        ASSERT_TRUE((hud::is_same_v<decltype(hud::hud::get<1>(*tuple3)), f32&>));
        ASSERT_TRUE((hud::is_same_v<decltype(hud::hud::get<2>(*tuple3)), wchar&>));
        ASSERT_NE((hud::hud::get<0>(*tuple3)), i32{});

        // Is nan always return false with clang in release with fast math enabled
        // See: https://clang.llvm.org/docs/UsersManual.html#cmdoption-ffast-math
        #if defined(HD_COMPILER_CLANG) && defined(HD_RELEASE) && defined(__FAST_MATH__)
        ASSERT_FALSE(isnan(hud::hud::get<1>(*tuple3)));
        #else
        ASSERT_TRUE(isnan(hud::hud::get<1>(*tuple3)));
        #endif
        ASSERT_NE((hud::hud::get<2>(*tuple3)), wchar{});
        hud::memory::destroy_array(tuple3, 1);
        hud::memory::free(tuple3);

        TupleType4* tuple4 = hud::memory::allocate_array<TupleType4>(1);
        hud::memory::set(tuple4, sizeof(TupleType4), 0xFF);
        hud::memory::construct_at(tuple4);
        ASSERT_TRUE((hud::is_same_v<decltype(hud::hud::get<0>(*tuple4)), ExplicitDefaultConstructible&>));
        ASSERT_TRUE((hud::is_same_v<decltype(hud::hud::get<1>(*tuple4)), ImplicitDefaultConstructible&>));
        ASSERT_EQ((hud::hud::get<0>(*tuple4).id()), ExplicitDefaultConstructible::DEFAULT_ID_VALUE);
        ASSERT_EQ((hud::hud::get<0>(*tuple4).default_constructor_count()), 1u);
        ASSERT_EQ((hud::hud::get<0>(*tuple4).constructor_count()), 0u);
        ASSERT_EQ((hud::hud::get<0>(*tuple4).copy_constructor_count()), 0u);
        ASSERT_EQ((hud::hud::get<0>(*tuple4).move_constructor_count()), 0u);
        ASSERT_EQ((hud::hud::get<1>(*tuple4).id()), ImplicitDefaultConstructible::DEFAULT_ID_VALUE);
        ASSERT_EQ((hud::hud::get<1>(*tuple4).default_constructor_count()), 1u);
        ASSERT_EQ((hud::hud::get<1>(*tuple4).constructor_count()), 0u);
        ASSERT_EQ((hud::hud::get<1>(*tuple4).copy_constructor_count()), 0u);
        ASSERT_EQ((hud::hud::get<1>(*tuple4).move_constructor_count()), 0u);
        hud::memory::destroy_array(tuple4, 1);
        hud::memory::free(tuple4);

        TupleType5* tuple5 = hud::memory::allocate_array<TupleType5>(1);
        hud::memory::set(tuple5, sizeof(TupleType5), 0xFF);
        hud::memory::construct_at(tuple5);
        ASSERT_TRUE((hud::is_same_v<decltype(hud::hud::get<0>(*tuple5)), ExplicitDefaultConstructible&>));
        ASSERT_TRUE((hud::is_same_v<decltype(hud::hud::get<1>(*tuple5)), i32&>));
        ASSERT_EQ((hud::hud::get<0>(*tuple5).id()), ExplicitDefaultConstructible::DEFAULT_ID_VALUE);
        ASSERT_EQ((hud::hud::get<0>(*tuple5).default_constructor_count()), 1u);
        ASSERT_EQ((hud::hud::get<0>(*tuple5).constructor_count()), 0u);
        ASSERT_EQ((hud::hud::get<0>(*tuple5).copy_constructor_count()), 0u);
        ASSERT_EQ((hud::hud::get<0>(*tuple5).move_constructor_count()), 0u);
        ASSERT_NE(hud::hud::get<1>(*tuple5), i32{});
        hud::memory::destroy_array(tuple5, 1);
        hud::memory::free(tuple5);

    }

    // Constant
    // Non empty uninit constexpr tuple is not possible because constexpr need initialized value
    {
        [[maybe_unused]] constexpr TupleType tuple;
        ASSERT_EQ(hud::TupleSizeV<TupleType>, 0u);
    }
}

TEST(Tuple, default_constructor_init) {
    using TupleType = hud::Tuple<>;
    using TupleType2 = hud::Tuple<i32, i32, i32>;
    using TupleType3 = hud::Tuple<i32, f32, wchar>;
    using TupleType4 = hud::Tuple<ExplicitDefaultConstructible, ImplicitDefaultConstructible>;
    using TupleType5 = hud::Tuple<ExplicitDefaultConstructible, i32>;

    ASSERT_EQ(hud::TupleSizeV<TupleType>, 0u);
    ASSERT_EQ(hud::TupleSizeV<TupleType2>, 3u);
    ASSERT_EQ(hud::TupleSizeV<TupleType3>, 3u);
    ASSERT_EQ(hud::TupleSizeV<TupleType4>, 2u);
    ASSERT_EQ(hud::TupleSizeV<TupleType5>, 2u);

    // Non constant
    {
        // No init for empty tuple
        [[maybe_unused]] TupleType tuple1;

        TupleType2 tuple2{ hud::tag_init };
        ASSERT_TRUE((hud::is_same_v<decltype(hud::hud::get<0>(tuple2)), i32&>));
        ASSERT_TRUE((hud::is_same_v<decltype(hud::hud::get<1>(tuple2)), i32&>));
        ASSERT_TRUE((hud::is_same_v<decltype(hud::hud::get<2>(tuple2)), i32&>));
        ASSERT_EQ((hud::hud::get<0>(tuple2)), i32{});
        ASSERT_EQ((hud::hud::get<1>(tuple2)), i32{});
        ASSERT_EQ((hud::hud::get<2>(tuple2)), i32{});

        TupleType3 tuple3{ hud::tag_init };
        ASSERT_TRUE((hud::is_same_v<decltype(hud::hud::get<0>(tuple3)), i32&>));
        ASSERT_TRUE((hud::is_same_v<decltype(hud::hud::get<1>(tuple3)), f32&>));
        ASSERT_TRUE((hud::is_same_v<decltype(hud::hud::get<2>(tuple3)), wchar&>));
        ASSERT_EQ((hud::hud::get<0>(tuple3)), i32{});
        ASSERT_EQ((hud::hud::get<1>(tuple3)), f32{});
        ASSERT_EQ((hud::hud::get<2>(tuple3)), wchar{});

        TupleType4 tuple4{ hud::tag_init };
        ASSERT_TRUE((hud::is_same_v<decltype(hud::hud::get<0>(tuple4)), ExplicitDefaultConstructible&>));
        ASSERT_TRUE((hud::is_same_v<decltype(hud::hud::get<1>(tuple4)), ImplicitDefaultConstructible&>));
        ASSERT_EQ((hud::hud::get<0>(tuple4).id()), ExplicitDefaultConstructible::DEFAULT_ID_VALUE);
        ASSERT_EQ((hud::hud::get<0>(tuple4).default_constructor_count()), 1u);
        ASSERT_EQ((hud::hud::get<0>(tuple4).constructor_count()), 0u);
        ASSERT_EQ((hud::hud::get<0>(tuple4).copy_constructor_count()), 0u);
        ASSERT_EQ((hud::hud::get<0>(tuple4).move_constructor_count()), 0u);
        ASSERT_EQ((hud::hud::get<1>(tuple4).id()), ImplicitDefaultConstructible::DEFAULT_ID_VALUE);
        ASSERT_EQ((hud::hud::get<1>(tuple4).default_constructor_count()), 1u);
        ASSERT_EQ((hud::hud::get<1>(tuple4).constructor_count()), 0u);
        ASSERT_EQ((hud::hud::get<1>(tuple4).copy_constructor_count()), 0u);
        ASSERT_EQ((hud::hud::get<1>(tuple4).move_constructor_count()), 0u);

        TupleType5 tuple5{ hud::tag_init };
        ASSERT_TRUE((hud::is_same_v<decltype(hud::hud::get<0>(tuple5)), ExplicitDefaultConstructible&>));
        ASSERT_TRUE((hud::is_same_v<decltype(hud::hud::get<1>(tuple5)), i32&>));
        ASSERT_EQ((hud::hud::get<0>(tuple5).id()), ExplicitDefaultConstructible::DEFAULT_ID_VALUE);
        ASSERT_EQ((hud::hud::get<0>(tuple5).default_constructor_count()), 1u);
        ASSERT_EQ((hud::hud::get<0>(tuple5).constructor_count()), 0u);
        ASSERT_EQ((hud::hud::get<0>(tuple5).copy_constructor_count()), 0u);
        ASSERT_EQ((hud::hud::get<0>(tuple5).move_constructor_count()), 0u);
        ASSERT_EQ((hud::hud::get<1>(tuple5)), i32{});
    }

    // Constant
    {
        [[maybe_unused]] constexpr TupleType tuple1;

        constexpr TupleType2 tuple2{ hud::tag_init };
        ASSERT_TRUE((hud::is_same_v<decltype(hud::hud::get<0>(tuple2)), const i32&>));
        ASSERT_TRUE((hud::is_same_v<decltype(hud::hud::get<1>(tuple2)), const i32&>));
        ASSERT_TRUE((hud::is_same_v<decltype(hud::hud::get<2>(tuple2)), const i32&>));

        ASSERT_EQ((hud::hud::get<0>(tuple2)), i32{});
        ASSERT_EQ((hud::hud::get<1>(tuple2)), i32{});
        ASSERT_EQ((hud::hud::get<2>(tuple2)), i32{});

        constexpr TupleType3 tuple3{ hud::tag_init };
        ASSERT_TRUE((hud::is_same_v<decltype(hud::hud::get<0>(tuple3)), const i32&>));
        ASSERT_TRUE((hud::is_same_v<decltype(hud::hud::get<1>(tuple3)), const f32&>));
        ASSERT_TRUE((hud::is_same_v<decltype(hud::hud::get<2>(tuple3)), const wchar&>));

        ASSERT_EQ((hud::hud::get<0>(tuple3)), i32{});
        ASSERT_EQ((hud::hud::get<1>(tuple3)), f32{});
        ASSERT_EQ((hud::hud::get<2>(tuple3)), wchar{});

        constexpr TupleType4 tuple4{ hud::tag_init };
        ASSERT_TRUE((hud::is_same_v<decltype(hud::hud::get<0>(tuple4)), const ExplicitDefaultConstructible&>));
        ASSERT_TRUE((hud::is_same_v<decltype(hud::hud::get<1>(tuple4)), const ImplicitDefaultConstructible&>));
        ASSERT_EQ((hud::hud::get<0>(tuple4).id()), ExplicitDefaultConstructible::DEFAULT_ID_VALUE);
        ASSERT_EQ((hud::hud::get<0>(tuple4).default_constructor_count()), 1u);
        ASSERT_EQ((hud::hud::get<0>(tuple4).constructor_count()), 0u);
        ASSERT_EQ((hud::hud::get<0>(tuple4).copy_constructor_count()), 0u);
        ASSERT_EQ((hud::hud::get<0>(tuple4).move_constructor_count()), 0u);
        ASSERT_EQ((hud::hud::get<1>(tuple4).id()), ImplicitDefaultConstructible::DEFAULT_ID_VALUE);
        ASSERT_EQ((hud::hud::get<1>(tuple4).default_constructor_count()), 1u);
        ASSERT_EQ((hud::hud::get<1>(tuple4).constructor_count()), 0u);
        ASSERT_EQ((hud::hud::get<1>(tuple4).copy_constructor_count()), 0u);
        ASSERT_EQ((hud::hud::get<1>(tuple4).move_constructor_count()), 0u);

        constexpr TupleType5 tuple5{ hud::tag_init };
        ASSERT_TRUE((hud::is_same_v<decltype(hud::hud::get<0>(tuple5)), const ExplicitDefaultConstructible&>));
        ASSERT_TRUE((hud::is_same_v<decltype(hud::hud::get<1>(tuple5)), const i32&>));
        ASSERT_EQ((hud::hud::get<0>(tuple5).id()), ExplicitDefaultConstructible::DEFAULT_ID_VALUE);
        ASSERT_EQ((hud::hud::get<0>(tuple5).default_constructor_count()), 1u);
        ASSERT_EQ((hud::hud::get<0>(tuple5).constructor_count()), 0u);
        ASSERT_EQ((hud::hud::get<0>(tuple5).copy_constructor_count()), 0u);
        ASSERT_EQ((hud::hud::get<0>(tuple5).move_constructor_count()), 0u);
        ASSERT_EQ((hud::hud::get<1>(tuple5)), i32{});
    }
}

TEST(Tuple, default_constructor_explicit_types) {
    using TupleType = hud::Tuple<ExplicitDefaultConstructible, ExplicitDefaultConstructible>;

    ASSERT_TRUE(hud::is_explicitly_default_constructible_v<TupleType>);

    // Non constant
    {
        // No init for empty tuple
        TupleType tuple;

        ASSERT_EQ((hud::hud::get<0>(tuple).id()), ExplicitDefaultConstructible::DEFAULT_ID_VALUE);
        ASSERT_EQ((hud::hud::get<0>(tuple).default_constructor_count()), 1u);
        ASSERT_EQ((hud::hud::get<0>(tuple).constructor_count()), 0u);
        ASSERT_EQ((hud::hud::get<0>(tuple).copy_constructor_count()), 0u);
        ASSERT_EQ((hud::hud::get<0>(tuple).move_constructor_count()), 0u);
        ASSERT_EQ((hud::hud::get<1>(tuple).id()), ExplicitDefaultConstructible::DEFAULT_ID_VALUE);
        ASSERT_EQ((hud::hud::get<1>(tuple).default_constructor_count()), 1u);
        ASSERT_EQ((hud::hud::get<1>(tuple).constructor_count()), 0u);
        ASSERT_EQ((hud::hud::get<1>(tuple).copy_constructor_count()), 0u);
        ASSERT_EQ((hud::hud::get<1>(tuple).move_constructor_count()), 0u);
    }

    // Constant
    {
        // No init for empty tuple
        constexpr TupleType tuple;

        ASSERT_EQ((hud::hud::get<0>(tuple).id()), ExplicitDefaultConstructible::DEFAULT_ID_VALUE);
        ASSERT_EQ((hud::hud::get<0>(tuple).default_constructor_count()), 1u);
        ASSERT_EQ((hud::hud::get<0>(tuple).constructor_count()), 0u);
        ASSERT_EQ((hud::hud::get<0>(tuple).copy_constructor_count()), 0u);
        ASSERT_EQ((hud::hud::get<0>(tuple).move_constructor_count()), 0u);
        ASSERT_EQ((hud::hud::get<1>(tuple).id()), ExplicitDefaultConstructible::DEFAULT_ID_VALUE);
        ASSERT_EQ((hud::hud::get<1>(tuple).default_constructor_count()), 1u);
        ASSERT_EQ((hud::hud::get<1>(tuple).constructor_count()), 0u);
        ASSERT_EQ((hud::hud::get<1>(tuple).copy_constructor_count()), 0u);
        ASSERT_EQ((hud::hud::get<1>(tuple).move_constructor_count()), 0u);
    }
}

TEST(Tuple, default_constructor_implicit_types) {
    using TupleType = hud::Tuple<ImplicitDefaultConstructible, ImplicitDefaultConstructible>;

    ASSERT_TRUE(hud::is_implicitly_default_constructible_v<TupleType>);

    // Non constant
    {
        // No init for empty tuple
        TupleType tuple;

        ASSERT_EQ((hud::hud::get<0>(tuple).id()), ImplicitDefaultConstructible::DEFAULT_ID_VALUE);
        ASSERT_EQ((hud::hud::get<0>(tuple).default_constructor_count()), 1u);
        ASSERT_EQ((hud::hud::get<0>(tuple).constructor_count()), 0u);
        ASSERT_EQ((hud::hud::get<0>(tuple).copy_constructor_count()), 0u);
        ASSERT_EQ((hud::hud::get<0>(tuple).move_constructor_count()), 0u);
        ASSERT_EQ((hud::hud::get<1>(tuple).id()), ImplicitDefaultConstructible::DEFAULT_ID_VALUE);
        ASSERT_EQ((hud::hud::get<1>(tuple).default_constructor_count()), 1u);
        ASSERT_EQ((hud::hud::get<1>(tuple).constructor_count()), 0u);
        ASSERT_EQ((hud::hud::get<1>(tuple).copy_constructor_count()), 0u);
        ASSERT_EQ((hud::hud::get<1>(tuple).move_constructor_count()), 0u);
    }

    // Constant
    {
        // No init for empty tuple
        constexpr TupleType tuple;

        ASSERT_EQ((hud::hud::get<0>(tuple).id()), ImplicitDefaultConstructible::DEFAULT_ID_VALUE);
        ASSERT_EQ((hud::hud::get<0>(tuple).default_constructor_count()), 1u);
        ASSERT_EQ((hud::hud::get<0>(tuple).constructor_count()), 0u);
        ASSERT_EQ((hud::hud::get<0>(tuple).copy_constructor_count()), 0u);
        ASSERT_EQ((hud::hud::get<0>(tuple).move_constructor_count()), 0u);
        ASSERT_EQ((hud::hud::get<1>(tuple).id()), ImplicitDefaultConstructible::DEFAULT_ID_VALUE);
        ASSERT_EQ((hud::hud::get<1>(tuple).default_constructor_count()), 1u);
        ASSERT_EQ((hud::hud::get<1>(tuple).constructor_count()), 0u);
        ASSERT_EQ((hud::hud::get<1>(tuple).copy_constructor_count()), 0u);
        ASSERT_EQ((hud::hud::get<1>(tuple).move_constructor_count()), 0u);
    }
}

TEST(Tuple, default_constructor_mix_implicit_explicit_types) {
    using TupleType = hud::Tuple<ImplicitDefaultConstructible, ExplicitDefaultConstructible>;

    ASSERT_TRUE(hud::is_explicitly_default_constructible_v<TupleType>);

    // Non constant
    {
        // No init for empty tuple
        TupleType tuple;

        ASSERT_EQ((hud::hud::get<0>(tuple).id()), ImplicitDefaultConstructible::DEFAULT_ID_VALUE);
        ASSERT_EQ((hud::hud::get<0>(tuple).default_constructor_count()), 1u);
        ASSERT_EQ((hud::hud::get<0>(tuple).constructor_count()), 0u);
        ASSERT_EQ((hud::hud::get<0>(tuple).copy_constructor_count()), 0u);
        ASSERT_EQ((hud::hud::get<0>(tuple).move_constructor_count()), 0u);
        ASSERT_EQ((hud::hud::get<1>(tuple).id()), ExplicitDefaultConstructible::DEFAULT_ID_VALUE);
        ASSERT_EQ((hud::hud::get<1>(tuple).default_constructor_count()), 1u);
        ASSERT_EQ((hud::hud::get<1>(tuple).constructor_count()), 0u);
        ASSERT_EQ((hud::hud::get<1>(tuple).copy_constructor_count()), 0u);
        ASSERT_EQ((hud::hud::get<1>(tuple).move_constructor_count()), 0u);
    }

    // Constant
    {
        // No init for empty tuple
        constexpr TupleType tuple;

        ASSERT_EQ((hud::hud::get<0>(tuple).id()), ImplicitDefaultConstructible::DEFAULT_ID_VALUE);
        ASSERT_EQ((hud::hud::get<0>(tuple).default_constructor_count()), 1u);
        ASSERT_EQ((hud::hud::get<0>(tuple).constructor_count()), 0u);
        ASSERT_EQ((hud::hud::get<0>(tuple).copy_constructor_count()), 0u);
        ASSERT_EQ((hud::hud::get<0>(tuple).move_constructor_count()), 0u);
        ASSERT_EQ((hud::hud::get<1>(tuple).id()), ExplicitDefaultConstructible::DEFAULT_ID_VALUE);
        ASSERT_EQ((hud::hud::get<1>(tuple).default_constructor_count()), 1u);
        ASSERT_EQ((hud::hud::get<1>(tuple).constructor_count()), 0u);
        ASSERT_EQ((hud::hud::get<1>(tuple).copy_constructor_count()), 0u);
        ASSERT_EQ((hud::hud::get<1>(tuple).move_constructor_count()), 0u);
    }
}

TEST(Tuple, default_constructor_is_explicit_if_at_least_one_type_is_explicitly_default_constructible) {
    ASSERT_FALSE(hud::is_explicitly_default_constructible_v<hud::Tuple<>>);
    ASSERT_FALSE((hud::is_explicitly_default_constructible_v<hud::Tuple<i32, f32, wchar>>));
    ASSERT_FALSE((hud::is_explicitly_default_constructible_v<hud::Tuple<ImplicitDefaultConstructible, ImplicitDefaultConstructible>>));
    ASSERT_TRUE((hud::is_explicitly_default_constructible_v<hud::Tuple<ImplicitDefaultConstructible, ExplicitDefaultConstructible>>));
    ASSERT_TRUE((hud::is_explicitly_default_constructible_v<hud::Tuple<ExplicitDefaultConstructible, ExplicitDefaultConstructible>>));
}

TEST(Tuple, constructor_by_copy_explicitly_copyable_same_types) {
    using TupleType = hud::Tuple<ExplicitlyCopyConstructible, ExplicitlyCopyConstructible>;
    
    ASSERT_TRUE((hud::is_explicitly_constructible_v<TupleType, const ExplicitlyCopyConstructible&, const ExplicitlyCopyConstructible&>));
    
    const auto test = []() {
        const ExplicitlyCopyConstructible obj1(1);
        const ExplicitlyCopyConstructible obj2(2);
        TupleType tuple(obj1, obj2);
        return std::tuple{
            hud::hud::get<0>(tuple).id() == 1,
            hud::hud::get<0>(tuple).copy_constructor_count() == 1u,
            hud::hud::get<1>(tuple).id() == 2,
            hud::hud::get<1>(tuple).copy_constructor_count() == 1u
        };
    };
    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
    }
}

TEST(Tuple, constructor_by_copy_explicitly_copyable_different_types) {
    using TupleType = hud::Tuple<ExplicitlyCopyConstructible2, ExplicitlyCopyConstructible2>;

    ASSERT_TRUE((hud::is_explicitly_constructible_v<TupleType, const ExplicitlyCopyConstructible&, const ExplicitlyCopyConstructible&>));

    const auto test = []() {
        const ExplicitlyCopyConstructible obj1(1);
        const ExplicitlyCopyConstructible obj2(2);
        TupleType tuple(obj1, obj2);
        return std::tuple{
            hud::hud::get<0>(tuple).id() == 1,
            hud::hud::get<0>(tuple).copy_constructor_count() == 1u,
            hud::hud::get<1>(tuple).id() == 2,
            hud::hud::get<1>(tuple).copy_constructor_count() == 1u
        };
    };
    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
    }
}

TEST(Tuple, constructor_by_copy_implicitly_copyable_same_types) {
    using TupleType = hud::Tuple<ImplicitlyCopyConstructible, ImplicitlyCopyConstructible>;

    ASSERT_TRUE((hud::is_implicitly_constructible_v<TupleType, const ImplicitlyCopyConstructible&, const ImplicitlyCopyConstructible&>));

    const auto test = []() {
        const ImplicitlyCopyConstructible obj1(1);
        const ImplicitlyCopyConstructible obj2(2);
        TupleType tuple(obj1, obj2);
        return std::tuple{
            hud::hud::get<0>(tuple).id() == 1,
            hud::hud::get<0>(tuple).copy_constructor_count() == 1u,
            hud::hud::get<1>(tuple).id() == 2,
            hud::hud::get<1>(tuple).copy_constructor_count() == 1u
        };
    };
    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
    }
}

TEST(Tuple, constructor_by_copy_implicitly_copyable_different_types) {
    using TupleType = hud::Tuple<ImplicitlyCopyConstructible2, ImplicitlyCopyConstructible2>;

    ASSERT_TRUE((hud::is_implicitly_constructible_v<TupleType, const ImplicitlyCopyConstructible&, const ImplicitlyCopyConstructible&>));

    const auto test = []() {
        const ImplicitlyCopyConstructible obj1(1);
        const ImplicitlyCopyConstructible obj2(2);
        TupleType tuple(obj1, obj2);
        return std::tuple{
            hud::hud::get<0>(tuple).id() == 1,
            hud::hud::get<0>(tuple).copy_constructor_count() == 1u,
            hud::hud::get<1>(tuple).id() == 2,
            hud::hud::get<1>(tuple).copy_constructor_count() == 1u
        };
    };
    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
    }
}

TEST(Tuple, constructor_by_copy_mix_implicitly_explicitly_copyable_same_types) {
    using TupleType = hud::Tuple<ImplicitlyCopyConstructible, ExplicitlyCopyConstructible>;

    ASSERT_TRUE((hud::is_explicitly_constructible_v<TupleType, const ImplicitlyCopyConstructible&, const ExplicitlyCopyConstructible&>));

    const auto test = []() {
        const ImplicitlyCopyConstructible obj1(1);
        const ExplicitlyCopyConstructible obj2(2);
        TupleType tuple(obj1, obj2);
        return std::tuple{
            hud::hud::get<0>(tuple).id() == 1,
            hud::hud::get<0>(tuple).copy_constructor_count() == 1u,
            hud::hud::get<1>(tuple).id() == 2,
            hud::hud::get<1>(tuple).copy_constructor_count() == 1u
        };
    };
    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
    }
}

TEST(Tuple, constructor_by_copy_mix_implicitly_explicitly_copyable_different_types) {
    using TupleType = hud::Tuple<ImplicitlyCopyConstructible2, ExplicitlyCopyConstructible2>;

    ASSERT_TRUE((hud::is_explicitly_constructible_v<TupleType, const ImplicitlyCopyConstructible&, const ExplicitlyCopyConstructible&>));

    const auto test = []() {
        const ImplicitlyCopyConstructible obj1(1);
        const ExplicitlyCopyConstructible obj2(2);
        TupleType tuple(obj1, obj2);
        return std::tuple{
            hud::hud::get<0>(tuple).id() == 1,
            hud::hud::get<0>(tuple).copy_constructor_count() == 1u,
            hud::hud::get<1>(tuple).id() == 2,
            hud::hud::get<1>(tuple).copy_constructor_count() == 1u
        };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
    }
}

TEST(Tuple, constructor_by_move_explicitly_copyable_same_types) {
    using TupleType = hud::Tuple<ExplicitlyCopyConstructible, ExplicitlyCopyConstructible>;

    ASSERT_TRUE((hud::is_explicitly_constructible_v<TupleType, ExplicitlyCopyConstructible&&, ExplicitlyCopyConstructible&&>));

    const auto test = []() {
        ExplicitlyCopyConstructible obj1(1);
        ExplicitlyCopyConstructible obj2(2);
        TupleType tuple(hud::move(obj1), hud::move(obj2));
        return std::tuple{
            hud::hud::get<0>(tuple).id() == 1,
            hud::hud::get<0>(tuple).copy_constructor_count() == 1u,
            hud::hud::get<1>(tuple).id() == 2,
            hud::hud::get<1>(tuple).copy_constructor_count() == 1u
        };
    };
    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
    }
}

TEST(Tuple, constructor_by_move_explicitly_copyable_different_types) {
    using TupleType = hud::Tuple<ExplicitlyCopyConstructible2, ExplicitlyCopyConstructible2>;

    ASSERT_TRUE((hud::is_explicitly_constructible_v<TupleType, ExplicitlyCopyConstructible&&, ExplicitlyCopyConstructible&&>));

    const auto test = []() {
        ExplicitlyCopyConstructible obj1(1);
        ExplicitlyCopyConstructible obj2(2);
        TupleType tuple(hud::move(obj1), hud::move(obj2));
        return std::tuple{
            hud::hud::get<0>(tuple).id() == 1,
            hud::hud::get<0>(tuple).copy_constructor_count() == 1u,
            hud::hud::get<1>(tuple).id() == 2,
            hud::hud::get<1>(tuple).copy_constructor_count() == 1u
        };
    };
    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
    }
}

TEST(Tuple, constructor_by_move_implicitly_copyable_same_types) {
    using TupleType = hud::Tuple<ImplicitlyCopyConstructible, ImplicitlyCopyConstructible>;

    ASSERT_TRUE((hud::is_implicitly_constructible_v<TupleType, ImplicitlyCopyConstructible&&, ImplicitlyCopyConstructible&&>));

    const auto test = []() {
        ImplicitlyCopyConstructible obj1(1);
        ImplicitlyCopyConstructible obj2(2);
        TupleType tuple(hud::move(obj1), hud::move(obj2));
        return std::tuple{
            hud::hud::get<0>(tuple).id() == 1,
            hud::hud::get<0>(tuple).copy_constructor_count() == 1u,
            hud::hud::get<1>(tuple).id() == 2,
            hud::hud::get<1>(tuple).copy_constructor_count() == 1u
        };
    };
    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
    }
}

TEST(Tuple, constructor_by_move_implicitly_copyable_different_types) {
    using TupleType = hud::Tuple<ImplicitlyCopyConstructible2, ImplicitlyCopyConstructible2>;

    ASSERT_TRUE((hud::is_implicitly_constructible_v<TupleType, ImplicitlyCopyConstructible&&, ImplicitlyCopyConstructible&&>));

    const auto test = []() {
        ImplicitlyCopyConstructible obj1(1);
        ImplicitlyCopyConstructible obj2(2);
        TupleType tuple(hud::move(obj1), hud::move(obj2));
        return std::tuple{
            hud::hud::get<0>(tuple).id() == 1,
            hud::hud::get<0>(tuple).copy_constructor_count() == 1u,
            hud::hud::get<1>(tuple).id() == 2,
            hud::hud::get<1>(tuple).copy_constructor_count() == 1u
        };
    };
    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
    }
}

TEST(Tuple, constructor_by_move_mix_implicitly_explicitly_copyable_same_types) {
    using TupleType = hud::Tuple<ImplicitlyCopyConstructible, ExplicitlyCopyConstructible>;

    ASSERT_TRUE((hud::is_explicitly_constructible_v<TupleType, ImplicitlyCopyConstructible&&, ExplicitlyCopyConstructible&&>));

    const auto test = []() {
        ImplicitlyCopyConstructible obj1(1);
        ExplicitlyCopyConstructible obj2(2);
        TupleType tuple(hud::move(obj1), hud::move(obj2));
        return std::tuple{
            hud::hud::get<0>(tuple).id() == 1,
            hud::hud::get<0>(tuple).copy_constructor_count() == 1u,
            hud::hud::get<1>(tuple).id() == 2,
            hud::hud::get<1>(tuple).copy_constructor_count() == 1u
        };
    };
    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
    }
}

TEST(Tuple, constructor_by_move_mix_implicitly_explicitly_copyable_different_types) {
    using TupleType = hud::Tuple<ImplicitlyCopyConstructible2, ExplicitlyCopyConstructible2>;

    ASSERT_TRUE((hud::is_explicitly_constructible_v<TupleType, ImplicitlyCopyConstructible&&, ExplicitlyCopyConstructible&&>));

    const auto test = []() {
        ImplicitlyCopyConstructible obj1(1);
        ExplicitlyCopyConstructible obj2(2);
        TupleType tuple(hud::move(obj1), hud::move(obj2));
        return std::tuple{
            hud::hud::get<0>(tuple).id() == 1,
            hud::hud::get<0>(tuple).copy_constructor_count() == 1u,
            hud::hud::get<1>(tuple).id() == 2,
            hud::hud::get<1>(tuple).copy_constructor_count() == 1u
        };
    };
    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
    }
}

TEST(Tuple, constructor_by_move_explicitly_moveable_same_types) {
    using TupleType = hud::Tuple<ExplicitlyMoveConstructible, ExplicitlyMoveConstructible>;

    ASSERT_TRUE((hud::is_explicitly_constructible_v<TupleType, ExplicitlyMoveConstructible&&, ExplicitlyMoveConstructible&&>));

    const auto test = []() {
        ExplicitlyMoveConstructible obj1(1);
        ExplicitlyMoveConstructible obj2(2);
        TupleType tuple(hud::move(obj1), hud::move(obj2));
        return std::tuple{
            hud::hud::get<0>(tuple).id() == 1,
            hud::hud::get<0>(tuple).move_constructor_count() == 1u,
            hud::hud::get<0>(tuple).copy_constructor_count() == 0u,
            hud::hud::get<1>(tuple).id() == 2,
            hud::hud::get<1>(tuple).move_constructor_count() == 1u,
            hud::hud::get<1>(tuple).copy_constructor_count() == 0u,
        };
    };
    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
        ASSERT_TRUE(hud::get<4>(result));
        ASSERT_TRUE(hud::get<5>(result));
    }

    // Constant
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

TEST(Tuple, constructor_by_move_explicitly_moveable_different_types) {
    using TupleType = hud::Tuple<ExplicitlyMoveConstructible2, ExplicitlyMoveConstructible2>;

    ASSERT_TRUE((hud::is_explicitly_constructible_v<TupleType, ExplicitlyMoveConstructible&&, ExplicitlyMoveConstructible&&>));

    const auto test = []() {
        ExplicitlyMoveConstructible obj1(1);
        ExplicitlyMoveConstructible obj2(2);
        TupleType tuple(hud::move(obj1), hud::move(obj2));
        return std::tuple{
            hud::hud::get<0>(tuple).id() == 1,
            hud::hud::get<0>(tuple).move_constructor_count() == 1u,
            hud::hud::get<0>(tuple).copy_constructor_count() == 0u,
            hud::hud::get<1>(tuple).id() == 2,
            hud::hud::get<1>(tuple).move_constructor_count() == 1u,
            hud::hud::get<1>(tuple).copy_constructor_count() == 0u,
        };
    };
    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
        ASSERT_TRUE(hud::get<4>(result));
        ASSERT_TRUE(hud::get<5>(result));
    }

    // Constant
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

TEST(Tuple, constructor_by_move_implicitly_moveable_same_types) {
    using TupleType = hud::Tuple<ImplicitlyMoveConstructible, ImplicitlyMoveConstructible>;

    ASSERT_TRUE((hud::is_implicitly_constructible_v<TupleType, ImplicitlyMoveConstructible&&, ImplicitlyMoveConstructible&&>));

    const auto test = []() {
        ImplicitlyMoveConstructible obj1(1);
        ImplicitlyMoveConstructible obj2(2);
        TupleType tuple(hud::move(obj1), hud::move(obj2));
        return std::tuple{
            hud::hud::get<0>(tuple).id() == 1,
            hud::hud::get<0>(tuple).move_constructor_count() == 1u,
            hud::hud::get<0>(tuple).copy_constructor_count() == 0u,
            hud::hud::get<1>(tuple).id() == 2,
            hud::hud::get<1>(tuple).move_constructor_count() == 1u,
            hud::hud::get<1>(tuple).copy_constructor_count() == 0u,
        };
    };
    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
        ASSERT_TRUE(hud::get<4>(result));
        ASSERT_TRUE(hud::get<5>(result));
    }

    // Constant
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

TEST(Tuple, constructor_by_move_implicitly_moveable_different_types) {
    using TupleType = hud::Tuple<ImplicitlyMoveConstructible2, ImplicitlyMoveConstructible2>;

    ASSERT_TRUE((hud::is_implicitly_constructible_v<TupleType, ImplicitlyMoveConstructible&&, ImplicitlyMoveConstructible&&>));

    const auto test = []() {
        ImplicitlyMoveConstructible obj1(1);
        ImplicitlyMoveConstructible obj2(2);
        TupleType tuple(hud::move(obj1), hud::move(obj2));
        return std::tuple{
            hud::hud::get<0>(tuple).id() == 1,
            hud::hud::get<0>(tuple).move_constructor_count() == 1u,
            hud::hud::get<0>(tuple).copy_constructor_count() == 0u,
            hud::hud::get<1>(tuple).id() == 2,
            hud::hud::get<1>(tuple).move_constructor_count() == 1u,
            hud::hud::get<1>(tuple).copy_constructor_count() == 0u,
        };
    };
    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
        ASSERT_TRUE(hud::get<4>(result));
        ASSERT_TRUE(hud::get<5>(result));
    }

    // Constant
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

TEST(Tuple, constructor_by_move_mix_implicitly_explicitly_moveable_same_types) {
    using TupleType = hud::Tuple<ImplicitlyMoveConstructible, ExplicitlyMoveConstructible>;

    ASSERT_TRUE((hud::is_explicitly_constructible_v<TupleType, ImplicitlyMoveConstructible&&, ExplicitlyMoveConstructible&&>));

    const auto test = []() {
        ImplicitlyMoveConstructible obj1(1);
        ExplicitlyMoveConstructible obj2(2);
        TupleType tuple(hud::move(obj1), hud::move(obj2));
        return std::tuple{
            hud::hud::get<0>(tuple).id() == 1,
            hud::hud::get<0>(tuple).move_constructor_count() == 1u,
            hud::hud::get<0>(tuple).copy_constructor_count() == 0u,
            hud::hud::get<1>(tuple).id() == 2,
            hud::hud::get<1>(tuple).move_constructor_count() == 1u,
            hud::hud::get<1>(tuple).copy_constructor_count() == 0u,
        };
    };
    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
        ASSERT_TRUE(hud::get<4>(result));
        ASSERT_TRUE(hud::get<5>(result));
    }

    // Constant
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

TEST(Tuple, constructor_by_move_mix_implicitly_explicitly_moveable_different_types) {
    using TupleType = hud::Tuple<ImplicitlyMoveConstructible2, ExplicitlyMoveConstructible2>;

    ASSERT_TRUE((hud::is_explicitly_constructible_v<TupleType, ImplicitlyMoveConstructible&&, ExplicitlyMoveConstructible&&>));

    const auto test = []() {
        ImplicitlyMoveConstructible obj1(1);
        ExplicitlyMoveConstructible obj2(2);
        TupleType tuple(hud::move(obj1), hud::move(obj2));
        return std::tuple{
            hud::hud::get<0>(tuple).id() == 1,
            hud::hud::get<0>(tuple).move_constructor_count() == 1u,
            hud::hud::get<0>(tuple).copy_constructor_count() == 0u,
            hud::hud::get<1>(tuple).id() == 2,
            hud::hud::get<1>(tuple).move_constructor_count() == 1u,
            hud::hud::get<1>(tuple).copy_constructor_count() == 0u,
        };
    };
    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
        ASSERT_TRUE(hud::get<4>(result));
        ASSERT_TRUE(hud::get<5>(result));
    }

    // Constant
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

TEST(Tuple, constructor_by_copy_pair_of_explictly_copyable_same_types) {
    using PairType = hud::Pair<ExplicitlyCopyConstructible, ExplicitlyCopyConstructible>;
    using TupleType = hud::Tuple<ExplicitlyCopyConstructible, ExplicitlyCopyConstructible>;


    ASSERT_TRUE((hud::is_explicitly_constructible_v<TupleType, const PairType&>));

    const auto test = []() {
        const PairType pair{ 1,2 };
        const TupleType tuple{ pair };
        return std::tuple{
            hud::hud::get<0>(tuple).id(),
            hud::hud::get<0>(tuple).copy_constructor_count() == 1u,
            hud::hud::get<1>(tuple).id(),
            hud::hud::get<1>(tuple).copy_constructor_count() == 1u,
        };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
    }

}

TEST(Tuple, constructor_by_copy_pair_of_explictly_copyable_different_types) {
    using PairType = hud::Pair<ExplicitlyCopyConstructible, ExplicitlyCopyConstructible>;
    using TupleType = hud::Tuple<ExplicitlyCopyConstructible2, ExplicitlyCopyConstructible2>;


    ASSERT_TRUE((hud::is_explicitly_constructible_v<TupleType, const PairType&>));

    const auto test = []() {
        const PairType pair{1,2};
        const TupleType tuple{ pair };
        return std::tuple{
            hud::hud::get<0>(tuple).id(),
            hud::hud::get<0>(tuple).copy_constructor_count() == 1u,
            hud::hud::get<1>(tuple).id(),
            hud::hud::get<1>(tuple).copy_constructor_count() == 1u,
        };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
    }
}

TEST(Tuple, constructor_by_copy_pair_of_implictly_copyable_same_types) {
    using PairType = hud::Pair<ImplicitlyCopyConstructible, ImplicitlyCopyConstructible>;
    using TupleType = hud::Tuple<ImplicitlyCopyConstructible, ImplicitlyCopyConstructible>;


    ASSERT_TRUE((hud::is_implicitly_constructible_v<TupleType, const PairType&>));

    const auto test = []() {
        const PairType pair{ 1,2 };
        const TupleType tuple{ pair };
        return std::tuple{
            hud::hud::get<0>(tuple).id(),
            hud::hud::get<0>(tuple).copy_constructor_count() == 1u,
            hud::hud::get<1>(tuple).id(),
            hud::hud::get<1>(tuple).copy_constructor_count() == 1u,
        };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
    }
}

TEST(Tuple, constructor_by_copy_pair_of_implictly_copyable_different_types) {
    using PairType = hud::Pair<ImplicitlyCopyConstructible, ImplicitlyCopyConstructible>;
    using TupleType = hud::Tuple<ImplicitlyCopyConstructible2, ImplicitlyCopyConstructible2>;


    ASSERT_TRUE((hud::is_implicitly_constructible_v<TupleType, const PairType&>));

    const auto test = []() {
        const PairType pair{ 1,2 };
        const TupleType tuple{ pair };
        return std::tuple{
            hud::hud::get<0>(tuple).id(),
            hud::hud::get<0>(tuple).copy_constructor_count() == 1u,
            hud::hud::get<1>(tuple).id(),
            hud::hud::get<1>(tuple).copy_constructor_count() == 1u,
        };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
    }
}

TEST(Tuple, constructor_by_copy_pair_of_mix_implictly_and_explitly_copyable_same_types) {
    using PairType = hud::Pair<ImplicitlyCopyConstructible, ExplicitlyCopyConstructible>;
    using TupleType = hud::Tuple<ImplicitlyCopyConstructible, ExplicitlyCopyConstructible>;


    ASSERT_TRUE((hud::is_explicitly_constructible_v<TupleType, const PairType&>));

    const auto test = []() {
        const PairType pair{ 1,2 };
        const TupleType tuple{ pair };
        return std::tuple{
            hud::hud::get<0>(tuple).id(),
            hud::hud::get<0>(tuple).copy_constructor_count() == 1u,
            hud::hud::get<1>(tuple).id(),
            hud::hud::get<1>(tuple).copy_constructor_count() == 1u,
        };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
    }
}

TEST(Tuple, constructor_by_copy_pair_of_mix_implictly_and_explitly_copyable_different_types) {
    using PairType = hud::Pair<ImplicitlyCopyConstructible, ExplicitlyCopyConstructible>;
    using TupleType = hud::Tuple<ImplicitlyCopyConstructible2, ExplicitlyCopyConstructible2>;


    ASSERT_TRUE((hud::is_explicitly_constructible_v<TupleType, const PairType&>));

    const auto test = []() {
        const PairType pair{ 1,2 };
        const TupleType tuple{ pair };
        return std::tuple{
            hud::hud::get<0>(tuple).id(),
            hud::hud::get<0>(tuple).copy_constructor_count() == 1u,
            hud::hud::get<1>(tuple).id(),
            hud::hud::get<1>(tuple).copy_constructor_count() == 1u,
        };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
    }
}

TEST(Tuple, constructor_by_move_pair_of_explictly_copyable_same_types) {
    using PairType = hud::Pair<ExplicitlyCopyConstructible, ExplicitlyCopyConstructible>;
    using TupleType = hud::Tuple<ExplicitlyCopyConstructible, ExplicitlyCopyConstructible>;


    ASSERT_TRUE((hud::is_explicitly_constructible_v<TupleType, PairType&&>));

    const auto test = []() {
        PairType pair{ 1,2 };
        const TupleType tuple{ hud::move(pair) };
        return std::tuple{
            hud::hud::get<0>(tuple).id(),
            hud::hud::get<0>(tuple).copy_constructor_count() == 1u,
            hud::hud::get<1>(tuple).id(),
            hud::hud::get<1>(tuple).copy_constructor_count() == 1u,
        };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
    }

}

TEST(Tuple, constructor_by_move_pair_of_explictly_copyable_different_types) {
    using PairType = hud::Pair<ExplicitlyCopyConstructible, ExplicitlyCopyConstructible>;
    using TupleType = hud::Tuple<ExplicitlyCopyConstructible2, ExplicitlyCopyConstructible2>;


    ASSERT_TRUE((hud::is_explicitly_constructible_v<TupleType, PairType&&>));

    const auto test = []() {
        PairType pair{ 1,2 };
        const TupleType tuple{ hud::move(pair) };
        return std::tuple{
            hud::hud::get<0>(tuple).id(),
            hud::hud::get<0>(tuple).copy_constructor_count() == 1u,
            hud::hud::get<1>(tuple).id(),
            hud::hud::get<1>(tuple).copy_constructor_count() == 1u,
        };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
    }
}

TEST(Tuple, constructor_by_move_pair_of_implictly_copyable_same_types) {
    using PairType = hud::Pair<ImplicitlyCopyConstructible, ImplicitlyCopyConstructible>;
    using TupleType = hud::Tuple<ImplicitlyCopyConstructible, ImplicitlyCopyConstructible>;


    ASSERT_TRUE((hud::is_implicitly_constructible_v<TupleType, PairType&&>));

    const auto test = []() {
        PairType pair{ 1,2 };
        const TupleType tuple{ hud::move(pair) };
        return std::tuple{
            hud::hud::get<0>(tuple).id(),
            hud::hud::get<0>(tuple).copy_constructor_count() == 1u,
            hud::hud::get<1>(tuple).id(),
            hud::hud::get<1>(tuple).copy_constructor_count() == 1u,
        };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
    }
}

TEST(Tuple, constructor_by_move_pair_of_implictly_copyable_different_types) {
    using PairType = hud::Pair<ImplicitlyCopyConstructible, ImplicitlyCopyConstructible>;
    using TupleType = hud::Tuple<ImplicitlyCopyConstructible2, ImplicitlyCopyConstructible2>;


    ASSERT_TRUE((hud::is_implicitly_constructible_v<TupleType, PairType&&>));

    const auto test = []() {
        PairType pair{ 1,2 };
        const TupleType tuple{ hud::move(pair) };
        return std::tuple{
            hud::hud::get<0>(tuple).id(),
            hud::hud::get<0>(tuple).copy_constructor_count() == 1u,
            hud::hud::get<1>(tuple).id(),
            hud::hud::get<1>(tuple).copy_constructor_count() == 1u,
        };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
    }
}

TEST(Tuple, constructor_by_move_pair_of_mix_implictly_and_explitly_copyable_same_types) {
    using PairType = hud::Pair<ImplicitlyCopyConstructible, ExplicitlyCopyConstructible>;
    using TupleType = hud::Tuple<ImplicitlyCopyConstructible, ExplicitlyCopyConstructible>;


    ASSERT_TRUE((hud::is_explicitly_constructible_v<TupleType, PairType&&>));

    const auto test = []() {
        PairType pair{ 1,2 };
        const TupleType tuple{ hud::move(pair) };
        return std::tuple{
            hud::hud::get<0>(tuple).id(),
            hud::hud::get<0>(tuple).copy_constructor_count() == 1u,
            hud::hud::get<1>(tuple).id(),
            hud::hud::get<1>(tuple).copy_constructor_count() == 1u,
        };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
    }
}

TEST(Tuple, constructor_by_move_pair_of_mix_implictly_and_explitly_copyable_different_types) {
    using PairType = hud::Pair<ImplicitlyCopyConstructible, ExplicitlyCopyConstructible>;
    using TupleType = hud::Tuple<ImplicitlyCopyConstructible2, ExplicitlyCopyConstructible2>;


    ASSERT_TRUE((hud::is_explicitly_constructible_v<TupleType, PairType&&>));

    const auto test = []() {
        PairType pair{ 1,2 };
        const TupleType tuple{ hud::move(pair) };
        return std::tuple{
            hud::hud::get<0>(tuple).id(),
            hud::hud::get<0>(tuple).copy_constructor_count() == 1u,
            hud::hud::get<1>(tuple).id(),
            hud::hud::get<1>(tuple).copy_constructor_count() == 1u,
        };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
    }
}

TEST(Tuple, constructor_by_move_pair_of_explictly_moveable_same_types) {
    using PairType = hud::Pair<ExplicitlyMoveConstructible, ExplicitlyMoveConstructible>;
    using TupleType = hud::Tuple<ExplicitlyMoveConstructible, ExplicitlyMoveConstructible>;


   // ASSERT_TRUE((hud::is_explicitly_constructible_v<TupleType, PairType&&>));
    static_assert((hud::is_explicitly_move_constructible_v<ExplicitlyMoveConstructible, ExplicitlyMoveConstructible>));
    static_assert((hud::details::IsPairExplictlyMoveConstructibleToTupleV < PairType, TupleType>));
    const auto test = []() {
        PairType pair{ 1,2 };
        const TupleType tuple{ hud::move(pair) };
        return std::tuple{
            hud::hud::get<0>(tuple).id(),
            hud::hud::get<0>(tuple).copy_constructor_count() == 0u,
            hud::hud::get<0>(tuple).move_constructor_count() == 1u,
            hud::hud::get<1>(tuple).id(),
            hud::hud::get<1>(tuple).copy_constructor_count() == 0u,
            hud::hud::get<1>(tuple).move_constructor_count() == 1u,
        };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
        ASSERT_TRUE(hud::get<4>(result));
        ASSERT_TRUE(hud::get<5>(result));
    }

    // Constant
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

TEST(Tuple, constructor_by_move_pair_of_explictly_moveable_different_types) {
    using PairType = hud::Pair<ExplicitlyMoveConstructible, ExplicitlyMoveConstructible>;
    using TupleType = hud::Tuple<ExplicitlyMoveConstructible2, ExplicitlyMoveConstructible2>;


    ASSERT_TRUE((hud::is_explicitly_constructible_v<TupleType, PairType&&>));

    const auto test = []() {
        PairType pair{ 1,2 };
        const TupleType tuple{ hud::move(pair) };
        return std::tuple{
            hud::hud::get<0>(tuple).id(),
            hud::hud::get<0>(tuple).copy_constructor_count() == 0u,
            hud::hud::get<0>(tuple).move_constructor_count() == 1u,
            hud::hud::get<1>(tuple).id(),
            hud::hud::get<1>(tuple).copy_constructor_count() == 0u,
            hud::hud::get<1>(tuple).move_constructor_count() == 1u,
        };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
        ASSERT_TRUE(hud::get<4>(result));
        ASSERT_TRUE(hud::get<5>(result));
    }

    // Constant
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

TEST(Tuple, constructor_by_move_pair_of_implictly_moveable_same_types) {
    using PairType = hud::Pair<ImplicitlyMoveConstructible, ImplicitlyMoveConstructible>;
    using TupleType = hud::Tuple<ImplicitlyMoveConstructible, ImplicitlyMoveConstructible>;


    ASSERT_TRUE((hud::is_implicitly_constructible_v<TupleType, PairType&&>));

    const auto test = []() {
        PairType pair{ 1,2 };
        const TupleType tuple{ hud::move(pair) };
        return std::tuple{
            hud::hud::get<0>(tuple).id(),
            hud::hud::get<0>(tuple).copy_constructor_count() == 0u,
            hud::hud::get<0>(tuple).move_constructor_count() == 1u,
            hud::hud::get<1>(tuple).id(),
            hud::hud::get<1>(tuple).copy_constructor_count() == 0u,
            hud::hud::get<1>(tuple).move_constructor_count() == 1u,
        };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
        ASSERT_TRUE(hud::get<4>(result));
        ASSERT_TRUE(hud::get<5>(result));
    }

    // Constant
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

TEST(Tuple, constructor_by_move_pair_of_implictly_moveable_different_types) {
    using PairType = hud::Pair<ImplicitlyMoveConstructible, ImplicitlyMoveConstructible>;
    using TupleType = hud::Tuple<ImplicitlyMoveConstructible2, ImplicitlyMoveConstructible2>;


    ASSERT_TRUE((hud::is_implicitly_constructible_v<TupleType, PairType&&>));

    const auto test = []() {
        PairType pair{ 1,2 };
        const TupleType tuple{ hud::move(pair) };
        return std::tuple{
            hud::hud::get<0>(tuple).id(),
            hud::hud::get<0>(tuple).copy_constructor_count() == 0u,
            hud::hud::get<0>(tuple).move_constructor_count() == 1u,
            hud::hud::get<1>(tuple).id(),
            hud::hud::get<1>(tuple).copy_constructor_count() == 0u,
            hud::hud::get<1>(tuple).move_constructor_count() == 1u,
        };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
        ASSERT_TRUE(hud::get<4>(result));
        ASSERT_TRUE(hud::get<5>(result));
    }

    // Constant
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

TEST(Tuple, constructor_by_move_pair_of_mix_implictly_and_explitly_moveable_same_types) {
    using PairType = hud::Pair<ImplicitlyMoveConstructible, ExplicitlyMoveConstructible>;
    using TupleType = hud::Tuple<ImplicitlyMoveConstructible, ExplicitlyMoveConstructible>;


    ASSERT_TRUE((hud::is_explicitly_constructible_v<TupleType, PairType&&>));

    const auto test = []() {
        PairType pair{ 1,2 };
        const TupleType tuple{ hud::move(pair) };
        return std::tuple{
            hud::hud::get<0>(tuple).id(),
            hud::hud::get<0>(tuple).copy_constructor_count() == 0u,
            hud::hud::get<0>(tuple).move_constructor_count() == 1u,
            hud::hud::get<1>(tuple).id(),
            hud::hud::get<1>(tuple).copy_constructor_count() == 0u,
            hud::hud::get<1>(tuple).move_constructor_count() == 1u,
        };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
        ASSERT_TRUE(hud::get<4>(result));
        ASSERT_TRUE(hud::get<5>(result));
    }

    // Constant
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

TEST(Tuple, constructor_by_move_pair_of_mix_implictly_and_explitly_moveable_different_types) {
    using PairType = hud::Pair<ImplicitlyMoveConstructible, ExplicitlyMoveConstructible>;
    using TupleType = hud::Tuple<ImplicitlyMoveConstructible2, ExplicitlyMoveConstructible2>;


    ASSERT_TRUE((hud::is_explicitly_constructible_v<TupleType, PairType&&>));

    const auto test = []() {
        PairType pair{ 1,2 };
        const TupleType tuple{ hud::move(pair) };
        return std::tuple{
            hud::hud::get<0>(tuple).id(),
            hud::hud::get<0>(tuple).copy_constructor_count() == 0u,
            hud::hud::get<0>(tuple).move_constructor_count() == 1u,
            hud::hud::get<1>(tuple).id(),
            hud::hud::get<1>(tuple).copy_constructor_count() == 0u,
            hud::hud::get<1>(tuple).move_constructor_count() == 1u,
        };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
        ASSERT_TRUE(hud::get<4>(result));
        ASSERT_TRUE(hud::get<5>(result));
    }

    // Constant
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

TEST(Tuple, copy_constructor_copyable_same_types) {
    using TupleType = hud::Tuple<ExplicitlyCopyConstructible, ImplicitlyCopyConstructible>;

    ASSERT_TRUE((hud::is_explicitly_constructible_v<TupleType, const ExplicitlyCopyConstructible&, const ImplicitlyCopyConstructible&>));

    const auto test = []() {
        const TupleType to_copy{ 1,2 };
        const TupleType copy{ to_copy };
        return std::tuple{
            hud::hud::get<0>(copy).id() == 1,
            hud::hud::get<0>(copy).copy_constructor_count() == 1u,
            hud::hud::get<1>(copy).id() == 2,
            hud::hud::get<1>(copy).copy_constructor_count() == 1u,
        };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
    }
}

TEST(Tuple, copy_constructor_copyable_different_types) {
    using TupleType = hud::Tuple<ExplicitlyCopyConstructible2, ImplicitlyCopyConstructible2>;
    using OtherTupleType = hud::Tuple<ExplicitlyCopyConstructible, ImplicitlyCopyConstructible>;

    ASSERT_TRUE((hud::is_explicitly_constructible_v<TupleType, const ExplicitlyCopyConstructible&, const ImplicitlyCopyConstructible&>));

    const auto test = []() {
        const OtherTupleType to_copy{ 1,2 };
        const TupleType copy{ to_copy };
        return std::tuple{
            hud::hud::get<0>(copy).id() == 1,
            hud::hud::get<0>(copy).copy_constructor_count() == 1u,
            hud::hud::get<1>(copy).id() == 2,
            hud::hud::get<1>(copy).copy_constructor_count() == 1u,
        };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
    }
}

TEST(Tuple, copy_constructor_moveable_same_types) {
    using TupleType = hud::Tuple<ExplicitlyMoveConstructible, ImplicitlyMoveConstructible>;

    ASSERT_TRUE((hud::is_explicitly_constructible_v<TupleType, const ExplicitlyMoveConstructible&, const ExplicitlyMoveConstructible&>));

    const auto test = []() {
        const TupleType to_copy{ 1,2 };
        const TupleType copy{ to_copy };
        return std::tuple{
            hud::hud::get<0>(copy).id() == 1,
            hud::hud::get<0>(copy).copy_constructor_count() == 1u,
            hud::hud::get<0>(copy).move_constructor_count() == 0u,
            hud::hud::get<1>(copy).id() == 2,
            hud::hud::get<1>(copy).copy_constructor_count() == 1u,
            hud::hud::get<1>(copy).move_constructor_count() == 0u,
        };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
        ASSERT_TRUE(hud::get<4>(result));
        ASSERT_TRUE(hud::get<5>(result));
    }

    // Constant
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

TEST(Tuple, copy_constructor_moveable_different_types) {
    using TupleType = hud::Tuple<ExplicitlyMoveConstructible2, ImplicitlyMoveConstructible2>;
    using OtherTupleType = hud::Tuple<ExplicitlyMoveConstructible, ImplicitlyMoveConstructible>;

    ASSERT_TRUE((hud::is_explicitly_constructible_v<TupleType, const ExplicitlyMoveConstructible&, const ImplicitlyMoveConstructible&>));

    const auto test = []() {
        const OtherTupleType to_copy{ 1,2 };
        const TupleType copy{ to_copy };
        return std::tuple{
            hud::hud::get<0>(copy).id() == 1,
            hud::hud::get<0>(copy).copy_constructor_count() == 1u,
            hud::hud::get<0>(copy).move_constructor_count() == 0u,
            hud::hud::get<1>(copy).id() == 2,
            hud::hud::get<1>(copy).copy_constructor_count() == 1u,
            hud::hud::get<1>(copy).move_constructor_count() == 0u,
        };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
        ASSERT_TRUE(hud::get<4>(result));
        ASSERT_TRUE(hud::get<5>(result));
    }

    // Constant
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

TEST(Tuple, move_constructor_copyable_same_types) {
    using TupleType = hud::Tuple<ExplicitlyCopyConstructible, ImplicitlyCopyConstructible>;

    ASSERT_TRUE((hud::is_explicitly_constructible_v<TupleType, ExplicitlyCopyConstructible&&, ImplicitlyCopyConstructible&&>));

    const auto test = []() {
        TupleType to_copy{ 1,2 };
        const TupleType copy{ hud::move(to_copy) };
        return std::tuple{
            hud::hud::get<0>(copy).id() == 1,
            hud::hud::get<0>(copy).copy_constructor_count() == 1u,
            hud::hud::get<1>(copy).id() == 2,
            hud::hud::get<1>(copy).copy_constructor_count() == 1u,
        };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
    }
}

TEST(Tuple, move_constructor_copyable_different_types) {
    using TupleType = hud::Tuple<ExplicitlyCopyConstructible2, ImplicitlyCopyConstructible2>;
    using OtherTupleType = hud::Tuple<ExplicitlyCopyConstructible, ImplicitlyCopyConstructible>;

    ASSERT_TRUE((hud::is_explicitly_constructible_v<TupleType, ExplicitlyCopyConstructible&&, ImplicitlyCopyConstructible&&>));

    const auto test = []() {
        OtherTupleType to_copy{ 1,2 };
        const TupleType copy{ hud::move(to_copy) };
        return std::tuple{
            hud::hud::get<0>(copy).id() == 1,
            hud::hud::get<0>(copy).copy_constructor_count() == 1u,
            hud::hud::get<1>(copy).id() == 2,
            hud::hud::get<1>(copy).copy_constructor_count() == 1u,
        };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
    }
}

TEST(Tuple, move_constructor_moveable_same_types) {
    using TupleType = hud::Tuple<ExplicitlyMoveConstructible, ImplicitlyMoveConstructible>;

    ASSERT_TRUE((hud::is_explicitly_constructible_v<TupleType, ExplicitlyMoveConstructible&&, ImplicitlyMoveConstructible&&>));

    const auto test = []() {
        TupleType to_copy{ 1,2 };
        const TupleType copy{ hud::move(to_copy) };
        return std::tuple{
            hud::hud::get<0>(copy).id() == 1,
            hud::hud::get<0>(copy).copy_constructor_count() == 0u,
            hud::hud::get<0>(copy).move_constructor_count() == 1u,
            hud::hud::get<1>(copy).id() == 2,
            hud::hud::get<1>(copy).copy_constructor_count() == 0u,
            hud::hud::get<1>(copy).move_constructor_count() == 1u,
        };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
        ASSERT_TRUE(hud::get<4>(result));
        ASSERT_TRUE(hud::get<5>(result));
    }

    // Constant
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

TEST(Tuple, move_constructor_moveable_different_types) {
    using TupleType = hud::Tuple<ExplicitlyMoveConstructible2, ImplicitlyMoveConstructible2>;
    using OtherTupleType = hud::Tuple<ExplicitlyMoveConstructible, ImplicitlyMoveConstructible>;

    ASSERT_TRUE((hud::is_explicitly_constructible_v<TupleType, ExplicitlyMoveConstructible&&, ImplicitlyMoveConstructible&&>));

    const auto test = []() {
        OtherTupleType to_copy{ 1,2 };
        const TupleType copy{ hud::move(to_copy) };
        return std::tuple{
            hud::hud::get<0>(copy).id() == 1,
            hud::hud::get<0>(copy).copy_constructor_count() == 0u,
            hud::hud::get<0>(copy).move_constructor_count() == 1u,
            hud::hud::get<1>(copy).id() == 2,
            hud::hud::get<1>(copy).copy_constructor_count() == 0u,
            hud::hud::get<1>(copy).move_constructor_count() == 1u,
        };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
        ASSERT_TRUE(hud::get<4>(result));
        ASSERT_TRUE(hud::get<5>(result));
    }

    // Constant
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
