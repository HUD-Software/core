#include <core/containers/shared_pointer.h>

GTEST_TEST(weak_pointer_array_not_safe, default_constructor)
{

    const auto test = []()
    {
        hud::weak_pointer<hud_test::non_bitwise_type[2]> weak_ptr;
        return std::tuple {
            weak_ptr.lock().pointer() == nullptr,
            weak_ptr.lock().shared_count()};
    };

    // Non constant
    {
        const auto result = test();
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
    }

    // Constant
    {
        constexpr auto result = test();
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
    }
}

GTEST_TEST(weak_pointer_array_not_safe, param_constructor_same_type)
{
    const auto test = [](i32 id)
    {
        i32 dtor_count[2] = {0, 0};
        hud_test::non_bitwise_type *type = new hud_test::non_bitwise_type[2] {{1, &dtor_count[0]}, {2, &dtor_count[1]}};
        hud::shared_pointer<hud_test::non_bitwise_type[2]> shared_ptr(type);
        hud::weak_pointer<hud_test::non_bitwise_type[2]> weak_ptr {shared_ptr};
        return std::tuple {
            shared_ptr.pointer() == type,
            shared_ptr.shared_count() == 1u,
            shared_ptr.pointer()[0].id() == 1,
            shared_ptr.pointer()[1].id() == 2,
            dtor_count[0] == 0,
            dtor_count[1] == 0,
            weak_ptr.lock().pointer() == type};
    };

    // Non constant
    {
        const auto result = test(123);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_TRUE(std::get<2>(result));
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_TRUE(std::get<6>(result));
    }

    // Constant
    // Not working under with msvc
    // https://developercommunity.visualstudio.com/t/constant-evaluation-with-do-not-works-wi/10058244
    // #if !defined(HD_COMPILER_MSVC)
    //     {
    //         constexpr auto result = test(123);
    //                 GTEST_ASSERT_TRUE(std::get<0>(result));
    // GTEST_ASSERT_TRUE(std::get<1>(result));
    // GTEST_ASSERT_TRUE(std::get<2>(result));
    // GTEST_ASSERT_TRUE(std::get<3>(result));
    // GTEST_ASSERT_TRUE(std::get<3>(result));
    // GTEST_ASSERT_TRUE(std::get<4>(result));
    // GTEST_ASSERT_TRUE(std::get<5>(result));
    // GTEST_ASSERT_TRUE(std::get<6>(result));
    //     }
    // #endif
}

GTEST_TEST(weak_pointer_array_not_safe, param_constructor_different_type)
{
    const auto test = [](i32 id)
    {
        i32 dtor_count[2] = {0, 0};
        hud_test::non_bitwise_type *type = new hud_test::non_bitwise_type[2] {{1, &dtor_count[0]}, {2, &dtor_count[1]}};
        hud::shared_pointer<hud_test::non_bitwise_type[2]> shared_ptr(type);
        hud::weak_pointer<const hud_test::non_bitwise_type[2]> weak_ptr {shared_ptr};
        return std::tuple {
            shared_ptr.pointer() == type,
            shared_ptr.shared_count() == 1u,
            shared_ptr.pointer()[0].id() == 1,
            shared_ptr.pointer()[1].id() == 2,
            dtor_count[0] == 0,
            dtor_count[1] == 0,
            weak_ptr.lock().pointer() == type};
    };

    // Non constant
    {
        const auto result = test(123);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_TRUE(std::get<2>(result));
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_TRUE(std::get<6>(result));
    }

    // Constant
    // Not working under with msvc
    // https://developercommunity.visualstudio.com/t/constant-evaluation-with-do-not-works-wi/10058244
    // #if !defined(HD_COMPILER_MSVC)
    //     {
    //         constexpr auto result = test(123);
    //        GTEST_ASSERT_TRUE(std::get<0>(result));
    // GTEST_ASSERT_TRUE(std::get<1>(result));
    // GTEST_ASSERT_TRUE(std::get<2>(result));
    // GTEST_ASSERT_TRUE(std::get<3>(result));
    // GTEST_ASSERT_TRUE(std::get<3>(result));
    // GTEST_ASSERT_TRUE(std::get<4>(result));
    // GTEST_ASSERT_TRUE(std::get<5>(result));
    // GTEST_ASSERT_TRUE(std::get<6>(result));
    //     }
    // #endif
}

