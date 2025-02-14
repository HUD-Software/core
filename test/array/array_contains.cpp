#include <core/containers/array.h>
#include "../misc/allocator_watcher.h"

GTEST_TEST(array, contains_non_trivial_type)
{

    using type = hud_test::non_bitwise_type;
    using array_type = hud::array<type, hud_test::allocator_watcher<alignof(type)>>;

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
    using array_type = hud::array<type, hud_test::allocator_watcher<alignof(type)>>;

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
    using array_type = hud::array<type, hud_test::allocator_watcher<alignof(type)>>;

    array_type array;

    const auto is_10u = [](const type &element)
    {
        return element == 10u;
    };
    const auto is_0u = [](const type &element)
    {
        return element == 0u;
    };

    // Empty find return nothing
    hud_assert_false(array.contains_by_predicate(is_10u));
    hud_assert_false(array.contains_by_predicate(is_0u));

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

    hud_assert_true(array.contains_by_predicate(is_10u));
    hud_assert_false(array.contains_by_predicate(is_0u));
}

GTEST_TEST(array, contains_by_predicate_trivial_type)
{

    using type = usize;
    using array_type = hud::array<type, hud_test::allocator_watcher<alignof(type)>>;

    array_type array;

    const auto is_10u = [](const type &element)
    {
        return element == 10u;
    };
    const auto is_0u = [](const type &element)
    {
        return element == 0u;
    };

    // Empty find return nothing
    hud_assert_false(array.contains_by_predicate(is_10u));
    hud_assert_false(array.contains_by_predicate(is_0u));

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

    hud_assert_true(array.contains_by_predicate(is_10u));
    hud_assert_false(array.contains_by_predicate(is_0u));
}