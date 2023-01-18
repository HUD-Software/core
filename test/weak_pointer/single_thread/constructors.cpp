#include <core/containers/shared_pointer.h>

GTEST_TEST(weak_pointer_not_safe, default_constructor)
{
    const auto test = []()
    {
        hud::weak_pointer<hud_test::non_bitwise_type> weak_ptr;

        return std::tuple {
            weak_ptr.lock().pointer() == nullptr,
            weak_ptr.lock().shared_count(),
        };
    };

    // Non constant
    {
        const auto result = test();
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
    }

    // Constant
    {
        constexpr auto result = test();
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
        GTEST_ASSERT_EQ(std::get<1>(result), 0u);
    }
}

GTEST_TEST(weak_pointer_not_safe, param_constructor_same_type)
{
    const auto test = [](i32 id)
    {
        i32 dtor_count = 0;
        hud_test::non_bitwise_type *type = new hud_test::non_bitwise_type {id, &dtor_count};
        hud::shared_pointer<hud_test::non_bitwise_type> shared_ptr {type};
        hud::weak_pointer<hud_test::non_bitwise_type> weak_ptr {shared_ptr};
        return std::tuple {
            shared_ptr.pointer() == type,
            shared_ptr.shared_count(),
            shared_ptr.pointer()->id(),
            dtor_count,
            weak_ptr.lock().pointer() == type};
    };

    // Non constant
    {
        const auto result = test(123);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 1u);
        GTEST_ASSERT_EQ(std::get<2>(result), 123);
        GTEST_ASSERT_EQ(std::get<3>(result), 0);
        GTEST_ASSERT_TRUE(std::get<4>(result));
    }

    // Constant
    // Not working under with msvc
    // https://developercommunity.visualstudio.com/t/constant-evaluation-with-do-not-works-wi/10058244
    // #if !defined(HD_COMPILER_MSVC)
    //     {
    //         constexpr auto result = test(123);
    //         GTEST_ASSERT_TRUE(std::get<0>(result));
    //         GTEST_ASSERT_EQ(std::get<1>(result), 1u);
    //         GTEST_ASSERT_EQ(std::get<2>(result), 123);
    //         GTEST_ASSERT_EQ(std::get<3>(result), 0);
    //         GTEST_ASSERT_TRUE(std::get<4>(result));
    //     }
    // #endif
}

GTEST_TEST(weak_pointer_not_safe, param_constructor_different_type)
{
    const auto test = [](i32 id)
    {
        i32 dtor_count = 0;
        hud_test::non_bitwise_type2 *type = new hud_test::non_bitwise_type2 {id, &dtor_count};
        hud::shared_pointer<hud_test::non_bitwise_type2> shared_ptr {type};
        hud::weak_pointer<hud_test::non_bitwise_type> weak_ptr {shared_ptr};
        return std::tuple {
            shared_ptr.pointer() == type,
            shared_ptr.shared_count(),
            shared_ptr.pointer()->id(),
            dtor_count,
            weak_ptr.lock().pointer() == type};
    };

    // Non constant
    {
        const auto result = test(123);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 1u);
        GTEST_ASSERT_EQ(std::get<2>(result), 123);
        GTEST_ASSERT_EQ(std::get<3>(result), 0);
    }

    // Constant
    // Not working under with msvc
    // https://developercommunity.visualstudio.com/t/constant-evaluation-with-do-not-works-wi/10058244
    // #if !defined(HD_COMPILER_MSVC)
    //     {
    //         constexpr auto result = test(123);
    //         GTEST_ASSERT_TRUE(std::get<0>(result));
    //         GTEST_ASSERT_EQ(std::get<1>(result), 1u);
    //         GTEST_ASSERT_EQ(std::get<2>(result), 123);
    //         GTEST_ASSERT_EQ(std::get<3>(result), 0);
    //         GTEST_ASSERT_TRUE(std::get<4>(result));
    //     }
    // #endif
}

