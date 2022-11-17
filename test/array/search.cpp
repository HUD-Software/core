#include <core/containers/array.h>
#include "allocators.h"

GTEST_TEST(array, find_first_index_non_trivial_type)
{

    using type = hud_test::non_bitwise_type;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    array_type array;

    // Empty find return nothing
    GTEST_ASSERT_EQ(array.find_first_index(10), hud::index_none);

    array.emplace_back(10, nullptr);
    array.emplace_back(20, nullptr);
    array.emplace_back(30, nullptr);
    array.emplace_back(40, nullptr);
    array.emplace_back(50, nullptr);
    array.emplace_back(10, nullptr);
    array.emplace_back(20, nullptr);
    array.emplace_back(30, nullptr);
    array.emplace_back(40, nullptr);
    array.emplace_back(50, nullptr);

    GTEST_ASSERT_EQ(array.find_first_index(10), 0u);
    GTEST_ASSERT_EQ(array.find_first_index(20), 1u);
    GTEST_ASSERT_EQ(array.find_first_index(30), 2u);
    GTEST_ASSERT_EQ(array.find_first_index(40), 3u);
    GTEST_ASSERT_EQ(array.find_first_index(50), 4u);
    GTEST_ASSERT_EQ(array.find_first_index(0), hud::index_none);
}

GTEST_TEST(array, find_first_index_trivial_type)
{

    using type = usize;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    array_type array;

    // Empty find return nothing
    GTEST_ASSERT_EQ(array.find_first_index(10u), hud::index_none);

    array.emplace_back(10);
    array.emplace_back(20);
    array.emplace_back(30);
    array.emplace_back(40);
    array.emplace_back(50);
    array.emplace_back(10);
    array.emplace_back(20);
    array.emplace_back(30);
    array.emplace_back(40);
    array.emplace_back(50);

    GTEST_ASSERT_EQ(array.find_first_index(10u), 0u);
    GTEST_ASSERT_EQ(array.find_first_index(20u), 1u);
    GTEST_ASSERT_EQ(array.find_first_index(30u), 2u);
    GTEST_ASSERT_EQ(array.find_first_index(40u), 3u);
    GTEST_ASSERT_EQ(array.find_first_index(50u), 4u);
    GTEST_ASSERT_EQ(array.find_first_index(0u), hud::index_none);
}

GTEST_TEST(array, find_last_index_non_trivial_type)
{

    using type = hud_test::non_bitwise_type;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    array_type array;

    // Empty find return nothing
    GTEST_ASSERT_EQ(array.find_last_index(10), hud::index_none);

    array.emplace_back(10, nullptr);
    array.emplace_back(20, nullptr);
    array.emplace_back(30, nullptr);
    array.emplace_back(40, nullptr);
    array.emplace_back(50, nullptr);
    array.emplace_back(10, nullptr);
    array.emplace_back(20, nullptr);
    array.emplace_back(30, nullptr);
    array.emplace_back(40, nullptr);
    array.emplace_back(50, nullptr);

    GTEST_ASSERT_EQ(array.find_last_index(10), 5u);
    GTEST_ASSERT_EQ(array.find_last_index(20), 6u);
    GTEST_ASSERT_EQ(array.find_last_index(30), 7u);
    GTEST_ASSERT_EQ(array.find_last_index(40), 8u);
    GTEST_ASSERT_EQ(array.find_last_index(50), 9u);
    GTEST_ASSERT_EQ(array.find_last_index(0), hud::index_none);
}

GTEST_TEST(array, find_last_index_trivial_type)
{

    using type = usize;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    array_type array;

    // Empty find return nothing
    GTEST_ASSERT_EQ(array.find_last_index(10u), hud::index_none);

    array.emplace_back(10);
    array.emplace_back(20);
    array.emplace_back(30);
    array.emplace_back(40);
    array.emplace_back(50);
    array.emplace_back(10);
    array.emplace_back(20);
    array.emplace_back(30);
    array.emplace_back(40);
    array.emplace_back(50);

    GTEST_ASSERT_EQ(array.find_last_index(10u), 5u);
    GTEST_ASSERT_EQ(array.find_last_index(20u), 6u);
    GTEST_ASSERT_EQ(array.find_last_index(30u), 7u);
    GTEST_ASSERT_EQ(array.find_last_index(40u), 8u);
    GTEST_ASSERT_EQ(array.find_last_index(50u), 9u);
    GTEST_ASSERT_EQ(array.find_last_index(0u), hud::index_none);
}

