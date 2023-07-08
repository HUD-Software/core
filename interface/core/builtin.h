#ifndef HD_INC_CORE_BUILTIN_H
#define HD_INC_CORE_BUILTIN_H

#if defined(HD_COMPILER_GCC) || defined(HD_COMPILER_CLANG)
    #define HD_HAS_BUILTIN_IS_CONVERTIBLE_TO __has_builtin(__is_convertible_to)                 // Clang and GCC maybe defined __is_convertible_to builtin
    #define HD_HAS_BUILTIN_IS_TRIVIALLY_DESTRUCTIBLE __has_builtin(__is_trivially_destructible) // Clang and GCC maybe defined __is_trivially_destructible builtin
    #define HD_HAS_BUILTIN_BUILTIN_BSWAP32 __has_builtin(__builtin_bswap32)                     // Clang and GCC maybe defined __builtin_bswap32 builtin
    #define HD_HAS_BUILTIN_BULITIN_BSWAP64 __has_builtin(__builtin_bswap64)                     // Clang and GCC maybe defined __builtin_bswap64 builtin
    #define HD_HAS_BUILTIN_BULITIN_ROTATELEFT32 __has_builtin(__builtin_rotateleft32)           // Clang and GCC maybe defined __builtin_rotateleft32 builtin
    #define HD_HAS_BUILTIN_BULITIN_ROTATELEFT64 __has_builtin(__builtin_rotateleft64)           // Clang and GCC maybe defined __builtin_rotateleft64 builtin
    #define HD_HAS_BUILTIN_BULITIN_ROTATERIGHT32 __has_builtin(__builtin_rotateright32)         // Clang and GCC maybe defined __builtin_rotateright32 builtin
    #define HD_HAS_BUILTIN_BULITIN_ROTATERIGHT64 __has_builtin(__builtin_rotateright64)         // Clang and GCC maybe defined __builtin_rotateright64 builtin
    #define HD_HAS_BUILTIN_ATOMIC_THREAD_FENCE __has_builtin(__atomic_thread_fence)             // Clang and GCC maybe defined __atomic_thread_fence builtin
    #define HD_HAS_BUILTIN_ATOMIC_LOAD_N __has_builtin(__atomic_load_n)                         // Clang and GCC maybe defined _atomic_load_n builtin
    #define HD_HAS_BUILTIN_ATOMIC_STORE_N __has_builtin(__atomic_store_n)                       // Clang and GCC maybe defined __atomic_store_n builtin
    #define HD_HAS_BUILTIN_SYNC_VAL_COMPARE_AND_SWAP __has_builtin(__sync_val_compare_and_swap) // Clang and GCC maybe defined __sync_val_compare_and_swap builtin
    #define HD_HAS_BUILTIN_ATOMIC_COMPARE_EXCHANGE_N __has_builtin(__atomic_compare_exchange_n) // Clang and GCC maybe defined __atomic_compare_exchange_n builtin
    #define HD_HAS_BUILTIN_ATOMIC_EXCHANGE_N __has_builtin(__atomic_exchange_n)                 // Clang and GCC maybe defined __atomic_exchange_n builtin
    #define HD_HAS_BUILTIN_ATOMIC_FETCH_ADD __has_builtin(__atomic_fetch_add)                   // Clang and GCC maybe defined __atomic_fetch_add builtin
    #define HD_HAS_BUILTIN_ATOMIC_FETCH_SUB __has_builtin(__atomic_fetch_sub)                   // Clang and GCC maybe defined __atomic_fetch_sub builtin
    #define HD_HAS_BUILTIN_DEBUGTRAP __has_builtin(__builtin_debugtrap)                         // Clang and GCC maybe defined __builtin_debugtrap builtin

#elif defined(HD_COMPILER_CLANG_CL)
    #define HD_HAS_BUILTIN_IS_CONVERTIBLE_TO __has_builtin(__is_convertible_to)                 // Clang-cl maybe defined __is_convertible_to builtin
    #define HD_HAS_BUILTIN_IS_TRIVIALLY_DESTRUCTIBLE __has_builtin(__is_trivially_destructible) // Clang-cl maybe defined __is_trivially_destructible builtin
    #define HD_HAS_BUILTIN_BUILTIN_BSWAP32 __has_builtin(__builtin_bswap32)                     // Clang-cl maybe defined __builtin_bswap32 builtin
    #define HD_HAS_BUILTIN_BULITIN_BSWAP64 __has_builtin(__builtin_bswap64)                     // Clang-cl maybe defined __builtin_bswap64 builtin
    #define HD_HAS_BUILTIN_BULITIN_ROTATELEFT32 __has_builtin(__builtin_rotateleft32)           // Clang-cl maybe defined __builtin_rotateleft32 builtin
    #define HD_HAS_BUILTIN_BULITIN_ROTATELEFT64 __has_builtin(__builtin_rotateleft64)           // Clang-cl maybe defined __builtin_rotateleft64 builtin
    #define HD_HAS_BUILTIN_BULITIN_ROTATERIGHT32 __has_builtin(__builtin_rotateright32)         // Clang-cl maybe defined __builtin_rotateright32 builtin
    #define HD_HAS_BUILTIN_BULITIN_ROTATERIGHT64 __has_builtin(__builtin_rotateright64)         // Clang-cl maybe defined __builtin_rotateright64 builtin
    #define HD_HAS_BUILTIN_ATOMIC_THREAD_FENCE __has_builtin(__atomic_thread_fence)             // Clang-cl maybe defined __atomic_thread_fence builtin
    #define HD_HAS_BUILTIN_ATOMIC_LOAD_N __has_builtin(_atomic_load_n)                          // Clang-cl maybe defined _atomic_load_n builtin
    #define HD_HAS_BUILTIN_ATOMIC_STORE_N __has_builtin(__atomic_store_n)                       // Clang-cl maybe defined __atomic_store_n builtin
    #define HD_HAS_BUILTIN_SYNC_VAL_COMPARE_AND_SWAP __has_builtin(__sync_val_compare_and_swap) // Clang-cl maybe defined __sync_val_compare_and_swap builtin
    #define HD_HAS_BUILTIN_ATOMIC_COMPARE_EXCHANGE_N __has_builtin(__atomic_compare_exchange_n) // Clang-cl maybe defined __atomic_compare_exchange_n builtin
    #define HD_HAS_BUILTIN_ATOMIC_EXCHANGE_N __has_builtin(__atomic_exchange_n)                 // Clang-cl maybe defined __atomic_exchange_n builtin
    #define HD_HAS_BUILTIN_ATOMIC_FETCH_ADD __has_builtin(__atomic_fetch_add)                   // Clang-cl maybe defined __atomic_fetch_add builtin
    #define HD_HAS_BUILTIN_ATOMIC_FETCH_SUB __has_builtin(__atomic_fetch_sub)                   // Clang-cl maybe defined __atomic_fetch_sub builtin
    #define HD_HAS_BUILTIN_DEBUGTRAP __has_builtin(__builtin_debugtrap)                         // Clang-cl maybe defined __builtin_debugtrap builtin

