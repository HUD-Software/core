#pragma once
#ifndef HD_INC_OSLAYER_ARRAY_H
#define HD_INC_OSLAYER_ARRAY_H
#include "../Minimal.h"
#include "../Allocators/AlignedHeapAllocator.h"
#include "../Traits/IsConstructible.h"
#include "../Memory.h"
#include "../Traits/IsCopyConstructible.h"
#include "../Traits/IsTriviallyDestructible.h"
#include "../Traits/IsMoveConstructible.h"
#include "../Traits/IsBitwiseMoveConstructible.h"
#include "../Traits/IsCopyAssignable.h"
#include "../Traits/IsBitwiseMoveAssignable.h"
#include "../Traits/IsMoveAssignable.h"
#include "../Traits/IsTriviallySwappable.h"
#include "../Traits/IsNothrowSwappable.h"
#include "../Traits/IsSame.h"
#include "../Traits/IsUnsigned.h"
#include "../Traits/IsConstantEvaluated.h"
#include "../Templates/Swap.h"
#include "../Slice.h"
#include "../Assert.h"
#include "../Templates/Forward.h"
#include "../Templates/Move.h"
#include "../Templates/LexicographicalCompare.h"
#include "../Iterators/RandomAccessIterator.h"
#include <initializer_list>

namespace hud {

    /**
    * Array is a fast and memory efficient sequence of elements of the same type.
    * Array is dynamically resizable and is responsible for the ownership of all elements it contains.
    * Elements are in a contiguous in memory in a well-define order.
    * @tparam T The element type
    * @tparam Allocator The allocator to use.
    */
    template<typename T, typename Allocator = AlignedHeapAllocator<alignof(T)>>
    class Array : private Allocator {

    public:
        /** The type of the allocator used. */
        using AllocatorType = Allocator;

        /** The type of allocation done by the allocator. */
        using AllocationType = typename AllocatorType::template AllocationType<T>;

        /** Mutable Array iterator type. */
        using Iterator = RandomAccessIterator<T*>;

        /** Constant Array iterator type. */
        using ConstIterator = RandomAccessIterator<const T*>;

        /**  Default constructor. */
        explicit constexpr Array() noexcept = default;

        /**
        * Copy construct from a raw buffer of continuous elements of type U.
        * @tparam U the element type of the raw data to copy.
        * @param first The pointer to the first element of the contiguous sequence of elements of type U
        * @param element_number The number of elements in the sequence to copy
        * @param allocator (Optional) The allocator instance to use. Copy the allocator.
        */
        template<typename U>
        constexpr explicit Array(const U* first, const usize element_number, const Allocator& allocator = Allocator()) noexcept requires(IsCopyConstructibleV<T, U>)
            : Allocator(allocator)
            , allocation(Allocator::template allocate<T>(element_number))
            , end_ptr(data_at(element_number)) {
            Memory::copy_construct_array(data(), first, element_number);
        }

        /**
        * Copy construct from a raw buffer of continuous elements of type U. Can optinally allocate more memory than needed.
        * @tparam U the element type of the raw data to copy.
        * @param first The pointer to the first element of the contiguous sequence of elements of type U
        * @param element_number The number of elements in the sequence to copy
        * @param extra_element_count (Optional) Number of extra element memory to allocate. Extra allocation is not construct
        * @param allocator (Optional) The allocator instance to use. Copy the allocator.
        */
        template<typename U>
        constexpr explicit Array(const U* first, const usize element_number, const usize extra_element_count, const Allocator& allocator = Allocator()) noexcept requires(IsCopyConstructibleV<T, U>)
            : Allocator(allocator)
            , allocation(Allocator::template allocate<T>(element_number + extra_element_count))
            , end_ptr(data_at(element_number)) {
            Memory::copy_construct_array(data(), first, element_number);
        }


        /**
        * Copy construct from a std::initializer_list of T. Can optinally allocate more memory than needed.
        * @tparam U The element type of the std::intializer_list
        * @param list The std::initializer_list of U
        * @param allocator (Optional) The allocator instance to use. Copy the allocator.
        */
        template<typename U>
        constexpr Array(std::initializer_list<U> list, const Allocator& allocator = Allocator()) noexcept requires(IsCopyConstructibleV<T, U>)
            : Allocator(allocator)
            , allocation(Allocator::template allocate<T>(list.size()))
            , end_ptr(data_at(list.size())) {
            // Be sure that the implementation is std::intilizer_list is a continuous array of memory
            // else it will fail at compile time if begin() is not a pointer
            Memory::copy_construct_array(data(), list.begin(), count());
        }

        /**
        * Copy construct from a std::initializer_list of T. Can optinally allocate more memory than needed.
        * @tparam U The element type of the std::intializer_list
        * @param list The std::initializer_list of U
        * @param extra_element_count Number of extra element memory to allocate. Extra allocation is not construct
        * @param allocator (Optional) The allocator instance to use. Copy the allocator.
        */
        template<typename U>
        constexpr Array(std::initializer_list<U> list, const usize extra_element_count, const Allocator& allocator = Allocator()) noexcept requires(IsCopyConstructibleV<T, U>)
            : Allocator(allocator)
            , allocation(Allocator::template allocate<T>(list.size() + extra_element_count))
            , end_ptr(data_at(list.size())) {
            // Be sure that the implementation is std::intilizer_list is a continuous array of memory
            // else it will fail at compile time if begin() is not a pointer
            Memory::copy_construct_array(data(), list.begin(), count());
        }

        /**
        * Copy construct from another Array.
        * @param other The other Array to copy
        * @param allocator (Optional) The allocator instance to use. Copy the allocator.
        */
        constexpr explicit Array(const Array& other, const Allocator& allocator = Allocator()) noexcept requires(IsCopyConstructibleV<T>)
            : Allocator(allocator)
            , allocation(Allocator::template allocate<T>(other.max_count()))
            , end_ptr(data_at(other.count())) {
            Memory::copy_construct_array(data(), other.data(), count());
        }