GTEST_TEST(array, find_first_index_by_predicate_non_trivial_type)
{

    using type = hud_test::non_bitwise_type;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    array_type array;

    // Empty find return nothing
    GTEST_ASSERT_EQ(array.find_first_index_by_predicate([](const type& element) { return element.id() == 10; }), hud::index_none);

    array.emplace_back(10, nullptr);
    array.emplace_back(20, nullptr);
    array.emplace_back(30, nullptr);
    array.emplace_back(40, nullptr);
    array.emplace_back(50, nullptr);
    array.emplace_back(10, nullptr);
    array.emplace_back(20, nullptr);
    array.emplace_back(30, nullptr);
    array.emplace_back(40, nullptr);
    array.emplace_back(50, nullptr);

    GTEST_ASSERT_EQ(array.find_first_index_by_predicate([](const type& element) { return element.id() == 10; }), 0u);
    GTEST_ASSERT_EQ(array.find_first_index_by_predicate([](const type& element) { return element.id() == 20; }), 1u);
    GTEST_ASSERT_EQ(array.find_first_index_by_predicate([](const type& element) { return element.id() == 30; }), 2u);
    GTEST_ASSERT_EQ(array.find_first_index_by_predicate([](const type& element) { return element.id() == 40; }), 3u);
    GTEST_ASSERT_EQ(array.find_first_index_by_predicate([](const type& element) { return element.id() == 50; }), 4u);
    GTEST_ASSERT_EQ(array.find_first_index_by_predicate([](const type& element) { return element.id() == 0; }), hud::index_none);
}

GTEST_TEST(array, find_first_index_by_predicate_trivial_type)
{

    using type = usize;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    array_type array;

    // Empty find return nothing
    GTEST_ASSERT_EQ(array.find_first_index_by_predicate([](const type& element) { return element == 10u; }), hud::index_none);

    array.emplace_back(10);
    array.emplace_back(20);
    array.emplace_back(30);
    array.emplace_back(40);
    array.emplace_back(50);
    array.emplace_back(10);
    array.emplace_back(20);
    array.emplace_back(30);
    array.emplace_back(40);
    array.emplace_back(50);

    GTEST_ASSERT_EQ(array.find_first_index_by_predicate([](const type& element) { return element == 10u; }), 0u);
    GTEST_ASSERT_EQ(array.find_first_index_by_predicate([](const type& element) { return element == 20u; }), 1u);
    GTEST_ASSERT_EQ(array.find_first_index_by_predicate([](const type& element) { return element == 30u; }), 2u);
    GTEST_ASSERT_EQ(array.find_first_index_by_predicate([](const type& element) { return element == 40u; }), 3u);
    GTEST_ASSERT_EQ(array.find_first_index_by_predicate([](const type& element) { return element == 50u; }), 4u);
    GTEST_ASSERT_EQ(array.find_first_index_by_predicate([](const type& element) { return element == 0u; }), hud::index_none);
}

GTEST_TEST(array, find_last_index_by_predicate_non_trivial_type)
{

    using type = hud_test::non_bitwise_type;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    array_type array;

    // Empty find return nothing
    GTEST_ASSERT_EQ(array.find_last_index_by_predicate([](const type& element) { return element.id() == 10; }), hud::index_none);

    array.emplace_back(10, nullptr);
    array.emplace_back(20, nullptr);
    array.emplace_back(30, nullptr);
    array.emplace_back(40, nullptr);
    array.emplace_back(50, nullptr);
    array.emplace_back(10, nullptr);
    array.emplace_back(20, nullptr);
    array.emplace_back(30, nullptr);
    array.emplace_back(40, nullptr);
    array.emplace_back(50, nullptr);

    GTEST_ASSERT_EQ(array.find_last_index_by_predicate([](const type& element) { return element.id() == 10; }), 5u);
    GTEST_ASSERT_EQ(array.find_last_index_by_predicate([](const type& element) { return element.id() == 20; }), 6u);
    GTEST_ASSERT_EQ(array.find_last_index_by_predicate([](const type& element) { return element.id() == 30; }), 7u);
    GTEST_ASSERT_EQ(array.find_last_index_by_predicate([](const type& element) { return element.id() == 40; }), 8u);
    GTEST_ASSERT_EQ(array.find_last_index_by_predicate([](const type& element) { return element.id() == 50; }), 9u);
    GTEST_ASSERT_EQ(array.find_last_index_by_predicate([](const type& element) { return element.id() == 0; }), hud::index_none);
}

GTEST_TEST(array, find_last_index_by_predicate_trivial_type)
{

    using type = usize;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    array_type array;

    // Empty find return nothing
    GTEST_ASSERT_EQ(array.find_last_index_by_predicate([](const type& element) { return element == 10u; }), hud::index_none);

    array.emplace_back(10);
    array.emplace_back(20);
    array.emplace_back(30);
    array.emplace_back(40);
    array.emplace_back(50);
    array.emplace_back(10);
    array.emplace_back(20);
    array.emplace_back(30);
    array.emplace_back(40);
    array.emplace_back(50);

    GTEST_ASSERT_EQ(array.find_last_index_by_predicate([](const type& element) { return element == 10u; }), 5u);
    GTEST_ASSERT_EQ(array.find_last_index_by_predicate([](const type& element) { return element == 20u; }), 6u);
    GTEST_ASSERT_EQ(array.find_last_index_by_predicate([](const type& element) { return element == 30u; }), 7u);
    GTEST_ASSERT_EQ(array.find_last_index_by_predicate([](const type& element) { return element == 40u; }), 8u);
    GTEST_ASSERT_EQ(array.find_last_index_by_predicate([](const type& element) { return element == 50u; }), 9u);
    GTEST_ASSERT_EQ(array.find_last_index_by_predicate([](const type& element) { return element == 0u; }), hud::index_none);
}

