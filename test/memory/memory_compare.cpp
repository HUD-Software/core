#include <core/memory.h>

GTEST_TEST(memory, compare_pointer_to_buffer)
{

    constexpr u8 buf[3] = {2, 4, 8};
    constexpr u8 buf_2[3] = {2, 4, 7};
    constexpr u8 buf_3[3] = {2, 4, 8};
    constexpr u8 buf_4[3] = {2, 4, 9};

    auto test = [](const u8 *lhs, const u8 *rhs1, const u8 *rhs2, const u8 *rhs3, const u8 *rhs4) -> std::tuple<i32, i32, i32, i32>
    {
        return {
            hud::memory::compare_memory(lhs, rhs1, 3 * sizeof(u8)),
            hud::memory::compare_memory(lhs, rhs2, 3 * sizeof(u8)),
            hud::memory::compare_memory(lhs, rhs3, 3 * sizeof(u8)),
            hud::memory::compare_memory(lhs, rhs4, 3 * sizeof(u8))
        };
    };

    // Non constant
    {
        std::tuple<i32, i32, i32, i32> result_buf = test(buf, buf, buf_2, buf_3, buf_4);
        hud_assert_eq(std::get<0>(result_buf), 0);
        GTEST_ASSERT_GT(std::get<1>(result_buf), 0);
        hud_assert_eq(std::get<2>(result_buf), 0);
        GTEST_ASSERT_LE(std::get<3>(result_buf), 0);

        std::tuple<i32, i32, i32, i32> result_buf_2 = test(buf_2, buf, buf_2, buf_3, buf_4);
        GTEST_ASSERT_LE(std::get<0>(result_buf_2), 0);
        hud_assert_eq(std::get<1>(result_buf_2), 0);
        GTEST_ASSERT_LE(std::get<2>(result_buf_2), 0);
        GTEST_ASSERT_LE(std::get<3>(result_buf_2), 0);

        std::tuple<i32, i32, i32, i32> result_buf_3 = test(buf_3, buf, buf_2, buf_3, buf_4);
        hud_assert_eq(std::get<0>(result_buf_3), 0);
        GTEST_ASSERT_GT(std::get<1>(result_buf_3), 0);
        hud_assert_eq(std::get<2>(result_buf_3), 0);
        GTEST_ASSERT_LE(std::get<3>(result_buf_3), 0);

        std::tuple<i32, i32, i32, i32> result_buf_4 = test(buf_4, buf, buf_2, buf_3, buf_4);
        GTEST_ASSERT_GT(std::get<0>(result_buf_4), 0);
        GTEST_ASSERT_GT(std::get<1>(result_buf_4), 0);
        GTEST_ASSERT_GT(std::get<2>(result_buf_4), 0);
        hud_assert_eq(std::get<3>(result_buf_4), 0);
    }

    // Constant
    {
        constexpr std::tuple<i32, i32, i32, i32> result_buf = test(buf, buf, buf_2, buf_3, buf_4);
        hud_assert_eq(std::get<0>(result_buf), 0);
        GTEST_ASSERT_GT(std::get<1>(result_buf), 0);
        hud_assert_eq(std::get<2>(result_buf), 0);
        GTEST_ASSERT_LE(std::get<3>(result_buf), 0);

        constexpr std::tuple<i32, i32, i32, i32> result_buf_2 = test(buf_2, buf, buf_2, buf_3, buf_4);
        GTEST_ASSERT_LE(std::get<0>(result_buf_2), 0);
        hud_assert_eq(std::get<1>(result_buf_2), 0);
        GTEST_ASSERT_LE(std::get<2>(result_buf_2), 0);
        GTEST_ASSERT_LE(std::get<3>(result_buf_2), 0);

        constexpr std::tuple<i32, i32, i32, i32> result_buf_3 = test(buf_3, buf, buf_2, buf_3, buf_4);
        hud_assert_eq(std::get<0>(result_buf_3), 0);
        GTEST_ASSERT_GT(std::get<1>(result_buf_3), 0);
        hud_assert_eq(std::get<2>(result_buf_3), 0);
        GTEST_ASSERT_LE(std::get<3>(result_buf_3), 0);

        constexpr std::tuple<i32, i32, i32, i32> result_buf_4 = test(buf_4, buf, buf_2, buf_3, buf_4);
        GTEST_ASSERT_GT(std::get<0>(result_buf_4), 0);
        GTEST_ASSERT_GT(std::get<1>(result_buf_4), 0);
        GTEST_ASSERT_GT(std::get<2>(result_buf_4), 0);
        hud_assert_eq(std::get<3>(result_buf_4), 0);
    }
}