        /**
        * Copy construct from another Array.
        * @param other The other Array to copy
        * @param extra_element_count Number of extra element memory to allocate. Extra allocation is not construct
        * @param allocator (Optional) The allocator instance to use. Copy the allocator.
        */
        constexpr explicit Array(const Array& other, const usize extra_element_count, const Allocator& allocator = Allocator()) noexcept requires(IsCopyConstructibleV<T>)
            : Allocator(allocator)
            , allocation(Allocator::template allocate<T>(other.max_count() + extra_element_count))
            , end_ptr(data_at(other.count())) {
            Memory::copy_construct_array(data(), other.data(), count());
        }

        /**
        * Copy construct from another Array<U, Allocator_U>.
        * @tparam U The element type of the other Array
        * @tparam Allocator_U The alloctor type of the other Array
        * @param other The other Array to copy
        * @param allocator (Optional) The allocator instance to use. Copy the allocator.
        */
        template<typename U, typename Allocator_U>
        constexpr explicit Array(const Array<U, Allocator_U>& other, const Allocator& allocator = Allocator()) noexcept requires(IsCopyConstructibleV<T, U>)
            : Allocator(allocator)
            , allocation(Allocator::template allocate<T>(other.max_count()))
            , end_ptr(data_at(other.count())) {
            Memory::copy_construct_array(data(), other.data(), count());
        }

        /**
        * Copy construct from another Array<U, Allocator_U>.
        * @tparam U The element type of the other Array
        * @tparam Allocator_U The alloctor type of the other Array
        * @param other The other Array to copy
        * @param extra_element_count Number of extra element memory to allocate. Extra allocation is not construct
        * @param allocator (Optional) The allocator instance to use. Copy the allocator.
        */
        template<typename U, typename Allocator_U>
        constexpr explicit Array(const Array<U, Allocator_U>& other, const usize extra_element_count, const Allocator& allocator = Allocator()) noexcept requires(IsCopyConstructibleV<T, U>)
            : Allocator(allocator)
            , allocation(Allocator::template allocate<T>(other.max_count() + extra_element_count))
            , end_ptr(data_at(other.count())) {
            Memory::copy_construct_array(data(), other.data(), count());
        }

        /**
        * Move construct from another Array with the same allocator.
        * If T is bitwise moveable, the allocator is moved and the internal allocation is just stoled.
        * @param other The other Array to move
        */
        constexpr explicit Array(Array&& other) noexcept requires(IsBitwiseMoveConstructibleV<T>)
            : Allocator(hud::move(*static_cast<Allocator*>(&other)))
            , allocation(hud::move(other.allocation))
            , end_ptr(other.end_ptr) {
            other.leak();
        }

        /**
        * Move construct from another Array<U> with the same allocator.
        * If U is bitwise moveable to T, the allocator is moved and the internal allocation is just stoled only if non constant evaluated.
        * Tecnically this will requires a reinterpret_cast but this type of cast is not allowed in constant evaluated.
        * In a constant evaluated expression we move the allocator, allocate a new allocation and move or copy all elements, then the moved array is freed.
        * @tparam U The element type of the other Array
        * @param other The other Array to move
        */
        template<typename U>
        constexpr explicit Array(Array<U, Allocator>&& other) noexcept requires(IsBitwiseMoveConstructibleV<T, U>)
            : Allocator(hud::move(*static_cast<Allocator*>(&other))) {

            // We moving an array of bitwise moveable constructible type where T != U We can't use reinterpret_cast to still the pointer 
            // in constant evaluation. So we allocate a new allocation, move elements then free the moved allocation.
            if (is_constant_evaluated()) {
                allocation = Allocator::template allocate<T>(other.max_count());
                end_ptr = data_at(other.count());
                Memory::move_or_copy_construct_array(data(), other.data(), count());
                other.free_to_null();
            }
            else {
                allocation = AllocationType(reinterpret_cast<T*>(other.data()), other.max_count());
                end_ptr = data_at(other.count());
                other.leak();
            }
        }

        /**
        * Move construct from another Array<U, Allocator_U>.
        * If U is bitwise moveable to T, the allocator is just moved, else the T's move constructor is called for each element.
        * @tparam U The element type of the other Array
        * @tparam Allocator_U The alloctor type of the other Array
        * @param other The other Array to move
        * @param allocator (Optional) The allocator instance to use. Copy the allocator.
        */
        template<typename U, typename Allocator_U>
        constexpr explicit Array(Array<U, Allocator_U>&& other, const Allocator& allocator = Allocator()) noexcept
            : Allocator(allocator)
            , allocation(Allocator::template allocate<T>(other.max_count()))
            , end_ptr(data_at(other.count())) {
            // If we have different type of allocator and the type is bitwise move constructible it faster to copy instead of moving 
            // This optimisation works only if allocator do not share the same memory allocation, this case is not used in the engine
            if (!is_constant_evaluated() && IsBitwiseCopyConstructibleV<T, U> && IsBitwiseMoveConstructibleV<T, U>) {
                Memory::copy(data(), other.data(), byte_count());
            }
            else {
                Memory::move_or_copy_construct_array(data(), other.data(), count());
            }
            other.free_to_null();
        }

        /**
        * Move construct from another Array<U, Allocator_U>.
        * If U is bitwise moveable to T, the allocator is just moved, else it call the T's move constructor is called for each element.
        * @tparam U The element type of the other Array
        * @tparam Allocator_U The alloctor type of the other Array
        * @param other The other Array to move
        * @param extra_element_count Number of extra element memory to allocate. Extra allocation is not construct
        * @param allocator (Optional) The allocator instance to use. Copy the allocator.
        */
        template<typename U, typename Allocator_U>
        constexpr explicit Array(Array<U, Allocator_U>&& other, const usize extra_element_count, const Allocator& allocator = Allocator()) noexcept
            : Allocator(allocator)
            , allocation(Allocator::template allocate<T>(other.max_count() + extra_element_count))
            , end_ptr(data_at(other.count())) {
            // If we have different type of allocator and the type is bitwise move constructible it faster to copy instead of moving 
            // This optimisation works only if allocator do not share the same memory buffer, this case is not used in the engine
            if (!is_constant_evaluated() && IsBitwiseCopyConstructibleV<T, U> && IsBitwiseMoveConstructibleV<T, U>) {
                Memory::copy(data(), other.data(), byte_count());
            }
            else {
                Memory::move_or_copy_construct_array(data(), other.data(), count());
            }
            other.free_to_null();
        }

