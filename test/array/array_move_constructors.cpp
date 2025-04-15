#include <core/containers/array.h>
#include "../misc/allocator_watcher.h"

GTEST_TEST(array, move_construct_bitwise_copy_constructible_same_type_same_allocator)
{

    /** The array we move for test, we allocate also extra memory to test if we really move the count(), not the max_count() elements */
    using type = i32;
    using AllocatorType = hud_test::allocator_watcher<alignof(type)>;

    // Ensure we test with different allocator
    static_assert(hud::is_bitwise_move_constructible_v<type>);
    using MovedType = hud::array<type, AllocatorType>;

    // Test default allocator
    {
        auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
        {
            MovedType moved(initializer, copied_extra);

            // Copy the array
            hud::array<type, AllocatorType> move(std::move(moved));

            // Ensure we moved all datas in order
            bool all_values_moved = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                if (move[index] != static_cast<type>(index))
                {
                    all_values_moved = false;
                    break;
                }
            }

            return std::tuple {
                // Ensure we copy all datas in order
                move.data() != nullptr,
                move.count(),
                move.max_count(),
                all_values_moved,

                // Ensure the copy data is not the same memory of the copied data
                moved.data() != move.data(),

                // Ensure we are allocating only one time
                move.allocator().allocation_count(),
                move.allocator().free_count(),
                move.allocator().move_assign_count(),
                move.allocator().move_construct_count(),
                moved.allocator().allocation_count(),
                moved.allocator().free_count(),
                moved.allocator().move_assign_count(),
                moved.allocator().move_construct_count()
            };
        };

        // Non constant
        {
            const auto result = test_default_allocator({0, 1, 2, 3}, 1u);
            // Ensure we copy all datas in order
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), 4u);
            hud_assert_eq(std::get<2>(result), 4u + 1u);
            hud_assert_true(std::get<3>(result));

            // Ensure the copy data is not the same memory of the copied data
            hud_assert_true(std::get<4>(result));

            // Ensure we are allocating only one time
            // Bitwise copyable with same type should just stole the pointer without reallocating
            // If allocator are not the same, a reallocation is done to allocate with the correct allocator
            hud_assert_eq(std::get<5>(result), 0u);
            hud_assert_eq(std::get<6>(result), 0u);
            hud_assert_eq(std::get<7>(result), 0u);
            hud_assert_eq(std::get<8>(result), 1u);
            hud_assert_eq(std::get<9>(result), 1u);
            hud_assert_eq(std::get<10>(result), 0u);
            hud_assert_eq(std::get<11>(result), 0u);
            hud_assert_eq(std::get<12>(result), 0u);
        }

        // Constant
        {
            constexpr auto result = test_default_allocator({0, 1, 2, 3}, 1u);
            // Ensure we copy all datas in order
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), 4u);
            hud_assert_eq(std::get<2>(result), 4u + 1u);
            hud_assert_true(std::get<3>(result));

            // Ensure the copy data is not the same memory of the copied data
            hud_assert_true(std::get<4>(result));

            // Ensure we are allocating only one time
            // Bitwise copyable with same type should just stole the pointer without reallocating
            // If allocator are not the same, a reallocation is done to allocate with the correct allocator
            hud_assert_eq(std::get<5>(result), 0u);
            hud_assert_eq(std::get<6>(result), 0u);
            hud_assert_eq(std::get<7>(result), 0u);
            hud_assert_eq(std::get<8>(result), 1u);
            hud_assert_eq(std::get<9>(result), 1u);
            hud_assert_eq(std::get<10>(result), 0u);
            hud_assert_eq(std::get<11>(result), 0u);
            hud_assert_eq(std::get<12>(result), 0u);
        }
    }

    // Test with allocator
    {
        auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
        {
            MovedType moved(initializer, copied_extra);

            // Copy the array
            hud::array<type, AllocatorType> move(std::move(moved), AllocatorType {});

            // Ensure we moved all datas in order
            bool all_values_moved = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                if (move[index] != static_cast<type>(index))
                {
                    all_values_moved = false;
                    break;
                }
            }

            return std::tuple {
                // Ensure we copy all datas in order
                move.data() != nullptr,
                move.count(),
                move.max_count(),
                all_values_moved,

                // Ensure the copy data is not the same memory of the copied data
                moved.data() != move.data(),

                // Ensure we are allocating only one time
                move.allocator().allocation_count(),
                move.allocator().free_count(),
                move.allocator().move_assign_count(),
                move.allocator().move_construct_count(),
                moved.allocator().allocation_count(),
                moved.allocator().free_count(),
                moved.allocator().move_assign_count(),
                moved.allocator().move_construct_count()
            };
        };

        // Non constant
        {
            const auto result = test_default_allocator({0, 1, 2, 3}, 1u);
            // Ensure we copy all datas in order
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), 4u);
            hud_assert_eq(std::get<2>(result), 4u + 1u);
            hud_assert_true(std::get<3>(result));

            // Ensure the copy data is not the same memory of the copied data
            hud_assert_true(std::get<4>(result));

            // Ensure we are allocating only one time
            hud_assert_eq(std::get<5>(result), 1u);
            hud_assert_eq(std::get<6>(result), 0u);
            hud_assert_eq(std::get<7>(result), 0u);
            hud_assert_eq(std::get<8>(result), 0u);
            hud_assert_eq(std::get<9>(result), 1u);
            hud_assert_eq(std::get<10>(result), 1u);
            hud_assert_eq(std::get<11>(result), 0u);
            hud_assert_eq(std::get<12>(result), 0u);
        }

        // Constant
        {
            constexpr auto result = test_default_allocator({0, 1, 2, 3}, 1u);
            // Ensure we copy all datas in order
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), 4u);
            hud_assert_eq(std::get<2>(result), 4u + 1u);
            hud_assert_true(std::get<3>(result));

            // Ensure the copy data is not the same memory of the copied data
            hud_assert_true(std::get<4>(result));

            // Ensure we are allocating only one time
            hud_assert_eq(std::get<5>(result), 1u);
            hud_assert_eq(std::get<6>(result), 0u);
            hud_assert_eq(std::get<7>(result), 0u);
            hud_assert_eq(std::get<8>(result), 0u);
            hud_assert_eq(std::get<9>(result), 1u);
            hud_assert_eq(std::get<10>(result), 1u);
            hud_assert_eq(std::get<11>(result), 0u);
            hud_assert_eq(std::get<12>(result), 0u);
        }
    }

    // Test with extra
    {
        hud_test::for_each_value<std::make_integer_sequence<usize, 5>>()(
            []<usize extra>()
            {
                // Test default allocator
                {

                    auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
                    {
                        MovedType moved(initializer, copied_extra);

                        // Copy the array
                        hud::array<type, AllocatorType> move(std::move(moved), extra);

                        // Ensure we moved all datas in order
                        bool all_values_moved = true;
                        for (usize index = 0; index < initializer.size(); index++)
                        {
                            if (move[index] != static_cast<type>(index))
                            {
                                all_values_moved = false;
                                break;
                            }
                        }

                        return std::tuple {
                            // Ensure we copy all datas in order
                            move.data() != nullptr,
                            move.count(),
                            move.max_count(),
                            all_values_moved,

                            // Ensure the copy data is not the same memory of the copied data
                            (void *)moved.data() != (void *)move.data(),

                            // Ensure we are allocating only one time
                            move.allocator().allocation_count(),
                            move.allocator().free_count(),
                            move.allocator().move_assign_count(),
                            move.allocator().move_construct_count(),
                            moved.allocator().allocation_count(),
                            moved.allocator().free_count(),
                            moved.allocator().move_assign_count(),
                            moved.allocator().move_construct_count()
                        };
                    };

                    // Non constant
                    {
                        const auto result = test_default_allocator({0, 1, 2, 3}, 1u);
                        // Ensure we copy all datas in order
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), 4u);
                        hud_assert_eq(std::get<2>(result), 4u + 1u + extra);
                        hud_assert_true(std::get<3>(result));

                        // Ensure the copy data is not the same memory of the copied data
                        hud_assert_true(std::get<4>(result));

                        // Ensure we allocate and free the moved array and just allocate the new array once
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                        hud_assert_eq(std::get<7>(result), 0u);
                        hud_assert_eq(std::get<8>(result), 0u);
                        hud_assert_eq(std::get<9>(result), 1u);
                        hud_assert_eq(std::get<10>(result), 1u);
                        hud_assert_eq(std::get<11>(result), 0u);
                        hud_assert_eq(std::get<12>(result), 0u);
                    }

                    // Constant
                    {
                        constexpr auto result = test_default_allocator({0, 1, 2, 3}, 1u);
                        // Ensure we copy all datas in order
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), 4u);
                        hud_assert_eq(std::get<2>(result), 4u + 1u + extra);
                        hud_assert_true(std::get<3>(result));

                        // Ensure the copy data is not the same memory of the copied data
                        hud_assert_true(std::get<4>(result));

                        // Ensure we allocate and free the moved array and just allocate the new array once
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                        hud_assert_eq(std::get<7>(result), 0u);
                        hud_assert_eq(std::get<8>(result), 0u);
                        hud_assert_eq(std::get<9>(result), 1u);
                        hud_assert_eq(std::get<10>(result), 1u);
                        hud_assert_eq(std::get<11>(result), 0u);
                        hud_assert_eq(std::get<12>(result), 0u);
                    }
                }

                // Test with allocator
                {
                    auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
                    {
                        MovedType moved(initializer, copied_extra);

                        // Copy the array
                        hud::array<type, AllocatorType> move(std::move(moved), extra, AllocatorType {});

                        // Ensure we moved all datas in order
                        bool all_values_moved = true;
                        for (usize index = 0; index < initializer.size(); index++)
                        {
                            if (move[index] != static_cast<type>(index))
                            {
                                all_values_moved = false;
                                break;
                            }
                        }

                        return std::tuple {
                            // Ensure we copy all datas in order
                            move.data() != nullptr,
                            move.count(),
                            move.max_count(),
                            all_values_moved,

                            // Ensure the copy data is not the same memory of the copied data
                            (void *)moved.data() != (void *)move.data(),

                            // Ensure we are allocating only one time
                            move.allocator().allocation_count(),
                            move.allocator().free_count(),
                            move.allocator().move_assign_count(),
                            move.allocator().move_construct_count(),
                            moved.allocator().allocation_count(),
                            moved.allocator().free_count(),
                            moved.allocator().move_assign_count(),
                            moved.allocator().move_construct_count()
                        };
                    };

                    // Non constant
                    {
                        const auto result = test_default_allocator({0, 1, 2, 3}, 1u);
                        // Ensure we copy all datas in order
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), 4u);
                        hud_assert_eq(std::get<2>(result), 4u + 1u + extra);
                        hud_assert_true(std::get<3>(result));

                        // Ensure the copy data is not the same memory of the copied data
                        hud_assert_true(std::get<4>(result));

                        // Ensure we allocate and free the moved array and just allocate the new array once
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                        hud_assert_eq(std::get<7>(result), 0u);
                        hud_assert_eq(std::get<8>(result), 0u);
                        hud_assert_eq(std::get<9>(result), 1u);
                        hud_assert_eq(std::get<10>(result), 1u);
                        hud_assert_eq(std::get<11>(result), 0u);
                        hud_assert_eq(std::get<12>(result), 0u);
                    }

                    // Constant
                    {
                        constexpr auto result = test_default_allocator({0, 1, 2, 3}, 1u);
                        // Ensure we copy all datas in order
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), 4u);
                        hud_assert_eq(std::get<2>(result), 4u + 1u + extra);
                        hud_assert_true(std::get<3>(result));

                        // Ensure the copy data is not the same memory of the copied data
                        hud_assert_true(std::get<4>(result));

                        // Ensure we allocate and free the moved array and just allocate the new array once
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                        hud_assert_eq(std::get<7>(result), 0u);
                        hud_assert_eq(std::get<8>(result), 0u);
                        hud_assert_eq(std::get<9>(result), 1u);
                        hud_assert_eq(std::get<10>(result), 1u);
                        hud_assert_eq(std::get<11>(result), 0u);
                        hud_assert_eq(std::get<12>(result), 0u);
                    }
                }
            }
        );
    }
}

GTEST_TEST(array, move_construct_bitwise_copy_constructible_same_type_different_allocator)
{

    /** The array we move for test, we allocate also extra memory to test if we really move the count(), not the max_count() elements */
    using type = i32;
    using AllocatorType = hud_test::allocator_watcher<alignof(type)>;
    using AllocatorType2 = hud_test::allocator_watcher_2<alignof(type)>;

    // Ensure we test with different allocator
    static_assert(!std::is_same_v<AllocatorType, AllocatorType2>);
    static_assert(hud::is_bitwise_move_constructible_v<type>);
    using MovedType = hud::array<type, AllocatorType>;

    // Test default allocator
    {

        auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
        {
            MovedType moved(initializer, copied_extra);

            // Copy the array
            hud::array<type, AllocatorType2> move(std::move(moved));

            // Ensure we moved all datas in order
            bool all_values_moved = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                if (move[index] != static_cast<type>(index))
                {
                    all_values_moved = false;
                    break;
                }
            }

            return std::tuple {
                // Ensure we copy all datas in order
                move.data() != nullptr,
                move.count(),
                move.max_count(),
                all_values_moved,

                // Ensure the copy data is not the same memory of the copied data
                moved.data() != move.data(),

                // Ensure we are allocating only one time
                move.allocator().allocation_count(),
                move.allocator().free_count(),
                move.allocator().move_assign_count(),
                move.allocator().move_construct_count(),
                moved.allocator().allocation_count(),
                moved.allocator().free_count(),
                moved.allocator().move_assign_count(),
                moved.allocator().move_construct_count()
            };
        };

        // Non constant
        {
            const auto result = test_default_allocator({0, 1, 2, 3}, 1u);
            // Ensure we copy all datas in order
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), 4u);
            hud_assert_eq(std::get<2>(result), 4u + 1u);
            hud_assert_true(std::get<3>(result));

            // Ensure the copy data is not the same memory of the copied data
            hud_assert_true(std::get<4>(result));

            // Ensure we are allocating only one time
            // Bitwise copyable with same type should just stole the pointer without reallocating
            // If allocator are not the same, a reallocation is done to allocate with the correct allocator
            hud_assert_eq(std::get<5>(result), 1u);
            hud_assert_eq(std::get<6>(result), 0u);
            hud_assert_eq(std::get<7>(result), 0u);
            hud_assert_eq(std::get<8>(result), 0u);
            hud_assert_eq(std::get<9>(result), 1u);
            hud_assert_eq(std::get<10>(result), 1u);
            hud_assert_eq(std::get<11>(result), 0u);
            hud_assert_eq(std::get<12>(result), 0u);
        }

        // Constant
        {
            constexpr auto result = test_default_allocator({0, 1, 2, 3}, 1u);
            // Ensure we copy all datas in order
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), 4u);
            hud_assert_eq(std::get<2>(result), 4u + 1u);
            hud_assert_true(std::get<3>(result));

            // Ensure the copy data is not the same memory of the copied data
            hud_assert_true(std::get<4>(result));

            // Ensure we are allocating only one time
            // Bitwise copyable with same type should just stole the pointer without reallocating
            // If allocator are not the same, a reallocation is done to allocate with the correct allocator
            hud_assert_eq(std::get<5>(result), 1u);
            hud_assert_eq(std::get<6>(result), 0u);
            hud_assert_eq(std::get<7>(result), 0u);
            hud_assert_eq(std::get<8>(result), 0u);
            hud_assert_eq(std::get<9>(result), 1u);
            hud_assert_eq(std::get<10>(result), 1u);
            hud_assert_eq(std::get<11>(result), 0u);
            hud_assert_eq(std::get<12>(result), 0u);
        }
    }

    // Test with allocator
    {
        auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
        {
            MovedType moved(initializer, copied_extra);

            // Copy the array
            hud::array<type, AllocatorType2> move(std::move(moved), AllocatorType2 {});

            // Ensure we moved all datas in order
            bool all_values_moved = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                if (move[index] != static_cast<type>(index))
                {
                    all_values_moved = false;
                    break;
                }
            }

            return std::tuple {
                // Ensure we copy all datas in order
                move.data() != nullptr,
                move.count(),
                move.max_count(),
                all_values_moved,

                // Ensure the copy data is not the same memory of the copied data
                moved.data() != move.data(),

                // Ensure we are allocating only one time
                move.allocator().allocation_count(),
                move.allocator().free_count(),
                move.allocator().move_assign_count(),
                move.allocator().move_construct_count(),
                moved.allocator().allocation_count(),
                moved.allocator().free_count(),
                moved.allocator().move_assign_count(),
                moved.allocator().move_construct_count()
            };
        };

        // Non constant
        {
            const auto result = test_default_allocator({0, 1, 2, 3}, 1u);
            // Ensure we copy all datas in order
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), 4u);
            hud_assert_eq(std::get<2>(result), 4u + 1u);
            hud_assert_true(std::get<3>(result));

            // Ensure the copy data is not the same memory of the copied data
            hud_assert_true(std::get<4>(result));

            // Ensure we are allocating only one time
            hud_assert_eq(std::get<5>(result), 1u);
            hud_assert_eq(std::get<6>(result), 0u);
            hud_assert_eq(std::get<7>(result), 0u);
            hud_assert_eq(std::get<8>(result), 0u);
            hud_assert_eq(std::get<9>(result), 1u);
            hud_assert_eq(std::get<10>(result), 1u);
            hud_assert_eq(std::get<11>(result), 0u);
            hud_assert_eq(std::get<12>(result), 0u);
        }

        // Constant
        {
            constexpr auto result = test_default_allocator({0, 1, 2, 3}, 1u);
            // Ensure we copy all datas in order
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), 4u);
            hud_assert_eq(std::get<2>(result), 4u + 1u);
            hud_assert_true(std::get<3>(result));

            // Ensure the copy data is not the same memory of the copied data
            hud_assert_true(std::get<4>(result));

            // Ensure we are allocating only one time
            hud_assert_eq(std::get<5>(result), 1u);
            hud_assert_eq(std::get<6>(result), 0u);
            hud_assert_eq(std::get<7>(result), 0u);
            hud_assert_eq(std::get<8>(result), 0u);
            hud_assert_eq(std::get<9>(result), 1u);
            hud_assert_eq(std::get<10>(result), 1u);
            hud_assert_eq(std::get<11>(result), 0u);
            hud_assert_eq(std::get<12>(result), 0u);
        }
    }

    // Test with extra
    {
        hud_test::for_each_value<std::make_integer_sequence<usize, 5>>()(
            []<usize extra>()
            {
                // Test default allocator
                {

                    auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
                    {
                        MovedType moved(initializer, copied_extra);

                        // Copy the array
                        hud::array<type, AllocatorType2> move(std::move(moved), extra);

                        // Ensure we moved all datas in order
                        bool all_values_moved = true;
                        for (usize index = 0; index < initializer.size(); index++)
                        {
                            if (move[index] != static_cast<type>(index))
                            {
                                all_values_moved = false;
                                break;
                            }
                        }

                        return std::tuple {
                            // Ensure we copy all datas in order
                            move.data() != nullptr,
                            move.count(),
                            move.max_count(),
                            all_values_moved,

                            // Ensure the copy data is not the same memory of the copied data
                            (void *)moved.data() != (void *)move.data(),

                            // Ensure we are allocating only one time
                            move.allocator().allocation_count(),
                            move.allocator().free_count(),
                            move.allocator().move_assign_count(),
                            move.allocator().move_construct_count(),
                            moved.allocator().allocation_count(),
                            moved.allocator().free_count(),
                            moved.allocator().move_assign_count(),
                            moved.allocator().move_construct_count()
                        };
                    };

                    // Non constant
                    {
                        const auto result = test_default_allocator({0, 1, 2, 3}, 1u);
                        // Ensure we copy all datas in order
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), 4u);
                        hud_assert_eq(std::get<2>(result), 4u + 1u + extra);
                        hud_assert_true(std::get<3>(result));

                        // Ensure the copy data is not the same memory of the copied data
                        hud_assert_true(std::get<4>(result));

                        // Ensure we allocate and free the moved array and just allocate the new array once
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                        hud_assert_eq(std::get<7>(result), 0u);
                        hud_assert_eq(std::get<8>(result), 0u);
                        hud_assert_eq(std::get<9>(result), 1u);
                        hud_assert_eq(std::get<10>(result), 1u);
                        hud_assert_eq(std::get<11>(result), 0u);
                        hud_assert_eq(std::get<12>(result), 0u);
                    }

                    // Constant
                    {
                        constexpr auto result = test_default_allocator({0, 1, 2, 3}, 1u);
                        // Ensure we copy all datas in order
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), 4u);
                        hud_assert_eq(std::get<2>(result), 4u + 1u + extra);
                        hud_assert_true(std::get<3>(result));

                        // Ensure the copy data is not the same memory of the copied data
                        hud_assert_true(std::get<4>(result));

                        // Ensure we allocate and free the moved array and just allocate the new array once
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                        hud_assert_eq(std::get<7>(result), 0u);
                        hud_assert_eq(std::get<8>(result), 0u);
                        hud_assert_eq(std::get<9>(result), 1u);
                        hud_assert_eq(std::get<10>(result), 1u);
                        hud_assert_eq(std::get<11>(result), 0u);
                        hud_assert_eq(std::get<12>(result), 0u);
                    }
                }

                // Test with allocator
                {
                    auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
                    {
                        MovedType moved(initializer, copied_extra);

                        // Copy the array
                        hud::array<type, AllocatorType2> move(std::move(moved), extra, AllocatorType2 {});

                        // Ensure we moved all datas in order
                        bool all_values_moved = true;
                        for (usize index = 0; index < initializer.size(); index++)
                        {
                            if (move[index] != static_cast<type>(index))
                            {
                                all_values_moved = false;
                                break;
                            }
                        }

                        return std::tuple {
                            // Ensure we copy all datas in order
                            move.data() != nullptr,
                            move.count(),
                            move.max_count(),
                            all_values_moved,

                            // Ensure the copy data is not the same memory of the copied data
                            (void *)moved.data() != (void *)move.data(),

                            // Ensure we are allocating only one time
                            move.allocator().allocation_count(),
                            move.allocator().free_count(),
                            move.allocator().move_assign_count(),
                            move.allocator().move_construct_count(),
                            moved.allocator().allocation_count(),
                            moved.allocator().free_count(),
                            moved.allocator().move_assign_count(),
                            moved.allocator().move_construct_count()
                        };
                    };

                    // Non constant
                    {
                        const auto result = test_default_allocator({0, 1, 2, 3}, 1u);
                        // Ensure we copy all datas in order
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), 4u);
                        hud_assert_eq(std::get<2>(result), 4u + 1u + extra);
                        hud_assert_true(std::get<3>(result));

                        // Ensure the copy data is not the same memory of the copied data
                        hud_assert_true(std::get<4>(result));

                        // Ensure we allocate and free the moved array and just allocate the new array once
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                        hud_assert_eq(std::get<7>(result), 0u);
                        hud_assert_eq(std::get<8>(result), 0u);
                        hud_assert_eq(std::get<9>(result), 1u);
                        hud_assert_eq(std::get<10>(result), 1u);
                        hud_assert_eq(std::get<11>(result), 0u);
                        hud_assert_eq(std::get<12>(result), 0u);
                    }

                    // Constant
                    {
                        constexpr auto result = test_default_allocator({0, 1, 2, 3}, 1u);
                        // Ensure we copy all datas in order
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), 4u);
                        hud_assert_eq(std::get<2>(result), 4u + 1u + extra);
                        hud_assert_true(std::get<3>(result));

                        // Ensure the copy data is not the same memory of the copied data
                        hud_assert_true(std::get<4>(result));

                        // Ensure we allocate and free the moved array and just allocate the new array once
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                        hud_assert_eq(std::get<7>(result), 0u);
                        hud_assert_eq(std::get<8>(result), 0u);
                        hud_assert_eq(std::get<9>(result), 1u);
                        hud_assert_eq(std::get<10>(result), 1u);
                        hud_assert_eq(std::get<11>(result), 0u);
                        hud_assert_eq(std::get<12>(result), 0u);
                    }
                }
            }
        );
    }
}

