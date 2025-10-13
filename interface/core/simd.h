#ifndef HD_INC_CORE_SIMD_H
#define HD_INC_CORE_SIMD_H
#if defined(HD_SSE2)
#include <emmintrin.h>
#endif
#if defined(HD_SSE4_1)
#include <smmintrin.h> // _mm_testz_si128
#endif
#endif // HD_INC_CORE_SIMD_H