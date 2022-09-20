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

struct Implicit {
    Implicit() noexcept = default;
    Implicit(wchar value) noexcept
        : mInner(value) {
    }
    Implicit(const Implicit& other) noexcept
        : mInner(other.mInner) {
    };
    Implicit(Implicit&& other) noexcept
        : mInner(other.mInner + 1) {
    };
    wchar mInner = L'\0';
};
static_assert(hud::is_implicitly_default_constructible_v<Implicit>);
static_assert(!hud::is_explicitly_default_constructible_v<Implicit>);
static_assert(hud::is_implicitly_constructible_v<Implicit, wchar>);
static_assert(!hud::is_explicitly_constructible_v<Implicit, wchar>);
static_assert(hud::is_implicitly_constructible_v<Implicit, const Implicit&>);
static_assert(!hud::is_explicitly_constructible_v<Implicit, const Implicit&>);
static_assert(hud::is_implicitly_constructible_v<Implicit, Implicit&&>);
static_assert(!hud::is_explicitly_constructible_v<Implicit, Implicit&&>);
static_assert(hud::is_convertible_v<const Implicit&, Implicit>);

struct Explicit {
    explicit Explicit() noexcept = default;
    explicit Explicit(i32 value) noexcept
        : mInner(value) {
    }
    explicit Explicit(const Explicit& other) noexcept
        : mInner(other.mInner) {
    };

    explicit Explicit(Explicit&& other) noexcept
        : mInner(other.mInner + 1) {
    }; 
    operator Implicit() const noexcept { return Implicit(static_cast<wchar>(mInner)); }
    i32 mInner = 0;
};
static_assert(!hud::is_implicitly_default_constructible_v<Explicit>);
static_assert(hud::is_explicitly_default_constructible_v<Explicit>);
static_assert(!hud::is_implicitly_constructible_v<Explicit, i32>);
static_assert(hud::is_explicitly_constructible_v<Explicit, i32>);

static_assert(!hud::is_implicitly_constructible_v<Explicit, const Explicit&>);
static_assert(hud::is_explicitly_constructible_v<Explicit, const Explicit&>);
static_assert(!hud::is_implicitly_constructible_v<Explicit, Explicit&&>);
static_assert(hud::is_explicitly_constructible_v<Explicit, Explicit&&>);
static_assert(!hud::is_convertible_v<const Explicit&, Explicit>);

TEST(Pair, default_constructor_is_explicit_if_T1_or_T2_is_not_implicitly_default_constructible) {


    static_assert(hud::is_implicitly_default_constructible_v<Implicit>);
    static_assert(hud::is_implicitly_default_constructible_v<Pair<i32, i32>>);
    static_assert(hud::is_implicitly_default_constructible_v<Pair<i32, f32>>);
    static_assert(hud::is_implicitly_default_constructible_v<Pair<i32,Implicit>>);
    static_assert(hud::is_implicitly_default_constructible_v<Pair<Implicit, Implicit>>);
    static_assert(hud::is_explicitly_default_constructible_v<Pair<i32, Explicit>>);
    static_assert(hud::is_explicitly_default_constructible_v<Pair<i32, Explicit>>);
    static_assert(hud::is_explicitly_default_constructible_v<Pair<Explicit, Explicit>>);
}


TEST(Pair, param_copy_constructor_is_explicit_if_T1_or_T2_is_not_implicitly_convertible) {


    static_assert(hud::is_convertible_v<const Explicit&, Implicit>);
    static_assert(hud::is_convertible_v<const Implicit&, Implicit>);
    static_assert(hud::is_implicitly_constructible_v<Pair<Implicit, Implicit>, const Implicit&, const Implicit&>);
    static_assert(hud::is_implicitly_constructible_v<Pair<Implicit, Implicit>, const Implicit&, wchar>);
    static_assert(hud::is_explicitly_constructible_v<Pair<Explicit, Implicit>, const Explicit&, const Implicit&>);
    static_assert(hud::is_explicitly_constructible_v<Pair<Explicit, Implicit>, i32, const Implicit&>);
}

TEST(Pair, param_move_constructor_is_explicit_if_T1_or_T2_is_not_implicitly_converible) {


    static_assert(hud::is_convertible_v<const Explicit&, Implicit>);
    static_assert(hud::is_convertible_v<const Implicit&, Implicit>);
    static_assert(hud::is_implicitly_constructible_v<Pair<Implicit, Implicit>, Implicit&&, Implicit&&>);
    static_assert(hud::is_implicitly_constructible_v<Pair<Implicit, Implicit>, Implicit&&, wchar>);
    static_assert(hud::is_explicitly_constructible_v<Pair<Explicit, Implicit>, Explicit&&, Implicit&&>);
    static_assert(hud::is_explicitly_constructible_v<Pair<Explicit, Implicit>, i32, Implicit&&>);
}

TEST(Pair, copy_constructor_is_explicit_if_T1_or_T2_is_not_implicitly_convertible) {


    static_assert(hud::is_convertible_v<const Explicit&, Implicit>);
    static_assert(hud::is_convertible_v<const Implicit&, Implicit>);
    static_assert(hud::is_copy_constructible_v<Explicit, Explicit>);
    static_assert(hud::is_copy_constructible_v<Implicit, Implicit>);

    // Same
    static_assert(hud::is_implicitly_copy_constructible_v < Pair<Implicit, Implicit>, Pair<Implicit, Implicit>>);
    static_assert(hud::is_explicitly_copy_constructible_v < Pair<Explicit, Implicit>, Pair<Explicit, Implicit>> );
    
    // Not same type_t
    static_assert(hud::is_implicitly_copy_constructible_v < Pair<Implicit, Implicit>, Pair<Implicit, wchar>>);
    static_assert(hud::is_explicitly_copy_constructible_v < Pair<Explicit, Implicit>, Pair <i32, Implicit>>);
    static_assert(hud::is_implicitly_copy_constructible_v < Pair<Implicit, Implicit>, Pair<i32, wchar>>);
}

