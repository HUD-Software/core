#pragma once
#ifndef HD_INC_CORE_ALLOCATOR_HEAP_H
#define HD_INC_CORE_ALLOCATOR_HEAP_H
#include "../memory.h"
#include "../traits/is_power_of_two.h"
#include "allocation.h"

namespace hud {

    /** Allocator that use the system heap allocation. */
    struct heap_allocator {

        /** The type of allocation done by this allocator. */
        template<typename type_t>
        using allocation_type = allocation<type_t>;

        /**
        * Allocate memory block with no alignment requirements
        * @tparam type_t The element type to allocate
        * @param count Number of element type_t to allocate
        * @return allocation view into the allocated memory, empty allocation if failed
        */
        template<typename type_t = u8>
        [[nodiscard]]
        constexpr allocation_type<type_t> allocate(const usize count) noexcept {
            if (count > 0) {
                return allocation_type<type_t>(memory::allocate_array<type_t>(count), count);
            }
            return allocation_type<type_t>{};
        }

        /**
        * Free memory block
        * @param buffer The buffer to free
        */
        template<typename type_t = u8>
        constexpr void free(const allocation_type<type_t>& buffer) noexcept {
           hud::memory::free_array(buffer.data(), buffer.count());
        }
    };

} // namespace hud

#endif // HD_INC_CORE_ALLOCATOR_HEAP_H