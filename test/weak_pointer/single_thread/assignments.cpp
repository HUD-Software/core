#include <core/containers/shared_pointer.h>

GTEST_TEST(weak_pointer_not_safe, copy_assignement_same_type)
{
    const auto test = [](i32 id)
    {
        i32 dtor_count = 0;
        hud_test::non_bitwise_type *type = new hud_test::non_bitwise_type {id, &dtor_count};
        hud::shared_pointer<hud_test::non_bitwise_type> shared_ptr {type}; // +1 shared_count
        const hud::weak_pointer<hud_test::non_bitwise_type> weak_ptr_to_copy {shared_ptr};
        hud::weak_pointer<hud_test::non_bitwise_type> weak_ptr_copy;
        weak_ptr_copy = weak_ptr_to_copy;
        const auto locked_weak_ptr_to_copy = weak_ptr_to_copy.lock(); // +1 shared_count
        const auto locked_weak_ptr_copy = weak_ptr_copy.lock();       // +1 shared_count

        return std::tuple {
            locked_weak_ptr_to_copy.pointer() == type,
            locked_weak_ptr_to_copy.shared_count() == 3u,
            locked_weak_ptr_to_copy.pointer()->id() == 123,
            locked_weak_ptr_to_copy.pointer()->constructor_count() == 1u,
            locked_weak_ptr_to_copy.pointer()->copy_assign_count() == 0u,
            locked_weak_ptr_to_copy.pointer()->move_assign_count() == 0u,
            locked_weak_ptr_to_copy.pointer()->copy_constructor_count() == 0u,
            locked_weak_ptr_to_copy.pointer()->move_constructor_count() == 0u,
            locked_weak_ptr_copy.pointer() == type,
            locked_weak_ptr_copy.shared_count() == 3u,
            locked_weak_ptr_copy.pointer()->id() == 123,
            locked_weak_ptr_copy.pointer()->constructor_count() == 1u,
            locked_weak_ptr_copy.pointer()->copy_assign_count() == 0u,
            locked_weak_ptr_copy.pointer()->move_assign_count() == 0u,
            locked_weak_ptr_copy.pointer()->copy_constructor_count() == 0u,
            locked_weak_ptr_copy.pointer()->move_constructor_count() == 0u,
            dtor_count == 0};
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
    //     }
    // #endif
}

GTEST_TEST(weak_pointer_not_safe, copy_assignement_different_type)
{
    const auto test = [](i32 id)
    {
        i32 dtor_count = 0;
        hud_test::non_bitwise_type *type = new hud_test::non_bitwise_type {id, &dtor_count};
        hud::shared_pointer<hud_test::non_bitwise_type> shared_ptr {type}; // +1 shared_count
        const hud::weak_pointer<hud_test::non_bitwise_type> weak_ptr_to_copy {shared_ptr};
        hud::weak_pointer<const hud_test::non_bitwise_type> weak_ptr_copy;
        weak_ptr_copy = weak_ptr_to_copy;
        const auto locked_weak_ptr_to_copy = weak_ptr_to_copy.lock(); // +1 shared_count
        const auto locked_weak_ptr_copy = weak_ptr_copy.lock();       // +1 shared_count

        return std::tuple {
            locked_weak_ptr_to_copy.pointer() == type,
            locked_weak_ptr_to_copy.shared_count() == 3u,
            locked_weak_ptr_to_copy.pointer()->id() == 123,
            locked_weak_ptr_to_copy.pointer()->constructor_count() == 1u,
            locked_weak_ptr_to_copy.pointer()->copy_assign_count() == 0u,
            locked_weak_ptr_to_copy.pointer()->move_assign_count() == 0u,
            locked_weak_ptr_to_copy.pointer()->copy_constructor_count() == 0u,
            locked_weak_ptr_to_copy.pointer()->move_constructor_count() == 0u,
            locked_weak_ptr_copy.pointer() == type,
            locked_weak_ptr_copy.shared_count() == 3u,
            locked_weak_ptr_copy.pointer()->id() == 123,
            locked_weak_ptr_copy.pointer()->constructor_count() == 1u,
            locked_weak_ptr_copy.pointer()->copy_assign_count() == 0u,
            locked_weak_ptr_copy.pointer()->move_assign_count() == 0u,
            locked_weak_ptr_copy.pointer()->copy_constructor_count() == 0u,
            locked_weak_ptr_copy.pointer()->move_constructor_count() == 0u,
            dtor_count == 0};
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
    //     }
    // #endif
}