GTEST_TEST(array, move_construct_bitwise_move_constructible_different_type_same_allocator)
{

    /** The array we move for test, we allocate also extra memory to test if we really move the count(), not the max_count() elements */
    using Type1 = i32;
    using Type2 = u32;
    using AllocatorType = hud_test::allocator_watcher<alignof(Type1)>;

    // Ensure we test with different allocator
    static_assert(!std::is_same_v<Type1, Type2>);
    static_assert(hud::is_bitwise_move_constructible_v<Type2, Type1>);

    using MovedType = hud::array<Type1, AllocatorType>;

    // Test default allocator
    {
        auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
        {
            MovedType moved(initializer, copied_extra);

            // Copy the array
            hud::array<Type2, AllocatorType> move(std::move(moved));

            // Ensure we moved all datas in order
            bool all_values_moved = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                if (move[index] != index)
                {
                    all_values_moved = false;
                    break;
                }
            }

            return std::tuple {
                // Ensure we copy all datas in order
                move.data() != nullptr,
                move.count(),
                move.max_count(),
                all_values_moved,

                // Ensure the copy data is not the same memory of the copied data
                (void *)moved.data() != (void *)move.data(),

                // Ensure we are allocating only one time
                move.allocator().allocation_count(),
                move.allocator().free_count(),
                move.allocator().move_assign_count(),
                move.allocator().move_construct_count(),
                moved.allocator().allocation_count(),
                moved.allocator().free_count(),
                moved.allocator().move_assign_count(),
                moved.allocator().move_construct_count()
            };
        };

        // Non constant
        {
            const auto result = test_default_allocator({0, 1, 2, 3}, 1u);
            // Ensure we copy all datas in order
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), 4u);
            hud_assert_eq(std::get<2>(result), 4u + 1u);
            hud_assert_true(std::get<3>(result));

            // Ensure the copy data is not the same memory of the copied data
            hud_assert_true(std::get<4>(result));

            // Ensure we are allocating only one time of not
            // Bitwise moveable with same type should just stole the pointer without reallocating
            // If allocator are not the same, a reallocation is done to allocate with the correct allocator
            hud_assert_eq(std::get<5>(result), 0u);
            hud_assert_eq(std::get<6>(result), 0u);
            hud_assert_eq(std::get<7>(result), 0u);
            hud_assert_eq(std::get<8>(result), 1u);
            hud_assert_eq(std::get<9>(result), 1u);
            hud_assert_eq(std::get<10>(result), 0u);
            hud_assert_eq(std::get<11>(result), 0u);
            hud_assert_eq(std::get<12>(result), 0u);
        }

        // Constant
        {
            constexpr auto result = test_default_allocator({0, 1, 2, 3}, 1u);
            // Ensure we copy all datas in order
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), 4u);
            hud_assert_eq(std::get<2>(result), 4u + 1u);
            hud_assert_true(std::get<3>(result));

            // Ensure the copy data is not the same memory of the copied data
            hud_assert_true(std::get<4>(result));

            // Ensure we are allocating only one time of not
            // We move bitwise move constructible of different type, we can't just still the buffer
            // ( Can't reinterpret_cast the pointer in constant evaluated fonction) so we have an allocation , move and free, always!
            // Allocator is move only if allocator are the same
            hud_assert_eq(std::get<5>(result), 1u);
            hud_assert_eq(std::get<6>(result), 0u);
            hud_assert_eq(std::get<7>(result), 0u);
            hud_assert_eq(std::get<8>(result), 1u);
            hud_assert_eq(std::get<9>(result), 1u);
            hud_assert_eq(std::get<10>(result), 1u);
            hud_assert_eq(std::get<11>(result), 0u);
            hud_assert_eq(std::get<12>(result), 0u);
        }
    }

    // Test with allocator
    {
        auto test_with_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
        {
            MovedType moved(initializer, copied_extra);

            // Copy the array
            hud::array<Type2, AllocatorType> move(std::move(moved), AllocatorType {});

            // Ensure we moved all datas in order
            bool all_values_moved = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                if (move[index] != index)
                {
                    all_values_moved = false;
                    break;
                }
            }

            return std::tuple {
                // Ensure we copy all datas in order
                move.data() != nullptr,
                move.count(),
                move.max_count(),
                all_values_moved,

                // Ensure the copy data is not the same memory of the copied data
                (void *)moved.data() != (void *)move.data(),

                // Ensure we are allocating only one time
                move.allocator().allocation_count(),
                move.allocator().free_count(),
                move.allocator().move_assign_count(),
                move.allocator().move_construct_count(),
                moved.allocator().allocation_count(),
                moved.allocator().free_count(),
                moved.allocator().move_assign_count(),
                moved.allocator().move_construct_count()
            };
        };

        // Non constant
        {
            const auto result = test_with_allocator({0, 1, 2, 3}, 1u);
            // Ensure we copy all datas in order
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), 4u);
            hud_assert_eq(std::get<2>(result), 4u + 1u);
            hud_assert_true(std::get<3>(result));

            // Ensure the copy data is not the same memory of the copied data
            hud_assert_true(std::get<4>(result));

            // Ensure we are allocating only one time of not
            // Providing an allocator force the realloation to be done with this allocator
            hud_assert_eq(std::get<5>(result), 1u);
            hud_assert_eq(std::get<6>(result), 0u);
            hud_assert_eq(std::get<7>(result), 0u);
            hud_assert_eq(std::get<8>(result), 0u);
            hud_assert_eq(std::get<9>(result), 1u);
            hud_assert_eq(std::get<10>(result), 1u);
            hud_assert_eq(std::get<11>(result), 0u);
            hud_assert_eq(std::get<12>(result), 0u);
        }

        // Constant
        {
            constexpr auto result = test_with_allocator({0, 1, 2, 3}, 1u);
            // Ensure we copy all datas in order
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), 4u);
            hud_assert_eq(std::get<2>(result), 4u + 1u);
            hud_assert_true(std::get<3>(result));

            // Ensure the copy data is not the same memory of the copied data
            hud_assert_true(std::get<4>(result));

            // Ensure we are allocating only one time of not
            // Providing an allocator force the realloation to be done with this allocator
            hud_assert_eq(std::get<5>(result), 1u);
            hud_assert_eq(std::get<6>(result), 0u);
            hud_assert_eq(std::get<7>(result), 0u);
            hud_assert_eq(std::get<8>(result), 0u);
            hud_assert_eq(std::get<9>(result), 1u);
            hud_assert_eq(std::get<10>(result), 1u);
            hud_assert_eq(std::get<11>(result), 0u);
            hud_assert_eq(std::get<12>(result), 0u);
        }
    }

    // Test with extra
    {
        hud_test::for_each_value<std::make_integer_sequence<usize, 5>>()(
            []<usize extra>()
            {
                // Test default allocator
                {
                    auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
                    {
                        MovedType moved(initializer, copied_extra);

                        // Copy the array
                        hud::array<Type2, AllocatorType> move(std::move(moved), extra);

                        // Ensure we moved all datas in order
                        bool all_values_moved = true;
                        for (usize index = 0; index < initializer.size(); index++)
                        {
                            if (move[index] != index)
                            {
                                all_values_moved = false;
                                break;
                            }
                        }

                        return std::tuple {
                            // Ensure we copy all datas in order
                            move.data() != nullptr,
                            move.count(),
                            move.max_count(),
                            all_values_moved,

                            // Ensure the copy data is not the same memory of the copied data
                            (void *)moved.data() != (void *)move.data(),

                            // Ensure we are allocating only one time
                            move.allocator().allocation_count(),
                            move.allocator().free_count(),
                            move.allocator().move_assign_count(),
                            move.allocator().move_construct_count(),
                            moved.allocator().allocation_count(),
                            moved.allocator().free_count(),
                            moved.allocator().move_assign_count(),
                            moved.allocator().move_construct_count()
                        };
                    };

                    // Non constant
                    {
                        const auto result = test_default_allocator({0, 1, 2, 3}, 1u);
                        // Ensure we copy all datas in order
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), 4u);
                        hud_assert_eq(std::get<2>(result), 4u + 1u + extra);
                        hud_assert_true(std::get<3>(result));

                        // Ensure the copy data is not the same memory of the copied data
                        hud_assert_true(std::get<4>(result));

                        // Ensure we are allocating only one time of not
                        // With extra a eallocation is done to allocate with the correct allocator
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                        hud_assert_eq(std::get<7>(result), 0u);
                        hud_assert_eq(std::get<8>(result), 0u);
                        hud_assert_eq(std::get<9>(result), 1u);
                        hud_assert_eq(std::get<10>(result), 1u);
                        hud_assert_eq(std::get<11>(result), 0u);
                        hud_assert_eq(std::get<12>(result), 0u);
                    }

                    // Constant
                    {
                        constexpr auto result = test_default_allocator({0, 1, 2, 3}, 1u);
                        // Ensure we copy all datas in order
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), 4u);
                        hud_assert_eq(std::get<2>(result), 4u + 1u + extra);
                        hud_assert_true(std::get<3>(result));

                        // Ensure the copy data is not the same memory of the copied data
                        hud_assert_true(std::get<4>(result));

                        // Ensure we are allocating only one time of not
                        // With extra a eallocation is done to allocate with the correct allocator
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                        hud_assert_eq(std::get<7>(result), 0u);
                        hud_assert_eq(std::get<8>(result), 0u);
                        hud_assert_eq(std::get<9>(result), 1u);
                        hud_assert_eq(std::get<10>(result), 1u);
                        hud_assert_eq(std::get<11>(result), 0u);
                        hud_assert_eq(std::get<12>(result), 0u);
                    }
                }

                // Test with allocator
                {
                    auto test_with_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
                    {
                        MovedType moved(initializer, copied_extra);

                        // Copy the array
                        hud::array<Type2, AllocatorType> move(std::move(moved), extra, AllocatorType {});

                        // Ensure we moved all datas in order
                        bool all_values_moved = true;
                        for (usize index = 0; index < initializer.size(); index++)
                        {
                            if (move[index] != index)
                            {
                                all_values_moved = false;
                                break;
                            }
                        }

                        return std::tuple {
                            // Ensure we copy all datas in order
                            move.data() != nullptr,
                            move.count(),
                            move.max_count(),
                            all_values_moved,

                            // Ensure the copy data is not the same memory of the copied data
                            (void *)moved.data() != (void *)move.data(),

                            // Ensure we are allocating only one time
                            move.allocator().allocation_count(),
                            move.allocator().free_count(),
                            move.allocator().move_assign_count(),
                            move.allocator().move_construct_count(),
                            moved.allocator().allocation_count(),
                            moved.allocator().free_count(),
                            moved.allocator().move_assign_count(),
                            moved.allocator().move_construct_count()
                        };
                    };

                    // Non constant
                    {
                        const auto result = test_with_allocator({0, 1, 2, 3}, 1u);
                        // Ensure we copy all datas in order
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), 4u);
                        hud_assert_eq(std::get<2>(result), 4u + 1u + extra);
                        hud_assert_true(std::get<3>(result));

                        // Ensure the copy data is not the same memory of the copied data
                        hud_assert_true(std::get<4>(result));

                        // Ensure we are allocating only one time of not
                        // Providing an allocator force the realloation to be done with this allocator
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                        hud_assert_eq(std::get<7>(result), 0u);
                        hud_assert_eq(std::get<8>(result), 0u);
                        hud_assert_eq(std::get<9>(result), 1u);
                        hud_assert_eq(std::get<10>(result), 1u);
                        hud_assert_eq(std::get<11>(result), 0u);
                        hud_assert_eq(std::get<12>(result), 0u);
                    }

                    // Constant
                    {
                        constexpr auto result = test_with_allocator({0, 1, 2, 3}, 1u);
                        // Ensure we copy all datas in order
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), 4u);
                        hud_assert_eq(std::get<2>(result), 4u + 1u + extra);
                        hud_assert_true(std::get<3>(result));

                        // Ensure the copy data is not the same memory of the copied data
                        hud_assert_true(std::get<4>(result));

                        // Ensure we are allocating only one time of not
                        // Providing an allocator force the realloation to be done with this allocator
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                        hud_assert_eq(std::get<7>(result), 0u);
                        hud_assert_eq(std::get<8>(result), 0u);
                        hud_assert_eq(std::get<9>(result), 1u);
                        hud_assert_eq(std::get<10>(result), 1u);
                        hud_assert_eq(std::get<11>(result), 0u);
                        hud_assert_eq(std::get<12>(result), 0u);
                    }
                }
            }
        );
    }
}

GTEST_TEST(array, move_construct_bitwise_move_constructible_different_type_different_allocator)
{

    /** The array we move for test, we allocate also extra memory to test if we really move the count(), not the max_count() elements */
    using Type1 = i32;
    using Type2 = u32;
    using AllocatorType = hud_test::allocator_watcher<alignof(Type1)>;
    using AllocatorType2 = hud_test::allocator_watcher_2<alignof(Type2)>;

    // Ensure we test with different allocator
    static_assert(!std::is_same_v<AllocatorType, AllocatorType2>);
    static_assert(!std::is_same_v<Type1, Type2>);
    static_assert(hud::is_bitwise_move_constructible_v<Type2, Type1>);

    using MovedType = hud::array<Type1, AllocatorType>;

    // Test default allocator
    {
        auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
        {
            MovedType moved(initializer, copied_extra);

            // Copy the array
            hud::array<Type2, AllocatorType2> move(std::move(moved));

            // Ensure we moved all datas in order
            bool all_values_moved = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                if (move[index] != index)
                {
                    all_values_moved = false;
                    break;
                }
            }

            return std::tuple {
                // Ensure we copy all datas in order
                move.data() != nullptr,
                move.count(),
                move.max_count(),
                all_values_moved,

                // Ensure the copy data is not the same memory of the copied data
                (void *)moved.data() != (void *)move.data(),

                // Ensure we are allocating only one time
                move.allocator().allocation_count(),
                move.allocator().free_count(),
                move.allocator().move_assign_count(),
                move.allocator().move_construct_count(),
                moved.allocator().allocation_count(),
                moved.allocator().free_count(),
                moved.allocator().move_assign_count(),
                moved.allocator().move_construct_count()
            };
        };

        // Non constant
        {
            const auto result = test_default_allocator({0, 1, 2, 3}, 1u);
            // Ensure we copy all datas in order
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), 4u);
            hud_assert_eq(std::get<2>(result), 4u + 1u);
            hud_assert_true(std::get<3>(result));

            // Ensure the copy data is not the same memory of the copied data
            hud_assert_true(std::get<4>(result));

            // Ensure we are allocating only one time of not
            // Bitwise movable with same type should just stole the pointer without reallocating
            // If allocator are not the same, a reallocation is done to allocate with the correct allocator
            hud_assert_eq(std::get<5>(result), 1u);
            hud_assert_eq(std::get<6>(result), 0u);
            hud_assert_eq(std::get<7>(result), 0u);
            hud_assert_eq(std::get<8>(result), 0u);
            hud_assert_eq(std::get<9>(result), 1u);
            hud_assert_eq(std::get<10>(result), 1u);
            hud_assert_eq(std::get<11>(result), 0u);
            hud_assert_eq(std::get<12>(result), 0u);
        }

        // Constant
        {
            constexpr auto result = test_default_allocator({0, 1, 2, 3}, 1u);
            // Ensure we copy all datas in order
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), 4u);
            hud_assert_eq(std::get<2>(result), 4u + 1u);
            hud_assert_true(std::get<3>(result));

            // Ensure the copy data is not the same memory of the copied data
            hud_assert_true(std::get<4>(result));

            // Ensure we are allocating only one time of not
            // We move bitwise move constructible of different type, we can't just still the buffer
            // ( Can't reinterpret_cast the pointer in constant evaluated fonction) so we have an allocation , move and free, always!
            // Allocator is move only if allocator are the same
            hud_assert_eq(std::get<5>(result), 1u);
            hud_assert_eq(std::get<6>(result), 0u);
            hud_assert_eq(std::get<7>(result), 0u);
            hud_assert_eq(std::get<8>(result), 0u);
            hud_assert_eq(std::get<9>(result), 1u);
            hud_assert_eq(std::get<10>(result), 1u);
            hud_assert_eq(std::get<11>(result), 0u);
            hud_assert_eq(std::get<12>(result), 0u);
        }
    }

    // Test with allocator
    {
        auto test_with_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
        {
            MovedType moved(initializer, copied_extra);

            // Copy the array
            hud::array<Type2, AllocatorType2> move(std::move(moved), AllocatorType2 {});

            // Ensure we moved all datas in order
            bool all_values_moved = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                if (move[index] != index)
                {
                    all_values_moved = false;
                    break;
                }
            }

            return std::tuple {
                // Ensure we copy all datas in order
                move.data() != nullptr,
                move.count(),
                move.max_count(),
                all_values_moved,

                // Ensure the copy data is not the same memory of the copied data
                (void *)moved.data() != (void *)move.data(),

                // Ensure we are allocating only one time
                move.allocator().allocation_count(),
                move.allocator().free_count(),
                move.allocator().move_assign_count(),
                move.allocator().move_construct_count(),
                moved.allocator().allocation_count(),
                moved.allocator().free_count(),
                moved.allocator().move_assign_count(),
                moved.allocator().move_construct_count()
            };
        };

        // Non constant
        {
            const auto result = test_with_allocator({0, 1, 2, 3}, 1u);
            // Ensure we copy all datas in order
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), 4u);
            hud_assert_eq(std::get<2>(result), 4u + 1u);
            hud_assert_true(std::get<3>(result));

            // Ensure the copy data is not the same memory of the copied data
            hud_assert_true(std::get<4>(result));

            // Ensure we are allocating only one time of not
            // Providing an allocator force the realloation to be done with this allocator
            hud_assert_eq(std::get<5>(result), 1u);
            hud_assert_eq(std::get<6>(result), 0u);
            hud_assert_eq(std::get<7>(result), 0u);
            hud_assert_eq(std::get<8>(result), 0u);
            hud_assert_eq(std::get<9>(result), 1u);
            hud_assert_eq(std::get<10>(result), 1u);
            hud_assert_eq(std::get<11>(result), 0u);
            hud_assert_eq(std::get<12>(result), 0u);
        }

        // Constant
        {
            constexpr auto result = test_with_allocator({0, 1, 2, 3}, 1u);
            // Ensure we copy all datas in order
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), 4u);
            hud_assert_eq(std::get<2>(result), 4u + 1u);
            hud_assert_true(std::get<3>(result));

            // Ensure the copy data is not the same memory of the copied data
            hud_assert_true(std::get<4>(result));

            // Ensure we are allocating only one time of not
            // Providing an allocator force the realloation to be done with this allocator
            hud_assert_eq(std::get<5>(result), 1u);
            hud_assert_eq(std::get<6>(result), 0u);
            hud_assert_eq(std::get<7>(result), 0u);
            hud_assert_eq(std::get<8>(result), 0u);
            hud_assert_eq(std::get<9>(result), 1u);
            hud_assert_eq(std::get<10>(result), 1u);
            hud_assert_eq(std::get<11>(result), 0u);
            hud_assert_eq(std::get<12>(result), 0u);
        }
    }

    // Test with extra
    {
        hud_test::for_each_value<std::make_integer_sequence<usize, 5>>()(
            []<usize extra>()
            {
                // Test default allocator
                {
                    auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
                    {
                        MovedType moved(initializer, copied_extra);

                        // Copy the array
                        hud::array<Type2, AllocatorType2> move(std::move(moved), extra);

                        // Ensure we moved all datas in order
                        bool all_values_moved = true;
                        for (usize index = 0; index < initializer.size(); index++)
                        {
                            if (move[index] != index)
                            {
                                all_values_moved = false;
                                break;
                            }
                        }

                        return std::tuple {
                            // Ensure we copy all datas in order
                            move.data() != nullptr,
                            move.count(),
                            move.max_count(),
                            all_values_moved,

                            // Ensure the copy data is not the same memory of the copied data
                            (void *)moved.data() != (void *)move.data(),

                            // Ensure we are allocating only one time
                            move.allocator().allocation_count(),
                            move.allocator().free_count(),
                            move.allocator().move_assign_count(),
                            move.allocator().move_construct_count(),
                            moved.allocator().allocation_count(),
                            moved.allocator().free_count(),
                            moved.allocator().move_assign_count(),
                            moved.allocator().move_construct_count()
                        };
                    };

                    // Non constant
                    {
                        const auto result = test_default_allocator({0, 1, 2, 3}, 1u);
                        // Ensure we copy all datas in order
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), 4u);
                        hud_assert_eq(std::get<2>(result), 4u + 1u + extra);
                        hud_assert_true(std::get<3>(result));

                        // Ensure the copy data is not the same memory of the copied data
                        hud_assert_true(std::get<4>(result));

                        // Ensure we are allocating only one time of not
                        // With extra a eallocation is done to allocate with the correct allocator
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                        hud_assert_eq(std::get<7>(result), 0u);
                        hud_assert_eq(std::get<8>(result), 0u);
                        hud_assert_eq(std::get<9>(result), 1u);
                        hud_assert_eq(std::get<10>(result), 1u);
                        hud_assert_eq(std::get<11>(result), 0u);
                        hud_assert_eq(std::get<12>(result), 0u);
                    }

                    // Constant
                    {
                        constexpr auto result = test_default_allocator({0, 1, 2, 3}, 1u);
                        // Ensure we copy all datas in order
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), 4u);
                        hud_assert_eq(std::get<2>(result), 4u + 1u + extra);
                        hud_assert_true(std::get<3>(result));

                        // Ensure the copy data is not the same memory of the copied data
                        hud_assert_true(std::get<4>(result));

                        // Ensure we are allocating only one time of not
                        // With extra a eallocation is done to allocate with the correct allocator
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                        hud_assert_eq(std::get<7>(result), 0u);
                        hud_assert_eq(std::get<8>(result), 0u);
                        hud_assert_eq(std::get<9>(result), 1u);
                        hud_assert_eq(std::get<10>(result), 1u);
                        hud_assert_eq(std::get<11>(result), 0u);
                        hud_assert_eq(std::get<12>(result), 0u);
                    }
                }

                // Test with allocator
                {
                    auto test_with_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
                    {
                        MovedType moved(initializer, copied_extra);

                        // Copy the array
                        hud::array<Type2, AllocatorType2> move(std::move(moved), extra, AllocatorType2 {});

                        // Ensure we moved all datas in order
                        bool all_values_moved = true;
                        for (usize index = 0; index < initializer.size(); index++)
                        {
                            if (move[index] != index)
                            {
                                all_values_moved = false;
                                break;
                            }
                        }

                        return std::tuple {
                            // Ensure we copy all datas in order
                            move.data() != nullptr,
                            move.count(),
                            move.max_count(),
                            all_values_moved,

                            // Ensure the copy data is not the same memory of the copied data
                            (void *)moved.data() != (void *)move.data(),

                            // Ensure we are allocating only one time
                            move.allocator().allocation_count(),
                            move.allocator().free_count(),
                            move.allocator().move_assign_count(),
                            move.allocator().move_construct_count(),
                            moved.allocator().allocation_count(),
                            moved.allocator().free_count(),
                            moved.allocator().move_assign_count(),
                            moved.allocator().move_construct_count()
                        };
                    };

                    // Non constant
                    {
                        const auto result = test_with_allocator({0, 1, 2, 3}, 1u);
                        // Ensure we copy all datas in order
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), 4u);
                        hud_assert_eq(std::get<2>(result), 4u + 1u + extra);
                        hud_assert_true(std::get<3>(result));

                        // Ensure the copy data is not the same memory of the copied data
                        hud_assert_true(std::get<4>(result));

                        // Ensure we are allocating only one time of not
                        // Providing an allocator force the realloation to be done with this allocator
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                        hud_assert_eq(std::get<7>(result), 0u);
                        hud_assert_eq(std::get<8>(result), 0u);
                        hud_assert_eq(std::get<9>(result), 1u);
                        hud_assert_eq(std::get<10>(result), 1u);
                        hud_assert_eq(std::get<11>(result), 0u);
                        hud_assert_eq(std::get<12>(result), 0u);
                    }

                    // Constant
                    {
                        constexpr auto result = test_with_allocator({0, 1, 2, 3}, 1u);
                        // Ensure we copy all datas in order
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), 4u);
                        hud_assert_eq(std::get<2>(result), 4u + 1u + extra);
                        hud_assert_true(std::get<3>(result));

                        // Ensure the copy data is not the same memory of the copied data
                        hud_assert_true(std::get<4>(result));

                        // Ensure we are allocating only one time of not
                        // Providing an allocator force the realloation to be done with this allocator
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                        hud_assert_eq(std::get<7>(result), 0u);
                        hud_assert_eq(std::get<8>(result), 0u);
                        hud_assert_eq(std::get<9>(result), 1u);
                        hud_assert_eq(std::get<10>(result), 1u);
                        hud_assert_eq(std::get<11>(result), 0u);
                        hud_assert_eq(std::get<12>(result), 0u);
                    }
                }
            }
        );
    }
}

