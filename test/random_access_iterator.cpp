#include <core/iterators/random_access_iterator.h>

TEST(RandomAccessIterator, check_const_correctness) {


    i32 arr[5] = { 10,20,30,40,50 };
    using ArrType = decltype(arr);
    const i32 arr_const[5] = { 10,20,30,40,50 };
    using ArrConstType = decltype(arr_const);

    RandomAccessIterator<ArrType> it(arr);
    ASSERT_FALSE(hud::is_const_v<decltype(*it)>);

    RandomAccessIterator<ArrConstType> it_const(arr_const);
    ASSERT_TRUE(hud::is_const_v<hud::remove_reference_t<decltype(*it_const)>>);

    RandomAccessIterator<ArrConstType> it_const_2(arr);
    ASSERT_TRUE(hud::is_const_v<hud::remove_reference_t<decltype(*it_const_2)>>);
}


TEST(RandomAccessIterator, constructor) {


    const auto test = []() {
        i32 arr[5] = { 10,20,30,40,50 };
        using ArrType = decltype(arr);
        const i32 arr_const[5] = { 10,20,30,40,50 };
        using ArrConstType = decltype(arr_const);

        RandomAccessIterator<ArrType> it(arr);
        RandomAccessIterator<ArrConstType> it_const(arr_const);
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

TEST(RandomAccessIterator, operator_arrow) {


    const auto test = []() {
        i32 arr[5] = { 10,20,30,40,50 };
        using ArrType = decltype(arr);
        const i32 arr_const[5] = { 10,20,30,40,50 };
        using ArrConstType = decltype(arr_const);

        return std::tuple{
            RandomAccessIterator<ArrType>(arr).operator->() == &arr[0], // Position is 0 (first)
            RandomAccessIterator<ArrType>(arr, 4).operator->() == &arr[4], // Position is 4 (last)
            RandomAccessIterator<ArrConstType>(arr_const).operator->() == &arr_const[0], // Position is 0 (first)
            RandomAccessIterator<ArrConstType>(arr_const, 4).operator->() == &arr_const[4] // Position is 4 (last) 
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

TEST(RandomAccessIterator, operator_dereference) {


    const auto test = []() {
        i32 arr[5] = { 10,20,30,40,50 };
        using ArrType = decltype(arr);
        const i32 arr_const[5] = { 10,20,30,40,50 };
        using ArrConstType = decltype(arr_const);

        return std::tuple{
            *hud::RandomAccessIterator<ArrType>(arr) == arr[0], // Position is 0 (first)
            *hud::RandomAccessIterator<ArrType>(arr, 4) == arr[4], // Position is 4 (last)
            *hud::RandomAccessIterator<ArrConstType>(arr_const) == arr_const[0], // Position is 0 (first)
            *hud::RandomAccessIterator<ArrConstType>(arr_const, 4) == arr_const[4] // Position is 4 (last) 
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

TEST(RandomAccessIterator, operator_pre_increment) {


    const auto test = []() {
        i32 arr[5] = { 10,20,30,40,50 };
        using ArrType = decltype(arr);
        const i32 arr_const[5] = { 10,20,30,40,50 };
        using ArrConstType = decltype(arr_const);

        RandomAccessIterator<ArrType> it(arr);
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
        RandomAccessIterator<ArrConstType> it_const(arr_const);
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


TEST(RandomAccessIterator, operator_post_increment) {


    const auto test = []() {
        i32 arr[5] = { 10,20,30,40,50 };
        using ArrType = decltype(arr);
        const i32 arr_const[5] = { 10,20,30,40,50 };
        using ArrConstType = decltype(arr_const);

        RandomAccessIterator<ArrType> it(arr);
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
        RandomAccessIterator<ArrConstType> it_const(arr_const);
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

TEST(RandomAccessIterator, operator_increment_assign) {


    const auto test = []() {
        i32 arr[5] = { 10,20,30,40,50 };
        using ArrType = decltype(arr);
        const i32 arr_const[5] = { 10,20,30,40,50 };
        using ArrConstType = decltype(arr_const);

        RandomAccessIterator<ArrType> it(arr);
        const auto mutable_it = std::tuple{
            it.operator->() == &arr[0],
            *it == arr[0],
            (it += 1).operator->() == &arr[1],
            *it == arr[1]
        };

        RandomAccessIterator<ArrConstType> it_const(arr_const);
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

TEST(RandomAccessIterator, operator_increment) {


    const auto test = []() {
        i32 arr[5] = { 10,20,30,40,50 };
        using ArrType = decltype(arr);
        const i32 arr_const[5] = { 10,20,30,40,50 };
        using ArrConstType = decltype(arr_const);

        RandomAccessIterator<ArrType> it(arr);
        RandomAccessIterator<ArrType> res_it = it + 1;
        const auto mutable_it = std::tuple{
            it.operator->() == &arr[0],
            *it == arr[0],
            res_it.operator->() == &arr[1],
            *res_it == arr[1]
        };

        RandomAccessIterator<ArrConstType> it_const(arr_const);
        RandomAccessIterator<ArrConstType> res_it_const = it_const + 1;
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


TEST(RandomAccessIterator, operator_pre_decrement) {


    const auto test = []() {
        i32 arr[5] = { 10,20,30,40,50 };
        using ArrType = decltype(arr);
        const i32 arr_const[5] = { 10,20,30,40,50 };
        using ArrConstType = decltype(arr_const);

        RandomAccessIterator<ArrType> it(arr, 4);
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
        RandomAccessIterator<ArrConstType> it_const(arr_const, 4);
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


TEST(RandomAccessIterator, operator_post_decrement) {


    const auto test = []() {
        i32 arr[5] = { 10,20,30,40,50 };
        using ArrType = decltype(arr);
        const i32 arr_const[5] = { 10,20,30,40,50 };
        using ArrConstType = decltype(arr_const);

        RandomAccessIterator<ArrType> it(arr, 4);
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
        RandomAccessIterator<ArrConstType> it_const(arr_const, 4);
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

TEST(RandomAccessIterator, operator_decrement_assign) {


    const auto test = []() {
        i32 arr[5] = { 10,20,30,40,50 };
        using ArrType = decltype(arr);
        const i32 arr_const[5] = { 10,20,30,40,50 };
        using ArrConstType = decltype(arr_const);

        RandomAccessIterator<ArrType> it(arr, 4);
        const auto mutable_it = std::tuple{
            it.operator->() == &arr[4],
            *it == arr[4],
            (it -= 1).operator->() == &arr[3],
            *it == arr[3]
        };

        RandomAccessIterator<ArrConstType> it_const(arr_const,4);
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

TEST(RandomAccessIterator, operator_decrement) {


    const auto test = []() {
        i32 arr[5] = { 10,20,30,40,50 };
        using ArrType = decltype(arr);
        const i32 arr_const[5] = { 10,20,30,40,50 };
        using ArrConstType = decltype(arr_const);

        RandomAccessIterator<ArrType> it(arr, 4);
        RandomAccessIterator<ArrType> res_it = it - 1;
        const auto mutable_it = std::tuple{
            it.operator->() == &arr[4],
            *it == arr[4],
            res_it.operator->() == &arr[3],
            *res_it == arr[3]
        };

        RandomAccessIterator<ArrConstType> it_const(arr_const, 4);
        RandomAccessIterator<ArrConstType> res_it_const = it_const - 1;
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

TEST(RandomAccessIterator, equal_operator)
{


    const auto test = []() {
        i32 arr[5] = { 10,20,30,40,50 };
        using ArrType = decltype(arr);

        RandomAccessIterator<ArrType> it(arr, 4);
        RandomAccessIterator<ArrType> it_same(arr, 4);
        RandomAccessIterator<ArrType> it_not_same(arr, 2);
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