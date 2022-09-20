#include <core/containers/shared_pointer.h>

TEST(SharedPointer_safe, default_constructor) {


    const auto test = []()
    {
        SharedPointer<hud::test::NonBitwiseType, EThreadSafety::safe> shared_ptr;
        return std::tuple{
            shared_ptr.pointer() == nullptr,
            shared_ptr.shared_count()
        };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_EQ(hud::get<1>(result), 0u);
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_EQ(hud::get<1>(result), 0u);
    }
}

TEST(SharedPointer_safe, param_constructor_nullptr) {


    const auto test = []()
    {
        SharedPointer<hud::test::NonBitwiseType, EThreadSafety::safe> shared_ptr(nullptr);
        return std::tuple{
            shared_ptr.pointer() == nullptr,
            shared_ptr.shared_count()
        };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_EQ(hud::get<1>(result), 0u);
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_EQ(hud::get<1>(result), 0u);
    }
}

TEST(SharedPointer_safe, param_constructor_same_type) {


    const auto test = [](i32 id) {
        i32 dtor_count = 0;
        hud::test::NonBitwiseType* type = new hud::test::NonBitwiseType(id, &dtor_count);
        SharedPointer<hud::test::NonBitwiseType, EThreadSafety::safe> shared_ptr(type);
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
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_EQ(hud::get<1>(result), 1u);
        ASSERT_EQ(hud::get<2>(result), 123);
        ASSERT_EQ(hud::get<3>(result), 0);
    }

    // Constant is not available with thread safe SharedPointer
    // Not working under with msvc
    // https://developercommunity.visualstudio.com/t/constant-evaluation-with-do-not-works-wi/10058244
    /*#if !defined(HD_COMPILER_MSVC)
        {
            constexpr auto result = test(123);
            ASSERT_TRUE(hud::get<0>(result));
            ASSERT_EQ(hud::get<1>(result), 1u);
            ASSERT_EQ(hud::get<2>(result), 123);
            ASSERT_EQ(hud::get<3>(result), 0);
        }
    #endif*/
}

TEST(SharedPointer_safe, param_constructor_different_type) {


    const auto test = [](i32 id) {
        i32 dtor_count = 0;
        hud::test::NonBitwiseType2* type = new hud::test::NonBitwiseType2(id, &dtor_count);
        SharedPointer<hud::test::NonBitwiseType, EThreadSafety::safe> shared_ptr(type);
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
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_EQ(hud::get<1>(result), 1u);
        ASSERT_EQ(hud::get<2>(result), 123);
        ASSERT_EQ(hud::get<3>(result), 0);
    }

    // Constant is not available with thread safe SharedPointer
    // Not working under with msvc
    // https://developercommunity.visualstudio.com/t/constant-evaluation-with-do-not-works-wi/10058244
   /* #if !defined(HD_COMPILER_MSVC)
        {
            constexpr auto result = test(123);
            ASSERT_TRUE(hud::get<0>(result));
            ASSERT_EQ(hud::get<1>(result), 1u);
            ASSERT_EQ(hud::get<2>(result), 123);
            ASSERT_EQ(hud::get<3>(result), 0);
        }
    #endif*/
}

