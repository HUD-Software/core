#ifndef HD_INC_CORE_ALLOCATOR_HEAP_H
#define HD_INC_CORE_ALLOCATOR_HEAP_H
#include "../memory.h"
#include "allocator.h"

namespace hud
{

    /** Allocator that use the system heap allocation. */
    struct heap_allocator
        : hud::allocator
    {

        /**
         * Allocate memory block with no alignment requirements
         * @tparam type_t The element type to allocate
         * @param count Number of element type_t to allocate
         * @return allocation view into the allocated memory, empty allocation if failed
         */
        template<typename type_t = u8>
        [[nodiscard]] constexpr allocation_type<type_t> allocate(const usize count) noexcept
        {
            return allocation_type<type_t>(memory::allocate_array<type_t>(count), count);
        }

        /**
         * Free memory block
         * @param buffer The buffer to free
         */
        template<typename type_t = u8>
        constexpr void free(const allocation_type<type_t> &buffer) noexcept
        {
            hud::memory::free_array(buffer.data(), buffer.count());
        }
    };

} // namespace hud

#endif // HD_INC_CORE_ALLOCATOR_HEAP_H