GTEST_TEST(weak_pointer_not_safe, copy_constructor_same_type)
{
    const auto test = [](i32 id)
    {
        i32 dtor_count = 0;
        hud_test::non_bitwise_type *type = new hud_test::non_bitwise_type {id, &dtor_count};
        hud::shared_pointer<hud_test::non_bitwise_type> shared_ptr {type}; // +1 shared_count
        const hud::weak_pointer<hud_test::non_bitwise_type> weak_ptr_to_copy {shared_ptr};
        hud::weak_pointer<hud_test::non_bitwise_type> weak_ptr_copy {weak_ptr_to_copy};
        const auto locked_weak_ptr_to_copy = weak_ptr_to_copy.lock(); // +1 shared_count
        const auto locked_weak_ptr_copy = weak_ptr_copy.lock();       // +1 shared_count

        return std::tuple {
            locked_weak_ptr_to_copy.pointer() == type,
            locked_weak_ptr_to_copy.shared_count(),
            locked_weak_ptr_to_copy.pointer()->id(),
            locked_weak_ptr_to_copy.pointer()->constructor_count(),
            locked_weak_ptr_to_copy.pointer()->copy_assign_count(),
            locked_weak_ptr_to_copy.pointer()->move_assign_count(),
            locked_weak_ptr_to_copy.pointer()->copy_constructor_count(),
            locked_weak_ptr_to_copy.pointer()->move_constructor_count(),
            locked_weak_ptr_copy.pointer() == type,
            locked_weak_ptr_copy.shared_count(),
            locked_weak_ptr_copy.pointer()->id(),
            locked_weak_ptr_copy.pointer()->constructor_count(),
            locked_weak_ptr_copy.pointer()->copy_assign_count(),
            locked_weak_ptr_copy.pointer()->move_assign_count(),
            locked_weak_ptr_copy.pointer()->copy_constructor_count(),
            locked_weak_ptr_copy.pointer()->move_constructor_count(),
            dtor_count};
    };

    // Non constant
    {
        const auto result = test(123);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 3u);
        GTEST_ASSERT_EQ(std::get<2>(result), 123);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
        GTEST_ASSERT_EQ(std::get<5>(result), 0u);
        GTEST_ASSERT_EQ(std::get<6>(result), 0u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
        GTEST_ASSERT_TRUE(std::get<8>(result));
        GTEST_ASSERT_EQ(std::get<9>(result), 3u);
        GTEST_ASSERT_EQ(std::get<10>(result), 123);
        GTEST_ASSERT_EQ(std::get<11>(result), 1u);
        GTEST_ASSERT_EQ(std::get<12>(result), 0u);
        GTEST_ASSERT_EQ(std::get<13>(result), 0u);
        GTEST_ASSERT_EQ(std::get<14>(result), 0u);
        GTEST_ASSERT_EQ(std::get<15>(result), 0u);
        GTEST_ASSERT_EQ(std::get<16>(result), 0);
    }

    // Constant
    // Not working under with msvc
    // https://developercommunity.visualstudio.com/t/constant-evaluation-with-do-not-works-wi/10058244
    // #if !defined(HD_COMPILER_MSVC)
    //     {
    //         constexpr auto result = test(123);
    //         GTEST_ASSERT_TRUE(std::get<0>(result));
    //         GTEST_ASSERT_EQ(std::get<1>(result), 3u);
    //         GTEST_ASSERT_EQ(std::get<2>(result), 123);
    //         GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    //         GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    //         GTEST_ASSERT_EQ(std::get<5>(result), 0u);
    //         GTEST_ASSERT_EQ(std::get<6>(result), 0u);
    //         GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    //         GTEST_ASSERT_TRUE(std::get<8>(result));
    //         GTEST_ASSERT_EQ(std::get<9>(result), 3u);
    //         GTEST_ASSERT_EQ(std::get<10>(result), 123);
    //         GTEST_ASSERT_EQ(std::get<11>(result), 1u);
    //         GTEST_ASSERT_EQ(std::get<12>(result), 0u);
    //         GTEST_ASSERT_EQ(std::get<13>(result), 0u);
    //         GTEST_ASSERT_EQ(std::get<14>(result), 0u);
    //         GTEST_ASSERT_EQ(std::get<15>(result), 0u);
    //         GTEST_ASSERT_EQ(std::get<16>(result), 0);
    //     }
    // #endif
}

