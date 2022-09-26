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
        : public hud_test::default_constructible_type
    {
        constexpr explicit ExplicitDefaultConstructible() noexcept
            : hud_test::default_constructible_type() {
        }
    };
    static_assert(hud::is_explicitly_default_constructible_v<ExplicitDefaultConstructible>);

    struct ImplicitDefaultConstructible
        : public hud_test::default_constructible_type
    {
        constexpr ImplicitDefaultConstructible() noexcept
            : hud_test::default_constructible_type() {
        }
    };

    static_assert(hud::is_implicitly_default_constructible_v<ImplicitDefaultConstructible>);

    struct ExplicitlyCopyConstructible
        : public hud_test::NonBitwiseCopyConstructibleType {

        constexpr ExplicitlyCopyConstructible(i32 id) noexcept
            : hud_test::NonBitwiseCopyConstructibleType(id) {
        }

        explicit constexpr ExplicitlyCopyConstructible(const ExplicitlyCopyConstructible& other) noexcept
            : hud_test::NonBitwiseCopyConstructibleType(other) {
        }
    };
    
    static_assert(hud::is_explicitly_copy_constructible_v<ExplicitlyCopyConstructible>);

    struct ExplicitlyCopyConstructible2
        : public hud_test::NonBitwiseCopyConstructibleType2 {

        constexpr ExplicitlyCopyConstructible2(i32 id) noexcept
            : hud_test::NonBitwiseCopyConstructibleType2(id) {
        }

        explicit constexpr ExplicitlyCopyConstructible2(const ExplicitlyCopyConstructible& other) noexcept
            : hud_test::NonBitwiseCopyConstructibleType2(other) {
        }
    };
    static_assert(hud::is_explicitly_copy_constructible_v<ExplicitlyCopyConstructible2, ExplicitlyCopyConstructible>);

    struct ImplicitlyCopyConstructible
        : public hud_test::NonBitwiseCopyConstructibleType {

        constexpr ImplicitlyCopyConstructible(i32 id) noexcept
            : hud_test::NonBitwiseCopyConstructibleType(id) {
        }

        constexpr ImplicitlyCopyConstructible(const ImplicitlyCopyConstructible& other) noexcept
            : hud_test::NonBitwiseCopyConstructibleType(other) {
        }
    };
    static_assert(hud::is_implicitly_copy_constructible_v<ImplicitlyCopyConstructible>);

    struct ImplicitlyCopyConstructible2
        : public hud_test::NonBitwiseCopyConstructibleType2 {

        constexpr ImplicitlyCopyConstructible2(i32 id) noexcept
            : hud_test::NonBitwiseCopyConstructibleType2(id) {
        }

        constexpr ImplicitlyCopyConstructible2(const ImplicitlyCopyConstructible& other) noexcept
            : hud_test::NonBitwiseCopyConstructibleType2(other) {
        }
    };
    static_assert(hud::is_implicitly_copy_constructible_v<ImplicitlyCopyConstructible2, ImplicitlyCopyConstructible>);

    struct ExplicitlyMoveConstructible
        : public hud_test::NonBitwiseMoveConstructibleType {

        constexpr ExplicitlyMoveConstructible(i32 id) noexcept
            : hud_test::NonBitwiseMoveConstructibleType(id) {
        }

        explicit constexpr ExplicitlyMoveConstructible(const ExplicitlyMoveConstructible& other) noexcept
            : hud_test::NonBitwiseMoveConstructibleType(other) {
        }

        explicit constexpr ExplicitlyMoveConstructible(ExplicitlyMoveConstructible&& other) noexcept
            : hud_test::NonBitwiseMoveConstructibleType(hud::move(other)) {
        }
    };
    static_assert(hud::is_explicitly_move_constructible_v<ExplicitlyMoveConstructible>);

    struct ExplicitlyMoveConstructible2
        : public hud_test::NonBitwiseMoveConstructibleType2 {

        constexpr ExplicitlyMoveConstructible2(i32 id) noexcept
            : hud_test::NonBitwiseMoveConstructibleType2(id) {
        }

        explicit constexpr ExplicitlyMoveConstructible2(const ExplicitlyMoveConstructible& other) noexcept
            : hud_test::NonBitwiseMoveConstructibleType2(other) {
        }

        explicit constexpr ExplicitlyMoveConstructible2(ExplicitlyMoveConstructible&& other) noexcept
            : hud_test::NonBitwiseMoveConstructibleType2(hud::move(other)) {
        }
    };
    static_assert(hud::is_explicitly_move_constructible_v<ExplicitlyMoveConstructible2, ExplicitlyMoveConstructible>);

    struct ImplicitlyMoveConstructible
        : public hud_test::NonBitwiseMoveConstructibleType {

        constexpr ImplicitlyMoveConstructible(i32 id) noexcept
            : hud_test::NonBitwiseMoveConstructibleType(id) {
        }

        constexpr ImplicitlyMoveConstructible(const ImplicitlyMoveConstructible& other) noexcept
            : hud_test::NonBitwiseMoveConstructibleType(other) {
        }

        constexpr ImplicitlyMoveConstructible(ImplicitlyMoveConstructible&& other) noexcept
            : hud_test::NonBitwiseMoveConstructibleType(hud::move(other)) {
        }
    };

    static_assert(hud::is_implicitly_move_constructible_v<ImplicitlyMoveConstructible>);

    struct ImplicitlyMoveConstructible2
        : public hud_test::NonBitwiseMoveConstructibleType2 {

        constexpr ImplicitlyMoveConstructible2(i32 id) noexcept
            : hud_test::NonBitwiseMoveConstructibleType2(id) {
        }

        constexpr ImplicitlyMoveConstructible2(const ImplicitlyMoveConstructible& other) noexcept
            : hud_test::NonBitwiseMoveConstructibleType2(other) {
        }

        constexpr ImplicitlyMoveConstructible2(ImplicitlyMoveConstructible&& other) noexcept
            : hud_test::NonBitwiseMoveConstructibleType2(hud::move(other)) {
        }
    };
    static_assert(hud::is_implicitly_move_constructible_v<ImplicitlyMoveConstructible2, ImplicitlyMoveConstructible>);
}


