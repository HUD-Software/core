#include <core/containers/shared_pointer.h>
#include <core/traits/is_same.h>

GTEST_TEST(shared_pointer_safe, equal_operator)
{

    const auto test = []()
    {
        hud::shared_pointer<i32, hud::thread_safety_e::safe> empty;
        hud::shared_pointer<i32, hud::thread_safety_e::safe> p = hud::make_shared<i32, hud::thread_safety_e::safe>(1);
        hud::shared_pointer<i32, hud::thread_safety_e::safe> p2(p);
        hud::shared_pointer<i32, hud::thread_safety_e::safe> p3 = hud::make_shared<i32, hud::thread_safety_e::safe>(3);

        return std::tuple {
            empty == empty,
            empty == p,
            p == p2,
            p2 == p,
            p == p3,
            p3 == p,
            empty == nullptr,
            p == nullptr,
            nullptr == empty,
            nullptr == p,
        };
    };

    // Non constant
    {
        const auto result = test();
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_FALSE(std::get<1>(result));
        GTEST_ASSERT_TRUE(std::get<2>(result));
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_FALSE(std::get<4>(result));
        GTEST_ASSERT_FALSE(std::get<5>(result));
        GTEST_ASSERT_TRUE(std::get<6>(result));
        GTEST_ASSERT_FALSE(std::get<7>(result));
        GTEST_ASSERT_TRUE(std::get<8>(result));
        GTEST_ASSERT_FALSE(std::get<9>(result));
    }

    // Constant is not available with thread safe SharedPointer
    // Not working under with msvc
    // https://developercommunity.visualstudio.com/t/constant-evaluation-with-do-not-works-wi/10058244
    /*#if !defined(HD_COMPILER_MSVC)
        {
            constexpr auto result = test();
            GTEST_ASSERT_TRUE(std::get<0>(result));
            GTEST_ASSERT_FALSE(std::get<1>(result));
            GTEST_ASSERT_TRUE(std::get<2>(result));
            GTEST_ASSERT_TRUE(std::get<3>(result));
            GTEST_ASSERT_FALSE(std::get<4>(result));
            GTEST_ASSERT_FALSE(std::get<5>(result));
            GTEST_ASSERT_TRUE(std::get<6>(result));
            GTEST_ASSERT_FALSE(std::get<7>(result));
            GTEST_ASSERT_TRUE(std::get<8>(result));
            GTEST_ASSERT_FALSE(std::get<9>(result));
        }
    #endif*/
}

GTEST_TEST(shared_pointer_safe, not_equal_operator)
{

    const auto test = []()
    {
        hud::shared_pointer<i32, hud::thread_safety_e::safe> empty;
        hud::shared_pointer<i32, hud::thread_safety_e::safe> p = hud::make_shared<i32, hud::thread_safety_e::safe>(1);
        hud::shared_pointer<i32, hud::thread_safety_e::safe> p2(p);
        hud::shared_pointer<i32, hud::thread_safety_e::safe> p3 = hud::make_shared<i32, hud::thread_safety_e::safe>(3);

        return std::tuple {
            empty != empty,
            empty != p,
            p != p2,
            p2 != p,
            p != p3,
            p3 != p,
            empty != nullptr,
            p != nullptr,
            nullptr != empty,
            nullptr != p,
        };
    };

    // Non constant
    {
        const auto result = test();
        GTEST_ASSERT_FALSE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_FALSE(std::get<2>(result));
        GTEST_ASSERT_FALSE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_FALSE(std::get<6>(result));
        GTEST_ASSERT_TRUE(std::get<7>(result));
        GTEST_ASSERT_FALSE(std::get<8>(result));
        GTEST_ASSERT_TRUE(std::get<9>(result));
    }

    // Constant is not available with thread safe SharedPointer
    // Not working under with msvc
    // https://developercommunity.visualstudio.com/t/constant-evaluation-with-do-not-works-wi/10058244
    /* #if !defined(HD_COMPILER_MSVC)
         {
             constexpr auto result = test();
             GTEST_ASSERT_FALSE(std::get<0>(result));
             GTEST_ASSERT_TRUE(std::get<1>(result));
             GTEST_ASSERT_FALSE(std::get<2>(result));
             GTEST_ASSERT_FALSE(std::get<3>(result));
             GTEST_ASSERT_TRUE(std::get<4>(result));
             GTEST_ASSERT_TRUE(std::get<5>(result));
             GTEST_ASSERT_FALSE(std::get<6>(result));
             GTEST_ASSERT_TRUE(std::get<7>(result));
             GTEST_ASSERT_FALSE(std::get<8>(result));
             GTEST_ASSERT_TRUE(std::get<9>(result));
         }
     #endif*/
}

