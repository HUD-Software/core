#include <core/containers/hashmap.h>
#include "../misc/allocator_watcher.h"

GTEST_TEST(hashmap, reserve_empty_to_zero_do_nothing)
{
    using AllocatorType = hud_test::allocator_watcher<1>;
    using HashMapType = hud::hashmap<i32, i64, hud::hashmap_default_hasher, hud::hashmap_default_key_equal<i32>, AllocatorType>;

    const auto test = []()
    {
        HashMapType map;
        bool reserve_ok = map.count() == 0;
        reserve_ok &= map.max_count() == 0;
        reserve_ok &= map.slack() == 0;
        reserve_ok &= map.allocator().allocation_count() == 0;
        reserve_ok &= map.allocator().free_count() == 0;
        map.reserve(0);

        reserve_ok &= map.count() == 0;
        reserve_ok &= map.max_count() == 0;
        reserve_ok &= map.slack() == 0;
        reserve_ok &= map.allocator().allocation_count() == 0;
        reserve_ok &= map.allocator().free_count() == 0;
        return reserve_ok;
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(result);
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(result);
    }
}

GTEST_TEST(hashmap, reserve_empty_to_size_allocate_only)
{
    using AllocatorType = hud_test::allocator_watcher<1>;
    using HashMapType = hud::hashmap<i32, i64, hud::hashmap_default_hasher, hud::hashmap_default_key_equal<i32>, AllocatorType>;
    const auto test = []()
    {
        HashMapType map;
        bool reserve_ok = map.count() == 0;
        reserve_ok &= map.max_count() == 0;
        reserve_ok &= map.slack() == 0;
        reserve_ok &= map.allocator().allocation_count() == 0;
        reserve_ok &= map.allocator().free_count() == 0;
        map.reserve(25);
        reserve_ok &= map.count() == 0;
        reserve_ok &= map.max_count() >= 25;
        reserve_ok &= map.slack() >= 0;
        reserve_ok &= (map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1));
        reserve_ok &= map.allocator().free_count() == 0;
        for (const auto &[key, value] : map)
        {
            reserve_ok = false;
        }
        return reserve_ok;
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(result);
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(result);
    }
}

GTEST_TEST(hashmap, reserve_non_empty_to_more_size_allocate_only)
{
    using AllocatorType = hud_test::allocator_watcher<1>;
    using HashMapType = hud::hashmap<i32, i64, hud::hashmap_default_hasher, hud::hashmap_default_key_equal<i32>, AllocatorType>;

    const auto test = []()
    {
        HashMapType map;
        bool reserve_ok = map.count() == 0;
        reserve_ok &= map.max_count() == 0;
        reserve_ok &= map.slack() == 0;
        reserve_ok &= map.allocator().allocation_count() == 0;
        reserve_ok &= map.allocator().free_count() == 0;
        map.reserve(2);
        map.add(1, 11);
        map.add(2, 22);
        reserve_ok &= map.count() == 2;
        reserve_ok &= map.max_count() >= 2;
        reserve_ok &= map.slack() >= 0;
        reserve_ok &= (map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1));
        reserve_ok &= map.allocator().free_count() == 0;

        const usize count_to_grow = map.max_count() * 2;
        map.reserve(count_to_grow);
        reserve_ok &= map.count() == 2;
        reserve_ok &= map.max_count() >= count_to_grow;
        reserve_ok &= map.slack() >= 0;
        reserve_ok &= (map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 4 : 2));
        reserve_ok &= (map.allocator().free_count() == (hud::is_constant_evaluated() ? 2 : 1));

        return reserve_ok;
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(result);
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(result);
    }
}

GTEST_TEST(hashmap, reserve_less_than_max_count_but_more_than_count_do_nothing)
{
    using AllocatorType = hud_test::allocator_watcher<1>;
    using HashMapType = hud::hashmap<i32, i64, hud::hashmap_default_hasher, hud::hashmap_default_key_equal<i32>, AllocatorType>;

    const auto test = []()
    {
        HashMapType map;
        bool reserve_ok = map.count() == 0;
        reserve_ok &= map.max_count() == 0;
        reserve_ok &= map.slack() == 0;
        reserve_ok &= map.allocator().allocation_count() == 0;
        reserve_ok &= map.allocator().free_count() == 0;
        map.reserve(4);
        map.add(1, 11);
        map.add(2, 22);
        reserve_ok &= map.count() == 2;
        reserve_ok &= map.max_count() >= 4;
        reserve_ok &= map.slack() >= 0;
        reserve_ok &= (map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1));
        reserve_ok &= map.allocator().free_count() == 0;

        map.reserve(3);
        reserve_ok &= map.count() == 2;
        reserve_ok &= map.max_count() >= 4;
        reserve_ok &= map.slack() >= 0;
        reserve_ok &= (map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1));
        reserve_ok &= map.allocator().free_count() == 0;

        return reserve_ok;
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(result);
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(result);
    }
}

GTEST_TEST(hashmap, reserve_less_than_count_but_more_than_count_do_nothing)
{
    using AllocatorType = hud_test::allocator_watcher<1>;
    using HashMapType = hud::hashmap<i32, i64, hud::hashmap_default_hasher, hud::hashmap_default_key_equal<i32>, AllocatorType>;

    const auto test = []()
    {
        HashMapType map;
        bool reserve_ok = map.count() == 0;
        reserve_ok &= map.max_count() == 0;
        reserve_ok &= map.slack() == 0;
        reserve_ok &= map.allocator().allocation_count() == 0;
        reserve_ok &= map.allocator().free_count() == 0;
        map.reserve(4);
        map.add(1, 11);
        map.add(2, 22);
        reserve_ok &= map.count() == 2;
        reserve_ok &= map.max_count() >= 4;
        reserve_ok &= map.slack() >= 0;
        reserve_ok &= (map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1));
        reserve_ok &= map.allocator().free_count() == 0;

        map.reserve(1);
        reserve_ok &= map.count() == 2;
        reserve_ok &= map.max_count() >= 4;
        reserve_ok &= map.slack() >= 0;
        reserve_ok &= (map.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1));
        reserve_ok &= map.allocator().free_count() == 0;

        return reserve_ok;
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(result);
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(result);
    }
}