GTEST_TEST(array, move_construct_non_bitwise_copy_constructible_same_type_same_allocator)
{

    /** The array we move for test, we allocate also extra memory to test if we really move the count(), not the max_count() elements */
    using type = hud_test::non_bitwise_copy_constructible_type;

    using AllocatorType = hud_test::allocator_watcher<alignof(type)>;

    // Ensure we test with different allocator
    static_assert(!hud::is_bitwise_move_constructible_v<type>);
    static_assert(!hud::is_bitwise_copy_constructible_v<type>);
    using MovedType = hud::array<type, AllocatorType>;

    // Test default allocator
    {
        auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
        {
            MovedType moved(initializer, copied_extra);

            // Copy the array
            hud::array<type, AllocatorType> move(std::move(moved));

            // Ensure we moved all datas in order
            bool all_values_moved = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                if (static_cast<usize>(move[index].id()) != index || move[index].copy_constructor_count() != 1u)
                {
                    all_values_moved = false;
                    break;
                }
            }

            return std::tuple {
                // Ensure we copy all datas in order
                move.data() != nullptr,
                move.count(),
                move.max_count(),
                all_values_moved,

                // Ensure the copy data is not the same memory of the copied data
                (void *)moved.data() != (void *)move.data(),

                // Ensure we are allocating only one time
                move.allocator().allocation_count(),
                move.allocator().free_count(),
                move.allocator().move_assign_count(),
                move.allocator().move_construct_count(),
                moved.allocator().allocation_count(),
                moved.allocator().free_count(),
                moved.allocator().move_assign_count(),
                moved.allocator().move_construct_count()
            };
        };

        // Non constant
        {
            const auto result = test_default_allocator({0, 1, 2, 3}, 1u);
            // Ensure we copy all datas in order
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), 4u);
            hud_assert_eq(std::get<2>(result), 4u + 1u);
            hud_assert_true(std::get<3>(result));

            // Ensure the copy data is not the same memory of the copied data
            hud_assert_true(std::get<4>(result));

            // Ensure we are allocating only one time of not
            // Always reallocate if types are non bitwise copy constructible types
            hud_assert_eq(std::get<5>(result), 1u);
            hud_assert_eq(std::get<6>(result), 0u);
            hud_assert_eq(std::get<7>(result), 0u);
            hud_assert_eq(std::get<8>(result), 0u);
            hud_assert_eq(std::get<9>(result), 1u);
            hud_assert_eq(std::get<10>(result), 1u);
            hud_assert_eq(std::get<11>(result), 0u);
            hud_assert_eq(std::get<12>(result), 0u);
        }

        // Constant
        {
            constexpr auto result = test_default_allocator({0, 1, 2, 3}, 1u);
            // Ensure we copy all datas in order
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), 4u);
            hud_assert_eq(std::get<2>(result), 4u + 1u);
            hud_assert_true(std::get<3>(result));

            // Ensure the copy data is not the same memory of the copied data
            hud_assert_true(std::get<4>(result));

            // Ensure we are allocating only one time of not
            // Always reallocate if types are non bitwise copy constructible types
            hud_assert_eq(std::get<5>(result), 1u);
            hud_assert_eq(std::get<6>(result), 0u);
            hud_assert_eq(std::get<7>(result), 0u);
            hud_assert_eq(std::get<8>(result), 0u);
            hud_assert_eq(std::get<9>(result), 1u);
            hud_assert_eq(std::get<10>(result), 1u);
            hud_assert_eq(std::get<11>(result), 0u);
            hud_assert_eq(std::get<12>(result), 0u);
        }
    }

    // Test with allocator
    {
        auto test_with_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
        {
            MovedType moved(initializer, copied_extra);

            // Copy the array
            hud::array<type, AllocatorType> move(std::move(moved), AllocatorType {});

            // Ensure we moved all datas in order
            bool all_values_moved = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                if (static_cast<usize>(move[index].id()) != index || move[index].copy_constructor_count() != 1u)
                {
                    all_values_moved = false;
                    break;
                }
            }

            return std::tuple {
                // Ensure we copy all datas in order
                move.data() != nullptr,
                move.count(),
                move.max_count(),
                all_values_moved,

                // Ensure the copy data is not the same memory of the copied data
                (void *)moved.data() != (void *)move.data(),

                // Ensure we are allocating only one time
                move.allocator().allocation_count(),
                move.allocator().free_count(),
                move.allocator().move_assign_count(),
                move.allocator().move_construct_count(),
                moved.allocator().allocation_count(),
                moved.allocator().free_count(),
                moved.allocator().move_assign_count(),
                moved.allocator().move_construct_count()
            };
        };

        // Non constant
        {
            const auto result = test_with_allocator({0, 1, 2, 3}, 1u);
            // Ensure we copy all datas in order
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), 4u);
            hud_assert_eq(std::get<2>(result), 4u + 1u);
            hud_assert_true(std::get<3>(result));

            // Ensure the copy data is not the same memory of the copied data
            hud_assert_true(std::get<4>(result));

            // Ensure we are allocating only one time of not
            // Always reallocate if types are non bitwise copy constructible types
            hud_assert_eq(std::get<5>(result), 1u);
            hud_assert_eq(std::get<6>(result), 0u);
            hud_assert_eq(std::get<7>(result), 0u);
            hud_assert_eq(std::get<8>(result), 0u);
            hud_assert_eq(std::get<9>(result), 1u);
            hud_assert_eq(std::get<10>(result), 1u);
            hud_assert_eq(std::get<11>(result), 0u);
            hud_assert_eq(std::get<12>(result), 0u);
        }

        // Constant
        {
            constexpr auto result = test_with_allocator({0, 1, 2, 3}, 1u);
            // Ensure we copy all datas in order
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), 4u);
            hud_assert_eq(std::get<2>(result), 4u + 1u);
            hud_assert_true(std::get<3>(result));

            // Ensure the copy data is not the same memory of the copied data
            hud_assert_true(std::get<4>(result));

            // Ensure we are allocating only one time of not
            // Always reallocate if types are non bitwise copy constructible types
            hud_assert_eq(std::get<5>(result), 1u);
            hud_assert_eq(std::get<6>(result), 0u);
            hud_assert_eq(std::get<7>(result), 0u);
            hud_assert_eq(std::get<8>(result), 0u);
            hud_assert_eq(std::get<9>(result), 1u);
            hud_assert_eq(std::get<10>(result), 1u);
            hud_assert_eq(std::get<11>(result), 0u);
            hud_assert_eq(std::get<12>(result), 0u);
        }
    }

    // Test with extra
    {
        hud_test::for_each_value<std::make_integer_sequence<usize, 5>>()(
            []<usize extra>()
            {
                // Test default allocator
                {
                    auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
                    {
                        MovedType moved(initializer, copied_extra);

                        // Copy the array
                        hud::array<type, AllocatorType> move(std::move(moved), extra);

                        // Ensure we moved all datas in order
                        bool all_values_moved = true;
                        for (usize index = 0; index < initializer.size(); index++)
                        {
                            if (static_cast<usize>(move[index].id()) != index || move[index].copy_constructor_count() != 1u)
                            {
                                all_values_moved = false;
                                break;
                            }
                        }

                        return std::tuple {
                            // Ensure we copy all datas in order
                            move.data() != nullptr,
                            move.count(),
                            move.max_count(),
                            all_values_moved,

                            // Ensure the copy data is not the same memory of the copied data
                            (void *)moved.data() != (void *)move.data(),

                            // Ensure we are allocating only one time
                            move.allocator().allocation_count(),
                            move.allocator().free_count(),
                            move.allocator().move_assign_count(),
                            move.allocator().move_construct_count(),
                            moved.allocator().allocation_count(),
                            moved.allocator().free_count(),
                            moved.allocator().move_assign_count(),
                            moved.allocator().move_construct_count()
                        };
                    };

                    // Non constant
                    {
                        const auto result = test_default_allocator({0, 1, 2, 3}, 1u);
                        // Ensure we copy all datas in order
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), 4u);
                        hud_assert_eq(std::get<2>(result), 4u + 1u + extra);
                        hud_assert_true(std::get<3>(result));

                        // Ensure the copy data is not the same memory of the copied data
                        hud_assert_true(std::get<4>(result));

                        // Ensure we are allocating only one time of not
                        // Always reallocate if types are non bitwise copy constructible types
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                        hud_assert_eq(std::get<7>(result), 0u);
                        hud_assert_eq(std::get<8>(result), 0u);
                        hud_assert_eq(std::get<9>(result), 1u);
                        hud_assert_eq(std::get<10>(result), 1u);
                        hud_assert_eq(std::get<11>(result), 0u);
                        hud_assert_eq(std::get<12>(result), 0u);
                    }

                    // Constant
                    {
                        constexpr auto result = test_default_allocator({0, 1, 2, 3}, 1u);
                        // Ensure we copy all datas in order
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), 4u);
                        hud_assert_eq(std::get<2>(result), 4u + 1u + extra);
                        hud_assert_true(std::get<3>(result));

                        // Ensure the copy data is not the same memory of the copied data
                        hud_assert_true(std::get<4>(result));

                        // Ensure we are allocating only one time of not
                        // Always reallocate if types are non bitwise copy constructible types
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                        hud_assert_eq(std::get<7>(result), 0u);
                        hud_assert_eq(std::get<8>(result), 0u);
                        hud_assert_eq(std::get<9>(result), 1u);
                        hud_assert_eq(std::get<10>(result), 1u);
                        hud_assert_eq(std::get<11>(result), 0u);
                        hud_assert_eq(std::get<12>(result), 0u);
                    }
                }

                // Test with allocator
                {
                    auto test_with_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
                    {
                        MovedType moved(initializer, copied_extra);

                        // Copy the array
                        hud::array<type, AllocatorType> move(std::move(moved), extra, AllocatorType {});

                        // Ensure we moved all datas in order
                        bool all_values_moved = true;
                        for (usize index = 0; index < initializer.size(); index++)
                        {
                            if (static_cast<usize>(move[index].id()) != index || move[index].copy_constructor_count() != 1u)
                            {
                                all_values_moved = false;
                                break;
                            }
                        }

                        return std::tuple {
                            // Ensure we copy all datas in order
                            move.data() != nullptr,
                            move.count(),
                            move.max_count(),
                            all_values_moved,

                            // Ensure the copy data is not the same memory of the copied data
                            (void *)moved.data() != (void *)move.data(),

                            // Ensure we are allocating only one time
                            move.allocator().allocation_count(),
                            move.allocator().free_count(),
                            move.allocator().move_assign_count(),
                            move.allocator().move_construct_count(),
                            moved.allocator().allocation_count(),
                            moved.allocator().free_count(),
                            moved.allocator().move_assign_count(),
                            moved.allocator().move_construct_count()
                        };
                    };

                    // Non constant
                    {
                        const auto result = test_with_allocator({0, 1, 2, 3}, 1u);
                        // Ensure we copy all datas in order
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), 4u);
                        hud_assert_eq(std::get<2>(result), 4u + 1u + extra);
                        hud_assert_true(std::get<3>(result));

                        // Ensure the copy data is not the same memory of the copied data
                        hud_assert_true(std::get<4>(result));

                        // Ensure we are allocating only one time of not
                        // Always reallocate if types are non bitwise copy constructible types
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                        hud_assert_eq(std::get<7>(result), 0u);
                        hud_assert_eq(std::get<8>(result), 0u);
                        hud_assert_eq(std::get<9>(result), 1u);
                        hud_assert_eq(std::get<10>(result), 1u);
                        hud_assert_eq(std::get<11>(result), 0u);
                        hud_assert_eq(std::get<12>(result), 0u);
                    }

                    // Constant
                    {
                        constexpr auto result = test_with_allocator({0, 1, 2, 3}, 1u);
                        // Ensure we copy all datas in order
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), 4u);
                        hud_assert_eq(std::get<2>(result), 4u + 1u + extra);
                        hud_assert_true(std::get<3>(result));

                        // Ensure the copy data is not the same memory of the copied data
                        hud_assert_true(std::get<4>(result));

                        // Ensure we are allocating only one time of not
                        // Always reallocate if types are non bitwise copy constructible types
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                        hud_assert_eq(std::get<7>(result), 0u);
                        hud_assert_eq(std::get<8>(result), 0u);
                        hud_assert_eq(std::get<9>(result), 1u);
                        hud_assert_eq(std::get<10>(result), 1u);
                        hud_assert_eq(std::get<11>(result), 0u);
                        hud_assert_eq(std::get<12>(result), 0u);
                    }
                }
            }
        );
    }
}

GTEST_TEST(array, move_construct_non_bitwise_copy_constructible_different_type_same_allocator)
{

    /** The array we move for test, we allocate also extra memory to test if we really move the count(), not the max_count() elements */
    using Type1 = hud_test::non_bitwise_copy_constructible_type;
    using Type2 = hud_test::non_bitwise_copy_constructible_type2;

    using AllocatorType = hud_test::allocator_watcher<alignof(Type1)>;

    // Ensure we test with different allocator
    static_assert(!std::is_same_v<Type1, Type2>);
    static_assert(!hud::is_bitwise_move_constructible_v<Type2, Type1>);
    static_assert(!hud::is_bitwise_copy_constructible_v<Type2, Type1>);

    using MovedType = hud::array<Type1, AllocatorType>;

    // Test default allocator
    {
        auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
        {
            MovedType moved(initializer, copied_extra);

            // Copy the array
            hud::array<Type1, AllocatorType> move(std::move(moved));

            // Ensure we moved all datas in order
            bool all_values_moved = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                if (static_cast<usize>(move[index].id()) != index || move[index].copy_constructor_count() != 1u)
                {
                    all_values_moved = false;
                    break;
                }
            }

            return std::tuple {
                // Ensure we copy all datas in order
                move.data() != nullptr,
                move.count(),
                move.max_count(),
                all_values_moved,

                // Ensure the copy data is not the same memory of the copied data
                (void *)moved.data() != (void *)move.data(),

                // Ensure we are allocating only one time
                move.allocator().allocation_count(),
                move.allocator().free_count(),
                move.allocator().move_assign_count(),
                move.allocator().move_construct_count(),
                moved.allocator().allocation_count(),
                moved.allocator().free_count(),
                moved.allocator().move_assign_count(),
                moved.allocator().move_construct_count()
            };
        };

        // Non constant
        {
            const auto result = test_default_allocator({0, 1, 2, 3}, 1u);
            // Ensure we copy all datas in order
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), 4u);
            hud_assert_eq(std::get<2>(result), 4u + 1u);
            hud_assert_true(std::get<3>(result));

            // Ensure the copy data is not the same memory of the copied data
            hud_assert_true(std::get<4>(result));

            // Ensure we are allocating only one time of not
            // Always reallocate if types are non bitwise copy constructible types
            hud_assert_eq(std::get<5>(result), 1u);
            hud_assert_eq(std::get<6>(result), 0u);
            hud_assert_eq(std::get<7>(result), 0u);
            hud_assert_eq(std::get<8>(result), 0u);
            hud_assert_eq(std::get<9>(result), 1u);
            hud_assert_eq(std::get<10>(result), 1u);
            hud_assert_eq(std::get<11>(result), 0u);
            hud_assert_eq(std::get<12>(result), 0u);
        }

        // Constant
        {
            constexpr auto result = test_default_allocator({0, 1, 2, 3}, 1u);
            // Ensure we copy all datas in order
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), 4u);
            hud_assert_eq(std::get<2>(result), 4u + 1u);
            hud_assert_true(std::get<3>(result));

            // Ensure the copy data is not the same memory of the copied data
            hud_assert_true(std::get<4>(result));

            // Ensure we are allocating only one time of not
            // Always reallocate if types are non bitwise copy constructible types
            hud_assert_eq(std::get<5>(result), 1u);
            hud_assert_eq(std::get<6>(result), 0u);
            hud_assert_eq(std::get<7>(result), 0u);
            hud_assert_eq(std::get<8>(result), 0u);
            hud_assert_eq(std::get<9>(result), 1u);
            hud_assert_eq(std::get<10>(result), 1u);
            hud_assert_eq(std::get<11>(result), 0u);
            hud_assert_eq(std::get<12>(result), 0u);
        }
    }

    // Test with allocator
    {
        auto test_with_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
        {
            MovedType moved(initializer, copied_extra);

            // Copy the array
            hud::array<Type1, AllocatorType> move(std::move(moved), AllocatorType {});

            // Ensure we moved all datas in order
            bool all_values_moved = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                if (static_cast<usize>(move[index].id()) != index || move[index].copy_constructor_count() != 1u)
                {
                    all_values_moved = false;
                    break;
                }
            }

            return std::tuple {
                // Ensure we copy all datas in order
                move.data() != nullptr,
                move.count(),
                move.max_count(),
                all_values_moved,

                // Ensure the copy data is not the same memory of the copied data
                (void *)moved.data() != (void *)move.data(),

                // Ensure we are allocating only one time
                move.allocator().allocation_count(),
                move.allocator().free_count(),
                move.allocator().move_assign_count(),
                move.allocator().move_construct_count(),
                moved.allocator().allocation_count(),
                moved.allocator().free_count(),
                moved.allocator().move_assign_count(),
                moved.allocator().move_construct_count()
            };
        };

        // Non constant
        {
            const auto result = test_with_allocator({0, 1, 2, 3}, 1u);
            // Ensure we copy all datas in order
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), 4u);
            hud_assert_eq(std::get<2>(result), 4u + 1u);
            hud_assert_true(std::get<3>(result));

            // Ensure the copy data is not the same memory of the copied data
            hud_assert_true(std::get<4>(result));

            // Ensure we are allocating only one time of not
            // Always reallocate if types are non bitwise copy constructible types
            hud_assert_eq(std::get<5>(result), 1u);
            hud_assert_eq(std::get<6>(result), 0u);
            hud_assert_eq(std::get<7>(result), 0u);
            hud_assert_eq(std::get<8>(result), 0u);
            hud_assert_eq(std::get<9>(result), 1u);
            hud_assert_eq(std::get<10>(result), 1u);
            hud_assert_eq(std::get<11>(result), 0u);
            hud_assert_eq(std::get<12>(result), 0u);
        }

        // Constant
        {
            constexpr auto result = test_with_allocator({0, 1, 2, 3}, 1u);
            // Ensure we copy all datas in order
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), 4u);
            hud_assert_eq(std::get<2>(result), 4u + 1u);
            hud_assert_true(std::get<3>(result));

            // Ensure the copy data is not the same memory of the copied data
            hud_assert_true(std::get<4>(result));

            // Ensure we are allocating only one time of not
            // Always reallocate if types are non bitwise copy constructible types
            hud_assert_eq(std::get<5>(result), 1u);
            hud_assert_eq(std::get<6>(result), 0u);
            hud_assert_eq(std::get<7>(result), 0u);
            hud_assert_eq(std::get<8>(result), 0u);
            hud_assert_eq(std::get<9>(result), 1u);
            hud_assert_eq(std::get<10>(result), 1u);
            hud_assert_eq(std::get<11>(result), 0u);
            hud_assert_eq(std::get<12>(result), 0u);
        }
    }

    // Test with extra
    {
        hud_test::for_each_value<std::make_integer_sequence<usize, 5>>()(
            []<usize extra>()
            {
                // Test default allocator
                {
                    auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
                    {
                        MovedType moved(initializer, copied_extra);

                        // Copy the array
                        hud::array<Type1, AllocatorType> move(std::move(moved), extra);

                        // Ensure we moved all datas in order
                        bool all_values_moved = true;
                        for (usize index = 0; index < initializer.size(); index++)
                        {
                            if (static_cast<usize>(move[index].id()) != index || move[index].copy_constructor_count() != 1u)
                            {
                                all_values_moved = false;
                                break;
                            }
                        }

                        return std::tuple {
                            // Ensure we copy all datas in order
                            move.data() != nullptr,
                            move.count(),
                            move.max_count(),
                            all_values_moved,

                            // Ensure the copy data is not the same memory of the copied data
                            (void *)moved.data() != (void *)move.data(),

                            // Ensure we are allocating only one time
                            move.allocator().allocation_count(),
                            move.allocator().free_count(),
                            move.allocator().move_assign_count(),
                            move.allocator().move_construct_count(),
                            moved.allocator().allocation_count(),
                            moved.allocator().free_count(),
                            moved.allocator().move_assign_count(),
                            moved.allocator().move_construct_count()
                        };
                    };

                    // Non constant
                    {
                        const auto result = test_default_allocator({0, 1, 2, 3}, 1u);
                        // Ensure we copy all datas in order
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), 4u);
                        hud_assert_eq(std::get<2>(result), 4u + 1u + extra);
                        hud_assert_true(std::get<3>(result));

                        // Ensure the copy data is not the same memory of the copied data
                        hud_assert_true(std::get<4>(result));

                        // Ensure we are allocating only one time of not
                        // Always reallocate if types are non bitwise copy constructible types
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                        hud_assert_eq(std::get<7>(result), 0u);
                        hud_assert_eq(std::get<8>(result), 0u);
                        hud_assert_eq(std::get<9>(result), 1u);
                        hud_assert_eq(std::get<10>(result), 1u);
                        hud_assert_eq(std::get<11>(result), 0u);
                        hud_assert_eq(std::get<12>(result), 0u);
                    }

                    // Constant
                    {
                        constexpr auto result = test_default_allocator({0, 1, 2, 3}, 1u);
                        // Ensure we copy all datas in order
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), 4u);
                        hud_assert_eq(std::get<2>(result), 4u + 1u + extra);
                        hud_assert_true(std::get<3>(result));

                        // Ensure the copy data is not the same memory of the copied data
                        hud_assert_true(std::get<4>(result));

                        // Ensure we are allocating only one time of not
                        // Always reallocate if types are non bitwise copy constructible types
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                        hud_assert_eq(std::get<7>(result), 0u);
                        hud_assert_eq(std::get<8>(result), 0u);
                        hud_assert_eq(std::get<9>(result), 1u);
                        hud_assert_eq(std::get<10>(result), 1u);
                        hud_assert_eq(std::get<11>(result), 0u);
                        hud_assert_eq(std::get<12>(result), 0u);
                    }
                }

                // Test with allocator
                {
                    auto test_with_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
                    {
                        MovedType moved(initializer, copied_extra);

                        // Copy the array
                        hud::array<Type1, AllocatorType> move(std::move(moved), extra, AllocatorType {});

                        // Ensure we moved all datas in order
                        bool all_values_moved = true;
                        for (usize index = 0; index < initializer.size(); index++)
                        {
                            if (static_cast<usize>(move[index].id()) != index || move[index].copy_constructor_count() != 1u)
                            {
                                all_values_moved = false;
                                break;
                            }
                        }

                        return std::tuple {
                            // Ensure we copy all datas in order
                            move.data() != nullptr,
                            move.count(),
                            move.max_count(),
                            all_values_moved,

                            // Ensure the copy data is not the same memory of the copied data
                            (void *)moved.data() != (void *)move.data(),

                            // Ensure we are allocating only one time
                            move.allocator().allocation_count(),
                            move.allocator().free_count(),
                            move.allocator().move_assign_count(),
                            move.allocator().move_construct_count(),
                            moved.allocator().allocation_count(),
                            moved.allocator().free_count(),
                            moved.allocator().move_assign_count(),
                            moved.allocator().move_construct_count()
                        };
                    };

                    // Non constant
                    {
                        const auto result = test_with_allocator({0, 1, 2, 3}, 1u);
                        // Ensure we copy all datas in order
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), 4u);
                        hud_assert_eq(std::get<2>(result), 4u + 1u + extra);
                        hud_assert_true(std::get<3>(result));

                        // Ensure the copy data is not the same memory of the copied data
                        hud_assert_true(std::get<4>(result));

                        // Ensure we are allocating only one time of not
                        // Always reallocate if types are non bitwise copy constructible types
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                        hud_assert_eq(std::get<7>(result), 0u);
                        hud_assert_eq(std::get<8>(result), 0u);
                        hud_assert_eq(std::get<9>(result), 1u);
                        hud_assert_eq(std::get<10>(result), 1u);
                        hud_assert_eq(std::get<11>(result), 0u);
                        hud_assert_eq(std::get<12>(result), 0u);
                    }

                    // Constant
                    {
                        constexpr auto result = test_with_allocator({0, 1, 2, 3}, 1u);
                        // Ensure we copy all datas in order
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), 4u);
                        hud_assert_eq(std::get<2>(result), 4u + 1u + extra);
                        hud_assert_true(std::get<3>(result));

                        // Ensure the copy data is not the same memory of the copied data
                        hud_assert_true(std::get<4>(result));

                        // Ensure we are allocating only one time of not
                        // Always reallocate if types are non bitwise copy constructible types
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                        hud_assert_eq(std::get<7>(result), 0u);
                        hud_assert_eq(std::get<8>(result), 0u);
                        hud_assert_eq(std::get<9>(result), 1u);
                        hud_assert_eq(std::get<10>(result), 1u);
                        hud_assert_eq(std::get<11>(result), 0u);
                        hud_assert_eq(std::get<12>(result), 0u);
                    }
                }
            }
        );
    }
}

