#include <core/traits/make_unsigned.h>

namespace hud_test
{
    enum class a;
    enum class B_char : char;
    enum class B_const_char : const char;
    enum class B_volatile_char : volatile char;
    enum class B_const_volatile_char : const volatile char;
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
    enum class y : char8;
    enum class z : wchar;
    enum class a_1 : char16;
    enum class b_1 : char32;
    enum class c_1 : iptr;
    enum class d_1 : uptr;
    enum class e_1 : usize;
} // namespace hud_test

GTEST_TEST(traits, make_unsigned)
{
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<char>, unsigned char>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<signed char>, unsigned char>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<short>, unsigned short>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<int>, unsigned int>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<long>, unsigned long>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<long long>, unsigned long long>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<const char>, const unsigned char>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<const signed char>, const unsigned char>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<const short>, const unsigned short>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<const int>, const unsigned int>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<const long>, const unsigned long>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<const long long>, const unsigned long long>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<volatile char>, volatile unsigned char>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<volatile signed char>, volatile unsigned char>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<volatile short>, volatile unsigned short>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<volatile int>, volatile unsigned int>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<volatile long>, volatile unsigned long>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<volatile long long>, volatile unsigned long long>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<const volatile char>, const volatile unsigned char>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<const volatile signed char>, const volatile unsigned char>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<const volatile short>, const volatile unsigned short>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<const volatile int>, const volatile unsigned int>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<const volatile long>, const volatile unsigned long>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<const volatile long long>, const volatile unsigned long long>));

    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<unsigned char>, unsigned char>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<unsigned short>, unsigned short>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<unsigned int>, unsigned int>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<unsigned long>, unsigned long>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<unsigned long long>, unsigned long long>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<const unsigned char>, const unsigned char>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<const unsigned short>, const unsigned short>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<const unsigned int>, const unsigned int>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<const unsigned long>, const unsigned long>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<const unsigned long long>, const unsigned long long>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<volatile unsigned char>, volatile unsigned char>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<volatile unsigned short>, volatile unsigned short>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<volatile unsigned int>, volatile unsigned int>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<volatile unsigned long>, volatile unsigned long>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<volatile unsigned long long>, volatile unsigned long long>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<const volatile unsigned char>, const volatile unsigned char>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<const volatile unsigned short>, const volatile unsigned short>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<const volatile unsigned int>, const volatile unsigned int>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<const volatile unsigned long>, const volatile unsigned long>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<const volatile unsigned long long>, const volatile unsigned long long>));

    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<i8>, u8>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<i16>, u16>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<i32>, u32>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<i64>, u64>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<u8>, u8>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<u16>, u16>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<u32>, u32>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<u64>, u64>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<char8>, u8>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<wchar>, hud::conditional_t<sizeof(wchar) == 2, u16, u32>>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<char16>, u16>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<char32>, u32>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<iptr>, uptr>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<uptr>, uptr>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<isize>, hud::conditional_t<sizeof(void *) == 4, u32, u64>>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<usize>, hud::conditional_t<sizeof(void *) == 4, u32, u64>>));

    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<hud_test::a>, unsigned int>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<hud_test::B_char>, unsigned char>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<hud_test::B_const_char>, unsigned char>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<hud_test::B_volatile_char>, unsigned char>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<hud_test::B_const_volatile_char>, unsigned char>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<hud_test::c>, unsigned char>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<hud_test::d>, unsigned short>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<hud_test::e>, unsigned int>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<hud_test::f>, unsigned long>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<hud_test::g>, unsigned long long>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<hud_test::h>, unsigned char>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<hud_test::i>, unsigned short>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<hud_test::j>, unsigned int>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<hud_test::k>, unsigned long>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<hud_test::l>, unsigned long long>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<hud_test::m>, hud::conditional_t<sizeof(wchar_t) == 2, unsigned short, unsigned int>>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<hud_test::n>, unsigned short>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<hud_test::o>, unsigned int>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<hud_test::p>, unsigned char>));

    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<hud_test::q>, u8>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<hud_test::r>, u16>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<hud_test::s>, u32>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<hud_test::t>, u64>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<hud_test::u>, u8>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<hud_test::v>, u16>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<hud_test::w>, u32>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<hud_test::x>, u64>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<hud_test::y>, u8>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<hud_test::z>, hud::conditional_t<sizeof(wchar) == 2, u16, u32>>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<hud_test::a_1>, u16>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<hud_test::b_1>, u32>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<hud_test::c_1>, uptr>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<hud_test::d_1>, hud::conditional_t<sizeof(void *) == 4, u32, u64>>));
    hud_assert_true((hud::is_same_v<hud::make_unsigned_t<hud_test::e_1>, hud::conditional_t<sizeof(void *) == 4, u32, u64>>));
}