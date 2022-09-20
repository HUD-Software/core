#include <core/containers/shared_pointer.h>

TEST(SharedPointer_array_not_safe, swap_with_empty) {


    // SharedPointer::swap
    {
        const auto test = []() {
            i32 dtor_count[2] = { 0,0 };
            auto ptr = new hud::test::NonBitwiseType[2]{ { 1, &dtor_count[0] }, {2, &dtor_count[1] } };
            SharedPointer<hud::test::NonBitwiseType[2]> shared_ptr(ptr);
            const bool is_pointer_correct = shared_ptr.pointer();
            const u32 shared_count_before = shared_ptr.shared_count();

            SharedPointer<hud::test::NonBitwiseType[2]> empty;
            shared_ptr.swap(empty);

            return std::tuple{
                dtor_count[0] == 0,
                dtor_count[1] == 0,
                is_pointer_correct,
                shared_count_before == 1u,
                shared_ptr.pointer() == nullptr,
                shared_ptr.shared_count() == 0u
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
//        {
//            constexpr auto result = test();
//            ASSERT_TRUE(hud::get<0>(result));
//            ASSERT_TRUE(hud::get<1>(result));
//            ASSERT_TRUE(hud::get<2>(result));
//            ASSERT_TRUE(hud::get<3>(result));
//            ASSERT_TRUE(hud::get<4>(result));
//            ASSERT_TRUE(hud::get<5>(result));
//        }
//#endif
    }

    // hud::swap
    {
        const auto test = []() {
            i32 dtor_count[2] = { 0,0 };
            auto ptr = new hud::test::NonBitwiseType[2]{ { 1, &dtor_count[0] }, { 2, &dtor_count[1] } };
            SharedPointer<hud::test::NonBitwiseType[2]> shared_ptr(ptr);
            const bool is_pointer_correct = shared_ptr.pointer();
            const u32 shared_count_before = shared_ptr.shared_count();

            SharedPointer<hud::test::NonBitwiseType[2]> empty;
            swap(shared_ptr, empty);

            return std::tuple{
                dtor_count[0] == 0,
                dtor_count[1] == 0,
                is_pointer_correct,
                shared_count_before == 1u,
                shared_ptr.pointer() == nullptr,
                shared_ptr.shared_count() == 0u
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
//        {
//            constexpr auto result = test();
//            ASSERT_TRUE(hud::get<0>(result));
//            ASSERT_TRUE(hud::get<1>(result));
//            ASSERT_TRUE(hud::get<2>(result));
//            ASSERT_TRUE(hud::get<3>(result));
//            ASSERT_TRUE(hud::get<4>(result));
//            ASSERT_TRUE(hud::get<5>(result));
//        }
//#endif
    }
}

TEST(SharedPointer_array_not_safe, swap_same_type) {


    // SharedPointer::swap
    {
        const auto test = []() {
            i32 dtor_count[2] = { 0,0 };
            i32 dtor_count_1[2] = { 0,0 };
            auto ptr = new hud::test::NonBitwiseType[2]{ { 1, &dtor_count[0] }, { 2, &dtor_count[1] } };
            SharedPointer<hud::test::NonBitwiseType[2]> shared_ptr(ptr);

            auto ptr2 = new hud::test::NonBitwiseType[2]{ { 3, &dtor_count_1[0] }, { 4, &dtor_count_1[1] } };
            SharedPointer<hud::test::NonBitwiseType[2]> shared_ptr_2(ptr2);
            shared_ptr.swap(shared_ptr_2);

            return std::tuple{
                dtor_count[0] == 0,
                dtor_count[1] == 0,
                dtor_count_1[0] == 0,
                dtor_count_1[1] == 0,
                shared_ptr.pointer() == ptr2,
                shared_ptr.shared_count() == 1u,
                shared_ptr_2.pointer() == ptr,
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
        // Not working under with msvc
        // https://developercommunity.visualstudio.com/t/constant-evaluation-with-do-not-works-wi/10058244
//#if !defined(HD_COMPILER_MSVC)
//        {
//            constexpr auto result = test();
//            ASSERT_TRUE(hud::get<0>(result));
//            ASSERT_TRUE(hud::get<1>(result));
//            ASSERT_TRUE(hud::get<2>(result));
//            ASSERT_TRUE(hud::get<3>(result));
//            ASSERT_TRUE(hud::get<4>(result));
//            ASSERT_TRUE(hud::get<5>(result));
//            ASSERT_TRUE(hud::get<6>(result));
//            ASSERT_TRUE(hud::get<7>(result));
//        }
//#endif
    }

    // hud::swap
    {
        const auto test = []() {
            i32 dtor_count[2] = { 0,0 };
            i32 dtor_count_1[2] = { 0,0 };
            auto ptr = new hud::test::NonBitwiseType[2]{ { 1, &dtor_count[0] }, { 2, &dtor_count[1] } };
            SharedPointer<hud::test::NonBitwiseType[2]> shared_ptr(ptr);

            auto ptr2 = new hud::test::NonBitwiseType[2]{ { 3, &dtor_count_1[0] }, { 4, &dtor_count_1[1] } };
            SharedPointer<hud::test::NonBitwiseType[2]> shared_ptr_2(ptr2);
            swap(shared_ptr,shared_ptr_2);

            return std::tuple{
                dtor_count[0] == 0,
                dtor_count[1] == 0,
                dtor_count_1[0] == 0,
                dtor_count_1[1] == 0,
                shared_ptr.pointer() == ptr2,
                shared_ptr.shared_count() == 1u,
                shared_ptr_2.pointer() == ptr,
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
        // Not working under with msvc
        // https://developercommunity.visualstudio.com/t/constant-evaluation-with-do-not-works-wi/10058244
//#if !defined(HD_COMPILER_MSVC)
//        {
//            constexpr auto result = test();
//            ASSERT_TRUE(hud::get<0>(result));
//            ASSERT_TRUE(hud::get<1>(result));
//            ASSERT_TRUE(hud::get<2>(result));
//            ASSERT_TRUE(hud::get<3>(result));
//            ASSERT_TRUE(hud::get<4>(result));
//            ASSERT_TRUE(hud::get<5>(result));
//            ASSERT_TRUE(hud::get<6>(result));
//            ASSERT_TRUE(hud::get<7>(result));
//        }
//#endif
    }
}