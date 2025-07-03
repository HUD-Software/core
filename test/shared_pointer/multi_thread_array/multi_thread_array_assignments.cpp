#include <core/containers/shared_pointer.h>

GTEST_TEST(shared_pointer_array_safe, copy_assignement_same_type)
{

    const auto test = []()
    {
        i32 dtor_count[2] = {0, 0};
        hud_test::non_bitwise_type *type = new hud_test::non_bitwise_type[2] {
            {1, &dtor_count[0]},
            {2, &dtor_count[1]}
        };
        const hud::shared_pointer<hud_test::non_bitwise_type[2], hud::thread_safety_e::safe> shared_ptr_to_copy(type);
        hud::shared_pointer<hud_test::non_bitwise_type[2], hud::thread_safety_e::safe> shared_ptr;
        shared_ptr = shared_ptr_to_copy;

        return std::tuple {
            shared_ptr_to_copy.pointer() == type,
            shared_ptr_to_copy.shared_count() == 2u,
            shared_ptr.pointer() == type,
            shared_ptr.shared_count() == 2u,
            shared_ptr_to_copy.pointer()[0].id() == 1,
            shared_ptr_to_copy.pointer()[0].constructor_count() == 1u,
            shared_ptr_to_copy.pointer()[0].copy_assign_count() == 0u,
            shared_ptr_to_copy.pointer()[0].move_assign_count() == 0u,
            shared_ptr_to_copy.pointer()[0].copy_constructor_count() == 0u,
            shared_ptr_to_copy.pointer()[0].move_constructor_count() == 0u,
            shared_ptr_to_copy.pointer()[1].id() == 2,
            shared_ptr_to_copy.pointer()[1].constructor_count() == 1u,
            shared_ptr_to_copy.pointer()[1].copy_assign_count() == 0u,
            shared_ptr_to_copy.pointer()[1].move_assign_count() == 0u,
            shared_ptr_to_copy.pointer()[1].copy_constructor_count() == 0u,
            shared_ptr_to_copy.pointer()[1].move_constructor_count() == 0u,
            dtor_count[0] == 0,
            dtor_count[1] == 0,
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
        hud_assert_true(std::get<10>(result));
        hud_assert_true(std::get<11>(result));
        hud_assert_true(std::get<12>(result));
        hud_assert_true(std::get<13>(result));
        hud_assert_true(std::get<14>(result));
        hud_assert_true(std::get<15>(result));
        hud_assert_true(std::get<16>(result));
        hud_assert_true(std::get<17>(result));
    }

    // Constant is not available with thread safe SharedPointer
    // #if !defined(HD_COMPILER_MSVC)
    //     {
    //         constexpr auto result = test();
    //         hud_assert_true(std::get<0>(result));
    //         hud_assert_true(std::get<1>(result));
    //         hud_assert_true(std::get<2>(result));
    //         hud_assert_true(std::get<3>(result));
    //         hud_assert_true(std::get<4>(result));
    //         hud_assert_true(std::get<5>(result));
    //         hud_assert_true(std::get<6>(result));
    //         hud_assert_true(std::get<7>(result));
    //         hud_assert_true(std::get<8>(result));
    //         hud_assert_true(std::get<9>(result));
    //         hud_assert_true(std::get<10>(result));
    //         hud_assert_true(std::get<11>(result));
    //         hud_assert_true(std::get<12>(result));
    //         hud_assert_true(std::get<13>(result));
    //         hud_assert_true(std::get<14>(result));
    //         hud_assert_true(std::get<15>(result));
    //         hud_assert_true(std::get<16>(result));
    //         hud_assert_true(std::get<17>(result));
    //     }
    // #endif
}

GTEST_TEST(shared_pointer_array_safe, copy_assignement_different_type)
{

    const auto test = []()
    {
        i32 dtor_count[2] = {0, 0};
        hud_test::non_bitwise_type *type = new hud_test::non_bitwise_type[2] {
            {1, &dtor_count[0]},
            {2, &dtor_count[1]}
        };
        const hud::shared_pointer<hud_test::non_bitwise_type[2], hud::thread_safety_e::safe> shared_ptr_to_copy(type);
        hud::shared_pointer<hud_test::non_bitwise_type[2], hud::thread_safety_e::safe> shared_ptr;
        shared_ptr = shared_ptr_to_copy;
        return std::tuple {
            shared_ptr_to_copy.pointer() == type,
            shared_ptr_to_copy.shared_count() == 2u,
            shared_ptr.pointer() == type,
            shared_ptr.shared_count() == 2u,
            shared_ptr_to_copy.pointer()[0].id() == 1,
            shared_ptr_to_copy.pointer()[0].constructor_count() == 1u,
            shared_ptr_to_copy.pointer()[0].copy_assign_count() == 0u,
            shared_ptr_to_copy.pointer()[0].move_assign_count() == 0u,
            shared_ptr_to_copy.pointer()[0].copy_constructor_count() == 0u,
            shared_ptr_to_copy.pointer()[0].move_constructor_count() == 0u,
            shared_ptr_to_copy.pointer()[1].id() == 2,
            shared_ptr_to_copy.pointer()[1].constructor_count() == 1u,
            shared_ptr_to_copy.pointer()[1].copy_assign_count() == 0u,
            shared_ptr_to_copy.pointer()[1].move_assign_count() == 0u,
            shared_ptr_to_copy.pointer()[1].copy_constructor_count() == 0u,
            shared_ptr_to_copy.pointer()[1].move_constructor_count() == 0u,
            dtor_count[0] == 0,
            dtor_count[1] == 0,
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
        hud_assert_true(std::get<10>(result));
        hud_assert_true(std::get<11>(result));
        hud_assert_true(std::get<12>(result));
        hud_assert_true(std::get<13>(result));
        hud_assert_true(std::get<14>(result));
        hud_assert_true(std::get<15>(result));
        hud_assert_true(std::get<16>(result));
        hud_assert_true(std::get<17>(result));
    }

    // Constant is not available with thread safe SharedPointer
    // #if !defined(HD_COMPILER_MSVC)
    //     {
    //         constexpr auto result = test();
    //         hud_assert_true(std::get<0>(result));
    //         hud_assert_true(std::get<1>(result));
    //         hud_assert_true(std::get<2>(result));
    //         hud_assert_true(std::get<3>(result));
    //         hud_assert_true(std::get<4>(result));
    //         hud_assert_true(std::get<5>(result));
    //         hud_assert_true(std::get<6>(result));
    //         hud_assert_true(std::get<7>(result));
    //         hud_assert_true(std::get<8>(result));
    //         hud_assert_true(std::get<9>(result));
    //         hud_assert_true(std::get<10>(result));
    //         hud_assert_true(std::get<11>(result));
    //         hud_assert_true(std::get<12>(result));
    //         hud_assert_true(std::get<13>(result));
    //         hud_assert_true(std::get<14>(result));
    //         hud_assert_true(std::get<15>(result));
    //         hud_assert_true(std::get<16>(result));
    //         hud_assert_true(std::get<17>(result));
    //     }
    // #endif
}

GTEST_TEST(shared_pointer_array_safe, copy_assignement_an_empty_same_type)
{

    const auto test = []()
    {
        i32 dtor_count[2] = {0, 0};
        hud_test::non_bitwise_type *type = new hud_test::non_bitwise_type[2] {
            {1, &dtor_count[0]},
            {2, &dtor_count[1]}
        };
        hud::shared_pointer<hud_test::non_bitwise_type[2], hud::thread_safety_e::safe> shared_ptr(type);
        const hud::shared_pointer<hud_test::non_bitwise_type[2], hud::thread_safety_e::safe> empty;
        shared_ptr = empty;

        return std::tuple {
            shared_ptr.pointer() == nullptr,
            shared_ptr.shared_count() == 0u,
            dtor_count[0] == 1,
            dtor_count[1] == 1,
        };
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
    }

    // Constant is not available with thread safe SharedPointer
    // #if !defined(HD_COMPILER_MSVC)
    //     {
    //         constexpr auto result = test();
    //         hud_assert_true(std::get<0>(result));
    //         hud_assert_true(std::get<1>(result));
    //         hud_assert_true(std::get<2>(result));
    //         hud_assert_true(std::get<3>(result));
    //     }
    // #endif
}

GTEST_TEST(shared_pointer_array_safe, copy_assignement_nullptr)
{

    const auto test = []()
    {
        i32 dtor_count[2] = {0, 0};
        hud_test::non_bitwise_type *type = new hud_test::non_bitwise_type[2] {
            {1, &dtor_count[0]},
            {2, &dtor_count[1]}
        };
        hud::shared_pointer<hud_test::non_bitwise_type[2], hud::thread_safety_e::safe> shared_ptr(type);
        shared_ptr = nullptr;

        return std::tuple {
            shared_ptr.pointer() == nullptr,
            shared_ptr.shared_count() == 0u,
            dtor_count[0] == 1,
            dtor_count[1] == 1,
        };
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
    }

    // Constant is not available with thread safe SharedPointer
    // #if !defined(HD_COMPILER_MSVC)
    //     {
    //         constexpr auto result = test();
    //         hud_assert_true(std::get<0>(result));
    //         hud_assert_true(std::get<1>(result));
    //         hud_assert_true(std::get<2>(result));
    //         hud_assert_true(std::get<3>(result));
    //     }
    // #endif
}

GTEST_TEST(shared_pointer_array_safe, move_assignement_same_type)
{

    const auto test = []()
    {
        i32 dtor_count[2] = {0, 0};
        hud_test::non_bitwise_type *type = new hud_test::non_bitwise_type[2] {
            {1, &dtor_count[0]},
            {2, &dtor_count[1]}
        };
        hud::shared_pointer<hud_test::non_bitwise_type[2], hud::thread_safety_e::safe> shared_ptr_to_move(type);
        hud::shared_pointer<hud_test::non_bitwise_type[2], hud::thread_safety_e::safe> shared_ptr;
        shared_ptr = hud::move(shared_ptr_to_move);
        return std::tuple {
            shared_ptr_to_move.pointer() == nullptr,
            shared_ptr_to_move.shared_count() == 0u,
            shared_ptr.pointer() == type,
            shared_ptr.shared_count() == 1u,
            shared_ptr.pointer()[0].id() == 1,
            shared_ptr.pointer()[0].constructor_count() == 1u,
            shared_ptr.pointer()[0].copy_assign_count() == 0u,
            shared_ptr.pointer()[0].move_assign_count() == 0u,
            shared_ptr.pointer()[0].copy_constructor_count() == 0u,
            shared_ptr.pointer()[0].move_constructor_count() == 0u,
            shared_ptr.pointer()[1].id() == 2,
            shared_ptr.pointer()[1].constructor_count() == 1u,
            shared_ptr.pointer()[1].copy_assign_count() == 0u,
            shared_ptr.pointer()[1].move_assign_count() == 0u,
            shared_ptr.pointer()[1].copy_constructor_count() == 0u,
            shared_ptr.pointer()[1].move_constructor_count() == 0u,
            dtor_count[0] == 0,
            dtor_count[1] == 0,
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
        hud_assert_true(std::get<10>(result));
        hud_assert_true(std::get<11>(result));
        hud_assert_true(std::get<12>(result));
        hud_assert_true(std::get<13>(result));
        hud_assert_true(std::get<14>(result));
        hud_assert_true(std::get<15>(result));
        hud_assert_true(std::get<16>(result));
        hud_assert_true(std::get<17>(result));
    }

    // Constant is not available with thread safe SharedPointer
    // #if !defined(HD_COMPILER_MSVC)
    //     {
    //         constexpr auto result = test();
    //         hud_assert_true(std::get<0>(result));
    //         hud_assert_true(std::get<1>(result));
    //         hud_assert_true(std::get<2>(result));
    //         hud_assert_true(std::get<3>(result));
    //         hud_assert_true(std::get<4>(result));
    //         hud_assert_true(std::get<5>(result));
    //         hud_assert_true(std::get<6>(result));
    //         hud_assert_true(std::get<7>(result));
    //         hud_assert_true(std::get<8>(result));
    //         hud_assert_true(std::get<9>(result));
    //         hud_assert_true(std::get<10>(result));
    //         hud_assert_true(std::get<11>(result));
    //         hud_assert_true(std::get<12>(result));
    //         hud_assert_true(std::get<13>(result));
    //         hud_assert_true(std::get<14>(result));
    //         hud_assert_true(std::get<15>(result));
    //         hud_assert_true(std::get<16>(result));
    //         hud_assert_true(std::get<17>(result));
    //     }
    // #endif
}

GTEST_TEST(shared_pointer_array_safe, move_assignement_different_type)
{

    const auto test = []()
    {
        i32 dtor_count[2] = {0, 0};
        hud_test::non_bitwise_type *type = new hud_test::non_bitwise_type[2] {
            {1, &dtor_count[0]},
            {2, &dtor_count[1]}
        };
        hud::shared_pointer<hud_test::non_bitwise_type[2], hud::thread_safety_e::safe> shared_ptr_to_move(type);
        hud::shared_pointer<const hud_test::non_bitwise_type[2], hud::thread_safety_e::safe> shared_ptr;
        shared_ptr = hud::move(shared_ptr_to_move);

        return std::tuple {
            shared_ptr_to_move.pointer() == nullptr,
            shared_ptr_to_move.shared_count() == 0u,
            shared_ptr.pointer() == type,
            shared_ptr.shared_count() == 1u,
            shared_ptr.pointer()[0].id() == 1,
            shared_ptr.pointer()[0].constructor_count() == 1u,
            shared_ptr.pointer()[0].copy_assign_count() == 0u,
            shared_ptr.pointer()[0].move_assign_count() == 0u,
            shared_ptr.pointer()[0].copy_constructor_count() == 0u,
            shared_ptr.pointer()[0].move_constructor_count() == 0u,
            shared_ptr.pointer()[1].id() == 2,
            shared_ptr.pointer()[1].constructor_count() == 1u,
            shared_ptr.pointer()[1].copy_assign_count() == 0u,
            shared_ptr.pointer()[1].move_assign_count() == 0u,
            shared_ptr.pointer()[1].copy_constructor_count() == 0u,
            shared_ptr.pointer()[1].move_constructor_count() == 0u,
            dtor_count[0] == 0,
            dtor_count[1] == 0,
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
        hud_assert_true(std::get<10>(result));
        hud_assert_true(std::get<11>(result));
        hud_assert_true(std::get<12>(result));
        hud_assert_true(std::get<13>(result));
        hud_assert_true(std::get<14>(result));
        hud_assert_true(std::get<15>(result));
        hud_assert_true(std::get<16>(result));
        hud_assert_true(std::get<17>(result));
    }

    // Constant is not available with thread safe SharedPointer
    // #if !defined(HD_COMPILER_MSVC)
    //     {
    //         constexpr auto result = test();
    //         hud_assert_true(std::get<0>(result));
    //         hud_assert_true(std::get<1>(result));
    //         hud_assert_true(std::get<2>(result));
    //         hud_assert_true(std::get<3>(result));
    //         hud_assert_true(std::get<4>(result));
    //         hud_assert_true(std::get<5>(result));
    //         hud_assert_true(std::get<6>(result));
    //         hud_assert_true(std::get<7>(result));
    //         hud_assert_true(std::get<8>(result));
    //         hud_assert_true(std::get<9>(result));
    //         hud_assert_true(std::get<10>(result));
    //         hud_assert_true(std::get<11>(result));
    //         hud_assert_true(std::get<12>(result));
    //         hud_assert_true(std::get<13>(result));
    //         hud_assert_true(std::get<14>(result));
    //         hud_assert_true(std::get<15>(result));
    //         hud_assert_true(std::get<16>(result));
    //         hud_assert_true(std::get<17>(result));
    //     }
    // #endif
}

GTEST_TEST(shared_pointer_array_safe, move_assignement_an_empty_same_type)
{

    const auto test = []()
    {
        i32 dtor_count[2] = {0, 0};
        hud_test::non_bitwise_type *type = new hud_test::non_bitwise_type[2] {
            {1, &dtor_count[0]},
            {2, &dtor_count[1]}
        };
        hud::shared_pointer<hud_test::non_bitwise_type[2], hud::thread_safety_e::safe> shared_ptr(type);
        hud::shared_pointer<hud_test::non_bitwise_type[2], hud::thread_safety_e::safe> empty;
        shared_ptr = hud::move(empty);
        return std::tuple {
            shared_ptr.pointer() == nullptr,
            shared_ptr.shared_count() == 0u,
            dtor_count[0] == 1,
            dtor_count[1] == 1
        };
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
    }

    // Constant is not available with thread safe SharedPointer
    // #if !defined(HD_COMPILER_MSVC)
    //     {
    //         constexpr auto result = test();
    //         hud_assert_true(std::get<0>(result));
    //         hud_assert_true(std::get<1>(result));
    //         hud_assert_true(std::get<2>(result));
    //         hud_assert_true(std::get<3>(result));
    //     }
    // #endif
}

GTEST_TEST(shared_pointer_array_safe, move_assignement_nullptr)
{

    const auto test = []()
    {
        i32 dtor_count[2] = {0, 0};
        hud_test::non_bitwise_type *type = new hud_test::non_bitwise_type[2] {
            {1, &dtor_count[0]},
            {2, &dtor_count[1]}
        };
        hud::shared_pointer<hud_test::non_bitwise_type[2], hud::thread_safety_e::safe> shared_ptr(type);
        shared_ptr = hud::move(nullptr);
        return std::tuple {
            shared_ptr.pointer() == nullptr,
            shared_ptr.shared_count() == 0u,
            dtor_count[0] == 1,
            dtor_count[1] == 1
        };
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
    }

    // Constant is not available with thread safe SharedPointer
    // #if !defined(HD_COMPILER_MSVC)
    //     {
    //         constexpr auto result = test();
    //         hud_assert_true(std::get<0>(result));
    //         hud_assert_true(std::get<1>(result));
    //         hud_assert_true(std::get<2>(result));
    //         hud_assert_true(std::get<3>(result));
    //     }
    // #endif
}

GTEST_TEST(shared_pointer_array_safe, copy_assignement_same_shared_pointer)
{
    const auto test = [](i32 id)
    {
        i32 dtor_count[2] = {0, 0};
        hud_test::non_bitwise_type *type = new hud_test::non_bitwise_type[2] {
            {1, &dtor_count[0]},
            {2, &dtor_count[1]}
        };
        hud::shared_pointer<hud_test::non_bitwise_type[2], hud::thread_safety_e::safe> shared_ptr(type);
        shared_ptr = shared_ptr;
        return std::tuple {
            shared_ptr.pointer() == type,
            shared_ptr.shared_count() == 1u,
            dtor_count[0] == 0,
            dtor_count[1] == 0
        };
    };

    // Non constant
    {
        const auto result = test(123);
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
    }

    // Constant is not available with thread safe SharedPointer
    /*#if !defined(HD_COMPILER_MSVC)
        {
            constexpr auto result = test(123);
            hud_assert_true(std::get<0>(result));
            hud_assert_true(std::get<1>(result));
            hud_assert_true(std::get<2>(result));
            hud_assert_true(std::get<3>(result));
        }
    #endif*/
}

GTEST_TEST(shared_pointer_array_safe, move_assignement_same_shared_pointer)
{
    const auto test = [](i32 id)
    {
        i32 dtor_count[2] = {0, 0};
        hud_test::non_bitwise_type *type = new hud_test::non_bitwise_type[2] {
            {1, &dtor_count[0]},
            {2, &dtor_count[1]}
        };
        hud::shared_pointer<hud_test::non_bitwise_type[2], hud::thread_safety_e::safe> shared_ptr(type);
        shared_ptr = hud::move(shared_ptr);
        return std::tuple {
            shared_ptr.pointer() == type,
            shared_ptr.shared_count() == 1u,
            dtor_count[0] == 0,
            dtor_count[1] == 0
        };
    };

    // Non constant
    {
        const auto result = test(123);
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
    }

    // Constant is not available with thread safe SharedPointer
    /* #if !defined(HD_COMPILER_MSVC)
         {
            constexpr auto result = test(123);
            hud_assert_true(std::get<0>(result));
            hud_assert_true(std::get<1>(result));
            hud_assert_true(std::get<2>(result));
            hud_assert_true(std::get<3>(result));
         }
     #endif*/
}