#include <core/containers/shared_pointer.h>

TEST(shared_pointer_not_safe, default_constructor) {


    const auto test = []()
    {
        hud::shared_pointer<hud_test::non_bitwise_type> shared_ptr;
        return std::tuple{
            shared_ptr.pointer() == nullptr,
            shared_ptr.shared_count()
        };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_EQ(std::get<1>(result), 0u);
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_EQ(std::get<1>(result), 0u);
    }
}

TEST(shared_pointer_not_safe, param_constructor_nullptr) {


    const auto test = []()
    {
        hud::shared_pointer<hud_test::non_bitwise_type> shared_ptr(nullptr);
        return std::tuple{
            shared_ptr.pointer() == nullptr,
            shared_ptr.shared_count()
        };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_EQ(std::get<1>(result), 0u);
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_EQ(std::get<1>(result), 0u);
    }
}

TEST(shared_pointer_not_safe, param_constructor_same_type) {


    const auto test = [](i32 id) {
        i32 dtor_count = 0;
        hud_test::non_bitwise_type* type = new hud_test::non_bitwise_type(id, &dtor_count);
        hud::shared_pointer<hud_test::non_bitwise_type> shared_ptr(type);
        return std::tuple{
            shared_ptr.pointer() == type,
            shared_ptr.shared_count(),
            shared_ptr.pointer()->id(),
            dtor_count
        };
    };

    // Non constant
    {
        const auto result = test(123);
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_EQ(std::get<1>(result), 1u);
        ASSERT_EQ(std::get<2>(result), 123);
        ASSERT_EQ(std::get<3>(result), 0);
    }

    // Constant
    // Not working under with msvc
    // https://developercommunity.visualstudio.com/t/constant-evaluation-with-do-not-works-wi/10058244
//#if !defined(HD_COMPILER_MSVC)
//    {
//        constexpr auto result = test(123);
//        ASSERT_TRUE(std::get<0>(result));
//        ASSERT_EQ(std::get<1>(result), 1u);
//        ASSERT_EQ(std::get<2>(result), 123);
//        ASSERT_EQ(std::get<3>(result), 0);
//    }
//#endif
}

TEST(shared_pointer_not_safe, param_constructor_different_type) {


    const auto test = [](i32 id) {
        i32 dtor_count = 0;
        hud_test::non_bitwise_type2* type = new hud_test::non_bitwise_type2(id, &dtor_count);
        hud::shared_pointer<hud_test::non_bitwise_type> shared_ptr(type);
        return std::tuple{
            shared_ptr.pointer() == type,
            shared_ptr.shared_count(),
            shared_ptr.pointer()->id(),
            dtor_count
        };
    };

    // Non constant
    {
        const auto result = test(123);
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_EQ(std::get<1>(result), 1u);
        ASSERT_EQ(std::get<2>(result), 123);
        ASSERT_EQ(std::get<3>(result), 0);
    }

    // Constant
    // Not working under with msvc
    // https://developercommunity.visualstudio.com/t/constant-evaluation-with-do-not-works-wi/10058244
//#if !defined(HD_COMPILER_MSVC)
//    {
//        constexpr auto result = test(123);
//        ASSERT_TRUE(std::get<0>(result));
//        ASSERT_EQ(std::get<1>(result), 1u);
//        ASSERT_EQ(std::get<2>(result), 123);
//        ASSERT_EQ(std::get<3>(result), 0);
//    }
//#endif
}

