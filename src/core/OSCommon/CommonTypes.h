#pragma once
#ifndef HD_INC_OSABSTRACTIONLAYER_OS_COMMON_TYPES_H
#define HD_INC_OSABSTRACTIONLAYER_OS_COMMON_TYPES_H
#include <stdint.h> // uint8_t, uint16_t, ....
namespace hud::OS::Common{

    struct Types {
        // Unsigned common types
        using u8 = uint8_t;     // 8 bits unsigned integer
        using u16 = uint16_t;   // 16 bits unsigned integer
        using u32 = uint32_t;   // 32 bits unsigned integer
        using u64 = uint64_t;   // 64 bits unsigned integer

        // Signed common types
        using i8 = int8_t;     // 8 bits signed integer
        using i16 = int16_t;   // 16 bits signed integer
        using i32 = int32_t;   // 32 bits signed integer
        using i64 = int64_t;   // 64 bits signed integer

        // Character common types
        using ansichar = char;      // 8 bits character
        using wchar = wchar_t;      // Platform dependent size bits wide character
        using char16 = char16_t;    // 16 bits character type
        using char32 = char32_t;    // 32 bits character type

        // Floating point types
        using f32 = float;      // 32 bits f32 (IEEE 754)
        using f64 = double;     // 64 bits f32 (IEEE 754)

    #if defined(HD_TARGET_32_BITS)
         // Pointer types
        using uptr = u32;   // unsigned integer same size as a pointer
        using iptr = i32;   // signed integer same size as a pointer

        // Size types
        using usize = u32;  // sizeof operator compatible value
        using isize = i32;  // signed integer representing a valid pointer subtraction operation

    #else
        // Pointer types
        using uptr = u64;   // unsigned integer same size as a pointer
        using iptr = i64;   // signed integer same size as a pointer

        // Size types
        using usize = u64;  // sizeof operator compatible value
        using isize = i64;  // signed integer representing a valid pointer subtraction operation

    #endif

        struct ptr {
            using null = decltype(nullptr); // null pointer representation
        };

        // Min-Max unsigned common type
        static inline constexpr u8 u8_max = 0xFF;
        static inline constexpr u8 u8_min = 0x00;
        static inline constexpr u16 u16_max = 0xFFFF;
        static inline constexpr u16 u16_min = 0x0000;
        static inline constexpr u32 u32_max = 0xFFFFFFFF;
        static inline constexpr u32 u32_min = 0x00000000;
        static inline constexpr u64 u64_max = 0xFFFFFFFFFFFFFFFF;
        static inline constexpr u64 u64_min = 0x0000000000000000;

        static inline constexpr i8 i8_max = 127;
        static inline constexpr i8 i8_min = (i8{-127}- 1);
        static inline constexpr i16 i16_max = 32767;
        static inline constexpr i16 i16_min = (i16{-32767} - 1);
        static inline constexpr i32 i32_max = 2147483647;
        static inline constexpr i32 i32_min = (i32{-2147483647} - 1);
        static inline constexpr i64 i64_max = 9223372036854775807;
        static inline constexpr i64 i64_min = (i64{-9223372036854775807} - 1);

        static inline constexpr ansichar ansichar_max = i8_max;
        static inline constexpr ansichar ansichar_min = i8_min;

        static inline constexpr f32 f32_max = 3.402823466e+38F;
        static inline constexpr f32 f32_min = 1.175494351e-38F;

        static inline constexpr f64 f64_max = 1.7976931348623158e+308;
        static inline constexpr f64 f64_min = 2.2250738585072014e-308;

#if defined(HD_TARGET_32_BITS)
        static inline constexpr uptr uptr_max = u32_max;
        static inline constexpr uptr uptr_min = u32_min;
        static inline constexpr iptr iptr_max = i32_max;
        static inline constexpr iptr iptr_min = i32_min;
        static inline constexpr usize usize_max = u32_max;
        static inline constexpr usize usize_min = u32_min;
        static inline constexpr isize isize_max = i32_max;
        static inline constexpr isize isize_min = i32_min;
#else
        static inline constexpr uptr uptr_max = u64_max;
        static inline constexpr uptr uptr_min = u64_min;
        static inline constexpr iptr iptr_max = i64_max;
        static inline constexpr iptr iptr_min = i64_min;
        static inline constexpr usize usize_max = u64_max;
        static inline constexpr usize usize_min = u64_min;
        static inline constexpr isize isize_max = i64_max;
        static inline constexpr isize isize_min = i64_min;
#endif
    };

} // namespace hud::OS::Common
#endif // HD_INC_OSABSTRACTIONLAYER_OS_COMMON_TYPES_H