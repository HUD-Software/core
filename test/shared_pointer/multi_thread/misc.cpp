#include <core/containers/shared_pointer.h>

GTEST_TEST(shared_pointer_safe, single_thread_pointer_return_object) {


    const auto test = []() {
        auto ptr = new hud_test::non_bitwise_type(15, nullptr);
        hud::shared_pointer<hud_test::non_bitwise_type, hud::thread_safety_e::safe> shared_ptr(ptr);
        return std::tuple{
            shared_ptr.pointer() == ptr
        };
    };

    // Non constant
    {
        const auto result = test();
        GTEST_ASSERT_TRUE(std::get<0>(result));
    }

    // Constant is not available with thread safe SharedPointer
    // Not working under with msvc
    // https://developercommunity.visualstudio.com/t/constant-evaluation-with-do-not-works-wi/10058244
    /*#if !defined(HD_COMPILER_MSVC)
        {
            constexpr auto result = test();
            GTEST_ASSERT_TRUE(std::get<0>(result));
        }
    #endif*/
}

GTEST_TEST(shared_pointer_safe, single_thread_empty_pointer_return_nullptr) {


    const auto test = []() {
        hud::shared_pointer<hud_test::non_bitwise_type, hud::thread_safety_e::safe> shared_ptr;
        return std::tuple{
            shared_ptr.pointer() == nullptr
        };
    };

    // Non constant
    {
        const auto result = test();
        GTEST_ASSERT_TRUE(std::get<0>(result));
    }

    // Constant
    // Not working under with msvc
    // https://developercommunity.visualstudio.com/t/constant-evaluation-with-do-not-works-wi/10058244
/*#if !defined(HD_COMPILER_MSVC)
    {
        constexpr auto result = test();
        GTEST_ASSERT_TRUE(std::get<0>(result));
    }
#endif*/
}

GTEST_TEST(shared_pointer_safe, single_thread_is_owning) {


    const auto test = []() {
        hud::shared_pointer<hud_test::non_bitwise_type, hud::thread_safety_e::safe> shared_ptr(new hud_test::non_bitwise_type(15, nullptr));
        hud::shared_pointer<hud_test::non_bitwise_type, hud::thread_safety_e::safe> empty_shared_ptr;
        return std::tuple{
            shared_ptr.is_owning(),
            empty_shared_ptr.is_owning()
        };
    };

    // Non constant
    {
        const auto result = test();
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_FALSE(std::get<1>(result));
    }

    // Constant is not available with thread safe SharedPointer
    // Not working under with msvc
    // https://developercommunity.visualstudio.com/t/constant-evaluation-with-do-not-works-wi/10058244
    /*#if !defined(HD_COMPILER_MSVC)
        {
            constexpr auto result = test();
            GTEST_ASSERT_TRUE(std::get<0>(result));
            GTEST_ASSERT_FALSE(std::get<1>(result));
        }
    #endif*/
}

GTEST_TEST(shared_pointer_safe, cast_to_bool) {


    const auto test = []() {
        hud::shared_pointer<hud_test::non_bitwise_type, hud::thread_safety_e::safe> shared_ptr(new hud_test::non_bitwise_type(15, nullptr));
        hud::shared_pointer<hud_test::non_bitwise_type, hud::thread_safety_e::safe> empty_shared_ptr;
        return std::tuple{
            static_cast<bool>(shared_ptr),
            static_cast<bool>(empty_shared_ptr)
        };
    };

    // Non constant
    {
        const auto result = test();
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_FALSE(std::get<1>(result));
    }

    // Constant is not available with thread safe SharedPointer
    // Not working under with msvc
    // https://developercommunity.visualstudio.com/t/constant-evaluation-with-do-not-works-wi/10058244
    /*#if !defined(HD_COMPILER_MSVC)
        {
            constexpr auto result = test();
            GTEST_ASSERT_TRUE(std::get<0>(result));
            GTEST_ASSERT_FALSE(std::get<1>(result));
        }
    #endif*/
}

GTEST_TEST(shared_pointer_safe, operator_dereference) {


    const auto test = []() {
        auto ptr = new hud_test::non_bitwise_type(15, nullptr);
        hud::shared_pointer<hud_test::non_bitwise_type, hud::thread_safety_e::safe> shared_ptr(ptr);
        return std::tuple{
            &(*shared_ptr) == ptr
        };
    };

    // Non constant
    {
        const auto result = test();
        GTEST_ASSERT_TRUE(std::get<0>(result));
    }

    // Constant is not available with thread safe SharedPointer
    // Not working under with msvc
    // https://developercommunity.visualstudio.com/t/constant-evaluation-with-do-not-works-wi/10058244
    /*#if !defined(HD_COMPILER_MSVC)
        {
            constexpr auto result = test();
            GTEST_ASSERT_TRUE(std::get<0>(result));
        }
    #endif*/
}