GTEST_TEST(weak_pointer_array_not_safe, copy_constructor_same_type)
{
    const auto test = [](i32 id)
    {
        i32 dtor_count[2] = {0, 0};
        hud_test::non_bitwise_type *type = new hud_test::non_bitwise_type[2] {{1, &dtor_count[0]}, {2, &dtor_count[1]}};
        const hud::shared_pointer<hud_test::non_bitwise_type[2]> shared_ptr {type}; // +1 shared_count
        const hud::weak_pointer<hud_test::non_bitwise_type[2]> weak_ptr_to_copy {shared_ptr};
        hud::weak_pointer<hud_test::non_bitwise_type[2]> weak_ptr_copy {weak_ptr_to_copy};
        const auto locked_weak_ptr_to_copy = weak_ptr_to_copy.lock(); // +1 shared_count
        const auto locked_weak_ptr_copy = weak_ptr_copy.lock();       // +1 shared_count

        return std::tuple {
            locked_weak_ptr_to_copy.pointer() == type,
            locked_weak_ptr_to_copy.shared_count() == 3u,
            locked_weak_ptr_copy.pointer() == type,
            locked_weak_ptr_copy.shared_count() == 3u,
            locked_weak_ptr_copy.pointer()[0].id() == 1,
            locked_weak_ptr_copy.pointer()[0].constructor_count() == 1u,
            locked_weak_ptr_copy.pointer()[0].copy_assign_count() == 0u,
            locked_weak_ptr_copy.pointer()[0].move_assign_count() == 0u,
            locked_weak_ptr_copy.pointer()[0].copy_constructor_count() == 0u,
            locked_weak_ptr_copy.pointer()[0].move_constructor_count() == 0u,
            locked_weak_ptr_copy.pointer()[1].id() == 2,
            locked_weak_ptr_copy.pointer()[1].constructor_count() == 1u,
            locked_weak_ptr_copy.pointer()[1].copy_assign_count() == 0u,
            locked_weak_ptr_copy.pointer()[1].move_assign_count() == 0u,
            locked_weak_ptr_copy.pointer()[1].copy_constructor_count() == 0u,
            locked_weak_ptr_copy.pointer()[1].move_constructor_count() == 0u,
            dtor_count[0] == 0,
            dtor_count[1] == 0};
    };

    // Non constant
    {
        const auto result = test(123);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_TRUE(std::get<2>(result));
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_TRUE(std::get<6>(result));
        GTEST_ASSERT_TRUE(std::get<7>(result));
        GTEST_ASSERT_TRUE(std::get<8>(result));
        GTEST_ASSERT_TRUE(std::get<9>(result));
        GTEST_ASSERT_TRUE(std::get<10>(result));
        GTEST_ASSERT_TRUE(std::get<11>(result));
        GTEST_ASSERT_TRUE(std::get<12>(result));
        GTEST_ASSERT_TRUE(std::get<13>(result));
        GTEST_ASSERT_TRUE(std::get<14>(result));
        GTEST_ASSERT_TRUE(std::get<15>(result));
        GTEST_ASSERT_TRUE(std::get<16>(result));
        GTEST_ASSERT_TRUE(std::get<17>(result));
    }

    // Constant
    // Not working under with msvc
    // https://developercommunity.visualstudio.com/t/constant-evaluation-with-do-not-works-wi/10058244
    // #if !defined(HD_COMPILER_MSVC)
    //     {
    //         constexpr auto result = test(123);
    //         GTEST_ASSERT_TRUE(std::get<0>(result));
    // GTEST_ASSERT_TRUE(std::get<1>(result));
    // GTEST_ASSERT_TRUE(std::get<2>(result));
    // GTEST_ASSERT_TRUE(std::get<3>(result));
    // GTEST_ASSERT_TRUE(std::get<4>(result));
    // GTEST_ASSERT_TRUE(std::get<5>(result));
    // GTEST_ASSERT_TRUE(std::get<6>(result));
    // GTEST_ASSERT_TRUE(std::get<7>(result));
    // GTEST_ASSERT_TRUE(std::get<8>(result));
    // GTEST_ASSERT_TRUE(std::get<9>(result));
    // GTEST_ASSERT_TRUE(std::get<10>(result));
    // GTEST_ASSERT_TRUE(std::get<11>(result));
    // GTEST_ASSERT_TRUE(std::get<12>(result));
    // GTEST_ASSERT_TRUE(std::get<13>(result));
    // GTEST_ASSERT_TRUE(std::get<14>(result));
    // GTEST_ASSERT_TRUE(std::get<15>(result));
    // GTEST_ASSERT_TRUE(std::get<16>(result));
    // GTEST_ASSERT_TRUE(std::get<17>(result));
    //     }
    // #endif
}

