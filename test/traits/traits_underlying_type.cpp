#include <core/traits/underlying_type.h>

namespace hud_test
{
    enum e1
    {
    };
    enum class e2 : i8;
    enum class e3 : i16;
    enum class e4 : i32;
    enum class e5 : i64;
    enum class e6 : u8;
    enum class e7 : u16;
    enum class e8 : u32;
    enum class e9 : u64;
    enum class e10 : char8;
    enum class e11 : wchar;
    enum class e12 : char16;
    enum class e13 : char32;
    enum class e14 : iptr;
    enum class e15 : uptr;
    enum class e16 : isize;
    enum class e17 : usize;

} // namespace hud_test

GTEST_TEST(traits, underlying_type)
{
// MSVC and ClangCL promote the enum an signed int when GCC and Clang promote it to unsigned int
#if defined(HD_COMPILER_MSVC) || defined(HD_COMPILER_CLANG_CL)
    hud_assert_true((hud::is_same_v<hud::underlying_type_t<hud_test::e1>, i32>));
#else
    hud_assert_true((hud::is_same_v<hud::underlying_type_t<hud_test::e1>, u32>));
#endif
    hud_assert_true((hud::is_same_v<hud::underlying_type_t<hud_test::e2>, i8>));
    hud_assert_true((hud::is_same_v<hud::underlying_type_t<hud_test::e3>, i16>));
    hud_assert_true((hud::is_same_v<hud::underlying_type_t<hud_test::e4>, i32>));
    hud_assert_true((hud::is_same_v<hud::underlying_type_t<hud_test::e5>, i64>));
    hud_assert_true((hud::is_same_v<hud::underlying_type_t<hud_test::e6>, u8>));
    hud_assert_true((hud::is_same_v<hud::underlying_type_t<hud_test::e7>, u16>));
    hud_assert_true((hud::is_same_v<hud::underlying_type_t<hud_test::e8>, u32>));
    hud_assert_true((hud::is_same_v<hud::underlying_type_t<hud_test::e9>, u64>));
    hud_assert_true((hud::is_same_v<hud::underlying_type_t<hud_test::e10>, char8>));
    hud_assert_true((hud::is_same_v<hud::underlying_type_t<hud_test::e11>, wchar>));
    hud_assert_true((hud::is_same_v<hud::underlying_type_t<hud_test::e12>, char16>));
    hud_assert_true((hud::is_same_v<hud::underlying_type_t<hud_test::e13>, char32>));
    hud_assert_true((hud::is_same_v<hud::underlying_type_t<hud_test::e14>, iptr>));
    hud_assert_true((hud::is_same_v<hud::underlying_type_t<hud_test::e15>, uptr>));
    hud_assert_true((hud::is_same_v<hud::underlying_type_t<hud_test::e16>, isize>));
    hud_assert_true((hud::is_same_v<hud::underlying_type_t<hud_test::e17>, usize>));
}