        /**
        * Destructor.
        * Call all the destructor of each element in the array if not trivially destructible, then free the allocated memory.
        */
        constexpr ~Array() noexcept {
            Memory::destroy_array(allocation.data(), count());
            Allocator::template free<T>(allocation);
        }

        /**
        * Copy assign another Array.
        * @tparam U The element type of the std::intializer_list
        * @param list The std::initilizer_list to assign
        * @return *this
        */
        template<typename U>
        constexpr Array& operator=(std::initializer_list<U> list) noexcept {
            // Be sure that the implementation is std::intilizer_list is a continuous array of memory
            // else it will fail at compile time if begin() is not a pointer
            copy_assign(list.begin(), list.size());
            return *this;
        }

        /**
        * Copy assign another Array.
        * The copy assignement only grow allocation and never shrink allocation.
        * No new allocation is done if the Array contains enough memory to copy all elements, in other words we don't copy the capacity of the copied Array.
        * @param other The other Array to copy
        * @return *this
        */
        constexpr Array& operator=(const Array& other) noexcept requires(IsCopyAssignableV<T>) {
            if (this != &other) {
                copy_assign(other.data(), other.count());
            }
            return *this;
        }

        /**
        * Copy assign another Array.
        * The copy assignement only grow allocation and never shrink allocation.
        * No new allocation is done if the Array contains enough memory to copy all elements, in other words we don't copy the capacity of the copied Array.
        * @tparam U The element type of the other Array
        * @tparam Allocator_U The alloctor type of the other Array
        * @param other The other Array to copy
        * @return *this
        */
        template<typename U, typename Allocator_U>
        constexpr Array& operator=(const Array<U, Allocator_U>& other) noexcept requires(IsCopyAssignableV<T, U>) {
            copy_assign(other.data(), other.count());
            return *this;
        }

        /**
        * Move assign another Array<U> with the same allocator.
        * Never assume that the move assignement will keep the capacity of the moved array.
        * Depending of the Type and the allocator the move operation can reallocate or not, this is by design and allow some move optimisation
        * @param other The other Array to move
        */
        constexpr Array& operator=(Array&& other) noexcept requires(IsMoveAssignableV<T>) {
            if (this != &other) {
                move_assign(hud::move(other));
            }
            return *this;
        }

        /**
        * Move assign another Array<U> with the same allocator.
        * Never assume that the move assignement will keep the capacity of the moved array.
        * Depending of the Type and the allocator the move operation can reallocate or not, this is by design and allow some move optimisation
        * If U is bitwise moveable to T, the allocator is just moved, else it call the T's move constructor is called for each element.
        * @tparam U The element type of the other Array
        * @tparam Allocator_U The alloctor type of the other Array
        * @param other The other Array to move
        */
        template<typename U, typename Allocator_U>
        constexpr Array& operator=(Array<U, Allocator_U>&& other) noexcept requires(IsMoveAssignableV<T, U>) {
            move_assign(hud::forward<Array<U, Allocator_U>>(other));
            return *this;
        }

        /**
        * Appends a new element to the end of the container by calling the constructor in-place.
        * If the new count() is greater than max_count() then all iterators and references are invalidated.
        * @tparam TArgs The T constructor arguments
        * @param args Arguments to forward to the constructor of the element
        * @return Index of the newly created element
        */
        template<typename... TArgs>
        constexpr usize emplace_back(TArgs&&... args) noexcept requires(IsConstructibleV<T, TArgs...>) {
            const usize old_count = count();
            const usize new_count = count() + 1u;

            // If we don't have enough place in allocated memory we need to reallocate.
            if (new_count > max_count()) {
                AllocationType new_allocation = Allocator::template allocate<T>(new_count);
                // Construct the element in-place
                Memory::construct_at(new_allocation.data_at(old_count), hud::forward<TArgs>(args)...);
                // Relocate the element that are before the newly added element
                Memory::fast_move_or_copy_construct_array_then_destroy(new_allocation.data(), allocation.data(), old_count);
                // Free the allocation and replace it with the newly created
                free_allocation_and_replace_it(hud::move(new_allocation), new_count);
            }
            else {
                Memory::construct_at(allocation.data_at(old_count), hud::forward<TArgs>(args)...);
                end_ptr = allocation.data_at(new_count);
            }

            return old_count;
        }

        /**
        * Appends a new element to the end of the container by calling the constructor in-place.
        * If the new count() is greater than max_count() then all iterators and references are invalidated.
        * @tparam TArgs The T constructor arguments
        * @param args Arguments to forward to the constructor of the element
        * @return Reference to the newly created element
        */
        template<typename... TArgs>
        [[nodiscard]]
        constexpr T& emplace_back_to_ref(TArgs&&... args) noexcept requires(IsConstructibleV<T, TArgs...>) {
            const usize element_index = emplace_back(hud::forward<TArgs>(args)...);
            return *data_at(element_index);
        }

        /**
        * Emplace a new element at the given index of the array by calling the constructor in-place.
        * If the new count() is greater than max_count() then all iterators and references are invalidated, else only the iterator after the given index are invalidated.
        * @tparam TArgs The T constructor arguments
        * @param idx The index of insertion
        * @param args Arguments to forward to the constructor of the element
        */
        template<typename... TArgs>
        void emplace_at(const usize idx, TArgs&&... args) noexcept {
            check(idx <= count());
            const usize old_count = count();
            const usize new_count = count() + 1u;


            // if we don't have enough place in allocated memory we need to reallocate.
            if (new_count > max_count()) {
                AllocationType new_allocation = Allocator::template allocate<T>(new_count);
                // Construct the element in-place
                Memory::construct_at(new_allocation.data_at(idx), hud::forward<TArgs>(args)...);
                // Relocate others before the emplaced element
                Memory::fast_move_or_copy_construct_array_then_destroy(new_allocation.data(), data(), idx);
                // Relocate others after the emplaced element
                Memory::fast_move_or_copy_construct_array_then_destroy(new_allocation.data_at(idx + 1), allocation.data_at(idx), old_count - idx);
                // Free the allocation and replace it with the newly created
                free_allocation_and_replace_it(hud::move(new_allocation), new_count);
            }
            else {
                // Relocate others after the emplaced element
                T* emplace_ptr = allocation.data_at(idx);
                Memory::move_or_copy_construct_array_then_destroy_backward(emplace_ptr + 1, emplace_ptr, static_cast<usize>(end_ptr - emplace_ptr));
                // Construct the element in-place
                Memory::construct_at(emplace_ptr, hud::forward<TArgs>(args)...);
                end_ptr++;
            }

        }