GTEST_TEST(shared_pointer_safe, operator_arrow) {


    const auto test = []() {
        auto ptr = new hud_test::non_bitwise_type(15, nullptr);
        hud::shared_pointer<hud_test::non_bitwise_type, hud::thread_safety_e::safe> shared_ptr(ptr);
        return std::tuple{
            shared_ptr.operator->() == ptr
        };
    };

    // Non constant
    {
        const auto result = test();
        GTEST_ASSERT_TRUE(std::get<0>(result));
    }

    // Constant is not available with thread safe SharedPointer
    // Not working under with msvc
    // https://developercommunity.visualstudio.com/t/constant-evaluation-with-do-not-works-wi/10058244
   /* #if !defined(HD_COMPILER_MSVC)
        {
            constexpr auto result = test();
            GTEST_ASSERT_TRUE(std::get<0>(result));
        }
    #endif*/
}

GTEST_TEST(shared_pointer_safe, reset_to_empty) {


    const auto test = []() {
        auto ptr = new hud_test::non_bitwise_type(15, nullptr);
        hud::shared_pointer<hud_test::non_bitwise_type, hud::thread_safety_e::safe> shared_ptr(ptr);
        const bool is_pointer_correct = shared_ptr.pointer() == ptr;
        hud::shared_pointer<hud_test::non_bitwise_type, hud::thread_safety_e::safe> empty_shared_ptr;
        const bool is_pointer_empty_correct = empty_shared_ptr.pointer() == nullptr;

        shared_ptr.reset();
        empty_shared_ptr.reset();

        return std::tuple{
            is_pointer_correct,
            shared_ptr.pointer() == nullptr,
            is_pointer_empty_correct,
            empty_shared_ptr.pointer() == nullptr
        };
    };

    // Non constant
    {
        const auto result = test();
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_TRUE(std::get<2>(result));
        GTEST_ASSERT_TRUE(std::get<3>(result));
    }

    // Constant is not available with thread safe SharedPointer
    // Not working under with msvc
    // https://developercommunity.visualstudio.com/t/constant-evaluation-with-do-not-works-wi/10058244
/*#if !defined(HD_COMPILER_MSVC)
    {
        constexpr auto result = test();
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_TRUE(std::get<2>(result));
        GTEST_ASSERT_TRUE(std::get<3>(result));
    }
#endif*/
}

GTEST_TEST(shared_pointer_safe, reset_to_nullptr) {


    const auto test = []() {
        auto ptr = new hud_test::non_bitwise_type(15, nullptr);
        hud::shared_pointer<hud_test::non_bitwise_type, hud::thread_safety_e::safe> shared_ptr(ptr);
        const bool is_pointer_correct = shared_ptr.pointer() == ptr;
        hud::shared_pointer<hud_test::non_bitwise_type, hud::thread_safety_e::safe> empty_shared_ptr;
        const bool is_pointer_empty_correct = empty_shared_ptr.pointer() == nullptr;

        shared_ptr.reset(nullptr);
        empty_shared_ptr.reset(nullptr);

        return std::tuple{
            is_pointer_correct,
            shared_ptr.pointer() == nullptr,
            is_pointer_empty_correct,
            empty_shared_ptr.pointer() == nullptr
        };
    };

    // Non constant
    {
        const auto result = test();
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_TRUE(std::get<2>(result));
        GTEST_ASSERT_TRUE(std::get<3>(result));
    }

    // Constant is not available with thread safe SharedPointer
    // Not working under with msvc
    // https://developercommunity.visualstudio.com/t/constant-evaluation-with-do-not-works-wi/10058244
/*#if !defined(HD_COMPILER_MSVC)
    {
        constexpr auto result = test();
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_TRUE(std::get<2>(result));
        GTEST_ASSERT_TRUE(std::get<3>(result));
    }
#endif*/
}

