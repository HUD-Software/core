#ifndef HD_INC_CORE_TYPES_H
#define HD_INC_CORE_TYPES_H
#include "compilation.h"

#if defined(HD_OS_WINDOWS)
    #include "os_windows/types.h"
#elif defined(HD_OS_LINUX)
    #include "os_linux/types.h"
#else
    #error Targeted OS not supported
#endif

namespace hud
{
#if defined(HD_OS_WINDOWS)
    using types = os::windows::types;
#elif defined(HD_OS_LINUX)
    using types = os::linux::types;
#endif

    // Unsigned common types
    using u8 = types::u8;   // 8 bits unsigned integer
    using u16 = types::u16; // 16 bits unsigned integer
    using u32 = types::u32; // 32 bits unsigned integer
    using u64 = types::u64; // 64 bits unsigned integer

    // Signed common types
    using i8 = types::i8;   // 8 bits signed integer
    using i16 = types::i16; // 16 bits signed integer
    using i32 = types::i32; // 32 bits signed integer
    using i64 = types::i64; // 64 bits signed integer

    // characteracter common types
    using ansichar = types::ansichar; // 8 bits character
    using wchar = types::wchar;       // Platform dependent size bits wide character
    using char16 = types::char16;     // 16 bits character type
    using char32 = types::char32;     // 32 bits character type

    // Floating point types
    using f32 = types::f32; // 32 bits f32 (IEEE 754)
    using f64 = types::f64; // 64 bits f32 (IEEE 754)

    // Pointer types
    using uptr = types::uptr; // unsigned integer same size as a pointer
    using iptr = types::iptr; // signed integer same size as a pointer

    // Size types
    using usize = types::usize; // sizeof operator compatible value
    using isize = types::isize; // signed integer representing a valid pointer subtraction operation

    // Pointer types
    using ptr = types::ptr; // contains pointer types

    // Min-Max unsigned common type
    static inline constexpr u8 u8_max = types::u8_max;
    static inline constexpr u8 u8_min = types::u8_min;
    static inline constexpr u16 u16_max = types::u16_max;
    static inline constexpr u16 u16_min = types::u16_min;
    static inline constexpr u32 u32_max = types::u32_max;
    static inline constexpr u32 u32_min = types::u32_min;
    static inline constexpr u64 u64_max = types::u64_max;
    static inline constexpr u64 u64_min = types::u64_min;

    static inline constexpr i8 i8_max = types::i8_max;
    static inline constexpr i8 i8_min = types::i8_min;
    static inline constexpr i16 i16_max = types::i16_max;
    static inline constexpr i16 i16_min = types::i16_min;
    static inline constexpr i32 i32_max = types::i32_max;
    static inline constexpr i32 i32_min = types::i32_min;
    static inline constexpr i64 i64_max = types::i64_max;
    static inline constexpr i64 i64_min = types::i64_min;

    static inline constexpr ansichar ansichar_max = types::ansichar_max;
    static inline constexpr ansichar ansichar_min = types::ansichar_min;

    static inline constexpr wchar wchar_max = types::wchar_max;
    static inline constexpr wchar wchar_min = types::wchar_min;

    static inline constexpr f32 f32_max = types::f32_max;
    static inline constexpr f32 f32_min = types::f32_min;

    static inline constexpr f64 f64_max = types::f64_max;
    static inline constexpr f64 f64_min = types::f64_min;

    static inline constexpr uptr uptr_max = types::uptr_max;
    static inline constexpr uptr uptr_min = types::uptr_min;
    static inline constexpr iptr iptr_max = types::iptr_max;
    static inline constexpr iptr iptr_min = types::iptr_min;
    static inline constexpr usize usize_max = types::usize_max;
    static inline constexpr usize usize_min = types::usize_min;
    static inline constexpr isize isize_max = types::isize_max;
    static inline constexpr isize isize_min = types::isize_min;

    static inline constexpr usize index_min = types::usize_min;
    static inline constexpr usize index_max = types::usize_max - 1;
    static inline constexpr usize index_none = static_cast<usize>(-1);

} // namespace hud

// Statically check for type size
static_assert(sizeof(bool) == 1, "sizeof(hud::bool) != 1 byte");
static_assert(sizeof(hud::i8) == 1, "sizeof(hud::i8) != 1 byte");
static_assert(sizeof(hud::i16) == 2, "sizeof(hud::i16) != 2 bytes");
static_assert(sizeof(hud::i32) == 4, "sizeof(hud::i32) != 4 bytes");
static_assert(sizeof(hud::i64) == 8, "sizeof(hud::i64) != 8 bytes");
static_assert(sizeof(hud::u8) == 1, "sizeof(hud::u8) != 1 byte");
static_assert(sizeof(hud::u16) == 2, "sizeof(hud::u16) != 2 bytes");
static_assert(sizeof(hud::u32) == 4, "sizeof(hud::u32) != 4 bytes");
static_assert(sizeof(hud::u64) == 8, "sizeof(hud::u64) != 8 bytes");
static_assert(sizeof(hud::ansichar) == 1, "sizeof(hud::ansichar) != 1 byte");
#if defined(HD_COMPILER_MSVC) || defined(HD_COMPILER_CLANG_CL)
static_assert(sizeof(hud::wchar) == 2, "sizeof(hud::wchar) != 2 bytes");
#else
static_assert(sizeof(hud::wchar) == 4, "sizeof(hud::wchar) != 4 bytes");
#endif