TEST(shared_pointer_not_safe, copy_constructor_same_type) {


    const auto test = [](i32 id) {
        i32 dtor_count = 0;
        hud_test::non_bitwise_type* type = new hud_test::non_bitwise_type(id, &dtor_count);
        const hud::shared_pointer<hud_test::non_bitwise_type> shared_ptr_to_copy(type);
        hud::shared_pointer<hud_test::non_bitwise_type> shared_ptr(shared_ptr_to_copy);

        return std::tuple{
            shared_ptr_to_copy.pointer() == type,
            shared_ptr_to_copy.shared_count(),
            shared_ptr_to_copy.pointer()->id(),
            shared_ptr_to_copy.pointer()->constructor_count(),
            shared_ptr_to_copy.pointer()->copy_assign_count(),
            shared_ptr_to_copy.pointer()->move_assign_count(),
            shared_ptr_to_copy.pointer()->copy_constructor_count(),
            shared_ptr_to_copy.pointer()->move_constructor_count(),
            shared_ptr.pointer() == type,
            shared_ptr.shared_count(),
            shared_ptr.pointer()->id(),
            shared_ptr.pointer()->constructor_count(),
            shared_ptr.pointer()->copy_assign_count(),
            shared_ptr.pointer()->move_assign_count(),
            shared_ptr.pointer()->copy_constructor_count(),
            shared_ptr.pointer()->move_constructor_count(),
            dtor_count
        };
    };

    // Non constant
    {
        const auto result = test(123);
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_EQ(std::get<1>(result), 2u);
        ASSERT_EQ(std::get<2>(result), 123);
        ASSERT_EQ(std::get<3>(result), 1u);
        ASSERT_EQ(std::get<4>(result), 0u);
        ASSERT_EQ(std::get<5>(result), 0u);
        ASSERT_EQ(std::get<6>(result), 0u);
        ASSERT_EQ(std::get<7>(result), 0u);
        ASSERT_TRUE(std::get<8>(result));
        ASSERT_EQ(std::get<9>(result), 2u);
        ASSERT_EQ(std::get<10>(result), 123);
        ASSERT_EQ(std::get<11>(result), 1u);
        ASSERT_EQ(std::get<12>(result), 0u);
        ASSERT_EQ(std::get<13>(result), 0u);
        ASSERT_EQ(std::get<14>(result), 0u);
        ASSERT_EQ(std::get<15>(result), 0u);
        ASSERT_EQ(std::get<16>(result), 0);
    }

    // Constant
    // Not working under with msvc
    // https://developercommunity.visualstudio.com/t/constant-evaluation-with-do-not-works-wi/10058244
//#if !defined(HD_COMPILER_MSVC)
//    {
//        constexpr auto result = test(123);
//        ASSERT_TRUE(std::get<0>(result));
//        ASSERT_EQ(std::get<1>(result), 2u);
//        ASSERT_EQ(std::get<2>(result), 123);
//        ASSERT_EQ(std::get<3>(result), 1u);
//        ASSERT_EQ(std::get<4>(result), 0u);
//        ASSERT_EQ(std::get<5>(result), 0u);
//        ASSERT_EQ(std::get<6>(result), 0u);
//        ASSERT_EQ(std::get<7>(result), 0u);
//        ASSERT_TRUE(std::get<8>(result));
//        ASSERT_EQ(std::get<9>(result), 2u);
//        ASSERT_EQ(std::get<10>(result), 123);
//        ASSERT_EQ(std::get<11>(result), 1u);
//        ASSERT_EQ(std::get<12>(result), 0u);
//        ASSERT_EQ(std::get<13>(result), 0u);
//        ASSERT_EQ(std::get<14>(result), 0u);
//        ASSERT_EQ(std::get<15>(result), 0u);
//        ASSERT_EQ(std::get<16>(result), 0);
//    }
//#endif
}

