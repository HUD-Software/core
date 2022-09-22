#include <core/memory.h>

TEST(memory,rotate_left) {




    auto test = [](const u32 shift) -> std::tuple<u32, u64> {
        return {
            hud::memory::rotate_left(0xFF00FFAB, shift),
            hud::memory::rotate_left(0xAB00FF00FF00FFAB, shift)
        };
    };

    // Non constant
    {
        std::tuple<u32, u64> result_shift_0 = test(0);
        ASSERT_EQ(std::get<0>(result_shift_0), 0xFF00FFABu);
        ASSERT_EQ(std::get<1>(result_shift_0), 0xAB00FF00FF00FFABull);
        std::tuple<u32, u64> result_shift_1 = test(1);
        ASSERT_EQ(std::get<0>(result_shift_1), 0xFE01FF57u);
        ASSERT_EQ(std::get<1>(result_shift_1), 0x5601FE01FE01FF57ull);
        std::tuple<u32, u64> result_shift_2 = test(2);
        ASSERT_EQ(std::get<0>(result_shift_2), 0xFC03FEAFu);
        ASSERT_EQ(std::get<1>(result_shift_2), 0xAC03FC03FC03FEAEull);
        std::tuple<u32, u64> result_shift_4 = test(4);
        ASSERT_EQ(std::get<0>(result_shift_4), 0xF00FFABFu);
        ASSERT_EQ(std::get<1>(result_shift_4), 0xB00FF00FF00FFABAull);
        std::tuple<u32, u64> result_shift_8 = test(8);
        ASSERT_EQ(std::get<0>(result_shift_8), 0x00FFABFFu);
        ASSERT_EQ(std::get<1>(result_shift_8), 0x00FF00FF00FFABABull);
    }

    // Constant
    {
        constexpr std::tuple<u32, u64> result_shift_0 = test(0);
        ASSERT_EQ(std::get<0>(result_shift_0), 0xFF00FFABu);
        ASSERT_EQ(std::get<1>(result_shift_0), 0xAB00FF00FF00FFABull);
        constexpr std::tuple<u32, u64> result_shift_1 = test(1);
        ASSERT_EQ(std::get<0>(result_shift_1), 0xFE01FF57u);
        ASSERT_EQ(std::get<1>(result_shift_1), 0x5601FE01FE01FF57ull);
        constexpr std::tuple<u32, u64> result_shift_2 = test(2);
        ASSERT_EQ(std::get<0>(result_shift_2), 0xFC03FEAFu);
        ASSERT_EQ(std::get<1>(result_shift_2), 0xAC03FC03FC03FEAEull);
        constexpr std::tuple<u32, u64> result_shift_4 = test(4);
        ASSERT_EQ(std::get<0>(result_shift_4), 0xF00FFABFu);
        ASSERT_EQ(std::get<1>(result_shift_4), 0xB00FF00FF00FFABAull);
        constexpr std::tuple<u32, u64> result_shift_8 = test(8);
        ASSERT_EQ(std::get<0>(result_shift_8), 0x00FFABFFu);
        ASSERT_EQ(std::get<1>(result_shift_8), 0x00FF00FF00FFABABull);
    }
}

TEST(memory,rotate_right) {


    auto test = [](const u32 shift) -> std::tuple<u32, u64> {
        return {
            hud::memory::rotate_right(0xFF00FFAB, shift),
            hud::memory::rotate_right(0xAB00FF00FF00FFAB, shift)
        };
    };

    // Non constant
    {
        std::tuple<u32, u64> result_shift_0 = test(0);
        ASSERT_EQ(std::get<0>(result_shift_0), 0xFF00FFABu);
        ASSERT_EQ(std::get<1>(result_shift_0), 0xAB00FF00FF00FFABull);
        std::tuple<u32, u64> result_shift_1 = test(1);
        ASSERT_EQ(std::get<0>(result_shift_1), 0xFF807FD5u);
        ASSERT_EQ(std::get<1>(result_shift_1), 0xD5807F807F807FD5ull);
        std::tuple<u32, u64> result_shift_2 = test(2);
        ASSERT_EQ(std::get<0>(result_shift_2), 0xFFC03FEAu);
        ASSERT_EQ(std::get<1>(result_shift_2), 0xEAC03FC03FC03FEAull);
        std::tuple<u32, u64> result_shift_4 = test(4);
        ASSERT_EQ(std::get<0>(result_shift_4), 0xBFF00FFAu);
        ASSERT_EQ(std::get<1>(result_shift_4), 0xBAB00FF00FF00FFAull);
        std::tuple<u32, u64> result_shift_8 = test(8);
        ASSERT_EQ(std::get<0>(result_shift_8), 0xABFF00FFu);
        ASSERT_EQ(std::get<1>(result_shift_8), 0xABAB00FF00FF00FFull);
    }

    // Constant
    {
        constexpr std::tuple<u32, u64> result_shift_0 = test(0);
        ASSERT_EQ(std::get<0>(result_shift_0), 0xFF00FFABu);
        ASSERT_EQ(std::get<1>(result_shift_0), 0xAB00FF00FF00FFABull);
        constexpr std::tuple<u32, u64> result_shift_1 = test(1);
        ASSERT_EQ(std::get<0>(result_shift_1), 0xFF807FD5u);
        ASSERT_EQ(std::get<1>(result_shift_1), 0xD5807F807F807FD5ull);
        constexpr std::tuple<u32, u64> result_shift_2 = test(2);
        ASSERT_EQ(std::get<0>(result_shift_2), 0xFFC03FEAu);
        ASSERT_EQ(std::get<1>(result_shift_2), 0xEAC03FC03FC03FEAull);
        constexpr std::tuple<u32, u64> result_shift_4 = test(4);
        ASSERT_EQ(std::get<0>(result_shift_4), 0xBFF00FFAu);
        ASSERT_EQ(std::get<1>(result_shift_4), 0xBAB00FF00FF00FFAull);
        constexpr std::tuple<u32, u64> result_shift_8 = test(8);
        ASSERT_EQ(std::get<0>(result_shift_8), 0xABFF00FFu);
        ASSERT_EQ(std::get<1>(result_shift_8), 0xABAB00FF00FF00FFull);
    }
}