GTEST_TEST(weak_pointer_array_not_safe, copy_constructor_different_type)
{

    const auto test = [](i32 id)
    {
        i32 dtor_count[2] = {0, 0};
        hud_test::non_bitwise_type *type = new hud_test::non_bitwise_type[2] {{1, &dtor_count[0]}, {2, &dtor_count[1]}};
        const hud::shared_pointer<hud_test::non_bitwise_type[2]> shared_ptr(type);
        const hud::weak_pointer<hud_test::non_bitwise_type[2]> weak_ptr_to_copy {shared_ptr};
        hud::weak_pointer<const hud_test::non_bitwise_type[2]> weak_ptr_copy {weak_ptr_to_copy};
        const auto locked_weak_ptr_to_copy = weak_ptr_to_copy.lock(); // +1 shared_count
        const auto locked_weak_ptr_copy = weak_ptr_copy.lock();       // +1 shared_count

        return std::tuple {
            locked_weak_ptr_to_copy.pointer() == type,
            locked_weak_ptr_to_copy.shared_count() == 3u,
            locked_weak_ptr_copy.pointer() == type,
            locked_weak_ptr_copy.shared_count() == 3u,
            locked_weak_ptr_copy.pointer()[0].id() == 1,
            locked_weak_ptr_copy.pointer()[0].constructor_count() == 1u,
            locked_weak_ptr_copy.pointer()[0].copy_assign_count() == 0u,
            locked_weak_ptr_copy.pointer()[0].move_assign_count() == 0u,
            locked_weak_ptr_copy.pointer()[0].copy_constructor_count() == 0u,
            locked_weak_ptr_copy.pointer()[0].move_constructor_count() == 0u,
            locked_weak_ptr_copy.pointer()[1].id() == 2,
            locked_weak_ptr_copy.pointer()[1].constructor_count() == 1u,
            locked_weak_ptr_copy.pointer()[1].copy_assign_count() == 0u,
            locked_weak_ptr_copy.pointer()[1].move_assign_count() == 0u,
            locked_weak_ptr_copy.pointer()[1].copy_constructor_count() == 0u,
            locked_weak_ptr_copy.pointer()[1].move_constructor_count() == 0u,
            dtor_count[0] == 0,
            dtor_count[1] == 0};
    };

    // Non constant
    {
        const auto result = test(123);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_TRUE(std::get<2>(result));
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_TRUE(std::get<6>(result));
        GTEST_ASSERT_TRUE(std::get<7>(result));
        GTEST_ASSERT_TRUE(std::get<8>(result));
        GTEST_ASSERT_TRUE(std::get<9>(result));
        GTEST_ASSERT_TRUE(std::get<10>(result));
        GTEST_ASSERT_TRUE(std::get<11>(result));
        GTEST_ASSERT_TRUE(std::get<12>(result));
        GTEST_ASSERT_TRUE(std::get<13>(result));
        GTEST_ASSERT_TRUE(std::get<14>(result));
        GTEST_ASSERT_TRUE(std::get<15>(result));
        GTEST_ASSERT_TRUE(std::get<16>(result));
        GTEST_ASSERT_TRUE(std::get<17>(result));
    }

    // Constant
    // Not working under with msvc
    // https://developercommunity.visualstudio.com/t/constant-evaluation-with-do-not-works-wi/10058244
    // #if !defined(HD_COMPILER_MSVC)
    //     {
    //         constexpr auto result = test(123);
    //         GTEST_ASSERT_TRUE(std::get<0>(result));
    // GTEST_ASSERT_TRUE(std::get<1>(result));
    // GTEST_ASSERT_TRUE(std::get<2>(result));
    // GTEST_ASSERT_TRUE(std::get<3>(result));
    // GTEST_ASSERT_TRUE(std::get<4>(result));
    // GTEST_ASSERT_TRUE(std::get<5>(result));
    // GTEST_ASSERT_TRUE(std::get<6>(result));
    // GTEST_ASSERT_TRUE(std::get<7>(result));
    // GTEST_ASSERT_TRUE(std::get<8>(result));
    // GTEST_ASSERT_TRUE(std::get<9>(result));
    // GTEST_ASSERT_TRUE(std::get<10>(result));
    // GTEST_ASSERT_TRUE(std::get<11>(result));
    // GTEST_ASSERT_TRUE(std::get<12>(result));
    // GTEST_ASSERT_TRUE(std::get<13>(result));
    // GTEST_ASSERT_TRUE(std::get<14>(result));
    // GTEST_ASSERT_TRUE(std::get<15>(result));
    // GTEST_ASSERT_TRUE(std::get<16>(result));
    // GTEST_ASSERT_TRUE(std::get<17>(result));
    //     }
    // #endif
}

