#include <core/containers/pair.h>
#include <core/traits/is_explicitly_default_constructible.h>
#include <core/traits/is_implicitly_default_constructible.h>
#include <core/traits/is_explicitly_copy_constructible.h>
#include <core/traits/is_implicitly_copy_constructible.h>
#include <core/traits/is_explicitly_move_constructible.h>
#include <core/traits/is_implicitly_move_constructible.h>
#include <core/traits/is_explicitly_constructible.h>
#include <core/traits/is_implicitly_constructible.h>
#include <core/traits/is_trivially_default_constructible.h>
#include <utility>

struct implicit_type
{
    implicit_type() noexcept = default;

    implicit_type(wchar value) noexcept
        : mInner(value)
    {
    }

    implicit_type(const implicit_type &other) noexcept
        : mInner(other.mInner) {};
    implicit_type(implicit_type &&other) noexcept
        : mInner(other.mInner + 1) {};
    wchar mInner = L'\0';
};

static_assert(hud::is_implicitly_default_constructible_v<implicit_type>);
static_assert(!hud::is_explicitly_default_constructible_v<implicit_type>);
static_assert(hud::is_implicitly_constructible_v<implicit_type, wchar>);
static_assert(!hud::is_explicitly_constructible_v<implicit_type, wchar>);
static_assert(hud::is_implicitly_constructible_v<implicit_type, const implicit_type &>);
static_assert(!hud::is_explicitly_constructible_v<implicit_type, const implicit_type &>);
static_assert(hud::is_implicitly_constructible_v<implicit_type, implicit_type &&>);
static_assert(!hud::is_explicitly_constructible_v<implicit_type, implicit_type &&>);
static_assert(hud::is_convertible_v<const implicit_type &, implicit_type>);

struct explicit_type
{
    explicit explicit_type() noexcept = default;

    explicit explicit_type(i32 value) noexcept
        : mInner(value)
    {
    }

    explicit explicit_type(const explicit_type &other) noexcept
        : mInner(other.mInner) {};

    explicit explicit_type(explicit_type &&other) noexcept
        : mInner(other.mInner + 1) {};

    operator implicit_type() const noexcept
    {
        return implicit_type(static_cast<wchar>(mInner));
    }

    i32 mInner = 0;
};

static_assert(!hud::is_implicitly_default_constructible_v<explicit_type>);
static_assert(hud::is_explicitly_default_constructible_v<explicit_type>);
static_assert(!hud::is_implicitly_constructible_v<explicit_type, i32>);
static_assert(hud::is_explicitly_constructible_v<explicit_type, i32>);

static_assert(!hud::is_implicitly_constructible_v<explicit_type, const explicit_type &>);
static_assert(hud::is_explicitly_constructible_v<explicit_type, const explicit_type &>);
static_assert(!hud::is_implicitly_constructible_v<explicit_type, explicit_type &&>);
static_assert(hud::is_explicitly_constructible_v<explicit_type, explicit_type &&>);
static_assert(!hud::is_convertible_v<const explicit_type &, explicit_type>);

GTEST_TEST(pair, default_constructor_is_explicit_if_T1_or_T2_is_not_implicitly_default_constructible)
{
    static_assert(hud::is_implicitly_default_constructible_v<implicit_type>);
    static_assert(hud::is_implicitly_default_constructible_v<hud::pair<i32, i32>>);
    static_assert(hud::is_implicitly_default_constructible_v<hud::pair<i32, f32>>);
    static_assert(hud::is_implicitly_default_constructible_v<hud::pair<i32, implicit_type>>);
    static_assert(hud::is_implicitly_default_constructible_v<hud::pair<implicit_type, implicit_type>>);
    static_assert(hud::is_explicitly_default_constructible_v<hud::pair<i32, explicit_type>>);
    static_assert(hud::is_explicitly_default_constructible_v<hud::pair<i32, explicit_type>>);
    static_assert(hud::is_explicitly_default_constructible_v<hud::pair<explicit_type, explicit_type>>);
}

GTEST_TEST(pair, param_copy_constructor_is_explicit_if_T1_or_T2_is_not_implicitly_convertible)
{

    static_assert(hud::is_convertible_v<const explicit_type &, implicit_type>);
    static_assert(hud::is_convertible_v<const implicit_type &, implicit_type>);
    static_assert(hud::is_implicitly_constructible_v<hud::pair<implicit_type, implicit_type>, const implicit_type &, const implicit_type &>);
    static_assert(hud::is_implicitly_constructible_v<hud::pair<implicit_type, implicit_type>, const implicit_type &, wchar>);
    static_assert(hud::is_explicitly_constructible_v<hud::pair<explicit_type, implicit_type>, const explicit_type &, const implicit_type &>);
    static_assert(hud::is_explicitly_constructible_v<hud::pair<explicit_type, implicit_type>, i32, const implicit_type &>);
}

GTEST_TEST(pair, param_move_constructor_is_explicit_if_T1_or_T2_is_not_implicitly_converible)
{

    static_assert(hud::is_convertible_v<const explicit_type &, implicit_type>);
    static_assert(hud::is_convertible_v<const implicit_type &, implicit_type>);
    static_assert(hud::is_implicitly_constructible_v<hud::pair<implicit_type, implicit_type>, implicit_type &&, implicit_type &&>);
    static_assert(hud::is_implicitly_constructible_v<hud::pair<implicit_type, implicit_type>, implicit_type &&, wchar>);
    static_assert(hud::is_explicitly_constructible_v<hud::pair<explicit_type, implicit_type>, explicit_type &&, implicit_type &&>);
    static_assert(hud::is_explicitly_constructible_v<hud::pair<explicit_type, implicit_type>, i32, implicit_type &&>);
}

GTEST_TEST(pair, copy_constructor_is_explicit_if_T1_or_T2_is_not_implicitly_convertible)
{

    static_assert(hud::is_convertible_v<const explicit_type &, implicit_type>);
    static_assert(hud::is_convertible_v<const implicit_type &, implicit_type>);
    static_assert(hud::is_copy_constructible_v<explicit_type, explicit_type>);
    static_assert(hud::is_copy_constructible_v<implicit_type, implicit_type>);

    // Same
    static_assert(hud::is_implicitly_copy_constructible_v<hud::pair<implicit_type, implicit_type>, hud::pair<implicit_type, implicit_type>>);
    static_assert(hud::is_explicitly_copy_constructible_v<hud::pair<explicit_type, implicit_type>, hud::pair<explicit_type, implicit_type>>);

    // Not same type_t
    static_assert(hud::is_implicitly_copy_constructible_v<hud::pair<implicit_type, implicit_type>, hud::pair<implicit_type, wchar>>);
    static_assert(hud::is_explicitly_copy_constructible_v<hud::pair<explicit_type, implicit_type>, hud::pair<i32, implicit_type>>);
    static_assert(hud::is_implicitly_copy_constructible_v<hud::pair<implicit_type, implicit_type>, hud::pair<i32, wchar>>);
}

GTEST_TEST(pair, move_constructor_is_explicit_if_T1_or_T2_is_not_implicitly_convertible)
{

    static_assert(hud::is_convertible_v<explicit_type, implicit_type>);
    static_assert(hud::is_convertible_v<implicit_type, implicit_type>);
    static_assert(hud::is_move_constructible_v<explicit_type, explicit_type>);
    static_assert(hud::is_move_constructible_v<implicit_type, implicit_type>);

    // Same
    static_assert(hud::is_implicitly_move_constructible_v<hud::pair<implicit_type, implicit_type>, hud::pair<implicit_type, implicit_type>>);
    static_assert(hud::is_explicitly_move_constructible_v<hud::pair<explicit_type, implicit_type>, hud::pair<explicit_type, implicit_type>>);
    static_assert(hud::is_explicitly_move_constructible_v<hud::pair<explicit_type, implicit_type>, hud::pair<i32, implicit_type>>);

    // Not same type_t
    static_assert(hud::is_implicitly_move_constructible_v<hud::pair<implicit_type, implicit_type>, hud::pair<implicit_type, wchar>>);
    static_assert(hud::is_explicitly_move_constructible_v<hud::pair<explicit_type, implicit_type>, hud::pair<i32, implicit_type>>);
    static_assert(hud::is_explicitly_move_constructible_v<hud::pair<explicit_type, implicit_type>, hud::pair<i32, wchar>>);
}

GTEST_TEST(pair, default_constructor_trivial)
{

    using Type1 = i32;
    using Type2 = f32;
    static_assert(hud::is_trivially_default_constructible_v<Type1>);
    static_assert(hud::is_trivially_default_constructible_v<Type2>);
    using TypePair = hud::pair<Type1, Type2>;

    const auto test = []()
    {
        TypePair *pair = hud::memory::allocate_array<TypePair>(1);
        hud::memory::construct_object_at(pair);
        const auto result = std::tuple {pair->first, pair->second};
        hud::memory::free_array(pair, 1);
        return result;
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_eq(std::get<0>(result), 0);
        hud_assert_eq(std::get<1>(result), 0.0f);
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_eq(std::get<0>(result), 0);
        hud_assert_eq(std::get<1>(result), 0.0f);
    }
}