GTEST_TEST(weak_pointer_not_safe, copy_constructor_different_type)
{

    const auto test = [](i32 id)
    {
        i32 dtor_count = 0;

        hud_test::non_bitwise_type2 *type = new hud_test::non_bitwise_type2 {id, &dtor_count};
        const hud::shared_pointer<hud_test::non_bitwise_type2> shared_ptr {type}; // +1 shared_count
        const hud::weak_pointer<hud_test::non_bitwise_type2> weak_ptr_to_copy {shared_ptr};
        hud::weak_pointer<hud_test::non_bitwise_type> weak_ptr_copy {weak_ptr_to_copy};
        const auto locked_weak_ptr_to_copy = weak_ptr_to_copy.lock(); // +1 shared_count
        const auto locked_weak_ptr_copy = weak_ptr_copy.lock();       // +1 shared_count

        return std::tuple {
            locked_weak_ptr_to_copy.pointer() == type,
            locked_weak_ptr_to_copy.shared_count(),
            locked_weak_ptr_to_copy.pointer()->id(),
            locked_weak_ptr_to_copy.pointer()->constructor_count(),
            locked_weak_ptr_to_copy.pointer()->copy_assign_count(),
            locked_weak_ptr_to_copy.pointer()->move_assign_count(),
            locked_weak_ptr_to_copy.pointer()->copy_constructor_count(),
            locked_weak_ptr_to_copy.pointer()->move_constructor_count(),
            locked_weak_ptr_copy.pointer() == type,
            locked_weak_ptr_copy.shared_count(),
            locked_weak_ptr_copy.pointer()->id(),
            locked_weak_ptr_copy.pointer()->constructor_count(),
            locked_weak_ptr_copy.pointer()->copy_assign_count(),
            locked_weak_ptr_copy.pointer()->move_assign_count(),
            locked_weak_ptr_copy.pointer()->copy_constructor_count(),
            locked_weak_ptr_copy.pointer()->move_constructor_count(),
            dtor_count};
    };

    // Non constant
    {
        const auto result = test(123);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 3u);
        GTEST_ASSERT_EQ(std::get<2>(result), 123);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
        GTEST_ASSERT_EQ(std::get<5>(result), 0u);
        GTEST_ASSERT_EQ(std::get<6>(result), 0u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
        GTEST_ASSERT_TRUE(std::get<8>(result));
        GTEST_ASSERT_EQ(std::get<9>(result), 3u);
        GTEST_ASSERT_EQ(std::get<10>(result), 123);
        GTEST_ASSERT_EQ(std::get<11>(result), 1u);
        GTEST_ASSERT_EQ(std::get<12>(result), 0u);
        GTEST_ASSERT_EQ(std::get<13>(result), 0u);
        GTEST_ASSERT_EQ(std::get<14>(result), 0u);
        GTEST_ASSERT_EQ(std::get<15>(result), 0u);
        GTEST_ASSERT_EQ(std::get<16>(result), 0);
    }

    // Constant
    // Not working under with msvc
    // https://developercommunity.visualstudio.com/t/constant-evaluation-with-do-not-works-wi/10058244
    // #if !defined(HD_COMPILER_MSVC)
    //     {
    //         constexpr auto result = test(123);
    //         GTEST_ASSERT_TRUE(std::get<0>(result));
    //         GTEST_ASSERT_EQ(std::get<1>(result), 3u);
    //         GTEST_ASSERT_EQ(std::get<2>(result), 123);
    //         GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    //         GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    //         GTEST_ASSERT_EQ(std::get<5>(result), 0u);
    //         GTEST_ASSERT_EQ(std::get<6>(result), 0u);
    //         GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    //         GTEST_ASSERT_TRUE(std::get<8>(result));
    //         GTEST_ASSERT_EQ(std::get<9>(result), 3u);
    //         GTEST_ASSERT_EQ(std::get<10>(result), 123);
    //         GTEST_ASSERT_EQ(std::get<11>(result), 1u);
    //         GTEST_ASSERT_EQ(std::get<12>(result), 0u);
    //         GTEST_ASSERT_EQ(std::get<13>(result), 0u);
    //         GTEST_ASSERT_EQ(std::get<14>(result), 0u);
    //         GTEST_ASSERT_EQ(std::get<15>(result), 0u);
    //         GTEST_ASSERT_EQ(std::get<16>(result), 0);
    //     }
    // #endif
}

GTEST_TEST(weak_pointer_not_safe, move_constructor_same_type)
{

    const auto test = [](i32 id)
    {
        i32 dtor_count = 0;
        hud_test::non_bitwise_type *type = new hud_test::non_bitwise_type {id, &dtor_count};
        hud::shared_pointer<hud_test::non_bitwise_type> shared_ptr {type}; // +1 shared_count
        hud::weak_pointer<hud_test::non_bitwise_type> weak_ptr_to_move {shared_ptr};
        hud::weak_pointer<hud_test::non_bitwise_type> weak_ptr_move {hud::move(weak_ptr_to_move)};
        const auto locked_weak_ptr_to_move = weak_ptr_to_move.lock();
        const auto locked_weak_ptr_move = weak_ptr_move.lock(); // +1 shared_count

        return std::tuple {
            locked_weak_ptr_move.pointer() == type,
            locked_weak_ptr_move.shared_count(),
            locked_weak_ptr_move.pointer()->id(),
            locked_weak_ptr_move.pointer()->constructor_count(),
            locked_weak_ptr_move.pointer()->copy_assign_count(),
            locked_weak_ptr_move.pointer()->move_assign_count(),
            locked_weak_ptr_move.pointer()->copy_constructor_count(),
            locked_weak_ptr_move.pointer()->move_constructor_count(),
            locked_weak_ptr_to_move.pointer() == nullptr,
            locked_weak_ptr_to_move.shared_count(),
            dtor_count};
    };

    // Non constant
    {
        const auto result = test(123);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 123);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
        GTEST_ASSERT_EQ(std::get<5>(result), 0u);
        GTEST_ASSERT_EQ(std::get<6>(result), 0u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
        GTEST_ASSERT_TRUE(std::get<8>(result));
        GTEST_ASSERT_EQ(std::get<9>(result), 0u);
        GTEST_ASSERT_EQ(std::get<10>(result), 0);
    }

    // Constant
    // Not working under with msvc
    // https://developercommunity.visualstudio.com/t/constant-evaluation-with-do-not-works-wi/10058244
    // #if !defined(HD_COMPILER_MSVC)
    //     {
    //         constexpr auto result = test(123);
    //         GTEST_ASSERT_TRUE(std::get<0>(result));
    //         GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    //         GTEST_ASSERT_EQ(std::get<2>(result), 123);
    //         GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    //         GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    //         GTEST_ASSERT_EQ(std::get<5>(result), 0u);
    //         GTEST_ASSERT_EQ(std::get<6>(result), 0u);
    //         GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    //         GTEST_ASSERT_TRUE(std::get<8>(result));
    //         GTEST_ASSERT_EQ(std::get<9>(result), 0u);
    //         GTEST_ASSERT_EQ(std::get<10>(result), 0);
    //     }
    // #endif
}

