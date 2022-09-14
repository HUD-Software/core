#pragma once
#ifndef HD_INC_CORE_RANDOM_ACCESS_ITERATOR_H
#define HD_INC_CORE_RANDOM_ACCESS_ITERATOR_H

namespace hud {

    /**
    * Forward declaration of random access iterator on container
    * Specialize for differente random accessible container types like array, c style array etc...
    */
    template<typename type_t>
    class random_access_iterator;

    /**
    * Random access iterator on a continuous allocated elements of type type_t.
    * @tparam type_t The type of object in the C array
    * @tparam size The size of the C array
    */
    template<typename type_t>
    class random_access_iterator<type_t*> {

    public:
        /**
        * Construct a random_access_iterator on the elements, starting at the given index.
        * @param elements Pointer to the continuous allocated elements.
        * @param index Index of the element to reach after the elements in the sequence
        */
        constexpr random_access_iterator(type_t* elements, const usize index = 0) noexcept
            : pointer(elements + index) {
        }

        /** Pre-increment the iterator. */
        constexpr random_access_iterator& operator++() noexcept {
            pointer++;
            return *this;
        }

        /** Post-increment the iterator. */
        constexpr random_access_iterator operator++(i32) noexcept {
            random_access_iterator tmp(*this);
            pointer++;
            return tmp;
        }

        /** Pre-decrement the iterator. */
        constexpr random_access_iterator& operator--() noexcept {
            pointer--;
            return *this;
        }

        /** Post-decrement the iterator. */
        constexpr random_access_iterator operator--(i32) noexcept {
            random_access_iterator tmp(*this);
            pointer--;
            return tmp;
        }

        /** Move the iterator by incrementing diff index. */
        constexpr random_access_iterator& operator+=(const usize diff) noexcept {
            pointer += diff;
            return *this;
        }

        /** Retrieves a random_access_iterator that move this by incrementing diff index. */
        [[nodiscard]]
        constexpr random_access_iterator operator+(const usize diff) const noexcept {
            return random_access_iterator(pointer + diff);
        }

        /** Move the iterator by decrementing diff index. */
        constexpr random_access_iterator& operator-=(const usize diff) noexcept {
            pointer -= diff;
            return *this;
        }

        /** Retrieves a random_access_iterator that move this by decrementing diff index. */
        [[nodiscard]]
        constexpr random_access_iterator operator-(const usize diff) const noexcept {
            return random_access_iterator(pointer - diff);
        }

        /** Dereference the iterator. */
        [[nodiscard]]
        constexpr type_t& operator*() const noexcept {
            return *pointer;
        }

        /** Dereference the iterator. */
        [[nodiscard]]
        constexpr type_t* operator->() const noexcept {
            return pointer;
        }

        /** Retrieves the difference with another random_access_iterator. */
        [[nodiscard]]
        constexpr isize operator-(const random_access_iterator& other) noexcept {
            return pointer - other.pointer;
        }

        /** Checks if 2 random_access_iterator are equals. */
        [[nodiscard]]
        constexpr bool operator==(const random_access_iterator& other) const noexcept {
            return pointer == other.pointer;
        }

        /** Checks if 2 random_access_iterator are not equals. */
        [[nodiscard]]
        constexpr bool operator!=(const random_access_iterator& other) const noexcept {
            return pointer != other.pointer;
        }

    private:
        /** Reference to the container to iterate */
        type_t* pointer;
    };

    /**
    * Random access  iterator on a C array of object of type TType
    * @tparam type_t The type of object in the C array
    * @tparam size The size of the C array
    */
    template<typename type_t, usize size>
    class random_access_iterator<type_t[size]> {

    public:
        /**
        * Construct a random_access_iterator on the elements, starting at the given index.
        * @param elements Pointer to the continuous allocated elements.
        * @param index Index of the element to reach after the elements in the sequence
        */
        constexpr random_access_iterator(type_t* elements, const usize index = 0) noexcept
            : pointer(elements + index) {
        }

        /** Pre-increment the iterator. */
        constexpr random_access_iterator& operator++() noexcept {
            pointer++;
            return *this;
        }

        /** Post-increment the iterator. */
        constexpr random_access_iterator operator++(i32) noexcept {
            random_access_iterator tmp(*this);
            pointer++;
            return tmp;
        }

        /** Pre-decrement the iterator. */
        constexpr random_access_iterator& operator--() noexcept {
            pointer--;
            return *this;
        }

        /** Post-decrement the iterator. */
        constexpr random_access_iterator operator--(i32) noexcept {
            random_access_iterator tmp(*this);
            pointer--;
            return tmp;
        }

        /** Move the iterator by incrementing diff index. */
        constexpr random_access_iterator& operator+=(const usize diff) noexcept {
            pointer += diff;
            return *this;
        }

        /** Retrieves a random_access_iterator that move this by incrementing diff index. */
        [[nodiscard]]
        constexpr random_access_iterator operator+(const usize diff) const noexcept {
            return random_access_iterator(pointer + diff);
        }

        /** Move the iterator by decrementing diff index. */
        constexpr random_access_iterator& operator-=(const usize diff) noexcept {
            pointer -= diff;
            return *this;
        }

        /** Retrieves a random_access_iterator that move this by decrementing diff index. */
        [[nodiscard]]
        constexpr random_access_iterator operator-(const usize diff) const noexcept {
            return random_access_iterator(pointer - diff);
        }

        /** Dereference the iterator. */
        [[nodiscard]]
        constexpr type_t& operator*() const noexcept {
            return *pointer;
        }

        /** Dereference the iterator. */
        [[nodiscard]]
        constexpr type_t* operator->() const noexcept {
            return pointer;
        }

        /** Retrieves the difference with another random_access_iterator. */
        [[nodiscard]]
        constexpr isize operator-(const random_access_iterator& other) noexcept {
            return pointer - other.pointer;
        }

        /** Checks if 2 random_access_iterator are equals. */
        [[nodiscard]]
        constexpr bool operator==(const random_access_iterator& other) const noexcept {
            return pointer == other.pointer;
        }

        /** Checks if 2 random_access_iterator are not equals. */
        [[nodiscard]]
        constexpr bool operator!=(const random_access_iterator& other) const noexcept {
            return pointer != other.pointer;
        }

    private:
        /** Reference to the container to iterate */
        type_t* pointer;
    };

}  // namespace hud

#endif // HD_INC_CORE_RANDOM_ACCESS_ITERATOR_H
