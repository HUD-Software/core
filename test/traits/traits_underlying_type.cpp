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
    enum class e10 : ansichar;
    enum class e11 : wchar;
    enum class e12 : char16;
    enum class e13 : char32;
    enum class e14 : iptr;
    enum class e15 : uptr;
    enum class e16 : isize;
    enum class e17 : usize;

    enum class e2c : const i8;
    enum class e3c : const i16;
    enum class e4c : const i32;
    enum class e5c : const i64;
    enum class e6c : const u8;
    enum class e7c : const u16;
    enum class e8c : const u32;
    enum class e9c : const u64;
    enum class e10c : const ansichar;
    enum class e11c : const wchar;
    enum class e12c : const char16;
    enum class e13c : const char32;
    enum class e14c : const iptr;
    enum class e15c : const uptr;
    enum class e16c : const isize;
    enum class e17c : const usize;

    enum class e2v : volatile i8;
    enum class e3v : volatile i16;
    enum class e4v : volatile i32;
    enum class e5v : volatile i64;
    enum class e6v : volatile u8;
    enum class e7v : volatile u16;
    enum class e8v : volatile u32;
    enum class e9v : volatile u64;
    enum class e10v : volatile ansichar;
    enum class e11v : volatile wchar;
    enum class e12v : volatile char16;
    enum class e13v : volatile char32;
    enum class e14v : volatile iptr;
    enum class e15v : volatile uptr;
    enum class e16v : volatile isize;
    enum class e17v : volatile usize;

    enum class e2cv : const volatile i8;
    enum class e3cv : const volatile i16;
    enum class e4cv : const volatile i32;
    enum class e5cv : const volatile i64;
    enum class e6cv : const volatile u8;
    enum class e7cv : const volatile u16;
    enum class e8cv : const volatile u32;
    enum class e9cv : const volatile u64;
    enum class e10cv : const volatile ansichar;
    enum class e11cv : const volatile wchar;
    enum class e12cv : const volatile char16;
    enum class e13cv : const volatile char32;
    enum class e14cv : const volatile iptr;
    enum class e15cv : const volatile uptr;
    enum class e16cv : const volatile isize;
    enum class e17cv : const volatile usize;
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
    hud_assert_true((hud::is_same_v<hud::underlying_type_t<hud_test::e10>, ansichar>));
    hud_assert_true((hud::is_same_v<hud::underlying_type_t<hud_test::e11>, wchar>));
    hud_assert_true((hud::is_same_v<hud::underlying_type_t<hud_test::e12>, char16>));
    hud_assert_true((hud::is_same_v<hud::underlying_type_t<hud_test::e13>, char32>));
    hud_assert_true((hud::is_same_v<hud::underlying_type_t<hud_test::e14>, iptr>));
    hud_assert_true((hud::is_same_v<hud::underlying_type_t<hud_test::e15>, uptr>));
    hud_assert_true((hud::is_same_v<hud::underlying_type_t<hud_test::e16>, isize>));
    hud_assert_true((hud::is_same_v<hud::underlying_type_t<hud_test::e17>, usize>));

    hud_assert_true((hud::is_same_v<hud::underlying_type_t<hud_test::e2c>, i8>));
    hud_assert_true((hud::is_same_v<hud::underlying_type_t<hud_test::e3c>, i16>));
    hud_assert_true((hud::is_same_v<hud::underlying_type_t<hud_test::e4c>, i32>));
    hud_assert_true((hud::is_same_v<hud::underlying_type_t<hud_test::e5c>, i64>));
    hud_assert_true((hud::is_same_v<hud::underlying_type_t<hud_test::e6c>, u8>));
    hud_assert_true((hud::is_same_v<hud::underlying_type_t<hud_test::e7c>, u16>));
    hud_assert_true((hud::is_same_v<hud::underlying_type_t<hud_test::e8c>, u32>));
    hud_assert_true((hud::is_same_v<hud::underlying_type_t<hud_test::e9c>, u64>));
    hud_assert_true((hud::is_same_v<hud::underlying_type_t<hud_test::e10c>, ansichar>));
    hud_assert_true((hud::is_same_v<hud::underlying_type_t<hud_test::e11c>, wchar>));
    hud_assert_true((hud::is_same_v<hud::underlying_type_t<hud_test::e12c>, char16>));
    hud_assert_true((hud::is_same_v<hud::underlying_type_t<hud_test::e13c>, char32>));
    hud_assert_true((hud::is_same_v<hud::underlying_type_t<hud_test::e14c>, iptr>));
    hud_assert_true((hud::is_same_v<hud::underlying_type_t<hud_test::e15c>, uptr>));
    hud_assert_true((hud::is_same_v<hud::underlying_type_t<hud_test::e16c>, isize>));
    hud_assert_true((hud::is_same_v<hud::underlying_type_t<hud_test::e17c>, usize>));

    hud_assert_true((hud::is_same_v<hud::underlying_type_t<hud_test::e2v>, i8>));
    hud_assert_true((hud::is_same_v<hud::underlying_type_t<hud_test::e3v>, i16>));
    hud_assert_true((hud::is_same_v<hud::underlying_type_t<hud_test::e4v>, i32>));
    hud_assert_true((hud::is_same_v<hud::underlying_type_t<hud_test::e5v>, i64>));
    hud_assert_true((hud::is_same_v<hud::underlying_type_t<hud_test::e6v>, u8>));
    hud_assert_true((hud::is_same_v<hud::underlying_type_t<hud_test::e7v>, u16>));
    hud_assert_true((hud::is_same_v<hud::underlying_type_t<hud_test::e8v>, u32>));
    hud_assert_true((hud::is_same_v<hud::underlying_type_t<hud_test::e9v>, u64>));
    hud_assert_true((hud::is_same_v<hud::underlying_type_t<hud_test::e10v>, ansichar>));
    hud_assert_true((hud::is_same_v<hud::underlying_type_t<hud_test::e11v>, wchar>));
    hud_assert_true((hud::is_same_v<hud::underlying_type_t<hud_test::e12v>, char16>));
    hud_assert_true((hud::is_same_v<hud::underlying_type_t<hud_test::e13v>, char32>));
    hud_assert_true((hud::is_same_v<hud::underlying_type_t<hud_test::e14v>, iptr>));
    hud_assert_true((hud::is_same_v<hud::underlying_type_t<hud_test::e15v>, uptr>));
    hud_assert_true((hud::is_same_v<hud::underlying_type_t<hud_test::e16v>, isize>));
    hud_assert_true((hud::is_same_v<hud::underlying_type_t<hud_test::e17v>, usize>));

    hud_assert_true((hud::is_same_v<hud::underlying_type_t<hud_test::e2cv>, i8>));
    hud_assert_true((hud::is_same_v<hud::underlying_type_t<hud_test::e3cv>, i16>));
    hud_assert_true((hud::is_same_v<hud::underlying_type_t<hud_test::e4cv>, i32>));
    hud_assert_true((hud::is_same_v<hud::underlying_type_t<hud_test::e5cv>, i64>));
    hud_assert_true((hud::is_same_v<hud::underlying_type_t<hud_test::e6cv>, u8>));
    hud_assert_true((hud::is_same_v<hud::underlying_type_t<hud_test::e7cv>, u16>));
    hud_assert_true((hud::is_same_v<hud::underlying_type_t<hud_test::e8cv>, u32>));
    hud_assert_true((hud::is_same_v<hud::underlying_type_t<hud_test::e9cv>, u64>));
    hud_assert_true((hud::is_same_v<hud::underlying_type_t<hud_test::e10cv>, ansichar>));
    hud_assert_true((hud::is_same_v<hud::underlying_type_t<hud_test::e11cv>, wchar>));
    hud_assert_true((hud::is_same_v<hud::underlying_type_t<hud_test::e12cv>, char16>));
    hud_assert_true((hud::is_same_v<hud::underlying_type_t<hud_test::e13cv>, char32>));
    hud_assert_true((hud::is_same_v<hud::underlying_type_t<hud_test::e14cv>, iptr>));
    hud_assert_true((hud::is_same_v<hud::underlying_type_t<hud_test::e15cv>, uptr>));
    hud_assert_true((hud::is_same_v<hud::underlying_type_t<hud_test::e16cv>, isize>));
    hud_assert_true((hud::is_same_v<hud::underlying_type_t<hud_test::e17cv>, usize>));
}