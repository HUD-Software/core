#ifndef HD_INC_OSABSTRACTIONLAYER_MEMORY_H
#define HD_INC_OSABSTRACTIONLAYER_MEMORY_H
#include "traits/is_bitwise_comparable.h"
#include "traits/is_bitwise_copy_assignable.h"
#include "traits/is_bitwise_copy_constructible.h"
#include "traits/is_bitwise_move_assignable.h"
#include "traits/is_bitwise_move_constructible.h"
#include "traits/is_comparable_with_equal.h"
#include "traits/is_comparable_with_not_equal.h"
#include "traits/is_constructible.h"
#include "traits/is_copy_assignable.h"
#include "traits/is_copy_constructible.h"
#include "traits/is_default_constructible.h"
#include "traits/is_destructible.h"
#include "traits/is_move_assignable.h"
#include "traits/is_move_constructible.h"
#include "traits/is_nothrow_constructible.h"
#include "traits/is_nothrow_copy_assignable.h"
#include "traits/is_nothrow_copy_constructible.h"
#include "traits/is_nothrow_default_constructible.h"
#include "traits/is_nothrow_destructible.h"
#include "traits/is_nothrow_move_assignable.h"
#include "traits/is_nothrow_move_constructible.h"
#include "traits/is_pointer.h"
#include "traits/is_trivially_default_constructible.h"
#include "traits/is_trivially_destructible.h"

#include "templates/forward.h"
#include "templates/move.h"

#include <memory> // std::construct_at

#if defined(HD_OS_WINDOWS)
    #include "os_windows/memory.h"
#elif defined(HD_OS_LINUX)
    #include "os_linux/memory.h"
#else
    #error Targeted OS not supported
#endif

namespace hud
{

