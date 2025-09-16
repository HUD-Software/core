#include <core/containers/vector.h>
#include "../misc/allocator_watcher.h"

GTEST_TEST(vector, default_constructor_should_allocate_no_memory)
{
    auto test = []() -> std::tuple<bool, usize, usize> {
        hud::vector<hud::u32> vector;
        return {vector.data() == nullptr, vector.count(), vector.max_count()};
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

GTEST_TEST(vector, construct_by_copying_raw_data_array_of_bitwise_copy_constructible_same_type)
{

    using TypeList = std::tuple<i8, i16, i32, i64, u8, u16, u32, u64, uptr, iptr, usize, isize>;

    hud_test::for_each_type<TypeList>()([]<typename type_t>() noexcept {

        static_assert(hud::is_bitwise_copy_constructible_v<type_t>);


        // test with default allocator no extra
        {
            auto test_default_allocator = [](const type_t* raw, usize element_count) {
                hud::vector<type_t> vector(raw, element_count);
                bool all_values_copied = true;

                // Ensure values are correclty copied
                for (usize index = 0; index < element_count; index++) {
                    if (*(vector.data() + index) != raw[index]) {
                        all_values_copied = false;
                        break;
                    }
                }

                return std::tuple{
                    // Allocation of 4 i32 should be done
                    vector.data() != nullptr,
                    vector.count(),
                    vector.max_count(),

                    // Ensure it's not the same memory buffer
                    vector.data() != &raw[0],

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
                hud::vector<type_t, hud_test::allocator_watcher<alignof(type_t)>> vector(raw, element_count, hud_test::allocator_watcher<alignof(type_t)>{});
                bool all_values_copied = true;

                // Ensure values are correclty copied
                for (usize index = 0; index < element_count; index++) {
                    if (*(vector.data() + index) != raw[index]) {
                        all_values_copied = false;
                        break;
                    }
                }

                return std::tuple{
                    // Allocation of 4 i32 should be done
                    vector.data() != nullptr,
                    vector.count(),
                    vector.max_count(),

                    // Ensure it's not the same memory buffer
                    vector.data() != &raw[0],

                    // Ensure values are correclty copied
                    all_values_copied,

                    // Ensure we are allocating only one time
                    vector.allocator().allocation_count(),
                    vector.allocator().free_count()
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
            hud_test::for_each_value(std::make_integer_sequence<usize, 5>(),
                []<usize extra>() {

                // Test default allocator with extra
                    {
                        auto test_default_allocator = [](const type_t* raw, usize element_count) {
                            hud::vector<type_t> vector(raw, element_count, extra);
                            bool all_values_copied = true;

                            // Ensure values are correclty copied
                            for (usize index = 0; index < element_count; index++) {
                                if (*(vector.data() + index) != raw[index]) {
                                    all_values_copied = false;
                                    break;
                                }
                            }

                            return std::tuple{
                                // Allocation of 4 i32 should be done
                                vector.data() != nullptr,
                                vector.count(),
                                vector.max_count(),

                                // Ensure it's not the same memory buffer
                                vector.data() != &raw[0],

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
                            hud::vector<type_t, hud_test::allocator_watcher<alignof(type_t)>> vector(raw, element_count, extra, hud_test::allocator_watcher<alignof(type_t)>{});
                            bool all_values_copied = true;

                            // Ensure values are correclty copied
                            for (usize index = 0; index < element_count; index++) {
                                if (*(vector.data() + index) != raw[index]) {
                                    all_values_copied = false;
                                    break;
                                }
                            }

                            return std::tuple{
                                // Allocation of 4 i32 should be done
                                vector.data() != nullptr,
                                vector.count(),
                                vector.max_count(),

                                // Ensure it's not the same memory buffer
                                vector.data() != &raw[0],

                                // Ensure values are correclty copied
                                all_values_copied,

                                // Ensure we are allocating only one time
                                vector.allocator().allocation_count(),
                                vector.allocator().free_count()
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

GTEST_TEST(vector, construct_by_copying_raw_data_array_of_non_bitwise_copy_constructible_same_type)
{

    using type = hud_test::non_bitwise_copy_constructible_type;
    static_assert(!hud::is_bitwise_copy_constructible_v<type>);

    // test with default allocator no extra
    {
        auto test_default_allocator = [](const type *raw, usize element_count) {
            hud::vector<type> vector(raw, element_count);

            // Ensure values are correclty copied in order
            bool all_values_copied_in_order = true;
            for (usize index = 0; index < element_count; index++) {
                if (vector[index].id() != raw[index].id()) {
                    all_values_copied_in_order = false;
                    break;
                }
            }

            // Ensure the copy constructor are called
            bool all_copy_constructor_are_called = true;
            for (usize index = 0; index < element_count; index++) {
                // Ensure we call the copy constructor and the id is correctly copied
                if (vector[index].copy_constructor_count() != 1u || vector[index].id() != raw[index].id()) {
                    all_copy_constructor_are_called = false;
                    break;
                }
            }

            return std::tuple {
                // Allocation of 4 i32 should be done
                vector.data() != nullptr,
                vector.count(),
                vector.max_count(),

                // Ensure it's not the same memory buffer
                vector.data() != &raw[0],

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
        auto test_with_allocator = [](const type *raw, usize element_count) {
            hud::vector<type, hud_test::allocator_watcher<alignof(type)>> vector(raw, element_count, hud_test::allocator_watcher<alignof(type)> {});

            // Ensure values are correclty copied in order
            bool all_values_copied_in_order = true;
            for (usize index = 0; index < element_count; index++) {
                if (vector[index].id() != raw[index].id()) {
                    all_values_copied_in_order = false;
                    break;
                }
            }

            // Ensure the copy constructor are called
            bool all_copy_constructor_are_called = true;
            for (usize index = 0; index < element_count; index++) {
                // Ensure we call the copy constructor and the id is correctly copied
                if (vector[index].copy_constructor_count() != 1u || vector[index].id() != raw[index].id()) {
                    all_copy_constructor_are_called = false;
                    break;
                }
            }
            return std::tuple {
                // Allocation of 4 i32 should be done
                vector.data() != nullptr,
                vector.count(),
                vector.max_count(),

                // Ensure it's not the same memory buffer
                vector.data() != &raw[0],

                // Ensure values are correclty copied in order
                all_values_copied_in_order,

                // Ensure the copy constructor are called
                all_copy_constructor_are_called,

                // Ensure we are allocating only one time
                vector.allocator().allocation_count(),
                vector.allocator().free_count()

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
        hud_test::for_each_value(std::make_integer_sequence<usize, 5>(), []<usize extra>() {
                // Test default allcoator with extra
                {
                    auto test_default_allocator = [](const type *raw, usize element_count)
                    {
                        hud::vector<type> vector(raw, element_count, extra);

                        // Ensure values are correclty copied in order
                        bool all_values_copied_in_order = true;
                        for (usize index = 0; index < element_count; index++)
                        {
                            if (vector[index].id() != raw[index].id())
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
                            if (vector[index].copy_constructor_count() != 1u || vector[index].id() != raw[index].id())
                            {
                                all_copy_constructor_are_called = false;
                                break;
                            }
                        }

                        return std::tuple {
                            // Allocation of 4 i32 should be done
                            vector.data() != nullptr,
                            vector.count(),
                            vector.max_count(),

                            // Ensure it's not the same memory buffer
                            vector.data() != &raw[0],

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
                        hud::vector<type, hud_test::allocator_watcher<alignof(type)>> vector(raw, element_count, extra, hud_test::allocator_watcher<alignof(type)> {});

                        // Ensure values are correclty copied in order
                        bool all_values_copied_in_order = true;
                        for (usize index = 0; index < element_count; index++)
                        {
                            if (vector[index].id() != raw[index].id())
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
                            if (vector[index].copy_constructor_count() != 1u || vector[index].id() != raw[index].id())
                            {
                                all_copy_constructor_are_called = false;
                                break;
                            }
                        }
                        return std::tuple {
                            // Allocation of 4 i32 should be done
                            vector.data() != nullptr,
                            vector.count(),
                            vector.max_count(),

                            // Ensure it's not the same memory buffer
                            vector.data() != &raw[0],

                            // Ensure values are correclty copied in order
                            all_values_copied_in_order,

                            // Ensure the copy constructor are called
                            all_copy_constructor_are_called,

                            // Ensure we are allocating only one time
                            vector.allocator().allocation_count(),
                            vector.allocator().free_count()

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
                } });
    }
}

GTEST_TEST(vector, construct_by_copying_raw_data_array_of_bitwise_copy_constructible_different_type)
{

    using TypesToTest = std::tuple<i8, i16, i32, i64, u8, u16, u32, u64, f32, f64, uptr, iptr, usize, isize>;

    bool at_least_one_copy_tested = false;
    hud_test::for_each_type<TypesToTest>()([&at_least_one_copy_tested]<typename type_t>() noexcept { hud_test::for_each_type<TypesToTest>()([&at_least_one_copy_tested]<typename U>() noexcept {

            // Test only types that are not the same but are bitwise copy constructible
            if constexpr (!std::is_same_v<type_t, U> && hud::is_bitwise_copy_constructible_v<type_t, U>)
            {
                at_least_one_copy_tested = true;

                //test with default allocator no extra
                {
                    auto test_default_allocator = [](const U* raw, usize element_count) {
                        hud::vector<type_t> vector(raw, element_count);
                        bool all_values_copied = true;

                        // Ensure values are correclty copied
                        for (usize index = 0; index < element_count; index++) {
                            if (*(vector.data() + index) != static_cast<type_t>(raw[index])) {
                                all_values_copied = false;
                                break;
                            }
                        }

                        return std::tuple{
                            // Allocation of 4 i32 should be done
                            vector.data() != nullptr,
                            vector.count(),
                            vector.max_count(),

                            // Ensure it's not the same memory buffer
                            (void*)vector.data() != (void*)&raw[0],

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
                        hud::vector<type_t, hud_test::allocator_watcher<alignof(type_t)>> vector(raw, element_count, hud_test::allocator_watcher<alignof(type_t)>{});
                        bool all_values_copied = true;

                        // Ensure values are correclty copied
                        for (usize index = 0; index < element_count; index++) {
                            if (*(vector.data() + index) != static_cast<type_t>(raw[index])) {
                                all_values_copied = false;
                                break;
                            }
                        }

                        return std::tuple{
                            // Allocation of 4 i32 should be done
                            vector.data() != nullptr,
                            vector.count(),
                            vector.max_count(),

                            // Ensure it's not the same memory buffer
                            (void*)vector.data() != (void*)&raw[0],

                            // Ensure values are correclty copied
                            all_values_copied,

                            // Ensure we are allocating only one time
                            vector.allocator().allocation_count(),
                            vector.allocator().free_count()
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
                    hud_test::for_each_value(std::make_integer_sequence<usize, 5>(),
                        []<usize extra>() {

                        // Test default allocator with extra
                            {
                                auto test_default_allocator = [](const U* raw, usize element_count) {
                                    hud::vector<type_t> vector(raw, element_count, extra);
                                    bool all_values_copied = true;

                                    // Ensure values are correclty copied
                                    for (usize index = 0; index < element_count; index++) {
                                        if (*(vector.data() + index) != static_cast<type_t>(raw[index])) {
                                            all_values_copied = false;
                                            break;
                                        }
                                    }

                                    return std::tuple{
                                        // Allocation of 4 i32 should be done
                                        vector.data() != nullptr,
                                        vector.count(),
                                        vector.max_count(),

                                        // Ensure it's not the same memory buffer
                                        (void*)vector.data() != (void*)&raw[0],

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
                                    hud::vector<type_t, hud_test::allocator_watcher<alignof(type_t)>> vector(raw, element_count, extra, hud_test::allocator_watcher<alignof(type_t)>{});
                                    bool all_values_copied = true;

                                    // Ensure values are correclty copied
                                    for (usize index = 0; index < element_count; index++) {
                                        if (*(vector.data() + index) != static_cast<type_t>(raw[index])) {
                                            all_values_copied = false;
                                            break;
                                        }
                                    }

                                    return std::tuple{
                                        // Allocation of 4 i32 should be done
                                        vector.data() != nullptr,
                                        vector.count(),
                                        vector.max_count(),

                                        // Ensure it's not the same memory buffer
                                        (void*)vector.data() != (void*)&raw[0],

                                        // Ensure values are correclty copied
                                        all_values_copied,

                                        // Ensure we are allocating only one time
                                        vector.allocator().allocation_count(),
                                        vector.allocator().free_count()
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

GTEST_TEST(vector, construct_by_copying_raw_data_array_of_non_bitwise_copy_constructible_different_type)
{

    using Type1 = hud_test::non_bitwise_copy_constructible_type2;
    using Type2 = hud_test::non_bitwise_copy_constructible_type;

    // test with default allocator no extra
    {
        auto test_default_allocator = [](const Type1 *raw, usize element_count) {
            hud::vector<Type2> vector(raw, element_count);

            // Ensure values are correclty copied in order
            bool all_values_copied_in_order = true;
            for (usize index = 0; index < element_count; index++) {
                if (vector[index].id() != raw[index].id()) {
                    all_values_copied_in_order = false;
                    break;
                }
            }

            // Ensure the copy constructor are called
            bool all_copy_constructor_are_called = true;
            for (usize index = 0; index < element_count; index++) {
                // Ensure we call the copy constructor and the id is correctly copied
                if (vector[index].copy_constructor_count() != 1u || vector[index].id() != raw[index].id()) {
                    all_copy_constructor_are_called = false;
                    break;
                }
            }

            return std::tuple {
                // Allocation of 4 i32 should be done
                vector.data() != nullptr,
                vector.count(),
                vector.max_count(),

                // Ensure it's not the same memory buffer
                (void *)vector.data() != (void *)&raw[0],

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
        auto test_with_allocator = [](const Type1 *raw, usize element_count) {
            hud::vector<Type2, hud_test::allocator_watcher<alignof(Type2)>> vector(raw, element_count, hud_test::allocator_watcher<alignof(Type2)> {});

            // Ensure values are correclty copied in order
            bool all_values_copied_in_order = true;
            for (usize index = 0; index < element_count; index++) {
                if (vector[index].id() != raw[index].id()) {
                    all_values_copied_in_order = false;
                    break;
                }
            }

            // Ensure the copy constructor are called
            bool all_copy_constructor_are_called = true;
            for (usize index = 0; index < element_count; index++) {
                // Ensure we call the copy constructor and the id is correctly copied
                if (vector[index].copy_constructor_count() != 1u || vector[index].id() != raw[index].id()) {
                    all_copy_constructor_are_called = false;
                    break;
                }
            }
            return std::tuple {
                // Allocation of 4 i32 should be done
                vector.data() != nullptr,
                vector.count(),
                vector.max_count(),

                // Ensure it's not the same memory buffer
                vector.data() != &raw[0],

                // Ensure values are correclty copied in order
                all_values_copied_in_order,

                // Ensure the copy constructor are called
                all_copy_constructor_are_called,

                // Ensure we are allocating only one time
                vector.allocator().allocation_count(),
                vector.allocator().free_count()

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
        hud_test::for_each_value(std::make_integer_sequence<usize, 5>(), []<usize extra>() {
                // Test default allcoator with extra
                {
                    auto test_default_allocator = [](const Type1 *raw, usize element_count)
                    {
                        hud::vector<Type2> vector(raw, element_count, extra);

                        // Ensure values are correclty copied in order
                        bool all_values_copied_in_order = true;
                        for (usize index = 0; index < element_count; index++)
                        {
                            if (vector[index].id() != raw[index].id())
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
                            if (vector[index].copy_constructor_count() != 1u || vector[index].id() != raw[index].id())
                            {
                                all_copy_constructor_are_called = false;
                                break;
                            }
                        }

                        return std::tuple {
                            // Allocation of 4 i32 should be done
                            vector.data() != nullptr,
                            vector.count(),
                            vector.max_count(),

                            // Ensure it's not the same memory buffer
                            vector.data() != &raw[0],

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
                        hud::vector<Type2, hud_test::allocator_watcher<alignof(Type2)>> vector(raw, element_count, extra, hud_test::allocator_watcher<alignof(Type2)> {});

                        // Ensure values are correclty copied in order
                        bool all_values_copied_in_order = true;
                        for (usize index = 0; index < element_count; index++)
                        {
                            if (vector[index].id() != raw[index].id())
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
                            if (vector[index].copy_constructor_count() != 1u || vector[index].id() != raw[index].id())
                            {
                                all_copy_constructor_are_called = false;
                                break;
                            }
                        }
                        return std::tuple {
                            // Allocation of 4 i32 should be done
                            vector.data() != nullptr,
                            vector.count(),
                            vector.max_count(),

                            // Ensure it's not the same memory buffer
                            vector.data() != &raw[0],

                            // Ensure values are correclty copied in order
                            all_values_copied_in_order,

                            // Ensure the copy constructor are called
                            all_copy_constructor_are_called,

                            // Ensure we are allocating only one time
                            vector.allocator().allocation_count(),
                            vector.allocator().free_count()

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
                } });
    }
}

GTEST_TEST(vector, construct_with_initializer_list_of_bitwise_copy_constructible_same_type)
{
    using type = i32;

    static_assert(hud::is_bitwise_copy_constructible_v<type>);

    // Test with default allocator and no extra
    {
        auto test_default_allocator = [](std::initializer_list<type> initializer) {
            hud::vector<type> vector(initializer);
            bool all_values_copied = true;

            // Ensure values are correclty copied
            for (usize index = 0; index < initializer.size(); index++) {
                if (*(vector.data() + index) != *(initializer.begin() + index)) {
                    all_values_copied = false;
                    break;
                }
            }

            return std::tuple {
                // Allocation of 4 i32 should be done
                vector.data() != nullptr,
                vector.count(),
                vector.max_count(),

                // Ensure it's not the same memory buffer
                (void *)vector.data() != (void *)initializer.begin(),

                // Ensure values are correclty copied
                all_values_copied
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
        auto test_with_allocator = [](std::initializer_list<type> initializer) {
            hud::vector<type, hud_test::allocator_watcher<alignof(type)>> vector(initializer, hud_test::allocator_watcher<alignof(type)> {});
            bool all_values_copied = true;

            // Ensure values are correclty copied
            for (usize index = 0; index < initializer.size(); index++) {
                if (*(vector.data() + index) != *(initializer.begin() + index)) {
                    all_values_copied = false;
                    break;
                }
            }

            return std::tuple {
                // Allocation of 4 i32 should be done
                vector.data() != nullptr,
                vector.count(),
                vector.max_count(),

                // Ensure it's not the same memory buffer
                vector.data() != initializer.begin(),

                // Ensure values are correclty copied
                all_values_copied,

                // Ensure we are allocating only one time
                vector.allocator().allocation_count(),
                vector.allocator().free_count()
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
        hud_test::for_each_value(std::make_integer_sequence<usize, 5>(), []<usize extra>() {
                // Test default allocator with extra
                {
                    auto test_default_allocator = [](std::initializer_list<type> initializer)
                    {
                        hud::vector<type> vector(initializer, extra);
                        bool all_values_copied = true;

                        // Ensure values are correclty copied
                        for (usize index = 0; index < initializer.size(); index++)
                        {
                            if (*(vector.data() + index) != *(initializer.begin() + index))
                            {
                                all_values_copied = false;
                                break;
                            }
                        }

                        return std::tuple {
                            // Allocation of 4 i32 should be done
                            vector.data() != nullptr,
                            vector.count(),
                            vector.max_count(),

                            // Ensure it's not the same memory buffer
                            vector.data() != initializer.begin(),

                            // Ensure values are correclty copied
                            all_values_copied
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
                        hud::vector<type, hud_test::allocator_watcher<alignof(type)>> vector(initializer, extra, hud_test::allocator_watcher<alignof(type)> {});
                        bool all_values_copied = true;

                        // Ensure values are correclty copied
                        for (usize index = 0; index < initializer.size(); index++)
                        {
                            if (*(vector.data() + index) != *(initializer.begin() + index))
                            {
                                all_values_copied = false;
                                break;
                            }
                        }

                        return std::tuple {
                            // Allocation of 4 i32 should be done
                            vector.data() != nullptr,
                            vector.count(),
                            vector.max_count(),

                            // Ensure it's not the same memory buffer
                            vector.data() != initializer.begin(),

                            // Ensure values are correclty copied
                            all_values_copied,

                            // Ensure we are allocating only one time
                            vector.allocator().allocation_count(),
                            vector.allocator().free_count()
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
                } });
    }
}

GTEST_TEST(vector, construct_with_initializer_list_of_bitwise_copy_constructible_different_type)
{

    using Type1 = i32;
    using Type2 = u32;

    static_assert(!std::is_same_v<Type1, Type2> && hud::is_bitwise_copy_constructible_v<Type2, Type1>);

    // test with default allocator no extra
    {
        auto test_default_allocator = [](std::initializer_list<Type1> initializer) {
            hud::vector<Type2> vector(initializer);
            bool all_values_copied = true;

            // Ensure values are correclty copied
            for (usize index = 0; index < initializer.size(); index++) {
                if (*(vector.data() + index) != static_cast<Type2>(*(initializer.begin() + index))) {
                    all_values_copied = false;
                    break;
                }
            }

            return std::tuple {
                // Allocation of 4 i32 should be done
                vector.data() != nullptr,
                vector.count(),
                vector.max_count(),

                // Ensure it's not the same memory buffer
                (void *)vector.data() != (void *)initializer.begin(),

                // Ensure values are correclty copied
                all_values_copied
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
        auto test_with_allocator = [](std::initializer_list<Type1> initializer) {
            hud::vector<Type2, hud_test::allocator_watcher<alignof(Type2)>> vector(initializer, hud_test::allocator_watcher<alignof(Type2)> {});
            bool all_values_copied = true;

            // Ensure values are correclty copied
            for (usize index = 0; index < initializer.size(); index++) {
                if (*(vector.data() + index) != static_cast<Type2>(*(initializer.begin() + index))) {
                    all_values_copied = false;
                    break;
                }
            }

            return std::tuple {
                // Allocation of 4 i32 should be done
                vector.data() != nullptr,
                vector.count(),
                vector.max_count(),

                // Ensure it's not the same memory buffer
                (void *)vector.data() != (void *)initializer.begin(),

                // Ensure values are correclty copied
                all_values_copied,

                // Ensure we are allocating only one time
                vector.allocator().allocation_count(),
                vector.allocator().free_count()
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
        hud_test::for_each_value(std::make_integer_sequence<usize, 5>(), []<usize extra>() {
                // Test default allocator with extra
                {
                    auto test_default_allocator = [](std::initializer_list<Type1> initializer)
                    {
                        hud::vector<Type2> vector(initializer, extra);
                        bool all_values_copied = true;

                        // Ensure values are correclty copied
                        for (usize index = 0; index < initializer.size(); index++)
                        {
                            if (*(vector.data() + index) != static_cast<Type2>(*(initializer.begin() + index)))
                            {
                                all_values_copied = false;
                                break;
                            }
                        }

                        return std::tuple {
                            // Allocation of 4 i32 should be done
                            vector.data() != nullptr,
                            vector.count(),
                            vector.max_count(),

                            // Ensure it's not the same memory buffer
                            (void *)vector.data() != (void *)initializer.begin(),

                            // Ensure values are correclty copied
                            all_values_copied
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
                        hud::vector<Type2, hud_test::allocator_watcher<alignof(Type2)>> vector(initializer, extra, hud_test::allocator_watcher<alignof(Type2)> {});
                        bool all_values_copied = true;

                        // Ensure values are correclty copied
                        for (usize index = 0; index < initializer.size(); index++)
                        {
                            if (*(vector.data() + index) != static_cast<Type2>(*(initializer.begin() + index)))
                            {
                                all_values_copied = false;
                                break;
                            }
                        }

                        return std::tuple {
                            // Allocation of 4 i32 should be done
                            vector.data() != nullptr,
                            vector.count(),
                            vector.max_count(),

                            // Ensure it's not the same memory buffer
                            (void *)vector.data() != (void *)initializer.begin(),

                            // Ensure values are correclty copied
                            all_values_copied,

                            // Ensure we are allocating only one time
                            vector.allocator().allocation_count(),
                            vector.allocator().free_count()
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
                } });
    }
}

GTEST_TEST(vector, construct_with_initializer_list_of_non_bitwise_copy_constructible_same_type)
{

    using type = hud_test::non_bitwise_copy_constructible_type;
    static_assert(!hud::is_bitwise_copy_constructible_v<type>);

    // test with default allocator no extra
    {
        auto test_default_allocator = [](std::initializer_list<type> initializer) {
            hud::vector<type> vector(initializer);

            // Ensure values are correclty copied in order
            bool all_values_copied_in_order = true;
            for (usize index = 0; index < initializer.size(); index++) {
                if (vector[index].id() != initializer.begin()[index].id()) {
                    all_values_copied_in_order = false;
                    break;
                }
            }

            // Ensure the copy constructor are called
            bool all_copy_constructor_are_called = true;
            for (usize index = 0; index < initializer.size(); index++) {
                // Ensure we call the copy constructor and the id is correctly copied
                if (vector[index].copy_constructor_count() != 1u || vector[index].id() != initializer.begin()[index].id()) {
                    all_copy_constructor_are_called = false;
                    break;
                }
            }

            return std::tuple {
                // Allocation of 4 i32 should be done
                vector.data() != nullptr,
                vector.count(),
                vector.max_count(),

                // Ensure it's not the same memory buffer
                (void *)vector.data() != (void *)initializer.begin(),

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
        auto test_with_allocator = [](std::initializer_list<type> initializer) {
            hud::vector<type, hud_test::allocator_watcher<alignof(type)>> vector(initializer, hud_test::allocator_watcher<alignof(type)> {});

            // Ensure values are correclty copied in order
            bool all_values_copied_in_order = true;
            for (usize index = 0; index < initializer.size(); index++) {
                if (vector[index].id() != initializer.begin()[index].id()) {
                    all_values_copied_in_order = false;
                    break;
                }
            }

            // Ensure the copy constructor are called
            bool all_copy_constructor_are_called = true;
            for (usize index = 0; index < initializer.size(); index++) {
                // Ensure we call the copy constructor and the id is correctly copied
                if (vector[index].copy_constructor_count() != 1u || vector[index].id() != initializer.begin()[index].id()) {
                    all_copy_constructor_are_called = false;
                    break;
                }
            }
            return std::tuple {
                // Allocation of 4 i32 should be done
                vector.data() != nullptr,
                vector.count(),
                vector.max_count(),

                // Ensure it's not the same memory buffer
                (void *)vector.data() != (void *)initializer.begin(),

                // Ensure values are correclty copied in order
                all_values_copied_in_order,

                // Ensure the copy constructor are called
                all_copy_constructor_are_called,

                // Ensure we are allocating only one time
                vector.allocator().allocation_count(),
                vector.allocator().free_count()

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
        hud_test::for_each_value(std::make_integer_sequence<usize, 5>(), []<usize extra>() {
                // Test default allcoator with extra
                {
                    auto test_default_allocator = [](std::initializer_list<type> initializer)
                    {
                        hud::vector<type> vector(initializer, extra);

                        // Ensure values are correclty copied in order
                        bool all_values_copied_in_order = true;
                        for (usize index = 0; index < initializer.size(); index++)
                        {
                            if (vector[index].id() != initializer.begin()[index].id())
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
                            if (vector[index].copy_constructor_count() != 1u || vector[index].id() != initializer.begin()[index].id())
                            {
                                all_copy_constructor_are_called = false;
                                break;
                            }
                        }

                        return std::tuple {
                            // Allocation of 4 i32 should be done
                            vector.data() != nullptr,
                            vector.count(),
                            vector.max_count(),

                            // Ensure it's not the same memory buffer
                            (void *)vector.data() != (void *)initializer.begin(),

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
                        hud::vector<type, hud_test::allocator_watcher<alignof(type)>> vector(initializer, extra, hud_test::allocator_watcher<alignof(type)> {});

                        // Ensure values are correclty copied in order
                        bool all_values_copied_in_order = true;
                        for (usize index = 0; index < initializer.size(); index++)
                        {
                            if (vector[index].id() != initializer.begin()[index].id())
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
                            if (vector[index].copy_constructor_count() != 1u || vector[index].id() != initializer.begin()[index].id())
                            {
                                all_copy_constructor_are_called = false;
                                break;
                            }
                        }
                        return std::tuple {
                            // Allocation of 4 i32 should be done
                            vector.data() != nullptr,
                            vector.count(),
                            vector.max_count(),

                            // Ensure it's not the same memory buffer
                            (void *)vector.data() != (void *)initializer.begin(),

                            // Ensure values are correclty copied in order
                            all_values_copied_in_order,

                            // Ensure the copy constructor are called
                            all_copy_constructor_are_called,

                            // Ensure we are allocating only one time
                            vector.allocator().allocation_count(),
                            vector.allocator().free_count()

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
                } });
    }
}

GTEST_TEST(vector, construct_with_initializer_list_of_non_bitwise_copy_constructible_different_type)
{

    using Type1 = hud_test::non_bitwise_copy_constructible_type;
    using Type2 = hud_test::non_bitwise_copy_constructible_type2;
    static_assert(!std::is_same_v<Type1, Type2> && !hud::is_bitwise_copy_constructible_v<Type2, Type1>);

    // test with default allocator no extra
    {
        auto test_default_allocator = [](std::initializer_list<Type1> initializer) {
            hud::vector<Type2> vector(initializer);

            // Ensure values are correclty copied in order
            bool all_values_copied_in_order = true;
            for (usize index = 0; index < initializer.size(); index++) {
                if (vector[index].id() != initializer.begin()[index].id()) {
                    all_values_copied_in_order = false;
                    break;
                }
            }

            // Ensure the copy constructor are called
            bool all_copy_constructor_are_called = true;
            for (usize index = 0; index < initializer.size(); index++) {
                // Ensure we call the copy constructor and the id is correctly copied
                if (vector[index].copy_constructor_count() != 1u || vector[index].id() != initializer.begin()[index].id()) {
                    all_copy_constructor_are_called = false;
                    break;
                }
            }

            return std::tuple {
                // Allocation of 4 i32 should be done
                vector.data() != nullptr,
                vector.count(),
                vector.max_count(),

                // Ensure it's not the same memory buffer
                (void *)vector.data() != (void *)initializer.begin(),

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
        auto test_with_allocator = [](std::initializer_list<Type1> initializer) {
            hud::vector<Type2, hud_test::allocator_watcher<alignof(Type2)>> vector(initializer, hud_test::allocator_watcher<alignof(Type2)> {});

            // Ensure values are correclty copied in order
            bool all_values_copied_in_order = true;
            for (usize index = 0; index < initializer.size(); index++) {
                if (vector[index].id() != initializer.begin()[index].id()) {
                    all_values_copied_in_order = false;
                    break;
                }
            }

            // Ensure the copy constructor are called
            bool all_copy_constructor_are_called = true;
            for (usize index = 0; index < initializer.size(); index++) {
                // Ensure we call the copy constructor and the id is correctly copied
                if (vector[index].copy_constructor_count() != 1u || vector[index].id() != initializer.begin()[index].id()) {
                    all_copy_constructor_are_called = false;
                    break;
                }
            }
            return std::tuple {
                // Allocation of 4 i32 should be done
                vector.data() != nullptr,
                vector.count(),
                vector.max_count(),

                // Ensure it's not the same memory buffer
                (void *)vector.data() != (void *)initializer.begin(),

                // Ensure values are correclty copied in order
                all_values_copied_in_order,

                // Ensure the copy constructor are called
                all_copy_constructor_are_called,

                // Ensure we are allocating only one time
                vector.allocator().allocation_count(),
                vector.allocator().free_count()

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
        hud_test::for_each_value(std::make_integer_sequence<usize, 5>(), []<usize extra>() {
                // Test default allcoator with extra
                {
                    auto test_default_allocator = [](std::initializer_list<Type1> initializer)
                    {
                        hud::vector<Type2> vector(initializer, extra);

                        // Ensure values are correclty copied in order
                        bool all_values_copied_in_order = true;
                        for (usize index = 0; index < initializer.size(); index++)
                        {
                            if (vector[index].id() != initializer.begin()[index].id())
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
                            if (vector[index].copy_constructor_count() != 1u || vector[index].id() != initializer.begin()[index].id())
                            {
                                all_copy_constructor_are_called = false;
                                break;
                            }
                        }

                        return std::tuple {
                            // Allocation of 4 i32 should be done
                            vector.data() != nullptr,
                            vector.count(),
                            vector.max_count(),

                            // Ensure it's not the same memory buffer
                            (void *)vector.data() != (void *)initializer.begin(),

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
                        hud::vector<Type2, hud_test::allocator_watcher<alignof(Type2)>> vector(initializer, extra, hud_test::allocator_watcher<alignof(Type2)> {});

                        // Ensure values are correclty copied in order
                        bool all_values_copied_in_order = true;
                        for (usize index = 0; index < initializer.size(); index++)
                        {
                            if (vector[index].id() != initializer.begin()[index].id())
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
                            if (vector[index].copy_constructor_count() != 1u || vector[index].id() != initializer.begin()[index].id())
                            {
                                all_copy_constructor_are_called = false;
                                break;
                            }
                        }
                        return std::tuple {
                            // Allocation of 4 i32 should be done
                            vector.data() != nullptr,
                            vector.count(),
                            vector.max_count(),

                            // Ensure it's not the same memory buffer
                            (void *)vector.data() != (void *)initializer.begin(),

                            // Ensure values are correclty copied in order
                            all_values_copied_in_order,

                            // Ensure the copy constructor are called
                            all_copy_constructor_are_called,

                            // Ensure we are allocating only one time
                            vector.allocator().allocation_count(),
                            vector.allocator().free_count()

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
                } });
    }
}