GTEST_TEST(memory, compare_C_buffer)
{

    constexpr u8 buf[3] = {2, 4, 8};
    constexpr u8 buf_2[3] = {2, 4, 7};
    constexpr u8 buf_3[3] = {2, 4, 8};
    constexpr u8 buf_4[3] = {2, 4, 9};

    auto test = [](const u8(&lhs)[3], const u8(&rhs1)[3], const u8(&rhs2)[3], const u8(&rhs3)[3], const u8(&rhs4)[3]) -> std::tuple<i32, i32, i32, i32>
    {
        return {
            hud::memory::compare_memory(lhs, rhs1),
            hud::memory::compare_memory(lhs, rhs2),
            hud::memory::compare_memory(lhs, rhs3),
            hud::memory::compare_memory(lhs, rhs4)
        };
    };

    // Non constant
    {
        std::tuple<i32, i32, i32, i32> result_buf = test(buf, buf, buf_2, buf_3, buf_4);
        hud_assert_eq(std::get<0>(result_buf), 0);
        GTEST_ASSERT_GT(std::get<1>(result_buf), 0);
        hud_assert_eq(std::get<2>(result_buf), 0);
        GTEST_ASSERT_LE(std::get<3>(result_buf), 0);

        std::tuple<i32, i32, i32, i32> result_buf_2 = test(buf_2, buf, buf_2, buf_3, buf_4);
        GTEST_ASSERT_LE(std::get<0>(result_buf_2), 0);
        hud_assert_eq(std::get<1>(result_buf_2), 0);
        GTEST_ASSERT_LE(std::get<2>(result_buf_2), 0);
        GTEST_ASSERT_LE(std::get<3>(result_buf_2), 0);

        std::tuple<i32, i32, i32, i32> result_buf_3 = test(buf_3, buf, buf_2, buf_3, buf_4);
        hud_assert_eq(std::get<0>(result_buf_3), 0);
        GTEST_ASSERT_GT(std::get<1>(result_buf_3), 0);
        hud_assert_eq(std::get<2>(result_buf_3), 0);
        GTEST_ASSERT_LE(std::get<3>(result_buf_3), 0);

        std::tuple<i32, i32, i32, i32> result_buf_4 = test(buf_4, buf, buf_2, buf_3, buf_4);
        GTEST_ASSERT_GT(std::get<0>(result_buf_4), 0);
        GTEST_ASSERT_GT(std::get<1>(result_buf_4), 0);
        GTEST_ASSERT_GT(std::get<2>(result_buf_4), 0);
        hud_assert_eq(std::get<3>(result_buf_4), 0);
    }

    // Constant
    {
        constexpr std::tuple<i32, i32, i32, i32> result_buf = test(buf, buf, buf_2, buf_3, buf_4);
        hud_assert_eq(std::get<0>(result_buf), 0);
        GTEST_ASSERT_GT(std::get<1>(result_buf), 0);
        hud_assert_eq(std::get<2>(result_buf), 0);
        GTEST_ASSERT_LE(std::get<3>(result_buf), 0);

        constexpr std::tuple<i32, i32, i32, i32> result_buf_2 = test(buf_2, buf, buf_2, buf_3, buf_4);
        GTEST_ASSERT_LE(std::get<0>(result_buf_2), 0);
        hud_assert_eq(std::get<1>(result_buf_2), 0);
        GTEST_ASSERT_LE(std::get<2>(result_buf_2), 0);
        GTEST_ASSERT_LE(std::get<3>(result_buf_2), 0);

        constexpr std::tuple<i32, i32, i32, i32> result_buf_3 = test(buf_3, buf, buf_2, buf_3, buf_4);
        hud_assert_eq(std::get<0>(result_buf_3), 0);
        GTEST_ASSERT_GT(std::get<1>(result_buf_3), 0);
        hud_assert_eq(std::get<2>(result_buf_3), 0);
        GTEST_ASSERT_LE(std::get<3>(result_buf_3), 0);

        constexpr std::tuple<i32, i32, i32, i32> result_buf_4 = test(buf_4, buf, buf_2, buf_3, buf_4);
        GTEST_ASSERT_GT(std::get<0>(result_buf_4), 0);
        GTEST_ASSERT_GT(std::get<1>(result_buf_4), 0);
        GTEST_ASSERT_GT(std::get<2>(result_buf_4), 0);
        hud_assert_eq(std::get<3>(result_buf_4), 0);
    }
}

