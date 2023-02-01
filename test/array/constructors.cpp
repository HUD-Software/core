#include <core/containers/array.h>
#include "../misc/allocators.h"

GTEST_TEST(array, default_constructor_should_allocate_no_memory)
{
    auto test = []() -> std::tuple<bool, usize, usize>
    {
        hud::array<hud::u32> array;
        return {array.data() == nullptr, array.count(), array.max_count()};
    };

    // Non Constant
    {
        auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 0u);
        hud_assert_eq(std::get<2>(result), 0u);
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_eq(std::get<1>(result), 0u);
        hud_assert_eq(std::get<2>(result), 0u);
    }
}

GTEST_TEST(array, construct_by_copying_raw_data_array_of_bitwise_copy_constructible_same_type)
{

    using TypeList = std::tuple<i8, i16, i32, i64, u8, u16, u32, u64, uptr, iptr, usize, isize>;

    hud_test::for_each_type<TypeList>()([]<typename type_t>() noexcept
                                        {

        static_assert(hud::is_bitwise_copy_constructible_v<type_t>);


        // test with default allocator no extra
        {
            auto test_default_allocator = [](const type_t* raw, usize element_count) {
                hud::array<type_t> array(raw, element_count);
                bool all_values_copied = true;

                // Ensure values are correclty copied
                for (usize index = 0; index < element_count; index++) {
                    if (*(array.data() + index) != raw[index]) {
                        all_values_copied = false;
                        break;
                    }
                }

                return std::tuple{
                    // Allocation of 4 i32 should be done
                    array.data() != nullptr,
                    array.count(),
                    array.max_count(),

                    // Ensure it's not the same memory buffer
                    array.data() != &raw[0],

                    // Ensure values are correclty copied
                    all_values_copied
                };
            };

            // Non Constant
            {
                constexpr usize element_count = 4;
                constexpr type_t raw[element_count] = { 1,2,3,4 };
                const auto result = test_default_allocator(raw, element_count);
                // Allocation of 4 i32 should be done
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), element_count);
                hud_assert_eq(std::get<2>(result), element_count);

                // Ensure it's not the same memory buffer
                hud_assert_true(std::get<3>(result));

                // Ensure values are correclty copied
                hud_assert_true(std::get<4>(result));
            }

            // Constant
            {
                constexpr usize element_count = 4;
                constexpr type_t raw[element_count] = { 1,2,3,4 };
                constexpr auto result = test_default_allocator(raw, element_count);
                // Allocation of 4 i32 should be done
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), element_count);
                hud_assert_eq(std::get<2>(result), element_count);

                // Ensure it's not the same memory buffer
                hud_assert_true(std::get<3>(result));

                // Ensure values are correclty copied
                hud_assert_true(std::get<4>(result));
            }
        }

        // test with allocator no extra
        {
            auto test_with_allocator = [](const type_t* raw, usize element_count) {
                hud::array<type_t, hud_test::array_allocator<alignof(type_t)>> array(raw, element_count, hud_test::array_allocator<alignof(type_t)>{});
                bool all_values_copied = true;

                // Ensure values are correclty copied
                for (usize index = 0; index < element_count; index++) {
                    if (*(array.data() + index) != raw[index]) {
                        all_values_copied = false;
                        break;
                    }
                }

                return std::tuple{
                    // Allocation of 4 i32 should be done
                    array.data() != nullptr,
                    array.count(),
                    array.max_count(),

                    // Ensure it's not the same memory buffer
                    array.data() != &raw[0],

                    // Ensure values are correclty copied
                    all_values_copied,

                    // Ensure we are allocating only one time
                    array.allocator().allocation_count(),
                    array.allocator().free_count()
                };
            };

            // Non Constant
            {
                constexpr usize element_count = 4;
                constexpr type_t raw[element_count] = { 1,2,3,4 };
                const auto result = test_with_allocator(raw, element_count);
                // Allocation of 4 i32 should be done
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), element_count);
                hud_assert_eq(std::get<2>(result), element_count);

                // Ensure it's not the same memory buffer
                hud_assert_true(std::get<3>(result));

                // Ensure values are correclty copied
                hud_assert_true(std::get<4>(result));

                // Ensure we are allocating only one time
                hud_assert_eq(std::get<5>(result), 1u);
                hud_assert_eq(std::get<6>(result), 0u);
            }

            // Constant
            {
                constexpr usize element_count = 4;
                constexpr type_t raw[element_count] = { 1,2,3,4 };
                constexpr auto result = test_with_allocator(raw, element_count);
                // Allocation of 4 i32 should be done
                hud_assert_true(std::get<0>(result));
                hud_assert_eq(std::get<1>(result), element_count);
                hud_assert_eq(std::get<2>(result), element_count);

                // Ensure it's not the same memory buffer
                hud_assert_true(std::get<3>(result));

                // Ensure values are correclty copied
                hud_assert_true(std::get<4>(result));

                // Ensure we are allocating only one time
                hud_assert_eq(std::get<5>(result), 1u);
                hud_assert_eq(std::get<6>(result), 0u);
            }
        }

        {
            hud_test::for_each_value<std::make_integer_sequence<usize, 5>>()(
                []<usize extra>() {

                // Test default allocator with extra
                    {
                        auto test_default_allocator = [](const type_t* raw, usize element_count) {
                            hud::array<type_t> array(raw, element_count, extra);
                            bool all_values_copied = true;

                            // Ensure values are correclty copied
                            for (usize index = 0; index < element_count; index++) {
                                if (*(array.data() + index) != raw[index]) {
                                    all_values_copied = false;
                                    break;
                                }
                            }

                            return std::tuple{
                                // Allocation of 4 i32 should be done
                                array.data() != nullptr,
                                array.count(),
                                array.max_count(),

                                // Ensure it's not the same memory buffer
                                array.data() != &raw[0],

                                // Ensure values are correclty copied
                                all_values_copied
                            };

                        };
                        // Non Constant
                        {
                            constexpr usize element_count = 4;
                            constexpr type_t raw[element_count] = { 1,2,3,4 };
                            const auto result = test_default_allocator(raw, element_count);
                            // Allocation of 4 i32 should be done
                            hud_assert_true(std::get<0>(result));
                            hud_assert_eq(std::get<1>(result), element_count);
                            hud_assert_eq(std::get<2>(result), element_count + extra);

                            // Ensure it's not the same memory buffer
                            hud_assert_true(std::get<3>(result));

                            // Ensure values are correclty copied
                            hud_assert_true(std::get<4>(result));
                        }

                        // Constant
                        {
                            constexpr usize element_count = 4;
                            constexpr type_t raw[element_count] = { 1,2,3,4 };
                            constexpr auto result = test_default_allocator(raw, element_count);
                            // Allocation of 4 i32 should be done
                            hud_assert_true(std::get<0>(result));
                            hud_assert_eq(std::get<1>(result), element_count);
                            hud_assert_eq(std::get<2>(result), element_count + extra);

                            // Ensure it's not the same memory buffer
                            hud_assert_true(std::get<3>(result));

                            // Ensure values are correclty copied
                            hud_assert_true(std::get<4>(result));
                        }

                    }

                    // Test with allocator with extra
                    {
                        auto test_with_allocator = [](const type_t* raw, usize element_count) {
                            hud::array<type_t, hud_test::array_allocator<alignof(type_t)>> array(raw, element_count, extra, hud_test::array_allocator<alignof(type_t)>{});
                            bool all_values_copied = true;

                            // Ensure values are correclty copied
                            for (usize index = 0; index < element_count; index++) {
                                if (*(array.data() + index) != raw[index]) {
                                    all_values_copied = false;
                                    break;
                                }
                            }

                            return std::tuple{
                                // Allocation of 4 i32 should be done
                                array.data() != nullptr,
                                array.count(),
                                array.max_count(),

                                // Ensure it's not the same memory buffer
                                array.data() != &raw[0],

                                // Ensure values are correclty copied
                                all_values_copied,

                                // Ensure we are allocating only one time
                                array.allocator().allocation_count(),
                                array.allocator().free_count()
                            };
                        };

                        // Non Constant
                        {
                            constexpr usize element_count = 4;
                            constexpr type_t raw[element_count] = { 1,2,3,4 };
                            const auto result = test_with_allocator(raw, element_count);
                            // Allocation of 4 i32 should be done
                            hud_assert_true(std::get<0>(result));
                            hud_assert_eq(std::get<1>(result), element_count);
                            hud_assert_eq(std::get<2>(result), element_count + extra);

                            // Ensure it's not the same memory buffer
                            hud_assert_true(std::get<3>(result));

                            // Ensure values are correclty copied
                            hud_assert_true(std::get<4>(result));

                            // Ensure we are allocating only one time
                            hud_assert_eq(std::get<5>(result), 1u);
                            hud_assert_eq(std::get<6>(result), 0u);
                        }

                        // Constant
                        {
                            constexpr usize element_count = 4;
                            constexpr type_t raw[element_count] = { 1,2,3,4 };
                            constexpr auto result = test_with_allocator(raw, element_count);

                            // Allocation of 4 i32 should be done
                            hud_assert_true(std::get<0>(result));
                            hud_assert_eq(std::get<1>(result), element_count);
                            hud_assert_eq(std::get<2>(result), element_count + extra);

                            // Ensure it's not the same memory buffer
                            hud_assert_true(std::get<3>(result));

                            // Ensure values are correclty copied
                            hud_assert_true(std::get<4>(result));

                            // Ensure we are allocating only one time
                            hud_assert_eq(std::get<5>(result), 1u);
                            hud_assert_eq(std::get<6>(result), 0u);
                        }
                    }
            });
        } });
}

