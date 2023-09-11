#include <core/containers/tuple.h>

#include <core/containers/pair.h>

#include <core/traits/is_explicitly_constructible.h>
#include <core/traits/is_explicitly_copy_constructible.h>
#include <core/traits/is_explicitly_move_constructible.h>
#include <core/traits/is_implicitly_constructible.h>
#include <core/traits/is_implicitly_copy_constructible.h>
#include <core/traits/is_implicitly_move_constructible.h>
#include <core/traits/is_same.h>

#include <math.h> // isnan

namespace hud_test
{
    struct explicit_default_constructible
        : public hud_test::default_constructible_type
    {
        constexpr explicit explicit_default_constructible() noexcept
            : hud_test::default_constructible_type()
        {
        }
    };

    static_assert(hud::is_explicitly_default_constructible_v<explicit_default_constructible>);

    struct implicit_default_constructible
        : public hud_test::default_constructible_type
    {
        constexpr implicit_default_constructible() noexcept
            : hud_test::default_constructible_type()
        {
        }
    };

    static_assert(hud::is_implicitly_default_constructible_v<implicit_default_constructible>);

    struct explicitly_copy_constructible
        : public hud_test::NonBitwiseCopyConstructibleType
    {

        constexpr explicitly_copy_constructible(i32 id) noexcept
            : hud_test::NonBitwiseCopyConstructibleType(id)
        {
        }

        explicit constexpr explicitly_copy_constructible(const explicitly_copy_constructible &other) noexcept
            : hud_test::NonBitwiseCopyConstructibleType(other)
        {
        }
    };

    static_assert(hud::is_explicitly_copy_constructible_v<explicitly_copy_constructible>);

    struct explicitly_copy_constructible2
        : public hud_test::NonBitwiseCopyConstructibleType2
    {

        constexpr explicitly_copy_constructible2(i32 id) noexcept
            : hud_test::NonBitwiseCopyConstructibleType2(id)
        {
        }

        explicit constexpr explicitly_copy_constructible2(const explicitly_copy_constructible &other) noexcept
            : hud_test::NonBitwiseCopyConstructibleType2(other)
        {
        }
    };

    static_assert(hud::is_explicitly_copy_constructible_v<explicitly_copy_constructible2, explicitly_copy_constructible>);

    struct implicitly_copy_constructible
        : public hud_test::NonBitwiseCopyConstructibleType
    {

        constexpr implicitly_copy_constructible(i32 id) noexcept
            : hud_test::NonBitwiseCopyConstructibleType(id)
        {
        }

        constexpr implicitly_copy_constructible(const implicitly_copy_constructible &other) noexcept
            : hud_test::NonBitwiseCopyConstructibleType(other)
        {
        }
    };

    static_assert(hud::is_implicitly_copy_constructible_v<implicitly_copy_constructible>);

    struct implicitly_copy_constructible2
        : public hud_test::NonBitwiseCopyConstructibleType2
    {

        constexpr implicitly_copy_constructible2(i32 id) noexcept
            : hud_test::NonBitwiseCopyConstructibleType2(id)
        {
        }

        constexpr implicitly_copy_constructible2(const implicitly_copy_constructible &other) noexcept
            : hud_test::NonBitwiseCopyConstructibleType2(other)
        {
        }
    };

    static_assert(hud::is_implicitly_copy_constructible_v<implicitly_copy_constructible2, implicitly_copy_constructible>);

    struct explicitly_move_constructible
        : public hud_test::NonBitwiseMoveConstructibleType
    {

        constexpr explicitly_move_constructible(i32 id) noexcept
            : hud_test::NonBitwiseMoveConstructibleType(id)
        {
        }

        explicit constexpr explicitly_move_constructible(const explicitly_move_constructible &other) noexcept
            : hud_test::NonBitwiseMoveConstructibleType(other)
        {
        }

        explicit constexpr explicitly_move_constructible(explicitly_move_constructible &&other) noexcept
            : hud_test::NonBitwiseMoveConstructibleType(hud::move(other))
        {
        }
    };

    static_assert(hud::is_explicitly_move_constructible_v<explicitly_move_constructible>);

    struct explicitly_move_constructible2
        : public hud_test::NonBitwiseMoveConstructibleType2
    {

        constexpr explicitly_move_constructible2(i32 id) noexcept
            : hud_test::NonBitwiseMoveConstructibleType2(id)
        {
        }

        explicit constexpr explicitly_move_constructible2(const explicitly_move_constructible &other) noexcept
            : hud_test::NonBitwiseMoveConstructibleType2(other)
        {
        }

        explicit constexpr explicitly_move_constructible2(explicitly_move_constructible &&other) noexcept
            : hud_test::NonBitwiseMoveConstructibleType2(hud::move(other))
        {
        }
    };

    static_assert(hud::is_explicitly_move_constructible_v<explicitly_move_constructible2, explicitly_move_constructible>);

    struct implicitly_move_constructible
        : public hud_test::NonBitwiseMoveConstructibleType
    {

        constexpr implicitly_move_constructible(i32 id) noexcept
            : hud_test::NonBitwiseMoveConstructibleType(id)
        {
        }

        constexpr implicitly_move_constructible(const implicitly_move_constructible &other) noexcept
            : hud_test::NonBitwiseMoveConstructibleType(other)
        {
        }

        constexpr implicitly_move_constructible(implicitly_move_constructible &&other) noexcept
            : hud_test::NonBitwiseMoveConstructibleType(hud::move(other))
        {
        }
    };

    static_assert(hud::is_implicitly_move_constructible_v<implicitly_move_constructible>);

    struct implicitly_move_constructible2
        : public hud_test::NonBitwiseMoveConstructibleType2
    {

        constexpr implicitly_move_constructible2(i32 id) noexcept
            : hud_test::NonBitwiseMoveConstructibleType2(id)
        {
        }

        constexpr implicitly_move_constructible2(const implicitly_move_constructible &other) noexcept
            : hud_test::NonBitwiseMoveConstructibleType2(other)
        {
        }

        constexpr implicitly_move_constructible2(implicitly_move_constructible &&other) noexcept
            : hud_test::NonBitwiseMoveConstructibleType2(hud::move(other))
        {
        }
    };

    static_assert(hud::is_implicitly_move_constructible_v<implicitly_move_constructible2, implicitly_move_constructible>);
} // namespace hud_test