GTEST_TEST(shared_pointer_safe, less_operator)
{

    const auto test = []()
    {
        i32 *ptri_1 = new i32;
        i32 *ptri_2 = new i32;
        i32 *buf[2];

        // Sort pointer in order
        // LCOV_EXCL_START
        if (ptri_1 < ptri_2)
        {
            buf[0] = ptri_1;
            buf[1] = ptri_2;
        }
        else
        {
            buf[0] = ptri_2;
            buf[1] = ptri_1;
        }
        // LCOV_EXCL_STOP
        i32 *ptr1 = buf[0];
        i32 *ptr2 = buf[1];

        hud::shared_pointer<i32, hud::thread_safety_e::safe> empty;
        hud::shared_pointer<i32, hud::thread_safety_e::safe> p1(ptr1);
        hud::shared_pointer<i32, hud::thread_safety_e::safe> p2(ptr2);

        return std::tuple {
            empty < empty,
            empty < p1,
            empty < p2,
            p1 < empty,
            p1 < p1,
            p1 < p2,
            p2 < empty,
            p2 < p1,
            p2 < p2,
            empty < nullptr,
            p1 < nullptr,
            p2 < nullptr,
            nullptr < empty,
            nullptr < p1,
            nullptr < p2,
        };
    };

    // Non constant
    {
        const auto result = test();
        GTEST_ASSERT_FALSE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_TRUE(std::get<2>(result));
        GTEST_ASSERT_FALSE(std::get<3>(result));
        GTEST_ASSERT_FALSE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_FALSE(std::get<6>(result));
        GTEST_ASSERT_FALSE(std::get<7>(result));
        GTEST_ASSERT_FALSE(std::get<8>(result));
        GTEST_ASSERT_FALSE(std::get<9>(result));
        GTEST_ASSERT_FALSE(std::get<10>(result));
        GTEST_ASSERT_FALSE(std::get<11>(result));
        GTEST_ASSERT_FALSE(std::get<12>(result));
        GTEST_ASSERT_TRUE(std::get<13>(result));
        GTEST_ASSERT_TRUE(std::get<14>(result));
    }

    // Constant
    // Not testable due to lack of pointer sort at compile time
    // Not working under with msvc
    // https://developercommunity.visualstudio.com/t/constant-evaluation-with-do-not-works-wi/10058244
    /*#if !defined(HD_COMPILER_MSVC)
        {
            constexpr auto result = test();
            GTEST_ASSERT_FALSE(std::get<0>(result));
            GTEST_ASSERT_FALSE(std::get<1>(result));
            GTEST_ASSERT_FALSE(std::get<2>(result));
            GTEST_ASSERT_FALSE(std::get<3>(result));
            GTEST_ASSERT_FALSE(std::get<4>(result));
            GTEST_ASSERT_TRUE(std::get<5>(result));
            GTEST_ASSERT_FALSE(std::get<6>(result));
            GTEST_ASSERT_FALSE(std::get<7>(result));
            GTEST_ASSERT_FALSE(std::get<8>(result));
            GTEST_ASSERT_FALSE(std::get<9>(result));
            GTEST_ASSERT_FALSE(std::get<10>(result));
            GTEST_ASSERT_FALSE(std::get<11>(result));
            GTEST_ASSERT_FALSE(std::get<12>(result));
            GTEST_ASSERT_TRUE(std::get<13>(result));
            GTEST_ASSERT_TRUE(std::get<14>(result));
        }
    #endif*/
}