TEST(tuple, default_constructor_uninit) {
    using tuple_type = hud::tuple<>;
    using tuple_type2 = hud::tuple<i32, i32, i32>;
    using tuple_type3 = hud::tuple<i32, f32, wchar>;
    using tuple_type4 = hud::tuple<ExplicitDefaultConstructible, ImplicitDefaultConstructible>;
    using tuple_type5 = hud::tuple<ExplicitDefaultConstructible, i32>;

    ASSERT_EQ(hud::tuple_size_v<tuple_type>, 0u);
    ASSERT_EQ(hud::tuple_size_v<tuple_type2>, 3u);
    ASSERT_EQ(hud::tuple_size_v<tuple_type3>, 3u);
    ASSERT_EQ(hud::tuple_size_v<tuple_type4>, 2u);
    ASSERT_EQ(hud::tuple_size_v<tuple_type5>, 2u);

    // Non constant
    {
        [[maybe_unused]] tuple_type tuple1;

        tuple_type2* tuple2 = hud::memory::allocate_array<tuple_type2>(1);
        hud::memory::set(tuple2, sizeof(tuple_type2), 0xFF);
        hud::memory::construct_at(tuple2);
        ASSERT_TRUE((hud::is_same_v<decltype(hud::get<0>(*tuple2)), i32&>));
        ASSERT_TRUE((hud::is_same_v<decltype(hud::get<1>(*tuple2)), i32&>));
        ASSERT_TRUE((hud::is_same_v<decltype(hud::get<2>(*tuple2)), i32&>));
        ASSERT_NE((hud::get<0>(*tuple2)), i32{});
        ASSERT_NE((hud::get<1>(*tuple2)), i32{});
        ASSERT_NE((hud::get<2>(*tuple2)), i32{});
        hud::memory::destroy_array(tuple2, 1);
        hud::memory::free(tuple2);

        tuple_type3* tuple3 = hud::memory::allocate_array<tuple_type3>(1);
        hud::memory::set(tuple3, sizeof(tuple_type3), 0xFF);
        hud::memory::construct_at(tuple3);
        ASSERT_TRUE((hud::is_same_v<decltype(hud::get<0>(*tuple3)), i32&>));
        ASSERT_TRUE((hud::is_same_v<decltype(hud::get<1>(*tuple3)), f32&>));
        ASSERT_TRUE((hud::is_same_v<decltype(hud::get<2>(*tuple3)), wchar&>));
        ASSERT_NE((hud::get<0>(*tuple3)), i32{});

        // Is nan always return false with clang in release with fast math enabled
        // See: https://clang.llvm.org/docs/UsersManual.html#cmdoption-ffast-math
        #if defined(HD_COMPILER_CLANG) && defined(HD_RELEASE) && defined(__FAST_MATH__)
        ASSERT_FALSE(isnan(hud::get<1>(*tuple3)));
        #else
        ASSERT_TRUE(isnan(hud::get<1>(*tuple3)));
        #endif
        ASSERT_NE((hud::get<2>(*tuple3)), wchar{});
        hud::memory::destroy_array(tuple3, 1);
        hud::memory::free(tuple3);

        tuple_type4* tuple4 = hud::memory::allocate_array<tuple_type4>(1);
        hud::memory::set(tuple4, sizeof(tuple_type4), 0xFF);
        hud::memory::construct_at(tuple4);
        ASSERT_TRUE((hud::is_same_v<decltype(hud::get<0>(*tuple4)), ExplicitDefaultConstructible&>));
        ASSERT_TRUE((hud::is_same_v<decltype(hud::get<1>(*tuple4)), ImplicitDefaultConstructible&>));
        ASSERT_EQ((hud::get<0>(*tuple4).id()), ExplicitDefaultConstructible::DEFAULT_ID_VALUE);
        ASSERT_EQ((hud::get<0>(*tuple4).default_constructor_count()), 1u);
        ASSERT_EQ((hud::get<0>(*tuple4).constructor_count()), 0u);
        ASSERT_EQ((hud::get<0>(*tuple4).copy_constructor_count()), 0u);
        ASSERT_EQ((hud::get<0>(*tuple4).move_constructor_count()), 0u);
        ASSERT_EQ((hud::get<1>(*tuple4).id()), ImplicitDefaultConstructible::DEFAULT_ID_VALUE);
        ASSERT_EQ((hud::get<1>(*tuple4).default_constructor_count()), 1u);
        ASSERT_EQ((hud::get<1>(*tuple4).constructor_count()), 0u);
        ASSERT_EQ((hud::get<1>(*tuple4).copy_constructor_count()), 0u);
        ASSERT_EQ((hud::get<1>(*tuple4).move_constructor_count()), 0u);
        hud::memory::destroy_array(tuple4, 1);
        hud::memory::free(tuple4);

        tuple_type5* tuple5 = hud::memory::allocate_array<tuple_type5>(1);
        hud::memory::set(tuple5, sizeof(tuple_type5), 0xFF);
        hud::memory::construct_at(tuple5);
        ASSERT_TRUE((hud::is_same_v<decltype(hud::get<0>(*tuple5)), ExplicitDefaultConstructible&>));
        ASSERT_TRUE((hud::is_same_v<decltype(hud::get<1>(*tuple5)), i32&>));
        ASSERT_EQ((hud::get<0>(*tuple5).id()), ExplicitDefaultConstructible::DEFAULT_ID_VALUE);
        ASSERT_EQ((hud::get<0>(*tuple5).default_constructor_count()), 1u);
        ASSERT_EQ((hud::get<0>(*tuple5).constructor_count()), 0u);
        ASSERT_EQ((hud::get<0>(*tuple5).copy_constructor_count()), 0u);
        ASSERT_EQ((hud::get<0>(*tuple5).move_constructor_count()), 0u);
        ASSERT_NE(hud::get<1>(*tuple5), i32{});
        hud::memory::destroy_array(tuple5, 1);
        hud::memory::free(tuple5);

    }

    // Constant
    // Non empty uninit constexpr tuple is not possible because constexpr need initialized value
    {
        [[maybe_unused]] constexpr tuple_type tuple;
        ASSERT_EQ(hud::tuple_size_v<tuple_type>, 0u);
    }
}

TEST(tuple, default_constructor_init) {
    using tuple_type = hud::tuple<>;
    using tuple_type2 = hud::tuple<i32, i32, i32>;
    using tuple_type3 = hud::tuple<i32, f32, wchar>;
    using tuple_type4 = hud::tuple<ExplicitDefaultConstructible, ImplicitDefaultConstructible>;
    using tuple_type5 = hud::tuple<ExplicitDefaultConstructible, i32>;

    ASSERT_EQ(hud::tuple_size_v<tuple_type>, 0u);
    ASSERT_EQ(hud::tuple_size_v<tuple_type2>, 3u);
    ASSERT_EQ(hud::tuple_size_v<tuple_type3>, 3u);
    ASSERT_EQ(hud::tuple_size_v<tuple_type4>, 2u);
    ASSERT_EQ(hud::tuple_size_v<tuple_type5>, 2u);

    // Non constant
    {
        // No init for empty tuple
        [[maybe_unused]] tuple_type tuple1;

        tuple_type2 tuple2{ hud::taginit };
        ASSERT_TRUE((hud::is_same_v<decltype(hud::get<0>(tuple2)), i32&>));
        ASSERT_TRUE((hud::is_same_v<decltype(hud::get<1>(tuple2)), i32&>));
        ASSERT_TRUE((hud::is_same_v<decltype(hud::get<2>(tuple2)), i32&>));
        ASSERT_EQ((hud::get<0>(tuple2)), i32{});
        ASSERT_EQ((hud::get<1>(tuple2)), i32{});
        ASSERT_EQ((hud::get<2>(tuple2)), i32{});

        tuple_type3 tuple3{ hud::taginit };
        ASSERT_TRUE((hud::is_same_v<decltype(hud::get<0>(tuple3)), i32&>));
        ASSERT_TRUE((hud::is_same_v<decltype(hud::get<1>(tuple3)), f32&>));
        ASSERT_TRUE((hud::is_same_v<decltype(hud::get<2>(tuple3)), wchar&>));
        ASSERT_EQ((hud::get<0>(tuple3)), i32{});
        ASSERT_EQ((hud::get<1>(tuple3)), f32{});
        ASSERT_EQ((hud::get<2>(tuple3)), wchar{});

        tuple_type4 tuple4{ hud::taginit };
        ASSERT_TRUE((hud::is_same_v<decltype(hud::get<0>(tuple4)), ExplicitDefaultConstructible&>));
        ASSERT_TRUE((hud::is_same_v<decltype(hud::get<1>(tuple4)), ImplicitDefaultConstructible&>));
        ASSERT_EQ((hud::get<0>(tuple4).id()), ExplicitDefaultConstructible::DEFAULT_ID_VALUE);
        ASSERT_EQ((hud::get<0>(tuple4).default_constructor_count()), 1u);
        ASSERT_EQ((hud::get<0>(tuple4).constructor_count()), 0u);
        ASSERT_EQ((hud::get<0>(tuple4).copy_constructor_count()), 0u);
        ASSERT_EQ((hud::get<0>(tuple4).move_constructor_count()), 0u);
        ASSERT_EQ((hud::get<1>(tuple4).id()), ImplicitDefaultConstructible::DEFAULT_ID_VALUE);
        ASSERT_EQ((hud::get<1>(tuple4).default_constructor_count()), 1u);
        ASSERT_EQ((hud::get<1>(tuple4).constructor_count()), 0u);
        ASSERT_EQ((hud::get<1>(tuple4).copy_constructor_count()), 0u);
        ASSERT_EQ((hud::get<1>(tuple4).move_constructor_count()), 0u);

        tuple_type5 tuple5{ hud::taginit };
        ASSERT_TRUE((hud::is_same_v<decltype(hud::get<0>(tuple5)), ExplicitDefaultConstructible&>));
        ASSERT_TRUE((hud::is_same_v<decltype(hud::get<1>(tuple5)), i32&>));
        ASSERT_EQ((hud::get<0>(tuple5).id()), ExplicitDefaultConstructible::DEFAULT_ID_VALUE);
        ASSERT_EQ((hud::get<0>(tuple5).default_constructor_count()), 1u);
        ASSERT_EQ((hud::get<0>(tuple5).constructor_count()), 0u);
        ASSERT_EQ((hud::get<0>(tuple5).copy_constructor_count()), 0u);
        ASSERT_EQ((hud::get<0>(tuple5).move_constructor_count()), 0u);
        ASSERT_EQ((hud::get<1>(tuple5)), i32{});
    }

    // Constant
    {
        [[maybe_unused]] constexpr tuple_type tuple1;

        constexpr tuple_type2 tuple2{ hud::taginit };
        ASSERT_TRUE((hud::is_same_v<decltype(hud::get<0>(tuple2)), const i32&>));
        ASSERT_TRUE((hud::is_same_v<decltype(hud::get<1>(tuple2)), const i32&>));
        ASSERT_TRUE((hud::is_same_v<decltype(hud::get<2>(tuple2)), const i32&>));

        ASSERT_EQ((hud::get<0>(tuple2)), i32{});
        ASSERT_EQ((hud::get<1>(tuple2)), i32{});
        ASSERT_EQ((hud::get<2>(tuple2)), i32{});

        constexpr tuple_type3 tuple3{ hud::taginit };
        ASSERT_TRUE((hud::is_same_v<decltype(hud::get<0>(tuple3)), const i32&>));
        ASSERT_TRUE((hud::is_same_v<decltype(hud::get<1>(tuple3)), const f32&>));
        ASSERT_TRUE((hud::is_same_v<decltype(hud::get<2>(tuple3)), const wchar&>));

        ASSERT_EQ((hud::get<0>(tuple3)), i32{});
        ASSERT_EQ((hud::get<1>(tuple3)), f32{});
        ASSERT_EQ((hud::get<2>(tuple3)), wchar{});

        constexpr tuple_type4 tuple4{ hud::taginit };
        ASSERT_TRUE((hud::is_same_v<decltype(hud::get<0>(tuple4)), const ExplicitDefaultConstructible&>));
        ASSERT_TRUE((hud::is_same_v<decltype(hud::get<1>(tuple4)), const ImplicitDefaultConstructible&>));
        ASSERT_EQ((hud::get<0>(tuple4).id()), ExplicitDefaultConstructible::DEFAULT_ID_VALUE);
        ASSERT_EQ((hud::get<0>(tuple4).default_constructor_count()), 1u);
        ASSERT_EQ((hud::get<0>(tuple4).constructor_count()), 0u);
        ASSERT_EQ((hud::get<0>(tuple4).copy_constructor_count()), 0u);
        ASSERT_EQ((hud::get<0>(tuple4).move_constructor_count()), 0u);
        ASSERT_EQ((hud::get<1>(tuple4).id()), ImplicitDefaultConstructible::DEFAULT_ID_VALUE);
        ASSERT_EQ((hud::get<1>(tuple4).default_constructor_count()), 1u);
        ASSERT_EQ((hud::get<1>(tuple4).constructor_count()), 0u);
        ASSERT_EQ((hud::get<1>(tuple4).copy_constructor_count()), 0u);
        ASSERT_EQ((hud::get<1>(tuple4).move_constructor_count()), 0u);

        constexpr tuple_type5 tuple5{ hud::taginit };
        ASSERT_TRUE((hud::is_same_v<decltype(hud::get<0>(tuple5)), const ExplicitDefaultConstructible&>));
        ASSERT_TRUE((hud::is_same_v<decltype(hud::get<1>(tuple5)), const i32&>));
        ASSERT_EQ((hud::get<0>(tuple5).id()), ExplicitDefaultConstructible::DEFAULT_ID_VALUE);
        ASSERT_EQ((hud::get<0>(tuple5).default_constructor_count()), 1u);
        ASSERT_EQ((hud::get<0>(tuple5).constructor_count()), 0u);
        ASSERT_EQ((hud::get<0>(tuple5).copy_constructor_count()), 0u);
        ASSERT_EQ((hud::get<0>(tuple5).move_constructor_count()), 0u);
        ASSERT_EQ((hud::get<1>(tuple5)), i32{});
    }
}

