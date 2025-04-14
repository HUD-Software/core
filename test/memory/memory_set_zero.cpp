#include <core/memory.h>

GTEST_TEST(memory, set_memory_zero_pointer_to_buffer)
{

    auto test = []() -> std::tuple<u8, u8, u8>
    {
        u8 buf[3] = {2, 4, 8};
        hud::memory::set_memory_zero(buf, 3 * sizeof(u8));
        return {buf[0], buf[1], buf[2]};
    };

    // Non constant
    {
        std::tuple<u8, u8, u8> result = test();
        hud_assert_eq(std::get<0>(result), 0u);
        hud_assert_eq(std::get<1>(result), 0u);
        hud_assert_eq(std::get<2>(result), 0u);
    }

    // Constant
    {
        constexpr std::tuple<u8, u8, u8> result = test();
        hud_assert_eq(std::get<0>(result), 0u);
        hud_assert_eq(std::get<1>(result), 0u);
        hud_assert_eq(std::get<2>(result), 0u);
    }
}

GTEST_TEST(memory, set_memory_zero_c_array)
{

    auto test = []() -> std::tuple<u8, u8, u8>
    {
        u8 buf[3] = {2, 4, 8};
        hud::memory::set_memory_zero(buf);
        return {buf[0], buf[1], buf[2]};
    };

    // Non constant
    {
        std::tuple<u8, u8, u8> result = test();
        hud_assert_eq(std::get<0>(result), 0u);
        hud_assert_eq(std::get<1>(result), 0u);
        hud_assert_eq(std::get<2>(result), 0u);
    }

    // Constant
    {
        constexpr std::tuple<u8, u8, u8> result = test();
        hud_assert_eq(std::get<0>(result), 0u);
        hud_assert_eq(std::get<1>(result), 0u);
        hud_assert_eq(std::get<2>(result), 0u);
    }
}

GTEST_TEST(memory, set_memory_zero_safe_pointer_to_buffer)
{
    auto test = []() -> std::tuple<u8, u8, u8>
    {
        u8 buf[3] = {2, 4, 8};
        hud::memory::set_memory_zero_safe(buf, 3 * sizeof(u8));
        return {buf[0], buf[1], buf[2]};
    };

    // Non constant
    {
        std::tuple<u8, u8, u8> result = test();
        hud_assert_eq(std::get<0>(result), 0u);
        hud_assert_eq(std::get<1>(result), 0u);
        hud_assert_eq(std::get<2>(result), 0u);
    }

    // Constant
    {
        constexpr std::tuple<u8, u8, u8> result = test();
        hud_assert_eq(std::get<0>(result), 0u);
        hud_assert_eq(std::get<1>(result), 0u);
        hud_assert_eq(std::get<2>(result), 0u);
    }
}

GTEST_TEST(memory, set_memory_zero_safe_c_array)
{

    auto test = []() -> std::tuple<u8, u8, u8>
    {
        u8 buf[3] = {2, 4, 8};
        hud::memory::set_memory_zero_safe(buf);
        return {buf[0], buf[1], buf[2]};
    };

    // Non constant
    {
        std::tuple<u8, u8, u8> result = test();
        hud_assert_eq(std::get<0>(result), 0u);
        hud_assert_eq(std::get<1>(result), 0u);
        hud_assert_eq(std::get<2>(result), 0u);
    }

    // Constant
    {
        constexpr std::tuple<u8, u8, u8> result = test();
        hud_assert_eq(std::get<0>(result), 0u);
        hud_assert_eq(std::get<1>(result), 0u);
        hud_assert_eq(std::get<2>(result), 0u);
    }
}

GTEST_TEST(memory, set_memory_zero_safe_object_to_buffer)
{
    hud_test::non_bitwise_type buf[3] = {2, 4, 8};
    const u8 zero_buffer[sizeof(hud_test::non_bitwise_type) * 3] = {0};
    hud::memory::set_memory_zero_safe(buf, 3 * sizeof(hud_test::non_bitwise_type));
    hud_assert_true(hud::memory::compare_equal(buf, zero_buffer, sizeof(hud_test::non_bitwise_type) * 3));
}