TEST(shared_pointer_not_safe, copy_constructor_different_type) {


    const auto test = [](i32 id) {
        i32 dtor_count = 0;
       
        hud_test::non_bitwise_type2* type = new hud_test::non_bitwise_type2(id, &dtor_count);
        const hud::shared_pointer<hud_test::non_bitwise_type2> shared_ptr_to_copy(type);
        hud::shared_pointer<hud_test::non_bitwise_type> shared_ptr(shared_ptr_to_copy);

        return std::tuple{
            shared_ptr_to_copy.pointer() == type,
            shared_ptr_to_copy.shared_count(),
            shared_ptr_to_copy.pointer()->id(),
            shared_ptr_to_copy.pointer()->constructor_count(),
            shared_ptr_to_copy.pointer()->copy_assign_count(),
            shared_ptr_to_copy.pointer()->move_assign_count(),
            shared_ptr_to_copy.pointer()->copy_constructor_count(),
            shared_ptr_to_copy.pointer()->move_constructor_count(),
            shared_ptr.pointer() == type,
            shared_ptr.shared_count(),
            shared_ptr.pointer()->id(),
            shared_ptr.pointer()->constructor_count(),
            shared_ptr.pointer()->copy_assign_count(),
            shared_ptr.pointer()->move_assign_count(),
            shared_ptr.pointer()->copy_constructor_count(),
            shared_ptr.pointer()->move_constructor_count(),
            dtor_count
        };
    };

    // Non constant
    {
        const auto result = test(123);
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_EQ(std::get<1>(result), 2u);
        ASSERT_EQ(std::get<2>(result), 123);
        ASSERT_EQ(std::get<3>(result), 1u);
        ASSERT_EQ(std::get<4>(result), 0u);
        ASSERT_EQ(std::get<5>(result), 0u);
        ASSERT_EQ(std::get<6>(result), 0u);
        ASSERT_EQ(std::get<7>(result), 0u);
        ASSERT_TRUE(std::get<8>(result));
        ASSERT_EQ(std::get<9>(result), 2u);
        ASSERT_EQ(std::get<10>(result), 123);
        ASSERT_EQ(std::get<11>(result), 1u);
        ASSERT_EQ(std::get<12>(result), 0u);
        ASSERT_EQ(std::get<13>(result), 0u);
        ASSERT_EQ(std::get<14>(result), 0u);
        ASSERT_EQ(std::get<15>(result), 0u);
        ASSERT_EQ(std::get<16>(result), 0);
    }

    // Constant
    // Not working under with msvc
    // https://developercommunity.visualstudio.com/t/constant-evaluation-with-do-not-works-wi/10058244
//#if !defined(HD_COMPILER_MSVC)
//    {
//        constexpr auto result = test(123);
//        ASSERT_TRUE(std::get<0>(result));
//        ASSERT_EQ(std::get<1>(result), 2u);
//        ASSERT_EQ(std::get<2>(result), 123);
//        ASSERT_EQ(std::get<3>(result), 1u);
//        ASSERT_EQ(std::get<4>(result), 0u);
//        ASSERT_EQ(std::get<5>(result), 0u);
//        ASSERT_EQ(std::get<6>(result), 0u);
//        ASSERT_EQ(std::get<7>(result), 0u);
//        ASSERT_TRUE(std::get<8>(result));
//        ASSERT_EQ(std::get<9>(result), 2u);
//        ASSERT_EQ(std::get<10>(result), 123);
//        ASSERT_EQ(std::get<11>(result), 1u);
//        ASSERT_EQ(std::get<12>(result), 0u);
//        ASSERT_EQ(std::get<13>(result), 0u);
//        ASSERT_EQ(std::get<14>(result), 0u);
//        ASSERT_EQ(std::get<15>(result), 0u);
//        ASSERT_EQ(std::get<16>(result), 0);
//    }
//#endif
}

