#pragma once
#ifndef HD_INC_OSLAYER_TEMPLATES_FORWARD_H
#define HD_INC_OSLAYER_TEMPLATES_FORWARD_H
#include "../Traits/RemoveReference.h"
#include "../Traits/Not.h"
#include "../Traits/IsReference.h"

namespace hud {

    /**
    * If object is an lvalue reference, the function returns object without modifying its type.
    * This is a helper function to allow perfect forwarding of arguments taken as rvalue references to deduced types, preserving any potential move semantics involved.
    * The need for this function stems from the fact that all named values (such as function parameters) always evaluate as lvalues (even those declared as rvalue references),
    * and this poses difficulties in preserving potential move semantics on template functions that forward arguments to other functions.
    * @tparam T The type of the object to forward
    * @param type The object to forward
    * @return An rvalue reference to object if object is not an lvalue reference.
    */
    template<typename T>
    constexpr T && forward(RemoveReferenceT<T> & object) noexcept {
        return static_cast<T&&>(object);
    }

    /**
    * If object is an lvalue reference, the function returns object without modifying its type.
    * This is a helper function to allow perfect forwarding of arguments taken as rvalue references to deduced types, preserving any potential move semantics involved.
    * The need for this function stems from the fact that all named values (such as function parameters) always evaluate as lvalues (even those declared as rvalue references),
    * and this poses difficulties in preserving potential move semantics on template functions that forward arguments to other functions.
    * @tparam T The type of the object to forward
    * @param type The object to forward
    * @return An rvalue reference to object if object is not an lvalue reference.
    */
    template<typename T>
    constexpr T && forward(RemoveReferenceT<T> && object) noexcept {
        static_assert(NotV< IsLValueReference<T>>, "can not forward an rvalue as an lvalue");
        return static_cast<T&&>(object);
    }

} // namespace hud

#endif // HD_INC_OSLAYER_TEMPLATES_FORWARD_H