GTEST_TEST(shared_pointer_safe, less_equal_operator)
{

    const auto test = []()
    {
        i32 *ptri_1 = new i32;
        i32 *ptri_2 = new i32;
        i32 *buf[2];

        // Sort pointer in order
        // LCOV_EXCL_START
        if (hud::less<i32 *>()(ptri_1, ptri_2))
        {
            buf[0] = ptri_1;
            buf[1] = ptri_2;
        }
        else
        {
            buf[0] = ptri_2;
            buf[1] = ptri_1;
        }
        // LCOV_EXCL_STOP
        i32 *ptr1 = buf[0];
        i32 *ptr2 = buf[1];

        hud::shared_pointer<i32, hud::thread_safety_e::safe> empty;
        hud::shared_pointer<i32, hud::thread_safety_e::safe> p1(ptr1);
        hud::shared_pointer<i32, hud::thread_safety_e::safe> p2(ptr2);

        return std::tuple {
            empty <= empty,
            empty <= p1,
            empty <= p2,
            p1 <= empty,
            p1 <= p1,
            p1 <= p2,
            p2 <= empty,
            p2 <= p1,
            p2 <= p2,
            empty <= nullptr,
            p1 <= nullptr,
            p2 <= nullptr,
            nullptr <= empty,
            nullptr <= p1,
            nullptr <= p2,
        };
    };

    // Non constant
    {
        const auto result = test();
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_TRUE(std::get<2>(result));
        GTEST_ASSERT_FALSE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_FALSE(std::get<6>(result));
        GTEST_ASSERT_FALSE(std::get<7>(result));
        GTEST_ASSERT_TRUE(std::get<8>(result));
        GTEST_ASSERT_TRUE(std::get<9>(result));
        GTEST_ASSERT_FALSE(std::get<10>(result));
        GTEST_ASSERT_FALSE(std::get<11>(result));
        GTEST_ASSERT_TRUE(std::get<12>(result));
        GTEST_ASSERT_TRUE(std::get<13>(result));
        GTEST_ASSERT_TRUE(std::get<14>(result));
    }

    // Constant
    // Not testable due to lack of pointer sort at compile time
    // Not working under with msvc
    // https://developercommunity.visualstudio.com/t/constant-evaluation-with-do-not-works-wi/10058244
    /*#if !defined(HD_COMPILER_MSVC)
        {
            constexpr auto result = test();
            GTEST_ASSERT_TRUE(std::get<0>(result));
            GTEST_ASSERT_TRUE(std::get<1>(result));
            GTEST_ASSERT_TRUE(std::get<2>(result));
            GTEST_ASSERT_FALSE(std::get<3>(result));
            GTEST_ASSERT_TRUE(std::get<4>(result));
            GTEST_ASSERT_TRUE(std::get<5>(result));
            GTEST_ASSERT_FALSE(std::get<6>(result));
            GTEST_ASSERT_FALSE(std::get<7>(result));
            GTEST_ASSERT_TRUE(std::get<8>(result));
            GTEST_ASSERT_TRUE(std::get<9>(result));
            GTEST_ASSERT_FALSE(std::get<10>(result));
            GTEST_ASSERT_FALSE(std::get<11>(result));
            GTEST_ASSERT_TRUE(std::get<12>(result));
            GTEST_ASSERT_TRUE(std::get<13>(result));
            GTEST_ASSERT_TRUE(std::get<14>(result));
        }
    #endif*/
}

GTEST_TEST(shared_pointer_safe, greater_operator)
{

    const auto test = []()
    {
        i32 *ptri_1 = new i32;
        i32 *ptri_2 = new i32;
        i32 *buf[2];

        // Sort pointer in order
        // LCOV_EXCL_START
        if (ptri_1 < ptri_2)
        {
            buf[0] = ptri_1;
            buf[1] = ptri_2;
        }
        else
        {
            buf[0] = ptri_2;
            buf[1] = ptri_1;
        }
        // LCOV_EXCL_STOP
        i32 *ptr1 = buf[0];
        i32 *ptr2 = buf[1];

        hud::shared_pointer<i32, hud::thread_safety_e::safe> empty;
        hud::shared_pointer<i32, hud::thread_safety_e::safe> p1(ptr1);
        hud::shared_pointer<i32, hud::thread_safety_e::safe> p2(ptr2);

        return std::tuple {
            empty > empty,
            empty > p1,
            empty > p2,
            p1 > empty,
            p1 > p1,
            p1 > p2,
            p2 > empty,
            p2 > p1,
            p2 > p2,
            empty > nullptr,
            p1 > nullptr,
            p2 > nullptr,
            nullptr > empty,
            nullptr > p1,
            nullptr > p2,
        };
    };

    // Non constant
    {
        const auto result = test();
        GTEST_ASSERT_FALSE(std::get<0>(result));
        GTEST_ASSERT_FALSE(std::get<1>(result));
        GTEST_ASSERT_FALSE(std::get<2>(result));
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_FALSE(std::get<4>(result));
        GTEST_ASSERT_FALSE(std::get<5>(result));
        GTEST_ASSERT_TRUE(std::get<6>(result));
        GTEST_ASSERT_TRUE(std::get<7>(result));
        GTEST_ASSERT_FALSE(std::get<8>(result));
        GTEST_ASSERT_FALSE(std::get<9>(result));
        GTEST_ASSERT_TRUE(std::get<10>(result));
        GTEST_ASSERT_TRUE(std::get<11>(result));
        GTEST_ASSERT_FALSE(std::get<12>(result));
        GTEST_ASSERT_FALSE(std::get<13>(result));
        GTEST_ASSERT_FALSE(std::get<14>(result));
    }

    // Constant
    // Not testable due to lack of pointer sort at compile time
    // Not working under with msvc
    // https://developercommunity.visualstudio.com/t/constant-evaluation-with-do-not-works-wi/10058244
    /*#if !defined(HD_COMPILER_MSVC)
        {
            constexpr auto result = test();
         GTEST_ASSERT_FALSE(std::get<0>(result));
         GTEST_ASSERT_FALSE(std::get<1>(result));
         GTEST_ASSERT_FALSE(std::get<2>(result));
         GTEST_ASSERT_TRUE(std::get<3>(result));
         GTEST_ASSERT_FALSE(std::get<4>(result));
         GTEST_ASSERT_FALSE(std::get<5>(result));
         GTEST_ASSERT_TRUE(std::get<6>(result));
         GTEST_ASSERT_TRUE(std::get<7>(result));
         GTEST_ASSERT_FALSE(std::get<8>(result));
         GTEST_ASSERT_FALSE(std::get<9>(result));
         GTEST_ASSERT_TRUE(std::get<10>(result));
         GTEST_ASSERT_TRUE(std::get<11>(result));
         GTEST_ASSERT_FALSE(std::get<12>(result));
         GTEST_ASSERT_FALSE(std::get<13>(result));
         GTEST_ASSERT_FALSE(std::get<14>(result));
        }
    #endif*/
}

