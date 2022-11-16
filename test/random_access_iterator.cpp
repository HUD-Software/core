#include <core/iterators/random_access_iterator.h>

GTEST_TEST(random_access_iterator, check_const_correctness) {
    i32 arr[5] = { 10,20,30,40,50 };
    using array_type = decltype(arr);
    const i32 arr_const[5] = { 10,20,30,40,50 };
    using const_array_type = decltype(arr_const);

    hud::random_access_iterator<array_type> it(arr);
    ASSERT_FALSE(hud::is_const_v<decltype(*it)>);

    hud::random_access_iterator<const_array_type> it_const(arr_const);
    ASSERT_TRUE(hud::is_const_v<hud::remove_reference_t<decltype(*it_const)>>);

    hud::random_access_iterator<const_array_type> it_const_2(arr);
    ASSERT_TRUE(hud::is_const_v<hud::remove_reference_t<decltype(*it_const_2)>>);
}


GTEST_TEST(random_access_iterator, constructor) {
    const auto test = []() {
        i32 arr[5] = { 10,20,30,40,50 };
        using array_type = decltype(arr);
        const i32 arr_const[5] = { 10,20,30,40,50 };
        using const_array_type = decltype(arr_const);

        hud::random_access_iterator<array_type> it(arr);
        hud::random_access_iterator<const_array_type> it_const(arr_const);
        return std::tuple{
            it.operator->() == &arr[0],
            it_const.operator->() == &arr_const[0]
        };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
    }
}

GTEST_TEST(random_access_iterator, operator_arrow) {
    const auto test = []() {
        i32 arr[5] = { 10,20,30,40,50 };
        using array_type = decltype(arr);
        const i32 arr_const[5] = { 10,20,30,40,50 };
        using const_array_type = decltype(arr_const);

        return std::tuple{
            hud::random_access_iterator<array_type>(arr).operator->() == &arr[0], // Position is 0 (first)
            hud::random_access_iterator<array_type>(arr, 4).operator->() == &arr[4], // Position is 4 (last)
            hud::random_access_iterator<const_array_type>(arr_const).operator->() == &arr_const[0], // Position is 0 (first)
            hud::random_access_iterator<const_array_type>(arr_const, 4).operator->() == &arr_const[4] // Position is 4 (last) 
        };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
    }

    // Constant
    {
        const auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
    }
}

GTEST_TEST(random_access_iterator, operator_dereference) {
const auto test = []() {
        i32 arr[5] = { 10,20,30,40,50 };
        using array_type = decltype(arr);
        const i32 arr_const[5] = { 10,20,30,40,50 };
        using const_array_type = decltype(arr_const);

        return std::tuple{
            *hud::random_access_iterator<array_type>(arr) == arr[0], // Position is 0 (first)
            *hud::random_access_iterator<array_type>(arr, 4) == arr[4], // Position is 4 (last)
            *hud::random_access_iterator<const_array_type>(arr_const) == arr_const[0], // Position is 0 (first)
            *hud::random_access_iterator<const_array_type>(arr_const, 4) == arr_const[4] // Position is 4 (last) 
        };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
    }

    // Constant
    {
        const auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
    }
}