GTEST_TEST(array, construct_by_copying_raw_data_array_of_non_bitwise_copy_constructible_same_type)
{

    using type = hud_test::NonBitwiseCopyConstructibleType;
    static_assert(!hud::is_bitwise_copy_constructible_v<type>);

    // test with default allocator no extra
    {
        auto test_default_allocator = [](const type *raw, usize element_count)
        {
            hud::array<type> array(raw, element_count);

            // Ensure values are correclty copied in order
            bool all_values_copied_in_order = true;
            for (usize index = 0; index < element_count; index++)
            {
                if (array[index].id() != raw[index].id())
                {
                    all_values_copied_in_order = false;
                    break;
                }
            }

            // Ensure the copy constructor are called
            bool all_copy_constructor_are_called = true;
            for (usize index = 0; index < element_count; index++)
            {
                // Ensure we call the copy constructor and the id is correctly copied
                if (array[index].copy_constructor_count() != 1u || array[index].id() != raw[index].id())
                {
                    all_copy_constructor_are_called = false;
                    break;
                }
            }

            return std::tuple {
                // Allocation of 4 i32 should be done
                array.data() != nullptr,
                array.count(),
                array.max_count(),

                // Ensure it's not the same memory buffer
                array.data() != &raw[0],

                // Ensure values are correclty copied in order
                all_values_copied_in_order,

                // Ensure the copy constructor are called
                all_copy_constructor_are_called

            };
        };

        // Non Constant
        {
            constexpr usize element_count = 4;
            constexpr type raw[element_count] = {1, 2, 3, 4};
            const auto result = test_default_allocator(raw, element_count);
            // Allocation of 4 i32 should be done
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), element_count);
            hud_assert_eq(std::get<2>(result), element_count);

            // Ensure it's not the same memory buffer
            hud_assert_true(std::get<3>(result));

            // Ensure values are correclty copied in order
            hud_assert_true(std::get<4>(result));

            // Ensure the copy constructor are called
            hud_assert_true(std::get<5>(result));
        }

        // Constant
        {
            constexpr usize element_count = 4;
            constexpr type raw[element_count] = {1, 2, 3, 4};
            constexpr auto result = test_default_allocator(raw, element_count);
            // Allocation of 4 i32 should be done
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), element_count);
            hud_assert_eq(std::get<2>(result), element_count);

            // Ensure it's not the same memory buffer
            hud_assert_true(std::get<3>(result));

            // Ensure values are correclty copied in order
            hud_assert_true(std::get<4>(result));

            // Ensure the copy constructor are called
            hud_assert_true(std::get<5>(result));
        }
    }

    // test with allocator no extra
    {
        auto test_with_allocator = [](const type *raw, usize element_count)
        {
            hud::array<type, hud_test::array_allocator<alignof(type)>> array(raw, element_count, hud_test::array_allocator<alignof(type)> {});

            // Ensure values are correclty copied in order
            bool all_values_copied_in_order = true;
            for (usize index = 0; index < element_count; index++)
            {
                if (array[index].id() != raw[index].id())
                {
                    all_values_copied_in_order = false;
                    break;
                }
            }

            // Ensure the copy constructor are called
            bool all_copy_constructor_are_called = true;
            for (usize index = 0; index < element_count; index++)
            {
                // Ensure we call the copy constructor and the id is correctly copied
                if (array[index].copy_constructor_count() != 1u || array[index].id() != raw[index].id())
                {
                    all_copy_constructor_are_called = false;
                    break;
                }
            }
            return std::tuple {
                // Allocation of 4 i32 should be done
                array.data() != nullptr,
                array.count(),
                array.max_count(),

                // Ensure it's not the same memory buffer
                array.data() != &raw[0],

                // Ensure values are correclty copied in order
                all_values_copied_in_order,

                // Ensure the copy constructor are called
                all_copy_constructor_are_called,

                // Ensure we are allocating only one time
                array.allocator().allocation_count(),
                array.allocator().free_count()

            };
        };

        // Non Constant
        {
            constexpr usize element_count = 4;
            constexpr type raw[element_count] = {1, 2, 3, 4};
            const auto result = test_with_allocator(raw, element_count);
            // Allocation of 4 i32 should be done
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), element_count);
            hud_assert_eq(std::get<2>(result), element_count);

            // Ensure it's not the same memory buffer
            hud_assert_true(std::get<3>(result));

            // Ensure values are correclty copied in order
            hud_assert_true(std::get<4>(result));

            // Ensure the copy constructor are called
            hud_assert_true(std::get<5>(result));

            // Ensure we are allocating only one time
            hud_assert_eq(std::get<6>(result), 1u);
            hud_assert_eq(std::get<7>(result), 0u);
        }

        // Constant
        {
            constexpr usize element_count = 4;
            constexpr type raw[element_count] = {1, 2, 3, 4};
            constexpr auto result = test_with_allocator(raw, element_count);
            // Allocation of 4 i32 should be done
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), element_count);
            hud_assert_eq(std::get<2>(result), element_count);

            // Ensure it's not the same memory buffer
            hud_assert_true(std::get<3>(result));

            // Ensure values are correclty copied in order
            hud_assert_true(std::get<4>(result));

            // Ensure the copy constructor are called
            hud_assert_true(std::get<5>(result));

            // Ensure we are allocating only one time
            hud_assert_eq(std::get<6>(result), 1u);
            hud_assert_eq(std::get<7>(result), 0u);
        }
    }

    {
        hud_test::for_each_value<std::make_integer_sequence<usize, 5>>()(
            []<usize extra>()
            {
                // Test default allcoator with extra
                {
                    auto test_default_allocator = [](const type *raw, usize element_count)
                    {
                        hud::array<type> array(raw, element_count, extra);

                        // Ensure values are correclty copied in order
                        bool all_values_copied_in_order = true;
                        for (usize index = 0; index < element_count; index++)
                        {
                            if (array[index].id() != raw[index].id())
                            {
                                all_values_copied_in_order = false;
                                break;
                            }
                        }

                        // Ensure the copy constructor are called
                        bool all_copy_constructor_are_called = true;
                        for (usize index = 0; index < element_count; index++)
                        {
                            // Ensure we call the copy constructor and the id is correctly copied
                            if (array[index].copy_constructor_count() != 1u || array[index].id() != raw[index].id())
                            {
                                all_copy_constructor_are_called = false;
                                break;
                            }
                        }

                        return std::tuple {
                            // Allocation of 4 i32 should be done
                            array.data() != nullptr,
                            array.count(),
                            array.max_count(),

                            // Ensure it's not the same memory buffer
                            array.data() != &raw[0],

                            // Ensure values are correclty copied in order
                            all_values_copied_in_order,

                            // Ensure the copy constructor are called
                            all_copy_constructor_are_called

                        };
                    };

                    // Non Constant
                    {
                        constexpr usize element_count = 4;
                        constexpr type raw[element_count] = {1, 2, 3, 4};
                        const auto result = test_default_allocator(raw, element_count);
                        // Allocation of 4 i32 should be done
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), element_count);
                        hud_assert_eq(std::get<2>(result), element_count + extra);

                        // Ensure it's not the same memory buffer
                        hud_assert_true(std::get<3>(result));

                        // Ensure values are correclty copied in order
                        hud_assert_true(std::get<4>(result));

                        // Ensure the copy constructor are called
                        hud_assert_true(std::get<5>(result));
                    }

                    // Constant
                    {
                        constexpr usize element_count = 4;
                        constexpr type raw[element_count] = {1, 2, 3, 4};
                        constexpr auto result = test_default_allocator(raw, element_count);
                        // Allocation of 4 i32 should be done
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), element_count);
                        hud_assert_eq(std::get<2>(result), element_count + extra);

                        // Ensure it's not the same memory buffer
                        hud_assert_true(std::get<3>(result));

                        // Ensure values are correclty copied in order
                        hud_assert_true(std::get<4>(result));

                        // Ensure the copy constructor are called
                        hud_assert_true(std::get<5>(result));
                    }
                }

                // Test with allocator with extra
                {
                    auto test_with_allocator = [](const type *raw, usize element_count)
                    {
                        hud::array<type, hud_test::array_allocator<alignof(type)>> array(raw, element_count, extra, hud_test::array_allocator<alignof(type)> {});

                        // Ensure values are correclty copied in order
                        bool all_values_copied_in_order = true;
                        for (usize index = 0; index < element_count; index++)
                        {
                            if (array[index].id() != raw[index].id())
                            {
                                all_values_copied_in_order = false;
                                break;
                            }
                        }

                        // Ensure the copy constructor are called
                        bool all_copy_constructor_are_called = true;
                        for (usize index = 0; index < element_count; index++)
                        {
                            // Ensure we call the copy constructor and the id is correctly copied
                            if (array[index].copy_constructor_count() != 1u || array[index].id() != raw[index].id())
                            {
                                all_copy_constructor_are_called = false;
                                break;
                            }
                        }
                        return std::tuple {
                            // Allocation of 4 i32 should be done
                            array.data() != nullptr,
                            array.count(),
                            array.max_count(),

                            // Ensure it's not the same memory buffer
                            array.data() != &raw[0],

                            // Ensure values are correclty copied in order
                            all_values_copied_in_order,

                            // Ensure the copy constructor are called
                            all_copy_constructor_are_called,

                            // Ensure we are allocating only one time
                            array.allocator().allocation_count(),
                            array.allocator().free_count()

                        };
                    };

                    // Non Constant
                    {
                        constexpr usize element_count = 4;
                        constexpr type raw[element_count] = {1, 2, 3, 4};
                        const auto result = test_with_allocator(raw, element_count);
                        // Allocation of 4 i32 should be done
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), element_count);
                        hud_assert_eq(std::get<2>(result), element_count + extra);

                        // Ensure it's not the same memory buffer
                        hud_assert_true(std::get<3>(result));

                        // Ensure values are correclty copied in order
                        hud_assert_true(std::get<4>(result));

                        // Ensure the copy constructor are called
                        hud_assert_true(std::get<5>(result));

                        // Ensure we are allocating only one time
                        hud_assert_eq(std::get<6>(result), 1u);
                        hud_assert_eq(std::get<7>(result), 0u);
                    }

                    // Constant
                    {
                        constexpr usize element_count = 4;
                        constexpr type raw[element_count] = {1, 2, 3, 4};
                        constexpr auto result = test_with_allocator(raw, element_count);
                        // Allocation of 4 i32 should be done
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), element_count);
                        hud_assert_eq(std::get<2>(result), element_count + extra);

                        // Ensure it's not the same memory buffer
                        hud_assert_true(std::get<3>(result));

                        // Ensure values are correclty copied in order
                        hud_assert_true(std::get<4>(result));

                        // Ensure the copy constructor are called
                        hud_assert_true(std::get<5>(result));

                        // Ensure we are allocating only one time
                        hud_assert_eq(std::get<6>(result), 1u);
                        hud_assert_eq(std::get<7>(result), 0u);
                    }
                }
            }
        );
    }
}

GTEST_TEST(array, construct_by_copying_raw_data_array_of_bitwise_copy_constructible_different_type)
{

    using TypesToTest = std::tuple<i8, i16, i32, i64, u8, u16, u32, u64, f32, f64, uptr, iptr, usize, isize>;

    bool at_least_one_copy_tested = false;
    hud_test::for_each_type<TypesToTest>()([&at_least_one_copy_tested]<typename type_t>() noexcept
                                           { hud_test::for_each_type<TypesToTest>()([&at_least_one_copy_tested]<typename U>() noexcept
                                                                                    {

            // Test only types that are not the same but are bitwise copy constructible
            if constexpr (!std::is_same_v<type_t, U> && hud::is_bitwise_copy_constructible_v<type_t, U>)
            {
                at_least_one_copy_tested = true;

                //test with default allocator no extra
                {
                    auto test_default_allocator = [](const U* raw, usize element_count) {
                        hud::array<type_t> array(raw, element_count);
                        bool all_values_copied = true;

                        // Ensure values are correclty copied
                        for (usize index = 0; index < element_count; index++) {
                            if (*(array.data() + index) != static_cast<type_t>(raw[index])) {
                                all_values_copied = false;
                                break;
                            }
                        }

                        return std::tuple{
                            // Allocation of 4 i32 should be done
                            array.data() != nullptr,
                            array.count(),
                            array.max_count(),

                            // Ensure it's not the same memory buffer
                            (void*)array.data() != (void*)&raw[0],

                            // Ensure values are correclty copied
                            all_values_copied
                        };
                    };

                    // Non Constant
                    {
                        constexpr usize element_count = 4;
                        constexpr U raw[element_count] = { 1,2,3,4 };
                        const auto result = test_default_allocator(raw, element_count);
                        // Allocation of 4 i32 should be done
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), element_count);
                        hud_assert_eq(std::get<2>(result), element_count);

                        // Ensure it's not the same memory buffer
                        hud_assert_true(std::get<3>(result));

                        // Ensure values are correclty copied
                        hud_assert_true(std::get<4>(result));
                    }

                    // Constant
                    {
                        constexpr usize element_count = 4;
                        constexpr U raw[element_count] = { 1,2,3,4 };
                        constexpr auto result = test_default_allocator(raw, element_count);
                        // Allocation of 4 i32 should be done
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), element_count);
                        hud_assert_eq(std::get<2>(result), element_count);

                        // Ensure it's not the same memory buffer
                        hud_assert_true(std::get<3>(result));

                        // Ensure values are correclty copied
                        hud_assert_true(std::get<4>(result));
                    }
                }

                // test with allocator no extra
                {
                    auto test_with_allocator = [](const U* raw, usize element_count) {
                        hud::array<type_t, hud_test::array_allocator<alignof(type_t)>> array(raw, element_count, hud_test::array_allocator<alignof(type_t)>{});
                        bool all_values_copied = true;

                        // Ensure values are correclty copied
                        for (usize index = 0; index < element_count; index++) {
                            if (*(array.data() + index) != static_cast<type_t>(raw[index])) {
                                all_values_copied = false;
                                break;
                            }
                        }

                        return std::tuple{
                            // Allocation of 4 i32 should be done
                            array.data() != nullptr,
                            array.count(),
                            array.max_count(),

                            // Ensure it's not the same memory buffer
                            (void*)array.data() != (void*)&raw[0],

                            // Ensure values are correclty copied
                            all_values_copied,

                            // Ensure we are allocating only one time
                            array.allocator().allocation_count(),
                            array.allocator().free_count()
                        };
                    };

                    // Non Constant
                    {
                        constexpr usize element_count = 4;
                        constexpr U raw[element_count] = { 1,2,3,4 };
                        const auto result = test_with_allocator(raw, element_count);
                        // Allocation of 4 i32 should be done
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), element_count);
                        hud_assert_eq(std::get<2>(result), element_count);

                        // Ensure it's not the same memory buffer
                        hud_assert_true(std::get<3>(result));

                        // Ensure values are correclty copied
                        hud_assert_true(std::get<4>(result));

                        // Ensure we are allocating only one time
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                    }

                    // Constant
                    {
                        constexpr usize element_count = 4;
                        constexpr U raw[element_count] = { 1,2,3,4 };
                        constexpr auto result = test_with_allocator(raw, element_count);
                        // Allocation of 4 i32 should be done
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), element_count);
                        hud_assert_eq(std::get<2>(result), element_count);

                        // Ensure it's not the same memory buffer
                        hud_assert_true(std::get<3>(result));

                        // Ensure values are correclty copied
                        hud_assert_true(std::get<4>(result));

                        // Ensure we are allocating only one time
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                    }
                }

                {
                    hud_test::for_each_value<std::make_integer_sequence<usize, 5>>()(
                        []<usize extra>() {

                        // Test default allocator with extra
                            {
                                auto test_default_allocator = [](const U* raw, usize element_count) {
                                    hud::array<type_t> array(raw, element_count, extra);
                                    bool all_values_copied = true;

                                    // Ensure values are correclty copied
                                    for (usize index = 0; index < element_count; index++) {
                                        if (*(array.data() + index) != static_cast<type_t>(raw[index])) {
                                            all_values_copied = false;
                                            break;
                                        }
                                    }

                                    return std::tuple{
                                        // Allocation of 4 i32 should be done
                                        array.data() != nullptr,
                                        array.count(),
                                        array.max_count(),

                                        // Ensure it's not the same memory buffer
                                        (void*)array.data() != (void*)&raw[0],

                                        // Ensure values are correclty copied
                                        all_values_copied
                                    };

                                };
                                // Non Constant
                                {
                                    constexpr usize element_count = 4;
                                    constexpr U raw[element_count] = { 1,2,3,4 };
                                    const auto result = test_default_allocator(raw, element_count);
                                    // Allocation of 4 i32 should be done
                                    hud_assert_true(std::get<0>(result));
                                    hud_assert_eq(std::get<1>(result), element_count);
                                    hud_assert_eq(std::get<2>(result), element_count + extra);

                                    // Ensure it's not the same memory buffer
                                    hud_assert_true(std::get<3>(result));

                                    // Ensure values are correclty copied
                                    hud_assert_true(std::get<4>(result));
                                }

                                // Constant
                                {
                                    constexpr usize element_count = 4;
                                    constexpr U raw[element_count] = { 1,2,3,4 };
                                    constexpr auto result = test_default_allocator(raw, element_count);
                                    // Allocation of 4 i32 should be done
                                    hud_assert_true(std::get<0>(result));
                                    hud_assert_eq(std::get<1>(result), element_count);
                                    hud_assert_eq(std::get<2>(result), element_count + extra);

                                    // Ensure it's not the same memory buffer
                                    hud_assert_true(std::get<3>(result));

                                    // Ensure values are correclty copied
                                    hud_assert_true(std::get<4>(result));
                                }

                            }

                            // Test with allocator with extra
                            {
                                auto test_with_allocator = [](const U* raw, usize element_count) {
                                    hud::array<type_t, hud_test::array_allocator<alignof(type_t)>> array(raw, element_count, extra, hud_test::array_allocator<alignof(type_t)>{});
                                    bool all_values_copied = true;

                                    // Ensure values are correclty copied
                                    for (usize index = 0; index < element_count; index++) {
                                        if (*(array.data() + index) != static_cast<type_t>(raw[index])) {
                                            all_values_copied = false;
                                            break;
                                        }
                                    }

                                    return std::tuple{
                                        // Allocation of 4 i32 should be done
                                        array.data() != nullptr,
                                        array.count(),
                                        array.max_count(),

                                        // Ensure it's not the same memory buffer
                                        (void*)array.data() != (void*)&raw[0],

                                        // Ensure values are correclty copied
                                        all_values_copied,

                                        // Ensure we are allocating only one time
                                        array.allocator().allocation_count(),
                                        array.allocator().free_count()
                                    };
                                };

                                // Non Constant
                                {
                                    constexpr usize element_count = 4;
                                    constexpr U raw[element_count] = { 1,2,3,4 };
                                    const auto result = test_with_allocator(raw, element_count);
                                    // Allocation of 4 i32 should be done
                                    hud_assert_true(std::get<0>(result));
                                    hud_assert_eq(std::get<1>(result), element_count);
                                    hud_assert_eq(std::get<2>(result), element_count + extra);

                                    // Ensure it's not the same memory buffer
                                    hud_assert_true(std::get<3>(result));

                                    // Ensure values are correclty copied
                                    hud_assert_true(std::get<4>(result));

                                    // Ensure we are allocating only one time
                                    hud_assert_eq(std::get<5>(result), 1u);
                                    hud_assert_eq(std::get<6>(result), 0u);
                                }

                                // Constant
                                {
                                    constexpr usize element_count = 4;
                                    constexpr U raw[element_count] = { 1,2,3,4 };
                                    constexpr auto result = test_with_allocator(raw, element_count);

                                    // Allocation of 4 i32 should be done
                                    hud_assert_true(std::get<0>(result));
                                    hud_assert_eq(std::get<1>(result), element_count);
                                    hud_assert_eq(std::get<2>(result), element_count + extra);

                                    // Ensure it's not the same memory buffer
                                    hud_assert_true(std::get<3>(result));

                                    // Ensure values are correclty copied
                                    hud_assert_true(std::get<4>(result));

                                    // Ensure we are allocating only one time
                                    hud_assert_eq(std::get<5>(result), 1u);
                                    hud_assert_eq(std::get<6>(result), 0u);
                                }
                            }
                    });
                }
            } }); });

    hud_assert_eq(at_least_one_copy_tested, true);
}