TEST(tuple, default_constructor_explicit_types) {
    using tuple_type = hud::tuple<ExplicitDefaultConstructible, ExplicitDefaultConstructible>;

    ASSERT_TRUE(hud::is_explicitly_default_constructible_v<tuple_type>);

    // Non constant
    {
        // No init for empty tuple
        tuple_type tuple;

        ASSERT_EQ((hud::get<0>(tuple).id()), ExplicitDefaultConstructible::DEFAULT_ID_VALUE);
        ASSERT_EQ((hud::get<0>(tuple).default_constructor_count()), 1u);
        ASSERT_EQ((hud::get<0>(tuple).constructor_count()), 0u);
        ASSERT_EQ((hud::get<0>(tuple).copy_constructor_count()), 0u);
        ASSERT_EQ((hud::get<0>(tuple).move_constructor_count()), 0u);
        ASSERT_EQ((hud::get<1>(tuple).id()), ExplicitDefaultConstructible::DEFAULT_ID_VALUE);
        ASSERT_EQ((hud::get<1>(tuple).default_constructor_count()), 1u);
        ASSERT_EQ((hud::get<1>(tuple).constructor_count()), 0u);
        ASSERT_EQ((hud::get<1>(tuple).copy_constructor_count()), 0u);
        ASSERT_EQ((hud::get<1>(tuple).move_constructor_count()), 0u);
    }

    // Constant
    {
        // No init for empty tuple
        constexpr tuple_type tuple;

        ASSERT_EQ((hud::get<0>(tuple).id()), ExplicitDefaultConstructible::DEFAULT_ID_VALUE);
        ASSERT_EQ((hud::get<0>(tuple).default_constructor_count()), 1u);
        ASSERT_EQ((hud::get<0>(tuple).constructor_count()), 0u);
        ASSERT_EQ((hud::get<0>(tuple).copy_constructor_count()), 0u);
        ASSERT_EQ((hud::get<0>(tuple).move_constructor_count()), 0u);
        ASSERT_EQ((hud::get<1>(tuple).id()), ExplicitDefaultConstructible::DEFAULT_ID_VALUE);
        ASSERT_EQ((hud::get<1>(tuple).default_constructor_count()), 1u);
        ASSERT_EQ((hud::get<1>(tuple).constructor_count()), 0u);
        ASSERT_EQ((hud::get<1>(tuple).copy_constructor_count()), 0u);
        ASSERT_EQ((hud::get<1>(tuple).move_constructor_count()), 0u);
    }
}

TEST(tuple, default_constructor_implicit_types) {
    using tuple_type = hud::tuple<ImplicitDefaultConstructible, ImplicitDefaultConstructible>;

    ASSERT_TRUE(hud::is_implicitly_default_constructible_v<tuple_type>);

    // Non constant
    {
        // No init for empty tuple
        tuple_type tuple;

        ASSERT_EQ((hud::get<0>(tuple).id()), ImplicitDefaultConstructible::DEFAULT_ID_VALUE);
        ASSERT_EQ((hud::get<0>(tuple).default_constructor_count()), 1u);
        ASSERT_EQ((hud::get<0>(tuple).constructor_count()), 0u);
        ASSERT_EQ((hud::get<0>(tuple).copy_constructor_count()), 0u);
        ASSERT_EQ((hud::get<0>(tuple).move_constructor_count()), 0u);
        ASSERT_EQ((hud::get<1>(tuple).id()), ImplicitDefaultConstructible::DEFAULT_ID_VALUE);
        ASSERT_EQ((hud::get<1>(tuple).default_constructor_count()), 1u);
        ASSERT_EQ((hud::get<1>(tuple).constructor_count()), 0u);
        ASSERT_EQ((hud::get<1>(tuple).copy_constructor_count()), 0u);
        ASSERT_EQ((hud::get<1>(tuple).move_constructor_count()), 0u);
    }

    // Constant
    {
        // No init for empty tuple
        constexpr tuple_type tuple;

        ASSERT_EQ((hud::get<0>(tuple).id()), ImplicitDefaultConstructible::DEFAULT_ID_VALUE);
        ASSERT_EQ((hud::get<0>(tuple).default_constructor_count()), 1u);
        ASSERT_EQ((hud::get<0>(tuple).constructor_count()), 0u);
        ASSERT_EQ((hud::get<0>(tuple).copy_constructor_count()), 0u);
        ASSERT_EQ((hud::get<0>(tuple).move_constructor_count()), 0u);
        ASSERT_EQ((hud::get<1>(tuple).id()), ImplicitDefaultConstructible::DEFAULT_ID_VALUE);
        ASSERT_EQ((hud::get<1>(tuple).default_constructor_count()), 1u);
        ASSERT_EQ((hud::get<1>(tuple).constructor_count()), 0u);
        ASSERT_EQ((hud::get<1>(tuple).copy_constructor_count()), 0u);
        ASSERT_EQ((hud::get<1>(tuple).move_constructor_count()), 0u);
    }
}

TEST(tuple, default_constructor_mix_implicit_explicit_types) {
    using tuple_type = hud::tuple<ImplicitDefaultConstructible, ExplicitDefaultConstructible>;

    ASSERT_TRUE(hud::is_explicitly_default_constructible_v<tuple_type>);

    // Non constant
    {
        // No init for empty tuple
        tuple_type tuple;

        ASSERT_EQ((hud::get<0>(tuple).id()), ImplicitDefaultConstructible::DEFAULT_ID_VALUE);
        ASSERT_EQ((hud::get<0>(tuple).default_constructor_count()), 1u);
        ASSERT_EQ((hud::get<0>(tuple).constructor_count()), 0u);
        ASSERT_EQ((hud::get<0>(tuple).copy_constructor_count()), 0u);
        ASSERT_EQ((hud::get<0>(tuple).move_constructor_count()), 0u);
        ASSERT_EQ((hud::get<1>(tuple).id()), ExplicitDefaultConstructible::DEFAULT_ID_VALUE);
        ASSERT_EQ((hud::get<1>(tuple).default_constructor_count()), 1u);
        ASSERT_EQ((hud::get<1>(tuple).constructor_count()), 0u);
        ASSERT_EQ((hud::get<1>(tuple).copy_constructor_count()), 0u);
        ASSERT_EQ((hud::get<1>(tuple).move_constructor_count()), 0u);
    }

    // Constant
    {
        // No init for empty tuple
        constexpr tuple_type tuple;

        ASSERT_EQ((hud::get<0>(tuple).id()), ImplicitDefaultConstructible::DEFAULT_ID_VALUE);
        ASSERT_EQ((hud::get<0>(tuple).default_constructor_count()), 1u);
        ASSERT_EQ((hud::get<0>(tuple).constructor_count()), 0u);
        ASSERT_EQ((hud::get<0>(tuple).copy_constructor_count()), 0u);
        ASSERT_EQ((hud::get<0>(tuple).move_constructor_count()), 0u);
        ASSERT_EQ((hud::get<1>(tuple).id()), ExplicitDefaultConstructible::DEFAULT_ID_VALUE);
        ASSERT_EQ((hud::get<1>(tuple).default_constructor_count()), 1u);
        ASSERT_EQ((hud::get<1>(tuple).constructor_count()), 0u);
        ASSERT_EQ((hud::get<1>(tuple).copy_constructor_count()), 0u);
        ASSERT_EQ((hud::get<1>(tuple).move_constructor_count()), 0u);
    }
}