GTEST_TEST(random_access_iterator, operator_pre_increment) {
    const auto test = []() {
        i32 arr[5] = { 10,20,30,40,50 };
        using array_type = decltype(arr);
        const i32 arr_const[5] = { 10,20,30,40,50 };
        using const_array_type = decltype(arr_const);

        hud::random_access_iterator<array_type> it(arr);
        const auto mutable_it = std::tuple{
            it.operator->() == &arr[0],
           *it == arr[0],
           (++it).operator->() == &arr[1],
           *it == arr[1],
           (++it).operator->() == &arr[2],
           *it == arr[2],
           (++it).operator->() == &arr[3],
           *it == arr[3],
           (++it).operator->() == &arr[4],
           *it == arr[4]
        };
        hud::random_access_iterator<const_array_type> it_const(arr_const);
        const auto const_it = std::tuple{
            it_const.operator->() == &arr_const[0],
           *it_const == arr_const[0],
           (++it_const).operator->() == &arr_const[1],
           *it_const == arr_const[1],
           (++it_const).operator->() == &arr_const[2],
           *it_const == arr_const[2],
           (++it_const).operator->() == &arr_const[3],
           *it_const == arr_const[3],
           (++it_const).operator->() == &arr_const[4],
           *it_const == arr_const[4]
        };
        return std::tuple{
           mutable_it,
           const_it
        };
    };

    // Non constant
    {
        const auto result = test();

        const auto& mutable_it = std::get<0>(result);
        ASSERT_TRUE(std::get<0>(mutable_it));
        ASSERT_TRUE(std::get<1>(mutable_it));
        ASSERT_TRUE(std::get<2>(mutable_it));
        ASSERT_TRUE(std::get<3>(mutable_it));
        ASSERT_TRUE(std::get<4>(mutable_it));
        ASSERT_TRUE(std::get<5>(mutable_it));
        ASSERT_TRUE(std::get<6>(mutable_it));
        ASSERT_TRUE(std::get<7>(mutable_it));
        ASSERT_TRUE(std::get<8>(mutable_it));
        ASSERT_TRUE(std::get<9>(mutable_it));

        const auto& const_it = std::get<1>(result);
        ASSERT_TRUE(std::get<0>(const_it));
        ASSERT_TRUE(std::get<1>(const_it));
        ASSERT_TRUE(std::get<2>(const_it));
        ASSERT_TRUE(std::get<3>(const_it));
        ASSERT_TRUE(std::get<4>(const_it));
        ASSERT_TRUE(std::get<5>(const_it));
        ASSERT_TRUE(std::get<6>(const_it));
        ASSERT_TRUE(std::get<7>(const_it));
        ASSERT_TRUE(std::get<8>(const_it));
        ASSERT_TRUE(std::get<9>(const_it));
    }

    // Constant
    {
        constexpr auto result = test();

        const auto& mutable_it = std::get<0>(result);
        ASSERT_TRUE(std::get<0>(mutable_it));
        ASSERT_TRUE(std::get<1>(mutable_it));
        ASSERT_TRUE(std::get<2>(mutable_it));
        ASSERT_TRUE(std::get<3>(mutable_it));
        ASSERT_TRUE(std::get<4>(mutable_it));
        ASSERT_TRUE(std::get<5>(mutable_it));
        ASSERT_TRUE(std::get<6>(mutable_it));
        ASSERT_TRUE(std::get<7>(mutable_it));
        ASSERT_TRUE(std::get<8>(mutable_it));
        ASSERT_TRUE(std::get<9>(mutable_it));

        const auto& const_it = std::get<1>(result);
        ASSERT_TRUE(std::get<0>(const_it));
        ASSERT_TRUE(std::get<1>(const_it));
        ASSERT_TRUE(std::get<2>(const_it));
        ASSERT_TRUE(std::get<3>(const_it));
        ASSERT_TRUE(std::get<4>(const_it));
        ASSERT_TRUE(std::get<5>(const_it));
        ASSERT_TRUE(std::get<6>(const_it));
        ASSERT_TRUE(std::get<7>(const_it));
        ASSERT_TRUE(std::get<8>(const_it));
        ASSERT_TRUE(std::get<9>(const_it));
    }
}


