#include <core/traits/is_integral.h>

namespace hud_test
{
    class a;
    enum class b : u32;
}

GTEST_TEST(traits, is_integral)
{

    GTEST_ASSERT_FALSE((hud::is_integral_v<hud_test::a>));
    GTEST_ASSERT_FALSE((hud::is_integral_v<hud_test::b>));
    GTEST_ASSERT_FALSE((hud::is_integral_v<f64>));
    GTEST_ASSERT_FALSE((hud::is_integral_v<f32>));

    GTEST_ASSERT_TRUE((hud::is_integral_v<bool>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<char>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<ansichar>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<char16_t>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<char32_t>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<char16_t>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<wchar_t>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<wchar>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<signed char>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<i8>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<short int>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<i16>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<int>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<i32>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<long int>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<i64>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<long long int>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<i64>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<long long>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<i64>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<unsigned char>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<u8>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<unsigned short int>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<u16>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<unsigned int>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<u32>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<unsigned long int>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<u64>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<unsigned long long int>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<u64>));

    GTEST_ASSERT_TRUE((hud::is_integral_v<const bool>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<const char>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<const ansichar>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<const char16_t>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<const char32_t>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<const char16_t>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<const wchar_t>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<const wchar>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<const signed char>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<const i8>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<const short int>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<const i16>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<const int>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<const i32>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<const long int>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<const i64>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<const long long int>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<const i64>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<const long long>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<const i64>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<const unsigned char>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<const u8>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<const unsigned short int>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<const u16>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<const unsigned int>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<const u32>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<const unsigned long int>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<const u64>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<const unsigned long long int>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<const u64>));

    GTEST_ASSERT_TRUE((hud::is_integral_v<const volatile bool>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<const volatile char>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<const volatile ansichar>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<const volatile char16_t>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<const volatile char32_t>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<const volatile char16_t>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<const volatile wchar_t>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<const volatile wchar>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<const volatile signed char>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<const volatile i8>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<const volatile short int>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<const volatile i16>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<const volatile int>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<const volatile i32>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<const volatile long int>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<const volatile i64>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<const volatile long long int>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<const volatile i64>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<const volatile long long>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<const volatile i64>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<const volatile unsigned char>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<const volatile u8>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<const volatile unsigned short int>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<const volatile u16>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<const volatile unsigned int>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<const volatile u32>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<const volatile unsigned long int>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<const volatile u64>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<const volatile unsigned long long int>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<const volatile u64>));

    GTEST_ASSERT_TRUE((hud::is_integral_v<volatile bool>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<volatile char>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<volatile ansichar>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<volatile char16_t>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<volatile char32_t>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<volatile char16_t>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<volatile wchar_t>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<volatile wchar>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<volatile signed char>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<volatile i8>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<volatile short int>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<volatile i16>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<volatile int>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<volatile i32>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<volatile long int>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<volatile i64>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<volatile long long int>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<volatile i64>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<volatile long long>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<volatile i64>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<volatile unsigned char>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<volatile u8>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<volatile unsigned short int>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<volatile u16>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<volatile unsigned int>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<volatile u32>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<volatile unsigned long int>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<volatile u64>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<volatile unsigned long long int>));
    GTEST_ASSERT_TRUE((hud::is_integral_v<volatile u64>));
}