TEST(tuple, default_constructor_is_explicit_if_at_least_one_type_is_explicitly_default_constructible) {
    ASSERT_FALSE(hud::is_explicitly_default_constructible_v<hud::tuple<>>);
    ASSERT_FALSE((hud::is_explicitly_default_constructible_v<hud::tuple<i32, f32, wchar>>));
    ASSERT_FALSE((hud::is_explicitly_default_constructible_v<hud::tuple<ImplicitDefaultConstructible, ImplicitDefaultConstructible>>));
    ASSERT_TRUE((hud::is_explicitly_default_constructible_v<hud::tuple<ImplicitDefaultConstructible, ExplicitDefaultConstructible>>));
    ASSERT_TRUE((hud::is_explicitly_default_constructible_v<hud::tuple<ExplicitDefaultConstructible, ExplicitDefaultConstructible>>));
}

TEST(tuple, constructor_by_copy_explicitly_copyable_same_types) {
    using tuple_type = hud::tuple<ExplicitlyCopyConstructible, ExplicitlyCopyConstructible>;
    
    ASSERT_TRUE((hud::is_explicitly_constructible_v<tuple_type, const ExplicitlyCopyConstructible&, const ExplicitlyCopyConstructible&>));
    
    const auto test = []() {
        const ExplicitlyCopyConstructible obj1(1);
        const ExplicitlyCopyConstructible obj2(2);
        tuple_type tuple(obj1, obj2);
        return std::tuple{
            hud::get<0>(tuple).id() == 1,
            hud::get<0>(tuple).copy_constructor_count() == 1u,
            hud::get<1>(tuple).id() == 2,
            hud::get<1>(tuple).copy_constructor_count() == 1u
        };
    };
    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
    }
}

TEST(tuple, constructor_by_copy_explicitly_copyable_different_types) {
    using tuple_type = hud::tuple<ExplicitlyCopyConstructible2, ExplicitlyCopyConstructible2>;

    ASSERT_TRUE((hud::is_explicitly_constructible_v<tuple_type, const ExplicitlyCopyConstructible&, const ExplicitlyCopyConstructible&>));

    const auto test = []() {
        const ExplicitlyCopyConstructible obj1(1);
        const ExplicitlyCopyConstructible obj2(2);
        tuple_type tuple(obj1, obj2);
        return std::tuple{
            hud::get<0>(tuple).id() == 1,
            hud::get<0>(tuple).copy_constructor_count() == 1u,
            hud::get<1>(tuple).id() == 2,
            hud::get<1>(tuple).copy_constructor_count() == 1u
        };
    };
    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
    }
}

TEST(tuple, constructor_by_copy_implicitly_copyable_same_types) {
    using tuple_type = hud::tuple<ImplicitlyCopyConstructible, ImplicitlyCopyConstructible>;

    ASSERT_TRUE((hud::is_implicitly_constructible_v<tuple_type, const ImplicitlyCopyConstructible&, const ImplicitlyCopyConstructible&>));

    const auto test = []() {
        const ImplicitlyCopyConstructible obj1(1);
        const ImplicitlyCopyConstructible obj2(2);
        tuple_type tuple(obj1, obj2);
        return std::tuple{
            hud::get<0>(tuple).id() == 1,
            hud::get<0>(tuple).copy_constructor_count() == 1u,
            hud::get<1>(tuple).id() == 2,
            hud::get<1>(tuple).copy_constructor_count() == 1u
        };
    };
    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
    }
}

TEST(tuple, constructor_by_copy_implicitly_copyable_different_types) {
    using tuple_type = hud::tuple<ImplicitlyCopyConstructible2, ImplicitlyCopyConstructible2>;

    ASSERT_TRUE((hud::is_implicitly_constructible_v<tuple_type, const ImplicitlyCopyConstructible&, const ImplicitlyCopyConstructible&>));

    const auto test = []() {
        const ImplicitlyCopyConstructible obj1(1);
        const ImplicitlyCopyConstructible obj2(2);
        tuple_type tuple(obj1, obj2);
        return std::tuple{
            hud::get<0>(tuple).id() == 1,
            hud::get<0>(tuple).copy_constructor_count() == 1u,
            hud::get<1>(tuple).id() == 2,
            hud::get<1>(tuple).copy_constructor_count() == 1u
        };
    };
    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
    }
}

TEST(tuple, constructor_by_copy_mix_implicitly_explicitly_copyable_same_types) {
    using tuple_type = hud::tuple<ImplicitlyCopyConstructible, ExplicitlyCopyConstructible>;

    ASSERT_TRUE((hud::is_explicitly_constructible_v<tuple_type, const ImplicitlyCopyConstructible&, const ExplicitlyCopyConstructible&>));

    const auto test = []() {
        const ImplicitlyCopyConstructible obj1(1);
        const ExplicitlyCopyConstructible obj2(2);
        tuple_type tuple(obj1, obj2);
        return std::tuple{
            hud::get<0>(tuple).id() == 1,
            hud::get<0>(tuple).copy_constructor_count() == 1u,
            hud::get<1>(tuple).id() == 2,
            hud::get<1>(tuple).copy_constructor_count() == 1u
        };
    };
    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
    }
}

TEST(tuple, constructor_by_copy_mix_implicitly_explicitly_copyable_different_types) {
    using tuple_type = hud::tuple<ImplicitlyCopyConstructible2, ExplicitlyCopyConstructible2>;

    ASSERT_TRUE((hud::is_explicitly_constructible_v<tuple_type, const ImplicitlyCopyConstructible&, const ExplicitlyCopyConstructible&>));

    const auto test = []() {
        const ImplicitlyCopyConstructible obj1(1);
        const ExplicitlyCopyConstructible obj2(2);
        tuple_type tuple(obj1, obj2);
        return std::tuple{
            hud::get<0>(tuple).id() == 1,
            hud::get<0>(tuple).copy_constructor_count() == 1u,
            hud::get<1>(tuple).id() == 2,
            hud::get<1>(tuple).copy_constructor_count() == 1u
        };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
    }
}

TEST(tuple, constructor_by_move_explicitly_copyable_same_types) {
    using tuple_type = hud::tuple<ExplicitlyCopyConstructible, ExplicitlyCopyConstructible>;

    ASSERT_TRUE((hud::is_explicitly_constructible_v<tuple_type, ExplicitlyCopyConstructible&&, ExplicitlyCopyConstructible&&>));

    const auto test = []() {
        ExplicitlyCopyConstructible obj1(1);
        ExplicitlyCopyConstructible obj2(2);
        tuple_type tuple(hud::move(obj1), hud::move(obj2));
        return std::tuple{
            hud::get<0>(tuple).id() == 1,
            hud::get<0>(tuple).copy_constructor_count() == 1u,
            hud::get<1>(tuple).id() == 2,
            hud::get<1>(tuple).copy_constructor_count() == 1u
        };
    };
    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
    }
}

TEST(tuple, constructor_by_move_explicitly_copyable_different_types) {
    using tuple_type = hud::tuple<ExplicitlyCopyConstructible2, ExplicitlyCopyConstructible2>;

    ASSERT_TRUE((hud::is_explicitly_constructible_v<tuple_type, ExplicitlyCopyConstructible&&, ExplicitlyCopyConstructible&&>));

    const auto test = []() {
        ExplicitlyCopyConstructible obj1(1);
        ExplicitlyCopyConstructible obj2(2);
        tuple_type tuple(hud::move(obj1), hud::move(obj2));
        return std::tuple{
            hud::get<0>(tuple).id() == 1,
            hud::get<0>(tuple).copy_constructor_count() == 1u,
            hud::get<1>(tuple).id() == 2,
            hud::get<1>(tuple).copy_constructor_count() == 1u
        };
    };
    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
    }
}

