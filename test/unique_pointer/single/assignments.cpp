#include <core/containers/unique_pointer.h>

namespace{
    template<typename type_t>
    struct CustomDeleter
        : public hud::default_deleter<type_t>
        , hud::test::NonBitwiseType {

        constexpr CustomDeleter() noexcept = default;
        constexpr CustomDeleter(const CustomDeleter& other) noexcept = default;
        constexpr CustomDeleter(CustomDeleter&& other) noexcept = default;
        constexpr CustomDeleter(hud::default_deleter<type_t>&& other) noexcept
            : hud::default_deleter<type_t>(hud::move(other))
            , hud::test::NonBitwiseType(hud::move(other)) {
        }
        template<typename U>
        constexpr CustomDeleter(CustomDeleter<U>&& other) noexcept
            : hud::default_deleter<type_t>(hud::move(other))
            , hud::test::NonBitwiseType(hud::move(other)) {
        }
        constexpr CustomDeleter& operator=(const CustomDeleter&) noexcept {
            return *this;
        }
        constexpr CustomDeleter& operator=(CustomDeleter&&) noexcept {
            return *this;
        }
    };

    using DeleterType = CustomDeleter<hud::test::NonBitwiseType>;

    template<typename type_t>
    struct CustomDeleter2 : public CustomDeleter<type_t> {
    };
    using DeleterType2 = CustomDeleter<hud::test::NonBitwiseType>;
}

TEST(UniquePointer, assign_by_move_same_type) {
    const auto test = []() {
        hud::test::NonBitwiseType* pi = new hud::test::NonBitwiseType(123, nullptr);
        DeleterType other_deleter;
        DeleterType deleter;
        hud::UniquePointer<hud::test::NonBitwiseType, DeleterType> other(pi, other_deleter);
        hud::UniquePointer<hud::test::NonBitwiseType, DeleterType> p(nullptr, deleter);
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
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
        ASSERT_TRUE(hud::get<4>(result));
        ASSERT_TRUE(hud::get<5>(result));
        ASSERT_TRUE(hud::get<6>(result));
        ASSERT_TRUE(hud::get<7>(result));
        ASSERT_TRUE(hud::get<8>(result));
        ASSERT_TRUE(hud::get<9>(result));
        ASSERT_TRUE(hud::get<10>(result));
        ASSERT_TRUE(hud::get<11>(result));
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
        ASSERT_TRUE(hud::get<6>(result));
        ASSERT_TRUE(hud::get<7>(result));
        ASSERT_TRUE(hud::get<8>(result));
        ASSERT_TRUE(hud::get<9>(result));
        ASSERT_TRUE(hud::get<10>(result));
        ASSERT_TRUE(hud::get<11>(result));
    }
}


