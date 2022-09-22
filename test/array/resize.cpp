#include <core/containers/array.h>
#include "allocators.h"

TEST(array, resize_empty_to_zero_do_nothing_with_non_trivial_type)
{

    using type = hud_test::non_bitwise_type;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    array_type array;
    ASSERT_EQ(array.data(), nullptr);
    ASSERT_EQ(array.count(), 0u);
    ASSERT_EQ(array.max_count(), 0u);
    ASSERT_EQ(array.allocator().allocation_count(), 0u);
    ASSERT_EQ(array.allocator().free_count(), 0u);

    //Resize 0 do nothing
    array.resize(0);
    ASSERT_EQ(array.data(), nullptr);
    ASSERT_EQ(array.count(), 0u);
    ASSERT_EQ(array.max_count(), 0u);
    ASSERT_EQ(array.allocator().allocation_count(), 0u);
    ASSERT_EQ(array.allocator().free_count(), 0u);
}

TEST(array, resize_empty_to_zero_do_nothing_with_trivial_type)
{

    using type = usize;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    array_type array;
    ASSERT_EQ(array.data(), nullptr);
    ASSERT_EQ(array.count(), 0u);
    ASSERT_EQ(array.max_count(), 0u);
    ASSERT_EQ(array.allocator().allocation_count(), 0u);
    ASSERT_EQ(array.allocator().free_count(), 0u);

    //Resize 0 do nothing
    array.resize(0);
    ASSERT_EQ(array.data(), nullptr);
    ASSERT_EQ(array.count(), 0u);
    ASSERT_EQ(array.max_count(), 0u);
    ASSERT_EQ(array.allocator().allocation_count(), 0u);
    ASSERT_EQ(array.allocator().free_count(), 0u);
}

TEST(array, resize_empty_to_more_size_non_trivial_type)
{

    using type = hud_test::non_bitwise_type;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    array_type array;
    ASSERT_EQ(array.data(), nullptr);
    ASSERT_EQ(array.count(), 0u);
    ASSERT_EQ(array.max_count(), 0u);
    ASSERT_EQ(array.allocator().allocation_count(), 0u);
    ASSERT_EQ(array.allocator().free_count(), 0u);

    array.resize(2);
    ASSERT_NE(array.data(), nullptr);
    ASSERT_EQ(array.count(), 2u);
    ASSERT_EQ(array.max_count(), 2u);
    ASSERT_EQ(array.allocator().allocation_count(), 1u);
    ASSERT_EQ(array.allocator().free_count(), 0u);

    // Should default construct
    for (const type& element : array) {
        ASSERT_EQ(element.id(), 0);
        ASSERT_EQ(element.destructor_counter(), nullptr);
        ASSERT_EQ(element.constructor_count(), 0u);
        ASSERT_EQ(element.copy_constructor_count(), 0u);
        ASSERT_EQ(element.move_constructor_count(), 0u);
        ASSERT_EQ(element.copy_assign_count(), 0u);
        ASSERT_EQ(element.move_assign_count(), 0u);
    }
}

TEST(array, resize_empty_to_more_size_trivial_type)
{

    using type = usize;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    array_type array;
    ASSERT_EQ(array.data(), nullptr);
    ASSERT_EQ(array.count(), 0u);
    ASSERT_EQ(array.max_count(), 0u);
    ASSERT_EQ(array.allocator().allocation_count(), 0u);
    ASSERT_EQ(array.allocator().free_count(), 0u);

    array.resize(2);
    ASSERT_NE(array.data(), nullptr);
    ASSERT_EQ(array.count(), 2u);
    ASSERT_EQ(array.max_count(), 2u);
    ASSERT_EQ(array.allocator().allocation_count(), 1u);
    ASSERT_EQ(array.allocator().free_count(), 0u);

    // Should default construct
    for (const type& element : array) {
        ASSERT_EQ(element, type());
    }
}