#elif defined(HD_COMPILER_MSVC)
    #define HD_HAS_BUILTIN_IS_CONVERTIBLE_TO 1         // MSVC defined __is_convertible_to builtin
    #define HD_HAS_BUILTIN_IS_TRIVIALLY_DESTRUCTIBLE 1 // MSVC defined __is_trivially_destructible
    #define HD_HAS_BUILTIN_BUILTIN_BSWAP32 0           // MSVC don't have __builtin_bswap32 builtin
    #define HD_HAS_BUILTIN_BULITIN_BSWAP64 0           // MSVC don't have __builtin_bswap64 builtin
    #define HD_HAS_BUILTIN_BULITIN_ROTATELEFT32 0      // MSVC don't have __builtin_rotateleft32 builtin
    #define HD_HAS_BUILTIN_BULITIN_ROTATELEFT64 0      // MSVC don't have __builtin_rotateleft64 builtin
    #define HD_HAS_BUILTIN_BULITIN_ROTATERIGHT32 0     // MSVC don't have __builtin_rotateright32 builtin
    #define HD_HAS_BUILTIN_BULITIN_ROTATERIGHT64 0     // MSVC don't have __builtin_rotateright64 builtin
    #define HD_HAS_BUILTIN_ATOMIC_THREAD_FENCE 0       // MSVC don't have __atomic_thread_fence builtin
    #define HD_HAS_BUILTIN_ATOMIC_LOAD_N 0             // MSVC don't have _atomic_load_n builtin
    #define HD_HAS_BUILTIN_ATOMIC_STORE_N 0            // MSVC don't have __atomic_store_n builtin
    #define HD_HAS_BUILTIN_SYNC_VAL_COMPARE_AND_SWAP 0 // MSVC don't have __sync_val_compare_and_swap builtin
    #define HD_HAS_BUILTIN_ATOMIC_COMPARE_EXCHANGE_N 0 // MSVC don't have __atomic_compare_exchange_n builtin
    #define HD_HAS_BUILTIN_ATOMIC_EXCHANGE_N 0         // MSVC don't have __atomic_compare_exchange_n builtin
    #define HD_HAS_BUILTIN_ATOMIC_FETCH_ADD 0          // MSVC don't have __atomic_fetch_add builtin
    #define HD_HAS_BUILTIN_ATOMIC_FETCH_SUB 0          // MSVC don't have __atomic_fetch_sub builtin
    #define HD_HAS_BUILTIN_DEBUGTRAP 0                 // MSVC don't have __builtin_debugtrap builtin

#endif

/** Check that builtin checks is complete */
#ifndef HD_HAS_BUILTIN_IS_CONVERTIBLE_TO
    #error has __is_convertible_to builtin is not defined
#endif
#ifndef HD_HAS_BUILTIN_IS_TRIVIALLY_DESTRUCTIBLE
    #error has __is_trivially_destructible builtin is not defined
#endif
#ifndef HD_HAS_BUILTIN_BUILTIN_BSWAP32
    #error has __builtin_bswap32 builtin is not defined
#endif
#ifndef HD_HAS_BUILTIN_BULITIN_BSWAP64
    #error has __builtin_bswap64 builtin is not defined
#endif
#ifndef HD_HAS_BUILTIN_BULITIN_ROTATELEFT32
    #error has __builtin_rotateleft32 builtin is not defined
#endif
#ifndef HD_HAS_BUILTIN_BULITIN_ROTATELEFT64
    #error has __builtin_rotateleft64 builtin is not defined
#endif
#ifndef HD_HAS_BUILTIN_BULITIN_ROTATERIGHT32
    #error has __builtin_rotateright32 builtin is not defined
#endif
#ifndef HD_HAS_BUILTIN_BULITIN_ROTATERIGHT64
    #error has __builtin_rotateright64 builtin is not defined
#endif
#ifndef HD_HAS_BUILTIN_ATOMIC_THREAD_FENCE
    #error has __atomic_thread_fence builtin is not defined
#endif

#endif // HD_INC_CORE_BUILTIN_H