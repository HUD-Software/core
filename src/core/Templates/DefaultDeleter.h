#pragma once
#ifndef HD_INC_OSLAYER_TEMPLATES_DEFAULT_DELETE_H
#define HD_INC_OSLAYER_TEMPLATES_DEFAULT_DELETE_H
#include "../Traits/EnableIf.h"
#include "../Traits/IsConvertible.h"
#include "../Traits/IsVoid.h"
#include "../Memory.h"

namespace hud {

    /** Function object class, whose function-like invokation takes an object of type T* and deletes it with delete operator. */
    template< typename T >
    struct DefaultDeleter
    {
        /** The pointer type to delete. */
        using PointerType = T*;

        /** Default constructor. */
        constexpr DefaultDeleter() noexcept = default;

        /**  Copy construct from another type. */
        template<typename U>
        constexpr DefaultDeleter(const DefaultDeleter<U>&) noexcept requires(hud::IsConvertibleV<U*, T*>) {
        }

        /** Delete the raw pointer using delete. */
        constexpr void operator()(T* ptr) const noexcept {

            // Ensure we don't have an incomplete type
            static_assert(0 < sizeof(T), "can't delete an incomplete type");

            delete ptr;
        }
    };


    /** Function object class, whose function-like invokation takes an object of type T* and deletes it with delete[] operator. */
    template< typename T >
    struct DefaultDeleter<T[]>
    {
        /** The pointer type to delete. */
        using PointerType = T*;

        /** Default constructor */
        constexpr DefaultDeleter() noexcept = default;

        /**  Copy construct from another type */
        template<typename U>
        constexpr DefaultDeleter(const DefaultDeleter<U[]>&) noexcept requires(hud::IsConvertibleV<U(*)[], T(*)[]>){
        }

        /** Delete the raw pointer using delete[] */
        template<typename U>
        constexpr void operator()(U* ptr) const noexcept requires(hud::IsConvertibleV<U(*)[], T(*)[]>) {

            // Ensure we don't have an incomplete type
            static_assert(0 < sizeof(T), "can't delete an incomplete type");

            delete[] ptr;
        }
    };

    /** Function object class, whose function-like invokation takes a pointer and frees it hud::memory:free */
    struct DefaultFree
    {
        /** Default constructor */
        constexpr DefaultFree() noexcept = default;

        /** Free the raw pointer using hud::Memory::free() */
        void operator()(void* ptr) const noexcept {
            Memory::free(ptr);
        }
    };

} // namespace hud

#endif // HD_INC_OSLAYER_TEMPLATES_DEFAULT_DELETE_H