    /** Provides memory operations. */
    struct memory :
#if defined(HD_OS_WINDOWS)
        os::windows::memory
#elif defined(HD_OS_LINUX)
        os::linux::memory
#endif
    {
        /**
         * Call constructor of type.
         * @tparam type_t Type to default construct
         * @param address The address of the type type_t to default construct
         */
        template<typename type_t, typename... args_t>
        requires(hud::is_constructible_v<type_t, args_t...>)
        static constexpr type_t *construct_at(type_t *const address, args_t &&...args) noexcept
        {
            static_assert(hud::is_nothrow_constructible_v<type_t, args_t...>, "type_t constructor is throwable.hud::memory::construct_at is not designed to allow throwable constructible type");
            return std::construct_at(address, hud::forward<args_t>(args)...);
        }

        /**
         * Call copy constructor of type of u_type_t is copy constructible type to type_t.
         * @tparam type_t Type to copy construct
         * @tparam u_type_t Type to copy
         * @param address The address of the type type_t to copy construct
         * @param other The objet to copy to address
         */
        template<typename type_t, typename u_type_t>
        requires(hud::is_copy_constructible_v<type_t, u_type_t>)
        static constexpr type_t *construct_at(type_t *const address, const u_type_t &other) noexcept
        {
            static_assert(hud::is_nothrow_copy_constructible_v<type_t, u_type_t>, "type_t(const u_type_t&) constructor is throwable.hud::memory::construct_at is not designed to allow throwable constructible type");
            return std::construct_at(address, other);
        }

        /**
         * Call move constructor of type of u_type_t is move constructible type to type_t.
         * @tparam type_t Type to move construct
         * @tparam u_type_t Type to move
         * @param address The address of the type type_t to move construct
         * @param other The objet to move to address
         */
        template<typename type_t, typename u_type_t>
        requires(hud::is_move_constructible_v<type_t, u_type_t>)
        static constexpr type_t *construct_at(type_t *const address, u_type_t &&other) noexcept
        {
            static_assert(hud::is_nothrow_move_constructible_v<type_t, u_type_t>, "type_t(u_type_t&&) constructor is throwable.hud::memory::construct_at is not designed to allow throwable constructible type");
            return std::construct_at(address, hud::forward<u_type_t>(other));
        }

        /**
         * Call constructor of first count contiguously stored elements of type type_t.
         * @tparam type_t Type to construct
         * @param address The address of the type type_t to default construct
         * @param count Number of element to construct
         * @param args Arguments forward to the type_t constructor
         */
        template<typename type_t, typename... args_t>
        requires(hud::is_constructible_v<type_t, args_t...>)
        static constexpr void construct_array_at(type_t *HD_RESTRICT begin, const type_t *HD_RESTRICT const end, args_t &&...args) noexcept
        {
            static_assert(hud::is_nothrow_constructible_v<type_t, args_t...>, "type_t constructor is throwable.hud::memory::construct_array_at is not designed to allow throwable constructible type");
            while (begin < end)
            {
                hud::memory::template construct_at(begin++, hud::forward<args_t>(args)...);
            }
        }

        /**
         * Call default constructor of type type_t.
         * @tparam type_t Type to default construct
         * @param address The address of the type type_t to default construct
         */
        template<typename type_t>
        requires(hud::is_default_constructible_v<type_t>)
        static constexpr void default_construct(type_t *address) noexcept
        {
            static_assert(hud::is_nothrow_default_constructible_v<type_t>, "type_t default constructor is throwable.hud::memory::default_construct is not designed to allow throwable default constructible type");
            hud::memory::template construct_at(address);
        }

        /**
        Call default constructor of first count contiguously stored elements of type type_t
        @tparam type_t Type to default construct
        @param address Address of the first element to default construct
        @param count Number of element to construct
        */
        template<typename type_t>
        requires(hud::is_default_constructible_v<type_t>)
        static constexpr void default_construct_array(type_t *HD_RESTRICT begin, type_t *HD_RESTRICT end) noexcept
        {
            static_assert(hud::is_nothrow_default_constructible_v<type_t>, "type_t default constructor is throwable.hud::memory::default_construct is not designed to allow throwable default constructible type");
            if (!hud::is_constant_evaluated() && hud::is_trivially_default_constructible_v<type_t>)
            {
                hud::memory::set_zero(begin, (end - begin) * sizeof(type_t));
            }
            else
            {
                hud::memory::template construct_array_at(begin, end);
            }
        }

        /**
        Call destructor of type type_t ( Only for type with non trivial destructible type )
        @tparam type_t Type to default destroy
        @param address The address of the type type_t to destroy
        */
        template<typename type_t>
        requires(is_destructible_v<type_t> && !hud::is_pointer_v<type_t>)
        static constexpr void destroy(type_t &obj) noexcept
        {
            if constexpr (!hud::is_trivially_destructible_v<type_t>)
            {
                static_assert(hud::is_nothrow_destructible_v<type_t>, "type_t destructor is throwable.hud::memory::destroy is not designed to allow throwable destructible type");
                obj.~type_t();
            }
        }

        /**
         * Call destructor of first count contiguously stored elements of type type_t ( Only for type with non trivial destructible type ).
         * Do nothing if address is nullptr.
         * @tparam type_t Type to default destroy
         * @param address Address of the first element to destroy
         * @param count Number of element to destroy
         */
        template<typename type_t>
        requires(is_destructible_v<type_t>)
        static constexpr void destroy_array([[maybe_unused]] type_t *address, [[maybe_unused]] usize count) noexcept
        {
            if constexpr (!hud::is_trivially_destructible_v<type_t>)
            {
                while (count)
                {
                    hud::memory::template destroy(*address);
                    address++;
                    count--;
                }
            }
        }

        /**
         * Call copy constructor of first count contiguously stored elements of type type_t by passing corresponding element in array u_type_t has parameter if the type is not bitwise copy constructible.
         * If the type is bitwise copy constructible just copy dest_source to dest_addr.
         * @tparam type_t Type to construct
         * @tparam u_type_t Type of parameter passed to the constructor
         * @param destination Address of first the element to copy construct
         * @param source Address of fisrt the element to pass to destination
         * @param count Number of element to copy construct
         */
        template<typename type_t, typename u_type_t>
        requires(hud::is_copy_constructible_v<type_t, u_type_t>)
        static constexpr void copy_construct_array(type_t *destination, const u_type_t *source, usize count) noexcept
        {
            static_assert(hud::is_nothrow_copy_constructible_v<type_t, u_type_t>, "type_t(const u_type_t&) copy constructor is throwable.hud::memory::copy_construct_array is not designed to allow throwable copy constructible type");
            if (!hud::is_constant_evaluated() && hud::is_bitwise_copy_constructible_v<type_t, u_type_t>)
            {
                hud::memory::copy(destination, source, count * sizeof(type_t));
            }
            else
            {
                while (count)
                {
                    hud::memory::template construct_at(destination, *source);
                    destination++;
                    source++;
                    count--;
                }
            }
        }

        /**
         * Call move constructor of first count contiguously stored elements of type type_t by passing corresponding element in array u_type_t has parameter if the type is not bitwise move constructible.
         * If the type is bitwise move constructible just move dest_source to dest_addr by callinghud::memory::move
         * Support overlapped destination and source
         * @tparam type_t Type to construct
         * @tparam u_type_t Type of parameter forward to the constructor
         * @param destination Address of the first element to move construct
         * @param source Address of the first element to pass to destination
         * @param count Number of element to copy construct
         */
        template<typename type_t, typename u_type_t>
        requires(hud::is_move_constructible_v<type_t, u_type_t> || hud::is_copy_constructible_v<type_t, u_type_t>)
        static constexpr void move_or_copy_construct_array(type_t *destination, u_type_t *source, usize count) noexcept
        {
            if constexpr (hud::is_move_constructible_v<type_t, u_type_t>)
            {
                static_assert(hud::is_nothrow_move_constructible_v<type_t, u_type_t>, "type_t(const u_type_t&) constructor is throwable.hud::memory::construct_at is not designed to allow throwable constructible type");
            }
            if constexpr (hud::is_copy_constructible_v<type_t, u_type_t>)
            {
                static_assert(hud::is_nothrow_copy_constructible_v<type_t, u_type_t>, "type_t(u_type_t&&) constructor is throwable.hud::memory::construct_at is not designed to allow throwable constructible type");
            }

            if (!hud::is_constant_evaluated() && hud::is_bitwise_move_constructible_v<type_t, u_type_t>)
            {
                hud::memory::move(destination, source, count * sizeof(type_t));
            }
            else
            {
                while (count)
                {
                    hud::memory::template construct_at(destination, hud::move(*source));
                    destination++;
                    source++;
                    count--;
                }
            }
        }

        /**
         * Copy assign of first count contiguously stored elements of type u_type_t to first count contiguously stored elements of type type_t
         * @tparam type_t Type of the copied element
         * @tparam u_type_t Type to copy
         * @param destination Address of the first element to copy assign
         * @param source Address of the first element to copy assign
         * @param count Number of elements to copy assign
         */
        template<typename type_t, typename u_type_t>
        requires(hud::is_copy_assignable_v<type_t, u_type_t>)
        static constexpr void copy_assign_array(type_t *destination, const u_type_t *source, usize count) noexcept
        {
            if (hud::is_bitwise_copy_assignable_v<type_t, u_type_t> && !hud::is_constant_evaluated())
            {
                hud::memory::copy(destination, source, count * sizeof(type_t));
            }
            else
            {
                while (count)
                {
                    static_assert(hud::is_nothrow_copy_assignable_v<type_t, u_type_t>, "type_t operator=(const u_type_t&) copy assign is throwable.hud::memory::copy_assign is not designed to allow throwable copy assignable type");
                    *destination = *source;
                    destination++;
                    source++;
                    count--;
                }
            }
        }

        /**
         * Move or copy assign an element of type u_type_t to an element of type type_t
         * The move assignement is called if u_type_t is move assignable to type_t, else the copy assignement is called if u_type_t is copy assignable to type_t
         * @tparam type_t Type of the moved or copied element
         * @tparam u_type_t Type to move or copy
         * @param destination Address of the element that receive the move or copy assign
         * @param source Address of the element that is moved or copied
         */
        template<typename type_t, typename u_type_t>
        requires(hud::is_move_assignable_v<type_t, u_type_t> || hud::is_copy_assignable_v<type_t, u_type_t>)
        static constexpr void move_or_copy_assign(type_t *destination, u_type_t &&source) noexcept
        {
            if constexpr (hud::is_move_assignable_v<type_t, u_type_t>)
            {
                static_assert(hud::is_nothrow_move_assignable_v<type_t, u_type_t>, "type_t operator=(u_type_t&&) move assign is throwable. hud::memory::move_or_copy_assign is not designed to allow throwable move assignable type");
            }
            if constexpr (hud::is_copy_assignable_v<type_t, u_type_t>)
            {
                static_assert(hud::is_nothrow_copy_assignable_v<type_t, u_type_t>, "type_t operator=(const u_type_t&) copy assign is throwable. hud::memory::move_or_copy_assign is not designed to allow throwable copy assignable type");
            }
            *destination = hud::forward<u_type_t>(source);
        }

        /**
         * Move assign of first count contiguously stored elements of type u_type_t to first count contiguously stored elements of type type_t
         * Support overlapped destination and source
         * @tparam type_t Type of the moved element
         * @tparam u_type_t Type to move
         * @param destination Address of the first element to move assign
         * @param source Address of the first element to move assign
         * @param count Number of elements to move assign
         */
        template<typename type_t, typename u_type_t>
        requires(hud::is_move_assignable_v<type_t, u_type_t> || hud::is_copy_assignable_v<type_t, u_type_t>)
        static constexpr void move_or_copy_assign_array(type_t *destination, u_type_t *source, u_type_t const *const HD_RESTRICT end_source) noexcept
        {
            if constexpr (hud::is_move_assignable_v<type_t, u_type_t>)
            {
                static_assert(hud::is_nothrow_move_assignable_v<type_t, u_type_t>, "type_t operator=(u_type_t&&) move assign is throwable. hud::memory::move_or_copy_assign_array is not designed to allow throwable move assignable type");
            }
            if constexpr (hud::is_copy_assignable_v<type_t, u_type_t>)
            {
                static_assert(hud::is_nothrow_copy_assignable_v<type_t, u_type_t>, "type_t operator=(const u_type_t&) copy assign is throwable. hud::memory::move_or_copy_assign_array is not designed to allow throwable copy assignable type");
            }

            if (hud::is_bitwise_move_assignable_v<type_t, u_type_t> && hud::is_same_size_v<type_t, u_type_t> && !hud::is_constant_evaluated())
            {
                hud::memory::move(destination, source, (end_source - source) * sizeof(type_t));
            }
            else
            {
                while (source < end_source)
                {
                    *destination = hud::move(*source);
                    destination++;
                    source++;
                }
            }
        }

        /**
         * Move or copy construct an element of type u_type_t to a type type_t
         * The move constructor is called if u_type_t is a move constructible type to type_t, else the copy constructor is called if u_type_t is a copy constructible type to type_t.
         * After the construction, the destructor of source if called
         * @tparam type_t Type of the relocated element
         * @tparam u_type_t Type to relocate
         * @param destination Address of the element type_t
         * @param source Address of the element u_type_t to relocate to destination
         */
        template<typename type_t, typename u_type_t>
        requires(hud::is_move_constructible_v<type_t, u_type_t> || hud::is_copy_constructible_v<type_t, u_type_t>)
        static constexpr void move_or_copy_construct_then_destroy(type_t *destination, u_type_t &&source) noexcept
        {
            hud::memory::template construct_at(destination, hud::forward<u_type_t>(source));
            hud::memory::template destroy(source);
        }

        /**
         * Move or copy construct first @p count of contiguously stored elements of type u_type_t to to first \p count of contiguously stored elements of type type_t
         * The move constructor is called if u_type_t is a move constructible type to type_t, else the copy constructor is called if u_type_t is a copy constructible type to type_t.
         * If type_t is bitwise copy constructible the source is copied by simply copy the source memory to destination with hud::memory::copy.
         * This implies that source and destination are not overlapping.
         * After the construction of all elements, destructor's are called for count elements in the contiguously stored source
         * @tparam type_t Type of the relocated element
         * @tparam u_type_t Type to relocate
         * @param destination Address of the first element type_t
         * @param source Address of the first element u_type_t to relocate to destination
         * @param count Number of element to relocate
         */
        template<typename type_t, typename u_type_t>
        requires((hud::is_move_constructible_v<type_t, u_type_t> || hud::is_copy_constructible_v<type_t, u_type_t>) && is_destructible_v<u_type_t>)
        static constexpr void fast_move_or_copy_construct_array_then_destroy(type_t *destination, u_type_t *source, usize count) noexcept
        {
            // If the source is bitwise copyable and bitwise moveable to destination then we make a copy instead of a move semantic
            // This performs better that using move semantic that we do a memory move instead
            if (!hud::is_constant_evaluated() && hud::is_bitwise_copy_constructible_v<type_t, u_type_t>)
            {
                hud::memory::copy(destination, source, count * sizeof(type_t));
            }
            else
            {
                hud::memory::template move_or_copy_construct_array(destination, source, count);
            }
            hud::memory::template destroy_array(source, count);
        }

        /**
         * Move or copy construct first count of contiguously stored elements of type u_type_t to to first count of contiguously stored elements of type type_t in backward order
         * The move constructor is called if u_type_t is a move constructible type to type_t, else the copy constructor is called if u_type_t is a copy constructible type to type_t.
         * After the construction of all elements, destructor's are called for count elements in the contiguously stored source
         * This version is slightly faster than relocate_array if element u_type_t is bitwise move constructible to type_t by callinghud::memory::copy instead ofhud::memory::move
         * but this must be use carefully, it do not support overlapped buffer. If you want to support overlapped buffer, use relocate_array instead.
         * @tparam type_t Type of the relocated element
         * @tparam u_type_t Type to relocate
         * @param destination Address of the first element type_t
         * @param source Address of the first element u_type_t to relocate to destination
         * @param count Number of element to relocate
         */
        template<typename type_t, typename u_type_t>
        requires((hud::is_move_constructible_v<type_t, u_type_t> || hud::is_copy_constructible_v<type_t, u_type_t>) && is_destructible_v<u_type_t>)
        static constexpr void move_or_copy_construct_array_then_destroy_backward(type_t *destination, u_type_t *source, const usize count) noexcept
        {
            if (!hud::is_constant_evaluated() && hud::is_bitwise_move_constructible_v<type_t, u_type_t>)
            {
                hud::memory::move(destination, source, count * sizeof(u_type_t));
                hud::memory::template destroy_array(source, count);
            }
            else
            {
                type_t *last_destination = destination + count;
                u_type_t *last_source = source + count;
                while (last_source > source)
                {
                    last_destination--;
                    last_source--;
                    hud::memory::template move_or_copy_construct_then_destroy(last_destination, hud::move(*last_source));
                }
            }
        }

        /**
         * Compare an element of type lhs_t with an element of type rhs_t
         * @tparam lhs_t Type of the left element to compare
         * @tparam rhs_t Type of the right element to compare
         * @param left Address of the left element to compare
         * @param right Address of the right element to compare
         * @return true if both elements are the equal, false otherwise
         */
        template<typename lhs_t, typename rhs_t>
        static HD_FORCEINLINE bool equal(const lhs_t *left, const rhs_t *right) noexcept
        {
            static_assert(hud::is_comparable_with_equal_v<lhs_t, rhs_t>, "Types lhs_t and rhs_t are not comparable");
            return *left == *right;
        }

        /**
         * Compare first count contiguously stored elements of type lhs_t with first count contiguously stored elements of type rhs_t
         * @tparam lhs_t Type of the left element to compare
         * @tparam rhs_t Type of the right element to compare
         * @param left Address of the first left element to compare
         * @param right Address of the first right element to compare
         * @param count Number of element to compare
         * @return true if all elements are the equal, false otherwise
         */
        template<typename lhs_t, typename rhs_t>
        static HD_FORCEINLINE bool equal_array(const lhs_t *left, const rhs_t *right, usize count) noexcept
        {
            if constexpr (hud::is_bitwise_comparable_v<lhs_t, rhs_t> && hud::is_same_size_v<lhs_t, rhs_t>)
            {
                return hud::memory::compare_equal(left, right, count * sizeof(lhs_t));
            }
            else
            {
                static_assert(hud::is_comparable_with_equal_v<lhs_t, rhs_t>, "Types lhs_t and rhs_t are not comparable");

                while (count)
                {
                    if (!hud::memory::template equal(left, right))
                    {
                        return false;
                    }
                    left++;
                    right++;
                    count--;
                }
                return true;
            }
        }

        /**
         * Compare an element of type lhs_t with an element of type rhs_t
         * @tparam lhs_t Type of the left element to compare
         * @tparam rhs_t Type of the right element to compare
         * @param left Address of the left element to compare
         * @param right Address of the right element to compare
         * @return true if elements are not the equal, false otherwise
         */
        template<typename lhs_t, typename rhs_t>
        static HD_FORCEINLINE bool not_equal(const lhs_t *left, const rhs_t *right) noexcept
        {
            static_assert(hud::is_comparable_with_not_equal_v<lhs_t, rhs_t>, "Types lhs_t and rhs_t are not comparable");
            return *left != *right;
        }

        /**
         * Compare first count contiguously stored elements of type lhs_t with first count contiguously stored elements of type rhs_t
         * @tparam lhs_t Type of the left element to compare
         * @tparam rhs_t Type of the right element to compare
         * @param left Address of the first left element to compare
         * @param right Address of the first right element to compare
         * @param count Number of element to compare
         * @return true if at least one element is not equal, false otherwise
         */
        template<typename lhs_t, typename rhs_t>
        static HD_FORCEINLINE bool not_equal_array(const lhs_t *left, const rhs_t *right, usize count) noexcept
        {
            if constexpr (hud::is_bitwise_comparable_v<lhs_t, rhs_t> && hud::is_same_size_v<lhs_t, rhs_t>)
            {
                return !hud::memory::compare_equal(left, right, count * sizeof(lhs_t));
            }
            else
            {
                static_assert(hud::is_comparable_with_not_equal_v<lhs_t, rhs_t>, "Types lhs_t and rhs_t are not comparable");

                while (count)
                {
                    if (hud::memory::template not_equal(left, right))
                    {
                        return true;
                    }
                    left++;
                    right++;
                    count--;
                }
                return false;
            }
        }
    };

} // namespace hud

#endif // HD_INC_OSABSTRACTIONLAYER_MEMORY_H