GTEST_TEST(pair, default_constructor_non_trivial)
{

    using Type1 = hud_test::non_bitwise_type;
    using Type2 = hud_test::non_bitwise_type;
    static_assert(!hud::is_trivially_default_constructible_v<Type1>);
    static_assert(!hud::is_trivially_default_constructible_v<Type2>);
    using TypePair = hud::pair<Type1, Type2>;

    const auto test = []()
    {
        TypePair *pair = hud::memory::allocate_array<TypePair>(1);
        hud::memory::construct_object_at(pair);
        const auto result = std::tuple {
            pair->first.move_assign_count(),
            pair->first.copy_assign_count(),
            pair->first.constructor_count(),
            pair->first.move_constructor_count(),
            pair->first.copy_constructor_count(),
            pair->first.id(),
            pair->second.move_assign_count(),
            pair->second.copy_assign_count(),
            pair->second.constructor_count(),
            pair->second.move_constructor_count(),
            pair->second.copy_constructor_count(),
            pair->second.id(),
        };
        hud::memory::free_array(pair, 1);
        return result;
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_eq(std::get<0>(result), 0u);
        hud_assert_eq(std::get<1>(result), 0u);
        hud_assert_eq(std::get<2>(result), 0u);
        hud_assert_eq(std::get<3>(result), 0u);
        hud_assert_eq(std::get<4>(result), 0u);
        hud_assert_eq(std::get<5>(result), 0);

        hud_assert_eq(std::get<6>(result), 0u);
        hud_assert_eq(std::get<7>(result), 0u);
        hud_assert_eq(std::get<8>(result), 0u);
        hud_assert_eq(std::get<9>(result), 0u);
        hud_assert_eq(std::get<10>(result), 0u);
        hud_assert_eq(std::get<11>(result), 0);
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_eq(std::get<0>(result), 0u);
        hud_assert_eq(std::get<1>(result), 0u);
        hud_assert_eq(std::get<2>(result), 0u);
        hud_assert_eq(std::get<3>(result), 0u);
        hud_assert_eq(std::get<4>(result), 0u);
        hud_assert_eq(std::get<5>(result), 0);

        hud_assert_eq(std::get<6>(result), 0u);
        hud_assert_eq(std::get<7>(result), 0u);
        hud_assert_eq(std::get<8>(result), 0u);
        hud_assert_eq(std::get<9>(result), 0u);
        hud_assert_eq(std::get<10>(result), 0u);
        hud_assert_eq(std::get<11>(result), 0);
    }
}

GTEST_TEST(pair, copy_constructor_trivial_type_same_type)
{

    using Type1 = i32;
    using Type2 = f32;
    static_assert(hud::is_trivially_copy_constructible_v<Type1>);
    static_assert(hud::is_trivially_copy_constructible_v<Type2>);
    using TypePair = hud::pair<Type1, Type2>;

    const auto test = [](const Type1 &t1, const Type2 &t2)
    {
        TypePair *pair = hud::memory::allocate_array<TypePair>(1);
        const TypePair other {t1, t2};
        hud::memory::construct_object_at(pair, other);
        const auto result = std::tuple {
            pair->first,
            pair->second
        };
        hud::memory::free_array(pair, 1);
        return result;
    };

    // Non constant
    {
        const auto result = test(123, 456.0f);
        hud_assert_eq(std::get<0>(result), 123);
        hud_assert_eq(std::get<1>(result), 456.0f);
    }

    // Constant
    {
        constexpr auto result = test(123, 456.0f);
        hud_assert_eq(std::get<0>(result), 123);
        hud_assert_eq(std::get<1>(result), 456.0f);
    }
}

GTEST_TEST(pair, copy_constructor_non_trivial_type_same_type)
{

    using Type1 = hud_test::non_bitwise_type;
    using Type2 = hud_test::non_bitwise_type;
    static_assert(!hud::is_trivially_copy_constructible_v<Type1>);
    static_assert(!hud::is_trivially_copy_constructible_v<Type2>);
    using TypePair = hud::pair<Type1, Type2>;

    const auto test = [](const i32 &id1, const i32 &id2)
    {
        TypePair *pair = hud::memory::allocate_array<TypePair>(1);
        const TypePair other {
            {id1, nullptr},
            {id2, nullptr}
        };
        hud::memory::construct_object_at(pair, other);
        const auto result = std::tuple {
            pair->first.move_assign_count(),
            pair->first.copy_assign_count(),
            pair->first.constructor_count(),
            pair->first.move_constructor_count(),
            pair->first.copy_constructor_count(),
            pair->first.id(),
            pair->second.move_assign_count(),
            pair->second.copy_assign_count(),
            pair->second.constructor_count(),
            pair->second.move_constructor_count(),
            pair->second.copy_constructor_count(),
            pair->second.id(),
        };
        hud::memory::free_array(pair, 1);
        return result;
    };

    // Non constant
    {
        const auto result = test(123, 456);
        hud_assert_eq(std::get<0>(result), 0u);
        hud_assert_eq(std::get<1>(result), 0u);
        hud_assert_eq(std::get<2>(result), 1u);
        hud_assert_eq(std::get<3>(result), 1u);
        hud_assert_eq(std::get<4>(result), 1u);
        hud_assert_eq(std::get<5>(result), 123);

        hud_assert_eq(std::get<6>(result), 0u);
        hud_assert_eq(std::get<7>(result), 0u);
        hud_assert_eq(std::get<8>(result), 1u);
        hud_assert_eq(std::get<9>(result), 1u);
        hud_assert_eq(std::get<10>(result), 1u);
        hud_assert_eq(std::get<11>(result), 456);
    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        hud_assert_eq(std::get<0>(result), 0u);
        hud_assert_eq(std::get<1>(result), 0u);
        hud_assert_eq(std::get<2>(result), 1u);
        hud_assert_eq(std::get<3>(result), 1u);
        hud_assert_eq(std::get<4>(result), 1u);
        hud_assert_eq(std::get<5>(result), 123);

        hud_assert_eq(std::get<6>(result), 0u);
        hud_assert_eq(std::get<7>(result), 0u);
        hud_assert_eq(std::get<8>(result), 1u);
        hud_assert_eq(std::get<9>(result), 1u);
        hud_assert_eq(std::get<10>(result), 1u);
        hud_assert_eq(std::get<11>(result), 456);
    }
}

GTEST_TEST(pair, copy_constructor_non_trivial_copy_constructible_type_same_type)
{

    using Type1 = hud_test::non_bitwise_copy_constructible_type;
    using Type2 = hud_test::non_bitwise_copy_constructible_type;
    static_assert(!hud::is_trivially_copy_constructible_v<Type1>);
    static_assert(!hud::is_trivially_copy_constructible_v<Type2>);
    using TypePair = hud::pair<Type1, Type2>;

    const auto test = [](const i32 &id1, const i32 &id2)
    {
        TypePair *pair = hud::memory::allocate_array<TypePair>(1);
        const TypePair other {id1, id2};
        hud::memory::construct_object_at(pair, other);
        const auto result = std::tuple {
            pair->first.copy_constructor_count(),
            pair->first.id(),
            pair->second.copy_constructor_count(),
            pair->second.id(),
        };
        hud::memory::free_array(pair, 1);
        return result;
    };

    // Non constant
    {
        const auto result = test(123, 456);
        hud_assert_eq(std::get<0>(result), 1u);
        hud_assert_eq(std::get<1>(result), 123);
        hud_assert_eq(std::get<2>(result), 1u);
        hud_assert_eq(std::get<3>(result), 456);
    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        hud_assert_eq(std::get<0>(result), 1u);
        hud_assert_eq(std::get<1>(result), 123);
        hud_assert_eq(std::get<2>(result), 1u);
        hud_assert_eq(std::get<3>(result), 456);
    }
}

GTEST_TEST(pair, copy_constructor_trivial_type_different_type)
{

    using Type1 = i64;
    using Type2 = i32;
    using OtherType1 = i16;
    using OtherType2 = i8;
    static_assert(hud::is_trivially_copy_constructible_v<Type1, OtherType1>);
    static_assert(hud::is_trivially_copy_constructible_v<Type2, OtherType2>);
    using TypePair = hud::pair<Type1, Type2>;
    using OtherTypePair = hud::pair<OtherType1, OtherType2>;

    const auto test = [](const OtherType1 &t1, const OtherType2 &t2)
    {
        TypePair *pair = hud::memory::allocate_array<TypePair>(1);
        const OtherTypePair other {t1, t2};
        hud::memory::construct_object_at(pair, other);
        const auto result = std::tuple {
            pair->first,
            pair->second
        };
        hud::memory::free_array(pair, 1);
        return result;
    };

    // Non constant
    {
        const auto result = test(123, 32);
        hud_assert_eq(std::get<0>(result), 123);
        hud_assert_eq(std::get<1>(result), 32);
    }

    // Constant
    {
        constexpr auto result = test(123, 32);
        hud_assert_eq(std::get<0>(result), 123);
        hud_assert_eq(std::get<1>(result), 32);
    }
}