static_assert(sizeof(hud::f32) == 4, "sizeof(hud::f32) != 4 bytes");
static_assert(sizeof(hud::f64) == 8, "sizeof(hud::f64) != 8 bytes");

#if defined(HD_TARGET_32_BITS)
static_assert(sizeof(hud::iptr) == 4, "sizeof(hud::iptr) != 4 bytes");
static_assert(sizeof(hud::uptr) == 4, "sizeof(hud::uptr) != 4 bytes");
static_assert(sizeof(hud::usize) == 4, "sizeof(hud::usize) != 4 bytes");
static_assert(sizeof(hud::isize) == 4, "sizeof(hud::isize) != 4 bytes");
#else
static_assert(sizeof(hud::iptr) == 8, "sizeof(hud::iptr) != 8 bytes");
static_assert(sizeof(hud::uptr) == 8, "sizeof(hud::uptr) != 8 bytes");
static_assert(sizeof(hud::usize) == 8, "sizeof(hud::usize) != 8 bytes");
static_assert(sizeof(hud::isize) == 8, "sizeof(hud::isize) != 8 bytes");
#endif

// Statically check for type sign
#define is_unsigned(type_t) (type_t(0) < type_t(-1))
#define is_signed(type_t) (type_t(-1) < type_t(0))
static_assert(!is_signed(bool) && is_unsigned(bool), "bool is signed");
static_assert(is_signed(hud::i8) && !is_unsigned(hud::i8), "hud::i8 is not signed");
static_assert(is_signed(hud::i16) && !is_unsigned(hud::i16), "hud::i16 is not signed");
static_assert(is_signed(hud::i32) && !is_unsigned(hud::i32), "hud::i32 is not signed");
static_assert(is_signed(hud::i64) && !is_unsigned(hud::i64), "hud::i64 is not signed");
static_assert(!is_signed(hud::u8) && is_unsigned(hud::u8), "hud::u8 is signed");
static_assert(!is_signed(hud::u16) && is_unsigned(hud::u16), "hud::u16 is signed");
static_assert(!is_signed(hud::u32) && is_unsigned(hud::u32), "hud::u32 is signed");
static_assert(!is_signed(hud::u64) && is_unsigned(hud::u64), "hud::u64 is signed");
static_assert(is_signed(hud::ansichar) && !is_unsigned(hud::ansichar), "hud::ansichar is not signed");
#if defined(HD_COMPILER_MSVC) || defined(HD_COMPILER_CLANG_CL)
static_assert(!is_signed(hud::wchar) && is_unsigned(hud::wchar), "hud::wchar is signed");
#else
static_assert(is_signed(hud::wchar) && !is_unsigned(hud::wchar), "hud::wchar is unsigned");
#endif
static_assert(is_signed(hud::iptr) && !is_unsigned(hud::iptr), "hud::iptr is not signed");
static_assert(!is_signed(hud::uptr) && is_unsigned(hud::uptr), "hud::uptr is signed");
static_assert(!is_signed(hud::usize) && is_unsigned(hud::usize), "hud::usize is signed");
static_assert(is_signed(hud::isize) && !is_unsigned(hud::isize), "hud::usize is unsigned");

#undef is_unsigned
#undef is_signed

// Statically check for type min-max
static_assert(hud::u8_max == 0xFF, "u8_max != 0xFF");
static_assert(hud::u8_min == 0x00, "u8_min != 0x00");
static_assert(hud::u16_max == 0xFFFF, "u16_max != 0xFFFF");
static_assert(hud::u16_min == 0x0000, "u16_min != 0x0000");
static_assert(hud::u32_max == 0xFFFFFFFF, "u32_max != 0xFFFFFFFF");
static_assert(hud::u32_min == 0x00000000, "u32_min != 0x00000000");
static_assert(hud::u64_max == 0xFFFFFFFFFFFFFFFF, "u64_max != 0xFFFFFFFFFFFFFFFF");
static_assert(hud::u64_min == 0x0000000000000000, "u64_min != 0x0000000000000000");

