#include <core/memory.h>

TEST(memory, copy_pointer_to_buffer) {



    auto test = []()  -> std::tuple<u8, u8, u8, bool> {
        u8 buf[3] = { 0,0,0 };
        u8 buf_2[3] = { 2,4,8 };
        u8* dest = hud::memory::copy(buf, buf_2, 2 * sizeof(u8));
        return { buf[0], buf[1], buf[2], dest == buf };
    };

    // Non constant
    {
        std::tuple<u8, u8, u8, bool> result = test();
        ASSERT_EQ(std::get<0>(result), 2);
        ASSERT_EQ(std::get<1>(result), 4);
        ASSERT_EQ(std::get<2>(result), 0);
        ASSERT_TRUE(std::get<3>(result));
    }

    // Constant
    {
        constexpr std::tuple<u8, u8, u8, bool> result = test();
        ASSERT_EQ(std::get<0>(result), 2);
        ASSERT_EQ(std::get<1>(result), 4);
        ASSERT_EQ(std::get<2>(result), 0);
        ASSERT_TRUE(std::get<3>(result));
    }
}

TEST(memory, copy_c_arary) {



    auto test = []() -> std::tuple<u8, u8, u8, bool> {
        u8 buf[3] = { 0,0,0 };
        u8 buf_2[3] = { 2,4,8 };
        u8* dest = hud::memory::copy(buf, buf_2);
        return { buf[0], buf[1], buf[2], dest == buf };
    };

    // Non constant
    {
        std::tuple<u8, u8, u8, bool> result = test();
        ASSERT_EQ(std::get<0>(result), 2);
        ASSERT_EQ(std::get<1>(result), 4);
        ASSERT_EQ(std::get<2>(result), 8);
        ASSERT_TRUE(std::get<3>(result));
    }

    // Constant
    {
        constexpr std::tuple<u8, u8, u8, bool> result = test();
        ASSERT_EQ(std::get<0>(result), 2);
        ASSERT_EQ(std::get<1>(result), 4);
        ASSERT_EQ(std::get<2>(result), 8);
        ASSERT_TRUE(std::get<3>(result));
    }
}