        /**
        * Emplace a new element at the given index of the array by calling the constructor in-place.
        * If the new count() is greater than max_count() then all iterators and references are invalidated, else only the iterator after the given index are invalidated.
        * @tparam TArgs The T constructor arguments
        * @param index The index of insertion
        * @param args Arguments to forward to the constructor of the element
        * @return Reference to the newly created element
        */
        template<typename... TArgs>
        [[nodiscard]]
        T& emplace_at_to_ref(const usize index, TArgs&&... args) noexcept {
            emplace_at(index, hud::forward<TArgs>(args)...);
            return *data_at(index);
        }

        /**
        * Append by copying a new element to the end of the container.
        * If the new count() is greater than max_count() then all iterators and references are invalidated.
        * @param element The element to copy to the end of the container
        * @return Index of the newly added element
        */
        constexpr usize add(const T& element) noexcept {
            return emplace_back(element);
        }

        /**
        * Append by moving a new element to the end of the container.
        * If the new count() is greater than max_count() then all iterators and references are invalidated.
        * @param element The element to copy to the end of the container
        * @return Index of the newly added element
        */
        constexpr usize add(T&& element) noexcept {
            return emplace_back(hud::move(element));
        }

        /**
        * Append by copying a new element to the end of the container.
        * If the new count() is greater than max_count() then all iterators and references are invalidated.
        * @param element The element to copy to the end of the container
        * @return Reference to the newly added element
        */
        [[nodiscard]]
        constexpr T& add_to_ref(const T& element) noexcept {
            return emplace_back_to_ref(element);
        }

        /**
        * Append by moving a new element to the end of the container.
        * If the new count() is greater than max_count() then all iterators and references are invalidated.
        * @param element The element to move to the end of the container
        * @return Reference to of the newly added element
        */
        [[nodiscard]]
        constexpr T& add_to_ref(T&& element) noexcept requires(IsMoveConstructibleV<T>) {
            return emplace_back_to_ref(hud::move(element));
        }

        /**
        * Add number of elements at the end of the array and grow the allocation if needed but do not call any constructor or setting memory to zero.
        * If the new count() is greater than max_count() then all iterators and references are invalidated.
        * Use this with caution, elements constructor are not called but destructor will be called on elements that do not call their constructor, this can lead to UB or crash.
        * @param element_number Number of element to add
        * @param Index of the first element added
        */
        constexpr usize add_no_construct(const usize element_number) noexcept {
            const usize old_count = count();
            const usize new_count = count() + element_number;

            reserve(new_count);
            end_ptr = data_at(new_count);
            return old_count;
        }

        /**
        * Resize the container allocation to fit the number of elements.
        * If no elements is contained in the array then allocation is free, else allocation is shrink to fit the number of elements contains in the Array.
        */
        void shrink_to_fit() noexcept {
            if (is_empty()) {
                free_to_null();
            }
            else if (end_ptr < allocation.data_end()) {
                AllocationType new_allocation = Allocator::template allocate<T>(count());
                Memory::fast_move_or_copy_construct_array_then_destroy(new_allocation.data(), allocation.data(), count());
                free_allocation_and_replace_it(hud::move(new_allocation), count());
            }
        }


        /**
        * Remove number of elements in the array from a given index.
        * @param index The index to remove
        * @param count_to_remove (Optional) Count of elements to remove. Default is 1.
        */
        void remove_at(const usize index, const usize count_to_remove = 1) noexcept {
            check(index < count());
            if (!is_empty()) {
                T* first_item_to_remove = data_at(index);
                check(count_to_remove <= count()); // Remove more elements than possible
                Memory::destroy_array(first_item_to_remove, count_to_remove);
                const usize remains = count() - count_to_remove;

                const usize count_to_reallocate_after = remains - index;
                if (count_to_reallocate_after > 0) {
                    const auto items_to_assign = first_item_to_remove + count_to_remove;
                    // relocate all elements left to keep element continuity
                    Memory::move_or_copy_assign_array(first_item_to_remove, items_to_assign, items_to_assign + count_to_reallocate_after);
                    Memory::destroy_array(first_item_to_remove + count_to_reallocate_after, index);
                }
                end_ptr = allocation.data_at(remains);
            }
        }

        /**
        * Remove number of elements in the array from a given index.
        * @param index The index to remove
        * @param count_to_remove (Optional) Count of elements to remove. Default is 1.
        */
        void remove_at_shrink(const usize index, const usize count_to_remove = 1) noexcept {
            check(index < count());
            if (count() > 0) {
                T* first_item_to_remove = data_at(index);
                check(count_to_remove <= count()); // Remove more elements than possible
                Memory::destroy_array(first_item_to_remove, count_to_remove);
                const usize remains = count() - count_to_remove;

                if (remains > 0) {
                    AllocationType new_allocation = Allocator::template allocate<T>(remains);
                    // Move or copy elements before the removed element then destroy moved or copied elements from the old allocation
                    Memory::fast_move_or_copy_construct_array_then_destroy(new_allocation.data(), allocation.data(), index);
                    // Move or copy elements after the removed element then destroy moved or copied elements from the old allocation
                    Memory::fast_move_or_copy_construct_array_then_destroy(new_allocation.data_at(index), allocation.data_at(index + count_to_remove), remains - index);
                    free_allocation_and_replace_it(hud::move(new_allocation), remains);
                }
                else {
                    free_to_null();
                }
            }
        }