GTEST_TEST(random_access_iterator, operator_post_increment) {
    const auto test = []() {
        i32 arr[5] = { 10,20,30,40,50 };
        using array_type = decltype(arr);
        const i32 arr_const[5] = { 10,20,30,40,50 };
        using const_array_type = decltype(arr_const);

        hud::random_access_iterator<array_type> it(arr);
        const auto mutable_it = std::tuple{
            it.operator->() == &arr[0],
           *it == arr[0],
           (it++).operator->() == &arr[0],
           *it == arr[1],
           (it++).operator->() == &arr[1],
           *it == arr[2],
           (it++).operator->() == &arr[2],
           *it == arr[3],
           (it++).operator->() == &arr[3],
           *it == arr[4]
        };
        hud::random_access_iterator<const_array_type> it_const(arr_const);
        const auto const_it = std::tuple{
            it_const.operator->() == &arr_const[0],
           *it_const == arr_const[0],
           (it_const++).operator->() == &arr_const[0],
           *it_const == arr_const[1],
           (it_const++).operator->() == &arr_const[1],
           *it_const == arr_const[2],
           (it_const++).operator->() == &arr_const[2],
           *it_const == arr_const[3],
           (it_const++).operator->() == &arr_const[3],
           *it_const == arr_const[4]
        };
        return std::tuple{
           mutable_it,
           const_it
        };
    };

    // Non constant
    {
        const auto result = test();

        const auto& mutable_it = std::get<0>(result);
        ASSERT_TRUE(std::get<0>(mutable_it));
        ASSERT_TRUE(std::get<1>(mutable_it));
        ASSERT_TRUE(std::get<2>(mutable_it));
        ASSERT_TRUE(std::get<3>(mutable_it));
        ASSERT_TRUE(std::get<4>(mutable_it));
        ASSERT_TRUE(std::get<5>(mutable_it));
        ASSERT_TRUE(std::get<6>(mutable_it));
        ASSERT_TRUE(std::get<7>(mutable_it));
        ASSERT_TRUE(std::get<8>(mutable_it));
        ASSERT_TRUE(std::get<9>(mutable_it));

        const auto& const_it = std::get<1>(result);
        ASSERT_TRUE(std::get<0>(const_it));
        ASSERT_TRUE(std::get<1>(const_it));
        ASSERT_TRUE(std::get<2>(const_it));
        ASSERT_TRUE(std::get<3>(const_it));
        ASSERT_TRUE(std::get<4>(const_it));
        ASSERT_TRUE(std::get<5>(const_it));
        ASSERT_TRUE(std::get<6>(const_it));
        ASSERT_TRUE(std::get<7>(const_it));
        ASSERT_TRUE(std::get<8>(const_it));
        ASSERT_TRUE(std::get<9>(const_it));
    }

    // Constant
    {
        constexpr auto result = test();

        const auto& mutable_it = std::get<0>(result);
        ASSERT_TRUE(std::get<0>(mutable_it));
        ASSERT_TRUE(std::get<1>(mutable_it));
        ASSERT_TRUE(std::get<2>(mutable_it));
        ASSERT_TRUE(std::get<3>(mutable_it));
        ASSERT_TRUE(std::get<4>(mutable_it));
        ASSERT_TRUE(std::get<5>(mutable_it));
        ASSERT_TRUE(std::get<6>(mutable_it));
        ASSERT_TRUE(std::get<7>(mutable_it));
        ASSERT_TRUE(std::get<8>(mutable_it));
        ASSERT_TRUE(std::get<9>(mutable_it));

        const auto& const_it = std::get<1>(result);
        ASSERT_TRUE(std::get<0>(const_it));
        ASSERT_TRUE(std::get<1>(const_it));
        ASSERT_TRUE(std::get<2>(const_it));
        ASSERT_TRUE(std::get<3>(const_it));
        ASSERT_TRUE(std::get<4>(const_it));
        ASSERT_TRUE(std::get<5>(const_it));
        ASSERT_TRUE(std::get<6>(const_it));
        ASSERT_TRUE(std::get<7>(const_it));
        ASSERT_TRUE(std::get<8>(const_it));
        ASSERT_TRUE(std::get<9>(const_it));
    }
}

GTEST_TEST(random_access_iterator, operator_increment_assign) {
    const auto test = []() {
        i32 arr[5] = { 10,20,30,40,50 };
        using array_type = decltype(arr);
        const i32 arr_const[5] = { 10,20,30,40,50 };
        using const_array_type = decltype(arr_const);

        hud::random_access_iterator<array_type> it(arr);
        const auto mutable_it = std::tuple{
            it.operator->() == &arr[0],
            *it == arr[0],
            (it += 1).operator->() == &arr[1],
            *it == arr[1]
        };

        hud::random_access_iterator<const_array_type> it_const(arr_const);
        const auto const_it = std::tuple{
              it_const.operator->() == &arr_const[0],
            *it_const == arr_const[0],
            (it_const += 1).operator->() == &arr_const[1],
            *it_const == arr_const[1]
        };
        return std::tuple{
          mutable_it,
          const_it
        };
    };

    // Non constant
    {
        const auto result = test();

        const auto& mutable_it = std::get<0>(result);
        ASSERT_TRUE(std::get<0>(mutable_it));
        ASSERT_TRUE(std::get<1>(mutable_it));
        ASSERT_TRUE(std::get<2>(mutable_it));
        ASSERT_TRUE(std::get<3>(mutable_it));

        const auto& const_it = std::get<1>(result);
        ASSERT_TRUE(std::get<0>(const_it));
        ASSERT_TRUE(std::get<1>(const_it));
        ASSERT_TRUE(std::get<2>(const_it));
        ASSERT_TRUE(std::get<3>(const_it));
    }

    // Constant
    {
        constexpr auto result = test();

        const auto& mutable_it = std::get<0>(result);
        ASSERT_TRUE(std::get<0>(mutable_it));
        ASSERT_TRUE(std::get<1>(mutable_it));
        ASSERT_TRUE(std::get<2>(mutable_it));
        ASSERT_TRUE(std::get<3>(mutable_it));

        const auto& const_it = std::get<1>(result);
        ASSERT_TRUE(std::get<0>(const_it));
        ASSERT_TRUE(std::get<1>(const_it));
        ASSERT_TRUE(std::get<2>(const_it));
        ASSERT_TRUE(std::get<3>(const_it));
    }
}