TEST(SharedPointer_safe, copy_constructor_same_type) {


    const auto test = [](i32 id) {
        i32 dtor_count = 0;
        hud::test::NonBitwiseType* type = new hud::test::NonBitwiseType(id, &dtor_count);
        const SharedPointer<hud::test::NonBitwiseType, EThreadSafety::safe> shared_ptr_to_copy(type);
        SharedPointer<hud::test::NonBitwiseType, EThreadSafety::safe> shared_ptr(shared_ptr_to_copy);

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
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_EQ(hud::get<1>(result), 2u);
        ASSERT_EQ(hud::get<2>(result), 123);
        ASSERT_EQ(hud::get<3>(result), 1u);
        ASSERT_EQ(hud::get<4>(result), 0u);
        ASSERT_EQ(hud::get<5>(result), 0u);
        ASSERT_EQ(hud::get<6>(result), 0u);
        ASSERT_EQ(hud::get<7>(result), 0u);
        ASSERT_TRUE(hud::get<8>(result));
        ASSERT_EQ(hud::get<9>(result), 2u);
        ASSERT_EQ(hud::get<10>(result), 123);
        ASSERT_EQ(hud::get<11>(result), 1u);
        ASSERT_EQ(hud::get<12>(result), 0u);
        ASSERT_EQ(hud::get<13>(result), 0u);
        ASSERT_EQ(hud::get<14>(result), 0u);
        ASSERT_EQ(hud::get<15>(result), 0u);
        ASSERT_EQ(hud::get<16>(result), 0);
    }

    // Constant is not available with thread safe SharedPointer
    // Not working under with msvc
    // https://developercommunity.visualstudio.com/t/constant-evaluation-with-do-not-works-wi/10058244
    /*#if !defined(HD_COMPILER_MSVC)
        {
            constexpr auto result = test(123);
            ASSERT_TRUE(hud::get<0>(result));
            ASSERT_EQ(hud::get<1>(result), 2u);
            ASSERT_EQ(hud::get<2>(result), 123);
            ASSERT_EQ(hud::get<3>(result), 1u);
            ASSERT_EQ(hud::get<4>(result), 0u);
            ASSERT_EQ(hud::get<5>(result), 0u);
            ASSERT_EQ(hud::get<6>(result), 0u);
            ASSERT_EQ(hud::get<7>(result), 0u);
            ASSERT_TRUE(hud::get<8>(result));
            ASSERT_EQ(hud::get<9>(result), 2u);
            ASSERT_EQ(hud::get<10>(result), 123);
            ASSERT_EQ(hud::get<11>(result), 1u);
            ASSERT_EQ(hud::get<12>(result), 0u);
            ASSERT_EQ(hud::get<13>(result), 0u);
            ASSERT_EQ(hud::get<14>(result), 0u);
            ASSERT_EQ(hud::get<15>(result), 0u);
            ASSERT_EQ(hud::get<16>(result), 0);
        }
    #endif*/
}

TEST(SharedPointer_safe, copy_constructor_different_type) {


    const auto test = [](i32 id) {
        i32 dtor_count = 0;

        hud::test::NonBitwiseType2* type = new hud::test::NonBitwiseType2(id, &dtor_count);
        const SharedPointer<hud::test::NonBitwiseType2, EThreadSafety::safe> shared_ptr_to_copy(type);
        SharedPointer<hud::test::NonBitwiseType, EThreadSafety::safe> shared_ptr(shared_ptr_to_copy);

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
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_EQ(hud::get<1>(result), 2u);
        ASSERT_EQ(hud::get<2>(result), 123);
        ASSERT_EQ(hud::get<3>(result), 1u);
        ASSERT_EQ(hud::get<4>(result), 0u);
        ASSERT_EQ(hud::get<5>(result), 0u);
        ASSERT_EQ(hud::get<6>(result), 0u);
        ASSERT_EQ(hud::get<7>(result), 0u);
        ASSERT_TRUE(hud::get<8>(result));
        ASSERT_EQ(hud::get<9>(result), 2u);
        ASSERT_EQ(hud::get<10>(result), 123);
        ASSERT_EQ(hud::get<11>(result), 1u);
        ASSERT_EQ(hud::get<12>(result), 0u);
        ASSERT_EQ(hud::get<13>(result), 0u);
        ASSERT_EQ(hud::get<14>(result), 0u);
        ASSERT_EQ(hud::get<15>(result), 0u);
        ASSERT_EQ(hud::get<16>(result), 0);
    }

    // Constant is not available with thread safe SharedPointer
    // Not working under with msvc
    // https://developercommunity.visualstudio.com/t/constant-evaluation-with-do-not-works-wi/10058244
    /*#if !defined(HD_COMPILER_MSVC)
        {
            constexpr auto result = test(123);
            ASSERT_TRUE(hud::get<0>(result));
            ASSERT_EQ(hud::get<1>(result), 2u);
            ASSERT_EQ(hud::get<2>(result), 123);
            ASSERT_EQ(hud::get<3>(result), 1u);
            ASSERT_EQ(hud::get<4>(result), 0u);
            ASSERT_EQ(hud::get<5>(result), 0u);
            ASSERT_EQ(hud::get<6>(result), 0u);
            ASSERT_EQ(hud::get<7>(result), 0u);
            ASSERT_TRUE(hud::get<8>(result));
            ASSERT_EQ(hud::get<9>(result), 2u);
            ASSERT_EQ(hud::get<10>(result), 123);
            ASSERT_EQ(hud::get<11>(result), 1u);
            ASSERT_EQ(hud::get<12>(result), 0u);
            ASSERT_EQ(hud::get<13>(result), 0u);
            ASSERT_EQ(hud::get<14>(result), 0u);
            ASSERT_EQ(hud::get<15>(result), 0u);
            ASSERT_EQ(hud::get<16>(result), 0);
        }
    #endif*/
}

