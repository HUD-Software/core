#include <core/containers/compressed_pair.h>
#include <core/traits/is_implicitly_default_constructible.h>
#include <core/traits/is_explicitly_default_constructible.h>
#include <core/traits/is_implicitly_constructible.h>
#include <core/traits/is_explicitly_constructible.h>
#include <core/traits/is_explicitly_copy_constructible.h>
#include <core/traits/is_explicitly_move_constructible.h>
#include <core/traits/is_empty.h>

struct implicit_type_non_empty
{
    implicit_type_non_empty() noexcept = default;

    implicit_type_non_empty(wchar value) noexcept
        : mInner(value)
    {
    }

    implicit_type_non_empty(const implicit_type_non_empty &other) noexcept
        : mInner(other.mInner) {};
    implicit_type_non_empty(implicit_type_non_empty &&other) noexcept
        : mInner(other.mInner + 1) {};
    wchar mInner = L'\0';
};

static_assert(hud::is_implicitly_default_constructible_v<implicit_type_non_empty>);
static_assert(!hud::is_explicitly_default_constructible_v<implicit_type_non_empty>);
static_assert(hud::is_implicitly_constructible_v<implicit_type_non_empty, wchar>);
static_assert(!hud::is_explicitly_constructible_v<implicit_type_non_empty, wchar>);
static_assert(hud::is_implicitly_constructible_v<implicit_type_non_empty, const implicit_type_non_empty &>);
static_assert(!hud::is_explicitly_constructible_v<implicit_type_non_empty, const implicit_type_non_empty &>);
static_assert(hud::is_implicitly_constructible_v<implicit_type_non_empty, implicit_type_non_empty &&>);
static_assert(!hud::is_explicitly_constructible_v<implicit_type_non_empty, implicit_type_non_empty &&>);
static_assert(hud::is_convertible_v<const implicit_type_non_empty &, implicit_type_non_empty>);
static_assert(!hud::is_empty_v<implicit_type_non_empty>);

struct implicit_type_empty
{
    implicit_type_empty() noexcept = default;
    implicit_type_empty(const implicit_type_empty &other) noexcept = default;
    implicit_type_empty(implicit_type_empty &&other) noexcept = default;
};

static_assert(hud::is_implicitly_default_constructible_v<implicit_type_empty>);
static_assert(!hud::is_explicitly_default_constructible_v<implicit_type_empty>);
static_assert(hud::is_implicitly_constructible_v<implicit_type_empty, const implicit_type_empty &>);
static_assert(!hud::is_explicitly_constructible_v<implicit_type_empty, const implicit_type_empty &>);
static_assert(hud::is_implicitly_constructible_v<implicit_type_empty, implicit_type_empty &&>);
static_assert(!hud::is_explicitly_constructible_v<implicit_type_empty, implicit_type_empty &&>);
static_assert(hud::is_convertible_v<const implicit_type_empty &, implicit_type_empty>);
static_assert(hud::is_empty_v<implicit_type_empty>);

struct explicit_type_non_empty
{
    explicit explicit_type_non_empty() noexcept = default;

    explicit explicit_type_non_empty(i32 value) noexcept
        : mInner(value)
    {
    }

    explicit explicit_type_non_empty(const explicit_type_non_empty &other) noexcept
        : mInner(other.mInner) {};

    explicit explicit_type_non_empty(explicit_type_non_empty &&other) noexcept
        : mInner(other.mInner + 1) {};

    operator implicit_type_non_empty() const noexcept
    {
        return implicit_type_non_empty(static_cast<wchar>(mInner));
    }

    i32 mInner = 0;
};

static_assert(!hud::is_implicitly_default_constructible_v<explicit_type_non_empty>);
static_assert(hud::is_explicitly_default_constructible_v<explicit_type_non_empty>);
static_assert(!hud::is_implicitly_constructible_v<explicit_type_non_empty, i32>);
static_assert(hud::is_explicitly_constructible_v<explicit_type_non_empty, i32>);
static_assert(!hud::is_implicitly_constructible_v<explicit_type_non_empty, const explicit_type_non_empty &>);
static_assert(hud::is_explicitly_constructible_v<explicit_type_non_empty, const explicit_type_non_empty &>);
static_assert(!hud::is_implicitly_constructible_v<explicit_type_non_empty, explicit_type_non_empty &&>);
static_assert(hud::is_explicitly_constructible_v<explicit_type_non_empty, explicit_type_non_empty &&>);
static_assert(!hud::is_convertible_v<const explicit_type_non_empty &, explicit_type_non_empty>);
static_assert(!hud::is_empty_v<explicit_type_non_empty>);

struct explicit_type_empty
{
    explicit explicit_type_empty() noexcept = default;
    explicit explicit_type_empty(const explicit_type_empty &other) noexcept = default;
    explicit explicit_type_empty(explicit_type_empty &&other) noexcept = default;

    operator implicit_type_empty() const noexcept
    {
        return implicit_type_empty {};
    }
};

static_assert(!hud::is_implicitly_default_constructible_v<explicit_type_empty>);
static_assert(hud::is_explicitly_default_constructible_v<explicit_type_empty>);
static_assert(!hud::is_implicitly_constructible_v<explicit_type_empty, const explicit_type_empty &>);
static_assert(hud::is_explicitly_constructible_v<explicit_type_empty, const explicit_type_empty &>);
static_assert(!hud::is_implicitly_constructible_v<explicit_type_empty, explicit_type_empty &&>);
static_assert(hud::is_explicitly_constructible_v<explicit_type_empty, explicit_type_empty &&>);
static_assert(!hud::is_convertible_v<const explicit_type_empty &, explicit_type_empty>);
static_assert(hud::is_empty_v<explicit_type_empty>);