TEST(tuple, constructor_by_move_implicitly_copyable_same_types) {
    using tuple_type = hud::tuple<ImplicitlyCopyConstructible, ImplicitlyCopyConstructible>;

    ASSERT_TRUE((hud::is_implicitly_constructible_v<tuple_type, ImplicitlyCopyConstructible&&, ImplicitlyCopyConstructible&&>));

    const auto test = []() {
        ImplicitlyCopyConstructible obj1(1);
        ImplicitlyCopyConstructible obj2(2);
        tuple_type tuple(hud::move(obj1), hud::move(obj2));
        return std::tuple{
            hud::get<0>(tuple).id() == 1,
            hud::get<0>(tuple).copy_constructor_count() == 1u,
            hud::get<1>(tuple).id() == 2,
            hud::get<1>(tuple).copy_constructor_count() == 1u
        };
    };
    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
    }
}

TEST(tuple, constructor_by_move_implicitly_copyable_different_types) {
    using tuple_type = hud::tuple<ImplicitlyCopyConstructible2, ImplicitlyCopyConstructible2>;

    ASSERT_TRUE((hud::is_implicitly_constructible_v<tuple_type, ImplicitlyCopyConstructible&&, ImplicitlyCopyConstructible&&>));

    const auto test = []() {
        ImplicitlyCopyConstructible obj1(1);
        ImplicitlyCopyConstructible obj2(2);
        tuple_type tuple(hud::move(obj1), hud::move(obj2));
        return std::tuple{
            hud::get<0>(tuple).id() == 1,
            hud::get<0>(tuple).copy_constructor_count() == 1u,
            hud::get<1>(tuple).id() == 2,
            hud::get<1>(tuple).copy_constructor_count() == 1u
        };
    };
    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
    }
}

TEST(tuple, constructor_by_move_mix_implicitly_explicitly_copyable_same_types) {
    using tuple_type = hud::tuple<ImplicitlyCopyConstructible, ExplicitlyCopyConstructible>;

    ASSERT_TRUE((hud::is_explicitly_constructible_v<tuple_type, ImplicitlyCopyConstructible&&, ExplicitlyCopyConstructible&&>));

    const auto test = []() {
        ImplicitlyCopyConstructible obj1(1);
        ExplicitlyCopyConstructible obj2(2);
        tuple_type tuple(hud::move(obj1), hud::move(obj2));
        return std::tuple{
            hud::get<0>(tuple).id() == 1,
            hud::get<0>(tuple).copy_constructor_count() == 1u,
            hud::get<1>(tuple).id() == 2,
            hud::get<1>(tuple).copy_constructor_count() == 1u
        };
    };
    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
    }
}

TEST(tuple, constructor_by_move_mix_implicitly_explicitly_copyable_different_types) {
    using tuple_type = hud::tuple<ImplicitlyCopyConstructible2, ExplicitlyCopyConstructible2>;

    ASSERT_TRUE((hud::is_explicitly_constructible_v<tuple_type, ImplicitlyCopyConstructible&&, ExplicitlyCopyConstructible&&>));

    const auto test = []() {
        ImplicitlyCopyConstructible obj1(1);
        ExplicitlyCopyConstructible obj2(2);
        tuple_type tuple(hud::move(obj1), hud::move(obj2));
        return std::tuple{
            hud::get<0>(tuple).id() == 1,
            hud::get<0>(tuple).copy_constructor_count() == 1u,
            hud::get<1>(tuple).id() == 2,
            hud::get<1>(tuple).copy_constructor_count() == 1u
        };
    };
    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
    }
}

TEST(tuple, constructor_by_move_explicitly_moveable_same_types) {
    using tuple_type = hud::tuple<ExplicitlyMoveConstructible, ExplicitlyMoveConstructible>;

    ASSERT_TRUE((hud::is_explicitly_constructible_v<tuple_type, ExplicitlyMoveConstructible&&, ExplicitlyMoveConstructible&&>));

    const auto test = []() {
        ExplicitlyMoveConstructible obj1(1);
        ExplicitlyMoveConstructible obj2(2);
        tuple_type tuple(hud::move(obj1), hud::move(obj2));
        return std::tuple{
            hud::get<0>(tuple).id() == 1,
            hud::get<0>(tuple).move_constructor_count() == 1u,
            hud::get<0>(tuple).copy_constructor_count() == 0u,
            hud::get<1>(tuple).id() == 2,
            hud::get<1>(tuple).move_constructor_count() == 1u,
            hud::get<1>(tuple).copy_constructor_count() == 0u,
        };
    };
    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
        ASSERT_TRUE(std::get<4>(result));
        ASSERT_TRUE(std::get<5>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
        ASSERT_TRUE(std::get<4>(result));
        ASSERT_TRUE(std::get<5>(result));
    }
}

TEST(tuple, constructor_by_move_explicitly_moveable_different_types) {
    using tuple_type = hud::tuple<ExplicitlyMoveConstructible2, ExplicitlyMoveConstructible2>;

    ASSERT_TRUE((hud::is_explicitly_constructible_v<tuple_type, ExplicitlyMoveConstructible&&, ExplicitlyMoveConstructible&&>));

    const auto test = []() {
        ExplicitlyMoveConstructible obj1(1);
        ExplicitlyMoveConstructible obj2(2);
        tuple_type tuple(hud::move(obj1), hud::move(obj2));
        return std::tuple{
            hud::get<0>(tuple).id() == 1,
            hud::get<0>(tuple).move_constructor_count() == 1u,
            hud::get<0>(tuple).copy_constructor_count() == 0u,
            hud::get<1>(tuple).id() == 2,
            hud::get<1>(tuple).move_constructor_count() == 1u,
            hud::get<1>(tuple).copy_constructor_count() == 0u,
        };
    };
    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
        ASSERT_TRUE(std::get<4>(result));
        ASSERT_TRUE(std::get<5>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
        ASSERT_TRUE(std::get<4>(result));
        ASSERT_TRUE(std::get<5>(result));
    }
}

TEST(tuple, constructor_by_move_implicitly_moveable_same_types) {
    using tuple_type = hud::tuple<ImplicitlyMoveConstructible, ImplicitlyMoveConstructible>;

    ASSERT_TRUE((hud::is_implicitly_constructible_v<tuple_type, ImplicitlyMoveConstructible&&, ImplicitlyMoveConstructible&&>));

    const auto test = []() {
        ImplicitlyMoveConstructible obj1(1);
        ImplicitlyMoveConstructible obj2(2);
        tuple_type tuple(hud::move(obj1), hud::move(obj2));
        return std::tuple{
            hud::get<0>(tuple).id() == 1,
            hud::get<0>(tuple).move_constructor_count() == 1u,
            hud::get<0>(tuple).copy_constructor_count() == 0u,
            hud::get<1>(tuple).id() == 2,
            hud::get<1>(tuple).move_constructor_count() == 1u,
            hud::get<1>(tuple).copy_constructor_count() == 0u,
        };
    };
    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
        ASSERT_TRUE(std::get<4>(result));
        ASSERT_TRUE(std::get<5>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
        ASSERT_TRUE(std::get<4>(result));
        ASSERT_TRUE(std::get<5>(result));
    }
}

TEST(tuple, constructor_by_move_implicitly_moveable_different_types) {
    using tuple_type = hud::tuple<ImplicitlyMoveConstructible2, ImplicitlyMoveConstructible2>;

    ASSERT_TRUE((hud::is_implicitly_constructible_v<tuple_type, ImplicitlyMoveConstructible&&, ImplicitlyMoveConstructible&&>));

    const auto test = []() {
        ImplicitlyMoveConstructible obj1(1);
        ImplicitlyMoveConstructible obj2(2);
        tuple_type tuple(hud::move(obj1), hud::move(obj2));
        return std::tuple{
            hud::get<0>(tuple).id() == 1,
            hud::get<0>(tuple).move_constructor_count() == 1u,
            hud::get<0>(tuple).copy_constructor_count() == 0u,
            hud::get<1>(tuple).id() == 2,
            hud::get<1>(tuple).move_constructor_count() == 1u,
            hud::get<1>(tuple).copy_constructor_count() == 0u,
        };
    };
    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
        ASSERT_TRUE(std::get<4>(result));
        ASSERT_TRUE(std::get<5>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
        ASSERT_TRUE(std::get<4>(result));
        ASSERT_TRUE(std::get<5>(result));
    }
}

TEST(tuple, constructor_by_move_mix_implicitly_explicitly_moveable_same_types) {
    using tuple_type = hud::tuple<ImplicitlyMoveConstructible, ExplicitlyMoveConstructible>;

    ASSERT_TRUE((hud::is_explicitly_constructible_v<tuple_type, ImplicitlyMoveConstructible&&, ExplicitlyMoveConstructible&&>));

    const auto test = []() {
        ImplicitlyMoveConstructible obj1(1);
        ExplicitlyMoveConstructible obj2(2);
        tuple_type tuple(hud::move(obj1), hud::move(obj2));
        return std::tuple{
            hud::get<0>(tuple).id() == 1,
            hud::get<0>(tuple).move_constructor_count() == 1u,
            hud::get<0>(tuple).copy_constructor_count() == 0u,
            hud::get<1>(tuple).id() == 2,
            hud::get<1>(tuple).move_constructor_count() == 1u,
            hud::get<1>(tuple).copy_constructor_count() == 0u,
        };
    };
    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
        ASSERT_TRUE(std::get<4>(result));
        ASSERT_TRUE(std::get<5>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
        ASSERT_TRUE(std::get<4>(result));
        ASSERT_TRUE(std::get<5>(result));
    }
}