GTEST_TEST(shared_pointer_safe, reset_to_same_type) {


    const auto test = []() {
        i32 dtor_count = 0;
        i32 dtor_count_1 = 0;
        i32 dtor_count_2 = 0;

        auto ptr = new hud_test::non_bitwise_type(15, &dtor_count);
        hud::shared_pointer<hud_test::non_bitwise_type, hud::thread_safety_e::safe> shared_ptr(ptr);
        const bool is_pointer_correct = shared_ptr.pointer() == ptr;

        hud::shared_pointer<hud_test::non_bitwise_type, hud::thread_safety_e::safe> empty_shared_ptr;
        const bool is_pointer_empty_correct = empty_shared_ptr.pointer() == nullptr;

        auto reset_1 = new hud_test::non_bitwise_type(15, &dtor_count_1);
        shared_ptr.reset(reset_1);

        auto reset_2 = new hud_test::non_bitwise_type(15, &dtor_count_2);
        empty_shared_ptr.reset(reset_2);

        return std::tuple{
            dtor_count == 1,
            is_pointer_correct,
            shared_ptr.pointer() == reset_1,
            shared_ptr.shared_count() == 1u,
            dtor_count_1 == 0,
            is_pointer_empty_correct,
            empty_shared_ptr.pointer() == reset_2,
            empty_shared_ptr.shared_count() == 1u,
            dtor_count_2 == 0,
        };
    };

    // Non constant
    {
        const auto result = test();
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_TRUE(std::get<2>(result));
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_TRUE(std::get<6>(result));
        GTEST_ASSERT_TRUE(std::get<7>(result));
        GTEST_ASSERT_TRUE(std::get<8>(result));
    }

    // Constant is not available with thread safe SharedPointer
    // Not working under with msvc
    // https://developercommunity.visualstudio.com/t/constant-evaluation-with-do-not-works-wi/10058244
/*#if !defined(HD_COMPILER_MSVC)
    {
        constexpr auto result = test();
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_TRUE(std::get<2>(result));
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_TRUE(std::get<6>(result));
        GTEST_ASSERT_TRUE(std::get<7>(result));
        GTEST_ASSERT_TRUE(std::get<8>(result));
    }
#endif*/
}

GTEST_TEST(shared_pointer_safe, reset_to_different_type) {


    const auto test = []() {
        i32 dtor_count = 0;
        i32 dtor_count_1 = 0;
        i32 dtor_count_2 = 0;

        auto ptr = new hud_test::non_bitwise_type(15, &dtor_count);
        hud::shared_pointer<const hud_test::non_bitwise_type, hud::thread_safety_e::safe> shared_ptr(ptr);
        const bool is_pointer_correct = shared_ptr.pointer() == ptr;

        hud::shared_pointer<const hud_test::non_bitwise_type, hud::thread_safety_e::safe> empty_shared_ptr;
        const bool is_pointer_empty_correct = empty_shared_ptr.pointer() == nullptr;

        auto reset_1 = new hud_test::non_bitwise_type(15, &dtor_count_1);
        shared_ptr.reset(reset_1);


        auto reset_2 = new hud_test::non_bitwise_type(15, &dtor_count_2);
        empty_shared_ptr.reset(reset_2);

        return std::tuple{
            dtor_count == 1,
            is_pointer_correct,
            shared_ptr.pointer() == reset_1,
            shared_ptr.shared_count() == 1u,
            dtor_count_1 == 0,
            is_pointer_empty_correct,
            empty_shared_ptr.pointer() == reset_2,
            empty_shared_ptr.shared_count() == 1u,
            dtor_count_2 == 0,
        };
    };

    // Non constant
    {
        const auto result = test();
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_TRUE(std::get<2>(result));
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_TRUE(std::get<6>(result));
        GTEST_ASSERT_TRUE(std::get<7>(result));
        GTEST_ASSERT_TRUE(std::get<8>(result));
    }

    // Constant is not available with thread safe SharedPointer
    // Not working under with msvc
    // https://developercommunity.visualstudio.com/t/constant-evaluation-with-do-not-works-wi/10058244
/*#if !defined(HD_COMPILER_MSVC)
    {
        constexpr auto result = test();
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_TRUE(std::get<2>(result));
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_TRUE(std::get<6>(result));
        GTEST_ASSERT_TRUE(std::get<7>(result));
        GTEST_ASSERT_TRUE(std::get<8>(result));
    }
#endif*/
}

