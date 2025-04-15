#include <core/containers/array.h>
#include "../misc/allocator_watcher.h"

GTEST_TEST(array, copy_construct_bitwise_copy_constructible_same_type_same_allocator)
{
    /** The array we copy for test, we allocate also extra memory to test if we really copy the count(), not the max_count() elements */
    using type = i32;

    static_assert(hud::is_bitwise_copy_constructible_v<type>);
    using AllocatorType = hud_test::allocator_watcher<alignof(type)>;
    using CopiedType = hud::array<type, AllocatorType>;

    // No extra
    {
        auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
        {
            const CopiedType copied(initializer, copied_extra);

            // Copy the array
            hud::array<type, AllocatorType> copy(copied);

            // Ensure we copy all datas in order
            bool all_values_copied = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                if (copy[index] != static_cast<type>(index))
                {
                    all_values_copied = false;
                    break;
                }
            }

            return std::tuple {
                // Ensure we copy all datas in order
                copy.data() != nullptr,
                copy.count(),
                copy.max_count(),
                all_values_copied,

                // Ensure the copy data is not the same memory of the copied data
                copied.data() != copy.data(),

                // Ensure we are allocating only one time
                copy.allocator().allocation_count(),
                copy.allocator().free_count()
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

            // Ensure we are allocating twice time (1 for each copy of array)
            hud_assert_eq(std::get<5>(result), 2u);
            hud_assert_eq(std::get<6>(result), 0u);
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
            hud_assert_eq(std::get<5>(result), 2u);
            hud_assert_eq(std::get<6>(result), 0u);
        }

        auto test_with_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
        {
            const CopiedType copied(initializer, copied_extra);

            // Copy the array
            hud::array<type, AllocatorType> copy(copied, AllocatorType {});

            // Ensure we copy all datas in order
            bool all_values_copied = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                if (copy[index] != static_cast<type>(index))
                {
                    all_values_copied = false;
                    break;
                }
            }

            return std::tuple {
                // Ensure we copy all datas in order
                copy.data() != nullptr,
                copy.count(),
                copy.max_count(),
                all_values_copied,

                // Ensure the copy data is not the same memory of the copied data
                copied.data() != copy.data(),

                // Ensure we are allocating only one time
                copy.allocator().allocation_count(),
                copy.allocator().free_count()
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

            // Ensure we are allocating only one time
            hud_assert_eq(std::get<5>(result), 1u);
            hud_assert_eq(std::get<6>(result), 0u);
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

            // Ensure we are allocating only one time
            hud_assert_eq(std::get<5>(result), 1u);
            hud_assert_eq(std::get<6>(result), 0u);
        }
    }

    // With extra
    {
        hud_test::for_each_value<std::make_integer_sequence<usize, 5>>()([]<usize extra>()
                                                                         {
                // Test with default allocator
                {
                    auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
                    {
                        const CopiedType copied(initializer, copied_extra);

                        // Copy the array
                        hud::array<type, AllocatorType> copy(copied, extra);

                        // Ensure we copy all datas in order
                        bool all_values_copied = true;
                        for (usize index = 0; index < initializer.size(); index++)
                        {
                            if (copy[index] != static_cast<type>(index))
                            {
                                all_values_copied = false;
                                break;
                            }
                        }

                        return std::tuple {
                            // Ensure we copy all datas in order
                            copy.data() != nullptr,
                            copy.count(),
                            copy.max_count(),
                            all_values_copied,

                            // Ensure the copy data is not the same memory of the copied data
                            copied.data() != copy.data(),

                            // Ensure we are allocating only one time
                            copy.allocator().allocation_count(),
                            copy.allocator().free_count()
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

                        // Ensure we are allocating only one time
                        hud_assert_eq(std::get<5>(result), 2u);
                        hud_assert_eq(std::get<6>(result), 0u);
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

                        // Ensure we are allocating only one time
                        hud_assert_eq(std::get<5>(result), 2u);
                        hud_assert_eq(std::get<6>(result), 0u);
                    }
                }

                // Test with allocator
                {
                    auto test_with_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
                    {
                        const CopiedType copied(initializer, copied_extra);

                        // Copy the array
                        hud::array<type, AllocatorType> copy(copied, extra, AllocatorType {});

                        // Ensure we copy all datas in order
                        bool all_values_copied = true;
                        for (usize index = 0; index < initializer.size(); index++)
                        {
                            if (copy[index] != static_cast<type>(index))
                            {
                                all_values_copied = false;
                                break;
                            }
                        }

                        return std::tuple {
                            // Ensure we copy all datas in order
                            copy.data() != nullptr,
                            copy.count(),
                            copy.max_count(),
                            all_values_copied,

                            // Ensure the copy data is not the same memory of the copied data
                            copied.data() != copy.data(),

                            // Ensure we are allocating only one time
                            copy.allocator().allocation_count(),
                            copy.allocator().free_count()
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

                        // Ensure we are allocating only one time
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
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

                        // Ensure we are allocating only one time
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                    }
                } });
    }
}

GTEST_TEST(array, copy_construct_bitwise_copy_constructible_same_type_different_allocator)
{
    /** The array we copy for test, we allocate also extra memory to test if we really copy the count(), not the max_count() elements */
    using type = i32;

    static_assert(hud::is_bitwise_copy_constructible_v<type>);
    using AllocatorType = hud_test::allocator_watcher<alignof(type)>;
    using AllocatorType2 = hud_test::allocator_watcher_2<alignof(type)>;
    using CopiedType = hud::array<type, AllocatorType>;

    // No extra
    {
        auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
        {
            const CopiedType copied(initializer, copied_extra);

            // Copy the array
            hud::array<type, AllocatorType2> copy(copied);

            // Ensure we copy all datas in order
            bool all_values_copied = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                if (copy[index] != static_cast<type>(index))
                {
                    all_values_copied = false;
                    break;
                }
            }

            return std::tuple {
                // Ensure we copy all datas in order
                copy.data() != nullptr,
                copy.count(),
                copy.max_count(),
                all_values_copied,

                // Ensure the copy data is not the same memory of the copied data
                copied.data() != copy.data(),

                // Ensure we are allocating only one time
                copy.allocator().allocation_count(),
                copy.allocator().free_count()
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

            // Ensure we are allocating twice time (1 for each copy of array)
            hud_assert_eq(std::get<5>(result), 1u);
            hud_assert_eq(std::get<6>(result), 0u);
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
        }

        auto test_with_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
        {
            const CopiedType copied(initializer, copied_extra);

            // Copy the array
            hud::array<type, AllocatorType2> copy(copied, AllocatorType2 {});

            // Ensure we copy all datas in order
            bool all_values_copied = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                if (copy[index] != static_cast<type>(index))
                {
                    all_values_copied = false;
                    break;
                }
            }

            return std::tuple {
                // Ensure we copy all datas in order
                copy.data() != nullptr,
                copy.count(),
                copy.max_count(),
                all_values_copied,

                // Ensure the copy data is not the same memory of the copied data
                copied.data() != copy.data(),

                // Ensure we are allocating only one time
                copy.allocator().allocation_count(),
                copy.allocator().free_count()
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

            // Ensure we are allocating only one time
            hud_assert_eq(std::get<5>(result), 1u);
            hud_assert_eq(std::get<6>(result), 0u);
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

            // Ensure we are allocating only one time
            hud_assert_eq(std::get<5>(result), 1u);
            hud_assert_eq(std::get<6>(result), 0u);
        }
    }

    // Test with extra
    {
        hud_test::for_each_value<std::make_integer_sequence<usize, 5>>()(
            []<usize extra>()
            {
                // Test with default allocator
                {
                    auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
                    {
                        const CopiedType copied(initializer, copied_extra);

                        // Copy the array
                        hud::array<type, AllocatorType2> copy(copied, extra);

                        // Ensure we copy all datas in order
                        bool all_values_copied = true;
                        for (usize index = 0; index < initializer.size(); index++)
                        {
                            if (copy[index] != static_cast<type>(index))
                            {
                                all_values_copied = false;
                                break;
                            }
                        }

                        return std::tuple {
                            // Ensure we copy all datas in order
                            copy.data() != nullptr,
                            copy.count(),
                            copy.max_count(),
                            all_values_copied,

                            // Ensure the copy data is not the same memory of the copied data
                            copied.data() != copy.data(),

                            // Ensure we are allocating only one time
                            copy.allocator().allocation_count(),
                            copy.allocator().free_count()
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

                        // Ensure we are allocating only one time
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
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

                        // Ensure we are allocating only one time
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                    }
                }

                // Test with allocator
                {
                    auto test_with_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
                    {
                        const CopiedType copied(initializer, copied_extra);

                        // Copy the array
                        hud::array<type, AllocatorType2> copy(copied, extra, AllocatorType2 {});

                        // Ensure we copy all datas in order
                        bool all_values_copied = true;
                        for (usize index = 0; index < initializer.size(); index++)
                        {
                            if (copy[index] != static_cast<type>(index))
                            {
                                all_values_copied = false;
                                break;
                            }
                        }

                        return std::tuple {
                            // Ensure we copy all datas in order
                            copy.data() != nullptr,
                            copy.count(),
                            copy.max_count(),
                            all_values_copied,

                            // Ensure the copy data is not the same memory of the copied data
                            copied.data() != copy.data(),

                            // Ensure we are allocating only one time
                            copy.allocator().allocation_count(),
                            copy.allocator().free_count()
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

                        // Ensure we are allocating only one time
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
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

                        // Ensure we are allocating only one time
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                    }
                }
            }
        );
    }
}

GTEST_TEST(array, copy_construct_bitwise_copy_constructible_different_type_same_allocator)
{
    /** The array we copy for test, we allocate also extra memory to test if we really copy the count(), not the max_count() elements */
    using Type1 = i32;
    using Type2 = u32;

    using AllocatorType = hud_test::allocator_watcher<alignof(Type1)>;

    // Ensure we test with different allocator
    static_assert(!std::is_same_v<Type1, Type2>);
    static_assert(hud::is_bitwise_copy_constructible_v<Type2, Type1>);

    // Test with default allocator no extra
    {
        auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
        {
            const hud::array<Type1, AllocatorType> copied(initializer, copied_extra);

            // Copy the array
            hud::array<Type2, AllocatorType> copy(copied);

            // Ensure we copy all datas in order
            bool all_values_copied = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                if (copy[index] != static_cast<Type2>(index))
                {
                    all_values_copied = false;
                    break;
                }
            }

            return std::tuple {
                // Ensure we copy all datas in order
                copy.data() != nullptr,
                copy.count(),
                copy.max_count(),
                all_values_copied,

                // Ensure the copy data is not the same memory of the copied data
                (void *)copied.data() != (void *)copy.data(),

                // Ensure we are allocating only one time
                copy.allocator().allocation_count(),
                copy.allocator().free_count()
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
        }
    }

    // Test with allocator no extra
    {
        auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
        {
            const hud::array<Type1, AllocatorType> copied(initializer, copied_extra);

            // Copy the array
            hud::array<Type2, AllocatorType> copy(copied, AllocatorType {});

            // Ensure we copy all datas in order
            bool all_values_copied = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                if (copy[index] != static_cast<Type2>(index))
                {
                    all_values_copied = false;
                    break;
                }
            }

            return std::tuple {
                // Ensure we copy all datas in order
                copy.data() != nullptr,
                copy.count(),
                copy.max_count(),
                all_values_copied,

                // Ensure the copy data is not the same memory of the copied data
                (void *)copied.data() != (void *)copy.data(),

                // Ensure we are allocating only one time
                copy.allocator().allocation_count(),
                copy.allocator().free_count()
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
        }
    }

    // Test with extra
    {
        hud_test::for_each_value<std::make_integer_sequence<usize, 5>>()(
            []<usize extra>()
            {
                // Test with default allocator
                {
                    auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
                    {
                        const hud::array<Type1, AllocatorType> copied(initializer, copied_extra);

                        // Copy the array
                        hud::array<Type2, AllocatorType> copy(copied, extra);

                        // Ensure we copy all datas in order
                        bool all_values_copied = true;
                        for (usize index = 0; index < initializer.size(); index++)
                        {
                            if (copy[index] != static_cast<Type2>(index))
                            {
                                all_values_copied = false;
                                break;
                            }
                        }

                        return std::tuple {
                            // Ensure we copy all datas in order
                            copy.data() != nullptr,
                            copy.count(),
                            copy.max_count(),
                            all_values_copied,

                            // Ensure the copy data is not the same memory of the copied data
                            (void *)copied.data() != (void *)copy.data(),

                            // Ensure we are allocating only one time
                            copy.allocator().allocation_count(),
                            copy.allocator().free_count()
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

                        // Ensure we are allocating only one time
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
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

                        // Ensure we are allocating only one time
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                    }
                }

                // Test with allocator no extra
                {
                    auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
                    {
                        const hud::array<Type1, AllocatorType> copied(initializer, copied_extra);

                        // Copy the array
                        hud::array<Type2, AllocatorType> copy(copied, extra, AllocatorType {});

                        // Ensure we copy all datas in order
                        bool all_values_copied = true;
                        for (usize index = 0; index < initializer.size(); index++)
                        {
                            if (copy[index] != static_cast<Type2>(index))
                            {
                                all_values_copied = false;
                                break;
                            }
                        }

                        return std::tuple {
                            // Ensure we copy all datas in order
                            copy.data() != nullptr,
                            copy.count(),
                            copy.max_count(),
                            all_values_copied,

                            // Ensure the copy data is not the same memory of the copied data
                            (void *)copied.data() != (void *)copy.data(),

                            // Ensure we are allocating only one time
                            copy.allocator().allocation_count(),
                            copy.allocator().free_count()
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

                        // Ensure we are allocating only one time
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
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

                        // Ensure we are allocating only one time
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                    }
                }
            }
        );
    }
}

GTEST_TEST(array, copy_construct_bitwise_copy_constructible_different_type_different_allocator)
{
    /** The array we copy for test, we allocate also extra memory to test if we really copy the count(), not the max_count() elements */
    using Type1 = i32;
    using Type2 = u32;

    using AllocatorType = hud_test::allocator_watcher<alignof(Type1)>;
    using AllocatorType2 = hud_test::allocator_watcher_2<alignof(Type2)>;

    // Ensure we test with different allocator
    static_assert(!std::is_same_v<AllocatorType, AllocatorType2>);
    static_assert(!std::is_same_v<Type1, Type2>);
    static_assert(hud::is_bitwise_copy_constructible_v<Type2, Type1>);

    // Test with default allocator no extra
    {
        auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
        {
            const hud::array<Type1, AllocatorType> copied(initializer, copied_extra);

            // Copy the array
            hud::array<Type2, AllocatorType2> copy(copied);

            // Ensure we copy all datas in order
            bool all_values_copied = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                if (copy[index] != static_cast<Type2>(index))
                {
                    all_values_copied = false;
                    break;
                }
            }

            return std::tuple {
                // Ensure we copy all datas in order
                copy.data() != nullptr,
                copy.count(),
                copy.max_count(),
                all_values_copied,

                // Ensure the copy data is not the same memory of the copied data
                (void *)copied.data() != (void *)copy.data(),

                // Ensure we are allocating only one time
                copy.allocator().allocation_count(),
                copy.allocator().free_count()
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
        }
    }

    // Test with allocator no extra
    {
        auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
        {
            const hud::array<Type1, AllocatorType> copied(initializer, copied_extra);

            // Copy the array
            hud::array<Type2, AllocatorType2> copy(copied, AllocatorType2 {});

            // Ensure we copy all datas in order
            bool all_values_copied = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                if (copy[index] != static_cast<Type2>(index))
                {
                    all_values_copied = false;
                    break;
                }
            }

            return std::tuple {
                // Ensure we copy all datas in order
                copy.data() != nullptr,
                copy.count(),
                copy.max_count(),
                all_values_copied,

                // Ensure the copy data is not the same memory of the copied data
                (void *)copied.data() != (void *)copy.data(),

                // Ensure we are allocating only one time
                copy.allocator().allocation_count(),
                copy.allocator().free_count()
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
        }
    }

    // Test with extra
    {
        hud_test::for_each_value<std::make_integer_sequence<usize, 5>>()(
            []<usize extra>()
            {
                // Test with default allocator
                {
                    auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
                    {
                        const hud::array<Type1, AllocatorType> copied(initializer, copied_extra);

                        // Copy the array
                        hud::array<Type2, AllocatorType2> copy(copied, extra);

                        // Ensure we copy all datas in order
                        bool all_values_copied = true;
                        for (usize index = 0; index < initializer.size(); index++)
                        {
                            if (copy[index] != static_cast<Type2>(index))
                            {
                                all_values_copied = false;
                                break;
                            }
                        }

                        return std::tuple {
                            // Ensure we copy all datas in order
                            copy.data() != nullptr,
                            copy.count(),
                            copy.max_count(),
                            all_values_copied,

                            // Ensure the copy data is not the same memory of the copied data
                            (void *)copied.data() != (void *)copy.data(),

                            // Ensure we are allocating only one time
                            copy.allocator().allocation_count(),
                            copy.allocator().free_count()
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

                        // Ensure we are allocating only one time
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
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

                        // Ensure we are allocating only one time
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                    }
                }

                // Test with allocator no extra
                {
                    auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
                    {
                        const hud::array<Type1, AllocatorType> copied(initializer, copied_extra);

                        // Copy the array
                        hud::array<Type2, AllocatorType2> copy(copied, extra, AllocatorType2 {});

                        // Ensure we copy all datas in order
                        bool all_values_copied = true;
                        for (usize index = 0; index < initializer.size(); index++)
                        {
                            if (copy[index] != static_cast<Type2>(index))
                            {
                                all_values_copied = false;
                                break;
                            }
                        }

                        return std::tuple {
                            // Ensure we copy all datas in order
                            copy.data() != nullptr,
                            copy.count(),
                            copy.max_count(),
                            all_values_copied,

                            // Ensure the copy data is not the same memory of the copied data
                            (void *)copied.data() != (void *)copy.data(),

                            // Ensure we are allocating only one time
                            copy.allocator().allocation_count(),
                            copy.allocator().free_count()
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

                        // Ensure we are allocating only one time
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
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

                        // Ensure we are allocating only one time
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                    }
                }
            }
        );
    }
}

GTEST_TEST(array, copy_construct_non_bitwise_copy_constructible_same_type_same_allocator)
{

    /** The array we copyfor test, we allocate also extra memory to test if we really copy the count(), not the max_count() elements */
    using type = hud_test::non_bitwise_copy_constructible_type;
    using AllocatorType = hud_test::allocator_watcher<alignof(type)>;

    // Ensure we test with different allocator
    static_assert(!hud::is_bitwise_copy_constructible_v<type>);

    using CopiedType = const hud::array<hud_test::non_bitwise_copy_constructible_type, AllocatorType>;

    // Test default allocator
    {
        auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
        {
            const CopiedType copied(initializer, copied_extra);

            // Copy the array
            hud::array<type, AllocatorType> copy(copied);

            // Ensure we copy all datas in order
            bool all_values_copied = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                if (static_cast<usize>(copy[index].id()) != index || copy[index].copy_constructor_count() != 1u)
                {
                    all_values_copied = false;
                    break;
                }
            }

            return std::tuple {
                // Ensure we copy all datas in order
                copy.data() != nullptr,
                copy.count(),
                copy.max_count(),
                all_values_copied,

                // Ensure the copy data is not the same memory of the copied data
                copied.data() != copy.data(),

                // Ensure we are allocating only one time
                copy.allocator().allocation_count(),
                copy.allocator().free_count()
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
            hud_assert_eq(std::get<5>(result), 2u);
            hud_assert_eq(std::get<6>(result), 0u);
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
            hud_assert_eq(std::get<5>(result), 2u);
            hud_assert_eq(std::get<6>(result), 0u);
        }
    }

    // Test with allocator
    {
        auto test_wtih_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
        {
            const CopiedType copied(initializer, copied_extra);

            // Copy the array
            hud::array<type, AllocatorType> copy(copied, AllocatorType {});

            // Ensure we copy all datas in order
            bool all_values_copied = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                if (static_cast<usize>(copy[index].id()) != index || copy[index].copy_constructor_count() != 1u)
                {
                    all_values_copied = false;
                    break;
                }
            }

            return std::tuple {
                // Ensure we copy all datas in order
                copy.data() != nullptr,
                copy.count(),
                copy.max_count(),
                all_values_copied,

                // Ensure the copy data is not the same memory of the copied data
                copied.data() != copy.data(),

                // Ensure we are allocating only one time
                copy.allocator().allocation_count(),
                copy.allocator().free_count()
            };
        };

        // Non constant
        {
            const auto result = test_wtih_allocator({0, 1, 2, 3}, 1u);
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
        }

        // Constant
        {
            constexpr auto result = test_wtih_allocator({0, 1, 2, 3}, 1u);
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
                        const CopiedType copied(initializer, copied_extra);

                        // Copy the array
                        hud::array<type, AllocatorType> copy(copied, extra);

                        // Ensure we copy all datas in order
                        bool all_values_copied = true;
                        for (usize index = 0; index < initializer.size(); index++)
                        {
                            if (static_cast<usize>(copy[index].id()) != index || copy[index].copy_constructor_count() != 1u)
                            {
                                all_values_copied = false;
                                break;
                            }
                        }

                        return std::tuple {
                            // Ensure we copy all datas in order
                            copy.data() != nullptr,
                            copy.count(),
                            copy.max_count(),
                            all_values_copied,

                            // Ensure the copy data is not the same memory of the copied data
                            copied.data() != copy.data(),

                            // Ensure we are allocating only one time
                            copy.allocator().allocation_count(),
                            copy.allocator().free_count()
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

                        // Ensure we are allocating only one time
                        hud_assert_eq(std::get<5>(result), 2u);
                        hud_assert_eq(std::get<6>(result), 0u);
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

                        // Ensure we are allocating only one time
                        hud_assert_eq(std::get<5>(result), 2u);
                        hud_assert_eq(std::get<6>(result), 0u);
                    }
                }

                // Test with allocator
                {
                    auto test_with_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
                    {
                        const CopiedType copied(initializer, copied_extra);

                        // Copy the array
                        hud::array<type, AllocatorType> copy(copied, extra, AllocatorType {});

                        // Ensure we copy all datas in order
                        bool all_values_copied = true;
                        for (usize index = 0; index < initializer.size(); index++)
                        {
                            if (static_cast<usize>(copy[index].id()) != index || copy[index].copy_constructor_count() != 1u)
                            {
                                all_values_copied = false;
                                break;
                            }
                        }

                        return std::tuple {
                            // Ensure we copy all datas in order
                            copy.data() != nullptr,
                            copy.count(),
                            copy.max_count(),
                            all_values_copied,

                            // Ensure the copy data is not the same memory of the copied data
                            copied.data() != copy.data(),

                            // Ensure we are allocating only one time
                            copy.allocator().allocation_count(),
                            copy.allocator().free_count()
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

                        // Ensure we are allocating only one time
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
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

                        // Ensure we are allocating only one time
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                    }
                }
            }
        );
    }
}

GTEST_TEST(array, copy_construct_non_bitwise_copy_constructible_same_type_different_allocator)
{

    /** The array we copyfor test, we allocate also extra memory to test if we really copy the count(), not the max_count() elements */
    using type = hud_test::non_bitwise_copy_constructible_type;
    using AllocatorType = hud_test::allocator_watcher<alignof(type)>;
    using AllocatorType2 = hud_test::allocator_watcher_2<alignof(type)>;

    // Ensure we test with different allocator
    static_assert(!std::is_same_v<AllocatorType, AllocatorType2>);
    static_assert(!hud::is_bitwise_copy_constructible_v<type>);

    using CopiedType = const hud::array<hud_test::non_bitwise_copy_constructible_type, AllocatorType>;

    // Test default allocator
    {
        auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
        {
            const CopiedType copied(initializer, copied_extra);

            // Copy the array
            hud::array<type, AllocatorType2> copy(copied);

            // Ensure we copy all datas in order
            bool all_values_copied = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                if (static_cast<usize>(copy[index].id()) != index || copy[index].copy_constructor_count() != 1u)
                {
                    all_values_copied = false;
                    break;
                }
            }

            return std::tuple {
                // Ensure we copy all datas in order
                copy.data() != nullptr,
                copy.count(),
                copy.max_count(),
                all_values_copied,

                // Ensure the copy data is not the same memory of the copied data
                copied.data() != copy.data(),

                // Ensure we are allocating only one time
                copy.allocator().allocation_count(),
                copy.allocator().free_count()
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
        }
    }

    // Test with allocator
    {
        auto test_wtih_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
        {
            const CopiedType copied(initializer, copied_extra);

            // Copy the array
            hud::array<type, AllocatorType2> copy(copied, AllocatorType2 {});

            // Ensure we copy all datas in order
            bool all_values_copied = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                if (static_cast<usize>(copy[index].id()) != index || copy[index].copy_constructor_count() != 1u)
                {
                    all_values_copied = false;
                    break;
                }
            }

            return std::tuple {
                // Ensure we copy all datas in order
                copy.data() != nullptr,
                copy.count(),
                copy.max_count(),
                all_values_copied,

                // Ensure the copy data is not the same memory of the copied data
                copied.data() != copy.data(),

                // Ensure we are allocating only one time
                copy.allocator().allocation_count(),
                copy.allocator().free_count()
            };
        };

        // Non constant
        {
            const auto result = test_wtih_allocator({0, 1, 2, 3}, 1u);
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
        }

        // Constant
        {
            constexpr auto result = test_wtih_allocator({0, 1, 2, 3}, 1u);
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
                        const CopiedType copied(initializer, copied_extra);

                        // Copy the array
                        hud::array<type, AllocatorType2> copy(copied, extra);

                        // Ensure we copy all datas in order
                        bool all_values_copied = true;
                        for (usize index = 0; index < initializer.size(); index++)
                        {
                            if (static_cast<usize>(copy[index].id()) != index || copy[index].copy_constructor_count() != 1u)
                            {
                                all_values_copied = false;
                                break;
                            }
                        }

                        return std::tuple {
                            // Ensure we copy all datas in order
                            copy.data() != nullptr,
                            copy.count(),
                            copy.max_count(),
                            all_values_copied,

                            // Ensure the copy data is not the same memory of the copied data
                            copied.data() != copy.data(),

                            // Ensure we are allocating only one time
                            copy.allocator().allocation_count(),
                            copy.allocator().free_count()
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

                        // Ensure we are allocating only one time
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
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

                        // Ensure we are allocating only one time
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                    }
                }

                // Test with allocator
                {
                    auto test_with_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
                    {
                        const CopiedType copied(initializer, copied_extra);

                        // Copy the array
                        hud::array<type, AllocatorType2> copy(copied, extra, AllocatorType2 {});

                        // Ensure we copy all datas in order
                        bool all_values_copied = true;
                        for (usize index = 0; index < initializer.size(); index++)
                        {
                            if (static_cast<usize>(copy[index].id()) != index || copy[index].copy_constructor_count() != 1u)
                            {
                                all_values_copied = false;
                                break;
                            }
                        }

                        return std::tuple {
                            // Ensure we copy all datas in order
                            copy.data() != nullptr,
                            copy.count(),
                            copy.max_count(),
                            all_values_copied,

                            // Ensure the copy data is not the same memory of the copied data
                            copied.data() != copy.data(),

                            // Ensure we are allocating only one time
                            copy.allocator().allocation_count(),
                            copy.allocator().free_count()
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

                        // Ensure we are allocating only one time
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
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

                        // Ensure we are allocating only one time
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                    }
                }
            }
        );
    }
}

GTEST_TEST(array, copy_construct_non_bitwise_copy_constructible_different_type_same_allocator)
{

    /** The array we copyfor test, we allocate also extra memory to test if we really copy the count(), not the max_count() elements */
    using Type1 = hud_test::non_bitwise_copy_constructible_type;
    using Type2 = hud_test::non_bitwise_copy_constructible_type2;
    using AllocatorType = hud_test::allocator_watcher<alignof(Type1)>;

    // Ensure we test with different allocator
    static_assert(!std::is_same_v<Type1, Type2>);
    static_assert(!hud::is_bitwise_copy_constructible_v<Type2, Type1>);

    // Test default allocator
    {
        auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
        {
            const hud::array<Type1, AllocatorType> copied(initializer, copied_extra);

            // Copy the array
            hud::array<Type2, AllocatorType> copy(copied);

            // Ensure we copy all datas in order
            bool all_values_copied = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                if (static_cast<usize>(copy[index].id()) != index || copy[index].copy_constructor_count() != 1u)
                {
                    all_values_copied = false;
                    break;
                }
            }

            return std::tuple {
                // Ensure we copy all datas in order
                copy.data() != nullptr,
                copy.count(),
                copy.max_count(),
                all_values_copied,

                // Ensure the copy data is not the same memory of the copied data
                copied.data() != copy.data(),

                // Ensure we are allocating only one time
                copy.allocator().allocation_count(),
                copy.allocator().free_count()
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
        }
    }

    // Test with allocator
    {
        auto test_with_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
        {
            const hud::array<Type1, AllocatorType> copied(initializer, copied_extra);

            // Copy the array
            hud::array<Type2, AllocatorType> copy(copied, AllocatorType {});

            // Ensure we copy all datas in order
            bool all_values_copied = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                if (static_cast<usize>(copy[index].id()) != index || copy[index].copy_constructor_count() != 1u)
                {
                    all_values_copied = false;
                    break;
                }
            }

            return std::tuple {
                // Ensure we copy all datas in order
                copy.data() != nullptr,
                copy.count(),
                copy.max_count(),
                all_values_copied,

                // Ensure the copy data is not the same memory of the copied data
                copied.data() != copy.data(),

                // Ensure we are allocating only one time
                copy.allocator().allocation_count(),
                copy.allocator().free_count()
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

            // Ensure we are allocating only one time
            hud_assert_eq(std::get<5>(result), 1u);
            hud_assert_eq(std::get<6>(result), 0u);
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

            // Ensure we are allocating only one time
            hud_assert_eq(std::get<5>(result), 1u);
            hud_assert_eq(std::get<6>(result), 0u);
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
                        const hud::array<Type1, AllocatorType> copied(initializer, copied_extra);

                        // Copy the array
                        hud::array<Type2, AllocatorType> copy(copied, extra);

                        // Ensure we copy all datas in order
                        bool all_values_copied = true;
                        for (usize index = 0; index < initializer.size(); index++)
                        {
                            if (static_cast<usize>(copy[index].id()) != index || copy[index].copy_constructor_count() != 1u)
                            {
                                all_values_copied = false;
                                break;
                            }
                        }

                        return std::tuple {
                            // Ensure we copy all datas in order
                            copy.data() != nullptr,
                            copy.count(),
                            copy.max_count(),
                            all_values_copied,

                            // Ensure the copy data is not the same memory of the copied data
                            copied.data() != copy.data(),

                            // Ensure we are allocating only one time
                            copy.allocator().allocation_count(),
                            copy.allocator().free_count()
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

                        // Ensure we are allocating only one time
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
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

                        // Ensure we are allocating only one time
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                    }
                }

                // Test with allocator
                {
                    auto test_with_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
                    {
                        const hud::array<Type1, AllocatorType> copied(initializer, copied_extra);

                        // Copy the array
                        hud::array<Type2, AllocatorType> copy(copied, extra, AllocatorType {});

                        // Ensure we copy all datas in order
                        bool all_values_copied = true;
                        for (usize index = 0; index < initializer.size(); index++)
                        {
                            if (static_cast<usize>(copy[index].id()) != index || copy[index].copy_constructor_count() != 1u)
                            {
                                all_values_copied = false;
                                break;
                            }
                        }

                        return std::tuple {
                            // Ensure we copy all datas in order
                            copy.data() != nullptr,
                            copy.count(),
                            copy.max_count(),
                            all_values_copied,

                            // Ensure the copy data is not the same memory of the copied data
                            copied.data() != copy.data(),

                            // Ensure we are allocating only one time
                            copy.allocator().allocation_count(),
                            copy.allocator().free_count()
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

                        // Ensure we are allocating only one time
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
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

                        // Ensure we are allocating only one time
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                    }
                }
            }
        );
    }
}

GTEST_TEST(array, copy_construct_non_bitwise_copy_constructible_different_type_different_allocator)
{

    /** The array we copyfor test, we allocate also extra memory to test if we really copy the count(), not the max_count() elements */
    using Type1 = hud_test::non_bitwise_copy_constructible_type;
    using Type2 = hud_test::non_bitwise_copy_constructible_type2;
    using AllocatorType = hud_test::allocator_watcher<alignof(Type1)>;
    using AllocatorType2 = hud_test::allocator_watcher_2<alignof(Type2)>;

    // Ensure we test with different allocator
    static_assert(!std::is_same_v<AllocatorType, AllocatorType2>);
    static_assert(!std::is_same_v<Type1, Type2>);
    static_assert(!hud::is_bitwise_copy_constructible_v<Type2, Type1>);

    // Test default allocator
    {
        auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
        {
            const hud::array<Type1, AllocatorType> copied(initializer, copied_extra);

            // Copy the array
            hud::array<Type2, AllocatorType2> copy(copied);

            // Ensure we copy all datas in order
            bool all_values_copied = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                if (static_cast<usize>(copy[index].id()) != index || copy[index].copy_constructor_count() != 1u)
                {
                    all_values_copied = false;
                    break;
                }
            }

            return std::tuple {
                // Ensure we copy all datas in order
                copy.data() != nullptr,
                copy.count(),
                copy.max_count(),
                all_values_copied,

                // Ensure the copy data is not the same memory of the copied data
                copied.data() != copy.data(),

                // Ensure we are allocating only one time
                copy.allocator().allocation_count(),
                copy.allocator().free_count()
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
        }
    }

    // Test with allocator
    {
        auto test_with_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
        {
            const hud::array<Type1, AllocatorType> copied(initializer, copied_extra);

            // Copy the array
            hud::array<Type2, AllocatorType2> copy(copied, AllocatorType2 {});

            // Ensure we copy all datas in order
            bool all_values_copied = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                if (static_cast<usize>(copy[index].id()) != index || copy[index].copy_constructor_count() != 1u)
                {
                    all_values_copied = false;
                    break;
                }
            }

            return std::tuple {
                // Ensure we copy all datas in order
                copy.data() != nullptr,
                copy.count(),
                copy.max_count(),
                all_values_copied,

                // Ensure the copy data is not the same memory of the copied data
                copied.data() != copy.data(),

                // Ensure we are allocating only one time
                copy.allocator().allocation_count(),
                copy.allocator().free_count()
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

            // Ensure we are allocating only one time
            hud_assert_eq(std::get<5>(result), 1u);
            hud_assert_eq(std::get<6>(result), 0u);
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

            // Ensure we are allocating only one time
            hud_assert_eq(std::get<5>(result), 1u);
            hud_assert_eq(std::get<6>(result), 0u);
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
                        const hud::array<Type1, AllocatorType> copied(initializer, copied_extra);

                        // Copy the array
                        hud::array<Type2, AllocatorType2> copy(copied, extra);

                        // Ensure we copy all datas in order
                        bool all_values_copied = true;
                        for (usize index = 0; index < initializer.size(); index++)
                        {
                            if (static_cast<usize>(copy[index].id()) != index || copy[index].copy_constructor_count() != 1u)
                            {
                                all_values_copied = false;
                                break;
                            }
                        }

                        return std::tuple {
                            // Ensure we copy all datas in order
                            copy.data() != nullptr,
                            copy.count(),
                            copy.max_count(),
                            all_values_copied,

                            // Ensure the copy data is not the same memory of the copied data
                            copied.data() != copy.data(),

                            // Ensure we are allocating only one time
                            copy.allocator().allocation_count(),
                            copy.allocator().free_count()
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

                        // Ensure we are allocating only one time
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
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

                        // Ensure we are allocating only one time
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                    }
                }

                // Test with allocator
                {
                    auto test_with_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
                    {
                        const hud::array<Type1, AllocatorType> copied(initializer, copied_extra);

                        // Copy the array
                        hud::array<Type2, AllocatorType2> copy(copied, extra, AllocatorType2 {});

                        // Ensure we copy all datas in order
                        bool all_values_copied = true;
                        for (usize index = 0; index < initializer.size(); index++)
                        {
                            if (static_cast<usize>(copy[index].id()) != index || copy[index].copy_constructor_count() != 1u)
                            {
                                all_values_copied = false;
                                break;
                            }
                        }

                        return std::tuple {
                            // Ensure we copy all datas in order
                            copy.data() != nullptr,
                            copy.count(),
                            copy.max_count(),
                            all_values_copied,

                            // Ensure the copy data is not the same memory of the copied data
                            copied.data() != copy.data(),

                            // Ensure we are allocating only one time
                            copy.allocator().allocation_count(),
                            copy.allocator().free_count()
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

                        // Ensure we are allocating only one time
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
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

                        // Ensure we are allocating only one time
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                    }
                }
            }
        );
    }
}

GTEST_TEST(array, copy_construct_non_bitwise_constructible_same_type_same_allocator)
{

    /** The array we copyfor test, we allocate also extra memory to test if we really copy the count(), not the max_count() elements */
    using type = hud_test::non_bitwise_type;
    using AllocatorType = hud_test::allocator_watcher<alignof(type)>;

    // Ensure we test with different allocator
    static_assert(!hud::is_bitwise_move_constructible_v<type>);

    using CopiedType = const hud::array<type, AllocatorType>;

    // Test default allocator
    {
        auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
        {
            const CopiedType copied(initializer, copied_extra);

            // Copy the array
            hud::array<type, AllocatorType> copy(copied);

            // Ensure we copy all datas in order
            bool all_values_copied = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                if (static_cast<usize>(copy[index].id()) != index || copy[index].copy_constructor_count() != 1u)
                {
                    all_values_copied = false;
                    break;
                }
            }

            return std::tuple {
                // Ensure we copy all datas in order
                copy.data() != nullptr,
                copy.count(),
                copy.max_count(),
                all_values_copied,

                // Ensure the copy data is not the same memory of the copied data
                copied.data() != copy.data(),

                // Ensure we are allocating only one time
                copy.allocator().allocation_count(),
                copy.allocator().free_count()
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
            hud_assert_eq(std::get<5>(result), 2u);
            hud_assert_eq(std::get<6>(result), 0u);
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
            hud_assert_eq(std::get<5>(result), 2u);
            hud_assert_eq(std::get<6>(result), 0u);
        }
    }

    // Test with allocator
    {
        auto test_wtih_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
        {
            const CopiedType copied(initializer, copied_extra);

            // Copy the array
            hud::array<type, AllocatorType> copy(copied, AllocatorType {});

            // Ensure we copy all datas in order
            bool all_values_copied = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                if (static_cast<usize>(copy[index].id()) != index || copy[index].copy_constructor_count() != 1u)
                {
                    all_values_copied = false;
                    break;
                }
            }

            return std::tuple {
                // Ensure we copy all datas in order
                copy.data() != nullptr,
                copy.count(),
                copy.max_count(),
                all_values_copied,

                // Ensure the copy data is not the same memory of the copied data
                copied.data() != copy.data(),

                // Ensure we are allocating only one time
                copy.allocator().allocation_count(),
                copy.allocator().free_count()
            };
        };

        // Non constant
        {
            const auto result = test_wtih_allocator({0, 1, 2, 3}, 1u);
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
        }

        // Constant
        {
            constexpr auto result = test_wtih_allocator({0, 1, 2, 3}, 1u);
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
                        const CopiedType copied(initializer, copied_extra);

                        // Copy the array
                        hud::array<type, AllocatorType> copy(copied, extra);

                        // Ensure we copy all datas in order
                        bool all_values_copied = true;
                        for (usize index = 0; index < initializer.size(); index++)
                        {
                            if (static_cast<usize>(copy[index].id()) != index || copy[index].copy_constructor_count() != 1u)
                            {
                                all_values_copied = false;
                                break;
                            }
                        }

                        return std::tuple {
                            // Ensure we copy all datas in order
                            copy.data() != nullptr,
                            copy.count(),
                            copy.max_count(),
                            all_values_copied,

                            // Ensure the copy data is not the same memory of the copied data
                            copied.data() != copy.data(),

                            // Ensure we are allocating only one time
                            copy.allocator().allocation_count(),
                            copy.allocator().free_count()
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

                        // Ensure we are allocating only one time
                        hud_assert_eq(std::get<5>(result), 2u);
                        hud_assert_eq(std::get<6>(result), 0u);
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

                        // Ensure we are allocating only one time
                        hud_assert_eq(std::get<5>(result), 2u);
                        hud_assert_eq(std::get<6>(result), 0u);
                    }
                }

                // Test with allocator
                {
                    auto test_with_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
                    {
                        const CopiedType copied(initializer, copied_extra);

                        // Copy the array
                        hud::array<type, AllocatorType> copy(copied, extra, AllocatorType {});

                        // Ensure we copy all datas in order
                        bool all_values_copied = true;
                        for (usize index = 0; index < initializer.size(); index++)
                        {
                            if (static_cast<usize>(copy[index].id()) != index || copy[index].copy_constructor_count() != 1u)
                            {
                                all_values_copied = false;
                                break;
                            }
                        }

                        return std::tuple {
                            // Ensure we copy all datas in order
                            copy.data() != nullptr,
                            copy.count(),
                            copy.max_count(),
                            all_values_copied,

                            // Ensure the copy data is not the same memory of the copied data
                            copied.data() != copy.data(),

                            // Ensure we are allocating only one time
                            copy.allocator().allocation_count(),
                            copy.allocator().free_count()
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

                        // Ensure we are allocating only one time
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
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

                        // Ensure we are allocating only one time
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                    }
                }
            }
        );
    }
}

