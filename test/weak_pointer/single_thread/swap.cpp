#include <core/containers/shared_pointer.h>

GTEST_TEST(weak_pointer_not_safe, swap_with_empty)
{
    // weak_pointer::swap
    {
        const auto test = []()
        {
            i32 dtor_count = 0;
            auto ptr = new hud_test::non_bitwise_type(1, &dtor_count);
            hud::shared_pointer<hud_test::non_bitwise_type> shared_ptr(ptr); // +1 shared_count
            hud::weak_pointer<hud_test::non_bitwise_type> weak_ptr(shared_ptr);
            hud::weak_pointer<hud_test::non_bitwise_type> empty;
            const bool is_pointer_correct = shared_ptr.pointer() && weak_ptr.lock().pointer();
            const u32 shared_count_before = shared_ptr.shared_count();
            weak_ptr.swap(empty);
            const auto locked_weak_ptr = weak_ptr.lock();
            const auto locked_empty = empty.lock(); // +1 shared_count

            return std::tuple {
                dtor_count == 0,
                is_pointer_correct,
                shared_count_before == 1u,
                shared_ptr.pointer() == ptr,
                shared_ptr.shared_count() == 2u,
                locked_weak_ptr.pointer() == nullptr,
                locked_weak_ptr.shared_count() == 0u,
                locked_empty.pointer() == ptr,
                locked_empty.shared_count() == 2u,
                locked_empty.pointer()->id() == 1,
            };
        };

        // Non constant
        {
            const auto result = test();
            hud_assert_true(std::get<0>(result));
            hud_assert_true(std::get<1>(result));
            hud_assert_true(std::get<2>(result));
            hud_assert_true(std::get<3>(result));
            hud_assert_true(std::get<4>(result));
            hud_assert_true(std::get<5>(result));
            hud_assert_true(std::get<6>(result));
            hud_assert_true(std::get<7>(result));
            hud_assert_true(std::get<8>(result));
            hud_assert_true(std::get<9>(result));
        }

        // Constant
        // Not working under with msvc
        // https://developercommunity.visualstudio.com/t/constant-evaluation-with-do-not-works-wi/10058244
        // #if !defined(HD_COMPILER_MSVC)
        // {
        //     constexpr auto result = test();
        //     hud_assert_true(std::get<0>(result));
        //     hud_assert_true(std::get<1>(result));
        //     hud_assert_true(std::get<2>(result));
        //     hud_assert_true(std::get<3>(result));
        //     hud_assert_true(std::get<4>(result));
        // }
        // #endif
    }

    // hud::swap
    {
        const auto test = []()
        {
            i32 dtor_count = 0;
            auto ptr = new hud_test::non_bitwise_type(1, &dtor_count);
            hud::shared_pointer<hud_test::non_bitwise_type> shared_ptr(ptr);
            hud::weak_pointer<hud_test::non_bitwise_type> weak_ptr(shared_ptr);
            hud::weak_pointer<hud_test::non_bitwise_type> empty;
            const bool is_pointer_correct = shared_ptr.pointer() && weak_ptr.lock().pointer();
            const u32 shared_count_before = shared_ptr.shared_count();
            hud::swap(weak_ptr, empty);
            const auto locked_weak_ptr = weak_ptr.lock();
            const auto locked_empty = empty.lock(); // +1 shared_count

            return std::tuple {
                dtor_count == 0,
                is_pointer_correct,
                shared_count_before == 1u,
                shared_ptr.pointer() == ptr,
                shared_ptr.shared_count() == 2u,
                locked_weak_ptr.pointer() == nullptr,
                locked_weak_ptr.shared_count() == 0u,
                locked_empty.pointer() == ptr,
                locked_empty.shared_count() == 2u,
                locked_empty.pointer()->id() == 1,
            };
        };

        // Non constant
        {
            const auto result = test();
            hud_assert_true(std::get<0>(result));
            hud_assert_true(std::get<1>(result));
            hud_assert_true(std::get<2>(result));
            hud_assert_true(std::get<3>(result));
            hud_assert_true(std::get<4>(result));
            hud_assert_true(std::get<5>(result));
            hud_assert_true(std::get<6>(result));
            hud_assert_true(std::get<7>(result));
            hud_assert_true(std::get<8>(result));
            hud_assert_true(std::get<9>(result));
        }

        // Constant
        // Not working under with msvc
        // https://developercommunity.visualstudio.com/t/constant-evaluation-with-do-not-works-wi/10058244
        // #if !defined(HD_COMPILER_MSVC)
        // {
        //     constexpr auto result = test();
        //     hud_assert_true(std::get<0>(result));
        //     hud_assert_true(std::get<1>(result));
        //     hud_assert_true(std::get<2>(result));
        //     hud_assert_true(std::get<3>(result));
        //     hud_assert_true(std::get<4>(result));
        // }
        // #endif
    }
}

