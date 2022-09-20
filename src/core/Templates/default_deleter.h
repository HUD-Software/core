#pragma once
#ifndef HD_INC_CORE_TEMPLATES_DEFAULT_DELETE_H
#define HD_INC_CORE_TEMPLATES_DEFAULT_DELETE_H
#include "../traits/enable_if.h"
#include "../traits/is_convertible.h"
#include "../traits/is_void.h"
#include "../memory.h"

namespace hud {

    /** Function object class, whose function-like invokation takes an object of type type_t* and deletes it with delete operator. */
    template< typename type_t >
    struct default_deleter
    {
        /** The pointer type to delete. */
        using pointer_type = type_t*;

        /** Default constructor. */
        constexpr default_deleter() noexcept = default;

        /**  Copy construct from another type. */
        template<typename u_type_t>
        constexpr default_deleter(const hud::default_deleter<u_type_t>&) noexcept requires(hud::is_convertible_v<u_type_t*, type_t*>) {
        }

        /** Delete the raw pointer using delete. */
        constexpr void operator()(type_t* ptr) const noexcept {

            // Ensure we don't have an incomplete type
            static_assert(0 < sizeof(type_t), "can't delete an incomplete type");

            delete ptr;
        }
    };


    /** Function object class, whose function-like invokation takes an object of type type_t* and deletes it with delete[] operator. */
    template< typename type_t >
    struct default_deleter<type_t[]>
    {
        /** The pointer type to delete. */
        using pointer_type = type_t*;

        /** Default constructor */
        constexpr default_deleter() noexcept = default;

        /**  Copy construct from another type */
        template<typename u_type_t>
        constexpr default_deleter(const hud::default_deleter<u_type_t[]>&) noexcept requires(hud::is_convertible_v<u_type_t(*)[], type_t(*)[]>){
        }

        /** Delete the raw pointer using delete[] */
        template<typename u_type_t>
        constexpr void operator()(u_type_t* ptr) const noexcept requires(hud::is_convertible_v<u_type_t(*)[], type_t(*)[]>) {

            // Ensure we don't have an incomplete type
            static_assert(0 < sizeof(type_t), "can't delete an incomplete type");

            delete[] ptr;
        }
    };

    /** Function object class, whose function-like invokation takes a pointer and frees it hud::memory:free */
    struct default_free
    {
        /** Default constructor */
        constexpr default_free() noexcept = default;

        /** Free the raw pointer using hud::memory::free() */
        void operator()(void* ptr) const noexcept {
           hud::memory::free(ptr);
        }
    };

} // namespace hud

#endif // HD_INC_CORE_TEMPLATES_DEFAULT_DELETE_H