GTEST_TEST(random_access_iterator, operator_increment) {
    const auto test = []() {
        i32 arr[5] = { 10,20,30,40,50 };
        using array_type = decltype(arr);
        const i32 arr_const[5] = { 10,20,30,40,50 };
        using const_array_type = decltype(arr_const);

        hud::random_access_iterator<array_type> it(arr);
        hud::random_access_iterator<array_type> res_it = it + 1;
        const auto mutable_it = std::tuple{
            it.operator->() == &arr[0],
            *it == arr[0],
            res_it.operator->() == &arr[1],
            *res_it == arr[1]
        };

        hud::random_access_iterator<const_array_type> it_const(arr_const);
        hud::random_access_iterator<const_array_type> res_it_const = it_const + 1;
        const auto const_it = std::tuple{
             it_const.operator->() == &arr_const[0],
            *it_const == arr[0],
            res_it_const.operator->() == &arr_const[1],
            *res_it_const == arr_const[1]
        };
        return std::tuple{
          mutable_it,
          const_it
        };
    };

    // Non constant
    {
        const auto result = test();

        const auto& mutable_it = std::get<0>(result);
        ASSERT_TRUE(std::get<0>(mutable_it));
        ASSERT_TRUE(std::get<1>(mutable_it));
        ASSERT_TRUE(std::get<2>(mutable_it));
        ASSERT_TRUE(std::get<3>(mutable_it));

        const auto& const_it = std::get<1>(result);
        ASSERT_TRUE(std::get<0>(const_it));
        ASSERT_TRUE(std::get<1>(const_it));
        ASSERT_TRUE(std::get<2>(const_it));
        ASSERT_TRUE(std::get<3>(const_it));
    }

    // Constant
    {
        constexpr auto result = test();

        const auto& mutable_it = std::get<0>(result);
        ASSERT_TRUE(std::get<0>(mutable_it));
        ASSERT_TRUE(std::get<1>(mutable_it));
        ASSERT_TRUE(std::get<2>(mutable_it));
        ASSERT_TRUE(std::get<3>(mutable_it));

        const auto& const_it = std::get<1>(result);
        ASSERT_TRUE(std::get<0>(const_it));
        ASSERT_TRUE(std::get<1>(const_it));
        ASSERT_TRUE(std::get<2>(const_it));
        ASSERT_TRUE(std::get<3>(const_it));
    }
}