GTEST_TEST(compressed_pair, default_constructor_is_explicit_if_T1_or_T2_is_not_implicitly_default_constructible)
{

    static_assert(hud::is_implicitly_default_constructible_v<implicit_type_non_empty>);
    static_assert(hud::is_implicitly_default_constructible_v<hud::compressed_pair<i32, i32>>);
    static_assert(hud::is_implicitly_default_constructible_v<hud::compressed_pair<i32, f32>>);
    static_assert(hud::is_implicitly_default_constructible_v<hud::compressed_pair<i32, implicit_type_non_empty>>);
    static_assert(hud::is_implicitly_default_constructible_v<hud::compressed_pair<implicit_type_non_empty, implicit_type_non_empty>>);
    static_assert(hud::is_explicitly_default_constructible_v<hud::compressed_pair<i32, explicit_type_non_empty>>);
    static_assert(hud::is_explicitly_default_constructible_v<hud::compressed_pair<i32, explicit_type_non_empty>>);
    static_assert(hud::is_explicitly_default_constructible_v<hud::compressed_pair<explicit_type_non_empty, explicit_type_non_empty>>);
}

GTEST_TEST(compressed_pair, param_copy_constructor_is_explicit_if_T1_or_T2_is_not_implicitly_convertible)
{

    static_assert(hud::is_convertible_v<const explicit_type_non_empty &, implicit_type_non_empty>);
    static_assert(hud::is_convertible_v<const implicit_type_non_empty &, implicit_type_non_empty>);
    static_assert(hud::is_implicitly_constructible_v<hud::compressed_pair<implicit_type_non_empty, implicit_type_non_empty>, const implicit_type_non_empty &, const implicit_type_non_empty &>);
    static_assert(hud::is_implicitly_constructible_v<hud::compressed_pair<implicit_type_non_empty, implicit_type_non_empty>, const implicit_type_non_empty &, wchar>);
    static_assert(hud::is_explicitly_constructible_v<hud::compressed_pair<explicit_type_non_empty, implicit_type_non_empty>, const explicit_type_non_empty &, const implicit_type_non_empty &>);
    static_assert(hud::is_explicitly_constructible_v<hud::compressed_pair<explicit_type_non_empty, implicit_type_non_empty>, i32, const implicit_type_non_empty &>);
}

GTEST_TEST(compressed_pair, param_move_constructor_is_explicit_if_T1_or_T2_is_not_implicitly_converible)
{

    static_assert(hud::is_convertible_v<const explicit_type_non_empty &, implicit_type_non_empty>);
    static_assert(hud::is_convertible_v<const implicit_type_non_empty &, implicit_type_non_empty>);
    static_assert(hud::is_implicitly_constructible_v<hud::compressed_pair<implicit_type_non_empty, implicit_type_non_empty>, implicit_type_non_empty &&, implicit_type_non_empty &&>);
    static_assert(hud::is_implicitly_constructible_v<hud::compressed_pair<implicit_type_non_empty, implicit_type_non_empty>, implicit_type_non_empty &&, wchar>);
    static_assert(hud::is_explicitly_constructible_v<hud::compressed_pair<explicit_type_non_empty, implicit_type_non_empty>, explicit_type_non_empty &&, implicit_type_non_empty &&>);
    static_assert(hud::is_explicitly_constructible_v<hud::compressed_pair<explicit_type_non_empty, implicit_type_non_empty>, i32, implicit_type_non_empty &&>);
}

GTEST_TEST(compressed_pair, copy_constructor_is_explicit_if_T1_or_T2_is_not_implicitly_convertible)
{

    static_assert(hud::is_convertible_v<const explicit_type_non_empty &, implicit_type_non_empty>);
    static_assert(hud::is_convertible_v<const implicit_type_non_empty &, implicit_type_non_empty>);
    static_assert(hud::is_copy_constructible_v<explicit_type_non_empty, explicit_type_non_empty>);
    static_assert(hud::is_copy_constructible_v<implicit_type_non_empty, implicit_type_non_empty>);

    // Same
    static_assert(hud::is_implicitly_copy_constructible_v<hud::compressed_pair<implicit_type_non_empty, implicit_type_non_empty>, hud::compressed_pair<implicit_type_non_empty, implicit_type_non_empty>>);
    static_assert(hud::is_explicitly_copy_constructible_v<hud::compressed_pair<explicit_type_non_empty, implicit_type_non_empty>, hud::compressed_pair<explicit_type_non_empty, implicit_type_non_empty>>);

    // Not same type_t
    static_assert(hud::is_implicitly_copy_constructible_v<hud::compressed_pair<implicit_type_non_empty, implicit_type_non_empty>, hud::compressed_pair<implicit_type_non_empty, wchar>>);
    static_assert(hud::is_explicitly_copy_constructible_v<hud::compressed_pair<explicit_type_non_empty, implicit_type_non_empty>, hud::compressed_pair<i32, implicit_type_non_empty>>);
    static_assert(hud::is_implicitly_copy_constructible_v<hud::compressed_pair<implicit_type_non_empty, implicit_type_non_empty>, hud::compressed_pair<i32, wchar>>);
}

