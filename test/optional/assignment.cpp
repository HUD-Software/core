#include <core/containers/optional.h>
#include <core/traits/is_trivially_copy_constructible.h>
#include <core/traits/is_trivially_move_constructible.h>
#include <core/traits/is_trivially_copy_assignable.h>
#include <core/traits/is_trivially_move_assignable.h>
#include <optional>

// Nullopt
TEST(optional, assign_empty_trivial_type_with_nullopt) {

    using type = i32;
    static_assert(hud::is_trivially_copy_constructible_v<type>);
    static_assert(hud::is_trivially_move_constructible_v<type>);
    static_assert(hud::is_trivially_copy_assignable_v<type>);
    static_assert(hud::is_trivially_move_assignable_v<type>);

    const auto test = []() {
        hud::optional<type> option;
        const bool had_value = option.has_value();

        option = nullopt;

        return std::tuple{
            had_value,
            option.has_value()
        };
    };

    // Non constant
    {
        auto result = test();
        ASSERT_FALSE(hud::get<0>(result));
        ASSERT_FALSE(hud::get<1>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_FALSE(hud::get<0>(result));
        ASSERT_FALSE(hud::get<1>(result));
    }
}

TEST(optional, assign_empty_non_trivial_type_with_nullopt) {

    using type = hud::test::NonBitwiseType;
    static_assert(!hud::is_trivially_copy_constructible_v<type>);
    static_assert(!hud::is_trivially_move_constructible_v<type>);
    static_assert(!hud::is_trivially_copy_assignable_v<type>);
    static_assert(!hud::is_trivially_move_assignable_v<type>);

    const auto test = []() {
        hud::optional<type> option;
        const bool had_value = option.has_value();

        option = nullopt;

        return std::tuple{
            had_value,
            option.has_value()
        };
    };

    // Non constant
    {
        auto result = test();
        ASSERT_FALSE(hud::get<0>(result));
        ASSERT_FALSE(hud::get<1>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_FALSE(hud::get<0>(result));
        ASSERT_FALSE(hud::get<1>(result));
    }
}

TEST(optional, assign_non_empty_trivial_type_with_nullopt) {

    using type = i32;
    static_assert(hud::is_trivially_copy_constructible_v<type>);
    static_assert(hud::is_trivially_move_constructible_v<type>);
    static_assert(hud::is_trivially_copy_assignable_v<type>);
    static_assert(hud::is_trivially_move_assignable_v<type>);

    const auto test = [](type value) {
        hud::optional<type> option{ value };
        const bool had_value_before = option.has_value();
        const type value_before = option.value();

        option = nullopt;

        return std::tuple{
            had_value_before,
            value_before,
            option.has_value(),

        };
    };

    // Non constant
    {
        auto result = test(123);
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_EQ(hud::get<1>(result), 123);
        ASSERT_FALSE(hud::get<2>(result));
    }

    // Constant
    {
        constexpr auto result = test(123);
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_EQ(hud::get<1>(result), 123);
        ASSERT_FALSE(hud::get<2>(result));
    }
}

TEST(optional, assign_non_empty_non_trivial_type_with_nullopt) {

    using type = hud::test::NonBitwiseType;
    static_assert(!hud::is_trivially_copy_constructible_v<type>);
    static_assert(!hud::is_trivially_move_constructible_v<type>);
    static_assert(!hud::is_trivially_copy_assignable_v<type>);
    static_assert(!hud::is_trivially_move_assignable_v<type>);

    const auto test = []() {
        i32 destructor_count{ 0 };
        hud::optional<type> option{ hud::in_place, &destructor_count };
        const bool had_value_before = option.has_value();
        const i32 destructor_count_before = destructor_count;
        const u32 move_assign_count_before = option.value().move_assign_count();
        const u32 copy_assign_count_before = option.value().copy_assign_count();
        const u32 constructor_count_before = option.value().constructor_count();
        const u32 move_constructor_count_before = option.value().move_constructor_count();
        const u32 copy_constructor_count_before = option.value().copy_constructor_count();

        option = nullopt;

        return std::tuple{
            had_value_before,
            destructor_count_before,
            move_assign_count_before,
            copy_assign_count_before,
            constructor_count_before,
            move_constructor_count_before,
            copy_constructor_count_before,
            option.has_value(),
            destructor_count
        };
    };

    // Non constant
    {
        auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_EQ(hud::get<1>(result), 0);
        ASSERT_EQ(hud::get<2>(result), 0u);
        ASSERT_EQ(hud::get<3>(result), 0u);
        ASSERT_EQ(hud::get<4>(result), 1u);
        ASSERT_EQ(hud::get<5>(result), 0u);
        ASSERT_EQ(hud::get<6>(result), 0u);
        ASSERT_FALSE(hud::get<7>(result));
        ASSERT_EQ(hud::get<8>(result), 1);
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_EQ(hud::get<1>(result), 0);
        ASSERT_EQ(hud::get<2>(result), 0u);
        ASSERT_EQ(hud::get<3>(result), 0u);
        ASSERT_EQ(hud::get<4>(result), 1u);
        ASSERT_EQ(hud::get<5>(result), 0u);
        ASSERT_EQ(hud::get<6>(result), 0u);
        ASSERT_FALSE(hud::get<7>(result));
        ASSERT_EQ(hud::get<8>(result), 1);
    }
}

TEST(optional, assign_by_copy_empty_trivially_copy_assignable_same_type) {

    using type = i32;
    static_assert(hud::is_trivially_copy_constructible_v<type>);
    static_assert(hud::is_trivially_copy_assignable_v<type>);

    const auto test = [](const type value) {
        hud::optional<type> option;
        const bool had_value_before = option.has_value();

        option = value;

        return std::tuple{
            had_value_before,
            option.has_value(),
            option.value()
        };
    };

    // Non constant
    {
        auto result = test(123);
        ASSERT_FALSE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_EQ(hud::get<2>(result), 123);
        
    }

    // Constant
    {
        constexpr auto result = test(123);
        ASSERT_FALSE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_EQ(hud::get<2>(result), 123);
    }
}

TEST(optional, assign_by_copy_empty_trivially_copy_assignable_different_type) {

    using type = i32;
    using OtherType = i16;
    static_assert(hud::is_trivially_copy_constructible_v<type, OtherType>);
    static_assert(hud::is_trivially_copy_assignable_v<type, OtherType>);
    static_assert(hud::is_not_same_v<type, OtherType>);

    const auto test = [](const OtherType value) {
        hud::optional<type> option;
        const bool had_value_before = option.has_value();

        option = value;

        return std::tuple{
            had_value_before,
            option.has_value(),
            option.value()
        };
    };

    // Non constant
    {
        auto result = test(123);
        ASSERT_FALSE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_EQ(hud::get<2>(result), 123);

    }

    // Constant
    {
        constexpr auto result = test(123);
        ASSERT_FALSE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_EQ(hud::get<2>(result), 123);
    }
}

TEST(optional, assign_by_copy_non_empty_trivially_copy_assignable_same_type) {

    using type = i32;
    static_assert(hud::is_trivially_copy_constructible_v<type>);
    static_assert(hud::is_trivially_copy_assignable_v<type>);

    const auto test = [](const type before, const type after) {
        hud::optional<type> option{ before };
        const bool had_value_before = option.has_value();
        const type value_before = option.value();

        option = after;

        return std::tuple{
            had_value_before,
            value_before,
            option.has_value(),
            option.value()
        };
    };

    // Non constant
    {
        auto result = test(123,456);
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_EQ(hud::get<1>(result), 123);
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_EQ(hud::get<3>(result), 456);

    }

    // Constant
    {
        constexpr auto result = test(123,456);
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_EQ(hud::get<1>(result), 123);
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_EQ(hud::get<3>(result), 456);
    }
}

TEST(optional, assign_by_copy_non_empty_trivially_copy_assignable_different_type) {

    using type = i32;
    using OtherType = i16;
    static_assert(hud::is_trivially_copy_constructible_v<type, OtherType>);
    static_assert(hud::is_trivially_copy_assignable_v<type, OtherType>);
    static_assert(hud::is_not_same_v<type, OtherType>);

    const auto test = [](const type before, const OtherType after) {
        hud::optional<type> option{ before };
        const bool had_value_before = option.has_value();
        const type value_before = option.value();

        option = after;

        return std::tuple{
            had_value_before,
            value_before,
            option.has_value(),
            option.value()
        };
    };

    // Non constant
    {
        auto result = test(123, 456);
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_EQ(hud::get<1>(result), 123);
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_EQ(hud::get<3>(result), 456);

    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_EQ(hud::get<1>(result), 123);
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_EQ(hud::get<3>(result), 456);
    }
}

TEST(optional, assign_by_copy_empty_non_trivially_copy_assignable_same_type) {


    using type = hud::test::NonBitwiseType;

    static_assert(!hud::is_trivially_copy_constructible_v<type>);
    static_assert(!hud::is_trivially_copy_assignable_v<type>);

    const auto test = [](const i32 value) {
        i32 destructor_count;
        hud::optional<type> option;
        const bool had_value_before = option.has_value();

        const type other{ value, &destructor_count };
        option = other;

        return std::tuple{
            had_value_before,
            option.has_value(),
            option.value().id(),
            option.value().move_assign_count(),
            option.value().copy_assign_count(),
            option.value().constructor_count(),
            option.value().move_constructor_count(),
            option.value().copy_constructor_count(),
            destructor_count
        };
    };


    // Non constant
    {
        auto result = test(123);
        ASSERT_FALSE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_EQ(hud::get<2>(result), 123);
        ASSERT_EQ(hud::get<3>(result), 0u);
        ASSERT_EQ(hud::get<4>(result), 0u);
        ASSERT_EQ(hud::get<5>(result), 1u);
        ASSERT_EQ(hud::get<6>(result), 0u);
        ASSERT_EQ(hud::get<7>(result), 1u);
        ASSERT_EQ(hud::get<8>(result), 0);
        
    }

    // Constant
   /* {
        constexpr auto result = test(123);
        ASSERT_FALSE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_EQ(hud::get<2>(result), 123);
        ASSERT_EQ(hud::get<3>(result), 0u);
        ASSERT_EQ(hud::get<4>(result), 0u);
        ASSERT_EQ(hud::get<5>(result), 1u);
        ASSERT_EQ(hud::get<6>(result), 0u);
        ASSERT_EQ(hud::get<7>(result), 1u);
        ASSERT_EQ(hud::get<8>(result), 0);
    }*/
}

TEST(optional, assign_by_copy_empty_non_trivially_copy_assignable_different_type) {


    using type = hud::test::NonBitwiseCopyAssignableType2;
    using OtherType = hud::test::NonBitwiseCopyAssignableType;

    static_assert(!hud::is_trivially_copy_constructible_v<type, OtherType>);
    static_assert(!hud::is_trivially_copy_assignable_v<type, OtherType>);
    static_assert(hud::is_not_same_v<type, OtherType>);

    const auto test = [](const i32 value) {
        hud::optional<type> option;
        const bool has_value_before = option.has_value();

        const OtherType other{ value };
        option = other;

        return std::tuple{
            has_value_before,
            option.has_value(),
            option.value().id(),
            option.value().copy_assign_count(),
            option.value().copy_constructor_count()
        };
    };

    // Non constant
    {
        auto result = test(123);
        ASSERT_FALSE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_EQ(hud::get<2>(result), 123);
        ASSERT_EQ(hud::get<3>(result), 0u);
        ASSERT_EQ(hud::get<4>(result), 1u);
    }

    // Constant
    {
        constexpr auto result = test(123);
        ASSERT_FALSE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_EQ(hud::get<2>(result), 123);
        ASSERT_EQ(hud::get<3>(result), 0u);
        ASSERT_EQ(hud::get<4>(result), 1u);
    }
}

TEST(optional, assign_by_copy_non_empty_non_trivially_copy_assignable_same_type) {

    using type = hud::test::NonBitwiseType;

    static_assert(!hud::is_trivially_copy_constructible_v<type>);
    static_assert(!hud::is_trivially_copy_assignable_v<type>);

    const auto test = [](const i32 before, const i32 after) {
        i32 destructor_count;
        i32 other_destructor_count;
        hud::optional<type> option{ hud::in_place, before, &destructor_count };

        const bool has_value_before = option.has_value();
        const i32 id_before = option.value().id();
        const u32 move_assign_count_before = option.value().move_assign_count();
        const u32 copy_assign_count_before = option.value().copy_assign_count();
        const u32 constructor_count_before = option.value().constructor_count();
        const u32 move_constructor_count_before = option.value().move_constructor_count();
        const u32 copy_constructor_count_before = option.value().copy_constructor_count();
        const i32 destructor_count_before = destructor_count;

        const type other{ after, &other_destructor_count };
        option = other;

        return std::tuple{
            has_value_before,
            id_before,
            move_assign_count_before,
            copy_assign_count_before,
            constructor_count_before,
            move_constructor_count_before,
            copy_constructor_count_before,
            destructor_count_before,
            option.has_value(),
            option.value().id(),
            option.value().move_assign_count(),
            option.value().copy_assign_count(),
            option.value().constructor_count(),
            option.value().move_constructor_count(),
            option.value().copy_constructor_count(),
            destructor_count,
            other_destructor_count,
        };
        
    };

    // Non constant
    {
        auto result = test(123, 456);
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_EQ(hud::get<1>(result), 123);
        ASSERT_EQ(hud::get<2>(result), 0u);
        ASSERT_EQ(hud::get<3>(result), 0u);
        ASSERT_EQ(hud::get<4>(result), 1u);
        ASSERT_EQ(hud::get<5>(result), 0u);
        ASSERT_EQ(hud::get<6>(result), 0u);
        ASSERT_EQ(hud::get<7>(result), 0);
        ASSERT_TRUE(hud::get<8>(result));
        ASSERT_EQ(hud::get<9>(result), 456);
        ASSERT_EQ(hud::get<10>(result), 0u);
        ASSERT_EQ(hud::get<11>(result), 1u);
        ASSERT_EQ(hud::get<12>(result), 1u);
        ASSERT_EQ(hud::get<13>(result), 0u);
        ASSERT_EQ(hud::get<14>(result), 0u);
        ASSERT_EQ(hud::get<15>(result), 0);
        ASSERT_EQ(hud::get<16>(result), 0);
    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_EQ(hud::get<1>(result), 123);
        ASSERT_EQ(hud::get<2>(result), 0u);
        ASSERT_EQ(hud::get<3>(result), 0u);
        ASSERT_EQ(hud::get<4>(result), 1u);
        ASSERT_EQ(hud::get<5>(result), 0u);
        ASSERT_EQ(hud::get<6>(result), 0u);
        ASSERT_EQ(hud::get<7>(result), 0);
        ASSERT_TRUE(hud::get<8>(result));
        ASSERT_EQ(hud::get<9>(result), 456);
        ASSERT_EQ(hud::get<10>(result), 0u);
        ASSERT_EQ(hud::get<11>(result), 1u);
        ASSERT_EQ(hud::get<12>(result), 1u);
        ASSERT_EQ(hud::get<13>(result), 0u);
        ASSERT_EQ(hud::get<14>(result), 0u);
        ASSERT_EQ(hud::get<15>(result), 0);
        ASSERT_EQ(hud::get<16>(result), 0);
    }
}

TEST(optional, assign_by_copy_non_empty_non_trivially_copy_assignable_different_type) {


    using type = hud::test::NonBitwiseCopyAssignableType2;
    using OtherType = hud::test::NonBitwiseCopyAssignableType;


    static_assert(!hud::is_trivially_copy_constructible_v<type, OtherType>);
    static_assert(!hud::is_trivially_copy_assignable_v<type, OtherType>);
    static_assert(hud::is_not_same_v<type, OtherType>);


    const auto test = [](const i32 before, const i32 after) {
        hud::optional<type> option{ before };
        const bool has_value_before = option.has_value();
        const i32 value_before = option.value().id();
        const u32 copy_assign_count_before = option.value().copy_assign_count();
        const u32 copy_constructor_count_before = option.value().copy_constructor_count();

        const OtherType other{ after };

        option = other;

        return std::tuple{
            has_value_before,
            value_before,
            copy_assign_count_before,
            copy_constructor_count_before,
            option.has_value(),
            option.value().id(),
            option.value().copy_assign_count(),
            option.value().copy_constructor_count()
        };
    };

    // Non constant
    {
        auto result = test(123, 456);
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_EQ(hud::get<1>(result), 123);
        ASSERT_EQ(hud::get<2>(result), 0u);
        ASSERT_EQ(hud::get<3>(result), 0u);
        ASSERT_TRUE(hud::get<4>(result));
        ASSERT_EQ(hud::get<5>(result), 456);
        ASSERT_EQ(hud::get<6>(result), 1u);
        ASSERT_EQ(hud::get<7>(result), 0u);
    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_EQ(hud::get<1>(result), 123);
        ASSERT_EQ(hud::get<2>(result), 0u);
        ASSERT_EQ(hud::get<3>(result), 0u);
        ASSERT_TRUE(hud::get<4>(result));
        ASSERT_EQ(hud::get<5>(result), 456);
        ASSERT_EQ(hud::get<6>(result), 1u);
        ASSERT_EQ(hud::get<7>(result), 0u);
    }
}

TEST(optional, assign_by_move_empty_trivially_copy_assignable_same_type) {

    using type = i32;
    static_assert(hud::is_trivially_copy_constructible_v<type>);
    static_assert(hud::is_trivially_copy_assignable_v<type>);

    const auto test = [](type value) {
        hud::optional<type> option;
        const bool had_value_before = option.has_value();

        option = hud::move(value);

        return std::tuple{
            had_value_before,
            option.has_value(),
            option.value()
        };
    };

    // Non constant
    {
        auto result = test(123);
        ASSERT_FALSE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_EQ(hud::get<2>(result), 123);

    }

    // Constant
    {
        constexpr auto result = test(123);
        ASSERT_FALSE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_EQ(hud::get<2>(result), 123);
    }
}

TEST(optional, assign_by_move_empty_trivially_copy_assignable_different_type) {

    using type = i32;
    using OtherType = i16;
    static_assert(hud::is_trivially_copy_constructible_v<type, OtherType>);
    static_assert(hud::is_trivially_copy_assignable_v<type, OtherType>);
    static_assert(hud::is_not_same_v<type, OtherType>);

    const auto test = [](OtherType value) {
        hud::optional<type> option;
        const bool had_value_before = option.has_value();

        option = hud::move(value);

        return std::tuple{
            had_value_before,
            option.has_value(),
            option.value()
        };
    };

    // Non constant
    {
        auto result = test(123);
        ASSERT_FALSE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_EQ(hud::get<2>(result), 123);

    }

    // Constant
    {
        constexpr auto result = test(123);
        ASSERT_FALSE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_EQ(hud::get<2>(result), 123);
    }
}

TEST(optional, assign_by_move_non_empty_trivially_copy_assignable_same_type) {

    using type = i32;
    static_assert(hud::is_trivially_copy_constructible_v<type>);
    static_assert(hud::is_trivially_copy_assignable_v<type>);

    const auto test = [](const type before, type after) {
        hud::optional<type> option{ before };
        const bool had_value_before = option.has_value();
        const type value_before = option.value();

        option = hud::move(after);

        return std::tuple{
            had_value_before,
            value_before,
            option.has_value(),
            option.value()
        };
    };

    // Non constant
    {
        auto result = test(123, 456);
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_EQ(hud::get<1>(result), 123);
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_EQ(hud::get<3>(result), 456);

    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_EQ(hud::get<1>(result), 123);
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_EQ(hud::get<3>(result), 456);
    }
}

TEST(optional, assign_by_move_non_empty_trivially_copy_assignable_different_type) {

    using type = i32;
    using OtherType = i16;
    static_assert(hud::is_trivially_copy_constructible_v<type, OtherType>);
    static_assert(hud::is_trivially_copy_assignable_v<type, OtherType>);
    static_assert(hud::is_not_same_v<type, OtherType>);

    const auto test = [](const type before, OtherType after) {
        hud::optional<type> option{ before };
        const bool had_value_before = option.has_value();
        const type value_before = option.value();

        option = hud::move(after);

        return std::tuple{
            had_value_before,
            value_before,
            option.has_value(),
            option.value()
        };
    };

    // Non constant
    {
        auto result = test(123, 456);
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_EQ(hud::get<1>(result), 123);
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_EQ(hud::get<3>(result), 456);

    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_EQ(hud::get<1>(result), 123);
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_EQ(hud::get<3>(result), 456);
    }
}

TEST(optional, assign_by_move_empty_non_trivially_copy_assignable_same_type) {
    // Current STD specification make it not possible
    // because hud::is_constructible_v<type_t, U> where type_t and U is not move assignable is false
    
    //using namespace hud;

    //using type = hud::test::NonBitwiseCopyAssignableType;

    //static_assert(!hud::is_trivially_copy_constructible_v<type>);
    //static_assert(!hud::is_trivially_copy_assignable_v<type>);

    //const auto test = [](const i32 value) {
    //    hud::optional<type> option;
    //    const bool had_value_before = option.has_value();

    //    type other{ value };

    //    option = hud::move(other);

    //    return std::tuple{
    //        had_value_before,
    //        option.has_value(),
    //        option.value().id(),
    //        option.value().copy_assign_count(),
    //        option.value().copy_constructor_count(),
    //    };
    //};


    //// Non constant
    //{
    //    auto result = test(123);
    //    ASSERT_FALSE(hud::get<0>(result));
    //    ASSERT_TRUE(hud::get<1>(result));
    //    ASSERT_EQ(hud::get<2>(result), 123);
    //    ASSERT_EQ(hud::get<3>(result), 0u);
    //    ASSERT_EQ(hud::get<4>(result), 1u);
    //}

    //// Constant
    //{
    //    auto result = test(123);
    //    ASSERT_FALSE(hud::get<0>(result));
    //    ASSERT_TRUE(hud::get<1>(result));
    //    ASSERT_EQ(hud::get<2>(result), 123);
    //    ASSERT_EQ(hud::get<3>(result), 0u);
    //    ASSERT_EQ(hud::get<4>(result), 1u);
    //}
}

TEST(optional, assign_by_move_empty_non_trivially_copy_assignable_different_type) {


    using type = hud::test::NonBitwiseCopyAssignableType2;
    using OtherType = hud::test::NonBitwiseCopyAssignableType;

    static_assert(!hud::is_trivially_copy_constructible_v<type, OtherType>);
    static_assert(!hud::is_trivially_copy_assignable_v<type, OtherType>);
    static_assert(hud::is_not_same_v<type, OtherType>);

    const auto test = [](const i32 value) {
        hud::optional<type> option;
        const bool has_value_before = option.has_value();

        OtherType other{ value };
        option = hud::move(other);

        return std::tuple{
            has_value_before,
            option.has_value(),
            option.value().id(),
            option.value().copy_assign_count(),
            option.value().copy_constructor_count()
        };
    };

    // Non constant
    {
        auto result = test(123);
        ASSERT_FALSE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_EQ(hud::get<2>(result), 123);
        ASSERT_EQ(hud::get<3>(result), 0u);
        ASSERT_EQ(hud::get<4>(result), 1u);
    }

    // Constant
    {
        constexpr auto result = test(123);
        ASSERT_FALSE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_EQ(hud::get<2>(result), 123);
        ASSERT_EQ(hud::get<3>(result), 0u);
        ASSERT_EQ(hud::get<4>(result), 1u);
    }
}

TEST(optional, assign_by_move_non_empty_non_trivially_copy_assignable_same_type) {
    // Current STD specification make it not possible
    // because hud::is_constructible_v<type_t, U> where type_t and U is not move assignable is false
  
    //using namespace hud;
    //using type = hud::test::NonBitwiseCopyAssignableType;

    //static_assert(!hud::is_trivially_copy_constructible_v<type>);
    //static_assert(!hud::is_trivially_copy_assignable_v<type>);

    //const auto test = [](const i32 before, const i32 after) {
    //    hud::optional<type> option{ hud::in_place, before };

    //    const bool has_value_before = option.has_value();
    //    const i32 id_before = option.value().id();
    //    const u32 copy_assign_count_before = option.value().copy_assign_count();
    //    const u32 copy_constructor_count_before = option.value().copy_constructor_count();

    //    type other{ after };
    //    option = hud::move(other);

    //    return std::tuple{
    //        has_value_before,
    //        id_before,
    //        copy_assign_count_before,
    //        copy_constructor_count_before,
    //        option.has_value(),
    //        option.value().id(),
    //        option.value().copy_assign_count(),
    //        option.value().copy_constructor_count(),
    //    };

    //};

    //// Non constant
    //{
    //    auto result = test(123, 456);
    //    ASSERT_TRUE(hud::get<0>(result));
    //    ASSERT_EQ(hud::get<1>(result), 123);
    //    ASSERT_EQ(hud::get<2>(result), 0u);
    //    ASSERT_EQ(hud::get<3>(result), 0u);
    //    ASSERT_TRUE(hud::get<4>(result));
    //    ASSERT_EQ(hud::get<5>(result), 456);
    //    ASSERT_EQ(hud::get<6>(result), 1u);
    //    ASSERT_EQ(hud::get<7>(result), 0u);
    //}

    //// Constant
    //{
    //    constexpr auto result = test(123, 456);
    //    ASSERT_TRUE(hud::get<0>(result));
    //    ASSERT_EQ(hud::get<1>(result), 123);
    //    ASSERT_EQ(hud::get<2>(result), 0u);
    //    ASSERT_EQ(hud::get<3>(result), 0u);
    //    ASSERT_TRUE(hud::get<4>(result));
    //    ASSERT_EQ(hud::get<5>(result), 456);
    //    ASSERT_EQ(hud::get<6>(result), 1u);
    //    ASSERT_EQ(hud::get<7>(result), 0u);
    //}
}

TEST(optional, assign_by_move_non_empty_non_trivially_copy_assignable_different_type) {


    using type = hud::test::NonBitwiseCopyAssignableType2;
    using OtherType = hud::test::NonBitwiseCopyAssignableType;


    static_assert(!hud::is_trivially_copy_constructible_v<type, OtherType>);
    static_assert(!hud::is_trivially_copy_assignable_v<type, OtherType>);
    static_assert(hud::is_not_same_v<type, OtherType>);


    const auto test = [](const i32 before, const i32 after) {
        hud::optional<type> option{ before };
        const bool has_value_before = option.has_value();
        const i32 value_before = option.value().id();
        const u32 copy_assign_count_before = option.value().copy_assign_count();
        const u32 copy_constructor_count_before = option.value().copy_constructor_count();

        OtherType other{ after };

        option = hud::move(other);

        return std::tuple{
            has_value_before,
            value_before,
            copy_assign_count_before,
            copy_constructor_count_before,
            option.has_value(),
            option.value().id(),
            option.value().copy_assign_count(),
            option.value().copy_constructor_count()
        };
    };

    // Non constant
    {
        auto result = test(123, 456);
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_EQ(hud::get<1>(result), 123);
        ASSERT_EQ(hud::get<2>(result), 0u);
        ASSERT_EQ(hud::get<3>(result), 0u);
        ASSERT_TRUE(hud::get<4>(result));
        ASSERT_EQ(hud::get<5>(result), 456);
        ASSERT_EQ(hud::get<6>(result), 1u);
        ASSERT_EQ(hud::get<7>(result), 0u);
    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_EQ(hud::get<1>(result), 123);
        ASSERT_EQ(hud::get<2>(result), 0u);
        ASSERT_EQ(hud::get<3>(result), 0u);
        ASSERT_TRUE(hud::get<4>(result));
        ASSERT_EQ(hud::get<5>(result), 456);
        ASSERT_EQ(hud::get<6>(result), 1u);
        ASSERT_EQ(hud::get<7>(result), 0u);
    }
}

TEST(optional, assign_by_move_empty_trivially_move_assignable_same_type) {

    using type = i32;
    static_assert(hud::is_trivially_move_constructible_v<type>);
    static_assert(hud::is_trivially_move_assignable_v<type>);

    const auto test = []( type value) {
        hud::optional<type> option;
        const bool had_value_before = option.has_value();

        option = hud::move(value);

        return std::tuple{
            had_value_before,
            option.has_value(),
            option.value()
        };
    };

    // Non constant
    {
        auto result = test(123);
        ASSERT_FALSE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_EQ(hud::get<2>(result), 123);

    }

    // Constant
    {
        constexpr auto result = test(123);
        ASSERT_FALSE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_EQ(hud::get<2>(result), 123);
    }
}

TEST(optional, assign_by_move_empty_trivially_move_assignable_different_type) {

    using type = i32;
    using OtherType = i16;
    static_assert(hud::is_trivially_move_constructible_v<type, OtherType>);
    static_assert(hud::is_trivially_move_assignable_v<type, OtherType>);
    static_assert(hud::is_not_same_v<type, OtherType>);

    const auto test = [](OtherType value) {
        hud::optional<type> option;
        const bool had_value_before = option.has_value();

        option = hud::move(value);

        return std::tuple{
            had_value_before,
            option.has_value(),
            option.value()
        };
    };

    // Non constant
    {
        auto result = test(123);
        ASSERT_FALSE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_EQ(hud::get<2>(result), 123);

    }

    // Constant
    {
        constexpr auto result = test(123);
        ASSERT_FALSE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_EQ(hud::get<2>(result), 123);
    }
}

TEST(optional, assign_by_move_non_empty_trivially_move_assignable_same_type) {

    using type = i32;
    static_assert(hud::is_trivially_move_constructible_v<type>);
    static_assert(hud::is_trivially_move_assignable_v<type>);

    const auto test = [](const type before, type after) {
        hud::optional<type> option{ before };
        const bool had_value_before = option.has_value();
        const type value_before = option.value();

        option = hud::move(after);

        return std::tuple{
            had_value_before,
            value_before,
            option.has_value(),
            option.value()
        };
    };

    // Non constant
    {
        auto result = test(123, 456);
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_EQ(hud::get<1>(result), 123);
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_EQ(hud::get<3>(result), 456);

    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_EQ(hud::get<1>(result), 123);
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_EQ(hud::get<3>(result), 456);
    }
}

TEST(optional, assign_by_move_non_empty_trivially_move_assignable_different_type) {

    using type = i32;
    using OtherType = i16;
    static_assert(hud::is_trivially_move_constructible_v<type, OtherType>);
    static_assert(hud::is_trivially_move_assignable_v<type, OtherType>);
    static_assert(hud::is_not_same_v<type, OtherType>);

    const auto test = [](const type before, OtherType after) {
        hud::optional<type> option{ before };
        const bool had_value_before = option.has_value();
        const type value_before = option.value();

        option = hud::move(after);

        return std::tuple{
            had_value_before,
            value_before,
            option.has_value(),
            option.value()
        };
    };

    // Non constant
    {
        auto result = test(123, 456);
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_EQ(hud::get<1>(result), 123);
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_EQ(hud::get<3>(result), 456);

    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_EQ(hud::get<1>(result), 123);
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_EQ(hud::get<3>(result), 456);
    }
}

TEST(optional, assign_by_move_empty_non_trivially_move_assignable_same_type) {


    using type = hud::test::NonBitwiseMoveAssignableType;

    static_assert(!hud::is_trivially_move_constructible_v<type>);
    static_assert(!hud::is_trivially_move_assignable_v<type>);

    const auto test = [](const i32 value) {
        hud::optional<type> option;
        const bool had_value_before = option.has_value();

        type other{ value };

        option = hud::move(other);

        return std::tuple{
            had_value_before,
            option.has_value(),
            option.value().id(),
            option.value().copy_assign_count(),
            option.value().copy_constructor_count(),
            option.value().move_assign_count(),
            option.value().move_constructor_count(),
        };
    };


    // Non constant
    {
        auto result = test(123);
        ASSERT_FALSE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_EQ(hud::get<2>(result), 123);
        ASSERT_EQ(hud::get<3>(result), 0u);
        ASSERT_EQ(hud::get<4>(result), 0u);
        ASSERT_EQ(hud::get<5>(result), 0u);
        ASSERT_EQ(hud::get<6>(result), 1u);
    }

    // Constant
    {
        auto result = test(123);
        ASSERT_FALSE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_EQ(hud::get<2>(result), 123);
        ASSERT_EQ(hud::get<3>(result), 0u);
        ASSERT_EQ(hud::get<4>(result), 0u);
        ASSERT_EQ(hud::get<5>(result), 0u);
        ASSERT_EQ(hud::get<6>(result), 1u);
    }
}

TEST(optional, assign_by_move_empty_non_trivially_move_assignable_different_type) {


    using type = hud::test::NonBitwiseMoveAssignableType2;
    using OtherType = hud::test::NonBitwiseMoveAssignableType;

    static_assert(!hud::is_trivially_move_constructible_v<type, OtherType>);
    static_assert(!hud::is_trivially_move_assignable_v<type, OtherType>);
    static_assert(hud::is_not_same_v<type, OtherType>);

    const auto test = [](const i32 value) {
        hud::optional<type> option;
        const bool has_value_before = option.has_value();

        OtherType other{ value };
        option = hud::move(other);

        return std::tuple{
            has_value_before,
            option.has_value(),
            option.value().id(),
            option.value().copy_assign_count(),
            option.value().copy_constructor_count(),
            option.value().move_assign_count(),
            option.value().move_constructor_count(),
        };
    };

    // Non constant
    {
        auto result = test(123);
        ASSERT_FALSE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_EQ(hud::get<2>(result), 123);
        ASSERT_EQ(hud::get<3>(result), 0u);
        ASSERT_EQ(hud::get<4>(result), 0u);
        ASSERT_EQ(hud::get<5>(result), 0u);
        ASSERT_EQ(hud::get<6>(result), 1u);
    }

    // Constant
    {
        constexpr auto result = test(123);
        ASSERT_FALSE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_EQ(hud::get<2>(result), 123);
        ASSERT_EQ(hud::get<3>(result), 0u);
        ASSERT_EQ(hud::get<4>(result), 0u);
        ASSERT_EQ(hud::get<5>(result), 0u);
        ASSERT_EQ(hud::get<6>(result), 1u);
    }
}

TEST(optional, assign_by_move_non_empty_non_trivially_move_assignable_same_type) {

    using type = hud::test::NonBitwiseMoveAssignableType;

    static_assert(!hud::is_trivially_move_constructible_v<type>);
    static_assert(!hud::is_trivially_move_assignable_v<type>);

    const auto test = [](const i32 before, const i32 after) {
        hud::optional<type> option{ hud::in_place, before };

        const bool has_value_before = option.has_value();
        const i32 id_before = option.value().id();
        const u32 copy_assign_count_before = option.value().copy_assign_count();
        const u32 copy_constructor_count_before = option.value().copy_constructor_count();
        const u32 move_assign_count_before = option.value().move_assign_count();
        const u32 move_constructor_count_before = option.value().move_constructor_count();

        type other{ after };

        option = hud::move(other);

        return std::tuple{
            has_value_before,
            id_before,
            copy_assign_count_before,
            copy_constructor_count_before,
            move_assign_count_before,
            move_constructor_count_before,
            option.has_value(),
            option.value().id(),
            option.value().copy_assign_count(),
            option.value().copy_constructor_count(),
            option.value().move_assign_count(),
            option.value().move_constructor_count(),
        };

    };

    // Non constant
    {
        auto result = test(123, 456);
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_EQ(hud::get<1>(result), 123);
        ASSERT_EQ(hud::get<2>(result), 0u);
        ASSERT_EQ(hud::get<3>(result), 0u);
        ASSERT_EQ(hud::get<4>(result), 0u);
        ASSERT_EQ(hud::get<5>(result), 0u);
        ASSERT_TRUE(hud::get<6>(result));
        ASSERT_EQ(hud::get<7>(result), 456);
        ASSERT_EQ(hud::get<8>(result), 0u);
        ASSERT_EQ(hud::get<9>(result), 0u);
        ASSERT_EQ(hud::get<10>(result), 1u);
        ASSERT_EQ(hud::get<11>(result), 0u);

    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_EQ(hud::get<1>(result), 123);
        ASSERT_EQ(hud::get<2>(result), 0u);
        ASSERT_EQ(hud::get<3>(result), 0u);
        ASSERT_EQ(hud::get<4>(result), 0u);
        ASSERT_EQ(hud::get<5>(result), 0u);
        ASSERT_TRUE(hud::get<6>(result));
        ASSERT_EQ(hud::get<7>(result), 456);
        ASSERT_EQ(hud::get<8>(result), 0u);
        ASSERT_EQ(hud::get<9>(result), 0u);
        ASSERT_EQ(hud::get<10>(result), 1u);
        ASSERT_EQ(hud::get<11>(result), 0u);
    }
}

TEST(optional, assign_by_move_non_empty_non_trivially_move_assignable_different_type) {


    using type = hud::test::NonBitwiseMoveAssignableType2;
    using OtherType = hud::test::NonBitwiseMoveAssignableType;


    static_assert(!hud::is_trivially_move_constructible_v<type, OtherType>);
    static_assert(!hud::is_trivially_move_assignable_v<type, OtherType>);
    static_assert(hud::is_not_same_v<type, OtherType>);


    const auto test = [](const i32 before, const i32 after) {
        hud::optional<type> option{ before };
        const bool has_value_before = option.has_value();
        const i32 value_before = option.value().id();
        const u32 copy_assign_count_before = option.value().copy_assign_count();
        const u32 copy_constructor_count_before = option.value().copy_constructor_count();
        const u32 move_assign_count_before = option.value().move_assign_count();
        const u32 move_constructor_count_before = option.value().move_constructor_count();

        OtherType other{ after };

        option = hud::move(other);

        return std::tuple{
            has_value_before,
            value_before,
            copy_assign_count_before,
            copy_constructor_count_before,
            move_assign_count_before,
            move_constructor_count_before,
            option.has_value(),
            option.value().id(),
            option.value().copy_assign_count(),
            option.value().copy_constructor_count(),
            option.value().move_assign_count(),
            option.value().move_constructor_count(),
        };
    };

    // Non constant
    {
        auto result = test(123, 456);
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_EQ(hud::get<1>(result), 123);
        ASSERT_EQ(hud::get<2>(result), 0u);
        ASSERT_EQ(hud::get<3>(result), 0u);
        ASSERT_EQ(hud::get<4>(result), 0u);
        ASSERT_EQ(hud::get<5>(result), 0u);
        ASSERT_TRUE(hud::get<6>(result));
        ASSERT_EQ(hud::get<7>(result), 456);
        ASSERT_EQ(hud::get<8>(result), 0u);
        ASSERT_EQ(hud::get<9>(result), 0u);
        ASSERT_EQ(hud::get<10>(result), 1u);
        ASSERT_EQ(hud::get<11>(result), 0u);

    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_EQ(hud::get<1>(result), 123);
        ASSERT_EQ(hud::get<2>(result), 0u);
        ASSERT_EQ(hud::get<3>(result), 0u);
        ASSERT_EQ(hud::get<4>(result), 0u);
        ASSERT_EQ(hud::get<5>(result), 0u);
        ASSERT_TRUE(hud::get<6>(result));
        ASSERT_EQ(hud::get<7>(result), 456);
        ASSERT_EQ(hud::get<8>(result), 0u);
        ASSERT_EQ(hud::get<9>(result), 0u);
        ASSERT_EQ(hud::get<10>(result), 1u);
        ASSERT_EQ(hud::get<11>(result), 0u);
    }
}

TEST(optional, copy_assign_empty_trivially_copy_assignable_same_type) {


    using type = i32;

    static_assert(hud::is_trivially_copy_constructible_v<type>);
    static_assert(hud::is_trivially_copy_assignable_v<type>);


    const auto test = [](const type value) {
        hud::optional<type> option;
        const bool has_value_before = option.has_value();

        const hud::optional<type> other{ value };

        option = other;

        return std::tuple{
            has_value_before,
            option.has_value(),
            option.value()
        };
    };


    // Non constant
    {
        auto result = test(123);
        ASSERT_FALSE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_EQ(hud::get<2>(result), 123);
    }

    // Non constant
    {
        constexpr auto result = test(123);
        ASSERT_FALSE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_EQ(hud::get<2>(result), 123);
    }
}

TEST(optional, copy_assign_empty_trivially_copy_assignable_different_type) {


    using type = i32;
    using OtherType = i16;

    static_assert(hud::is_trivially_copy_constructible_v<type, OtherType>);
    static_assert(hud::is_trivially_copy_assignable_v<type, OtherType>);
    static_assert(hud::is_not_same_v<type, OtherType>);

    const auto test = [](const OtherType value) {
        hud::optional<type> option;
        const bool has_value_before = option.has_value();

        const hud::optional<OtherType> other{ value };

        option = other;

        return std::tuple{
            has_value_before,
            option.has_value(),
            option.value()
        };
    };


    // Non constant
    {
        auto result = test(123);
        ASSERT_FALSE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_EQ(hud::get<2>(result), 123);
    }

    // Non constant
    {
        constexpr auto result = test(123);
        ASSERT_FALSE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_EQ(hud::get<2>(result), 123);
    }
}

TEST(optional, copy_assign_empty_non_trivially_copy_assignable_same_type) {


    using type = hud::test::NonBitwiseType;

    static_assert(!hud::is_trivially_copy_constructible_v<type>);
    static_assert(!hud::is_trivially_copy_assignable_v<type>);


    const auto test = [](const i32 value) {
        i32 destructor_count_other{ 0 };
        hud::optional<type> option;
        const bool has_value_before = option.has_value();

        const hud::optional<type> other{ hud::in_place, value,  &destructor_count_other };

        option = other;

        return std::tuple{
            has_value_before,
            option.has_value(),
            option.value().id(),
            option.value().move_assign_count(),
            option.value().copy_assign_count(),
            option.value().constructor_count(),
            option.value().move_constructor_count(),
            option.value().copy_constructor_count(),
            destructor_count_other
        };
    };

    // Non constant
    {
        auto result = test(123);
        ASSERT_FALSE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_EQ(hud::get<2>(result), 123);
        ASSERT_EQ(hud::get<3>(result), 0u);
        ASSERT_EQ(hud::get<4>(result), 0u);
        ASSERT_EQ(hud::get<5>(result), 1u);
        ASSERT_EQ(hud::get<6>(result), 0u);
        ASSERT_EQ(hud::get<7>(result), 1u);
        ASSERT_EQ(hud::get<8>(result), 0);
    }


    // Constant
    {
        constexpr auto result = test(123);
        ASSERT_FALSE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_EQ(hud::get<2>(result), 123);
        ASSERT_EQ(hud::get<3>(result), 0u);
        ASSERT_EQ(hud::get<4>(result), 0u);
        ASSERT_EQ(hud::get<5>(result), 1u);
        ASSERT_EQ(hud::get<6>(result), 0u);
        ASSERT_EQ(hud::get<7>(result), 1u);
        ASSERT_EQ(hud::get<8>(result), 0);
    }

}

TEST(optional, copy_assign_empty_non_trivially_copy_assignable_different_type) {


    using type = hud::test::NonBitwiseCopyAssignableType2;
    using OtherType = hud::test::NonBitwiseCopyAssignableType;

    static_assert(!hud::is_trivially_copy_constructible_v<type, OtherType>);
    static_assert(!hud::is_trivially_copy_assignable_v<type, OtherType>);
    static_assert(hud::is_not_same_v<type, OtherType>);

    const auto test = [](const i32 value) {
        hud::optional<type> option;
        const bool had_value_before = option.has_value();

        const hud::optional<OtherType> other{ value };

        option = other;

        return std::tuple{
            had_value_before,
            option.has_value(),
            option.value().id(),
            option.value().copy_assign_count(),
            option.value().copy_constructor_count(),
        };
    };

    // Non constant
    {
        const auto result = test(123);
        ASSERT_FALSE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_EQ(hud::get<2>(result), 123);
        ASSERT_EQ(hud::get<3>(result), 0u);
        ASSERT_EQ(hud::get<4>(result), 1u);
    }

    // Constant
    {
        constexpr auto result = test(123);
        ASSERT_FALSE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_EQ(hud::get<2>(result), 123);
        ASSERT_EQ(hud::get<3>(result), 0u);
        ASSERT_EQ(hud::get<4>(result), 1u);
    }

}

TEST(optional, copy_assign_non_empty_trivially_copy_assignable_same_type) {


    using type = i32;

    static_assert(hud::is_trivially_copy_constructible_v<type>);
    static_assert(hud::is_trivially_copy_assignable_v<type>);

    const auto test = [](const type before, const type after) {
        hud::optional<type> option{ before };
        const bool had_value_before = option.has_value();
        const type value_before = option.value();

        const hud::optional<type> other{ after };
        option = other;
        return std::tuple{
            had_value_before,
            value_before,
            option.has_value(),
            option.value()
        };
    };

    // Non constant
    {
        const auto result = test(123,456);
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_EQ(hud::get<1>(result), 123);
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_EQ(hud::get<3>(result), 456);
    }


    // Constant
    {
        constexpr auto result = test(123, 456);
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_EQ(hud::get<1>(result), 123);
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_EQ(hud::get<3>(result), 456);
    }
}

TEST(optional, copy_assign_non_empty_trivially_copy_assignable_different_type) {


    using type = i32;
    using OtherType = i16;

    static_assert(hud::is_trivially_copy_constructible_v<type, OtherType>);
    static_assert(hud::is_trivially_copy_assignable_v<type, OtherType>);
    static_assert(hud::is_not_same_v<type, OtherType>);


    const auto test = [](const type before, const OtherType after) {
        hud::optional<type> option{ before };
        const bool had_value_before = option.has_value();
        const type value_before = option.value();

        const hud::optional<OtherType> other{ after };
        option = other;
        return std::tuple{
            had_value_before,
            value_before,
            option.has_value(),
            option.value()
        };
    };

    // Non constant
    {
        const auto result = test(123, 456);
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_EQ(hud::get<1>(result), 123);
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_EQ(hud::get<3>(result), 456);
    }


    // Constant
    {
        constexpr auto result = test(123, 456);
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_EQ(hud::get<1>(result), 123);
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_EQ(hud::get<3>(result), 456);
    }
}

TEST(optional, copy_assign_non_empty_non_trivially_copy_assignable_same_type) {


    using type = hud::test::NonBitwiseType;
    static_assert(!hud::is_trivially_copy_constructible_v<type>);
    static_assert(!hud::is_trivially_copy_assignable_v<type>);

    const auto test = [](const i32 before, const i32 after) {
        i32 destructor_count;
        i32 other_destructor_count;
        hud::optional<type> option{ hud::in_place, before, &destructor_count };

        const bool has_value_before = option.has_value();
        const i32 id_before = option.value().id();
        const u32 move_assign_count_before = option.value().move_assign_count();
        const u32 copy_assign_count_before = option.value().copy_assign_count();
        const u32 constructor_count_before = option.value().constructor_count();
        const u32 move_constructor_count_before = option.value().move_constructor_count();
        const u32 copy_constructor_count_before = option.value().copy_constructor_count();
        const i32 destructor_count_before = destructor_count;

        const hud::optional<type> other{ hud::in_place, after,  &other_destructor_count };
        option = other;

        return std::tuple{
            has_value_before,
            id_before,
            move_assign_count_before,
            copy_assign_count_before,
            constructor_count_before,
            move_constructor_count_before,
            copy_constructor_count_before,
            destructor_count_before,
            option.has_value(),
            option.value().id(),
            option.value().move_assign_count(),
            option.value().copy_assign_count(),
            option.value().constructor_count(),
            option.value().move_constructor_count(),
            option.value().copy_constructor_count(),
            destructor_count,
            other_destructor_count,
        };

    };

    // Non constant
    {
        auto result = test(123, 456);
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_EQ(hud::get<1>(result), 123);
        ASSERT_EQ(hud::get<2>(result), 0u);
        ASSERT_EQ(hud::get<3>(result), 0u);
        ASSERT_EQ(hud::get<4>(result), 1u);
        ASSERT_EQ(hud::get<5>(result), 0u);
        ASSERT_EQ(hud::get<6>(result), 0u);
        ASSERT_EQ(hud::get<7>(result), 0);
        ASSERT_TRUE(hud::get<8>(result));
        ASSERT_EQ(hud::get<9>(result), 456);
        ASSERT_EQ(hud::get<10>(result), 0u);
        ASSERT_EQ(hud::get<11>(result), 1u);
        ASSERT_EQ(hud::get<12>(result), 1u);
        ASSERT_EQ(hud::get<13>(result), 0u);
        ASSERT_EQ(hud::get<14>(result), 0u);
        ASSERT_EQ(hud::get<15>(result), 0);
        ASSERT_EQ(hud::get<16>(result), 0);
    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_EQ(hud::get<1>(result), 123);
        ASSERT_EQ(hud::get<2>(result), 0u);
        ASSERT_EQ(hud::get<3>(result), 0u);
        ASSERT_EQ(hud::get<4>(result), 1u);
        ASSERT_EQ(hud::get<5>(result), 0u);
        ASSERT_EQ(hud::get<6>(result), 0u);
        ASSERT_EQ(hud::get<7>(result), 0);
        ASSERT_TRUE(hud::get<8>(result));
        ASSERT_EQ(hud::get<9>(result), 456);
        ASSERT_EQ(hud::get<10>(result), 0u);
        ASSERT_EQ(hud::get<11>(result), 1u);
        ASSERT_EQ(hud::get<12>(result), 1u);
        ASSERT_EQ(hud::get<13>(result), 0u);
        ASSERT_EQ(hud::get<14>(result), 0u);
        ASSERT_EQ(hud::get<15>(result), 0);
        ASSERT_EQ(hud::get<16>(result), 0);
    }
}

TEST(optional, copy_assign_non_empty_non_trivially_copy_assignable_different_type) {

    using type = hud::test::NonBitwiseCopyAssignableType2;
    using OtherType = hud::test::NonBitwiseCopyAssignableType;

    static_assert(!hud::is_trivially_copy_constructible_v<type, OtherType>);
    static_assert(!hud::is_trivially_copy_assignable_v<type, OtherType>);
    static_assert(hud::is_not_same_v<type, OtherType>);

    const auto test = [](const i32 before, const i32 after) {
        hud::optional<type> option{ before };
        const bool has_value_before = option.has_value();
        const i32 value_before = option.value().id();
        const u32 copy_assign_count_before = option.value().copy_assign_count();
        const u32 copy_constructor_count_before = option.value().copy_constructor_count();

        const hud::optional<OtherType> other{ after };

        option = other;

        return std::tuple{
            has_value_before,
            value_before,
            copy_assign_count_before,
            copy_constructor_count_before,
            option.has_value(),
            option.value().id(),
            option.value().copy_assign_count(),
            option.value().copy_constructor_count()
        };
    };

    // Non constant
    {
        auto result = test(123, 456);
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_EQ(hud::get<1>(result), 123);
        ASSERT_EQ(hud::get<2>(result), 0u);
        ASSERT_EQ(hud::get<3>(result), 0u);
        ASSERT_TRUE(hud::get<4>(result));
        ASSERT_EQ(hud::get<5>(result), 456);
        ASSERT_EQ(hud::get<6>(result), 1u);
        ASSERT_EQ(hud::get<7>(result), 0u);
    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_EQ(hud::get<1>(result), 123);
        ASSERT_EQ(hud::get<2>(result), 0u);
        ASSERT_EQ(hud::get<3>(result), 0u);
        ASSERT_TRUE(hud::get<4>(result));
        ASSERT_EQ(hud::get<5>(result), 456);
        ASSERT_EQ(hud::get<6>(result), 1u);
        ASSERT_EQ(hud::get<7>(result), 0u);
    }
}

TEST(optional, move_assign_empty_trivially_copy_assignable_same_type) {


    using type = i32;

    static_assert(hud::is_trivially_copy_constructible_v<type>);
    static_assert(hud::is_trivially_copy_assignable_v<type>);

    const auto test = [](const type value) {
        hud::optional<type> option;
        const bool has_value_before = option.has_value();

        const hud::optional<type> other{ value };

        option = hud::move(other);

        return std::tuple{
            has_value_before,
            option.has_value(),
            option.value()
        };
    };


    // Non constant
    {
        auto result = test(123);
        ASSERT_FALSE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_EQ(hud::get<2>(result), 123);
    }

    // Non constant
    {
        constexpr auto result = test(123);
        ASSERT_FALSE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_EQ(hud::get<2>(result), 123);
    }
}

TEST(optional, move_assign_empty_non_trivially_copy_assignable_same_type) {


    using type = hud::test::NonBitwiseCopyAssignableType;

    static_assert(!hud::is_trivially_copy_constructible_v<type>);
    static_assert(!hud::is_trivially_copy_assignable_v<type>);

    const auto test = [](const i32 value) {
        hud::optional<type> option;
        const bool has_value_before = option.has_value();

        const hud::optional<type> other{ hud::in_place, value};

        option = hud::move(other);

        return std::tuple{
            has_value_before,
            option.has_value(),
            option.value().id(),
            option.value().copy_assign_count(),
            option.value().copy_constructor_count(),
        };
    };

    // Non constant
    {
        auto result = test(123);
        ASSERT_FALSE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_EQ(hud::get<2>(result), 123);
        ASSERT_EQ(hud::get<3>(result), 0u);
        ASSERT_EQ(hud::get<4>(result), 1u);
    }


    // Constant
    {
        constexpr auto result = test(123);
        ASSERT_FALSE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_EQ(hud::get<2>(result), 123);
        ASSERT_EQ(hud::get<3>(result), 0u);
        ASSERT_EQ(hud::get<4>(result), 1u);
    }
}

TEST(optional, move_assign_empty_trivially_copy_assignable_different_type) {


    using type = i32;
    using OtherType = i16;

    static_assert(hud::is_trivially_copy_constructible_v<type, OtherType>);
    static_assert(hud::is_trivially_copy_assignable_v<type, OtherType>);
    static_assert(hud::is_not_same_v<type, OtherType>);

    const auto test = [](const OtherType value) {
        hud::optional<type> option;
        const bool has_value_before = option.has_value();

        hud::optional<OtherType> other{ value };

        option = hud::move(other);

        return std::tuple{
            has_value_before,
            option.has_value(),
            option.value()
        };
    };


    // Non constant
    {
        auto result = test(123);
        ASSERT_FALSE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_EQ(hud::get<2>(result), 123);
    }

    // Non constant
    {
        constexpr auto result = test(123);
        ASSERT_FALSE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_EQ(hud::get<2>(result), 123);
    }
}

TEST(optional, move_assign_empty_non_trivially_copy_assignable_different_type) {


    using type = hud::test::NonBitwiseCopyAssignableType2;
    using OtherType = hud::test::NonBitwiseCopyAssignableType;

    static_assert(!hud::is_trivially_copy_constructible_v<type>);
    static_assert(!hud::is_trivially_copy_assignable_v<type>);
    static_assert(hud::is_not_same_v<type, OtherType>);

    const auto test = [](const i32 value) {
        hud::optional<type> option;
        const bool had_value_before = option.has_value();

        const hud::optional<OtherType> other{ value };

        option = hud::move(other);

        return std::tuple{
            had_value_before,
            option.has_value(),
            option.value().id(),
            option.value().copy_assign_count(),
            option.value().copy_constructor_count(),
        };
    };

    // Non constant
    {
        const auto result = test(123);
        ASSERT_FALSE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_EQ(hud::get<2>(result), 123);
        ASSERT_EQ(hud::get<3>(result), 0u);
        ASSERT_EQ(hud::get<4>(result), 1u);
    }

    // Constant
    {
        constexpr auto result = test(123);
        ASSERT_FALSE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_EQ(hud::get<2>(result), 123);
        ASSERT_EQ(hud::get<3>(result), 0u);
        ASSERT_EQ(hud::get<4>(result), 1u);
    }
}

TEST(optional, move_assign_empty_trivially_move_assignable_same_type) {


    using type = i32;

    static_assert(hud::is_trivially_move_constructible_v<type>);
    static_assert(hud::is_trivially_move_assignable_v<type>);

    const auto test = [](const type value) {
        hud::optional<type> option;
        const bool has_value_before = option.has_value();

        hud::optional<type> other{ value };

        option = hud::move(other);

        return std::tuple{
            has_value_before,
            option.has_value(),
            option.value()
        };
    };


    // Non constant
    {
        auto result = test(123);
        ASSERT_FALSE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_EQ(hud::get<2>(result), 123);
    }

    // Non constant
    {
        constexpr auto result = test(123);
        ASSERT_FALSE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_EQ(hud::get<2>(result), 123);
    }
}

TEST(optional, move_assign_empty_non_trivially_move_assignable_same_type) {


    using type = hud::test::NonBitwiseMoveAssignableType;

    static_assert(!hud::is_trivially_move_constructible_v<type>);
    static_assert(!hud::is_trivially_move_assignable_v<type>);

    const auto test = [](const i32 value) {
        hud::optional<type> option;
        const bool has_value_before = option.has_value();

        hud::optional<type> other{ hud::in_place, value };

        option = hud::move(other);

        return std::tuple{
            has_value_before,
            option.has_value(),
            option.value().id(),
            option.value().copy_assign_count(),
            option.value().copy_constructor_count(),
            option.value().move_assign_count(),
            option.value().move_constructor_count(),
        };
    };

    // Non constant
    {
        auto result = test(123);
        ASSERT_FALSE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_EQ(hud::get<2>(result), 123);
        ASSERT_EQ(hud::get<3>(result), 0u);
        ASSERT_EQ(hud::get<4>(result), 0u);
        ASSERT_EQ(hud::get<5>(result), 0u);
        ASSERT_EQ(hud::get<6>(result), 1u);
    }


    // Constant
    {
        constexpr auto result = test(123);
        ASSERT_FALSE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_EQ(hud::get<2>(result), 123);
        ASSERT_EQ(hud::get<3>(result), 0u);
        ASSERT_EQ(hud::get<4>(result), 0u);
        ASSERT_EQ(hud::get<5>(result), 0u);
        ASSERT_EQ(hud::get<6>(result), 1u);
    }
}

TEST(optional, move_assign_empty_trivially_move_assignable_different_type) {


    using type = i32;
    using OtherType = i16;

    static_assert(hud::is_trivially_move_constructible_v<type, OtherType>);
    static_assert(hud::is_trivially_move_assignable_v<type, OtherType>);
    static_assert(hud::is_not_same_v<type, OtherType>);

    const auto test = [](const OtherType value) {
        hud::optional<type> option;
        const bool has_value_before = option.has_value();

        hud::optional<OtherType> other{ value };

        option = hud::move(other);

        return std::tuple{
            has_value_before,
            option.has_value(),
            option.value()
        };
    };


    // Non constant
    {
        auto result = test(123);
        ASSERT_FALSE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_EQ(hud::get<2>(result), 123);
    }

    // Non constant
    {
        constexpr auto result = test(123);
        ASSERT_FALSE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_EQ(hud::get<2>(result), 123);
    }
}

TEST(optional, move_assign_empty_non_trivially_move_assignable_different_type) {


    using type = hud::test::NonBitwiseMoveAssignableType2;
    using OtherType = hud::test::NonBitwiseMoveAssignableType;

    static_assert(!hud::is_trivially_move_constructible_v<type>);
    static_assert(!hud::is_trivially_move_assignable_v<type>);
    static_assert(hud::is_not_same_v<type, OtherType>);

    const auto test = [](const i32 value) {
        hud::optional<type> option;
        const bool had_value_before = option.has_value();

        hud::optional<OtherType> other{ value };

        option = hud::move(other);

        return std::tuple{
            had_value_before,
            option.has_value(),
            option.value().id(),
            option.value().copy_assign_count(),
            option.value().copy_constructor_count(),
            option.value().move_assign_count(),
            option.value().move_constructor_count()
        };
    };

    // Non constant
    {
        const auto result = test(123);
        ASSERT_FALSE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_EQ(hud::get<2>(result), 123);
        ASSERT_EQ(hud::get<3>(result), 0u);
        ASSERT_EQ(hud::get<4>(result), 0u);
        ASSERT_EQ(hud::get<5>(result), 0u);
        ASSERT_EQ(hud::get<6>(result), 1u);
    }

    // Constant
    {
        constexpr auto result = test(123);
        ASSERT_FALSE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_EQ(hud::get<2>(result), 123);
        ASSERT_EQ(hud::get<3>(result), 0u);
        ASSERT_EQ(hud::get<4>(result), 0u);
        ASSERT_EQ(hud::get<5>(result), 0u);
        ASSERT_EQ(hud::get<6>(result), 1u);
    }
}

TEST(optional, move_assign_non_empty_trivially_copy_assignable_same_type) {


    using type = i32;

    static_assert(hud::is_trivially_copy_constructible_v<type>);
    static_assert(hud::is_trivially_copy_assignable_v<type>);

    hud::optional<type> option{ 123 };
    ASSERT_TRUE(option.has_value());
    ASSERT_EQ(option.value(), 123);

    hud::optional<type> other{ 456 };
    option = hud::move(other);

    ASSERT_TRUE(option.has_value());
    ASSERT_EQ(option.value(), 456);
}

TEST(optional, move_assign_non_empty_non_trivially_copy_assignable_same_type) {


    using type = hud::test::NonBitwiseCopyAssignableType;
    static_assert(!hud::is_trivially_copy_constructible_v<type>);
    static_assert(!hud::is_trivially_copy_assignable_v<type>);

    hud::optional<type> option{ 123 };
    ASSERT_TRUE(option.has_value());
    ASSERT_EQ(option.value().id(), 123);
    ASSERT_EQ(option.value().copy_assign_count(), 0u);
    ASSERT_EQ(option.value().copy_constructor_count(), 0u);

    hud::optional<type> other{ 456 };
    option = hud::move(other);

    ASSERT_TRUE(option.has_value());
    ASSERT_EQ(option.value().id(), 456);
    ASSERT_EQ(option.value().copy_assign_count(), 1u);
    ASSERT_EQ(option.value().copy_constructor_count(), 0u);
}

TEST(optional, move_assign_non_empty_trivially_copy_assignable_different_type) {


    using type = i32;
    using OtherType = i16;

    static_assert(hud::is_trivially_copy_constructible_v<type, OtherType>);
    static_assert(hud::is_trivially_copy_assignable_v<type, OtherType>);
    static_assert(hud::is_not_same_v<type, OtherType>);


    const auto test = [](const type before, const OtherType after) {
        hud::optional<type> option{ before };
        const bool had_value_before = option.has_value();
        const type value_before = option.value();

        hud::optional<OtherType> other{ after };

        option = hud::move(other);

        return std::tuple{
            had_value_before,
            value_before,
            option.has_value(),
            option.value()
        };
    };

    // Non constant
    {
        const auto result = test(123, 456);
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_EQ(hud::get<1>(result), 123);
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_EQ(hud::get<3>(result), 456);
    }


    // Constant
    {
        constexpr auto result = test(123, 456);
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_EQ(hud::get<1>(result), 123);
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_EQ(hud::get<3>(result), 456);
    }
}

TEST(optional, move_assign_non_empty_non_trivially_copy_assignable_different_type) {

    using type = hud::test::NonBitwiseCopyAssignableType2;
    using OtherType = hud::test::NonBitwiseCopyAssignableType;

    static_assert(!hud::is_trivially_copy_constructible_v<type, OtherType>);
    static_assert(!hud::is_trivially_copy_assignable_v<type, OtherType>);
    static_assert(hud::is_not_same_v<type, OtherType>);

    const auto test = [](const i32 before, const i32 after) {
        hud::optional<type> option{ before };
        const bool has_value_before = option.has_value();
        const i32 value_before = option.value().id();
        const u32 copy_assign_count_before = option.value().copy_assign_count();
        const u32 copy_constructor_count_before = option.value().copy_constructor_count();

        hud::optional<OtherType> other{ after };

        option = hud::move(other);

        return std::tuple{
            has_value_before,
            value_before,
            copy_assign_count_before,
            copy_constructor_count_before,
            option.has_value(),
            option.value().id(),
            option.value().copy_assign_count(),
            option.value().copy_constructor_count()
        };
    };

    // Non constant
    {
        auto result = test(123, 456);
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_EQ(hud::get<1>(result), 123);
        ASSERT_EQ(hud::get<2>(result), 0u);
        ASSERT_EQ(hud::get<3>(result), 0u);
        ASSERT_TRUE(hud::get<4>(result));
        ASSERT_EQ(hud::get<5>(result), 456);
        ASSERT_EQ(hud::get<6>(result), 1u);
        ASSERT_EQ(hud::get<7>(result), 0u);
    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_EQ(hud::get<1>(result), 123);
        ASSERT_EQ(hud::get<2>(result), 0u);
        ASSERT_EQ(hud::get<3>(result), 0u);
        ASSERT_TRUE(hud::get<4>(result));
        ASSERT_EQ(hud::get<5>(result), 456);
        ASSERT_EQ(hud::get<6>(result), 1u);
        ASSERT_EQ(hud::get<7>(result), 0u);
    }
}

TEST(optional, move_assign_non_empty_trivially_move_assignable_same_type) {


    using type = i32;

    static_assert(hud::is_trivially_move_constructible_v<type>);
    static_assert(hud::is_trivially_move_assignable_v<type>);

    const auto test = [](const type before, const type after) {
        hud::optional<type> option{ before };
        const bool had_value_before = option.has_value();
        const type value_before = option.value();

        hud::optional<type> other{ after };

        option = hud::move(other);

        return std::tuple{
            had_value_before,
            value_before,
            option.has_value(),
            option.value()
        };
    };

    // Non constant
    {
        const auto result = test(123, 456);
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_EQ(hud::get<1>(result), 123);
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_EQ(hud::get<3>(result), 456);
    }


    // Constant
    {
        constexpr auto result = test(123, 456);
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_EQ(hud::get<1>(result), 123);
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_EQ(hud::get<3>(result), 456);
    }
}

TEST(optional, move_assign_non_empty_non_trivially_move_assignable_same_type) {


    using type = hud::test::NonBitwiseType;
    static_assert(!hud::is_trivially_copy_constructible_v<type>);
    static_assert(!hud::is_trivially_copy_assignable_v<type>);

    const auto test = [](const i32 before, const i32 after) {
        i32 destructor_count;
        i32 other_destructor_count;
        hud::optional<type> option{ hud::in_place, before, &destructor_count };

        const bool has_value_before = option.has_value();
        const i32 id_before = option.value().id();
        const u32 move_assign_count_before = option.value().move_assign_count();
        const u32 copy_assign_count_before = option.value().copy_assign_count();
        const u32 constructor_count_before = option.value().constructor_count();
        const u32 move_constructor_count_before = option.value().move_constructor_count();
        const u32 copy_constructor_count_before = option.value().copy_constructor_count();
        const i32 destructor_count_before = destructor_count;

        hud::optional<type> other{ hud::in_place, after,  &other_destructor_count };
        option = hud::move(other);

        return std::tuple{
            has_value_before,
            id_before,
            move_assign_count_before,
            copy_assign_count_before,
            constructor_count_before,
            move_constructor_count_before,
            copy_constructor_count_before,
            destructor_count_before,
            option.has_value(),
            option.value().id(),
            option.value().move_assign_count(),
            option.value().copy_assign_count(),
            option.value().constructor_count(),
            option.value().move_constructor_count(),
            option.value().copy_constructor_count(),
            destructor_count,
            other_destructor_count,
        };

    };

    // Non constant
    {
        auto result = test(123, 456);
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_EQ(hud::get<1>(result), 123);
        ASSERT_EQ(hud::get<2>(result), 0u);
        ASSERT_EQ(hud::get<3>(result), 0u);
        ASSERT_EQ(hud::get<4>(result), 1u);
        ASSERT_EQ(hud::get<5>(result), 0u);
        ASSERT_EQ(hud::get<6>(result), 0u);
        ASSERT_EQ(hud::get<7>(result), 0);
        ASSERT_TRUE(hud::get<8>(result));
        ASSERT_EQ(hud::get<9>(result), 456);
        ASSERT_EQ(hud::get<10>(result), 1u);
        ASSERT_EQ(hud::get<11>(result), 0u);
        ASSERT_EQ(hud::get<12>(result), 1u);
        ASSERT_EQ(hud::get<13>(result), 0u);
        ASSERT_EQ(hud::get<14>(result), 0u);
        ASSERT_EQ(hud::get<15>(result), 0);
        ASSERT_EQ(hud::get<16>(result), 0);
    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_EQ(hud::get<1>(result), 123);
        ASSERT_EQ(hud::get<2>(result), 0u);
        ASSERT_EQ(hud::get<3>(result), 0u);
        ASSERT_EQ(hud::get<4>(result), 1u);
        ASSERT_EQ(hud::get<5>(result), 0u);
        ASSERT_EQ(hud::get<6>(result), 0u);
        ASSERT_EQ(hud::get<7>(result), 0);
        ASSERT_TRUE(hud::get<8>(result));
        ASSERT_EQ(hud::get<9>(result), 456);
        ASSERT_EQ(hud::get<10>(result), 1u);
        ASSERT_EQ(hud::get<11>(result), 0u);
        ASSERT_EQ(hud::get<12>(result), 1u);
        ASSERT_EQ(hud::get<13>(result), 0u);
        ASSERT_EQ(hud::get<14>(result), 0u);
        ASSERT_EQ(hud::get<15>(result), 0);
        ASSERT_EQ(hud::get<16>(result), 0);
    }
}

TEST(optional, move_assign_non_empty_trivially_move_assignable_different_type) {


    using type = i32;
    using OtherType = i16;

    static_assert(hud::is_trivially_move_constructible_v<type, OtherType>);
    static_assert(hud::is_trivially_move_assignable_v<type, OtherType>);
    static_assert(hud::is_not_same_v<type, OtherType>);


    const auto test = [](const type before, const OtherType after) {
        hud::optional<type> option{ before };
        const bool had_value_before = option.has_value();
        const type value_before = option.value();

        hud::optional<OtherType> other{ after };
        option = hud::move(other);
        return std::tuple{
            had_value_before,
            value_before,
            option.has_value(),
            option.value()
        };
    };

    // Non constant
    {
        const auto result = test(123, 456);
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_EQ(hud::get<1>(result), 123);
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_EQ(hud::get<3>(result), 456);
    }


    // Constant
    {
        constexpr auto result = test(123, 456);
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_EQ(hud::get<1>(result), 123);
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_EQ(hud::get<3>(result), 456);
    }
}

TEST(optional, move_assign_non_empty_non_trivially_move_assignable_different_type) {

    using type = hud::test::NonBitwiseMoveAssignableType2;
    using OtherType = hud::test::NonBitwiseMoveAssignableType;

    static_assert(!hud::is_trivially_move_constructible_v<type, OtherType>);
    static_assert(!hud::is_trivially_move_assignable_v<type, OtherType>);
    static_assert(hud::is_not_same_v<type, OtherType>);

    const auto test = [](const i32 before, const i32 after) {
        hud::optional<type> option{ before };
        const bool has_value_before = option.has_value();
        const i32 value_before = option.value().id();
        const u32 copy_assign_count_before = option.value().copy_assign_count();
        const u32 copy_constructor_count_before = option.value().copy_constructor_count();
        const u32 move_assign_count_before = option.value().move_assign_count();
        const u32 move_constructor_count_before = option.value().move_constructor_count();

        hud::optional<OtherType> other{ after };

        option = hud::move(other);

        return std::tuple{
            has_value_before,
            value_before,
            copy_assign_count_before,
            copy_constructor_count_before,
            move_assign_count_before,
            move_constructor_count_before,
            option.has_value(),
            option.value().id(),
            option.value().copy_assign_count(),
            option.value().copy_constructor_count(),
            option.value().move_assign_count(),
            option.value().move_constructor_count()
        };
    };

    // Non constant
    {
        auto result = test(123, 456);
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_EQ(hud::get<1>(result), 123);
        ASSERT_EQ(hud::get<2>(result), 0u);
        ASSERT_EQ(hud::get<3>(result), 0u);
        ASSERT_EQ(hud::get<4>(result), 0u);
        ASSERT_EQ(hud::get<5>(result), 0u);
        ASSERT_TRUE(hud::get<6>(result));
        ASSERT_EQ(hud::get<7>(result), 456);
        ASSERT_EQ(hud::get<8>(result), 0u);
        ASSERT_EQ(hud::get<9>(result), 0u);
        ASSERT_EQ(hud::get<10>(result), 1u);
        ASSERT_EQ(hud::get<11>(result), 0u);
    }

    // Constant
    {
        constexpr auto result = test(123, 456);
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_EQ(hud::get<1>(result), 123);
        ASSERT_EQ(hud::get<2>(result), 0u);
        ASSERT_EQ(hud::get<3>(result), 0u);
        ASSERT_EQ(hud::get<4>(result), 0u);
        ASSERT_EQ(hud::get<5>(result), 0u);
        ASSERT_TRUE(hud::get<6>(result));
        ASSERT_EQ(hud::get<7>(result), 456);
        ASSERT_EQ(hud::get<8>(result), 0u);
        ASSERT_EQ(hud::get<9>(result), 0u);
        ASSERT_EQ(hud::get<10>(result), 1u);
        ASSERT_EQ(hud::get<11>(result), 0u);
    }
}