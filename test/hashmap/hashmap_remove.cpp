#include <core/containers/hashmap.h>

namespace hud_test
{
    struct colliding_hasher
    {
        /**
         * Operator to hash the value and combine it with the current hasher value.
         * This function uses variadic templates to accept multiple arguments.
         * @tparam type_t Types of the arguments to hash.
         * @param values Arguments to hash.
         * @return A 64-bit hash value.
         */
        [[nodiscard]] constexpr u64 operator()(const hud_test::non_bitwise_type &custom) noexcept
        {
            return hud::hash_64<i64> {}(custom.id() / 3);
        }

        /**
         * Function to hash the value and combine it with the current hasher value.
         * This function uses variadic templates to accept multiple arguments.
         * @tparam type_t Types of the arguments to hash.
         * @param values Arguments to hash.
         * @return A 64-bit hash value.
         */
        template<typename... type_t>
        [[nodiscard]] constexpr u64 hash(type_t &&...values) noexcept
        {
            return (*this)(hud::forward<type_t>(values)...);
        }
    };

} // namespace hud_test

GTEST_TEST(hashmap, remove_non_trivial_type)
{
    using key_type = hud_test::non_bitwise_type;
    using value_type = hud_test::non_bitwise_type;
    using hashmap_type = hud::hashmap<key_type, value_type>;

    // Simple test
    const auto test = []()
    {
        // Empty map
        hashmap_type empty_map;
        bool ok_empty = !empty_map.contains(1);
        empty_map.remove(1);
        ok_empty &= empty_map.count() == 0;
        ok_empty &= empty_map.max_count() == 0;
        ok_empty &= empty_map.slack() == 0;

        // Remove 1/1 element
        hashmap_type map_1_1;
        map_1_1.add({1, 11});
        bool ok_1_1 = map_1_1.contains(1);
        ok_1_1 &= map_1_1.count() == 1;
        ok_1_1 &= map_1_1.max_count() >= 1;
        ok_1_1 &= map_1_1.slack() >= 0;
        map_1_1.remove(0);
        ok_1_1 &= map_1_1.contains(1);
        ok_1_1 &= map_1_1.count() == 1;
        ok_1_1 &= map_1_1.max_count() >= 1;
        ok_1_1 &= map_1_1.slack() >= 0;
        map_1_1.remove(1);
        ok_1_1 &= !map_1_1.contains(0);
        ok_1_1 &= map_1_1.count() == 0;
        ok_1_1 &= map_1_1.max_count() >= 1;
        ok_1_1 &= map_1_1.slack() >= 1;

        return std::tuple {
            ok_empty,
            ok_1_1,
        };
    };

    // This test add 128 value remove every y elements check the map,
    // then readd the removed elements and re check again the map by iterating over values
    const auto test_2 = [](usize y)
    {
        // Remove elements then add it again
        hashmap_type map_1_x;
        bool ok_map_1_x = true;
        for (usize x = 0; x < 128; x++)
        {
            map_1_x.add({x, x * x});
            ok_map_1_x &= map_1_x.count() == (x + 1);
            ok_map_1_x &= map_1_x.max_count() >= (x + 1);
            ok_map_1_x &= map_1_x.slack() >= 0;
        }

        // Check we really have all values
        for (usize x = 0; x < 128; x++)
        {
            const auto it = map_1_x.find(x);
            if (it != map_1_x.end())
            {
                const auto &[key, value] = *it;
                ok_map_1_x &= (key == x);
                ok_map_1_x &= (value == (x * x));
            }
        }

        // Remove every element 2 elements
        for (usize x = 0; x < 128; x += y)
        {
            map_1_x.remove(x);
            ok_map_1_x &= !map_1_x.contains(x);
        }
        // Add removed element
        for (usize x = 0; x < 128; x += y)
        {
            map_1_x.add({x, x * x});
        }
        // Check we really have all values
        for (usize x = 0; x < 128; x++)
        {
            const auto it = map_1_x.find(x);
            if (it != map_1_x.end())
            {
                const auto &[key, value] = *it;
                ok_map_1_x &= (key == x);
                ok_map_1_x &= (value == (x * x));
            }
        }
        ok_map_1_x &= map_1_x.count() == 128;
        ok_map_1_x &= map_1_x.max_count() >= 128;
        ok_map_1_x &= map_1_x.slack() >= 0;

        // Check we can still iterate
        // This works because the hash of an interger is always the value of the integer,
        // the order of key is kept in order du to the hash
        usize x = 0;
        for (const auto &[key, value] : map_1_x)
        {
            ok_map_1_x &= (key == x);
            ok_map_1_x &= (value == (x * x));
            x++;
        }

        return std::tuple {
            ok_map_1_x
        };
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));

        const auto result_1 = test_2(1);
        hud_assert_true(std::get<0>(result_1));

        const auto result_2 = test_2(2);
        hud_assert_true(std::get<0>(result_2));

        const auto result_3 = test_2(3);
        hud_assert_true(std::get<0>(result_3));

        const auto result_4 = test_2(4);
        hud_assert_true(std::get<0>(result_4));

        const auto result_5 = test_2(5);
        hud_assert_true(std::get<0>(result_5));

        const auto result_10 = test_2(10);
        hud_assert_true(std::get<0>(result_10));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));

        constexpr auto result_1 = test_2(1);
        hud_assert_true(std::get<0>(result_1));

        constexpr auto result_2 = test_2(2);
        hud_assert_true(std::get<0>(result_2));

        constexpr auto result_3 = test_2(3);
        hud_assert_true(std::get<0>(result_3));

        constexpr auto result_4 = test_2(4);
        hud_assert_true(std::get<0>(result_4));

        constexpr auto result_5 = test_2(5);
        hud_assert_true(std::get<0>(result_5));

        constexpr auto result_10 = test_2(10);
        hud_assert_true(std::get<0>(result_10));
    }
}