GTEST_TEST(array, contains_non_trivial_type)
{

    using type = hud_test::non_bitwise_type;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    array_type array;

    // Empty find return nothing
    GTEST_ASSERT_FALSE(array.contains(10));

    array.emplace_back(10, nullptr);
    array.emplace_back(20, nullptr);
    array.emplace_back(30, nullptr);
    array.emplace_back(40, nullptr);
    array.emplace_back(50, nullptr);
    array.emplace_back(10, nullptr);
    array.emplace_back(20, nullptr);
    array.emplace_back(30, nullptr);
    array.emplace_back(40, nullptr);
    array.emplace_back(50, nullptr);

    GTEST_ASSERT_TRUE(array.contains(10));
    GTEST_ASSERT_TRUE(array.contains(20));
    GTEST_ASSERT_TRUE(array.contains(30));
    GTEST_ASSERT_TRUE(array.contains(40));
    GTEST_ASSERT_TRUE(array.contains(50));
    GTEST_ASSERT_FALSE(array.contains(0));
}

GTEST_TEST(array, contains_trivial_type)
{

    using type = usize;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    array_type array;

    // Empty find return nothing
    GTEST_ASSERT_FALSE(array.contains(10u));

    array.emplace_back(10);
    array.emplace_back(20);
    array.emplace_back(30);
    array.emplace_back(40);
    array.emplace_back(50);
    array.emplace_back(10);
    array.emplace_back(20);
    array.emplace_back(30);
    array.emplace_back(40);
    array.emplace_back(50);

    GTEST_ASSERT_TRUE(array.contains(10u));
    GTEST_ASSERT_TRUE(array.contains(20u));
    GTEST_ASSERT_TRUE(array.contains(30u));
    GTEST_ASSERT_TRUE(array.contains(40u));
    GTEST_ASSERT_TRUE(array.contains(50u));
    GTEST_ASSERT_FALSE(array.contains(0u));
}

GTEST_TEST(array, contains_by_predicate_non_trivial_type)
{

    using type = hud_test::non_bitwise_type;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    array_type array;

    // Empty find return nothing
    GTEST_ASSERT_FALSE(array.contains_by_predicate([](const type& element) { return element.id() == 10; }));

    array.emplace_back(10, nullptr);
    array.emplace_back(20, nullptr);
    array.emplace_back(30, nullptr);
    array.emplace_back(40, nullptr);
    array.emplace_back(50, nullptr);
    array.emplace_back(10, nullptr);
    array.emplace_back(20, nullptr);
    array.emplace_back(30, nullptr);
    array.emplace_back(40, nullptr);
    array.emplace_back(50, nullptr);

    GTEST_ASSERT_TRUE(array.contains_by_predicate([](const type& element) {return element.id() == 10; }));
    GTEST_ASSERT_TRUE(array.contains_by_predicate([](const type& element) {return element.id() == 20; }));
    GTEST_ASSERT_TRUE(array.contains_by_predicate([](const type& element) {return element.id() == 30; }));
    GTEST_ASSERT_TRUE(array.contains_by_predicate([](const type& element) {return element.id() == 40; }));
    GTEST_ASSERT_TRUE(array.contains_by_predicate([](const type& element) {return element.id() == 50; }));
    GTEST_ASSERT_FALSE(array.contains_by_predicate([](const type& element) {return element.id() == 0; }));
}

GTEST_TEST(array, contains_by_predicate_trivial_type)
{

    using type = usize;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    array_type array;

    // Empty find return nothing
    GTEST_ASSERT_FALSE(array.contains_by_predicate([](const type& element) { return element == 10u; }));

    array.emplace_back(10);
    array.emplace_back(20);
    array.emplace_back(30);
    array.emplace_back(40);
    array.emplace_back(50);
    array.emplace_back(10);
    array.emplace_back(20);
    array.emplace_back(30);
    array.emplace_back(40);
    array.emplace_back(50);

    GTEST_ASSERT_TRUE(array.contains_by_predicate([](const type& element) {return element == 10u; }));
    GTEST_ASSERT_TRUE(array.contains_by_predicate([](const type& element) {return element == 20u; }));
    GTEST_ASSERT_TRUE(array.contains_by_predicate([](const type& element) {return element == 30u; }));
    GTEST_ASSERT_TRUE(array.contains_by_predicate([](const type& element) {return element == 40u; }));
    GTEST_ASSERT_TRUE(array.contains_by_predicate([](const type& element) {return element == 50u; }));
    GTEST_ASSERT_FALSE(array.contains_by_predicate([](const type& element) {return element == 0u; }));
}