TEST(array, resize_non_empty_to_more_size_non_trivial_type)
{

    using type = hud_test::non_bitwise_type;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    array_type array;
    ASSERT_EQ(array.data(), nullptr);
    ASSERT_EQ(array.count(), 0u);
    ASSERT_EQ(array.max_count(), 0u);
    ASSERT_EQ(array.allocator().allocation_count(), 0u);
    ASSERT_EQ(array.allocator().free_count(), 0u);

    array.resize(2);
    ASSERT_NE(array.data(), nullptr);
    ASSERT_EQ(array.count(), 2u);
    ASSERT_EQ(array.max_count(), 2u);
    ASSERT_EQ(array.allocator().allocation_count(), 1u);
    ASSERT_EQ(array.allocator().free_count(), 0u);

    // Should default construct
    for (const type& element : array) {
        ASSERT_EQ(element.id(), 0);
        ASSERT_EQ(element.destructor_counter(), nullptr);
        ASSERT_EQ(element.constructor_count(), 0u);
        ASSERT_EQ(element.copy_constructor_count(), 0u);
        ASSERT_EQ(element.move_constructor_count(), 0u);
        ASSERT_EQ(element.copy_assign_count(), 0u);
        ASSERT_EQ(element.move_assign_count(), 0u);
    }

    array.resize(4);
    // Should move already present
    for (usize index = 0; index < 2; index++) {
        ASSERT_EQ(array[index].id(), 0);
        ASSERT_EQ(array[index].destructor_counter(), nullptr);
        ASSERT_EQ(array[index].constructor_count(), 0u);
        ASSERT_EQ(array[index].copy_constructor_count(), 0u);
        ASSERT_EQ(array[index].move_constructor_count(), 1u);
        ASSERT_EQ(array[index].copy_assign_count(), 0u);
        ASSERT_EQ(array[index].move_assign_count(), 0u);
    }
    // Should default construct newly added
    for (usize index = 2; index < 4; index++) {
        ASSERT_EQ(array[index].id(), 0);
        ASSERT_EQ(array[index].destructor_counter(), nullptr);
        ASSERT_EQ(array[index].constructor_count(), 0u);
        ASSERT_EQ(array[index].copy_constructor_count(), 0u);
        ASSERT_EQ(array[index].move_constructor_count(), 0u);
        ASSERT_EQ(array[index].copy_assign_count(), 0u);
        ASSERT_EQ(array[index].move_assign_count(), 0u);
    }
}

TEST(array, resize_non_empty_to_more_size_trivial_type)
{

    using type = usize;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    array_type array;
    ASSERT_EQ(array.data(), nullptr);
    ASSERT_EQ(array.count(), 0u);
    ASSERT_EQ(array.max_count(), 0u);
    ASSERT_EQ(array.allocator().allocation_count(), 0u);
    ASSERT_EQ(array.allocator().free_count(), 0u);

    array.resize(2);
    ASSERT_NE(array.data(), nullptr);
    ASSERT_EQ(array.count(), 2u);
    ASSERT_EQ(array.max_count(), 2u);
    ASSERT_EQ(array.allocator().allocation_count(), 1u);
    ASSERT_EQ(array.allocator().free_count(), 0u);

    // Should default construct
    for (const type& element : array) {
        ASSERT_EQ(element, type());
    }

    // Change value to ensure resize keep values
    array[0] = type(10);
    array[1] = type(20);

    array.resize(4);

    // Should not modify already present element
    ASSERT_EQ(array[0], type(10));
    ASSERT_EQ(array[1], type(20));

    // Should default construct new element
    ASSERT_EQ(array[2], type());
    ASSERT_EQ(array[3], type());

}

