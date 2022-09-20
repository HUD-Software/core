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
}

TEST(UniquePointer, equal_operator) {
    
    const auto test = []() {
        i32* pi = new i32(0);
        hud::UniquePointer<i32, CustomDeleter<i32>> p1(pi);
        hud::UniquePointer<i32, CustomDeleter<i32>> p2(pi);
        hud::UniquePointer<i32> p3(new i32(0));
        hud::UniquePointer<i32> p4;
        const auto result = std::tuple{
            p1 == p2,
            p2 == p1,
            p1 == p3,
            p3 == p1,
            p1 == p4,
            p4 == nullptr,
            p2 == nullptr,
            nullptr == p4,
            nullptr == p2
        };
        [[maybe_unused]] auto ptr = p1.leak();
        return result;
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_FALSE(std::get<2>(result));
        ASSERT_FALSE(std::get<3>(result));
        ASSERT_FALSE(std::get<4>(result));
        ASSERT_TRUE(std::get<5>(result));
        ASSERT_FALSE(std::get<6>(result));
        ASSERT_TRUE(std::get<7>(result));
        ASSERT_FALSE(std::get<8>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_FALSE(std::get<2>(result));
        ASSERT_FALSE(std::get<3>(result));
        ASSERT_FALSE(std::get<4>(result));
        ASSERT_TRUE(std::get<5>(result));
        ASSERT_FALSE(std::get<6>(result));
        ASSERT_TRUE(std::get<7>(result));
        ASSERT_FALSE(std::get<8>(result));
    }
}


TEST(UniquePointer, not_equal_operator) {

    const auto test = []() {
        i32* pi = new i32(0);
        hud::UniquePointer<i32, CustomDeleter<i32>> p1(pi);
        hud::UniquePointer<i32, CustomDeleter<i32>> p2(pi);
        hud::UniquePointer<i32> p3(new i32(0));
        hud::UniquePointer<i32> p4;
        const auto result = std::tuple{
            p1 != p2,
            p2 != p1,
            p1 != p3,
            p3 != p1,
            p1 != p4,
            p4 != nullptr,
            p2 != nullptr,
            nullptr != p4,
            nullptr != p2
        };
        [[maybe_unused]] auto ptr = p1.leak();
        return result;
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_FALSE(std::get<0>(result));
        ASSERT_FALSE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
        ASSERT_TRUE(std::get<4>(result));
        ASSERT_FALSE(std::get<5>(result));
        ASSERT_TRUE(std::get<6>(result));
        ASSERT_FALSE(std::get<7>(result));
        ASSERT_TRUE(std::get<8>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_FALSE(std::get<0>(result));
        ASSERT_FALSE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
        ASSERT_TRUE(std::get<4>(result));
        ASSERT_FALSE(std::get<5>(result));
        ASSERT_TRUE(std::get<6>(result));
        ASSERT_FALSE(std::get<7>(result));
        ASSERT_TRUE(std::get<8>(result));
    }
}


TEST(UniquePointer, less_operator) {

    const auto test = []() {
        i32 buf[2];
        i32* ptr1 = &(buf[0]);
        i32* ptr2 = &(buf[1]);
        hud::UniquePointer<i32, CustomDeleter<i32>> p1(ptr1);
        hud::UniquePointer<i32, CustomDeleter<i32>> p2(ptr2);
        hud::UniquePointer<i32> p4;
        if (hud::is_constant_evaluated()) {
            const auto result = std::tuple{
                false, // not constexpr p1 < p1,
                false, // not constexpr p1 < p2,
                false, // not constexpr p2 < p1,
                false, // not constexpr p1 < p4,
                p4 < nullptr,
                p2 < nullptr,
                nullptr < p4,
                nullptr < p2
            };
            [[maybe_unused]] auto leak = p1.leak();
            [[maybe_unused]] auto leak2 = p2.leak();
            return result;
        }
        else {
            const auto result = std::tuple{
                p1 < p1,
                p1 < p2,
                p2 < p1,
                p1 < p4,
                p4 < nullptr,
                p2 < nullptr,
                nullptr < p4,
                nullptr < p2
            };
            [[maybe_unused]] auto leak = p1.leak();
            [[maybe_unused]] auto leak2 = p2.leak();
            return result;
        }
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_FALSE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_FALSE(std::get<2>(result));
        ASSERT_FALSE(std::get<3>(result));
        ASSERT_FALSE(std::get<4>(result));
        ASSERT_FALSE(std::get<5>(result));
        ASSERT_FALSE(std::get<6>(result));
        ASSERT_TRUE(std::get<7>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_FALSE(std::get<0>(result));
        ASSERT_FALSE(std::get<1>(result));
        ASSERT_FALSE(std::get<2>(result));
        ASSERT_FALSE(std::get<3>(result));
        ASSERT_FALSE(std::get<4>(result));
        ASSERT_FALSE(std::get<5>(result));
        ASSERT_FALSE(std::get<6>(result));
        ASSERT_TRUE(std::get<7>(result));
    }
}

TEST(UniquePointer, less_equal_operator) {

    const auto test = []() {
        i32 buf[2];
        i32* ptr1 = &(buf[0]);
        i32* ptr2 = &(buf[1]);
        hud::UniquePointer<i32, CustomDeleter<i32>> p1(ptr1);
        hud::UniquePointer<i32, CustomDeleter<i32>> p2(ptr2);
        hud::UniquePointer<i32> p4;
        if (hud::is_constant_evaluated()) {
            const auto result = std::tuple{
                false, // not constexpr p1 <= p2,
                false, // not constexpr p2 <= p1,
                false, // not constexpr p1 <= p4,
                false, // not constexpr p1 <= p1,
                p4 <= nullptr,
                p2 <= nullptr,
                nullptr <= p4,
                nullptr <= p2
            };
            [[maybe_unused]] auto leak =p1.leak();
            [[maybe_unused]] auto leak2 =p2.leak();
            return result;
        }
        else {
            const auto result = std::tuple{
                p1 <= p1,
                p1 <= p2,
                p2 <= p1,
                p1 <= p4,
                p4 <= nullptr,
                p2 <= nullptr,
                nullptr <= p4,
                nullptr <= p2
            };
            [[maybe_unused]] auto leak =p1.leak();
            [[maybe_unused]] auto leak2 =p2.leak();
            return result;
        }
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_FALSE(std::get<2>(result));
        ASSERT_FALSE(std::get<3>(result));
        ASSERT_TRUE(std::get<4>(result));
        ASSERT_FALSE(std::get<5>(result));
        ASSERT_TRUE(std::get<6>(result));
        ASSERT_TRUE(std::get<7>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_FALSE(std::get<0>(result));
        ASSERT_FALSE(std::get<1>(result));
        ASSERT_FALSE(std::get<2>(result));
        ASSERT_FALSE(std::get<3>(result));
        ASSERT_TRUE(std::get<4>(result));
        ASSERT_FALSE(std::get<5>(result));
        ASSERT_TRUE(std::get<6>(result));
        ASSERT_TRUE(std::get<7>(result));
    }
}


TEST(UniquePointer, greater_operator) {

    const auto test = []() {
        i32 buf[2];
        i32* ptr1 = &(buf[0]);
        i32* ptr2 = &(buf[1]);
        hud::UniquePointer<i32, CustomDeleter<i32>> p1(ptr1);
        hud::UniquePointer<i32, CustomDeleter<i32>> p2(ptr2);
        hud::UniquePointer<i32> p4;
        if (hud::is_constant_evaluated()) {
            const auto result = std::tuple{
                false, // not constexpr p1 > p1,
                false, // not constexpr p1 > p2,
                false, // not constexpr p2 > p1,
                false, // not constexpr p1 > p4,
                p4 > nullptr,
                p2 > nullptr,
                nullptr > p4,
                nullptr > p2
            };
            [[maybe_unused]] auto leak = p1.leak();
            [[maybe_unused]] auto leak2 = p2.leak();
            return result;
        }
        else {
            const auto result = std::tuple{
                p1 > p1,
                p1 > p2,
                p2 > p1,
                p1 > p4,
                p4 > nullptr,
                p2 > nullptr,
                nullptr > p4,
                nullptr > p2
            };
            [[maybe_unused]] auto leak = p1.leak();
            [[maybe_unused]] auto leak2 = p2.leak();
            return result;
        }
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_FALSE(std::get<0>(result));
        ASSERT_FALSE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
        ASSERT_FALSE(std::get<4>(result));
        ASSERT_TRUE(std::get<5>(result));
        ASSERT_FALSE(std::get<6>(result));
        ASSERT_FALSE(std::get<7>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_FALSE(std::get<0>(result));
        ASSERT_FALSE(std::get<1>(result));
        ASSERT_FALSE(std::get<2>(result));
        ASSERT_FALSE(std::get<3>(result));
        ASSERT_FALSE(std::get<4>(result));
        ASSERT_TRUE(std::get<5>(result));
        ASSERT_FALSE(std::get<6>(result));
        ASSERT_FALSE(std::get<7>(result));
    }
}

TEST(UniquePointer, greater_equal_operator) {

    const auto test = []() {
        i32 buf[2];
        i32* ptr1 = &(buf[0]);
        i32* ptr2 = &(buf[1]);
        hud::UniquePointer<i32, CustomDeleter<i32>> p1(ptr1);
        hud::UniquePointer<i32, CustomDeleter<i32>> p2(ptr2);
        hud::UniquePointer<i32> p4;
        if (hud::is_constant_evaluated()) {
            const auto result = std::tuple{
                false, // not constexpr p1 >= p1,
                false, // not constexpr p1 >= p2,
                false, // not constexpr p2 >= p1,
                false, // not constexpr p1 >= p4,
                p4 >= nullptr,
                p2 >= nullptr,
                nullptr >= p4,
                nullptr >= p2
            };
            [[maybe_unused]] auto leak = p1.leak();
            [[maybe_unused]] auto leak2 = p2.leak();
            return result;
        }
        else {
            const auto result = std::tuple{
                p1 >= p1,
                p1 >= p2,
                p2 >= p1,
                p1 >= p4,
                p4 >= nullptr,
                p2 >= nullptr,
                nullptr >= p4,
                nullptr >= p2
            };
            [[maybe_unused]] auto leak = p1.leak();
            [[maybe_unused]] auto leak2 = p2.leak();
            return result;
        }
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_FALSE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
        ASSERT_TRUE(std::get<4>(result));
        ASSERT_TRUE(std::get<5>(result));
        ASSERT_TRUE(std::get<6>(result));
        ASSERT_FALSE(std::get<7>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_FALSE(std::get<0>(result));
        ASSERT_FALSE(std::get<1>(result));
        ASSERT_FALSE(std::get<2>(result));
        ASSERT_FALSE(std::get<3>(result));
        ASSERT_TRUE(std::get<4>(result));
        ASSERT_TRUE(std::get<5>(result));
        ASSERT_TRUE(std::get<6>(result));
        ASSERT_FALSE(std::get<7>(result));
    }
}