TEST(SharedPointer_safe, move_constructor_same_type) {


    const auto test = [](i32 id) {
        i32 dtor_count = 0;
        hud::test::NonBitwiseType* type = new hud::test::NonBitwiseType(id, &dtor_count);
        SharedPointer<hud::test::NonBitwiseType, EThreadSafety::safe> shared_ptr_to_move(type);
        SharedPointer<hud::test::NonBitwiseType, EThreadSafety::safe> shared_ptr(hud::move(shared_ptr_to_move));

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
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_EQ(hud::get<1>(result), 1u);
        ASSERT_EQ(hud::get<2>(result), 123);
        ASSERT_EQ(hud::get<3>(result), 1u);
        ASSERT_EQ(hud::get<4>(result), 0u);
        ASSERT_EQ(hud::get<5>(result), 0u);
        ASSERT_EQ(hud::get<6>(result), 0u);
        ASSERT_EQ(hud::get<7>(result), 0u);
        ASSERT_TRUE(hud::get<8>(result));
        ASSERT_EQ(hud::get<9>(result), 0u);
        ASSERT_EQ(hud::get<10>(result), 0);
    }

    // Constant is not available with thread safe SharedPointer
    // Not working under with msvc
    // https://developercommunity.visualstudio.com/t/constant-evaluation-with-do-not-works-wi/10058244
   /* #if !defined(HD_COMPILER_MSVC)
        {
            constexpr auto result = test(123);
            ASSERT_TRUE(hud::get<0>(result));
            ASSERT_EQ(hud::get<1>(result), 1u);
            ASSERT_EQ(hud::get<2>(result), 123);
            ASSERT_EQ(hud::get<3>(result), 1u);
            ASSERT_EQ(hud::get<4>(result), 0u);
            ASSERT_EQ(hud::get<5>(result), 0u);
            ASSERT_EQ(hud::get<6>(result), 0u);
            ASSERT_EQ(hud::get<7>(result), 0u);
            ASSERT_TRUE(hud::get<8>(result));
            ASSERT_EQ(hud::get<9>(result), 0u);
            ASSERT_EQ(hud::get<10>(result), 0);
        }
    #endif*/
}

TEST(SharedPointer_safe, move_constructor_different_type) {


    const auto test = [](i32 id) {
        i32 dtor_count = 0;
        hud::test::NonBitwiseType2* type = new hud::test::NonBitwiseType2(id, &dtor_count);
        SharedPointer<hud::test::NonBitwiseType2, EThreadSafety::safe> shared_ptr_to_move(type);
        SharedPointer<hud::test::NonBitwiseType, EThreadSafety::safe> shared_ptr(hud::move(shared_ptr_to_move));

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
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_EQ(hud::get<1>(result), 1u);
        ASSERT_EQ(hud::get<2>(result), 123);
        ASSERT_EQ(hud::get<3>(result), 1u);
        ASSERT_EQ(hud::get<4>(result), 0u);
        ASSERT_EQ(hud::get<5>(result), 0u);
        ASSERT_EQ(hud::get<6>(result), 0u);
        ASSERT_EQ(hud::get<7>(result), 0u);
        ASSERT_TRUE(hud::get<8>(result));
        ASSERT_EQ(hud::get<9>(result), 0u);
        ASSERT_EQ(hud::get<10>(result), 0);
    }

    // Constant is not available with thread safe SharedPointer
    // Not working under with msvc
    // https://developercommunity.visualstudio.com/t/constant-evaluation-with-do-not-works-wi/10058244
    /*#if !defined(HD_COMPILER_MSVC)
        {
            constexpr auto result = test(123);
            ASSERT_TRUE(hud::get<0>(result));
            ASSERT_EQ(hud::get<1>(result), 1u);
            ASSERT_EQ(hud::get<2>(result), 123);
            ASSERT_EQ(hud::get<3>(result), 1u);
            ASSERT_EQ(hud::get<4>(result), 0u);
            ASSERT_EQ(hud::get<5>(result), 0u);
            ASSERT_EQ(hud::get<6>(result), 0u);
            ASSERT_EQ(hud::get<7>(result), 0u);
            ASSERT_TRUE(hud::get<8>(result));
            ASSERT_EQ(hud::get<9>(result), 0u);
            ASSERT_EQ(hud::get<10>(result), 0);
        }
    #endif*/
}
