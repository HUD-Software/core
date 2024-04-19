#ifndef HD_INC_CORE_ALLOCATOR_ALIGNED_HEAP_H
#define HD_INC_CORE_ALLOCATOR_ALIGNED_HEAP_H
#include "../memory.h"
#include "../traits/is_power_of_two.h"
#include "memory_allocation.h"
#include "allocator_traits.h"

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
        using memory_allocation_type = hud::memory_allocation<type_t>;

        /**
         * Allocate aligned memory block
         * @tparam type_t The element type to allocate
         * @param count Number of element type_t to allocate
         * @return allocation of the allocated aligned memory block, empty allocation if failed
         */
        template<typename type_t = u8>
        [[nodiscard]] constexpr memory_allocation_type<type_t> allocate(const usize count) noexcept
        {
            return memory_allocation_type<type_t>(memory::allocate_align<type_t>(count, alignment), count);
        }

        /**
         * Free memory block
         * @param buffer The buffer to free
         */
        template<typename type_t = u8>
        constexpr void free(const memory_allocation_type<type_t> &buffer) noexcept
        {
            hud::memory::free_align(buffer.data(), buffer.count());
        }
    };

    template<u32 alignment>
    struct allocator_traits<aligned_heap_allocator<alignment>>
    {
        using is_always_equal = hud::true_type;
        using copy_when_container_copy = hud::false_type;
        using move_when_container_move = hud::false_type;
        using swap_when_container_swap = hud::false_type;
    };

    static_assert(hud::allocator_traits<aligned_heap_allocator<8>>::is_always_equal::value);
    static_assert(!hud::allocator_traits<aligned_heap_allocator<8>>::copy_when_container_copy::value);
    static_assert(!hud::allocator_traits<aligned_heap_allocator<8>>::move_when_container_move::value);
    static_assert(!hud::allocator_traits<aligned_heap_allocator<8>>::swap_when_container_swap::value);

} // namespace hud

#endif // HD_INC_CORE_ALLOCATOR_ALIGNED_HEAP_H