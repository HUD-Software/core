#include <core/containers/vector.h>
#include "../misc/allocator_watcher.h"

GTEST_TEST(vector, resize_empty_to_zero_do_nothing_with_non_trivial_type)
{

    using type = hud_test::non_bitwise_type;
    using array_type = hud::vector<type, hud_test::allocator_watcher<alignof(type)>>;

    array_type vector;
    hud_assert_eq(vector.data(), nullptr);
    hud_assert_eq(vector.count(), 0u);
    hud_assert_eq(vector.max_count(), 0u);
    hud_assert_eq(vector.allocator().allocation_count(), 0u);
    hud_assert_eq(vector.allocator().free_count(), 0u);

    // Resize 0 do nothing
    vector.resize(0);
    hud_assert_eq(vector.data(), nullptr);
    hud_assert_eq(vector.count(), 0u);
    hud_assert_eq(vector.max_count(), 0u);
    hud_assert_eq(vector.allocator().allocation_count(), 0u);
    hud_assert_eq(vector.allocator().free_count(), 0u);
}

GTEST_TEST(vector, resize_empty_to_zero_do_nothing_with_trivial_type)
{

    using type = usize;
    using array_type = hud::vector<type, hud_test::allocator_watcher<alignof(type)>>;

    array_type vector;
    hud_assert_eq(vector.data(), nullptr);
    hud_assert_eq(vector.count(), 0u);
    hud_assert_eq(vector.max_count(), 0u);
    hud_assert_eq(vector.allocator().allocation_count(), 0u);
    hud_assert_eq(vector.allocator().free_count(), 0u);

    // Resize 0 do nothing
    vector.resize(0);
    hud_assert_eq(vector.data(), nullptr);
    hud_assert_eq(vector.count(), 0u);
    hud_assert_eq(vector.max_count(), 0u);
    hud_assert_eq(vector.allocator().allocation_count(), 0u);
    hud_assert_eq(vector.allocator().free_count(), 0u);
}

GTEST_TEST(vector, resize_empty_to_more_size_non_trivial_type)
{

    using type = hud_test::non_bitwise_type;
    using array_type = hud::vector<type, hud_test::allocator_watcher<alignof(type)>>;

    array_type vector;
    hud_assert_eq(vector.data(), nullptr);
    hud_assert_eq(vector.count(), 0u);
    hud_assert_eq(vector.max_count(), 0u);
    hud_assert_eq(vector.allocator().allocation_count(), 0u);
    hud_assert_eq(vector.allocator().free_count(), 0u);

    vector.resize(2);
    hud_assert_ne(vector.data(), nullptr);
    hud_assert_eq(vector.count(), 2u);
    hud_assert_eq(vector.max_count(), 2u);
    hud_assert_eq(vector.allocator().allocation_count(), 1u);
    hud_assert_eq(vector.allocator().free_count(), 0u);

    // Should default construct
    for (const type &element : vector) {
        hud_assert_eq(element.id(), 0);
        hud_assert_eq(element.destructor_counter(), nullptr);
        hud_assert_eq(element.constructor_count(), 0u);
        hud_assert_eq(element.copy_constructor_count(), 0u);
        hud_assert_eq(element.move_constructor_count(), 0u);
        hud_assert_eq(element.copy_assign_count(), 0u);
        hud_assert_eq(element.move_assign_count(), 0u);
    }
}

GTEST_TEST(vector, resize_empty_to_more_size_trivial_type)
{

    using type = usize;
    using array_type = hud::vector<type, hud_test::allocator_watcher<alignof(type)>>;

    array_type vector;
    hud_assert_eq(vector.data(), nullptr);
    hud_assert_eq(vector.count(), 0u);
    hud_assert_eq(vector.max_count(), 0u);
    hud_assert_eq(vector.allocator().allocation_count(), 0u);
    hud_assert_eq(vector.allocator().free_count(), 0u);

    vector.resize(2);
    hud_assert_ne(vector.data(), nullptr);
    hud_assert_eq(vector.count(), 2u);
    hud_assert_eq(vector.max_count(), 2u);
    hud_assert_eq(vector.allocator().allocation_count(), 1u);
    hud_assert_eq(vector.allocator().free_count(), 0u);

    // Should default construct
    for (const type &element : vector) {
        hud_assert_eq(element, type());
    }
}