GTEST_TEST(array, move_construct_non_bitwise_copy_constructible_same_type_different_allocator)
{

    /** The array we move for test, we allocate also extra memory to test if we really move the count(), not the max_count() elements */
    using type = hud_test::non_bitwise_copy_constructible_type;

    using AllocatorType = hud_test::allocator_watcher<alignof(type)>;
    using AllocatorType2 = hud_test::allocator_watcher_2<alignof(type)>;

    // Ensure we test with different allocator
    static_assert(!std::is_same_v<AllocatorType, AllocatorType2>);
    static_assert(!hud::is_bitwise_move_constructible_v<type>);

    using MovedType = hud::array<type, AllocatorType>;

    // Test default allocator
    {
        auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
        {
            MovedType moved(initializer, copied_extra);

            // Copy the array
            hud::array<type, AllocatorType2> move(std::move(moved));

            // Ensure we moved all datas in order
            bool all_values_moved = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                if (static_cast<usize>(move[index].id()) != index || move[index].copy_constructor_count() != 1u)
                {
                    all_values_moved = false;
                    break;
                }
            }

            return std::tuple {
                // Ensure we copy all datas in order
                move.data() != nullptr,
                move.count(),
                move.max_count(),
                all_values_moved,

                // Ensure the copy data is not the same memory of the copied data
                (void *)moved.data() != (void *)move.data(),

                // Ensure we are allocating only one time
                move.allocator().allocation_count(),
                move.allocator().free_count(),
                move.allocator().move_assign_count(),
                move.allocator().move_construct_count(),
                moved.allocator().allocation_count(),
                moved.allocator().free_count(),
                moved.allocator().move_assign_count(),
                moved.allocator().move_construct_count()
            };
        };

        // Non constant
        {
            const auto result = test_default_allocator({0, 1, 2, 3}, 1u);
            // Ensure we copy all datas in order
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), 4u);
            hud_assert_eq(std::get<2>(result), 4u + 1u);
            hud_assert_true(std::get<3>(result));

            // Ensure the copy data is not the same memory of the copied data
            hud_assert_true(std::get<4>(result));

            // Ensure we are allocating only one time of not
            // Always reallocate if types are non bitwise copy constructible types
            hud_assert_eq(std::get<5>(result), 1u);
            hud_assert_eq(std::get<6>(result), 0u);
            hud_assert_eq(std::get<7>(result), 0u);
            hud_assert_eq(std::get<8>(result), 0u);
            hud_assert_eq(std::get<9>(result), 1u);
            hud_assert_eq(std::get<10>(result), 1u);
            hud_assert_eq(std::get<11>(result), 0u);
            hud_assert_eq(std::get<12>(result), 0u);
        }

        // Constant
        {
            constexpr auto result = test_default_allocator({0, 1, 2, 3}, 1u);
            // Ensure we copy all datas in order
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), 4u);
            hud_assert_eq(std::get<2>(result), 4u + 1u);
            hud_assert_true(std::get<3>(result));

            // Ensure the copy data is not the same memory of the copied data
            hud_assert_true(std::get<4>(result));

            // Ensure we are allocating only one time of not
            // Always reallocate if types are non bitwise copy constructible types
            hud_assert_eq(std::get<5>(result), 1u);
            hud_assert_eq(std::get<6>(result), 0u);
            hud_assert_eq(std::get<7>(result), 0u);
            hud_assert_eq(std::get<8>(result), 0u);
            hud_assert_eq(std::get<9>(result), 1u);
            hud_assert_eq(std::get<10>(result), 1u);
            hud_assert_eq(std::get<11>(result), 0u);
            hud_assert_eq(std::get<12>(result), 0u);
        }
    }

    // Test with allocator
    {
        auto test_with_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
        {
            MovedType moved(initializer, copied_extra);

            // Copy the array
            hud::array<type, AllocatorType2> move(std::move(moved), AllocatorType2 {});

            // Ensure we moved all datas in order
            bool all_values_moved = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                if (static_cast<usize>(move[index].id()) != index || move[index].copy_constructor_count() != 1u)
                {
                    all_values_moved = false;
                    break;
                }
            }

            return std::tuple {
                // Ensure we copy all datas in order
                move.data() != nullptr,
                move.count(),
                move.max_count(),
                all_values_moved,

                // Ensure the copy data is not the same memory of the copied data
                (void *)moved.data() != (void *)move.data(),

                // Ensure we are allocating only one time
                move.allocator().allocation_count(),
                move.allocator().free_count(),
                move.allocator().move_assign_count(),
                move.allocator().move_construct_count(),
                moved.allocator().allocation_count(),
                moved.allocator().free_count(),
                moved.allocator().move_assign_count(),
                moved.allocator().move_construct_count()
            };
        };

        // Non constant
        {
            const auto result = test_with_allocator({0, 1, 2, 3}, 1u);
            // Ensure we copy all datas in order
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), 4u);
            hud_assert_eq(std::get<2>(result), 4u + 1u);
            hud_assert_true(std::get<3>(result));

            // Ensure the copy data is not the same memory of the copied data
            hud_assert_true(std::get<4>(result));

            // Ensure we are allocating only one time of not
            // Always reallocate if types are non bitwise copy constructible types
            hud_assert_eq(std::get<5>(result), 1u);
            hud_assert_eq(std::get<6>(result), 0u);
            hud_assert_eq(std::get<7>(result), 0u);
            hud_assert_eq(std::get<8>(result), 0u);
            hud_assert_eq(std::get<9>(result), 1u);
            hud_assert_eq(std::get<10>(result), 1u);
            hud_assert_eq(std::get<11>(result), 0u);
            hud_assert_eq(std::get<12>(result), 0u);
        }

        // Constant
        {
            constexpr auto result = test_with_allocator({0, 1, 2, 3}, 1u);
            // Ensure we copy all datas in order
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), 4u);
            hud_assert_eq(std::get<2>(result), 4u + 1u);
            hud_assert_true(std::get<3>(result));

            // Ensure the copy data is not the same memory of the copied data
            hud_assert_true(std::get<4>(result));

            // Ensure we are allocating only one time of not
            // Always reallocate if types are non bitwise copy constructible types
            hud_assert_eq(std::get<5>(result), 1u);
            hud_assert_eq(std::get<6>(result), 0u);
            hud_assert_eq(std::get<7>(result), 0u);
            hud_assert_eq(std::get<8>(result), 0u);
            hud_assert_eq(std::get<9>(result), 1u);
            hud_assert_eq(std::get<10>(result), 1u);
            hud_assert_eq(std::get<11>(result), 0u);
            hud_assert_eq(std::get<12>(result), 0u);
        }
    }

    // Test with extra
    {
        hud_test::for_each_value<std::make_integer_sequence<usize, 5>>()(
            []<usize extra>()
            {
                // Test default allocator
                {
                    auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
                    {
                        MovedType moved(initializer, copied_extra);

                        // Copy the array
                        hud::array<type, AllocatorType2> move(std::move(moved), extra);

                        // Ensure we moved all datas in order
                        bool all_values_moved = true;
                        for (usize index = 0; index < initializer.size(); index++)
                        {
                            if (static_cast<usize>(move[index].id()) != index || move[index].copy_constructor_count() != 1u)
                            {
                                all_values_moved = false;
                                break;
                            }
                        }

                        return std::tuple {
                            // Ensure we copy all datas in order
                            move.data() != nullptr,
                            move.count(),
                            move.max_count(),
                            all_values_moved,

                            // Ensure the copy data is not the same memory of the copied data
                            (void *)moved.data() != (void *)move.data(),

                            // Ensure we are allocating only one time
                            move.allocator().allocation_count(),
                            move.allocator().free_count(),
                            move.allocator().move_assign_count(),
                            move.allocator().move_construct_count(),
                            moved.allocator().allocation_count(),
                            moved.allocator().free_count(),
                            moved.allocator().move_assign_count(),
                            moved.allocator().move_construct_count()
                        };
                    };

                    // Non constant
                    {
                        const auto result = test_default_allocator({0, 1, 2, 3}, 1u);
                        // Ensure we copy all datas in order
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), 4u);
                        hud_assert_eq(std::get<2>(result), 4u + 1u + extra);
                        hud_assert_true(std::get<3>(result));

                        // Ensure the copy data is not the same memory of the copied data
                        hud_assert_true(std::get<4>(result));

                        // Ensure we are allocating only one time of not
                        // Always reallocate if types are non bitwise copy constructible types
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                        hud_assert_eq(std::get<7>(result), 0u);
                        hud_assert_eq(std::get<8>(result), 0u);
                        hud_assert_eq(std::get<9>(result), 1u);
                        hud_assert_eq(std::get<10>(result), 1u);
                        hud_assert_eq(std::get<11>(result), 0u);
                        hud_assert_eq(std::get<12>(result), 0u);
                    }

                    // Constant
                    {
                        constexpr auto result = test_default_allocator({0, 1, 2, 3}, 1u);
                        // Ensure we copy all datas in order
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), 4u);
                        hud_assert_eq(std::get<2>(result), 4u + 1u + extra);
                        hud_assert_true(std::get<3>(result));

                        // Ensure the copy data is not the same memory of the copied data
                        hud_assert_true(std::get<4>(result));

                        // Ensure we are allocating only one time of not
                        // Always reallocate if types are non bitwise copy constructible types
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                        hud_assert_eq(std::get<7>(result), 0u);
                        hud_assert_eq(std::get<8>(result), 0u);
                        hud_assert_eq(std::get<9>(result), 1u);
                        hud_assert_eq(std::get<10>(result), 1u);
                        hud_assert_eq(std::get<11>(result), 0u);
                        hud_assert_eq(std::get<12>(result), 0u);
                    }
                }

                // Test with allocator
                {
                    auto test_with_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
                    {
                        MovedType moved(initializer, copied_extra);

                        // Copy the array
                        hud::array<type, AllocatorType2> move(std::move(moved), extra, AllocatorType2 {});

                        // Ensure we moved all datas in order
                        bool all_values_moved = true;
                        for (usize index = 0; index < initializer.size(); index++)
                        {
                            if (static_cast<usize>(move[index].id()) != index || move[index].copy_constructor_count() != 1u)
                            {
                                all_values_moved = false;
                                break;
                            }
                        }

                        return std::tuple {
                            // Ensure we copy all datas in order
                            move.data() != nullptr,
                            move.count(),
                            move.max_count(),
                            all_values_moved,

                            // Ensure the copy data is not the same memory of the copied data
                            (void *)moved.data() != (void *)move.data(),

                            // Ensure we are allocating only one time
                            move.allocator().allocation_count(),
                            move.allocator().free_count(),
                            move.allocator().move_assign_count(),
                            move.allocator().move_construct_count(),
                            moved.allocator().allocation_count(),
                            moved.allocator().free_count(),
                            moved.allocator().move_assign_count(),
                            moved.allocator().move_construct_count()
                        };
                    };

                    // Non constant
                    {
                        const auto result = test_with_allocator({0, 1, 2, 3}, 1u);
                        // Ensure we copy all datas in order
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), 4u);
                        hud_assert_eq(std::get<2>(result), 4u + 1u + extra);
                        hud_assert_true(std::get<3>(result));

                        // Ensure the copy data is not the same memory of the copied data
                        hud_assert_true(std::get<4>(result));

                        // Ensure we are allocating only one time of not
                        // Always reallocate if types are non bitwise copy constructible types
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                        hud_assert_eq(std::get<7>(result), 0u);
                        hud_assert_eq(std::get<8>(result), 0u);
                        hud_assert_eq(std::get<9>(result), 1u);
                        hud_assert_eq(std::get<10>(result), 1u);
                        hud_assert_eq(std::get<11>(result), 0u);
                        hud_assert_eq(std::get<12>(result), 0u);
                    }

                    // Constant
                    {
                        constexpr auto result = test_with_allocator({0, 1, 2, 3}, 1u);
                        // Ensure we copy all datas in order
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), 4u);
                        hud_assert_eq(std::get<2>(result), 4u + 1u + extra);
                        hud_assert_true(std::get<3>(result));

                        // Ensure the copy data is not the same memory of the copied data
                        hud_assert_true(std::get<4>(result));

                        // Ensure we are allocating only one time of not
                        // Always reallocate if types are non bitwise copy constructible types
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                        hud_assert_eq(std::get<7>(result), 0u);
                        hud_assert_eq(std::get<8>(result), 0u);
                        hud_assert_eq(std::get<9>(result), 1u);
                        hud_assert_eq(std::get<10>(result), 1u);
                        hud_assert_eq(std::get<11>(result), 0u);
                        hud_assert_eq(std::get<12>(result), 0u);
                    }
                }
            }
        );
    }
}

GTEST_TEST(array, move_construct_non_bitwise_copy_constructible_different_type_different_allocator)
{

    /** The array we move for test, we allocate also extra memory to test if we really move the count(), not the max_count() elements */
    using Type1 = hud_test::non_bitwise_copy_constructible_type;
    using Type2 = hud_test::non_bitwise_copy_constructible_type2;

    using AllocatorType = hud_test::allocator_watcher<alignof(Type1)>;
    using AllocatorType2 = hud_test::allocator_watcher<alignof(Type2)>;

    // Ensure we test with different allocator
    static_assert(!std::is_same_v<Type1, Type2>);
    static_assert(!hud::is_bitwise_move_constructible_v<Type2, Type1>);
    static_assert(!hud::is_bitwise_copy_constructible_v<Type2, Type1>);

    using MovedType = hud::array<Type1, AllocatorType>;

    // Test default allocator
    {
        auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
        {
            MovedType moved(initializer, copied_extra);

            // Copy the array
            hud::array<Type1, AllocatorType2> move(std::move(moved));

            // Ensure we moved all datas in order
            bool all_values_moved = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                if (static_cast<usize>(move[index].id()) != index || move[index].copy_constructor_count() != 1u)
                {
                    all_values_moved = false;
                    break;
                }
            }

            return std::tuple {
                // Ensure we copy all datas in order
                move.data() != nullptr,
                move.count(),
                move.max_count(),
                all_values_moved,

                // Ensure the copy data is not the same memory of the copied data
                (void *)moved.data() != (void *)move.data(),

                // Ensure we are allocating only one time
                move.allocator().allocation_count(),
                move.allocator().free_count(),
                move.allocator().move_assign_count(),
                move.allocator().move_construct_count(),
                moved.allocator().allocation_count(),
                moved.allocator().free_count(),
                moved.allocator().move_assign_count(),
                moved.allocator().move_construct_count()
            };
        };

        // Non constant
        {
            const auto result = test_default_allocator({0, 1, 2, 3}, 1u);
            // Ensure we copy all datas in order
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), 4u);
            hud_assert_eq(std::get<2>(result), 4u + 1u);
            hud_assert_true(std::get<3>(result));

            // Ensure the copy data is not the same memory of the copied data
            hud_assert_true(std::get<4>(result));

            // Ensure we are allocating only one time of not
            // Always reallocate if types are non bitwise copy constructible types
            hud_assert_eq(std::get<5>(result), 1u);
            hud_assert_eq(std::get<6>(result), 0u);
            hud_assert_eq(std::get<7>(result), 0u);
            hud_assert_eq(std::get<8>(result), 0u);
            hud_assert_eq(std::get<9>(result), 1u);
            hud_assert_eq(std::get<10>(result), 1u);
            hud_assert_eq(std::get<11>(result), 0u);
            hud_assert_eq(std::get<12>(result), 0u);
        }

        // Constant
        {
            constexpr auto result = test_default_allocator({0, 1, 2, 3}, 1u);
            // Ensure we copy all datas in order
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), 4u);
            hud_assert_eq(std::get<2>(result), 4u + 1u);
            hud_assert_true(std::get<3>(result));

            // Ensure the copy data is not the same memory of the copied data
            hud_assert_true(std::get<4>(result));

            // Ensure we are allocating only one time of not
            // Always reallocate if types are non bitwise copy constructible types
            hud_assert_eq(std::get<5>(result), 1u);
            hud_assert_eq(std::get<6>(result), 0u);
            hud_assert_eq(std::get<7>(result), 0u);
            hud_assert_eq(std::get<8>(result), 0u);
            hud_assert_eq(std::get<9>(result), 1u);
            hud_assert_eq(std::get<10>(result), 1u);
            hud_assert_eq(std::get<11>(result), 0u);
            hud_assert_eq(std::get<12>(result), 0u);
        }
    }

    // Test with allocator
    {
        auto test_with_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
        {
            MovedType moved(initializer, copied_extra);

            // Copy the array
            hud::array<Type1, AllocatorType> move(std::move(moved), AllocatorType {});

            // Ensure we moved all datas in order
            bool all_values_moved = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                if (static_cast<usize>(move[index].id()) != index || move[index].copy_constructor_count() != 1u)
                {
                    all_values_moved = false;
                    break;
                }
            }

            return std::tuple {
                // Ensure we copy all datas in order
                move.data() != nullptr,
                move.count(),
                move.max_count(),
                all_values_moved,

                // Ensure the copy data is not the same memory of the copied data
                (void *)moved.data() != (void *)move.data(),

                // Ensure we are allocating only one time
                move.allocator().allocation_count(),
                move.allocator().free_count(),
                move.allocator().move_assign_count(),
                move.allocator().move_construct_count(),
                moved.allocator().allocation_count(),
                moved.allocator().free_count(),
                moved.allocator().move_assign_count(),
                moved.allocator().move_construct_count()
            };
        };

        // Non constant
        {
            const auto result = test_with_allocator({0, 1, 2, 3}, 1u);
            // Ensure we copy all datas in order
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), 4u);
            hud_assert_eq(std::get<2>(result), 4u + 1u);
            hud_assert_true(std::get<3>(result));

            // Ensure the copy data is not the same memory of the copied data
            hud_assert_true(std::get<4>(result));

            // Ensure we are allocating only one time of not
            // Always reallocate if types are non bitwise copy constructible types
            hud_assert_eq(std::get<5>(result), 1u);
            hud_assert_eq(std::get<6>(result), 0u);
            hud_assert_eq(std::get<7>(result), 0u);
            hud_assert_eq(std::get<8>(result), 0u);
            hud_assert_eq(std::get<9>(result), 1u);
            hud_assert_eq(std::get<10>(result), 1u);
            hud_assert_eq(std::get<11>(result), 0u);
            hud_assert_eq(std::get<12>(result), 0u);
        }

        // Constant
        {
            constexpr auto result = test_with_allocator({0, 1, 2, 3}, 1u);
            // Ensure we copy all datas in order
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), 4u);
            hud_assert_eq(std::get<2>(result), 4u + 1u);
            hud_assert_true(std::get<3>(result));

            // Ensure the copy data is not the same memory of the copied data
            hud_assert_true(std::get<4>(result));

            // Ensure we are allocating only one time of not
            // Always reallocate if types are non bitwise copy constructible types
            hud_assert_eq(std::get<5>(result), 1u);
            hud_assert_eq(std::get<6>(result), 0u);
            hud_assert_eq(std::get<7>(result), 0u);
            hud_assert_eq(std::get<8>(result), 0u);
            hud_assert_eq(std::get<9>(result), 1u);
            hud_assert_eq(std::get<10>(result), 1u);
            hud_assert_eq(std::get<11>(result), 0u);
            hud_assert_eq(std::get<12>(result), 0u);
        }
    }

    // Test with extra
    {
        hud_test::for_each_value<std::make_integer_sequence<usize, 5>>()(
            []<usize extra>()
            {
                // Test default allocator
                {
                    auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
                    {
                        MovedType moved(initializer, copied_extra);

                        // Copy the array
                        hud::array<Type1, AllocatorType> move(std::move(moved), extra);

                        // Ensure we moved all datas in order
                        bool all_values_moved = true;
                        for (usize index = 0; index < initializer.size(); index++)
                        {
                            if (static_cast<usize>(move[index].id()) != index || move[index].copy_constructor_count() != 1u)
                            {
                                all_values_moved = false;
                                break;
                            }
                        }

                        return std::tuple {
                            // Ensure we copy all datas in order
                            move.data() != nullptr,
                            move.count(),
                            move.max_count(),
                            all_values_moved,

                            // Ensure the copy data is not the same memory of the copied data
                            (void *)moved.data() != (void *)move.data(),

                            // Ensure we are allocating only one time
                            move.allocator().allocation_count(),
                            move.allocator().free_count(),
                            move.allocator().move_assign_count(),
                            move.allocator().move_construct_count(),
                            moved.allocator().allocation_count(),
                            moved.allocator().free_count(),
                            moved.allocator().move_assign_count(),
                            moved.allocator().move_construct_count()
                        };
                    };

                    // Non constant
                    {
                        const auto result = test_default_allocator({0, 1, 2, 3}, 1u);
                        // Ensure we copy all datas in order
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), 4u);
                        hud_assert_eq(std::get<2>(result), 4u + 1u + extra);
                        hud_assert_true(std::get<3>(result));

                        // Ensure the copy data is not the same memory of the copied data
                        hud_assert_true(std::get<4>(result));

                        // Ensure we are allocating only one time of not
                        // Always reallocate if types are non bitwise copy constructible types
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                        hud_assert_eq(std::get<7>(result), 0u);
                        hud_assert_eq(std::get<8>(result), 0u);
                        hud_assert_eq(std::get<9>(result), 1u);
                        hud_assert_eq(std::get<10>(result), 1u);
                        hud_assert_eq(std::get<11>(result), 0u);
                        hud_assert_eq(std::get<12>(result), 0u);
                    }

                    // Constant
                    {
                        constexpr auto result = test_default_allocator({0, 1, 2, 3}, 1u);
                        // Ensure we copy all datas in order
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), 4u);
                        hud_assert_eq(std::get<2>(result), 4u + 1u + extra);
                        hud_assert_true(std::get<3>(result));

                        // Ensure the copy data is not the same memory of the copied data
                        hud_assert_true(std::get<4>(result));

                        // Ensure we are allocating only one time of not
                        // Always reallocate if types are non bitwise copy constructible types
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                        hud_assert_eq(std::get<7>(result), 0u);
                        hud_assert_eq(std::get<8>(result), 0u);
                        hud_assert_eq(std::get<9>(result), 1u);
                        hud_assert_eq(std::get<10>(result), 1u);
                        hud_assert_eq(std::get<11>(result), 0u);
                        hud_assert_eq(std::get<12>(result), 0u);
                    }
                }

                // Test with allocator
                {
                    auto test_with_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
                    {
                        MovedType moved(initializer, copied_extra);

                        // Copy the array
                        hud::array<Type1, AllocatorType> move(std::move(moved), extra, AllocatorType {});

                        // Ensure we moved all datas in order
                        bool all_values_moved = true;
                        for (usize index = 0; index < initializer.size(); index++)
                        {
                            if (static_cast<usize>(move[index].id()) != index || move[index].copy_constructor_count() != 1u)
                            {
                                all_values_moved = false;
                                break;
                            }
                        }

                        return std::tuple {
                            // Ensure we copy all datas in order
                            move.data() != nullptr,
                            move.count(),
                            move.max_count(),
                            all_values_moved,

                            // Ensure the copy data is not the same memory of the copied data
                            (void *)moved.data() != (void *)move.data(),

                            // Ensure we are allocating only one time
                            move.allocator().allocation_count(),
                            move.allocator().free_count(),
                            move.allocator().move_assign_count(),
                            move.allocator().move_construct_count(),
                            moved.allocator().allocation_count(),
                            moved.allocator().free_count(),
                            moved.allocator().move_assign_count(),
                            moved.allocator().move_construct_count()
                        };
                    };

                    // Non constant
                    {
                        const auto result = test_with_allocator({0, 1, 2, 3}, 1u);
                        // Ensure we copy all datas in order
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), 4u);
                        hud_assert_eq(std::get<2>(result), 4u + 1u + extra);
                        hud_assert_true(std::get<3>(result));

                        // Ensure the copy data is not the same memory of the copied data
                        hud_assert_true(std::get<4>(result));

                        // Ensure we are allocating only one time of not
                        // Always reallocate if types are non bitwise copy constructible types
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                        hud_assert_eq(std::get<7>(result), 0u);
                        hud_assert_eq(std::get<8>(result), 0u);
                        hud_assert_eq(std::get<9>(result), 1u);
                        hud_assert_eq(std::get<10>(result), 1u);
                        hud_assert_eq(std::get<11>(result), 0u);
                        hud_assert_eq(std::get<12>(result), 0u);
                    }

                    // Constant
                    {
                        constexpr auto result = test_with_allocator({0, 1, 2, 3}, 1u);
                        // Ensure we copy all datas in order
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), 4u);
                        hud_assert_eq(std::get<2>(result), 4u + 1u + extra);
                        hud_assert_true(std::get<3>(result));

                        // Ensure the copy data is not the same memory of the copied data
                        hud_assert_true(std::get<4>(result));

                        // Ensure we are allocating only one time of not
                        // Always reallocate if types are non bitwise copy constructible types
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                        hud_assert_eq(std::get<7>(result), 0u);
                        hud_assert_eq(std::get<8>(result), 0u);
                        hud_assert_eq(std::get<9>(result), 1u);
                        hud_assert_eq(std::get<10>(result), 1u);
                        hud_assert_eq(std::get<11>(result), 0u);
                        hud_assert_eq(std::get<12>(result), 0u);
                    }
                }
            }
        );
    }
}

