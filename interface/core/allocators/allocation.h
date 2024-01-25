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
     * `memory_allocation` describes an allocation that refer to a contiguous sequence of type type_t with the first element of the sequence at position zero.
     * Because memory_allocation represent an allocated memory on the heap, it can't be copied but it can be moved to give ownership of the allocated memory to another allocation.
     * memory_allocation is similar to slice by because of it nature and due to the fact that the C++ Standard forbids containers of const elements because allocator<const type_t> is ill-formed,
     * the pointed memory return by data() is not const type_t but type_t even if allocation is const.
     */
    template<typename type_t>
    class memory_allocation
    {

    public:
        /** Mutable slice iterator type */
        using iterator_type = random_access_iterator<type_t *>;
        /** Constant slice iterator type */
        using const_iterator_type = random_access_iterator<const type_t *>;
        /** The element type of the allocation. */
        using element_type = type_t;
        /** The pointer to element type of the allocation. */
        using pointer_type = element_type *;

        /** Default construct with value-initialized slice. */
        constexpr memory_allocation() noexcept = default;

        /**
         * Constructor for the `memory_allocation` class with specified `begin` and `end` pointers.
         * @param begin The pointer to the first element of the contiguous sequence of elements
         * @param end The pointer one past the last element of the contiguous sequence
         */
        HD_FORCEINLINE constexpr memory_allocation(type_t *begin, type_t *end) noexcept
            : begin_ptr(begin)
            , end_ptr(end)
        {
        }

        /**
         * Constructor for the `memory_allocation` class with a user-defined pointer and a specified number of elements.
         * @param first The pointer to the first element of the contiguous sequence of elements
         * @param count The count of elements in the sequence
         */
        HD_FORCEINLINE constexpr memory_allocation(type_t *first, const usize count) noexcept
            : begin_ptr(first)
            , end_ptr(first + count)
        {
        }

        /**
         * Move constructor for the `memory_allocation` class, taking ownership of `other`.
         * @param other The `memory_allocation` to be moved
         */
        HD_FORCEINLINE constexpr memory_allocation(memory_allocation &&other) noexcept
            : begin_ptr(other.begin_ptr)
            , end_ptr(other.end_ptr)
        {
            other.begin_ptr = nullptr;
            other.end_ptr = nullptr;
        }

        /**
         * Move assignment operator for the `memory_allocation` class, taking ownership of `other`.
         * @param other The `memory_allocation` to be moved
         * @return Reference to *this
         */
        HD_FORCEINLINE constexpr memory_allocation &operator=(memory_allocation &&other) noexcept
        {
            if (this != &other) [[likely]]
            {
                begin_ptr = other.begin_ptr;
                other.begin_ptr = nullptr;
                end_ptr = other.end_ptr;
                other.end_ptr = nullptr;
            }
            return *this;
        }

        /**
         * Reset the `memory_allocation` without freeing the memory. This function is safe to use if memory has been deallocated separately.
         * Sets the internal pointers to nullptr and the count to zero.
         */
        HD_FORCEINLINE constexpr void leak() noexcept
        {
            begin_ptr = nullptr;
            end_ptr = nullptr;
        }

        /**
         * Retrieves a reference to the element at the specified `index`.
         * @param index The `index` of the element to retrieve
         * @return Reference to the element at the specified `index`
         */
        [[nodiscard]] HD_FORCEINLINE constexpr hud::add_lvalue_reference_t<type_t> operator[](const usize index) const noexcept
        {
            check(is_valid_index(index));
            return *(data() + index);
        }

        /** Checks if the `memory_allocation` is empty. */
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
         * Retrieves a pointer to the element at the given `index` of the sequence.
         * A special case is allowed when `data_at(count())` is called.
         * No assertion is done, but the pointer is not valid for dereferencing.
         * This feature is useful for obtaining a pointer to the end to perform pointer arithmetic.
         */
        [[nodiscard]] constexpr type_t *data_at(const usize index) const noexcept
        {
            check(begin_ptr + index <= end_ptr);
            return data() + index;
        }

        /** Retrieves the count of elements in the `memory_allocation`. */
        [[nodiscard]] HD_FORCEINLINE constexpr usize count() const noexcept
        {
            return static_cast<usize>(end_ptr - begin_ptr);
        }

        /** Retrieves the count of bytes in the `memory_allocation`. */
        [[nodiscard]] HD_FORCEINLINE constexpr usize byte_count() const noexcept
        {
            return count() * sizeof(type_t);
        }

        /** Checks whether `index` is in a valid range. */
        [[nodiscard]] HD_FORCEINLINE constexpr bool is_valid_index(const usize index) const noexcept
        {
            return begin_ptr + index < end_ptr;
        }

        /**
         * Reinterprets the bits of the `memory_allocation` type as the given type.
         * Caution: Be very careful when using this method; struct alignment may not be respected.
         * This is a reinterpret_cast.
         * @tparam u_type_t The type to reinterpret the `memory_allocation` as
         * @return The `memory_allocation` transmuted to another type
         */
        template<typename u_type_t>
        [[nodiscard]] memory_allocation<u_type_t> HD_FORCEINLINE reinterpret_cast_to() const noexcept
        {
            static_assert(alignof(u_type_t) == alignof(type_t), "Alignement requirement mismatch");
            return memory_allocation<u_type_t> {reinterpret_cast<u_type_t *>(begin_ptr), reinterpret_cast<u_type_t *>(end_ptr)};
        }

        /**
         * Retrieves a slice of a part or the entire allocation of the `memory_allocation`.
         * @param first_index The index of the first element in the slice sequence
         * @param count The count of elements in the sub-slice
         * @return The sub-slice starting from data() + first_index over a sequence of count elements
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
        memory_allocation(const memory_allocation &) = delete;
        memory_allocation &operator=(const memory_allocation &) = delete;

    private:
        /** Pointer to the first element */
        pointer_type HD_RESTRICT begin_ptr = nullptr;
        /** Number of element */
        pointer_type HD_RESTRICT end_ptr = nullptr;
    };

} // namespace hud

#endif // HD_INC_CORE_HEAP_BUFFER_H