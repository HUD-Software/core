#include <core/containers/pair.h>
#include <core/hash.h>

GTEST_TEST(pair, hash_pair)
{
    const auto test = []()
    {
        using PairType = hud::pair<i32, hud_test::non_bitwise_type>;
        u32 result_hash32 = hud::hash_32<PairType> {}(PairType {1, 2});
        hud::hasher_32 hasher32;
        hasher32.hash(i32 {1});
        hasher32.hash(hud_test::non_bitwise_type {2});

        u64 result_hash64 = hud::hash_64<PairType> {}(PairType {1, 2});
        hud::hasher_64 hasher64;
        hasher64.hash(i32 {1});
        hasher64.hash(hud_test::non_bitwise_type {2});
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