TEST(tuple, constructor_by_move_mix_implicitly_explicitly_moveable_different_types) {
    using tuple_type = hud::tuple<ImplicitlyMoveConstructible2, ExplicitlyMoveConstructible2>;

    ASSERT_TRUE((hud::is_explicitly_constructible_v<tuple_type, ImplicitlyMoveConstructible&&, ExplicitlyMoveConstructible&&>));

    const auto test = []() {
        ImplicitlyMoveConstructible obj1(1);
        ExplicitlyMoveConstructible obj2(2);
        tuple_type tuple(hud::move(obj1), hud::move(obj2));
        return std::tuple{
            hud::get<0>(tuple).id() == 1,
            hud::get<0>(tuple).move_constructor_count() == 1u,
            hud::get<0>(tuple).copy_constructor_count() == 0u,
            hud::get<1>(tuple).id() == 2,
            hud::get<1>(tuple).move_constructor_count() == 1u,
            hud::get<1>(tuple).copy_constructor_count() == 0u,
        };
    };
    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
        ASSERT_TRUE(std::get<4>(result));
        ASSERT_TRUE(std::get<5>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
        ASSERT_TRUE(std::get<4>(result));
        ASSERT_TRUE(std::get<5>(result));
    }
}

TEST(tuple, constructor_by_copy_pair_of_explictly_copyable_same_types) {
    using pair_type = hud::pair<ExplicitlyCopyConstructible, ExplicitlyCopyConstructible>;
    using tuple_type = hud::tuple<ExplicitlyCopyConstructible, ExplicitlyCopyConstructible>;


    ASSERT_TRUE((hud::is_explicitly_constructible_v<tuple_type, const pair_type&>));

    const auto test = []() {
        const pair_type pair{ 1,2 };
        const tuple_type tuple{ pair };
        return std::tuple{
            hud::get<0>(tuple).id(),
            hud::get<0>(tuple).copy_constructor_count() == 1u,
            hud::get<1>(tuple).id(),
            hud::get<1>(tuple).copy_constructor_count() == 1u,
        };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
    }

}

TEST(tuple, constructor_by_copy_pair_of_explictly_copyable_different_types) {
    using pair_type = hud::pair<ExplicitlyCopyConstructible, ExplicitlyCopyConstructible>;
    using tuple_type = hud::tuple<ExplicitlyCopyConstructible2, ExplicitlyCopyConstructible2>;


    ASSERT_TRUE((hud::is_explicitly_constructible_v<tuple_type, const pair_type&>));

    const auto test = []() {
        const pair_type pair{1,2};
        const tuple_type tuple{ pair };
        return std::tuple{
            hud::get<0>(tuple).id(),
            hud::get<0>(tuple).copy_constructor_count() == 1u,
            hud::get<1>(tuple).id(),
            hud::get<1>(tuple).copy_constructor_count() == 1u,
        };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
    }
}

TEST(tuple, constructor_by_copy_pair_of_implictly_copyable_same_types) {
    using pair_type = hud::pair<ImplicitlyCopyConstructible, ImplicitlyCopyConstructible>;
    using tuple_type = hud::tuple<ImplicitlyCopyConstructible, ImplicitlyCopyConstructible>;


    ASSERT_TRUE((hud::is_implicitly_constructible_v<tuple_type, const pair_type&>));

    const auto test = []() {
        const pair_type pair{ 1,2 };
        const tuple_type tuple{ pair };
        return std::tuple{
            hud::get<0>(tuple).id(),
            hud::get<0>(tuple).copy_constructor_count() == 1u,
            hud::get<1>(tuple).id(),
            hud::get<1>(tuple).copy_constructor_count() == 1u,
        };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
    }
}

TEST(tuple, constructor_by_copy_pair_of_implictly_copyable_different_types) {
    using pair_type = hud::pair<ImplicitlyCopyConstructible, ImplicitlyCopyConstructible>;
    using tuple_type = hud::tuple<ImplicitlyCopyConstructible2, ImplicitlyCopyConstructible2>;


    ASSERT_TRUE((hud::is_implicitly_constructible_v<tuple_type, const pair_type&>));

    const auto test = []() {
        const pair_type pair{ 1,2 };
        const tuple_type tuple{ pair };
        return std::tuple{
            hud::get<0>(tuple).id(),
            hud::get<0>(tuple).copy_constructor_count() == 1u,
            hud::get<1>(tuple).id(),
            hud::get<1>(tuple).copy_constructor_count() == 1u,
        };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
    }
}

TEST(tuple, constructor_by_copy_pair_of_mix_implictly_and_explitly_copyable_same_types) {
    using pair_type = hud::pair<ImplicitlyCopyConstructible, ExplicitlyCopyConstructible>;
    using tuple_type = hud::tuple<ImplicitlyCopyConstructible, ExplicitlyCopyConstructible>;


    ASSERT_TRUE((hud::is_explicitly_constructible_v<tuple_type, const pair_type&>));

    const auto test = []() {
        const pair_type pair{ 1,2 };
        const tuple_type tuple{ pair };
        return std::tuple{
            hud::get<0>(tuple).id(),
            hud::get<0>(tuple).copy_constructor_count() == 1u,
            hud::get<1>(tuple).id(),
            hud::get<1>(tuple).copy_constructor_count() == 1u,
        };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
    }
}

TEST(tuple, constructor_by_copy_pair_of_mix_implictly_and_explitly_copyable_different_types) {
    using pair_type = hud::pair<ImplicitlyCopyConstructible, ExplicitlyCopyConstructible>;
    using tuple_type = hud::tuple<ImplicitlyCopyConstructible2, ExplicitlyCopyConstructible2>;


    ASSERT_TRUE((hud::is_explicitly_constructible_v<tuple_type, const pair_type&>));

    const auto test = []() {
        const pair_type pair{ 1,2 };
        const tuple_type tuple{ pair };
        return std::tuple{
            hud::get<0>(tuple).id(),
            hud::get<0>(tuple).copy_constructor_count() == 1u,
            hud::get<1>(tuple).id(),
            hud::get<1>(tuple).copy_constructor_count() == 1u,
        };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
    }
}

TEST(tuple, constructor_by_move_pair_of_explictly_copyable_same_types) {
    using pair_type = hud::pair<ExplicitlyCopyConstructible, ExplicitlyCopyConstructible>;
    using tuple_type = hud::tuple<ExplicitlyCopyConstructible, ExplicitlyCopyConstructible>;


    ASSERT_TRUE((hud::is_explicitly_constructible_v<tuple_type, pair_type&&>));

    const auto test = []() {
        pair_type pair{ 1,2 };
        const tuple_type tuple{ hud::move(pair) };
        return std::tuple{
            hud::get<0>(tuple).id(),
            hud::get<0>(tuple).copy_constructor_count() == 1u,
            hud::get<1>(tuple).id(),
            hud::get<1>(tuple).copy_constructor_count() == 1u,
        };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
    }

}

TEST(tuple, constructor_by_move_pair_of_explictly_copyable_different_types) {
    using pair_type = hud::pair<ExplicitlyCopyConstructible, ExplicitlyCopyConstructible>;
    using tuple_type = hud::tuple<ExplicitlyCopyConstructible2, ExplicitlyCopyConstructible2>;


    ASSERT_TRUE((hud::is_explicitly_constructible_v<tuple_type, pair_type&&>));

    const auto test = []() {
        pair_type pair{ 1,2 };
        const tuple_type tuple{ hud::move(pair) };
        return std::tuple{
            hud::get<0>(tuple).id(),
            hud::get<0>(tuple).copy_constructor_count() == 1u,
            hud::get<1>(tuple).id(),
            hud::get<1>(tuple).copy_constructor_count() == 1u,
        };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
    }
}

TEST(tuple, constructor_by_move_pair_of_implictly_copyable_same_types) {
    using pair_type = hud::pair<ImplicitlyCopyConstructible, ImplicitlyCopyConstructible>;
    using tuple_type = hud::tuple<ImplicitlyCopyConstructible, ImplicitlyCopyConstructible>;


    ASSERT_TRUE((hud::is_implicitly_constructible_v<tuple_type, pair_type&&>));

    const auto test = []() {
        pair_type pair{ 1,2 };
        const tuple_type tuple{ hud::move(pair) };
        return std::tuple{
            hud::get<0>(tuple).id(),
            hud::get<0>(tuple).copy_constructor_count() == 1u,
            hud::get<1>(tuple).id(),
            hud::get<1>(tuple).copy_constructor_count() == 1u,
        };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
    }
}