GTEST_TEST(array, move_construct_non_bitwise_move_constructible_same_type_same_allocator)
{

    /** The array we copy for test, we allocate also extra memory to test if we really copy the count(), not the max_count() elements */
    using type = hud_test::non_bitwise_move_constructible_type;
    using AllocatorType = hud_test::allocator_watcher<alignof(type)>;

    // Ensure we test with different allocator
    static_assert(!hud::is_bitwise_move_constructible_v<type>);

    using MovedType = hud::array<type, AllocatorType>;

    // Test default allocator
    {
        auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
        {
            MovedType moved(initializer, copied_extra);

            // Copy the array
            hud::array<type, AllocatorType> move(std::move(moved));

            // Ensure we moved all datas in order
            bool all_values_moved = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                if (static_cast<usize>(move[index].id()) != index)
                {
                    all_values_moved = false;
                    break;
                }
                if (move[index].move_constructor_count() != 1u)
                {
                    all_values_moved = false;
                    break;
                }
            }

            return std::tuple {
                // Ensure we copy all datas in order
                move.data() != nullptr,
                move.count(),
                move.max_count(),
                all_values_moved,

                // Ensure the copy data is not the same memory of the copied data
                (void *)moved.data() != (void *)move.data(),

                // Ensure we are allocating only one time
                move.allocator().allocation_count(),
                move.allocator().free_count(),
                move.allocator().move_assign_count(),
                move.allocator().move_construct_count(),
                moved.allocator().allocation_count(),
                moved.allocator().free_count(),
                moved.allocator().move_assign_count(),
                moved.allocator().move_construct_count()
            };
        };

        // Non constant
        {
            const auto result = test_default_allocator({0, 1, 2, 3}, 1u);
            // Ensure we copy all datas in order
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), 4u);
            hud_assert_eq(std::get<2>(result), 4u + 1u);
            hud_assert_true(std::get<3>(result));

            // Ensure the copy data is not the same memory of the copied data
            hud_assert_true(std::get<4>(result));

            // Ensure we are allocating only one time of not
            // Always reallocate if types are non bitwise copy constructible types
            hud_assert_eq(std::get<5>(result), 1u);
            hud_assert_eq(std::get<6>(result), 0u);
            hud_assert_eq(std::get<7>(result), 0u);
            hud_assert_eq(std::get<8>(result), 0u);
            hud_assert_eq(std::get<9>(result), 1u);
            hud_assert_eq(std::get<10>(result), 1u);
            hud_assert_eq(std::get<11>(result), 0u);
            hud_assert_eq(std::get<12>(result), 0u);
        }

        // Constant
        {
            constexpr auto result = test_default_allocator({0, 1, 2, 3}, 1u);
            // Ensure we copy all datas in order
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), 4u);
            hud_assert_eq(std::get<2>(result), 4u + 1u);
            hud_assert_true(std::get<3>(result));

            // Ensure the copy data is not the same memory of the copied data
            hud_assert_true(std::get<4>(result));

            // Ensure we are allocating only one time of not
            // Always reallocate if types are non bitwise copy constructible types
            hud_assert_eq(std::get<5>(result), 1u);
            hud_assert_eq(std::get<6>(result), 0u);
            hud_assert_eq(std::get<7>(result), 0u);
            hud_assert_eq(std::get<8>(result), 0u);
            hud_assert_eq(std::get<9>(result), 1u);
            hud_assert_eq(std::get<10>(result), 1u);
            hud_assert_eq(std::get<11>(result), 0u);
            hud_assert_eq(std::get<12>(result), 0u);
        }
    }

    // Test with allocator
    {
        auto test_with_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
        {
            MovedType moved(initializer, copied_extra);

            // Copy the array
            hud::array<type, AllocatorType> move(std::move(moved), AllocatorType {});

            // Ensure we moved all datas in order
            bool all_values_moved = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                if (static_cast<usize>(move[index].id()) != index)
                {
                    all_values_moved = false;
                    break;
                }
                if (move[index].move_constructor_count() != 1u)
                {
                    all_values_moved = false;
                    break;
                }
            }

            return std::tuple {
                // Ensure we copy all datas in order
                move.data() != nullptr,
                move.count(),
                move.max_count(),
                all_values_moved,

                // Ensure the copy data is not the same memory of the copied data
                (void *)moved.data() != (void *)move.data(),

                // Ensure we are allocating only one time
                move.allocator().allocation_count(),
                move.allocator().free_count(),
                move.allocator().move_assign_count(),
                move.allocator().move_construct_count(),
                moved.allocator().allocation_count(),
                moved.allocator().free_count(),
                moved.allocator().move_assign_count(),
                moved.allocator().move_construct_count()
            };
        };

        // Non constant
        {
            const auto result = test_with_allocator({0, 1, 2, 3}, 1u);
            // Ensure we copy all datas in order
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), 4u);
            hud_assert_eq(std::get<2>(result), 4u + 1u);
            hud_assert_true(std::get<3>(result));

            // Ensure the copy data is not the same memory of the copied data
            hud_assert_true(std::get<4>(result));

            // Ensure we are allocating only one time of not
            // Always reallocate if types are non bitwise copy constructible types
            hud_assert_eq(std::get<5>(result), 1u);
            hud_assert_eq(std::get<6>(result), 0u);
            hud_assert_eq(std::get<7>(result), 0u);
            hud_assert_eq(std::get<8>(result), 0u);
            hud_assert_eq(std::get<9>(result), 1u);
            hud_assert_eq(std::get<10>(result), 1u);
            hud_assert_eq(std::get<11>(result), 0u);
            hud_assert_eq(std::get<12>(result), 0u);
        }

        // Constant
        {
            constexpr auto result = test_with_allocator({0, 1, 2, 3}, 1u);
            // Ensure we copy all datas in order
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), 4u);
            hud_assert_eq(std::get<2>(result), 4u + 1u);
            hud_assert_true(std::get<3>(result));

            // Ensure the copy data is not the same memory of the copied data
            hud_assert_true(std::get<4>(result));

            // Ensure we are allocating only one time of not
            // Always reallocate if types are non bitwise copy constructible types
            hud_assert_eq(std::get<5>(result), 1u);
            hud_assert_eq(std::get<6>(result), 0u);
            hud_assert_eq(std::get<7>(result), 0u);
            hud_assert_eq(std::get<8>(result), 0u);
            hud_assert_eq(std::get<9>(result), 1u);
            hud_assert_eq(std::get<10>(result), 1u);
            hud_assert_eq(std::get<11>(result), 0u);
            hud_assert_eq(std::get<12>(result), 0u);
        }
    }

    // Test with extra
    {
        hud_test::for_each_value<std::make_integer_sequence<usize, 5>>()(
            []<usize extra>()
            {
                // Test default allocator
                {
                    auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
                    {
                        MovedType moved(initializer, copied_extra);

                        // Copy the array
                        hud::array<type, AllocatorType> move(std::move(moved), extra);

                        // Ensure we moved all datas in order
                        bool all_values_moved = true;
                        for (usize index = 0; index < initializer.size(); index++)
                        {
                            if (static_cast<usize>(move[index].id()) != index)
                            {
                                all_values_moved = false;
                                break;
                            }
                            if (move[index].move_constructor_count() != 1u)
                            {
                                all_values_moved = false;
                                break;
                            }
                        }

                        return std::tuple {
                            // Ensure we copy all datas in order
                            move.data() != nullptr,
                            move.count(),
                            move.max_count(),
                            all_values_moved,

                            // Ensure the copy data is not the same memory of the copied data
                            (void *)moved.data() != (void *)move.data(),

                            // Ensure we are allocating only one time
                            move.allocator().allocation_count(),
                            move.allocator().free_count(),
                            move.allocator().move_assign_count(),
                            move.allocator().move_construct_count(),
                            moved.allocator().allocation_count(),
                            moved.allocator().free_count(),
                            moved.allocator().move_assign_count(),
                            moved.allocator().move_construct_count()
                        };
                    };

                    // Non constant
                    {
                        const auto result = test_default_allocator({0, 1, 2, 3}, 1u);
                        // Ensure we copy all datas in order
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), 4u);
                        hud_assert_eq(std::get<2>(result), 4u + 1u + extra);
                        hud_assert_true(std::get<3>(result));

                        // Ensure the copy data is not the same memory of the copied data
                        hud_assert_true(std::get<4>(result));

                        // Ensure we are allocating only one time of not
                        // Always reallocate if types are non bitwise copy constructible types
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                        hud_assert_eq(std::get<7>(result), 0u);
                        hud_assert_eq(std::get<8>(result), 0u);
                        hud_assert_eq(std::get<9>(result), 1u);
                        hud_assert_eq(std::get<10>(result), 1u);
                        hud_assert_eq(std::get<11>(result), 0u);
                        hud_assert_eq(std::get<12>(result), 0u);
                    }

                    // Constant
                    {
                        constexpr auto result = test_default_allocator({0, 1, 2, 3}, 1u);
                        // Ensure we copy all datas in order
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), 4u);
                        hud_assert_eq(std::get<2>(result), 4u + 1u + extra);
                        hud_assert_true(std::get<3>(result));

                        // Ensure the copy data is not the same memory of the copied data
                        hud_assert_true(std::get<4>(result));

                        // Ensure we are allocating only one time of not
                        // Always reallocate if types are non bitwise copy constructible types
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                        hud_assert_eq(std::get<7>(result), 0u);
                        hud_assert_eq(std::get<8>(result), 0u);
                        hud_assert_eq(std::get<9>(result), 1u);
                        hud_assert_eq(std::get<10>(result), 1u);
                        hud_assert_eq(std::get<11>(result), 0u);
                        hud_assert_eq(std::get<12>(result), 0u);
                    }
                }

                // Test with allocator
                {
                    auto test_with_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
                    {
                        MovedType moved(initializer, copied_extra);

                        // Copy the array
                        hud::array<type, AllocatorType> move(std::move(moved), extra, AllocatorType {});

                        // Ensure we moved all datas in order
                        bool all_values_moved = true;
                        for (usize index = 0; index < initializer.size(); index++)
                        {
                            if (static_cast<usize>(move[index].id()) != index)
                            {
                                all_values_moved = false;
                                break;
                            }
                            if (move[index].move_constructor_count() != 1u)
                            {
                                all_values_moved = false;
                                break;
                            }
                        }

                        return std::tuple {
                            // Ensure we copy all datas in order
                            move.data() != nullptr,
                            move.count(),
                            move.max_count(),
                            all_values_moved,

                            // Ensure the copy data is not the same memory of the copied data
                            (void *)moved.data() != (void *)move.data(),

                            // Ensure we are allocating only one time
                            move.allocator().allocation_count(),
                            move.allocator().free_count(),
                            move.allocator().move_assign_count(),
                            move.allocator().move_construct_count(),
                            moved.allocator().allocation_count(),
                            moved.allocator().free_count(),
                            moved.allocator().move_assign_count(),
                            moved.allocator().move_construct_count()
                        };
                    };

                    // Non constant
                    {
                        const auto result = test_with_allocator({0, 1, 2, 3}, 1u);
                        // Ensure we copy all datas in order
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), 4u);
                        hud_assert_eq(std::get<2>(result), 4u + 1u + extra);
                        hud_assert_true(std::get<3>(result));

                        // Ensure the copy data is not the same memory of the copied data
                        hud_assert_true(std::get<4>(result));

                        // Ensure we are allocating only one time of not
                        // Always reallocate if types are non bitwise copy constructible types
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                        hud_assert_eq(std::get<7>(result), 0u);
                        hud_assert_eq(std::get<8>(result), 0u);
                        hud_assert_eq(std::get<9>(result), 1u);
                        hud_assert_eq(std::get<10>(result), 1u);
                        hud_assert_eq(std::get<11>(result), 0u);
                        hud_assert_eq(std::get<12>(result), 0u);
                    }

                    // Constant
                    {
                        constexpr auto result = test_with_allocator({0, 1, 2, 3}, 1u);
                        // Ensure we copy all datas in order
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), 4u);
                        hud_assert_eq(std::get<2>(result), 4u + 1u + extra);
                        hud_assert_true(std::get<3>(result));

                        // Ensure the copy data is not the same memory of the copied data
                        hud_assert_true(std::get<4>(result));

                        // Ensure we are allocating only one time of not
                        // Always reallocate if types are non bitwise copy constructible types
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                        hud_assert_eq(std::get<7>(result), 0u);
                        hud_assert_eq(std::get<8>(result), 0u);
                        hud_assert_eq(std::get<9>(result), 1u);
                        hud_assert_eq(std::get<10>(result), 1u);
                        hud_assert_eq(std::get<11>(result), 0u);
                        hud_assert_eq(std::get<12>(result), 0u);
                    }
                }
            }
        );
    }
}

GTEST_TEST(array, move_construct_non_bitwise_move_constructible_same_type_different_allocator)
{

    /** The array we copy for test, we allocate also extra memory to test if we really copy the count(), not the max_count() elements */
    using type = hud_test::non_bitwise_move_constructible_type;
    using AllocatorType = hud_test::allocator_watcher<alignof(type)>;
    using AllocatorType2 = hud_test::allocator_watcher_2<alignof(type)>;

    // Ensure we test with different allocator
    static_assert(!std::is_same_v<AllocatorType, AllocatorType2>);
    static_assert(!hud::is_bitwise_move_constructible_v<type>);

    using MovedType = hud::array<type, AllocatorType>;

    // Test default allocator
    {
        auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
        {
            MovedType moved(initializer, copied_extra);

            // Copy the array
            hud::array<type, AllocatorType2> move(std::move(moved));

            // Ensure we moved all datas in order
            bool all_values_moved = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                if (static_cast<usize>(move[index].id()) != index)
                {
                    all_values_moved = false;
                    break;
                }
                if (move[index].move_constructor_count() != 1u)
                {
                    all_values_moved = false;
                    break;
                }
            }

            return std::tuple {
                // Ensure we copy all datas in order
                move.data() != nullptr,
                move.count(),
                move.max_count(),
                all_values_moved,

                // Ensure the copy data is not the same memory of the copied data
                (void *)moved.data() != (void *)move.data(),

                // Ensure we are allocating only one time
                move.allocator().allocation_count(),
                move.allocator().free_count(),
                move.allocator().move_assign_count(),
                move.allocator().move_construct_count(),
                moved.allocator().allocation_count(),
                moved.allocator().free_count(),
                moved.allocator().move_assign_count(),
                moved.allocator().move_construct_count()
            };
        };

        // Non constant
        {
            const auto result = test_default_allocator({0, 1, 2, 3}, 1u);
            // Ensure we copy all datas in order
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), 4u);
            hud_assert_eq(std::get<2>(result), 4u + 1u);
            hud_assert_true(std::get<3>(result));

            // Ensure the copy data is not the same memory of the copied data
            hud_assert_true(std::get<4>(result));

            // Ensure we are allocating only one time of not
            // Always reallocate if types are non bitwise copy constructible types
            hud_assert_eq(std::get<5>(result), 1u);
            hud_assert_eq(std::get<6>(result), 0u);
            hud_assert_eq(std::get<7>(result), 0u);
            hud_assert_eq(std::get<8>(result), 0u);
            hud_assert_eq(std::get<9>(result), 1u);
            hud_assert_eq(std::get<10>(result), 1u);
            hud_assert_eq(std::get<11>(result), 0u);
            hud_assert_eq(std::get<12>(result), 0u);
        }

        // Constant
        {
            constexpr auto result = test_default_allocator({0, 1, 2, 3}, 1u);
            // Ensure we copy all datas in order
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), 4u);
            hud_assert_eq(std::get<2>(result), 4u + 1u);
            hud_assert_true(std::get<3>(result));

            // Ensure the copy data is not the same memory of the copied data
            hud_assert_true(std::get<4>(result));

            // Ensure we are allocating only one time of not
            // Always reallocate if types are non bitwise copy constructible types
            hud_assert_eq(std::get<5>(result), 1u);
            hud_assert_eq(std::get<6>(result), 0u);
            hud_assert_eq(std::get<7>(result), 0u);
            hud_assert_eq(std::get<8>(result), 0u);
            hud_assert_eq(std::get<9>(result), 1u);
            hud_assert_eq(std::get<10>(result), 1u);
            hud_assert_eq(std::get<11>(result), 0u);
            hud_assert_eq(std::get<12>(result), 0u);
        }
    }

    // Test with allocator
    {
        auto test_with_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
        {
            MovedType moved(initializer, copied_extra);

            // Copy the array
            hud::array<type, AllocatorType2> move(std::move(moved), AllocatorType2 {});

            // Ensure we moved all datas in order
            bool all_values_moved = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                if (static_cast<usize>(move[index].id()) != index)
                {
                    all_values_moved = false;
                    break;
                }
                if (move[index].move_constructor_count() != 1u)
                {
                    all_values_moved = false;
                    break;
                }
            }

            return std::tuple {
                // Ensure we copy all datas in order
                move.data() != nullptr,
                move.count(),
                move.max_count(),
                all_values_moved,

                // Ensure the copy data is not the same memory of the copied data
                (void *)moved.data() != (void *)move.data(),

                // Ensure we are allocating only one time
                move.allocator().allocation_count(),
                move.allocator().free_count(),
                move.allocator().move_assign_count(),
                move.allocator().move_construct_count(),
                moved.allocator().allocation_count(),
                moved.allocator().free_count(),
                moved.allocator().move_assign_count(),
                moved.allocator().move_construct_count()
            };
        };

        // Non constant
        {
            const auto result = test_with_allocator({0, 1, 2, 3}, 1u);
            // Ensure we copy all datas in order
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), 4u);
            hud_assert_eq(std::get<2>(result), 4u + 1u);
            hud_assert_true(std::get<3>(result));

            // Ensure the copy data is not the same memory of the copied data
            hud_assert_true(std::get<4>(result));

            // Ensure we are allocating only one time of not
            // Always reallocate if types are non bitwise copy constructible types
            hud_assert_eq(std::get<5>(result), 1u);
            hud_assert_eq(std::get<6>(result), 0u);
            hud_assert_eq(std::get<7>(result), 0u);
            hud_assert_eq(std::get<8>(result), 0u);
            hud_assert_eq(std::get<9>(result), 1u);
            hud_assert_eq(std::get<10>(result), 1u);
            hud_assert_eq(std::get<11>(result), 0u);
            hud_assert_eq(std::get<12>(result), 0u);
        }

        // Constant
        {
            constexpr auto result = test_with_allocator({0, 1, 2, 3}, 1u);
            // Ensure we copy all datas in order
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), 4u);
            hud_assert_eq(std::get<2>(result), 4u + 1u);
            hud_assert_true(std::get<3>(result));

            // Ensure the copy data is not the same memory of the copied data
            hud_assert_true(std::get<4>(result));

            // Ensure we are allocating only one time of not
            // Always reallocate if types are non bitwise copy constructible types
            hud_assert_eq(std::get<5>(result), 1u);
            hud_assert_eq(std::get<6>(result), 0u);
            hud_assert_eq(std::get<7>(result), 0u);
            hud_assert_eq(std::get<8>(result), 0u);
            hud_assert_eq(std::get<9>(result), 1u);
            hud_assert_eq(std::get<10>(result), 1u);
            hud_assert_eq(std::get<11>(result), 0u);
            hud_assert_eq(std::get<12>(result), 0u);
        }
    }

    // Test with extra
    {
        hud_test::for_each_value<std::make_integer_sequence<usize, 5>>()(
            []<usize extra>()
            {
                // Test default allocator
                {
                    auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
                    {
                        MovedType moved(initializer, copied_extra);

                        // Copy the array
                        hud::array<type, AllocatorType2> move(std::move(moved), extra);

                        // Ensure we moved all datas in order
                        bool all_values_moved = true;
                        for (usize index = 0; index < initializer.size(); index++)
                        {
                            if (static_cast<usize>(move[index].id()) != index)
                            {
                                all_values_moved = false;
                                break;
                            }
                            if (move[index].move_constructor_count() != 1u)
                            {
                                all_values_moved = false;
                                break;
                            }
                        }

                        return std::tuple {
                            // Ensure we copy all datas in order
                            move.data() != nullptr,
                            move.count(),
                            move.max_count(),
                            all_values_moved,

                            // Ensure the copy data is not the same memory of the copied data
                            (void *)moved.data() != (void *)move.data(),

                            // Ensure we are allocating only one time
                            move.allocator().allocation_count(),
                            move.allocator().free_count(),
                            move.allocator().move_assign_count(),
                            move.allocator().move_construct_count(),
                            moved.allocator().allocation_count(),
                            moved.allocator().free_count(),
                            moved.allocator().move_assign_count(),
                            moved.allocator().move_construct_count()
                        };
                    };

                    // Non constant
                    {
                        const auto result = test_default_allocator({0, 1, 2, 3}, 1u);
                        // Ensure we copy all datas in order
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), 4u);
                        hud_assert_eq(std::get<2>(result), 4u + 1u + extra);
                        hud_assert_true(std::get<3>(result));

                        // Ensure the copy data is not the same memory of the copied data
                        hud_assert_true(std::get<4>(result));

                        // Ensure we are allocating only one time of not
                        // Always reallocate if types are non bitwise copy constructible types
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                        hud_assert_eq(std::get<7>(result), 0u);
                        hud_assert_eq(std::get<8>(result), 0u);
                        hud_assert_eq(std::get<9>(result), 1u);
                        hud_assert_eq(std::get<10>(result), 1u);
                        hud_assert_eq(std::get<11>(result), 0u);
                        hud_assert_eq(std::get<12>(result), 0u);
                    }

                    // Constant
                    {
                        constexpr auto result = test_default_allocator({0, 1, 2, 3}, 1u);
                        // Ensure we copy all datas in order
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), 4u);
                        hud_assert_eq(std::get<2>(result), 4u + 1u + extra);
                        hud_assert_true(std::get<3>(result));

                        // Ensure the copy data is not the same memory of the copied data
                        hud_assert_true(std::get<4>(result));

                        // Ensure we are allocating only one time of not
                        // Always reallocate if types are non bitwise copy constructible types
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                        hud_assert_eq(std::get<7>(result), 0u);
                        hud_assert_eq(std::get<8>(result), 0u);
                        hud_assert_eq(std::get<9>(result), 1u);
                        hud_assert_eq(std::get<10>(result), 1u);
                        hud_assert_eq(std::get<11>(result), 0u);
                        hud_assert_eq(std::get<12>(result), 0u);
                    }
                }

                // Test with allocator
                {
                    auto test_with_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
                    {
                        MovedType moved(initializer, copied_extra);

                        // Copy the array
                        hud::array<type, AllocatorType2> move(std::move(moved), extra, AllocatorType2 {});

                        // Ensure we moved all datas in order
                        bool all_values_moved = true;
                        for (usize index = 0; index < initializer.size(); index++)
                        {
                            if (static_cast<usize>(move[index].id()) != index)
                            {
                                all_values_moved = false;
                                break;
                            }
                            if (move[index].move_constructor_count() != 1u)
                            {
                                all_values_moved = false;
                                break;
                            }
                        }

                        return std::tuple {
                            // Ensure we copy all datas in order
                            move.data() != nullptr,
                            move.count(),
                            move.max_count(),
                            all_values_moved,

                            // Ensure the copy data is not the same memory of the copied data
                            (void *)moved.data() != (void *)move.data(),

                            // Ensure we are allocating only one time
                            move.allocator().allocation_count(),
                            move.allocator().free_count(),
                            move.allocator().move_assign_count(),
                            move.allocator().move_construct_count(),
                            moved.allocator().allocation_count(),
                            moved.allocator().free_count(),
                            moved.allocator().move_assign_count(),
                            moved.allocator().move_construct_count()
                        };
                    };

                    // Non constant
                    {
                        const auto result = test_with_allocator({0, 1, 2, 3}, 1u);
                        // Ensure we copy all datas in order
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), 4u);
                        hud_assert_eq(std::get<2>(result), 4u + 1u + extra);
                        hud_assert_true(std::get<3>(result));

                        // Ensure the copy data is not the same memory of the copied data
                        hud_assert_true(std::get<4>(result));

                        // Ensure we are allocating only one time of not
                        // Always reallocate if types are non bitwise copy constructible types
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                        hud_assert_eq(std::get<7>(result), 0u);
                        hud_assert_eq(std::get<8>(result), 0u);
                        hud_assert_eq(std::get<9>(result), 1u);
                        hud_assert_eq(std::get<10>(result), 1u);
                        hud_assert_eq(std::get<11>(result), 0u);
                        hud_assert_eq(std::get<12>(result), 0u);
                    }

                    // Constant
                    {
                        constexpr auto result = test_with_allocator({0, 1, 2, 3}, 1u);
                        // Ensure we copy all datas in order
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), 4u);
                        hud_assert_eq(std::get<2>(result), 4u + 1u + extra);
                        hud_assert_true(std::get<3>(result));

                        // Ensure the copy data is not the same memory of the copied data
                        hud_assert_true(std::get<4>(result));

                        // Ensure we are allocating only one time of not
                        // Always reallocate if types are non bitwise copy constructible types
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                        hud_assert_eq(std::get<7>(result), 0u);
                        hud_assert_eq(std::get<8>(result), 0u);
                        hud_assert_eq(std::get<9>(result), 1u);
                        hud_assert_eq(std::get<10>(result), 1u);
                        hud_assert_eq(std::get<11>(result), 0u);
                        hud_assert_eq(std::get<12>(result), 0u);
                    }
                }
            }
        );
    }
}

