#ifndef HD_INC_CORE_BUILTIN_H
#define HD_INC_CORE_BUILTIN_H

#if (defined(HD_COMPILER_GCC) || defined(HD_COMPILER_CLANG) || defined(HD_COMPILER_CLANG_CL)) && !defined(__has_builtin)
    #error __has_builtin need to be defined.
#endif

#if defined(HD_COMPILER_GCC) || defined(HD_COMPILER_CLANG)
    #define HD_HAS_BUILTIN_IS_CONVERTIBLE_TO __has_builtin(__is_convertible_to)
    #define HD_HAS_BUILTIN_IS_TRIVIALLY_DESTRUCTIBLE __has_builtin(__is_trivially_destructible)
    #define HD_HAS_BUILTIN_BUILTIN_BSWAP32 __has_builtin(__builtin_bswap32)
    #define HD_HAS_BUILTIN_BULITIN_BSWAP64 __has_builtin(__builtin_bswap64)
    #define HD_HAS_BUILTIN_BULITIN_ROTATELEFT32 __has_builtin(__builtin_rotateleft32)
    #define HD_HAS_BUILTIN_BULITIN_ROTATELEFT64 __has_builtin(__builtin_rotateleft64)
    #define HD_HAS_BUILTIN_BULITIN_ROTATERIGHT32 __has_builtin(__builtin_rotateright32)
    #define HD_HAS_BUILTIN_BULITIN_ROTATERIGHT64 __has_builtin(__builtin_rotateright64)
    #define HD_HAS_BUILTIN_ATOMIC_THREAD_FENCE __has_builtin(__atomic_thread_fence)
    #define HD_HAS_BUILTIN_ATOMIC_LOAD_N __has_builtin(__atomic_load_n)
    #define HD_HAS_BUILTIN_ATOMIC_STORE_N __has_builtin(__atomic_store_n)
    #define HD_HAS_BUILTIN_SYNC_VAL_COMPARE_AND_SWAP __has_builtin(__sync_val_compare_and_swap)
    #define HD_HAS_BUILTIN_ATOMIC_COMPARE_EXCHANGE_N __has_builtin(__atomic_compare_exchange_n)
    #define HD_HAS_BUILTIN_ATOMIC_EXCHANGE_N __has_builtin(__atomic_exchange_n)
    #define HD_HAS_BUILTIN_ATOMIC_FETCH_ADD __has_builtin(__atomic_fetch_add)
    #define HD_HAS_BUILTIN_ATOMIC_FETCH_SUB __has_builtin(__atomic_fetch_sub)
    #define HD_HAS_BUILTIN_DEBUGTRAP __has_builtin(__builtin_debugtrap)
    #define HD_HAS_BUILTIN_ASSUME __has_builtin(__builtin_assume)
    #define HD_HAS_BUILTIN_CLZS __has_builtin(__builtin_clzs)
    #define HD_HAS_BUILTIN_CLZ __has_builtin(__builtin_clz)
    #define HD_HAS_BUILTIN_CLZLL __has_builtin(__builtin_clzll)
    #define HD_HAS_BUILTIN_CTZ __has_builtin(__builtin_ctz)
    #define HD_HAS_BUILTIN_CTZLL __has_builtin(__builtin_clzll)

