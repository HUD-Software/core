#pragma once
#ifndef HD_INC_OSLAYER_ALLOCATOR_HEAP_H
#define HD_INC_OSLAYER_ALLOCATOR_HEAP_H
#include "../Memory.h"
#include "../Traits/IsPowerOfTwo.h"
#include "Allocation.h"

namespace hud {

    /** Allocator that use the system heap allocation. */
    struct HeapAllocator {

        /** The type of allocation done by this allocator. */
        template<typename T>
        using AllocationType = Allocation<T>;

        /**
        * Allocate memory block with no alignment requirements
        * @tparam T The element type to allocate
        * @param count Number of element T to allocate
        * @return Allocation view into the allocated memory, empty Allocation if failed
        */
        template<typename T = u8>
        [[nodiscard]]
        constexpr AllocationType<T> allocate(const usize count) noexcept {
            if (count > 0) {
                return AllocationType<T>(Memory::allocate_array<T>(count), count);
            }
            return AllocationType<T>{};
        }

        /**
        * Free memory block
        * @param buffer The buffer to free
        */
        template<typename T = u8>
        constexpr void free(const AllocationType<T>& buffer) noexcept {
            Memory::free_array(buffer.data(), buffer.count());
        }
    };

} // namespace hud

#endif // HD_INC_OSLAYER_ALLOCATOR_HEAP_H