GTEST_TEST(array, construct_by_copying_raw_data_array_of_non_bitwise_copy_constructible_different_type)
{

    using Type1 = hud_test::NonBitwiseCopyConstructibleType2;
    using Type2 = hud_test::NonBitwiseCopyConstructibleType;

    // test with default allocator no extra
    {
        auto test_default_allocator = [](const Type1 *raw, usize element_count)
        {
            hud::array<Type2> array(raw, element_count);

            // Ensure values are correclty copied in order
            bool all_values_copied_in_order = true;
            for (usize index = 0; index < element_count; index++)
            {
                if (array[index].id() != raw[index].id())
                {
                    all_values_copied_in_order = false;
                    break;
                }
            }

            // Ensure the copy constructor are called
            bool all_copy_constructor_are_called = true;
            for (usize index = 0; index < element_count; index++)
            {
                // Ensure we call the copy constructor and the id is correctly copied
                if (array[index].copy_constructor_count() != 1u || array[index].id() != raw[index].id())
                {
                    all_copy_constructor_are_called = false;
                    break;
                }
            }

            return std::tuple {
                // Allocation of 4 i32 should be done
                array.data() != nullptr,
                array.count(),
                array.max_count(),

                // Ensure it's not the same memory buffer
                (void *)array.data() != (void *)&raw[0],

                // Ensure values are correclty copied in order
                all_values_copied_in_order,

                // Ensure the copy constructor are called
                all_copy_constructor_are_called

            };
        };

        // Non Constant
        {
            constexpr usize element_count = 4;
            constexpr Type1 raw[element_count] = {1, 2, 3, 4};
            const auto result = test_default_allocator(raw, element_count);
            // Allocation of 4 i32 should be done
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), element_count);
            hud_assert_eq(std::get<2>(result), element_count);

            // Ensure it's not the same memory buffer
            hud_assert_true(std::get<3>(result));

            // Ensure values are correclty copied in order
            hud_assert_true(std::get<4>(result));

            // Ensure the copy constructor are called
            hud_assert_true(std::get<5>(result));
        }

        // Constant
        {
            constexpr usize element_count = 4;
            constexpr Type1 raw[element_count] = {1, 2, 3, 4};
            constexpr auto result = test_default_allocator(raw, element_count);
            // Allocation of 4 i32 should be done
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), element_count);
            hud_assert_eq(std::get<2>(result), element_count);

            // Ensure it's not the same memory buffer
            hud_assert_true(std::get<3>(result));

            // Ensure values are correclty copied in order
            hud_assert_true(std::get<4>(result));

            // Ensure the copy constructor are called
            hud_assert_true(std::get<5>(result));
        }
    }

    // test with allocator no extra
    {
        auto test_with_allocator = [](const Type1 *raw, usize element_count)
        {
            hud::array<Type2, hud_test::array_allocator<alignof(Type2)>> array(raw, element_count, hud_test::array_allocator<alignof(Type2)> {});

            // Ensure values are correclty copied in order
            bool all_values_copied_in_order = true;
            for (usize index = 0; index < element_count; index++)
            {
                if (array[index].id() != raw[index].id())
                {
                    all_values_copied_in_order = false;
                    break;
                }
            }

            // Ensure the copy constructor are called
            bool all_copy_constructor_are_called = true;
            for (usize index = 0; index < element_count; index++)
            {
                // Ensure we call the copy constructor and the id is correctly copied
                if (array[index].copy_constructor_count() != 1u || array[index].id() != raw[index].id())
                {
                    all_copy_constructor_are_called = false;
                    break;
                }
            }
            return std::tuple {
                // Allocation of 4 i32 should be done
                array.data() != nullptr,
                array.count(),
                array.max_count(),

                // Ensure it's not the same memory buffer
                array.data() != &raw[0],

                // Ensure values are correclty copied in order
                all_values_copied_in_order,

                // Ensure the copy constructor are called
                all_copy_constructor_are_called,

                // Ensure we are allocating only one time
                array.allocator().allocation_count(),
                array.allocator().free_count()

            };
        };

        // Non Constant
        {
            constexpr usize element_count = 4;
            constexpr Type1 raw[element_count] = {1, 2, 3, 4};
            const auto result = test_with_allocator(raw, element_count);
            // Allocation of 4 i32 should be done
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), element_count);
            hud_assert_eq(std::get<2>(result), element_count);

            // Ensure it's not the same memory buffer
            hud_assert_true(std::get<3>(result));

            // Ensure values are correclty copied in order
            hud_assert_true(std::get<4>(result));

            // Ensure the copy constructor are called
            hud_assert_true(std::get<5>(result));

            // Ensure we are allocating only one time
            hud_assert_eq(std::get<6>(result), 1u);
            hud_assert_eq(std::get<7>(result), 0u);
        }

        // Constant
        {
            constexpr usize element_count = 4;
            constexpr Type1 raw[element_count] = {1, 2, 3, 4};
            constexpr auto result = test_with_allocator(raw, element_count);
            // Allocation of 4 i32 should be done
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), element_count);
            hud_assert_eq(std::get<2>(result), element_count);

            // Ensure it's not the same memory buffer
            hud_assert_true(std::get<3>(result));

            // Ensure values are correclty copied in order
            hud_assert_true(std::get<4>(result));

            // Ensure the copy constructor are called
            hud_assert_true(std::get<5>(result));

            // Ensure we are allocating only one time
            hud_assert_eq(std::get<6>(result), 1u);
            hud_assert_eq(std::get<7>(result), 0u);
        }
    }

    {
        hud_test::for_each_value<std::make_integer_sequence<usize, 5>>()(
            []<usize extra>()
            {
                // Test default allcoator with extra
                {
                    auto test_default_allocator = [](const Type1 *raw, usize element_count)
                    {
                        hud::array<Type2> array(raw, element_count, extra);

                        // Ensure values are correclty copied in order
                        bool all_values_copied_in_order = true;
                        for (usize index = 0; index < element_count; index++)
                        {
                            if (array[index].id() != raw[index].id())
                            {
                                all_values_copied_in_order = false;
                                break;
                            }
                        }

                        // Ensure the copy constructor are called
                        bool all_copy_constructor_are_called = true;
                        for (usize index = 0; index < element_count; index++)
                        {
                            // Ensure we call the copy constructor and the id is correctly copied
                            if (array[index].copy_constructor_count() != 1u || array[index].id() != raw[index].id())
                            {
                                all_copy_constructor_are_called = false;
                                break;
                            }
                        }

                        return std::tuple {
                            // Allocation of 4 i32 should be done
                            array.data() != nullptr,
                            array.count(),
                            array.max_count(),

                            // Ensure it's not the same memory buffer
                            array.data() != &raw[0],

                            // Ensure values are correclty copied in order
                            all_values_copied_in_order,

                            // Ensure the copy constructor are called
                            all_copy_constructor_are_called

                        };
                    };

                    // Non Constant
                    {
                        constexpr usize element_count = 4;
                        constexpr Type1 raw[element_count] = {1, 2, 3, 4};
                        const auto result = test_default_allocator(raw, element_count);
                        // Allocation of 4 i32 should be done
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), element_count);
                        hud_assert_eq(std::get<2>(result), element_count + extra);

                        // Ensure it's not the same memory buffer
                        hud_assert_true(std::get<3>(result));

                        // Ensure values are correclty copied in order
                        hud_assert_true(std::get<4>(result));

                        // Ensure the copy constructor are called
                        hud_assert_true(std::get<5>(result));
                    }

                    // Constant
                    {
                        constexpr usize element_count = 4;
                        constexpr Type1 raw[element_count] = {1, 2, 3, 4};
                        constexpr auto result = test_default_allocator(raw, element_count);
                        // Allocation of 4 i32 should be done
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), element_count);
                        hud_assert_eq(std::get<2>(result), element_count + extra);

                        // Ensure it's not the same memory buffer
                        hud_assert_true(std::get<3>(result));

                        // Ensure values are correclty copied in order
                        hud_assert_true(std::get<4>(result));

                        // Ensure the copy constructor are called
                        hud_assert_true(std::get<5>(result));
                    }
                }

                // Test with allocator with extra
                {
                    auto test_with_allocator = [](const Type1 *raw, usize element_count)
                    {
                        hud::array<Type2, hud_test::array_allocator<alignof(Type2)>> array(raw, element_count, extra, hud_test::array_allocator<alignof(Type2)> {});

                        // Ensure values are correclty copied in order
                        bool all_values_copied_in_order = true;
                        for (usize index = 0; index < element_count; index++)
                        {
                            if (array[index].id() != raw[index].id())
                            {
                                all_values_copied_in_order = false;
                                break;
                            }
                        }

                        // Ensure the copy constructor are called
                        bool all_copy_constructor_are_called = true;
                        for (usize index = 0; index < element_count; index++)
                        {
                            // Ensure we call the copy constructor and the id is correctly copied
                            if (array[index].copy_constructor_count() != 1u || array[index].id() != raw[index].id())
                            {
                                all_copy_constructor_are_called = false;
                                break;
                            }
                        }
                        return std::tuple {
                            // Allocation of 4 i32 should be done
                            array.data() != nullptr,
                            array.count(),
                            array.max_count(),

                            // Ensure it's not the same memory buffer
                            array.data() != &raw[0],

                            // Ensure values are correclty copied in order
                            all_values_copied_in_order,

                            // Ensure the copy constructor are called
                            all_copy_constructor_are_called,

                            // Ensure we are allocating only one time
                            array.allocator().allocation_count(),
                            array.allocator().free_count()

                        };
                    };

                    // Non Constant
                    {
                        constexpr usize element_count = 4;
                        constexpr Type1 raw[element_count] = {1, 2, 3, 4};
                        const auto result = test_with_allocator(raw, element_count);
                        // Allocation of 4 i32 should be done
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), element_count);
                        hud_assert_eq(std::get<2>(result), element_count + extra);

                        // Ensure it's not the same memory buffer
                        hud_assert_true(std::get<3>(result));

                        // Ensure values are correclty copied in order
                        hud_assert_true(std::get<4>(result));

                        // Ensure the copy constructor are called
                        hud_assert_true(std::get<5>(result));

                        // Ensure we are allocating only one time
                        hud_assert_eq(std::get<6>(result), 1u);
                        hud_assert_eq(std::get<7>(result), 0u);
                    }

                    // Constant
                    {
                        constexpr usize element_count = 4;
                        constexpr Type1 raw[element_count] = {1, 2, 3, 4};
                        constexpr auto result = test_with_allocator(raw, element_count);
                        // Allocation of 4 i32 should be done
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), element_count);
                        hud_assert_eq(std::get<2>(result), element_count + extra);

                        // Ensure it's not the same memory buffer
                        hud_assert_true(std::get<3>(result));

                        // Ensure values are correclty copied in order
                        hud_assert_true(std::get<4>(result));

                        // Ensure the copy constructor are called
                        hud_assert_true(std::get<5>(result));

                        // Ensure we are allocating only one time
                        hud_assert_eq(std::get<6>(result), 1u);
                        hud_assert_eq(std::get<7>(result), 0u);
                    }
                }
            }
        );
    }
}