        /**
        * Resize the array to ensure it contains a number of elements.
        * Call defaut constructor or set memory to zero depending if the type is trivially constructible or not.
        * @param element_number Number of element the array must contains
        */
        void resize(const usize element_number) noexcept {
            const isize diff = static_cast<const isize>(element_number - count());
            // If we grow the element count
            if (diff > 0) {
                const usize old_count = add_no_construct(static_cast<usize>(diff));
                T* first_added = data_at(old_count);
                Memory::default_construct_array(first_added, first_added + static_cast<usize>(diff));
            }
            else if (diff < 0) {
                remove_at(element_number, static_cast<usize>(-diff));
            }
        }

        /**
        * Reserve enough memory to ensure the Array can contains a number of elements without reallocating.
        * Only grow the allocation and relocate object if the number of elements requested is bigger than the current max number of elements.
        * Do nothing if the given element number is less or equal the maximum element count.
        * @param element_number Number of element th array must be able to contains in memory
        */
        constexpr void reserve(const usize element_number) noexcept {
            if (element_number > max_count()) {
                AllocationType new_allocation = Allocator::template allocate<T>(element_number);
                Memory::fast_move_or_copy_construct_array_then_destroy(new_allocation.data(), allocation.data(), count());
                free_allocation_and_replace_it(hud::move(new_allocation), count());
            }
        }

        /**
        * Remove all elements from the array.
        * Call all the destructor of each element in the array if not trivially destructible but do not free the allocated memory.
        */
        HD_FORCEINLINE constexpr void clear() noexcept {
            if (count() > 0) {
                Memory::destroy_array(allocation.data(), count());
                end_ptr = allocation.data();
            }
        }

        /**
        * Remove all elements from the array.
        * Call all the destructor of each element in the array if not trivially destructible, then free the allocated memory.
        */
        HD_FORCEINLINE void clear_shrink() noexcept {
            if (count() > 0) {
                Memory::destroy_array(allocation.data(), count());
                free_to_null();
            }
        }

        /** Retreives number of elements in the array. */
        [[nodiscard]]
        HD_FORCEINLINE constexpr usize count() const noexcept {
            return static_cast<usize>(end_ptr - allocation.data());
        }

        /** Retreives number of elements in the array in bytes. */
        [[nodiscard]]
        HD_FORCEINLINE constexpr usize byte_count() const noexcept {
            return count() * sizeof(T);
        }

        /** Retreives maximum number of elements the array can contains. */
        [[nodiscard]]
        HD_FORCEINLINE constexpr usize max_count() const noexcept {
            return allocation.count();
        }

        /** Retreives maximum number of elements the array can contains. */
        [[nodiscard]]
        HD_FORCEINLINE constexpr usize max_byte_count() const noexcept {
            return allocation.byte_count();
        }

        /** Retrieves the allocator. */
        [[nodiscard]]
        HD_FORCEINLINE constexpr const Allocator& allocator() const noexcept {
            return *static_cast<const Allocator*>(this);
        }

        /** Retrieves a constant pointer to the first element of the contiguous elements. */
        [[nodiscard]]
        HD_FORCEINLINE constexpr const T* data() const noexcept {
            return allocation.data();
        }

        /** Retrieves a pointer to the raw data first element of the contiguous elements. */
        [[nodiscard]]
        constexpr T* data() noexcept {
            return allocation.data();
        }

        /** Return the slack in number of elements. */
        [[nodiscard]]
        HD_FORCEINLINE constexpr usize slack() const noexcept {
            return static_cast<usize>(allocation.data_end() - end_ptr);
        }

        /**
        * Check whether index is in valid range or not.
        * @param index The index to check
        * @return true if the index is in valid range, false otherwise
        */
        [[nodiscard]]
        constexpr bool is_valid_index(const usize index) const noexcept {
            // When index is unsigned, we don't need to check for negative values
            static_assert(IsUnsignedV<decltype(index)>);
            return index < count();
        }

        /** Checks whether the Array is empty of not. */
        [[nodiscard]]
        HD_FORCEINLINE constexpr bool is_empty() const noexcept {
            return end_ptr == allocation.data();
        }

        /**
        * Retrieves reference on the element at the given index.
        * @param index The index of the element to retrieve
        * @return Reference on the element at the given index
        */
        [[nodiscard]]
        constexpr T& operator[](const usize index) noexcept {
            check(is_valid_index(index));
            return *data_at(index);
        }

        /**
        * Retrieves reference on the element at the given index.
        * @param index The index of the element to retrieve
        * @return Reference on the element at the given index
        */
        [[nodiscard]]
        HD_FORCEINLINE constexpr const T& operator[](const usize index) const noexcept {
            check(is_valid_index(index));
            return *data_at(index);
        }

        /**
        * Retrieves a sub-Slice of the Slice.
        * @param first_index The index of the first element in the Slice sequence
        * @param count The number of elements the Slice sequence must contains
        * @return The sub-Slice from data()+first_index over a sequence of count elements
        */
        [[nodiscard]]
        HD_FORCEINLINE constexpr const Slice<T> sub_slice(const usize first_index, const usize count) const noexcept {
            return allocation.sub_slice(first_index, count);
        }

        /** Retrieves reference on the first element. */
        [[nodiscard]]
        HD_FORCEINLINE constexpr const T& first() const noexcept {
            check(!is_empty());
            return data()[0];
        }

        /** Retrieves reference on the first element. */
        [[nodiscard]]
        HD_FORCEINLINE constexpr T& first() noexcept {
            check(!is_empty());
            return data()[0];
        }

        /**
        * Retrieves reference on the first element
        * @param from_the_start The index to access from the start of the Array
        * @return Reference on the first element
        */
        [[nodiscard]]
        HD_FORCEINLINE constexpr const T& first(usize from_the_start) const noexcept {
            check(is_valid_index(from_the_start));
            return data()[from_the_start];
        }