GTEST_TEST(compressed_pair, move_constructor_is_explicit_if_T1_or_T2_is_not_implicitly_convertible)
{

    static_assert(hud::is_convertible_v<explicit_type_non_empty, implicit_type_non_empty>);
    static_assert(hud::is_convertible_v<implicit_type_non_empty, implicit_type_non_empty>);
    static_assert(hud::is_move_constructible_v<explicit_type_non_empty, explicit_type_non_empty>);
    static_assert(hud::is_move_constructible_v<implicit_type_non_empty, implicit_type_non_empty>);

    // Same
    static_assert(hud::is_implicitly_move_constructible_v<hud::compressed_pair<implicit_type_non_empty, implicit_type_non_empty>, hud::compressed_pair<implicit_type_non_empty, implicit_type_non_empty>>);
    static_assert(hud::is_explicitly_move_constructible_v<hud::compressed_pair<explicit_type_non_empty, implicit_type_non_empty>, hud::compressed_pair<explicit_type_non_empty, implicit_type_non_empty>>);
    static_assert(hud::is_explicitly_move_constructible_v<hud::compressed_pair<explicit_type_non_empty, implicit_type_non_empty>, hud::compressed_pair<i32, implicit_type_non_empty>>);

    // Not same type_t
    static_assert(hud::is_implicitly_move_constructible_v<hud::compressed_pair<implicit_type_non_empty, implicit_type_non_empty>, hud::compressed_pair<implicit_type_non_empty, wchar>>);
    static_assert(hud::is_explicitly_move_constructible_v<hud::compressed_pair<explicit_type_non_empty, implicit_type_non_empty>, hud::compressed_pair<i32, implicit_type_non_empty>>);
    static_assert(hud::is_explicitly_move_constructible_v<hud::compressed_pair<explicit_type_non_empty, implicit_type_non_empty>, hud::compressed_pair<i32, wchar>>);
}