TEST(shared_pointer_not_safe, move_constructor_same_type) {


    const auto test = [](i32 id) {
        i32 dtor_count = 0;
        hud_test::non_bitwise_type* type = new hud_test::non_bitwise_type(id, &dtor_count);
        hud::shared_pointer<hud_test::non_bitwise_type> shared_ptr_to_move(type);
        hud::shared_pointer<hud_test::non_bitwise_type> shared_ptr(hud::move(shared_ptr_to_move));

        return std::tuple{
            shared_ptr.pointer() == type,
            shared_ptr.shared_count(),
            shared_ptr.pointer()->id(),
            shared_ptr.pointer()->constructor_count(),
            shared_ptr.pointer()->copy_assign_count(),
            shared_ptr.pointer()->move_assign_count(),
            shared_ptr.pointer()->copy_constructor_count(),
            shared_ptr.pointer()->move_constructor_count(),
            shared_ptr_to_move.pointer() == nullptr,
            shared_ptr_to_move.shared_count(),
            dtor_count
        };
    };

    // Non constant
    {
        const auto result = test(123);
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_EQ(std::get<1>(result), 1u);
        ASSERT_EQ(std::get<2>(result), 123);
        ASSERT_EQ(std::get<3>(result), 1u);
        ASSERT_EQ(std::get<4>(result), 0u);
        ASSERT_EQ(std::get<5>(result), 0u);
        ASSERT_EQ(std::get<6>(result), 0u);
        ASSERT_EQ(std::get<7>(result), 0u);
        ASSERT_TRUE(std::get<8>(result));
        ASSERT_EQ(std::get<9>(result), 0u);
        ASSERT_EQ(std::get<10>(result), 0);
    }

    // Constant
    // Not working under with msvc
    // https://developercommunity.visualstudio.com/t/constant-evaluation-with-do-not-works-wi/10058244
//#if !defined(HD_COMPILER_MSVC)
//    {
//        constexpr auto result = test(123);
//        ASSERT_TRUE(std::get<0>(result));
//        ASSERT_EQ(std::get<1>(result), 1u);
//        ASSERT_EQ(std::get<2>(result), 123);
//        ASSERT_EQ(std::get<3>(result), 1u);
//        ASSERT_EQ(std::get<4>(result), 0u);
//        ASSERT_EQ(std::get<5>(result), 0u);
//        ASSERT_EQ(std::get<6>(result), 0u);
//        ASSERT_EQ(std::get<7>(result), 0u);
//        ASSERT_TRUE(std::get<8>(result));
//        ASSERT_EQ(std::get<9>(result), 0u);
//        ASSERT_EQ(std::get<10>(result), 0);
//    }
//#endif
}

TEST(shared_pointer_not_safe, move_constructor_different_type) {


    const auto test = [](i32 id) {
        i32 dtor_count = 0;
        hud_test::non_bitwise_type2* type = new hud_test::non_bitwise_type2(id, &dtor_count);
        hud::shared_pointer<hud_test::non_bitwise_type2> shared_ptr_to_move(type);
        hud::shared_pointer<hud_test::non_bitwise_type> shared_ptr(hud::move(shared_ptr_to_move));

        return std::tuple{
            shared_ptr.pointer() == type,
            shared_ptr.shared_count(),
            shared_ptr.pointer()->id(),
            shared_ptr.pointer()->constructor_count(),
            shared_ptr.pointer()->copy_assign_count(),
            shared_ptr.pointer()->move_assign_count(),
            shared_ptr.pointer()->copy_constructor_count(),
            shared_ptr.pointer()->move_constructor_count(),
            shared_ptr_to_move.pointer() == nullptr,
            shared_ptr_to_move.shared_count(),
            dtor_count
        };
    };

    // Non constant
    {
        const auto result = test(123);
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_EQ(std::get<1>(result), 1u);
        ASSERT_EQ(std::get<2>(result), 123);
        ASSERT_EQ(std::get<3>(result), 1u);
        ASSERT_EQ(std::get<4>(result), 0u);
        ASSERT_EQ(std::get<5>(result), 0u);
        ASSERT_EQ(std::get<6>(result), 0u);
        ASSERT_EQ(std::get<7>(result), 0u);
        ASSERT_TRUE(std::get<8>(result));
        ASSERT_EQ(std::get<9>(result), 0u);
        ASSERT_EQ(std::get<10>(result), 0);
    }

    // Constant
    // Not working under with msvc
    // https://developercommunity.visualstudio.com/t/constant-evaluation-with-do-not-works-wi/10058244
//#if !defined(HD_COMPILER_MSVC)
//    {
//        constexpr auto result = test(123);
//        ASSERT_TRUE(std::get<0>(result));
//        ASSERT_EQ(std::get<1>(result), 1u);
//        ASSERT_EQ(std::get<2>(result), 123);
//        ASSERT_EQ(std::get<3>(result), 1u);
//        ASSERT_EQ(std::get<4>(result), 0u);
//        ASSERT_EQ(std::get<5>(result), 0u);
//        ASSERT_EQ(std::get<6>(result), 0u);
//        ASSERT_EQ(std::get<7>(result), 0u);
//        ASSERT_TRUE(std::get<8>(result));
//        ASSERT_EQ(std::get<9>(result), 0u);
//        ASSERT_EQ(std::get<10>(result), 0);
//    }
//#endif
}