        /**
        * Retrieves reference on the first element
        * @param from_the_start The index to access from the start of the Array
        * @return Reference on the first element
        */
        [[nodiscard]]
        HD_FORCEINLINE constexpr T& first(usize from_the_start) noexcept {
            check(is_valid_index(from_the_start));
            return data()[from_the_start];
        }

        /** Retrieves reference on the last element. */
        [[nodiscard]]
        HD_FORCEINLINE constexpr const T& last() const noexcept {
            check(!is_empty());
            return data()[count() - 1];
        }

        /** Retrieves reference on the last element. */
        [[nodiscard]]
        HD_FORCEINLINE constexpr T& last() noexcept {
            check(!is_empty());
            return data()[count() - 1];
        }

        /**
        * Retrieves reference on the last element.
        * @param from_the_end The index to access from the end of the Array
        * @return Reference on the last element
        */
        [[nodiscard]]
        HD_FORCEINLINE constexpr const T& last(usize from_the_end) const noexcept {
            check(is_valid_index(count() - 1 - from_the_end));
            return data()[count() - 1 - from_the_end];
        }

        /**
        * Retrieves reference on the last element.
        * @param from_the_end The index to access from the end of the Array
        * @return Reference on the last element
        */
        [[nodiscard]]
        HD_FORCEINLINE constexpr T& last(usize from_the_end) noexcept {
            check(is_valid_index(count() - 1 - from_the_end));
            return data()[count() - 1 - from_the_end];
        }

        /**
        * Swap with another Array.
        * @param other The Array to swap with
        */
        void swap(Array& other) noexcept requires(IsSwappableV<T>) {
            static_assert(IsNothrowSwappableV<T>, "swap(Array<T>&) is throwable. Array is not designed to allow throwable swappable components");
            hud::swap(end_ptr, other.end_ptr);
            hud::swap(allocation, other.allocation);
            hud::swap(*static_cast<Allocator*>(this), *static_cast<Allocator*>(&other));
        }

        /**
        * Find the fist index of a given element where the predicate Array[index] == to_find is true.
        * Given comparand must be comparable with T operator==.
        * @param CompType The comparand type used to compare
        * @param to_find The element to find
        * @return first index of the element where the predicate Array[index] == element is true, index_none otherwise
        */
        template<typename CompType>
        [[nodiscard]]
        usize find_first_index(const CompType& to_find) const requires(IsComparableWithEqualV<T, CompType>) {
            const T* HD_RESTRICT begin = data();
            const T* HD_RESTRICT end = end_ptr;

            for (const T* HD_RESTRICT cur = begin; cur != end; cur++) {
                if (*cur == to_find) {
                    return static_cast<usize>(cur - begin);
                }
            }
            return index_none;
        }

        /**
        * Find the last index of an element where the predicate Array[index] == to_find is true.
        * Given comparand must be comparable with T operator==.
        * @param CompType The comparand type used to compare
        * @param to_find The element to find
        * @return last index of the element where the predicate Array[index] == element is true, index_none otherwise
        */
        template<typename CompType>
        [[nodiscard]]
        usize find_last_index(const CompType& to_find) const requires(IsComparableWithEqualV<T, CompType>) {
            const T* HD_RESTRICT begin = data();
            const T* HD_RESTRICT end = end_ptr;
            const T* HD_RESTRICT cur = end;
            while (cur != begin) {
                cur--;
                if (*cur == to_find) {
                    return static_cast<usize>(cur - begin);
                }
            }
            return index_none;
        }

        /**
        * Find the fist index of an element where the user-defined predicate return true.
        * @tparam UnaryPredicate The Unary predicate to use
        * @param predicate The predicate to use
        * @return first index of the element matching the predicate, index_none otherwise
        */
        template<typename UnaryPredicate>
        [[nodiscard]]
        usize find_first_index_by_predicate(const UnaryPredicate predicate) const {
            const T* HD_RESTRICT begin = data();
            const T* HD_RESTRICT end = end_ptr;

            for (const T* HD_RESTRICT cur = begin; cur != end; cur++) {
                if (predicate(*cur)) {
                    return static_cast<usize>(cur - begin);
                }
            }
            return index_none;
        }

        /**
        * Find the last index of a given element where the user-defined predicate return true.
        * @tparam UnaryPredicate The Unary predicate to use
        * @param predicate The predicate to use
        * @return last index of the element matching the predicate, index_none otherwise
        */
        template<typename UnaryPredicate>
        [[nodiscard]]
        usize find_last_index_by_predicate(const UnaryPredicate predicate) const {
            const T* HD_RESTRICT begin = data();
            const T* HD_RESTRICT end = end_ptr;
            const T* HD_RESTRICT cur = end;
            while (cur != begin) {
                cur--;
                if (predicate(*cur)) {
                    return static_cast<usize>(cur - begin);
                }
            }
            return index_none;
        }

        /**
        * Checks whether an element is contained in the Array or not.
        * @param CompType The comparand type used to compare
        * @param to_find The element to find
        * @return true if the element is contained in the Array, false otherwise
        */
        template<typename CompType>
        [[nodiscard]]
        bool contains(const CompType& to_find) const requires(IsComparableWithEqualV<T, CompType>) {
            return find_first_index(to_find) != index_none;
        }

        /**
        * Checks whether an element match the user-defined predicate is contained in the Array or not.
        * @tparam UnaryPredicate The Unary predicate to use
        * @param predicate The predicate to use
        * @return true if an element match the user-defined predicate is contained in the Array, false otherwise
        */
        template<typename UnaryPredicate>
        [[nodiscard]]
        bool contains_by_predicate(const UnaryPredicate predicate) const {
            return find_first_index_by_predicate(predicate) != index_none;
        }

        /** Retrieves an Iterator to the beginning of the Array. */
        [[nodiscard]]
        HD_FORCEINLINE constexpr Iterator begin() noexcept {
            return Iterator(data());
        }

        /** Retrieves an Iterator to the beginning of the Array. */
        [[nodiscard]]
        HD_FORCEINLINE constexpr ConstIterator begin() const noexcept {
            return ConstIterator(data());
        }