GTEST_TEST(pair, copy_constructor_non_trivial_type_different_type)
{

    using Type1 = hud_test::non_bitwise_copy_constructible_type2;
    using Type2 = hud_test::non_bitwise_copy_constructible_type2;
    using OtherType1 = hud_test::non_bitwise_copy_constructible_type;
    using OtherType2 = hud_test::non_bitwise_copy_constructible_type;
    static_assert(hud::is_copy_constructible_v<Type1, OtherType1>);
    static_assert(!hud::is_trivially_copy_constructible_v<Type1, OtherType1>);
    static_assert(hud::is_copy_constructible_v<Type2, OtherType2>);
    static_assert(!hud::is_trivially_copy_constructible_v<Type2, OtherType2>);
    using TypePair = hud::pair<Type1, Type2>;
    using OtherTypePair = hud::pair<OtherType1, OtherType2>;

    const auto test = [](const i32 &id1, const i32 &id2)
    {
        TypePair *pair = hud::memory::allocate_array<TypePair>(1);
        const OtherTypePair other {id1, id2};
        hud::memory::construct_object_at(pair, other);
        const auto result = std::tuple {
            pair->first.copy_constructor_count(),
            pair->first.id(),
            pair->second.copy_constructor_count(),
            pair->second.id(),
        };
        hud::memory::free_array(pair, 1);
        return result;
    };

    // Non constant
    {
        const auto result = test(123, 456);
        hud_assert_eq(std::get<0>(result), 1u);
        hud_assert_eq(std::get<1>(result), 123);
        hud_assert_eq(std::get<2>(result), 1u);
        hud_assert_eq(std::get<3>(result), 456);
    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        hud_assert_eq(std::get<0>(result), 1u);
        hud_assert_eq(std::get<1>(result), 123);
        hud_assert_eq(std::get<2>(result), 1u);
        hud_assert_eq(std::get<3>(result), 456);
    }
}

GTEST_TEST(pair, copy_constructor_non_trivial_copy_constructible_type_different_type)
{

    using Type1 = hud_test::non_bitwise_copy_constructible_type2;
    using Type2 = hud_test::non_bitwise_copy_constructible_type2;
    using OtherType1 = hud_test::non_bitwise_copy_constructible_type;
    using OtherType2 = hud_test::non_bitwise_copy_constructible_type;
    static_assert(hud::is_copy_constructible_v<Type1, OtherType1>);
    static_assert(!hud::is_trivially_copy_constructible_v<Type1, OtherType1>);
    static_assert(hud::is_copy_constructible_v<Type2, OtherType2>);
    static_assert(!hud::is_trivially_copy_constructible_v<Type2, OtherType2>);
    using TypePair = hud::pair<Type1, Type2>;
    using OtherTypePair = hud::pair<OtherType1, OtherType2>;

    const auto test = [](const i32 &id1, const i32 &id2)
    {
        TypePair *pair = hud::memory::allocate_array<TypePair>(1);
        const OtherTypePair other {id1, id2};
        hud::memory::construct_object_at(pair, other);
        const auto result = std::tuple {
            pair->first.copy_constructor_count(),
            pair->first.id(),
            pair->second.copy_constructor_count(),
            pair->second.id(),
        };
        hud::memory::free_array(pair, 1);
        return result;
    };

    // Non constant
    {
        const auto result = test(123, 456);
        hud_assert_eq(std::get<0>(result), 1u);
        hud_assert_eq(std::get<1>(result), 123);
        hud_assert_eq(std::get<2>(result), 1u);
        hud_assert_eq(std::get<3>(result), 456);
    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        hud_assert_eq(std::get<0>(result), 1u);
        hud_assert_eq(std::get<1>(result), 123);
        hud_assert_eq(std::get<2>(result), 1u);
        hud_assert_eq(std::get<3>(result), 456);
    }
}

GTEST_TEST(pair, move_constructor_trivial_type_same_type)
{

    using Type1 = i32;
    using Type2 = f32;
    static_assert(hud::is_trivially_copy_constructible_v<Type1>);
    static_assert(hud::is_trivially_copy_constructible_v<Type2>);
    using TypePair = hud::pair<Type1, Type2>;

    const auto test = [](const Type1 &t1, const Type2 &t2)
    {
        TypePair *pair = hud::memory::allocate_array<TypePair>(1);
        TypePair other {t1, t2};
        hud::memory::construct_object_at(pair, hud::move(other));
        const auto result = std::tuple {
            pair->first,
            pair->second
        };
        hud::memory::free_array(pair, 1);
        return result;
    };

    // Non constant
    {
        const auto result = test(123, 456.0f);
        hud_assert_eq(std::get<0>(result), 123);
        hud_assert_eq(std::get<1>(result), 456.0f);
    }

    // Constant
    {
        constexpr auto result = test(123, 456.0f);
        hud_assert_eq(std::get<0>(result), 123);
        hud_assert_eq(std::get<1>(result), 456.0f);
    }
}

GTEST_TEST(pair, move_constructor_non_trivial_type_same_type)
{

    using Type1 = hud_test::non_bitwise_type;
    using Type2 = hud_test::non_bitwise_type;
    static_assert(!hud::is_trivially_copy_constructible_v<Type1>);
    static_assert(!hud::is_trivially_copy_constructible_v<Type2>);
    using TypePair = hud::pair<Type1, Type2>;

    const auto test = [](const i32 &id1, const i32 &id2)
    {
        TypePair *pair = hud::memory::allocate_array<TypePair>(1);
        TypePair other {
            {id1, nullptr},
            {id2, nullptr}
        };
        hud::memory::construct_object_at(pair, hud::move(other));
        const auto result = std::tuple {
            pair->first.move_assign_count(),
            pair->first.copy_assign_count(),
            pair->first.constructor_count(),
            pair->first.move_constructor_count(),
            pair->first.copy_constructor_count(),
            pair->first.id(),
            pair->second.move_assign_count(),
            pair->second.copy_assign_count(),
            pair->second.constructor_count(),
            pair->second.move_constructor_count(),
            pair->second.copy_constructor_count(),
            pair->second.id(),
        };
        hud::memory::free_array(pair, 1);
        return result;
    };

    // Non constant
    {
        const auto result = test(123, 456);
        hud_assert_eq(std::get<0>(result), 0u);
        hud_assert_eq(std::get<1>(result), 0u);
        hud_assert_eq(std::get<2>(result), 1u);
        hud_assert_eq(std::get<3>(result), 2u);
        hud_assert_eq(std::get<4>(result), 0u);
        hud_assert_eq(std::get<5>(result), 123);

        hud_assert_eq(std::get<6>(result), 0u);
        hud_assert_eq(std::get<7>(result), 0u);
        hud_assert_eq(std::get<8>(result), 1u);
        hud_assert_eq(std::get<9>(result), 2u);
        hud_assert_eq(std::get<10>(result), 0u);
        hud_assert_eq(std::get<11>(result), 456);
    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        hud_assert_eq(std::get<0>(result), 0u);
        hud_assert_eq(std::get<1>(result), 0u);
        hud_assert_eq(std::get<2>(result), 1u);
        hud_assert_eq(std::get<3>(result), 2u);
        hud_assert_eq(std::get<4>(result), 0u);
        hud_assert_eq(std::get<5>(result), 123);

        hud_assert_eq(std::get<6>(result), 0u);
        hud_assert_eq(std::get<7>(result), 0u);
        hud_assert_eq(std::get<8>(result), 1u);
        hud_assert_eq(std::get<9>(result), 2u);
        hud_assert_eq(std::get<10>(result), 0u);
        hud_assert_eq(std::get<11>(result), 456);
    }
}

GTEST_TEST(pair, move_constructor_non_trivial_copy_constructible_type_same_type)
{

    using Type1 = hud_test::non_bitwise_copy_constructible_type;
    using Type2 = hud_test::non_bitwise_copy_constructible_type;
    static_assert(!hud::is_trivially_copy_constructible_v<Type1>);
    static_assert(!hud::is_trivially_copy_constructible_v<Type2>);
    using TypePair = hud::pair<Type1, Type2>;

    const auto test = [](const i32 &id1, const i32 &id2)
    {
        TypePair *pair = hud::memory::allocate_array<TypePair>(1);
        TypePair other {id1, id2};
        hud::memory::construct_object_at(pair, hud::move(other));
        const auto result = std::tuple {
            pair->first.copy_constructor_count(),
            pair->first.id(),
            pair->second.copy_constructor_count(),
            pair->second.id(),
        };
        hud::memory::free_array(pair, 1);
        return result;
    };

    // Non constant
    {
        const auto result = test(123, 456);
        hud_assert_eq(std::get<0>(result), 1u);
        hud_assert_eq(std::get<1>(result), 123);
        hud_assert_eq(std::get<2>(result), 1u);
        hud_assert_eq(std::get<3>(result), 456);
    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        hud_assert_eq(std::get<0>(result), 1u);
        hud_assert_eq(std::get<1>(result), 123);
        hud_assert_eq(std::get<2>(result), 1u);
        hud_assert_eq(std::get<3>(result), 456);
    }
}

