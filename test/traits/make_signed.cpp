#include <core/traits/make_signed.h>

namespace {
    enum class a;
    enum class b_char : char;
    enum class b_const_char : const char;
    enum class b_volatile_char : volatile char;
    enum class b_const_volatile_char : const volatile char;
    enum class c : signed char;
    enum class d : short;
    enum class e : int;
    enum class f : long;
    enum class g : long long;
    enum class h : unsigned char;
    enum class i : unsigned short;
    enum class j : unsigned int;
    enum class k : unsigned long;
    enum class l : unsigned long long;
    enum class m : wchar_t;
    enum class n : char16_t;
    enum class o : char32_t;
    enum class p : char8_t;

    enum class q : i8;
    enum class r : i16;
    enum class s : i32;
    enum class t : i64;
    enum class u : u8;
    enum class v : u16;
    enum class w : u32;
    enum class x : u64;
    enum class y : ansichar;
    enum class z : wchar;
    enum class a_1 : char16;
    enum class b_1 : char32;
    enum class c_1 : iptr;
    enum class d_1 : uptr;
    enum class e_1 : usize;
}

TEST(traits, make_signed) {
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<char>, signed char>));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<signed char>, signed char>));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<short>, signed short>));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<int>, signed int>));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<long>, signed long>));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<long long>, signed long long>));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<const char>, const signed char>));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<const signed char>, const signed char>));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<const short>, const signed short>));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<const int>, const signed int>));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<const long>, const signed long>));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<const long long>, const signed long long>));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<volatile char>, volatile signed char>));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<volatile signed char>, volatile signed char>));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<volatile short>, volatile signed short>));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<volatile int>, volatile signed int>));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<volatile long>, volatile signed long>));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<volatile long long>, volatile signed long long>));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<const volatile char>, const volatile signed char>));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<const volatile signed char>, const volatile signed char>));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<const volatile short>, const volatile signed short>));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<const volatile int>, const volatile signed int>));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<const volatile long>, const volatile signed long>));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<const volatile long long>, const volatile signed long long>));

    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<unsigned char>, signed char>));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<unsigned short>, signed short>));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<unsigned int>, signed int>));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<unsigned long>, signed long>));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<unsigned long long>, signed long long >));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<const unsigned char>, const signed char>));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<const unsigned short>, const signed short>));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<const unsigned int>, const signed int>));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<const unsigned long>, const signed long>));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<const unsigned long long>, const signed long long >));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<volatile unsigned char>, volatile signed char>));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<volatile unsigned short>, volatile signed short>));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<volatile unsigned int>, volatile signed int>));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<volatile unsigned long>, volatile signed long>));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<volatile unsigned long long>, volatile signed long long >));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<const volatile unsigned char>, const volatile signed char>));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<const volatile unsigned short>, const volatile signed short>));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<const volatile unsigned int>, const volatile signed int>));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<const volatile unsigned long>, const volatile signed long>));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<const volatile unsigned long long>, const volatile signed long long >));

    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<i8>, signed char>));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<i16>, i16>));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<i32>, i32>));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<i64>, i64>));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<u8>, signed char>));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<u16>, i16>));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<u32>, i32>));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<u64>, i64>));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<ansichar>, signed char>));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<wchar>, hud::conditional_t< sizeof(wchar) == 2, i16, i32> >));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<char16>, i16>));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<char32>, i32>));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<iptr>, iptr>));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<uptr>, iptr>));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<usize>, hud::conditional_t< sizeof(void*) == 4, i32, i64>>));

    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<a>, signed int>));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<b_char>, signed char>));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<b_const_char>, signed char>));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<b_volatile_char>, signed char>));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<b_const_volatile_char>, signed char>));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<c>, signed char>));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<d>, signed short>));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<e>, signed int>));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<f>, signed long>));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<g>, signed long long>));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<h>, signed char>));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<i>, signed short>));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<j>, signed int >));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<k>, signed long >));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<l>, signed long long>));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<m>, hud::conditional_t< sizeof(wchar_t) == 2, signed short, signed int> >));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<n>, signed short>));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<o>, signed int>));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<p>, signed char>));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<q>, signed char>));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<r>, i16>));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<s>, i32>));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<t>, i64>));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<u>, signed char>));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<v>, i16>));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<w>, i32>));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<x>, i64>));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<y>, signed char>));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<z>, hud::conditional_t< sizeof(wchar) == 2, i16, i32>>));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<a_1>, i16>));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<b_1>, i32>));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<c_1>, iptr>));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<d_1>, hud::conditional_t< sizeof(void*) == 4, i32, i64>>));
    ASSERT_TRUE((hud::is_same_v<hud::make_signed_t<e_1>, hud::conditional_t< sizeof(void*) == 4, i32, i64>>));
}
