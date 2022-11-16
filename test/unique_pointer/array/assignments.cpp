#include <core/containers/unique_pointer.h>

namespace hud_test {
    template<typename type_t>
    struct custom_deleter
        : public hud::default_deleter<type_t>
        , hud_test::non_bitwise_type {

        constexpr custom_deleter() noexcept = default;
        constexpr custom_deleter(const custom_deleter& other) noexcept = default;
        constexpr custom_deleter(custom_deleter&& other) noexcept = default;
        constexpr custom_deleter(hud::default_deleter<type_t>&& other) noexcept
            : hud::default_deleter<type_t>(hud::move(other))
            , hud_test::non_bitwise_type(hud::move(other)) {
        }
        template<typename U>
        constexpr custom_deleter(custom_deleter<U>&& other) noexcept
            : hud::default_deleter<type_t>(hud::move(other))
            , hud_test::non_bitwise_type(hud::move(other)) {
        }
        constexpr custom_deleter& operator=(const custom_deleter&) noexcept {
            return *this;
        }
        constexpr custom_deleter& operator=(custom_deleter&&) noexcept {
            return *this;
        }
    };

    using deleter_type = custom_deleter<hud_test::non_bitwise_type[]>;
    using const_deleter_type = custom_deleter<const hud_test::non_bitwise_type[]>;

    template<typename type_t>
    struct custom_deleter2 : public custom_deleter<type_t> {
    };
    using deleter_type2 = custom_deleter2<hud_test::non_bitwise_type[]>;
    using const_deleter_type2 = custom_deleter2<const hud_test::non_bitwise_type[]>;
}

