#include <core/containers/shared_pointer.h>


TEST(SharedPointer_array_not_safe, single_thread_pointer_return_object) {


    const auto test = []() {
        auto ptr = new hud::test::NonBitwiseType[2]{ { 1, nullptr }, {2, nullptr} };
        SharedPointer<hud::test::NonBitwiseType[2]> shared_ptr(ptr);
        return std::tuple{
            shared_ptr.pointer() == ptr
        };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
    }

    // Constant
    // Not working under with msvc
    // https://developercommunity.visualstudio.com/t/constant-evaluation-with-do-not-works-wi/10058244
//#if !defined(HD_COMPILER_MSVC)
//    {
//        constexpr auto result = test();
//        ASSERT_TRUE(hud::get<0>(result));
//    }
//#endif
}

TEST(SharedPointer_array_not_safe, single_thread_empty_pointer_return_nullptr) {


    const auto test = []() {
        SharedPointer<hud::test::NonBitwiseType[2]> shared_ptr;
        return std::tuple{
            shared_ptr.pointer() == nullptr
        };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
    }

    // Constant
    // Not working under with msvc
    // https://developercommunity.visualstudio.com/t/constant-evaluation-with-do-not-works-wi/10058244
//#if !defined(HD_COMPILER_MSVC)
//    {
//        constexpr auto result = test();
//        ASSERT_TRUE(hud::get<0>(result));
//    }
//#endif
}

TEST(SharedPointer_array_not_safe, single_thread_is_owning) {


    const auto test = []() {
        SharedPointer<hud::test::NonBitwiseType[2]> shared_ptr(new hud::test::NonBitwiseType[2]{ { 1, nullptr }, {2, nullptr} });
        SharedPointer<hud::test::NonBitwiseType[2]> empty_shared_ptr;
        return std::tuple{
            shared_ptr.is_owning(),
            empty_shared_ptr.is_owning()
        };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_FALSE(hud::get<1>(result));
    }

    // Constant
    // Not working under with msvc
    // https://developercommunity.visualstudio.com/t/constant-evaluation-with-do-not-works-wi/10058244
//#if !defined(HD_COMPILER_MSVC)
//    {
//        constexpr auto result = test();
//        ASSERT_TRUE(hud::get<0>(result));
//        ASSERT_FALSE(hud::get<1>(result));
//    }
//#endif
}

TEST(SharedPointer_array_not_safe, cast_to_bool) {


    const auto test = []() {
        SharedPointer<hud::test::NonBitwiseType[2]> shared_ptr(new hud::test::NonBitwiseType[2]{ { 1, nullptr }, { 2, nullptr } });
        SharedPointer<hud::test::NonBitwiseType[2]> empty_shared_ptr;
        return std::tuple{
            static_cast<bool>(shared_ptr),
            static_cast<bool>(empty_shared_ptr)
        };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_FALSE(hud::get<1>(result));
    }

    // Constant
    // Not working under with msvc
    // https://developercommunity.visualstudio.com/t/constant-evaluation-with-do-not-works-wi/10058244
//#if !defined(HD_COMPILER_MSVC)
//    {
//        constexpr auto result = test();
//        ASSERT_TRUE(hud::get<0>(result));
//        ASSERT_FALSE(hud::get<1>(result));
//    }
//#endif
}

TEST(SharedPointer_array_not_safe, operator_dereference) {


    const auto test = []() {
        auto ptr = new hud::test::NonBitwiseType[2]{ { 1, nullptr }, { 2, nullptr } };
        SharedPointer<hud::test::NonBitwiseType[2]> shared_ptr(ptr);
        return std::tuple{
            &(*shared_ptr) == ptr
        };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
    }

    // Constant
    // Not working under with msvc
    // https://developercommunity.visualstudio.com/t/constant-evaluation-with-do-not-works-wi/10058244
//#if !defined(HD_COMPILER_MSVC)
//    {
//        constexpr auto result = test();
//        ASSERT_TRUE(hud::get<0>(result));
//    }
//#endif
}