        /** Retrieves an Iterator to the end of the Array. */
        [[nodiscard]]
        HD_FORCEINLINE constexpr Iterator end() noexcept {
            return Iterator(end_ptr);
        }

        /** Retrieves an Iterator to the end of the Array. */
        [[nodiscard]]
        HD_FORCEINLINE constexpr ConstIterator end() const noexcept {
            return ConstIterator(end_ptr);
        }

    private:

        /**
        * Copy assign a source_count elements from the source of data to the array.
        * @tparam U The element type of the other Array
        * @param source The source of data to copy
        * @param source_count Element count in the source to copy
        */
        template<typename U>
        constexpr void copy_assign(const U* source, const usize source_count) noexcept {
            // Grow the allocation if we don't have enough room
            // If we need to reallocate, we destroy all elements before reallocating the allocation
            // Then we copy construct all elements of source in the allocation
            if (source_count > max_count()) {
                // Destroy existing
                Memory::destroy_array(data(), count());
                // Allocate a new allocation and copy construct source into it
                AllocationType new_allocation = Allocator::template allocate<T>(source_count);
                Memory::copy_construct_array(new_allocation.data(), source, source_count);
                // Save the newly allocated allocation
                free_allocation_and_replace_it(hud::move(new_allocation), source_count);
            }
            // If we don't need to reallocate
            else {
                if (is_constant_evaluated() || !IsBitwiseCopyAssignableV<T, U>) {
                    const isize extra_to_construct = static_cast<isize>(source_count - count());
                    // We assign all elements that are already in the allocation,
                    // Then we copy construct all remaining elements at the end of the assigned elements
                    if (extra_to_construct > 0) {
                        Memory::copy_assign_array(data(), source, count());
                        Memory::copy_construct_array(data_at(count()), source + count(), static_cast<usize>(extra_to_construct));
                    }
                    // If we assign less or equal count of elements than the current element count
                    // we copy assign all new elements of the source in the allocation, then we destroy the remaining elements
                    else {
                        Memory::copy_assign_array(data(), source, source_count);
                        Memory::destroy_array(data() + source_count, static_cast<usize>(-extra_to_construct));
                    }
                }
                else {
                    Memory::copy_assign_array(data(), source, source_count);
                }
                end_ptr = allocation.data_at(source_count);
            }

        }

        /**
       * Move assign another array.
       * If U is bitwise moveable to T and the allocator are the same, the internal allocation ownership is given to this by stealing the pointer
       * If U is not bitwise moveable to T or allocators are differents, it do not still the pointer and call the T's move constructor is called for each element.
       * @tparam U The element type of the other Array
       * @tparam Allocator_U The alloctor type of the other Array
       * @param other The other Array to move
       */
        template<typename U, typename Allocator_U>
        constexpr void move_assign(Array<U, Allocator_U>&& other) noexcept {
            if (!is_constant_evaluated() && IsSameV<Allocator, Allocator_U> && IsBitwiseMoveAssignableV<T, U>) {
                // Destroy existing elements
                Memory::destroy_array(data(), count());
                // Still the allocation and the allocator
                *static_cast<Allocator*>(this) = hud::move(*static_cast<Allocator*>(&other));
                free_allocation_and_replace_it(AllocationType(reinterpret_cast<T*>(other.allocation.data()), other.allocation.count()), other.count());
                other.leak();
            }
            else {
                // If we need to reallocate, we destroy all elements before reallocating the allocation
                // Then we copy construct all elements of source in the allocation
                if (other.count() > max_count()) {
                    // Delete existing elements
                    Memory::destroy_array(data(), count());
                    AllocationType new_allocation = Allocator::template allocate<T>(other.count());
                    Memory::move_or_copy_construct_array(new_allocation.data(), other.data(), other.count());
                    free_allocation_and_replace_it(hud::move(new_allocation), other.count());
                }
                // If we don't need to reallocate
                else {
                   
                    // We assign all elements that are already in the allocation,
                    // Then we copy construct all remaining elements at the end of the assigned elements
                    if (other.count() > count()) {
                        const auto to_assign_end = other.data() + count();
                        Memory::move_or_copy_assign_array(data(), other.data(), to_assign_end);
                        Memory::move_or_copy_construct_array(data() + count(), to_assign_end, other.count() - count());
                    }
                    // If we assign less or equal count of elements than the current element count
                    // we copy assign all new elements of the source in the allocation, then we destroy the remaining elements
                    else {
                        Memory::move_or_copy_assign_array(data(), other.data(), other.end_ptr);
                        Memory::destroy_array(data() + other.count(), count() - other.count());
                    }
                    end_ptr = allocation.data_at(other.count());
                }

                // Free the moved memory
                other.free_to_null();
            }
        }

        /**
        * Free the allocation and replace the allocation and count.
        * @param new_allocation The new allocation to move
        * @param new_count_of_element The new count of element to set
        */
        constexpr void free_allocation_and_replace_it(AllocationType&& new_allocation, const usize new_count_of_element) noexcept {
            Allocator::template free<T>(allocation);
            allocation = hud::move(new_allocation);
            end_ptr = allocation.data_at(new_count_of_element);
        }

        /** Free the allocation and set everything to default */
        constexpr void free_to_null() noexcept {
            Allocator::template free<T>(allocation);
            leak();
        }

        /** Set all internal pointers to nullptr without freeing the memory. Be sure of what you are doing... */
        constexpr void leak() noexcept {
            allocation.leak();
            end_ptr = nullptr;
        }

        /**
        * Retrieves a pointer to the elements at given index of the sequence.
        * A special case is allowed when data_at(count()) is called.
        * No assertion is done but the pointer is not valid to dereference.
        * This feature is usefull to get a pointer end to performs pointer aritmetics.
        */
        [[nodiscard]]
        constexpr T* data_at(const usize index) const noexcept {
            return allocation.data_at(index);
        }

    private:
        template<typename U, typename Allocator_U> friend class Array; // Friend with other Array of other types

    private:
        /** The allocation */
        AllocationType allocation;

        /** Pointer to the end of the element sequence. */
        T* end_ptr = nullptr;
    };

