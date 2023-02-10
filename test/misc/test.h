#ifndef HD_INC_TEST_MISC_TEST_H
#define HD_INC_TEST_MISC_TEST_H

namespace hud_test
{
    template<typename A>
    void assert_true(A condition, const char *const condition_str, int line, const char *file) noexcept
    {
        // LCOV_EXCL_START
        try
        {
            GTEST_ASSERT_TRUE(condition) << file << " " << line << ": " << condition_str << " != true";
        }
        catch (const std::exception &e)
        {
            GTEST_FAIL() << "Exception is throw: " << e.what();
        }
        catch (...)
        {
            GTEST_FAIL();
        }
        // LCOV_EXCL_STOP
    }

    template<typename A>
    void assert_false(A condition, const char *const condition_str, int line, const char *file) noexcept
    {
        // LCOV_EXCL_START
        try
        {
            GTEST_ASSERT_FALSE(condition) << file << " " << line << ": " << condition_str << " != false";
        }
        catch (const std::exception &e)
        {
            GTEST_FAIL() << "Exception is throw: " << e.what();
        }
        catch (...)
        {
            GTEST_FAIL();
        }
        // LCOV_EXCL_STOP
    }

    template<typename A, typename B>
    void assert_eq(A &&a, B &&b, const char *const a_str, const char *const b_str, int line, const char *file) noexcept
    {
        // LCOV_EXCL_START
        try
        {
            GTEST_ASSERT_EQ(a, b) << file << " " << line << ": " << a_str << " == " << b_str;
        }
        catch (const std::exception &e)
        {
            GTEST_FAIL() << "Exception is throw: " << e.what();
        }
        catch (...)
        {
            GTEST_FAIL();
        }
        // LCOV_EXCL_STOP
    }

    template<typename A, typename B>
    void assert_ne(A &&a, B &&b, const char *const a_str, const char *const b_str, int line, const char *file) noexcept
    {
        // LCOV_EXCL_START
        try
        {
            GTEST_ASSERT_NE(a, b) << file << " " << line << ": " << a_str << " != " << b_str;
        }
        catch (const std::exception &e)
        {
            GTEST_FAIL() << "Exception is throw: " << e.what();
        }
        catch (...)
        {
            GTEST_FAIL();
        }
        // LCOV_EXCL_STOP
    }
} // namespace hud_test

#define hud_assert_true(condition) hud_test::assert_true(condition, #condition, __LINE__, __FILE__)
#define hud_assert_false(condition) hud_test::assert_false(condition, #condition, __LINE__, __FILE__)
#define hud_assert_eq(a, b) hud_test::assert_eq(a, b, #a, #b, __LINE__, __FILE__)
#define hud_assert_ne(a, b) hud_test::assert_ne(a, b, #a, #b, __LINE__, __FILE__)

#endif // HD_INC_TEST_MISC_TEST_H