TEST(SharedPointer_array_not_safe, operator_arrow) {


    const auto test = []() {
        auto ptr = new hud::test::NonBitwiseType[2]{{ 1, nullptr }, { 2, nullptr }};
        SharedPointer<hud::test::NonBitwiseType[2]> shared_ptr(ptr);
        return std::tuple{
            shared_ptr.operator->() == ptr
        };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
    }

    // Constant
    // Not working under with msvc
    // https://developercommunity.visualstudio.com/t/constant-evaluation-with-do-not-works-wi/10058244
//#if !defined(HD_COMPILER_MSVC)
//    {
//        constexpr auto result = test();
//        ASSERT_TRUE(hud::get<0>(result));
//    }
//#endif
}

TEST(SharedPointer_array_not_safe, reset_to_empty) {


    const auto test = []() {
        i32 dtor_count[2] = { 0, 0 };
        auto ptr = new hud::test::NonBitwiseType[2]{ { 1, &dtor_count[0] }, { 2, &dtor_count[1] } };
        SharedPointer<hud::test::NonBitwiseType[2]> shared_ptr(ptr);
        const bool is_pointer_correct = shared_ptr.pointer() == ptr;
        SharedPointer<hud::test::NonBitwiseType[2]> empty_shared_ptr;
        const bool is_pointer_empty_correct = empty_shared_ptr.pointer() == nullptr;

        shared_ptr.reset();
        empty_shared_ptr.reset();

        return std::tuple{
            dtor_count[0] == 1,
            dtor_count[1] == 1,
            is_pointer_correct,
            shared_ptr.pointer() == nullptr,
            is_pointer_empty_correct,
            empty_shared_ptr.pointer() == nullptr
        };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
        ASSERT_TRUE(hud::get<4>(result));
        ASSERT_TRUE(hud::get<5>(result));
    }

    // Constant
    // Not working under with msvc
    // https://developercommunity.visualstudio.com/t/constant-evaluation-with-do-not-works-wi/10058244
//#if !defined(HD_COMPILER_MSVC)
//    {
//        constexpr auto result = test();
//        ASSERT_TRUE(hud::get<0>(result));
//        ASSERT_TRUE(hud::get<1>(result));
//        ASSERT_TRUE(hud::get<2>(result));
//        ASSERT_TRUE(hud::get<3>(result));
//        ASSERT_TRUE(hud::get<4>(result));
//        ASSERT_TRUE(hud::get<5>(result));
//    }
//#endif
}

TEST(SharedPointer_array_not_safe, reset_to_nullptr) {


    const auto test = []() {
        i32 dtor_count[2] = { 0, 0 };
        auto ptr = new hud::test::NonBitwiseType[2]{ { 1, &dtor_count[0] }, { 2, &dtor_count[1] } };
        SharedPointer<hud::test::NonBitwiseType[2]> shared_ptr(ptr);
        const bool is_pointer_correct = shared_ptr.pointer() == ptr;
        SharedPointer<hud::test::NonBitwiseType[2]> empty_shared_ptr;
        const bool is_pointer_empty_correct = empty_shared_ptr.pointer() == nullptr;

        shared_ptr.reset(nullptr);
        empty_shared_ptr.reset(nullptr);

        return std::tuple{
            dtor_count[0] == 1,
            dtor_count[1] == 1,
            is_pointer_correct,
            shared_ptr.pointer() == nullptr,
            is_pointer_empty_correct,
            empty_shared_ptr.pointer() == nullptr
        };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
        ASSERT_TRUE(hud::get<4>(result));
        ASSERT_TRUE(hud::get<5>(result));
    }

    // Constant
    // Not working under with msvc
    // https://developercommunity.visualstudio.com/t/constant-evaluation-with-do-not-works-wi/10058244
//#if !defined(HD_COMPILER_MSVC)
//    {
//        constexpr auto result = test();
//        ASSERT_TRUE(hud::get<0>(result));
//        ASSERT_TRUE(hud::get<1>(result));
//        ASSERT_TRUE(hud::get<2>(result));
//        ASSERT_TRUE(hud::get<3>(result));
//        ASSERT_TRUE(hud::get<4>(result));
//        ASSERT_TRUE(hud::get<5>(result));
//    }
//#endif
}