GTEST_TEST(tuple, default_constructor_uninit)
{
    using tuple_type = hud::tuple<>;
    using tuple_type2 = hud::tuple<i32, i32, i32>;
    using tuple_type3 = hud::tuple<i32, f32, wchar>;
    using tuple_type4 = hud::tuple<hud_test::explicit_default_constructible, hud_test::implicit_default_constructible>;
    using tuple_type5 = hud::tuple<hud_test::explicit_default_constructible, i32>;

    hud_assert_eq(hud::tuple_size_v<tuple_type>, 0u);
    hud_assert_eq(hud::tuple_size_v<tuple_type2>, 3u);
    hud_assert_eq(hud::tuple_size_v<tuple_type3>, 3u);
    hud_assert_eq(hud::tuple_size_v<tuple_type4>, 2u);
    hud_assert_eq(hud::tuple_size_v<tuple_type5>, 2u);

    // Non constant
    {
        [[maybe_unused]] tuple_type tuple1;

        tuple_type2 *tuple2 = hud::memory::allocate_array<tuple_type2>(1);
        // Be sure optimizer don't remove the memmory::set
        volatile tuple_type2 *do_not_optimize_set_tuple2 = (volatile tuple_type2 *)hud::memory::set(tuple2, sizeof(tuple_type2), 0xFF);
        hud::memory::construct_at((tuple_type2 *)do_not_optimize_set_tuple2);
        hud_assert_true((hud::is_same_v<decltype(hud::get<0>(*tuple2)), i32 &>));
        hud_assert_true((hud::is_same_v<decltype(hud::get<1>(*tuple2)), i32 &>));
        hud_assert_true((hud::is_same_v<decltype(hud::get<2>(*tuple2)), i32 &>));
        hud_assert_ne((hud::get<0>(*tuple2)), i32 {});
        hud_assert_ne((hud::get<1>(*tuple2)), i32 {});
        hud_assert_ne((hud::get<2>(*tuple2)), i32 {});
        hud::memory::destroy_array(tuple2, 1);
        hud::memory::free(tuple2);

        tuple_type3 *tuple3 = hud::memory::allocate_array<tuple_type3>(1);
        // Be sure optimizer don't remove the memmory::set
        volatile tuple_type3 *do_not_optimize_set_tuple3 = (volatile tuple_type3 *)hud::memory::set(tuple3, sizeof(tuple_type3), 0xFF);
        hud::memory::construct_at((tuple_type3 *)do_not_optimize_set_tuple3);
        hud_assert_true((hud::is_same_v<decltype(hud::get<0>(*tuple3)), i32 &>));
        hud_assert_true((hud::is_same_v<decltype(hud::get<1>(*tuple3)), f32 &>));
        hud_assert_true((hud::is_same_v<decltype(hud::get<2>(*tuple3)), wchar &>));
        hud_assert_ne((hud::get<0>(*tuple3)), i32 {});

// Is nan always return false with clang in release with fast math enabled
// See: https://clang.llvm.org/docs/UsersManual.html#cmdoption-ffast-math
#if defined(HD_COMPILER_CLANG) && defined(HD_RELEASE) && defined(__FAST_MATH__)
        hud_assert_false(isnan(hud::get<1>(*tuple3)));
#else
        hud_assert_true(isnan(hud::get<1>(*tuple3)));
#endif
        hud_assert_ne((hud::get<2>(*tuple3)), wchar {});
        hud::memory::destroy_array(tuple3, 1);
        hud::memory::free(tuple3);

        tuple_type4 *tuple4 = hud::memory::allocate_array<tuple_type4>(1);
        // Be sure optimizer don't remove the memmory::set
        volatile tuple_type4 *do_not_optimize_set_tuple4 = (volatile tuple_type4 *)hud::memory::set(tuple4, sizeof(tuple_type4), 0xFF);
        hud::memory::construct_at((tuple_type4 *)do_not_optimize_set_tuple4);
        hud_assert_true((hud::is_same_v<decltype(hud::get<0>(*tuple4)), hud_test::explicit_default_constructible &>));
        hud_assert_true((hud::is_same_v<decltype(hud::get<1>(*tuple4)), hud_test::implicit_default_constructible &>));
        hud_assert_eq((hud::get<0>(*tuple4).id()), hud_test::explicit_default_constructible::DEFAULT_ID_VALUE);
        hud_assert_eq((hud::get<0>(*tuple4).default_constructor_count()), 1u);
        hud_assert_eq((hud::get<0>(*tuple4).constructor_count()), 0u);
        hud_assert_eq((hud::get<0>(*tuple4).copy_constructor_count()), 0u);
        hud_assert_eq((hud::get<0>(*tuple4).move_constructor_count()), 0u);
        hud_assert_eq((hud::get<1>(*tuple4).id()), hud_test::implicit_default_constructible::DEFAULT_ID_VALUE);
        hud_assert_eq((hud::get<1>(*tuple4).default_constructor_count()), 1u);
        hud_assert_eq((hud::get<1>(*tuple4).constructor_count()), 0u);
        hud_assert_eq((hud::get<1>(*tuple4).copy_constructor_count()), 0u);
        hud_assert_eq((hud::get<1>(*tuple4).move_constructor_count()), 0u);
        hud::memory::destroy_array(tuple4, 1);
        hud::memory::free(tuple4);

        tuple_type5 *tuple5 = hud::memory::allocate_array<tuple_type5>(1);
        // Be sure optimizer don't remove the memmory::set
        volatile tuple_type5 *do_not_optimize_set_tuple5 = (volatile tuple_type5 *)hud::memory::set(tuple5, sizeof(tuple_type5), 0xFF);
        hud::memory::construct_at((tuple_type5 *)do_not_optimize_set_tuple5);
        hud_assert_true((hud::is_same_v<decltype(hud::get<0>(*tuple5)), hud_test::explicit_default_constructible &>));
        hud_assert_true((hud::is_same_v<decltype(hud::get<1>(*tuple5)), i32 &>));
        hud_assert_eq((hud::get<0>(*tuple5).id()), hud_test::explicit_default_constructible::DEFAULT_ID_VALUE);
        hud_assert_eq((hud::get<0>(*tuple5).default_constructor_count()), 1u);
        hud_assert_eq((hud::get<0>(*tuple5).constructor_count()), 0u);
        hud_assert_eq((hud::get<0>(*tuple5).copy_constructor_count()), 0u);
        hud_assert_eq((hud::get<0>(*tuple5).move_constructor_count()), 0u);
        hud_assert_ne(hud::get<1>(*tuple5), i32 {});
        hud::memory::destroy_array(tuple5, 1);
        hud::memory::free(tuple5);
    }

    // Constant
    // Non empty uninit constexpr tuple is not possible because constexpr need initialized value
    {
        [[maybe_unused]] constexpr tuple_type tuple;
        hud_assert_eq(hud::tuple_size_v<tuple_type>, 0u);
    }
}