TEST(UniquePointer, assign_by_move_same_type_with_deleter_ref) {
    const auto test = []() {
        hud::test::NonBitwiseType* pi = new hud::test::NonBitwiseType(123, nullptr);
        DeleterType other_deleter;
        DeleterType deleter;
        hud::UniquePointer<hud::test::NonBitwiseType, DeleterType&> other(pi, other_deleter);
        hud::UniquePointer<hud::test::NonBitwiseType, DeleterType&> p(nullptr, deleter);
        p = hud::move(other);
        return std::tuple{
            other.pointer() == nullptr,
            p.pointer() == pi,
            & (other.deleter()) == &other_deleter,
            & (p.deleter()) == &deleter,
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
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
        ASSERT_TRUE(hud::get<4>(result));
        ASSERT_TRUE(hud::get<5>(result));
        ASSERT_TRUE(hud::get<6>(result));
        ASSERT_TRUE(hud::get<7>(result));
        ASSERT_TRUE(hud::get<8>(result));
        ASSERT_TRUE(hud::get<9>(result));
        ASSERT_TRUE(hud::get<10>(result));
        ASSERT_TRUE(hud::get<11>(result));
        ASSERT_TRUE(hud::get<12>(result));
        ASSERT_TRUE(hud::get<13>(result));
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
        ASSERT_TRUE(hud::get<6>(result));
        ASSERT_TRUE(hud::get<7>(result));
        ASSERT_TRUE(hud::get<8>(result));
        ASSERT_TRUE(hud::get<9>(result));
        ASSERT_TRUE(hud::get<10>(result));
        ASSERT_TRUE(hud::get<11>(result));
        ASSERT_TRUE(hud::get<12>(result));
        ASSERT_TRUE(hud::get<13>(result));
    }
}

TEST(UniquePointer, assign_by_move_same_type_with_different_deleter_ref) {
    const auto test = []() {
        hud::test::NonBitwiseType* pi = new hud::test::NonBitwiseType(123, nullptr);
        DeleterType2 other_deleter;
        DeleterType deleter;
        hud::UniquePointer<hud::test::NonBitwiseType, DeleterType2&> other(pi, other_deleter);
        hud::UniquePointer<hud::test::NonBitwiseType, DeleterType&> p(nullptr, deleter);
        p = hud::move(other);
        return std::tuple{
            other.pointer() == nullptr,
            p.pointer() == pi,
            & (other.deleter()) == &other_deleter,
            & (p.deleter()) == &deleter,
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
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
        ASSERT_TRUE(hud::get<4>(result));
        ASSERT_TRUE(hud::get<5>(result));
        ASSERT_TRUE(hud::get<6>(result));
        ASSERT_TRUE(hud::get<7>(result));
        ASSERT_TRUE(hud::get<8>(result));
        ASSERT_TRUE(hud::get<9>(result));
        ASSERT_TRUE(hud::get<10>(result));
        ASSERT_TRUE(hud::get<11>(result));
        ASSERT_TRUE(hud::get<12>(result));
        ASSERT_TRUE(hud::get<13>(result));
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
        ASSERT_TRUE(hud::get<6>(result));
        ASSERT_TRUE(hud::get<7>(result));
        ASSERT_TRUE(hud::get<8>(result));
        ASSERT_TRUE(hud::get<9>(result));
        ASSERT_TRUE(hud::get<10>(result));
        ASSERT_TRUE(hud::get<11>(result));
        ASSERT_TRUE(hud::get<12>(result));
        ASSERT_TRUE(hud::get<13>(result));
    }
}

TEST(UniquePointer, assign_by_move_different_type) {
    const auto test = []() {
        hud::test::NonBitwiseType2* pi = new hud::test::NonBitwiseType2(123, nullptr);
        DeleterType other_deleter;
        DeleterType deleter;
        hud::UniquePointer<hud::test::NonBitwiseType2, DeleterType> other(pi, other_deleter);
        hud::UniquePointer<hud::test::NonBitwiseType, DeleterType> p(nullptr, deleter);
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
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
        ASSERT_TRUE(hud::get<4>(result));
        ASSERT_TRUE(hud::get<5>(result));
        ASSERT_TRUE(hud::get<6>(result));
        ASSERT_TRUE(hud::get<7>(result));
        ASSERT_TRUE(hud::get<8>(result));
        ASSERT_TRUE(hud::get<9>(result));
        ASSERT_TRUE(hud::get<10>(result));
        ASSERT_TRUE(hud::get<11>(result));
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
        ASSERT_TRUE(hud::get<6>(result));
        ASSERT_TRUE(hud::get<7>(result));
        ASSERT_TRUE(hud::get<8>(result));
        ASSERT_TRUE(hud::get<9>(result));
        ASSERT_TRUE(hud::get<10>(result));
        ASSERT_TRUE(hud::get<11>(result));
    }
}

TEST(UniquePointer, assign_by_move_different_type_different_deleter) {
    const auto test = []() {
        hud::test::NonBitwiseType2* pi = new hud::test::NonBitwiseType2(123, nullptr);
        DeleterType2 other_deleter;
        DeleterType deleter;
        hud::UniquePointer<hud::test::NonBitwiseType2, DeleterType2> other(pi, other_deleter);
        hud::UniquePointer<hud::test::NonBitwiseType, DeleterType> p(nullptr, deleter);
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
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
        ASSERT_TRUE(hud::get<4>(result));
        ASSERT_TRUE(hud::get<5>(result));
        ASSERT_TRUE(hud::get<6>(result));
        ASSERT_TRUE(hud::get<7>(result));
        ASSERT_TRUE(hud::get<8>(result));
        ASSERT_TRUE(hud::get<9>(result));
        ASSERT_TRUE(hud::get<10>(result));
        ASSERT_TRUE(hud::get<11>(result));
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
        ASSERT_TRUE(hud::get<6>(result));
        ASSERT_TRUE(hud::get<7>(result));
        ASSERT_TRUE(hud::get<8>(result));
        ASSERT_TRUE(hud::get<9>(result));
        ASSERT_TRUE(hud::get<10>(result));
        ASSERT_TRUE(hud::get<11>(result));
    }
}

TEST(UniquePointer, assign_by_move_different_type_with_deleter_ref) {
    const auto test = []() {
        hud::test::NonBitwiseType2* pi = new hud::test::NonBitwiseType2(123, nullptr);
        DeleterType other_deleter;
        DeleterType deleter;
        hud::UniquePointer<hud::test::NonBitwiseType2, DeleterType&> other(pi, other_deleter);
        hud::UniquePointer<hud::test::NonBitwiseType, DeleterType&> p(nullptr, deleter);
        p = hud::move(other);
        return std::tuple{
            other.pointer() == nullptr,
            p.pointer() == pi,
            & (other.deleter()) == &other_deleter,
            & (p.deleter()) == &deleter,
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
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
        ASSERT_TRUE(hud::get<4>(result));
        ASSERT_TRUE(hud::get<5>(result));
        ASSERT_TRUE(hud::get<6>(result));
        ASSERT_TRUE(hud::get<7>(result));
        ASSERT_TRUE(hud::get<8>(result));
        ASSERT_TRUE(hud::get<9>(result));
        ASSERT_TRUE(hud::get<10>(result));
        ASSERT_TRUE(hud::get<11>(result));
        ASSERT_TRUE(hud::get<12>(result));
        ASSERT_TRUE(hud::get<13>(result));
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
        ASSERT_TRUE(hud::get<6>(result));
        ASSERT_TRUE(hud::get<7>(result));
        ASSERT_TRUE(hud::get<8>(result));
        ASSERT_TRUE(hud::get<9>(result));
        ASSERT_TRUE(hud::get<10>(result));
        ASSERT_TRUE(hud::get<11>(result));
        ASSERT_TRUE(hud::get<12>(result));
        ASSERT_TRUE(hud::get<13>(result));
    }
}

TEST(UniquePointer, assign_by_move_different_type_with_different_deleter_ref) {
    const auto test = []() {
        hud::test::NonBitwiseType2* pi = new hud::test::NonBitwiseType2(123, nullptr);
        DeleterType2 other_deleter;
        DeleterType deleter;
        hud::UniquePointer<hud::test::NonBitwiseType2, DeleterType2&> other(pi, other_deleter);
        hud::UniquePointer<hud::test::NonBitwiseType, DeleterType&> p(nullptr, deleter);
        p = hud::move(other);
        return std::tuple{
            other.pointer() == nullptr,
            p.pointer() == pi,
            & (other.deleter()) == &other_deleter,
            & (p.deleter()) == &deleter,
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
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
        ASSERT_TRUE(hud::get<4>(result));
        ASSERT_TRUE(hud::get<5>(result));
        ASSERT_TRUE(hud::get<6>(result));
        ASSERT_TRUE(hud::get<7>(result));
        ASSERT_TRUE(hud::get<8>(result));
        ASSERT_TRUE(hud::get<9>(result));
        ASSERT_TRUE(hud::get<10>(result));
        ASSERT_TRUE(hud::get<11>(result));
        ASSERT_TRUE(hud::get<12>(result));
        ASSERT_TRUE(hud::get<13>(result));
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
        ASSERT_TRUE(hud::get<6>(result));
        ASSERT_TRUE(hud::get<7>(result));
        ASSERT_TRUE(hud::get<8>(result));
        ASSERT_TRUE(hud::get<9>(result));
        ASSERT_TRUE(hud::get<10>(result));
        ASSERT_TRUE(hud::get<11>(result));
        ASSERT_TRUE(hud::get<12>(result));
        ASSERT_TRUE(hud::get<13>(result));
    }
}