GTEST_TEST(array, construct_with_initializer_list_of_bitwise_copy_constructible_same_type)
{

    using type = i32;

    static_assert(hud::is_bitwise_copy_constructible_v<type>);

    {
        auto test_default_allocator = [](std::initializer_list<type> initializer)
        {
            hud::array<type> array(initializer);
            bool all_values_copied = true;

            // Ensure values are correclty copied
            for (usize index = 0; index < initializer.size(); index++)
            {
                if (*(array.data() + index) != *(initializer.begin() + index))
                {
                    all_values_copied = false;
                    break;
                }
            }

            return std::tuple {
                // Allocation of 4 i32 should be done
                array.data() != nullptr,
                array.count(),
                array.max_count(),

                // Ensure it's not the same memory buffer
                (void *)array.data() != (void *)initializer.begin(),

                // Ensure values are correclty copied
                all_values_copied};
        };

        // Non Constant
        {
            const auto result = test_default_allocator({1, 2, 3, 4});
            // Allocation of 4 i32 should be done
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), 4u);
            hud_assert_eq(std::get<2>(result), 4u);

            // Ensure it's not the same memory buffer
            hud_assert_true(std::get<3>(result));

            // Ensure values are correclty copied
            hud_assert_true(std::get<4>(result));
        }

        // Constant
        {
            constexpr auto result = test_default_allocator({1, 2, 3, 4});
            // Allocation of 4 i32 should be done
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), 4u);
            hud_assert_eq(std::get<2>(result), 4u);

            // Ensure it's not the same memory buffer
            hud_assert_true(std::get<3>(result));

            // Ensure values are correclty copied
            hud_assert_true(std::get<4>(result));
        }
    }

    // test with allocator no extra
    {
        auto test_with_allocator = [](std::initializer_list<type> initializer)
        {
            hud::array<type, hud_test::array_allocator<alignof(type)>> array(initializer, hud_test::array_allocator<alignof(type)> {});
            bool all_values_copied = true;

            // Ensure values are correclty copied
            for (usize index = 0; index < initializer.size(); index++)
            {
                if (*(array.data() + index) != *(initializer.begin() + index))
                {
                    all_values_copied = false;
                    break;
                }
            }

            return std::tuple {
                // Allocation of 4 i32 should be done
                array.data() != nullptr,
                array.count(),
                array.max_count(),

                // Ensure it's not the same memory buffer
                array.data() != initializer.begin(),

                // Ensure values are correclty copied
                all_values_copied,

                // Ensure we are allocating only one time
                array.allocator().allocation_count(),
                array.allocator().free_count()};
        };

        // Non Constant
        {
            const auto result = test_with_allocator({1, 2, 3, 4});
            // Allocation of 4 i32 should be done
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), 4u);
            hud_assert_eq(std::get<2>(result), 4u);

            // Ensure it's not the same memory buffer
            hud_assert_true(std::get<3>(result));

            // Ensure values are correclty copied
            hud_assert_true(std::get<4>(result));

            // Ensure we are allocating only one time
            hud_assert_eq(std::get<5>(result), 1u);
            hud_assert_eq(std::get<6>(result), 0u);
        }

        // Constant
        {
            constexpr auto result = test_with_allocator({1, 2, 3, 4});
            // Allocation of 4 i32 should be done
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), 4u);
            hud_assert_eq(std::get<2>(result), 4u);

            // Ensure it's not the same memory buffer
            hud_assert_true(std::get<3>(result));

            // Ensure values are correclty copied
            hud_assert_true(std::get<4>(result));

            // Ensure we are allocating only one time
            hud_assert_eq(std::get<5>(result), 1u);
            hud_assert_eq(std::get<6>(result), 0u);
        }
    }

    {
        hud_test::for_each_value<std::make_integer_sequence<usize, 5>>()(
            []<usize extra>()
            {
                // Test default allocator with extra
                {
                    auto test_default_allocator = [](std::initializer_list<type> initializer)
                    {
                        hud::array<type> array(initializer, extra);
                        bool all_values_copied = true;

                        // Ensure values are correclty copied
                        for (usize index = 0; index < initializer.size(); index++)
                        {
                            if (*(array.data() + index) != *(initializer.begin() + index))
                            {
                                all_values_copied = false;
                                break;
                            }
                        }

                        return std::tuple {
                            // Allocation of 4 i32 should be done
                            array.data() != nullptr,
                            array.count(),
                            array.max_count(),

                            // Ensure it's not the same memory buffer
                            array.data() != initializer.begin(),

                            // Ensure values are correclty copied
                            all_values_copied};
                    };
                    // Non Constant
                    {
                        const auto result = test_default_allocator({1, 2, 3, 4});
                        // Allocation of 4 i32 should be done
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), 4u);
                        hud_assert_eq(std::get<2>(result), 4u + extra);

                        // Ensure it's not the same memory buffer
                        hud_assert_true(std::get<3>(result));

                        // Ensure values are correclty copied
                        hud_assert_true(std::get<4>(result));
                    }

                    // Constant
                    {
                        constexpr auto result = test_default_allocator({1, 2, 3, 4});
                        // Allocation of 4 i32 should be done
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), 4u);
                        hud_assert_eq(std::get<2>(result), 4u + extra);

                        // Ensure it's not the same memory buffer
                        hud_assert_true(std::get<3>(result));

                        // Ensure values are correclty copied
                        hud_assert_true(std::get<4>(result));
                    }
                }

                // Test with allocator with extra
                {
                    auto test_with_allocator = [](std::initializer_list<i32> initializer)
                    {
                        hud::array<type, hud_test::array_allocator<alignof(type)>> array(initializer, extra, hud_test::array_allocator<alignof(type)> {});
                        bool all_values_copied = true;

                        // Ensure values are correclty copied
                        for (usize index = 0; index < initializer.size(); index++)
                        {
                            if (*(array.data() + index) != *(initializer.begin() + index))
                            {
                                all_values_copied = false;
                                break;
                            }
                        }

                        return std::tuple {
                            // Allocation of 4 i32 should be done
                            array.data() != nullptr,
                            array.count(),
                            array.max_count(),

                            // Ensure it's not the same memory buffer
                            array.data() != initializer.begin(),

                            // Ensure values are correclty copied
                            all_values_copied,

                            // Ensure we are allocating only one time
                            array.allocator().allocation_count(),
                            array.allocator().free_count()};
                    };

                    // Non Constant
                    {
                        const auto result = test_with_allocator({1, 2, 3, 4});
                        // Allocation of 4 i32 should be done
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), 4u);
                        hud_assert_eq(std::get<2>(result), 4u + extra);

                        // Ensure it's not the same memory buffer
                        hud_assert_true(std::get<3>(result));

                        // Ensure values are correclty copied
                        hud_assert_true(std::get<4>(result));

                        // Ensure we are allocating only one time
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                    }

                    // Constant
                    {
                        constexpr auto result = test_with_allocator({1, 2, 3, 4});

                        // Allocation of 4 i32 should be done
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), 4u);
                        hud_assert_eq(std::get<2>(result), 4u + extra);

                        // Ensure it's not the same memory buffer
                        hud_assert_true(std::get<3>(result));

                        // Ensure values are correclty copied
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

GTEST_TEST(array, construct_with_initializer_list_of_bitwise_copy_constructible_different_type)
{

    using Type1 = i32;
    using Type2 = u32;

    static_assert(!std::is_same_v<Type1, Type2> && hud::is_bitwise_copy_constructible_v<Type2, Type1>);

    // test with default allocator no extra
    {
        auto test_default_allocator = [](std::initializer_list<Type1> initializer)
        {
            hud::array<Type2> array(initializer);
            bool all_values_copied = true;

            // Ensure values are correclty copied
            for (usize index = 0; index < initializer.size(); index++)
            {
                if (*(array.data() + index) != static_cast<Type2>(*(initializer.begin() + index)))
                {
                    all_values_copied = false;
                    break;
                }
            }

            return std::tuple {
                // Allocation of 4 i32 should be done
                array.data() != nullptr,
                array.count(),
                array.max_count(),

                // Ensure it's not the same memory buffer
                (void *)array.data() != (void *)initializer.begin(),

                // Ensure values are correclty copied
                all_values_copied};
        };

        // Non Constant
        {
            const auto result = test_default_allocator({1, 2, 3, 4});
            // Allocation of 4 i32 should be done
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), 4u);
            hud_assert_eq(std::get<2>(result), 4u);

            // Ensure it's not the same memory buffer
            hud_assert_true(std::get<3>(result));

            // Ensure values are correclty copied
            hud_assert_true(std::get<4>(result));
        }

        // Constant
        {
            constexpr auto result = test_default_allocator({1, 2, 3, 4});
            // Allocation of 4 i32 should be done
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), 4u);
            hud_assert_eq(std::get<2>(result), 4u);

            // Ensure it's not the same memory buffer
            hud_assert_true(std::get<3>(result));

            // Ensure values are correclty copied
            hud_assert_true(std::get<4>(result));
        }
    }

    // test with allocator no extra
    {
        auto test_with_allocator = [](std::initializer_list<Type1> initializer)
        {
            hud::array<Type2, hud_test::array_allocator<alignof(Type2)>> array(initializer, hud_test::array_allocator<alignof(Type2)> {});
            bool all_values_copied = true;

            // Ensure values are correclty copied
            for (usize index = 0; index < initializer.size(); index++)
            {
                if (*(array.data() + index) != static_cast<Type2>(*(initializer.begin() + index)))
                {
                    all_values_copied = false;
                    break;
                }
            }

            return std::tuple {
                // Allocation of 4 i32 should be done
                array.data() != nullptr,
                array.count(),
                array.max_count(),

                // Ensure it's not the same memory buffer
                (void *)array.data() != (void *)initializer.begin(),

                // Ensure values are correclty copied
                all_values_copied,

                // Ensure we are allocating only one time
                array.allocator().allocation_count(),
                array.allocator().free_count()};
        };

        // Non Constant
        {
            const auto result = test_with_allocator({1, 2, 3, 4});
            // Allocation of 4 i32 should be done
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), 4u);
            hud_assert_eq(std::get<2>(result), 4u);

            // Ensure it's not the same memory buffer
            hud_assert_true(std::get<3>(result));

            // Ensure values are correclty copied
            hud_assert_true(std::get<4>(result));

            // Ensure we are allocating only one time
            hud_assert_eq(std::get<5>(result), 1u);
            hud_assert_eq(std::get<6>(result), 0u);
        }

        // Constant
        {
            constexpr auto result = test_with_allocator({1, 2, 3, 4});
            // Allocation of 4 i32 should be done
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), 4u);
            hud_assert_eq(std::get<2>(result), 4u);

            // Ensure it's not the same memory buffer
            hud_assert_true(std::get<3>(result));

            // Ensure values are correclty copied
            hud_assert_true(std::get<4>(result));

            // Ensure we are allocating only one time
            hud_assert_eq(std::get<5>(result), 1u);
            hud_assert_eq(std::get<6>(result), 0u);
        }
    }

    {
        hud_test::for_each_value<std::make_integer_sequence<usize, 5>>()(
            []<usize extra>()
            {
                // Test default allocator with extra
                {
                    auto test_default_allocator = [](std::initializer_list<Type1> initializer)
                    {
                        hud::array<Type2> array(initializer, extra);
                        bool all_values_copied = true;

                        // Ensure values are correclty copied
                        for (usize index = 0; index < initializer.size(); index++)
                        {
                            if (*(array.data() + index) != static_cast<Type2>(*(initializer.begin() + index)))
                            {
                                all_values_copied = false;
                                break;
                            }
                        }

                        return std::tuple {
                            // Allocation of 4 i32 should be done
                            array.data() != nullptr,
                            array.count(),
                            array.max_count(),

                            // Ensure it's not the same memory buffer
                            (void *)array.data() != (void *)initializer.begin(),

                            // Ensure values are correclty copied
                            all_values_copied};
                    };
                    // Non Constant
                    {
                        const auto result = test_default_allocator({1, 2, 3, 4});
                        // Allocation of 4 i32 should be done
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), 4u);
                        hud_assert_eq(std::get<2>(result), 4u + extra);

                        // Ensure it's not the same memory buffer
                        hud_assert_true(std::get<3>(result));

                        // Ensure values are correclty copied
                        hud_assert_true(std::get<4>(result));
                    }

                    // Constant
                    {
                        constexpr auto result = test_default_allocator({1, 2, 3, 4});
                        // Allocation of 4 i32 should be done
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), 4u);
                        hud_assert_eq(std::get<2>(result), 4u + extra);

                        // Ensure it's not the same memory buffer
                        hud_assert_true(std::get<3>(result));

                        // Ensure values are correclty copied
                        hud_assert_true(std::get<4>(result));
                    }
                }

                // Test with allocator with extra
                {
                    auto test_with_allocator = [](std::initializer_list<Type1> initializer)
                    {
                        hud::array<Type2, hud_test::array_allocator<alignof(Type2)>> array(initializer, extra, hud_test::array_allocator<alignof(Type2)> {});
                        bool all_values_copied = true;

                        // Ensure values are correclty copied
                        for (usize index = 0; index < initializer.size(); index++)
                        {
                            if (*(array.data() + index) != static_cast<Type2>(*(initializer.begin() + index)))
                            {
                                all_values_copied = false;
                                break;
                            }
                        }

                        return std::tuple {
                            // Allocation of 4 i32 should be done
                            array.data() != nullptr,
                            array.count(),
                            array.max_count(),

                            // Ensure it's not the same memory buffer
                            (void *)array.data() != (void *)initializer.begin(),

                            // Ensure values are correclty copied
                            all_values_copied,

                            // Ensure we are allocating only one time
                            array.allocator().allocation_count(),
                            array.allocator().free_count()};
                    };

                    // Non Constant
                    {
                        const auto result = test_with_allocator({1, 2, 3, 4});
                        // Allocation of 4 i32 should be done
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), 4u);
                        hud_assert_eq(std::get<2>(result), 4u + extra);

                        // Ensure it's not the same memory buffer
                        hud_assert_true(std::get<3>(result));

                        // Ensure values are correclty copied
                        hud_assert_true(std::get<4>(result));

                        // Ensure we are allocating only one time
                        hud_assert_eq(std::get<5>(result), 1u);
                        hud_assert_eq(std::get<6>(result), 0u);
                    }

                    // Constant
                    {
                        constexpr auto result = test_with_allocator({1, 2, 3, 4});

                        // Allocation of 4 i32 should be done
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), 4u);
                        hud_assert_eq(std::get<2>(result), 4u + extra);

                        // Ensure it's not the same memory buffer
                        hud_assert_true(std::get<3>(result));

                        // Ensure values are correclty copied
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