GTEST_TEST(array, copy_construct_non_bitwise_constructible_same_type_different_allocator)
{

    /** The array we copyfor test, we allocate also extra memory to test if we really copy the count(), not the max_count() elements */
    using type = hud_test::non_bitwise_type;
    using AllocatorType = hud_test::allocator_watcher<alignof(type)>;
    using AllocatorType2 = hud_test::allocator_watcher_2<alignof(type)>;

    // Ensure we test with different allocator
    static_assert(!std::is_same_v<AllocatorType, AllocatorType2>);
    static_assert(!hud::is_bitwise_copy_constructible_v<type>);

    using CopiedType = const hud::array<type, AllocatorType>;

    // Test default allocator
    {
        auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
        {
            const CopiedType copied(initializer, copied_extra);

            // Copy the array
            hud::array<type, AllocatorType2> copy(copied);

            // Ensure we copy all datas in order
            bool all_values_copied = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                if (static_cast<usize>(copy[index].id()) != index || copy[index].copy_constructor_count() != 1u)
                {
                    all_values_copied = false;
                    break;
                }
            }

            return std::tuple {
                // Ensure we copy all datas in order
                copy.data() != nullptr,
                copy.count(),
                copy.max_count(),
                all_values_copied,

                // Ensure the copy data is not the same memory of the copied data
                copied.data() != copy.data(),

                // Ensure we are allocating only one time
                copy.allocator().allocation_count(),
                copy.allocator().free_count()
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
        }
    }

    // Test with allocator
    {
        auto test_wtih_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
        {
            const CopiedType copied(initializer, copied_extra);

            // Copy the array
            hud::array<type, AllocatorType2> copy(copied, AllocatorType2 {});

            // Ensure we copy all datas in order
            bool all_values_copied = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                if (static_cast<usize>(copy[index].id()) != index || copy[index].copy_constructor_count() != 1u)
                {
                    all_values_copied = false;
                    break;
                }
            }

            return std::tuple {
                // Ensure we copy all datas in order
                copy.data() != nullptr,
                copy.count(),
                copy.max_count(),
                all_values_copied,

                // Ensure the copy data is not the same memory of the copied data
                copied.data() != copy.data(),

                // Ensure we are allocating only one time
                copy.allocator().allocation_count(),
                copy.allocator().free_count()
            };
        };

        // Non constant
        {
            const auto result = test_wtih_allocator({0, 1, 2, 3}, 1u);
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
        }

        // Constant
        {
            constexpr auto result = test_wtih_allocator({0, 1, 2, 3}, 1u);
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
                        const CopiedType copied(initializer, copied_extra);

                        // Copy the array
                        hud::array<type, AllocatorType2> copy(copied, extra);

                        // Ensure we copy all datas in order
                        bool all_values_copied = true;
                        for (usize index = 0; index < initializer.size(); index++)
                        {
                            if (static_cast<usize>(copy[index].id()) != index || copy[index].copy_constructor_count() != 1u)
                            {
                                all_values_copied = false;
                                break;
                            }
                        }

                        return std::tuple {
                            // Ensure we copy all datas in order
                            copy.data() != nullptr,
                            copy.count(),
                            copy.max_count(),
                            all_values_copied,

                            // Ensure the copy data is not the same memory of the copied data
                            copied.data() != copy.data(),

                            // Ensure we are allocating only one time
                            copy.allocator().allocation_count(),
                            copy.allocator().free_count()
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

                        // Ensure we are allocating only one time
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
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

                        // Ensure we are allocating only one time
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                    }
                }

                // Test with allocator
                {
                    auto test_with_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
                    {
                        const CopiedType copied(initializer, copied_extra);

                        // Copy the array
                        hud::array<type, AllocatorType2> copy(copied, extra, AllocatorType2 {});

                        // Ensure we copy all datas in order
                        bool all_values_copied = true;
                        for (usize index = 0; index < initializer.size(); index++)
                        {
                            if (static_cast<usize>(copy[index].id()) != index || copy[index].copy_constructor_count() != 1u)
                            {
                                all_values_copied = false;
                                break;
                            }
                        }

                        return std::tuple {
                            // Ensure we copy all datas in order
                            copy.data() != nullptr,
                            copy.count(),
                            copy.max_count(),
                            all_values_copied,

                            // Ensure the copy data is not the same memory of the copied data
                            copied.data() != copy.data(),

                            // Ensure we are allocating only one time
                            copy.allocator().allocation_count(),
                            copy.allocator().free_count()
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

                        // Ensure we are allocating only one time
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
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

                        // Ensure we are allocating only one time
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                    }
                }
            }
        );
    }
}