GTEST_TEST(memory, compare_equal_pointer_to_buffer)
{

    constexpr u8 buf[3] = {2, 4, 8};
    constexpr u8 buf_2[3] = {2, 4, 7};
    constexpr u8 buf_3[3] = {2, 4, 8};
    constexpr u8 buf_4[3] = {2, 4, 9};

    auto test = [](const u8 *lhs, const u8 *rhs1, const u8 *rhs2, const u8 *rhs3, const u8 *rhs4) -> std::tuple<bool, bool, bool, bool>
    {
        return {
            hud::memory::is_memory_compare_equal(lhs, rhs1, 3 * sizeof(u8)),
            hud::memory::is_memory_compare_equal(lhs, rhs2, 3 * sizeof(u8)),
            hud::memory::is_memory_compare_equal(lhs, rhs3, 3 * sizeof(u8)),
            hud::memory::is_memory_compare_equal(lhs, rhs4, 3 * sizeof(u8))
        };
    };

    // Non constant
    {
        std::tuple<bool, bool, bool, bool> result_buf = test(buf, buf, buf_2, buf_3, buf_4);
        hud_assert_true(std::get<0>(result_buf));
        hud_assert_false(std::get<1>(result_buf));
        hud_assert_true(std::get<2>(result_buf));
        hud_assert_false(std::get<3>(result_buf));

        std::tuple<bool, bool, bool, bool> result_buf_2 = test(buf_2, buf, buf_2, buf_3, buf_4);
        hud_assert_false(std::get<0>(result_buf_2));
        hud_assert_true(std::get<1>(result_buf_2));
        hud_assert_false(std::get<2>(result_buf_2));
        hud_assert_false(std::get<3>(result_buf_2));

        std::tuple<bool, bool, bool, bool> result_buf_3 = test(buf_3, buf, buf_2, buf_3, buf_4);
        hud_assert_true(std::get<0>(result_buf_3));
        hud_assert_false(std::get<1>(result_buf_3));
        hud_assert_true(std::get<2>(result_buf_3));
        hud_assert_false(std::get<3>(result_buf_3));

        std::tuple<bool, bool, bool, bool> result_buf_4 = test(buf_4, buf, buf_2, buf_3, buf_4);
        hud_assert_false(std::get<0>(result_buf_4));
        hud_assert_false(std::get<1>(result_buf_4));
        hud_assert_false(std::get<2>(result_buf_4));
        hud_assert_true(std::get<3>(result_buf_4));
    }

    // Constant
    {
        constexpr std::tuple<bool, bool, bool, bool> result_buf = test(buf, buf, buf_2, buf_3, buf_4);
        hud_assert_true(std::get<0>(result_buf));
        hud_assert_false(std::get<1>(result_buf));
        hud_assert_true(std::get<2>(result_buf));
        hud_assert_false(std::get<3>(result_buf));

        constexpr std::tuple<bool, bool, bool, bool> result_buf_2 = test(buf_2, buf, buf_2, buf_3, buf_4);
        hud_assert_false(std::get<0>(result_buf_2));
        hud_assert_true(std::get<1>(result_buf_2));
        hud_assert_false(std::get<2>(result_buf_2));
        hud_assert_false(std::get<3>(result_buf_2));

        constexpr std::tuple<bool, bool, bool, bool> result_buf_3 = test(buf_3, buf, buf_2, buf_3, buf_4);
        hud_assert_true(std::get<0>(result_buf_3));
        hud_assert_false(std::get<1>(result_buf_3));
        hud_assert_true(std::get<2>(result_buf_3));
        hud_assert_false(std::get<3>(result_buf_3));

        constexpr std::tuple<bool, bool, bool, bool> result_buf_4 = test(buf_4, buf, buf_2, buf_3, buf_4);
        hud_assert_false(std::get<0>(result_buf_4));
        hud_assert_false(std::get<1>(result_buf_4));
        hud_assert_false(std::get<2>(result_buf_4));
        hud_assert_true(std::get<3>(result_buf_4));
    }
}

