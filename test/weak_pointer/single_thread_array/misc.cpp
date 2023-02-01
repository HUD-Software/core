#include <core/containers/shared_pointer.h>
#include <memory>

GTEST_TEST(weak_pointer_array_not_safe, reset)
{
    const auto test = []()
    {
        i32 dtor_count[2] = {0, 0};
        auto ptr = new hud_test::non_bitwise_type[2] {
            {1, &dtor_count[0]},
            {2, &dtor_count[1]}
        };
        hud::shared_pointer<hud_test::non_bitwise_type[2]> shared_ptr {ptr};
        hud::weak_pointer<hud_test::non_bitwise_type[2]> weak_ptr {shared_ptr};
        const bool pointer_ok_before_reset = weak_ptr.lock().pointer() == ptr;
        const bool shared_count_ok_before_reset = shared_ptr.shared_count() == 1u;

        weak_ptr.reset();

        const auto locked_weak_ptr = weak_ptr.lock();

        return std::tuple {
            pointer_ok_before_reset,
            shared_count_ok_before_reset,
            locked_weak_ptr.pointer() == nullptr,
            locked_weak_ptr.shared_count() == 0u,
            shared_ptr.shared_count() == 1u,
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

    // Constant
    // Not working under with msvc
    // https://developercommunity.visualstudio.com/t/constant-evaluation-with-do-not-works-wi/10058244
    // #if !defined(HD_COMPILER_MSVC)
    // {
    //     constexpr auto result = test();
    //     hud_assert_true(std::get<0>(result));
    //     hud_assert_true(std::get<1>(result));
    //     hud_assert_true(std::get<2>(result));
    //     hud_assert_true(std::get<3>(result));
    // }
    // #endif
}

GTEST_TEST(weak_pointer_array_not_safe, shared_count)
{
    const auto test = []()
    {
        i32 dtor_count[2] = {0, 0};
        auto ptr = new hud_test::non_bitwise_type[2] {
            {1, &dtor_count[0]},
            {2, &dtor_count[1]}
        };
        hud::shared_pointer<hud_test::non_bitwise_type[2]> shared_ptr {ptr};
        hud::weak_pointer<hud_test::non_bitwise_type[2]> weak_ptr {shared_ptr};

        return std::tuple {
            weak_ptr.shared_count() == 1u,
            shared_ptr.shared_count() == 1u,
        };
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
    }

    // Constant
    // Not working under with msvc
    // https://developercommunity.visualstudio.com/t/constant-evaluation-with-do-not-works-wi/10058244
    // #if !defined(HD_COMPILER_MSVC)
    // {
    //     constexpr auto result = test();
    //     hud_assert_true(std::get<0>(result));
    //     hud_assert_true(std::get<1>(result));
    //     hud_assert_true(std::get<2>(result));
    //     hud_assert_true(std::get<3>(result));
    // }
    // #endif
}

GTEST_TEST(weak_pointer_array_not_safe, expired)
{
    const auto test = []()
    {
        i32 dtor_count[2] = {0, 0};
        auto ptr = new hud_test::non_bitwise_type[2] {
            {1, &dtor_count[0]},
            {2, &dtor_count[1]}
        };
        hud::shared_pointer<hud_test::non_bitwise_type[2]> shared_ptr {ptr};
        hud::weak_pointer<hud_test::non_bitwise_type[2]> weak_ptr {shared_ptr};
        const bool pointer_ok_before_reset = weak_ptr.lock().pointer() == ptr;
        const bool shared_count_ok_before_reset = shared_ptr.shared_count() == 1u;
        const bool expire_is_ok_before_reset = !weak_ptr.expired();

        weak_ptr.reset();

        return std::tuple {
            pointer_ok_before_reset,
            shared_count_ok_before_reset,
            expire_is_ok_before_reset,
            weak_ptr.expired(),
            shared_ptr.shared_count() == 1u,
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

    // Constant
    // Not working under with msvc
    // https://developercommunity.visualstudio.com/t/constant-evaluation-with-do-not-works-wi/10058244
    // #if !defined(HD_COMPILER_MSVC)
    // {
    //     constexpr auto result = test();
    //     hud_assert_true(std::get<0>(result));
    //     hud_assert_true(std::get<1>(result));
    //     hud_assert_true(std::get<2>(result));
    //     hud_assert_true(std::get<3>(result));
    // }
    // #endif

    struct Foo
    {
        int n1;
        int n2;

        Foo(int a, int b)
            : n1(a)
            , n2(b)
        {
        }
    };

    auto p1 = std::make_shared<Foo>(1, 2);
    std::shared_ptr<int> p2(p1, &p1->n1);
    std::shared_ptr<int> p3(p1, &p1->n2);
}