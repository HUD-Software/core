#include <core/traits/is_unsigned.h>

namespace hud_test
{
    struct a
    {
    };
} // namespace hud_test

GTEST_TEST(traits, is_unsigned)
{
    hud_assert_true((hud::is_unsigned_v<bool>));
    hud_assert_false((hud::is_unsigned_v<i8>));
    hud_assert_false((hud::is_unsigned_v<i16>));
    hud_assert_false((hud::is_unsigned_v<i32>));
    hud_assert_false((hud::is_unsigned_v<i64>));
    hud_assert_false((hud::is_unsigned_v<iptr>));
    hud_assert_false((hud::is_unsigned_v<f32>));
    hud_assert_false((hud::is_unsigned_v<f64>));

    hud_assert_true((hud::is_unsigned_v<u8>));
    hud_assert_true((hud::is_unsigned_v<u16>));
    hud_assert_true((hud::is_unsigned_v<u32>));
    hud_assert_true((hud::is_unsigned_v<u64>));
    hud_assert_true((hud::is_unsigned_v<uptr>));

    hud_assert_false((hud::is_unsigned_v<isize>));
    hud_assert_true((hud::is_unsigned_v<usize>));
    hud_assert_true((hud::is_unsigned_v<char16>));
    hud_assert_true((hud::is_unsigned_v<char32>));
    hud_assert_false((hud::is_unsigned_v<char8>));
#if defined(HD_HOST_WINDOWS)
    hud_assert_true((hud::is_unsigned_v<wchar>));
#else // HD_HOST_LINUX
    hud_assert_false((hud::is_unsigned_v<wchar>));
#endif
    hud_assert_false((hud::is_unsigned_v<hud_test::a>));
}
