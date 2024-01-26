#ifndef HD_INC_CORE_ALLOCATOR_HEAP_H
#define HD_INC_CORE_ALLOCATOR_HEAP_H
#include "../memory.h"
#include "allocation.h"
#include "allocator_traits.h"

namespace hud
{

    /** Allocator that use the system heap allocation. */
    struct heap_allocator
    {
        /** The type of allocation done by this allocator. */
        template<typename type_t>
        using memory_allocation_type = hud::memory_allocation<type_t>;

        /**
         * Allocate memory block with no alignment requirements
         * @tparam type_t The element type to allocate
         * @param count Number of element type_t to allocate
         * @return allocation view into the allocated memory, empty allocation if failed
         */
        template<typename type_t = u8>
        [[nodiscard]] constexpr memory_allocation_type<type_t> allocate(const usize count) noexcept
        {
            return memory_allocation_type<type_t>(memory::allocate_array<type_t>(count), count);
        }

        /**
         * Free memory block
         * @param buffer The buffer to free
         */
        template<typename type_t = u8>
        constexpr void free(const memory_allocation_type<type_t> &buffer) noexcept
        {
            hud::memory::free_array(buffer.data(), buffer.count());
        }
    };

    template<>
    struct allocator_traits<heap_allocator>
    {
        using is_always_equal = hud::true_type;
        using copy_when_container_copy = hud::false_type;
        using move_when_container_move = hud::false_type;
        using swap_when_container_swap = hud::false_type;
    };

    static_assert(hud::allocator_traits<heap_allocator>::is_always_equal::value);
    static_assert(!hud::allocator_traits<heap_allocator>::copy_when_container_copy::value);
    static_assert(!hud::allocator_traits<heap_allocator>::move_when_container_move::value);
    static_assert(!hud::allocator_traits<heap_allocator>::swap_when_container_swap::value);
} // namespace hud

#endif // HD_INC_CORE_ALLOCATOR_HEAP_H