GTEST_TEST(memory, compare_equal_C_buffer)
{

    constexpr u8 buf[3] = {2, 4, 8};
    constexpr u8 buf_2[3] = {2, 4, 7};
    constexpr u8 buf_3[3] = {2, 4, 8};
    constexpr u8 buf_4[3] = {2, 4, 9};

    auto test = [](const u8(&lhs)[3], const u8(&rhs1)[3], const u8(&rhs2)[3], const u8(&rhs3)[3], const u8(&rhs4)[3]) -> std::tuple<bool, bool, bool, bool>
    {
        return {
            hud::memory::is_memory_compare_equal(lhs, rhs1),
            hud::memory::is_memory_compare_equal(lhs, rhs2),
            hud::memory::is_memory_compare_equal(lhs, rhs3),
            hud::memory::is_memory_compare_equal(lhs, rhs4)
        };
    };

    // Non constant
    {
        std::tuple<bool, bool, bool, bool> result_buf = test(buf, buf, buf_2, buf_3, buf_4);
        hud_assert_true(std::get<0>(result_buf));
        hud_assert_false(std::get<1>(result_buf));
        hud_assert_true(std::get<2>(result_buf));
        hud_assert_false(std::get<3>(result_buf));

        std::tuple<bool, bool, bool, bool> result_buf_2 = test(buf_2, buf, buf_2, buf_3, buf_4);
        hud_assert_false(std::get<0>(result_buf_2));
        hud_assert_true(std::get<1>(result_buf_2));
        hud_assert_false(std::get<2>(result_buf_2));
        hud_assert_false(std::get<3>(result_buf_2));

        std::tuple<bool, bool, bool, bool> result_buf_3 = test(buf_3, buf, buf_2, buf_3, buf_4);
        hud_assert_true(std::get<0>(result_buf_3));
        hud_assert_false(std::get<1>(result_buf_3));
        hud_assert_true(std::get<2>(result_buf_3));
        hud_assert_false(std::get<3>(result_buf_3));

        std::tuple<bool, bool, bool, bool> result_buf_4 = test(buf_4, buf, buf_2, buf_3, buf_4);
        hud_assert_false(std::get<0>(result_buf_4));
        hud_assert_false(std::get<1>(result_buf_4));
        hud_assert_false(std::get<2>(result_buf_4));
        hud_assert_true(std::get<3>(result_buf_4));
    }

    // Constant
    {
        constexpr std::tuple<bool, bool, bool, bool> result_buf = test(buf, buf, buf_2, buf_3, buf_4);
        hud_assert_true(std::get<0>(result_buf));
        hud_assert_false(std::get<1>(result_buf));
        hud_assert_true(std::get<2>(result_buf));
        hud_assert_false(std::get<3>(result_buf));

        constexpr std::tuple<bool, bool, bool, bool> result_buf_2 = test(buf_2, buf, buf_2, buf_3, buf_4);
        hud_assert_false(std::get<0>(result_buf_2));
        hud_assert_true(std::get<1>(result_buf_2));
        hud_assert_false(std::get<2>(result_buf_2));
        hud_assert_false(std::get<3>(result_buf_2));

        constexpr std::tuple<bool, bool, bool, bool> result_buf_3 = test(buf_3, buf, buf_2, buf_3, buf_4);
        hud_assert_true(std::get<0>(result_buf_3));
        hud_assert_false(std::get<1>(result_buf_3));
        hud_assert_true(std::get<2>(result_buf_3));
        hud_assert_false(std::get<3>(result_buf_3));

        constexpr std::tuple<bool, bool, bool, bool> result_buf_4 = test(buf_4, buf, buf_2, buf_3, buf_4);
        hud_assert_false(std::get<0>(result_buf_4));
        hud_assert_false(std::get<1>(result_buf_4));
        hud_assert_false(std::get<2>(result_buf_4));
        hud_assert_true(std::get<3>(result_buf_4));
    }
}