GTEST_TEST(weak_pointer_array_not_safe, move_constructor_same_type)
{

    const auto test = [](i32 id)
    {
        i32 dtor_count[2] = {0, 0};
        hud_test::non_bitwise_type *type = new hud_test::non_bitwise_type[2] {{1, &dtor_count[0]}, {2, &dtor_count[1]}};
        const hud::shared_pointer<hud_test::non_bitwise_type[2]> shared_ptr {type}; // +1 shared_count
        hud::weak_pointer<hud_test::non_bitwise_type[2]> weak_ptr_to_move {shared_ptr};
        hud::weak_pointer<hud_test::non_bitwise_type[2]> weak_ptr_move {hud::move(weak_ptr_to_move)};
        const auto locked_weak_ptr_to_move = weak_ptr_to_move.lock();
        const auto locked_weak_ptr_move = weak_ptr_move.lock(); // +1 shared_count

        return std::tuple {
            locked_weak_ptr_to_move.pointer() == nullptr,
            locked_weak_ptr_to_move.shared_count() == 0u,
            locked_weak_ptr_move.pointer() == type,
            locked_weak_ptr_move.shared_count() == 2u,
            locked_weak_ptr_move.pointer()[0].id() == 1,
            locked_weak_ptr_move.pointer()[0].constructor_count() == 1u,
            locked_weak_ptr_move.pointer()[0].copy_assign_count() == 0u,
            locked_weak_ptr_move.pointer()[0].move_assign_count() == 0u,
            locked_weak_ptr_move.pointer()[0].copy_constructor_count() == 0u,
            locked_weak_ptr_move.pointer()[0].move_constructor_count() == 0u,
            locked_weak_ptr_move.pointer()[1].id() == 2,
            locked_weak_ptr_move.pointer()[1].constructor_count() == 1u,
            locked_weak_ptr_move.pointer()[1].copy_assign_count() == 0u,
            locked_weak_ptr_move.pointer()[1].move_assign_count() == 0u,
            locked_weak_ptr_move.pointer()[1].copy_constructor_count() == 0u,
            locked_weak_ptr_move.pointer()[1].move_constructor_count() == 0u,
            dtor_count[0] == 0,
            dtor_count[1] == 0};
    };

    // Non constant
    {
        const auto result = test(123);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_TRUE(std::get<2>(result));
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_TRUE(std::get<6>(result));
        GTEST_ASSERT_TRUE(std::get<7>(result));
        GTEST_ASSERT_TRUE(std::get<8>(result));
        GTEST_ASSERT_TRUE(std::get<9>(result));
        GTEST_ASSERT_TRUE(std::get<10>(result));
        GTEST_ASSERT_TRUE(std::get<11>(result));
        GTEST_ASSERT_TRUE(std::get<12>(result));
        GTEST_ASSERT_TRUE(std::get<13>(result));
        GTEST_ASSERT_TRUE(std::get<14>(result));
        GTEST_ASSERT_TRUE(std::get<15>(result));
        GTEST_ASSERT_TRUE(std::get<16>(result));
        GTEST_ASSERT_TRUE(std::get<17>(result));
    }

    // Constant
    // Not working under with msvc
    // https://developercommunity.visualstudio.com/t/constant-evaluation-with-do-not-works-wi/10058244
    // #if !defined(HD_COMPILER_MSVC)
    //     {
    //         constexpr auto result = test(123);
    // GTEST_ASSERT_TRUE(std::get<0>(result));
    // GTEST_ASSERT_TRUE(std::get<1>(result));
    // GTEST_ASSERT_TRUE(std::get<2>(result));
    // GTEST_ASSERT_TRUE(std::get<3>(result));
    // GTEST_ASSERT_TRUE(std::get<4>(result));
    // GTEST_ASSERT_TRUE(std::get<5>(result));
    // GTEST_ASSERT_TRUE(std::get<6>(result));
    // GTEST_ASSERT_TRUE(std::get<7>(result));
    // GTEST_ASSERT_TRUE(std::get<8>(result));
    // GTEST_ASSERT_TRUE(std::get<9>(result));
    // GTEST_ASSERT_TRUE(std::get<10>(result));
    // GTEST_ASSERT_TRUE(std::get<11>(result));
    // GTEST_ASSERT_TRUE(std::get<12>(result));
    // GTEST_ASSERT_TRUE(std::get<13>(result));
    // GTEST_ASSERT_TRUE(std::get<14>(result));
    // GTEST_ASSERT_TRUE(std::get<15>(result));
    // GTEST_ASSERT_TRUE(std::get<16>(result));
    // GTEST_ASSERT_TRUE(std::get<17>(result));
    //     }
    // #endif
}

