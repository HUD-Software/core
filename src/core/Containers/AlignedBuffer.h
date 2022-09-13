#pragma once
#ifndef HD_INC_OSLAYER_ALIGNED_BUFFER_H
#define HD_INC_OSLAYER_ALIGNED_BUFFER_H
#include "../Traits/IsPowerOfTwo.h"

namespace hud {

    /**
    * Provides an uninitialized compile-time aligned array of bytes,
    * The aligned memory can be retrieves with pointer() function.
    * The given alignment must be a power of two. 
    */
    template<usize size, u32 alignment> requires(IsPowerOfTwoV<alignment>)
    class AlignedBuffer {
    public:
        /** Retrieves a pointer to the compile-time aligned array of bytes. */
        [[nodiscard]]
        HD_FORCEINLINE void* pointer() noexcept {
            return storage;
        }

        /** Retrieves a pointer of type T to the compile-time aligned array of bytes. */
        template<typename T>
        [[nodiscard]]
        HD_FORCEINLINE T* pointer_as() noexcept {
            return static_cast<T*>(pointer());
        }

        /** Retrieves a pointer to the compile-time aligned array of bytes. */
        [[nodiscard]]
        HD_FORCEINLINE const void* pointer() const noexcept {
            return storage;
        }

        /** Retrieves a pointer of type T to the compile-time aligned array of bytes. */
        template<typename T>
        [[nodiscard]]
        HD_FORCEINLINE const T* pointer() const noexcept {
            return static_cast<const T*>(pointer());
        }

    private:
        /** The compile-time aligned array of bytes */
        alignas(alignment) u8 storage[size];
    };
} // namespace hud

#endif // HD_INC_OSLAYER_ALIGNED_BUFFER_H
