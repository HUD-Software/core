#ifndef HD_INC_CORE_ALLOCATOR_ALIGNED_HEAP_H
#define HD_INC_CORE_ALLOCATOR_ALIGNED_HEAP_H
#include "../memory.h"
#include "../traits/is_power_of_two.h"
#include "allocation.h"

namespace hud
{
    /**
     * Allocator that use the system heap allocation and align all allocation with the given alignment.
     * The given alignment must be a power of two.
     */
    template<u32 alignment>
    requires(is_power_of_two_v<alignment>)
    struct aligned_heap_allocator
    {
        /** The type of allocation done by this allocator. */
        template<typename type_t>
        using allocation_type = hud::allocation<type_t>;

        /** Do not propagate the allocator when container is moved. */
        using propagate_on_container_move_assignment = hud::false_type;

        /** Do not propagate the allocator when containes if copied. */
        using propagate_on_container_copy_assignment = hud::false_type;

        /** Do not propagate the allocator when containes if swapped. */
        using propagate_on_container_swap = hud::false_type;

        /**
         * Allocate aligned memory block
         * @tparam type_t The element type to allocate
         * @param count Number of element type_t to allocate
         * @return allocation of the allocated aligned memory block, empty allocation if failed
         */
        template<typename type_t = u8>
        [[nodiscard]] constexpr allocation_type<type_t> allocate(const usize count) noexcept
        {
            return allocation_type<type_t>(memory::allocate_align<type_t>(count, alignment), count);
        }

        /**
         * Free memory block
         * @param buffer The buffer to free
         */
        template<typename type_t = u8>
        constexpr void free(const allocation_type<type_t> &buffer) noexcept
        {
            hud::memory::free_align(buffer.data(), buffer.count());
        }
    };
} // namespace hud

#endif // HD_INC_CORE_ALLOCATOR_ALIGNED_HEAP_H