TEST(SharedPointer_array_not_safe, reset_to_same_type) {


    const auto test = []() {
        i32 dtor_count[2] = { 0, 0 };
        i32 dtor_count_1[2] = { 0, 0 };
        i32 dtor_count_2[2] = { 0, 0 };
        auto ptr = new hud::test::NonBitwiseType[2]{ { 1, &dtor_count[0] }, { 2, &dtor_count[1] } };
        SharedPointer<hud::test::NonBitwiseType[2]> shared_ptr(ptr);
        const bool is_pointer_correct = shared_ptr.pointer() == ptr;

        SharedPointer<hud::test::NonBitwiseType[2]> empty_shared_ptr;
        const bool is_pointer_empty_correct = empty_shared_ptr.pointer() == nullptr;

        auto reset_1 = new hud::test::NonBitwiseType[2]{ { 2, &dtor_count_1[0] }, { 3, &dtor_count_1[1] } };
        shared_ptr.reset(reset_1);

        auto reset_2 = new hud::test::NonBitwiseType[2]{ { 4, &dtor_count_1[0] }, { 5, &dtor_count_1[1] } };
        empty_shared_ptr.reset(reset_2);

        return std::tuple{
            dtor_count[0] == 1,
            dtor_count[1] == 1,
            is_pointer_correct,
            shared_ptr.pointer() == reset_1,
            shared_ptr.shared_count() == 1u,
            dtor_count_1[0] == 0,
            dtor_count_1[1] == 0,
            is_pointer_empty_correct,
            empty_shared_ptr.pointer() == reset_2,
            empty_shared_ptr.shared_count() == 1u,
            dtor_count_2[0] == 0,
            dtor_count_2[1] == 0,
        };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
        ASSERT_TRUE(hud::get<4>(result));
        ASSERT_TRUE(hud::get<5>(result));
        ASSERT_TRUE(hud::get<6>(result));
        ASSERT_TRUE(hud::get<7>(result));
        ASSERT_TRUE(hud::get<8>(result));
        ASSERT_TRUE(hud::get<9>(result));
        ASSERT_TRUE(hud::get<10>(result));
        ASSERT_TRUE(hud::get<11>(result));
    }

    // Constant
    // Not working under with msvc
    // https://developercommunity.visualstudio.com/t/constant-evaluation-with-do-not-works-wi/10058244
//#if !defined(HD_COMPILER_MSVC)
//    {
//        constexpr auto result = test();
//        ASSERT_TRUE(hud::get<0>(result));
//        ASSERT_TRUE(hud::get<1>(result));
//        ASSERT_TRUE(hud::get<2>(result));
//        ASSERT_TRUE(hud::get<3>(result));
//        ASSERT_TRUE(hud::get<4>(result));
//        ASSERT_TRUE(hud::get<5>(result));
//        ASSERT_TRUE(hud::get<6>(result));
//        ASSERT_TRUE(hud::get<7>(result));
//        ASSERT_TRUE(hud::get<8>(result));
//        ASSERT_TRUE(hud::get<9>(result));
//        ASSERT_TRUE(hud::get<10>(result));
//        ASSERT_TRUE(hud::get<11>(result));
//    }
//#endif
}

