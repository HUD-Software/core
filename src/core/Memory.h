#pragma once
#ifndef HD_INC_OSABSTRACTIONLAYER_MEMORY_H
#define HD_INC_OSABSTRACTIONLAYER_MEMORY_H
#include "Traits/IsConstructible.h"
#include "Traits/IsNothrowConstructible.h"
#include "Traits/IsDefaultConstructible.h"
#include "Traits/IsTriviallyDefaultConstructible.h"
#include "Traits/IsNothrowDefaultConstructible.h"
#include "Traits/IsDestructible.h"
#include "Traits/IsTriviallyDestructible.h"
#include "Traits/IsNothrowDestructible.h"
#include "Traits/IsCopyConstructible.h"
#include "Traits/IsBitwiseCopyConstructible.h"
#include "Traits/IsNothrowCopyConstructible.h"
#include "Traits/IsMoveConstructible.h"
#include "Traits/IsBitwiseMoveConstructible.h"
#include "Traits/IsNothrowMoveConstructible.h"
#include "Traits/IsCopyAssignable.h"
#include "Traits/IsBitwiseCopyAssignable.h"
#include "Traits/IsNothrowCopyAssignable.h"
#include "Traits/IsMoveAssignable.h"
#include "Traits/IsBitwiseMoveAssignable.h"
#include "Traits/IsNothrowMoveAssignable.h"
#include "Traits/IsBitwiseComparable.h"
#include "Traits/IsComparableWithEqual.h"
#include "Traits/IsComparableWithNotEqual.h"
#include "Traits/IsPointer.h"

#include "Templates/Move.h"
#include "Templates/Forward.h"

#include <memory> // std::construct_at

#if defined(HD_OS_WINDOWS)
#include "OSWindows/WindowsMemory.h"
#elif defined(HD_OS_LINUX)
#include "OSLinux/LinuxMemory.h"
#else
#error Targeted OS not supported
#endif

namespace hud {