TEST(array, resize_non_empty_to_same_size_do_nothing_with_non_trivial_type)
{

    using type = hud_test::non_bitwise_type;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    array_type array;
    ASSERT_EQ(array.data(), nullptr);
    ASSERT_EQ(array.count(), 0u);
    ASSERT_EQ(array.max_count(), 0u);
    ASSERT_EQ(array.allocator().allocation_count(), 0u);
    ASSERT_EQ(array.allocator().free_count(), 0u);

    array.resize(2);
    ASSERT_NE(array.data(), nullptr);
    ASSERT_EQ(array.count(), 2u);
    ASSERT_EQ(array.max_count(), 2u);
    ASSERT_EQ(array.allocator().allocation_count(), 1u);
    ASSERT_EQ(array.allocator().free_count(), 0u);

    // Should default construct
    for (const type& element : array) {
        ASSERT_EQ(element.id(), 0);
        ASSERT_EQ(element.destructor_counter(), nullptr);
        ASSERT_EQ(element.constructor_count(), 0u);
        ASSERT_EQ(element.copy_constructor_count(), 0u);
        ASSERT_EQ(element.move_constructor_count(), 0u);
        ASSERT_EQ(element.copy_assign_count(), 0u);
        ASSERT_EQ(element.move_assign_count(), 0u);
    }

    // resize same size as count do nothing
    array.resize(2);
    ASSERT_NE(array.data(), nullptr);
    ASSERT_EQ(array.count(), 2u);
    ASSERT_EQ(array.max_count(), 2u);
    ASSERT_EQ(array.allocator().allocation_count(), 1u);
    ASSERT_EQ(array.allocator().free_count(), 0u);

    // Should default construct
    for (const type& element : array) {
        ASSERT_EQ(element.id(), 0);
        ASSERT_EQ(element.destructor_counter(), nullptr);
        ASSERT_EQ(element.constructor_count(), 0u);
        ASSERT_EQ(element.copy_constructor_count(), 0u);
        ASSERT_EQ(element.move_constructor_count(), 0u);
        ASSERT_EQ(element.copy_assign_count(), 0u);
        ASSERT_EQ(element.move_assign_count(), 0u);
    }
}

TEST(array, resize_non_empty_to_same_size_do_nothing_with_trivial_type)
{

    using type = usize;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    array_type array;
    ASSERT_EQ(array.data(), nullptr);
    ASSERT_EQ(array.count(), 0u);
    ASSERT_EQ(array.max_count(), 0u);
    ASSERT_EQ(array.allocator().allocation_count(), 0u);
    ASSERT_EQ(array.allocator().free_count(), 0u);

    array.resize(2);
    ASSERT_NE(array.data(), nullptr);
    ASSERT_EQ(array.count(), 2u);
    ASSERT_EQ(array.max_count(), 2u);
    ASSERT_EQ(array.allocator().allocation_count(), 1u);
    ASSERT_EQ(array.allocator().free_count(), 0u);

    // Should default construct
    for (const type& element : array) {
        ASSERT_EQ(element, type());
    }
    // Change value to ensure resize keep values
    array[0] = type(10);
    array[1] = type(20);

    // resize same size as count do nothing
    array.resize(2);
    ASSERT_NE(array.data(), nullptr);
    ASSERT_EQ(array.count(), 2u);
    ASSERT_EQ(array.max_count(), 2u);
    ASSERT_EQ(array.allocator().allocation_count(), 1u);
    ASSERT_EQ(array.allocator().free_count(), 0u);

    // Should not modify already present element
    ASSERT_EQ(array[0], type(10));
    ASSERT_EQ(array[1], type(20));
}