GTEST_TEST(weak_pointer_not_safe, move_constructor_different_type)
{

    const auto test = [](i32 id)
    {
        i32 dtor_count = 0;
        hud_test::non_bitwise_type2 *type = new hud_test::non_bitwise_type2(id, &dtor_count);
        hud::shared_pointer<hud_test::non_bitwise_type2> shared_ptr {type}; // +1 shared_count
        hud::weak_pointer<hud_test::non_bitwise_type2> weak_ptr_to_move {shared_ptr};
        hud::weak_pointer<hud_test::non_bitwise_type> weak_ptr_move {hud::move(weak_ptr_to_move)};
        const auto locked_weak_ptr_to_move = weak_ptr_to_move.lock();
        const auto locked_weak_ptr_move = weak_ptr_move.lock(); // +1 shared_count

        return std::tuple {
            locked_weak_ptr_move.pointer() == type,
            locked_weak_ptr_move.shared_count(),
            locked_weak_ptr_move.pointer()->id(),
            locked_weak_ptr_move.pointer()->constructor_count(),
            locked_weak_ptr_move.pointer()->copy_assign_count(),
            locked_weak_ptr_move.pointer()->move_assign_count(),
            locked_weak_ptr_move.pointer()->copy_constructor_count(),
            locked_weak_ptr_move.pointer()->move_constructor_count(),
            locked_weak_ptr_to_move.pointer() == nullptr,
            locked_weak_ptr_to_move.shared_count(),
            dtor_count};
    };

    // Non constant
    {
        const auto result = test(123);
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_EQ(std::get<1>(result), 2u);
        GTEST_ASSERT_EQ(std::get<2>(result), 123);
        GTEST_ASSERT_EQ(std::get<3>(result), 1u);
        GTEST_ASSERT_EQ(std::get<4>(result), 0u);
        GTEST_ASSERT_EQ(std::get<5>(result), 0u);
        GTEST_ASSERT_EQ(std::get<6>(result), 0u);
        GTEST_ASSERT_EQ(std::get<7>(result), 0u);
        GTEST_ASSERT_TRUE(std::get<8>(result));
        GTEST_ASSERT_EQ(std::get<9>(result), 0u);
        GTEST_ASSERT_EQ(std::get<10>(result), 0);
    }

    // Constant
    // Not working under with msvc
    // https://developercommunity.visualstudio.com/t/constant-evaluation-with-do-not-works-wi/10058244
    // #if !defined(HD_COMPILER_MSVC)
    //     {
    //         constexpr auto result = test(123);
    //         GTEST_ASSERT_TRUE(std::get<0>(result));
    //         GTEST_ASSERT_EQ(std::get<1>(result), 2u);
    //         GTEST_ASSERT_EQ(std::get<2>(result), 123);
    //         GTEST_ASSERT_EQ(std::get<3>(result), 1u);
    //         GTEST_ASSERT_EQ(std::get<4>(result), 0u);
    //         GTEST_ASSERT_EQ(std::get<5>(result), 0u);
    //         GTEST_ASSERT_EQ(std::get<6>(result), 0u);
    //         GTEST_ASSERT_EQ(std::get<7>(result), 0u);
    //         GTEST_ASSERT_TRUE(std::get<8>(result));
    //         GTEST_ASSERT_EQ(std::get<9>(result), 0u);
    //         GTEST_ASSERT_EQ(std::get<10>(result), 0);
    //     }
    // #endif
}