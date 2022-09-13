#pragma once
#ifndef HD_INC_OSLAYER_RANDOM_ACCESS_ITERATOR_H
#define HD_INC_OSLAYER_RANDOM_ACCESS_ITERATOR_H

namespace hud {

    /**
    * Forward declaration of random access iterator on container
    * Specialize for differente random accessible container types like array, c style array etc...
    */
    template<typename Container>
    class RandomAccessIterator;

    /**
    * Random access iterator on a continuous allocated elements of type T.
    * @tparam T The type of object in the C array
    * @tparam size The size of the C array
    */
    template<typename T>
    class RandomAccessIterator<T*> {

    public:
        /**
        * Construct a RandomAccessIterator on the elements, starting at the given index.
        * @param elements Pointer to the continuous allocated elements.
        * @param index Index of the element to reach after the elements in the sequence
        */
        constexpr RandomAccessIterator(T* elements, const usize index = 0) noexcept
            : pointer(elements + index) {
        }

        /** Pre-increment the iterator. */
        constexpr RandomAccessIterator& operator++() noexcept {
            pointer++;
            return *this;
        }

        /** Post-increment the iterator. */
        constexpr RandomAccessIterator operator++(i32) noexcept {
            RandomAccessIterator tmp(*this);
            pointer++;
            return tmp;
        }

        /** Pre-decrement the iterator. */
        constexpr RandomAccessIterator& operator--() noexcept {
            pointer--;
            return *this;
        }

        /** Post-decrement the iterator. */
        constexpr RandomAccessIterator operator--(i32) noexcept {
            RandomAccessIterator tmp(*this);
            pointer--;
            return tmp;
        }

        /** Move the iterator by incrementing diff index. */
        constexpr RandomAccessIterator& operator+=(const usize diff) noexcept {
            pointer += diff;
            return *this;
        }

        /** Retrieves a RandomAccessIterator that move this by incrementing diff index. */
        [[nodiscard]]
        constexpr RandomAccessIterator operator+(const usize diff) const noexcept {
            return RandomAccessIterator(pointer + diff);
        }

        /** Move the iterator by decrementing diff index. */
        constexpr RandomAccessIterator& operator-=(const usize diff) noexcept {
            pointer -= diff;
            return *this;
        }

        /** Retrieves a RandomAccessIterator that move this by decrementing diff index. */
        [[nodiscard]]
        constexpr RandomAccessIterator operator-(const usize diff) const noexcept {
            return RandomAccessIterator(pointer - diff);
        }

        /** Dereference the iterator. */
        [[nodiscard]]
        constexpr T& operator*() const noexcept {
            return *pointer;
        }

        /** Dereference the iterator. */
        [[nodiscard]]
        constexpr T* operator->() const noexcept {
            return pointer;
        }

        /** Retrieves the difference with another RandomAccessIterator. */
        [[nodiscard]]
        constexpr isize operator-(const RandomAccessIterator& other) noexcept {
            return pointer - other.pointer;
        }

        /** Checks if 2 RandomAccessIterator are equals. */
        [[nodiscard]]
        constexpr bool operator==(const RandomAccessIterator& other) const noexcept {
            return pointer == other.pointer;
        }

        /** Checks if 2 RandomAccessIterator are not equals. */
        [[nodiscard]]
        constexpr bool operator!=(const RandomAccessIterator& other) const noexcept {
            return pointer != other.pointer;
        }

    private:
        /** Reference to the container to iterate */
        T* pointer;
    };

    /**
    * Random access  iterator on a C array of object of type TType
    * @tparam T The type of object in the C array
    * @tparam size The size of the C array
    */
    template<typename T, usize size>
    class RandomAccessIterator<T[size]> {

    public:
        /**
        * Construct a RandomAccessIterator on the elements, starting at the given index.
        * @param elements Pointer to the continuous allocated elements.
        * @param index Index of the element to reach after the elements in the sequence
        */
        constexpr RandomAccessIterator(T* elements, const usize index = 0) noexcept
            : pointer(elements + index) {
        }

        /** Pre-increment the iterator. */
        constexpr RandomAccessIterator& operator++() noexcept {
            pointer++;
            return *this;
        }

        /** Post-increment the iterator. */
        constexpr RandomAccessIterator operator++(i32) noexcept {
            RandomAccessIterator tmp(*this);
            pointer++;
            return tmp;
        }

        /** Pre-decrement the iterator. */
        constexpr RandomAccessIterator& operator--() noexcept {
            pointer--;
            return *this;
        }

        /** Post-decrement the iterator. */
        constexpr RandomAccessIterator operator--(i32) noexcept {
            RandomAccessIterator tmp(*this);
            pointer--;
            return tmp;
        }

        /** Move the iterator by incrementing diff index. */
        constexpr RandomAccessIterator& operator+=(const usize diff) noexcept {
            pointer += diff;
            return *this;
        }

        /** Retrieves a RandomAccessIterator that move this by incrementing diff index. */
        [[nodiscard]]
        constexpr RandomAccessIterator operator+(const usize diff) const noexcept {
            return RandomAccessIterator(pointer + diff);
        }

        /** Move the iterator by decrementing diff index. */
        constexpr RandomAccessIterator& operator-=(const usize diff) noexcept {
            pointer -= diff;
            return *this;
        }

        /** Retrieves a RandomAccessIterator that move this by decrementing diff index. */
        [[nodiscard]]
        constexpr RandomAccessIterator operator-(const usize diff) const noexcept {
            return RandomAccessIterator(pointer - diff);
        }

        /** Dereference the iterator. */
        [[nodiscard]]
        constexpr T& operator*() const noexcept {
            return *pointer;
        }

        /** Dereference the iterator. */
        [[nodiscard]]
        constexpr T* operator->() const noexcept {
            return pointer;
        }

        /** Retrieves the difference with another RandomAccessIterator. */
        [[nodiscard]]
        constexpr isize operator-(const RandomAccessIterator& other) noexcept {
            return pointer - other.pointer;
        }

        /** Checks if 2 RandomAccessIterator are equals. */
        [[nodiscard]]
        constexpr bool operator==(const RandomAccessIterator& other) const noexcept {
            return pointer == other.pointer;
        }

        /** Checks if 2 RandomAccessIterator are not equals. */
        [[nodiscard]]
        constexpr bool operator!=(const RandomAccessIterator& other) const noexcept {
            return pointer != other.pointer;
        }

    private:
        /** Reference to the container to iterate */
        T* pointer;
    };

}  // namespace hud

#endif // HD_INC_OSLAYER_RANDOM_ACCESS_ITERATOR_H