GTEST_TEST(array, construct_with_initializer_list_of_non_bitwise_copy_constructible_same_type)
{

    using type = hud_test::NonBitwiseCopyConstructibleType;
    static_assert(!hud::is_bitwise_copy_constructible_v<type>);

    // test with default allocator no extra
    {
        auto test_default_allocator = [](std::initializer_list<type> initializer)
        {
            hud::array<type> array(initializer);

            // Ensure values are correclty copied in order
            bool all_values_copied_in_order = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                if (array[index].id() != initializer.begin()[index].id())
                {
                    all_values_copied_in_order = false;
                    break;
                }
            }

            // Ensure the copy constructor are called
            bool all_copy_constructor_are_called = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                // Ensure we call the copy constructor and the id is correctly copied
                if (array[index].copy_constructor_count() != 1u || array[index].id() != initializer.begin()[index].id())
                {
                    all_copy_constructor_are_called = false;
                    break;
                }
            }

            return std::tuple {
                // Allocation of 4 i32 should be done
                array.data() != nullptr,
                array.count(),
                array.max_count(),

                // Ensure it's not the same memory buffer
                (void *)array.data() != (void *)initializer.begin(),

                // Ensure values are correclty copied in order
                all_values_copied_in_order,

                // Ensure the copy constructor are called
                all_copy_constructor_are_called

            };
        };

        // Non Constant
        {
            const auto result = test_default_allocator({1, 2, 3, 4});
            // Allocation of 4 i32 should be done
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), 4u);
            hud_assert_eq(std::get<2>(result), 4u);

            // Ensure it's not the same memory buffer
            hud_assert_true(std::get<3>(result));

            // Ensure values are correclty copied in order
            hud_assert_true(std::get<4>(result));

            // Ensure the copy constructor are called
            hud_assert_true(std::get<5>(result));
        }

        // Constant
        {
            constexpr auto result = test_default_allocator({1, 2, 3, 4});
            // Allocation of 4 i32 should be done
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), 4u);
            hud_assert_eq(std::get<2>(result), 4u);

            // Ensure it's not the same memory buffer
            hud_assert_true(std::get<3>(result));

            // Ensure values are correclty copied in order
            hud_assert_true(std::get<4>(result));

            // Ensure the copy constructor are called
            hud_assert_true(std::get<5>(result));
        }
    }

    // test with allocator no extra
    {
        auto test_with_allocator = [](std::initializer_list<type> initializer)
        {
            hud::array<type, hud_test::array_allocator<alignof(type)>> array(initializer, hud_test::array_allocator<alignof(type)> {});

            // Ensure values are correclty copied in order
            bool all_values_copied_in_order = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                if (array[index].id() != initializer.begin()[index].id())
                {
                    all_values_copied_in_order = false;
                    break;
                }
            }

            // Ensure the copy constructor are called
            bool all_copy_constructor_are_called = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                // Ensure we call the copy constructor and the id is correctly copied
                if (array[index].copy_constructor_count() != 1u || array[index].id() != initializer.begin()[index].id())
                {
                    all_copy_constructor_are_called = false;
                    break;
                }
            }
            return std::tuple {
                // Allocation of 4 i32 should be done
                array.data() != nullptr,
                array.count(),
                array.max_count(),

                // Ensure it's not the same memory buffer
                (void *)array.data() != (void *)initializer.begin(),

                // Ensure values are correclty copied in order
                all_values_copied_in_order,

                // Ensure the copy constructor are called
                all_copy_constructor_are_called,

                // Ensure we are allocating only one time
                array.allocator().allocation_count(),
                array.allocator().free_count()

            };
        };

        // Non Constant
        {
            const auto result = test_with_allocator({1, 2, 3, 4});
            // Allocation of 4 i32 should be done
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), 4u);
            hud_assert_eq(std::get<2>(result), 4u);

            // Ensure it's not the same memory buffer
            hud_assert_true(std::get<3>(result));

            // Ensure values are correclty copied in order
            hud_assert_true(std::get<4>(result));

            // Ensure the copy constructor are called
            hud_assert_true(std::get<5>(result));

            // Ensure we are allocating only one time
            hud_assert_eq(std::get<6>(result), 1u);
            hud_assert_eq(std::get<7>(result), 0u);
        }

        // Constant
        {
            constexpr auto result = test_with_allocator({1, 2, 3, 4});
            // Allocation of 4 i32 should be done
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), 4u);
            hud_assert_eq(std::get<2>(result), 4u);

            // Ensure it's not the same memory buffer
            hud_assert_true(std::get<3>(result));

            // Ensure values are correclty copied in order
            hud_assert_true(std::get<4>(result));

            // Ensure the copy constructor are called
            hud_assert_true(std::get<5>(result));

            // Ensure we are allocating only one time
            hud_assert_eq(std::get<6>(result), 1u);
            hud_assert_eq(std::get<7>(result), 0u);
        }
    }

    {
        hud_test::for_each_value<std::make_integer_sequence<usize, 5>>()(
            []<usize extra>()
            {
                // Test default allcoator with extra
                {
                    auto test_default_allocator = [](std::initializer_list<type> initializer)
                    {
                        hud::array<type> array(initializer, extra);

                        // Ensure values are correclty copied in order
                        bool all_values_copied_in_order = true;
                        for (usize index = 0; index < initializer.size(); index++)
                        {
                            if (array[index].id() != initializer.begin()[index].id())
                            {
                                all_values_copied_in_order = false;
                                break;
                            }
                        }

                        // Ensure the copy constructor are called
                        bool all_copy_constructor_are_called = true;
                        for (usize index = 0; index < initializer.size(); index++)
                        {
                            // Ensure we call the copy constructor and the id is correctly copied
                            if (array[index].copy_constructor_count() != 1u || array[index].id() != initializer.begin()[index].id())
                            {
                                all_copy_constructor_are_called = false;
                                break;
                            }
                        }

                        return std::tuple {
                            // Allocation of 4 i32 should be done
                            array.data() != nullptr,
                            array.count(),
                            array.max_count(),

                            // Ensure it's not the same memory buffer
                            (void *)array.data() != (void *)initializer.begin(),

                            // Ensure values are correclty copied in order
                            all_values_copied_in_order,

                            // Ensure the copy constructor are called
                            all_copy_constructor_are_called

                        };
                    };

                    // Non Constant
                    {
                        const auto result = test_default_allocator({1, 2, 3, 4});
                        // Allocation of 4 i32 should be done
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), 4u);
                        hud_assert_eq(std::get<2>(result), 4u + extra);

                        // Ensure it's not the same memory buffer
                        hud_assert_true(std::get<3>(result));

                        // Ensure values are correclty copied in order
                        hud_assert_true(std::get<4>(result));

                        // Ensure the copy constructor are called
                        hud_assert_true(std::get<5>(result));
                    }

                    // Constant
                    {
                        constexpr auto result = test_default_allocator({1, 2, 3, 4});
                        // Allocation of 4 i32 should be done
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), 4u);
                        hud_assert_eq(std::get<2>(result), 4u + extra);

                        // Ensure it's not the same memory buffer
                        hud_assert_true(std::get<3>(result));

                        // Ensure values are correclty copied in order
                        hud_assert_true(std::get<4>(result));

                        // Ensure the copy constructor are called
                        hud_assert_true(std::get<5>(result));
                    }
                }

                // Test with allocator with extra
                {
                    auto test_with_allocator = [](std::initializer_list<type> initializer)
                    {
                        hud::array<type, hud_test::array_allocator<alignof(type)>> array(initializer, extra, hud_test::array_allocator<alignof(type)> {});

                        // Ensure values are correclty copied in order
                        bool all_values_copied_in_order = true;
                        for (usize index = 0; index < initializer.size(); index++)
                        {
                            if (array[index].id() != initializer.begin()[index].id())
                            {
                                all_values_copied_in_order = false;
                                break;
                            }
                        }

                        // Ensure the copy constructor are called
                        bool all_copy_constructor_are_called = true;
                        for (usize index = 0; index < initializer.size(); index++)
                        {
                            // Ensure we call the copy constructor and the id is correctly copied
                            if (array[index].copy_constructor_count() != 1u || array[index].id() != initializer.begin()[index].id())
                            {
                                all_copy_constructor_are_called = false;
                                break;
                            }
                        }
                        return std::tuple {
                            // Allocation of 4 i32 should be done
                            array.data() != nullptr,
                            array.count(),
                            array.max_count(),

                            // Ensure it's not the same memory buffer
                            (void *)array.data() != (void *)initializer.begin(),

                            // Ensure values are correclty copied in order
                            all_values_copied_in_order,

                            // Ensure the copy constructor are called
                            all_copy_constructor_are_called,

                            // Ensure we are allocating only one time
                            array.allocator().allocation_count(),
                            array.allocator().free_count()

                        };
                    };

                    // Non Constant
                    {
                        const auto result = test_with_allocator({1, 2, 3, 4});
                        // Allocation of 4 i32 should be done
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), 4u);
                        hud_assert_eq(std::get<2>(result), 4u + extra);

                        // Ensure it's not the same memory buffer
                        hud_assert_true(std::get<3>(result));

                        // Ensure values are correclty copied in order
                        hud_assert_true(std::get<4>(result));

                        // Ensure the copy constructor are called
                        hud_assert_true(std::get<5>(result));

                        // Ensure we are allocating only one time
                        hud_assert_eq(std::get<6>(result), 1u);
                        hud_assert_eq(std::get<7>(result), 0u);
                    }

                    // Constant
                    {
                        constexpr auto result = test_with_allocator({1, 2, 3, 4});
                        // Allocation of 4 i32 should be done
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), 4u);
                        hud_assert_eq(std::get<2>(result), 4u + extra);

                        // Ensure it's not the same memory buffer
                        hud_assert_true(std::get<3>(result));

                        // Ensure values are correclty copied in order
                        hud_assert_true(std::get<4>(result));

                        // Ensure the copy constructor are called
                        hud_assert_true(std::get<5>(result));

                        // Ensure we are allocating only one time
                        hud_assert_eq(std::get<6>(result), 1u);
                        hud_assert_eq(std::get<7>(result), 0u);
                    }
                }
            }
        );
    }
}

GTEST_TEST(array, construct_with_initializer_list_of_non_bitwise_copy_constructible_different_type)
{

    using Type1 = hud_test::NonBitwiseCopyConstructibleType;
    using Type2 = hud_test::NonBitwiseCopyConstructibleType2;
    static_assert(!std::is_same_v<Type1, Type2> && !hud::is_bitwise_copy_constructible_v<Type2, Type1>);

    // test with default allocator no extra
    {
        auto test_default_allocator = [](std::initializer_list<Type1> initializer)
        {
            hud::array<Type2> array(initializer);

            // Ensure values are correclty copied in order
            bool all_values_copied_in_order = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                if (array[index].id() != initializer.begin()[index].id())
                {
                    all_values_copied_in_order = false;
                    break;
                }
            }

            // Ensure the copy constructor are called
            bool all_copy_constructor_are_called = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                // Ensure we call the copy constructor and the id is correctly copied
                if (array[index].copy_constructor_count() != 1u || array[index].id() != initializer.begin()[index].id())
                {
                    all_copy_constructor_are_called = false;
                    break;
                }
            }

            return std::tuple {
                // Allocation of 4 i32 should be done
                array.data() != nullptr,
                array.count(),
                array.max_count(),

                // Ensure it's not the same memory buffer
                (void *)array.data() != (void *)initializer.begin(),

                // Ensure values are correclty copied in order
                all_values_copied_in_order,

                // Ensure the copy constructor are called
                all_copy_constructor_are_called

            };
        };

        // Non Constant
        {
            const auto result = test_default_allocator({1, 2, 3, 4});
            // Allocation of 4 i32 should be done
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), 4u);
            hud_assert_eq(std::get<2>(result), 4u);

            // Ensure it's not the same memory buffer
            hud_assert_true(std::get<3>(result));

            // Ensure values are correclty copied in order
            hud_assert_true(std::get<4>(result));

            // Ensure the copy constructor are called
            hud_assert_true(std::get<5>(result));
        }

        // Constant
        {
            constexpr auto result = test_default_allocator({1, 2, 3, 4});
            // Allocation of 4 i32 should be done
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), 4u);
            hud_assert_eq(std::get<2>(result), 4u);

            // Ensure it's not the same memory buffer
            hud_assert_true(std::get<3>(result));

            // Ensure values are correclty copied in order
            hud_assert_true(std::get<4>(result));

            // Ensure the copy constructor are called
            hud_assert_true(std::get<5>(result));
        }
    }

    // test with allocator no extra
    {
        auto test_with_allocator = [](std::initializer_list<Type1> initializer)
        {
            hud::array<Type2, hud_test::array_allocator<alignof(Type2)>> array(initializer, hud_test::array_allocator<alignof(Type2)> {});

            // Ensure values are correclty copied in order
            bool all_values_copied_in_order = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                if (array[index].id() != initializer.begin()[index].id())
                {
                    all_values_copied_in_order = false;
                    break;
                }
            }

            // Ensure the copy constructor are called
            bool all_copy_constructor_are_called = true;
            for (usize index = 0; index < initializer.size(); index++)
            {
                // Ensure we call the copy constructor and the id is correctly copied
                if (array[index].copy_constructor_count() != 1u || array[index].id() != initializer.begin()[index].id())
                {
                    all_copy_constructor_are_called = false;
                    break;
                }
            }
            return std::tuple {
                // Allocation of 4 i32 should be done
                array.data() != nullptr,
                array.count(),
                array.max_count(),

                // Ensure it's not the same memory buffer
                (void *)array.data() != (void *)initializer.begin(),

                // Ensure values are correclty copied in order
                all_values_copied_in_order,

                // Ensure the copy constructor are called
                all_copy_constructor_are_called,

                // Ensure we are allocating only one time
                array.allocator().allocation_count(),
                array.allocator().free_count()

            };
        };

        // Non Constant
        {
            const auto result = test_with_allocator({1, 2, 3, 4});
            // Allocation of 4 i32 should be done
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), 4u);
            hud_assert_eq(std::get<2>(result), 4u);

            // Ensure it's not the same memory buffer
            hud_assert_true(std::get<3>(result));

            // Ensure values are correclty copied in order
            hud_assert_true(std::get<4>(result));

            // Ensure the copy constructor are called
            hud_assert_true(std::get<5>(result));

            // Ensure we are allocating only one time
            hud_assert_eq(std::get<6>(result), 1u);
            hud_assert_eq(std::get<7>(result), 0u);
        }

        // Constant
        {
            constexpr auto result = test_with_allocator({1, 2, 3, 4});
            // Allocation of 4 i32 should be done
            hud_assert_true(std::get<0>(result));
            hud_assert_eq(std::get<1>(result), 4u);
            hud_assert_eq(std::get<2>(result), 4u);

            // Ensure it's not the same memory buffer
            hud_assert_true(std::get<3>(result));

            // Ensure values are correclty copied in order
            hud_assert_true(std::get<4>(result));

            // Ensure the copy constructor are called
            hud_assert_true(std::get<5>(result));

            // Ensure we are allocating only one time
            hud_assert_eq(std::get<6>(result), 1u);
            hud_assert_eq(std::get<7>(result), 0u);
        }
    }

    {
        hud_test::for_each_value<std::make_integer_sequence<usize, 5>>()(
            []<usize extra>()
            {
                // Test default allcoator with extra
                {
                    auto test_default_allocator = [](std::initializer_list<Type1> initializer)
                    {
                        hud::array<Type2> array(initializer, extra);

                        // Ensure values are correclty copied in order
                        bool all_values_copied_in_order = true;
                        for (usize index = 0; index < initializer.size(); index++)
                        {
                            if (array[index].id() != initializer.begin()[index].id())
                            {
                                all_values_copied_in_order = false;
                                break;
                            }
                        }

                        // Ensure the copy constructor are called
                        bool all_copy_constructor_are_called = true;
                        for (usize index = 0; index < initializer.size(); index++)
                        {
                            // Ensure we call the copy constructor and the id is correctly copied
                            if (array[index].copy_constructor_count() != 1u || array[index].id() != initializer.begin()[index].id())
                            {
                                all_copy_constructor_are_called = false;
                                break;
                            }
                        }

                        return std::tuple {
                            // Allocation of 4 i32 should be done
                            array.data() != nullptr,
                            array.count(),
                            array.max_count(),

                            // Ensure it's not the same memory buffer
                            (void *)array.data() != (void *)initializer.begin(),

                            // Ensure values are correclty copied in order
                            all_values_copied_in_order,

                            // Ensure the copy constructor are called
                            all_copy_constructor_are_called

                        };
                    };

                    // Non Constant
                    {
                        const auto result = test_default_allocator({1, 2, 3, 4});
                        // Allocation of 4 i32 should be done
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), 4u);
                        hud_assert_eq(std::get<2>(result), 4u + extra);

                        // Ensure it's not the same memory buffer
                        hud_assert_true(std::get<3>(result));

                        // Ensure values are correclty copied in order
                        hud_assert_true(std::get<4>(result));

                        // Ensure the copy constructor are called
                        hud_assert_true(std::get<5>(result));
                    }

                    // Constant
                    {
                        constexpr auto result = test_default_allocator({1, 2, 3, 4});
                        // Allocation of 4 i32 should be done
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), 4u);
                        hud_assert_eq(std::get<2>(result), 4u + extra);

                        // Ensure it's not the same memory buffer
                        hud_assert_true(std::get<3>(result));

                        // Ensure values are correclty copied in order
                        hud_assert_true(std::get<4>(result));

                        // Ensure the copy constructor are called
                        hud_assert_true(std::get<5>(result));
                    }
                }

                // Test with allocator with extra
                {
                    auto test_with_allocator = [](std::initializer_list<Type1> initializer)
                    {
                        hud::array<Type2, hud_test::array_allocator<alignof(Type2)>> array(initializer, extra, hud_test::array_allocator<alignof(Type2)> {});

                        // Ensure values are correclty copied in order
                        bool all_values_copied_in_order = true;
                        for (usize index = 0; index < initializer.size(); index++)
                        {
                            if (array[index].id() != initializer.begin()[index].id())
                            {
                                all_values_copied_in_order = false;
                                break;
                            }
                        }

                        // Ensure the copy constructor are called
                        bool all_copy_constructor_are_called = true;
                        for (usize index = 0; index < initializer.size(); index++)
                        {
                            // Ensure we call the copy constructor and the id is correctly copied
                            if (array[index].copy_constructor_count() != 1u || array[index].id() != initializer.begin()[index].id())
                            {
                                all_copy_constructor_are_called = false;
                                break;
                            }
                        }
                        return std::tuple {
                            // Allocation of 4 i32 should be done
                            array.data() != nullptr,
                            array.count(),
                            array.max_count(),

                            // Ensure it's not the same memory buffer
                            (void *)array.data() != (void *)initializer.begin(),

                            // Ensure values are correclty copied in order
                            all_values_copied_in_order,

                            // Ensure the copy constructor are called
                            all_copy_constructor_are_called,

                            // Ensure we are allocating only one time
                            array.allocator().allocation_count(),
                            array.allocator().free_count()

                        };
                    };

                    // Non Constant
                    {
                        const auto result = test_with_allocator({1, 2, 3, 4});
                        // Allocation of 4 i32 should be done
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), 4u);
                        hud_assert_eq(std::get<2>(result), 4u + extra);

                        // Ensure it's not the same memory buffer
                        hud_assert_true(std::get<3>(result));

                        // Ensure values are correclty copied in order
                        hud_assert_true(std::get<4>(result));

                        // Ensure the copy constructor are called
                        hud_assert_true(std::get<5>(result));

                        // Ensure we are allocating only one time
                        hud_assert_eq(std::get<6>(result), 1u);
                        hud_assert_eq(std::get<7>(result), 0u);
                    }

                    // Constant
                    {
                        constexpr auto result = test_with_allocator({1, 2, 3, 4});
                        // Allocation of 4 i32 should be done
                        hud_assert_true(std::get<0>(result));
                        hud_assert_eq(std::get<1>(result), 4u);
                        hud_assert_eq(std::get<2>(result), 4u + extra);

                        // Ensure it's not the same memory buffer
                        hud_assert_true(std::get<3>(result));

                        // Ensure values are correclty copied in order
                        hud_assert_true(std::get<4>(result));

                        // Ensure the copy constructor are called
                        hud_assert_true(std::get<5>(result));

                        // Ensure we are allocating only one time
                        hud_assert_eq(std::get<6>(result), 1u);
                        hud_assert_eq(std::get<7>(result), 0u);
                    }
                }
            }
        );
    }
}

