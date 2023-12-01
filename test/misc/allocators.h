#ifndef HD_INC_TEST_ARRAY_ALLOCATORS_H
#define HD_INC_TEST_ARRAY_ALLOCATORS_H
#include <core/allocators/aligned_heap_allocator.h>
#include <core/allocators/allocation.h>

namespace hud_test
{
    template<u32 alignement>
    struct array_allocator
    {
        using allocator_type = hud::aligned_heap_allocator<alignement>;

        template<typename type_t>
        using allocation_type = hud::allocation<type_t>;

    public:
        /** Default constructor. */
        constexpr array_allocator() noexcept = default;

        /** Construct allocator with a unique id. */
        constexpr array_allocator(u32 id) noexcept
            : unique_id(id)
        {
        }

        /** Copy constructor. */
        constexpr array_allocator(const array_allocator &other) noexcept = default;

        /** Move construct the array allocator. */
        constexpr array_allocator(array_allocator &&) noexcept
            : count_of_move_construct(1)
        {
            // Ignore the move of informations,
            // Just increment counter for tests
        }

        /** Move assign the array allocator. */
        constexpr array_allocator &operator=(array_allocator &&) noexcept
        {
            // Ignore the move of informations
            // Just increment counter for tests
            count_of_move_assign++;
            return *this;
        }

        /** Copy assign the array allocator. */
        constexpr array_allocator &operator=(const array_allocator &) noexcept = default;

        /**
         * Allocate memory block
         * @tparam type_t The element type to allocate
         * @param count Number of element type_t to allocate
         * @return Allocation of the allocated aligned memory block, empty Allocation if failed
         */
        template<typename type_t = u8>
        [[nodiscard]] constexpr allocation_type<type_t> allocate(const usize count) noexcept
        {
            auto buffer = allocator.template allocate<type_t>(count);
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
        template<typename type_t = u8>
        constexpr void free(allocation_type<type_t> &buffer) noexcept
        {
            if (!buffer.is_empty())
            {
                count_of_free++;
            }
            allocator.free(buffer);
        }

        /** Retrieves count of allocation done. */
        constexpr u32 allocation_count() const noexcept
        {
            return count_of_alloc;
        }

        /** Retrieves count of free done. */
        constexpr u32 free_count() const noexcept
        {
            return count_of_free;
        }

        /** Retrieves count of move assignement done. */
        constexpr u32 move_assign_count() const noexcept
        {
            return count_of_move_assign;
        }

        /** Retrieves count of move construction done. */
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
    struct array_allocator_2 : public array_allocator<alignement>
    {
    };

} // namespace hud_test

#endif // HD_INC_TEST_ARRAY_ALLOCATORS_H
