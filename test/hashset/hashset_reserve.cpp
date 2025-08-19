#include <core/containers/hashset.h>
#include "../misc/allocator_watcher.h"

GTEST_TEST(hashset, reserve_empty_to_zero_do_nothing)
{
    using AllocatorType = hud_test::allocator_watcher<1>;
    using HashMapType = hud::hashset<i32, hud::hash_64<i32>, hud::equal<i32>, AllocatorType>;

    const auto test = []()
    {
        HashMapType set;
        bool reserve_ok = set.count() == 0;
        reserve_ok &= set.max_count() == 0;
        reserve_ok &= set.slack() == 0;
        reserve_ok &= set.allocator().allocation_count() == 0;
        reserve_ok &= set.allocator().free_count() == 0;
        set.reserve(0);

        reserve_ok &= set.count() == 0;
        reserve_ok &= set.max_count() == 0;
        reserve_ok &= set.slack() == 0;
        reserve_ok &= set.allocator().allocation_count() == 0;
        reserve_ok &= set.allocator().free_count() == 0;
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

GTEST_TEST(hashset, reserve_empty_to_size_allocate_only)
{
    using AllocatorType = hud_test::allocator_watcher<1>;
    using HashMapType = hud::hashset<i32, hud::hash_64<i32>, hud::equal<i32>, AllocatorType>;
    const auto test = []()
    {
        HashMapType set;
        bool reserve_ok = set.count() == 0;
        reserve_ok &= set.max_count() == 0;
        reserve_ok &= set.slack() == 0;
        reserve_ok &= set.allocator().allocation_count() == 0;
        reserve_ok &= set.allocator().free_count() == 0;
        set.reserve(25);
        reserve_ok &= set.count() == 0;
        reserve_ok &= set.max_count() >= 25;
        reserve_ok &= set.slack() >= 0;
        reserve_ok &= (set.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1));
        reserve_ok &= set.allocator().free_count() == 0;
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

GTEST_TEST(hashset, reserve_non_empty_to_more_size_allocate_only)
{
    using AllocatorType = hud_test::allocator_watcher<1>;
    using HashMapType = hud::hashset<i32, hud::hash_64<i32>, hud::equal<i32>, AllocatorType>;

    const auto test = []()
    {
        HashMapType set;
        bool reserve_ok = set.count() == 0;
        reserve_ok &= set.max_count() == 0;
        reserve_ok &= set.slack() == 0;
        reserve_ok &= set.allocator().allocation_count() == 0;
        reserve_ok &= set.allocator().free_count() == 0;
        set.reserve(2);
        set.add(1);
        set.add(2);
        reserve_ok &= set.count() == 2;
        reserve_ok &= set.max_count() >= 2;
        reserve_ok &= set.slack() >= 0;
        reserve_ok &= (set.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1));
        reserve_ok &= set.allocator().free_count() == 0;

        const usize count_to_grow = set.max_count() * 2;
        set.reserve(count_to_grow);
        reserve_ok &= set.count() == 2;
        reserve_ok &= set.max_count() >= count_to_grow;
        reserve_ok &= set.slack() >= 0;
        reserve_ok &= (set.allocator().allocation_count() == (hud::is_constant_evaluated() ? 4 : 2));
        reserve_ok &= (set.allocator().free_count() == (hud::is_constant_evaluated() ? 2 : 1));

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

GTEST_TEST(hashset, reserve_less_than_max_count_but_more_than_count_do_nothing)
{
    using AllocatorType = hud_test::allocator_watcher<1>;
    using HashMapType = hud::hashset<i32, hud::hash_64<i32>, hud::equal<i32>, AllocatorType>;

    const auto test = []()
    {
        HashMapType set;
        bool reserve_ok = set.count() == 0;
        reserve_ok &= set.max_count() == 0;
        reserve_ok &= set.slack() == 0;
        reserve_ok &= set.allocator().allocation_count() == 0;
        reserve_ok &= set.allocator().free_count() == 0;
        set.reserve(4);
        set.add(1);
        set.add(2);
        reserve_ok &= set.count() == 2;
        reserve_ok &= set.max_count() >= 4;
        reserve_ok &= set.slack() >= 0;
        reserve_ok &= (set.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1));
        reserve_ok &= set.allocator().free_count() == 0;

        set.reserve(3);
        reserve_ok &= set.count() == 2;
        reserve_ok &= set.max_count() >= 4;
        reserve_ok &= set.slack() >= 0;
        reserve_ok &= (set.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1));
        reserve_ok &= set.allocator().free_count() == 0;

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

GTEST_TEST(hashset, reserve_less_than_count_but_more_than_count_do_nothing)
{
    using AllocatorType = hud_test::allocator_watcher<1>;
    using HashMapType = hud::hashset<i32, hud::hash_64<i32>, hud::equal<i32>, AllocatorType>;

    const auto test = []()
    {
        HashMapType set;
        bool reserve_ok = set.count() == 0;
        reserve_ok &= set.max_count() == 0;
        reserve_ok &= set.slack() == 0;
        reserve_ok &= set.allocator().allocation_count() == 0;
        reserve_ok &= set.allocator().free_count() == 0;
        set.reserve(4);
        set.add(1);
        set.add(2);
        reserve_ok &= set.count() == 2;
        reserve_ok &= set.max_count() >= 4;
        reserve_ok &= set.slack() >= 0;
        reserve_ok &= (set.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1));
        reserve_ok &= set.allocator().free_count() == 0;

        set.reserve(1);
        reserve_ok &= set.count() == 2;
        reserve_ok &= set.max_count() >= 4;
        reserve_ok &= set.slack() >= 0;
        reserve_ok &= (set.allocator().allocation_count() == (hud::is_constant_evaluated() ? 2 : 1));
        reserve_ok &= set.allocator().free_count() == 0;

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