GTEST_TEST(vector, resize_non_empty_to_more_size_non_trivial_type)
{

    using type = hud_test::non_bitwise_type;
    using array_type = hud::vector<type, hud_test::allocator_watcher<alignof(type)>>;

    array_type vector;
    hud_assert_eq(vector.data(), nullptr);
    hud_assert_eq(vector.count(), 0u);
    hud_assert_eq(vector.max_count(), 0u);
    hud_assert_eq(vector.allocator().allocation_count(), 0u);
    hud_assert_eq(vector.allocator().free_count(), 0u);

    vector.resize(2);
    hud_assert_ne(vector.data(), nullptr);
    hud_assert_eq(vector.count(), 2u);
    hud_assert_eq(vector.max_count(), 2u);
    hud_assert_eq(vector.allocator().allocation_count(), 1u);
    hud_assert_eq(vector.allocator().free_count(), 0u);

    // Should default construct
    for (const type &element : vector) {
        hud_assert_eq(element.id(), 0);
        hud_assert_eq(element.destructor_counter(), nullptr);
        hud_assert_eq(element.constructor_count(), 0u);
        hud_assert_eq(element.copy_constructor_count(), 0u);
        hud_assert_eq(element.move_constructor_count(), 0u);
        hud_assert_eq(element.copy_assign_count(), 0u);
        hud_assert_eq(element.move_assign_count(), 0u);
    }

    vector.resize(4);
    // Should move already present
    for (usize index = 0; index < 2; index++) {
        hud_assert_eq(vector[index].id(), 0);
        hud_assert_eq(vector[index].destructor_counter(), nullptr);
        hud_assert_eq(vector[index].constructor_count(), 0u);
        hud_assert_eq(vector[index].copy_constructor_count(), 0u);
        hud_assert_eq(vector[index].move_constructor_count(), 1u);
        hud_assert_eq(vector[index].copy_assign_count(), 0u);
        hud_assert_eq(vector[index].move_assign_count(), 0u);
    }
    // Should default construct newly added
    for (usize index = 2; index < 4; index++) {
        hud_assert_eq(vector[index].id(), 0);
        hud_assert_eq(vector[index].destructor_counter(), nullptr);
        hud_assert_eq(vector[index].constructor_count(), 0u);
        hud_assert_eq(vector[index].copy_constructor_count(), 0u);
        hud_assert_eq(vector[index].move_constructor_count(), 0u);
        hud_assert_eq(vector[index].copy_assign_count(), 0u);
        hud_assert_eq(vector[index].move_assign_count(), 0u);
    }
}

GTEST_TEST(vector, resize_non_empty_to_more_size_trivial_type)
{

    using type = usize;
    using array_type = hud::vector<type, hud_test::allocator_watcher<alignof(type)>>;

    array_type vector;
    hud_assert_eq(vector.data(), nullptr);
    hud_assert_eq(vector.count(), 0u);
    hud_assert_eq(vector.max_count(), 0u);
    hud_assert_eq(vector.allocator().allocation_count(), 0u);
    hud_assert_eq(vector.allocator().free_count(), 0u);

    vector.resize(2);
    hud_assert_ne(vector.data(), nullptr);
    hud_assert_eq(vector.count(), 2u);
    hud_assert_eq(vector.max_count(), 2u);
    hud_assert_eq(vector.allocator().allocation_count(), 1u);
    hud_assert_eq(vector.allocator().free_count(), 0u);

    // Should default construct
    for (const type &element : vector) {
        hud_assert_eq(element, type());
    }

    // Change value to ensure resize keep values
    vector[0] = type(10);
    vector[1] = type(20);

    vector.resize(4);

    // Should not modify already present element
    hud_assert_eq(vector[0], type(10));
    hud_assert_eq(vector[1], type(20));

    // Should default construct new element
    hud_assert_eq(vector[2], type());
    hud_assert_eq(vector[3], type());
}