GTEST_TEST(memory, compare_less_pointer_to_buffer)
{

    constexpr u8 buf[3] = {2, 4, 8};
    constexpr u8 buf_2[3] = {2, 4, 7};
    constexpr u8 buf_3[3] = {2, 4, 8};
    constexpr u8 buf_4[3] = {2, 4, 9};

    auto test = [](const u8 *lhs, const u8 *rhs1, const u8 *rhs2, const u8 *rhs3, const u8 *rhs4) -> std::tuple<bool, bool, bool, bool>
    {
        return {
            hud::memory::is_memory_compare_less(lhs, rhs1, 3 * sizeof(u8)),
            hud::memory::is_memory_compare_less(lhs, rhs2, 3 * sizeof(u8)),
            hud::memory::is_memory_compare_less(lhs, rhs3, 3 * sizeof(u8)),
            hud::memory::is_memory_compare_less(lhs, rhs4, 3 * sizeof(u8))
        };
    };

    // Non constant
    {
        std::tuple<bool, bool, bool, bool> result_buf = test(buf, buf, buf_2, buf_3, buf_4);
        hud_assert_false(std::get<0>(result_buf));
        hud_assert_false(std::get<1>(result_buf));
        hud_assert_false(std::get<2>(result_buf));
        hud_assert_true(std::get<3>(result_buf));

        std::tuple<bool, bool, bool, bool> result_buf_2 = test(buf_2, buf, buf_2, buf_3, buf_4);
        hud_assert_true(std::get<0>(result_buf_2));
        hud_assert_false(std::get<1>(result_buf_2));
        hud_assert_true(std::get<2>(result_buf_2));
        hud_assert_true(std::get<3>(result_buf_2));

        std::tuple<bool, bool, bool, bool> result_buf_3 = test(buf_3, buf, buf_2, buf_3, buf_4);
        hud_assert_false(std::get<0>(result_buf_3));
        hud_assert_false(std::get<1>(result_buf_3));
        hud_assert_false(std::get<2>(result_buf_3));
        hud_assert_true(std::get<3>(result_buf_3));

        std::tuple<bool, bool, bool, bool> result_buf_4 = test(buf_4, buf, buf_2, buf_3, buf_4);
        hud_assert_false(std::get<0>(result_buf_4));
        hud_assert_false(std::get<1>(result_buf_4));
        hud_assert_false(std::get<2>(result_buf_4));
        hud_assert_false(std::get<3>(result_buf_4));
    }

    // Constant
    {
        constexpr std::tuple<bool, bool, bool, bool> result_buf = test(buf, buf, buf_2, buf_3, buf_4);
        hud_assert_false(std::get<0>(result_buf));
        hud_assert_false(std::get<1>(result_buf));
        hud_assert_false(std::get<2>(result_buf));
        hud_assert_true(std::get<3>(result_buf));

        constexpr std::tuple<bool, bool, bool, bool> result_buf_2 = test(buf_2, buf, buf_2, buf_3, buf_4);
        hud_assert_true(std::get<0>(result_buf_2));
        hud_assert_false(std::get<1>(result_buf_2));
        hud_assert_true(std::get<2>(result_buf_2));
        hud_assert_true(std::get<3>(result_buf_2));

        constexpr std::tuple<bool, bool, bool, bool> result_buf_3 = test(buf_3, buf, buf_2, buf_3, buf_4);
        hud_assert_false(std::get<0>(result_buf_3));
        hud_assert_false(std::get<1>(result_buf_3));
        hud_assert_false(std::get<2>(result_buf_3));
        hud_assert_true(std::get<3>(result_buf_3));

        constexpr std::tuple<bool, bool, bool, bool> result_buf_4 = test(buf_4, buf, buf_2, buf_3, buf_4);
        hud_assert_false(std::get<0>(result_buf_4));
        hud_assert_false(std::get<1>(result_buf_4));
        hud_assert_false(std::get<2>(result_buf_4));
        hud_assert_false(std::get<3>(result_buf_4));
    }
}