GTEST_TEST(tuple, default_constructor_init)
{
    using tuple_type = hud::tuple<>;
    using tuple_type2 = hud::tuple<i32, i32, i32>;
    using tuple_type3 = hud::tuple<i32, f32, wchar>;
    using tuple_type4 = hud::tuple<hud_test::explicit_default_constructible, hud_test::implicit_default_constructible>;
    using tuple_type5 = hud::tuple<hud_test::explicit_default_constructible, i32>;

    hud_assert_eq(hud::tuple_size_v<tuple_type>, 0u);
    hud_assert_eq(hud::tuple_size_v<tuple_type2>, 3u);
    hud_assert_eq(hud::tuple_size_v<tuple_type3>, 3u);
    hud_assert_eq(hud::tuple_size_v<tuple_type4>, 2u);
    hud_assert_eq(hud::tuple_size_v<tuple_type5>, 2u);

    // Non constant
    {
        // No init for empty tuple
        [[maybe_unused]] tuple_type tuple1;

        tuple_type2 tuple2 {hud::taginit};
        hud_assert_true((hud::is_same_v<decltype(hud::get<0>(tuple2)), i32 &>));
        hud_assert_true((hud::is_same_v<decltype(hud::get<1>(tuple2)), i32 &>));
        hud_assert_true((hud::is_same_v<decltype(hud::get<2>(tuple2)), i32 &>));
        hud_assert_eq((hud::get<0>(tuple2)), i32 {});
        hud_assert_eq((hud::get<1>(tuple2)), i32 {});
        hud_assert_eq((hud::get<2>(tuple2)), i32 {});

        tuple_type3 tuple3 {hud::taginit};
        hud_assert_true((hud::is_same_v<decltype(hud::get<0>(tuple3)), i32 &>));
        hud_assert_true((hud::is_same_v<decltype(hud::get<1>(tuple3)), f32 &>));
        hud_assert_true((hud::is_same_v<decltype(hud::get<2>(tuple3)), wchar &>));
        hud_assert_eq((hud::get<0>(tuple3)), i32 {});
        hud_assert_eq((hud::get<1>(tuple3)), f32 {});
        hud_assert_eq((hud::get<2>(tuple3)), wchar {});

        tuple_type4 tuple4 {hud::taginit};
        hud_assert_true((hud::is_same_v<decltype(hud::get<0>(tuple4)), hud_test::explicit_default_constructible &>));
        hud_assert_true((hud::is_same_v<decltype(hud::get<1>(tuple4)), hud_test::implicit_default_constructible &>));
        hud_assert_eq((hud::get<0>(tuple4).id()), hud_test::explicit_default_constructible::DEFAULT_ID_VALUE);
        hud_assert_eq((hud::get<0>(tuple4).default_constructor_count()), 1u);
        hud_assert_eq((hud::get<0>(tuple4).constructor_count()), 0u);
        hud_assert_eq((hud::get<0>(tuple4).copy_constructor_count()), 0u);
        hud_assert_eq((hud::get<0>(tuple4).move_constructor_count()), 0u);
        hud_assert_eq((hud::get<1>(tuple4).id()), hud_test::implicit_default_constructible::DEFAULT_ID_VALUE);
        hud_assert_eq((hud::get<1>(tuple4).default_constructor_count()), 1u);
        hud_assert_eq((hud::get<1>(tuple4).constructor_count()), 0u);
        hud_assert_eq((hud::get<1>(tuple4).copy_constructor_count()), 0u);
        hud_assert_eq((hud::get<1>(tuple4).move_constructor_count()), 0u);

        tuple_type5 tuple5 {hud::taginit};
        hud_assert_true((hud::is_same_v<decltype(hud::get<0>(tuple5)), hud_test::explicit_default_constructible &>));
        hud_assert_true((hud::is_same_v<decltype(hud::get<1>(tuple5)), i32 &>));
        hud_assert_eq((hud::get<0>(tuple5).id()), hud_test::explicit_default_constructible::DEFAULT_ID_VALUE);
        hud_assert_eq((hud::get<0>(tuple5).default_constructor_count()), 1u);
        hud_assert_eq((hud::get<0>(tuple5).constructor_count()), 0u);
        hud_assert_eq((hud::get<0>(tuple5).copy_constructor_count()), 0u);
        hud_assert_eq((hud::get<0>(tuple5).move_constructor_count()), 0u);
        hud_assert_eq((hud::get<1>(tuple5)), i32 {});
    }

    // Constant
    {
        [[maybe_unused]] constexpr tuple_type tuple1;

        constexpr tuple_type2 tuple2 {hud::taginit};
        hud_assert_true((hud::is_same_v<decltype(hud::get<0>(tuple2)), const i32 &>));
        hud_assert_true((hud::is_same_v<decltype(hud::get<1>(tuple2)), const i32 &>));
        hud_assert_true((hud::is_same_v<decltype(hud::get<2>(tuple2)), const i32 &>));

        hud_assert_eq((hud::get<0>(tuple2)), i32 {});
        hud_assert_eq((hud::get<1>(tuple2)), i32 {});
        hud_assert_eq((hud::get<2>(tuple2)), i32 {});

        constexpr tuple_type3 tuple3 {hud::taginit};
        hud_assert_true((hud::is_same_v<decltype(hud::get<0>(tuple3)), const i32 &>));
        hud_assert_true((hud::is_same_v<decltype(hud::get<1>(tuple3)), const f32 &>));
        hud_assert_true((hud::is_same_v<decltype(hud::get<2>(tuple3)), const wchar &>));

        hud_assert_eq((hud::get<0>(tuple3)), i32 {});
        hud_assert_eq((hud::get<1>(tuple3)), f32 {});
        hud_assert_eq((hud::get<2>(tuple3)), wchar {});

        constexpr tuple_type4 tuple4 {hud::taginit};
        hud_assert_true((hud::is_same_v<decltype(hud::get<0>(tuple4)), const hud_test::explicit_default_constructible &>));
        hud_assert_true((hud::is_same_v<decltype(hud::get<1>(tuple4)), const hud_test::implicit_default_constructible &>));
        hud_assert_eq((hud::get<0>(tuple4).id()), hud_test::explicit_default_constructible::DEFAULT_ID_VALUE);
        hud_assert_eq((hud::get<0>(tuple4).default_constructor_count()), 1u);
        hud_assert_eq((hud::get<0>(tuple4).constructor_count()), 0u);
        hud_assert_eq((hud::get<0>(tuple4).copy_constructor_count()), 0u);
        hud_assert_eq((hud::get<0>(tuple4).move_constructor_count()), 0u);
        hud_assert_eq((hud::get<1>(tuple4).id()), hud_test::implicit_default_constructible::DEFAULT_ID_VALUE);
        hud_assert_eq((hud::get<1>(tuple4).default_constructor_count()), 1u);
        hud_assert_eq((hud::get<1>(tuple4).constructor_count()), 0u);
        hud_assert_eq((hud::get<1>(tuple4).copy_constructor_count()), 0u);
        hud_assert_eq((hud::get<1>(tuple4).move_constructor_count()), 0u);

        constexpr tuple_type5 tuple5 {hud::taginit};
        hud_assert_true((hud::is_same_v<decltype(hud::get<0>(tuple5)), const hud_test::explicit_default_constructible &>));
        hud_assert_true((hud::is_same_v<decltype(hud::get<1>(tuple5)), const i32 &>));
        hud_assert_eq((hud::get<0>(tuple5).id()), hud_test::explicit_default_constructible::DEFAULT_ID_VALUE);
        hud_assert_eq((hud::get<0>(tuple5).default_constructor_count()), 1u);
        hud_assert_eq((hud::get<0>(tuple5).constructor_count()), 0u);
        hud_assert_eq((hud::get<0>(tuple5).copy_constructor_count()), 0u);
        hud_assert_eq((hud::get<0>(tuple5).move_constructor_count()), 0u);
        hud_assert_eq((hud::get<1>(tuple5)), i32 {});
    }
}

GTEST_TEST(tuple, default_constructor_explicit_types)
{
    using tuple_type = hud::tuple<hud_test::explicit_default_constructible, hud_test::explicit_default_constructible>;

    hud_assert_true(hud::is_explicitly_default_constructible_v<tuple_type>);

    // Non constant
    {
        // No init for empty tuple
        tuple_type tuple;

        hud_assert_eq((hud::get<0>(tuple).id()), hud_test::explicit_default_constructible::DEFAULT_ID_VALUE);
        hud_assert_eq((hud::get<0>(tuple).default_constructor_count()), 1u);
        hud_assert_eq((hud::get<0>(tuple).constructor_count()), 0u);
        hud_assert_eq((hud::get<0>(tuple).copy_constructor_count()), 0u);
        hud_assert_eq((hud::get<0>(tuple).move_constructor_count()), 0u);
        hud_assert_eq((hud::get<1>(tuple).id()), hud_test::explicit_default_constructible::DEFAULT_ID_VALUE);
        hud_assert_eq((hud::get<1>(tuple).default_constructor_count()), 1u);
        hud_assert_eq((hud::get<1>(tuple).constructor_count()), 0u);
        hud_assert_eq((hud::get<1>(tuple).copy_constructor_count()), 0u);
        hud_assert_eq((hud::get<1>(tuple).move_constructor_count()), 0u);
    }

    // Constant
    {
        // No init for empty tuple
        constexpr tuple_type tuple;

        hud_assert_eq((hud::get<0>(tuple).id()), hud_test::explicit_default_constructible::DEFAULT_ID_VALUE);
        hud_assert_eq((hud::get<0>(tuple).default_constructor_count()), 1u);
        hud_assert_eq((hud::get<0>(tuple).constructor_count()), 0u);
        hud_assert_eq((hud::get<0>(tuple).copy_constructor_count()), 0u);
        hud_assert_eq((hud::get<0>(tuple).move_constructor_count()), 0u);
        hud_assert_eq((hud::get<1>(tuple).id()), hud_test::explicit_default_constructible::DEFAULT_ID_VALUE);
        hud_assert_eq((hud::get<1>(tuple).default_constructor_count()), 1u);
        hud_assert_eq((hud::get<1>(tuple).constructor_count()), 0u);
        hud_assert_eq((hud::get<1>(tuple).copy_constructor_count()), 0u);
        hud_assert_eq((hud::get<1>(tuple).move_constructor_count()), 0u);
    }
}

