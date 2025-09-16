#include <core/traits/is_scalar.h>

namespace hud_test
{
    struct a
    {
        void fn()
        {
        }
    };
    enum class e
    {
    };
} // namespace hud_test

GTEST_TEST(traits, is_scalar)
{
    hud_assert_false((hud::is_scalar_v<hud_test::a>));
    hud_assert_true((hud::is_scalar_v<hud_test::e>));
    hud_assert_true((hud::is_scalar_v<i8>));
    hud_assert_true((hud::is_scalar_v<i16>));
    hud_assert_true((hud::is_scalar_v<i32>));
    hud_assert_true((hud::is_scalar_v<i64>));
    hud_assert_true((hud::is_scalar_v<iptr>));
    hud_assert_true((hud::is_scalar_v<u8>));
    hud_assert_true((hud::is_scalar_v<u16>));
    hud_assert_true((hud::is_scalar_v<u32>));
    hud_assert_true((hud::is_scalar_v<u64>));
    hud_assert_true((hud::is_scalar_v<uptr>));
    hud_assert_true((hud::is_scalar_v<isize>));
    hud_assert_true((hud::is_scalar_v<usize>));
    hud_assert_true((hud::is_scalar_v<char8>));
    hud_assert_true((hud::is_scalar_v<wchar>));
    hud_assert_true((hud::is_scalar_v<hud_test::a *>));
    hud_assert_true((hud::is_scalar_v<decltype(&hud_test::a::fn)>));
    hud_assert_false((hud::is_scalar_v<int(int)>));
}