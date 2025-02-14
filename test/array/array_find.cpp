#include <core/containers/array.h>
#include "../misc/allocator_watcher.h"

GTEST_TEST(array, find_first_index_non_trivial_type)
{

    using type = hud_test::non_bitwise_type;
    using array_type = hud::array<type, hud_test::allocator_watcher<alignof(type)>>;

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
    using array_type = hud::array<type, hud_test::allocator_watcher<alignof(type)>>;

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
    using array_type = hud::array<type, hud_test::allocator_watcher<alignof(type)>>;

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
    using array_type = hud::array<type, hud_test::allocator_watcher<alignof(type)>>;

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
    using array_type = hud::array<type, hud_test::allocator_watcher<alignof(type)>>;

    array_type array;

    const auto find_20u = [](const type &element)
    {
        return element == 20u;
    };
    const auto find_0u = [](const type &element)
    {
        return element == 0u;
    };

    // Empty find return nothing
    hud_assert_eq(array.find_first_index_by_predicate(find_20u), hud::index_none);
    hud_assert_eq(array.find_first_index_by_predicate(find_0u), hud::index_none);

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

    hud_assert_eq(array.find_first_index_by_predicate(find_20u), 1u);
    hud_assert_eq(array.find_first_index_by_predicate(find_0u), hud::index_none);
}

GTEST_TEST(array, find_first_index_by_predicate_trivial_type)
{

    using type = usize;
    using array_type = hud::array<type, hud_test::allocator_watcher<alignof(type)>>;

    array_type array;

    const auto find_20u = [](const type &element)
    {
        return element == 20u;
    };
    const auto find_0u = [](const type &element)
    {
        return element == 0u;
    };

    // Empty find return nothing
    hud_assert_eq(array.find_first_index_by_predicate(find_20u), hud::index_none);
    hud_assert_eq(array.find_first_index_by_predicate(find_0u), hud::index_none);

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

    hud_assert_eq(array.find_first_index_by_predicate(find_20u), 1u);
    hud_assert_eq(array.find_first_index_by_predicate(find_0u), hud::index_none);
}

GTEST_TEST(array, find_last_index_by_predicate_non_trivial_type)
{

    using type = hud_test::non_bitwise_type;
    using array_type = hud::array<type, hud_test::allocator_watcher<alignof(type)>>;

    array_type array;

    const auto find_10u = [](const type &element)
    {
        return element.id() == 10u;
    };
    const auto find_0u = [](const type &element)
    {
        return element.id() == 0u;
    };

    // Empty find return nothing
    hud_assert_eq(array.find_last_index_by_predicate(find_10u), hud::index_none);
    hud_assert_eq(array.find_last_index_by_predicate(find_0u), hud::index_none);

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

    hud_assert_eq(array.find_last_index_by_predicate(find_10u), 5u);
    hud_assert_eq(array.find_last_index_by_predicate(find_0u), hud::index_none);
}

GTEST_TEST(array, find_last_index_by_predicate_trivial_type)
{

    using type = usize;
    using array_type = hud::array<type, hud_test::allocator_watcher<alignof(type)>>;

    array_type array;

    const auto find_10u = [](const type &element)
    {
        return element == 10u;
    };
    const auto find_0u = [](const type &element)
    {
        return element == 0u;
    };

    // Empty find return nothing
    hud_assert_eq(array.find_last_index_by_predicate(find_10u), hud::index_none);
    hud_assert_eq(array.find_last_index_by_predicate(find_0u), hud::index_none);

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

    hud_assert_eq(array.find_last_index_by_predicate(find_10u), 5u);
    hud_assert_eq(array.find_last_index_by_predicate(find_0u), hud::index_none);
}