#elif defined(HD_COMPILER_CLANG_CL)
    #define HD_HAS_BUILTIN_IS_CONVERTIBLE_TO __has_builtin(__is_convertible_to)
    #define HD_HAS_BUILTIN_IS_TRIVIALLY_DESTRUCTIBLE __has_builtin(__is_trivially_destructible)
    #define HD_HAS_BUILTIN_BUILTIN_BSWAP32 __has_builtin(__builtin_bswap32)
    #define HD_HAS_BUILTIN_BULITIN_BSWAP64 __has_builtin(__builtin_bswap64)
    #define HD_HAS_BUILTIN_BULITIN_ROTATELEFT32 __has_builtin(__builtin_rotateleft32)
    #define HD_HAS_BUILTIN_BULITIN_ROTATELEFT64 __has_builtin(__builtin_rotateleft64)
    #define HD_HAS_BUILTIN_BULITIN_ROTATERIGHT32 __has_builtin(__builtin_rotateright32)
    #define HD_HAS_BUILTIN_BULITIN_ROTATERIGHT64 __has_builtin(__builtin_rotateright64)
    #define HD_HAS_BUILTIN_ATOMIC_THREAD_FENCE __has_builtin(__atomic_thread_fence)
    #define HD_HAS_BUILTIN_ATOMIC_LOAD_N __has_builtin(_atomic_load_n)
    #define HD_HAS_BUILTIN_ATOMIC_STORE_N __has_builtin(__atomic_store_n)
    #define HD_HAS_BUILTIN_SYNC_VAL_COMPARE_AND_SWAP __has_builtin(__sync_val_compare_and_swap)
    #define HD_HAS_BUILTIN_ATOMIC_COMPARE_EXCHANGE_N __has_builtin(__atomic_compare_exchange_n)
    #define HD_HAS_BUILTIN_ATOMIC_EXCHANGE_N __has_builtin(__atomic_exchange_n)
    #define HD_HAS_BUILTIN_ATOMIC_FETCH_ADD __has_builtin(__atomic_fetch_add)
    #define HD_HAS_BUILTIN_ATOMIC_FETCH_SUB __has_builtin(__atomic_fetch_sub)
    #define HD_HAS_BUILTIN_DEBUGTRAP __has_builtin(__builtin_debugtrap)
    #define HD_HAS_BUILTIN_ASSUME __has_builtin(__builtin_assume)
    #define HD_HAS_BUILTIN_CLZS __has_builtin(__builtin_clzs)
    #define HD_HAS_BUILTIN_CLZ __has_builtin(__builtin_clz)
    #define HD_HAS_BUILTIN_CLZLL __has_builtin(__builtin_clzll)
    #define HD_HAS_BUILTIN_CTZ __has_builtin(__builtin_ctz)
    #define HD_HAS_BUILTIN_CTZLL __has_builtin(__builtin_clzll)

#elif defined(HD_COMPILER_MSVC)
    #define HD_HAS_BUILTIN_IS_CONVERTIBLE_TO 1
    #define HD_HAS_BUILTIN_IS_TRIVIALLY_DESTRUCTIBLE 1
    #define HD_HAS_BUILTIN_BUILTIN_BSWAP32 0
    #define HD_HAS_BUILTIN_BULITIN_BSWAP64 0
    #define HD_HAS_BUILTIN_BULITIN_ROTATELEFT32 0
    #define HD_HAS_BUILTIN_BULITIN_ROTATELEFT64 0
    #define HD_HAS_BUILTIN_BULITIN_ROTATERIGHT32 0
    #define HD_HAS_BUILTIN_BULITIN_ROTATERIGHT64 0
    #define HD_HAS_BUILTIN_ATOMIC_THREAD_FENCE 0
    #define HD_HAS_BUILTIN_ATOMIC_LOAD_N 0
    #define HD_HAS_BUILTIN_ATOMIC_STORE_N 0
    #define HD_HAS_BUILTIN_SYNC_VAL_COMPARE_AND_SWAP 0
    #define HD_HAS_BUILTIN_ATOMIC_COMPARE_EXCHANGE_N 0
    #define HD_HAS_BUILTIN_ATOMIC_EXCHANGE_N 0
    #define HD_HAS_BUILTIN_ATOMIC_FETCH_ADD 0
    #define HD_HAS_BUILTIN_ATOMIC_FETCH_SUB 0
    #define HD_HAS_BUILTIN_DEBUGTRAP 0
    #define HD_HAS_BUILTIN_ASSUME 1
    #define HD_HAS_BUILTIN_CLZS 0
    #define HD_HAS_BUILTIN_CLZ 0
    #define HD_HAS_BUILTIN_CLZLL 0
    #define HD_HAS_BUILTIN_CTZ 0
    #define HD_HAS_BUILTIN_CTZLL 0

#endif

#endif // HD_INC_CORE_BUILTIN_H