    /** Provides Memory operations. */
    struct Memory :
#if defined(HD_OS_WINDOWS)
        OS::Windows::Memory
#elif defined(HD_OS_LINUX)
        OS::Linux::Memory
#endif
    {
        /**
        * Call constructor of type.
        * @tparam T Type to default construct
        * @param address The address of the type T to default construct
        */
        template<typename T, typename... TArgs>
        static constexpr T* construct_at(T* const address, TArgs&&... args) noexcept requires(IsConstructibleV<T, TArgs... >) {
            static_assert(IsNothrowConstructibleV<T, TArgs...>, "T constructor is throwable. Memory::construct_at is not designed to allow throwable constructible type");
            return std::construct_at(address, forward<TArgs>(args)...);
        }

        /**
        * Call copy constructor of type of U is copy constructible type to T.
        * @tparam T Type to copy construct
        * @tparam U Type to copy
        * @param address The address of the type T to copy construct
        * @param other The objet to copy to address
        */
        template<typename T, typename U>
        static constexpr T* construct_at(T* const address, const U& other) noexcept requires(IsCopyConstructibleV<T, U>) {
            static_assert(IsNothrowCopyConstructibleV<T, U>, "T(const U&) constructor is throwable. Memory::construct_at is not designed to allow throwable constructible type");
            return std::construct_at(address, other);
        }

        /**
        * Call move constructor of type of U is move constructible type to T.
        * @tparam T Type to move construct
        * @tparam U Type to move
        * @param address The address of the type T to move construct
        * @param other The objet to move to address
        */
        template<typename T, typename U>
        static constexpr T* construct_at(T* const address, U&& other) noexcept requires(IsMoveConstructibleV<T, U>) {
            static_assert(IsNothrowMoveConstructibleV<T, U>, "T(U&&) constructor is throwable. Memory::construct_at is not designed to allow throwable constructible type");
            return std::construct_at(address, forward<U>(other));
        }

        /**
        * Call constructor of first count contiguously stored elements of type T.
        * @tparam T Type to construct
        * @param address The address of the type T to default construct
        * @param count Number of element to construct
        * @param args Arguments forward to the T constructor
        */
        template<typename T, typename... TArgs>
        static constexpr void construct_array_at(T*HD_RESTRICT begin, const T* HD_RESTRICT const end, TArgs&&... args) noexcept requires(IsConstructibleV<T, TArgs... >) {
            static_assert(IsNothrowConstructibleV<T, TArgs...>, "T constructor is throwable. Memory::construct is not designed to allow throwable constructible type");
            while(begin < end) {
                Memory::template construct_at(begin++, forward<TArgs>(args)...);
            }
        }

        /**
        * Call default constructor of type T.
        * @tparam T Type to default construct
        * @param address The address of the type T to default construct
        */
        template<typename T>
        static constexpr void default_construct(T* address) noexcept requires(IsDefaultConstructibleV<T>) {
            static_assert(IsNothrowDefaultConstructibleV<T>, "T default constructor is throwable. Memory::default_construct is not designed to allow throwable default constructible type");
            Memory::template construct_at(address);
        }

        /**
        Call default constructor of first count contiguously stored elements of type T
        @tparam T Type to default construct
        @param address Address of the first element to default construct
        @param count Number of element to construct
        */
        template<typename T>
        static constexpr void default_construct_array(T* HD_RESTRICT begin, T* HD_RESTRICT end) noexcept requires(IsDefaultConstructibleV<T>) {
            static_assert(IsNothrowDefaultConstructibleV<T>, "T default constructor is throwable. Memory::default_construct is not designed to allow throwable default constructible type");
            if (!is_constant_evaluated() && IsTriviallyDefaultConstructibleV<T>) {
                Memory::set_zero(begin, (end - begin)*sizeof(T));
            }
            else {
                Memory::template construct_array_at(begin, end);
            }
        }

        /**
        Call destructor of type T ( Only for type with non trivial destructible type )
        @tparam T Type to default destroy
        @param address The address of the type T to destroy
        */
        template<typename T>
        static constexpr void destroy(T& obj) noexcept requires(IsDestructibleV<T> && !IsPointerV<T>) {
            if constexpr (!IsTriviallyDestructibleV<T>) {
                static_assert(IsNothrowDestructibleV<T>, "T destructor is throwable. Memory::destroy is not designed to allow throwable destructible type");
                obj.~T();
            }
        }

        /**
        Call destructor of first count contiguously stored elements of type T ( Only for type with non trivial destructible type )
        @tparam T Type to default destroy
        @param address Address of the first element to destroy
        @param count Number of element to destroy
        */
        template<typename T>
        static constexpr void destroy_array([[maybe_unused]] T* address, [[maybe_unused]] usize count) noexcept requires(IsDestructibleV<T>) {
            if constexpr (!IsTriviallyDestructibleV<T>) {
                while (count) {
                    Memory::template destroy(*address);
                    address++;
                    count--;
                }
            }
        }

        /**
        * Call copy constructor of first count contiguously stored elements of type T by passing corresponding element in array U has parameter if the type is not bitwise copy constructible.
        * If the type is bitwise copy constructible just copy dest_source to dest_addr.
        * @tparam T Type to construct
        * @tparam U Type of parameter passed to the constructor
        * @param destination Address of first the element to copy construct
        * @param source Address of fisrt the element to pass to destination
        * @param count Number of element to copy construct
        */
        template<typename T, typename U>
        static constexpr void copy_construct_array(T* destination, const U* source, usize count) noexcept requires(IsCopyConstructibleV<T, U>) {
            static_assert(IsNothrowCopyConstructibleV<T, U>, "T(const U&) copy constructor is throwable. Memory::copy_construct_array is not designed to allow throwable copy constructible type");
            if (!is_constant_evaluated() && IsBitwiseCopyConstructibleV<T, U>) {
                Memory::copy(destination, source, count * sizeof(T));
            }
            else {
                while (count) {
                    Memory::template construct_at(destination, *source);
                    destination++;
                    source++;
                    count--;
                }
            }
        }

        /**
        * Call move constructor of first count contiguously stored elements of type T by passing corresponding element in array U has parameter if the type is not bitwise move constructible.
        * If the type is bitwise move constructible just move dest_source to dest_addr by calling Memory::move
        * Support overlapped destination and source
        * @tparam T Type to construct
        * @tparam U Type of parameter forward to the constructor
        * @param destination Address of the first element to move construct
        * @param source Address of the first element to pass to destination
        * @param count Number of element to copy construct
        */
        template<typename T, typename U>
        static constexpr void move_or_copy_construct_array(T* destination, U* source, usize count) noexcept requires(IsMoveConstructibleV<T, U> || IsCopyConstructibleV<T, U>) {
            if constexpr (IsMoveConstructibleV<T, U>) {
                static_assert(IsNothrowMoveConstructibleV<T, U>, "T(const U&) constructor is throwable. Memory::construct_at is not designed to allow throwable constructible type");
            }
            if constexpr (IsCopyConstructibleV<T, U>) {
                static_assert(IsNothrowCopyConstructibleV<T, U>, "T(U&&) constructor is throwable. Memory::construct_at is not designed to allow throwable constructible type");
            }

            if (!is_constant_evaluated() && IsBitwiseMoveConstructibleV<T, U>) {
                Memory::move(destination, source, count * sizeof(T));
            }
            else {
                while (count) {
                    Memory::template construct_at(destination, hud::move(*source));
                    destination++;
                    source++;
                    count--;
                }
            }
        }

        /**
        * Copy assign of first count contiguously stored elements of type U to first count contiguously stored elements of type T
        * @tparam T Type of the copied element
        * @tparam U Type to copy
        * @param destination Address of the first element to copy assign
        * @param source Address of the first element to copy assign
        * @param count Number of elements to copy assign
        */
        template<typename T, typename U>
        static constexpr void copy_assign_array(T* destination, const U* source, usize count) noexcept requires(IsCopyAssignableV<T, U>) {
            if constexpr (IsBitwiseCopyAssignableV<T, U> && !is_constant_evaluated()) {
                Memory::copy(destination, source, count * sizeof(T));
            }
            else {
                while (count) {
                    static_assert(IsNothrowCopyAssignableV<T, U>, "T operator=(const U&) copy assign is throwable. Memory::copy_assign is not designed to allow throwable copy assignable type");
                    *destination = *source;
                    destination++;
                    source++;
                    count--;
                }
            }
        }

        /**
        * Move or copy assign an element of type U to an element of type T
        * The move assignement is called if U is move assignable to T, else the copy assignement is called if U is copy assignable to T
        * @tparam T Type of the moved or copied element
        * @tparam U Type to move or copy
        * @param destination Address of the element that receive the move or copy assign
        * @param source Address of the element that is moved or copied
        */
        template<typename T, typename U>
        static constexpr void move_or_copy_assign(T* destination, U&& source) noexcept requires(IsMoveAssignableV<T, U> || IsCopyAssignableV<T, U>) {
            if constexpr (IsMoveAssignableV<T, U>) {
                static_assert(IsNothrowMoveAssignableV<T, U>, "T operator=(U&&) move assign is throwable. Memory::move_or_copy_assign is not designed to allow throwable move assignable type");
            }
            if constexpr (IsCopyAssignableV<T, U>) {
                static_assert(IsNothrowCopyAssignableV<T, U>, "T operator=(const U&) copy assign is throwable. Memory::move_or_copy_assign is not designed to allow throwable copy assignable type");
            }
            *destination = hud::forward<U>(source);
        }



        /**
        * Move assign of first count contiguously stored elements of type U to first count contiguously stored elements of type T
        * Support overlapped destination and source
        * @tparam T Type of the moved element
        * @tparam U Type to move
        * @param destination Address of the first element to move assign
        * @param source Address of the first element to move assign
        * @param count Number of elements to move assign
        */
        template<typename T, typename U>
        static constexpr void move_or_copy_assign_array(T* destination, U* HD_RESTRICT source, U const * const HD_RESTRICT end_source) noexcept requires(IsMoveAssignableV<T, U> || IsCopyAssignableV<T, U>) {
            if constexpr (IsMoveAssignableV<T, U>) {
                static_assert(IsNothrowMoveAssignableV<T, U>, "T operator=(U&&) move assign is throwable. Memory::move_or_copy_assign is not designed to allow throwable move assignable type");
            }
            if constexpr (IsCopyAssignableV<T, U>) {
                static_assert(IsNothrowCopyAssignableV<T, U>, "T operator=(const U&) copy assign is throwable. Memory::move_or_copy_assign is not designed to allow throwable copy assignable type");
            }

            if constexpr (IsBitwiseMoveAssignableV<T, U> && IsSameSizeV<T,U> &&!is_constant_evaluated()) {
                Memory::move(destination, source, (end_source-source) * sizeof(T));
            }
            else {
                while (source < end_source) {
                    *destination = hud::move(*source);
                    destination++;
                    source++;
                }
            }
        }

        /**
        * Move or copy construct an element of type U to a type T
        * The move constructor is called if U is a move constructible type to T, else the copy constructor is called if U is a copy constructible type to T.
        * After the construction, the destructor of source if called
        * @tparam T Type of the relocated element
        * @tparam U Type to relocate
        * @param destination Address of the element T
        * @param source Address of the element U to relocate to destination
        */
        template<typename T, typename U>
        static constexpr void move_or_copy_construct_then_destroy(T* destination, U&& source) noexcept requires(IsMoveConstructibleV<T, U> || IsCopyConstructibleV<T, U>) {
            Memory::template construct_at(destination, hud::forward<U>(source));
            Memory::template destroy(source);
        }

        /**
        * Move or copy construct first count of contiguously stored elements of type U to to first count of contiguously stored elements of type T
        * The move constructor is called if U is a move constructible type to T, else the copy constructor is called if U is a copy constructible type to T.
        * If T is bitwise copy constructible the source is copied by simply copy the source memory to destination with Memory::copy.
        * This implies that source and destination are not overlapping.
        * After the construction of all elements, destructor's are called for count elements in the contiguously stored source
        * @tparam T Type of the relocated element
        * @tparam U Type to relocate
        * @param destination Address of the first element T
        * @param source Address of the first element U to relocate to destination
        * @param count Number of element to relocate
        */
        template<typename T, typename U>
        static constexpr void fast_move_or_copy_construct_array_then_destroy(T* destination, U* source, usize count) noexcept requires((IsMoveConstructibleV<T, U> || IsCopyConstructibleV<T, U>) && IsDestructibleV<U>) {
            // If the source is bitwise copyable and bitwise moveable to destination then we make a copy instead of a move semantic
            // This performs better that using move semantic that we do a memory move instead
            if (!is_constant_evaluated() && IsBitwiseCopyConstructibleV<T, U>) {
                Memory::copy(destination, source, count * sizeof(T));
            }
            else {
                Memory::template move_or_copy_construct_array(destination, source, count);
            }
            Memory::template destroy_array(source, count);
        }

        /**
        * Move or copy construct first count of contiguously stored elements of type U to to first count of contiguously stored elements of type T in backward order
        * The move constructor is called if U is a move constructible type to T, else the copy constructor is called if U is a copy constructible type to T.
        * After the construction of all elements, destructor's are called for count elements in the contiguously stored source
        * This version is slightly faster than relocate_array if element U is bitwise move constructible to T by calling Memory::copy instead of Memory::move
        * but this must be use carefully, it do not support overlapped buffer. If you want to support overlapped buffer, use relocate_array instead.
        * @tparam T Type of the relocated element
        * @tparam U Type to relocate
        * @param destination Address of the first element T
        * @param source Address of the first element U to relocate to destination
        * @param count Number of element to relocate
        */
        template<typename T, typename U>
        static constexpr void move_or_copy_construct_array_then_destroy_backward(T* destination, U* source, const usize count) noexcept requires((IsMoveConstructibleV<T, U> || IsCopyConstructibleV<T, U>) && IsDestructibleV<U>) {
            if (!is_constant_evaluated() && IsBitwiseMoveConstructibleV<T, U>) {
                Memory::move(destination, source, count*sizeof(U));
                Memory::template destroy_array(source, count);
            }
            else {
                T* last_destination = destination + count;
                U* last_source = source + count;
                while (last_source > source) {
                    last_destination--;
                    last_source--;
                    Memory::template move_or_copy_construct_then_destroy(last_destination, hud::move(*last_source));
                }
            }
        }

        /**
        * Compare an element of type T with an element of type U
        * @tparam T Type of the left element to compare
        * @tparam U Type of the right element to compare
        * @param left Address of the left element to compare
        * @param right Address of the right element to compare
        * @return true if both elements are the equal, false otherwise
        */
        template<typename T, typename U>
        static HD_FORCEINLINE bool equal(const T* left, const U* right) noexcept {
            static_assert(IsComparableWithEqualV<T, U>, "Types T and U are not comparable");
            return *left == *right;
        }

        /**
        * Compare first count contiguously stored elements of type T with first count contiguously stored elements of type U
        * @tparam T Type of the left element to compare
        * @tparam U Type of the right element to compare
        * @param left Address of the first left element to compare
        * @param right Address of the first right element to compare
        * @param count Number of element to compare
        * @return true if all elements are the equal, false otherwise
        */
        template<typename T, typename U>
        static HD_FORCEINLINE bool equal_array(const T* left, const U* right, usize count) noexcept {
            if constexpr (IsBitwiseComparableV<T, U> && IsSameSizeV<T, U>) {
                return Memory::compare_equal(left, right, count * sizeof(T));
            }
            else {
                static_assert(IsComparableWithEqualV<T, U>, "Types T and U are not comparable");

                while (count) {
                    if (!Memory::template equal(left, right)) {
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
        * Compare an element of type T with an element of type U
        * @tparam T Type of the left element to compare
        * @tparam U Type of the right element to compare
        * @param left Address of the left element to compare
        * @param right Address of the right element to compare
        * @return true if elements are not the equal, false otherwise
        */
        template<typename T, typename U>
        static HD_FORCEINLINE bool not_equal(const T* left, const U* right) noexcept {
            static_assert(IsComparableWithNotEqualV<T, U>, "Types T and U are not comparable");
            return *left != *right;
        }

        /**
        * Compare first count contiguously stored elements of type T with first count contiguously stored elements of type U
        * @tparam T Type of the left element to compare
        * @tparam U Type of the right element to compare
        * @param left Address of the first left element to compare
        * @param right Address of the first right element to compare
        * @param count Number of element to compare
        * @return true if at least one element is not equal, false otherwise
        */
        template<typename T, typename U>
        static HD_FORCEINLINE bool not_equal_array(const T* left, const U* right, usize count) noexcept {
            if constexpr (IsBitwiseComparableV<T, U> && IsSameSizeV<T,U>) {
                return !Memory::compare_equal(left, right, count * sizeof(T));
            }
            else {
                static_assert(IsComparableWithNotEqualV<T, U>, "Types T and U are not comparable");

                while (count) {
                    if (Memory::template not_equal(left, right)) {
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