TEST(memory, move_pointer_to_buffer) {


    auto test = [](u32 index_dest, u32 index_src) -> std::tuple<u8, u8, u8, u8> {
        u8 buf[4] = { 2, 4, 8, 16 };
        hud::memory::move(buf + index_dest, buf + index_src, std::min(4 - index_src, 4 - index_dest) * sizeof(u8));
        return { buf[0], buf[1], buf[2], buf[3] };
    };

    // Non constant
    {
        std::tuple<u8, u8, u8, u8> result_00 = test(0,0);
        ASSERT_EQ(std::get<0>(result_00), 2);
        ASSERT_EQ(std::get<1>(result_00), 4);
        ASSERT_EQ(std::get<2>(result_00), 8);
        ASSERT_EQ(std::get<3>(result_00), 16);

        std::tuple<u8, u8, u8, u8> result_01 = test(0,1);
        ASSERT_EQ(std::get<0>(result_01), 4);
        ASSERT_EQ(std::get<1>(result_01), 8);
        ASSERT_EQ(std::get<2>(result_01), 16);
        ASSERT_EQ(std::get<3>(result_01), 16);

        std::tuple<u8, u8, u8, u8> result_02 = test(0,2);
        ASSERT_EQ(std::get<0>(result_02), 8);
        ASSERT_EQ(std::get<1>(result_02), 16);
        ASSERT_EQ(std::get<2>(result_02), 8);
        ASSERT_EQ(std::get<3>(result_02), 16);

        std::tuple<u8, u8, u8, u8> result_03 = test(0,3);
        ASSERT_EQ(std::get<0>(result_03), 16);
        ASSERT_EQ(std::get<1>(result_03), 4);
        ASSERT_EQ(std::get<2>(result_03), 8);
        ASSERT_EQ(std::get<3>(result_03), 16);

        std::tuple<u8, u8, u8, u8> result_04 = test(0, 4);
        ASSERT_EQ(std::get<0>(result_04), 2);
        ASSERT_EQ(std::get<1>(result_04), 4);
        ASSERT_EQ(std::get<2>(result_04), 8);
        ASSERT_EQ(std::get<3>(result_04), 16);

        std::tuple<u8, u8, u8, u8> result_10 = test(1, 0);
        ASSERT_EQ(std::get<0>(result_10), 2);
        ASSERT_EQ(std::get<1>(result_10), 2);
        ASSERT_EQ(std::get<2>(result_10), 4);
        ASSERT_EQ(std::get<3>(result_10), 8);

        std::tuple<u8, u8, u8, u8> result_11 = test(1, 1);
        ASSERT_EQ(std::get<0>(result_11), 2);
        ASSERT_EQ(std::get<1>(result_11), 4);
        ASSERT_EQ(std::get<2>(result_11), 8);
        ASSERT_EQ(std::get<3>(result_11), 16);

        std::tuple<u8, u8, u8, u8> result_12 = test(1, 2);
        ASSERT_EQ(std::get<0>(result_12), 2);
        ASSERT_EQ(std::get<1>(result_12), 8);
        ASSERT_EQ(std::get<2>(result_12), 16);
        ASSERT_EQ(std::get<3>(result_12), 16);

        std::tuple<u8, u8, u8, u8> result_13 = test(1, 3);
        ASSERT_EQ(std::get<0>(result_13), 2);
        ASSERT_EQ(std::get<1>(result_13), 16);
        ASSERT_EQ(std::get<2>(result_13), 8);
        ASSERT_EQ(std::get<3>(result_13), 16);

        std::tuple<u8, u8, u8, u8> result_14 = test(1, 4);
        ASSERT_EQ(std::get<0>(result_14), 2);
        ASSERT_EQ(std::get<1>(result_14), 4);
        ASSERT_EQ(std::get<2>(result_14), 8);
        ASSERT_EQ(std::get<3>(result_14), 16);

        std::tuple<u8, u8, u8, u8> result_20 = test(2, 0);
        ASSERT_EQ(std::get<0>(result_20), 2);
        ASSERT_EQ(std::get<1>(result_20), 4);
        ASSERT_EQ(std::get<2>(result_20), 2);
        ASSERT_EQ(std::get<3>(result_20), 4);

        std::tuple<u8, u8, u8, u8> result_21 = test(2, 1);
        ASSERT_EQ(std::get<0>(result_21), 2);
        ASSERT_EQ(std::get<1>(result_21), 4);
        ASSERT_EQ(std::get<2>(result_21), 4);
        ASSERT_EQ(std::get<3>(result_21), 8);

        std::tuple<u8, u8, u8, u8> result_22 = test(2, 2);
        ASSERT_EQ(std::get<0>(result_22), 2);
        ASSERT_EQ(std::get<1>(result_22), 4);
        ASSERT_EQ(std::get<2>(result_22), 8);
        ASSERT_EQ(std::get<3>(result_22), 16);

        std::tuple<u8, u8, u8, u8> result_23 = test(2, 3);
        ASSERT_EQ(std::get<0>(result_23), 2);
        ASSERT_EQ(std::get<1>(result_23), 4);
        ASSERT_EQ(std::get<2>(result_23), 16);
        ASSERT_EQ(std::get<3>(result_23), 16);

        std::tuple<u8, u8, u8, u8> result_24 = test(2, 4);
        ASSERT_EQ(std::get<0>(result_24), 2);
        ASSERT_EQ(std::get<1>(result_24), 4);
        ASSERT_EQ(std::get<2>(result_24), 8);
        ASSERT_EQ(std::get<3>(result_24), 16);

        std::tuple<u8, u8, u8, u8> result_30 = test(3, 0);
        ASSERT_EQ(std::get<0>(result_30), 2);
        ASSERT_EQ(std::get<1>(result_30), 4);
        ASSERT_EQ(std::get<2>(result_30), 8);
        ASSERT_EQ(std::get<3>(result_30), 2);

        std::tuple<u8, u8, u8, u8> result_31 = test(3, 1);
        ASSERT_EQ(std::get<0>(result_31), 2);
        ASSERT_EQ(std::get<1>(result_31), 4);
        ASSERT_EQ(std::get<2>(result_31), 8);
        ASSERT_EQ(std::get<3>(result_31), 4);

        std::tuple<u8, u8, u8, u8> result_32 = test(3, 2);
        ASSERT_EQ(std::get<0>(result_32), 2);
        ASSERT_EQ(std::get<1>(result_32), 4);
        ASSERT_EQ(std::get<2>(result_32), 8);
        ASSERT_EQ(std::get<3>(result_32), 8);

        std::tuple<u8, u8, u8, u8> result_33 = test(3, 3);
        ASSERT_EQ(std::get<0>(result_33), 2);
        ASSERT_EQ(std::get<1>(result_33), 4);
        ASSERT_EQ(std::get<2>(result_33), 8);
        ASSERT_EQ(std::get<3>(result_33), 16);

        std::tuple<u8, u8, u8, u8> result_34 = test(3, 4);
        ASSERT_EQ(std::get<0>(result_34), 2);
        ASSERT_EQ(std::get<1>(result_34), 4);
        ASSERT_EQ(std::get<2>(result_34), 8);
        ASSERT_EQ(std::get<3>(result_34), 16);

        std::tuple<u8, u8, u8, u8> result_40 = test(4, 0);
        ASSERT_EQ(std::get<0>(result_40), 2);
        ASSERT_EQ(std::get<1>(result_40), 4);
        ASSERT_EQ(std::get<2>(result_40), 8);
        ASSERT_EQ(std::get<3>(result_40), 16);

        std::tuple<u8, u8, u8, u8> result_41 = test(4, 1);
        ASSERT_EQ(std::get<0>(result_41), 2);
        ASSERT_EQ(std::get<1>(result_41), 4);
        ASSERT_EQ(std::get<2>(result_41), 8);
        ASSERT_EQ(std::get<3>(result_41), 16);

        std::tuple<u8, u8, u8, u8> result_42 = test(4, 2);
        ASSERT_EQ(std::get<0>(result_42), 2);
        ASSERT_EQ(std::get<1>(result_42), 4);
        ASSERT_EQ(std::get<2>(result_42), 8);
        ASSERT_EQ(std::get<3>(result_42), 16);

        std::tuple<u8, u8, u8, u8> result_43 = test(4, 3);
        ASSERT_EQ(std::get<0>(result_43), 2);
        ASSERT_EQ(std::get<1>(result_43), 4);
        ASSERT_EQ(std::get<2>(result_43), 8);
        ASSERT_EQ(std::get<3>(result_43), 16);

        std::tuple<u8, u8, u8, u8> result_44 = test(4, 4);
        ASSERT_EQ(std::get<0>(result_44), 2);
        ASSERT_EQ(std::get<1>(result_44), 4);
        ASSERT_EQ(std::get<2>(result_44), 8);
        ASSERT_EQ(std::get<3>(result_44), 16);
    }

    // Constant
    {
        constexpr std::tuple<u8, u8, u8, u8> result_00 = test(0, 0);
        ASSERT_EQ(std::get<0>(result_00), 2);
        ASSERT_EQ(std::get<1>(result_00), 4);
        ASSERT_EQ(std::get<2>(result_00), 8);
        ASSERT_EQ(std::get<3>(result_00), 16);

        constexpr std::tuple<u8, u8, u8, u8> result_01 = test(0, 1);
        ASSERT_EQ(std::get<0>(result_01), 4);
        ASSERT_EQ(std::get<1>(result_01), 8);
        ASSERT_EQ(std::get<2>(result_01), 16);
        ASSERT_EQ(std::get<3>(result_01), 16);

        constexpr std::tuple<u8, u8, u8, u8> result_02 = test(0, 2);
        ASSERT_EQ(std::get<0>(result_02), 8);
        ASSERT_EQ(std::get<1>(result_02), 16);
        ASSERT_EQ(std::get<2>(result_02), 8);
        ASSERT_EQ(std::get<3>(result_02), 16);

        constexpr std::tuple<u8, u8, u8, u8> result_03 = test(0, 3);
        ASSERT_EQ(std::get<0>(result_03), 16);
        ASSERT_EQ(std::get<1>(result_03), 4);
        ASSERT_EQ(std::get<2>(result_03), 8);
        ASSERT_EQ(std::get<3>(result_03), 16);

        constexpr std::tuple<u8, u8, u8, u8> result_04 = test(0, 4);
        ASSERT_EQ(std::get<0>(result_04), 2);
        ASSERT_EQ(std::get<1>(result_04), 4);
        ASSERT_EQ(std::get<2>(result_04), 8);
        ASSERT_EQ(std::get<3>(result_04), 16);

        constexpr std::tuple<u8, u8, u8, u8> result_10 = test(1, 0);
        ASSERT_EQ(std::get<0>(result_10), 2);
        ASSERT_EQ(std::get<1>(result_10), 2);
        ASSERT_EQ(std::get<2>(result_10), 4);
        ASSERT_EQ(std::get<3>(result_10), 8);

        constexpr std::tuple<u8, u8, u8, u8> result_11 = test(1, 1);
        ASSERT_EQ(std::get<0>(result_11), 2);
        ASSERT_EQ(std::get<1>(result_11), 4);
        ASSERT_EQ(std::get<2>(result_11), 8);
        ASSERT_EQ(std::get<3>(result_11), 16);

        constexpr std::tuple<u8, u8, u8, u8> result_12 = test(1, 2);
        ASSERT_EQ(std::get<0>(result_12), 2);
        ASSERT_EQ(std::get<1>(result_12), 8);
        ASSERT_EQ(std::get<2>(result_12), 16);
        ASSERT_EQ(std::get<3>(result_12), 16);

        constexpr std::tuple<u8, u8, u8, u8> result_13 = test(1, 3);
        ASSERT_EQ(std::get<0>(result_13), 2);
        ASSERT_EQ(std::get<1>(result_13), 16);
        ASSERT_EQ(std::get<2>(result_13), 8);
        ASSERT_EQ(std::get<3>(result_13), 16);

        constexpr std::tuple<u8, u8, u8, u8> result_14 = test(1, 4);
        ASSERT_EQ(std::get<0>(result_14), 2);
        ASSERT_EQ(std::get<1>(result_14), 4);
        ASSERT_EQ(std::get<2>(result_14), 8);
        ASSERT_EQ(std::get<3>(result_14), 16);

        constexpr std::tuple<u8, u8, u8, u8> result_20 = test(2, 0);
        ASSERT_EQ(std::get<0>(result_20), 2);
        ASSERT_EQ(std::get<1>(result_20), 4);
        ASSERT_EQ(std::get<2>(result_20), 2);
        ASSERT_EQ(std::get<3>(result_20), 4);

        constexpr std::tuple<u8, u8, u8, u8> result_21 = test(2, 1);
        ASSERT_EQ(std::get<0>(result_21), 2);
        ASSERT_EQ(std::get<1>(result_21), 4);
        ASSERT_EQ(std::get<2>(result_21), 4);
        ASSERT_EQ(std::get<3>(result_21), 8);

        constexpr std::tuple<u8, u8, u8, u8> result_22 = test(2, 2);
        ASSERT_EQ(std::get<0>(result_22), 2);
        ASSERT_EQ(std::get<1>(result_22), 4);
        ASSERT_EQ(std::get<2>(result_22), 8);
        ASSERT_EQ(std::get<3>(result_22), 16);

        constexpr std::tuple<u8, u8, u8, u8> result_23 = test(2, 3);
        ASSERT_EQ(std::get<0>(result_23), 2);
        ASSERT_EQ(std::get<1>(result_23), 4);
        ASSERT_EQ(std::get<2>(result_23), 16);
        ASSERT_EQ(std::get<3>(result_23), 16);

        constexpr std::tuple<u8, u8, u8, u8> result_24 = test(2, 4);
        ASSERT_EQ(std::get<0>(result_24), 2);
        ASSERT_EQ(std::get<1>(result_24), 4);
        ASSERT_EQ(std::get<2>(result_24), 8);
        ASSERT_EQ(std::get<3>(result_24), 16);

        constexpr std::tuple<u8, u8, u8, u8> result_30 = test(3, 0);
        ASSERT_EQ(std::get<0>(result_30), 2);
        ASSERT_EQ(std::get<1>(result_30), 4);
        ASSERT_EQ(std::get<2>(result_30), 8);
        ASSERT_EQ(std::get<3>(result_30), 2);

        constexpr std::tuple<u8, u8, u8, u8> result_31 = test(3, 1);
        ASSERT_EQ(std::get<0>(result_31), 2);
        ASSERT_EQ(std::get<1>(result_31), 4);
        ASSERT_EQ(std::get<2>(result_31), 8);
        ASSERT_EQ(std::get<3>(result_31), 4);

        constexpr std::tuple<u8, u8, u8, u8> result_32 = test(3, 2);
        ASSERT_EQ(std::get<0>(result_32), 2);
        ASSERT_EQ(std::get<1>(result_32), 4);
        ASSERT_EQ(std::get<2>(result_32), 8);
        ASSERT_EQ(std::get<3>(result_32), 8);

        constexpr std::tuple<u8, u8, u8, u8> result_33 = test(3, 3);
        ASSERT_EQ(std::get<0>(result_33), 2);
        ASSERT_EQ(std::get<1>(result_33), 4);
        ASSERT_EQ(std::get<2>(result_33), 8);
        ASSERT_EQ(std::get<3>(result_33), 16);

        constexpr std::tuple<u8, u8, u8, u8> result_34 = test(3, 4);
        ASSERT_EQ(std::get<0>(result_34), 2);
        ASSERT_EQ(std::get<1>(result_34), 4);
        ASSERT_EQ(std::get<2>(result_34), 8);
        ASSERT_EQ(std::get<3>(result_34), 16);

        constexpr std::tuple<u8, u8, u8, u8> result_40 = test(4, 0);
        ASSERT_EQ(std::get<0>(result_40), 2);
        ASSERT_EQ(std::get<1>(result_40), 4);
        ASSERT_EQ(std::get<2>(result_40), 8);
        ASSERT_EQ(std::get<3>(result_40), 16);

        constexpr std::tuple<u8, u8, u8, u8> result_41 = test(4, 1);
        ASSERT_EQ(std::get<0>(result_41), 2);
        ASSERT_EQ(std::get<1>(result_41), 4);
        ASSERT_EQ(std::get<2>(result_41), 8);
        ASSERT_EQ(std::get<3>(result_41), 16);

        constexpr std::tuple<u8, u8, u8, u8> result_42 = test(4, 2);
        ASSERT_EQ(std::get<0>(result_42), 2);
        ASSERT_EQ(std::get<1>(result_42), 4);
        ASSERT_EQ(std::get<2>(result_42), 8);
        ASSERT_EQ(std::get<3>(result_42), 16);

        constexpr std::tuple<u8, u8, u8, u8> result_43 = test(4, 3);
        ASSERT_EQ(std::get<0>(result_43), 2);
        ASSERT_EQ(std::get<1>(result_43), 4);
        ASSERT_EQ(std::get<2>(result_43), 8);
        ASSERT_EQ(std::get<3>(result_43), 16);

        constexpr std::tuple<u8, u8, u8, u8> result_44 = test(4, 4);
        ASSERT_EQ(std::get<0>(result_44), 2);
        ASSERT_EQ(std::get<1>(result_44), 4);
        ASSERT_EQ(std::get<2>(result_44), 8);
        ASSERT_EQ(std::get<3>(result_44), 16);
    }
}