#include <core/containers/unique_pointer.h>


namespace {
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

    using DeleterType = CustomDeleter<hud::test::NonBitwiseType[]>;
    using ConstDeleterType = CustomDeleter<const hud::test::NonBitwiseType[]>;

    template<typename type_t>
    struct CustomDeleter2 : public CustomDeleter<type_t> {
    };
    using DeleterType2 = CustomDeleter2<hud::test::NonBitwiseType[]>;
    using ConstDeleterType2 = CustomDeleter2<const hud::test::NonBitwiseType[]>;
}


TEST(UniquePointer_array, default_constructor) {
    const auto test = []() {
        hud::UniquePointer<hud::test::NonBitwiseType[]> p;
        return p.pointer() == nullptr;
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(result);
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(result);
    }
}

TEST(UniquePointer_array, constructor_with_same_pointer) {

    const auto test = []() {
        hud::test::NonBitwiseType* pi = new hud::test::NonBitwiseType[2]{ {123, nullptr}, {456, nullptr}  }; 
        hud::UniquePointer<hud::test::NonBitwiseType[]> p(pi);
        return std::tuple{
            p.pointer() == pi,
            pi->constructor_count() == 1u,
            pi->copy_constructor_count() == 0u,
            pi->move_constructor_count() == 0u,
            pi->copy_assign_count() == 0u,
            pi->move_assign_count() == 0u
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

TEST(UniquePointer_array, constructor_with_different_pointer) {

    const auto test = []() {
        hud::test::NonBitwiseType* pi = new hud::test::NonBitwiseType[2]{ {123, nullptr}, {456, nullptr} };
        hud::UniquePointer<const hud::test::NonBitwiseType[]> p(pi);
        return std::tuple{
            p.pointer() == pi,
            pi->constructor_count() == 1u,
            pi->copy_constructor_count() == 0u,
            pi->move_constructor_count() == 0u,
            pi->copy_assign_count() == 0u,
            pi->move_assign_count() == 0u
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

TEST(UniquePointer_array, constructor_with_pointer_and_same_deleter_by_copy) {

    const auto test = []() {
        hud::test::NonBitwiseType* pi = new hud::test::NonBitwiseType[2]{ {123, nullptr}, {456, nullptr} };
        DeleterType deleter;
        hud::UniquePointer<hud::test::NonBitwiseType[], DeleterType > p(pi, deleter);
        return std::tuple{
            p.pointer() == pi,
            hud::is_same_v<decltype(p.deleter()), DeleterType&>,
            pi->constructor_count() == 1u,
            pi->copy_constructor_count() == 0u,
            pi->move_constructor_count() == 0u,
            pi->copy_assign_count() == 0u,
            pi->move_assign_count() == 0u,
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

TEST(UniquePointer_array, constructor_with_pointer_and_different_deleter_by_copy) {

    const auto test = []() {
        hud::test::NonBitwiseType* pi = new hud::test::NonBitwiseType[2]{ {123, nullptr}, {456, nullptr} };
        DeleterType2 deleter;
        hud::UniquePointer<hud::test::NonBitwiseType[], DeleterType > p(pi, deleter);
        return std::tuple{
            p.pointer() == pi,
            hud::is_same_v<decltype(p.deleter()), DeleterType&>,
            pi->constructor_count() == 1u,
            pi->copy_constructor_count() == 0u,
            pi->move_constructor_count() == 0u,
            pi->copy_assign_count() == 0u,
            pi->move_assign_count() == 0u,
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


TEST(UniquePointer_array, constructor_with_pointer_and_same_deleter_by_move) {
    const auto test = []() {
        static_assert(hud::is_move_constructible_v<DeleterType>);
        hud::test::NonBitwiseType* pi = new hud::test::NonBitwiseType[2]{ {123, nullptr}, {456, nullptr} };
        hud::UniquePointer<hud::test::NonBitwiseType[], DeleterType > p(pi, DeleterType{});
        return std::tuple{
            p.pointer() == pi,
            hud::is_same_v<decltype(p.deleter()), DeleterType&>,
            pi->constructor_count() == 1u,
            pi->copy_constructor_count() == 0u,
            pi->move_constructor_count() == 0u,
            pi->copy_assign_count() == 0u,
            pi->move_assign_count() == 0u,
            p.deleter().constructor_count() == 0u,
            p.deleter().copy_constructor_count() == 0u,
            p.deleter().move_constructor_count() == 1u,
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

TEST(UniquePointer_array, constructor_with_pointer_and_different_deleter_by_move) {
    const auto test = []() {
        static_assert(hud::is_move_constructible_v<DeleterType>);
        hud::test::NonBitwiseType* pi = new hud::test::NonBitwiseType[2]{ {123, nullptr}, {456, nullptr} };
        hud::UniquePointer<hud::test::NonBitwiseType[], DeleterType > p(pi, DeleterType2{});
        return std::tuple{
            p.pointer() == pi,
            hud::is_same_v<decltype(p.deleter()), DeleterType&>,
            pi->constructor_count() == 1u,
            pi->copy_constructor_count() == 0u,
            pi->move_constructor_count() == 0u,
            pi->copy_assign_count() == 0u,
            pi->move_assign_count() == 0u,
            p.deleter().constructor_count() == 0u,
            p.deleter().copy_constructor_count() == 0u,
            p.deleter().move_constructor_count() == 1u,
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

TEST(UniquePointer_array, constructor_with_pointer_and_ref_same_deleter) {
    const auto test = []() {
        hud::test::NonBitwiseType* pi = new hud::test::NonBitwiseType[2]{ {123, nullptr}, {456, nullptr} };
        DeleterType deleter;
        hud::UniquePointer<hud::test::NonBitwiseType[], DeleterType& > p(pi, deleter);
        return std::tuple{
            p.pointer() == pi,
            &(p.deleter()) == &deleter,
            pi->constructor_count() == 1u,
            pi->copy_constructor_count() == 0u,
            pi->move_constructor_count() == 0u,
            pi->copy_assign_count() == 0u,
            pi->move_assign_count() == 0u,
            p.deleter().constructor_count() == 0u,
            p.deleter().copy_constructor_count() == 0u,
            p.deleter().move_constructor_count() == 0u,
            p.deleter().copy_assign_count() == 0u,
            p.deleter().move_assign_count() == 0u,
            deleter.constructor_count() == 0u,
            deleter.copy_constructor_count() == 0u,
            deleter.move_constructor_count() == 0u,
            deleter.copy_assign_count() == 0u,
            deleter.move_assign_count() == 0u,
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
        ASSERT_TRUE(std::get<14>(result));
        ASSERT_TRUE(std::get<15>(result));
        ASSERT_TRUE(std::get<16>(result));
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
        ASSERT_TRUE(std::get<14>(result));
        ASSERT_TRUE(std::get<15>(result));
        ASSERT_TRUE(std::get<16>(result));
    }
}

TEST(UniquePointer_array, constructor_with_pointer_and_ref_different_deleter) {
    const auto test = []() {
        hud::test::NonBitwiseType* pi = new hud::test::NonBitwiseType[2]{ {123, nullptr}, {456, nullptr} };
        DeleterType2 deleter;
        hud::UniquePointer<hud::test::NonBitwiseType[], DeleterType& > p(pi, deleter);
        return std::tuple{
            p.pointer() == pi,
            &(p.deleter()) == &deleter,
            pi->constructor_count() == 1u,
            pi->copy_constructor_count() == 0u,
            pi->move_constructor_count() == 0u,
            pi->copy_assign_count() == 0u,
            pi->move_assign_count() == 0u,
            p.deleter().constructor_count() == 0u,
            p.deleter().copy_constructor_count() == 0u,
            p.deleter().move_constructor_count() == 0u,
            p.deleter().copy_assign_count() == 0u,
            p.deleter().move_assign_count() == 0u,
            deleter.constructor_count() == 0u,
            deleter.copy_constructor_count() == 0u,
            deleter.move_constructor_count() == 0u,
            deleter.copy_assign_count() == 0u,
            deleter.move_assign_count() == 0u,
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
        ASSERT_TRUE(std::get<14>(result));
        ASSERT_TRUE(std::get<15>(result));
        ASSERT_TRUE(std::get<16>(result));
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
        ASSERT_TRUE(std::get<14>(result));
        ASSERT_TRUE(std::get<15>(result));
        ASSERT_TRUE(std::get<16>(result));
    }
}

TEST(UniquePointer_array, constructor_with_nullptr) {
    const auto test = []() {
        hud::UniquePointer<hud::test::NonBitwiseType[]> p(nullptr);
        return p.pointer() == nullptr;
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(result);
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(result);
    }
}

TEST(UniquePointer_array, constructor_with_nullptr_with_deleter) {
    const auto test = []() {
        DeleterType deleter;
        hud::UniquePointer<hud::test::NonBitwiseType[], DeleterType> p(nullptr, deleter);
        return p.pointer() == nullptr;
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(result);
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(result);
    }
}


TEST(UniquePointer_array, constructor_with_nullptr_with_deleter_ref) {
    const auto test = []() {
        DeleterType deleter;
        hud::UniquePointer<hud::test::NonBitwiseType[], DeleterType&> p(nullptr, deleter);
        return p.pointer() == nullptr;
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(result);
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(result);
    }
}

TEST(UniquePointer_array, move_constructor_same_type) {
    const auto test = []() {
        hud::test::NonBitwiseType* pi = new hud::test::NonBitwiseType[2]{ {123, nullptr}, {456, nullptr} };
        hud::UniquePointer<hud::test::NonBitwiseType[], DeleterType > other(pi);
        hud::UniquePointer<hud::test::NonBitwiseType[], DeleterType > p(hud::move(other));
        return std::tuple{
            other.pointer() == nullptr,
            p.pointer() == pi,
            p.deleter().constructor_count() == 0u,
            p.deleter().copy_constructor_count() == 0u,
            p.deleter().move_constructor_count() == 1u,
            p.deleter().copy_assign_count() == 0u,
            p.deleter().move_assign_count() == 0u,
            other.deleter().constructor_count() == 0u,
            other.deleter().copy_constructor_count() == 0u,
            other.deleter().move_constructor_count() == 0u,
            other.deleter().copy_assign_count() == 0u,
            other.deleter().move_assign_count() == 0u,
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

TEST(UniquePointer_array, move_constructor_same_type_different_deleter) {
    
    const auto test = []() {
        hud::test::NonBitwiseType* pi = new hud::test::NonBitwiseType[2]{ {123, nullptr}, {456, nullptr} };
        hud::UniquePointer<hud::test::NonBitwiseType[], DeleterType2 > other(pi);
        hud::UniquePointer<const hud::test::NonBitwiseType[], ConstDeleterType > p(hud::move(other));
        return std::tuple{
            other.pointer() == nullptr,
            p.pointer() == pi,
            p.deleter().constructor_count() == 0u,
            p.deleter().copy_constructor_count() == 0u,
            p.deleter().move_constructor_count() == 1u,
            p.deleter().copy_assign_count() == 0u,
            p.deleter().move_assign_count() == 0u,
            other.deleter().constructor_count() == 0u,
            other.deleter().copy_constructor_count() == 0u,
            other.deleter().move_constructor_count() == 0u,
            other.deleter().copy_assign_count() == 0u,
            other.deleter().move_assign_count() == 0u,
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

TEST(UniquePointer_array, move_constructor_same_type_with_same_deleter_ref) {

    const auto test = []() {
        hud::test::NonBitwiseType* pi = new hud::test::NonBitwiseType[2]{ {123, nullptr}, {456, nullptr} };
        DeleterType deleter;
        hud::UniquePointer<hud::test::NonBitwiseType[], DeleterType& > other(pi, deleter);
        hud::UniquePointer<hud::test::NonBitwiseType[], DeleterType& > p(hud::move(other));
        return std::tuple{
            other.pointer() == nullptr,
            p.pointer() == pi,
            &(p.deleter()) == &deleter,
            p.deleter().constructor_count() == 0u,
            p.deleter().copy_constructor_count() == 0u,
            p.deleter().move_constructor_count() == 0u,
            p.deleter().copy_assign_count() == 0u,
            p.deleter().move_assign_count() == 0u,
            other.deleter().constructor_count() == 0u,
            other.deleter().copy_constructor_count() == 0u,
            other.deleter().move_constructor_count() == 0u,
            other.deleter().copy_assign_count() == 0u,
            other.deleter().move_assign_count() == 0u,
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
    }
}

TEST(UniquePointer_array, move_constructor_different_type_same_deleter) {

    const auto test = []() {
        hud::test::NonBitwiseType* pi = new hud::test::NonBitwiseType[2]{ {123, nullptr}, {456, nullptr} };
        hud::UniquePointer<hud::test::NonBitwiseType[], DeleterType > other(pi);
        hud::UniquePointer<const hud::test::NonBitwiseType[], ConstDeleterType > p(hud::move(other));
        return std::tuple{
            other.pointer() == nullptr,
            p.pointer() == pi,
            p.deleter().constructor_count() == 0u,
            p.deleter().copy_constructor_count() == 0u,
            p.deleter().move_constructor_count() == 1u,
            p.deleter().copy_assign_count() == 0u,
            p.deleter().move_assign_count() == 0u,
            other.deleter().constructor_count() == 0u,
            other.deleter().copy_constructor_count() == 0u,
            other.deleter().move_constructor_count() == 0u,
            other.deleter().copy_assign_count() == 0u,
            other.deleter().move_assign_count() == 0u,
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

TEST(UniquePointer_array, move_constructor_different_type_different_deleter) {
    const auto test = []() {
        hud::test::NonBitwiseType* pi = new hud::test::NonBitwiseType[2]{ {123, nullptr}, {456, nullptr} };
        hud::UniquePointer<hud::test::NonBitwiseType[], DeleterType2 > other(pi);
        hud::UniquePointer<const hud::test::NonBitwiseType[], ConstDeleterType > p(hud::move(other));
        return std::tuple{
            other.pointer() == nullptr,
            p.pointer() == pi,
            p.deleter().constructor_count() == 0u,
            p.deleter().copy_constructor_count() == 0u,
            p.deleter().move_constructor_count() == 1u,
            p.deleter().copy_assign_count() == 0u,
            p.deleter().move_assign_count() == 0u,
            other.deleter().constructor_count() == 0u,
            other.deleter().copy_constructor_count() == 0u,
            other.deleter().move_constructor_count() == 0u,
            other.deleter().copy_assign_count() == 0u,
            other.deleter().move_assign_count() == 0u,
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