GTEST_TEST(array, copy_construct_non_bitwise_constructible_different_type_same_allocator)
{

    /** The array we copyfor test, we allocate also extra memory to test if we really copy the count(), not the max_count() elements */
    using Type1 = hud_test::non_bitwise_type;
    using Type2 = hud_test::non_bitwise_type2;
    using AllocatorType = hud_test::allocator_watcher<alignof(Type1)>;

    // Ensure we test with different allocator
    static_assert(!std::is_same_v<Type1, Type2>);
    static_assert(!hud::is_bitwise_copy_constructible_v<Type2, Type1>);

    // Test default allocator
    {
        auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
        {
            const hud::array<Type1, AllocatorType> copied(initializer, copied_extra);

            // Copy the array
            hud::array<Type2, AllocatorType> copy(copied);

            // Ensure we copy all datas in order
            bool all_values_copied = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                if (static_cast<usize>(copy[index].id()) != index || copy[index].copy_constructor_count() != 1u)
                {
                    all_values_copied = false;
                    break;
                }
            }

            return std::tuple {
                // Ensure we copy all datas in order
                copy.data() != nullptr,
                copy.count(),
                copy.max_count(),
                all_values_copied,

                // Ensure the copy data is not the same memory of the copied data
                copied.data() != copy.data(),

                // Ensure we are allocating only one time
                copy.allocator().allocation_count(),
                copy.allocator().free_count()
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
        }
    }

    // Test with allocator
    {
        auto test_with_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
        {
            const hud::array<Type1, AllocatorType> copied(initializer, copied_extra);

            // Copy the array
            hud::array<Type2, AllocatorType> copy(copied, AllocatorType {});

            // Ensure we copy all datas in order
            bool all_values_copied = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                if (static_cast<usize>(copy[index].id()) != index || copy[index].copy_constructor_count() != 1u)
                {
                    all_values_copied = false;
                    break;
                }
            }

            return std::tuple {
                // Ensure we copy all datas in order
                copy.data() != nullptr,
                copy.count(),
                copy.max_count(),
                all_values_copied,

                // Ensure the copy data is not the same memory of the copied data
                copied.data() != copy.data(),

                // Ensure we are allocating only one time
                copy.allocator().allocation_count(),
                copy.allocator().free_count()
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

            // Ensure we are allocating only one time
            hud_assert_eq(std::get<5>(result), 1u);
            hud_assert_eq(std::get<6>(result), 0u);
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

            // Ensure we are allocating only one time
            hud_assert_eq(std::get<5>(result), 1u);
            hud_assert_eq(std::get<6>(result), 0u);
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
                        const hud::array<Type1, AllocatorType> copied(initializer, copied_extra);

                        // Copy the array
                        hud::array<Type2, AllocatorType> copy(copied, extra);

                        // Ensure we copy all datas in order
                        bool all_values_copied = true;
                        for (usize index = 0; index < initializer.size(); index++)
                        {
                            if (static_cast<usize>(copy[index].id()) != index || copy[index].copy_constructor_count() != 1u)
                            {
                                all_values_copied = false;
                                break;
                            }
                        }

                        return std::tuple {
                            // Ensure we copy all datas in order
                            copy.data() != nullptr,
                            copy.count(),
                            copy.max_count(),
                            all_values_copied,

                            // Ensure the copy data is not the same memory of the copied data
                            copied.data() != copy.data(),

                            // Ensure we are allocating only one time
                            copy.allocator().allocation_count(),
                            copy.allocator().free_count()
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

                        // Ensure we are allocating only one time
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
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

                        // Ensure we are allocating only one time
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                    }
                }

                // Test with allocator
                {
                    auto test_with_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
                    {
                        const hud::array<Type1, AllocatorType> copied(initializer, copied_extra);

                        // Copy the array
                        hud::array<Type2, AllocatorType> copy(copied, extra, AllocatorType {});

                        // Ensure we copy all datas in order
                        bool all_values_copied = true;
                        for (usize index = 0; index < initializer.size(); index++)
                        {
                            if (static_cast<usize>(copy[index].id()) != index || copy[index].copy_constructor_count() != 1u)
                            {
                                all_values_copied = false;
                                break;
                            }
                        }

                        return std::tuple {
                            // Ensure we copy all datas in order
                            copy.data() != nullptr,
                            copy.count(),
                            copy.max_count(),
                            all_values_copied,

                            // Ensure the copy data is not the same memory of the copied data
                            copied.data() != copy.data(),

                            // Ensure we are allocating only one time
                            copy.allocator().allocation_count(),
                            copy.allocator().free_count()
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

                        // Ensure we are allocating only one time
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
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

                        // Ensure we are allocating only one time
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                    }
                }
            }
        );
    }
}