GTEST_TEST(tuple, default_constructor_implicit_types)
{
    using tuple_type = hud::tuple<hud_test::implicit_default_constructible, hud_test::implicit_default_constructible>;

    hud_assert_true(hud::is_implicitly_default_constructible_v<tuple_type>);

    // Non constant
    {
        // No init for empty tuple
        tuple_type tuple;

        hud_assert_eq((hud::get<0>(tuple).id()), hud_test::implicit_default_constructible::DEFAULT_ID_VALUE);
        hud_assert_eq((hud::get<0>(tuple).default_constructor_count()), 1u);
        hud_assert_eq((hud::get<0>(tuple).constructor_count()), 0u);
        hud_assert_eq((hud::get<0>(tuple).copy_constructor_count()), 0u);
        hud_assert_eq((hud::get<0>(tuple).move_constructor_count()), 0u);
        hud_assert_eq((hud::get<1>(tuple).id()), hud_test::implicit_default_constructible::DEFAULT_ID_VALUE);
        hud_assert_eq((hud::get<1>(tuple).default_constructor_count()), 1u);
        hud_assert_eq((hud::get<1>(tuple).constructor_count()), 0u);
        hud_assert_eq((hud::get<1>(tuple).copy_constructor_count()), 0u);
        hud_assert_eq((hud::get<1>(tuple).move_constructor_count()), 0u);
    }

    // Constant
    {
        // No init for empty tuple
        constexpr tuple_type tuple;

        hud_assert_eq((hud::get<0>(tuple).id()), hud_test::implicit_default_constructible::DEFAULT_ID_VALUE);
        hud_assert_eq((hud::get<0>(tuple).default_constructor_count()), 1u);
        hud_assert_eq((hud::get<0>(tuple).constructor_count()), 0u);
        hud_assert_eq((hud::get<0>(tuple).copy_constructor_count()), 0u);
        hud_assert_eq((hud::get<0>(tuple).move_constructor_count()), 0u);
        hud_assert_eq((hud::get<1>(tuple).id()), hud_test::implicit_default_constructible::DEFAULT_ID_VALUE);
        hud_assert_eq((hud::get<1>(tuple).default_constructor_count()), 1u);
        hud_assert_eq((hud::get<1>(tuple).constructor_count()), 0u);
        hud_assert_eq((hud::get<1>(tuple).copy_constructor_count()), 0u);
        hud_assert_eq((hud::get<1>(tuple).move_constructor_count()), 0u);
    }
}

GTEST_TEST(tuple, default_constructor_mix_implicit_explicit_types)
{
    using tuple_type = hud::tuple<hud_test::implicit_default_constructible, hud_test::explicit_default_constructible>;

    hud_assert_true(hud::is_explicitly_default_constructible_v<tuple_type>);

    // Non constant
    {
        // No init for empty tuple
        tuple_type tuple;

        hud_assert_eq((hud::get<0>(tuple).id()), hud_test::implicit_default_constructible::DEFAULT_ID_VALUE);
        hud_assert_eq((hud::get<0>(tuple).default_constructor_count()), 1u);
        hud_assert_eq((hud::get<0>(tuple).constructor_count()), 0u);
        hud_assert_eq((hud::get<0>(tuple).copy_constructor_count()), 0u);
        hud_assert_eq((hud::get<0>(tuple).move_constructor_count()), 0u);
        hud_assert_eq((hud::get<1>(tuple).id()), hud_test::explicit_default_constructible::DEFAULT_ID_VALUE);
        hud_assert_eq((hud::get<1>(tuple).default_constructor_count()), 1u);
        hud_assert_eq((hud::get<1>(tuple).constructor_count()), 0u);
        hud_assert_eq((hud::get<1>(tuple).copy_constructor_count()), 0u);
        hud_assert_eq((hud::get<1>(tuple).move_constructor_count()), 0u);
    }

    // Constant
    {
        // No init for empty tuple
        constexpr tuple_type tuple;

        hud_assert_eq((hud::get<0>(tuple).id()), hud_test::implicit_default_constructible::DEFAULT_ID_VALUE);
        hud_assert_eq((hud::get<0>(tuple).default_constructor_count()), 1u);
        hud_assert_eq((hud::get<0>(tuple).constructor_count()), 0u);
        hud_assert_eq((hud::get<0>(tuple).copy_constructor_count()), 0u);
        hud_assert_eq((hud::get<0>(tuple).move_constructor_count()), 0u);
        hud_assert_eq((hud::get<1>(tuple).id()), hud_test::explicit_default_constructible::DEFAULT_ID_VALUE);
        hud_assert_eq((hud::get<1>(tuple).default_constructor_count()), 1u);
        hud_assert_eq((hud::get<1>(tuple).constructor_count()), 0u);
        hud_assert_eq((hud::get<1>(tuple).copy_constructor_count()), 0u);
        hud_assert_eq((hud::get<1>(tuple).move_constructor_count()), 0u);
    }
}

GTEST_TEST(tuple, default_constructor_is_explicit_if_at_least_one_type_is_explicitly_default_constructible)
{
    hud_assert_false(hud::is_explicitly_default_constructible_v<hud::tuple<>>);
    hud_assert_false((hud::is_explicitly_default_constructible_v<hud::tuple<i32, f32, wchar>>));
    hud_assert_false((hud::is_explicitly_default_constructible_v<hud::tuple<hud_test::implicit_default_constructible, hud_test::implicit_default_constructible>>));
    hud_assert_true((hud::is_explicitly_default_constructible_v<hud::tuple<hud_test::implicit_default_constructible, hud_test::explicit_default_constructible>>));
    hud_assert_true((hud::is_explicitly_default_constructible_v<hud::tuple<hud_test::explicit_default_constructible, hud_test::explicit_default_constructible>>));
}

GTEST_TEST(tuple, constructor_by_copy_explicitly_copyable_same_types)
{
    using tuple_type = hud::tuple<hud_test::explicitly_copy_constructible, hud_test::explicitly_copy_constructible>;

    hud_assert_true((hud::is_explicitly_constructible_v<tuple_type, const hud_test::explicitly_copy_constructible &, const hud_test::explicitly_copy_constructible &>));

    const auto test = []()
    {
        const hud_test::explicitly_copy_constructible obj1(1);
        const hud_test::explicitly_copy_constructible obj2(2);
        tuple_type tuple(obj1, obj2);
        return std::tuple {
            hud::get<0>(tuple).id() == 1,
            hud::get<0>(tuple).copy_constructor_count() == 1u,
            hud::get<1>(tuple).id() == 2,
            hud::get<1>(tuple).copy_constructor_count() == 1u};
    };
    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
    }
}

GTEST_TEST(tuple, constructor_by_copy_explicitly_copyable_different_types)
{
    using tuple_type = hud::tuple<hud_test::explicitly_copy_constructible2, hud_test::explicitly_copy_constructible2>;

    hud_assert_true((hud::is_explicitly_constructible_v<tuple_type, const hud_test::explicitly_copy_constructible &, const hud_test::explicitly_copy_constructible &>));

    const auto test = []()
    {
        const hud_test::explicitly_copy_constructible obj1(1);
        const hud_test::explicitly_copy_constructible obj2(2);
        tuple_type tuple(obj1, obj2);
        return std::tuple {
            hud::get<0>(tuple).id() == 1,
            hud::get<0>(tuple).copy_constructor_count() == 1u,
            hud::get<1>(tuple).id() == 2,
            hud::get<1>(tuple).copy_constructor_count() == 1u};
    };
    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
    }
}

GTEST_TEST(tuple, constructor_by_copy_implicitly_copyable_same_types)
{
    using tuple_type = hud::tuple<hud_test::implicitly_copy_constructible, hud_test::implicitly_copy_constructible>;

    hud_assert_true((hud::is_implicitly_constructible_v<tuple_type, const hud_test::implicitly_copy_constructible &, const hud_test::implicitly_copy_constructible &>));

    const auto test = []()
    {
        const hud_test::implicitly_copy_constructible obj1(1);
        const hud_test::implicitly_copy_constructible obj2(2);
        tuple_type tuple(obj1, obj2);
        return std::tuple {
            hud::get<0>(tuple).id() == 1,
            hud::get<0>(tuple).copy_constructor_count() == 1u,
            hud::get<1>(tuple).id() == 2,
            hud::get<1>(tuple).copy_constructor_count() == 1u};
    };
    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
    }
}