GTEST_TEST(compressed_pair, default_constructor_trivial)
{
    // Both non empty
    {
        using Type1 = i32;
        using Type2 = f32;
        static_assert(hud::is_trivially_default_constructible_v<Type1>);
        static_assert(hud::is_trivially_default_constructible_v<Type2>);
        using TypePair = hud::compressed_pair<Type1, Type2>;

        const auto test = []() {
            TypePair *pair = hud::memory::allocate_array<TypePair>(1);
            hud::memory::construct_object_at(pair);
            const auto result = std::tuple {pair->first(), pair->second()};
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

    // First empty
    {
        using Type1 = implicit_type_empty;
        using Type2 = f32;
        static_assert(hud::is_trivially_default_constructible_v<Type1>);
        static_assert(hud::is_trivially_default_constructible_v<Type2>);
        using TypePair = hud::compressed_pair<Type1, Type2>;

        const auto test = []() {
            TypePair *pair = hud::memory::allocate_array<TypePair>(1);
            hud::memory::construct_object_at(pair);
            const auto result = std::tuple {pair->first(), pair->second()};
            hud::memory::free_array(pair, 1);
            return result;
        };

        // Non constant
        {
            const auto result = test();
            hud_assert_eq(sizeof(std::get<0>(result)), 1);
            hud_assert_eq(std::get<1>(result), 0.0f);
        }

        // Constant
        {
            constexpr auto result = test();
            hud_assert_eq(sizeof(std::get<0>(result)), 1);
            hud_assert_eq(std::get<1>(result), 0.0f);
        }
    }

    // Second empty
    {
        using Type1 = i32;
        using Type2 = implicit_type_empty;
        static_assert(hud::is_trivially_default_constructible_v<Type1>);
        static_assert(hud::is_trivially_default_constructible_v<Type2>);
        using TypePair = hud::compressed_pair<Type1, Type2>;

        const auto test = []() {
            TypePair *pair = hud::memory::allocate_array<TypePair>(1);
            hud::memory::construct_object_at(pair);
            const auto result = std::tuple {pair->first(), pair->second()};
            hud::memory::free_array(pair, 1);
            return result;
        };

        // Non constant
        {
            const auto result = test();
            hud_assert_eq(std::get<0>(result), 0);
            hud_assert_eq(sizeof(std::get<1>(result)), 1);
        }

        // Constant
        {
            constexpr auto result = test();
            hud_assert_eq(std::get<0>(result), 0);
            hud_assert_eq(sizeof(std::get<1>(result)), 1);
        }
    }

    // Both empty
    {
        using Type1 = implicit_type_empty;
        using Type2 = implicit_type_empty;
        static_assert(hud::is_trivially_default_constructible_v<Type1>);
        static_assert(hud::is_trivially_default_constructible_v<Type2>);
        using TypePair = hud::compressed_pair<Type1, Type2>;

        const auto test = []() {
            TypePair *pair = hud::memory::allocate_array<TypePair>(1);
            hud::memory::construct_object_at(pair);
            const auto result = std::tuple {pair->first(), pair->second()};
            hud::memory::free_array(pair, 1);
            return result;
        };

        // Non constant
        {
            const auto result = test();
            hud_assert_eq(sizeof(std::get<0>(result)), 1);
            hud_assert_eq(sizeof(std::get<1>(result)), 1);
        }

        // Constant
        {
            constexpr auto result = test();
            hud_assert_eq(sizeof(std::get<0>(result)), 1);
            hud_assert_eq(sizeof(std::get<1>(result)), 1);
        }
    }
}

GTEST_TEST(compressed_pair, default_constructor_non_trivial)
{

    using Type1 = hud_test::non_bitwise_type;
    using Type2 = hud_test::non_bitwise_type;
    static_assert(!hud::is_trivially_default_constructible_v<Type1>);
    static_assert(!hud::is_trivially_default_constructible_v<Type2>);
    using TypePair = hud::compressed_pair<Type1, Type2>;

    const auto test = []() {
        TypePair *pair = hud::memory::allocate_array<TypePair>(1);
        hud::memory::construct_object_at(pair);
        const auto result = std::tuple {
            pair->first().move_assign_count(),
            pair->first().copy_assign_count(),
            pair->first().constructor_count(),
            pair->first().move_constructor_count(),
            pair->first().copy_constructor_count(),
            pair->first().id(),
            pair->second().move_assign_count(),
            pair->second().copy_assign_count(),
            pair->second().constructor_count(),
            pair->second().move_constructor_count(),
            pair->second().copy_constructor_count(),
            pair->second().id(),
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

GTEST_TEST(compressed_pair, copy_constructor_trivial_type_same_type)
{

    using Type1 = i32;
    using Type2 = f32;
    static_assert(hud::is_trivially_copy_constructible_v<Type1>);
    static_assert(hud::is_trivially_copy_constructible_v<Type2>);
    using TypePair = hud::compressed_pair<Type1, Type2>;

    const auto test = [](const Type1 &t1, const Type2 &t2) {
        TypePair *pair = hud::memory::allocate_array<TypePair>(1);
        const TypePair other {t1, t2};
        hud::memory::construct_object_at(pair, other);
        const auto result = std::tuple {
            pair->first(),
            pair->second()
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

GTEST_TEST(compressed_pair, copy_constructor_non_trivial_type_same_type)
{

    using Type1 = hud_test::non_bitwise_type;
    using Type2 = hud_test::non_bitwise_type;
    static_assert(!hud::is_trivially_copy_constructible_v<Type1>);
    static_assert(!hud::is_trivially_copy_constructible_v<Type2>);
    using TypePair = hud::compressed_pair<Type1, Type2>;

    const auto test = [](const i32 &id1, const i32 &id2) {
        TypePair *pair = hud::memory::allocate_array<TypePair>(1);
        const TypePair other {
            {id1, nullptr},
            {id2, nullptr}
        };
        hud::memory::construct_object_at(pair, other);
        const auto result = std::tuple {
            pair->first().move_assign_count(),
            pair->first().copy_assign_count(),
            pair->first().constructor_count(),
            pair->first().move_constructor_count(),
            pair->first().copy_constructor_count(),
            pair->first().id(),
            pair->second().move_assign_count(),
            pair->second().copy_assign_count(),
            pair->second().constructor_count(),
            pair->second().move_constructor_count(),
            pair->second().copy_constructor_count(),
            pair->second().id(),
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

GTEST_TEST(compressed_pair, copy_constructor_non_trivial_copy_constructible_type_same_type)
{

    using Type1 = hud_test::non_bitwise_copy_constructible_type;
    using Type2 = hud_test::non_bitwise_copy_constructible_type;
    static_assert(!hud::is_trivially_copy_constructible_v<Type1>);
    static_assert(!hud::is_trivially_copy_constructible_v<Type2>);
    using TypePair = hud::compressed_pair<Type1, Type2>;

    const auto test = [](const i32 &id1, const i32 &id2) {
        TypePair *pair = hud::memory::allocate_array<TypePair>(1);
        const TypePair other {id1, id2};
        hud::memory::construct_object_at(pair, other);
        const auto result = std::tuple {
            pair->first().copy_constructor_count(),
            pair->first().id(),
            pair->second().copy_constructor_count(),
            pair->second().id(),
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

GTEST_TEST(compressed_pair, copy_constructor_trivial_type_different_type)
{

    using Type1 = i64;
    using Type2 = i32;
    using OtherType1 = i16;
    using OtherType2 = i8;
    static_assert(hud::is_trivially_copy_constructible_v<Type1, OtherType1>);
    static_assert(hud::is_trivially_copy_constructible_v<Type2, OtherType2>);
    using TypePair = hud::compressed_pair<Type1, Type2>;
    using OtherTypePair = hud::compressed_pair<OtherType1, OtherType2>;

    const auto test = [](const OtherType1 &t1, const OtherType2 &t2) {
        TypePair *pair = hud::memory::allocate_array<TypePair>(1);
        const OtherTypePair other {t1, t2};
        hud::memory::construct_object_at(pair, other);
        const auto result = std::tuple {
            pair->first(),
            pair->second()
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

GTEST_TEST(compressed_pair, copy_constructor_non_trivial_type_different_type)
{

    using Type1 = hud_test::non_bitwise_copy_constructible_type2;
    using Type2 = hud_test::non_bitwise_copy_constructible_type2;
    using OtherType1 = hud_test::non_bitwise_copy_constructible_type;
    using OtherType2 = hud_test::non_bitwise_copy_constructible_type;
    static_assert(hud::is_copy_constructible_v<Type1, OtherType1>);
    static_assert(!hud::is_trivially_copy_constructible_v<Type1, OtherType1>);
    static_assert(hud::is_copy_constructible_v<Type2, OtherType2>);
    static_assert(!hud::is_trivially_copy_constructible_v<Type2, OtherType2>);
    using TypePair = hud::compressed_pair<Type1, Type2>;
    using OtherTypePair = hud::compressed_pair<OtherType1, OtherType2>;

    const auto test = [](const i32 &id1, const i32 &id2) {
        TypePair *pair = hud::memory::allocate_array<TypePair>(1);
        const OtherTypePair other {id1, id2};
        hud::memory::construct_object_at(pair, other);
        const auto result = std::tuple {
            pair->first().copy_constructor_count(),
            pair->first().id(),
            pair->second().copy_constructor_count(),
            pair->second().id(),
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

GTEST_TEST(compressed_pair, copy_constructor_non_trivial_copy_constructible_type_different_type)
{

    using Type1 = hud_test::non_bitwise_copy_constructible_type2;
    using Type2 = hud_test::non_bitwise_copy_constructible_type2;
    using OtherType1 = hud_test::non_bitwise_copy_constructible_type;
    using OtherType2 = hud_test::non_bitwise_copy_constructible_type;
    static_assert(hud::is_copy_constructible_v<Type1, OtherType1>);
    static_assert(!hud::is_trivially_copy_constructible_v<Type1, OtherType1>);
    static_assert(hud::is_copy_constructible_v<Type2, OtherType2>);
    static_assert(!hud::is_trivially_copy_constructible_v<Type2, OtherType2>);
    using TypePair = hud::compressed_pair<Type1, Type2>;
    using OtherTypePair = hud::compressed_pair<OtherType1, OtherType2>;

    const auto test = [](const i32 &id1, const i32 &id2) {
        TypePair *pair = hud::memory::allocate_array<TypePair>(1);
        const OtherTypePair other {id1, id2};
        hud::memory::construct_object_at(pair, other);
        const auto result = std::tuple {
            pair->first().copy_constructor_count(),
            pair->first().id(),
            pair->second().copy_constructor_count(),
            pair->second().id(),
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

GTEST_TEST(compressed_pair, move_constructor_trivial_type_same_type)
{

    using Type1 = i32;
    using Type2 = f32;
    static_assert(hud::is_trivially_copy_constructible_v<Type1>);
    static_assert(hud::is_trivially_copy_constructible_v<Type2>);
    using TypePair = hud::compressed_pair<Type1, Type2>;

    const auto test = [](const Type1 &t1, const Type2 &t2) {
        TypePair *pair = hud::memory::allocate_array<TypePair>(1);
        TypePair other {t1, t2};
        hud::memory::construct_object_at(pair, hud::move(other));
        const auto result = std::tuple {
            pair->first(),
            pair->second()
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

GTEST_TEST(compressed_pair, move_constructor_non_trivial_type_same_type)
{

    using Type1 = hud_test::non_bitwise_type;
    using Type2 = hud_test::non_bitwise_type;
    static_assert(!hud::is_trivially_copy_constructible_v<Type1>);
    static_assert(!hud::is_trivially_copy_constructible_v<Type2>);
    using TypePair = hud::compressed_pair<Type1, Type2>;

    const auto test = [](const i32 &id1, const i32 &id2) {
        TypePair *pair = hud::memory::allocate_array<TypePair>(1);
        TypePair other {
            {id1, nullptr},
            {id2, nullptr}
        };
        hud::memory::construct_object_at(pair, hud::move(other));
        const auto result = std::tuple {
            pair->first().move_assign_count(),
            pair->first().copy_assign_count(),
            pair->first().constructor_count(),
            pair->first().move_constructor_count(),
            pair->first().copy_constructor_count(),
            pair->first().id(),
            pair->second().move_assign_count(),
            pair->second().copy_assign_count(),
            pair->second().constructor_count(),
            pair->second().move_constructor_count(),
            pair->second().copy_constructor_count(),
            pair->second().id(),
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

GTEST_TEST(compressed_pair, move_constructor_non_trivial_copy_constructible_type_same_type)
{

    using Type1 = hud_test::non_bitwise_copy_constructible_type;
    using Type2 = hud_test::non_bitwise_copy_constructible_type;
    static_assert(!hud::is_trivially_copy_constructible_v<Type1>);
    static_assert(!hud::is_trivially_copy_constructible_v<Type2>);
    using TypePair = hud::compressed_pair<Type1, Type2>;

    const auto test = [](const i32 &id1, const i32 &id2) {
        TypePair *pair = hud::memory::allocate_array<TypePair>(1);
        TypePair other {id1, id2};
        hud::memory::construct_object_at(pair, hud::move(other));
        const auto result = std::tuple {
            pair->first().copy_constructor_count(),
            pair->first().id(),
            pair->second().copy_constructor_count(),
            pair->second().id(),
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

GTEST_TEST(compressed_pair, move_constructor_non_trivial_move_constructible_type_same_type)
{

    using Type1 = hud_test::non_bitwise_move_constructible_type;
    using Type2 = hud_test::non_bitwise_move_constructible_type;
    static_assert(!hud::is_trivially_move_constructible_v<Type1>);
    static_assert(!hud::is_trivially_move_constructible_v<Type2>);
    using TypePair = hud::compressed_pair<Type1, Type2>;

    const auto test = [](const i32 &id1, const i32 &id2) {
        TypePair *pair = hud::memory::allocate_array<TypePair>(1);
        TypePair other {id1, id2};
        hud::memory::construct_object_at(pair, hud::move(other));
        const auto result = std::tuple {
            pair->first().copy_constructor_count(),
            pair->first().move_constructor_count(),
            pair->first().id(),
            pair->second().copy_constructor_count(),
            pair->second().move_constructor_count(),
            pair->second().id(),
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

GTEST_TEST(compressed_pair, move_constructor_trivial_type_different_type)
{

    using Type1 = i64;
    using Type2 = i32;
    using OtherType1 = i16;
    using OtherType2 = i8;
    static_assert(hud::is_trivially_copy_constructible_v<Type1, OtherType1>);
    static_assert(hud::is_trivially_copy_constructible_v<Type2, OtherType2>);
    using TypePair = hud::compressed_pair<Type1, Type2>;
    using OtherTypePair = hud::compressed_pair<OtherType1, OtherType2>;

    const auto test = [](const OtherType1 &t1, const OtherType2 &t2) {
        TypePair *pair = hud::memory::allocate_array<TypePair>(1);
        OtherTypePair other {t1, t2};
        hud::memory::construct_object_at(pair, hud::move(other));
        const auto result = std::tuple {
            pair->first(),
            pair->second()
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

GTEST_TEST(compressed_pair, move_constructor_non_trivial_copy_constructible_type_different_type)
{

    using Type1 = hud_test::non_bitwise_copy_constructible_type2;
    using Type2 = hud_test::non_bitwise_copy_constructible_type2;
    using OtherType1 = hud_test::non_bitwise_copy_constructible_type;
    using OtherType2 = hud_test::non_bitwise_copy_constructible_type;
    static_assert(hud::is_copy_constructible_v<Type1, OtherType1>);
    static_assert(!hud::is_trivially_copy_constructible_v<Type1, OtherType1>);
    static_assert(hud::is_copy_constructible_v<Type2, OtherType2>);
    static_assert(!hud::is_trivially_copy_constructible_v<Type2, OtherType2>);
    using TypePair = hud::compressed_pair<Type1, Type2>;
    using OtherTypePair = hud::compressed_pair<OtherType1, OtherType2>;

    const auto test = [](const i32 &id1, const i32 &id2) {
        TypePair *pair = hud::memory::allocate_array<TypePair>(1);
        OtherTypePair other {id1, id2};
        hud::memory::construct_object_at(pair, hud::move(other));
        const auto result = std::tuple {
            pair->first().copy_constructor_count(),
            pair->first().id(),
            pair->second().copy_constructor_count(),
            pair->second().id(),
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

GTEST_TEST(compressed_pair, move_constructor_non_trivial_move_constructible_type_different_type)
{

    using Type1 = hud_test::non_bitwise_move_constructible_type2;
    using Type2 = hud_test::non_bitwise_move_constructible_type2;
    using OtherType1 = hud_test::non_bitwise_move_constructible_type;
    using OtherType2 = hud_test::non_bitwise_move_constructible_type;
    static_assert(hud::is_move_constructible_v<Type1, OtherType1>);
    static_assert(!hud::is_trivially_move_constructible_v<Type1, OtherType1>);
    static_assert(hud::is_move_constructible_v<Type2, OtherType2>);
    static_assert(!hud::is_trivially_move_constructible_v<Type2, OtherType2>);
    using TypePair = hud::compressed_pair<Type1, Type2>;
    using OtherTypePair = hud::compressed_pair<OtherType1, OtherType2>;

    const auto test = [](const i32 &id1, const i32 &id2) {
        TypePair *pair = hud::memory::allocate_array<TypePair>(1);
        OtherTypePair other {id1, id2};
        hud::memory::construct_object_at(pair, hud::move(other));
        const auto result = std::tuple {
            pair->first().copy_constructor_count(),
            pair->first().move_constructor_count(),
            pair->first().id(),
            pair->second().copy_constructor_count(),
            pair->second().move_constructor_count(),
            pair->second().id(),
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

GTEST_TEST(compressed_pair, param_copy_constructor_trivial_type_same_type)
{

    using Type1 = i32;
    using Type2 = f32;
    static_assert(hud::is_trivially_copy_constructible_v<Type1>);
    static_assert(hud::is_trivially_copy_constructible_v<Type2>);
    using TypePair = hud::compressed_pair<Type1, Type2>;

    const auto test = [](const Type1 &t1, const Type2 &t2) {
        TypePair *pair = hud::memory::allocate_array<TypePair>(1);
        hud::memory::construct_object_at(pair, t1, t2);
        const auto result = std::tuple {
            pair->first(),
            pair->second()
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

GTEST_TEST(compressed_pair, param_copy_constructor_trivial_type_different_type)
{

    using Type1 = i64;
    using Type2 = i32;
    using OtherType1 = i16;
    using OtherType2 = i8;
    static_assert(hud::is_trivially_copy_constructible_v<Type1, OtherType1>);
    static_assert(hud::is_trivially_copy_constructible_v<Type2, OtherType2>);
    using TypePair = hud::compressed_pair<Type1, Type2>;

    const auto test = [](const OtherType1 &t1, const OtherType2 &t2) {
        TypePair *pair = hud::memory::allocate_array<TypePair>(1);
        hud::memory::construct_object_at(pair, t1, t2);
        const auto result = std::tuple {
            pair->first(),
            pair->second()
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

GTEST_TEST(compressed_pair, param_copy_constructor_non_trivial_type_same_type)
{

    using Type1 = hud_test::non_bitwise_type;
    using Type2 = hud_test::non_bitwise_type;
    static_assert(!hud::is_trivially_copy_constructible_v<Type1>);
    static_assert(!hud::is_trivially_copy_constructible_v<Type2>);
    using TypePair = hud::compressed_pair<Type1, Type2>;

    const auto test = [](const i32 &id1, const i32 &id2) {
        TypePair *pair = hud::memory::allocate_array<TypePair>(1);
        const Type1 t1 {id1, nullptr};
        const Type2 t2 {id2, nullptr};
        hud::memory::construct_object_at(pair, t1, t2);
        const auto result = std::tuple {
            pair->first().move_assign_count(),
            pair->first().copy_assign_count(),
            pair->first().constructor_count(),
            pair->first().move_constructor_count(),
            pair->first().copy_constructor_count(),
            pair->first().id(),
            pair->second().move_assign_count(),
            pair->second().copy_assign_count(),
            pair->second().constructor_count(),
            pair->second().move_constructor_count(),
            pair->second().copy_constructor_count(),
            pair->second().id(),
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

GTEST_TEST(compressed_pair, param_copy_constructor_non_trivial_copy_constructible_type_same_type)
{

    using Type1 = hud_test::non_bitwise_copy_constructible_type;
    using Type2 = hud_test::non_bitwise_copy_constructible_type;
    static_assert(!hud::is_trivially_copy_constructible_v<Type1>);
    static_assert(!hud::is_trivially_copy_constructible_v<Type2>);
    using TypePair = hud::compressed_pair<Type1, Type2>;

    const auto test = [](const i32 &id1, const i32 &id2) {
        TypePair *pair = hud::memory::allocate_array<TypePair>(1);
        const Type1 t1 {id1};
        const Type2 t2 {id2};
        hud::memory::construct_object_at(pair, t1, t2);
        const auto result = std::tuple {
            pair->first().copy_constructor_count(),
            pair->first().id(),
            pair->second().copy_constructor_count(),
            pair->second().id(),
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

GTEST_TEST(compressed_pair, param_copy_constructor_non_trivial_type_different_type)
{

    using Type1 = hud_test::non_bitwise_copy_constructible_type2;
    using Type2 = hud_test::non_bitwise_copy_constructible_type2;
    using OtherType1 = hud_test::non_bitwise_copy_constructible_type;
    using OtherType2 = hud_test::non_bitwise_copy_constructible_type;
    static_assert(hud::is_copy_constructible_v<Type1, OtherType1>);
    static_assert(!hud::is_trivially_copy_constructible_v<Type1, OtherType1>);
    static_assert(hud::is_copy_constructible_v<Type2, OtherType2>);
    static_assert(!hud::is_trivially_copy_constructible_v<Type2, OtherType2>);
    using TypePair = hud::compressed_pair<Type1, Type2>;

    const auto test = [](const i32 &id1, const i32 &id2) {
        TypePair *pair = hud::memory::allocate_array<TypePair>(1);
        const OtherType1 t1 {id1};
        const OtherType2 t2 {id2};
        hud::memory::construct_object_at(pair, t1, t2);
        const auto result = std::tuple {
            pair->first().copy_constructor_count(),
            pair->first().id(),
            pair->second().copy_constructor_count(),
            pair->second().id(),
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

GTEST_TEST(compressed_pair, param_copy_constructor_non_trivial_copy_constructible_type_different_type)
{

    using Type1 = hud_test::non_bitwise_copy_constructible_type2;
    using Type2 = hud_test::non_bitwise_copy_constructible_type2;
    using OtherType1 = hud_test::non_bitwise_copy_constructible_type;
    using OtherType2 = hud_test::non_bitwise_copy_constructible_type;
    static_assert(hud::is_copy_constructible_v<Type1, OtherType1>);
    static_assert(!hud::is_trivially_copy_constructible_v<Type1, OtherType1>);
    static_assert(hud::is_copy_constructible_v<Type2, OtherType2>);
    static_assert(!hud::is_trivially_copy_constructible_v<Type2, OtherType2>);
    using TypePair = hud::compressed_pair<Type1, Type2>;

    const auto test = [](const i32 &id1, const i32 &id2) {
        TypePair *pair = hud::memory::allocate_array<TypePair>(1);
        const OtherType1 t1 {id1};
        const OtherType2 t2 {id2};
        hud::memory::construct_object_at(pair, t1, t2);
        const auto result = std::tuple {
            pair->first().copy_constructor_count(),
            pair->first().id(),
            pair->second().copy_constructor_count(),
            pair->second().id(),
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

GTEST_TEST(compressed_pair, param_move_constructor_trivial_type_same_type)
{

    using Type1 = i32;
    using Type2 = f32;
    static_assert(hud::is_trivially_copy_constructible_v<Type1>);
    static_assert(hud::is_trivially_copy_constructible_v<Type2>);
    using TypePair = hud::compressed_pair<Type1, Type2>;

    const auto test = [](Type1 &&t1, Type2 &&t2) {
        TypePair *pair = hud::memory::allocate_array<TypePair>(1);
        hud::memory::construct_object_at(pair, hud::forward<Type1>(t1), hud::forward<Type2>(t2));
        const auto result = std::tuple {
            pair->first(),
            pair->second()
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

GTEST_TEST(compressed_pair, param_move_constructor_non_trivial_type_same_type)
{

    using Type1 = hud_test::non_bitwise_type;
    using Type2 = hud_test::non_bitwise_type;
    static_assert(!hud::is_trivially_copy_constructible_v<Type1>);
    static_assert(!hud::is_trivially_copy_constructible_v<Type2>);
    using TypePair = hud::compressed_pair<Type1, Type2>;

    const auto test = [](const i32 &id1, const i32 &id2) {
        TypePair *pair = hud::memory::allocate_array<TypePair>(1);
        Type1 t1 {id1, nullptr};
        Type2 t2 {id2, nullptr};
        hud::memory::construct_object_at(pair, hud::move(t1), hud::move(t2));
        const auto result = std::tuple {
            pair->first().move_assign_count(),
            pair->first().copy_assign_count(),
            pair->first().constructor_count(),
            pair->first().move_constructor_count(),
            pair->first().copy_constructor_count(),
            pair->first().id(),
            pair->second().move_assign_count(),
            pair->second().copy_assign_count(),
            pair->second().constructor_count(),
            pair->second().move_constructor_count(),
            pair->second().copy_constructor_count(),
            pair->second().id(),
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

GTEST_TEST(compressed_pair, param_move_constructor_non_trivial_copy_constructible_type_same_type)
{
    using Type1 = hud_test::non_bitwise_copy_constructible_type;
    using Type2 = hud_test::non_bitwise_copy_constructible_type;
    static_assert(!hud::is_trivially_copy_constructible_v<Type1>);
    static_assert(!hud::is_trivially_copy_constructible_v<Type2>);
    static_assert(hud::is_same_v<Type1, Type2>);
    using TypePair = hud::compressed_pair<Type1, Type2>;
    const auto test = [](const i32 &id1, const i32 &id2) {
        TypePair *pair = hud::memory::allocate_array<TypePair>(1);
        Type1 t1 {id1};
        Type2 t2 {id2};
        hud::memory::construct_object_at(pair, hud::move(t1), hud::move(t2));
        const auto result = std::tuple {
            pair->first().copy_constructor_count(),
            pair->first().id(),
            pair->second().copy_constructor_count(),
            pair->second().id(),
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

GTEST_TEST(compressed_pair, param_move_constructor_non_trivial_move_constructible_type_same_type)
{

    using Type1 = hud_test::non_bitwise_move_constructible_type;
    using Type2 = hud_test::non_bitwise_move_constructible_type;
    static_assert(!hud::is_trivially_move_constructible_v<Type1>);
    static_assert(!hud::is_trivially_move_constructible_v<Type2>);
    using TypePair = hud::compressed_pair<Type1, Type2>;

    const auto test = [](const i32 &id1, const i32 &id2) {
        TypePair *pair = hud::memory::allocate_array<TypePair>(1);
        Type1 t1 {id1};
        Type2 t2 {id2};

        hud::memory::construct_object_at(pair, hud::move(t1), hud::move(t2));
        const auto result = std::tuple {
            pair->first().copy_constructor_count(),
            pair->first().move_constructor_count(),
            pair->first().id(),
            pair->second().copy_constructor_count(),
            pair->second().move_constructor_count(),
            pair->second().id(),
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

GTEST_TEST(compressed_pair, param_move_constructor_trivial_type_different_type)
{

    using Type1 = i64;
    using Type2 = i32;
    using OtherType1 = i16;
    using OtherType2 = i8;
    static_assert(hud::is_trivially_copy_constructible_v<Type1, OtherType1>);
    static_assert(hud::is_trivially_copy_constructible_v<Type2, OtherType2>);
    using TypePair = hud::compressed_pair<Type1, Type2>;

    const auto test = [](OtherType1 &&t1, OtherType2 &&t2) {
        TypePair *pair = hud::memory::allocate_array<TypePair>(1);
        hud::memory::construct_object_at(pair, hud::forward<OtherType1>(t1), hud::forward<OtherType2>(t2));
        const auto result = std::tuple {
            pair->first(),
            pair->second()
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

GTEST_TEST(compressed_pair, param_move_constructor_non_trivial_type_different_type)
{

    using Type1 = hud_test::non_bitwise_copy_constructible_type2;
    using Type2 = hud_test::non_bitwise_copy_constructible_type2;
    using OtherType1 = hud_test::non_bitwise_copy_constructible_type;
    using OtherType2 = hud_test::non_bitwise_copy_constructible_type;
    static_assert(hud::is_copy_constructible_v<Type1, OtherType1>);
    static_assert(!hud::is_trivially_copy_constructible_v<Type1, OtherType1>);
    static_assert(hud::is_copy_constructible_v<Type2, OtherType2>);
    static_assert(!hud::is_trivially_copy_constructible_v<Type2, OtherType2>);
    using TypePair = hud::compressed_pair<Type1, Type2>;

    const auto test = [](const i32 &id1, const i32 &id2) {
        TypePair *pair = hud::memory::allocate_array<TypePair>(1);
        OtherType1 t1 {id1};
        OtherType2 t2 {id2};
        hud::memory::construct_object_at(pair, hud::move(t1), hud::move(t2));
        const auto result = std::tuple {
            pair->first().copy_constructor_count(),
            pair->first().id(),
            pair->second().copy_constructor_count(),
            pair->second().id(),
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

GTEST_TEST(compressed_pair, param_move_constructor_non_trivial_copy_constructible_type_different_type)
{

    using Type1 = hud_test::non_bitwise_copy_constructible_type2;
    using Type2 = hud_test::non_bitwise_copy_constructible_type2;
    using OtherType1 = hud_test::non_bitwise_copy_constructible_type;
    using OtherType2 = hud_test::non_bitwise_copy_constructible_type;
    static_assert(hud::is_copy_constructible_v<Type1, OtherType1>);
    static_assert(!hud::is_trivially_copy_constructible_v<Type1, OtherType1>);
    static_assert(hud::is_copy_constructible_v<Type2, OtherType2>);
    static_assert(!hud::is_trivially_copy_constructible_v<Type2, OtherType2>);
    using TypePair = hud::compressed_pair<Type1, Type2>;

    const auto test = [](const i32 &id1, const i32 &id2) {
        TypePair *pair = hud::memory::allocate_array<TypePair>(1);
        OtherType1 t1 {id1};
        OtherType2 t2 {id2};
        hud::memory::construct_object_at(pair, hud::move(t1), hud::move(t2));
        const auto result = std::tuple {
            pair->first().copy_constructor_count(),
            pair->first().id(),
            pair->second().copy_constructor_count(),
            pair->second().id(),
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

GTEST_TEST(compressed_pair, param_move_constructor_non_trivial_move_constructible_type_different_type)
{

    using Type1 = hud_test::non_bitwise_move_constructible_type2;
    using Type2 = hud_test::non_bitwise_move_constructible_type2;
    using OtherType1 = hud_test::non_bitwise_move_constructible_type;
    using OtherType2 = hud_test::non_bitwise_move_constructible_type;
    static_assert(hud::is_move_constructible_v<Type1, OtherType1>);
    static_assert(!hud::is_trivially_move_constructible_v<Type1, OtherType1>);
    static_assert(hud::is_move_constructible_v<Type2, OtherType2>);
    static_assert(!hud::is_trivially_move_constructible_v<Type2, OtherType2>);
    using TypePair = hud::compressed_pair<Type1, Type2>;

    const auto test = [](const i32 &id1, const i32 &id2) {
        TypePair *pair = hud::memory::allocate_array<TypePair>(1);
        OtherType1 t1 {id1};
        OtherType2 t2 {id2};

        hud::memory::construct_object_at(pair, hud::move(t1), hud::move(t2));
        const auto result = std::tuple {
            pair->first().copy_constructor_count(),
            pair->first().move_constructor_count(),
            pair->first().id(),
            pair->second().copy_constructor_count(),
            pair->second().move_constructor_count(),
            pair->second().id(),
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