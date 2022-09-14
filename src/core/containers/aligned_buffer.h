#pragma once
#ifndef HD_INC_CORE_ALIGNED_BUFFER_H
#define HD_INC_CORE_ALIGNED_BUFFER_H
#include "../traits/is_power_of_two.h"

namespace hud {

    /**
    * Provides an uninitialized compile-time aligned array of bytes,
    * The aligned memory can be retrieves with pointer() function.
    * The given alignment must be a power of two. 
    */
    template<usize size, u32 alignment> requires(is_power_of_two_v<alignment>)
    class aligned_buffer {
    public:
        /** Retrieves a pointer to the compile-time aligned array of bytes. */
        [[nodiscard]]
        HD_FORCEINLINE void* pointer() noexcept {
            return storage;
        }

        /** Retrieves a pointer of type type_t to the compile-time aligned array of bytes. */
        template<typename type_t>
        [[nodiscard]]
        HD_FORCEINLINE type_t* pointer_as() noexcept {
            return static_cast<type_t*>(pointer());
        }

        /** Retrieves a pointer to the compile-time aligned array of bytes. */
        [[nodiscard]]
        HD_FORCEINLINE const void* pointer() const noexcept {
            return storage;
        }

        /** Retrieves a pointer of type type_t to the compile-time aligned array of bytes. */
        template<typename type_t>
        [[nodiscard]]
        HD_FORCEINLINE const type_t* pointer() const noexcept {
            return static_cast<const type_t*>(pointer());
        }

    private:
        /** The compile-time aligned array of bytes */
        alignas(alignment) u8 storage[size];
    };
} // namespace hud

#endif // HD_INC_CORE_ALIGNED_BUFFER_H
