#ifndef HD_INC_TEST_ALLOCATORS_WATCHER_H
#define HD_INC_TEST_ALLOCATORS_WATCHER_H
#include <core/allocators/aligned_heap_allocator.h>
#include <core/allocators/memory_allocation.h>
#include <core/allocators/allocator_traits.h>

namespace hud_test
{
    template<u32 alignement>
    struct allocator_watcher
    {
        using allocator_type = hud::aligned_heap_allocator<alignement>;

        template<typename type_t>
        using memory_allocation_type = hud::memory_allocation<type_t>;

    public:
        /** Default constructor. */
        constexpr explicit allocator_watcher() noexcept = default;

        /** Construct allocator with a unique id. */
        constexpr explicit allocator_watcher(u32 id) noexcept
            : unique_id(id)
        {
        }

        /** Copy constructor. */
        constexpr explicit allocator_watcher(const allocator_watcher &other) noexcept = default;

        /** Move construct the array allocator. */
        constexpr explicit allocator_watcher(allocator_watcher &&) noexcept
            : count_of_move_construct(1)
        {
            // Ignore the move of informations,
            // Just increment counter for tests
        }

        /** Move assign the array allocator. */
        constexpr allocator_watcher &operator=(allocator_watcher &&) noexcept
        {
            // Ignore the move of informations
            // Just increment counter for tests
            count_of_move_assign++;
            return *this;
        }

        /** Copy assign the array allocator. */
        constexpr allocator_watcher &operator=(const allocator_watcher &) noexcept = default;

        /**
         * Allocate memory block
         * @tparam type_t The element type to allocate
         * @param count Number of element type_t to allocate
         * @return Allocation of the allocated aligned memory block, empty Allocation if failed
         */
        template<typename type_t = u8>
        [[nodiscard]] constexpr memory_allocation_type<type_t> allocate(const usize count) noexcept
        {
            memory_allocation_type<type_t> buffer = allocator.template allocate<type_t>(count);
            if (!buffer.is_empty())
            {
                count_of_alloc++;
            }
            return buffer;
        }

        /**
         * Free memory block
         * @param slice The slice to free
         */
        template<typename type_t>
        constexpr void free(const memory_allocation_type<type_t> &buffer) noexcept
        {
            if (!buffer.is_empty())
            {
                count_of_free++;
            }
            allocator.free(buffer);
        }

        /** Retrieves the count of allocations done. */
        constexpr u32 allocation_count() const noexcept
        {
            return count_of_alloc;
        }

        /** Retrieves the count of free done. */
        constexpr u32 free_count() const noexcept
        {
            return count_of_free;
        }

        /** Retrieves the count of move assignement done. */
        constexpr u32 move_assign_count() const noexcept
        {
            return count_of_move_assign;
        }

        /** Retrieves the count of move construction done. */
        constexpr u32 move_construct_count() const noexcept
        {
            return count_of_move_construct;
        }

        /** Retrieves id the the allocator. */
        constexpr u32 id() const noexcept
        {
            return unique_id;
        }

    private:
        allocator_type allocator;
        u32 count_of_alloc = 0u;
        u32 count_of_free = 0u;
        u32 count_of_move_assign = 0u;
        u32 count_of_move_construct = 0u;
        u32 unique_id = 0;
    };

    template<u32 alignement>
    struct allocator_watcher_2 : public allocator_watcher<alignement>
    {
    };

} // namespace hud_test

namespace hud
{
    template<u32 alignment>
    struct allocator_traits<hud_test::allocator_watcher<alignment>>
    {
        using is_always_equal = hud::true_type;
        using copy_when_container_copy_assigned = hud::false_type;
        using move_when_container_move_assigned = hud::false_type;
        using swap_when_container_swap = hud::false_type;
    };

    template<u32 alignment>
    struct allocator_traits<hud_test::allocator_watcher_2<alignment>>
    {
        using is_always_equal = hud::true_type;
        using copy_when_container_copy_assigned = hud::false_type;
        using move_when_container_move_assigned = hud::false_type;
        using swap_when_container_swap = hud::false_type;
    };
} // namespace hud

#endif // HD_INC_TEST_ALLOCATORS_WATCHER_H
