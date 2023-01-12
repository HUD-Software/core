#include <core/containers/unique_pointer.h>

namespace hud_test
{
    template <typename type_t>
    struct custom_deleter
        : public hud::default_deleter<type_t>,
          hud_test::non_bitwise_type
    {

        constexpr custom_deleter() noexcept = default;
        constexpr custom_deleter(const custom_deleter &other) noexcept = default;
        constexpr custom_deleter(custom_deleter &&other) noexcept = default;
        constexpr custom_deleter(hud::default_deleter<type_t> &&other) noexcept
            : hud::default_deleter<type_t>(hud::move(other)), hud_test::non_bitwise_type(hud::move(other))
        {
        }
        template <typename U>
        constexpr custom_deleter(custom_deleter<U> &&other) noexcept
            : hud::default_deleter<type_t>(hud::move(other)), hud_test::non_bitwise_type(hud::move(other))
        {
        }
        constexpr custom_deleter &operator=(const custom_deleter &) noexcept
        {
            return *this;
        }
        constexpr custom_deleter &operator=(custom_deleter &&) noexcept
        {
            return *this;
        }
    };
}

GTEST_TEST(unique_pointer_array, equal_operator)
{

    const auto test = []()
    {
        i32 *pi = new i32[2]{0, 0};
        hud::unique_pointer<i32[], hud_test::custom_deleter<i32[]>> p1(pi);
        hud::unique_pointer<i32[], hud_test::custom_deleter<i32[]>> p2(pi);
        i32 *other_new = new i32[2]{0, 0};
        hud::unique_pointer<i32[]> p3(other_new);
        hud::unique_pointer<i32[]> p4;
        const auto result = std::tuple{
            p1 == p2,
            p2 == p1,
            p1 == p3,
            p3 == p1,
            p1 == p4,
            p4 == nullptr,
            p2 == nullptr,
            nullptr == p4,
            nullptr == p2};
        [[maybe_unused]] auto leak = p1.leak();
        return result;
    };

    // Non constant
    {
        const auto result = test();
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_FALSE(std::get<2>(result));
        GTEST_ASSERT_FALSE(std::get<3>(result));
        GTEST_ASSERT_FALSE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_FALSE(std::get<6>(result));
        GTEST_ASSERT_TRUE(std::get<7>(result));
        GTEST_ASSERT_FALSE(std::get<8>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_FALSE(std::get<2>(result));
        GTEST_ASSERT_FALSE(std::get<3>(result));
        GTEST_ASSERT_FALSE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_FALSE(std::get<6>(result));
        GTEST_ASSERT_TRUE(std::get<7>(result));
        GTEST_ASSERT_FALSE(std::get<8>(result));
    }
}

GTEST_TEST(unique_pointer_array, not_equal_operator)
{

    const auto test = []()
    {
        i32 *pi = new i32[2]{0, 0};
        hud::unique_pointer<i32[], hud_test::custom_deleter<i32[]>> p1(pi);
        hud::unique_pointer<i32[], hud_test::custom_deleter<i32[]>> p2(pi);
        hud::unique_pointer<i32[]> p3(new i32[2]{0, 0});
        hud::unique_pointer<i32[]> p4;
        const auto result = std::tuple{
            p1 != p2,
            p2 != p1,
            p1 != p3,
            p3 != p1,
            p1 != p4,
            p4 != nullptr,
            p2 != nullptr,
            nullptr != p4,
            nullptr != p2};
        [[maybe_unused]] auto leak = p1.leak();
        return result;
    };

    // Non constant
    {
        const auto result = test();
        GTEST_ASSERT_FALSE(std::get<0>(result));
        GTEST_ASSERT_FALSE(std::get<1>(result));
        GTEST_ASSERT_TRUE(std::get<2>(result));
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_FALSE(std::get<5>(result));
        GTEST_ASSERT_TRUE(std::get<6>(result));
        GTEST_ASSERT_FALSE(std::get<7>(result));
        GTEST_ASSERT_TRUE(std::get<8>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        GTEST_ASSERT_FALSE(std::get<0>(result));
        GTEST_ASSERT_FALSE(std::get<1>(result));
        GTEST_ASSERT_TRUE(std::get<2>(result));
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_FALSE(std::get<5>(result));
        GTEST_ASSERT_TRUE(std::get<6>(result));
        GTEST_ASSERT_FALSE(std::get<7>(result));
        GTEST_ASSERT_TRUE(std::get<8>(result));
    }
}

GTEST_TEST(unique_pointer_array, less_operator)
{

    const auto test = []()
    {
        i32 buf[2];
        i32 *ptr1 = &(buf[0]);
        i32 *ptr2 = &(buf[1]);
        hud::unique_pointer<i32[], hud_test::custom_deleter<i32[]>> p1(ptr1);
        hud::unique_pointer<i32[], hud_test::custom_deleter<i32[]>> p2(ptr2);
        hud::unique_pointer<i32[]> p4;

        const auto result = std::tuple{
            p1 < p1,
            p1 < p2,
            p2 < p1,
            p1 < p4,
            p4 < nullptr,
            p2 < nullptr,
            nullptr < p4,
            nullptr < p2};
        [[maybe_unused]] auto leak = p1.leak();
        [[maybe_unused]] auto leak2 = p2.leak();
        return result;
    };

    // Non constant
    {
        const auto result = test();
        GTEST_ASSERT_FALSE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_FALSE(std::get<2>(result));
        GTEST_ASSERT_FALSE(std::get<3>(result));
        GTEST_ASSERT_FALSE(std::get<4>(result));
        GTEST_ASSERT_FALSE(std::get<5>(result));
        GTEST_ASSERT_FALSE(std::get<6>(result));
        GTEST_ASSERT_TRUE(std::get<7>(result));
    }

    // Constant
    {
        // std::unique_pointer::operator< is not constexpr because constexpr pointer order comparison is undefined
    }
}

GTEST_TEST(unique_pointer_array, less_equal_operator)
{

    const auto test = []()
    {
        i32 buf[2];
        i32 *ptr1 = &(buf[0]);
        i32 *ptr2 = &(buf[1]);
        hud::unique_pointer<i32[], hud_test::custom_deleter<i32[]>> p1(ptr1);
        hud::unique_pointer<i32[], hud_test::custom_deleter<i32[]>> p2(ptr2);
        hud::unique_pointer<i32[]> p4;

        const auto result = std::tuple{
            p1 <= p1,
            p1 <= p2,
            p2 <= p1,
            p1 <= p4,
            p4 <= nullptr,
            p2 <= nullptr,
            nullptr <= p4,
            nullptr <= p2};
        [[maybe_unused]] auto leak = p1.leak();
        [[maybe_unused]] auto leak2 = p2.leak();
        return result;
    };

    // Non constant
    {
        const auto result = test();
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_FALSE(std::get<2>(result));
        GTEST_ASSERT_FALSE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_FALSE(std::get<5>(result));
        GTEST_ASSERT_TRUE(std::get<6>(result));
        GTEST_ASSERT_TRUE(std::get<7>(result));
    }

    // Constant
    {
        // std::unique_pointer::operator<= is not constexpr because constexpr pointer order comparison is undefined
    }
}

GTEST_TEST(unique_pointer_array, greater_operator)
{

    const auto test = []()
    {
        i32 buf[2];
        i32 *ptr1 = &(buf[0]);
        i32 *ptr2 = &(buf[1]);
        hud::unique_pointer<i32[], hud_test::custom_deleter<i32[]>> p1(ptr1);
        hud::unique_pointer<i32[], hud_test::custom_deleter<i32[]>> p2(ptr2);
        hud::unique_pointer<i32[]> p4;

        const auto result = std::tuple{
            p1 > p1,
            p1 > p2,
            p2 > p1,
            p1 > p4,
            p4 > nullptr,
            p2 > nullptr,
            nullptr > p4,
            nullptr > p2};
        [[maybe_unused]] auto leak = p1.leak();
        [[maybe_unused]] auto leak2 = p2.leak();
        return result;
    };

    // Non constant
    {
        const auto result = test();
        GTEST_ASSERT_FALSE(std::get<0>(result));
        GTEST_ASSERT_FALSE(std::get<1>(result));
        GTEST_ASSERT_TRUE(std::get<2>(result));
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_FALSE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_FALSE(std::get<6>(result));
        GTEST_ASSERT_FALSE(std::get<7>(result));
    }

    // Constant
    {
        // std::unique_pointer::operator> is not constexpr because constexpr pointer order comparison is undefined
    }
}

GTEST_TEST(unique_pointer_array, greater_equal_operator)
{

    const auto test = []()
    {
        i32 buf[2];
        i32 *ptr1 = &(buf[0]);
        i32 *ptr2 = &(buf[1]);
        hud::unique_pointer<i32[], hud_test::custom_deleter<i32[]>> p1(ptr1);
        hud::unique_pointer<i32[], hud_test::custom_deleter<i32[]>> p2(ptr2);
        hud::unique_pointer<i32[]> p4;

        const auto result = std::tuple{
            p1 >= p1,
            p1 >= p2,
            p2 >= p1,
            p1 >= p4,
            p4 >= nullptr,
            p2 >= nullptr,
            nullptr >= p4,
            nullptr >= p2};
        [[maybe_unused]] auto leak = p1.leak();
        [[maybe_unused]] auto leak2 = p2.leak();
        return result;
    };

    // Non constant
    {
        const auto result = test();
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_FALSE(std::get<1>(result));
        GTEST_ASSERT_TRUE(std::get<2>(result));
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_TRUE(std::get<6>(result));
        GTEST_ASSERT_FALSE(std::get<7>(result));
    }

    // Constant
    {
        // std::unique_pointer::operator>= is not constexpr because constexpr pointer order comparison is undefined
    }
}