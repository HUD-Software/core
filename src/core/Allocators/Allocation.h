#pragma once
#ifndef HD_INC_OSLAYER_HEAP_BUFFER_H
#define HD_INC_OSLAYER_HEAP_BUFFER_H
#include "../Traits/AddLValueReference.h"
#include "../Traits/AddConst.h"
#include "../Assert.h"
#include "../Iterators/RandomAccessIterator.h"
#include "../Slice.h"

namespace hud {

    /**
    * Allocation describes an allocation that refer to a contiguous sequence of type T with the first element of the sequence at position zero.
    * Because Allocation represent an allocated memory on the heap, it can't be copied but it can be moved to give ownership of the allocated memory to another Allocation.
    * Allocation is similar to Slice by because of it nature and due to the fact that the C++ Standard forbids containers of const elements because allocator<const T> is ill-formed, 
    * the pointed memory return by data() is not const T but T even if Allocation is const.
    */
    template<typename T>
    class Allocation {

    public:
        /** Mutable Slice iterator type */
        using Iterator = RandomAccessIterator<T*>;
        /** Constant Slice iterator type */
        using ConstIterator = RandomAccessIterator<const T*>;


        /** Default construct with value-initialized Slice. */
        constexpr Allocation() noexcept = default;

        /**
        * Construct with user-defined pointer and number of elements to sequence.
        * @param first The pointer to the first element of the contiguous sequence of elements
        * @param count The count of elements in the sequence
        */
        HD_FORCEINLINE constexpr Allocation(T* first, const usize count) noexcept
            : begin_ptr(first)
            , end_ptr(first + count) {
        }

        /**
        * Move construct the Allocation.
        * @param other The Allocation to move
        */
        HD_FORCEINLINE constexpr Allocation(Allocation&& other) noexcept
            : begin_ptr(other.begin_ptr)
            , end_ptr(other.end_ptr) {
            other.begin_ptr = nullptr;
            other.end_ptr = nullptr;
        }

        /**
        * Move assign the Allocation.
        * @param other The Allocation to move
        * @return *this
        */
        HD_FORCEINLINE constexpr Allocation& operator=(Allocation&& other) noexcept {
            if (this != &other) {
                begin_ptr = other.begin_ptr;
                other.begin_ptr = nullptr;
                end_ptr = other.end_ptr;
                other.end_ptr = nullptr;
            }
            return *this;
        }

        /**
        * Reset the Allocation but do not free the memory. This function is safe if you free the memory before.
        * Set to internal pointer to nullptr and the count to zero.
        */
        HD_FORCEINLINE constexpr void leak() noexcept {
            begin_ptr = nullptr;
            end_ptr = nullptr;
        }

        /**
        * Retrieves reference on the element at the given index.
        * @param index The index of the element to retrieve
        * @return Reference on the element at the given index
        */
        [[nodiscard]]
        HD_FORCEINLINE constexpr AddLValueReferenceT<T> operator[](const usize index) const noexcept {
            check(is_valid_index(index));
            return *(data() + index);
        }

        /** Checks if the Allocation is empty or not. */
        [[nodiscard]]
        HD_FORCEINLINE constexpr bool is_empty() const noexcept {
            return begin_ptr == end_ptr;
        }

        /** Retrieves a pointer to the beginning of the sequence. */
        [[nodiscard]]
        HD_FORCEINLINE constexpr T* data() const noexcept {
            return begin_ptr;
        }

        /** Retrieves a pointer to the end of the sequence. */
        [[nodiscard]]
        HD_FORCEINLINE constexpr T* data_end() const noexcept {
            return end_ptr;
        }

        /**
        * Retrieves a pointer to the elements at given index of the sequence.
        * A special case is allowed when data_at(count()) is called.
        * No assertion is done but the pointer is not valid to dereference.
        * This feature is usefull to get a pointer end to performs pointer aritmetics.
        */
        [[nodiscard]]
        constexpr T* data_at(const usize index) const noexcept {
            check(begin_ptr + index <= end_ptr);
            return data() + index;
        }

        /** Retrieves the count of elements the Allocation refers to. */
        [[nodiscard]]
        HD_FORCEINLINE constexpr usize count() const noexcept {
            return static_cast<usize>(end_ptr - begin_ptr);
        }

        /** Retrieves the count of bytes the Allocation refers to. */
        [[nodiscard]]
        HD_FORCEINLINE constexpr usize byte_count() const noexcept {
            return count() * sizeof(T);
        }

        /** Check whether index is in valid range or not. */
        [[nodiscard]]
        HD_FORCEINLINE constexpr bool is_valid_index(const usize index) const noexcept {
            return begin_ptr + index < end_ptr;
        }

        [[nodiscard]]
        HD_FORCEINLINE constexpr Allocation sub_allocation(const usize first_index, const usize count) const noexcept {
            check(first_index <= this->count()); // "sub_slice can't start after the end of the Slice"
            check(first_index + count <= this->count()); // "sub_slice can't end after the end of the Slice"
            return Allocation(begin_ptr+first_index, count);
        }

        /**
        * Retrieves a sub-Slice of the Slice.
        * @param first_index The index of the first element in the Slice sequence
        * @param count The count of elements the Slice sequence must contains
        * @return The sub-Slice from data()+first_index over a sequence of count elements
        */
        [[nodiscard]]
        HD_FORCEINLINE constexpr Slice<T> sub_slice(const usize first_index, const usize count) const noexcept {
            check(first_index < this->count()); // "sub_slice can't start after the end of the Slice"
            check(first_index + count <= this->count()); // "sub_slice can't end after the end of the Slice"
            return Slice(data() + first_index, count);
        }

        /** Convert the allocation to a slice. */
        [[nodiscard]]
        HD_FORCEINLINE constexpr Slice<T> to_slice() const noexcept {
            return Slice(begin_ptr, count());
        }

        /** Retrieves an ConstIterator to the beginning of the Slice. */
        [[nodiscard]]
        HD_FORCEINLINE constexpr Iterator begin() const noexcept {
            return Iterator(begin_ptr);
        }

        /** Retrieves an ConstIterator to the end of the Slice. */
        [[nodiscard]]
        HD_FORCEINLINE constexpr Iterator end() const noexcept {
            return Iterator(end_ptr);
        }

     private:
         Allocation(const Allocation&) = delete;
         Allocation& operator=(const Allocation&) = delete;

     private:
         /** Pointer to the first element */
         T* HD_RESTRICT begin_ptr = nullptr;
         /** Number of element */
         T* HD_RESTRICT end_ptr = nullptr;
     };

} // namespace hud

#endif  // HD_INC_OSLAYER_HEAP_BUFFER_H