#include <core/containers/array.h>
#include "../misc/allocators.h"

GTEST_TEST(array, find_first_index_non_trivial_type)
{

    using type = hud_test::non_bitwise_type;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    array_type array;

    // Empty find return nothing
    hud_assert_eq(array.find_first_index(10), hud::index_none);

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

    hud_assert_eq(array.find_first_index(10), 0u);
    hud_assert_eq(array.find_first_index(20), 1u);
    hud_assert_eq(array.find_first_index(30), 2u);
    hud_assert_eq(array.find_first_index(40), 3u);
    hud_assert_eq(array.find_first_index(50), 4u);
    hud_assert_eq(array.find_first_index(0), hud::index_none);
}

GTEST_TEST(array, find_first_index_trivial_type)
{

    using type = usize;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    array_type array;

    // Empty find return nothing
    hud_assert_eq(array.find_first_index(10u), hud::index_none);

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

    hud_assert_eq(array.find_first_index(10u), 0u);
    hud_assert_eq(array.find_first_index(20u), 1u);
    hud_assert_eq(array.find_first_index(30u), 2u);
    hud_assert_eq(array.find_first_index(40u), 3u);
    hud_assert_eq(array.find_first_index(50u), 4u);
    hud_assert_eq(array.find_first_index(0u), hud::index_none);
}

GTEST_TEST(array, find_last_index_non_trivial_type)
{

    using type = hud_test::non_bitwise_type;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    array_type array;

    // Empty find return nothing
    hud_assert_eq(array.find_last_index(10), hud::index_none);

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

    hud_assert_eq(array.find_last_index(10), 5u);
    hud_assert_eq(array.find_last_index(20), 6u);
    hud_assert_eq(array.find_last_index(30), 7u);
    hud_assert_eq(array.find_last_index(40), 8u);
    hud_assert_eq(array.find_last_index(50), 9u);
    hud_assert_eq(array.find_last_index(0), hud::index_none);
}

GTEST_TEST(array, find_last_index_trivial_type)
{

    using type = usize;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    array_type array;

    // Empty find return nothing
    hud_assert_eq(array.find_last_index(10u), hud::index_none);

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

    hud_assert_eq(array.find_last_index(10u), 5u);
    hud_assert_eq(array.find_last_index(20u), 6u);
    hud_assert_eq(array.find_last_index(30u), 7u);
    hud_assert_eq(array.find_last_index(40u), 8u);
    hud_assert_eq(array.find_last_index(50u), 9u);
    hud_assert_eq(array.find_last_index(0u), hud::index_none);
}

GTEST_TEST(array, find_first_index_by_predicate_non_trivial_type)
{

    using type = hud_test::non_bitwise_type;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    array_type array;

    // Empty find return nothing
    hud_assert_eq(array.find_first_index_by_predicate([](const type &element)
                                                      { return element.id() == 10; }),
                  hud::index_none);

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

    hud_assert_eq(array.find_first_index_by_predicate([](const type &element)
                                                      { return element.id() == 10; }),
                  0u);
    hud_assert_eq(array.find_first_index_by_predicate([](const type &element)
                                                      { return element.id() == 20; }),
                  1u);
    hud_assert_eq(array.find_first_index_by_predicate([](const type &element)
                                                      { return element.id() == 30; }),
                  2u);
    hud_assert_eq(array.find_first_index_by_predicate([](const type &element)
                                                      { return element.id() == 40; }),
                  3u);
    hud_assert_eq(array.find_first_index_by_predicate([](const type &element)
                                                      { return element.id() == 50; }),
                  4u);
    hud_assert_eq(array.find_first_index_by_predicate([](const type &element)
                                                      { return element.id() == 0; }),
                  hud::index_none);
}

GTEST_TEST(array, find_first_index_by_predicate_trivial_type)
{

    using type = usize;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    array_type array;

    // Empty find return nothing
    hud_assert_eq(array.find_first_index_by_predicate([](const type &element)
                                                      { return element == 10u; }),
                  hud::index_none);

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

    hud_assert_eq(array.find_first_index_by_predicate([](const type &element)
                                                      { return element == 10u; }),
                  0u);
    hud_assert_eq(array.find_first_index_by_predicate([](const type &element)
                                                      { return element == 20u; }),
                  1u);
    hud_assert_eq(array.find_first_index_by_predicate([](const type &element)
                                                      { return element == 30u; }),
                  2u);
    hud_assert_eq(array.find_first_index_by_predicate([](const type &element)
                                                      { return element == 40u; }),
                  3u);
    hud_assert_eq(array.find_first_index_by_predicate([](const type &element)
                                                      { return element == 50u; }),
                  4u);
    hud_assert_eq(array.find_first_index_by_predicate([](const type &element)
                                                      { return element == 0u; }),
                  hud::index_none);
}

