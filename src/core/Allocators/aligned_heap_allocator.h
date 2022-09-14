#pragma once
#ifndef HD_INC_CORE_ALLOCATOR_ALIGNED_HEAP_H
#define HD_INC_CORE_ALLOCATOR_ALIGNED_HEAP_H
#include "../memory.h"
#include "../traits/is_power_of_two.h"
#include "allocation.h"

namespace hud {

    /** 
    * Allocator that use the system heap allocation and align all allocation with the given alignment. 
    * The given alignment must be a power of two.
    */
    template<u32 alignment> requires(is_power_of_two_v<alignment>)
    struct aligned_heap_allocator {

        /** The type of allocation done by this allocator. */
        template<typename type_t>
        using allocation_type = allocation<type_t>;

        /**
        * Allocate aligned memory block
        * @tparam type_t The element type to allocate
        * @param count Number of element type_t to allocate
        * @return allocation of the allocated aligned memory block, empty allocation if failed
        */
        template<typename type_t = u8>
        [[nodiscard]]
        constexpr allocation_type<type_t> allocate(const usize count) noexcept {
            if (count > 0) {
                return allocation_type<type_t>(static_cast<type_t*>(memory::allocate_align<type_t>(count, alignment)), count);
            }
            return allocation_type<type_t>{};
        }

        /**
        * Free memory block
        * @param buffer The buffer to free
        */
        template<typename type_t = u8>
        constexpr void free(const allocation_type<type_t>& buffer) noexcept {
           hud::memory::free_align(buffer.data(), buffer.count());
        }
    };
}

#endif // HD_INC_CORE_ALLOCATOR_ALIGNED_HEAP_H