GTEST_TEST(pair, move_constructor_non_trivial_move_constructible_type_same_type)
{

    using Type1 = hud_test::non_bitwise_move_constructible_type;
    using Type2 = hud_test::non_bitwise_move_constructible_type;
    static_assert(!hud::is_trivially_move_constructible_v<Type1>);
    static_assert(!hud::is_trivially_move_constructible_v<Type2>);
    using TypePair = hud::pair<Type1, Type2>;

    const auto test = [](const i32 &id1, const i32 &id2)
    {
        TypePair *pair = hud::memory::allocate_array<TypePair>(1);
        TypePair other {id1, id2};
        hud::memory::construct_object_at(pair, hud::move(other));
        const auto result = std::tuple {
            pair->first.copy_constructor_count(),
            pair->first.move_constructor_count(),
            pair->first.id(),
            pair->second.copy_constructor_count(),
            pair->second.move_constructor_count(),
            pair->second.id(),
        };
        hud::memory::free_array(pair, 1);
        return result;
    };

    // Non constant
    {
        const auto result = test(123, 456);
        hud_assert_eq(std::get<0>(result), 0u);
        hud_assert_eq(std::get<1>(result), 1u);
        hud_assert_eq(std::get<2>(result), 123);
        hud_assert_eq(std::get<3>(result), 0u);
        hud_assert_eq(std::get<4>(result), 1u);
        hud_assert_eq(std::get<5>(result), 456);
    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        hud_assert_eq(std::get<0>(result), 0u);
        hud_assert_eq(std::get<1>(result), 1u);
        hud_assert_eq(std::get<2>(result), 123);
        hud_assert_eq(std::get<3>(result), 0u);
        hud_assert_eq(std::get<4>(result), 1u);
        hud_assert_eq(std::get<5>(result), 456);
    }
}

GTEST_TEST(pair, move_constructor_trivial_type_different_type)
{

    using Type1 = i64;
    using Type2 = i32;
    using OtherType1 = i16;
    using OtherType2 = i8;
    static_assert(hud::is_trivially_copy_constructible_v<Type1, OtherType1>);
    static_assert(hud::is_trivially_copy_constructible_v<Type2, OtherType2>);
    using TypePair = hud::pair<Type1, Type2>;
    using OtherTypePair = hud::pair<OtherType1, OtherType2>;

    const auto test = [](const OtherType1 &t1, const OtherType2 &t2)
    {
        TypePair *pair = hud::memory::allocate_array<TypePair>(1);
        OtherTypePair other {t1, t2};
        hud::memory::construct_object_at(pair, hud::move(other));
        const auto result = std::tuple {
            pair->first,
            pair->second
        };
        hud::memory::free_array(pair, 1);
        return result;
    };

    // Non constant
    {
        const auto result = test(123, 32);
        hud_assert_eq(std::get<0>(result), 123);
        hud_assert_eq(std::get<1>(result), 32);
    }

    // Constant
    {
        constexpr auto result = test(123, 32);
        hud_assert_eq(std::get<0>(result), 123);
        hud_assert_eq(std::get<1>(result), 32);
    }
}

GTEST_TEST(pair, move_constructor_non_trivial_copy_constructible_type_different_type)
{

    using Type1 = hud_test::non_bitwise_copy_constructible_type2;
    using Type2 = hud_test::non_bitwise_copy_constructible_type2;
    using OtherType1 = hud_test::non_bitwise_copy_constructible_type;
    using OtherType2 = hud_test::non_bitwise_copy_constructible_type;
    static_assert(hud::is_copy_constructible_v<Type1, OtherType1>);
    static_assert(!hud::is_trivially_copy_constructible_v<Type1, OtherType1>);
    static_assert(hud::is_copy_constructible_v<Type2, OtherType2>);
    static_assert(!hud::is_trivially_copy_constructible_v<Type2, OtherType2>);
    using TypePair = hud::pair<Type1, Type2>;
    using OtherTypePair = hud::pair<OtherType1, OtherType2>;

    const auto test = [](const i32 &id1, const i32 &id2)
    {
        TypePair *pair = hud::memory::allocate_array<TypePair>(1);
        OtherTypePair other {id1, id2};
        hud::memory::construct_object_at(pair, hud::move(other));
        const auto result = std::tuple {
            pair->first.copy_constructor_count(),
            pair->first.id(),
            pair->second.copy_constructor_count(),
            pair->second.id(),
        };
        hud::memory::free_array(pair, 1);
        return result;
    };

    // Non constant
    {
        const auto result = test(123, 456);
        hud_assert_eq(std::get<0>(result), 1u);
        hud_assert_eq(std::get<1>(result), 123);
        hud_assert_eq(std::get<2>(result), 1u);
        hud_assert_eq(std::get<3>(result), 456);
    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        hud_assert_eq(std::get<0>(result), 1u);
        hud_assert_eq(std::get<1>(result), 123);
        hud_assert_eq(std::get<2>(result), 1u);
        hud_assert_eq(std::get<3>(result), 456);
    }
}

GTEST_TEST(pair, move_constructor_non_trivial_move_constructible_type_different_type)
{

    using Type1 = hud_test::non_bitwise_move_constructible_type2;
    using Type2 = hud_test::non_bitwise_move_constructible_type2;
    using OtherType1 = hud_test::non_bitwise_move_constructible_type;
    using OtherType2 = hud_test::non_bitwise_move_constructible_type;
    static_assert(hud::is_move_constructible_v<Type1, OtherType1>);
    static_assert(!hud::is_trivially_move_constructible_v<Type1, OtherType1>);
    static_assert(hud::is_move_constructible_v<Type2, OtherType2>);
    static_assert(!hud::is_trivially_move_constructible_v<Type2, OtherType2>);
    using TypePair = hud::pair<Type1, Type2>;
    using OtherTypePair = hud::pair<OtherType1, OtherType2>;

    const auto test = [](const i32 &id1, const i32 &id2)
    {
        TypePair *pair = hud::memory::allocate_array<TypePair>(1);
        OtherTypePair other {id1, id2};
        hud::memory::construct_object_at(pair, hud::move(other));
        const auto result = std::tuple {
            pair->first.copy_constructor_count(),
            pair->first.move_constructor_count(),
            pair->first.id(),
            pair->second.copy_constructor_count(),
            pair->second.move_constructor_count(),
            pair->second.id(),
        };
        hud::memory::free_array(pair, 1);
        return result;
    };

    // Non constant
    {
        const auto result = test(123, 456);
        hud_assert_eq(std::get<0>(result), 0u);
        hud_assert_eq(std::get<1>(result), 1u);
        hud_assert_eq(std::get<2>(result), 123);
        hud_assert_eq(std::get<3>(result), 0u);
        hud_assert_eq(std::get<4>(result), 1u);
        hud_assert_eq(std::get<5>(result), 456);
    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        hud_assert_eq(std::get<0>(result), 0u);
        hud_assert_eq(std::get<1>(result), 1u);
        hud_assert_eq(std::get<2>(result), 123);
        hud_assert_eq(std::get<3>(result), 0u);
        hud_assert_eq(std::get<4>(result), 1u);
        hud_assert_eq(std::get<5>(result), 456);
    }
}

GTEST_TEST(pair, param_copy_constructor_trivial_type_same_type)
{

    using Type1 = i32;
    using Type2 = f32;
    static_assert(hud::is_trivially_copy_constructible_v<Type1>);
    static_assert(hud::is_trivially_copy_constructible_v<Type2>);
    using TypePair = hud::pair<Type1, Type2>;

    const auto test = [](const Type1 &t1, const Type2 &t2)
    {
        TypePair *pair = hud::memory::allocate_array<TypePair>(1);
        hud::memory::construct_object_at(pair, t1, t2);
        const auto result = std::tuple {
            pair->first,
            pair->second
        };
        hud::memory::free_array(pair, 1);
        return result;
    };

    // Non constant
    {
        const auto result = test(123, 456.0f);
        hud_assert_eq(std::get<0>(result), 123);
        hud_assert_eq(std::get<1>(result), 456.0f);
    }

    // Constant
    {
        constexpr auto result = test(123, 456.0f);
        hud_assert_eq(std::get<0>(result), 123);
        hud_assert_eq(std::get<1>(result), 456.0f);
    }
}

GTEST_TEST(pair, param_copy_constructor_trivial_type_different_type)
{

    using Type1 = i64;
    using Type2 = i32;
    using OtherType1 = i16;
    using OtherType2 = i8;
    static_assert(hud::is_trivially_copy_constructible_v<Type1, OtherType1>);
    static_assert(hud::is_trivially_copy_constructible_v<Type2, OtherType2>);
    using TypePair = hud::pair<Type1, Type2>;

    const auto test = [](const OtherType1 &t1, const OtherType2 &t2)
    {
        TypePair *pair = hud::memory::allocate_array<TypePair>(1);
        hud::memory::construct_object_at(pair, t1, t2);
        const auto result = std::tuple {
            pair->first,
            pair->second
        };
        hud::memory::free_array(pair, 1);
        return result;
    };

    // Non constant
    {
        const auto result = test(123, 32);
        hud_assert_eq(std::get<0>(result), 123);
        hud_assert_eq(std::get<1>(result), 32);
    }

    // Constant
    {
        constexpr auto result = test(123, 32);
        hud_assert_eq(std::get<0>(result), 123);
        hud_assert_eq(std::get<1>(result), 32);
    }
}