GTEST_TEST(shared_pointer_safe, greater_equal_operator)
{

    const auto test = []()
    {
        i32 *ptri_1 = new i32;
        i32 *ptri_2 = new i32;
        i32 *buf[2];

        // Sort pointer in order
        // LCOV_EXCL_START
        if (ptri_1 < ptri_2)
        {
            buf[0] = ptri_1;
            buf[1] = ptri_2;
        }
        else
        {
            buf[0] = ptri_2;
            buf[1] = ptri_1;
        }
        // LCOV_EXCL_STOP
        i32 *ptr1 = buf[0];
        i32 *ptr2 = buf[1];

        hud::shared_pointer<i32, hud::thread_safety_e::safe> empty;
        hud::shared_pointer<i32, hud::thread_safety_e::safe> p1(ptr1);
        hud::shared_pointer<i32, hud::thread_safety_e::safe> p2(ptr2);

        return std::tuple {
            empty >= empty,
            empty >= p1,
            empty >= p2,
            p1 >= empty,
            p1 >= p1,
            p1 >= p2,
            p2 >= empty,
            p2 >= p1,
            p2 >= p2,
            empty >= nullptr,
            p1 >= nullptr,
            p2 >= nullptr,
            nullptr >= empty,
            nullptr >= p1,
            nullptr >= p2,
        };
    };

    // Non constant
    {
        const auto result = test();
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_FALSE(std::get<1>(result));
        GTEST_ASSERT_FALSE(std::get<2>(result));
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_FALSE(std::get<5>(result));
        GTEST_ASSERT_TRUE(std::get<6>(result));
        GTEST_ASSERT_TRUE(std::get<7>(result));
        GTEST_ASSERT_TRUE(std::get<8>(result));
        GTEST_ASSERT_TRUE(std::get<9>(result));
        GTEST_ASSERT_TRUE(std::get<10>(result));
        GTEST_ASSERT_TRUE(std::get<11>(result));
        GTEST_ASSERT_TRUE(std::get<12>(result));
        GTEST_ASSERT_FALSE(std::get<13>(result));
        GTEST_ASSERT_FALSE(std::get<14>(result));
    }

    // Constant
    // Not testable due to lack of pointer sort at compile time
    // Not working under with msvc
    // https://developercommunity.visualstudio.com/t/constant-evaluation-with-do-not-works-wi/10058244
    /*#if !defined(HD_COMPILER_MSVC)
        {
            constexpr auto result = test();
            GTEST_ASSERT_TRUE(std::get<0>(result));
            GTEST_ASSERT_FALSE(std::get<1>(result));
            GTEST_ASSERT_FALSE(std::get<2>(result));
            GTEST_ASSERT_TRUE(std::get<3>(result));
            GTEST_ASSERT_TRUE(std::get<4>(result));
            GTEST_ASSERT_FALSE(std::get<5>(result));
            GTEST_ASSERT_TRUE(std::get<6>(result));
            GTEST_ASSERT_TRUE(std::get<7>(result));
            GTEST_ASSERT_TRUE(std::get<8>(result));
            GTEST_ASSERT_TRUE(std::get<9>(result));
            GTEST_ASSERT_TRUE(std::get<10>(result));
            GTEST_ASSERT_TRUE(std::get<11>(result));
            GTEST_ASSERT_TRUE(std::get<12>(result));
            GTEST_ASSERT_FALSE(std::get<13>(result));
            GTEST_ASSERT_FALSE(std::get<14>(result));
        }
    #endif*/
}