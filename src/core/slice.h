#pragma once
#ifndef HD_INC_CORE_SLICE_H
#define HD_INC_CORE_SLICE_H
// #include "templates/swap.h"
#include "assert.h"
#include "iterators/random_access_iterator.h"
#include "traits/is_unsigned.h"

namespace hud
{

    /**
     * slice describes an object that can refer to a contiguous sequence of elements to type type_t
     * with the first element of the sequence at position zero.
     * @tparam type_t The element type
     */
    template <typename type_t>
    class slice
    {
    public:
        /** Mutable slice iterator type */
        using iterator = random_access_iterator<type_t *>;
        /** Constant slice iterator type */
        using const_iterator = random_access_iterator<const type_t *>;

        /** Default construct with value-initialized slice. */
        constexpr slice() noexcept = default;

        /**
         * Construct with user-defined begin_ptr and number of elements to sequence.
         * @param first The begin_ptr to the first element of the contiguous sequence of elements
         * @param count The number of elements in the sequence
         */
        HD_FORCEINLINE constexpr slice(type_t *first, const usize count) noexcept
            : begin_ptr(first), count_element(count)
        {
        }

        /** Copy construct the slice. */
        constexpr slice(const slice &other) noexcept = default;

        /** Move construct the slice. */
        HD_FORCEINLINE constexpr slice(slice &&other) noexcept
            : begin_ptr(other.begin_ptr), count_element(other.count_element)
        {
            other.begin_ptr = nullptr;
            other.count_element = 0u;
        }

        /** Copy assign the slice. */
        constexpr slice &operator=(const slice &other) noexcept = default;

        /** Move assign the slice. */
        HD_FORCEINLINE constexpr slice &operator=(slice &&other) noexcept
        {
            if (this != &other)
            {
                begin_ptr = other.begin_ptr;
                other.begin_ptr = nullptr;
                count_element = other.count_element;
                other.count_element = 0u;
            }
            return *this;
        }

        /** Reset the slice to empty. */
        HD_FORCEINLINE constexpr void reset() noexcept
        {
            begin_ptr = nullptr;
            count_element = 0u;
        }

        /** Retrieves reference on the element at the given index. */
        [[nodiscard]] HD_FORCEINLINE constexpr type_t &operator[](const usize index) noexcept
        {
            return *data_at(index);
        }

        /** Retrieves reference on the element at the given index. */
        [[nodiscard]] HD_FORCEINLINE constexpr const type_t &operator[](const usize index) const noexcept
        {
            return const_cast<slice &>(*this)[index];
        }

        /** Checks if the slice is empty or not. */
        [[nodiscard]] HD_FORCEINLINE constexpr bool is_empty() const noexcept
        {
            return count_element == 0u;
        }

        /** Retrieves a pointer to the beginning of the sequence. */
        [[nodiscard]] HD_FORCEINLINE constexpr type_t *data() noexcept
        {
            return begin_ptr;
        }

        /** Retrieves a pointer to the beginning of the sequence. */
        [[nodiscard]] HD_FORCEINLINE constexpr const type_t *data() const noexcept
        {
            return begin_ptr;
        }

        /** Retrieves a pointer to the the sequence at the given index. */
        [[nodiscard]] HD_FORCEINLINE constexpr type_t *data_at(const usize index) noexcept
        {
            check(is_valid_index(index));
            return data() + index;
        }

        /** Retrieves a pointer to the the sequence at the given index. */
        [[nodiscard]] HD_FORCEINLINE constexpr const type_t *data_at(const usize index) const noexcept
        {
            return const_cast<slice &>(*this).data_at(index);
        }

        /** Retrieves the number of elements the slice refers to. */
        [[nodiscard]] HD_FORCEINLINE constexpr usize count() const noexcept
        {
            return count_element;
        }

        /** Retrieves the count of bytes the slice refers to. */
        [[nodiscard]] HD_FORCEINLINE constexpr usize byte_count() const noexcept
        {
            return count() * sizeof(type_t);
        }

        /** Check whether index is in valid range or not. */
        [[nodiscard]] HD_FORCEINLINE constexpr bool is_valid_index(const usize index) const noexcept
        {
            // When index is unsigned, we don't need to check for negative values
            static_assert(is_unsigned_v<decltype(index)>);
            return count() - index > 0u;
        }

        /** Check whether a range from index of countis in valid or not. */
        [[nodiscard]] HD_FORCEINLINE constexpr bool is_valid_range(const usize start_index, const usize range_count) const noexcept
        {
            return is_valid_index(start_index) && (start_index + range_count) <= count(); // Valid if more element is available than required
        }

        /**
         * Retrieves a sub slice of the slice.
         * @param first_index The index of the first element in the slice sequence
         * @param count The number of elements the slice sequence must contains
         * @return The sub slice from data()+first_index over a sequence of count elements
         */
        [[nodiscard]] HD_FORCEINLINE constexpr slice sub_slice(const usize first_index, const usize count) noexcept
        {
            check(is_valid_range(first_index, count));
            return slice(data_at(first_index), count);
        }

        /**
         * Retrieves a sub-slice of the slice.
         * @param first_index The index of the first element in the slice sequence
         * @param count The number of elements the slice sequence must contains
         * @return The sub-slice from data()+first_index over a sequence of count elements
         */
        [[nodiscard]] HD_FORCEINLINE constexpr const slice sub_slice(const usize first_index, const usize count) const noexcept
        {
            return const_cast<slice &>(*this).sub_slice(first_index, count);
        }

        /** Retrieves an iterator to the beginning of the slice. */
        [[nodiscard]] HD_FORCEINLINE constexpr iterator begin() noexcept
        {
            return iterator(begin_ptr);
        }

        /** Retrieves an const_iterator to the beginning of the slice. */
        [[nodiscard]] HD_FORCEINLINE constexpr const_iterator begin() const noexcept
        {
            return const_iterator(begin_ptr);
        }

        /** Retrieves an iterator to the end of the slice. */
        [[nodiscard]] HD_FORCEINLINE constexpr iterator end() noexcept
        {
            return iterator(begin_ptr + count_element);
        }

        /** Retrieves an const_iterator to the end of the slice. */
        [[nodiscard]] HD_FORCEINLINE constexpr const_iterator end() const noexcept
        {
            return const_iterator(begin_ptr + count_element);
        }

    private:
        /** Pointer to the first element */
        type_t *begin_ptr = nullptr;
        /** Count of element */
        usize count_element = 0u;
    };

} // namespace hud

#endif // HD_INC_CORE_SLICE_H