TEST(SharedPointer_array_not_safe, reset_to_different_type) {


    const auto test = []() {
        i32 dtor_count[2] = { 0, 0 };
        i32 dtor_count_1[2] = { 0, 0 };
        i32 dtor_count_2[2] = { 0, 0 };
        auto ptr = new hud::test::NonBitwiseType[2]{ { 1, &dtor_count[0] }, { 2, &dtor_count[1] } };
        auto reset_1 = new hud::test::NonBitwiseType[2]{ { 2, &dtor_count_1[0] }, { 3, &dtor_count_1[1] } };
        auto reset_2 = new hud::test::NonBitwiseType[2]{ { 4, &dtor_count_1[0] }, { 5, &dtor_count_1[1] } };

        SharedPointer<const hud::test::NonBitwiseType[2]> shared_ptr(ptr);
        const bool is_pointer_correct = shared_ptr.pointer() == ptr;

        SharedPointer<const hud::test::NonBitwiseType[2]> empty_shared_ptr;
        const bool is_pointer_empty_correct = empty_shared_ptr.pointer() == nullptr;

        shared_ptr.reset(reset_1);

        empty_shared_ptr.reset(reset_2);

        return std::tuple{
            dtor_count[0] == 1,
            dtor_count[1] == 1,
            is_pointer_correct,
            shared_ptr.pointer() == reset_1,
            shared_ptr.shared_count() == 1u,
            dtor_count_1[0] == 0,
            dtor_count_1[1] == 0,
            is_pointer_empty_correct,
            empty_shared_ptr.pointer() == reset_2,
            empty_shared_ptr.shared_count() == 1u,
            dtor_count_2[0] == 0,
            dtor_count_2[1] == 0,
        };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
        ASSERT_TRUE(hud::get<4>(result));
        ASSERT_TRUE(hud::get<5>(result));
        ASSERT_TRUE(hud::get<6>(result));
        ASSERT_TRUE(hud::get<7>(result));
        ASSERT_TRUE(hud::get<8>(result));
        ASSERT_TRUE(hud::get<9>(result));
        ASSERT_TRUE(hud::get<10>(result));
        ASSERT_TRUE(hud::get<11>(result));

    }

    // Constant
    // Not working under with msvc
    // https://developercommunity.visualstudio.com/t/constant-evaluation-with-do-not-works-wi/10058244
//#if !defined(HD_COMPILER_MSVC)
//    {
//        constexpr auto result = test();
//        ASSERT_TRUE(hud::get<0>(result));
//        ASSERT_TRUE(hud::get<1>(result));
//        ASSERT_TRUE(hud::get<2>(result));
//        ASSERT_TRUE(hud::get<3>(result));
//        ASSERT_TRUE(hud::get<4>(result));
//        ASSERT_TRUE(hud::get<5>(result));
//        ASSERT_TRUE(hud::get<6>(result));
//        ASSERT_TRUE(hud::get<7>(result));
//        ASSERT_TRUE(hud::get<8>(result));
//        ASSERT_TRUE(hud::get<9>(result));
//        ASSERT_TRUE(hud::get<10>(result));
//        ASSERT_TRUE(hud::get<11>(result));
//    }
//#endif
}