GTEST_TEST(vector, resize_non_empty_to_same_size_do_nothing_with_non_trivial_type)
{

    using type = hud_test::non_bitwise_type;
    using array_type = hud::vector<type, hud_test::allocator_watcher<alignof(type)>>;

    array_type vector;
    hud_assert_eq(vector.data(), nullptr);
    hud_assert_eq(vector.count(), 0u);
    hud_assert_eq(vector.max_count(), 0u);
    hud_assert_eq(vector.allocator().allocation_count(), 0u);
    hud_assert_eq(vector.allocator().free_count(), 0u);

    vector.resize(2);
    hud_assert_ne(vector.data(), nullptr);
    hud_assert_eq(vector.count(), 2u);
    hud_assert_eq(vector.max_count(), 2u);
    hud_assert_eq(vector.allocator().allocation_count(), 1u);
    hud_assert_eq(vector.allocator().free_count(), 0u);

    // Should default construct
    for (const type &element : vector) {
        hud_assert_eq(element.id(), 0);
        hud_assert_eq(element.destructor_counter(), nullptr);
        hud_assert_eq(element.constructor_count(), 0u);
        hud_assert_eq(element.copy_constructor_count(), 0u);
        hud_assert_eq(element.move_constructor_count(), 0u);
        hud_assert_eq(element.copy_assign_count(), 0u);
        hud_assert_eq(element.move_assign_count(), 0u);
    }

    // resize same size as count do nothing
    vector.resize(2);
    hud_assert_ne(vector.data(), nullptr);
    hud_assert_eq(vector.count(), 2u);
    hud_assert_eq(vector.max_count(), 2u);
    hud_assert_eq(vector.allocator().allocation_count(), 1u);
    hud_assert_eq(vector.allocator().free_count(), 0u);

    // Should default construct
    for (const type &element : vector) {
        hud_assert_eq(element.id(), 0);
        hud_assert_eq(element.destructor_counter(), nullptr);
        hud_assert_eq(element.constructor_count(), 0u);
        hud_assert_eq(element.copy_constructor_count(), 0u);
        hud_assert_eq(element.move_constructor_count(), 0u);
        hud_assert_eq(element.copy_assign_count(), 0u);
        hud_assert_eq(element.move_assign_count(), 0u);
    }
}

GTEST_TEST(vector, resize_non_empty_to_same_size_do_nothing_with_trivial_type)
{

    using type = usize;
    using array_type = hud::vector<type, hud_test::allocator_watcher<alignof(type)>>;

    array_type vector;
    hud_assert_eq(vector.data(), nullptr);
    hud_assert_eq(vector.count(), 0u);
    hud_assert_eq(vector.max_count(), 0u);
    hud_assert_eq(vector.allocator().allocation_count(), 0u);
    hud_assert_eq(vector.allocator().free_count(), 0u);

    vector.resize(2);
    hud_assert_ne(vector.data(), nullptr);
    hud_assert_eq(vector.count(), 2u);
    hud_assert_eq(vector.max_count(), 2u);
    hud_assert_eq(vector.allocator().allocation_count(), 1u);
    hud_assert_eq(vector.allocator().free_count(), 0u);

    // Should default construct
    for (const type &element : vector) {
        hud_assert_eq(element, type());
    }
    // Change value to ensure resize keep values
    vector[0] = type(10);
    vector[1] = type(20);

    // resize same size as count do nothing
    vector.resize(2);
    hud_assert_ne(vector.data(), nullptr);
    hud_assert_eq(vector.count(), 2u);
    hud_assert_eq(vector.max_count(), 2u);
    hud_assert_eq(vector.allocator().allocation_count(), 1u);
    hud_assert_eq(vector.allocator().free_count(), 0u);

    // Should not modify already present element
    hud_assert_eq(vector[0], type(10));
    hud_assert_eq(vector[1], type(20));
}

