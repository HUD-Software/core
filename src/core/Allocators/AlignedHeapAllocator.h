#pragma once
#ifndef HD_INC_OSLAYER_ALLOCATOR_ALIGNED_HEAP_H
#define HD_INC_OSLAYER_ALLOCATOR_ALIGNED_HEAP_H
#include "../Memory.h"
#include "../Traits/IsPowerOfTwo.h"
#include "Allocation.h"

namespace hud {

    /** 
    * Allocator that use the system heap allocation and align all allocation with the given alignment. 
    * The given alignment must be a power of two.
    */
    template<u32 alignment> requires(IsPowerOfTwoV<alignment>)
    struct AlignedHeapAllocator {

        /** The type of allocation done by this allocator. */
        template<typename T>
        using AllocationType = Allocation<T>;

        /**
        * Allocate aligned memory block
        * @tparam T The element type to allocate
        * @param count Number of element T to allocate
        * @return Allocation of the allocated aligned memory block, empty Allocation if failed
        */
        template<typename T = u8>
        [[nodiscard]]
        constexpr AllocationType<T> allocate(const usize count) noexcept {
            if (count > 0) {
                return AllocationType<T>(static_cast<T*>(Memory::allocate_align<T>(count, alignment)), count);
            }
            return AllocationType<T>{};
        }

        /**
        * Free memory block
        * @param buffer The buffer to free
        */
        template<typename T = u8>
        constexpr void free(const AllocationType<T>& buffer) noexcept {
            Memory::free_align(buffer.data(), buffer.count());
        }
    };
}

#endif // HD_INC_OSLAYER_ALLOCATOR_ALIGNED_HEAP_H