GTEST_TEST(weak_pointer_not_safe, swap_same_type)
{
    // weak_pointer::swap
    {
        const auto test = []()
        {
            i32 dtor_count = 0;
            i32 dtor_count_1 = 0;

            const auto ptr = new hud_test::non_bitwise_type(1, &dtor_count);
            hud::shared_pointer<hud_test::non_bitwise_type> shared_ptr(ptr); // +1 shared_count
            hud::weak_pointer<hud_test::non_bitwise_type> weak_ptr(shared_ptr);

            const auto ptr2 = new hud_test::non_bitwise_type(2, &dtor_count_1);
            hud::shared_pointer<hud_test::non_bitwise_type> shared_ptr_2(ptr2); // +1 shared_count
            hud::weak_pointer<hud_test::non_bitwise_type> weak_ptr_2(shared_ptr_2);

            weak_ptr.swap(weak_ptr_2);

            const auto locked_weak_ptr = weak_ptr.lock();     // +1 shared_count
            const auto locked_weak_ptr_2 = weak_ptr_2.lock(); // +1 shared_count

            return std::tuple {
                dtor_count == 0,
                dtor_count_1 == 0,
                locked_weak_ptr.pointer() == ptr2,
                locked_weak_ptr.shared_count() == 2u,
                locked_weak_ptr.pointer()->id() == 2,
                locked_weak_ptr_2.pointer() == ptr,
                locked_weak_ptr_2.shared_count() == 2u,
                locked_weak_ptr_2.pointer()->id() == 1,
            };
        };

        // Non constant
        {
            const auto result = test();
            hud_assert_true(std::get<0>(result));
            hud_assert_true(std::get<1>(result));
            hud_assert_true(std::get<2>(result));
            hud_assert_true(std::get<3>(result));
            hud_assert_true(std::get<4>(result));
            hud_assert_true(std::get<5>(result));
            hud_assert_true(std::get<6>(result));
            hud_assert_true(std::get<7>(result));
        }

        // Constant
        // Not working under with msvc
        // https://developercommunity.visualstudio.com/t/constant-evaluation-with-do-not-works-wi/10058244
        // #if !defined(HD_COMPILER_MSVC)
        // {
        //     constexpr auto result = test();
        //     hud_assert_true(std::get<0>(result));
        //     hud_assert_true(std::get<1>(result));
        //     hud_assert_true(std::get<2>(result));
        //     hud_assert_true(std::get<3>(result));
        //     hud_assert_true(std::get<4>(result));
        //     hud_assert_true(std::get<5>(result));
        // }
        // #endif
    }

    // hud::swap
    {
        const auto test = []()
        {
            i32 dtor_count = 0;
            i32 dtor_count_1 = 0;

            const auto ptr = new hud_test::non_bitwise_type(1, &dtor_count);
            hud::shared_pointer<hud_test::non_bitwise_type> shared_ptr(ptr); // +1 shared_count
            hud::weak_pointer<hud_test::non_bitwise_type> weak_ptr(shared_ptr);

            const auto ptr2 = new hud_test::non_bitwise_type(2, &dtor_count_1);
            hud::shared_pointer<hud_test::non_bitwise_type> shared_ptr_2(ptr2); // +1 shared_count
            hud::weak_pointer<hud_test::non_bitwise_type> weak_ptr_2(shared_ptr_2);

            hud::swap(weak_ptr, weak_ptr_2);

            const auto locked_weak_ptr = weak_ptr.lock();     // +1 shared_count
            const auto locked_weak_ptr_2 = weak_ptr_2.lock(); // +1 shared_count

            return std::tuple {
                dtor_count == 0,
                dtor_count_1 == 0,
                locked_weak_ptr.pointer() == ptr2,
                locked_weak_ptr.shared_count() == 2u,
                locked_weak_ptr.pointer()->id() == 2,
                locked_weak_ptr_2.pointer() == ptr,
                locked_weak_ptr_2.shared_count() == 2u,
                locked_weak_ptr_2.pointer()->id() == 1,
            };
        };

        // Non constant
        {
            const auto result = test();
            hud_assert_true(std::get<0>(result));
            hud_assert_true(std::get<1>(result));
            hud_assert_true(std::get<2>(result));
            hud_assert_true(std::get<3>(result));
            hud_assert_true(std::get<4>(result));
            hud_assert_true(std::get<5>(result));
            hud_assert_true(std::get<6>(result));
            hud_assert_true(std::get<7>(result));
        }

        // Constant
        // Not working under with msvc
        // https://developercommunity.visualstudio.com/t/constant-evaluation-with-do-not-works-wi/10058244
        // #if !defined(HD_COMPILER_MSVC)
        // {
        //     constexpr auto result = test();
        //     hud_assert_true(std::get<0>(result));
        //     hud_assert_true(std::get<1>(result));
        //     hud_assert_true(std::get<2>(result));
        //     hud_assert_true(std::get<3>(result));
        //     hud_assert_true(std::get<4>(result));
        //     hud_assert_true(std::get<5>(result));
        // }
        // #endif
    }
}