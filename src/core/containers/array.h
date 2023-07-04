#ifndef HD_INC_CORE_ARRAY_H
#define HD_INC_CORE_ARRAY_H
#include "../minimal.h"
#include "../allocators/aligned_heap_allocator.h"
#include "../traits/is_constructible.h"
#include "../memory.h"
#include "../traits/is_copy_constructible.h"
#include "../traits/is_trivially_destructible.h"
#include "../traits/is_move_constructible.h"
#include "../traits/is_bitwise_move_constructible.h"
#include "../traits/is_copy_assignable.h"
#include "../traits/is_bitwise_move_assignable.h"
#include "../traits/is_move_assignable.h"
#include "../traits/is_trivially_swappable.h"
#include "../traits/is_nothrow_swappable.h"
#include "../traits/is_same.h"
#include "../traits/is_unsigned.h"
#include "../traits/is_constant_evaluated.h"
#include "../templates/swap.h"
#include "../slice.h"
#include "../assert.h"
#include "../templates/forward.h"
#include "../templates/move.h"
#include "../templates/lexicographical_compare.h"
#include "../iterators/random_access_iterator.h"
#include <initializer_list>

namespace hud
{

    /**
     * array is a fast and memory efficient sequence of elements of the same type.
     * array is dynamically resizable and is responsible for the ownership of all elements it contains.
     * Elements are in a contiguous in memory in a well-define order.
     * @tparam type_t The element type
     * @tparam allocator_t The allocator to use.
     */
    template<typename type_t, typename allocator_t = hud::aligned_heap_allocator<alignof(type_t)>>
    class array
        : private allocator_t
    {

    public:
        /** the type contained in the array. */
        using value_type = type_t;

        /** The type of the allocator used. */
        using allocator_type = allocator_t;

        /** The type of allocation done by the allocator. */
        using allocation_type = typename allocator_type::template allocation_type<type_t>;

        /** Mutable array iterator type. */
        using iterator = random_access_iterator<type_t *>;

        /** Constant array iterator type. */
        using const_iterator = random_access_iterator<const type_t *>;

        /**  Default constructor. */
        explicit constexpr array() noexcept = default;

        /**
         * Copy construct from a raw buffer of continuous elements of type u_type_t.
         * @tparam u_type_t the element type of the raw data to copy.
         * @param first The pointer to the first element of the contiguous sequence of elements of type u_type_t
         * @param element_number The number of elements in the sequence to copy
         * @param allocator (Optional) The allocator instance to use. Copy the allocator.
         */
        template<typename u_type_t>
        requires(hud::is_copy_constructible_v<type_t, u_type_t>)
        constexpr explicit array(const u_type_t *first, const usize element_number, const allocator_type &allocator = allocator_type()) noexcept
            : allocator_t(allocator)
            , allocation(allocator_type::template allocate<type_t>(element_number))
            , end_ptr(data_at(element_number))
        {
            hud::memory::copy_construct_array(data(), first, element_number);
        }

        /**
         * Copy construct from a raw buffer of continuous elements of type u_type_t. Can optinally allocate more memory than needed.
         * @tparam u_type_tU the element type of the raw data to copy.
         * @param first The pointer to the first element of the contiguous sequence of elements of type u_type_t
         * @param element_number The number of elements in the sequence to copy
         * @param extra_element_count (Optional) Number of extra element memory to allocate. Extra allocation is not construct
         * @param allocator (Optional) The allocator instance to use. Copy the allocator.
         */
        template<typename u_type_t>
        requires(hud::is_copy_constructible_v<type_t, u_type_t>)
        constexpr explicit array(const u_type_t *first, const usize element_number, const usize extra_element_count, const allocator_type &allocator = allocator_type()) noexcept
            : allocator_t(allocator)
            , allocation(allocator_type::template allocate<type_t>(element_number + extra_element_count))
            , end_ptr(data_at(element_number))
        {
            hud::memory::copy_construct_array(data(), first, element_number);
        }

        /**
         * Copy construct from a std::initializer_list of type_t. Can optinally allocate more memory than needed.
         * @tparam u_type_t The element type of the std::intializer_list
         * @param list The std::initializer_list of u_type_t
         * @param allocator (Optional) The allocator instance to use. Copy the allocator.
         */
        template<typename u_type_t>
        requires(hud::is_copy_constructible_v<type_t, u_type_t>)
        constexpr array(std::initializer_list<u_type_t> list, const allocator_type &allocator = allocator_type()) noexcept
            : allocator_t(allocator)
            , allocation(allocator_type::template allocate<type_t>(list.size()))
            , end_ptr(data_at(list.size()))
        {
            // Be sure that the implementation is std::intilizer_list is a continuous array of memory
            // else it will fail at compile time if begin() is not a pointer
            if (list.size() > 0u)
            {
                hud::memory::copy_construct_array(data(), list.begin(), list.size());
            }
        }

        /**
         * Copy construct from a std::initializer_list of type_t. Can optinally allocate more memory than needed.
         * @tparam u_type_t The element type of the std::intializer_list
         * @param list The std::initializer_list of u_type_t
         * @param extra_element_count Number of extra element memory to allocate. Extra allocation is not construct
         * @param allocator (Optional) The allocator instance to use. Copy the allocator.
         */
        template<typename u_type_t>
        requires(hud::is_copy_constructible_v<type_t, u_type_t>)
        constexpr array(std::initializer_list<u_type_t> list, const usize extra_element_count, const allocator_type &allocator = allocator_type()) noexcept
            : allocator_t(allocator)
            , allocation(allocator_type::template allocate<type_t>(list.size() + extra_element_count))
            , end_ptr(data_at(list.size()))
        {
            // Be sure that the implementation is std::intilizer_list is a continuous array of memory
            // else it will fail at compile time if begin() is not a pointer
            if (list.size() > 0u)
            {
                hud::memory::copy_construct_array(data(), list.begin(), list.size());
            }
        }

        /**
         * Copy construct from another array.
         * @param other The other array to copy
         * @param allocator (Optional) The allocator instance to use. Copy the allocator.
         */
        constexpr explicit array(const array &other, const allocator_type &allocator = allocator_type()) noexcept
        requires(hud::is_copy_constructible_v<type_t>)
            : allocator_t(allocator)
            , allocation(allocator_type::template allocate<type_t>(other.max_count()))
            , end_ptr(data_at(other.count()))
        {
            hud::memory::copy_construct_array(data(), other.data(), other.count());
        }

        /**
         * Copy construct from another array.
         * @param other The other array to copy
         * @param extra_element_count Number of extra element memory to allocate. Extra allocation is not construct
         * @param allocator (Optional) The allocator instance to use. Copy the allocator.
         */

        constexpr explicit array(const array &other, const usize extra_element_count, const allocator_type &allocator = allocator_type()) noexcept
        requires(hud::is_copy_constructible_v<type_t>)
            : allocator_t(allocator)
            , allocation(allocator_type::template allocate<type_t>(other.max_count() + extra_element_count))
            , end_ptr(data_at(other.count()))
        {
            hud::memory::copy_construct_array(data(), other.data(), count());
        }

        /**
         * Copy construct from another array<u_type_t, u_allocator_t>.
         * @tparam u_type_t The element type of the other array
         * @tparam u_allocator_t The alloctor type of the other array
         * @param other The other array to copy
         * @param allocator (Optional) The allocator instance to use. Copy the allocator.
         */
        template<typename u_type_t, typename u_allocator_t>
        requires(hud::is_copy_constructible_v<type_t, u_type_t>)
        constexpr explicit array(const array<u_type_t, u_allocator_t> &other, const allocator_type &allocator = allocator_type()) noexcept
            : allocator_t(allocator)
            , allocation(allocator_type::template allocate<type_t>(other.max_count()))
            , end_ptr(data_at(other.count()))
        {
            hud::memory::copy_construct_array(data(), other.data(), count());
        }

        /**
         * Copy construct from another array<u_type_t, u_allocator_t>.
         * @tparam u_type_t The element type of the other array
         * @tparam u_allocator_t The alloctor type of the other array
         * @param other The other array to copy
         * @param extra_element_count Number of extra element memory to allocate. Extra allocation is not construct
         * @param allocator (Optional) The allocator instance to use. Copy the allocator.
         */
        template<typename u_type_t, typename u_allocator_t>
        requires(hud::is_copy_constructible_v<type_t, u_type_t>)
        constexpr explicit array(const array<u_type_t, u_allocator_t> &other, const usize extra_element_count, const allocator_type &allocator = allocator_type()) noexcept
            : allocator_t(allocator)
            , allocation(allocator_type::template allocate<type_t>(other.max_count() + extra_element_count))
            , end_ptr(data_at(other.count()))
        {
            hud::memory::copy_construct_array(data(), other.data(), count());
        }

        /**
         * Move construct from another array with the same allocator.
         * If type_t is bitwise moveable, the allocator is moved and the internal allocation is just stoled.
         * @param other The other array to move
         */
        constexpr explicit array(array &&other) noexcept
        requires(hud::is_bitwise_move_constructible_v<type_t>)
            : allocator_t(hud::move(*static_cast<allocator_t *>(&other)))
            , allocation(hud::move(other.allocation))
            , end_ptr(other.end_ptr)
        {
            other.leak();
        }

        /**
         * Move construct from another array<u_type_t> with the same allocator.
         * If u_type_t is bitwise moveable to type_t, the allocator is moved and the internal allocation is just stoled only if non constant evaluated.
         * Tecnically this will requires a reinterpret_cast but this type of cast is not allowed in constant evaluated.
         * In a constant evaluated expression we move the allocator, allocate a new allocation and move or copy all elements, then the moved array is freed.
         * @tparam u_type_t The element type of the other array
         * @param other The other array to move
         */
        template<typename u_type_t>
        constexpr explicit array(array<u_type_t, allocator_t> &&other) noexcept
        requires(hud::is_bitwise_move_constructible_v<type_t, u_type_t>)
            : allocator_t(hud::move(*static_cast<allocator_t *>(&other)))
        {

            // We moving an array of bitwise moveable constructible type where type_t != u_type_t We can't use reinterpret_cast to still the pointer
            // in constant evaluation. So we allocate a new allocation, move elements then free the moved allocation.
            if (hud::is_constant_evaluated())
            // LCOV_EXCL_START
            {
                allocation = allocator_type::template allocate<type_t>(other.max_count());
                end_ptr = data_at(other.count());
                hud::memory::move_or_copy_construct_array(data(), other.data(), count());
                other.free_to_null();
            }
            // LCOV_EXCL_STOP
            else
            {
                allocation = allocation_type(reinterpret_cast<type_t *>(other.data()), other.max_count());
                end_ptr = data_at(other.count());
                other.leak();
            }
        }

        /**
         * Move construct from another array<u_type_t, u_allocator_t>.
         * If u_type_t is bitwise moveable to type_t, the allocator is just moved, else the type_t's move constructor is called for each element.
         * @tparam u_type_t The element type of the other array
         * @tparam u_allocator_t The alloctor type of the other array
         * @param other The other array to move
         * @param allocator (Optional) The allocator instance to use. Copy the allocator.
         */
        template<typename u_type_t, typename u_allocator_t>
        constexpr explicit array(array<u_type_t, u_allocator_t> &&other, const allocator_type &allocator = allocator_type()) noexcept
            : allocator_t(allocator)
            , allocation(allocator_type::template allocate<type_t>(other.max_count()))
            , end_ptr(data_at(other.count()))
        {
            // If we have different type of allocator and the type is bitwise move constructible it faster to copy instead of moving
            // This optimisation works only if allocator do not share the same memory allocation, this case is not used in the engine
            if (!hud::is_constant_evaluated() && hud::is_bitwise_copy_constructible_v<type_t, u_type_t> && hud::is_bitwise_move_constructible_v<type_t, u_type_t>)
            {
                hud::memory::copy(data(), other.data(), byte_count());
            }
            else
            {
                hud::memory::move_or_copy_construct_array(data(), other.data(), count());
            }
            other.free_to_null();
        }

        /**
         * Move construct from another array<u_type_t, u_allocator_t>.
         * If u_type_t is bitwise moveable to type_t, the allocator is just moved, else it call the type_t's move constructor is called for each element.
         * @tparam u_type_t The element type of the other array
         * @tparam u_allocator_t The alloctor type of the other array
         * @param other The other array to move
         * @param extra_element_count Number of extra element memory to allocate. Extra allocation is not construct
         * @param allocator (Optional) The allocator instance to use. Copy the allocator.
         */
        template<typename u_type_t, typename u_allocator_t>
        constexpr explicit array(array<u_type_t, u_allocator_t> &&other, const usize extra_element_count, const allocator_type &allocator = allocator_type()) noexcept
            : allocator_t(allocator)
            , allocation(allocator_type::template allocate<type_t>(other.max_count() + extra_element_count))
            , end_ptr(data_at(other.count()))
        {
            // If we have different type of allocator and the type is bitwise move constructible it faster to copy instead of moving
            // This optimisation works only if allocator do not share the same memory buffer, this case is not used in the engine
            if (!hud::is_constant_evaluated() && hud::is_bitwise_copy_constructible_v<type_t, u_type_t> && hud::is_bitwise_move_constructible_v<type_t, u_type_t>)
            {
                hud::memory::copy(data(), other.data(), byte_count());
            }
            else
            {
                hud::memory::move_or_copy_construct_array(data(), other.data(), count());
            }
            other.free_to_null();
        }

        /**
         * Destructor.
         * Call all the destructor of each element in the array if not trivially destructible, then free the allocated memory.
         */
        constexpr ~array() noexcept
        {
            hud::memory::destroy_array(allocation.data(), count());
            allocator_type::template free<type_t>(allocation);
        }

        /**
         * Copy assign another array.
         * @tparam u_type_t The element type of the std::intializer_list
         * @param list The std::initilizer_list to assign
         * @return *this
         */
        template<typename u_type_t>
        constexpr array &operator=(std::initializer_list<u_type_t> list) noexcept
        {
            // Be sure that the implementation is std::intilizer_list is a continuous array of memory
            // else it will fail at compile time if begin() is not a pointer
            copy_assign(list.begin(), list.size());
            return *this;
        }

        /**
         * Copy assign another array.
         * The copy assignement only grow allocation and never shrink allocation.
         * No new allocation is done if the array contains enough memory to copy all elements, in other words we don't copy the capacity of the copied array.
         * @param other The other array to copy
         * @return *this
         */
        constexpr array &operator=(const array &other) noexcept
        requires(hud::is_copy_assignable_v<type_t>)
        {
            if (this != &other) [[likely]]
            {
                copy_assign(other.data(), other.count());
            }
            return *this;
        }

        /**
         * Copy assign another array.
         * The copy assignement only grow allocation and never shrink allocation.
         * No new allocation is done if the array contains enough memory to copy all elements, in other words we don't copy the capacity of the copied array.
         * @tparam u_type_t The element type of the other array
         * @tparam u_allocator_t The alloctor type of the other array
         * @param other The other array to copy
         * @return *this
         */
        template<typename u_type_t, typename u_allocator_t>
        requires(hud::is_copy_assignable_v<type_t, u_type_t>)
        constexpr array &operator=(const array<u_type_t, u_allocator_t> &other) noexcept
        {
            copy_assign(other.data(), other.count());
            return *this;
        }

        /**
         * Move assign another array<u_type_t> with the same allocator.
         * Never assume that the move assignement will keep the capacity of the moved array.
         * Depending of the Type and the allocator the move operation can reallocate or not, this is by design and allow some move optimisation
         * @param other The other array to move
         */
        constexpr array &operator=(array &&other) noexcept
        requires(hud::is_move_assignable_v<type_t>)
        {
            if (this != &other) [[likely]]
            {
                move_assign(hud::move(other));
            }
            return *this;
        }

        /**
         * Move assign another array<u_type_t> with the same allocator.
         * Never assume that the move assignement will keep the capacity of the moved array.
         * Depending of the Type and the allocator the move operation can reallocate or not, this is by design and allow some move optimisation
         * If u_type_t is bitwise moveable to type_t, the allocator is just moved, else it call the type_t's move constructor is called for each element.
         * @tparam u_type_t The element type of the other array
         * @tparam u_allocator_t The alloctor type of the other array
         * @param other The other array to move
         */
        template<typename u_type_t, typename u_allocator_t>
        requires(hud::is_move_assignable_v<type_t, u_type_t>)
        constexpr array &operator=(array<u_type_t, u_allocator_t> &&other) noexcept
        {
            move_assign(hud::forward<array<u_type_t, u_allocator_t>>(other));
            return *this;
        }

        /**
         * Appends a new element to the end of the container by calling the constructor in-place.
         * If the new count() is greater than max_count() then all iterators and references are invalidated.
         * @tparam args_t The type_t constructor arguments
         * @param args Arguments to forward to the constructor of the element
         * @return Index of the newly created element
         */
        template<typename... args_t>
        requires(hud::is_constructible_v<type_t, args_t...>)
        constexpr usize emplace_back(args_t &&...args) noexcept
        {
            const usize old_count = count();
            const usize new_count = count() + 1u;

            // If we don't have enough place in allocated memory we need to reallocate.
            if (new_count > max_count())
            {
                allocation_type new_allocation = allocator_type::template allocate<type_t>(new_count);
                // Construct the element in-place
                hud::memory::construct_at(new_allocation.data_at(old_count), hud::forward<args_t>(args)...);
                // Relocate the element that are before the newly added element if any
                if (old_count != 0u)
                {
                    hud::memory::fast_move_or_copy_construct_array_then_destroy(new_allocation.data(), allocation.data(), old_count);
                }
                // Free the allocation and replace it with the newly created
                free_allocation_and_replace_it(hud::move(new_allocation), new_count);
            }
            else
            {
                hud::memory::construct_at(allocation.data_at(old_count), hud::forward<args_t>(args)...);
                end_ptr = allocation.data_at(new_count);
            }

            return old_count;
        }

        /**
         * Appends a new element to the end of the container by calling the constructor in-place.
         * If the new count() is greater than max_count() then all iterators and references are invalidated.
         * @tparam args_t The type_t constructor arguments
         * @param args Arguments to forward to the constructor of the element
         * @return Reference to the newly created element
         */
        template<typename... args_t>
        requires(hud::is_constructible_v<type_t, args_t...>)
        [[nodiscard]] constexpr type_t &emplace_back_to_ref(args_t &&...args) noexcept
        {
            const usize element_index = emplace_back(hud::forward<args_t>(args)...);
            return *data_at(element_index);
        }

        /**
         * Emplace a new element at the given index of the array by calling the constructor in-place.
         * If the new count() is greater than max_count() then all iterators and references are invalidated, else only the iterator after the given index are invalidated.
         * @tparam args_t The type_t constructor arguments
         * @param idx The index of insertion
         * @param args Arguments to forward to the constructor of the element
         */
        template<typename... args_t>
        void emplace_at(const usize idx, args_t &&...args) noexcept
        {
            check(idx <= count());
            const usize old_count = count();
            const usize new_count = count() + 1u;

            // if we don't have enough place in allocated memory we need to reallocate.
            if (new_count > max_count())
            {
                allocation_type new_allocation = allocator_type::template allocate<type_t>(new_count);
                // Construct the element in-place
                hud::memory::construct_at(new_allocation.data_at(idx), hud::forward<args_t>(args)...);
                // Relocate elements if any
                if (old_count > 0u)
                {
                    // Relocate others before the emplaced element
                    hud::memory::fast_move_or_copy_construct_array_then_destroy(new_allocation.data(), data(), idx);
                    // Relocate others after the emplaced element
                    hud::memory::fast_move_or_copy_construct_array_then_destroy(new_allocation.data_at(idx + 1), allocation.data_at(idx), old_count - idx);
                }
                // Free the allocation and replace it with the newly created
                free_allocation_and_replace_it(hud::move(new_allocation), new_count);
            }
            else
            {
                // Relocate others after the emplaced element
                type_t *emplace_ptr = allocation.data_at(idx);
                hud::memory::move_or_copy_construct_array_then_destroy_backward(emplace_ptr + 1, emplace_ptr, static_cast<usize>(end_ptr - emplace_ptr));
                // Construct the element in-place
                hud::memory::construct_at(emplace_ptr, hud::forward<args_t>(args)...);
                end_ptr++;
            }
        }

        /**
         * Emplace a new element at the given index of the array by calling the constructor in-place.
         * If the new count() is greater than max_count() then all iterators and references are invalidated, else only the iterator after the given index are invalidated.
         * @tparam args_t The type_t constructor arguments
         * @param index The index of insertion
         * @param args Arguments to forward to the constructor of the element
         * @return Reference to the newly created element
         */
        template<typename... args_t>
        [[nodiscard]] type_t &emplace_at_to_ref(const usize index, args_t &&...args) noexcept
        {
            emplace_at(index, hud::forward<args_t>(args)...);
            return *data_at(index);
        }

        /**
         * Append by copying a new element to the end of the container.
         * If the new count() is greater than max_count() then all iterators and references are invalidated.
         * @param element The element to copy to the end of the container
         * @return Index of the newly added element
         */
        constexpr usize add(const type_t &element) noexcept
        {
            return emplace_back(element);
        }

        /**
         * Append by moving a new element to the end of the container.
         * If the new count() is greater than max_count() then all iterators and references are invalidated.
         * @param element The element to copy to the end of the container
         * @return Index of the newly added element
         */
        constexpr usize add(type_t &&element) noexcept
        {
            return emplace_back(hud::move(element));
        }

        /**
         * Append by copying a new element to the end of the container.
         * If the new count() is greater than max_count() then all iterators and references are invalidated.
         * @param element The element to copy to the end of the container
         * @return Reference to the newly added element
         */
        [[nodiscard]] constexpr type_t &add_to_ref(const type_t &element) noexcept
        {
            return emplace_back_to_ref(element);
        }

        /**
         * Append by moving a new element to the end of the container.
         * If the new count() is greater than max_count() then all iterators and references are invalidated.
         * @param element The element to move to the end of the container
         * @return Reference to of the newly added element
         */
        [[nodiscard]] constexpr type_t &add_to_ref(type_t &&element) noexcept
        requires(hud::is_move_constructible_v<type_t>)

        {
            return emplace_back_to_ref(hud::move(element));
        }

        /**
         * Add number of elements at the end of the array and grow the allocation if needed but do not call any constructor or setting memory to zero.
         * If the new count() is greater than max_count() then all iterators and references are invalidated.
         * Use this with caution, elements constructor are not called but destructor will be called on elements that do not call their constructor, this can lead to UB or crash.
         * @param element_number Number of element to add
         * @param Index of the first element added
         */
        constexpr usize add_no_construct(const usize element_number) noexcept
        {
            const usize old_count = count();
            const usize new_count = count() + element_number;

            reserve(new_count);
            end_ptr = data_at(new_count);
            return old_count;
        }

        /**
         * Resize the container allocation to fit the number of elements.
         * If no elements is contained in the array then allocation is free, else allocation is shrink to fit the number of elements contains in the array.
         */
        void shrink_to_fit() noexcept
        {
            if (is_empty())
            {
                free_to_null();
            }
            else if (end_ptr < allocation.data_end())
            {
                allocation_type new_allocation = allocator_type::template allocate<type_t>(count());
                hud::memory::fast_move_or_copy_construct_array_then_destroy(new_allocation.data(), allocation.data(), count());
                free_allocation_and_replace_it(hud::move(new_allocation), count());
            }
        }

        /**
         * Remove number of elements in the array from a given index.
         * @param index The index to remove
         * @param count_to_remove (Optional) Count of elements to remove. Default is 1.
         */
        void remove_at(const usize index, const usize count_to_remove = 1) noexcept
        {
            check(index < count());
            if (!is_empty())
            {
                type_t *first_item_to_remove = data_at(index);
                check(count_to_remove <= count()); // Remove more elements than possible
                hud::memory::destroy_array(first_item_to_remove, count_to_remove);
                const usize remains = count() - count_to_remove;

                const usize count_to_relocate_after = remains - index;
                if (count_to_relocate_after > 0)
                {
                    // We need to copy construct elements stored in removed elements memory
                    // We need to move assign elements stored after the elements that are copy constructed
                    type_t *first_items_to_relocate = first_item_to_remove + count_to_remove;
                    type_t *first_items_to_move = first_items_to_relocate + count_to_remove;

                    // Copy or move construct elements to free space
                    hud::memory::move_or_copy_construct_array(first_item_to_remove, first_items_to_relocate, count_to_remove);
                    // Relocate all elements left to keep element continuity
                    hud::memory::move_or_copy_assign_array(first_items_to_relocate, first_items_to_move, end_ptr);
                    hud::memory::destroy_array(end_ptr, index);
                }
                end_ptr = allocation.data_at(remains);
            }
        }

        /**
         * Remove number of elements in the array from a given index.
         * @param index The index to remove
         * @param count_to_remove (Optional) Count of elements to remove. Default is 1.
         */
        void remove_at_shrink(const usize index, const usize count_to_remove = 1) noexcept
        {
            if (count() > 0)
            {
                check(index < count());
                type_t *first_item_to_remove = data_at(index);
                check(count_to_remove <= count()); // Remove more elements than possible
                hud::memory::destroy_array(first_item_to_remove, count_to_remove);
                const usize remains = count() - count_to_remove;

                if (remains > 0)
                {
                    allocation_type new_allocation = allocator_type::template allocate<type_t>(remains);
                    // Move or copy elements before the removed element then destroy moved or copied elements from the old allocation
                    hud::memory::fast_move_or_copy_construct_array_then_destroy(new_allocation.data(), allocation.data(), index);
                    // Move or copy elements after the removed element then destroy moved or copied elements from the old allocation
                    hud::memory::fast_move_or_copy_construct_array_then_destroy(new_allocation.data_at(index), allocation.data_at(index + count_to_remove), remains - index);
                    free_allocation_and_replace_it(hud::move(new_allocation), remains);
                }
                else
                {
                    free_to_null();
                }
            }
        }

        /**
         * Resize the array to ensure it contains a number of elements.
         * Call defaut constructor or set memory to zero depending if the type is trivially constructible or not.
         * @param element_number Number of element the array must contains
         */
        void resize(const usize element_number) noexcept
        {
            const isize diff = static_cast<const isize>(element_number - count());
            // If we grow the element count
            if (diff > 0)
            {
                const usize old_count = add_no_construct(static_cast<usize>(diff));
                type_t *first_added = data_at(old_count);
                hud::memory::default_construct_array(first_added, first_added + static_cast<usize>(diff));
            }
            else if (diff < 0)
            {
                remove_at(element_number, static_cast<usize>(-diff));
            }
        }

        /**
         * Reserve enough memory to ensure the array can contains a number of elements without reallocating.
         * Only grow the allocation and relocate object if the number of elements requested is bigger than the current max number of elements.
         * Do nothing if the given element number is less or equal the maximum element count.
         * @param element_number Number of element th array must be able to contains in memory
         */
        constexpr void reserve(const usize element_number) noexcept
        {
            if (element_number > max_count())
            {
                allocation_type new_allocation = allocator_type::template allocate<type_t>(element_number);
                if (count() > 0u)
                {
                    hud::memory::fast_move_or_copy_construct_array_then_destroy(new_allocation.data(), allocation.data(), count());
                }
                free_allocation_and_replace_it(hud::move(new_allocation), count());
            }
        }

        /**
         * Remove all elements from the array.
         * Call all the destructor of each element in the array if not trivially destructible but do not free the allocated memory.
         */
        HD_FORCEINLINE constexpr void clear() noexcept
        {
            if (count() > 0)
            {
                hud::memory::destroy_array(allocation.data(), count());
                end_ptr = allocation.data();
            }
        }

        /**
         * Remove all elements from the array.
         * Call all the destructor of each element in the array if not trivially destructible, then free the allocated memory.
         */
        HD_FORCEINLINE void clear_shrink() noexcept
        {
            if (count() > 0)
            {
                hud::memory::destroy_array(allocation.data(), count());
                free_to_null();
            }
        }

        /** Retreives number of elements in the array. */
        [[nodiscard]] HD_FORCEINLINE constexpr usize count() const noexcept
        {
            return static_cast<usize>(end_ptr - allocation.data());
        }

        /** Retreives number of elements in the array in bytes. */
        [[nodiscard]] HD_FORCEINLINE constexpr usize byte_count() const noexcept
        {
            return count() * sizeof(type_t);
        }

        /** Retreives maximum number of elements the array can contains. */
        [[nodiscard]] HD_FORCEINLINE constexpr usize max_count() const noexcept
        {
            return allocation.count();
        }

        /** Retreives maximum number of elements the array can contains. */
        [[nodiscard]] HD_FORCEINLINE constexpr usize max_byte_count() const noexcept
        {
            return allocation.byte_count();
        }

        /** Retrieves the allocator. */
        [[nodiscard]] HD_FORCEINLINE constexpr const allocator_type &allocator() const noexcept
        {
            return *static_cast<const allocator_type *>(this);
        }

        /** Retrieves a constant pointer to the first element of the contiguous elements. */
        [[nodiscard]] HD_FORCEINLINE constexpr const type_t *data() const noexcept
        {
            return allocation.data();
        }

        /** Retrieves a pointer to the raw data first element of the contiguous elements. */
        [[nodiscard]] constexpr type_t *data() noexcept
        {
            return allocation.data();
        }

        /** Return the slack in number of elements. */
        [[nodiscard]] HD_FORCEINLINE constexpr usize slack() const noexcept
        {
            return static_cast<usize>(allocation.data_end() - end_ptr);
        }

        /**
         * Check whether index is in valid range or not.
         * @param index The index to check
         * @return true if the index is in valid range, false otherwise
         */
        [[nodiscard]] constexpr bool is_valid_index(const usize index) const noexcept
        {
            // When index is unsigned, we don't need to check for negative values
            static_assert(is_unsigned_v<decltype(index)>);
            return index < count();
        }

        /** Checks whether the array is empty of not. */
        [[nodiscard]] HD_FORCEINLINE constexpr bool is_empty() const noexcept
        {
            return end_ptr == allocation.data();
        }

        /**
         * Retrieves reference on the element at the given index.
         * @param index The index of the element to retrieve
         * @return Reference on the element at the given index
         */
        [[nodiscard]] constexpr type_t &operator[](const usize index) noexcept
        {
            check(is_valid_index(index));
            return *data_at(index);
        }

        /**
         * Retrieves reference on the element at the given index.
         * @param index The index of the element to retrieve
         * @return Reference on the element at the given index
         */
        [[nodiscard]] HD_FORCEINLINE constexpr const type_t &operator[](const usize index) const noexcept
        {
            check(is_valid_index(index));
            return *data_at(index);
        }

        /**
         * Retrieves a sub-slice of the slice.
         * @param first_index The index of the first element in the slice sequence
         * @param count The number of elements the slice sequence must contains
         * @return The sub-slice from data()+first_index over a sequence of count elements
         */
        [[nodiscard]] HD_FORCEINLINE constexpr const slice<type_t> sub_slice(const usize first_index, const usize count) const noexcept
        {
            return allocation.sub_slice(first_index, count);
        }

        /** Retrieves reference on the first element. */
        [[nodiscard]] HD_FORCEINLINE constexpr const type_t &first() const noexcept
        {
            check(!is_empty());
            return data()[0];
        }

        /** Retrieves reference on the first element. */
        [[nodiscard]] HD_FORCEINLINE constexpr type_t &first() noexcept
        {
            check(!is_empty());
            return data()[0];
        }

        /**
         * Retrieves reference on the first element
         * @param from_the_start The index to access from the start of the array
         * @return Reference on the first element
         */
        [[nodiscard]] HD_FORCEINLINE constexpr const type_t &first(usize from_the_start) const noexcept
        {
            check(is_valid_index(from_the_start));
            return data()[from_the_start];
        }

        /**
         * Retrieves reference on the first element
         * @param from_the_start The index to access from the start of the array
         * @return Reference on the first element
         */
        [[nodiscard]] HD_FORCEINLINE constexpr type_t &first(usize from_the_start) noexcept
        {
            check(is_valid_index(from_the_start));
            return data()[from_the_start];
        }

        /** Retrieves reference on the last element. */
        [[nodiscard]] HD_FORCEINLINE constexpr const type_t &last() const noexcept
        {
            check(!is_empty());
            return data()[count() - 1];
        }

        /** Retrieves reference on the last element. */
        [[nodiscard]] HD_FORCEINLINE constexpr type_t &last() noexcept
        {
            check(!is_empty());
            return data()[count() - 1];
        }

        /**
         * Retrieves reference on the last element.
         * @param from_the_end The index to access from the end of the array
         * @return Reference on the last element
         */
        [[nodiscard]] HD_FORCEINLINE constexpr const type_t &last(usize from_the_end) const noexcept
        {
            check(is_valid_index(count() - 1 - from_the_end));
            return data()[count() - 1 - from_the_end];
        }

        /**
         * Retrieves reference on the last element.
         * @param from_the_end The index to access from the end of the array
         * @return Reference on the last element
         */
        [[nodiscard]] HD_FORCEINLINE constexpr type_t &last(usize from_the_end) noexcept
        {
            check(is_valid_index(count() - 1 - from_the_end));
            return data()[count() - 1 - from_the_end];
        }

        /**
         * Swap with another array.
         * @param other The array to swap with
         */
        void swap(array &other) noexcept
        requires(hud::is_swappable_v<type_t>)
        {
            static_assert(hud::is_nothrow_swappable_v<type_t>, "swap(array<type_t>&) is throwable. array is not designed to allow throwable swappable components");
            hud::swap(end_ptr, other.end_ptr);
            hud::swap(allocation, other.allocation);
            hud::swap(*static_cast<allocator_type *>(this), *static_cast<allocator_type *>(&other));
        }

        /**
         * Find the fist index of a given element where the predicate array[index] == to_find is true.
         * Given comparand must be comparable with type_t operator==.
         * @param CompType The comparand type used to compare
         * @param to_find The element to find
         * @return first index of the element where the predicate array[index] == element is true, hud::index_none otherwise
         */
        template<typename CompType>
        requires(hud::is_comparable_with_equal_v<type_t, CompType>)
        [[nodiscard]] usize find_first_index(const CompType &to_find) const
        {
            const type_t *HD_RESTRICT begin = data();
            const type_t *HD_RESTRICT end = end_ptr;

            for (const type_t *HD_RESTRICT cur = begin; cur != end; cur++)
            {
                if (*cur == to_find)
                {
                    return static_cast<usize>(cur - begin);
                }
            }
            return hud::index_none;
        }

        /**
         * Find the last index of an element where the predicate array[index] == to_find is true.
         * Given comparand must be comparable with type_t operator==.
         * @param CompType The comparand type used to compare
         * @param to_find The element to find
         * @return last index of the element where the predicate array[index] == element is true, hud::index_none otherwise
         */
        template<typename CompType>
        requires(hud::is_comparable_with_equal_v<type_t, CompType>)
        [[nodiscard]] usize find_last_index(const CompType &to_find) const
        {
            const type_t *HD_RESTRICT begin = data();
            const type_t *HD_RESTRICT end = end_ptr;
            const type_t *HD_RESTRICT cur = end;
            while (cur != begin)
            {
                cur--;
                if (*cur == to_find)
                {
                    return static_cast<usize>(cur - begin);
                }
            }
            return hud::index_none;
        }

        /**
         * Find the fist index of an element where the user-defined predicate return true.
         * @tparam UnaryPredicate The Unary predicate to use
         * @param predicate The predicate to use
         * @return first index of the element matching the predicate, hud::index_none otherwise
         */
        template<typename UnaryPredicate>
        [[nodiscard]] usize find_first_index_by_predicate(const UnaryPredicate predicate) const
        {
            const type_t *HD_RESTRICT begin = data();
            const type_t *HD_RESTRICT end = end_ptr;

            for (const type_t *HD_RESTRICT cur = begin; cur != end; cur++)
            {
                if (predicate(*cur))
                {
                    return static_cast<usize>(cur - begin);
                }
            }
            return hud::index_none;
        }

        /**
         * Find the last index of a given element where the user-defined predicate return true.
         * @tparam UnaryPredicate The Unary predicate to use
         * @param predicate The predicate to use
         * @return last index of the element matching the predicate, hud::index_none otherwise
         */
        template<typename UnaryPredicate>
        [[nodiscard]] usize find_last_index_by_predicate(const UnaryPredicate predicate) const
        {
            const type_t *HD_RESTRICT begin = data();
            const type_t *HD_RESTRICT end = end_ptr;
            const type_t *HD_RESTRICT cur = end;
            while (cur != begin)
            {
                cur--;
                if (predicate(*cur))
                {
                    return static_cast<usize>(cur - begin);
                }
            }
            return hud::index_none;
        }

        /**
         * Checks whether an element is contained in the array or not.
         * @param CompType The comparand type used to compare
         * @param to_find The element to find
         * @return true if the element is contained in the array, false otherwise
         */
        template<typename CompType>
        requires(hud::is_comparable_with_equal_v<type_t, CompType>)
        [[nodiscard]] bool contains(const CompType &to_find) const
        {
            return find_first_index(to_find) != hud::index_none;
        }

        /**
         * Checks whether an element match the user-defined predicate is contained in the array or not.
         * @tparam UnaryPredicate The Unary predicate to use
         * @param predicate The predicate to use
         * @return true if an element match the user-defined predicate is contained in the array, false otherwise
         */
        template<typename UnaryPredicate>
        [[nodiscard]] bool contains_by_predicate(const UnaryPredicate predicate) const
        {
            return find_first_index_by_predicate(predicate) != hud::index_none;
        }

        /** Retrieves an iterator to the beginning of the array. */
        [[nodiscard]] HD_FORCEINLINE constexpr iterator begin() noexcept
        {
            return iterator(data());
        }

        /** Retrieves an iterator to the beginning of the array. */
        [[nodiscard]] HD_FORCEINLINE constexpr const_iterator begin() const noexcept
        {
            return const_iterator(data());
        }

        /** Retrieves an iterator to the end of the array. */
        [[nodiscard]] HD_FORCEINLINE constexpr iterator end() noexcept
        {
            return iterator(end_ptr);
        }

        /** Retrieves an iterator to the end of the array. */
        [[nodiscard]] HD_FORCEINLINE constexpr const_iterator end() const noexcept
        {
            return const_iterator(end_ptr);
        }

    private:
        /**
         * Copy assign a source_count elements from the source of data to the array.
         * @tparam u_type_t The element type of the other array
         * @param source The source of data to copy
         * @param source_count Element count in the source to copy
         */
        template<typename u_type_t>
        constexpr void copy_assign(const u_type_t *source, const usize source_count) noexcept
        {
            // Grow the allocation if we don't have enough room
            // If we need to reallocate, we destroy all elements before reallocating the allocation
            // Then we copy construct all elements of source in the allocation
            if (source_count > max_count())
            {
                // Destroy existing
                hud::memory::destroy_array(data(), count());
                // Allocate a new allocation and copy construct source into it
                allocation_type new_allocation = allocator_type::template allocate<type_t>(source_count);
                hud::memory::copy_construct_array(new_allocation.data(), source, source_count);
                // Save the newly allocated allocation
                free_allocation_and_replace_it(hud::move(new_allocation), source_count);
            }
            // If we don't need to reallocate
            else
            {
                if (hud::is_constant_evaluated())
                // LCOV_EXCL_START
                {
                    copy_assign_or_copy_construct_no_reallocation(source, source_count);
                }
                // LCOV_EXCL_STOP
                else if (!hud::is_bitwise_copy_assignable_v<type_t, u_type_t>)
                {
                    copy_assign_or_copy_construct_no_reallocation(source, source_count);
                }
                else if (source_count > 0u)
                {
                    hud::memory::copy_assign_array(data(), source, source_count);
                }
                end_ptr = allocation.data_at(source_count);
            }
        }

        /**
         * Copy assign a source_count elements from the source of data to the array.
         * Call assign for already existing elements or copy constructor for non existing elements.
         * This method supposed source_count <= max_count() because it does not reallocate the buffer.
         * @tparam u_type_t The element type of the other array
         * @param source The source of data to copy
         * @param source_count Element count in the source to copy
         */
        template<typename u_type_t>
        constexpr void copy_assign_or_copy_construct_no_reallocation(const u_type_t *source, const usize source_count) noexcept
        {
            check(source_count <= max_count());

            const isize extra_to_construct = source_count - count();
            // We assign all elements that are already in the allocation,
            // Then we copy construct all remaining elements at the end of the assigned elements
            if (extra_to_construct > 0)
            {
                hud::memory::copy_assign_array(data(), source, count());
                hud::memory::copy_construct_array(data_at(count()), source + count(), static_cast<usize>(extra_to_construct));
            }
            // If we assign less or equal count of elements than the current element count
            // we copy assign all new elements of the source in the allocation, then we destroy the remaining elements
            else
            {
                hud::memory::copy_assign_array(data(), source, source_count);
                hud::memory::destroy_array(data() + source_count, static_cast<usize>(-extra_to_construct));
            }
        }

        /**
         * Move assign another array.
         * If u_type_t is bitwise moveable to type_t and the allocator are the same, the internal allocation ownership is given to this by stealing the pointer
         * If u_type_t is not bitwise moveable to type_t or allocators are differents, it do not still the pointer and call the type_t's move constructor is called for each element.
         * @tparam u_type_t The element type of the other array
         * @tparam u_allocator_t The alloctor type of the other array
         * @param other The other array to move
         */
        template<typename u_type_t, typename u_allocator_t>
        constexpr void move_assign(array<u_type_t, u_allocator_t> &&other) noexcept
        {
            if (!hud::is_constant_evaluated() && is_same_v<allocator_type, u_allocator_t> && hud::is_bitwise_move_assignable_v<type_t, u_type_t>)
            {
                // Destroy existing elements
                hud::memory::destroy_array(data(), count());
                // Still the allocation and the allocator
                *static_cast<allocator_type *>(this) = hud::move(*static_cast<allocator_type *>(&other));
                free_allocation_and_replace_it(allocation_type(reinterpret_cast<type_t *>(other.allocation.data()), other.allocation.count()), other.count());
                other.leak();
            }
            else
            {
                // If we need to reallocate, we destroy all elements before reallocating the allocation
                // Then we copy construct all elements of source in the allocation
                if (other.count() > max_count())
                {
                    // Delete existing elements
                    hud::memory::destroy_array(data(), count());
                    allocation_type new_allocation = allocator_type::template allocate<type_t>(other.count());
                    hud::memory::move_or_copy_construct_array(new_allocation.data(), other.data(), other.count());
                    free_allocation_and_replace_it(hud::move(new_allocation), other.count());
                }
                // If we don't need to reallocate
                else
                {

                    // We assign all elements that are already in the allocation,
                    // Then we copy construct all remaining elements at the end of the assigned elements
                    if (other.count() > count())
                    {
                        const auto to_assign_end = other.data() + count();
                        hud::memory::move_or_copy_assign_array(data(), other.data(), to_assign_end);
                        hud::memory::move_or_copy_construct_array(data() + count(), to_assign_end, other.count() - count());
                    }
                    // If we assign less or equal count of elements than the current element count
                    // we copy assign all new elements of the source in the allocation, then we destroy the remaining elements
                    else
                    {
                        hud::memory::move_or_copy_assign_array(data(), other.data(), other.end_ptr);
                        hud::memory::destroy_array(data() + other.count(), count() - other.count());
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
         * @param new_count_of_element The new count of elements to set
         */
        constexpr void free_allocation_and_replace_it(allocation_type &&new_allocation, const usize new_count_of_element) noexcept
        {
            allocator_type::template free<type_t>(allocation);
            allocation = hud::move(new_allocation);
            end_ptr = allocation.data_at(new_count_of_element);
        }

        /** Free the allocation and set everything to default */
        constexpr void free_to_null() noexcept
        {
            allocator_type::template free<type_t>(allocation);
            leak();
        }

        /** Set all internal pointers to nullptr without freeing the memory. Be sure of what you are doing... */
        constexpr void leak() noexcept
        {
            allocation.leak();
            end_ptr = nullptr;
        }

        /**
         * Retrieves a pointer to the elements at given index of the sequence.
         * A special case is allowed when data_at(count()) is called.
         * No assertion is done but the pointer is not valid to dereference.
         * This feature is usefull to get a pointer end to performs pointer aritmetics.
         */
        [[nodiscard]] constexpr type_t *data_at(const usize index) const noexcept
        {
            return allocation.data_at(index);
        }

    private:
        template<typename u_type_t, typename u_allocator_t>
        friend class array; // Friend with other array of other types

    private:
        /** The allocation */
        allocation_type allocation;

        /** Pointer to the end of the element sequence. */
        type_t *end_ptr = nullptr;
    };

    /**
     * Swap first array with the second array.
     * Same as first.swap(second).
     * @tparam type_t The element type
     * @tparam allocator_t The allocator type of both array
     * @param first The first array to swap
     * @param second The second array to swap
     */
    template<typename type_t, typename allocator_t>
    requires(hud::is_swappable_v<type_t>)
    HD_FORCEINLINE void swap(array<type_t, allocator_t> &first, array<type_t, allocator_t> &second) noexcept
    {
        first.swap(second);
    }

    /**
     * Checks whether right and left array are equal.
     * array are equal if both contains same number of elements and all values are equal.
     * Value types must be comparable with operator==() if types are not bitwise comparable with equal operator.
     * @tparam left_t Value type of the left array
     * @tparam left_allocator_t The allocator type of the left array
     * @tparam right_t Value type of the right array
     * @tparam right_allocator_t The allocator type of the right array
     * @param left The left array to compare
     * @param right The right array to compare
     * @param true if right and left Arrays are equal, false otherwise
     */
    template<typename left_t, typename left_allocator_t, typename right_t, typename right_allocator_t>
    [[nodiscard]] HD_FORCEINLINE constexpr bool operator==(const array<left_t, left_allocator_t> &left, const array<right_t, right_allocator_t> &right) noexcept
    {
        // Arrays are not equal if the counts of elements differ
        if (left.count() != right.count())
        {
            return false;
        }

        if (left.data() != nullptr)
        {
            if (right.data() != nullptr) // If left and right are not nullptr, compare them
            {
                return hud::memory::equal_array(left.data(), right.data(), left.count());
            }
            return false; // Arrays are not equal if right is not nullptr but left is nullptr
        }
        return right.data() != nullptr; // If left is nullptr, arrays are not equal if right is also nullptr
    }

    /**
     * Checks whether right and left array are not equals.
     * array are equal if they do not contains the same number of elements or at least one element is not equal in both array.
     * Value types must be comparable with operator!=() if types are not bitwise comparable with not equal operator.
     * @tparam left_t Value type of the left array
     * @tparam left_allocator_t The allocator type of the left array
     * @tparam right_t Value type of the right array
     * @tparam right_allocator_t The allocator type of the right array
     * @param left The left array to compare
     * @param right The right array to compare
     * @param true if right and left Arrays are not equals, false otherwise
     */
    template<typename left_t, typename left_allocator_t, typename right_t, typename right_allocator_t>
    [[nodiscard]] HD_FORCEINLINE constexpr bool operator!=(const array<left_t, left_allocator_t> &left, const array<right_t, right_allocator_t> &right) noexcept
    {
        // Arrays are not equal if the counts of elements differ
        if (left.count() != right.count())
        {
            return false;
        }

        if (left.data() != nullptr)
        {
            if (right.data() != nullptr) // If left and right are not nullptr, compare them
            {
                return hud::memory::not_equal_array(left.data(), right.data(), left.count());
            }
            return true; // Arrays are not equal if right is not nullptr but left is nullptr
        }
        return right.data() == nullptr; // If left is nullptr, arrays are not equal if right is also nullptr
    }

    /**
     * Checks whether right is less than left array.
     * An right is less than left if :
     *     - right don't contains a value while left do, or,
     *     - both contains a value and right value is less than left value.
     * Value types must be comparable with operator<() if types are not bitwise comparable with less operator.
     * @tparam left_t Value type of the left array
     * @tparam left_allocator_t The allocator type of the left array
     * @tparam right_t Value type of the right array
     * @tparam right_allocator_t The allocator type of the right array
     * @param left The left array to compare
     * @param right The right array to compare
     * @param true if right is less than left array, false otherwise
     */
    template<typename left_t, typename left_allocator_t, typename right_t, typename right_allocator_t>
    [[nodiscard]] HD_FORCEINLINE constexpr bool operator<(const array<left_t, left_allocator_t> &left, const array<right_t, right_allocator_t> &right) noexcept
    {
        return lexicographical_compare(left.data(), left.data() + left.count(), right.data(), right.data() + right.count());
    }

    /**
     * Checks whether right is greater than left array.
     * An right is greater than left if :
     *     - right don't contains a value while left do, or,
     *     - both contains a value and right value is greater than left value.
     * Value types must be comparable with operator<() if types are not bitwise comparable with less operator.
     * @tparam left_t Value type of the left array
     * @tparam left_allocator_t The allocator type of the left array
     * @tparam right_t Value type of the right array
     * @tparam right_allocator_t The allocator type of the right array
     * @param left The left array to compare
     * @param right The right array to compare
     * @param true if right is greater than left array, false otherwise
     */
    template<typename left_t, typename left_allocator_t, typename right_t, typename right_allocator_t>
    [[nodiscard]] HD_FORCEINLINE constexpr bool operator>(const array<left_t, left_allocator_t> &left, const array<right_t, right_allocator_t> &right) noexcept
    {
        return right < left;
    }

    /**
     * Checks whether right is less or equal left array.
     * An right is less or equal left if :
     *     - right don't contains a value while left do, or,
     *     - both contains a value and right value is less or equal left value.
     * Value types must be comparable with operator<() if types are not bitwise comparable with less operator.
     * @tparam left_t Value type of the left array
     * @tparam left_allocator_t The allocator type of the left array
     * @tparam right_t Value type of the right array
     * @tparam right_allocator_t The allocator type of the right array
     * @param left The left array to compare
     * @param right The right array to compare
     * @param true if right is less or equal left array, false otherwise
     */
    template<typename left_t, typename left_allocator_t, typename right_t, typename right_allocator_t>
    [[nodiscard]] HD_FORCEINLINE constexpr bool operator<=(const array<left_t, left_allocator_t> &left, const array<right_t, right_allocator_t> &right) noexcept
    {
        return !(right < left);
    }

    /**
     * Checks whether right is greater or equal left array.
     * An right is greater or equal left if :
     *     - right don't contains a value while left do, or,
     *     - both contains a value and right value is greater or equal left value.
     * Value types must be comparable with operator<() if types are not bitwise comparable with less operator.
     * @tparam left_t Value type of the left array
     * @tparam left_allocator_t The allocator type of the left array
     * @tparam right_t Value type of the right array
     * @tparam right_allocator_t The allocator type of the right array
     * @param left The left array to compare
     * @param right The right array to compare
     * @param true if right is greater or equal left array, false otherwise
     */
    template<typename left_t, typename left_allocator_t, typename right_t, typename right_allocator_t>
    [[nodiscard]] HD_FORCEINLINE constexpr bool operator>=(const array<left_t, left_allocator_t> &left, const array<right_t, right_allocator_t> &right) noexcept
    {
        return !(left < right);
    }

} // namespace hud

#endif // HD_INC_CORE_ARRAY_H