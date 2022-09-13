#pragma once
#ifndef HD_INC_OSLAYER_DEFINES_H
#define HD_INC_OSLAYER_DEFINES_H
#include "CompilerDefines.h"

#if defined(HD_OS_WINDOWS)
#if defined(HD_COMPILER_MSVC) || defined(HD_COMPILER_CLANG_CL)

#define HD_MS_ALIGN(bytes) __declspec(align(bytes))  // Set alignment requierement

// Use __forceinline only in non debug mode ( With clang-cl __forceinline inline code even in Debug configuration ) 
#if defined(HD_DEBUG)
#define HD_FORCEINLINE 
#else 
#define HD_FORCEINLINE __forceinline // Overrides the cost/benefit analysis and relies on the judgment of the programmer instead
#endif 

#define HD_RESTRICT __restrict //  Indicates that a symbol is not aliased in the current scope.
#define NOMINMAX // Do not defines min and max macro

#else 
#error Unsupported compiler for Windows
# endif//  HD_COMPILER_MSVC || HD_COMPILER_CLANG_CL

#elif defined(HD_OS_LINUX)

#if defined(HD_COMPILER_CLANG)

// Use __forceinline only in non debug mode 
#if defined(HD_DEBUG)
#define HD_FORCEINLINE 
#else 
#define HD_FORCEINLINE __attribute__((always_inline)) // Overrides the cost/benefit analysis and relies on the judgment of the programmer instead
#endif 

#define HD_RESTRICT __restrict__ //  Indicates that a symbol is not aliased in the current scope.
#define __STDC_WANT_LIB_EXT1__ 1 // Enable bounds-checked functions

#else 
#error Unsupported compiler for Linux
#endif // defined(HD_COMPILER_CLANG)

#else 
#error Unsupported operating system

#endif // defined(HD_OS_...)

#endif // HD_INC_OSLAYER_DEFINES_H

