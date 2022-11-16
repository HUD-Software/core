#include <core/memory.h>

GTEST_TEST(memory, reverse_bits) {


    // Non constant
    {
        const u32 reverse_bits = hud::memory::reverse_bits(0x04C11DB7);
        ASSERT_EQ(reverse_bits, 0xEDB88320);
    }

    // Constant
    {
        constexpr u32 reverse_bits = hud::memory::reverse_bits(0x04C11DB7);
        ASSERT_EQ(reverse_bits, 0xEDB88320);
    }
}

GTEST_TEST(memory,reverse) {


    auto test = []() -> std::tuple<u32, u64> {
        return {
            hud::memory::reverse(u32{0xAB00FF00}),
            hud::memory::reverse(u64{0x123456789ABCDEF})
        };
    };

    // Non constant
    {
        std::tuple<u32, u64> result_shift_0 = test();
        ASSERT_EQ(std::get<0>(result_shift_0), 0x00FF00ABu);
        ASSERT_EQ(std::get<1>(result_shift_0), 0xEFCDAB8967452301ull);
    }

    // Constant
    {
        constexpr std::tuple<u32, u64> result_shift_0 = test();
        ASSERT_EQ(std::get<0>(result_shift_0), 0x00FF00ABu);
        ASSERT_EQ(std::get<1>(result_shift_0), 0xEFCDAB8967452301ull);
    }
}