TEST(tuple, constructor_by_move_pair_of_implictly_copyable_different_types) {
    using pair_type = hud::pair<ImplicitlyCopyConstructible, ImplicitlyCopyConstructible>;
    using tuple_type = hud::tuple<ImplicitlyCopyConstructible2, ImplicitlyCopyConstructible2>;


    ASSERT_TRUE((hud::is_implicitly_constructible_v<tuple_type, pair_type&&>));

    const auto test = []() {
        pair_type pair{ 1,2 };
        const tuple_type tuple{ hud::move(pair) };
        return std::tuple{
            hud::get<0>(tuple).id(),
            hud::get<0>(tuple).copy_constructor_count() == 1u,
            hud::get<1>(tuple).id(),
            hud::get<1>(tuple).copy_constructor_count() == 1u,
        };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
    }
}

TEST(tuple, constructor_by_move_pair_of_mix_implictly_and_explitly_copyable_same_types) {
    using pair_type = hud::pair<ImplicitlyCopyConstructible, ExplicitlyCopyConstructible>;
    using tuple_type = hud::tuple<ImplicitlyCopyConstructible, ExplicitlyCopyConstructible>;


    ASSERT_TRUE((hud::is_explicitly_constructible_v<tuple_type, pair_type&&>));

    const auto test = []() {
        pair_type pair{ 1,2 };
        const tuple_type tuple{ hud::move(pair) };
        return std::tuple{
            hud::get<0>(tuple).id(),
            hud::get<0>(tuple).copy_constructor_count() == 1u,
            hud::get<1>(tuple).id(),
            hud::get<1>(tuple).copy_constructor_count() == 1u,
        };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
    }
}

TEST(tuple, constructor_by_move_pair_of_mix_implictly_and_explitly_copyable_different_types) {
    using pair_type = hud::pair<ImplicitlyCopyConstructible, ExplicitlyCopyConstructible>;
    using tuple_type = hud::tuple<ImplicitlyCopyConstructible2, ExplicitlyCopyConstructible2>;


    ASSERT_TRUE((hud::is_explicitly_constructible_v<tuple_type, pair_type&&>));

    const auto test = []() {
        pair_type pair{ 1,2 };
        const tuple_type tuple{ hud::move(pair) };
        return std::tuple{
            hud::get<0>(tuple).id(),
            hud::get<0>(tuple).copy_constructor_count() == 1u,
            hud::get<1>(tuple).id(),
            hud::get<1>(tuple).copy_constructor_count() == 1u,
        };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
    }
}

