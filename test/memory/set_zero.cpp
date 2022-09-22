#include <core/memory.h>

TEST(memory, set_zero_pointer_to_buffer) {



    auto test = []() -> std::tuple<u8, u8, u8> {
        u8 buf[3] = { 2, 4, 8 };
        hud::memory::set_zero(buf, 3 * sizeof(u8));
        return { buf[0], buf[1], buf[2] };
    };

    // Non constant
    {
        std::tuple<u8, u8, u8> result = test();
        ASSERT_EQ(std::get<0>(result), 0u);
        ASSERT_EQ(std::get<1>(result), 0u);
        ASSERT_EQ(std::get<2>(result), 0u);
    }

    // Constant
    {
        constexpr std::tuple<u8, u8, u8> result = test();
        ASSERT_EQ(std::get<0>(result), 0u);
        ASSERT_EQ(std::get<1>(result), 0u);
        ASSERT_EQ(std::get<2>(result), 0u);
    }
}

TEST(memory, set_zero_C_array) {



    auto test = []() -> std::tuple<u8, u8, u8> {
        u8 buf[3] = { 2, 4, 8 };
        hud::memory::set_zero(buf);
        return { buf[0], buf[1], buf[2] };
    };

    // Non constant
    {
        std::tuple<u8, u8, u8> result = test();
        ASSERT_EQ(std::get<0>(result), 0u);
        ASSERT_EQ(std::get<1>(result), 0u);
        ASSERT_EQ(std::get<2>(result), 0u);
    }

    // Constant
    {
        constexpr std::tuple<u8, u8, u8> result = test();
        ASSERT_EQ(std::get<0>(result), 0u);
        ASSERT_EQ(std::get<1>(result), 0u);
        ASSERT_EQ(std::get<2>(result), 0u);
    }
}