GTEST_TEST(weak_pointer_array_not_safe, move_constructor_different_type)
{

    const auto test = [](i32 id)
    {
        i32 dtor_count[2] = {0, 0};
        hud_test::non_bitwise_type *type = new hud_test::non_bitwise_type[2] {{1, &dtor_count[0]}, {2, &dtor_count[1]}};
        const hud::shared_pointer<hud_test::non_bitwise_type[2]> shared_ptr {type}; // +1 shared_count
        hud::weak_pointer<hud_test::non_bitwise_type[2]> weak_ptr_to_move {shared_ptr};
        hud::weak_pointer<const hud_test::non_bitwise_type[2]> weak_ptr_move {hud::move(weak_ptr_to_move)};
        const auto locked_weak_ptr_to_move = weak_ptr_to_move.lock();
        const auto locked_weak_ptr_move = weak_ptr_move.lock(); // +1 shared_count

        return std::tuple {
            locked_weak_ptr_to_move.pointer() == nullptr,
            locked_weak_ptr_to_move.shared_count() == 0u,
            locked_weak_ptr_move.pointer() == type,
            locked_weak_ptr_move.shared_count() == 2u,
            locked_weak_ptr_move.pointer()[0].id() == 1,
            locked_weak_ptr_move.pointer()[0].constructor_count() == 1u,
            locked_weak_ptr_move.pointer()[0].copy_assign_count() == 0u,
            locked_weak_ptr_move.pointer()[0].move_assign_count() == 0u,
            locked_weak_ptr_move.pointer()[0].copy_constructor_count() == 0u,
            locked_weak_ptr_move.pointer()[0].move_constructor_count() == 0u,
            locked_weak_ptr_move.pointer()[1].id() == 2,
            locked_weak_ptr_move.pointer()[1].constructor_count() == 1u,
            locked_weak_ptr_move.pointer()[1].copy_assign_count() == 0u,
            locked_weak_ptr_move.pointer()[1].move_assign_count() == 0u,
            locked_weak_ptr_move.pointer()[1].copy_constructor_count() == 0u,
            locked_weak_ptr_move.pointer()[1].move_constructor_count() == 0u,
            dtor_count[0] == 0,
            dtor_count[1] == 0};
    };

    // Non constant
    {
        const auto result = test(123);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_TRUE(std::get<2>(result));
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_TRUE(std::get<6>(result));
        GTEST_ASSERT_TRUE(std::get<7>(result));
        GTEST_ASSERT_TRUE(std::get<8>(result));
        GTEST_ASSERT_TRUE(std::get<9>(result));
        GTEST_ASSERT_TRUE(std::get<10>(result));
        GTEST_ASSERT_TRUE(std::get<11>(result));
        GTEST_ASSERT_TRUE(std::get<12>(result));
        GTEST_ASSERT_TRUE(std::get<13>(result));
        GTEST_ASSERT_TRUE(std::get<14>(result));
        GTEST_ASSERT_TRUE(std::get<15>(result));
        GTEST_ASSERT_TRUE(std::get<16>(result));
        GTEST_ASSERT_TRUE(std::get<17>(result));
    }

    // Constant
    // Not working under with msvc
    // https://developercommunity.visualstudio.com/t/constant-evaluation-with-do-not-works-wi/10058244
    // #if !defined(HD_COMPILER_MSVC)
    //     {
    //         constexpr auto result = test(123);
    // GTEST_ASSERT_TRUE(std::get<0>(result));
    // GTEST_ASSERT_TRUE(std::get<1>(result));
    // GTEST_ASSERT_TRUE(std::get<2>(result));
    // GTEST_ASSERT_TRUE(std::get<3>(result));
    // GTEST_ASSERT_TRUE(std::get<4>(result));
    // GTEST_ASSERT_TRUE(std::get<5>(result));
    // GTEST_ASSERT_TRUE(std::get<6>(result));
    // GTEST_ASSERT_TRUE(std::get<7>(result));
    // GTEST_ASSERT_TRUE(std::get<8>(result));
    // GTEST_ASSERT_TRUE(std::get<9>(result));
    // GTEST_ASSERT_TRUE(std::get<10>(result));
    // GTEST_ASSERT_TRUE(std::get<11>(result));
    // GTEST_ASSERT_TRUE(std::get<12>(result));
    // GTEST_ASSERT_TRUE(std::get<13>(result));
    // GTEST_ASSERT_TRUE(std::get<14>(result));
    // GTEST_ASSERT_TRUE(std::get<15>(result));
    // GTEST_ASSERT_TRUE(std::get<16>(result));
    // GTEST_ASSERT_TRUE(std::get<17>(result));
    //     }
    // #endif
}