GTEST_TEST(shared_pointer_safe, reset_to_empty_should_not_delete_if_other_reference_exists) {


    const auto test = []() {
        i32 dtor_count = 0;
        auto ptr = new hud_test::non_bitwise_type(15, &dtor_count);
        hud::shared_pointer<hud_test::non_bitwise_type, hud::thread_safety_e::safe> shared_ptr(ptr);
        const bool is_pointer_correct = shared_ptr.pointer() == ptr;
        hud::shared_pointer<hud_test::non_bitwise_type, hud::thread_safety_e::safe> second_shared_ptr(shared_ptr);
        const bool have_2_references = shared_ptr.shared_count() == second_shared_ptr.shared_count() && second_shared_ptr.shared_count() == 2;

        shared_ptr.reset();

        return std::tuple{
            dtor_count == 0,
            is_pointer_correct,
            have_2_references,
            shared_ptr.pointer() == nullptr,
            second_shared_ptr.pointer() == ptr,
            second_shared_ptr.shared_count() == 1u
        };
    };

    // Non constant
    {
        const auto result = test();
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_TRUE(std::get<2>(result));
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
    }

    // Constant is not available with thread safe SharedPointer
    // Not working under with msvc
    // https://developercommunity.visualstudio.com/t/constant-evaluation-with-do-not-works-wi/10058244
/*#if !defined(HD_COMPILER_MSVC)
    {
        constexpr auto result = test();
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_TRUE(std::get<2>(result));
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
    }
#endif*/
}

GTEST_TEST(shared_pointer_safe, reset_to_other_pointer_should_not_delete_if_other_reference_exists) {


    const auto test = []() {
        i32 dtor_count = 0;
        i32 other_dtor_count = 0;

        auto ptr = new hud_test::non_bitwise_type(15, &dtor_count);
        hud::shared_pointer<hud_test::non_bitwise_type, hud::thread_safety_e::safe> shared_ptr(ptr);
        const bool is_pointer_correct = shared_ptr.pointer() == ptr;
        hud::shared_pointer<hud_test::non_bitwise_type, hud::thread_safety_e::safe> second_shared_ptr(shared_ptr);
        const bool have_2_references = shared_ptr.shared_count() == second_shared_ptr.shared_count() && second_shared_ptr.shared_count() == 2;

        auto other_ptr = new hud_test::non_bitwise_type(15, &other_dtor_count);

        shared_ptr.reset(other_ptr);

        return std::tuple{
            dtor_count == 0,
            other_dtor_count == 0,
            is_pointer_correct,
            have_2_references,
            shared_ptr.pointer() == other_ptr,
            shared_ptr.shared_count() == 1u,
            second_shared_ptr.pointer() == ptr,
            second_shared_ptr.shared_count() == 1u
        };
    };

    // Non constant
    {
        const auto result = test();
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_TRUE(std::get<2>(result));
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_TRUE(std::get<6>(result));
        GTEST_ASSERT_TRUE(std::get<7>(result));
    }

    // Constant is not available with thread safe SharedPointer
    // Not working under with msvc
    // https://developercommunity.visualstudio.com/t/constant-evaluation-with-do-not-works-wi/10058244
/*#if !defined(HD_COMPILER_MSVC)
    {
        constexpr auto result = test();
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_TRUE(std::get<2>(result));
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
        GTEST_ASSERT_TRUE(std::get<6>(result));
        GTEST_ASSERT_TRUE(std::get<7>(result));
    }
#endif*/
}

GTEST_TEST(shared_pointer_safe, make_shared) {


    const auto test = []() {
        const auto shared_ptr = hud::make_shared<i32, hud::thread_safety_e::safe>(123);
        i32 dtor_count = 0;
        const auto shared_ptr_2 = hud::make_shared<hud_test::non_bitwise_type, hud::thread_safety_e::safe>(123, &dtor_count);

        return std::tuple{
            hud::is_same_v<decltype(shared_ptr.pointer()), i32*>,
            *shared_ptr.pointer() == 123,
            shared_ptr.shared_count() == 1u,
            hud::is_same_v<decltype(shared_ptr_2.pointer()), hud_test::non_bitwise_type*>,
            shared_ptr_2->id() == 123,
            shared_ptr_2.shared_count() == 1u
        };
    };

    // Non constant
    {
        const auto result = test();
        GTEST_ASSERT_TRUE(std::get<0>(result));
        GTEST_ASSERT_TRUE(std::get<1>(result));
        GTEST_ASSERT_TRUE(std::get<2>(result));
        GTEST_ASSERT_TRUE(std::get<3>(result));
        GTEST_ASSERT_TRUE(std::get<4>(result));
        GTEST_ASSERT_TRUE(std::get<5>(result));
    }

    // Constant
    // Not possible, internal SharedPointer use a Controller that contains the "allocated" value.
    // constant evaluation do not allowed to reinterpret_cast the storage of the value to a pointer to that value
}

GTEST_TEST(shared_pointer_safe, hash) {

    const auto test = []() {
        hud::shared_pointer<i32, hud::thread_safety_e::safe> ptr(new i32(123));
        return hud::hash(ptr) == hud::hash(ptr.pointer());
    };

    GTEST_ASSERT_TRUE(test());
}