TEST(Pair, move_constructor_is_explicit_if_T1_or_T2_is_not_implicitly_convertible) {


    static_assert(hud::is_convertible_v<Explicit, Implicit>);
    static_assert(hud::is_convertible_v<Implicit, Implicit>);
    static_assert(hud::is_move_constructible_v<Explicit, Explicit>);
    static_assert(hud::is_move_constructible_v<Implicit, Implicit>);

    // Same
    static_assert(hud::is_implicitly_move_constructible_v < Pair<Implicit, Implicit>, Pair<Implicit, Implicit>>);
    static_assert(hud::is_explicitly_move_constructible_v < Pair<Explicit, Implicit>, Pair<Explicit, Implicit>>);
    static_assert(hud::is_explicitly_move_constructible_v < Pair<Explicit, Implicit>, Pair <i32, Implicit>>);

    // Not same type_t
    static_assert(hud::is_implicitly_move_constructible_v < Pair<Implicit, Implicit>, Pair<Implicit, wchar>>);
    static_assert(hud::is_explicitly_move_constructible_v < Pair<Explicit, Implicit>, Pair <i32, Implicit>>);
    static_assert(hud::is_explicitly_move_constructible_v < Pair<Explicit, Implicit>, Pair <i32, wchar>>);
}


TEST(Pair, default_constructor_trivial) {

    using Type1 = i32;
    using Type2 = f32;
    static_assert(hud::is_trivially_default_constructible_v<Type1>);
    static_assert(hud::is_trivially_default_constructible_v<Type2>);
    using TypePair = Pair<Type1, Type2>;

    const auto test = []() {
        TypePair* pair = hud::memory::allocate_array<TypePair>(1);
        hud::memory::construct_at(pair);
        const auto result = std::tuple{ pair->first, pair->second };
        hud::memory::free_array(pair, 1);
        return result;
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_EQ(hud::get<0>(result), 0);
        ASSERT_EQ(hud::get<1>(result), 0.0f);
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_EQ(hud::get<0>(result), 0);
        ASSERT_EQ(hud::get<1>(result), 0.0f);
    }
}