GTEST_TEST(array, find_last_index_by_predicate_non_trivial_type)
{

    using type = hud_test::non_bitwise_type;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    array_type array;

    // Empty find return nothing
    hud_assert_eq(array.find_last_index_by_predicate([](const type &element)
                                                     { return element.id() == 10; }),
                  hud::index_none);

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

    hud_assert_eq(array.find_last_index_by_predicate([](const type &element)
                                                     { return element.id() == 10; }),
                  5u);
    hud_assert_eq(array.find_last_index_by_predicate([](const type &element)
                                                     { return element.id() == 20; }),
                  6u);
    hud_assert_eq(array.find_last_index_by_predicate([](const type &element)
                                                     { return element.id() == 30; }),
                  7u);
    hud_assert_eq(array.find_last_index_by_predicate([](const type &element)
                                                     { return element.id() == 40; }),
                  8u);
    hud_assert_eq(array.find_last_index_by_predicate([](const type &element)
                                                     { return element.id() == 50; }),
                  9u);
    hud_assert_eq(array.find_last_index_by_predicate([](const type &element)
                                                     { return element.id() == 0; }),
                  hud::index_none);
}

GTEST_TEST(array, find_last_index_by_predicate_trivial_type)
{

    using type = usize;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    array_type array;

    // Empty find return nothing
    hud_assert_eq(array.find_last_index_by_predicate([](const type &element)
                                                     { return element == 10u; }),
                  hud::index_none);

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

    hud_assert_eq(array.find_last_index_by_predicate([](const type &element)
                                                     { return element == 10u; }),
                  5u);
    hud_assert_eq(array.find_last_index_by_predicate([](const type &element)
                                                     { return element == 20u; }),
                  6u);
    hud_assert_eq(array.find_last_index_by_predicate([](const type &element)
                                                     { return element == 30u; }),
                  7u);
    hud_assert_eq(array.find_last_index_by_predicate([](const type &element)
                                                     { return element == 40u; }),
                  8u);
    hud_assert_eq(array.find_last_index_by_predicate([](const type &element)
                                                     { return element == 50u; }),
                  9u);
    hud_assert_eq(array.find_last_index_by_predicate([](const type &element)
                                                     { return element == 0u; }),
                  hud::index_none);
}

GTEST_TEST(array, contains_non_trivial_type)
{

    using type = hud_test::non_bitwise_type;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    array_type array;

    // Empty find return nothing
    hud_assert_false(array.contains(10));

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

    hud_assert_true(array.contains(10));
    hud_assert_true(array.contains(20));
    hud_assert_true(array.contains(30));
    hud_assert_true(array.contains(40));
    hud_assert_true(array.contains(50));
    hud_assert_false(array.contains(0));
}

GTEST_TEST(array, contains_trivial_type)
{

    using type = usize;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    array_type array;

    // Empty find return nothing
    hud_assert_false(array.contains(10u));

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

    hud_assert_true(array.contains(10u));
    hud_assert_true(array.contains(20u));
    hud_assert_true(array.contains(30u));
    hud_assert_true(array.contains(40u));
    hud_assert_true(array.contains(50u));
    hud_assert_false(array.contains(0u));
}

GTEST_TEST(array, contains_by_predicate_non_trivial_type)
{

    using type = hud_test::non_bitwise_type;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    array_type array;

    // Empty find return nothing
    hud_assert_false(array.contains_by_predicate([](const type &element)
                                                 { return element.id() == 10; }));

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

    hud_assert_true(array.contains_by_predicate([](const type &element)
                                                { return element.id() == 10; }));
    hud_assert_true(array.contains_by_predicate([](const type &element)
                                                { return element.id() == 20; }));
    hud_assert_true(array.contains_by_predicate([](const type &element)
                                                { return element.id() == 30; }));
    hud_assert_true(array.contains_by_predicate([](const type &element)
                                                { return element.id() == 40; }));
    hud_assert_true(array.contains_by_predicate([](const type &element)
                                                { return element.id() == 50; }));
    hud_assert_false(array.contains_by_predicate([](const type &element)
                                                 { return element.id() == 0; }));
}

GTEST_TEST(array, contains_by_predicate_trivial_type)
{

    using type = usize;
    using array_type = hud::array<type, hud_test::array_allocator<alignof(type)>>;

    array_type array;

    // Empty find return nothing
    hud_assert_false(array.contains_by_predicate([](const type &element)
                                                 { return element == 10u; }));

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

    hud_assert_true(array.contains_by_predicate([](const type &element)
                                                { return element == 10u; }));
    hud_assert_true(array.contains_by_predicate([](const type &element)
                                                { return element == 20u; }));
    hud_assert_true(array.contains_by_predicate([](const type &element)
                                                { return element == 30u; }));
    hud_assert_true(array.contains_by_predicate([](const type &element)
                                                { return element == 40u; }));
    hud_assert_true(array.contains_by_predicate([](const type &element)
                                                { return element == 50u; }));
    hud_assert_false(array.contains_by_predicate([](const type &element)
                                                 { return element == 0u; }));
}