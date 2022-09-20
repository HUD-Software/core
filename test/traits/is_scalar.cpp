#include <core/traits/is_scalar.h>

namespace {
    struct a { void fn() {} };
    enum class e {};
}

TEST(traits, is_scalar) {
    ASSERT_FALSE((hud::is_scalar_v<a>));
    ASSERT_TRUE((hud::is_scalar_v<e>));
    ASSERT_TRUE((hud::is_scalar_v<i8>));
    ASSERT_TRUE((hud::is_scalar_v<i16>));
    ASSERT_TRUE((hud::is_scalar_v<i32>));
    ASSERT_TRUE((hud::is_scalar_v<i64>));
    ASSERT_TRUE((hud::is_scalar_v<iptr>));
    ASSERT_TRUE((hud::is_scalar_v<u8>));
    ASSERT_TRUE((hud::is_scalar_v<u16>));
    ASSERT_TRUE((hud::is_scalar_v<u32>));
    ASSERT_TRUE((hud::is_scalar_v<u64>));
    ASSERT_TRUE((hud::is_scalar_v<uptr>));
    ASSERT_TRUE((hud::is_scalar_v<isize>));
    ASSERT_TRUE((hud::is_scalar_v<usize>));
    ASSERT_TRUE((hud::is_scalar_v<ansichar>));
    ASSERT_TRUE((hud::is_scalar_v<wchar>));
    ASSERT_TRUE((hud::is_scalar_v<a*>));
    ASSERT_TRUE((hud::is_scalar_v<decltype(&a::fn)>));
    ASSERT_FALSE((hud::is_scalar_v<int(int)>));
}