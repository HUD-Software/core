#include <core/containers/tuple.h>
#include <core/hash.h>

GTEST_TEST(tuple, hash_empty_tuple)
{
    const auto test = []()
    {
        using TupleType = hud::tuple<>;
        return std::tuple {
            hud::hash_32<TupleType> {}(TupleType {}) == 0,
            hud::hash_64<TupleType> {}(TupleType {}) == 0
        };
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
    }
}

GTEST_TEST(tuple, hash_tuple)
{
    const auto test = []()
    {
        using TupleType = hud::tuple<i32, hud_test::non_bitwise_type, const char *>;
        u32 result_hash32 = hud::hash_32<TupleType> {}(TupleType {1, 2, "hello"});
        hud::hasher_32 hasher32;
        hasher32.hash(i32 {1});
        hasher32.hash(hud_test::non_bitwise_type {2});
        hasher32.hash("hello");

        u64 result_hash64 = hud::hash_64<TupleType> {}(TupleType {1, 2, "hello"});
        hud::hasher_64 hasher64;
        hasher64.hash(i32 {1});
        hasher64.hash(hud_test::non_bitwise_type {2});
        hasher64.hash("hello");
        return std::tuple {
            result_hash32 == hasher32.result(),
            result_hash64 == hasher64.result()
        };
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
    }
}