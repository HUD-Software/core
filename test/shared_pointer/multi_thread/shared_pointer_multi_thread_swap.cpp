#include <core/containers/shared_pointer.h>

GTEST_TEST(shared_pointer_safe, swap_with_empty)
{

    // SharedPointer::swap
    {
        const auto test = []()
        {
            i32 dtor_count = 0;
            auto ptr = new hud_test::non_bitwise_type(1, &dtor_count);
            hud::shared_pointer<hud_test::non_bitwise_type, hud::thread_safety_e::safe> shared_ptr(ptr);
            const bool is_pointer_correct = shared_ptr.pointer();
            const u32 shared_count_before = shared_ptr.shared_count();

            hud::shared_pointer<hud_test::non_bitwise_type, hud::thread_safety_e::safe> empty;
            shared_ptr.swap(empty);

            return std::tuple {
                dtor_count == 0,
                is_pointer_correct,
                shared_count_before == 1u,
                shared_ptr.pointer() == nullptr,
                shared_ptr.shared_count() == 0u
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
        }

        // Constant is not available with thread safe SharedPointer
        /*#if !defined(HD_COMPILER_MSVC)
                {
                    constexpr auto result = test();
                    hud_assert_true(std::get<0>(result));
                    hud_assert_true(std::get<1>(result));
                    hud_assert_true(std::get<2>(result));
                    hud_assert_true(std::get<3>(result));
                    hud_assert_true(std::get<4>(result));
                }
        #endif*/
    }

    // hud::swap
    {
        const auto test = []()
        {
            i32 dtor_count = 0;
            auto ptr = new hud_test::non_bitwise_type(1, &dtor_count);
            hud::shared_pointer<hud_test::non_bitwise_type, hud::thread_safety_e::safe> shared_ptr(ptr);
            const bool is_pointer_correct = shared_ptr.pointer();
            const u32 shared_count_before = shared_ptr.shared_count();

            hud::shared_pointer<hud_test::non_bitwise_type, hud::thread_safety_e::safe> empty;
            swap(shared_ptr, empty);

            return std::tuple {
                dtor_count == 0,
                is_pointer_correct,
                shared_count_before == 1u,
                shared_ptr.pointer() == nullptr,
                shared_ptr.shared_count() == 0u
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
        }

        // Constant is not available with thread safe SharedPointer
        /*#if !defined(HD_COMPILER_MSVC)
                {
                    constexpr auto result = test();
                    hud_assert_true(std::get<0>(result));
                    hud_assert_true(std::get<1>(result));
                    hud_assert_true(std::get<2>(result));
                    hud_assert_true(std::get<3>(result));
                    hud_assert_true(std::get<4>(result));
                }
        #endif*/
    }
}

GTEST_TEST(shared_pointer_safe, swap_same_type)
{

    // SharedPointer::swap
    {
        const auto test = []()
        {
            i32 dtor_count = 0;
            i32 dtor_count_1 = 0;

            auto ptr = new hud_test::non_bitwise_type(1, &dtor_count);
            hud::shared_pointer<hud_test::non_bitwise_type, hud::thread_safety_e::safe> shared_ptr(ptr);

            auto ptr2 = new hud_test::non_bitwise_type(2, &dtor_count_1);
            hud::shared_pointer<hud_test::non_bitwise_type, hud::thread_safety_e::safe> shared_ptr_2(ptr2);
            shared_ptr.swap(shared_ptr_2);

            return std::tuple {
                dtor_count == 0,
                dtor_count_1 == 0,
                shared_ptr.pointer() == ptr2,
                shared_ptr.shared_count() == 1u,
                shared_ptr_2.pointer() == ptr,
                shared_ptr_2.shared_count() == 1u
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
        }

        // Constant is not available with thread safe SharedPointer
        /*#if !defined(HD_COMPILER_MSVC)
                {
                    constexpr auto result = test();
                    hud_assert_true(std::get<0>(result));
                    hud_assert_true(std::get<1>(result));
                    hud_assert_true(std::get<2>(result));
                    hud_assert_true(std::get<3>(result));
                    hud_assert_true(std::get<4>(result));
                    hud_assert_true(std::get<5>(result));
                }
        #endif*/
    }

    // hud::swap
    {
        const auto test = []()
        {
            i32 dtor_count = 0;
            i32 dtor_count_1 = 0;

            auto ptr = new hud_test::non_bitwise_type(1, &dtor_count);
            hud::shared_pointer<hud_test::non_bitwise_type, hud::thread_safety_e::safe> shared_ptr(ptr);

            auto ptr2 = new hud_test::non_bitwise_type(2, &dtor_count_1);
            hud::shared_pointer<hud_test::non_bitwise_type, hud::thread_safety_e::safe> shared_ptr_2(ptr2);
            swap(shared_ptr, shared_ptr_2);

            return std::tuple {
                dtor_count == 0,
                dtor_count_1 == 0,
                shared_ptr.pointer() == ptr2,
                shared_ptr.shared_count() == 1u,
                shared_ptr_2.pointer() == ptr,
                shared_ptr_2.shared_count() == 1u
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
        }

        // Constant is not available with thread safe SharedPointer
        /*#if !defined(HD_COMPILER_MSVC)
                {
                    constexpr auto result = test();
                    hud_assert_true(std::get<0>(result));
                    hud_assert_true(std::get<1>(result));
                    hud_assert_true(std::get<2>(result));
                    hud_assert_true(std::get<3>(result));
                    hud_assert_true(std::get<4>(result));
                    hud_assert_true(std::get<5>(result));
                }
        #endif*/
    }
}