GTEST_TEST(pair, param_copy_constructor_non_trivial_type_same_type)
{

    using Type1 = hud_test::non_bitwise_type;
    using Type2 = hud_test::non_bitwise_type;
    static_assert(!hud::is_trivially_copy_constructible_v<Type1>);
    static_assert(!hud::is_trivially_copy_constructible_v<Type2>);
    using TypePair = hud::pair<Type1, Type2>;

    const auto test = [](const i32 &id1, const i32 &id2)
    {
        TypePair *pair = hud::memory::allocate_array<TypePair>(1);
        const Type1 t1 {id1, nullptr};
        const Type2 t2 {id2, nullptr};
        hud::memory::construct_object_at(pair, t1, t2);
        const auto result = std::tuple {
            pair->first.move_assign_count(),
            pair->first.copy_assign_count(),
            pair->first.constructor_count(),
            pair->first.move_constructor_count(),
            pair->first.copy_constructor_count(),
            pair->first.id(),
            pair->second.move_assign_count(),
            pair->second.copy_assign_count(),
            pair->second.constructor_count(),
            pair->second.move_constructor_count(),
            pair->second.copy_constructor_count(),
            pair->second.id(),
        };
        hud::memory::free_array(pair, 1);
        return result;
    };

    // Non constant
    {
        const auto result = test(123, 456);
        hud_assert_eq(std::get<0>(result), 0u);
        hud_assert_eq(std::get<1>(result), 0u);
        hud_assert_eq(std::get<2>(result), 1u);
        hud_assert_eq(std::get<3>(result), 0u);
        hud_assert_eq(std::get<4>(result), 1u);
        hud_assert_eq(std::get<5>(result), 123);

        hud_assert_eq(std::get<6>(result), 0u);
        hud_assert_eq(std::get<7>(result), 0u);
        hud_assert_eq(std::get<8>(result), 1u);
        hud_assert_eq(std::get<9>(result), 0u);
        hud_assert_eq(std::get<10>(result), 1u);
        hud_assert_eq(std::get<11>(result), 456);
    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        hud_assert_eq(std::get<0>(result), 0u);
        hud_assert_eq(std::get<1>(result), 0u);
        hud_assert_eq(std::get<2>(result), 1u);
        hud_assert_eq(std::get<3>(result), 0u);
        hud_assert_eq(std::get<4>(result), 1u);
        hud_assert_eq(std::get<5>(result), 123);

        hud_assert_eq(std::get<6>(result), 0u);
        hud_assert_eq(std::get<7>(result), 0u);
        hud_assert_eq(std::get<8>(result), 1u);
        hud_assert_eq(std::get<9>(result), 0u);
        hud_assert_eq(std::get<10>(result), 1u);
        hud_assert_eq(std::get<11>(result), 456);
    }
}

GTEST_TEST(pair, param_copy_constructor_non_trivial_copy_constructible_type_same_type)
{

    using Type1 = hud_test::non_bitwise_copy_constructible_type;
    using Type2 = hud_test::non_bitwise_copy_constructible_type;
    static_assert(!hud::is_trivially_copy_constructible_v<Type1>);
    static_assert(!hud::is_trivially_copy_constructible_v<Type2>);
    using TypePair = hud::pair<Type1, Type2>;

    const auto test = [](const i32 &id1, const i32 &id2)
    {
        TypePair *pair = hud::memory::allocate_array<TypePair>(1);
        const Type1 t1 {id1};
        const Type2 t2 {id2};
        hud::memory::construct_object_at(pair, t1, t2);
        const auto result = std::tuple {
            pair->first.copy_constructor_count(),
            pair->first.id(),
            pair->second.copy_constructor_count(),
            pair->second.id(),
        };
        hud::memory::free_array(pair, 1);
        return result;
    };

    // Non constant
    {
        const auto result = test(123, 456);
        hud_assert_eq(std::get<0>(result), 1u);
        hud_assert_eq(std::get<1>(result), 123);
        hud_assert_eq(std::get<2>(result), 1u);
        hud_assert_eq(std::get<3>(result), 456);
    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        hud_assert_eq(std::get<0>(result), 1u);
        hud_assert_eq(std::get<1>(result), 123);
        hud_assert_eq(std::get<2>(result), 1u);
        hud_assert_eq(std::get<3>(result), 456);
    }
}

GTEST_TEST(pair, param_copy_constructor_non_trivial_type_different_type)
{

    using Type1 = hud_test::non_bitwise_copy_constructible_type2;
    using Type2 = hud_test::non_bitwise_copy_constructible_type2;
    using OtherType1 = hud_test::non_bitwise_copy_constructible_type;
    using OtherType2 = hud_test::non_bitwise_copy_constructible_type;
    static_assert(hud::is_copy_constructible_v<Type1, OtherType1>);
    static_assert(!hud::is_trivially_copy_constructible_v<Type1, OtherType1>);
    static_assert(hud::is_copy_constructible_v<Type2, OtherType2>);
    static_assert(!hud::is_trivially_copy_constructible_v<Type2, OtherType2>);
    using TypePair = hud::pair<Type1, Type2>;

    const auto test = [](const i32 &id1, const i32 &id2)
    {
        TypePair *pair = hud::memory::allocate_array<TypePair>(1);
        const OtherType1 t1 {id1};
        const OtherType2 t2 {id2};
        hud::memory::construct_object_at(pair, t1, t2);
        const auto result = std::tuple {
            pair->first.copy_constructor_count(),
            pair->first.id(),
            pair->second.copy_constructor_count(),
            pair->second.id(),
        };
        hud::memory::free_array(pair, 1);
        return result;
    };

    // Non constant
    {
        const auto result = test(123, 456);
        hud_assert_eq(std::get<0>(result), 1u);
        hud_assert_eq(std::get<1>(result), 123);
        hud_assert_eq(std::get<2>(result), 1u);
        hud_assert_eq(std::get<3>(result), 456);
    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        hud_assert_eq(std::get<0>(result), 1u);
        hud_assert_eq(std::get<1>(result), 123);
        hud_assert_eq(std::get<2>(result), 1u);
        hud_assert_eq(std::get<3>(result), 456);
    }
}

GTEST_TEST(pair, param_copy_constructor_non_trivial_copy_constructible_type_different_type)
{

    using Type1 = hud_test::non_bitwise_copy_constructible_type2;
    using Type2 = hud_test::non_bitwise_copy_constructible_type2;
    using OtherType1 = hud_test::non_bitwise_copy_constructible_type;
    using OtherType2 = hud_test::non_bitwise_copy_constructible_type;
    static_assert(hud::is_copy_constructible_v<Type1, OtherType1>);
    static_assert(!hud::is_trivially_copy_constructible_v<Type1, OtherType1>);
    static_assert(hud::is_copy_constructible_v<Type2, OtherType2>);
    static_assert(!hud::is_trivially_copy_constructible_v<Type2, OtherType2>);
    using TypePair = hud::pair<Type1, Type2>;

    const auto test = [](const i32 &id1, const i32 &id2)
    {
        TypePair *pair = hud::memory::allocate_array<TypePair>(1);
        const OtherType1 t1 {id1};
        const OtherType2 t2 {id2};
        hud::memory::construct_object_at(pair, t1, t2);
        const auto result = std::tuple {
            pair->first.copy_constructor_count(),
            pair->first.id(),
            pair->second.copy_constructor_count(),
            pair->second.id(),
        };
        hud::memory::free_array(pair, 1);
        return result;
    };

    // Non constant
    {
        const auto result = test(123, 456);
        hud_assert_eq(std::get<0>(result), 1u);
        hud_assert_eq(std::get<1>(result), 123);
        hud_assert_eq(std::get<2>(result), 1u);
        hud_assert_eq(std::get<3>(result), 456);
    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        hud_assert_eq(std::get<0>(result), 1u);
        hud_assert_eq(std::get<1>(result), 123);
        hud_assert_eq(std::get<2>(result), 1u);
        hud_assert_eq(std::get<3>(result), 456);
    }
}

GTEST_TEST(pair, param_move_constructor_trivial_type_same_type)
{

    using Type1 = i32;
    using Type2 = f32;
    static_assert(hud::is_trivially_copy_constructible_v<Type1>);
    static_assert(hud::is_trivially_copy_constructible_v<Type2>);
    using TypePair = hud::pair<Type1, Type2>;

    const auto test = [](Type1 &&t1, Type2 &&t2)
    {
        TypePair *pair = hud::memory::allocate_array<TypePair>(1);
        hud::memory::construct_object_at(pair, hud::forward<Type1>(t1), hud::forward<Type2>(t2));
        const auto result = std::tuple {
            pair->first,
            pair->second
        };
        hud::memory::free_array(pair, 1);
        return result;
    };

    // Non constant
    {
        const auto result = test(123, 456.0f);
        hud_assert_eq(std::get<0>(result), 123);
        hud_assert_eq(std::get<1>(result), 456.0f);
    }

    // Constant
    {
        constexpr auto result = test(123, 456.0f);
        hud_assert_eq(std::get<0>(result), 123);
        hud_assert_eq(std::get<1>(result), 456.0f);
    }
}