GTEST_TEST(array, move_construct_non_bitwise_move_constructible_different_type_same_allocator)
{

    /** The array we move for test, we allocate also extra memory to test if we really move the count(), not the max_count() elements */
    using Type1 = hud_test::non_bitwise_move_constructible_type;
    using Type2 = hud_test::non_bitwise_move_constructible_type2;
    using AllocatorType = hud_test::allocator_watcher<alignof(Type1)>;

    // Ensure we test with different allocator and types
    static_assert(!std::is_same_v<Type1, Type2>);
    static_assert(!hud::is_bitwise_move_constructible_v<Type2, Type1>);

    using MovedType = hud::array<Type1, AllocatorType>;
    // Test default allocator
    {
        auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
        {
            MovedType moved(initializer, copied_extra);

            // Copy the array
            hud::array<Type2, AllocatorType> move(std::move(moved));

            // Ensure we moved all datas in order
            bool all_values_moved = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                if (static_cast<usize>(move[index].id()) != index)
                {
                    all_values_moved = false;
                    break;
                }
                if (move[index].move_constructor_count() != 1u)
                {
                    all_values_moved = false;
                    break;
                }
            }

            return std::tuple {
                // Ensure we copy all datas in order
                move.data() != nullptr,
                move.count(),
                move.max_count(),
                all_values_moved,

                // Ensure the copy data is not the same memory of the copied data
                (void *)moved.data() != (void *)move.data(),

                // Ensure we are allocating only one time
                move.allocator().allocation_count(),
                move.allocator().free_count(),
                move.allocator().move_assign_count(),
                move.allocator().move_construct_count(),
                moved.allocator().allocation_count(),
                moved.allocator().free_count(),
                moved.allocator().move_assign_count(),
                moved.allocator().move_construct_count()
            };
        };

        // Non constant
        {
            const auto result = test_default_allocator({0, 1, 2, 3}, 1u);
            // Ensure we copy all datas in order
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), 4u);
            hud_assert_eq(std::get<2>(result), 4u + 1u);
            hud_assert_true(std::get<3>(result));

            // Ensure the copy data is not the same memory of the copied data
            hud_assert_true(std::get<4>(result));

            // Ensure we are allocating only one time of not
            // Always reallocate if types are non bitwise copy constructible types
            hud_assert_eq(std::get<5>(result), 1u);
            hud_assert_eq(std::get<6>(result), 0u);
            hud_assert_eq(std::get<7>(result), 0u);
            hud_assert_eq(std::get<8>(result), 0u);
            hud_assert_eq(std::get<9>(result), 1u);
            hud_assert_eq(std::get<10>(result), 1u);
            hud_assert_eq(std::get<11>(result), 0u);
            hud_assert_eq(std::get<12>(result), 0u);
        }

        // Constant
        {
            constexpr auto result = test_default_allocator({0, 1, 2, 3}, 1u);
            // Ensure we copy all datas in order
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), 4u);
            hud_assert_eq(std::get<2>(result), 4u + 1u);
            hud_assert_true(std::get<3>(result));

            // Ensure the copy data is not the same memory of the copied data
            hud_assert_true(std::get<4>(result));

            // Ensure we are allocating only one time of not
            // Always reallocate if types are non bitwise copy constructible types
            hud_assert_eq(std::get<5>(result), 1u);
            hud_assert_eq(std::get<6>(result), 0u);
            hud_assert_eq(std::get<7>(result), 0u);
            hud_assert_eq(std::get<8>(result), 0u);
            hud_assert_eq(std::get<9>(result), 1u);
            hud_assert_eq(std::get<10>(result), 1u);
            hud_assert_eq(std::get<11>(result), 0u);
            hud_assert_eq(std::get<12>(result), 0u);
        }
    }

    // Test wtih allocator
    {
        auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
        {
            MovedType moved(initializer, copied_extra);

            // Copy the array
            hud::array<Type2, AllocatorType> move(std::move(moved), AllocatorType {});

            // Ensure we moved all datas in order
            bool all_values_moved = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                if (static_cast<usize>(move[index].id()) != index)
                {
                    all_values_moved = false;
                    break;
                }
                if (move[index].move_constructor_count() != 1u)
                {
                    all_values_moved = false;
                    break;
                }
            }

            return std::tuple {
                // Ensure we copy all datas in order
                move.data() != nullptr,
                move.count(),
                move.max_count(),
                all_values_moved,

                // Ensure the copy data is not the same memory of the copied data
                (void *)moved.data() != (void *)move.data(),

                // Ensure we are allocating only one time
                move.allocator().allocation_count(),
                move.allocator().free_count(),
                move.allocator().move_assign_count(),
                move.allocator().move_construct_count(),
                moved.allocator().allocation_count(),
                moved.allocator().free_count(),
                moved.allocator().move_assign_count(),
                moved.allocator().move_construct_count()
            };
        };

        // Non constant
        {
            const auto result = test_default_allocator({0, 1, 2, 3}, 1u);
            // Ensure we copy all datas in order
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), 4u);
            hud_assert_eq(std::get<2>(result), 4u + 1u);
            hud_assert_true(std::get<3>(result));

            // Ensure the copy data is not the same memory of the copied data
            hud_assert_true(std::get<4>(result));

            // Ensure we are allocating only one time of not
            // Always reallocate if types are non bitwise copy constructible types
            hud_assert_eq(std::get<5>(result), 1u);
            hud_assert_eq(std::get<6>(result), 0u);
            hud_assert_eq(std::get<7>(result), 0u);
            hud_assert_eq(std::get<8>(result), 0u);
            hud_assert_eq(std::get<9>(result), 1u);
            hud_assert_eq(std::get<10>(result), 1u);
            hud_assert_eq(std::get<11>(result), 0u);
            hud_assert_eq(std::get<12>(result), 0u);
        }

        // Constant
        {
            constexpr auto result = test_default_allocator({0, 1, 2, 3}, 1u);
            // Ensure we copy all datas in order
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), 4u);
            hud_assert_eq(std::get<2>(result), 4u + 1u);
            hud_assert_true(std::get<3>(result));

            // Ensure the copy data is not the same memory of the copied data
            hud_assert_true(std::get<4>(result));

            // Ensure we are allocating only one time of not
            // Always reallocate if types are non bitwise copy constructible types
            hud_assert_eq(std::get<5>(result), 1u);
            hud_assert_eq(std::get<6>(result), 0u);
            hud_assert_eq(std::get<7>(result), 0u);
            hud_assert_eq(std::get<8>(result), 0u);
            hud_assert_eq(std::get<9>(result), 1u);
            hud_assert_eq(std::get<10>(result), 1u);
            hud_assert_eq(std::get<11>(result), 0u);
            hud_assert_eq(std::get<12>(result), 0u);
        }
    }

    // Test with extra
    {
        hud_test::for_each_value<std::make_integer_sequence<usize, 5>>()(
            []<usize extra>()
            {
                // Test default allocator
                {
                    auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
                    {
                        MovedType moved(initializer, copied_extra);

                        // Copy the array
                        hud::array<Type2, AllocatorType> move(std::move(moved), extra);

                        // Ensure we moved all datas in order
                        bool all_values_moved = true;
                        for (usize index = 0; index < initializer.size(); index++)
                        {
                            if (static_cast<usize>(move[index].id()) != index)
                            {
                                all_values_moved = false;
                                break;
                            }
                            if (move[index].move_constructor_count() != 1u)
                            {
                                all_values_moved = false;
                                break;
                            }
                        }

                        return std::tuple {
                            // Ensure we copy all datas in order
                            move.data() != nullptr,
                            move.count(),
                            move.max_count(),
                            all_values_moved,

                            // Ensure the copy data is not the same memory of the copied data
                            (void *)moved.data() != (void *)move.data(),

                            // Ensure we are allocating only one time
                            move.allocator().allocation_count(),
                            move.allocator().free_count(),
                            move.allocator().move_assign_count(),
                            move.allocator().move_construct_count(),
                            moved.allocator().allocation_count(),
                            moved.allocator().free_count(),
                            moved.allocator().move_assign_count(),
                            moved.allocator().move_construct_count()
                        };
                    };

                    // Non constant
                    {
                        const auto result = test_default_allocator({0, 1, 2, 3}, 1u);
                        // Ensure we copy all datas in order
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), 4u);
                        hud_assert_eq(std::get<2>(result), 4u + 1u + extra);
                        hud_assert_true(std::get<3>(result));

                        // Ensure the copy data is not the same memory of the copied data
                        hud_assert_true(std::get<4>(result));

                        // Ensure we are allocating only one time of not
                        // Always reallocate if types are non bitwise copy constructible types
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                        hud_assert_eq(std::get<7>(result), 0u);
                        hud_assert_eq(std::get<8>(result), 0u);
                        hud_assert_eq(std::get<9>(result), 1u);
                        hud_assert_eq(std::get<10>(result), 1u);
                        hud_assert_eq(std::get<11>(result), 0u);
                        hud_assert_eq(std::get<12>(result), 0u);
                    }

                    // Constant
                    {
                        constexpr auto result = test_default_allocator({0, 1, 2, 3}, 1u);
                        // Ensure we copy all datas in order
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), 4u);
                        hud_assert_eq(std::get<2>(result), 4u + 1u + extra);
                        hud_assert_true(std::get<3>(result));

                        // Ensure the copy data is not the same memory of the copied data
                        hud_assert_true(std::get<4>(result));

                        // Ensure we are allocating only one time of not
                        // Always reallocate if types are non bitwise copy constructible types
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                        hud_assert_eq(std::get<7>(result), 0u);
                        hud_assert_eq(std::get<8>(result), 0u);
                        hud_assert_eq(std::get<9>(result), 1u);
                        hud_assert_eq(std::get<10>(result), 1u);
                        hud_assert_eq(std::get<11>(result), 0u);
                        hud_assert_eq(std::get<12>(result), 0u);
                    }
                }

                // Test wtih allocator
                {
                    auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
                    {
                        MovedType moved(initializer, copied_extra);

                        // Copy the array
                        hud::array<Type2, AllocatorType> move(std::move(moved), extra, AllocatorType {});

                        // Ensure we moved all datas in order
                        bool all_values_moved = true;
                        for (usize index = 0; index < initializer.size(); index++)
                        {
                            if (static_cast<usize>(move[index].id()) != index)
                            {
                                all_values_moved = false;
                                break;
                            }
                            if (move[index].move_constructor_count() != 1u)
                            {
                                all_values_moved = false;
                                break;
                            }
                        }

                        return std::tuple {
                            // Ensure we copy all datas in order
                            move.data() != nullptr,
                            move.count(),
                            move.max_count(),
                            all_values_moved,

                            // Ensure the copy data is not the same memory of the copied data
                            (void *)moved.data() != (void *)move.data(),

                            // Ensure we are allocating only one time
                            move.allocator().allocation_count(),
                            move.allocator().free_count(),
                            move.allocator().move_assign_count(),
                            move.allocator().move_construct_count(),
                            moved.allocator().allocation_count(),
                            moved.allocator().free_count(),
                            moved.allocator().move_assign_count(),
                            moved.allocator().move_construct_count()
                        };
                    };

                    // Non constant
                    {
                        const auto result = test_default_allocator({0, 1, 2, 3}, 1u);
                        // Ensure we copy all datas in order
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), 4u);
                        hud_assert_eq(std::get<2>(result), 4u + 1u + extra);
                        hud_assert_true(std::get<3>(result));

                        // Ensure the copy data is not the same memory of the copied data
                        hud_assert_true(std::get<4>(result));

                        // Ensure we are allocating only one time of not
                        // Always reallocate if types are non bitwise copy constructible types
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                        hud_assert_eq(std::get<7>(result), 0u);
                        hud_assert_eq(std::get<8>(result), 0u);
                        hud_assert_eq(std::get<9>(result), 1u);
                        hud_assert_eq(std::get<10>(result), 1u);
                        hud_assert_eq(std::get<11>(result), 0u);
                        hud_assert_eq(std::get<12>(result), 0u);
                    }

                    // Constant
                    {
                        constexpr auto result = test_default_allocator({0, 1, 2, 3}, 1u);
                        // Ensure we copy all datas in order
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), 4u);
                        hud_assert_eq(std::get<2>(result), 4u + 1u + extra);
                        hud_assert_true(std::get<3>(result));

                        // Ensure the copy data is not the same memory of the copied data
                        hud_assert_true(std::get<4>(result));

                        // Ensure we are allocating only one time of not
                        // Always reallocate if types are non bitwise copy constructible types
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                        hud_assert_eq(std::get<7>(result), 0u);
                        hud_assert_eq(std::get<8>(result), 0u);
                        hud_assert_eq(std::get<9>(result), 1u);
                        hud_assert_eq(std::get<10>(result), 1u);
                        hud_assert_eq(std::get<11>(result), 0u);
                        hud_assert_eq(std::get<12>(result), 0u);
                    }
                }
            }
        );
    }
}

GTEST_TEST(array, move_construct_non_bitwise_move_constructible_different_type_different_allocator)
{

    /** The array we move for test, we allocate also extra memory to test if we really move the count(), not the max_count() elements */
    using Type1 = hud_test::non_bitwise_move_constructible_type;
    using Type2 = hud_test::non_bitwise_move_constructible_type2;
    using AllocatorType = hud_test::allocator_watcher<alignof(Type1)>;
    using AllocatorType2 = hud_test::allocator_watcher_2<alignof(Type2)>;

    // Ensure we test with different allocator and types
    static_assert(!std::is_same_v<AllocatorType, AllocatorType2>);
    static_assert(!std::is_same_v<Type1, Type2>);
    static_assert(!hud::is_bitwise_move_constructible_v<Type2, Type1>);

    using MovedType = hud::array<Type1, AllocatorType>;

    // Test default allocator
    {
        auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
        {
            MovedType moved(initializer, copied_extra);

            // Copy the array
            hud::array<Type2, AllocatorType2> move(std::move(moved));

            // Ensure we moved all datas in order
            bool all_values_moved = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                if (static_cast<usize>(move[index].id()) != index)
                {
                    all_values_moved = false;
                    break;
                }
                if (move[index].move_constructor_count() != 1u)
                {
                    all_values_moved = false;
                    break;
                }
            }

            return std::tuple {
                // Ensure we copy all datas in order
                move.data() != nullptr,
                move.count(),
                move.max_count(),
                all_values_moved,

                // Ensure the copy data is not the same memory of the copied data
                (void *)moved.data() != (void *)move.data(),

                // Ensure we are allocating only one time
                move.allocator().allocation_count(),
                move.allocator().free_count(),
                move.allocator().move_assign_count(),
                move.allocator().move_construct_count(),
                moved.allocator().allocation_count(),
                moved.allocator().free_count(),
                moved.allocator().move_assign_count(),
                moved.allocator().move_construct_count()
            };
        };

        // Non constant
        {
            const auto result = test_default_allocator({0, 1, 2, 3}, 1u);
            // Ensure we copy all datas in order
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), 4u);
            hud_assert_eq(std::get<2>(result), 4u + 1u);
            hud_assert_true(std::get<3>(result));

            // Ensure the copy data is not the same memory of the copied data
            hud_assert_true(std::get<4>(result));

            // Ensure we are allocating only one time of not
            // Always reallocate if types are non bitwise copy constructible types
            hud_assert_eq(std::get<5>(result), 1u);
            hud_assert_eq(std::get<6>(result), 0u);
            hud_assert_eq(std::get<7>(result), 0u);
            hud_assert_eq(std::get<8>(result), 0u);
            hud_assert_eq(std::get<9>(result), 1u);
            hud_assert_eq(std::get<10>(result), 1u);
            hud_assert_eq(std::get<11>(result), 0u);
            hud_assert_eq(std::get<12>(result), 0u);
        }

        // Constant
        {
            constexpr auto result = test_default_allocator({0, 1, 2, 3}, 1u);
            // Ensure we copy all datas in order
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), 4u);
            hud_assert_eq(std::get<2>(result), 4u + 1u);
            hud_assert_true(std::get<3>(result));

            // Ensure the copy data is not the same memory of the copied data
            hud_assert_true(std::get<4>(result));

            // Ensure we are allocating only one time of not
            // Always reallocate if types are non bitwise copy constructible types
            hud_assert_eq(std::get<5>(result), 1u);
            hud_assert_eq(std::get<6>(result), 0u);
            hud_assert_eq(std::get<7>(result), 0u);
            hud_assert_eq(std::get<8>(result), 0u);
            hud_assert_eq(std::get<9>(result), 1u);
            hud_assert_eq(std::get<10>(result), 1u);
            hud_assert_eq(std::get<11>(result), 0u);
            hud_assert_eq(std::get<12>(result), 0u);
        }
    }

    // Test wtih allocator
    {
        auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
        {
            MovedType moved(initializer, copied_extra);

            // Copy the array
            hud::array<Type2, AllocatorType2> move(std::move(moved), AllocatorType2 {});

            // Ensure we moved all datas in order
            bool all_values_moved = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                if (static_cast<usize>(move[index].id()) != index)
                {
                    all_values_moved = false;
                    break;
                }
                if (move[index].move_constructor_count() != 1u)
                {
                    all_values_moved = false;
                    break;
                }
            }

            return std::tuple {
                // Ensure we copy all datas in order
                move.data() != nullptr,
                move.count(),
                move.max_count(),
                all_values_moved,

                // Ensure the copy data is not the same memory of the copied data
                (void *)moved.data() != (void *)move.data(),

                // Ensure we are allocating only one time
                move.allocator().allocation_count(),
                move.allocator().free_count(),
                move.allocator().move_assign_count(),
                move.allocator().move_construct_count(),
                moved.allocator().allocation_count(),
                moved.allocator().free_count(),
                moved.allocator().move_assign_count(),
                moved.allocator().move_construct_count()
            };
        };

        // Non constant
        {
            const auto result = test_default_allocator({0, 1, 2, 3}, 1u);
            // Ensure we copy all datas in order
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), 4u);
            hud_assert_eq(std::get<2>(result), 4u + 1u);
            hud_assert_true(std::get<3>(result));

            // Ensure the copy data is not the same memory of the copied data
            hud_assert_true(std::get<4>(result));

            // Ensure we are allocating only one time of not
            // Always reallocate if types are non bitwise copy constructible types
            hud_assert_eq(std::get<5>(result), 1u);
            hud_assert_eq(std::get<6>(result), 0u);
            hud_assert_eq(std::get<7>(result), 0u);
            hud_assert_eq(std::get<8>(result), 0u);
            hud_assert_eq(std::get<9>(result), 1u);
            hud_assert_eq(std::get<10>(result), 1u);
            hud_assert_eq(std::get<11>(result), 0u);
            hud_assert_eq(std::get<12>(result), 0u);
        }

        // Constant
        {
            constexpr auto result = test_default_allocator({0, 1, 2, 3}, 1u);
            // Ensure we copy all datas in order
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), 4u);
            hud_assert_eq(std::get<2>(result), 4u + 1u);
            hud_assert_true(std::get<3>(result));

            // Ensure the copy data is not the same memory of the copied data
            hud_assert_true(std::get<4>(result));

            // Ensure we are allocating only one time of not
            // Always reallocate if types are non bitwise copy constructible types
            hud_assert_eq(std::get<5>(result), 1u);
            hud_assert_eq(std::get<6>(result), 0u);
            hud_assert_eq(std::get<7>(result), 0u);
            hud_assert_eq(std::get<8>(result), 0u);
            hud_assert_eq(std::get<9>(result), 1u);
            hud_assert_eq(std::get<10>(result), 1u);
            hud_assert_eq(std::get<11>(result), 0u);
            hud_assert_eq(std::get<12>(result), 0u);
        }
    }

    // Test with extra
    {
        hud_test::for_each_value<std::make_integer_sequence<usize, 5>>()(
            []<usize extra>()
            {
                // Test default allocator
                {
                    auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
                    {
                        MovedType moved(initializer, copied_extra);

                        // Copy the array
                        hud::array<Type2, AllocatorType2> move(std::move(moved), extra);

                        // Ensure we moved all datas in order
                        bool all_values_moved = true;
                        for (usize index = 0; index < initializer.size(); index++)
                        {
                            if (static_cast<usize>(move[index].id()) != index)
                            {
                                all_values_moved = false;
                                break;
                            }
                            if (move[index].move_constructor_count() != 1u)
                            {
                                all_values_moved = false;
                                break;
                            }
                        }

                        return std::tuple {
                            // Ensure we copy all datas in order
                            move.data() != nullptr,
                            move.count(),
                            move.max_count(),
                            all_values_moved,

                            // Ensure the copy data is not the same memory of the copied data
                            (void *)moved.data() != (void *)move.data(),

                            // Ensure we are allocating only one time
                            move.allocator().allocation_count(),
                            move.allocator().free_count(),
                            move.allocator().move_assign_count(),
                            move.allocator().move_construct_count(),
                            moved.allocator().allocation_count(),
                            moved.allocator().free_count(),
                            moved.allocator().move_assign_count(),
                            moved.allocator().move_construct_count()
                        };
                    };

                    // Non constant
                    {
                        const auto result = test_default_allocator({0, 1, 2, 3}, 1u);
                        // Ensure we copy all datas in order
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), 4u);
                        hud_assert_eq(std::get<2>(result), 4u + 1u + extra);
                        hud_assert_true(std::get<3>(result));

                        // Ensure the copy data is not the same memory of the copied data
                        hud_assert_true(std::get<4>(result));

                        // Ensure we are allocating only one time of not
                        // Always reallocate if types are non bitwise copy constructible types
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                        hud_assert_eq(std::get<7>(result), 0u);
                        hud_assert_eq(std::get<8>(result), 0u);
                        hud_assert_eq(std::get<9>(result), 1u);
                        hud_assert_eq(std::get<10>(result), 1u);
                        hud_assert_eq(std::get<11>(result), 0u);
                        hud_assert_eq(std::get<12>(result), 0u);
                    }

                    // Constant
                    {
                        constexpr auto result = test_default_allocator({0, 1, 2, 3}, 1u);
                        // Ensure we copy all datas in order
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), 4u);
                        hud_assert_eq(std::get<2>(result), 4u + 1u + extra);
                        hud_assert_true(std::get<3>(result));

                        // Ensure the copy data is not the same memory of the copied data
                        hud_assert_true(std::get<4>(result));

                        // Ensure we are allocating only one time of not
                        // Always reallocate if types are non bitwise copy constructible types
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                        hud_assert_eq(std::get<7>(result), 0u);
                        hud_assert_eq(std::get<8>(result), 0u);
                        hud_assert_eq(std::get<9>(result), 1u);
                        hud_assert_eq(std::get<10>(result), 1u);
                        hud_assert_eq(std::get<11>(result), 0u);
                        hud_assert_eq(std::get<12>(result), 0u);
                    }
                }

                // Test wtih allocator
                {
                    auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
                    {
                        MovedType moved(initializer, copied_extra);

                        // Copy the array
                        hud::array<Type2, AllocatorType2> move(std::move(moved), extra, AllocatorType2 {});

                        // Ensure we moved all datas in order
                        bool all_values_moved = true;
                        for (usize index = 0; index < initializer.size(); index++)
                        {
                            if (static_cast<usize>(move[index].id()) != index)
                            {
                                all_values_moved = false;
                                break;
                            }
                            if (move[index].move_constructor_count() != 1u)
                            {
                                all_values_moved = false;
                                break;
                            }
                        }

                        return std::tuple {
                            // Ensure we copy all datas in order
                            move.data() != nullptr,
                            move.count(),
                            move.max_count(),
                            all_values_moved,

                            // Ensure the copy data is not the same memory of the copied data
                            (void *)moved.data() != (void *)move.data(),

                            // Ensure we are allocating only one time
                            move.allocator().allocation_count(),
                            move.allocator().free_count(),
                            move.allocator().move_assign_count(),
                            move.allocator().move_construct_count(),
                            moved.allocator().allocation_count(),
                            moved.allocator().free_count(),
                            moved.allocator().move_assign_count(),
                            moved.allocator().move_construct_count()
                        };
                    };

                    // Non constant
                    {
                        const auto result = test_default_allocator({0, 1, 2, 3}, 1u);
                        // Ensure we copy all datas in order
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), 4u);
                        hud_assert_eq(std::get<2>(result), 4u + 1u + extra);
                        hud_assert_true(std::get<3>(result));

                        // Ensure the copy data is not the same memory of the copied data
                        hud_assert_true(std::get<4>(result));

                        // Ensure we are allocating only one time of not
                        // Always reallocate if types are non bitwise copy constructible types
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                        hud_assert_eq(std::get<7>(result), 0u);
                        hud_assert_eq(std::get<8>(result), 0u);
                        hud_assert_eq(std::get<9>(result), 1u);
                        hud_assert_eq(std::get<10>(result), 1u);
                        hud_assert_eq(std::get<11>(result), 0u);
                        hud_assert_eq(std::get<12>(result), 0u);
                    }

                    // Constant
                    {
                        constexpr auto result = test_default_allocator({0, 1, 2, 3}, 1u);
                        // Ensure we copy all datas in order
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), 4u);
                        hud_assert_eq(std::get<2>(result), 4u + 1u + extra);
                        hud_assert_true(std::get<3>(result));

                        // Ensure the copy data is not the same memory of the copied data
                        hud_assert_true(std::get<4>(result));

                        // Ensure we are allocating only one time of not
                        // Always reallocate if types are non bitwise copy constructible types
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                        hud_assert_eq(std::get<7>(result), 0u);
                        hud_assert_eq(std::get<8>(result), 0u);
                        hud_assert_eq(std::get<9>(result), 1u);
                        hud_assert_eq(std::get<10>(result), 1u);
                        hud_assert_eq(std::get<11>(result), 0u);
                        hud_assert_eq(std::get<12>(result), 0u);
                    }
                }
            }
        );
    }
}