GTEST_TEST(memory, compare_less_c_array)
{

    constexpr u8 buf[3] = {2, 4, 8};
    constexpr u8 buf_2[3] = {2, 4, 7};
    constexpr u8 buf_3[3] = {2, 4, 8};
    constexpr u8 buf_4[3] = {2, 4, 9};

    auto test = [](const u8(&lhs)[3], const u8(&rhs1)[3], const u8(&rhs2)[3], const u8(&rhs3)[3], const u8(&rhs4)[3]) -> std::tuple<bool, bool, bool, bool>
    {
        return {
            hud::memory::is_memory_compare_less(lhs, rhs1),
            hud::memory::is_memory_compare_less(lhs, rhs2),
            hud::memory::is_memory_compare_less(lhs, rhs3),
            hud::memory::is_memory_compare_less(lhs, rhs4)
        };
    };

    // Non constant
    {
        std::tuple<bool, bool, bool, bool> result_buf = test(buf, buf, buf_2, buf_3, buf_4);
        hud_assert_false(std::get<0>(result_buf));
        hud_assert_false(std::get<1>(result_buf));
        hud_assert_false(std::get<2>(result_buf));
        hud_assert_true(std::get<3>(result_buf));

        std::tuple<bool, bool, bool, bool> result_buf_2 = test(buf_2, buf, buf_2, buf_3, buf_4);
        hud_assert_true(std::get<0>(result_buf_2));
        hud_assert_false(std::get<1>(result_buf_2));
        hud_assert_true(std::get<2>(result_buf_2));
        hud_assert_true(std::get<3>(result_buf_2));

        std::tuple<bool, bool, bool, bool> result_buf_3 = test(buf_3, buf, buf_2, buf_3, buf_4);
        hud_assert_false(std::get<0>(result_buf_3));
        hud_assert_false(std::get<1>(result_buf_3));
        hud_assert_false(std::get<2>(result_buf_3));
        hud_assert_true(std::get<3>(result_buf_3));

        std::tuple<bool, bool, bool, bool> result_buf_4 = test(buf_4, buf, buf_2, buf_3, buf_4);
        hud_assert_false(std::get<0>(result_buf_4));
        hud_assert_false(std::get<1>(result_buf_4));
        hud_assert_false(std::get<2>(result_buf_4));
        hud_assert_false(std::get<3>(result_buf_4));
    }

    // Constant
    {
        constexpr std::tuple<bool, bool, bool, bool> result_buf = test(buf, buf, buf_2, buf_3, buf_4);
        hud_assert_false(std::get<0>(result_buf));
        hud_assert_false(std::get<1>(result_buf));
        hud_assert_false(std::get<2>(result_buf));
        hud_assert_true(std::get<3>(result_buf));

        constexpr std::tuple<bool, bool, bool, bool> result_buf_2 = test(buf_2, buf, buf_2, buf_3, buf_4);
        hud_assert_true(std::get<0>(result_buf_2));
        hud_assert_false(std::get<1>(result_buf_2));
        hud_assert_true(std::get<2>(result_buf_2));
        hud_assert_true(std::get<3>(result_buf_2));

        constexpr std::tuple<bool, bool, bool, bool> result_buf_3 = test(buf_3, buf, buf_2, buf_3, buf_4);
        hud_assert_false(std::get<0>(result_buf_3));
        hud_assert_false(std::get<1>(result_buf_3));
        hud_assert_false(std::get<2>(result_buf_3));
        hud_assert_true(std::get<3>(result_buf_3));

        constexpr std::tuple<bool, bool, bool, bool> result_buf_4 = test(buf_4, buf, buf_2, buf_3, buf_4);
        hud_assert_false(std::get<0>(result_buf_4));
        hud_assert_false(std::get<1>(result_buf_4));
        hud_assert_false(std::get<2>(result_buf_4));
        hud_assert_false(std::get<3>(result_buf_4));
    }
}