GTEST_TEST(pair, param_move_constructor_non_trivial_type_same_type)
{

    using Type1 = hud_test::non_bitwise_type;
    using Type2 = hud_test::non_bitwise_type;
    static_assert(!hud::is_trivially_copy_constructible_v<Type1>);
    static_assert(!hud::is_trivially_copy_constructible_v<Type2>);
    using TypePair = hud::pair<Type1, Type2>;

    const auto test = [](const i32 &id1, const i32 &id2)
    {
        TypePair *pair = hud::memory::allocate_array<TypePair>(1);
        Type1 t1 {id1, nullptr};
        Type2 t2 {id2, nullptr};
        hud::memory::construct_object_at(pair, hud::move(t1), hud::move(t2));
        const auto result = std::tuple {
            pair->first.move_assign_count(),
            pair->first.copy_assign_count(),
            pair->first.constructor_count(),
            pair->first.move_constructor_count(),
            pair->first.copy_constructor_count(),
            pair->first.id(),
            pair->second.move_assign_count(),
            pair->second.copy_assign_count(),
            pair->second.constructor_count(),
            pair->second.move_constructor_count(),
            pair->second.copy_constructor_count(),
            pair->second.id(),
        };
        hud::memory::free_array(pair, 1);
        return result;
    };

    // Non constant
    {
        const auto result = test(123, 456);
        hud_assert_eq(std::get<0>(result), 0u);
        hud_assert_eq(std::get<1>(result), 0u);
        hud_assert_eq(std::get<2>(result), 1u);
        hud_assert_eq(std::get<3>(result), 1u);
        hud_assert_eq(std::get<4>(result), 0u);
        hud_assert_eq(std::get<5>(result), 123);

        hud_assert_eq(std::get<6>(result), 0u);
        hud_assert_eq(std::get<7>(result), 0u);
        hud_assert_eq(std::get<8>(result), 1u);
        hud_assert_eq(std::get<9>(result), 1u);
        hud_assert_eq(std::get<10>(result), 0u);
        hud_assert_eq(std::get<11>(result), 456);
    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        hud_assert_eq(std::get<0>(result), 0u);
        hud_assert_eq(std::get<1>(result), 0u);
        hud_assert_eq(std::get<2>(result), 1u);
        hud_assert_eq(std::get<3>(result), 1u);
        hud_assert_eq(std::get<4>(result), 0u);
        hud_assert_eq(std::get<5>(result), 123);

        hud_assert_eq(std::get<6>(result), 0u);
        hud_assert_eq(std::get<7>(result), 0u);
        hud_assert_eq(std::get<8>(result), 1u);
        hud_assert_eq(std::get<9>(result), 1u);
        hud_assert_eq(std::get<10>(result), 0u);
        hud_assert_eq(std::get<11>(result), 456);
    }
}

GTEST_TEST(pair, param_move_constructor_non_trivial_copy_constructible_type_same_type)
{

    using Type1 = hud_test::non_bitwise_copy_constructible_type;
    using Type2 = hud_test::non_bitwise_copy_constructible_type;
    static_assert(!hud::is_trivially_copy_constructible_v<Type1>);
    static_assert(!hud::is_trivially_copy_constructible_v<Type2>);
    using TypePair = hud::pair<Type1, Type2>;

    const auto test = [](const i32 &id1, const i32 &id2)
    {
        TypePair *pair = hud::memory::allocate_array<TypePair>(1);
        Type1 t1 {id1};
        Type2 t2 {id2};
        hud::memory::construct_object_at(pair, hud::move(t1), hud::move(t2));
        const auto result = std::tuple {
            pair->first.copy_constructor_count(),
            pair->first.id(),
            pair->second.copy_constructor_count(),
            pair->second.id(),
        };
        hud::memory::free_array(pair, 1);
        return result;
    };

    // Non constant
    {
        const auto result = test(123, 456);
        hud_assert_eq(std::get<0>(result), 1u);
        hud_assert_eq(std::get<1>(result), 123);
        hud_assert_eq(std::get<2>(result), 1u);
        hud_assert_eq(std::get<3>(result), 456);
    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        hud_assert_eq(std::get<0>(result), 1u);
        hud_assert_eq(std::get<1>(result), 123);
        hud_assert_eq(std::get<2>(result), 1u);
        hud_assert_eq(std::get<3>(result), 456);
    }
}

GTEST_TEST(pair, param_move_constructor_non_trivial_move_constructible_type_same_type)
{

    using Type1 = hud_test::non_bitwise_move_constructible_type;
    using Type2 = hud_test::non_bitwise_move_constructible_type;
    static_assert(!hud::is_trivially_move_constructible_v<Type1>);
    static_assert(!hud::is_trivially_move_constructible_v<Type2>);
    using TypePair = hud::pair<Type1, Type2>;

    const auto test = [](const i32 &id1, const i32 &id2)
    {
        TypePair *pair = hud::memory::allocate_array<TypePair>(1);
        Type1 t1 {id1};
        Type2 t2 {id2};

        hud::memory::construct_object_at(pair, hud::move(t1), hud::move(t2));
        const auto result = std::tuple {
            pair->first.copy_constructor_count(),
            pair->first.move_constructor_count(),
            pair->first.id(),
            pair->second.copy_constructor_count(),
            pair->second.move_constructor_count(),
            pair->second.id(),
        };
        hud::memory::free_array(pair, 1);
        return result;
    };

    // Non constant
    {
        const auto result = test(123, 456);
        hud_assert_eq(std::get<0>(result), 0u);
        hud_assert_eq(std::get<1>(result), 1u);
        hud_assert_eq(std::get<2>(result), 123);
        hud_assert_eq(std::get<3>(result), 0u);
        hud_assert_eq(std::get<4>(result), 1u);
        hud_assert_eq(std::get<5>(result), 456);
    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        hud_assert_eq(std::get<0>(result), 0u);
        hud_assert_eq(std::get<1>(result), 1u);
        hud_assert_eq(std::get<2>(result), 123);
        hud_assert_eq(std::get<3>(result), 0u);
        hud_assert_eq(std::get<4>(result), 1u);
        hud_assert_eq(std::get<5>(result), 456);
    }
}

GTEST_TEST(pair, param_move_constructor_trivial_type_different_type)
{

    using Type1 = i64;
    using Type2 = i32;
    using OtherType1 = i16;
    using OtherType2 = i8;
    static_assert(hud::is_trivially_copy_constructible_v<Type1, OtherType1>);
    static_assert(hud::is_trivially_copy_constructible_v<Type2, OtherType2>);
    using TypePair = hud::pair<Type1, Type2>;

    const auto test = [](OtherType1 &&t1, OtherType2 &&t2)
    {
        TypePair *pair = hud::memory::allocate_array<TypePair>(1);
        hud::memory::construct_object_at(pair, hud::forward<OtherType1>(t1), hud::forward<OtherType2>(t2));
        const auto result = std::tuple {
            pair->first,
            pair->second
        };
        hud::memory::free_array(pair, 1);
        return result;
    };

    // Non constant
    {
        const auto result = test(123, 32);
        hud_assert_eq(std::get<0>(result), 123);
        hud_assert_eq(std::get<1>(result), 32);
    }

    // Constant
    {
        constexpr auto result = test(123, 32);
        hud_assert_eq(std::get<0>(result), 123);
        hud_assert_eq(std::get<1>(result), 32);
    }
}

GTEST_TEST(pair, param_move_constructor_non_trivial_type_different_type)
{

    using Type1 = hud_test::non_bitwise_copy_constructible_type2;
    using Type2 = hud_test::non_bitwise_copy_constructible_type2;
    using OtherType1 = hud_test::non_bitwise_copy_constructible_type;
    using OtherType2 = hud_test::non_bitwise_copy_constructible_type;
    static_assert(hud::is_copy_constructible_v<Type1, OtherType1>);
    static_assert(!hud::is_trivially_copy_constructible_v<Type1, OtherType1>);
    static_assert(hud::is_copy_constructible_v<Type2, OtherType2>);
    static_assert(!hud::is_trivially_copy_constructible_v<Type2, OtherType2>);
    using TypePair = hud::pair<Type1, Type2>;

    const auto test = [](const i32 &id1, const i32 &id2)
    {
        TypePair *pair = hud::memory::allocate_array<TypePair>(1);
        OtherType1 t1 {id1};
        OtherType2 t2 {id2};
        hud::memory::construct_object_at(pair, hud::move(t1), hud::move(t2));
        const auto result = std::tuple {
            pair->first.copy_constructor_count(),
            pair->first.id(),
            pair->second.copy_constructor_count(),
            pair->second.id(),
        };
        hud::memory::free_array(pair, 1);
        return result;
    };

    // Non constant
    {
        const auto result = test(123, 456);
        hud_assert_eq(std::get<0>(result), 1u);
        hud_assert_eq(std::get<1>(result), 123);
        hud_assert_eq(std::get<2>(result), 1u);
        hud_assert_eq(std::get<3>(result), 456);
    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        hud_assert_eq(std::get<0>(result), 1u);
        hud_assert_eq(std::get<1>(result), 123);
        hud_assert_eq(std::get<2>(result), 1u);
        hud_assert_eq(std::get<3>(result), 456);
    }
}

GTEST_TEST(pair, param_move_constructor_non_trivial_copy_constructible_type_different_type)
{

    using Type1 = hud_test::non_bitwise_copy_constructible_type2;
    using Type2 = hud_test::non_bitwise_copy_constructible_type2;
    using OtherType1 = hud_test::non_bitwise_copy_constructible_type;
    using OtherType2 = hud_test::non_bitwise_copy_constructible_type;
    static_assert(hud::is_copy_constructible_v<Type1, OtherType1>);
    static_assert(!hud::is_trivially_copy_constructible_v<Type1, OtherType1>);
    static_assert(hud::is_copy_constructible_v<Type2, OtherType2>);
    static_assert(!hud::is_trivially_copy_constructible_v<Type2, OtherType2>);
    using TypePair = hud::pair<Type1, Type2>;

    const auto test = [](const i32 &id1, const i32 &id2)
    {
        TypePair *pair = hud::memory::allocate_array<TypePair>(1);
        OtherType1 t1 {id1};
        OtherType2 t2 {id2};
        hud::memory::construct_object_at(pair, hud::move(t1), hud::move(t2));
        const auto result = std::tuple {
            pair->first.copy_constructor_count(),
            pair->first.id(),
            pair->second.copy_constructor_count(),
            pair->second.id(),
        };
        hud::memory::free_array(pair, 1);
        return result;
    };

    // Non constant
    {
        const auto result = test(123, 456);
        hud_assert_eq(std::get<0>(result), 1u);
        hud_assert_eq(std::get<1>(result), 123);
        hud_assert_eq(std::get<2>(result), 1u);
        hud_assert_eq(std::get<3>(result), 456);
    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        hud_assert_eq(std::get<0>(result), 1u);
        hud_assert_eq(std::get<1>(result), 123);
        hud_assert_eq(std::get<2>(result), 1u);
        hud_assert_eq(std::get<3>(result), 456);
    }
}