GTEST_TEST(unique_pointer_array, assign_by_move_same_type) {
    const auto test = []() {
        hud_test::non_bitwise_type* pi = new hud_test::non_bitwise_type[2]{ {123, nullptr}, {456, nullptr} };
        hud_test::deleter_type other_deleter;
        hud_test::deleter_type deleter;
        hud::unique_pointer<hud_test::non_bitwise_type[], hud_test::deleter_type> other(pi, other_deleter);
        hud::unique_pointer<hud_test::non_bitwise_type[], hud_test::deleter_type> p(nullptr, deleter);
        
        p = hud::move(other);
        return std::tuple{
            other.pointer() == nullptr,
            p.pointer() == pi,
            other.deleter().constructor_count() == 0u,
            other.deleter().copy_constructor_count() == 1u,
            other.deleter().move_constructor_count() == 0u,
            other.deleter().copy_assign_count() == 0u,
            other.deleter().move_assign_count() == 0u,
            p.deleter().constructor_count() == 0u,
            p.deleter().copy_constructor_count() == 1u,
            p.deleter().move_constructor_count() == 0u,
            p.deleter().copy_assign_count() == 0u,
            p.deleter().move_assign_count() == 0u,
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
        ASSERT_TRUE(std::get<6>(result));
        ASSERT_TRUE(std::get<7>(result));
        ASSERT_TRUE(std::get<8>(result));
        ASSERT_TRUE(std::get<9>(result));
        ASSERT_TRUE(std::get<10>(result));
        ASSERT_TRUE(std::get<11>(result));
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
        ASSERT_TRUE(std::get<6>(result));
        ASSERT_TRUE(std::get<7>(result));
        ASSERT_TRUE(std::get<8>(result));
        ASSERT_TRUE(std::get<9>(result));
        ASSERT_TRUE(std::get<10>(result));
        ASSERT_TRUE(std::get<11>(result));
    }
}


GTEST_TEST(unique_pointer_array, assign_by_move_same_type_with_deleter_ref) {
    const auto test = []() {
        hud_test::non_bitwise_type* pi = new hud_test::non_bitwise_type[2]{ {123, nullptr}, {456, nullptr} };
        hud_test::deleter_type other_deleter;
        hud_test::deleter_type deleter;
        hud::unique_pointer<hud_test::non_bitwise_type[], hud_test::deleter_type&> other(pi, other_deleter);
        hud::unique_pointer<hud_test::non_bitwise_type[], hud_test::deleter_type&> p(nullptr, deleter);
        p = hud::move(other);
        return std::tuple {
            other.pointer() == nullptr,
            p.pointer() == pi,
            &(other.deleter()) == &other_deleter,
            &(p.deleter()) == &deleter,
            other.deleter().constructor_count() == 0u,
            other.deleter().copy_constructor_count() == 0u,
            other.deleter().move_constructor_count() == 0u,
            other.deleter().copy_assign_count() == 0u,
            other.deleter().move_assign_count() == 0u,
            p.deleter().constructor_count() == 0u,
            p.deleter().copy_constructor_count() == 0u,
            p.deleter().move_constructor_count() == 0u,
            p.deleter().copy_assign_count() == 0u,
            p.deleter().move_assign_count() == 0u,
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
        ASSERT_TRUE(std::get<6>(result));
        ASSERT_TRUE(std::get<7>(result));
        ASSERT_TRUE(std::get<8>(result));
        ASSERT_TRUE(std::get<9>(result));
        ASSERT_TRUE(std::get<10>(result));
        ASSERT_TRUE(std::get<11>(result));
        ASSERT_TRUE(std::get<12>(result));
        ASSERT_TRUE(std::get<13>(result));
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
        ASSERT_TRUE(std::get<6>(result));
        ASSERT_TRUE(std::get<7>(result));
        ASSERT_TRUE(std::get<8>(result));
        ASSERT_TRUE(std::get<9>(result));
        ASSERT_TRUE(std::get<10>(result));
        ASSERT_TRUE(std::get<11>(result));
        ASSERT_TRUE(std::get<12>(result));
        ASSERT_TRUE(std::get<13>(result));
    }
}

GTEST_TEST(unique_pointer_array, assign_by_move_same_type_with_different_deleter_ref) {
    const auto test = []() {
        hud_test::non_bitwise_type* pi = new hud_test::non_bitwise_type[2]{ {123, nullptr}, {456, nullptr} };
        hud_test::deleter_type2 other_deleter;
        hud_test::deleter_type deleter;
        hud::unique_pointer<hud_test::non_bitwise_type[], hud_test::deleter_type2&> other(pi, other_deleter);
        hud::unique_pointer<hud_test::non_bitwise_type[], hud_test::deleter_type&> p(nullptr, deleter);
        p = hud::move(other);
        return std::tuple{
            other.pointer() == nullptr,
            p.pointer() == pi,
            &(other.deleter()) == &other_deleter,
            &(p.deleter()) == &deleter,
            other.deleter().constructor_count() == 0u,
            other.deleter().copy_constructor_count() == 0u,
            other.deleter().move_constructor_count() == 0u,
            other.deleter().copy_assign_count() == 0u,
            other.deleter().move_assign_count() == 0u,
            p.deleter().constructor_count() == 0u,
            p.deleter().copy_constructor_count() == 0u,
            p.deleter().move_constructor_count() == 0u,
            p.deleter().copy_assign_count() == 0u,
            p.deleter().move_assign_count() == 0u,
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
        ASSERT_TRUE(std::get<6>(result));
        ASSERT_TRUE(std::get<7>(result));
        ASSERT_TRUE(std::get<8>(result));
        ASSERT_TRUE(std::get<9>(result));
        ASSERT_TRUE(std::get<10>(result));
        ASSERT_TRUE(std::get<11>(result));
        ASSERT_TRUE(std::get<12>(result));
        ASSERT_TRUE(std::get<13>(result));
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
        ASSERT_TRUE(std::get<6>(result));
        ASSERT_TRUE(std::get<7>(result));
        ASSERT_TRUE(std::get<8>(result));
        ASSERT_TRUE(std::get<9>(result));
        ASSERT_TRUE(std::get<10>(result));
        ASSERT_TRUE(std::get<11>(result));
        ASSERT_TRUE(std::get<12>(result));
        ASSERT_TRUE(std::get<13>(result));
    }
}

GTEST_TEST(unique_pointer_array, assign_by_move_different_type) {
    const auto test = []() {
        hud_test::non_bitwise_type* pi = new hud_test::non_bitwise_type[2]{ {123, nullptr}, {456, nullptr} };
        hud_test::deleter_type other_deleter;
        hud_test::const_deleter_type deleter;
        hud::unique_pointer<hud_test::non_bitwise_type[], hud_test::deleter_type> other(pi, other_deleter);
        hud::unique_pointer<const hud_test::non_bitwise_type[], hud_test::const_deleter_type> p(nullptr, deleter);
   
        p = hud::move(other);
        return std::tuple{
            other.pointer() == nullptr,
            p.pointer() == pi,
            other.deleter().constructor_count() == 0u,
            other.deleter().copy_constructor_count() == 1u,
            other.deleter().move_constructor_count() == 0u,
            other.deleter().copy_assign_count() == 0u,
            other.deleter().move_assign_count() == 0u,
            p.deleter().constructor_count() == 0u,
            p.deleter().copy_constructor_count() == 1u,
            p.deleter().move_constructor_count() == 0u,
            p.deleter().copy_assign_count() == 0u,
            p.deleter().move_assign_count() == 0u,
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
        ASSERT_TRUE(std::get<6>(result));
        ASSERT_TRUE(std::get<7>(result));
        ASSERT_TRUE(std::get<8>(result));
        ASSERT_TRUE(std::get<9>(result));
        ASSERT_TRUE(std::get<10>(result));
        ASSERT_TRUE(std::get<11>(result));
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
        ASSERT_TRUE(std::get<6>(result));
        ASSERT_TRUE(std::get<7>(result));
        ASSERT_TRUE(std::get<8>(result));
        ASSERT_TRUE(std::get<9>(result));
        ASSERT_TRUE(std::get<10>(result));
        ASSERT_TRUE(std::get<11>(result));
    }
}