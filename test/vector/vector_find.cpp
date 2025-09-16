#include <core/containers/vector.h>
#include "../misc/allocator_watcher.h"

GTEST_TEST(vector, find_first_index_non_trivial_type)
{

    using type = hud_test::non_bitwise_type;
    using array_type = hud::vector<type, hud_test::allocator_watcher<alignof(type)>>;

    array_type vector;

    // Empty find return nothing
    hud_assert_eq(vector.find_first_index(10), hud::index_none);

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

    hud_assert_eq(vector.find_first_index(10), 0u);
    hud_assert_eq(vector.find_first_index(20), 1u);
    hud_assert_eq(vector.find_first_index(30), 2u);
    hud_assert_eq(vector.find_first_index(40), 3u);
    hud_assert_eq(vector.find_first_index(50), 4u);
    hud_assert_eq(vector.find_first_index(0), hud::index_none);
}

GTEST_TEST(vector, find_first_index_trivial_type)
{

    using type = usize;
    using array_type = hud::vector<type, hud_test::allocator_watcher<alignof(type)>>;

    array_type vector;

    // Empty find return nothing
    hud_assert_eq(vector.find_first_index(10u), hud::index_none);

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

    hud_assert_eq(vector.find_first_index(10u), 0u);
    hud_assert_eq(vector.find_first_index(20u), 1u);
    hud_assert_eq(vector.find_first_index(30u), 2u);
    hud_assert_eq(vector.find_first_index(40u), 3u);
    hud_assert_eq(vector.find_first_index(50u), 4u);
    hud_assert_eq(vector.find_first_index(0u), hud::index_none);
}

GTEST_TEST(vector, find_last_index_non_trivial_type)
{

    using type = hud_test::non_bitwise_type;
    using array_type = hud::vector<type, hud_test::allocator_watcher<alignof(type)>>;

    array_type vector;

    // Empty find return nothing
    hud_assert_eq(vector.find_last_index(10), hud::index_none);

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

    hud_assert_eq(vector.find_last_index(10), 5u);
    hud_assert_eq(vector.find_last_index(20), 6u);
    hud_assert_eq(vector.find_last_index(30), 7u);
    hud_assert_eq(vector.find_last_index(40), 8u);
    hud_assert_eq(vector.find_last_index(50), 9u);
    hud_assert_eq(vector.find_last_index(0), hud::index_none);
}

GTEST_TEST(vector, find_last_index_trivial_type)
{

    using type = usize;
    using array_type = hud::vector<type, hud_test::allocator_watcher<alignof(type)>>;

    array_type vector;

    // Empty find return nothing
    hud_assert_eq(vector.find_last_index(10u), hud::index_none);

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

    hud_assert_eq(vector.find_last_index(10u), 5u);
    hud_assert_eq(vector.find_last_index(20u), 6u);
    hud_assert_eq(vector.find_last_index(30u), 7u);
    hud_assert_eq(vector.find_last_index(40u), 8u);
    hud_assert_eq(vector.find_last_index(50u), 9u);
    hud_assert_eq(vector.find_last_index(0u), hud::index_none);
}

GTEST_TEST(vector, find_first_index_by_predicate_non_trivial_type)
{

    using type = hud_test::non_bitwise_type;
    using array_type = hud::vector<type, hud_test::allocator_watcher<alignof(type)>>;

    array_type vector;

    const auto find_20u = [](const type &element) {
        return element == 20u;
    };
    const auto find_0u = [](const type &element) {
        return element == 0u;
    };

    // Empty find return nothing
    hud_assert_eq(vector.find_first_index_by_predicate(find_20u), hud::index_none);
    hud_assert_eq(vector.find_first_index_by_predicate(find_0u), hud::index_none);

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

    hud_assert_eq(vector.find_first_index_by_predicate(find_20u), 1u);
    hud_assert_eq(vector.find_first_index_by_predicate(find_0u), hud::index_none);
}

GTEST_TEST(vector, find_first_index_by_predicate_trivial_type)
{

    using type = usize;
    using array_type = hud::vector<type, hud_test::allocator_watcher<alignof(type)>>;

    array_type vector;

    const auto find_20u = [](const type &element) {
        return element == 20u;
    };
    const auto find_0u = [](const type &element) {
        return element == 0u;
    };

    // Empty find return nothing
    hud_assert_eq(vector.find_first_index_by_predicate(find_20u), hud::index_none);
    hud_assert_eq(vector.find_first_index_by_predicate(find_0u), hud::index_none);

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

    hud_assert_eq(vector.find_first_index_by_predicate(find_20u), 1u);
    hud_assert_eq(vector.find_first_index_by_predicate(find_0u), hud::index_none);
}

GTEST_TEST(vector, find_last_index_by_predicate_non_trivial_type)
{

    using type = hud_test::non_bitwise_type;
    using array_type = hud::vector<type, hud_test::allocator_watcher<alignof(type)>>;

    array_type vector;

    const auto find_10u = [](const type &element) {
        return element.id() == 10u;
    };
    const auto find_0u = [](const type &element) {
        return element.id() == 0u;
    };

    // Empty find return nothing
    hud_assert_eq(vector.find_last_index_by_predicate(find_10u), hud::index_none);
    hud_assert_eq(vector.find_last_index_by_predicate(find_0u), hud::index_none);

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

    hud_assert_eq(vector.find_last_index_by_predicate(find_10u), 5u);
    hud_assert_eq(vector.find_last_index_by_predicate(find_0u), hud::index_none);
}

GTEST_TEST(vector, find_last_index_by_predicate_trivial_type)
{

    using type = usize;
    using array_type = hud::vector<type, hud_test::allocator_watcher<alignof(type)>>;

    array_type vector;

    const auto find_10u = [](const type &element) {
        return element == 10u;
    };
    const auto find_0u = [](const type &element) {
        return element == 0u;
    };

    // Empty find return nothing
    hud_assert_eq(vector.find_last_index_by_predicate(find_10u), hud::index_none);
    hud_assert_eq(vector.find_last_index_by_predicate(find_0u), hud::index_none);

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

    hud_assert_eq(vector.find_last_index_by_predicate(find_10u), 5u);
    hud_assert_eq(vector.find_last_index_by_predicate(find_0u), hud::index_none);
}