GTEST_TEST(pair, param_move_constructor_non_trivial_move_constructible_type_different_type)
{

    using Type1 = hud_test::non_bitwise_move_constructible_type2;
    using Type2 = hud_test::non_bitwise_move_constructible_type2;
    using OtherType1 = hud_test::non_bitwise_move_constructible_type;
    using OtherType2 = hud_test::non_bitwise_move_constructible_type;
    static_assert(hud::is_move_constructible_v<Type1, OtherType1>);
    static_assert(!hud::is_trivially_move_constructible_v<Type1, OtherType1>);
    static_assert(hud::is_move_constructible_v<Type2, OtherType2>);
    static_assert(!hud::is_trivially_move_constructible_v<Type2, OtherType2>);
    using TypePair = hud::pair<Type1, Type2>;

    const auto test = [](const i32 &id1, const i32 &id2)
    {
        TypePair *pair = hud::memory::allocate_array<TypePair>(1);
        OtherType1 t1 {id1};
        OtherType2 t2 {id2};

        hud::memory::construct_object_at(pair, hud::move(t1), hud::move(t2));
        const auto result = std::tuple {
            pair->first.copy_constructor_count(),
            pair->first.move_constructor_count(),
            pair->first.id(),
            pair->second.copy_constructor_count(),
            pair->second.move_constructor_count(),
            pair->second.id(),
        };
        hud::memory::free_array(pair, 1);
        return result;
    };

    // Non constant
    {
        const auto result = test(123, 456);
        hud_assert_eq(std::get<0>(result), 0u);
        hud_assert_eq(std::get<1>(result), 1u);
        hud_assert_eq(std::get<2>(result), 123);
        hud_assert_eq(std::get<3>(result), 0u);
        hud_assert_eq(std::get<4>(result), 1u);
        hud_assert_eq(std::get<5>(result), 456);
    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        hud_assert_eq(std::get<0>(result), 0u);
        hud_assert_eq(std::get<1>(result), 1u);
        hud_assert_eq(std::get<2>(result), 123);
        hud_assert_eq(std::get<3>(result), 0u);
        hud_assert_eq(std::get<4>(result), 1u);
        hud_assert_eq(std::get<5>(result), 456);
    }
};

GTEST_TEST(pair, piecewise_constructor_trivial_type_same_type)
{
    const auto test = []()
    {
        hud::pair<i32, i64> pair {hud::tag_piecewise_construct, hud::forward_as_tuple(i32 {1}), hud::forward_as_tuple(i64 {2})};
        return std::tuple {
            hud::get<0>(pair) == 1, // 0
            hud::get<1>(pair) == 2, // 1
        };
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
    }

    // Non constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
    }
}

GTEST_TEST(pair, piecewise_constructor_trivial_type_different_type)
{
    const auto test = []()
    {
        hud::pair<i32, i64> pair {hud::tag_piecewise_construct, hud::forward_as_tuple(u32 {1}), hud::forward_as_tuple(u64 {2})};
        return std::tuple {
            hud::get<0>(pair) == 1, // 0
            hud::get<1>(pair) == 2, // 1
        };
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
    }

    // Non constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
    }
}