GTEST_TEST(array, move_construct_non_bitwise_same_type_same_allocator)
{

    /** The array we copy for test, we allocate also extra memory to test if we really copy the count(), not the max_count() elements */
    using type = hud_test::non_bitwise_type;
    using AllocatorType = hud_test::allocator_watcher<alignof(type)>;

    // Ensure we test with different allocator
    static_assert(!hud::is_bitwise_move_constructible_v<type>);

    using MovedType = hud::array<type, AllocatorType>;

    // Test default allocator
    {
        auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
        {
            MovedType moved(initializer, copied_extra);

            // Copy the array
            hud::array<type, AllocatorType> move(std::move(moved));

            // Ensure we moved all datas in order
            bool all_values_moved = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                if (static_cast<usize>(move[index].id()) != index)
                {
                    all_values_moved = false;
                    break;
                }
                if (move[index].move_constructor_count() != 1u)
                {
                    all_values_moved = false;
                    break;
                }
                if (move[index].copy_constructor_count() != 0u)
                {
                    all_values_moved = false;
                    break;
                }
            }

            return std::tuple {
                // Ensure we copy all datas in order
                move.data() != nullptr,
                move.count(),
                move.max_count(),
                all_values_moved,

                // Ensure the copy data is not the same memory of the copied data
                (void *)moved.data() != (void *)move.data(),

                // Ensure we are allocating only one time
                move.allocator().allocation_count(),
                move.allocator().free_count(),
                move.allocator().move_assign_count(),
                move.allocator().move_construct_count(),
                moved.allocator().allocation_count(),
                moved.allocator().free_count(),
                moved.allocator().move_assign_count(),
                moved.allocator().move_construct_count()
            };
        };

        // Non constant
        {
            const auto result = test_default_allocator({0, 1, 2, 3}, 1u);
            // Ensure we copy all datas in order
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), 4u);
            hud_assert_eq(std::get<2>(result), 4u + 1u);
            hud_assert_true(std::get<3>(result));

            // Ensure the copy data is not the same memory of the copied data
            hud_assert_true(std::get<4>(result));

            // Ensure we are allocating only one time of not
            // Always reallocate if types are non bitwise copy constructible types
            hud_assert_eq(std::get<5>(result), 1u);
            hud_assert_eq(std::get<6>(result), 0u);
            hud_assert_eq(std::get<7>(result), 0u);
            hud_assert_eq(std::get<8>(result), 0u);
            hud_assert_eq(std::get<9>(result), 1u);
            hud_assert_eq(std::get<10>(result), 1u);
            hud_assert_eq(std::get<11>(result), 0u);
            hud_assert_eq(std::get<12>(result), 0u);
        }

        // Constant
        {
            constexpr auto result = test_default_allocator({0, 1, 2, 3}, 1u);
            // Ensure we copy all datas in order
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), 4u);
            hud_assert_eq(std::get<2>(result), 4u + 1u);
            hud_assert_true(std::get<3>(result));

            // Ensure the copy data is not the same memory of the copied data
            hud_assert_true(std::get<4>(result));

            // Ensure we are allocating only one time of not
            // Always reallocate if types are non bitwise copy constructible types
            hud_assert_eq(std::get<5>(result), 1u);
            hud_assert_eq(std::get<6>(result), 0u);
            hud_assert_eq(std::get<7>(result), 0u);
            hud_assert_eq(std::get<8>(result), 0u);
            hud_assert_eq(std::get<9>(result), 1u);
            hud_assert_eq(std::get<10>(result), 1u);
            hud_assert_eq(std::get<11>(result), 0u);
            hud_assert_eq(std::get<12>(result), 0u);
        }
    }

    // Test with allocator
    {
        auto test_with_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
        {
            MovedType moved(initializer, copied_extra);

            // Copy the array
            hud::array<type, AllocatorType> move(std::move(moved), AllocatorType {});

            // Ensure we moved all datas in order
            bool all_values_moved = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                if (static_cast<usize>(move[index].id()) != index)
                {
                    all_values_moved = false;
                    break;
                }
                if (move[index].move_constructor_count() != 1u)
                {
                    all_values_moved = false;
                    break;
                }
                if (move[index].copy_constructor_count() != 0u)
                {
                    all_values_moved = false;
                    break;
                }
            }

            return std::tuple {
                // Ensure we copy all datas in order
                move.data() != nullptr,
                move.count(),
                move.max_count(),
                all_values_moved,

                // Ensure the copy data is not the same memory of the copied data
                (void *)moved.data() != (void *)move.data(),

                // Ensure we are allocating only one time
                move.allocator().allocation_count(),
                move.allocator().free_count(),
                move.allocator().move_assign_count(),
                move.allocator().move_construct_count(),
                moved.allocator().allocation_count(),
                moved.allocator().free_count(),
                moved.allocator().move_assign_count(),
                moved.allocator().move_construct_count()
            };
        };

        // Non constant
        {
            const auto result = test_with_allocator({0, 1, 2, 3}, 1u);
            // Ensure we copy all datas in order
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), 4u);
            hud_assert_eq(std::get<2>(result), 4u + 1u);
            hud_assert_true(std::get<3>(result));

            // Ensure the copy data is not the same memory of the copied data
            hud_assert_true(std::get<4>(result));

            // Ensure we are allocating only one time of not
            // Always reallocate if types are non bitwise copy constructible types
            hud_assert_eq(std::get<5>(result), 1u);
            hud_assert_eq(std::get<6>(result), 0u);
            hud_assert_eq(std::get<7>(result), 0u);
            hud_assert_eq(std::get<8>(result), 0u);
            hud_assert_eq(std::get<9>(result), 1u);
            hud_assert_eq(std::get<10>(result), 1u);
            hud_assert_eq(std::get<11>(result), 0u);
            hud_assert_eq(std::get<12>(result), 0u);
        }

        // Constant
        {
            constexpr auto result = test_with_allocator({0, 1, 2, 3}, 1u);
            // Ensure we copy all datas in order
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), 4u);
            hud_assert_eq(std::get<2>(result), 4u + 1u);
            hud_assert_true(std::get<3>(result));

            // Ensure the copy data is not the same memory of the copied data
            hud_assert_true(std::get<4>(result));

            // Ensure we are allocating only one time of not
            // Always reallocate if types are non bitwise copy constructible types
            hud_assert_eq(std::get<5>(result), 1u);
            hud_assert_eq(std::get<6>(result), 0u);
            hud_assert_eq(std::get<7>(result), 0u);
            hud_assert_eq(std::get<8>(result), 0u);
            hud_assert_eq(std::get<9>(result), 1u);
            hud_assert_eq(std::get<10>(result), 1u);
            hud_assert_eq(std::get<11>(result), 0u);
            hud_assert_eq(std::get<12>(result), 0u);
        }
    }

    // Test with extra
    {
        hud_test::for_each_value<std::make_integer_sequence<usize, 5>>()(
            []<usize extra>()
            {
                // Test default allocator
                {
                    auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
                    {
                        MovedType moved(initializer, copied_extra);

                        // Copy the array
                        hud::array<type, AllocatorType> move(std::move(moved), extra);

                        // Ensure we moved all datas in order
                        bool all_values_moved = true;
                        for (usize index = 0; index < initializer.size(); index++)
                        {
                            if (static_cast<usize>(move[index].id()) != index)
                            {
                                all_values_moved = false;
                                break;
                            }
                            if (move[index].move_constructor_count() != 1u)
                            {
                                all_values_moved = false;
                                break;
                            }
                            if (move[index].copy_constructor_count() != 0u)
                            {
                                all_values_moved = false;
                                break;
                            }
                        }

                        return std::tuple {
                            // Ensure we copy all datas in order
                            move.data() != nullptr,
                            move.count(),
                            move.max_count(),
                            all_values_moved,

                            // Ensure the copy data is not the same memory of the copied data
                            (void *)moved.data() != (void *)move.data(),

                            // Ensure we are allocating only one time
                            move.allocator().allocation_count(),
                            move.allocator().free_count(),
                            move.allocator().move_assign_count(),
                            move.allocator().move_construct_count(),
                            moved.allocator().allocation_count(),
                            moved.allocator().free_count(),
                            moved.allocator().move_assign_count(),
                            moved.allocator().move_construct_count()
                        };
                    };

                    // Non constant
                    {
                        const auto result = test_default_allocator({0, 1, 2, 3}, 1u);
                        // Ensure we copy all datas in order
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), 4u);
                        hud_assert_eq(std::get<2>(result), 4u + 1u + extra);
                        hud_assert_true(std::get<3>(result));

                        // Ensure the copy data is not the same memory of the copied data
                        hud_assert_true(std::get<4>(result));

                        // Ensure we are allocating only one time of not
                        // Always reallocate if types are non bitwise copy constructible types
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                        hud_assert_eq(std::get<7>(result), 0u);
                        hud_assert_eq(std::get<8>(result), 0u);
                        hud_assert_eq(std::get<9>(result), 1u);
                        hud_assert_eq(std::get<10>(result), 1u);
                        hud_assert_eq(std::get<11>(result), 0u);
                        hud_assert_eq(std::get<12>(result), 0u);
                    }

                    // Constant
                    {
                        constexpr auto result = test_default_allocator({0, 1, 2, 3}, 1u);
                        // Ensure we copy all datas in order
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), 4u);
                        hud_assert_eq(std::get<2>(result), 4u + 1u + extra);
                        hud_assert_true(std::get<3>(result));

                        // Ensure the copy data is not the same memory of the copied data
                        hud_assert_true(std::get<4>(result));

                        // Ensure we are allocating only one time of not
                        // Always reallocate if types are non bitwise copy constructible types
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                        hud_assert_eq(std::get<7>(result), 0u);
                        hud_assert_eq(std::get<8>(result), 0u);
                        hud_assert_eq(std::get<9>(result), 1u);
                        hud_assert_eq(std::get<10>(result), 1u);
                        hud_assert_eq(std::get<11>(result), 0u);
                        hud_assert_eq(std::get<12>(result), 0u);
                    }
                }

                // Test with allocator
                {
                    auto test_with_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
                    {
                        MovedType moved(initializer, copied_extra);

                        // Copy the array
                        hud::array<type, AllocatorType> move(std::move(moved), extra, AllocatorType {});

                        // Ensure we moved all datas in order
                        bool all_values_moved = true;
                        for (usize index = 0; index < initializer.size(); index++)
                        {
                            if (static_cast<usize>(move[index].id()) != index)
                            {
                                all_values_moved = false;
                                break;
                            }
                            if (move[index].move_constructor_count() != 1u)
                            {
                                all_values_moved = false;
                                break;
                            }
                            if (move[index].copy_constructor_count() != 0u)
                            {
                                all_values_moved = false;
                                break;
                            }
                        }

                        return std::tuple {
                            // Ensure we copy all datas in order
                            move.data() != nullptr,
                            move.count(),
                            move.max_count(),
                            all_values_moved,

                            // Ensure the copy data is not the same memory of the copied data
                            (void *)moved.data() != (void *)move.data(),

                            // Ensure we are allocating only one time
                            move.allocator().allocation_count(),
                            move.allocator().free_count(),
                            move.allocator().move_assign_count(),
                            move.allocator().move_construct_count(),
                            moved.allocator().allocation_count(),
                            moved.allocator().free_count(),
                            moved.allocator().move_assign_count(),
                            moved.allocator().move_construct_count()
                        };
                    };

                    // Non constant
                    {
                        const auto result = test_with_allocator({0, 1, 2, 3}, 1u);
                        // Ensure we copy all datas in order
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), 4u);
                        hud_assert_eq(std::get<2>(result), 4u + 1u + extra);
                        hud_assert_true(std::get<3>(result));

                        // Ensure the copy data is not the same memory of the copied data
                        hud_assert_true(std::get<4>(result));

                        // Ensure we are allocating only one time of not
                        // Always reallocate if types are non bitwise copy constructible types
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                        hud_assert_eq(std::get<7>(result), 0u);
                        hud_assert_eq(std::get<8>(result), 0u);
                        hud_assert_eq(std::get<9>(result), 1u);
                        hud_assert_eq(std::get<10>(result), 1u);
                        hud_assert_eq(std::get<11>(result), 0u);
                        hud_assert_eq(std::get<12>(result), 0u);
                    }

                    // Constant
                    {
                        constexpr auto result = test_with_allocator({0, 1, 2, 3}, 1u);
                        // Ensure we copy all datas in order
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), 4u);
                        hud_assert_eq(std::get<2>(result), 4u + 1u + extra);
                        hud_assert_true(std::get<3>(result));

                        // Ensure the copy data is not the same memory of the copied data
                        hud_assert_true(std::get<4>(result));

                        // Ensure we are allocating only one time of not
                        // Always reallocate if types are non bitwise copy constructible types
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                        hud_assert_eq(std::get<7>(result), 0u);
                        hud_assert_eq(std::get<8>(result), 0u);
                        hud_assert_eq(std::get<9>(result), 1u);
                        hud_assert_eq(std::get<10>(result), 1u);
                        hud_assert_eq(std::get<11>(result), 0u);
                        hud_assert_eq(std::get<12>(result), 0u);
                    }
                }
            }
        );
    }
}

GTEST_TEST(array, move_construct_non_bitwise_same_type_different_allocator)
{

    /** The array we copy for test, we allocate also extra memory to test if we really copy the count(), not the max_count() elements */
    using type = hud_test::non_bitwise_type;
    using AllocatorType = hud_test::allocator_watcher<alignof(type)>;
    using AllocatorType2 = hud_test::allocator_watcher_2<alignof(type)>;

    // Ensure we test with different allocator
    static_assert(!std::is_same_v<AllocatorType, AllocatorType2>);
    static_assert(!hud::is_bitwise_move_constructible_v<type>);

    using MovedType = hud::array<type, AllocatorType>;

    // Test default allocator
    {
        auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
        {
            MovedType moved(initializer, copied_extra);

            // Copy the array
            hud::array<type, AllocatorType2> move(std::move(moved));

            // Ensure we moved all datas in order
            bool all_values_moved = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                if (static_cast<usize>(move[index].id()) != index)
                {
                    all_values_moved = false;
                    break;
                }
                if (move[index].move_constructor_count() != 1u)
                {
                    all_values_moved = false;
                    break;
                }
                if (move[index].copy_constructor_count() != 0u)
                {
                    all_values_moved = false;
                    break;
                }
            }

            return std::tuple {
                // Ensure we copy all datas in order
                move.data() != nullptr,
                move.count(),
                move.max_count(),
                all_values_moved,

                // Ensure the copy data is not the same memory of the copied data
                (void *)moved.data() != (void *)move.data(),

                // Ensure we are allocating only one time
                move.allocator().allocation_count(),
                move.allocator().free_count(),
                move.allocator().move_assign_count(),
                move.allocator().move_construct_count(),
                moved.allocator().allocation_count(),
                moved.allocator().free_count(),
                moved.allocator().move_assign_count(),
                moved.allocator().move_construct_count()
            };
        };

        // Non constant
        {
            const auto result = test_default_allocator({0, 1, 2, 3}, 1u);
            // Ensure we copy all datas in order
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), 4u);
            hud_assert_eq(std::get<2>(result), 4u + 1u);
            hud_assert_true(std::get<3>(result));

            // Ensure the copy data is not the same memory of the copied data
            hud_assert_true(std::get<4>(result));

            // Ensure we are allocating only one time of not
            // Always reallocate if types are non bitwise copy constructible types
            hud_assert_eq(std::get<5>(result), 1u);
            hud_assert_eq(std::get<6>(result), 0u);
            hud_assert_eq(std::get<7>(result), 0u);
            hud_assert_eq(std::get<8>(result), 0u);
            hud_assert_eq(std::get<9>(result), 1u);
            hud_assert_eq(std::get<10>(result), 1u);
            hud_assert_eq(std::get<11>(result), 0u);
            hud_assert_eq(std::get<12>(result), 0u);
        }

        // Constant
        {
            constexpr auto result = test_default_allocator({0, 1, 2, 3}, 1u);
            // Ensure we copy all datas in order
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), 4u);
            hud_assert_eq(std::get<2>(result), 4u + 1u);
            hud_assert_true(std::get<3>(result));

            // Ensure the copy data is not the same memory of the copied data
            hud_assert_true(std::get<4>(result));

            // Ensure we are allocating only one time of not
            // Always reallocate if types are non bitwise copy constructible types
            hud_assert_eq(std::get<5>(result), 1u);
            hud_assert_eq(std::get<6>(result), 0u);
            hud_assert_eq(std::get<7>(result), 0u);
            hud_assert_eq(std::get<8>(result), 0u);
            hud_assert_eq(std::get<9>(result), 1u);
            hud_assert_eq(std::get<10>(result), 1u);
            hud_assert_eq(std::get<11>(result), 0u);
            hud_assert_eq(std::get<12>(result), 0u);
        }
    }

    // Test with allocator
    {
        auto test_with_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
        {
            MovedType moved(initializer, copied_extra);

            // Copy the array
            hud::array<type, AllocatorType2> move(std::move(moved), AllocatorType2 {});

            // Ensure we moved all datas in order
            bool all_values_moved = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                if (static_cast<usize>(move[index].id()) != index)
                {
                    all_values_moved = false;
                    break;
                }
                if (move[index].move_constructor_count() != 1u)
                {
                    all_values_moved = false;
                    break;
                }
                if (move[index].copy_constructor_count() != 0u)
                {
                    all_values_moved = false;
                    break;
                }
            }

            return std::tuple {
                // Ensure we copy all datas in order
                move.data() != nullptr,
                move.count(),
                move.max_count(),
                all_values_moved,

                // Ensure the copy data is not the same memory of the copied data
                (void *)moved.data() != (void *)move.data(),

                // Ensure we are allocating only one time
                move.allocator().allocation_count(),
                move.allocator().free_count(),
                move.allocator().move_assign_count(),
                move.allocator().move_construct_count(),
                moved.allocator().allocation_count(),
                moved.allocator().free_count(),
                moved.allocator().move_assign_count(),
                moved.allocator().move_construct_count()
            };
        };

        // Non constant
        {
            const auto result = test_with_allocator({0, 1, 2, 3}, 1u);
            // Ensure we copy all datas in order
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), 4u);
            hud_assert_eq(std::get<2>(result), 4u + 1u);
            hud_assert_true(std::get<3>(result));

            // Ensure the copy data is not the same memory of the copied data
            hud_assert_true(std::get<4>(result));

            // Ensure we are allocating only one time of not
            // Always reallocate if types are non bitwise copy constructible types
            hud_assert_eq(std::get<5>(result), 1u);
            hud_assert_eq(std::get<6>(result), 0u);
            hud_assert_eq(std::get<7>(result), 0u);
            hud_assert_eq(std::get<8>(result), 0u);
            hud_assert_eq(std::get<9>(result), 1u);
            hud_assert_eq(std::get<10>(result), 1u);
            hud_assert_eq(std::get<11>(result), 0u);
            hud_assert_eq(std::get<12>(result), 0u);
        }

        // Constant
        {
            constexpr auto result = test_with_allocator({0, 1, 2, 3}, 1u);
            // Ensure we copy all datas in order
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), 4u);
            hud_assert_eq(std::get<2>(result), 4u + 1u);
            hud_assert_true(std::get<3>(result));

            // Ensure the copy data is not the same memory of the copied data
            hud_assert_true(std::get<4>(result));

            // Ensure we are allocating only one time of not
            // Always reallocate if types are non bitwise copy constructible types
            hud_assert_eq(std::get<5>(result), 1u);
            hud_assert_eq(std::get<6>(result), 0u);
            hud_assert_eq(std::get<7>(result), 0u);
            hud_assert_eq(std::get<8>(result), 0u);
            hud_assert_eq(std::get<9>(result), 1u);
            hud_assert_eq(std::get<10>(result), 1u);
            hud_assert_eq(std::get<11>(result), 0u);
            hud_assert_eq(std::get<12>(result), 0u);
        }
    }

    // Test with extra
    {
        hud_test::for_each_value<std::make_integer_sequence<usize, 5>>()(
            []<usize extra>()
            {
                // Test default allocator
                {
                    auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
                    {
                        MovedType moved(initializer, copied_extra);

                        // Copy the array
                        hud::array<type, AllocatorType2> move(std::move(moved), extra);

                        // Ensure we moved all datas in order
                        bool all_values_moved = true;
                        for (usize index = 0; index < initializer.size(); index++)
                        {
                            if (static_cast<usize>(move[index].id()) != index)
                            {
                                all_values_moved = false;
                                break;
                            }
                            if (move[index].move_constructor_count() != 1u)
                            {
                                all_values_moved = false;
                                break;
                            }
                            if (move[index].copy_constructor_count() != 0u)
                            {
                                all_values_moved = false;
                                break;
                            }
                        }

                        return std::tuple {
                            // Ensure we copy all datas in order
                            move.data() != nullptr,
                            move.count(),
                            move.max_count(),
                            all_values_moved,

                            // Ensure the copy data is not the same memory of the copied data
                            (void *)moved.data() != (void *)move.data(),

                            // Ensure we are allocating only one time
                            move.allocator().allocation_count(),
                            move.allocator().free_count(),
                            move.allocator().move_assign_count(),
                            move.allocator().move_construct_count(),
                            moved.allocator().allocation_count(),
                            moved.allocator().free_count(),
                            moved.allocator().move_assign_count(),
                            moved.allocator().move_construct_count()
                        };
                    };

                    // Non constant
                    {
                        const auto result = test_default_allocator({0, 1, 2, 3}, 1u);
                        // Ensure we copy all datas in order
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), 4u);
                        hud_assert_eq(std::get<2>(result), 4u + 1u + extra);
                        hud_assert_true(std::get<3>(result));

                        // Ensure the copy data is not the same memory of the copied data
                        hud_assert_true(std::get<4>(result));

                        // Ensure we are allocating only one time of not
                        // Always reallocate if types are non bitwise copy constructible types
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                        hud_assert_eq(std::get<7>(result), 0u);
                        hud_assert_eq(std::get<8>(result), 0u);
                        hud_assert_eq(std::get<9>(result), 1u);
                        hud_assert_eq(std::get<10>(result), 1u);
                        hud_assert_eq(std::get<11>(result), 0u);
                        hud_assert_eq(std::get<12>(result), 0u);
                    }

                    // Constant
                    {
                        constexpr auto result = test_default_allocator({0, 1, 2, 3}, 1u);
                        // Ensure we copy all datas in order
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), 4u);
                        hud_assert_eq(std::get<2>(result), 4u + 1u + extra);
                        hud_assert_true(std::get<3>(result));

                        // Ensure the copy data is not the same memory of the copied data
                        hud_assert_true(std::get<4>(result));

                        // Ensure we are allocating only one time of not
                        // Always reallocate if types are non bitwise copy constructible types
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                        hud_assert_eq(std::get<7>(result), 0u);
                        hud_assert_eq(std::get<8>(result), 0u);
                        hud_assert_eq(std::get<9>(result), 1u);
                        hud_assert_eq(std::get<10>(result), 1u);
                        hud_assert_eq(std::get<11>(result), 0u);
                        hud_assert_eq(std::get<12>(result), 0u);
                    }
                }

                // Test with allocator
                {
                    auto test_with_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
                    {
                        MovedType moved(initializer, copied_extra);

                        // Copy the array
                        hud::array<type, AllocatorType2> move(std::move(moved), extra, AllocatorType2 {});

                        // Ensure we moved all datas in order
                        bool all_values_moved = true;
                        for (usize index = 0; index < initializer.size(); index++)
                        {
                            if (static_cast<usize>(move[index].id()) != index)
                            {
                                all_values_moved = false;
                                break;
                            }
                            if (move[index].move_constructor_count() != 1u)
                            {
                                all_values_moved = false;
                                break;
                            }
                            if (move[index].copy_constructor_count() != 0u)
                            {
                                all_values_moved = false;
                                break;
                            }
                        }

                        return std::tuple {
                            // Ensure we copy all datas in order
                            move.data() != nullptr,
                            move.count(),
                            move.max_count(),
                            all_values_moved,

                            // Ensure the copy data is not the same memory of the copied data
                            (void *)moved.data() != (void *)move.data(),

                            // Ensure we are allocating only one time
                            move.allocator().allocation_count(),
                            move.allocator().free_count(),
                            move.allocator().move_assign_count(),
                            move.allocator().move_construct_count(),
                            moved.allocator().allocation_count(),
                            moved.allocator().free_count(),
                            moved.allocator().move_assign_count(),
                            moved.allocator().move_construct_count()
                        };
                    };

                    // Non constant
                    {
                        const auto result = test_with_allocator({0, 1, 2, 3}, 1u);
                        // Ensure we copy all datas in order
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), 4u);
                        hud_assert_eq(std::get<2>(result), 4u + 1u + extra);
                        hud_assert_true(std::get<3>(result));

                        // Ensure the copy data is not the same memory of the copied data
                        hud_assert_true(std::get<4>(result));

                        // Ensure we are allocating only one time of not
                        // Always reallocate if types are non bitwise copy constructible types
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                        hud_assert_eq(std::get<7>(result), 0u);
                        hud_assert_eq(std::get<8>(result), 0u);
                        hud_assert_eq(std::get<9>(result), 1u);
                        hud_assert_eq(std::get<10>(result), 1u);
                        hud_assert_eq(std::get<11>(result), 0u);
                        hud_assert_eq(std::get<12>(result), 0u);
                    }

                    // Constant
                    {
                        constexpr auto result = test_with_allocator({0, 1, 2, 3}, 1u);
                        // Ensure we copy all datas in order
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), 4u);
                        hud_assert_eq(std::get<2>(result), 4u + 1u + extra);
                        hud_assert_true(std::get<3>(result));

                        // Ensure the copy data is not the same memory of the copied data
                        hud_assert_true(std::get<4>(result));

                        // Ensure we are allocating only one time of not
                        // Always reallocate if types are non bitwise copy constructible types
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                        hud_assert_eq(std::get<7>(result), 0u);
                        hud_assert_eq(std::get<8>(result), 0u);
                        hud_assert_eq(std::get<9>(result), 1u);
                        hud_assert_eq(std::get<10>(result), 1u);
                        hud_assert_eq(std::get<11>(result), 0u);
                        hud_assert_eq(std::get<12>(result), 0u);
                    }
                }
            }
        );
    }
}

