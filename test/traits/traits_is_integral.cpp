#include <core/traits/is_integral.h>

namespace hud_test
{
    class a;
    enum class b : u32;
} // namespace hud_test

GTEST_TEST(traits, is_integral)
{

    hud_assert_false((hud::is_integral_v<hud_test::a>));
    hud_assert_false((hud::is_integral_v<hud_test::b>));
    hud_assert_false((hud::is_integral_v<f64>));
    hud_assert_false((hud::is_integral_v<f32>));

    hud_assert_true((hud::is_integral_v<bool>));
    hud_assert_true((hud::is_integral_v<char>));
    hud_assert_true((hud::is_integral_v<ansichar>));
    hud_assert_true((hud::is_integral_v<char16_t>));
    hud_assert_true((hud::is_integral_v<char32_t>));
    hud_assert_true((hud::is_integral_v<char16_t>));
    hud_assert_true((hud::is_integral_v<wchar_t>));
    hud_assert_true((hud::is_integral_v<wchar>));
    hud_assert_true((hud::is_integral_v<signed char>));
    hud_assert_true((hud::is_integral_v<i8>));
    hud_assert_true((hud::is_integral_v<short int>));
    hud_assert_true((hud::is_integral_v<i16>));
    hud_assert_true((hud::is_integral_v<int>));
    hud_assert_true((hud::is_integral_v<i32>));
    hud_assert_true((hud::is_integral_v<long int>));
    hud_assert_true((hud::is_integral_v<i64>));
    hud_assert_true((hud::is_integral_v<long long int>));
    hud_assert_true((hud::is_integral_v<i64>));
    hud_assert_true((hud::is_integral_v<long long>));
    hud_assert_true((hud::is_integral_v<i64>));
    hud_assert_true((hud::is_integral_v<unsigned char>));
    hud_assert_true((hud::is_integral_v<u8>));
    hud_assert_true((hud::is_integral_v<unsigned short int>));
    hud_assert_true((hud::is_integral_v<u16>));
    hud_assert_true((hud::is_integral_v<unsigned int>));
    hud_assert_true((hud::is_integral_v<u32>));
    hud_assert_true((hud::is_integral_v<unsigned long int>));
    hud_assert_true((hud::is_integral_v<u64>));
    hud_assert_true((hud::is_integral_v<unsigned long long int>));
    hud_assert_true((hud::is_integral_v<u64>));

    hud_assert_true((hud::is_integral_v<const bool>));
    hud_assert_true((hud::is_integral_v<const char>));
    hud_assert_true((hud::is_integral_v<const ansichar>));
    hud_assert_true((hud::is_integral_v<const char16_t>));
    hud_assert_true((hud::is_integral_v<const char32_t>));
    hud_assert_true((hud::is_integral_v<const char16_t>));
    hud_assert_true((hud::is_integral_v<const wchar_t>));
    hud_assert_true((hud::is_integral_v<const wchar>));
    hud_assert_true((hud::is_integral_v<const signed char>));
    hud_assert_true((hud::is_integral_v<const i8>));
    hud_assert_true((hud::is_integral_v<const short int>));
    hud_assert_true((hud::is_integral_v<const i16>));
    hud_assert_true((hud::is_integral_v<const int>));
    hud_assert_true((hud::is_integral_v<const i32>));
    hud_assert_true((hud::is_integral_v<const long int>));
    hud_assert_true((hud::is_integral_v<const i64>));
    hud_assert_true((hud::is_integral_v<const long long int>));
    hud_assert_true((hud::is_integral_v<const i64>));
    hud_assert_true((hud::is_integral_v<const long long>));
    hud_assert_true((hud::is_integral_v<const i64>));
    hud_assert_true((hud::is_integral_v<const unsigned char>));
    hud_assert_true((hud::is_integral_v<const u8>));
    hud_assert_true((hud::is_integral_v<const unsigned short int>));
    hud_assert_true((hud::is_integral_v<const u16>));
    hud_assert_true((hud::is_integral_v<const unsigned int>));
    hud_assert_true((hud::is_integral_v<const u32>));
    hud_assert_true((hud::is_integral_v<const unsigned long int>));
    hud_assert_true((hud::is_integral_v<const u64>));
    hud_assert_true((hud::is_integral_v<const unsigned long long int>));
    hud_assert_true((hud::is_integral_v<const u64>));

    hud_assert_true((hud::is_integral_v<const volatile bool>));
    hud_assert_true((hud::is_integral_v<const volatile char>));
    hud_assert_true((hud::is_integral_v<const volatile ansichar>));
    hud_assert_true((hud::is_integral_v<const volatile char16_t>));
    hud_assert_true((hud::is_integral_v<const volatile char32_t>));
    hud_assert_true((hud::is_integral_v<const volatile char16_t>));
    hud_assert_true((hud::is_integral_v<const volatile wchar_t>));
    hud_assert_true((hud::is_integral_v<const volatile wchar>));
    hud_assert_true((hud::is_integral_v<const volatile signed char>));
    hud_assert_true((hud::is_integral_v<const volatile i8>));
    hud_assert_true((hud::is_integral_v<const volatile short int>));
    hud_assert_true((hud::is_integral_v<const volatile i16>));
    hud_assert_true((hud::is_integral_v<const volatile int>));
    hud_assert_true((hud::is_integral_v<const volatile i32>));
    hud_assert_true((hud::is_integral_v<const volatile long int>));
    hud_assert_true((hud::is_integral_v<const volatile i64>));
    hud_assert_true((hud::is_integral_v<const volatile long long int>));
    hud_assert_true((hud::is_integral_v<const volatile i64>));
    hud_assert_true((hud::is_integral_v<const volatile long long>));
    hud_assert_true((hud::is_integral_v<const volatile i64>));
    hud_assert_true((hud::is_integral_v<const volatile unsigned char>));
    hud_assert_true((hud::is_integral_v<const volatile u8>));
    hud_assert_true((hud::is_integral_v<const volatile unsigned short int>));
    hud_assert_true((hud::is_integral_v<const volatile u16>));
    hud_assert_true((hud::is_integral_v<const volatile unsigned int>));
    hud_assert_true((hud::is_integral_v<const volatile u32>));
    hud_assert_true((hud::is_integral_v<const volatile unsigned long int>));
    hud_assert_true((hud::is_integral_v<const volatile u64>));
    hud_assert_true((hud::is_integral_v<const volatile unsigned long long int>));
    hud_assert_true((hud::is_integral_v<const volatile u64>));

    hud_assert_true((hud::is_integral_v<volatile bool>));
    hud_assert_true((hud::is_integral_v<volatile char>));
    hud_assert_true((hud::is_integral_v<volatile ansichar>));
    hud_assert_true((hud::is_integral_v<volatile char16_t>));
    hud_assert_true((hud::is_integral_v<volatile char32_t>));
    hud_assert_true((hud::is_integral_v<volatile char16_t>));
    hud_assert_true((hud::is_integral_v<volatile wchar_t>));
    hud_assert_true((hud::is_integral_v<volatile wchar>));
    hud_assert_true((hud::is_integral_v<volatile signed char>));
    hud_assert_true((hud::is_integral_v<volatile i8>));
    hud_assert_true((hud::is_integral_v<volatile short int>));
    hud_assert_true((hud::is_integral_v<volatile i16>));
    hud_assert_true((hud::is_integral_v<volatile int>));
    hud_assert_true((hud::is_integral_v<volatile i32>));
    hud_assert_true((hud::is_integral_v<volatile long int>));
    hud_assert_true((hud::is_integral_v<volatile i64>));
    hud_assert_true((hud::is_integral_v<volatile long long int>));
    hud_assert_true((hud::is_integral_v<volatile i64>));
    hud_assert_true((hud::is_integral_v<volatile long long>));
    hud_assert_true((hud::is_integral_v<volatile i64>));
    hud_assert_true((hud::is_integral_v<volatile unsigned char>));
    hud_assert_true((hud::is_integral_v<volatile u8>));
    hud_assert_true((hud::is_integral_v<volatile unsigned short int>));
    hud_assert_true((hud::is_integral_v<volatile u16>));
    hud_assert_true((hud::is_integral_v<volatile unsigned int>));
    hud_assert_true((hud::is_integral_v<volatile u32>));
    hud_assert_true((hud::is_integral_v<volatile unsigned long int>));
    hud_assert_true((hud::is_integral_v<volatile u64>));
    hud_assert_true((hud::is_integral_v<volatile unsigned long long int>));
    hud_assert_true((hud::is_integral_v<volatile u64>));
}