GTEST_TEST(pair, piecewise_constructor_non_trivial_type_same_type)
{
    // By param
    {
        const auto test = []()
        {
            using type = hud_test::non_bitwise_type;
            const type to_copy_0;
            const type to_copy_1;
            hud::pair<type, type> pair {hud::tag_piecewise_construct, hud::forward_as_tuple(1, nullptr), hud::forward_as_tuple(2, nullptr)};
            return std::tuple {
                hud::get<0>(pair).id(),                          // 0
                hud::get<0>(pair).constructor_count() == 1,      // 1
                hud::get<0>(pair).copy_constructor_count() == 0, // 2
                hud::get<0>(pair).move_constructor_count() == 0, // 3
                hud::get<0>(pair).copy_assign_count() == 0,      // 4
                hud::get<0>(pair).move_assign_count() == 0,      // 5
                hud::get<1>(pair).id(),                          // 6
                hud::get<1>(pair).constructor_count() == 1,      // 7
                hud::get<1>(pair).copy_constructor_count() == 0, // 8
                hud::get<1>(pair).move_constructor_count() == 0, // 9
                hud::get<1>(pair).copy_assign_count() == 0,      // 10
                hud::get<1>(pair).move_assign_count() == 0,      // 11
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
            hud_assert_true(std::get<6>(result));
            hud_assert_true(std::get<7>(result));
            hud_assert_true(std::get<8>(result));
            hud_assert_true(std::get<9>(result));
        }

        // Non constant
        {
            constexpr auto result = test();
            hud_assert_true(std::get<0>(result));
            hud_assert_true(std::get<1>(result));
            hud_assert_true(std::get<2>(result));
            hud_assert_true(std::get<3>(result));
            hud_assert_true(std::get<4>(result));
            hud_assert_true(std::get<5>(result));
            hud_assert_true(std::get<6>(result));
            hud_assert_true(std::get<7>(result));
            hud_assert_true(std::get<8>(result));
            hud_assert_true(std::get<9>(result));
        }
    }

    // By copy
    {
        const auto test = []()
        {
            using type = hud_test::non_bitwise_type;
            const type to_copy_0;
            const type to_copy_1;
            hud::pair<type, type> pair {hud::tag_piecewise_construct, hud::forward_as_tuple(to_copy_0), hud::forward_as_tuple(to_copy_1)};
            return std::tuple {
                hud::get<0>(pair).constructor_count() == 0,      // 0
                hud::get<0>(pair).copy_constructor_count() == 1, // 1
                hud::get<0>(pair).move_constructor_count() == 0, // 2
                hud::get<0>(pair).copy_assign_count() == 0,      // 3
                hud::get<0>(pair).move_assign_count() == 0,      // 4
                hud::get<1>(pair).constructor_count() == 0,      // 5
                hud::get<1>(pair).copy_constructor_count() == 1, // 6
                hud::get<1>(pair).move_constructor_count() == 0, // 7
                hud::get<1>(pair).copy_assign_count() == 0,      // 8
                hud::get<1>(pair).move_assign_count() == 0,      // 9
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
            hud_assert_true(std::get<6>(result));
            hud_assert_true(std::get<7>(result));
            hud_assert_true(std::get<8>(result));
            hud_assert_true(std::get<9>(result));
        }

        // Non constant
        {
            constexpr auto result = test();
            hud_assert_true(std::get<0>(result));
            hud_assert_true(std::get<1>(result));
            hud_assert_true(std::get<2>(result));
            hud_assert_true(std::get<3>(result));
            hud_assert_true(std::get<4>(result));
            hud_assert_true(std::get<5>(result));
            hud_assert_true(std::get<6>(result));
            hud_assert_true(std::get<7>(result));
            hud_assert_true(std::get<8>(result));
            hud_assert_true(std::get<9>(result));
        }
    }

    // By move
    {
        const auto test = []()
        {
            using type = hud_test::non_bitwise_type;
            hud::pair<type, type> pair {hud::tag_piecewise_construct, hud::forward_as_tuple(type {}), hud::forward_as_tuple(type {})};
            return std::tuple {
                hud::get<0>(pair).constructor_count() == 0,      // 0
                hud::get<0>(pair).copy_constructor_count() == 0, // 1
                hud::get<0>(pair).move_constructor_count() == 1, // 2
                hud::get<0>(pair).copy_assign_count() == 0,      // 3
                hud::get<0>(pair).move_assign_count() == 0,      // 4
                hud::get<1>(pair).constructor_count() == 0,      // 5
                hud::get<1>(pair).copy_constructor_count() == 0, // 6
                hud::get<1>(pair).move_constructor_count() == 1, // 7
                hud::get<1>(pair).copy_assign_count() == 0,      // 8
                hud::get<1>(pair).move_assign_count() == 0,      // 9
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
            hud_assert_true(std::get<6>(result));
            hud_assert_true(std::get<7>(result));
            hud_assert_true(std::get<8>(result));
            hud_assert_true(std::get<9>(result));
        }

        // Non constant
        {
            constexpr auto result = test();
            hud_assert_true(std::get<0>(result));
            hud_assert_true(std::get<1>(result));
            hud_assert_true(std::get<2>(result));
            hud_assert_true(std::get<3>(result));
            hud_assert_true(std::get<4>(result));
            hud_assert_true(std::get<5>(result));
            hud_assert_true(std::get<6>(result));
            hud_assert_true(std::get<7>(result));
            hud_assert_true(std::get<8>(result));
            hud_assert_true(std::get<9>(result));
        }
    }
}

GTEST_TEST(pair, piecewise_constructor_non_trivial_type_different_type)
{
    // By copy
    {
        const auto test = []()
        {
            using type = hud_test::non_bitwise_type;
            using type2 = hud_test::non_bitwise_type2;
            const type to_copy_0;
            const type to_copy_1;
            hud::pair<type2, type2> pair {hud::tag_piecewise_construct, hud::forward_as_tuple(to_copy_0), hud::forward_as_tuple(to_copy_1)};
            return std::tuple {
                hud::get<0>(pair).constructor_count() == 0,      // 0
                hud::get<0>(pair).copy_constructor_count() == 1, // 1
                hud::get<0>(pair).move_constructor_count() == 0, // 2
                hud::get<0>(pair).copy_assign_count() == 0,      // 3
                hud::get<0>(pair).move_assign_count() == 0,      // 4
                hud::get<1>(pair).constructor_count() == 0,      // 5
                hud::get<1>(pair).copy_constructor_count() == 1, // 6
                hud::get<1>(pair).move_constructor_count() == 0, // 7
                hud::get<1>(pair).copy_assign_count() == 0,      // 8
                hud::get<1>(pair).move_assign_count() == 0,      // 9
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
            hud_assert_true(std::get<6>(result));
            hud_assert_true(std::get<7>(result));
            hud_assert_true(std::get<8>(result));
            hud_assert_true(std::get<9>(result));
        }

        // Non constant
        {
            constexpr auto result = test();
            hud_assert_true(std::get<0>(result));
            hud_assert_true(std::get<1>(result));
            hud_assert_true(std::get<2>(result));
            hud_assert_true(std::get<3>(result));
            hud_assert_true(std::get<4>(result));
            hud_assert_true(std::get<5>(result));
            hud_assert_true(std::get<6>(result));
            hud_assert_true(std::get<7>(result));
            hud_assert_true(std::get<8>(result));
            hud_assert_true(std::get<9>(result));
        }
    }

    // By move
    {
        const auto test = []()
        {
            using type = hud_test::non_bitwise_type;
            using type2 = hud_test::non_bitwise_type2;
            hud::pair<type2, type2> pair {hud::tag_piecewise_construct, hud::forward_as_tuple(type {}), hud::forward_as_tuple(type {})};
            return std::tuple {
                hud::get<0>(pair).constructor_count() == 0,      // 0
                hud::get<0>(pair).copy_constructor_count() == 0, // 1
                hud::get<0>(pair).move_constructor_count() == 1, // 2
                hud::get<0>(pair).copy_assign_count() == 0,      // 3
                hud::get<0>(pair).move_assign_count() == 0,      // 4
                hud::get<1>(pair).constructor_count() == 0,      // 5
                hud::get<1>(pair).copy_constructor_count() == 0, // 6
                hud::get<1>(pair).move_constructor_count() == 1, // 7
                hud::get<1>(pair).copy_assign_count() == 0,      // 8
                hud::get<1>(pair).move_assign_count() == 0,      // 9
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
            hud_assert_true(std::get<6>(result));
            hud_assert_true(std::get<7>(result));
            hud_assert_true(std::get<8>(result));
            hud_assert_true(std::get<9>(result));
        }

        // Non constant
        {
            constexpr auto result = test();
            hud_assert_true(std::get<0>(result));
            hud_assert_true(std::get<1>(result));
            hud_assert_true(std::get<2>(result));
            hud_assert_true(std::get<3>(result));
            hud_assert_true(std::get<4>(result));
            hud_assert_true(std::get<5>(result));
            hud_assert_true(std::get<6>(result));
            hud_assert_true(std::get<7>(result));
            hud_assert_true(std::get<8>(result));
            hud_assert_true(std::get<9>(result));
        }
    }
}

GTEST_TEST(pair, piecewise_constructor_non_trivial_copy_constructible_type_same_type)
{
    // By copy
    {
        const auto test = []()
        {
            using type = hud_test::non_bitwise_copy_constructible_type;
            const type to_copy_0;
            const type to_copy_1;
            hud::pair<type, type> pair {hud::tag_piecewise_construct, hud::forward_as_tuple(to_copy_0), hud::forward_as_tuple(to_copy_1)};
            return std::tuple {

                hud::get<0>(pair).copy_constructor_count() == 1, // 0
                hud::get<1>(pair).copy_constructor_count() == 1, // 1
            };
        };

        // Non constant
        {
            const auto result = test();
            hud_assert_true(std::get<0>(result));
            hud_assert_true(std::get<1>(result));
        }

        // Non constant
        {
            constexpr auto result = test();
            hud_assert_true(std::get<0>(result));
            hud_assert_true(std::get<1>(result));
        }
    }
}

GTEST_TEST(pair, piecewise_constructor_non_trivial_copy_constructible_type_different_type)
{
    // By copy
    {
        const auto test = []()
        {
            using type = hud_test::non_bitwise_copy_constructible_type;
            using type2 = hud_test::non_bitwise_copy_constructible_type2;
            const type to_copy_0;
            const type to_copy_1;
            hud::pair<type2, type2> pair {hud::tag_piecewise_construct, hud::forward_as_tuple(to_copy_0), hud::forward_as_tuple(to_copy_1)};
            return std::tuple {

                hud::get<0>(pair).copy_constructor_count() == 1, // 0
                hud::get<1>(pair).copy_constructor_count() == 1, // 1
            };
        };

        // Non constant
        {
            const auto result = test();
            hud_assert_true(std::get<0>(result));
            hud_assert_true(std::get<1>(result));
        }

        // Non constant
        {
            constexpr auto result = test();
            hud_assert_true(std::get<0>(result));
            hud_assert_true(std::get<1>(result));
        }
    }
}

GTEST_TEST(pair, piecewise_constructor_non_trivial_move_constructible_type_same_type)
{
    // By copy
    {
        const auto test = []()
        {
            using type = hud_test::non_bitwise_move_constructible_type;
            const type to_copy_0;
            const type to_copy_1;
            hud::pair<type, type> pair {hud::tag_piecewise_construct, hud::forward_as_tuple(to_copy_0), hud::forward_as_tuple(to_copy_1)};
            return std::tuple {

                hud::get<0>(pair).copy_constructor_count() == 1, // 0
                hud::get<1>(pair).copy_constructor_count() == 1, // 1
                hud::get<0>(pair).move_constructor_count() == 0, // 2
                hud::get<1>(pair).move_constructor_count() == 0, // 3
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

        // Non constant
        {
            constexpr auto result = test();
            hud_assert_true(std::get<0>(result));
            hud_assert_true(std::get<1>(result));
            hud_assert_true(std::get<2>(result));
            hud_assert_true(std::get<3>(result));
        }
    }

    // By move
    {
        const auto test = []()
        {
            using type = hud_test::non_bitwise_move_constructible_type;
            hud::pair<type, type> pair {hud::tag_piecewise_construct, hud::forward_as_tuple(type {}), hud::forward_as_tuple(type {})};
            return std::tuple {

                hud::get<0>(pair).copy_constructor_count() == 0, // 0
                hud::get<1>(pair).copy_constructor_count() == 0, // 1
                hud::get<0>(pair).move_constructor_count() == 1, // 2
                hud::get<1>(pair).move_constructor_count() == 1, // 3
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

        // Non constant
        {
            constexpr auto result = test();
            hud_assert_true(std::get<0>(result));
            hud_assert_true(std::get<1>(result));
            hud_assert_true(std::get<2>(result));
            hud_assert_true(std::get<3>(result));
        }
    }
}

GTEST_TEST(pair, piecewise_constructor_non_trivial_move_constructible_type_different_type)
{
    // By copy
    {
        const auto test = []()
        {
            using type = hud_test::non_bitwise_move_constructible_type;
            using type2 = hud_test::non_bitwise_move_constructible_type2;
            const type to_copy_0;
            const type to_copy_1;
            hud::pair<type2, type2> pair {hud::tag_piecewise_construct, hud::forward_as_tuple(to_copy_0), hud::forward_as_tuple(to_copy_1)};
            return std::tuple {

                hud::get<0>(pair).copy_constructor_count() == 1, // 0
                hud::get<1>(pair).copy_constructor_count() == 1, // 1
                hud::get<0>(pair).move_constructor_count() == 0, // 2
                hud::get<1>(pair).move_constructor_count() == 0, // 3
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

        // Non constant
        {
            constexpr auto result = test();
            hud_assert_true(std::get<0>(result));
            hud_assert_true(std::get<1>(result));
            hud_assert_true(std::get<2>(result));
            hud_assert_true(std::get<3>(result));
        }
    }

    // By move
    {
        const auto test = []()
        {
            using type = hud_test::non_bitwise_move_constructible_type;
            using type2 = hud_test::non_bitwise_move_constructible_type2;
            hud::pair<type2, type2> pair {hud::tag_piecewise_construct, hud::forward_as_tuple(type {}), hud::forward_as_tuple(type {})};
            return std::tuple {

                hud::get<0>(pair).copy_constructor_count() == 0, // 0
                hud::get<1>(pair).copy_constructor_count() == 0, // 1
                hud::get<0>(pair).move_constructor_count() == 1, // 2
                hud::get<1>(pair).move_constructor_count() == 1, // 3
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

        // Non constant
        {
            constexpr auto result = test();
            hud_assert_true(std::get<0>(result));
            hud_assert_true(std::get<1>(result));
            hud_assert_true(std::get<2>(result));
            hud_assert_true(std::get<3>(result));
        }
    }
}