TEST(SharedPointer_array_not_safe, reset_to_empty_should_not_delete_if_other_reference_exists) {


    const auto test = []() {
        i32 dtor_count[2] = { 0, 0 };
        auto ptr = new hud::test::NonBitwiseType[2]{ { 1, &dtor_count[0] }, { 2, &dtor_count[1] } };
        SharedPointer<hud::test::NonBitwiseType[2]> shared_ptr(ptr);
        const bool is_pointer_correct = shared_ptr.pointer() == ptr;
        SharedPointer<hud::test::NonBitwiseType[2]> second_shared_ptr(shared_ptr);
        const bool have_2_references = shared_ptr.shared_count() == second_shared_ptr.shared_count() && second_shared_ptr.shared_count() == 2;

        shared_ptr.reset();

        return std::tuple{
            dtor_count[0] == 0,
            dtor_count[1] == 0,
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
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
        ASSERT_TRUE(hud::get<4>(result));
        ASSERT_TRUE(hud::get<5>(result));
        ASSERT_TRUE(hud::get<6>(result));
    }

    // Constant
    // Not working under with msvc
    // https://developercommunity.visualstudio.com/t/constant-evaluation-with-do-not-works-wi/10058244
//#if !defined(HD_COMPILER_MSVC)
//    {
//        constexpr auto result = test();
//        ASSERT_TRUE(hud::get<0>(result));
//        ASSERT_TRUE(hud::get<1>(result));
//        ASSERT_TRUE(hud::get<2>(result));
//        ASSERT_TRUE(hud::get<3>(result));
//        ASSERT_TRUE(hud::get<4>(result));
//        ASSERT_TRUE(hud::get<5>(result));
//        ASSERT_TRUE(hud::get<6>(result));
//    }
//#endif
}


TEST(SharedPointer_array_not_safe, reset_to_other_pointer_should_not_delete_if_other_reference_exists) {


    const auto test = []() {
        i32 dtor_count[2] = { 0,0 };
        i32 other_dtor_count[2] = { 0,0 };

        auto ptr = new hud::test::NonBitwiseType[2]{ { 1, &dtor_count[0] }, { 2, &dtor_count[1] } };
        SharedPointer<hud::test::NonBitwiseType[2]> shared_ptr(ptr);
        const bool is_pointer_correct = shared_ptr.pointer() == ptr;
        SharedPointer<hud::test::NonBitwiseType[2]> second_shared_ptr(shared_ptr);
        const bool have_2_references = shared_ptr.shared_count() == second_shared_ptr.shared_count() && second_shared_ptr.shared_count() == 2;
        
        auto other_ptr = new hud::test::NonBitwiseType[2]{ { 3, &dtor_count[0] }, { 4, &dtor_count[1] } };

        shared_ptr.reset(other_ptr);

        return std::tuple{
            dtor_count[0] == 0,
            dtor_count[1] == 0,
            other_dtor_count[0] == 0,
            other_dtor_count[1] == 0,
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
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
        ASSERT_TRUE(hud::get<4>(result));
        ASSERT_TRUE(hud::get<5>(result));
        ASSERT_TRUE(hud::get<6>(result));
        ASSERT_TRUE(hud::get<7>(result));
        ASSERT_TRUE(hud::get<8>(result));
        ASSERT_TRUE(hud::get<9>(result));
    }

    // Constant
    // Not working under with msvc
    // https://developercommunity.visualstudio.com/t/constant-evaluation-with-do-not-works-wi/10058244
//#if !defined(HD_COMPILER_MSVC)
//    {
//        constexpr auto result = test();
//        ASSERT_TRUE(hud::get<0>(result));
//        ASSERT_TRUE(hud::get<1>(result));
//        ASSERT_TRUE(hud::get<2>(result));
//        ASSERT_TRUE(hud::get<3>(result));
//        ASSERT_TRUE(hud::get<4>(result));
//        ASSERT_TRUE(hud::get<5>(result));
//        ASSERT_TRUE(hud::get<6>(result));
//        ASSERT_TRUE(hud::get<7>(result));
//        ASSERT_TRUE(hud::get<8>(result));
//        ASSERT_TRUE(hud::get<9>(result));
//    }
//#endif
}

TEST(SharedPointer_array_not_safe, make_shared) {


    const auto test = []() {
        const auto shared_ptr = make_shared<i32[]>(2);
        hud::memory::construct_at(&shared_ptr[0], 1);
        hud::memory::construct_at(&shared_ptr[1], 2);
        i32 dtor_count[2] = { 0,0 };
        const auto shared_ptr_2 = make_shared<hud::test::NonBitwiseType[]>(2);
        hud::memory::construct_at(&shared_ptr_2[0], 1, &dtor_count[0]);
        hud::memory::construct_at(&shared_ptr_2[1], 2, &dtor_count[1]);

        return std::tuple{
            hud::is_same_v<decltype(shared_ptr.pointer()), i32*>,
            shared_ptr[0] == 1,
            shared_ptr[1] == 2,
            shared_ptr.shared_count() == 1u,
            hud::is_same_v<decltype(shared_ptr_2.pointer()), hud::test::NonBitwiseType*>,
            shared_ptr_2[0].id() == 1,
            shared_ptr_2[1].id() == 2,
            shared_ptr_2.shared_count() == 1u
        };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(hud::get<0>(result));
        ASSERT_TRUE(hud::get<1>(result));
        ASSERT_TRUE(hud::get<2>(result));
        ASSERT_TRUE(hud::get<3>(result));
        ASSERT_TRUE(hud::get<4>(result));
        ASSERT_TRUE(hud::get<5>(result));
        ASSERT_TRUE(hud::get<6>(result));
        ASSERT_TRUE(hud::get<7>(result));
    }

    // Constant
    // Not possible, internal SharedPointer use a Controller that contains the "allocated" value.
    // constant evaluation do not allowed to reinterpret_cast the storage of the value to a pointer to that value
}

TEST(SharedPointer_array_not_safe, hash) {

    const auto test = []() {
        SharedPointer<i32[2]> ptr(new i32[2]{ 1,2 });
        return hash(ptr) == hash(ptr.pointer());
    };

    ASSERT_TRUE(test());
}