TEST(Pair, default_constructor_non_trivial) {

    using Type1 = hud::test::NonBitwiseType;
    using Type2 = hud::test::NonBitwiseType;
    static_assert(!hud::is_trivially_default_constructible_v<Type1>);
    static_assert(!hud::is_trivially_default_constructible_v<Type2>);
    using TypePair = Pair<Type1, Type2>;

    const auto test = []() {
        TypePair* pair = hud::memory::allocate_array<TypePair>(1);
        hud::memory::construct_at(pair);
        const auto result = std::tuple{ 
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
        ASSERT_EQ(hud::get<0>(result), 0u);
        ASSERT_EQ(hud::get<1>(result), 0u);
        ASSERT_EQ(hud::get<2>(result), 0u);
        ASSERT_EQ(hud::get<3>(result), 0u);
        ASSERT_EQ(hud::get<4>(result), 0u);
        ASSERT_EQ(hud::get<5>(result), 0);

        ASSERT_EQ(hud::get<6>(result), 0u);
        ASSERT_EQ(hud::get<7>(result), 0u);
        ASSERT_EQ(hud::get<8>(result), 0u);
        ASSERT_EQ(hud::get<9>(result), 0u);
        ASSERT_EQ(hud::get<10>(result), 0u);
        ASSERT_EQ(hud::get<11>(result), 0);
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_EQ(hud::get<0>(result), 0u);
        ASSERT_EQ(hud::get<1>(result), 0u);
        ASSERT_EQ(hud::get<2>(result), 0u);
        ASSERT_EQ(hud::get<3>(result), 0u);
        ASSERT_EQ(hud::get<4>(result), 0u);
        ASSERT_EQ(hud::get<5>(result), 0);

        ASSERT_EQ(hud::get<6>(result), 0u);
        ASSERT_EQ(hud::get<7>(result), 0u);
        ASSERT_EQ(hud::get<8>(result), 0u);
        ASSERT_EQ(hud::get<9>(result), 0u);
        ASSERT_EQ(hud::get<10>(result), 0u);
        ASSERT_EQ(hud::get<11>(result), 0);
    }
}

TEST(Pair, copy_constructor_trivial_type_same_type) {

    using Type1 = i32;
    using Type2 = f32;
    static_assert(hud::is_trivially_copy_constructible_v<Type1>);
    static_assert(hud::is_trivially_copy_constructible_v<Type2>);
    using TypePair = Pair<Type1, Type2>;

    const auto test = [](const Type1& t1, const Type2& t2) {
        TypePair* pair = hud::memory::allocate_array<TypePair>(1);
        const TypePair other{ t1, t2 };
        hud::memory::construct_at(pair, other);
        const auto result = std::tuple{
            pair->first,
            pair->second
        };
        hud::memory::free_array(pair, 1);
        return result;
    };

    // Non constant
    {
        const auto result = test(123,456.0f);
        ASSERT_EQ(hud::get<0>(result), 123);
        ASSERT_EQ(hud::get<1>(result), 456.0f);
    }

    // Constant
    {
        constexpr auto result = test(123, 456.0f);
        ASSERT_EQ(hud::get<0>(result), 123);
        ASSERT_EQ(hud::get<1>(result), 456.0f);
    }
}

TEST(Pair, copy_constructor_non_trivial_type_same_type) {

    using Type1 = hud::test::NonBitwiseType;
    using Type2 = hud::test::NonBitwiseType;
    static_assert(!hud::is_trivially_copy_constructible_v<Type1>);
    static_assert(!hud::is_trivially_copy_constructible_v<Type2>);
    using TypePair = Pair<Type1, Type2>;

    const auto test = [](const i32& id1, const i32& id2) {
        TypePair* pair = hud::memory::allocate_array<TypePair>(1);
        const TypePair other{ {id1, nullptr}, {id2, nullptr } };
        hud::memory::construct_at(pair, other);
        const auto result = std::tuple{
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
        const auto result = test(123,456);
        ASSERT_EQ(hud::get<0>(result), 0u);
        ASSERT_EQ(hud::get<1>(result), 0u);
        ASSERT_EQ(hud::get<2>(result), 1u);
        ASSERT_EQ(hud::get<3>(result), 1u);
        ASSERT_EQ(hud::get<4>(result), 1u);
        ASSERT_EQ(hud::get<5>(result), 123);

        ASSERT_EQ(hud::get<6>(result), 0u);
        ASSERT_EQ(hud::get<7>(result), 0u);
        ASSERT_EQ(hud::get<8>(result), 1u);
        ASSERT_EQ(hud::get<9>(result), 1u);
        ASSERT_EQ(hud::get<10>(result), 1u);
        ASSERT_EQ(hud::get<11>(result), 456);
    }

    // Constant
    {
        constexpr auto result = test(123,456);
        ASSERT_EQ(hud::get<0>(result), 0u);
        ASSERT_EQ(hud::get<1>(result), 0u);
        ASSERT_EQ(hud::get<2>(result), 1u);
        ASSERT_EQ(hud::get<3>(result), 1u);
        ASSERT_EQ(hud::get<4>(result), 1u);
        ASSERT_EQ(hud::get<5>(result), 123);

        ASSERT_EQ(hud::get<6>(result), 0u);
        ASSERT_EQ(hud::get<7>(result), 0u);
        ASSERT_EQ(hud::get<8>(result), 1u);
        ASSERT_EQ(hud::get<9>(result), 1u);
        ASSERT_EQ(hud::get<10>(result), 1u);
        ASSERT_EQ(hud::get<11>(result), 456);
    }
}

TEST(Pair, copy_constructor_non_trivial_copy_constructible_type_same_type) {

    using Type1 = hud::test::NonBitwiseCopyConstructibleType;
    using Type2 = hud::test::NonBitwiseCopyConstructibleType;
    static_assert(!hud::is_trivially_copy_constructible_v<Type1>);
    static_assert(!hud::is_trivially_copy_constructible_v<Type2>);
    using TypePair = Pair<Type1, Type2>;

    const auto test = [](const i32& id1, const i32& id2) {
        TypePair* pair = hud::memory::allocate_array<TypePair>(1);
        const TypePair other{ id1,  id2 };
        hud::memory::construct_at(pair, other);
        const auto result = std::tuple{
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
        ASSERT_EQ(hud::get<0>(result), 1u);
        ASSERT_EQ(hud::get<1>(result), 123);
        ASSERT_EQ(hud::get<2>(result), 1u);
        ASSERT_EQ(hud::get<3>(result), 456);
    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        ASSERT_EQ(hud::get<0>(result), 1u);
        ASSERT_EQ(hud::get<1>(result), 123);
        ASSERT_EQ(hud::get<2>(result), 1u);
        ASSERT_EQ(hud::get<3>(result), 456);
    }
}

TEST(Pair, copy_constructor_trivial_type_different_type) {

    using Type1 = i64;
    using Type2 = i32;
    using OtherType1 = i16;
    using OtherType2 = i8;
    static_assert(hud::is_trivially_copy_constructible_v<Type1, OtherType1>);
    static_assert(hud::is_trivially_copy_constructible_v<Type2, OtherType2>);
    using TypePair = Pair<Type1, Type2>;
    using OtherTypePair = Pair<OtherType1, OtherType2>;

    const auto test = [](const OtherType1& t1, const OtherType2& t2) {
        TypePair* pair = hud::memory::allocate_array<TypePair>(1);
        const OtherTypePair other{ t1, t2 };
        hud::memory::construct_at(pair, other);
        const auto result = std::tuple{
            pair->first,
            pair->second
        };
        hud::memory::free_array(pair, 1);
        return result;
    };

    // Non constant
    {
        const auto result = test(123, 32);
        ASSERT_EQ(hud::get<0>(result), 123);
        ASSERT_EQ(hud::get<1>(result), 32);
    }

    // Constant
    {
        constexpr auto result = test(123, 32);
        ASSERT_EQ(hud::get<0>(result), 123);
        ASSERT_EQ(hud::get<1>(result), 32);
    }
}

TEST(Pair, copy_constructor_non_trivial_type_different_type) {

    using Type1 = hud::test::NonBitwiseCopyConstructibleType2;
    using Type2 = hud::test::NonBitwiseCopyConstructibleType2;
    using OtherType1 = hud::test::NonBitwiseCopyConstructibleType;
    using OtherType2 = hud::test::NonBitwiseCopyConstructibleType;
    static_assert(hud::is_copy_constructible_v<Type1, OtherType1>);
    static_assert(!hud::is_trivially_copy_constructible_v<Type1, OtherType1>);
    static_assert(hud::is_copy_constructible_v<Type2, OtherType2>);
    static_assert(!hud::is_trivially_copy_constructible_v<Type2, OtherType2>);
    using TypePair = Pair<Type1, Type2>;
    using OtherTypePair = Pair<OtherType1, OtherType2>;

    const auto test = [](const i32& id1, const i32& id2) {
        TypePair* pair = hud::memory::allocate_array<TypePair>(1);
        const OtherTypePair other{ id1, id2 };
        hud::memory::construct_at(pair, other);
        const auto result = std::tuple{
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
        ASSERT_EQ(hud::get<0>(result), 1u);
        ASSERT_EQ(hud::get<1>(result), 123);
        ASSERT_EQ(hud::get<2>(result), 1u);
        ASSERT_EQ(hud::get<3>(result), 456);
    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        ASSERT_EQ(hud::get<0>(result), 1u);
        ASSERT_EQ(hud::get<1>(result), 123);
        ASSERT_EQ(hud::get<2>(result), 1u);
        ASSERT_EQ(hud::get<3>(result), 456);
    }
}
           
TEST(Pair, copy_constructor_non_trivial_copy_constructible_type_different_type) {

    using Type1 = hud::test::NonBitwiseCopyConstructibleType2;
    using Type2 = hud::test::NonBitwiseCopyConstructibleType2;
    using OtherType1 = hud::test::NonBitwiseCopyConstructibleType;
    using OtherType2 = hud::test::NonBitwiseCopyConstructibleType;
    static_assert(hud::is_copy_constructible_v<Type1, OtherType1>);
    static_assert(!hud::is_trivially_copy_constructible_v<Type1, OtherType1>);
    static_assert(hud::is_copy_constructible_v<Type2, OtherType2>);
    static_assert(!hud::is_trivially_copy_constructible_v<Type2, OtherType2>);
    using TypePair = Pair<Type1, Type2>;
    using OtherTypePair = Pair<OtherType1, OtherType2>;

    const auto test = [](const i32& id1, const i32& id2) {
        TypePair* pair = hud::memory::allocate_array<TypePair>(1);
        const OtherTypePair other{ id1, id2 };
        hud::memory::construct_at(pair, other);
        const auto result = std::tuple{
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
        ASSERT_EQ(hud::get<0>(result), 1u);
        ASSERT_EQ(hud::get<1>(result), 123);
        ASSERT_EQ(hud::get<2>(result), 1u);
        ASSERT_EQ(hud::get<3>(result), 456);
    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        ASSERT_EQ(hud::get<0>(result), 1u);
        ASSERT_EQ(hud::get<1>(result), 123);
        ASSERT_EQ(hud::get<2>(result), 1u);
        ASSERT_EQ(hud::get<3>(result), 456);
    }
}

TEST(Pair, move_constructor_trivial_type_same_type) {

    using Type1 = i32;
    using Type2 = f32;
    static_assert(hud::is_trivially_copy_constructible_v<Type1>);
    static_assert(hud::is_trivially_copy_constructible_v<Type2>);
    using TypePair = Pair<Type1, Type2>;

    const auto test = [](const Type1& t1, const Type2& t2) {
        TypePair* pair = hud::memory::allocate_array<TypePair>(1);
        TypePair other{ t1, t2 };
        hud::memory::construct_at(pair, hud::move(other));
        const auto result = std::tuple{
            pair->first,
            pair->second
        };
        hud::memory::free_array(pair, 1);
        return result;
    };

    // Non constant
    {
        const auto result = test(123, 456.0f);
        ASSERT_EQ(hud::get<0>(result), 123);
        ASSERT_EQ(hud::get<1>(result), 456.0f);
    }

    // Constant
    {
        constexpr auto result = test(123, 456.0f);
        ASSERT_EQ(hud::get<0>(result), 123);
        ASSERT_EQ(hud::get<1>(result), 456.0f);
    }
}

TEST(Pair, move_constructor_non_trivial_type_same_type) {

    using Type1 = hud::test::NonBitwiseType;
    using Type2 = hud::test::NonBitwiseType;
    static_assert(!hud::is_trivially_copy_constructible_v<Type1>);
    static_assert(!hud::is_trivially_copy_constructible_v<Type2>);
    using TypePair = Pair<Type1, Type2>;

    const auto test = [](const i32& id1, const i32& id2) {
        TypePair* pair = hud::memory::allocate_array<TypePair>(1);
        TypePair other{ {id1, nullptr}, {id2, nullptr } };
        hud::memory::construct_at(pair, hud::move(other));
        const auto result = std::tuple{
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
        ASSERT_EQ(hud::get<0>(result), 0u);
        ASSERT_EQ(hud::get<1>(result), 0u);
        ASSERT_EQ(hud::get<2>(result), 1u);
        ASSERT_EQ(hud::get<3>(result), 2u);
        ASSERT_EQ(hud::get<4>(result), 0u);
        ASSERT_EQ(hud::get<5>(result), 123);

        ASSERT_EQ(hud::get<6>(result), 0u);
        ASSERT_EQ(hud::get<7>(result), 0u);
        ASSERT_EQ(hud::get<8>(result), 1u);
        ASSERT_EQ(hud::get<9>(result), 2u);
        ASSERT_EQ(hud::get<10>(result), 0u);
        ASSERT_EQ(hud::get<11>(result), 456);
    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        ASSERT_EQ(hud::get<0>(result), 0u);
        ASSERT_EQ(hud::get<1>(result), 0u);
        ASSERT_EQ(hud::get<2>(result), 1u);
        ASSERT_EQ(hud::get<3>(result), 2u);
        ASSERT_EQ(hud::get<4>(result), 0u);
        ASSERT_EQ(hud::get<5>(result), 123);

        ASSERT_EQ(hud::get<6>(result), 0u);
        ASSERT_EQ(hud::get<7>(result), 0u);
        ASSERT_EQ(hud::get<8>(result), 1u);
        ASSERT_EQ(hud::get<9>(result), 2u);
        ASSERT_EQ(hud::get<10>(result), 0u);
        ASSERT_EQ(hud::get<11>(result), 456);
    }
}

TEST(Pair, move_constructor_non_trivial_copy_constructible_type_same_type) {

    using Type1 = hud::test::NonBitwiseCopyConstructibleType;
    using Type2 = hud::test::NonBitwiseCopyConstructibleType;
    static_assert(!hud::is_trivially_copy_constructible_v<Type1>);
    static_assert(!hud::is_trivially_copy_constructible_v<Type2>);
    using TypePair = Pair<Type1, Type2>;

    const auto test = [](const i32& id1, const i32& id2) {
        TypePair* pair = hud::memory::allocate_array<TypePair>(1);
        TypePair other{ id1,  id2 };
        hud::memory::construct_at(pair, hud::move(other));
        const auto result = std::tuple{
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
        ASSERT_EQ(hud::get<0>(result), 1u);
        ASSERT_EQ(hud::get<1>(result), 123);
        ASSERT_EQ(hud::get<2>(result), 1u);
        ASSERT_EQ(hud::get<3>(result), 456);
    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        ASSERT_EQ(hud::get<0>(result), 1u);
        ASSERT_EQ(hud::get<1>(result), 123);
        ASSERT_EQ(hud::get<2>(result), 1u);
        ASSERT_EQ(hud::get<3>(result), 456);
    }
}

TEST(Pair, move_constructor_non_trivial_move_constructible_type_same_type) {

    using Type1 = hud::test::NonBitwiseMoveConstructibleType;
    using Type2 = hud::test::NonBitwiseMoveConstructibleType;
    static_assert(!hud::is_trivially_move_constructible_v<Type1>);
    static_assert(!hud::is_trivially_move_constructible_v<Type2>);
    using TypePair = Pair<Type1, Type2>;

    const auto test = [](const i32& id1, const i32& id2) {
        TypePair* pair = hud::memory::allocate_array<TypePair>(1);
        TypePair other{ id1,  id2 };
        hud::memory::construct_at(pair, hud::move(other));
        const auto result = std::tuple{
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
        ASSERT_EQ(hud::get<0>(result), 0u);
        ASSERT_EQ(hud::get<1>(result), 1u);
        ASSERT_EQ(hud::get<2>(result), 123);
        ASSERT_EQ(hud::get<3>(result), 0u);
        ASSERT_EQ(hud::get<4>(result), 1u);
        ASSERT_EQ(hud::get<5>(result), 456);
    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        ASSERT_EQ(hud::get<0>(result), 0u);
        ASSERT_EQ(hud::get<1>(result), 1u);
        ASSERT_EQ(hud::get<2>(result), 123);
        ASSERT_EQ(hud::get<3>(result), 0u);
        ASSERT_EQ(hud::get<4>(result), 1u);
        ASSERT_EQ(hud::get<5>(result), 456);
    }
}

TEST(Pair, move_constructor_trivial_type_different_type) {

    using Type1 = i64;
    using Type2 = i32;
    using OtherType1 = i16;
    using OtherType2 = i8;
    static_assert(hud::is_trivially_copy_constructible_v<Type1, OtherType1>);
    static_assert(hud::is_trivially_copy_constructible_v<Type2, OtherType2>);
    using TypePair = Pair<Type1, Type2>;
    using OtherTypePair = Pair<OtherType1, OtherType2>;

    const auto test = [](const OtherType1& t1, const OtherType2& t2) {
        TypePair* pair = hud::memory::allocate_array<TypePair>(1);
        OtherTypePair other{ t1, t2 };
        hud::memory::construct_at(pair, hud::move(other));
        const auto result = std::tuple{
            pair->first,
            pair->second
        };
        hud::memory::free_array(pair, 1);
        return result;
    };

    // Non constant
    {
        const auto result = test(123, 32);
        ASSERT_EQ(hud::get<0>(result), 123);
        ASSERT_EQ(hud::get<1>(result), 32);
    }

    // Constant
    {
        constexpr auto result = test(123, 32);
        ASSERT_EQ(hud::get<0>(result), 123);
        ASSERT_EQ(hud::get<1>(result), 32);
    }
}

TEST(Pair, move_constructor_non_trivial_copy_constructible_type_different_type) {

    using Type1 = hud::test::NonBitwiseCopyConstructibleType2;
    using Type2 = hud::test::NonBitwiseCopyConstructibleType2;
    using OtherType1 = hud::test::NonBitwiseCopyConstructibleType;
    using OtherType2 = hud::test::NonBitwiseCopyConstructibleType;
    static_assert(hud::is_copy_constructible_v<Type1, OtherType1>);
    static_assert(!hud::is_trivially_copy_constructible_v<Type1, OtherType1>);
    static_assert(hud::is_copy_constructible_v<Type2, OtherType2>);
    static_assert(!hud::is_trivially_copy_constructible_v<Type2, OtherType2>);
    using TypePair = Pair<Type1, Type2>;
    using OtherTypePair = Pair<OtherType1, OtherType2>;

    const auto test = [](const i32& id1, const i32& id2) {
        TypePair* pair = hud::memory::allocate_array<TypePair>(1);
        OtherTypePair other{ id1, id2 };
        hud::memory::construct_at(pair, hud::move(other));
        const auto result = std::tuple{
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
        ASSERT_EQ(hud::get<0>(result), 1u);
        ASSERT_EQ(hud::get<1>(result), 123);
        ASSERT_EQ(hud::get<2>(result), 1u);
        ASSERT_EQ(hud::get<3>(result), 456);
    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        ASSERT_EQ(hud::get<0>(result), 1u);
        ASSERT_EQ(hud::get<1>(result), 123);
        ASSERT_EQ(hud::get<2>(result), 1u);
        ASSERT_EQ(hud::get<3>(result), 456);
    }
}

TEST(Pair, move_constructor_non_trivial_move_constructible_type_different_type) {

    using Type1 = hud::test::NonBitwiseMoveConstructibleType2;
    using Type2 = hud::test::NonBitwiseMoveConstructibleType2;
    using OtherType1 = hud::test::NonBitwiseMoveConstructibleType;
    using OtherType2 = hud::test::NonBitwiseMoveConstructibleType;
    static_assert(hud::is_move_constructible_v<Type1, OtherType1>);
    static_assert(!hud::is_trivially_move_constructible_v<Type1, OtherType1>);
    static_assert(hud::is_move_constructible_v<Type2, OtherType2>);
    static_assert(!hud::is_trivially_move_constructible_v<Type2, OtherType2>);
    using TypePair = Pair<Type1, Type2>;
    using OtherTypePair = Pair<OtherType1, OtherType2>;

    const auto test = [](const i32& id1, const i32& id2) {
        TypePair* pair = hud::memory::allocate_array<TypePair>(1);
        OtherTypePair other{ id1, id2 };
        hud::memory::construct_at(pair, hud::move(other));
        const auto result = std::tuple{
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
        ASSERT_EQ(hud::get<0>(result), 0u);
        ASSERT_EQ(hud::get<1>(result), 1u);
        ASSERT_EQ(hud::get<2>(result), 123);
        ASSERT_EQ(hud::get<3>(result), 0u);
        ASSERT_EQ(hud::get<4>(result), 1u);
        ASSERT_EQ(hud::get<5>(result), 456);
    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        ASSERT_EQ(hud::get<0>(result), 0u);
        ASSERT_EQ(hud::get<1>(result), 1u);
        ASSERT_EQ(hud::get<2>(result), 123);
        ASSERT_EQ(hud::get<3>(result), 0u);
        ASSERT_EQ(hud::get<4>(result), 1u);
        ASSERT_EQ(hud::get<5>(result), 456);
    }
}

TEST(Pair, param_copy_constructor_trivial_type_same_type) {

    using Type1 = i32;
    using Type2 = f32;
    static_assert(hud::is_trivially_copy_constructible_v<Type1>);
    static_assert(hud::is_trivially_copy_constructible_v<Type2>);
    using TypePair = Pair<Type1, Type2>;

    const auto test = [](const Type1& t1, const Type2& t2) {
        TypePair* pair = hud::memory::allocate_array<TypePair>(1);
        hud::memory::construct_at(pair, t1, t2);
        const auto result = std::tuple{
            pair->first,
            pair->second
        };
        hud::memory::free_array(pair, 1);
        return result;
    };

    // Non constant
    {
        const auto result = test(123, 456.0f);
        ASSERT_EQ(hud::get<0>(result), 123);
        ASSERT_EQ(hud::get<1>(result), 456.0f);
    }

    // Constant
    {
        constexpr auto result = test(123, 456.0f);
        ASSERT_EQ(hud::get<0>(result), 123);
        ASSERT_EQ(hud::get<1>(result), 456.0f);
    }
}

TEST(Pair, param_copy_constructor_trivial_type_different_type) {

    using Type1 = i64;
    using Type2 = i32;
    using OtherType1 = i16;
    using OtherType2 = i8;
    static_assert(hud::is_trivially_copy_constructible_v<Type1, OtherType1>);
    static_assert(hud::is_trivially_copy_constructible_v<Type2, OtherType2>);
    using TypePair = Pair<Type1, Type2>;

    const auto test = [](const OtherType1& t1, const OtherType2& t2) {
        TypePair* pair = hud::memory::allocate_array<TypePair>(1);
        hud::memory::construct_at(pair, t1, t2);
        const auto result = std::tuple{
            pair->first,
            pair->second
        };
        hud::memory::free_array(pair, 1);
        return result;
    };

    // Non constant
    {
        const auto result = test(123, 32);
        ASSERT_EQ(hud::get<0>(result), 123);
        ASSERT_EQ(hud::get<1>(result), 32);
    }

    // Constant
    {
        constexpr auto result = test(123, 32);
        ASSERT_EQ(hud::get<0>(result), 123);
        ASSERT_EQ(hud::get<1>(result), 32);
    }
}

TEST(Pair, param_copy_constructor_non_trivial_type_same_type) {

    using Type1 = hud::test::NonBitwiseType;
    using Type2 = hud::test::NonBitwiseType;
    static_assert(!hud::is_trivially_copy_constructible_v<Type1>);
    static_assert(!hud::is_trivially_copy_constructible_v<Type2>);
    using TypePair = Pair<Type1, Type2>;

    const auto test = [](const i32& id1, const i32& id2) {
        TypePair* pair = hud::memory::allocate_array<TypePair>(1);
        const Type1 t1{ id1, nullptr };
        const Type2 t2{ id2, nullptr };
        hud::memory::construct_at(pair, t1, t2);
        const auto result = std::tuple{
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
        ASSERT_EQ(hud::get<0>(result), 0u);
        ASSERT_EQ(hud::get<1>(result), 0u);
        ASSERT_EQ(hud::get<2>(result), 1u);
        ASSERT_EQ(hud::get<3>(result), 0u);
        ASSERT_EQ(hud::get<4>(result), 1u);
        ASSERT_EQ(hud::get<5>(result), 123);

        ASSERT_EQ(hud::get<6>(result), 0u);
        ASSERT_EQ(hud::get<7>(result), 0u);
        ASSERT_EQ(hud::get<8>(result), 1u);
        ASSERT_EQ(hud::get<9>(result), 0u);
        ASSERT_EQ(hud::get<10>(result), 1u);
        ASSERT_EQ(hud::get<11>(result), 456);
    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        ASSERT_EQ(hud::get<0>(result), 0u);
        ASSERT_EQ(hud::get<1>(result), 0u);
        ASSERT_EQ(hud::get<2>(result), 1u);
        ASSERT_EQ(hud::get<3>(result), 0u);
        ASSERT_EQ(hud::get<4>(result), 1u);
        ASSERT_EQ(hud::get<5>(result), 123);

        ASSERT_EQ(hud::get<6>(result), 0u);
        ASSERT_EQ(hud::get<7>(result), 0u);
        ASSERT_EQ(hud::get<8>(result), 1u);
        ASSERT_EQ(hud::get<9>(result), 0u);
        ASSERT_EQ(hud::get<10>(result), 1u);
        ASSERT_EQ(hud::get<11>(result), 456);
    }
}

TEST(Pair, param_copy_constructor_non_trivial_copy_constructible_type_same_type) {

    using Type1 = hud::test::NonBitwiseCopyConstructibleType;
    using Type2 = hud::test::NonBitwiseCopyConstructibleType;
    static_assert(!hud::is_trivially_copy_constructible_v<Type1>);
    static_assert(!hud::is_trivially_copy_constructible_v<Type2>);
    using TypePair = Pair<Type1, Type2>;

    const auto test = [](const i32& id1, const i32& id2) {
        TypePair* pair = hud::memory::allocate_array<TypePair>(1);
        const Type1 t1{ id1 };
        const Type2 t2{ id2 };
        hud::memory::construct_at(pair, t1, t2);
        const auto result = std::tuple{
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
        ASSERT_EQ(hud::get<0>(result), 1u);
        ASSERT_EQ(hud::get<1>(result), 123);
        ASSERT_EQ(hud::get<2>(result), 1u);
        ASSERT_EQ(hud::get<3>(result), 456);
    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        ASSERT_EQ(hud::get<0>(result), 1u);
        ASSERT_EQ(hud::get<1>(result), 123);
        ASSERT_EQ(hud::get<2>(result), 1u);
        ASSERT_EQ(hud::get<3>(result), 456);
    }
}

TEST(Pair, param_copy_constructor_non_trivial_type_different_type) {

    using Type1 = hud::test::NonBitwiseCopyConstructibleType2;
    using Type2 = hud::test::NonBitwiseCopyConstructibleType2;
    using OtherType1 = hud::test::NonBitwiseCopyConstructibleType;
    using OtherType2 = hud::test::NonBitwiseCopyConstructibleType;
    static_assert(hud::is_copy_constructible_v<Type1, OtherType1>);
    static_assert(!hud::is_trivially_copy_constructible_v<Type1, OtherType1>);
    static_assert(hud::is_copy_constructible_v<Type2, OtherType2>);
    static_assert(!hud::is_trivially_copy_constructible_v<Type2, OtherType2>);
    using TypePair = Pair<Type1, Type2>;

    const auto test = [](const i32& id1, const i32& id2) {
        TypePair* pair = hud::memory::allocate_array<TypePair>(1);
        const OtherType1 t1{ id1 };
        const OtherType2 t2{ id2 };
        hud::memory::construct_at(pair, t1, t2);
        const auto result = std::tuple{
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
        ASSERT_EQ(hud::get<0>(result), 1u);
        ASSERT_EQ(hud::get<1>(result), 123);
        ASSERT_EQ(hud::get<2>(result), 1u);
        ASSERT_EQ(hud::get<3>(result), 456);
    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        ASSERT_EQ(hud::get<0>(result), 1u);
        ASSERT_EQ(hud::get<1>(result), 123);
        ASSERT_EQ(hud::get<2>(result), 1u);
        ASSERT_EQ(hud::get<3>(result), 456);
    }
}

TEST(Pair, param_copy_constructor_non_trivial_copy_constructible_type_different_type) {

    using Type1 = hud::test::NonBitwiseCopyConstructibleType2;
    using Type2 = hud::test::NonBitwiseCopyConstructibleType2;
    using OtherType1 = hud::test::NonBitwiseCopyConstructibleType;
    using OtherType2 = hud::test::NonBitwiseCopyConstructibleType;
    static_assert(hud::is_copy_constructible_v<Type1, OtherType1>);
    static_assert(!hud::is_trivially_copy_constructible_v<Type1, OtherType1>);
    static_assert(hud::is_copy_constructible_v<Type2, OtherType2>);
    static_assert(!hud::is_trivially_copy_constructible_v<Type2, OtherType2>);
    using TypePair = Pair<Type1, Type2>;

    const auto test = [](const i32& id1, const i32& id2) {
        TypePair* pair = hud::memory::allocate_array<TypePair>(1);
        const OtherType1 t1{ id1 };
        const OtherType2 t2{ id2 };
        hud::memory::construct_at(pair, t1, t2);
        const auto result = std::tuple{
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
        ASSERT_EQ(hud::get<0>(result), 1u);
        ASSERT_EQ(hud::get<1>(result), 123);
        ASSERT_EQ(hud::get<2>(result), 1u);
        ASSERT_EQ(hud::get<3>(result), 456);
    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        ASSERT_EQ(hud::get<0>(result), 1u);
        ASSERT_EQ(hud::get<1>(result), 123);
        ASSERT_EQ(hud::get<2>(result), 1u);
        ASSERT_EQ(hud::get<3>(result), 456);
    }
}

TEST(Pair, param_move_constructor_trivial_type_same_type) {

    using Type1 = i32;
    using Type2 = f32;
    static_assert(hud::is_trivially_copy_constructible_v<Type1>);
    static_assert(hud::is_trivially_copy_constructible_v<Type2>);
    using TypePair = Pair<Type1, Type2>;

    const auto test = [](Type1&& t1, Type2&& t2) {
        TypePair* pair = hud::memory::allocate_array<TypePair>(1);
        hud::memory::construct_at(pair, hud::forward<Type1>(t1), hud::forward<Type2>(t2));
        const auto result = std::tuple{
            pair->first,
            pair->second
        };
        hud::memory::free_array(pair, 1);
        return result;
    };

    // Non constant
    {
        const auto result = test(123, 456.0f);
        ASSERT_EQ(hud::get<0>(result), 123);
        ASSERT_EQ(hud::get<1>(result), 456.0f);
    }

    // Constant
    {
        constexpr auto result = test(123, 456.0f);
        ASSERT_EQ(hud::get<0>(result), 123);
        ASSERT_EQ(hud::get<1>(result), 456.0f);
    }
}

TEST(Pair, param_move_constructor_non_trivial_type_same_type) {

    using Type1 = hud::test::NonBitwiseType;
    using Type2 = hud::test::NonBitwiseType;
    static_assert(!hud::is_trivially_copy_constructible_v<Type1>);
    static_assert(!hud::is_trivially_copy_constructible_v<Type2>);
    using TypePair = Pair<Type1, Type2>;

    const auto test = [](const i32& id1, const i32& id2) {
        TypePair* pair = hud::memory::allocate_array<TypePair>(1);
        Type1 t1{ id1, nullptr };
        Type2 t2{ id2, nullptr };
        hud::memory::construct_at(pair, hud::move(t1), hud::move(t2));
        const auto result = std::tuple{
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
        ASSERT_EQ(hud::get<0>(result), 0u);
        ASSERT_EQ(hud::get<1>(result), 0u);
        ASSERT_EQ(hud::get<2>(result), 1u);
        ASSERT_EQ(hud::get<3>(result), 1u);
        ASSERT_EQ(hud::get<4>(result), 0u);
        ASSERT_EQ(hud::get<5>(result), 123);

        ASSERT_EQ(hud::get<6>(result), 0u);
        ASSERT_EQ(hud::get<7>(result), 0u);
        ASSERT_EQ(hud::get<8>(result), 1u);
        ASSERT_EQ(hud::get<9>(result), 1u);
        ASSERT_EQ(hud::get<10>(result), 0u);
        ASSERT_EQ(hud::get<11>(result), 456);
    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        ASSERT_EQ(hud::get<0>(result), 0u);
        ASSERT_EQ(hud::get<1>(result), 0u);
        ASSERT_EQ(hud::get<2>(result), 1u);
        ASSERT_EQ(hud::get<3>(result), 1u);
        ASSERT_EQ(hud::get<4>(result), 0u);
        ASSERT_EQ(hud::get<5>(result), 123);

        ASSERT_EQ(hud::get<6>(result), 0u);
        ASSERT_EQ(hud::get<7>(result), 0u);
        ASSERT_EQ(hud::get<8>(result), 1u);
        ASSERT_EQ(hud::get<9>(result), 1u);
        ASSERT_EQ(hud::get<10>(result), 0u);
        ASSERT_EQ(hud::get<11>(result), 456);
    }
}

TEST(Pair, param_move_constructor_non_trivial_copy_constructible_type_same_type) {

    using Type1 = hud::test::NonBitwiseCopyConstructibleType;
    using Type2 = hud::test::NonBitwiseCopyConstructibleType;
    static_assert(!hud::is_trivially_copy_constructible_v<Type1>);
    static_assert(!hud::is_trivially_copy_constructible_v<Type2>);
    using TypePair = Pair<Type1, Type2>;

    const auto test = [](const i32& id1, const i32& id2) {
        TypePair* pair = hud::memory::allocate_array<TypePair>(1);
        Type1 t1{ id1 };
        Type2 t2{ id2 };
        hud::memory::construct_at(pair, hud::move(t1), hud::move(t2));
        const auto result = std::tuple{
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
        ASSERT_EQ(hud::get<0>(result), 1u);
        ASSERT_EQ(hud::get<1>(result), 123);
        ASSERT_EQ(hud::get<2>(result), 1u);
        ASSERT_EQ(hud::get<3>(result), 456);
    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        ASSERT_EQ(hud::get<0>(result), 1u);
        ASSERT_EQ(hud::get<1>(result), 123);
        ASSERT_EQ(hud::get<2>(result), 1u);
        ASSERT_EQ(hud::get<3>(result), 456);
    }
}

TEST(Pair, param_move_constructor_non_trivial_move_constructible_type_same_type) {

    using Type1 = hud::test::NonBitwiseMoveConstructibleType;
    using Type2 = hud::test::NonBitwiseMoveConstructibleType;
    static_assert(!hud::is_trivially_move_constructible_v<Type1>);
    static_assert(!hud::is_trivially_move_constructible_v<Type2>);
    using TypePair = Pair<Type1, Type2>;

    const auto test = [](const i32& id1, const i32& id2) {
        TypePair* pair = hud::memory::allocate_array<TypePair>(1);
        Type1 t1{ id1 };
        Type2 t2{ id2 };

        hud::memory::construct_at(pair, hud::move(t1), hud::move(t2));
        const auto result = std::tuple{
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
        ASSERT_EQ(hud::get<0>(result), 0u);
        ASSERT_EQ(hud::get<1>(result), 1u);
        ASSERT_EQ(hud::get<2>(result), 123);
        ASSERT_EQ(hud::get<3>(result), 0u);
        ASSERT_EQ(hud::get<4>(result), 1u);
        ASSERT_EQ(hud::get<5>(result), 456);
    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        ASSERT_EQ(hud::get<0>(result), 0u);
        ASSERT_EQ(hud::get<1>(result), 1u);
        ASSERT_EQ(hud::get<2>(result), 123);
        ASSERT_EQ(hud::get<3>(result), 0u);
        ASSERT_EQ(hud::get<4>(result), 1u);
        ASSERT_EQ(hud::get<5>(result), 456);
    }
}

TEST(Pair, param_move_constructor_trivial_type_different_type) {

    using Type1 = i64;
    using Type2 = i32;
    using OtherType1 = i16;
    using OtherType2 = i8;
    static_assert(hud::is_trivially_copy_constructible_v<Type1, OtherType1>);
    static_assert(hud::is_trivially_copy_constructible_v<Type2, OtherType2>);
    using TypePair = Pair<Type1, Type2>;

    const auto test = [](OtherType1&& t1, OtherType2&& t2) {
        TypePair* pair = hud::memory::allocate_array<TypePair>(1);
        hud::memory::construct_at(pair, hud::forward<OtherType1>(t1), hud::forward<OtherType2>(t2));
        const auto result = std::tuple{
            pair->first,
            pair->second
        };
        hud::memory::free_array(pair, 1);
        return result;
    };

    // Non constant
    {
        const auto result = test(123, 32);
        ASSERT_EQ(hud::get<0>(result), 123);
        ASSERT_EQ(hud::get<1>(result), 32);
    }

    // Constant
    {
        constexpr auto result = test(123, 32);
        ASSERT_EQ(hud::get<0>(result), 123);
        ASSERT_EQ(hud::get<1>(result), 32);
    }
}

TEST(Pair, param_move_constructor_non_trivial_type_different_type) {

    using Type1 = hud::test::NonBitwiseCopyConstructibleType2;
    using Type2 = hud::test::NonBitwiseCopyConstructibleType2;
    using OtherType1 = hud::test::NonBitwiseCopyConstructibleType;
    using OtherType2 = hud::test::NonBitwiseCopyConstructibleType;
    static_assert(hud::is_copy_constructible_v<Type1, OtherType1>);
    static_assert(!hud::is_trivially_copy_constructible_v<Type1, OtherType1>);
    static_assert(hud::is_copy_constructible_v<Type2, OtherType2>);
    static_assert(!hud::is_trivially_copy_constructible_v<Type2, OtherType2>);
    using TypePair = Pair<Type1, Type2>;

    const auto test = [](const i32& id1, const i32& id2) {
        TypePair* pair = hud::memory::allocate_array<TypePair>(1);
        OtherType1 t1{ id1 };
        OtherType2 t2{ id2 };
        hud::memory::construct_at(pair, hud::move(t1), hud::move(t2));
        const auto result = std::tuple{
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
        ASSERT_EQ(hud::get<0>(result), 1u);
        ASSERT_EQ(hud::get<1>(result), 123);
        ASSERT_EQ(hud::get<2>(result), 1u);
        ASSERT_EQ(hud::get<3>(result), 456);
    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        ASSERT_EQ(hud::get<0>(result), 1u);
        ASSERT_EQ(hud::get<1>(result), 123);
        ASSERT_EQ(hud::get<2>(result), 1u);
        ASSERT_EQ(hud::get<3>(result), 456);
    }
}

TEST(Pair, param_move_constructor_non_trivial_copy_constructible_type_different_type) {

    using Type1 = hud::test::NonBitwiseCopyConstructibleType2;
    using Type2 = hud::test::NonBitwiseCopyConstructibleType2;
    using OtherType1 = hud::test::NonBitwiseCopyConstructibleType;
    using OtherType2 = hud::test::NonBitwiseCopyConstructibleType;
    static_assert(hud::is_copy_constructible_v<Type1, OtherType1>);
    static_assert(!hud::is_trivially_copy_constructible_v<Type1, OtherType1>);
    static_assert(hud::is_copy_constructible_v<Type2, OtherType2>);
    static_assert(!hud::is_trivially_copy_constructible_v<Type2, OtherType2>);
    using TypePair = Pair<Type1, Type2>;

    const auto test = [](const i32& id1, const i32& id2) {
        TypePair* pair = hud::memory::allocate_array<TypePair>(1);
        OtherType1 t1{ id1 };
        OtherType2 t2{ id2 };
        hud::memory::construct_at(pair, hud::move(t1), hud::move(t2));
        const auto result = std::tuple{
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
        ASSERT_EQ(hud::get<0>(result), 1u);
        ASSERT_EQ(hud::get<1>(result), 123);
        ASSERT_EQ(hud::get<2>(result), 1u);
        ASSERT_EQ(hud::get<3>(result), 456);
    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        ASSERT_EQ(hud::get<0>(result), 1u);
        ASSERT_EQ(hud::get<1>(result), 123);
        ASSERT_EQ(hud::get<2>(result), 1u);
        ASSERT_EQ(hud::get<3>(result), 456);
    }
}

TEST(Pair, param_move_constructor_non_trivial_move_constructible_type_different_type) {

    using Type1 = hud::test::NonBitwiseMoveConstructibleType2;
    using Type2 = hud::test::NonBitwiseMoveConstructibleType2;
    using OtherType1 = hud::test::NonBitwiseMoveConstructibleType;
    using OtherType2 = hud::test::NonBitwiseMoveConstructibleType;
    static_assert(hud::is_move_constructible_v<Type1, OtherType1>);
    static_assert(!hud::is_trivially_move_constructible_v<Type1, OtherType1>);
    static_assert(hud::is_move_constructible_v<Type2, OtherType2>);
    static_assert(!hud::is_trivially_move_constructible_v<Type2, OtherType2>);
    using TypePair = Pair<Type1, Type2>;

    const auto test = [](const i32& id1, const i32& id2) {
        TypePair* pair = hud::memory::allocate_array<TypePair>(1);
        OtherType1 t1{ id1 };
        OtherType2 t2{ id2 };

        hud::memory::construct_at(pair, hud::move(t1), hud::move(t2));
        const auto result = std::tuple{
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
        ASSERT_EQ(hud::get<0>(result), 0u);
        ASSERT_EQ(hud::get<1>(result), 1u);
        ASSERT_EQ(hud::get<2>(result), 123);
        ASSERT_EQ(hud::get<3>(result), 0u);
        ASSERT_EQ(hud::get<4>(result), 1u);
        ASSERT_EQ(hud::get<5>(result), 456);
    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        ASSERT_EQ(hud::get<0>(result), 0u);
        ASSERT_EQ(hud::get<1>(result), 1u);
        ASSERT_EQ(hud::get<2>(result), 123);
        ASSERT_EQ(hud::get<3>(result), 0u);
        ASSERT_EQ(hud::get<4>(result), 1u);
        ASSERT_EQ(hud::get<5>(result), 456);
    }
};