GTEST_TEST(array, copy_construct_bitwise_copy_constructible_same_type)
{

    /** The array we copy for test, we allocate also extra memory to test if we really copy the count(), not the max_count() elements */
    using type = i32;

    static_assert(hud::is_bitwise_copy_constructible_v<type>);

    using CopiedType = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    hud_test::for_each_type<hud_test::array_allocator<alignof(type)>, hud_test::ArrayAllocator2<alignof(type)>>()([]<typename Allocator>() noexcept
                                                                                                                  {

        auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra) {
            const CopiedType copied(initializer, copied_extra);

            // Copy the array
            hud::array<type, Allocator> copy(copied);

            // Ensure we copy all datas in order
            bool all_values_copied = true;
            for (usize index = 0; index < initializer.size(); index++) {
                if (copy[index] != static_cast<type>(index)) {
                    all_values_copied = false;
                    break;
                }
            }

            return std::tuple{
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
            const auto result = test_default_allocator({ 0,1,2,3 }, 1u);
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
            constexpr auto result = test_default_allocator({ 0,1,2,3 }, 1u);
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


        auto test_with_allocator = [](std::initializer_list<i32> initializer, usize copied_extra) {
            const CopiedType copied(initializer, copied_extra);

            // Copy the array
            hud::array<type, Allocator> copy(copied, Allocator{});

            // Ensure we copy all datas in order
            bool all_values_copied = true;
            for (usize index = 0; index < initializer.size(); index++) {
                if (copy[index] != static_cast<type>(index)) {
                    all_values_copied = false;
                    break;
                }
            }

            return std::tuple{
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
            const auto result = test_with_allocator({ 0,1,2,3 }, 1u);
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
            constexpr auto result = test_with_allocator({ 0,1,2,3 }, 1u);
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


        // Test with extra
        {
            hud_test::for_each_value<std::make_integer_sequence<usize, 5>>()(
                []<usize extra>() {

                // Test with default allocator
                    {
                        auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra) {
                            const CopiedType copied(initializer, copied_extra);

                            // Copy the array
                            hud::array<type, Allocator> copy(copied, extra);

                            // Ensure we copy all datas in order
                            bool all_values_copied = true;
                            for (usize index = 0; index < initializer.size(); index++) {
                                if (copy[index] != static_cast<type>(index)) {
                                    all_values_copied = false;
                                    break;
                                }
                            }

                            return std::tuple{
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
                            const auto result = test_default_allocator({ 0,1,2,3 }, 1u);
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
                            constexpr auto result = test_default_allocator({ 0,1,2,3 }, 1u);
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
                        auto test_with_allocator = [](std::initializer_list<i32> initializer, usize copied_extra) {
                            const CopiedType copied(initializer, copied_extra);

                            // Copy the array
                            hud::array<type, Allocator> copy(copied, extra, Allocator{});

                            // Ensure we copy all datas in order
                            bool all_values_copied = true;
                            for (usize index = 0; index < initializer.size(); index++) {
                                if (copy[index] != static_cast<type>(index)) {
                                    all_values_copied = false;
                                    break;
                                }
                            }

                            return std::tuple{
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
                            const auto result = test_with_allocator({ 0,1,2,3 }, 1u);
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
                            constexpr auto result = test_with_allocator({ 0,1,2,3 }, 1u);
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
            });
        } });
}

GTEST_TEST(array, copy_construct_bitwise_copy_constructible_different_type)
{

    /** The array we copy for test, we allocate also extra memory to test if we really copy the count(), not the max_count() elements */
    using Type1 = i32;
    using Type2 = u32;

    using AllocatorOfCopiedArray = hud_test::array_allocator<alignof(Type1)>;
    using OtherAllocatorOfArray = hud_test::ArrayAllocator2<alignof(Type2)>;

    // Ensure we test with different allocator
    static_assert(!std::is_same_v<AllocatorOfCopiedArray, OtherAllocatorOfArray>);
    static_assert(!std::is_same_v<Type1, Type2>);
    static_assert(hud::is_bitwise_copy_constructible_v<Type2, Type1>);

    hud_test::for_each_type<AllocatorOfCopiedArray, OtherAllocatorOfArray>()([]<typename Allocator>() noexcept
                                                                             {

        // Test with default allocator no extra
        {
            auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra) {
                const hud::array<Type1, AllocatorOfCopiedArray> copied(initializer, copied_extra);

                // Copy the array
                hud::array<Type2, Allocator> copy(copied);

                // Ensure we copy all datas in order
                bool all_values_copied = true;
                for (usize index = 0; index < initializer.size(); index++) {
                    if (copy[index] != static_cast<Type2>(index)) {
                        all_values_copied = false;
                        break;
                    }
                }

                return std::tuple{
                    // Ensure we copy all datas in order
                    copy.data() != nullptr,
                    copy.count(),
                    copy.max_count(),
                    all_values_copied,

                    // Ensure the copy data is not the same memory of the copied data
                    (void*)copied.data() != (void*)copy.data(),

                    // Ensure we are allocating only one time
                    copy.allocator().allocation_count(),
                    copy.allocator().free_count()
                };
            };

            // Non constant
            {
                const auto result = test_default_allocator({ 0,1,2,3 }, 1u);
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
                constexpr auto result = test_default_allocator({ 0,1,2,3 }, 1u);
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
            auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra) {
                const hud::array<Type1, AllocatorOfCopiedArray> copied(initializer, copied_extra);

                // Copy the array
                hud::array<Type2, Allocator> copy(copied, Allocator{});

                // Ensure we copy all datas in order
                bool all_values_copied = true;
                for (usize index = 0; index < initializer.size(); index++) {
                    if (copy[index] != static_cast<Type2>(index)) {
                        all_values_copied = false;
                        break;
                    }
                }

                return std::tuple{
                    // Ensure we copy all datas in order
                    copy.data() != nullptr,
                    copy.count(),
                    copy.max_count(),
                    all_values_copied,

                    // Ensure the copy data is not the same memory of the copied data
                    (void*)copied.data() != (void*)copy.data(),

                    // Ensure we are allocating only one time
                    copy.allocator().allocation_count(),
                    copy.allocator().free_count()
                };
            };

            // Non constant
            {
                const auto result = test_default_allocator({ 0,1,2,3 }, 1u);
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
                constexpr auto result = test_default_allocator({ 0,1,2,3 }, 1u);
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
                []<usize extra>() {

                // Test with default allocator
                    {
                        auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra) {
                            const hud::array<Type1, AllocatorOfCopiedArray> copied(initializer, copied_extra);

                            // Copy the array
                            hud::array<Type2, Allocator> copy(copied, extra);

                            // Ensure we copy all datas in order
                            bool all_values_copied = true;
                            for (usize index = 0; index < initializer.size(); index++) {
                                if (copy[index] != static_cast<Type2>(index)) {
                                    all_values_copied = false;
                                    break;
                                }
                            }

                            return std::tuple{
                                // Ensure we copy all datas in order
                                copy.data() != nullptr,
                                copy.count(),
                                copy.max_count(),
                                all_values_copied,

                                // Ensure the copy data is not the same memory of the copied data
                                (void*)copied.data() != (void*)copy.data(),

                                // Ensure we are allocating only one time
                                copy.allocator().allocation_count(),
                                copy.allocator().free_count()
                            };
                        };

                        // Non constant
                        {
                            const auto result = test_default_allocator({ 0,1,2,3 }, 1u);
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
                            constexpr auto result = test_default_allocator({ 0,1,2,3 }, 1u);
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
                        auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra) {
                            const hud::array<Type1, AllocatorOfCopiedArray> copied(initializer, copied_extra);

                            // Copy the array
                            hud::array<Type2, Allocator> copy(copied, extra, Allocator{});

                            // Ensure we copy all datas in order
                            bool all_values_copied = true;
                            for (usize index = 0; index < initializer.size(); index++) {
                                if (copy[index] != static_cast<Type2>(index)) {
                                    all_values_copied = false;
                                    break;
                                }
                            }

                            return std::tuple{
                                // Ensure we copy all datas in order
                                copy.data() != nullptr,
                                copy.count(),
                                copy.max_count(),
                                all_values_copied,

                                // Ensure the copy data is not the same memory of the copied data
                                (void*)copied.data() != (void*)copy.data(),

                                // Ensure we are allocating only one time
                                copy.allocator().allocation_count(),
                                copy.allocator().free_count()
                            };
                        };

                        // Non constant
                        {
                            const auto result = test_default_allocator({ 0,1,2,3 }, 1u);
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
                            constexpr auto result = test_default_allocator({ 0,1,2,3 }, 1u);
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
            });
        } });
}

GTEST_TEST(array, copy_construct_non_bitwise_copy_constructible_same_type)
{

    /** The array we copyfor test, we allocate also extra memory to test if we really copy the count(), not the max_count() elements */
    using type = hud_test::NonBitwiseCopyConstructibleType;
    using AllocatorOfCopiedArray = hud_test::array_allocator<alignof(type)>;
    using OtherAllocatorOfArray = hud_test::ArrayAllocator2<alignof(type)>;

    // Ensure we test with different allocator
    static_assert(!std::is_same_v<AllocatorOfCopiedArray, OtherAllocatorOfArray>);
    static_assert(!hud::is_bitwise_copy_constructible_v<type>);

    using CopiedType = const hud::array<hud_test::NonBitwiseCopyConstructibleType, AllocatorOfCopiedArray>;

    hud_test::for_each_type<AllocatorOfCopiedArray, OtherAllocatorOfArray>()([]<typename Allocator>() noexcept
                                                                             {

        // Test default allocator
        {
            auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra) {
                const CopiedType copied(initializer, copied_extra);

                // Copy the array
                hud::array<type, Allocator> copy(copied);

                // Ensure we copy all datas in order
                bool all_values_copied = true;
                for (usize index = 0; index < initializer.size(); index++) {
                    if (static_cast<usize>(copy[index].id()) != index ||
                        copy[index].copy_constructor_count() != 1u) {
                        all_values_copied = false;
                        break;
                    }
                }

                return std::tuple{
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
                const auto result = test_default_allocator({ 0,1,2,3 }, 1u);
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
                constexpr auto result = test_default_allocator({ 0,1,2,3 }, 1u);
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
            auto test_wtih_allocator = [](std::initializer_list<i32> initializer, usize copied_extra) {
                const CopiedType copied(initializer, copied_extra);

                // Copy the array
                hud::array<type, Allocator> copy(copied, Allocator{});

                // Ensure we copy all datas in order
                bool all_values_copied = true;
                for (usize index = 0; index < initializer.size(); index++) {
                    if (static_cast<usize>(copy[index].id()) != index ||
                        copy[index].copy_constructor_count() != 1u) {
                        all_values_copied = false;
                        break;
                    }
                }

                return std::tuple{
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
                const auto result = test_wtih_allocator({ 0,1,2,3 }, 1u);
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
                constexpr auto result = test_wtih_allocator({ 0,1,2,3 }, 1u);
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
                []<usize extra>() {

                // Test default allocator
                    {
                        auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra) {
                            const CopiedType copied(initializer, copied_extra);

                            // Copy the array
                            hud::array<type, Allocator> copy(copied, extra);

                            // Ensure we copy all datas in order
                            bool all_values_copied = true;
                            for (usize index = 0; index < initializer.size(); index++) {
                                if (static_cast<usize>(copy[index].id()) != index ||
                                    copy[index].copy_constructor_count() != 1u) {
                                    all_values_copied = false;
                                    break;
                                }
                            }

                            return std::tuple{
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
                            const auto result = test_default_allocator({ 0,1,2,3 }, 1u);
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
                            constexpr auto result = test_default_allocator({ 0,1,2,3 }, 1u);
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
                        auto test_with_allocator = [](std::initializer_list<i32> initializer, usize copied_extra) {
                            const CopiedType copied(initializer, copied_extra);

                            // Copy the array
                            hud::array<type, Allocator> copy(copied, extra, Allocator{});

                            // Ensure we copy all datas in order
                            bool all_values_copied = true;
                            for (usize index = 0; index < initializer.size(); index++) {
                                if (static_cast<usize>(copy[index].id()) != index ||
                                    copy[index].copy_constructor_count() != 1u) {
                                    all_values_copied = false;
                                    break;
                                }
                            }

                            return std::tuple{
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
                            const auto result = test_with_allocator({ 0,1,2,3 }, 1u);
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
                            constexpr auto result = test_with_allocator({ 0,1,2,3 }, 1u);
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

            });
        } });
}

GTEST_TEST(array, copy_construct_non_bitwise_copy_constructible_different_type)
{

    /** The array we copyfor test, we allocate also extra memory to test if we really copy the count(), not the max_count() elements */
    using Type1 = hud_test::NonBitwiseCopyConstructibleType;
    using Type2 = hud_test::NonBitwiseCopyConstructibleType2;
    using AllocatorOfCopiedArray = hud_test::array_allocator<alignof(Type1)>;
    using OtherAllocatorOfArray = hud_test::ArrayAllocator2<alignof(Type2)>;

    // Ensure we test with different allocator
    static_assert(!std::is_same_v<AllocatorOfCopiedArray, OtherAllocatorOfArray>);
    static_assert(!std::is_same_v<Type1, Type2>);
    static_assert(!hud::is_bitwise_copy_constructible_v<Type2, Type1>);

    hud_test::for_each_type<AllocatorOfCopiedArray, OtherAllocatorOfArray>()([]<typename Allocator>() noexcept
                                                                             {

        // Test default allocator
        {
            auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra) {
                const hud::array<Type1, AllocatorOfCopiedArray>  copied(initializer, copied_extra);

                // Copy the array
                hud::array<Type2, Allocator> copy(copied);

                // Ensure we copy all datas in order
                bool all_values_copied = true;
                for (usize index = 0; index < initializer.size(); index++) {
                    if (static_cast<usize>(copy[index].id()) != index ||
                        copy[index].copy_constructor_count() != 1u) {
                        all_values_copied = false;
                        break;
                    }
                }

                return std::tuple{
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
                const auto result = test_default_allocator({ 0,1,2,3 }, 1u);
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
                constexpr auto result = test_default_allocator({ 0,1,2,3 }, 1u);
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
            auto test_with_allocator = [](std::initializer_list<i32> initializer, usize copied_extra) {
                const hud::array<Type1, AllocatorOfCopiedArray>  copied(initializer, copied_extra);

                // Copy the array
                hud::array<Type2, Allocator> copy(copied, Allocator{});

                // Ensure we copy all datas in order
                bool all_values_copied = true;
                for (usize index = 0; index < initializer.size(); index++) {
                    if (static_cast<usize>(copy[index].id()) != index ||
                        copy[index].copy_constructor_count() != 1u) {
                        all_values_copied = false;
                        break;
                    }
                }

                return std::tuple{
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
                const auto result = test_with_allocator({ 0,1,2,3 }, 1u);
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
                constexpr auto result = test_with_allocator({ 0,1,2,3 }, 1u);
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
                []<usize extra>() {

                // Test default allocator
                    {
                        auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra) {
                            const hud::array<Type1, AllocatorOfCopiedArray>  copied(initializer, copied_extra);

                            // Copy the array
                            hud::array<Type2, Allocator> copy(copied, extra);

                            // Ensure we copy all datas in order
                            bool all_values_copied = true;
                            for (usize index = 0; index < initializer.size(); index++) {
                                if (static_cast<usize>(copy[index].id()) != index ||
                                    copy[index].copy_constructor_count() != 1u) {
                                    all_values_copied = false;
                                    break;
                                }
                            }

                            return std::tuple{
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
                            const auto result = test_default_allocator({ 0,1,2,3 }, 1u);
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
                            constexpr auto result = test_default_allocator({ 0,1,2,3 }, 1u);
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
                        auto test_with_allocator = [](std::initializer_list<i32> initializer, usize copied_extra) {
                            const hud::array<Type1, AllocatorOfCopiedArray>  copied(initializer, copied_extra);

                            // Copy the array
                            hud::array<Type2, Allocator> copy(copied, extra, Allocator{});

                            // Ensure we copy all datas in order
                            bool all_values_copied = true;
                            for (usize index = 0; index < initializer.size(); index++) {
                                if (static_cast<usize>(copy[index].id()) != index ||
                                    copy[index].copy_constructor_count() != 1u) {
                                    all_values_copied = false;
                                    break;
                                }
                            }

                            return std::tuple{
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
                            const auto result = test_with_allocator({ 0,1,2,3 }, 1u);
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
                            constexpr auto result = test_with_allocator({ 0,1,2,3 }, 1u);
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
            });
        } });
}

GTEST_TEST(array, move_construct_bitwise_copy_constructible_same_type)
{

    /** The array we move for test, we allocate also extra memory to test if we really move the count(), not the max_count() elements */
    using type = i32;
    using AllocatorOfMovedArray = hud_test::array_allocator<alignof(type)>;
    using OtherAllocatorOfArray = hud_test::ArrayAllocator2<alignof(type)>;

    // Ensure we test with different allocator
    static_assert(!std::is_same_v<AllocatorOfMovedArray, OtherAllocatorOfArray>);
    static_assert(hud::is_bitwise_move_constructible_v<type>);
    using MovedType = hud::array<type, AllocatorOfMovedArray>;

    hud_test::for_each_type<AllocatorOfMovedArray, OtherAllocatorOfArray>()([]<typename Allocator>() noexcept
                                                                            {

        // Test default allocator
        {

            auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra) {
                MovedType moved(initializer, copied_extra);

                // Copy the array
                hud::array<type, Allocator> move(std::move(moved));

                // Ensure we moved all datas in order
                bool all_values_moved = true;
                for (usize index = 0; index < initializer.size(); index++) {
                    if (move[index] != static_cast<type>(index)) {
                        all_values_moved = false;
                        break;
                    }
                }

                return std::tuple{
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
                const auto result = test_default_allocator({ 0,1,2,3 }, 1u);
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
                if (std::is_same_v<AllocatorOfMovedArray, Allocator>) {
                    hud_assert_eq(std::get<5>(result), 0u);
                    hud_assert_eq(std::get<6>(result), 0u);
                    hud_assert_eq(std::get<7>(result), 0u);
                    hud_assert_eq(std::get<8>(result), 1u);
                    hud_assert_eq(std::get<9>(result), 1u);
                    hud_assert_eq(std::get<10>(result), 0u);
                    hud_assert_eq(std::get<11>(result), 0u);
                    hud_assert_eq(std::get<12>(result), 0u);
                }
                else {
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

            // Constant
            {
                constexpr auto result = test_default_allocator({ 0,1,2,3 }, 1u);
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
                if (std::is_same_v<AllocatorOfMovedArray, Allocator>) {
                    hud_assert_eq(std::get<5>(result), 0u);
                    hud_assert_eq(std::get<6>(result), 0u);
                    hud_assert_eq(std::get<7>(result), 0u);
                    hud_assert_eq(std::get<8>(result), 1u);
                    hud_assert_eq(std::get<9>(result), 1u);
                    hud_assert_eq(std::get<10>(result), 0u);
                    hud_assert_eq(std::get<11>(result), 0u);
                    hud_assert_eq(std::get<12>(result), 0u);
                }
                else {
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

        // Test with allocator
        {
            auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra) {
                MovedType moved(initializer, copied_extra);

                // Copy the array
                hud::array<type, Allocator> move(std::move(moved), Allocator{});

                // Ensure we moved all datas in order
                bool all_values_moved = true;
                for (usize index = 0; index < initializer.size(); index++) {
                    if (move[index] != static_cast<type>(index)) {
                        all_values_moved = false;
                        break;
                    }
                }

                return std::tuple{
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
                const auto result = test_default_allocator({ 0,1,2,3 }, 1u);
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
                constexpr auto result = test_default_allocator({ 0,1,2,3 }, 1u);
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
                []<usize extra>() {

                // Test default allocator
                    {

                        auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra) {
                            MovedType moved(initializer, copied_extra);

                            // Copy the array
                            hud::array<type, Allocator> move(std::move(moved), extra);

                            // Ensure we moved all datas in order
                            bool all_values_moved = true;
                            for (usize index = 0; index < initializer.size(); index++) {
                                if (move[index] != static_cast<type>(index)) {
                                    all_values_moved = false;
                                    break;
                                }
                            }

                            return std::tuple{
                                // Ensure we copy all datas in order
                                move.data() != nullptr,
                                move.count(),
                                move.max_count(),
                                all_values_moved,

                                // Ensure the copy data is not the same memory of the copied data
                                (void*)moved.data() != (void*)move.data(),

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
                            const auto result = test_default_allocator({ 0,1,2,3 }, 1u);
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
                            constexpr auto result = test_default_allocator({ 0,1,2,3 }, 1u);
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
                        auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra) {
                            MovedType moved(initializer, copied_extra);

                            // Copy the array
                            hud::array<type, Allocator> move(std::move(moved), extra, Allocator{});

                            // Ensure we moved all datas in order
                            bool all_values_moved = true;
                            for (usize index = 0; index < initializer.size(); index++) {
                                if (move[index] != static_cast<type>(index)) {
                                    all_values_moved = false;
                                    break;
                                }
                            }

                            return std::tuple{
                                // Ensure we copy all datas in order
                                move.data() != nullptr,
                                move.count(),
                                move.max_count(),
                                all_values_moved,

                                // Ensure the copy data is not the same memory of the copied data
                                (void*)moved.data() != (void*)move.data(),

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
                            const auto result = test_default_allocator({ 0,1,2,3 }, 1u);
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
                            constexpr auto result = test_default_allocator({ 0,1,2,3 }, 1u);
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
            });
        } });
}

GTEST_TEST(array, move_construct_bitwise_move_constructible_different_type)
{

    /** The array we move for test, we allocate also extra memory to test if we really move the count(), not the max_count() elements */
    using Type1 = i32;
    using Type2 = u32;
    using AllocatorOfMovedArray = hud_test::array_allocator<alignof(Type1)>;
    using OtherAllocatorOfArray = hud_test::ArrayAllocator2<alignof(Type2)>;

    // Ensure we test with different allocator
    static_assert(!std::is_same_v<AllocatorOfMovedArray, OtherAllocatorOfArray>);
    static_assert(!std::is_same_v<Type1, Type2>);
    static_assert(hud::is_bitwise_move_constructible_v<Type2, Type1>);

    using MovedType = hud::array<Type1, AllocatorOfMovedArray>;

    hud_test::for_each_type<AllocatorOfMovedArray, OtherAllocatorOfArray>()([]<typename Allocator>() noexcept
                                                                            {

        // Test default allocator
        {
            auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra) {
                MovedType moved(initializer, copied_extra);

                // Copy the array
                hud::array<Type2, Allocator> move(std::move(moved));

                // Ensure we moved all datas in order
                bool all_values_moved = true;
                for (usize index = 0; index < initializer.size(); index++) {
                    if (move[index] != index) {
                        all_values_moved = false;
                        break;
                    }
                }

                return std::tuple{
                    // Ensure we copy all datas in order
                    move.data() != nullptr,
                    move.count(),
                    move.max_count(),
                    all_values_moved,

                    // Ensure the copy data is not the same memory of the copied data
                    (void*)moved.data() != (void*)move.data(),

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
                const auto result = test_default_allocator({ 0,1,2,3 }, 1u);
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
                if (std::is_same_v<AllocatorOfMovedArray, Allocator>) {
                    hud_assert_eq(std::get<5>(result), 0u);
                    hud_assert_eq(std::get<6>(result), 0u);
                    hud_assert_eq(std::get<7>(result), 0u);
                    hud_assert_eq(std::get<8>(result), 1u);
                    hud_assert_eq(std::get<9>(result), 1u);
                    hud_assert_eq(std::get<10>(result), 0u);
                    hud_assert_eq(std::get<11>(result), 0u);
                    hud_assert_eq(std::get<12>(result), 0u);
                }
                else {
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

            // Constant
            {
                constexpr auto result = test_default_allocator({ 0,1,2,3 }, 1u);
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
                if (std::is_same_v<AllocatorOfMovedArray, Allocator>) {
                    hud_assert_eq(std::get<5>(result), 1u);
                    hud_assert_eq(std::get<6>(result), 0u);
                    hud_assert_eq(std::get<7>(result), 0u);
                    hud_assert_eq(std::get<8>(result), 1u);
                    hud_assert_eq(std::get<9>(result), 1u);
                    hud_assert_eq(std::get<10>(result), 1u);
                    hud_assert_eq(std::get<11>(result), 0u);
                    hud_assert_eq(std::get<12>(result), 0u);
                }
                else {
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

        // Test with allocator
        {
            auto test_with_allocator = [](std::initializer_list<i32> initializer, usize copied_extra) {
                MovedType moved(initializer, copied_extra);

                // Copy the array
                hud::array<Type2, Allocator> move(std::move(moved), Allocator{});

                // Ensure we moved all datas in order
                bool all_values_moved = true;
                for (usize index = 0; index < initializer.size(); index++) {
                    if (move[index] != index) {
                        all_values_moved = false;
                        break;
                    }
                }

                return std::tuple{
                    // Ensure we copy all datas in order
                    move.data() != nullptr,
                    move.count(),
                    move.max_count(),
                    all_values_moved,

                    // Ensure the copy data is not the same memory of the copied data
                    (void*)moved.data() != (void*)move.data(),

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
                const auto result = test_with_allocator({ 0,1,2,3 }, 1u);
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
                constexpr auto result = test_with_allocator({ 0,1,2,3 }, 1u);
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
                []<usize extra>() {

                // Test default allocator
                    {
                        auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra) {
                            MovedType moved(initializer, copied_extra);

                            // Copy the array
                            hud::array<Type2, Allocator> move(std::move(moved), extra);

                            // Ensure we moved all datas in order
                            bool all_values_moved = true;
                            for (usize index = 0; index < initializer.size(); index++) {
                                if (move[index] != index) {
                                    all_values_moved = false;
                                    break;
                                }
                            }

                            return std::tuple{
                                // Ensure we copy all datas in order
                                move.data() != nullptr,
                                move.count(),
                                move.max_count(),
                                all_values_moved,

                                // Ensure the copy data is not the same memory of the copied data
                                (void*)moved.data() != (void*)move.data(),

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
                            const auto result = test_default_allocator({ 0,1,2,3 }, 1u);
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
                            constexpr auto result = test_default_allocator({ 0,1,2,3 }, 1u);
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
                        auto test_with_allocator = [](std::initializer_list<i32> initializer, usize copied_extra) {
                            MovedType moved(initializer, copied_extra);

                            // Copy the array
                            hud::array<Type2, Allocator> move(std::move(moved), extra, Allocator{});

                            // Ensure we moved all datas in order
                            bool all_values_moved = true;
                            for (usize index = 0; index < initializer.size(); index++) {
                                if (move[index] != index) {
                                    all_values_moved = false;
                                    break;
                                }
                            }

                            return std::tuple{
                                // Ensure we copy all datas in order
                                move.data() != nullptr,
                                move.count(),
                                move.max_count(),
                                all_values_moved,

                                // Ensure the copy data is not the same memory of the copied data
                                (void*)moved.data() != (void*)move.data(),

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
                            const auto result = test_with_allocator({ 0,1,2,3 }, 1u);
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
                            constexpr auto result = test_with_allocator({ 0,1,2,3 }, 1u);
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
            });
        } });
}

GTEST_TEST(array, move_construct_non_bitwise_copy_constructible_same_type)
{

    /** The array we move for test, we allocate also extra memory to test if we really move the count(), not the max_count() elements */
    using type = hud_test::NonBitwiseCopyConstructibleType;

    using AllocatorOfMovedArray = hud_test::array_allocator<alignof(type)>;
    using OtherAllocatorOfArray = hud_test::ArrayAllocator2<alignof(type)>;

    // Ensure we test with different allocator
    static_assert(!std::is_same_v<AllocatorOfMovedArray, OtherAllocatorOfArray>);
    static_assert(!hud::is_bitwise_move_constructible_v<type>);

    using MovedType = hud::array<type, AllocatorOfMovedArray>;

    hud_test::for_each_type<AllocatorOfMovedArray, OtherAllocatorOfArray>()([]<typename Allocator>() noexcept
                                                                            {

        // Test default allocator
        {
            auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra) {
                MovedType moved(initializer, copied_extra);

                // Copy the array
                hud::array<type, Allocator> move(std::move(moved));

                // Ensure we moved all datas in order
                bool all_values_moved = true;
                for (usize index = 0; index < initializer.size(); index++) {
                    if (static_cast<usize>(move[index].id()) != index ||
                        move[index].copy_constructor_count() != 1u) {
                        all_values_moved = false;
                        break;
                    }
                }

                return std::tuple{
                    // Ensure we copy all datas in order
                    move.data() != nullptr,
                    move.count(),
                    move.max_count(),
                    all_values_moved,

                    // Ensure the copy data is not the same memory of the copied data
                    (void*)moved.data() != (void*)move.data(),

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
                const auto result = test_default_allocator({ 0,1,2,3 }, 1u);
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
                constexpr auto result = test_default_allocator({ 0,1,2,3 }, 1u);
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
            auto test_with_allocator = [](std::initializer_list<i32> initializer, usize copied_extra) {
                MovedType moved(initializer, copied_extra);

                // Copy the array
                hud::array<type, Allocator> move(std::move(moved), Allocator{});

                // Ensure we moved all datas in order
                bool all_values_moved = true;
                for (usize index = 0; index < initializer.size(); index++) {
                    if (static_cast<usize>(move[index].id()) != index ||
                        move[index].copy_constructor_count() != 1u) {
                        all_values_moved = false;
                        break;
                    }
                }

                return std::tuple{
                    // Ensure we copy all datas in order
                    move.data() != nullptr,
                    move.count(),
                    move.max_count(),
                    all_values_moved,

                    // Ensure the copy data is not the same memory of the copied data
                    (void*)moved.data() != (void*)move.data(),

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
                const auto result = test_with_allocator({ 0,1,2,3 }, 1u);
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
                constexpr auto result = test_with_allocator({ 0,1,2,3 }, 1u);
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
                []<usize extra>() {

                // Test default allocator
                {
                    auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra) {
                        MovedType moved(initializer, copied_extra);

                        // Copy the array
                        hud::array<type, Allocator> move(std::move(moved), extra);

                        // Ensure we moved all datas in order
                        bool all_values_moved = true;
                        for (usize index = 0; index < initializer.size(); index++) {
                            if (static_cast<usize>(move[index].id()) != index ||
                                move[index].copy_constructor_count() != 1u) {
                                all_values_moved = false;
                                break;
                            }
                        }

                        return std::tuple{
                            // Ensure we copy all datas in order
                            move.data() != nullptr,
                            move.count(),
                            move.max_count(),
                            all_values_moved,

                            // Ensure the copy data is not the same memory of the copied data
                            (void*)moved.data() != (void*)move.data(),

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
                        const auto result = test_default_allocator({ 0,1,2,3 }, 1u);
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
                        constexpr auto result = test_default_allocator({ 0,1,2,3 }, 1u);
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
                    auto test_with_allocator = [](std::initializer_list<i32> initializer, usize copied_extra) {
                        MovedType moved(initializer, copied_extra);

                        // Copy the array
                        hud::array<type, Allocator> move(std::move(moved), extra, Allocator{});

                        // Ensure we moved all datas in order
                        bool all_values_moved = true;
                        for (usize index = 0; index < initializer.size(); index++) {
                            if (static_cast<usize>(move[index].id()) != index ||
                                move[index].copy_constructor_count() != 1u) {
                                all_values_moved = false;
                                break;
                            }
                        }

                        return std::tuple{
                            // Ensure we copy all datas in order
                            move.data() != nullptr,
                            move.count(),
                            move.max_count(),
                            all_values_moved,

                            // Ensure the copy data is not the same memory of the copied data
                            (void*)moved.data() != (void*)move.data(),

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
                        const auto result = test_with_allocator({ 0,1,2,3 }, 1u);
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
                        constexpr auto result = test_with_allocator({ 0,1,2,3 }, 1u);
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
            });
        } });
}

GTEST_TEST(array, move_construct_non_bitwise_move_constructible_same_type)
{

    /** The array we copy for test, we allocate also extra memory to test if we really copy the count(), not the max_count() elements */
    using type = hud_test::NonBitwiseMoveConstructibleType;
    using AllocatorOfMovedArray = hud_test::array_allocator<alignof(type)>;
    using OtherAllocatorOfArray = hud_test::ArrayAllocator2<alignof(type)>;

    // Ensure we test with different allocator
    static_assert(!std::is_same_v<AllocatorOfMovedArray, OtherAllocatorOfArray>);
    static_assert(!hud::is_bitwise_move_constructible_v<type>);

    using MovedType = hud::array<type, AllocatorOfMovedArray>;

    hud_test::for_each_type<AllocatorOfMovedArray, OtherAllocatorOfArray>()([]<typename Allocator>() noexcept
                                                                            {
        
        // Test default allocator
        {
            auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra) {
                MovedType moved(initializer, copied_extra);

                // Copy the array
                hud::array<type, Allocator> move(std::move(moved));

                // Ensure we moved all datas in order
                bool all_values_moved = true;
                for (usize index = 0; index < initializer.size(); index++) {
                    if (static_cast<usize>(move[index].id()) != index) {
                        all_values_moved = false;
                        break;
                    }
                    if (move[index].move_constructor_count() != 1u) {
                        all_values_moved = false;
                        break;
                    }
                }

                return std::tuple{
                    // Ensure we copy all datas in order
                    move.data() != nullptr,
                    move.count(),
                    move.max_count(),
                    all_values_moved,

                    // Ensure the copy data is not the same memory of the copied data
                    (void*)moved.data() != (void*)move.data(),

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
                const auto result = test_default_allocator({ 0,1,2,3 }, 1u);
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
                constexpr auto result = test_default_allocator({ 0,1,2,3 }, 1u);
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
            auto test_with_allocator = [](std::initializer_list<i32> initializer, usize copied_extra) {
                MovedType moved(initializer, copied_extra);

                // Copy the array
                hud::array<type, Allocator> move(std::move(moved), Allocator{});

                // Ensure we moved all datas in order
                bool all_values_moved = true;
                for (usize index = 0; index < initializer.size(); index++) {
                    if (static_cast<usize>(move[index].id()) != index) {
                        all_values_moved = false;
                        break;
                    }
                    if (move[index].move_constructor_count() != 1u) {
                        all_values_moved = false;
                        break;
                    }
                }

                return std::tuple{
                    // Ensure we copy all datas in order
                    move.data() != nullptr,
                    move.count(),
                    move.max_count(),
                    all_values_moved,

                    // Ensure the copy data is not the same memory of the copied data
                    (void*)moved.data() != (void*)move.data(),

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
                const auto result = test_with_allocator({ 0,1,2,3 }, 1u);
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
                constexpr auto result = test_with_allocator({ 0,1,2,3 }, 1u);
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
                []<usize extra>() {

                // Test default allocator
                    {
                        auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra) {
                            MovedType moved(initializer, copied_extra);

                            // Copy the array
                            hud::array<type, Allocator> move(std::move(moved), extra);

                            // Ensure we moved all datas in order
                            bool all_values_moved = true;
                            for (usize index = 0; index < initializer.size(); index++) {
                                if (static_cast<usize>(move[index].id()) != index) {
                                    all_values_moved = false;
                                    break;
                                }
                                if (move[index].move_constructor_count() != 1u) {
                                    all_values_moved = false;
                                    break;
                                }
                            }

                            return std::tuple{
                                // Ensure we copy all datas in order
                                move.data() != nullptr,
                                move.count(),
                                move.max_count(),
                                all_values_moved,

                                // Ensure the copy data is not the same memory of the copied data
                                (void*)moved.data() != (void*)move.data(),

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
                            const auto result = test_default_allocator({ 0,1,2,3 }, 1u);
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
                            constexpr auto result = test_default_allocator({ 0,1,2,3 }, 1u);
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
                        auto test_with_allocator = [](std::initializer_list<i32> initializer, usize copied_extra) {
                            MovedType moved(initializer, copied_extra);

                            // Copy the array
                            hud::array<type, Allocator> move(std::move(moved), extra, Allocator{});

                            // Ensure we moved all datas in order
                            bool all_values_moved = true;
                            for (usize index = 0; index < initializer.size(); index++) {
                                if (static_cast<usize>(move[index].id()) != index) {
                                    all_values_moved = false;
                                    break;
                                }
                                if (move[index].move_constructor_count() != 1u) {
                                    all_values_moved = false;
                                    break;
                                }
                            }

                            return std::tuple{
                                // Ensure we copy all datas in order
                                move.data() != nullptr,
                                move.count(),
                                move.max_count(),
                                all_values_moved,

                                // Ensure the copy data is not the same memory of the copied data
                                (void*)moved.data() != (void*)move.data(),

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
                            const auto result = test_with_allocator({ 0,1,2,3 }, 1u);
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
                            constexpr auto result = test_with_allocator({ 0,1,2,3 }, 1u);
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
            });
        } });
}

GTEST_TEST(array, move_construct_non_bitwise_move_constructible_different_type)
{

    /** The array we move for test, we allocate also extra memory to test if we really move the count(), not the max_count() elements */
    using Type1 = hud_test::NonBitwiseMoveConstructibleType;
    using Type2 = hud_test::NonBitwiseMoveConstructibleType2;
    using AllocatorOfMovedArray = hud_test::array_allocator<alignof(Type1)>;
    using OtherAllocatorOfArray = hud_test::ArrayAllocator2<alignof(Type2)>;

    // Ensure we test with different allocator and types
    static_assert(!std::is_same_v<AllocatorOfMovedArray, OtherAllocatorOfArray>);
    static_assert(!std::is_same_v<Type1, Type2>);
    static_assert(!hud::is_bitwise_move_constructible_v<Type2, Type1>);

    using MovedType = hud::array<Type1, AllocatorOfMovedArray>;

    hud_test::for_each_type<AllocatorOfMovedArray, OtherAllocatorOfArray>()([]<typename Allocator>() noexcept
                                                                            {

        // Test default allocator
        {
            auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra) {
                MovedType moved(initializer, copied_extra);

                // Copy the array
                hud::array<Type2, Allocator> move(std::move(moved));

                // Ensure we moved all datas in order
                bool all_values_moved = true;
                for (usize index = 0; index < initializer.size(); index++) {
                    if (static_cast<usize>(move[index].id()) != index) {
                        all_values_moved = false;
                        break;
                    }
                    if (move[index].move_constructor_count() != 1u) {
                        all_values_moved = false;
                        break;
                    }
                }

                return std::tuple{
                    // Ensure we copy all datas in order
                    move.data() != nullptr,
                    move.count(),
                    move.max_count(),
                    all_values_moved,

                    // Ensure the copy data is not the same memory of the copied data
                    (void*)moved.data() != (void*)move.data(),

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
                const auto result = test_default_allocator({ 0,1,2,3 }, 1u);
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
                constexpr auto result = test_default_allocator({ 0,1,2,3 }, 1u);
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
            auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra) {
                MovedType moved(initializer, copied_extra);

                // Copy the array
                hud::array<Type2, Allocator> move(std::move(moved), Allocator{});

                // Ensure we moved all datas in order
                bool all_values_moved = true;
                for (usize index = 0; index < initializer.size(); index++) {
                    if (static_cast<usize>(move[index].id()) != index) {
                        all_values_moved = false;
                        break;
                    }
                    if (move[index].move_constructor_count() != 1u) {
                        all_values_moved = false;
                        break;
                    }
                }

                return std::tuple{
                    // Ensure we copy all datas in order
                    move.data() != nullptr,
                    move.count(),
                    move.max_count(),
                    all_values_moved,

                    // Ensure the copy data is not the same memory of the copied data
                    (void*)moved.data() != (void*)move.data(),

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
                const auto result = test_default_allocator({ 0,1,2,3 }, 1u);
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
                constexpr auto result = test_default_allocator({ 0,1,2,3 }, 1u);
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
                []<usize extra>() {

                // Test default allocator
                {
                    auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra) {
                        MovedType moved(initializer, copied_extra);

                        // Copy the array
                        hud::array<Type2, Allocator> move(std::move(moved), extra);

                        // Ensure we moved all datas in order
                        bool all_values_moved = true;
                        for (usize index = 0; index < initializer.size(); index++) {
                            if (static_cast<usize>(move[index].id()) != index) {
                                all_values_moved = false;
                                break;
                            }
                            if (move[index].move_constructor_count() != 1u) {
                                all_values_moved = false;
                                break;
                            }
                        }

                        return std::tuple{
                            // Ensure we copy all datas in order
                            move.data() != nullptr,
                            move.count(),
                            move.max_count(),
                            all_values_moved,

                            // Ensure the copy data is not the same memory of the copied data
                            (void*)moved.data() != (void*)move.data(),

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
                        const auto result = test_default_allocator({ 0,1,2,3 }, 1u);
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
                        constexpr auto result = test_default_allocator({ 0,1,2,3 }, 1u);
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
                    auto test_default_allocator = [](std::initializer_list<i32> initializer, usize copied_extra) {
                        MovedType moved(initializer, copied_extra);

                        // Copy the array
                        hud::array<Type2, Allocator> move(std::move(moved), extra, Allocator{});

                        // Ensure we moved all datas in order
                        bool all_values_moved = true;
                        for (usize index = 0; index < initializer.size(); index++) {
                            if (static_cast<usize>(move[index].id()) != index) {
                                all_values_moved = false;
                                break;
                            }
                            if (move[index].move_constructor_count() != 1u) {
                                all_values_moved = false;
                                break;
                            }
                        }

                        return std::tuple{
                            // Ensure we copy all datas in order
                            move.data() != nullptr,
                            move.count(),
                            move.max_count(),
                            all_values_moved,

                            // Ensure the copy data is not the same memory of the copied data
                            (void*)moved.data() != (void*)move.data(),

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
                        const auto result = test_default_allocator({ 0,1,2,3 }, 1u);
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
                        constexpr auto result = test_default_allocator({ 0,1,2,3 }, 1u);
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
            });
        } });
}