GTEST_TEST(array, move_construct_non_bitwise_different_type_same_allocator)
{

    /** The array we move for test, we allocate also extra memory to test if we really move the count(), not the max_count() elements */
    using Type1 = hud_test::non_bitwise_type;
    using Type2 = hud_test::non_bitwise_type2;
    using AllocatorType = hud_test::allocator_watcher<alignof(Type1)>;

    // Ensure we test with different allocator and types
    static_assert(!std::is_same_v<Type1, Type2>);
    static_assert(!hud::is_bitwise_move_constructible_v<Type2, Type1>);

    using MovedType = hud::array<Type1, AllocatorType>;
    // Test default allocator
    {
        auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
        {
            MovedType moved(initializer, copied_extra);

            // Copy the array
            hud::array<Type2, AllocatorType> move(std::move(moved));

            // Ensure we moved all datas in order
            bool all_values_moved = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                if (static_cast<usize>(move[index].id()) != index)
                {
                    all_values_moved = false;
                    break;
                }
                if (move[index].move_constructor_count() != 1u)
                {
                    all_values_moved = false;
                    break;
                }
                if (move[index].copy_constructor_count() != 0u)
                {
                    all_values_moved = false;
                    break;
                }
            }

            return std::tuple {
                // Ensure we copy all datas in order
                move.data() != nullptr,
                move.count(),
                move.max_count(),
                all_values_moved,

                // Ensure the copy data is not the same memory of the copied data
                (void *)moved.data() != (void *)move.data(),

                // Ensure we are allocating only one time
                move.allocator().allocation_count(),
                move.allocator().free_count(),
                move.allocator().move_assign_count(),
                move.allocator().move_construct_count(),
                moved.allocator().allocation_count(),
                moved.allocator().free_count(),
                moved.allocator().move_assign_count(),
                moved.allocator().move_construct_count()
            };
        };

        // Non constant
        {
            const auto result = test_default_allocator({0, 1, 2, 3}, 1u);
            // Ensure we copy all datas in order
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), 4u);
            hud_assert_eq(std::get<2>(result), 4u + 1u);
            hud_assert_true(std::get<3>(result));

            // Ensure the copy data is not the same memory of the copied data
            hud_assert_true(std::get<4>(result));

            // Ensure we are allocating only one time of not
            // Always reallocate if types are non bitwise copy constructible types
            hud_assert_eq(std::get<5>(result), 1u);
            hud_assert_eq(std::get<6>(result), 0u);
            hud_assert_eq(std::get<7>(result), 0u);
            hud_assert_eq(std::get<8>(result), 0u);
            hud_assert_eq(std::get<9>(result), 1u);
            hud_assert_eq(std::get<10>(result), 1u);
            hud_assert_eq(std::get<11>(result), 0u);
            hud_assert_eq(std::get<12>(result), 0u);
        }

        // Constant
        {
            constexpr auto result = test_default_allocator({0, 1, 2, 3}, 1u);
            // Ensure we copy all datas in order
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), 4u);
            hud_assert_eq(std::get<2>(result), 4u + 1u);
            hud_assert_true(std::get<3>(result));

            // Ensure the copy data is not the same memory of the copied data
            hud_assert_true(std::get<4>(result));

            // Ensure we are allocating only one time of not
            // Always reallocate if types are non bitwise copy constructible types
            hud_assert_eq(std::get<5>(result), 1u);
            hud_assert_eq(std::get<6>(result), 0u);
            hud_assert_eq(std::get<7>(result), 0u);
            hud_assert_eq(std::get<8>(result), 0u);
            hud_assert_eq(std::get<9>(result), 1u);
            hud_assert_eq(std::get<10>(result), 1u);
            hud_assert_eq(std::get<11>(result), 0u);
            hud_assert_eq(std::get<12>(result), 0u);
        }
    }

    // Test wtih allocator
    {
        auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
        {
            MovedType moved(initializer, copied_extra);

            // Copy the array
            hud::array<Type2, AllocatorType> move(std::move(moved), AllocatorType {});

            // Ensure we moved all datas in order
            bool all_values_moved = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                if (static_cast<usize>(move[index].id()) != index)
                {
                    all_values_moved = false;
                    break;
                }
                if (move[index].move_constructor_count() != 1u)
                {
                    all_values_moved = false;
                    break;
                }
                if (move[index].copy_constructor_count() != 0u)
                {
                    all_values_moved = false;
                    break;
                }
            }

            return std::tuple {
                // Ensure we copy all datas in order
                move.data() != nullptr,
                move.count(),
                move.max_count(),
                all_values_moved,

                // Ensure the copy data is not the same memory of the copied data
                (void *)moved.data() != (void *)move.data(),

                // Ensure we are allocating only one time
                move.allocator().allocation_count(),
                move.allocator().free_count(),
                move.allocator().move_assign_count(),
                move.allocator().move_construct_count(),
                moved.allocator().allocation_count(),
                moved.allocator().free_count(),
                moved.allocator().move_assign_count(),
                moved.allocator().move_construct_count()
            };
        };

        // Non constant
        {
            const auto result = test_default_allocator({0, 1, 2, 3}, 1u);
            // Ensure we copy all datas in order
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), 4u);
            hud_assert_eq(std::get<2>(result), 4u + 1u);
            hud_assert_true(std::get<3>(result));

            // Ensure the copy data is not the same memory of the copied data
            hud_assert_true(std::get<4>(result));

            // Ensure we are allocating only one time of not
            // Always reallocate if types are non bitwise copy constructible types
            hud_assert_eq(std::get<5>(result), 1u);
            hud_assert_eq(std::get<6>(result), 0u);
            hud_assert_eq(std::get<7>(result), 0u);
            hud_assert_eq(std::get<8>(result), 0u);
            hud_assert_eq(std::get<9>(result), 1u);
            hud_assert_eq(std::get<10>(result), 1u);
            hud_assert_eq(std::get<11>(result), 0u);
            hud_assert_eq(std::get<12>(result), 0u);
        }

        // Constant
        {
            constexpr auto result = test_default_allocator({0, 1, 2, 3}, 1u);
            // Ensure we copy all datas in order
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), 4u);
            hud_assert_eq(std::get<2>(result), 4u + 1u);
            hud_assert_true(std::get<3>(result));

            // Ensure the copy data is not the same memory of the copied data
            hud_assert_true(std::get<4>(result));

            // Ensure we are allocating only one time of not
            // Always reallocate if types are non bitwise copy constructible types
            hud_assert_eq(std::get<5>(result), 1u);
            hud_assert_eq(std::get<6>(result), 0u);
            hud_assert_eq(std::get<7>(result), 0u);
            hud_assert_eq(std::get<8>(result), 0u);
            hud_assert_eq(std::get<9>(result), 1u);
            hud_assert_eq(std::get<10>(result), 1u);
            hud_assert_eq(std::get<11>(result), 0u);
            hud_assert_eq(std::get<12>(result), 0u);
        }
    }

    // Test with extra
    {
        hud_test::for_each_value<std::make_integer_sequence<usize, 5>>()(
            []<usize extra>()
            {
                // Test default allocator
                {
                    auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
                    {
                        MovedType moved(initializer, copied_extra);

                        // Copy the array
                        hud::array<Type2, AllocatorType> move(std::move(moved), extra);

                        // Ensure we moved all datas in order
                        bool all_values_moved = true;
                        for (usize index = 0; index < initializer.size(); index++)
                        {
                            if (static_cast<usize>(move[index].id()) != index)
                            {
                                all_values_moved = false;
                                break;
                            }
                            if (move[index].move_constructor_count() != 1u)
                            {
                                all_values_moved = false;
                                break;
                            }
                            if (move[index].copy_constructor_count() != 0u)
                            {
                                all_values_moved = false;
                                break;
                            }
                        }

                        return std::tuple {
                            // Ensure we copy all datas in order
                            move.data() != nullptr,
                            move.count(),
                            move.max_count(),
                            all_values_moved,

                            // Ensure the copy data is not the same memory of the copied data
                            (void *)moved.data() != (void *)move.data(),

                            // Ensure we are allocating only one time
                            move.allocator().allocation_count(),
                            move.allocator().free_count(),
                            move.allocator().move_assign_count(),
                            move.allocator().move_construct_count(),
                            moved.allocator().allocation_count(),
                            moved.allocator().free_count(),
                            moved.allocator().move_assign_count(),
                            moved.allocator().move_construct_count()
                        };
                    };

                    // Non constant
                    {
                        const auto result = test_default_allocator({0, 1, 2, 3}, 1u);
                        // Ensure we copy all datas in order
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), 4u);
                        hud_assert_eq(std::get<2>(result), 4u + 1u + extra);
                        hud_assert_true(std::get<3>(result));

                        // Ensure the copy data is not the same memory of the copied data
                        hud_assert_true(std::get<4>(result));

                        // Ensure we are allocating only one time of not
                        // Always reallocate if types are non bitwise copy constructible types
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                        hud_assert_eq(std::get<7>(result), 0u);
                        hud_assert_eq(std::get<8>(result), 0u);
                        hud_assert_eq(std::get<9>(result), 1u);
                        hud_assert_eq(std::get<10>(result), 1u);
                        hud_assert_eq(std::get<11>(result), 0u);
                        hud_assert_eq(std::get<12>(result), 0u);
                    }

                    // Constant
                    {
                        constexpr auto result = test_default_allocator({0, 1, 2, 3}, 1u);
                        // Ensure we copy all datas in order
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), 4u);
                        hud_assert_eq(std::get<2>(result), 4u + 1u + extra);
                        hud_assert_true(std::get<3>(result));

                        // Ensure the copy data is not the same memory of the copied data
                        hud_assert_true(std::get<4>(result));

                        // Ensure we are allocating only one time of not
                        // Always reallocate if types are non bitwise copy constructible types
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                        hud_assert_eq(std::get<7>(result), 0u);
                        hud_assert_eq(std::get<8>(result), 0u);
                        hud_assert_eq(std::get<9>(result), 1u);
                        hud_assert_eq(std::get<10>(result), 1u);
                        hud_assert_eq(std::get<11>(result), 0u);
                        hud_assert_eq(std::get<12>(result), 0u);
                    }
                }

                // Test wtih allocator
                {
                    auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
                    {
                        MovedType moved(initializer, copied_extra);

                        // Copy the array
                        hud::array<Type2, AllocatorType> move(std::move(moved), extra, AllocatorType {});

                        // Ensure we moved all datas in order
                        bool all_values_moved = true;
                        for (usize index = 0; index < initializer.size(); index++)
                        {
                            if (static_cast<usize>(move[index].id()) != index)
                            {
                                all_values_moved = false;
                                break;
                            }
                            if (move[index].move_constructor_count() != 1u)
                            {
                                all_values_moved = false;
                                break;
                            }
                            if (move[index].copy_constructor_count() != 0u)
                            {
                                all_values_moved = false;
                                break;
                            }
                        }

                        return std::tuple {
                            // Ensure we copy all datas in order
                            move.data() != nullptr,
                            move.count(),
                            move.max_count(),
                            all_values_moved,

                            // Ensure the copy data is not the same memory of the copied data
                            (void *)moved.data() != (void *)move.data(),

                            // Ensure we are allocating only one time
                            move.allocator().allocation_count(),
                            move.allocator().free_count(),
                            move.allocator().move_assign_count(),
                            move.allocator().move_construct_count(),
                            moved.allocator().allocation_count(),
                            moved.allocator().free_count(),
                            moved.allocator().move_assign_count(),
                            moved.allocator().move_construct_count()
                        };
                    };

                    // Non constant
                    {
                        const auto result = test_default_allocator({0, 1, 2, 3}, 1u);
                        // Ensure we copy all datas in order
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), 4u);
                        hud_assert_eq(std::get<2>(result), 4u + 1u + extra);
                        hud_assert_true(std::get<3>(result));

                        // Ensure the copy data is not the same memory of the copied data
                        hud_assert_true(std::get<4>(result));

                        // Ensure we are allocating only one time of not
                        // Always reallocate if types are non bitwise copy constructible types
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                        hud_assert_eq(std::get<7>(result), 0u);
                        hud_assert_eq(std::get<8>(result), 0u);
                        hud_assert_eq(std::get<9>(result), 1u);
                        hud_assert_eq(std::get<10>(result), 1u);
                        hud_assert_eq(std::get<11>(result), 0u);
                        hud_assert_eq(std::get<12>(result), 0u);
                    }

                    // Constant
                    {
                        constexpr auto result = test_default_allocator({0, 1, 2, 3}, 1u);
                        // Ensure we copy all datas in order
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), 4u);
                        hud_assert_eq(std::get<2>(result), 4u + 1u + extra);
                        hud_assert_true(std::get<3>(result));

                        // Ensure the copy data is not the same memory of the copied data
                        hud_assert_true(std::get<4>(result));

                        // Ensure we are allocating only one time of not
                        // Always reallocate if types are non bitwise copy constructible types
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                        hud_assert_eq(std::get<7>(result), 0u);
                        hud_assert_eq(std::get<8>(result), 0u);
                        hud_assert_eq(std::get<9>(result), 1u);
                        hud_assert_eq(std::get<10>(result), 1u);
                        hud_assert_eq(std::get<11>(result), 0u);
                        hud_assert_eq(std::get<12>(result), 0u);
                    }
                }
            }
        );
    }
}

GTEST_TEST(array, move_construct_non_bitwise_different_type_different_allocator)
{

    /** The array we move for test, we allocate also extra memory to test if we really move the count(), not the max_count() elements */
    using Type1 = hud_test::non_bitwise_type;
    using Type2 = hud_test::non_bitwise_type2;
    using AllocatorType = hud_test::allocator_watcher<alignof(Type1)>;
    using AllocatorType2 = hud_test::allocator_watcher_2<alignof(Type2)>;

    // Ensure we test with different allocator and types
    static_assert(!std::is_same_v<AllocatorType, AllocatorType2>);
    static_assert(!std::is_same_v<Type1, Type2>);
    static_assert(!hud::is_bitwise_move_constructible_v<Type2, Type1>);

    using MovedType = hud::array<Type1, AllocatorType>;

    // Test default allocator
    {
        auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
        {
            MovedType moved(initializer, copied_extra);

            // Copy the array
            hud::array<Type2, AllocatorType2> move(std::move(moved));

            // Ensure we moved all datas in order
            bool all_values_moved = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                if (static_cast<usize>(move[index].id()) != index)
                {
                    all_values_moved = false;
                    break;
                }
                if (move[index].move_constructor_count() != 1u)
                {
                    all_values_moved = false;
                    break;
                }
                if (move[index].copy_constructor_count() != 0u)
                {
                    all_values_moved = false;
                    break;
                }
            }

            return std::tuple {
                // Ensure we copy all datas in order
                move.data() != nullptr,
                move.count(),
                move.max_count(),
                all_values_moved,

                // Ensure the copy data is not the same memory of the copied data
                (void *)moved.data() != (void *)move.data(),

                // Ensure we are allocating only one time
                move.allocator().allocation_count(),
                move.allocator().free_count(),
                move.allocator().move_assign_count(),
                move.allocator().move_construct_count(),
                moved.allocator().allocation_count(),
                moved.allocator().free_count(),
                moved.allocator().move_assign_count(),
                moved.allocator().move_construct_count()
            };
        };

        // Non constant
        {
            const auto result = test_default_allocator({0, 1, 2, 3}, 1u);
            // Ensure we copy all datas in order
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), 4u);
            hud_assert_eq(std::get<2>(result), 4u + 1u);
            hud_assert_true(std::get<3>(result));

            // Ensure the copy data is not the same memory of the copied data
            hud_assert_true(std::get<4>(result));

            // Ensure we are allocating only one time of not
            // Always reallocate if types are non bitwise copy constructible types
            hud_assert_eq(std::get<5>(result), 1u);
            hud_assert_eq(std::get<6>(result), 0u);
            hud_assert_eq(std::get<7>(result), 0u);
            hud_assert_eq(std::get<8>(result), 0u);
            hud_assert_eq(std::get<9>(result), 1u);
            hud_assert_eq(std::get<10>(result), 1u);
            hud_assert_eq(std::get<11>(result), 0u);
            hud_assert_eq(std::get<12>(result), 0u);
        }

        // Constant
        {
            constexpr auto result = test_default_allocator({0, 1, 2, 3}, 1u);
            // Ensure we copy all datas in order
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), 4u);
            hud_assert_eq(std::get<2>(result), 4u + 1u);
            hud_assert_true(std::get<3>(result));

            // Ensure the copy data is not the same memory of the copied data
            hud_assert_true(std::get<4>(result));

            // Ensure we are allocating only one time of not
            // Always reallocate if types are non bitwise copy constructible types
            hud_assert_eq(std::get<5>(result), 1u);
            hud_assert_eq(std::get<6>(result), 0u);
            hud_assert_eq(std::get<7>(result), 0u);
            hud_assert_eq(std::get<8>(result), 0u);
            hud_assert_eq(std::get<9>(result), 1u);
            hud_assert_eq(std::get<10>(result), 1u);
            hud_assert_eq(std::get<11>(result), 0u);
            hud_assert_eq(std::get<12>(result), 0u);
        }
    }

    // Test wtih allocator
    {
        auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
        {
            MovedType moved(initializer, copied_extra);

            // Copy the array
            hud::array<Type2, AllocatorType2> move(std::move(moved), AllocatorType2 {});

            // Ensure we moved all datas in order
            bool all_values_moved = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                if (static_cast<usize>(move[index].id()) != index)
                {
                    all_values_moved = false;
                    break;
                }
                if (move[index].move_constructor_count() != 1u)
                {
                    all_values_moved = false;
                    break;
                }
                if (move[index].copy_constructor_count() != 0u)
                {
                    all_values_moved = false;
                    break;
                }
            }

            return std::tuple {
                // Ensure we copy all datas in order
                move.data() != nullptr,
                move.count(),
                move.max_count(),
                all_values_moved,

                // Ensure the copy data is not the same memory of the copied data
                (void *)moved.data() != (void *)move.data(),

                // Ensure we are allocating only one time
                move.allocator().allocation_count(),
                move.allocator().free_count(),
                move.allocator().move_assign_count(),
                move.allocator().move_construct_count(),
                moved.allocator().allocation_count(),
                moved.allocator().free_count(),
                moved.allocator().move_assign_count(),
                moved.allocator().move_construct_count()
            };
        };

        // Non constant
        {
            const auto result = test_default_allocator({0, 1, 2, 3}, 1u);
            // Ensure we copy all datas in order
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), 4u);
            hud_assert_eq(std::get<2>(result), 4u + 1u);
            hud_assert_true(std::get<3>(result));

            // Ensure the copy data is not the same memory of the copied data
            hud_assert_true(std::get<4>(result));

            // Ensure we are allocating only one time of not
            // Always reallocate if types are non bitwise copy constructible types
            hud_assert_eq(std::get<5>(result), 1u);
            hud_assert_eq(std::get<6>(result), 0u);
            hud_assert_eq(std::get<7>(result), 0u);
            hud_assert_eq(std::get<8>(result), 0u);
            hud_assert_eq(std::get<9>(result), 1u);
            hud_assert_eq(std::get<10>(result), 1u);
            hud_assert_eq(std::get<11>(result), 0u);
            hud_assert_eq(std::get<12>(result), 0u);
        }

        // Constant
        {
            constexpr auto result = test_default_allocator({0, 1, 2, 3}, 1u);
            // Ensure we copy all datas in order
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), 4u);
            hud_assert_eq(std::get<2>(result), 4u + 1u);
            hud_assert_true(std::get<3>(result));

            // Ensure the copy data is not the same memory of the copied data
            hud_assert_true(std::get<4>(result));

            // Ensure we are allocating only one time of not
            // Always reallocate if types are non bitwise copy constructible types
            hud_assert_eq(std::get<5>(result), 1u);
            hud_assert_eq(std::get<6>(result), 0u);
            hud_assert_eq(std::get<7>(result), 0u);
            hud_assert_eq(std::get<8>(result), 0u);
            hud_assert_eq(std::get<9>(result), 1u);
            hud_assert_eq(std::get<10>(result), 1u);
            hud_assert_eq(std::get<11>(result), 0u);
            hud_assert_eq(std::get<12>(result), 0u);
        }
    }

    // Test with extra
    {
        hud_test::for_each_value<std::make_integer_sequence<usize, 5>>()(
            []<usize extra>()
            {
                // Test default allocator
                {
                    auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
                    {
                        MovedType moved(initializer, copied_extra);

                        // Copy the array
                        hud::array<Type2, AllocatorType2> move(std::move(moved), extra);

                        // Ensure we moved all datas in order
                        bool all_values_moved = true;
                        for (usize index = 0; index < initializer.size(); index++)
                        {
                            if (static_cast<usize>(move[index].id()) != index)
                            {
                                all_values_moved = false;
                                break;
                            }
                            if (move[index].move_constructor_count() != 1u)
                            {
                                all_values_moved = false;
                                break;
                            }
                            if (move[index].copy_constructor_count() != 0u)
                            {
                                all_values_moved = false;
                                break;
                            }
                        }

                        return std::tuple {
                            // Ensure we copy all datas in order
                            move.data() != nullptr,
                            move.count(),
                            move.max_count(),
                            all_values_moved,

                            // Ensure the copy data is not the same memory of the copied data
                            (void *)moved.data() != (void *)move.data(),

                            // Ensure we are allocating only one time
                            move.allocator().allocation_count(),
                            move.allocator().free_count(),
                            move.allocator().move_assign_count(),
                            move.allocator().move_construct_count(),
                            moved.allocator().allocation_count(),
                            moved.allocator().free_count(),
                            moved.allocator().move_assign_count(),
                            moved.allocator().move_construct_count()
                        };
                    };

                    // Non constant
                    {
                        const auto result = test_default_allocator({0, 1, 2, 3}, 1u);
                        // Ensure we copy all datas in order
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), 4u);
                        hud_assert_eq(std::get<2>(result), 4u + 1u + extra);
                        hud_assert_true(std::get<3>(result));

                        // Ensure the copy data is not the same memory of the copied data
                        hud_assert_true(std::get<4>(result));

                        // Ensure we are allocating only one time of not
                        // Always reallocate if types are non bitwise copy constructible types
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                        hud_assert_eq(std::get<7>(result), 0u);
                        hud_assert_eq(std::get<8>(result), 0u);
                        hud_assert_eq(std::get<9>(result), 1u);
                        hud_assert_eq(std::get<10>(result), 1u);
                        hud_assert_eq(std::get<11>(result), 0u);
                        hud_assert_eq(std::get<12>(result), 0u);
                    }

                    // Constant
                    {
                        constexpr auto result = test_default_allocator({0, 1, 2, 3}, 1u);
                        // Ensure we copy all datas in order
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), 4u);
                        hud_assert_eq(std::get<2>(result), 4u + 1u + extra);
                        hud_assert_true(std::get<3>(result));

                        // Ensure the copy data is not the same memory of the copied data
                        hud_assert_true(std::get<4>(result));

                        // Ensure we are allocating only one time of not
                        // Always reallocate if types are non bitwise copy constructible types
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                        hud_assert_eq(std::get<7>(result), 0u);
                        hud_assert_eq(std::get<8>(result), 0u);
                        hud_assert_eq(std::get<9>(result), 1u);
                        hud_assert_eq(std::get<10>(result), 1u);
                        hud_assert_eq(std::get<11>(result), 0u);
                        hud_assert_eq(std::get<12>(result), 0u);
                    }
                }

                // Test wtih allocator
                {
                    auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
                    {
                        MovedType moved(initializer, copied_extra);

                        // Copy the array
                        hud::array<Type2, AllocatorType2> move(std::move(moved), extra, AllocatorType2 {});

                        // Ensure we moved all datas in order
                        bool all_values_moved = true;
                        for (usize index = 0; index < initializer.size(); index++)
                        {
                            if (static_cast<usize>(move[index].id()) != index)
                            {
                                all_values_moved = false;
                                break;
                            }
                            if (move[index].move_constructor_count() != 1u)
                            {
                                all_values_moved = false;
                                break;
                            }
                            if (move[index].copy_constructor_count() != 0u)
                            {
                                all_values_moved = false;
                                break;
                            }
                        }

                        return std::tuple {
                            // Ensure we copy all datas in order
                            move.data() != nullptr,
                            move.count(),
                            move.max_count(),
                            all_values_moved,

                            // Ensure the copy data is not the same memory of the copied data
                            (void *)moved.data() != (void *)move.data(),

                            // Ensure we are allocating only one time
                            move.allocator().allocation_count(),
                            move.allocator().free_count(),
                            move.allocator().move_assign_count(),
                            move.allocator().move_construct_count(),
                            moved.allocator().allocation_count(),
                            moved.allocator().free_count(),
                            moved.allocator().move_assign_count(),
                            moved.allocator().move_construct_count()
                        };
                    };

                    // Non constant
                    {
                        const auto result = test_default_allocator({0, 1, 2, 3}, 1u);
                        // Ensure we copy all datas in order
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), 4u);
                        hud_assert_eq(std::get<2>(result), 4u + 1u + extra);
                        hud_assert_true(std::get<3>(result));

                        // Ensure the copy data is not the same memory of the copied data
                        hud_assert_true(std::get<4>(result));

                        // Ensure we are allocating only one time of not
                        // Always reallocate if types are non bitwise copy constructible types
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                        hud_assert_eq(std::get<7>(result), 0u);
                        hud_assert_eq(std::get<8>(result), 0u);
                        hud_assert_eq(std::get<9>(result), 1u);
                        hud_assert_eq(std::get<10>(result), 1u);
                        hud_assert_eq(std::get<11>(result), 0u);
                        hud_assert_eq(std::get<12>(result), 0u);
                    }

                    // Constant
                    {
                        constexpr auto result = test_default_allocator({0, 1, 2, 3}, 1u);
                        // Ensure we copy all datas in order
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), 4u);
                        hud_assert_eq(std::get<2>(result), 4u + 1u + extra);
                        hud_assert_true(std::get<3>(result));

                        // Ensure the copy data is not the same memory of the copied data
                        hud_assert_true(std::get<4>(result));

                        // Ensure we are allocating only one time of not
                        // Always reallocate if types are non bitwise copy constructible types
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                        hud_assert_eq(std::get<7>(result), 0u);
                        hud_assert_eq(std::get<8>(result), 0u);
                        hud_assert_eq(std::get<9>(result), 1u);
                        hud_assert_eq(std::get<10>(result), 1u);
                        hud_assert_eq(std::get<11>(result), 0u);
                        hud_assert_eq(std::get<12>(result), 0u);
                    }
                }
            }
        );
    }
}
