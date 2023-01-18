#ifndef HD_INC_CORE_HEAP_BUFFER_H
#define HD_INC_CORE_HEAP_BUFFER_H
#include "../traits/add_lvalue_reference.h"
#include "../traits/add_const.h"
#include "../assert.h"
#include "../iterators/random_access_iterator.h"
#include "../slice.h"

namespace hud
{

    /**
     * allocation describes an allocation that refer to a contiguous sequence of type type_t with the first element of the sequence at position zero.
     * Because allocation represent an allocated memory on the heap, it can't be copied but it can be moved to give ownership of the allocated memory to another allocation.
     * allocation is similar to slice by because of it nature and due to the fact that the C++ Standard forbids containers of const elements because allocator<const type_t> is ill-formed,
     * the pointed memory return by data() is not const type_t but type_t even if allocation is const.
     */
    template<typename type_t>
    class allocation
    {

    public:
        /** Mutable slice iterator type */
        using iterator_type = random_access_iterator<type_t *>;
        /** Constant slice iterator type */
        using const_iterator_type = random_access_iterator<const type_t *>;

        /** Default construct with value-initialized slice. */
        constexpr allocation() noexcept = default;

        /**
         * Construct with user-defined pointer and number of elements to sequence.
         * @param first The pointer to the first element of the contiguous sequence of elements
         * @param count The count of elements in the sequence
         */
        HD_FORCEINLINE constexpr allocation(type_t *first, const usize count) noexcept
            : begin_ptr(first)
            , end_ptr(first + count)
        {
        }

        /**
         * Move construct the allocation.
         * @param other The allocation to move
         */
        HD_FORCEINLINE constexpr allocation(allocation &&other) noexcept
            : begin_ptr(other.begin_ptr)
            , end_ptr(other.end_ptr)
        {
            other.begin_ptr = nullptr;
            other.end_ptr = nullptr;
        }

        /**
         * Move assign the allocation.
         * @param other The allocation to move
         * @return *this
         */
        HD_FORCEINLINE constexpr allocation &operator=(allocation &&other) noexcept
        {
            if (this != &other)
            {
                begin_ptr = other.begin_ptr;
                other.begin_ptr = nullptr;
                end_ptr = other.end_ptr;
                other.end_ptr = nullptr;
            }
            return *this;
        }

        /**
         * Reset the allocation but do not free the memory. This function is safe if you free the memory before.
         * Set to internal pointer to nullptr and the count to zero.
         */
        HD_FORCEINLINE constexpr void leak() noexcept
        {
            begin_ptr = nullptr;
            end_ptr = nullptr;
        }

        /**
         * Retrieves reference on the element at the given index.
         * @param index The index of the element to retrieve
         * @return Reference on the element at the given index
         */
        [[nodiscard]] HD_FORCEINLINE constexpr hud::add_lvalue_reference_t<type_t> operator[](const usize index) const noexcept
        {
            check(is_valid_index(index));
            return *(data() + index);
        }

        /** Checks if the allocation is empty or not. */
        [[nodiscard]] HD_FORCEINLINE constexpr bool is_empty() const noexcept
        {
            return begin_ptr == end_ptr;
        }

        /** Retrieves a pointer to the beginning of the sequence. */
        [[nodiscard]] HD_FORCEINLINE constexpr type_t *data() const noexcept
        {
            return begin_ptr;
        }

        /** Retrieves a pointer to the end of the sequence. */
        [[nodiscard]] HD_FORCEINLINE constexpr type_t *data_end() const noexcept
        {
            return end_ptr;
        }

        /**
         * Retrieves a pointer to the elements at given index of the sequence.
         * A special case is allowed when data_at(count()) is called.
         * No assertion is done but the pointer is not valid to dereference.
         * This feature is usefull to get a pointer end to performs pointer aritmetics.
         */
        [[nodiscard]] constexpr type_t *data_at(const usize index) const noexcept
        {
            check(begin_ptr + index <= end_ptr);
            return data() + index;
        }

        /** Retrieves the count of elements the allocation refers to. */
        [[nodiscard]] HD_FORCEINLINE constexpr usize count() const noexcept
        {
            return static_cast<usize>(end_ptr - begin_ptr);
        }

        /** Retrieves the count of bytes the allocation refers to. */
        [[nodiscard]] HD_FORCEINLINE constexpr usize byte_count() const noexcept
        {
            return count() * sizeof(type_t);
        }

        /** Check whether index is in valid range or not. */
        [[nodiscard]] HD_FORCEINLINE constexpr bool is_valid_index(const usize index) const noexcept
        {
            return begin_ptr + index < end_ptr;
        }

        [[nodiscard]] HD_FORCEINLINE constexpr allocation sub_allocation(const usize first_index, const usize count) const noexcept
        {
            check(first_index <= this->count());         // "sub_slice can't start after the end of the slice"
            check(first_index + count <= this->count()); // "sub_slice can't end after the end of the slice"
            return allocation(begin_ptr + first_index, count);
        }

        /**
         * Retrieves a sub-slice of the slice.
         * @param first_index The index of the first element in the slice sequence
         * @param count The count of elements the slice sequence must contains
         * @return The sub-slice from data()+first_index over a sequence of count elements
         */
        [[nodiscard]] HD_FORCEINLINE constexpr slice<type_t> sub_slice(const usize first_index, const usize count) const noexcept
        {
            check(first_index < this->count());          // "sub_slice can't start after the end of the slice"
            check(first_index + count <= this->count()); // "sub_slice can't end after the end of the slice"
            return slice(data() + first_index, count);
        }

        /** Convert the allocation to a slice. */
        [[nodiscard]] HD_FORCEINLINE constexpr slice<type_t> to_slice() const noexcept
        {
            return slice(begin_ptr, count());
        }

        /** Retrieves an iterator_type to the beginning of the slice. */
        [[nodiscard]] HD_FORCEINLINE constexpr iterator_type begin() const noexcept
        {
            return iterator_type(begin_ptr);
        }

        /** Retrieves an iterator_type to the end of the slice. */
        [[nodiscard]] HD_FORCEINLINE constexpr iterator_type end() const noexcept
        {
            return iterator_type(end_ptr);
        }

    private:
        allocation(const allocation &) = delete;
        allocation &operator=(const allocation &) = delete;

    private:
        /** Pointer to the first element */
        type_t *HD_RESTRICT begin_ptr = nullptr;
        /** Number of element */
        type_t *HD_RESTRICT end_ptr = nullptr;
    };

} // namespace hud

#endif // HD_INC_CORE_HEAP_BUFFER_H