GTEST_TEST(hashmap, remove_collided_key)
{
    using key_type = hud_test::non_bitwise_type;
    using value_type = hud_test::non_bitwise_type;
    using hashmap_type = hud::hashmap<key_type, value_type, hud_test::colliding_hasher>;

    // This test add 128 value remove every y elements check the map,
    // then readd the removed elements and re check again the map by iterating over values
    // Key collided every odd number
    const auto test = [](usize y)
    {
        // Remove elements then add it again
        hashmap_type map_1_x;
        bool ok_map_1_x = true;
        for (usize x = 0; x < 64; x++)
        {
            map_1_x.add({x, x * x});
            ok_map_1_x &= map_1_x.count() == (x + 1);
            ok_map_1_x &= map_1_x.max_count() >= (x + 1);
            ok_map_1_x &= map_1_x.slack() >= 0;
        }

        // Check we really have all values
        for (usize x = 0; x < 64; x++)
        {
            const auto it = map_1_x.find(x);
            if (it != map_1_x.end())
            {
                const auto &[key, value] = *it;
                ok_map_1_x &= (key == x);
                ok_map_1_x &= (value == (x * x));
            }
        }

        // Remove every element 2 elements
        for (usize x = 0; x < 64; x += y)
        {
            map_1_x.remove(x);
            ok_map_1_x &= !map_1_x.contains(x);
        }
        // Add removed element
        for (usize x = 0; x < 64; x += y)
        {
            map_1_x.add({x, x * x});
        }
        // Check we really have all values
        for (usize x = 0; x < 64; x++)
        {
            const auto it = map_1_x.find(x);
            if (it != map_1_x.end())
            {
                const auto &[key, value] = *it;
                ok_map_1_x &= (key == x);
                ok_map_1_x &= (value == (x * x));
            }
        }
        ok_map_1_x &= map_1_x.count() == 64;
        ok_map_1_x &= map_1_x.max_count() >= 64;
        ok_map_1_x &= map_1_x.slack() >= 0;

        // Check we can still iterate
        // This works because the hash of an interger is always the value of the integer,
        // the order of key is kept in order du to the hash
        usize x = 0;
        for (const auto &[key, value] : map_1_x)
        {
            ok_map_1_x &= (key == x);
            ok_map_1_x &= (value == (x * x));
            x++;
        }

        return std::tuple {
            ok_map_1_x
        };
    };

    // Non constant
    {
        const auto result_1 = test(1);
        hud_assert_true(std::get<0>(result_1));

        const auto result_2 = test(2);
        hud_assert_true(std::get<0>(result_2));

        const auto result_3 = test(3);
        hud_assert_true(std::get<0>(result_3));

        const auto result_4 = test(4);
        hud_assert_true(std::get<0>(result_4));

        const auto result_5 = test(5);
        hud_assert_true(std::get<0>(result_5));

        const auto result_10 = test(10);
        hud_assert_true(std::get<0>(result_10));
    }

    // Constant
    {
        constexpr auto result_1 = test(1);
        hud_assert_true(std::get<0>(result_1));

        constexpr auto result_2 = test(2);
        hud_assert_true(std::get<0>(result_2));

        constexpr auto result_3 = test(3);
        hud_assert_true(std::get<0>(result_3));

        constexpr auto result_4 = test(4);
        hud_assert_true(std::get<0>(result_4));

        constexpr auto result_5 = test(5);
        hud_assert_true(std::get<0>(result_5));

        constexpr auto result_10 = test(10);
        hud_assert_true(std::get<0>(result_10));
    }
}