TEST(array, resize_when_enough_memory_do_not_allocate_but_construct_in_place_with_non_trivial_type)
{

    using type = hud_test::non_bitwise_type;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    array_type array;
    ASSERT_EQ(array.data(), nullptr);
    ASSERT_EQ(array.count(), 0u);
    ASSERT_EQ(array.max_count(), 0u);
    ASSERT_EQ(array.allocator().allocation_count(), 0u);
    ASSERT_EQ(array.allocator().free_count(), 0u);

    array.resize(2);
    ASSERT_NE(array.data(), nullptr);
    ASSERT_EQ(array.count(), 2u);
    ASSERT_EQ(array.max_count(), 2u);
    ASSERT_EQ(array.allocator().allocation_count(), 1u);
    ASSERT_EQ(array.allocator().free_count(), 0u);

    // Should default construct
    for (const type& element : array) {
        ASSERT_EQ(element.id(), 0);
        ASSERT_EQ(element.destructor_counter(), nullptr);
        ASSERT_EQ(element.constructor_count(), 0u);
        ASSERT_EQ(element.copy_constructor_count(), 0u);
        ASSERT_EQ(element.move_constructor_count(), 0u);
        ASSERT_EQ(element.copy_assign_count(), 0u);
        ASSERT_EQ(element.move_assign_count(), 0u);
    }

    // reserve to be sure we have memory
    array.reserve(4);
    ASSERT_NE(array.data(), nullptr);
    ASSERT_EQ(array.count(), 2u);
    ASSERT_EQ(array.max_count(), 4u);
    ASSERT_EQ(array.allocator().allocation_count(), 2u);
    ASSERT_EQ(array.allocator().free_count(), 1u);

    // Should move element to new memory
    for (const type& element : array) {
        ASSERT_EQ(element.id(), 0);
        ASSERT_EQ(element.destructor_counter(), nullptr);
        ASSERT_EQ(element.constructor_count(), 0u);
        ASSERT_EQ(element.copy_constructor_count(), 0u);
        ASSERT_EQ(element.move_constructor_count(), 1u);
        ASSERT_EQ(element.copy_assign_count(), 0u);
        ASSERT_EQ(element.move_assign_count(), 0u);
    }

    array.resize(4);
    ASSERT_NE(array.data(), nullptr);
    ASSERT_EQ(array.count(), 4u);
    ASSERT_EQ(array.max_count(), 4u);
    ASSERT_EQ(array.allocator().allocation_count(), 2u);
    ASSERT_EQ(array.allocator().free_count(), 1u);
    // Should not touch already present elements
    for (usize index = 0; index < 2; index++) {
        ASSERT_EQ(array[index].id(), 0);
        ASSERT_EQ(array[index].destructor_counter(), nullptr);
        ASSERT_EQ(array[index].constructor_count(), 0u);
        ASSERT_EQ(array[index].copy_constructor_count(), 0u);
        ASSERT_EQ(array[index].move_constructor_count(), 1u);
        ASSERT_EQ(array[index].copy_assign_count(), 0u);
        ASSERT_EQ(array[index].move_assign_count(), 0u);
    }
    // Should default construct newnly added element
    for (usize index = 2; index < 4; index++) {
        ASSERT_EQ(array[index].id(), 0);
        ASSERT_EQ(array[index].destructor_counter(), nullptr);
        ASSERT_EQ(array[index].constructor_count(), 0u);
        ASSERT_EQ(array[index].copy_constructor_count(), 0u);
        ASSERT_EQ(array[index].move_constructor_count(), 0u);
        ASSERT_EQ(array[index].copy_assign_count(), 0u);
        ASSERT_EQ(array[index].move_assign_count(), 0u);
    }
}

TEST(array, resize_when_enough_memory_do_not_allocate_but_construct_in_place_with_trivial_type)
{

    using type = usize;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    array_type array;
    ASSERT_EQ(array.data(), nullptr);
    ASSERT_EQ(array.count(), 0u);
    ASSERT_EQ(array.max_count(), 0u);
    ASSERT_EQ(array.allocator().allocation_count(), 0u);
    ASSERT_EQ(array.allocator().free_count(), 0u);

    array.resize(2);
    ASSERT_NE(array.data(), nullptr);
    ASSERT_EQ(array.count(), 2u);
    ASSERT_EQ(array.max_count(), 2u);
    ASSERT_EQ(array.allocator().allocation_count(), 1u);
    ASSERT_EQ(array.allocator().free_count(), 0u);

    // Should default construct
    for (const type& element : array) {
        ASSERT_EQ(element, type());
    }
    // Change value to ensure resize keep values
    array[0] = type(10);
    array[1] = type(20);

    // reserve to be sure we have memory
    array.reserve(4);
    ASSERT_NE(array.data(), nullptr);
    ASSERT_EQ(array.count(), 2u);
    ASSERT_EQ(array.max_count(), 4u);
    ASSERT_EQ(array.allocator().allocation_count(), 2u);
    ASSERT_EQ(array.allocator().free_count(), 1u);

    // Reserve should not change the values
    array[0] = type(10);
    array[1] = type(20);


    array.resize(4);
    ASSERT_NE(array.data(), nullptr);
    ASSERT_EQ(array.count(), 4u);
    ASSERT_EQ(array.max_count(), 4u);
    ASSERT_EQ(array.allocator().allocation_count(), 2u);
    ASSERT_EQ(array.allocator().free_count(), 1u);

    // Should not modify already present element
    ASSERT_EQ(array[0], type(10));
    ASSERT_EQ(array[1], type(20));

    // Should default construct new element
    ASSERT_EQ(array[2], type());
    ASSERT_EQ(array[3], type());
}

