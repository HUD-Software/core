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
        hud_assert_true(std::get<0>(result));
        hud_assert_false(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_false(std::get<4>(result));
        hud_assert_false(std::get<5>(result));
        hud_assert_true(std::get<6>(result));
        hud_assert_false(std::get<7>(result));
        hud_assert_true(std::get<8>(result));
        hud_assert_false(std::get<9>(result));
    }

    // Constant is not available with thread safe SharedPointer
    // Not working under with msvc
    // https://developercommunity.visualstudio.com/t/constant-evaluation-with-do-not-works-wi/10058244
    /*#if !defined(HD_COMPILER_MSVC)
        {
            constexpr auto result = test();
            hud_assert_true(std::get<0>(result));
            hud_assert_false(std::get<1>(result));
            hud_assert_true(std::get<2>(result));
            hud_assert_true(std::get<3>(result));
            hud_assert_false(std::get<4>(result));
            hud_assert_false(std::get<5>(result));
            hud_assert_true(std::get<6>(result));
            hud_assert_false(std::get<7>(result));
            hud_assert_true(std::get<8>(result));
            hud_assert_false(std::get<9>(result));
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
        hud_assert_false(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_false(std::get<2>(result));
        hud_assert_false(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_false(std::get<6>(result));
        hud_assert_true(std::get<7>(result));
        hud_assert_false(std::get<8>(result));
        hud_assert_true(std::get<9>(result));
    }

    // Constant is not available with thread safe SharedPointer
    // Not working under with msvc
    // https://developercommunity.visualstudio.com/t/constant-evaluation-with-do-not-works-wi/10058244
    /* #if !defined(HD_COMPILER_MSVC)
         {
             constexpr auto result = test();
             hud_assert_false(std::get<0>(result));
             hud_assert_true(std::get<1>(result));
             hud_assert_false(std::get<2>(result));
             hud_assert_false(std::get<3>(result));
             hud_assert_true(std::get<4>(result));
             hud_assert_true(std::get<5>(result));
             hud_assert_false(std::get<6>(result));
             hud_assert_true(std::get<7>(result));
             hud_assert_false(std::get<8>(result));
             hud_assert_true(std::get<9>(result));
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
        hud_assert_false(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_false(std::get<3>(result));
        hud_assert_false(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_false(std::get<6>(result));
        hud_assert_false(std::get<7>(result));
        hud_assert_false(std::get<8>(result));
        hud_assert_false(std::get<9>(result));
        hud_assert_false(std::get<10>(result));
        hud_assert_false(std::get<11>(result));
        hud_assert_false(std::get<12>(result));
        hud_assert_true(std::get<13>(result));
        hud_assert_true(std::get<14>(result));
    }

    // Constant
    // Not testable due to lack of pointer sort at compile time
    // Not working under with msvc
    // https://developercommunity.visualstudio.com/t/constant-evaluation-with-do-not-works-wi/10058244
    /*#if !defined(HD_COMPILER_MSVC)
        {
            constexpr auto result = test();
            hud_assert_false(std::get<0>(result));
            hud_assert_false(std::get<1>(result));
            hud_assert_false(std::get<2>(result));
            hud_assert_false(std::get<3>(result));
            hud_assert_false(std::get<4>(result));
            hud_assert_true(std::get<5>(result));
            hud_assert_false(std::get<6>(result));
            hud_assert_false(std::get<7>(result));
            hud_assert_false(std::get<8>(result));
            hud_assert_false(std::get<9>(result));
            hud_assert_false(std::get<10>(result));
            hud_assert_false(std::get<11>(result));
            hud_assert_false(std::get<12>(result));
            hud_assert_true(std::get<13>(result));
            hud_assert_true(std::get<14>(result));
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
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_false(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_false(std::get<6>(result));
        hud_assert_false(std::get<7>(result));
        hud_assert_true(std::get<8>(result));
        hud_assert_true(std::get<9>(result));
        hud_assert_false(std::get<10>(result));
        hud_assert_false(std::get<11>(result));
        hud_assert_true(std::get<12>(result));
        hud_assert_true(std::get<13>(result));
        hud_assert_true(std::get<14>(result));
    }

    // Constant
    // Not testable due to lack of pointer sort at compile time
    // Not working under with msvc
    // https://developercommunity.visualstudio.com/t/constant-evaluation-with-do-not-works-wi/10058244
    /*#if !defined(HD_COMPILER_MSVC)
        {
            constexpr auto result = test();
            hud_assert_true(std::get<0>(result));
            hud_assert_true(std::get<1>(result));
            hud_assert_true(std::get<2>(result));
            hud_assert_false(std::get<3>(result));
            hud_assert_true(std::get<4>(result));
            hud_assert_true(std::get<5>(result));
            hud_assert_false(std::get<6>(result));
            hud_assert_false(std::get<7>(result));
            hud_assert_true(std::get<8>(result));
            hud_assert_true(std::get<9>(result));
            hud_assert_false(std::get<10>(result));
            hud_assert_false(std::get<11>(result));
            hud_assert_true(std::get<12>(result));
            hud_assert_true(std::get<13>(result));
            hud_assert_true(std::get<14>(result));
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
        hud_assert_false(std::get<0>(result));
        hud_assert_false(std::get<1>(result));
        hud_assert_false(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_false(std::get<4>(result));
        hud_assert_false(std::get<5>(result));
        hud_assert_true(std::get<6>(result));
        hud_assert_true(std::get<7>(result));
        hud_assert_false(std::get<8>(result));
        hud_assert_false(std::get<9>(result));
        hud_assert_true(std::get<10>(result));
        hud_assert_true(std::get<11>(result));
        hud_assert_false(std::get<12>(result));
        hud_assert_false(std::get<13>(result));
        hud_assert_false(std::get<14>(result));
    }

    // Constant
    // Not testable due to lack of pointer sort at compile time
    // Not working under with msvc
    // https://developercommunity.visualstudio.com/t/constant-evaluation-with-do-not-works-wi/10058244
    /*#if !defined(HD_COMPILER_MSVC)
        {
            constexpr auto result = test();
         hud_assert_false(std::get<0>(result));
         hud_assert_false(std::get<1>(result));
         hud_assert_false(std::get<2>(result));
         hud_assert_true(std::get<3>(result));
         hud_assert_false(std::get<4>(result));
         hud_assert_false(std::get<5>(result));
         hud_assert_true(std::get<6>(result));
         hud_assert_true(std::get<7>(result));
         hud_assert_false(std::get<8>(result));
         hud_assert_false(std::get<9>(result));
         hud_assert_true(std::get<10>(result));
         hud_assert_true(std::get<11>(result));
         hud_assert_false(std::get<12>(result));
         hud_assert_false(std::get<13>(result));
         hud_assert_false(std::get<14>(result));
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
        hud_assert_true(std::get<0>(result));
        hud_assert_false(std::get<1>(result));
        hud_assert_false(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_false(std::get<5>(result));
        hud_assert_true(std::get<6>(result));
        hud_assert_true(std::get<7>(result));
        hud_assert_true(std::get<8>(result));
        hud_assert_true(std::get<9>(result));
        hud_assert_true(std::get<10>(result));
        hud_assert_true(std::get<11>(result));
        hud_assert_true(std::get<12>(result));
        hud_assert_false(std::get<13>(result));
        hud_assert_false(std::get<14>(result));
    }

    // Constant
    // Not testable due to lack of pointer sort at compile time
    // Not working under with msvc
    // https://developercommunity.visualstudio.com/t/constant-evaluation-with-do-not-works-wi/10058244
    /*#if !defined(HD_COMPILER_MSVC)
        {
            constexpr auto result = test();
            hud_assert_true(std::get<0>(result));
            hud_assert_false(std::get<1>(result));
            hud_assert_false(std::get<2>(result));
            hud_assert_true(std::get<3>(result));
            hud_assert_true(std::get<4>(result));
            hud_assert_false(std::get<5>(result));
            hud_assert_true(std::get<6>(result));
            hud_assert_true(std::get<7>(result));
            hud_assert_true(std::get<8>(result));
            hud_assert_true(std::get<9>(result));
            hud_assert_true(std::get<10>(result));
            hud_assert_true(std::get<11>(result));
            hud_assert_true(std::get<12>(result));
            hud_assert_false(std::get<13>(result));
            hud_assert_false(std::get<14>(result));
        }
    #endif*/
}