GTEST_TEST(memory, compare_greater_pointer_to_buffer)
{

    constexpr u8 buf[3] = {2, 4, 8};
    constexpr u8 buf_2[3] = {2, 4, 7};
    constexpr u8 buf_3[3] = {2, 4, 8};
    constexpr u8 buf_4[3] = {2, 4, 9};

    auto test = [](const u8 *lhs, const u8 *rhs1, const u8 *rhs2, const u8 *rhs3, const u8 *rhs4) -> std::tuple<bool, bool, bool, bool>
    {
        return {
            hud::memory::is_memory_compare_greater(lhs, rhs1, 3 * sizeof(u8)),
            hud::memory::is_memory_compare_greater(lhs, rhs2, 3 * sizeof(u8)),
            hud::memory::is_memory_compare_greater(lhs, rhs3, 3 * sizeof(u8)),
            hud::memory::is_memory_compare_greater(lhs, rhs4, 3 * sizeof(u8))
        };
    };

    // Non constant
    {
        std::tuple<bool, bool, bool, bool> result_buf = test(buf, buf, buf_2, buf_3, buf_4);
        hud_assert_false(std::get<0>(result_buf));
        hud_assert_true(std::get<1>(result_buf));
        hud_assert_false(std::get<2>(result_buf));
        hud_assert_false(std::get<3>(result_buf));

        std::tuple<bool, bool, bool, bool> result_buf_2 = test(buf_2, buf, buf_2, buf_3, buf_4);
        hud_assert_false(std::get<0>(result_buf_2));
        hud_assert_false(std::get<1>(result_buf_2));
        hud_assert_false(std::get<2>(result_buf_2));
        hud_assert_false(std::get<3>(result_buf_2));

        std::tuple<bool, bool, bool, bool> result_buf_3 = test(buf_3, buf, buf_2, buf_3, buf_4);
        hud_assert_false(std::get<0>(result_buf_3));
        hud_assert_true(std::get<1>(result_buf_3));
        hud_assert_false(std::get<2>(result_buf_3));
        hud_assert_false(std::get<3>(result_buf_3));

        std::tuple<bool, bool, bool, bool> result_buf_4 = test(buf_4, buf, buf_2, buf_3, buf_4);
        hud_assert_true(std::get<0>(result_buf_4));
        hud_assert_true(std::get<1>(result_buf_4));
        hud_assert_true(std::get<2>(result_buf_4));
        hud_assert_false(std::get<3>(result_buf_4));
    }

    // Constant
    {
        constexpr std::tuple<bool, bool, bool, bool> result_buf = test(buf, buf, buf_2, buf_3, buf_4);
        hud_assert_false(std::get<0>(result_buf));
        hud_assert_true(std::get<1>(result_buf));
        hud_assert_false(std::get<2>(result_buf));
        hud_assert_false(std::get<3>(result_buf));

        constexpr std::tuple<bool, bool, bool, bool> result_buf_2 = test(buf_2, buf, buf_2, buf_3, buf_4);
        hud_assert_false(std::get<0>(result_buf_2));
        hud_assert_false(std::get<1>(result_buf_2));
        hud_assert_false(std::get<2>(result_buf_2));
        hud_assert_false(std::get<3>(result_buf_2));

        constexpr std::tuple<bool, bool, bool, bool> result_buf_3 = test(buf_3, buf, buf_2, buf_3, buf_4);
        hud_assert_false(std::get<0>(result_buf_3));
        hud_assert_true(std::get<1>(result_buf_3));
        hud_assert_false(std::get<2>(result_buf_3));
        hud_assert_false(std::get<3>(result_buf_3));

        constexpr std::tuple<bool, bool, bool, bool> result_buf_4 = test(buf_4, buf, buf_2, buf_3, buf_4);
        hud_assert_true(std::get<0>(result_buf_4));
        hud_assert_true(std::get<1>(result_buf_4));
        hud_assert_true(std::get<2>(result_buf_4));
        hud_assert_false(std::get<3>(result_buf_4));
    }
}

GTEST_TEST(memory, compare_greater_c_array)
{

    constexpr u8 buf[3] = {2, 4, 8};
    constexpr u8 buf_2[3] = {2, 4, 7};
    constexpr u8 buf_3[3] = {2, 4, 8};
    constexpr u8 buf_4[3] = {2, 4, 9};

    auto test = [](const u8(&lhs)[3], const u8(&rhs1)[3], const u8(&rhs2)[3], const u8(&rhs3)[3], const u8(&rhs4)[3]) -> std::tuple<bool, bool, bool, bool>
    {
        return {
            hud::memory::is_memory_compare_greater(lhs, rhs1),
            hud::memory::is_memory_compare_greater(lhs, rhs2),
            hud::memory::is_memory_compare_greater(lhs, rhs3),
            hud::memory::is_memory_compare_greater(lhs, rhs4)
        };
    };

    // Non constant
    {
        std::tuple<bool, bool, bool, bool> result_buf = test(buf, buf, buf_2, buf_3, buf_4);
        hud_assert_false(std::get<0>(result_buf));
        hud_assert_true(std::get<1>(result_buf));
        hud_assert_false(std::get<2>(result_buf));
        hud_assert_false(std::get<3>(result_buf));

        std::tuple<bool, bool, bool, bool> result_buf_2 = test(buf_2, buf, buf_2, buf_3, buf_4);
        hud_assert_false(std::get<0>(result_buf_2));
        hud_assert_false(std::get<1>(result_buf_2));
        hud_assert_false(std::get<2>(result_buf_2));
        hud_assert_false(std::get<3>(result_buf_2));

        std::tuple<bool, bool, bool, bool> result_buf_3 = test(buf_3, buf, buf_2, buf_3, buf_4);
        hud_assert_false(std::get<0>(result_buf_3));
        hud_assert_true(std::get<1>(result_buf_3));
        hud_assert_false(std::get<2>(result_buf_3));
        hud_assert_false(std::get<3>(result_buf_3));

        std::tuple<bool, bool, bool, bool> result_buf_4 = test(buf_4, buf, buf_2, buf_3, buf_4);
        hud_assert_true(std::get<0>(result_buf_4));
        hud_assert_true(std::get<1>(result_buf_4));
        hud_assert_true(std::get<2>(result_buf_4));
        hud_assert_false(std::get<3>(result_buf_4));
    }

    // Constant
    {
        constexpr std::tuple<bool, bool, bool, bool> result_buf = test(buf, buf, buf_2, buf_3, buf_4);
        hud_assert_false(std::get<0>(result_buf));
        hud_assert_true(std::get<1>(result_buf));
        hud_assert_false(std::get<2>(result_buf));
        hud_assert_false(std::get<3>(result_buf));

        constexpr std::tuple<bool, bool, bool, bool> result_buf_2 = test(buf_2, buf, buf_2, buf_3, buf_4);
        hud_assert_false(std::get<0>(result_buf_2));
        hud_assert_false(std::get<1>(result_buf_2));
        hud_assert_false(std::get<2>(result_buf_2));
        hud_assert_false(std::get<3>(result_buf_2));

        constexpr std::tuple<bool, bool, bool, bool> result_buf_3 = test(buf_3, buf, buf_2, buf_3, buf_4);
        hud_assert_false(std::get<0>(result_buf_3));
        hud_assert_true(std::get<1>(result_buf_3));
        hud_assert_false(std::get<2>(result_buf_3));
        hud_assert_false(std::get<3>(result_buf_3));

        constexpr std::tuple<bool, bool, bool, bool> result_buf_4 = test(buf_4, buf, buf_2, buf_3, buf_4);
        hud_assert_true(std::get<0>(result_buf_4));
        hud_assert_true(std::get<1>(result_buf_4));
        hud_assert_true(std::get<2>(result_buf_4));
        hud_assert_false(std::get<3>(result_buf_4));
    }
}