GTEST_TEST(random_access_iterator, operator_pre_decrement) {
    const auto test = []() {
        i32 arr[5] = { 10,20,30,40,50 };
        using array_type = decltype(arr);
        const i32 arr_const[5] = { 10,20,30,40,50 };
        using const_array_type = decltype(arr_const);

        hud::random_access_iterator<array_type> it(arr, 4);
        const auto mutable_it = std::tuple{
            it.operator->() == &arr[4],
           *it == arr[4],
           (--it).operator->() == &arr[3],
           *it == arr[3],
           (--it).operator->() == &arr[2],
           *it == arr[2],
           (--it).operator->() == &arr[1],
           *it == arr[1],
           (--it).operator->() == &arr[0],
           *it == arr[0]
        };
        hud::random_access_iterator<const_array_type> it_const(arr_const, 4);
        const auto const_it = std::tuple{
            it_const.operator->() == &arr_const[4],
           *it_const == arr_const[4],
           (--it_const).operator->() == &arr_const[3],
           *it_const == arr_const[3],
           (--it_const).operator->() == &arr_const[2],
           *it_const == arr_const[2],
           (--it_const).operator->() == &arr_const[1],
           *it_const == arr_const[1],
           (--it_const).operator->() == &arr_const[0],
           *it_const == arr_const[0]
        };
        return std::tuple{
           mutable_it,
           const_it
        };
    };

    // Non constant
    {
        const auto result = test();

        const auto& mutable_it = std::get<0>(result);
        ASSERT_TRUE(std::get<0>(mutable_it));
        ASSERT_TRUE(std::get<1>(mutable_it));
        ASSERT_TRUE(std::get<2>(mutable_it));
        ASSERT_TRUE(std::get<3>(mutable_it));
        ASSERT_TRUE(std::get<4>(mutable_it));
        ASSERT_TRUE(std::get<5>(mutable_it));
        ASSERT_TRUE(std::get<6>(mutable_it));
        ASSERT_TRUE(std::get<7>(mutable_it));
        ASSERT_TRUE(std::get<8>(mutable_it));
        ASSERT_TRUE(std::get<9>(mutable_it));

        const auto& const_it = std::get<1>(result);
        ASSERT_TRUE(std::get<0>(const_it));
        ASSERT_TRUE(std::get<1>(const_it));
        ASSERT_TRUE(std::get<2>(const_it));
        ASSERT_TRUE(std::get<3>(const_it));
        ASSERT_TRUE(std::get<4>(const_it));
        ASSERT_TRUE(std::get<5>(const_it));
        ASSERT_TRUE(std::get<6>(const_it));
        ASSERT_TRUE(std::get<7>(const_it));
        ASSERT_TRUE(std::get<8>(const_it));
        ASSERT_TRUE(std::get<9>(const_it));
    }

    // Constant
    {
        constexpr auto result = test();

        const auto& mutable_it = std::get<0>(result);
        ASSERT_TRUE(std::get<0>(mutable_it));
        ASSERT_TRUE(std::get<1>(mutable_it));
        ASSERT_TRUE(std::get<2>(mutable_it));
        ASSERT_TRUE(std::get<3>(mutable_it));
        ASSERT_TRUE(std::get<4>(mutable_it));
        ASSERT_TRUE(std::get<5>(mutable_it));
        ASSERT_TRUE(std::get<6>(mutable_it));
        ASSERT_TRUE(std::get<7>(mutable_it));
        ASSERT_TRUE(std::get<8>(mutable_it));
        ASSERT_TRUE(std::get<9>(mutable_it));

        const auto& const_it = std::get<1>(result);
        ASSERT_TRUE(std::get<0>(const_it));
        ASSERT_TRUE(std::get<1>(const_it));
        ASSERT_TRUE(std::get<2>(const_it));
        ASSERT_TRUE(std::get<3>(const_it));
        ASSERT_TRUE(std::get<4>(const_it));
        ASSERT_TRUE(std::get<5>(const_it));
        ASSERT_TRUE(std::get<6>(const_it));
        ASSERT_TRUE(std::get<7>(const_it));
        ASSERT_TRUE(std::get<8>(const_it));
        ASSERT_TRUE(std::get<9>(const_it));
    }
}


