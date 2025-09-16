#include <core/containers/vector.h>
#include "../misc/allocator_watcher.h"

GTEST_TEST(vector, contains_non_trivial_type)
{

    using type = hud_test::non_bitwise_type;
    using array_type = hud::vector<type, hud_test::allocator_watcher<alignof(type)>>;

    array_type vector;

    // Empty find return nothing
    hud_assert_false(vector.contains(10));

    vector.emplace_back(10, nullptr);
    vector.emplace_back(20, nullptr);
    vector.emplace_back(30, nullptr);
    vector.emplace_back(40, nullptr);
    vector.emplace_back(50, nullptr);
    vector.emplace_back(10, nullptr);
    vector.emplace_back(20, nullptr);
    vector.emplace_back(30, nullptr);
    vector.emplace_back(40, nullptr);
    vector.emplace_back(50, nullptr);

    hud_assert_true(vector.contains(10));
    hud_assert_true(vector.contains(20));
    hud_assert_true(vector.contains(30));
    hud_assert_true(vector.contains(40));
    hud_assert_true(vector.contains(50));
    hud_assert_false(vector.contains(0));
}

GTEST_TEST(vector, contains_trivial_type)
{

    using type = usize;
    using array_type = hud::vector<type, hud_test::allocator_watcher<alignof(type)>>;

    array_type vector;

    // Empty find return nothing
    hud_assert_false(vector.contains(10u));

    vector.emplace_back(10);
    vector.emplace_back(20);
    vector.emplace_back(30);
    vector.emplace_back(40);
    vector.emplace_back(50);
    vector.emplace_back(10);
    vector.emplace_back(20);
    vector.emplace_back(30);
    vector.emplace_back(40);
    vector.emplace_back(50);

    hud_assert_true(vector.contains(10u));
    hud_assert_true(vector.contains(20u));
    hud_assert_true(vector.contains(30u));
    hud_assert_true(vector.contains(40u));
    hud_assert_true(vector.contains(50u));
    hud_assert_false(vector.contains(0u));
}

GTEST_TEST(vector, contains_by_predicate_non_trivial_type)
{

    using type = hud_test::non_bitwise_type;
    using array_type = hud::vector<type, hud_test::allocator_watcher<alignof(type)>>;

    array_type vector;

    const auto is_10u = [](const type &element) {
        return element == 10u;
    };
    const auto is_0u = [](const type &element) {
        return element == 0u;
    };

    // Empty find return nothing
    hud_assert_false(vector.contains_by_predicate(is_10u));
    hud_assert_false(vector.contains_by_predicate(is_0u));

    vector.emplace_back(10, nullptr);
    vector.emplace_back(20, nullptr);
    vector.emplace_back(30, nullptr);
    vector.emplace_back(40, nullptr);
    vector.emplace_back(50, nullptr);
    vector.emplace_back(10, nullptr);
    vector.emplace_back(20, nullptr);
    vector.emplace_back(30, nullptr);
    vector.emplace_back(40, nullptr);
    vector.emplace_back(50, nullptr);

    hud_assert_true(vector.contains_by_predicate(is_10u));
    hud_assert_false(vector.contains_by_predicate(is_0u));
}

GTEST_TEST(vector, contains_by_predicate_trivial_type)
{

    using type = usize;
    using array_type = hud::vector<type, hud_test::allocator_watcher<alignof(type)>>;

    array_type vector;

    const auto is_10u = [](const type &element) {
        return element == 10u;
    };
    const auto is_0u = [](const type &element) {
        return element == 0u;
    };

    // Empty find return nothing
    hud_assert_false(vector.contains_by_predicate(is_10u));
    hud_assert_false(vector.contains_by_predicate(is_0u));

    vector.emplace_back(10);
    vector.emplace_back(20);
    vector.emplace_back(30);
    vector.emplace_back(40);
    vector.emplace_back(50);
    vector.emplace_back(10);
    vector.emplace_back(20);
    vector.emplace_back(30);
    vector.emplace_back(40);
    vector.emplace_back(50);

    hud_assert_true(vector.contains_by_predicate(is_10u));
    hud_assert_false(vector.contains_by_predicate(is_0u));
}