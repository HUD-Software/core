#include <core/containers/array.h>
#include "../misc/array_allocators.h"
#include <core/templates/bit_cast.h>

GTEST_TEST(array, destructor_call_elements_destructors)
{

    // no extra
    {
        auto test_destructor = []()
        {
            i32 dtor_order[2];
            hud::memory::set_zero(dtor_order);
            i32 *dtor_order_ptr[2] = {&dtor_order[0], &dtor_order[1]};

            bool all_destructor_are_not_called = true;
            {
                hud::array<hud_test::SetBoolToTrueWhenDestroyed, hud_test::array_allocator<alignof(hud_test::SetBoolToTrueWhenDestroyed)>> array(dtor_order_ptr, 2);
                // Ensure element's destructors are not called
                for (usize index = 0; index < 2; index++)
                {
                    if (*array[index].ptr() != 0)
                    {
                        // LCOV_EXCL_START
                        all_destructor_are_not_called = false;
                        break;
                        // LCOV_EXCL_STOP
                    }
                }
            }

            // Ensure element's destructors are called
            bool all_destructor_are_called = true;
            for (usize index = 0; index < 2; index++)
            {
                if (dtor_order[index] == 0)
                {
                    // LCOV_EXCL_START
                    all_destructor_are_called = false;
                    break;
                    // LCOV_EXCL_STOP
                }
            }

            return std::tuple {
                all_destructor_are_not_called,
                all_destructor_are_called
            };
        };

        // Non constant
        {
            const auto result = test_destructor();
            hud_assert_true(std::get<0>(result));
            hud_assert_true(std::get<1>(result));
        }

        // Constant
        {
            constexpr auto result = test_destructor();
            hud_assert_true(std::get<0>(result));
            hud_assert_true(std::get<1>(result));
        }
    }

    // With extra
    {
        auto test_destructor = []()
        {
            i32 dtor_order[2];
            hud::memory::set_zero(dtor_order);
            i32 *dtor_order_ptr[2] = {&dtor_order[0], &dtor_order[1]};

            bool all_destructor_are_not_called = true;
            {
                hud::array<hud_test::SetBoolToTrueWhenDestroyed, hud_test::array_allocator<alignof(hud_test::SetBoolToTrueWhenDestroyed)>> array(dtor_order_ptr, 2, 4);
                // Ensure element's destructors are not called
                for (usize index = 0; index < 2; index++)
                {
                    if (*array[index].ptr() != 0)
                    {
                        // LCOV_EXCL_START
                        all_destructor_are_not_called = false;
                        break;
                        // LCOV_EXCL_STOP
                    }
                }
            }

            // Ensure element's destructors are called
            bool all_destructor_are_called = true;
            for (usize index = 0; index < 2; index++)
            {
                if (dtor_order[index] == 0)
                {
                    // LCOV_EXCL_START
                    all_destructor_are_called = false;
                    break;
                    // LCOV_EXCL_STOP
                }
            }

            return std::tuple {
                all_destructor_are_not_called,
                all_destructor_are_called
            };
        };

        // Non constant
        {
            const auto result = test_destructor();
            hud_assert_true(std::get<0>(result));
            hud_assert_true(std::get<1>(result));
        }

        // Constant
        {
            constexpr auto result = test_destructor();
            hud_assert_true(std::get<0>(result));
            hud_assert_true(std::get<1>(result));
        }
    }
}