GTEST_TEST(random_access_iterator, operator_post_decrement) {
    const auto test = []() {
        i32 arr[5] = { 10,20,30,40,50 };
        using array_type = decltype(arr);
        const i32 arr_const[5] = { 10,20,30,40,50 };
        using const_array_type = decltype(arr_const);

        hud::random_access_iterator<array_type> it(arr, 4);
        const auto mutable_it = std::tuple{
            it.operator->() == &arr[4],
           *it == arr[4],
           (it--).operator->() == &arr[4],
           *it == arr[3],
           (it--).operator->() == &arr[3],
           *it == arr[2],
           (it--).operator->() == &arr[2],
           *it == arr[1],
           (it--).operator->() == &arr[1],
           *it == arr[0]
        };
        hud::random_access_iterator<const_array_type> it_const(arr_const, 4);
        const auto const_it = std::tuple{
            it_const.operator->() == &arr_const[4],
           *it_const == arr_const[4],
           (it_const--).operator->() == &arr_const[4],
           *it_const == arr_const[3],
           (it_const--).operator->() == &arr_const[3],
           *it_const == arr_const[2],
           (it_const--).operator->() == &arr_const[2],
           *it_const == arr_const[1],
           (it_const--).operator->() == &arr_const[1],
           *it_const == arr_const[0]
        };
        return std::tuple{
           mutable_it,
           const_it
        };
    };

    // Non constant
    {
        const auto result = test();

        const auto& mutable_it = std::get<0>(result);
        ASSERT_TRUE(std::get<0>(mutable_it));
        ASSERT_TRUE(std::get<1>(mutable_it));
        ASSERT_TRUE(std::get<2>(mutable_it));
        ASSERT_TRUE(std::get<3>(mutable_it));
        ASSERT_TRUE(std::get<4>(mutable_it));
        ASSERT_TRUE(std::get<5>(mutable_it));
        ASSERT_TRUE(std::get<6>(mutable_it));
        ASSERT_TRUE(std::get<7>(mutable_it));
        ASSERT_TRUE(std::get<8>(mutable_it));
        ASSERT_TRUE(std::get<9>(mutable_it));

        const auto& const_it = std::get<1>(result);
        ASSERT_TRUE(std::get<0>(const_it));
        ASSERT_TRUE(std::get<1>(const_it));
        ASSERT_TRUE(std::get<2>(const_it));
        ASSERT_TRUE(std::get<3>(const_it));
        ASSERT_TRUE(std::get<4>(const_it));
        ASSERT_TRUE(std::get<5>(const_it));
        ASSERT_TRUE(std::get<6>(const_it));
        ASSERT_TRUE(std::get<7>(const_it));
        ASSERT_TRUE(std::get<8>(const_it));
        ASSERT_TRUE(std::get<9>(const_it));
    }

    // Constant
    {
        constexpr auto result = test();

        const auto& mutable_it = std::get<0>(result);
        ASSERT_TRUE(std::get<0>(mutable_it));
        ASSERT_TRUE(std::get<1>(mutable_it));
        ASSERT_TRUE(std::get<2>(mutable_it));
        ASSERT_TRUE(std::get<3>(mutable_it));
        ASSERT_TRUE(std::get<4>(mutable_it));
        ASSERT_TRUE(std::get<5>(mutable_it));
        ASSERT_TRUE(std::get<6>(mutable_it));
        ASSERT_TRUE(std::get<7>(mutable_it));
        ASSERT_TRUE(std::get<8>(mutable_it));
        ASSERT_TRUE(std::get<9>(mutable_it));

        const auto& const_it = std::get<1>(result);
        ASSERT_TRUE(std::get<0>(const_it));
        ASSERT_TRUE(std::get<1>(const_it));
        ASSERT_TRUE(std::get<2>(const_it));
        ASSERT_TRUE(std::get<3>(const_it));
        ASSERT_TRUE(std::get<4>(const_it));
        ASSERT_TRUE(std::get<5>(const_it));
        ASSERT_TRUE(std::get<6>(const_it));
        ASSERT_TRUE(std::get<7>(const_it));
        ASSERT_TRUE(std::get<8>(const_it));
        ASSERT_TRUE(std::get<9>(const_it));
    }
}

GTEST_TEST(random_access_iterator, operator_decrement_assign) {
    const auto test = []() {
        i32 arr[5] = { 10,20,30,40,50 };
        using array_type = decltype(arr);
        const i32 arr_const[5] = { 10,20,30,40,50 };
        using const_array_type = decltype(arr_const);

        hud::random_access_iterator<array_type> it(arr, 4);
        const auto mutable_it = std::tuple{
            it.operator->() == &arr[4],
            *it == arr[4],
            (it -= 1).operator->() == &arr[3],
            *it == arr[3]
        };

        hud::random_access_iterator<const_array_type> it_const(arr_const,4);
        const auto const_it = std::tuple{
              it_const.operator->() == &arr_const[4],
            *it_const == arr_const[4],
            (it_const -= 1).operator->() == &arr_const[3],
            *it_const == arr_const[3]
        };
        return std::tuple{
          mutable_it,
          const_it
        };
    };

    // Non constant
    {
        const auto result = test();

        const auto& mutable_it = std::get<0>(result);
        ASSERT_TRUE(std::get<0>(mutable_it));
        ASSERT_TRUE(std::get<1>(mutable_it));
        ASSERT_TRUE(std::get<2>(mutable_it));
        ASSERT_TRUE(std::get<3>(mutable_it));

        const auto& const_it = std::get<1>(result);
        ASSERT_TRUE(std::get<0>(const_it));
        ASSERT_TRUE(std::get<1>(const_it));
        ASSERT_TRUE(std::get<2>(const_it));
        ASSERT_TRUE(std::get<3>(const_it));
    }

    // Constant
    {
        constexpr auto result = test();

        const auto& mutable_it = std::get<0>(result);
        ASSERT_TRUE(std::get<0>(mutable_it));
        ASSERT_TRUE(std::get<1>(mutable_it));
        ASSERT_TRUE(std::get<2>(mutable_it));
        ASSERT_TRUE(std::get<3>(mutable_it));

        const auto& const_it = std::get<1>(result);
        ASSERT_TRUE(std::get<0>(const_it));
        ASSERT_TRUE(std::get<1>(const_it));
        ASSERT_TRUE(std::get<2>(const_it));
        ASSERT_TRUE(std::get<3>(const_it));
    }
}