GTEST_TEST(vector, resize_when_enough_memory_do_not_allocate_but_construct_in_place_with_non_trivial_type)
{

    using type = hud_test::non_bitwise_type;
    using array_type = hud::vector<type, hud_test::allocator_watcher<alignof(type)>>;

    array_type vector;
    hud_assert_eq(vector.data(), nullptr);
    hud_assert_eq(vector.count(), 0u);
    hud_assert_eq(vector.max_count(), 0u);
    hud_assert_eq(vector.allocator().allocation_count(), 0u);
    hud_assert_eq(vector.allocator().free_count(), 0u);

    vector.resize(2);
    hud_assert_ne(vector.data(), nullptr);
    hud_assert_eq(vector.count(), 2u);
    hud_assert_eq(vector.max_count(), 2u);
    hud_assert_eq(vector.allocator().allocation_count(), 1u);
    hud_assert_eq(vector.allocator().free_count(), 0u);

    // Should default construct
    for (const type &element : vector) {
        hud_assert_eq(element.id(), 0);
        hud_assert_eq(element.destructor_counter(), nullptr);
        hud_assert_eq(element.constructor_count(), 0u);
        hud_assert_eq(element.copy_constructor_count(), 0u);
        hud_assert_eq(element.move_constructor_count(), 0u);
        hud_assert_eq(element.copy_assign_count(), 0u);
        hud_assert_eq(element.move_assign_count(), 0u);
    }

    // reserve to be sure we have memory
    vector.reserve(4);
    hud_assert_ne(vector.data(), nullptr);
    hud_assert_eq(vector.count(), 2u);
    hud_assert_eq(vector.max_count(), 4u);
    hud_assert_eq(vector.allocator().allocation_count(), 2u);
    hud_assert_eq(vector.allocator().free_count(), 1u);

    // Should move element to new memory
    for (const type &element : vector) {
        hud_assert_eq(element.id(), 0);
        hud_assert_eq(element.destructor_counter(), nullptr);
        hud_assert_eq(element.constructor_count(), 0u);
        hud_assert_eq(element.copy_constructor_count(), 0u);
        hud_assert_eq(element.move_constructor_count(), 1u);
        hud_assert_eq(element.copy_assign_count(), 0u);
        hud_assert_eq(element.move_assign_count(), 0u);
    }

    vector.resize(4);
    hud_assert_ne(vector.data(), nullptr);
    hud_assert_eq(vector.count(), 4u);
    hud_assert_eq(vector.max_count(), 4u);
    hud_assert_eq(vector.allocator().allocation_count(), 2u);
    hud_assert_eq(vector.allocator().free_count(), 1u);
    // Should not touch already present elements
    for (usize index = 0; index < 2; index++) {
        hud_assert_eq(vector[index].id(), 0);
        hud_assert_eq(vector[index].destructor_counter(), nullptr);
        hud_assert_eq(vector[index].constructor_count(), 0u);
        hud_assert_eq(vector[index].copy_constructor_count(), 0u);
        hud_assert_eq(vector[index].move_constructor_count(), 1u);
        hud_assert_eq(vector[index].copy_assign_count(), 0u);
        hud_assert_eq(vector[index].move_assign_count(), 0u);
    }
    // Should default construct newnly added element
    for (usize index = 2; index < 4; index++) {
        hud_assert_eq(vector[index].id(), 0);
        hud_assert_eq(vector[index].destructor_counter(), nullptr);
        hud_assert_eq(vector[index].constructor_count(), 0u);
        hud_assert_eq(vector[index].copy_constructor_count(), 0u);
        hud_assert_eq(vector[index].move_constructor_count(), 0u);
        hud_assert_eq(vector[index].copy_assign_count(), 0u);
        hud_assert_eq(vector[index].move_assign_count(), 0u);
    }
}

GTEST_TEST(vector, resize_when_enough_memory_do_not_allocate_but_construct_in_place_with_trivial_type)
{

    using type = usize;
    using array_type = hud::vector<type, hud_test::allocator_watcher<alignof(type)>>;

    array_type vector;
    hud_assert_eq(vector.data(), nullptr);
    hud_assert_eq(vector.count(), 0u);
    hud_assert_eq(vector.max_count(), 0u);
    hud_assert_eq(vector.allocator().allocation_count(), 0u);
    hud_assert_eq(vector.allocator().free_count(), 0u);

    vector.resize(2);
    hud_assert_ne(vector.data(), nullptr);
    hud_assert_eq(vector.count(), 2u);
    hud_assert_eq(vector.max_count(), 2u);
    hud_assert_eq(vector.allocator().allocation_count(), 1u);
    hud_assert_eq(vector.allocator().free_count(), 0u);

    // Should default construct
    for (const type &element : vector) {
        hud_assert_eq(element, type());
    }
    // Change value to ensure resize keep values
    vector[0] = type(10);
    vector[1] = type(20);

    // reserve to be sure we have memory
    vector.reserve(4);
    hud_assert_ne(vector.data(), nullptr);
    hud_assert_eq(vector.count(), 2u);
    hud_assert_eq(vector.max_count(), 4u);
    hud_assert_eq(vector.allocator().allocation_count(), 2u);
    hud_assert_eq(vector.allocator().free_count(), 1u);

    // Reserve should not change the values
    vector[0] = type(10);
    vector[1] = type(20);

    vector.resize(4);
    hud_assert_ne(vector.data(), nullptr);
    hud_assert_eq(vector.count(), 4u);
    hud_assert_eq(vector.max_count(), 4u);
    hud_assert_eq(vector.allocator().allocation_count(), 2u);
    hud_assert_eq(vector.allocator().free_count(), 1u);

    // Should not modify already present element
    hud_assert_eq(vector[0], type(10));
    hud_assert_eq(vector[1], type(20));

    // Should default construct new element
    hud_assert_eq(vector[2], type());
    hud_assert_eq(vector[3], type());
}