TEST(tuple, constructor_by_move_pair_of_explictly_moveable_same_types) {
    using pair_type = hud::pair<ExplicitlyMoveConstructible, ExplicitlyMoveConstructible>;
    using tuple_type = hud::tuple<ExplicitlyMoveConstructible, ExplicitlyMoveConstructible>;


   // ASSERT_TRUE((hud::is_explicitly_constructible_v<tuple_type, pair_type&&>));
    static_assert((hud::is_explicitly_move_constructible_v<ExplicitlyMoveConstructible, ExplicitlyMoveConstructible>));
    static_assert((hud::details::is_pair_explicitly_move_constructible_to_tuple_v< pair_type, tuple_type>));
    const auto test = []() {
        pair_type pair{ 1,2 };
        const tuple_type tuple{ hud::move(pair) };
        return std::tuple{
            hud::get<0>(tuple).id(),
            hud::get<0>(tuple).copy_constructor_count() == 0u,
            hud::get<0>(tuple).move_constructor_count() == 1u,
            hud::get<1>(tuple).id(),
            hud::get<1>(tuple).copy_constructor_count() == 0u,
            hud::get<1>(tuple).move_constructor_count() == 1u,
        };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
        ASSERT_TRUE(std::get<4>(result));
        ASSERT_TRUE(std::get<5>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
        ASSERT_TRUE(std::get<4>(result));
        ASSERT_TRUE(std::get<5>(result));
    }

}

TEST(tuple, constructor_by_move_pair_of_explictly_moveable_different_types) {
    using pair_type = hud::pair<ExplicitlyMoveConstructible, ExplicitlyMoveConstructible>;
    using tuple_type = hud::tuple<ExplicitlyMoveConstructible2, ExplicitlyMoveConstructible2>;


    ASSERT_TRUE((hud::is_explicitly_constructible_v<tuple_type, pair_type&&>));

    const auto test = []() {
        pair_type pair{ 1,2 };
        const tuple_type tuple{ hud::move(pair) };
        return std::tuple{
            hud::get<0>(tuple).id(),
            hud::get<0>(tuple).copy_constructor_count() == 0u,
            hud::get<0>(tuple).move_constructor_count() == 1u,
            hud::get<1>(tuple).id(),
            hud::get<1>(tuple).copy_constructor_count() == 0u,
            hud::get<1>(tuple).move_constructor_count() == 1u,
        };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
        ASSERT_TRUE(std::get<4>(result));
        ASSERT_TRUE(std::get<5>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
        ASSERT_TRUE(std::get<4>(result));
        ASSERT_TRUE(std::get<5>(result));
    }
}

TEST(tuple, constructor_by_move_pair_of_implictly_moveable_same_types) {
    using pair_type = hud::pair<ImplicitlyMoveConstructible, ImplicitlyMoveConstructible>;
    using tuple_type = hud::tuple<ImplicitlyMoveConstructible, ImplicitlyMoveConstructible>;


    ASSERT_TRUE((hud::is_implicitly_constructible_v<tuple_type, pair_type&&>));

    const auto test = []() {
        pair_type pair{ 1,2 };
        const tuple_type tuple{ hud::move(pair) };
        return std::tuple{
            hud::get<0>(tuple).id(),
            hud::get<0>(tuple).copy_constructor_count() == 0u,
            hud::get<0>(tuple).move_constructor_count() == 1u,
            hud::get<1>(tuple).id(),
            hud::get<1>(tuple).copy_constructor_count() == 0u,
            hud::get<1>(tuple).move_constructor_count() == 1u,
        };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
        ASSERT_TRUE(std::get<4>(result));
        ASSERT_TRUE(std::get<5>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
        ASSERT_TRUE(std::get<4>(result));
        ASSERT_TRUE(std::get<5>(result));
    }
}

TEST(tuple, constructor_by_move_pair_of_implictly_moveable_different_types) {
    using pair_type = hud::pair<ImplicitlyMoveConstructible, ImplicitlyMoveConstructible>;
    using tuple_type = hud::tuple<ImplicitlyMoveConstructible2, ImplicitlyMoveConstructible2>;


    ASSERT_TRUE((hud::is_implicitly_constructible_v<tuple_type, pair_type&&>));

    const auto test = []() {
        pair_type pair{ 1,2 };
        const tuple_type tuple{ hud::move(pair) };
        return std::tuple{
            hud::get<0>(tuple).id(),
            hud::get<0>(tuple).copy_constructor_count() == 0u,
            hud::get<0>(tuple).move_constructor_count() == 1u,
            hud::get<1>(tuple).id(),
            hud::get<1>(tuple).copy_constructor_count() == 0u,
            hud::get<1>(tuple).move_constructor_count() == 1u,
        };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
        ASSERT_TRUE(std::get<4>(result));
        ASSERT_TRUE(std::get<5>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
        ASSERT_TRUE(std::get<4>(result));
        ASSERT_TRUE(std::get<5>(result));
    }
}

TEST(tuple, constructor_by_move_pair_of_mix_implictly_and_explitly_moveable_same_types) {
    using pair_type = hud::pair<ImplicitlyMoveConstructible, ExplicitlyMoveConstructible>;
    using tuple_type = hud::tuple<ImplicitlyMoveConstructible, ExplicitlyMoveConstructible>;


    ASSERT_TRUE((hud::is_explicitly_constructible_v<tuple_type, pair_type&&>));

    const auto test = []() {
        pair_type pair{ 1,2 };
        const tuple_type tuple{ hud::move(pair) };
        return std::tuple{
            hud::get<0>(tuple).id(),
            hud::get<0>(tuple).copy_constructor_count() == 0u,
            hud::get<0>(tuple).move_constructor_count() == 1u,
            hud::get<1>(tuple).id(),
            hud::get<1>(tuple).copy_constructor_count() == 0u,
            hud::get<1>(tuple).move_constructor_count() == 1u,
        };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
        ASSERT_TRUE(std::get<4>(result));
        ASSERT_TRUE(std::get<5>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
        ASSERT_TRUE(std::get<4>(result));
        ASSERT_TRUE(std::get<5>(result));
    }
}

TEST(tuple, constructor_by_move_pair_of_mix_implictly_and_explitly_moveable_different_types) {
    using pair_type = hud::pair<ImplicitlyMoveConstructible, ExplicitlyMoveConstructible>;
    using tuple_type = hud::tuple<ImplicitlyMoveConstructible2, ExplicitlyMoveConstructible2>;


    ASSERT_TRUE((hud::is_explicitly_constructible_v<tuple_type, pair_type&&>));

    const auto test = []() {
        pair_type pair{ 1,2 };
        const tuple_type tuple{ hud::move(pair) };
        return std::tuple{
            hud::get<0>(tuple).id(),
            hud::get<0>(tuple).copy_constructor_count() == 0u,
            hud::get<0>(tuple).move_constructor_count() == 1u,
            hud::get<1>(tuple).id(),
            hud::get<1>(tuple).copy_constructor_count() == 0u,
            hud::get<1>(tuple).move_constructor_count() == 1u,
        };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
        ASSERT_TRUE(std::get<4>(result));
        ASSERT_TRUE(std::get<5>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
        ASSERT_TRUE(std::get<4>(result));
        ASSERT_TRUE(std::get<5>(result));
    }
}

TEST(tuple, copy_constructor_copyable_same_types) {
    using tuple_type = hud::tuple<ExplicitlyCopyConstructible, ImplicitlyCopyConstructible>;

    ASSERT_TRUE((hud::is_explicitly_constructible_v<tuple_type, const ExplicitlyCopyConstructible&, const ImplicitlyCopyConstructible&>));

    const auto test = []() {
        const tuple_type to_copy{ 1,2 };
        const tuple_type copy{ to_copy };
        return std::tuple{
            hud::get<0>(copy).id() == 1,
            hud::get<0>(copy).copy_constructor_count() == 1u,
            hud::get<1>(copy).id() == 2,
            hud::get<1>(copy).copy_constructor_count() == 1u,
        };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
    }
}

TEST(tuple, copy_constructor_copyable_different_types) {
    using tuple_type = hud::tuple<ExplicitlyCopyConstructible2, ImplicitlyCopyConstructible2>;
    using other_tuple_type = hud::tuple<ExplicitlyCopyConstructible, ImplicitlyCopyConstructible>;

    ASSERT_TRUE((hud::is_explicitly_constructible_v<tuple_type, const ExplicitlyCopyConstructible&, const ImplicitlyCopyConstructible&>));

    const auto test = []() {
        const other_tuple_type to_copy{ 1,2 };
        const tuple_type copy{ to_copy };
        return std::tuple{
            hud::get<0>(copy).id() == 1,
            hud::get<0>(copy).copy_constructor_count() == 1u,
            hud::get<1>(copy).id() == 2,
            hud::get<1>(copy).copy_constructor_count() == 1u,
        };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
    }
}

TEST(tuple, copy_constructor_moveable_same_types) {
    using tuple_type = hud::tuple<ExplicitlyMoveConstructible, ImplicitlyMoveConstructible>;

    ASSERT_TRUE((hud::is_explicitly_constructible_v<tuple_type, const ExplicitlyMoveConstructible&, const ExplicitlyMoveConstructible&>));

    const auto test = []() {
        const tuple_type to_copy{ 1,2 };
        const tuple_type copy{ to_copy };
        return std::tuple{
            hud::get<0>(copy).id() == 1,
            hud::get<0>(copy).copy_constructor_count() == 1u,
            hud::get<0>(copy).move_constructor_count() == 0u,
            hud::get<1>(copy).id() == 2,
            hud::get<1>(copy).copy_constructor_count() == 1u,
            hud::get<1>(copy).move_constructor_count() == 0u,
        };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
        ASSERT_TRUE(std::get<4>(result));
        ASSERT_TRUE(std::get<5>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
        ASSERT_TRUE(std::get<4>(result));
        ASSERT_TRUE(std::get<5>(result));
    }
}

TEST(tuple, copy_constructor_moveable_different_types) {
    using tuple_type = hud::tuple<ExplicitlyMoveConstructible2, ImplicitlyMoveConstructible2>;
    using other_tuple_type = hud::tuple<ExplicitlyMoveConstructible, ImplicitlyMoveConstructible>;

    ASSERT_TRUE((hud::is_explicitly_constructible_v<tuple_type, const ExplicitlyMoveConstructible&, const ImplicitlyMoveConstructible&>));

    const auto test = []() {
        const other_tuple_type to_copy{ 1,2 };
        const tuple_type copy{ to_copy };
        return std::tuple{
            hud::get<0>(copy).id() == 1,
            hud::get<0>(copy).copy_constructor_count() == 1u,
            hud::get<0>(copy).move_constructor_count() == 0u,
            hud::get<1>(copy).id() == 2,
            hud::get<1>(copy).copy_constructor_count() == 1u,
            hud::get<1>(copy).move_constructor_count() == 0u,
        };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
        ASSERT_TRUE(std::get<4>(result));
        ASSERT_TRUE(std::get<5>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
        ASSERT_TRUE(std::get<4>(result));
        ASSERT_TRUE(std::get<5>(result));
    }
}

TEST(tuple, move_constructor_copyable_same_types) {
    using tuple_type = hud::tuple<ExplicitlyCopyConstructible, ImplicitlyCopyConstructible>;

    ASSERT_TRUE((hud::is_explicitly_constructible_v<tuple_type, ExplicitlyCopyConstructible&&, ImplicitlyCopyConstructible&&>));

    const auto test = []() {
        tuple_type to_copy{ 1,2 };
        const tuple_type copy{ hud::move(to_copy) };
        return std::tuple{
            hud::get<0>(copy).id() == 1,
            hud::get<0>(copy).copy_constructor_count() == 1u,
            hud::get<1>(copy).id() == 2,
            hud::get<1>(copy).copy_constructor_count() == 1u,
        };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
    }
}

TEST(tuple, move_constructor_copyable_different_types) {
    using tuple_type = hud::tuple<ExplicitlyCopyConstructible2, ImplicitlyCopyConstructible2>;
    using other_tuple_type = hud::tuple<ExplicitlyCopyConstructible, ImplicitlyCopyConstructible>;

    ASSERT_TRUE((hud::is_explicitly_constructible_v<tuple_type, ExplicitlyCopyConstructible&&, ImplicitlyCopyConstructible&&>));

    const auto test = []() {
        other_tuple_type to_copy{ 1,2 };
        const tuple_type copy{ hud::move(to_copy) };
        return std::tuple{
            hud::get<0>(copy).id() == 1,
            hud::get<0>(copy).copy_constructor_count() == 1u,
            hud::get<1>(copy).id() == 2,
            hud::get<1>(copy).copy_constructor_count() == 1u,
        };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
    }
}

TEST(tuple, move_constructor_moveable_same_types) {
    using tuple_type = hud::tuple<ExplicitlyMoveConstructible, ImplicitlyMoveConstructible>;

    ASSERT_TRUE((hud::is_explicitly_constructible_v<tuple_type, ExplicitlyMoveConstructible&&, ImplicitlyMoveConstructible&&>));

    const auto test = []() {
        tuple_type to_copy{ 1,2 };
        const tuple_type copy{ hud::move(to_copy) };
        return std::tuple{
            hud::get<0>(copy).id() == 1,
            hud::get<0>(copy).copy_constructor_count() == 0u,
            hud::get<0>(copy).move_constructor_count() == 1u,
            hud::get<1>(copy).id() == 2,
            hud::get<1>(copy).copy_constructor_count() == 0u,
            hud::get<1>(copy).move_constructor_count() == 1u,
        };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
        ASSERT_TRUE(std::get<4>(result));
        ASSERT_TRUE(std::get<5>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
        ASSERT_TRUE(std::get<4>(result));
        ASSERT_TRUE(std::get<5>(result));
    }
}

TEST(tuple, move_constructor_moveable_different_types) {
    using tuple_type = hud::tuple<ExplicitlyMoveConstructible2, ImplicitlyMoveConstructible2>;
    using other_tuple_type = hud::tuple<ExplicitlyMoveConstructible, ImplicitlyMoveConstructible>;

    ASSERT_TRUE((hud::is_explicitly_constructible_v<tuple_type, ExplicitlyMoveConstructible&&, ImplicitlyMoveConstructible&&>));

    const auto test = []() {
        other_tuple_type to_copy{ 1,2 };
        const tuple_type copy{ hud::move(to_copy) };
        return std::tuple{
            hud::get<0>(copy).id() == 1,
            hud::get<0>(copy).copy_constructor_count() == 0u,
            hud::get<0>(copy).move_constructor_count() == 1u,
            hud::get<1>(copy).id() == 2,
            hud::get<1>(copy).copy_constructor_count() == 0u,
            hud::get<1>(copy).move_constructor_count() == 1u,
        };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
        ASSERT_TRUE(std::get<4>(result));
        ASSERT_TRUE(std::get<5>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
        ASSERT_TRUE(std::get<4>(result));
        ASSERT_TRUE(std::get<5>(result));
    }
}