GTEST_TEST(tuple, constructor_by_copy_implicitly_copyable_different_types)
{
    using tuple_type = hud::tuple<hud_test::implicitly_copy_constructible2, hud_test::implicitly_copy_constructible2>;

    hud_assert_true((hud::is_implicitly_constructible_v<tuple_type, const hud_test::implicitly_copy_constructible &, const hud_test::implicitly_copy_constructible &>));

    const auto test = []()
    {
        const hud_test::implicitly_copy_constructible obj1(1);
        const hud_test::implicitly_copy_constructible obj2(2);
        tuple_type tuple(obj1, obj2);
        return std::tuple {
            hud::get<0>(tuple).id() == 1,
            hud::get<0>(tuple).copy_constructor_count() == 1u,
            hud::get<1>(tuple).id() == 2,
            hud::get<1>(tuple).copy_constructor_count() == 1u};
    };
    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
    }
}

GTEST_TEST(tuple, constructor_by_copy_mix_implicitly_explicitly_copyable_same_types)
{
    using tuple_type = hud::tuple<hud_test::implicitly_copy_constructible, hud_test::explicitly_copy_constructible>;

    hud_assert_true((hud::is_explicitly_constructible_v<tuple_type, const hud_test::implicitly_copy_constructible &, const hud_test::explicitly_copy_constructible &>));

    const auto test = []()
    {
        const hud_test::implicitly_copy_constructible obj1(1);
        const hud_test::explicitly_copy_constructible obj2(2);
        tuple_type tuple(obj1, obj2);
        return std::tuple {
            hud::get<0>(tuple).id() == 1,
            hud::get<0>(tuple).copy_constructor_count() == 1u,
            hud::get<1>(tuple).id() == 2,
            hud::get<1>(tuple).copy_constructor_count() == 1u};
    };
    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
    }
}

GTEST_TEST(tuple, constructor_by_copy_mix_implicitly_explicitly_copyable_different_types)
{
    using tuple_type = hud::tuple<hud_test::implicitly_copy_constructible2, hud_test::explicitly_copy_constructible2>;

    hud_assert_true((hud::is_explicitly_constructible_v<tuple_type, const hud_test::implicitly_copy_constructible &, const hud_test::explicitly_copy_constructible &>));

    const auto test = []()
    {
        const hud_test::implicitly_copy_constructible obj1(1);
        const hud_test::explicitly_copy_constructible obj2(2);
        tuple_type tuple(obj1, obj2);
        return std::tuple {
            hud::get<0>(tuple).id() == 1,
            hud::get<0>(tuple).copy_constructor_count() == 1u,
            hud::get<1>(tuple).id() == 2,
            hud::get<1>(tuple).copy_constructor_count() == 1u};
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
    }
}

GTEST_TEST(tuple, constructor_by_move_explicitly_copyable_same_types)
{
    using tuple_type = hud::tuple<hud_test::explicitly_copy_constructible, hud_test::explicitly_copy_constructible>;

    hud_assert_true((hud::is_explicitly_constructible_v<tuple_type, hud_test::explicitly_copy_constructible &&, hud_test::explicitly_copy_constructible &&>));

    const auto test = []()
    {
        hud_test::explicitly_copy_constructible obj1(1);
        hud_test::explicitly_copy_constructible obj2(2);
        tuple_type tuple(hud::move(obj1), hud::move(obj2));
        return std::tuple {
            hud::get<0>(tuple).id() == 1,
            hud::get<0>(tuple).copy_constructor_count() == 1u,
            hud::get<1>(tuple).id() == 2,
            hud::get<1>(tuple).copy_constructor_count() == 1u};
    };
    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
    }
}

GTEST_TEST(tuple, constructor_by_move_explicitly_copyable_different_types)
{
    using tuple_type = hud::tuple<hud_test::explicitly_copy_constructible2, hud_test::explicitly_copy_constructible2>;

    hud_assert_true((hud::is_explicitly_constructible_v<tuple_type, hud_test::explicitly_copy_constructible &&, hud_test::explicitly_copy_constructible &&>));

    const auto test = []()
    {
        hud_test::explicitly_copy_constructible obj1(1);
        hud_test::explicitly_copy_constructible obj2(2);
        tuple_type tuple(hud::move(obj1), hud::move(obj2));
        return std::tuple {
            hud::get<0>(tuple).id() == 1,
            hud::get<0>(tuple).copy_constructor_count() == 1u,
            hud::get<1>(tuple).id() == 2,
            hud::get<1>(tuple).copy_constructor_count() == 1u};
    };
    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
    }
}

GTEST_TEST(tuple, constructor_by_move_implicitly_copyable_same_types)
{
    using tuple_type = hud::tuple<hud_test::implicitly_copy_constructible, hud_test::implicitly_copy_constructible>;

    hud_assert_true((hud::is_implicitly_constructible_v<tuple_type, hud_test::implicitly_copy_constructible &&, hud_test::implicitly_copy_constructible &&>));

    const auto test = []()
    {
        hud_test::implicitly_copy_constructible obj1(1);
        hud_test::implicitly_copy_constructible obj2(2);
        tuple_type tuple(hud::move(obj1), hud::move(obj2));
        return std::tuple {
            hud::get<0>(tuple).id() == 1,
            hud::get<0>(tuple).copy_constructor_count() == 1u,
            hud::get<1>(tuple).id() == 2,
            hud::get<1>(tuple).copy_constructor_count() == 1u};
    };
    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
    }
}

GTEST_TEST(tuple, constructor_by_move_implicitly_copyable_different_types)
{
    using tuple_type = hud::tuple<hud_test::implicitly_copy_constructible2, hud_test::implicitly_copy_constructible2>;

    hud_assert_true((hud::is_implicitly_constructible_v<tuple_type, hud_test::implicitly_copy_constructible &&, hud_test::implicitly_copy_constructible &&>));

    const auto test = []()
    {
        hud_test::implicitly_copy_constructible obj1(1);
        hud_test::implicitly_copy_constructible obj2(2);
        tuple_type tuple(hud::move(obj1), hud::move(obj2));
        return std::tuple {
            hud::get<0>(tuple).id() == 1,
            hud::get<0>(tuple).copy_constructor_count() == 1u,
            hud::get<1>(tuple).id() == 2,
            hud::get<1>(tuple).copy_constructor_count() == 1u};
    };
    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
    }
}

GTEST_TEST(tuple, constructor_by_move_mix_implicitly_explicitly_copyable_same_types)
{
    using tuple_type = hud::tuple<hud_test::implicitly_copy_constructible, hud_test::explicitly_copy_constructible>;

    hud_assert_true((hud::is_explicitly_constructible_v<tuple_type, hud_test::implicitly_copy_constructible &&, hud_test::explicitly_copy_constructible &&>));

    const auto test = []()
    {
        hud_test::implicitly_copy_constructible obj1(1);
        hud_test::explicitly_copy_constructible obj2(2);
        tuple_type tuple(hud::move(obj1), hud::move(obj2));
        return std::tuple {
            hud::get<0>(tuple).id() == 1,
            hud::get<0>(tuple).copy_constructor_count() == 1u,
            hud::get<1>(tuple).id() == 2,
            hud::get<1>(tuple).copy_constructor_count() == 1u};
    };
    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
    }
}

GTEST_TEST(tuple, constructor_by_move_mix_implicitly_explicitly_copyable_different_types)
{
    using tuple_type = hud::tuple<hud_test::implicitly_copy_constructible2, hud_test::explicitly_copy_constructible2>;

    hud_assert_true((hud::is_explicitly_constructible_v<tuple_type, hud_test::implicitly_copy_constructible &&, hud_test::explicitly_copy_constructible &&>));

    const auto test = []()
    {
        hud_test::implicitly_copy_constructible obj1(1);
        hud_test::explicitly_copy_constructible obj2(2);
        tuple_type tuple(hud::move(obj1), hud::move(obj2));
        return std::tuple {
            hud::get<0>(tuple).id() == 1,
            hud::get<0>(tuple).copy_constructor_count() == 1u,
            hud::get<1>(tuple).id() == 2,
            hud::get<1>(tuple).copy_constructor_count() == 1u};
    };
    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
    }
}