GTEST_TEST(array, copy_construct_non_bitwise_constructible_different_type_different_allocator)
{

    /** The array we copyfor test, we allocate also extra memory to test if we really copy the count(), not the max_count() elements */
    using Type1 = hud_test::non_bitwise_type;
    using Type2 = hud_test::non_bitwise_type2;
    using AllocatorType = hud_test::allocator_watcher<alignof(Type1)>;
    using AllocatorType2 = hud_test::allocator_watcher_2<alignof(Type2)>;

    // Ensure we test with different allocator
    static_assert(!std::is_same_v<AllocatorType, AllocatorType2>);
    static_assert(!std::is_same_v<Type1, Type2>);
    static_assert(!hud::is_bitwise_copy_constructible_v<Type2, Type1>);

    // Test default allocator
    {
        auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
        {
            const hud::array<Type1, AllocatorType> copied(initializer, copied_extra);

            // Copy the array
            hud::array<Type2, AllocatorType2> copy(copied);

            // Ensure we copy all datas in order
            bool all_values_copied = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                if (static_cast<usize>(copy[index].id()) != index || copy[index].copy_constructor_count() != 1u)
                {
                    all_values_copied = false;
                    break;
                }
            }

            return std::tuple {
                // Ensure we copy all datas in order
                copy.data() != nullptr,
                copy.count(),
                copy.max_count(),
                all_values_copied,

                // Ensure the copy data is not the same memory of the copied data
                copied.data() != copy.data(),

                // Ensure we are allocating only one time
                copy.allocator().allocation_count(),
                copy.allocator().free_count()
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
        }
    }

    // Test with allocator
    {
        auto test_with_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
        {
            const hud::array<Type1, AllocatorType> copied(initializer, copied_extra);

            // Copy the array
            hud::array<Type2, AllocatorType2> copy(copied, AllocatorType2 {});

            // Ensure we copy all datas in order
            bool all_values_copied = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                if (static_cast<usize>(copy[index].id()) != index || copy[index].copy_constructor_count() != 1u)
                {
                    all_values_copied = false;
                    break;
                }
            }

            return std::tuple {
                // Ensure we copy all datas in order
                copy.data() != nullptr,
                copy.count(),
                copy.max_count(),
                all_values_copied,

                // Ensure the copy data is not the same memory of the copied data
                copied.data() != copy.data(),

                // Ensure we are allocating only one time
                copy.allocator().allocation_count(),
                copy.allocator().free_count()
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

            // Ensure we are allocating only one time
            hud_assert_eq(std::get<5>(result), 1u);
            hud_assert_eq(std::get<6>(result), 0u);
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

            // Ensure we are allocating only one time
            hud_assert_eq(std::get<5>(result), 1u);
            hud_assert_eq(std::get<6>(result), 0u);
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
                        const hud::array<Type1, AllocatorType> copied(initializer, copied_extra);

                        // Copy the array
                        hud::array<Type2, AllocatorType2> copy(copied, extra);

                        // Ensure we copy all datas in order
                        bool all_values_copied = true;
                        for (usize index = 0; index < initializer.size(); index++)
                        {
                            if (static_cast<usize>(copy[index].id()) != index || copy[index].copy_constructor_count() != 1u)
                            {
                                all_values_copied = false;
                                break;
                            }
                        }

                        return std::tuple {
                            // Ensure we copy all datas in order
                            copy.data() != nullptr,
                            copy.count(),
                            copy.max_count(),
                            all_values_copied,

                            // Ensure the copy data is not the same memory of the copied data
                            copied.data() != copy.data(),

                            // Ensure we are allocating only one time
                            copy.allocator().allocation_count(),
                            copy.allocator().free_count()
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

                        // Ensure we are allocating only one time
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
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

                        // Ensure we are allocating only one time
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                    }
                }

                // Test with allocator
                {
                    auto test_with_allocator = [](std::initializer_list<i32> initializer, usize copied_extra)
                    {
                        const hud::array<Type1, AllocatorType> copied(initializer, copied_extra);

                        // Copy the array
                        hud::array<Type2, AllocatorType2> copy(copied, extra, AllocatorType2 {});

                        // Ensure we copy all datas in order
                        bool all_values_copied = true;
                        for (usize index = 0; index < initializer.size(); index++)
                        {
                            if (static_cast<usize>(copy[index].id()) != index || copy[index].copy_constructor_count() != 1u)
                            {
                                all_values_copied = false;
                                break;
                            }
                        }

                        return std::tuple {
                            // Ensure we copy all datas in order
                            copy.data() != nullptr,
                            copy.count(),
                            copy.max_count(),
                            all_values_copied,

                            // Ensure the copy data is not the same memory of the copied data
                            copied.data() != copy.data(),

                            // Ensure we are allocating only one time
                            copy.allocator().allocation_count(),
                            copy.allocator().free_count()
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

                        // Ensure we are allocating only one time
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
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

                        // Ensure we are allocating only one time
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                    }
                }
            }
        );
    }
}