GTEST_TEST(random_access_iterator, operator_decrement) {
    const auto test = []() {
        i32 arr[5] = { 10,20,30,40,50 };
        using array_type = decltype(arr);
        const i32 arr_const[5] = { 10,20,30,40,50 };
        using const_array_type = decltype(arr_const);

        hud::random_access_iterator<array_type> it(arr, 4);
        hud::random_access_iterator<array_type> res_it = it - 1;
        const auto mutable_it = std::tuple{
            it.operator->() == &arr[4],
            *it == arr[4],
            res_it.operator->() == &arr[3],
            *res_it == arr[3]
        };

        hud::random_access_iterator<const_array_type> it_const(arr_const, 4);
        hud::random_access_iterator<const_array_type> res_it_const = it_const - 1;
        const auto const_it = std::tuple{
             it_const.operator->() == &arr_const[4],
            *it_const == arr[4],
            res_it_const.operator->() == &arr_const[3],
            *res_it_const == arr_const[3]
        };
        return std::tuple{
          mutable_it,
          const_it
        };
    };

    // Non constant
    {
        const auto result = test();

        const auto& mutable_it = std::get<0>(result);
        ASSERT_TRUE(std::get<0>(mutable_it));
        ASSERT_TRUE(std::get<1>(mutable_it));
        ASSERT_TRUE(std::get<2>(mutable_it));
        ASSERT_TRUE(std::get<3>(mutable_it));

        const auto& const_it = std::get<1>(result);
        ASSERT_TRUE(std::get<0>(const_it));
        ASSERT_TRUE(std::get<1>(const_it));
        ASSERT_TRUE(std::get<2>(const_it));
        ASSERT_TRUE(std::get<3>(const_it));
    }

    // Constant
    {
        constexpr auto result = test();

        const auto& mutable_it = std::get<0>(result);
        ASSERT_TRUE(std::get<0>(mutable_it));
        ASSERT_TRUE(std::get<1>(mutable_it));
        ASSERT_TRUE(std::get<2>(mutable_it));
        ASSERT_TRUE(std::get<3>(mutable_it));

        const auto& const_it = std::get<1>(result);
        ASSERT_TRUE(std::get<0>(const_it));
        ASSERT_TRUE(std::get<1>(const_it));
        ASSERT_TRUE(std::get<2>(const_it));
        ASSERT_TRUE(std::get<3>(const_it));
    }
}

GTEST_TEST(random_access_iterator, equal_operator) {
    const auto test = []() {
        i32 arr[5] = { 10,20,30,40,50 };
        using array_type = decltype(arr);

        hud::random_access_iterator<array_type> it(arr, 4);
        hud::random_access_iterator<array_type> it_same(arr, 4);
        hud::random_access_iterator<array_type> it_not_same(arr, 2);
        return std::tuple{
            it == it_same,
            it == it_not_same,
            it != it_not_same,
            it != it_same
        };
    };

    // Non constant
    {
        const auto result = test();

        ASSERT_TRUE(std::get<0>(result));
        ASSERT_FALSE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_FALSE(std::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();

        ASSERT_TRUE(std::get<0>(result));
        ASSERT_FALSE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_FALSE(std::get<3>(result));
    }
}