GTEST_TEST(tuple, constructor_by_move_explicitly_moveable_same_types)
{
    using tuple_type = hud::tuple<hud_test::explicitly_move_constructible, hud_test::explicitly_move_constructible>;

    hud_assert_true((hud::is_explicitly_constructible_v<tuple_type, hud_test::explicitly_move_constructible &&, hud_test::explicitly_move_constructible &&>));

    const auto test = []()
    {
        hud_test::explicitly_move_constructible obj1(1);
        hud_test::explicitly_move_constructible obj2(2);
        tuple_type tuple(hud::move(obj1), hud::move(obj2));
        return std::tuple {
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
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
    }

    // Constant
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

GTEST_TEST(tuple, constructor_by_move_explicitly_moveable_different_types)
{
    using tuple_type = hud::tuple<hud_test::explicitly_move_constructible2, hud_test::explicitly_move_constructible2>;

    hud_assert_true((hud::is_explicitly_constructible_v<tuple_type, hud_test::explicitly_move_constructible &&, hud_test::explicitly_move_constructible &&>));

    const auto test = []()
    {
        hud_test::explicitly_move_constructible obj1(1);
        hud_test::explicitly_move_constructible obj2(2);
        tuple_type tuple(hud::move(obj1), hud::move(obj2));
        return std::tuple {
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
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
    }

    // Constant
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

GTEST_TEST(tuple, constructor_by_move_implicitly_moveable_same_types)
{
    using tuple_type = hud::tuple<hud_test::implicitly_move_constructible, hud_test::implicitly_move_constructible>;

    hud_assert_true((hud::is_implicitly_constructible_v<tuple_type, hud_test::implicitly_move_constructible &&, hud_test::implicitly_move_constructible &&>));

    const auto test = []()
    {
        hud_test::implicitly_move_constructible obj1(1);
        hud_test::implicitly_move_constructible obj2(2);
        tuple_type tuple(hud::move(obj1), hud::move(obj2));
        return std::tuple {
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
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
    }

    // Constant
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

GTEST_TEST(tuple, constructor_by_move_implicitly_moveable_different_types)
{
    using tuple_type = hud::tuple<hud_test::implicitly_move_constructible2, hud_test::implicitly_move_constructible2>;

    hud_assert_true((hud::is_implicitly_constructible_v<tuple_type, hud_test::implicitly_move_constructible &&, hud_test::implicitly_move_constructible &&>));

    const auto test = []()
    {
        hud_test::implicitly_move_constructible obj1(1);
        hud_test::implicitly_move_constructible obj2(2);
        tuple_type tuple(hud::move(obj1), hud::move(obj2));
        return std::tuple {
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
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
    }

    // Constant
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

GTEST_TEST(tuple, constructor_by_move_mix_implicitly_explicitly_moveable_same_types)
{
    using tuple_type = hud::tuple<hud_test::implicitly_move_constructible, hud_test::explicitly_move_constructible>;

    hud_assert_true((hud::is_explicitly_constructible_v<tuple_type, hud_test::implicitly_move_constructible &&, hud_test::explicitly_move_constructible &&>));

    const auto test = []()
    {
        hud_test::implicitly_move_constructible obj1(1);
        hud_test::explicitly_move_constructible obj2(2);
        tuple_type tuple(hud::move(obj1), hud::move(obj2));
        return std::tuple {
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
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
    }

    // Constant
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

GTEST_TEST(tuple, constructor_by_move_mix_implicitly_explicitly_moveable_different_types)
{
    using tuple_type = hud::tuple<hud_test::implicitly_move_constructible2, hud_test::explicitly_move_constructible2>;

    hud_assert_true((hud::is_explicitly_constructible_v<tuple_type, hud_test::implicitly_move_constructible &&, hud_test::explicitly_move_constructible &&>));

    const auto test = []()
    {
        hud_test::implicitly_move_constructible obj1(1);
        hud_test::explicitly_move_constructible obj2(2);
        tuple_type tuple(hud::move(obj1), hud::move(obj2));
        return std::tuple {
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
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
    }

    // Constant
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

GTEST_TEST(tuple, constructor_by_copy_pair_of_explictly_copyable_same_types)
{
    using pair_type = hud::pair<hud_test::explicitly_copy_constructible, hud_test::explicitly_copy_constructible>;
    using tuple_type = hud::tuple<hud_test::explicitly_copy_constructible, hud_test::explicitly_copy_constructible>;

    hud_assert_true((hud::is_explicitly_constructible_v<tuple_type, const pair_type &>));

    const auto test = []()
    {
        const pair_type pair {1, 2};
        const tuple_type tuple {pair};
        return std::tuple {
            hud::get<0>(tuple).id(),
            hud::get<0>(tuple).copy_constructor_count() == 1u,
            hud::get<1>(tuple).id(),
            hud::get<1>(tuple).copy_constructor_count() == 1u,
        };
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
    }
}

GTEST_TEST(tuple, constructor_by_copy_pair_of_explictly_copyable_different_types)
{
    using pair_type = hud::pair<hud_test::explicitly_copy_constructible, hud_test::explicitly_copy_constructible>;
    using tuple_type = hud::tuple<hud_test::explicitly_copy_constructible2, hud_test::explicitly_copy_constructible2>;

    hud_assert_true((hud::is_explicitly_constructible_v<tuple_type, const pair_type &>));

    const auto test = []()
    {
        const pair_type pair {1, 2};
        const tuple_type tuple {pair};
        return std::tuple {
            hud::get<0>(tuple).id(),
            hud::get<0>(tuple).copy_constructor_count() == 1u,
            hud::get<1>(tuple).id(),
            hud::get<1>(tuple).copy_constructor_count() == 1u,
        };
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
    }
}

GTEST_TEST(tuple, constructor_by_copy_pair_of_implictly_copyable_same_types)
{
    using pair_type = hud::pair<hud_test::implicitly_copy_constructible, hud_test::implicitly_copy_constructible>;
    using tuple_type = hud::tuple<hud_test::implicitly_copy_constructible, hud_test::implicitly_copy_constructible>;

    hud_assert_true((hud::is_implicitly_constructible_v<tuple_type, const pair_type &>));

    const auto test = []()
    {
        const pair_type pair {1, 2};
        const tuple_type tuple {pair};
        return std::tuple {
            hud::get<0>(tuple).id(),
            hud::get<0>(tuple).copy_constructor_count() == 1u,
            hud::get<1>(tuple).id(),
            hud::get<1>(tuple).copy_constructor_count() == 1u,
        };
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
    }
}

GTEST_TEST(tuple, constructor_by_copy_pair_of_implictly_copyable_different_types)
{
    using pair_type = hud::pair<hud_test::implicitly_copy_constructible, hud_test::implicitly_copy_constructible>;
    using tuple_type = hud::tuple<hud_test::implicitly_copy_constructible2, hud_test::implicitly_copy_constructible2>;

    hud_assert_true((hud::is_implicitly_constructible_v<tuple_type, const pair_type &>));

    const auto test = []()
    {
        const pair_type pair {1, 2};
        const tuple_type tuple {pair};
        return std::tuple {
            hud::get<0>(tuple).id(),
            hud::get<0>(tuple).copy_constructor_count() == 1u,
            hud::get<1>(tuple).id(),
            hud::get<1>(tuple).copy_constructor_count() == 1u,
        };
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
    }
}

GTEST_TEST(tuple, constructor_by_copy_pair_of_mix_implictly_and_explitly_copyable_same_types)
{
    using pair_type = hud::pair<hud_test::implicitly_copy_constructible, hud_test::explicitly_copy_constructible>;
    using tuple_type = hud::tuple<hud_test::implicitly_copy_constructible, hud_test::explicitly_copy_constructible>;

    hud_assert_true((hud::is_explicitly_constructible_v<tuple_type, const pair_type &>));

    const auto test = []()
    {
        const pair_type pair {1, 2};
        const tuple_type tuple {pair};
        return std::tuple {
            hud::get<0>(tuple).id(),
            hud::get<0>(tuple).copy_constructor_count() == 1u,
            hud::get<1>(tuple).id(),
            hud::get<1>(tuple).copy_constructor_count() == 1u,
        };
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
    }
}

GTEST_TEST(tuple, constructor_by_copy_pair_of_mix_implictly_and_explitly_copyable_different_types)
{
    using pair_type = hud::pair<hud_test::implicitly_copy_constructible, hud_test::explicitly_copy_constructible>;
    using tuple_type = hud::tuple<hud_test::implicitly_copy_constructible2, hud_test::explicitly_copy_constructible2>;

    hud_assert_true((hud::is_explicitly_constructible_v<tuple_type, const pair_type &>));

    const auto test = []()
    {
        const pair_type pair {1, 2};
        const tuple_type tuple {pair};
        return std::tuple {
            hud::get<0>(tuple).id(),
            hud::get<0>(tuple).copy_constructor_count() == 1u,
            hud::get<1>(tuple).id(),
            hud::get<1>(tuple).copy_constructor_count() == 1u,
        };
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
    }
}

GTEST_TEST(tuple, constructor_by_move_pair_of_explictly_copyable_same_types)
{
    using pair_type = hud::pair<hud_test::explicitly_copy_constructible, hud_test::explicitly_copy_constructible>;
    using tuple_type = hud::tuple<hud_test::explicitly_copy_constructible, hud_test::explicitly_copy_constructible>;

    hud_assert_true((hud::is_explicitly_constructible_v<tuple_type, pair_type &&>));

    const auto test = []()
    {
        pair_type pair {1, 2};
        const tuple_type tuple {hud::move(pair)};
        return std::tuple {
            hud::get<0>(tuple).id(),
            hud::get<0>(tuple).copy_constructor_count() == 1u,
            hud::get<1>(tuple).id(),
            hud::get<1>(tuple).copy_constructor_count() == 1u,
        };
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
    }
}

GTEST_TEST(tuple, constructor_by_move_pair_of_explictly_copyable_different_types)
{
    using pair_type = hud::pair<hud_test::explicitly_copy_constructible, hud_test::explicitly_copy_constructible>;
    using tuple_type = hud::tuple<hud_test::explicitly_copy_constructible2, hud_test::explicitly_copy_constructible2>;

    hud_assert_true((hud::is_explicitly_constructible_v<tuple_type, pair_type &&>));

    const auto test = []()
    {
        pair_type pair {1, 2};
        const tuple_type tuple {hud::move(pair)};
        return std::tuple {
            hud::get<0>(tuple).id(),
            hud::get<0>(tuple).copy_constructor_count() == 1u,
            hud::get<1>(tuple).id(),
            hud::get<1>(tuple).copy_constructor_count() == 1u,
        };
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
    }
}

GTEST_TEST(tuple, constructor_by_move_pair_of_implictly_copyable_same_types)
{
    using pair_type = hud::pair<hud_test::implicitly_copy_constructible, hud_test::implicitly_copy_constructible>;
    using tuple_type = hud::tuple<hud_test::implicitly_copy_constructible, hud_test::implicitly_copy_constructible>;

    hud_assert_true((hud::is_implicitly_constructible_v<tuple_type, pair_type &&>));

    const auto test = []()
    {
        pair_type pair {1, 2};
        const tuple_type tuple {hud::move(pair)};
        return std::tuple {
            hud::get<0>(tuple).id(),
            hud::get<0>(tuple).copy_constructor_count() == 1u,
            hud::get<1>(tuple).id(),
            hud::get<1>(tuple).copy_constructor_count() == 1u,
        };
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
    }
}

GTEST_TEST(tuple, constructor_by_move_pair_of_implictly_copyable_different_types)
{
    using pair_type = hud::pair<hud_test::implicitly_copy_constructible, hud_test::implicitly_copy_constructible>;
    using tuple_type = hud::tuple<hud_test::implicitly_copy_constructible2, hud_test::implicitly_copy_constructible2>;

    hud_assert_true((hud::is_implicitly_constructible_v<tuple_type, pair_type &&>));

    const auto test = []()
    {
        pair_type pair {1, 2};
        const tuple_type tuple {hud::move(pair)};
        return std::tuple {
            hud::get<0>(tuple).id(),
            hud::get<0>(tuple).copy_constructor_count() == 1u,
            hud::get<1>(tuple).id(),
            hud::get<1>(tuple).copy_constructor_count() == 1u,
        };
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
    }
}

GTEST_TEST(tuple, constructor_by_move_pair_of_mix_implictly_and_explitly_copyable_same_types)
{
    using pair_type = hud::pair<hud_test::implicitly_copy_constructible, hud_test::explicitly_copy_constructible>;
    using tuple_type = hud::tuple<hud_test::implicitly_copy_constructible, hud_test::explicitly_copy_constructible>;

    hud_assert_true((hud::is_explicitly_constructible_v<tuple_type, pair_type &&>));

    const auto test = []()
    {
        pair_type pair {1, 2};
        const tuple_type tuple {hud::move(pair)};
        return std::tuple {
            hud::get<0>(tuple).id(),
            hud::get<0>(tuple).copy_constructor_count() == 1u,
            hud::get<1>(tuple).id(),
            hud::get<1>(tuple).copy_constructor_count() == 1u,
        };
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
    }
}

GTEST_TEST(tuple, constructor_by_move_pair_of_mix_implictly_and_explitly_copyable_different_types)
{
    using pair_type = hud::pair<hud_test::implicitly_copy_constructible, hud_test::explicitly_copy_constructible>;
    using tuple_type = hud::tuple<hud_test::implicitly_copy_constructible2, hud_test::explicitly_copy_constructible2>;

    hud_assert_true((hud::is_explicitly_constructible_v<tuple_type, pair_type &&>));

    const auto test = []()
    {
        pair_type pair {1, 2};
        const tuple_type tuple {hud::move(pair)};
        return std::tuple {
            hud::get<0>(tuple).id(),
            hud::get<0>(tuple).copy_constructor_count() == 1u,
            hud::get<1>(tuple).id(),
            hud::get<1>(tuple).copy_constructor_count() == 1u,
        };
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
    }
}

GTEST_TEST(tuple, constructor_by_move_pair_of_explictly_moveable_same_types)
{
    using pair_type = hud::pair<hud_test::explicitly_move_constructible, hud_test::explicitly_move_constructible>;
    using tuple_type = hud::tuple<hud_test::explicitly_move_constructible, hud_test::explicitly_move_constructible>;

    // hud_assert_true((hud::is_explicitly_constructible_v<tuple_type, pair_type&&>));
    static_assert((hud::is_explicitly_move_constructible_v<hud_test::explicitly_move_constructible, hud_test::explicitly_move_constructible>));
    static_assert((hud::details::is_pair_explicitly_move_constructible_to_tuple_v<pair_type, tuple_type>));
    const auto test = []()
    {
        pair_type pair {1, 2};
        const tuple_type tuple {hud::move(pair)};
        return std::tuple {
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
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
    }

    // Constant
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

GTEST_TEST(tuple, constructor_by_move_pair_of_explictly_moveable_different_types)
{
    using pair_type = hud::pair<hud_test::explicitly_move_constructible, hud_test::explicitly_move_constructible>;
    using tuple_type = hud::tuple<hud_test::explicitly_move_constructible2, hud_test::explicitly_move_constructible2>;

    hud_assert_true((hud::is_explicitly_constructible_v<tuple_type, pair_type &&>));

    const auto test = []()
    {
        pair_type pair {1, 2};
        const tuple_type tuple {hud::move(pair)};
        return std::tuple {
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
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
    }

    // Constant
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

GTEST_TEST(tuple, constructor_by_move_pair_of_implictly_moveable_same_types)
{
    using pair_type = hud::pair<hud_test::implicitly_move_constructible, hud_test::implicitly_move_constructible>;
    using tuple_type = hud::tuple<hud_test::implicitly_move_constructible, hud_test::implicitly_move_constructible>;

    hud_assert_true((hud::is_implicitly_constructible_v<tuple_type, pair_type &&>));

    const auto test = []()
    {
        pair_type pair {1, 2};
        const tuple_type tuple {hud::move(pair)};
        return std::tuple {
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
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
    }

    // Constant
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

GTEST_TEST(tuple, constructor_by_move_pair_of_implictly_moveable_different_types)
{
    using pair_type = hud::pair<hud_test::implicitly_move_constructible, hud_test::implicitly_move_constructible>;
    using tuple_type = hud::tuple<hud_test::implicitly_move_constructible2, hud_test::implicitly_move_constructible2>;

    hud_assert_true((hud::is_implicitly_constructible_v<tuple_type, pair_type &&>));

    const auto test = []()
    {
        pair_type pair {1, 2};
        const tuple_type tuple {hud::move(pair)};
        return std::tuple {
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
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
    }

    // Constant
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

GTEST_TEST(tuple, constructor_by_move_pair_of_mix_implictly_and_explitly_moveable_same_types)
{
    using pair_type = hud::pair<hud_test::implicitly_move_constructible, hud_test::explicitly_move_constructible>;
    using tuple_type = hud::tuple<hud_test::implicitly_move_constructible, hud_test::explicitly_move_constructible>;

    hud_assert_true((hud::is_explicitly_constructible_v<tuple_type, pair_type &&>));

    const auto test = []()
    {
        pair_type pair {1, 2};
        const tuple_type tuple {hud::move(pair)};
        return std::tuple {
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
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
    }

    // Constant
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

GTEST_TEST(tuple, constructor_by_move_pair_of_mix_implictly_and_explitly_moveable_different_types)
{
    using pair_type = hud::pair<hud_test::implicitly_move_constructible, hud_test::explicitly_move_constructible>;
    using tuple_type = hud::tuple<hud_test::implicitly_move_constructible2, hud_test::explicitly_move_constructible2>;

    hud_assert_true((hud::is_explicitly_constructible_v<tuple_type, pair_type &&>));

    const auto test = []()
    {
        pair_type pair {1, 2};
        const tuple_type tuple {hud::move(pair)};
        return std::tuple {
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
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
    }

    // Constant
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

GTEST_TEST(tuple, copy_constructor_copyable_same_types)
{
    using tuple_type = hud::tuple<hud_test::explicitly_copy_constructible, hud_test::implicitly_copy_constructible>;

    hud_assert_true((hud::is_explicitly_constructible_v<tuple_type, const hud_test::explicitly_copy_constructible &, const hud_test::implicitly_copy_constructible &>));

    const auto test = []()
    {
        const tuple_type to_copy {1, 2};
        const tuple_type copy {to_copy};
        return std::tuple {
            hud::get<0>(copy).id() == 1,
            hud::get<0>(copy).copy_constructor_count() == 1u,
            hud::get<1>(copy).id() == 2,
            hud::get<1>(copy).copy_constructor_count() == 1u,
        };
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
    }
}

GTEST_TEST(tuple, copy_constructor_copyable_different_types)
{
    using tuple_type = hud::tuple<hud_test::explicitly_copy_constructible2, hud_test::implicitly_copy_constructible2>;
    using other_tuple_type = hud::tuple<hud_test::explicitly_copy_constructible, hud_test::implicitly_copy_constructible>;

    hud_assert_true((hud::is_explicitly_constructible_v<tuple_type, const hud_test::explicitly_copy_constructible &, const hud_test::implicitly_copy_constructible &>));

    const auto test = []()
    {
        const other_tuple_type to_copy {1, 2};
        const tuple_type copy {to_copy};
        return std::tuple {
            hud::get<0>(copy).id() == 1,
            hud::get<0>(copy).copy_constructor_count() == 1u,
            hud::get<1>(copy).id() == 2,
            hud::get<1>(copy).copy_constructor_count() == 1u,
        };
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
    }
}

GTEST_TEST(tuple, copy_constructor_moveable_same_types)
{
    using tuple_type = hud::tuple<hud_test::explicitly_move_constructible, hud_test::implicitly_move_constructible>;

    hud_assert_true((hud::is_explicitly_constructible_v<tuple_type, const hud_test::explicitly_move_constructible &, const hud_test::explicitly_move_constructible &>));

    const auto test = []()
    {
        const tuple_type to_copy {1, 2};
        const tuple_type copy {to_copy};
        return std::tuple {
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
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
    }

    // Constant
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

GTEST_TEST(tuple, copy_constructor_moveable_different_types)
{
    using tuple_type = hud::tuple<hud_test::explicitly_move_constructible2, hud_test::implicitly_move_constructible2>;
    using other_tuple_type = hud::tuple<hud_test::explicitly_move_constructible, hud_test::implicitly_move_constructible>;

    hud_assert_true((hud::is_explicitly_constructible_v<tuple_type, const hud_test::explicitly_move_constructible &, const hud_test::implicitly_move_constructible &>));

    const auto test = []()
    {
        const other_tuple_type to_copy {1, 2};
        const tuple_type copy {to_copy};
        return std::tuple {
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
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
    }

    // Constant
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

GTEST_TEST(tuple, move_constructor_copyable_same_types)
{
    using tuple_type = hud::tuple<hud_test::explicitly_copy_constructible, hud_test::implicitly_copy_constructible>;

    hud_assert_true((hud::is_explicitly_constructible_v<tuple_type, hud_test::explicitly_copy_constructible &&, hud_test::implicitly_copy_constructible &&>));

    const auto test = []()
    {
        tuple_type to_copy {1, 2};
        const tuple_type copy {hud::move(to_copy)};
        return std::tuple {
            hud::get<0>(copy).id() == 1,
            hud::get<0>(copy).copy_constructor_count() == 1u,
            hud::get<1>(copy).id() == 2,
            hud::get<1>(copy).copy_constructor_count() == 1u,
        };
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
    }
}

GTEST_TEST(tuple, move_constructor_copyable_different_types)
{
    using tuple_type = hud::tuple<hud_test::explicitly_copy_constructible2, hud_test::implicitly_copy_constructible2>;
    using other_tuple_type = hud::tuple<hud_test::explicitly_copy_constructible, hud_test::implicitly_copy_constructible>;

    hud_assert_true((hud::is_explicitly_constructible_v<tuple_type, hud_test::explicitly_copy_constructible &&, hud_test::implicitly_copy_constructible &&>));

    const auto test = []()
    {
        other_tuple_type to_copy {1, 2};
        const tuple_type copy {hud::move(to_copy)};
        return std::tuple {
            hud::get<0>(copy).id() == 1,
            hud::get<0>(copy).copy_constructor_count() == 1u,
            hud::get<1>(copy).id() == 2,
            hud::get<1>(copy).copy_constructor_count() == 1u,
        };
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
    }
}

GTEST_TEST(tuple, move_constructor_moveable_same_types)
{
    using tuple_type = hud::tuple<hud_test::explicitly_move_constructible, hud_test::implicitly_move_constructible>;

    hud_assert_true((hud::is_explicitly_constructible_v<tuple_type, hud_test::explicitly_move_constructible &&, hud_test::implicitly_move_constructible &&>));

    const auto test = []()
    {
        tuple_type to_copy {1, 2};
        const tuple_type copy {hud::move(to_copy)};
        return std::tuple {
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
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
    }

    // Constant
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

GTEST_TEST(tuple, move_constructor_moveable_different_types)
{
    using tuple_type = hud::tuple<hud_test::explicitly_move_constructible2, hud_test::implicitly_move_constructible2>;
    using other_tuple_type = hud::tuple<hud_test::explicitly_move_constructible, hud_test::implicitly_move_constructible>;

    hud_assert_true((hud::is_explicitly_constructible_v<tuple_type, hud_test::explicitly_move_constructible &&, hud_test::implicitly_move_constructible &&>));

    const auto test = []()
    {
        other_tuple_type to_copy {1, 2};
        const tuple_type copy {hud::move(to_copy)};
        return std::tuple {
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
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
    }

    // Constant
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
