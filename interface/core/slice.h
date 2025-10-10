#ifndef HD_INC_CORE_SLICE_H
#define HD_INC_CORE_SLICE_H
// #include "templates/swap.h"
#include "assert.h"
#include "iterators/random_access_iterator.h"
#include "traits/is_unsigned.h"

namespace hud
{

    /**
     * Represents a contiguous sequence of elements of type `type_t`, starting at index zero.
     * This class provides bounds-checked access, iteration, and the ability to create sub-slices.
     * Note: Copy and move operations do NOT perform deep copies; they only copy the pointer and the count.
     * @tparam type_t The element type of the slice.
     */
    template<typename type_t>
    class slice
    {
    public:
        /** Mutable iterator type for the slice. */
        using iterator = random_access_iterator<type_t *>;
        /** Constant iterator type for the slice. */
        using const_iterator = random_access_iterator<const type_t *>;

        /** Default constructs an empty slice. */
        constexpr slice() noexcept = default;

        /**
         * Constructs a slice from a pointer to the first element and a number of elements.
         * @param first Pointer to the first element of the contiguous sequence.
         * @param count Number of elements in the sequence.
         */
        HD_FORCEINLINE constexpr slice(type_t *first, const usize count) noexcept
            : begin_ptr_(first)
            , count_element_(count)
        {
        }

        /** Copy constructor (shallow copy, does NOT copy underlying elements). */
        constexpr slice(const slice &other) noexcept = default;

        /** Move constructor (shallow move, resets moved-from slice to empty). */
        HD_FORCEINLINE constexpr slice(slice &&other) noexcept
            : begin_ptr_(other.begin_ptr_)
            , count_element_(other.count_element_)
        {
            other.begin_ptr_ = nullptr;
            other.count_element_ = 0u;
        }

        /** Copy assignment (shallow copy, does NOT copy underlying elements). */
        constexpr slice &operator=(const slice &other) noexcept = default;

        /** Move assignment (shallow move, resets moved-from slice to empty). */
        HD_FORCEINLINE constexpr slice &operator=(slice &&other) noexcept
        {
            if (this != &other) {
                begin_ptr_ = other.begin_ptr_;
                other.begin_ptr_ = nullptr;
                count_element_ = other.count_element_;
                other.count_element_ = 0u;
            }
            return *this;
        }

        /** Resets the slice to empty. */
        HD_FORCEINLINE constexpr void reset() noexcept
        {
            begin_ptr_ = nullptr;
            count_element_ = 0u;
        }

        /** Returns a reference to the element at the given index (bounds-checked). */
        [[nodiscard]] HD_FORCEINLINE constexpr type_t &operator[](const usize index) noexcept
        {
            return *data_at(index);
        }

        /** Returns a const reference to the element at the given index (bounds-checked). */
        [[nodiscard]] HD_FORCEINLINE constexpr const type_t &operator[](const usize index) const noexcept
        {
            return const_cast<slice &>(*this)[index];
        }

        /** Checks if the slice is empty or not. */
        [[nodiscard]] HD_FORCEINLINE constexpr bool is_empty() const noexcept
        {
            return count_element_ == 0u;
        }

        /** Returns  a pointer to the beginning of the sequence. */
        [[nodiscard]] HD_FORCEINLINE constexpr type_t *data() noexcept
        {
            return begin_ptr_;
        }

        /** Returns  a pointer to the beginning of the sequence. */
        [[nodiscard]] HD_FORCEINLINE constexpr const type_t *data() const noexcept
        {
            return begin_ptr_;
        }

        /** Returns  a pointer to the the sequence at the given index. */
        [[nodiscard]] HD_FORCEINLINE constexpr type_t *data_at(const usize index) noexcept
        {
            check(is_valid_index(index));
            return data() + index;
        }

        /** Returns  a pointer to the the sequence at the given index. */
        [[nodiscard]] HD_FORCEINLINE constexpr const type_t *data_at(const usize index) const noexcept
        {
            return const_cast<slice &>(*this).data_at(index);
        }

        /** Returns the number of elements the slice covers. */
        [[nodiscard]] HD_FORCEINLINE constexpr usize count() const noexcept
        {
            return count_element_;
        }

        /** Returns the number of bytes the slice covers. */
        [[nodiscard]] HD_FORCEINLINE constexpr usize byte_count() const noexcept
        {
            return count() * sizeof(type_t);
        }

        /** Returns true if the range [start_index, start_index+range_count) is valid. */
        [[nodiscard]] HD_FORCEINLINE constexpr bool is_valid_index(const usize index) const noexcept
        {
            // When index is unsigned, we don't need to check for negative values
            static_assert(is_unsigned_v<decltype(index)>);
            return count() - index > 0u;
        }

        /** Returns true if the range [start_index, start_index+range_count) is valid. */
        [[nodiscard]] HD_FORCEINLINE constexpr bool is_valid_range(const usize start_index, const usize range_count) const noexcept
        {
            return is_valid_index(start_index) && (start_index + range_count) <= count(); // Valid if more element is available than required
        }

        /**
         * Returns a sub-slice starting at `first_index` spanning `count` elements.
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
         * Returns a sub-slice starting at `first_index` spanning `count` elements.
         * @param first_index The index of the first element in the slice sequence
         * @param count The number of elements the slice sequence must contains
         * @return The sub-slice from data()+first_index over a sequence of count elements
         */
        [[nodiscard]] HD_FORCEINLINE constexpr const slice sub_slice(const usize first_index, const usize count) const noexcept
        {
            return const_cast<slice &>(*this).sub_slice(first_index, count);
        }

        /** Returns an iterator to the beginning of the slice. */
        [[nodiscard]] HD_FORCEINLINE constexpr iterator begin() noexcept
        {
            return iterator(begin_ptr_);
        }

        /** Returns an const_iterator to the beginning of the slice. */
        [[nodiscard]] HD_FORCEINLINE constexpr const_iterator begin() const noexcept
        {
            return const_iterator(begin_ptr_);
        }

        /** Returns an iterator to the end of the slice. */
        [[nodiscard]] HD_FORCEINLINE constexpr iterator end() noexcept
        {
            return iterator(begin_ptr_ + count_element_);
        }

        /** Returns an const_iterator to the end of the slice. */
        [[nodiscard]] HD_FORCEINLINE constexpr const_iterator end() const noexcept
        {
            return const_iterator(begin_ptr_ + count_element_);
        }

    private:
        /** Pointer to the first element. */
        type_t *begin_ptr_ = nullptr;
        /** Count of elements. */
        usize count_element_ = 0u;
    };

    /**
     * Class template argument deduction guide for C-style arrays of const elements.
     * Deduces a slice of const type from a const C-style array.
     */
    template<typename type_t, size_t N>
    slice(const type_t (&)[N]) -> slice<const type_t>;

    /**
     * Class template argument deduction guide for C-style arrays of mutable elements.
     * Deduces a slice of mutable type from a C-style array.
     */
    template<typename type_t, size_t N>
    slice(type_t (&)[N]) -> slice<type_t>;
} // namespace hud

#endif // HD_INC_CORE_SLICE_H