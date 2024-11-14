#include <core/bits.h>

GTEST_TEST(bits, reverse_bytes)
{

    auto test = []() -> std::tuple<u32, u64>
    {
        return {
            hud::bits::reverse_bytes(0xAB00FF00u),
            hud::bits::reverse_bytes(0x123456789ABCDEFu)
        };
    };

    // Non constant
    {
        std::tuple<u32, u64> result_shift_0 = test();
        hud_assert_eq(std::get<0>(result_shift_0), 0x00FF00ABu);
        hud_assert_eq(std::get<1>(result_shift_0), 0xEFCDAB8967452301ull);
    }

    // Constant
    {
        constexpr std::tuple<u32, u64> result_shift_0 = test();
        hud_assert_eq(std::get<0>(result_shift_0), 0x00FF00ABu);
        hud_assert_eq(std::get<1>(result_shift_0), 0xEFCDAB8967452301ull);
    }
}