    /**
    * Swap first Array with the second Array.
    * Same as first.swap(second).
    * @tparam T The element type
    * @tparam Allocator The allocator type of both Array
    * @param first The first Array to swap
    * @param second The second Array to swap
    */
    template<typename T, typename Allocator>
    HD_FORCEINLINE void swap(Array<T, Allocator>& first, Array<T, Allocator>& second) noexcept requires(IsSwappableV<T>) {
        first.swap(second);
    }

    /**
    * Checks whether right and left Array are equals.
    * Array are equals if both contains same number of elements and all values are equals.
    * Value types must be comparable with operator==() if types are not bitwise comparable with equal operator.
    * @tparam Left Value type of the left Array
    * @tparam LeftAllocator The allocator type of the left Array
    * @tparam Right Value type of the right Array
    * @tparam RightAllocator The allocator type of the right Array
    * @param left The left Array to compare
    * @param right The right Array to compare
    * @param true if right and left Arrays are equals, false otherwise
    */
    template<typename Left, typename LeftAllocator, typename Right, typename RightAllocator>
    [[nodiscard]]
    HD_FORCEINLINE constexpr bool operator==(const Array<Left, LeftAllocator>& left, const Array<Right, RightAllocator>& right) noexcept {
        const usize left_count = left.count();
        return left_count == right.count() && Memory::equal_array(left.data(), right.data(), left_count);
    }

    /**
    * Checks whether right and left Array are not equals.
    * Array are equals if they do not contains the same number of elements or at least one element is not equal in both Array.
    * Value types must be comparable with operator!=() if types are not bitwise comparable with not equal operator.
    * @tparam Left Value type of the left Array
    * @tparam LeftAllocator The allocator type of the left Array
    * @tparam Right Value type of the right Array
    * @tparam RightAllocator The allocator type of the right Array
    * @param left The left Array to compare
    * @param right The right Array to compare
    * @param true if right and left Arrays are not equals, false otherwise
    */
    template<typename Left, typename LeftAllocator, typename Right, typename RightAllocator>
    [[nodiscard]]
    HD_FORCEINLINE constexpr bool operator!=(const Array<Left, LeftAllocator>& left, const Array<Right, RightAllocator>& right) noexcept {
        const usize left_count = left.count();
        return left_count != right.count() || Memory::not_equal_array(left.data(), right.data(), left_count);
    }

    /**
    * Checks whether right is less than left Array.
    * An right is less than left if :
    *     - right don't contains a value while left do, or,
    *     - both contains a value and right value is less than left value.
    * Value types must be comparable with operator<() if types are not bitwise comparable with less operator.
    * @tparam Left Value type of the left Array
    * @tparam LeftAllocator The allocator type of the left Array
    * @tparam Right Value type of the right Array
    * @tparam RightAllocator The allocator type of the right Array
    * @param left The left Array to compare
    * @param right The right Array to compare
    * @param true if right is less than left Array, false otherwise
    */
    template<typename Left, typename LeftAllocator, typename Right, typename RightAllocator>
    [[nodiscard]]
    HD_FORCEINLINE constexpr bool operator<(const Array<Left, LeftAllocator>& left, const Array<Right, RightAllocator>& right) noexcept {
        return lexicographical_compare(left.data(), left.data() + left.count(), right.data(), right.data() + right.count());
    }

    /**
    * Checks whether right is greater than left Array.
    * An right is greater than left if :
    *     - right don't contains a value while left do, or,
    *     - both contains a value and right value is greater than left value.
    * Value types must be comparable with operator<() if types are not bitwise comparable with less operator.
    * @tparam Left Value type of the left Array
    * @tparam LeftAllocator The allocator type of the left Array
    * @tparam Right Value type of the right Array
    * @tparam RightAllocator The allocator type of the right Array
    * @param left The left Array to compare
    * @param right The right Array to compare
    * @param true if right is greater than left Array, false otherwise
    */
    template<typename Left, typename LeftAllocator, typename Right, typename RightAllocator>
    [[nodiscard]]
    HD_FORCEINLINE constexpr bool operator>(const Array<Left, LeftAllocator>& left, const Array<Right, RightAllocator>& right) noexcept {
        return right < left;
    }

    /**
    * Checks whether right is less or equal left Array.
    * An right is less or equal left if :
    *     - right don't contains a value while left do, or,
    *     - both contains a value and right value is less or equal left value.
    * Value types must be comparable with operator<() if types are not bitwise comparable with less operator.
    * @tparam Left Value type of the left Array
    * @tparam LeftAllocator The allocator type of the left Array
    * @tparam Right Value type of the right Array
    * @tparam RightAllocator The allocator type of the right Array
    * @param left The left Array to compare
    * @param right The right Array to compare
    * @param true if right is less or equal left Array, false otherwise
    */
    template<typename Left, typename LeftAllocator, typename Right, typename RightAllocator>
    [[nodiscard]]
    HD_FORCEINLINE constexpr bool operator<=(const Array<Left, LeftAllocator>& left, const Array<Right, RightAllocator>& right) noexcept {
        return !(right < left);
    }

    /**
    * Checks whether right is greater or equal left Array.
    * An right is greater or equal left if :
    *     - right don't contains a value while left do, or,
    *     - both contains a value and right value is greater or equal left value.
    * Value types must be comparable with operator<() if types are not bitwise comparable with less operator.
    * @tparam Left Value type of the left Array
    * @tparam LeftAllocator The allocator type of the left Array
    * @tparam Right Value type of the right Array
    * @tparam RightAllocator The allocator type of the right Array
    * @param left The left Array to compare
    * @param right The right Array to compare
    * @param true if right is greater or equal left Array, false otherwise
    */
    template<typename Left, typename LeftAllocator, typename Right, typename RightAllocator>
    [[nodiscard]]
    HD_FORCEINLINE constexpr bool operator>=(const Array<Left, LeftAllocator>& left, const Array<Right, RightAllocator>& right) noexcept {
        return !(left < right);
    }

} // namespace hud

#endif // HD_INC_OSLAYER_ARRAY_H