GTEST_TEST(memory, compare_greater_void_pointer_to_buffer)
{

    constexpr u8 buf[3] = {2, 4, 8};
    constexpr u8 buf_2[3] = {2, 4, 7};
    constexpr u8 buf_3[3] = {2, 4, 8};
    constexpr u8 buf_4[3] = {2, 4, 9};

    auto test = [](const u8 *lhs, const u8 *rhs1, const u8 *rhs2, const u8 *rhs3, const u8 *rhs4) -> std::tuple<bool, bool, bool, bool>
    {
        return {
            hud::memory::is_memory_compare_greater(static_cast<const void *>(lhs), rhs1, 3 * sizeof(u8)),
            hud::memory::is_memory_compare_greater(static_cast<const void *>(lhs), rhs2, 3 * sizeof(u8)),
            hud::memory::is_memory_compare_greater(static_cast<const void *>(lhs), rhs3, 3 * sizeof(u8)),
            hud::memory::is_memory_compare_greater(static_cast<const void *>(lhs), rhs4, 3 * sizeof(u8))
        };
    };

    // Non constant
    {
        std::tuple<bool, bool, bool, bool> result_buf = test(buf, buf, buf_2, buf_3, buf_4);
        hud_assert_false(std::get<0>(result_buf));
        hud_assert_true(std::get<1>(result_buf));
        hud_assert_false(std::get<2>(result_buf));
        hud_assert_false(std::get<3>(result_buf));

        std::tuple<bool, bool, bool, bool> result_buf_2 = test(buf_2, buf, buf_2, buf_3, buf_4);
        hud_assert_false(std::get<0>(result_buf_2));
        hud_assert_false(std::get<1>(result_buf_2));
        hud_assert_false(std::get<2>(result_buf_2));
        hud_assert_false(std::get<3>(result_buf_2));

        std::tuple<bool, bool, bool, bool> result_buf_3 = test(buf_3, buf, buf_2, buf_3, buf_4);
        hud_assert_false(std::get<0>(result_buf_3));
        hud_assert_true(std::get<1>(result_buf_3));
        hud_assert_false(std::get<2>(result_buf_3));
        hud_assert_false(std::get<3>(result_buf_3));

        std::tuple<bool, bool, bool, bool> result_buf_4 = test(buf_4, buf, buf_2, buf_3, buf_4);
        hud_assert_true(std::get<0>(result_buf_4));
        hud_assert_true(std::get<1>(result_buf_4));
        hud_assert_true(std::get<2>(result_buf_4));
        hud_assert_false(std::get<3>(result_buf_4));
    }
}