static_assert(hud::i8_max == 127, "i8_max != 127");
static_assert(hud::i8_min == (-127 - 1), "i8_min != (-127 - 1)");
static_assert(hud::i16_max == 32767, "i16_max != 32767");
static_assert(hud::i16_min == (-32767 - 1), "i16_min != (-32767 - 1)");
static_assert(hud::i32_max == 2147483647, "i32_max != 2147483647");
static_assert(hud::i32_min == (-2147483648), "i32_min != (-2147483647 - 1)");
static_assert(hud::i64_max == 9223372036854775807, "i64_max != 9223372036854775807");
static_assert(hud::i64_min == (-9223372036854775807 - 1), "i64_min != (-9223372036854775807 - 1)");

static_assert(hud::ansichar_max == 127, "ansichar_max != 127");
static_assert(hud::ansichar_min == (-127 - 1), "ansichar_min != (-127 - 1)");

#if defined(HD_COMPILER_MSVC) || defined(HD_COMPILER_CLANG_CL)
static_assert(hud::wchar_max == 0xFFFF, "wchar_max != 0xFFFF");
static_assert(hud::wchar_min == 0, "wchar_max != 0");
#else
static_assert(hud::wchar_max == hud::i32_max, "wchar_max != i32_max");
static_assert(hud::wchar_min == hud::i32_min, "wchar_max != hud::i32_min");
#endif

static_assert(hud::f32_max == 3.402823466e+38F, "f32_max!= 3.402823466e+38F");
static_assert(hud::f32_min == 1.175494351e-38F, "f32_min!= 1.175494351e-38F");
static_assert(hud::f64_max == 1.7976931348623158e+308, "f64_max!= 1.7976931348623158e+308");
static_assert(hud::f64_min == 2.2250738585072014e-308, "f64_min!= 2.2250738585072014e-308");

#if defined(HD_TARGET_32_BITS)
static_assert(hud::iptr_max == 2147483647, "iptr_max!= 2147483647");
static_assert(hud::iptr_min == (-2147483647 - 1), "iptr_min!= (-2147483647 - 1)");
static_assert(hud::uptr_max == 0xFFFFFFFF, "uptr_max!= 0xFFFFFFFF");
static_assert(hud::uptr_min == 0, "uptr_min!= 0");
static_assert(hud::usize_max == 0xFFFFFFFF, "usize_max!= 0xFFFFFFFF");
static_assert(hud::usize_min == 0, "usize_min!= 0");
static_assert(hud::isize_max == 2147483647, "isize_max!= 2147483647");
static_assert(hud::isize_min == (-2147483647 - 1), "isize_min!= 0");
#else
static_assert(hud::iptr_max == 9223372036854775807, "iptr_max!= 9223372036854775807");
static_assert(hud::iptr_min == (-9223372036854775807 - 1), "iptr_min!= (-9223372036854775807 - 1)");
static_assert(hud::uptr_max == 0xFFFFFFFFFFFFFFFF, "uptr_max!= 0xFFFFFFFFFFFFFFFF");
static_assert(hud::uptr_min == 0, "uptr_min!= 0");
static_assert(hud::usize_max == 0xFFFFFFFFFFFFFFFF, "usize_max!= 0xFFFFFFFFFFFFFFFF");
static_assert(hud::usize_min == 0, "usize_min!= 0");
static_assert(hud::isize_max == 9223372036854775807, "isize_max!= 9223372036854775807");
static_assert(hud::isize_min == (-9223372036854775807 - 1), "isize_min!= 0");
#endif

// Redefine types in the global namespace if requested
#if defined(HD_GLOBAL_NAMESPACE_TYPES)
// Unsigned common types
using u8 = hud::u8;   // 8 bits unsigned integer
using u16 = hud::u16; // 16 bits unsigned integer
using u32 = hud::u32; // 32 bits unsigned integer
using u64 = hud::u64; // 64 bits unsigned integer

// Signed common types
using i8 = hud::i8;   // 8 bits signed integer
using i16 = hud::i16; // 16 bits signed integer
using i32 = hud::i32; // 32 bits signed integer
using i64 = hud::i64; // 64 bits signed integer

// characteracter common types
using ansichar = hud::ansichar; // 8 bits character
using wchar = hud::wchar;       // Platform dependent size bits wide character
using char16 = hud::char16;     // 16 bits character type
using char32 = hud::char32;     // 32 bits character type

// Floating point types
using f32 = hud::f32; // 32 bits f32 (IEEE 754)
using f64 = hud::f64; // 64 bits f32 (IEEE 754)

// Pointer types
using uptr = hud::uptr; // unsigned integer same size as a pointer
using iptr = hud::iptr; // signed integer same size as a pointer

// Size types
using usize = hud::usize; // sizeof operator compatible value
using isize = hud::isize; // signed integer representing a valid pointer subtraction operation

// Pointer types
using ptr = hud::ptr; // contains pointer types

#endif

#endif // HD_INC_CORE_TYPES_H
