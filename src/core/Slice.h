#pragma once
#ifndef HD_INC_OSLAYER_SLICE_H
#define HD_INC_OSLAYER_SLICE_H
//#include "Templates/Swap.h"
#include "Assert.h"
#include "Iterators/RandomAccessIterator.h"
#include "Traits/IsUnsigned.h"

namespace hud {

    /**
    * Slice describes an object that can refer to a contiguous sequence of elements to type T
    * with the first element of the sequence at position zero.
    * @tparam T The element type
    */
    template<typename T>
    class Slice {
    public:

        /** Mutable Slice iterator type */
        using Iterator = RandomAccessIterator<T*>;
        /** Constant Slice iterator type */
        using ConstIterator = RandomAccessIterator<const T*>;

        /** Default construct with value-initialized Slice. */
        constexpr Slice() noexcept = default;

        /**
        * Construct with user-defined begin_ptr and number of elements to sequence.
        * @param first The begin_ptr to the first element of the contiguous sequence of elements
        * @param count The number of elements in the sequence
        */
        HD_FORCEINLINE constexpr Slice(T* first, const usize count) noexcept
            : begin_ptr(first)
            , count_element(count) {
        }

        /** Copy construct the Slice. */
        constexpr Slice(const Slice& other) noexcept = default;

        /** Move construct the Slice. */
        HD_FORCEINLINE constexpr Slice(Slice&& other) noexcept
            : begin_ptr(other.begin_ptr)
            , count_element(other.count_element) {
            other.begin_ptr = nullptr;
            other.count_element = 0u;
        }

        /** Copy assign the Slice. */
        constexpr Slice& operator=(const Slice& other) noexcept = default;

        /** Move assign the Slice. */
        HD_FORCEINLINE constexpr Slice& operator=(Slice&& other) noexcept {
            if (this != &other) {
                begin_ptr = other.begin_ptr;
                other.begin_ptr = nullptr;
                count_element = other.count_element;
                other.count_element = 0u;
            }
            return *this;
        }

        /** Reset the Slice to empty. */
        HD_FORCEINLINE constexpr void reset() noexcept {
            begin_ptr = nullptr;
            count_element = 0u;
        }

        /** Retrieves reference on the element at the given index. */
        [[nodiscard]]
        HD_FORCEINLINE constexpr T& operator[](const usize index) noexcept {
            check(is_valid_index(index));
            return *(data() + index);
        }

        /** Retrieves reference on the element at the given index. */
        [[nodiscard]]
        HD_FORCEINLINE constexpr const T& operator[](const usize index) const noexcept {
            check(is_valid_index(index));
            return *(data() + index);
        }

        /** Checks if the Slice is empty or not. */
        [[nodiscard]]
        HD_FORCEINLINE constexpr bool is_empty() const noexcept {
            return count_element == 0u;
        }

        /** Retrieves a begin_ptr to the beginning of the sequence. */
        [[nodiscard]]
        HD_FORCEINLINE constexpr T* data() noexcept {
            return begin_ptr;
        }

        /** Retrieves a begin_ptr to the beginning of the sequence. */
        [[nodiscard]]
        HD_FORCEINLINE constexpr const T* data() const noexcept {
            return begin_ptr;
        }

        /** Retrieves the number of elements the Slice refers to. */
        [[nodiscard]]
        HD_FORCEINLINE constexpr usize count() const noexcept {
            return count_element;
        }

        /** Retrieves the count of bytes the Slice refers to. */
        [[nodiscard]]
        HD_FORCEINLINE constexpr usize byte_count() const noexcept {
            return count() * sizeof(T);
        }

        /** Check whether index is in valid range or not. */
        [[nodiscard]]
        HD_FORCEINLINE constexpr bool is_valid_index(const usize index) const noexcept {
            // When index is unsigned, we don't need to check for negative values
            static_assert(IsUnsignedV<decltype(index)>);
            return begin_ptr + index < begin_ptr + count_element;
        }

        /**
        * Retrieves a sub Slice of the Slice.
        * @param first_index The index of the first element in the Slice sequence
        * @param count The number of elements the Slice sequence must contains
        * @return The sub Slice from data()+first_index over a sequence of count elements
        */
        [[nodiscard]]
        HD_FORCEINLINE constexpr Slice sub_slice(const usize first_index, const usize count) noexcept {
            check(first_index < this->count()); // "sub_slice can't start after the end of the Slice"
            check(first_index + count <= this->count()); // "sub_slice can't end after the end of the Slice"
            return Slice(data() + first_index, count);
        }

        /**
        * Retrieves a sub-Slice of the Slice.
        * @param first_index The index of the first element in the Slice sequence
        * @param count The number of elements the Slice sequence must contains
        * @return The sub-Slice from data()+first_index over a sequence of count elements
        */
        [[nodiscard]]
        HD_FORCEINLINE constexpr const Slice sub_slice(const usize first_index, const usize count) const noexcept {
            check(first_index < this->count()); // "sub_slice can't start after the end of the Slice"
            check(first_index + count <= this->count()); // "sub_slice can't end after the end of the Slice"
            return Slice(data() + first_index, count);
        }

        /** Retrieves an Iterator to the beginning of the Slice. */
        [[nodiscard]]
        HD_FORCEINLINE constexpr Iterator begin() noexcept {
            return Iterator(begin_ptr);
        }

        /** Retrieves an ConstIterator to the beginning of the Slice. */
        [[nodiscard]]
        HD_FORCEINLINE constexpr ConstIterator begin() const noexcept {
            return ConstIterator(begin_ptr);
        }

        /** Retrieves an Iterator to the end of the Slice. */
        [[nodiscard]]
        HD_FORCEINLINE constexpr Iterator end() noexcept {
            return Iterator(begin_ptr + count_element);
        }

        /** Retrieves an ConstIterator to the end of the Slice. */
        [[nodiscard]]
        HD_FORCEINLINE constexpr ConstIterator end() const noexcept {
            return ConstIterator(begin_ptr + count_element);
        }

    private:
        /** Pointer to the first element */
        T* begin_ptr = nullptr;
        /** Count of element */
        usize count_element = 0u;
    };

} // namespace hud

#endif  // HD_INC_OSLAYER_SLICE_H