GTEST_TEST(vector, resize_less_than_count_destroy_elements_but_not_reallocate_with_non_trivial_type)
{

    using type = hud_test::non_bitwise_type;
    using array_type = hud::vector<type, hud_test::allocator_watcher<alignof(type)>>;

    array_type vector;
    hud_assert_eq(vector.data(), nullptr);
    hud_assert_eq(vector.count(), 0u);
    hud_assert_eq(vector.max_count(), 0u);
    hud_assert_eq(vector.allocator().allocation_count(), 0u);
    hud_assert_eq(vector.allocator().free_count(), 0u);

    vector.resize(2);
    hud_assert_ne(vector.data(), nullptr);
    hud_assert_eq(vector.count(), 2u);
    hud_assert_eq(vector.max_count(), 2u);
    hud_assert_eq(vector.allocator().allocation_count(), 1u);
    hud_assert_eq(vector.allocator().free_count(), 0u);

    // Should default construct
    for (const type &element : vector) {
        hud_assert_eq(element.id(), 0);
        hud_assert_eq(element.destructor_counter(), nullptr);
        hud_assert_eq(element.constructor_count(), 0u);
        hud_assert_eq(element.copy_constructor_count(), 0u);
        hud_assert_eq(element.move_constructor_count(), 0u);
        hud_assert_eq(element.copy_assign_count(), 0u);
        hud_assert_eq(element.move_assign_count(), 0u);
    }

    // Should destroy the 2nd element but not touch the 1st
    i32 destructor_called = 0;
    vector[1].set_dtor_counter_ptr(&destructor_called);
    vector.resize(1);
    hud_assert_ne(vector.data(), nullptr);
    hud_assert_eq(vector.count(), 1u);
    hud_assert_eq(vector.max_count(), 2u);
    hud_assert_eq(vector.allocator().allocation_count(), 1u);
    hud_assert_eq(vector.allocator().free_count(), 0u);

    // Should not touch already present elements
    hud_assert_eq(vector[0].id(), 0);
    hud_assert_eq(vector[0].destructor_counter(), nullptr);
    hud_assert_eq(vector[0].constructor_count(), 0u);
    hud_assert_eq(vector[0].copy_constructor_count(), 0u);
    hud_assert_eq(vector[0].move_constructor_count(), 0u);
    hud_assert_eq(vector[0].copy_assign_count(), 0u);
    hud_assert_eq(vector[0].move_assign_count(), 0u);

    hud_assert_true(destructor_called);
}

GTEST_TEST(vector, resize_less_than_count_destroy_elements_but_not_reallocate_with_trivial_type)
{

    using type = usize;
    using array_type = hud::vector<type, hud_test::allocator_watcher<alignof(type)>>;

    array_type vector;
    hud_assert_eq(vector.data(), nullptr);
    hud_assert_eq(vector.count(), 0u);
    hud_assert_eq(vector.max_count(), 0u);
    hud_assert_eq(vector.allocator().allocation_count(), 0u);
    hud_assert_eq(vector.allocator().free_count(), 0u);

    vector.resize(2);
    hud_assert_ne(vector.data(), nullptr);
    hud_assert_eq(vector.count(), 2u);
    hud_assert_eq(vector.max_count(), 2u);
    hud_assert_eq(vector.allocator().allocation_count(), 1u);
    hud_assert_eq(vector.allocator().free_count(), 0u);

    // Should default construct
    for (const type &element : vector) {
        hud_assert_eq(element, type());
    }
    // Change values to check the element at index 1 is correctly destroy while index 0 is not modify
    vector[0] = type(10);
    vector[1] = type(20);

    // Should destroy the 2nd element but not touch the 1st
    vector.resize(1);
    hud_assert_ne(vector.data(), nullptr);
    hud_assert_eq(vector.count(), 1u);
    hud_assert_eq(vector.max_count(), 2u);
    hud_assert_eq(vector.allocator().allocation_count(), 1u);
    hud_assert_eq(vector.allocator().free_count(), 0u);

    hud_assert_eq(vector[0], type(10));
}