TEST(array, resize_less_than_count_destroy_elements_but_not_reallocate_with_non_trivial_type)
{

    using type = hud_test::non_bitwise_type;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    array_type array;
    ASSERT_EQ(array.data(), nullptr);
    ASSERT_EQ(array.count(), 0u);
    ASSERT_EQ(array.max_count(), 0u);
    ASSERT_EQ(array.allocator().allocation_count(), 0u);
    ASSERT_EQ(array.allocator().free_count(), 0u);

    array.resize(2);
    ASSERT_NE(array.data(), nullptr);
    ASSERT_EQ(array.count(), 2u);
    ASSERT_EQ(array.max_count(), 2u);
    ASSERT_EQ(array.allocator().allocation_count(), 1u);
    ASSERT_EQ(array.allocator().free_count(), 0u);

    // Should default construct
    for (const type& element : array) {
        ASSERT_EQ(element.id(), 0);
        ASSERT_EQ(element.destructor_counter(), nullptr);
        ASSERT_EQ(element.constructor_count(), 0u);
        ASSERT_EQ(element.copy_constructor_count(), 0u);
        ASSERT_EQ(element.move_constructor_count(), 0u);
        ASSERT_EQ(element.copy_assign_count(), 0u);
        ASSERT_EQ(element.move_assign_count(), 0u);
    }

    // Should destroy the 2nd element but not touch the 1st
    i32 destructor_called = 0;
    array[1].set_dtor_counter_ptr(&destructor_called);
    array.resize(1);
    ASSERT_NE(array.data(), nullptr);
    ASSERT_EQ(array.count(), 1u);
    ASSERT_EQ(array.max_count(), 2u);
    ASSERT_EQ(array.allocator().allocation_count(), 1u);
    ASSERT_EQ(array.allocator().free_count(), 0u);

    // Should not touch already present elements
    ASSERT_EQ(array[0].id(), 0);
    ASSERT_EQ(array[0].destructor_counter(), nullptr);
    ASSERT_EQ(array[0].constructor_count(), 0u);
    ASSERT_EQ(array[0].copy_constructor_count(), 0u);
    ASSERT_EQ(array[0].move_constructor_count(), 0u);
    ASSERT_EQ(array[0].copy_assign_count(), 0u);
    ASSERT_EQ(array[0].move_assign_count(), 0u);

    ASSERT_TRUE(destructor_called);
}

TEST(array, resize_less_than_count_destroy_elements_but_not_reallocate_with_trivial_type)
{

    using type = usize;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    array_type array;
    ASSERT_EQ(array.data(), nullptr);
    ASSERT_EQ(array.count(), 0u);
    ASSERT_EQ(array.max_count(), 0u);
    ASSERT_EQ(array.allocator().allocation_count(), 0u);
    ASSERT_EQ(array.allocator().free_count(), 0u);

    array.resize(2);
    ASSERT_NE(array.data(), nullptr);
    ASSERT_EQ(array.count(), 2u);
    ASSERT_EQ(array.max_count(), 2u);
    ASSERT_EQ(array.allocator().allocation_count(), 1u);
    ASSERT_EQ(array.allocator().free_count(), 0u);

    // Should default construct
    for (const type& element : array) {
        ASSERT_EQ(element, type());
    }
    // Change values to check the element at index 1 is correctly destroy while index 0 is not modify
    array[0] = type(10);
    array[1] = type(20);

    // Should destroy the 2nd element but not touch the 1st
    array.resize(1);
    ASSERT_NE(array.data(), nullptr);
    ASSERT_EQ(array.count(), 1u);
    ASSERT_EQ(array.max_count(), 2u);
    ASSERT_EQ(array.allocator().allocation_count(), 1u);
    ASSERT_EQ(array.allocator().free_count(), 0u);

    ASSERT_EQ(array[0], type(10));
}