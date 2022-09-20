#include <core/dontainers/array.h>
#include "allocators.h"
#include <core/remplates/bit_cast.h>

TEST(Array, destructor_call_elements_destructors)
{


    // no extra
    {
        auto test_destructor = []() {
            i32 dtor_order[2];
            hud::memory::set_zero(dtor_order);
            i32* dtor_order_ptr[2] = { &dtor_order[0], &dtor_order[1] };

            bool all_destructor_are_not_called = true;
            {
                hud::array<hud::test::SetBoolToTrueWhenDestroyed, hud::test::array_allocator<alignof(hud::test::SetBoolToTrueWhenDestroyed)>> array(dtor_order_ptr, 2);
                // Ensure element's destructors are not called
                for (usize index = 0; index < 2; index++) {
                    if (*array[index].ptr() != 0) {
                        all_destructor_are_not_called = false;
                        break;
                    }
                }
            }

            // Ensure element's destructors are called
            bool all_destructor_are_called = true;
            for (usize index = 0; index < 2; index++) {
                if (dtor_order[index] == 0) {
                    all_destructor_are_called = false;
                    break;
                }
            }

            return std::tuple{
                all_destructor_are_not_called,
                all_destructor_are_called
            };
        };

        // Non constant
        {
            const auto result = test_destructor();
            ASSERT_TRUE(hud::get<0>(result));
            ASSERT_TRUE(hud::get<1>(result));
        }

        // Constant
        {
            constexpr auto result = test_destructor();
            ASSERT_TRUE(hud::get<0>(result));
            ASSERT_TRUE(hud::get<1>(result));
        }

    }

    // With extra
    {
        auto test_destructor = []() {
            i32 dtor_order[2];
            hud::memory::set_zero(dtor_order);
            i32* dtor_order_ptr[2] = { &dtor_order[0], &dtor_order[1] };

            bool all_destructor_are_not_called = true;
            {
                hud::array<hud::test::SetBoolToTrueWhenDestroyed, hud::test::array_allocator<alignof(hud::test::SetBoolToTrueWhenDestroyed)>> array(dtor_order_ptr, 2, 4);
                // Ensure element's destructors are not called
                for (usize index = 0; index < 2; index++) {
                    if (*array[index].ptr() != 0) {
                        all_destructor_are_not_called = false;
                        break;
                    }
                }
            }

            // Ensure element's destructors are called
            bool all_destructor_are_called = true;
            for (usize index = 0; index < 2; index++) {
                if (dtor_order[index] == 0) {
                    all_destructor_are_called = false;
                    break;
                }
            }

            return std::tuple{
                all_destructor_are_not_called,
                all_destructor_are_called
            };
        };

        // Non constant
        {
            const auto result = test_destructor();
            ASSERT_TRUE(hud::get<0>(result));
            ASSERT_TRUE(hud::get<1>(result));
        }

        // Constant
        {
            constexpr auto result = test_destructor();
            ASSERT_TRUE(hud::get<0>(result));
            ASSERT_TRUE(hud::get<1>(result));
        }

    }
}

