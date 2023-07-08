#ifndef HD_INC_CORE_TEMPLATES_FORWARD_H
#define HD_INC_CORE_TEMPLATES_FORWARD_H
#include "../traits/remove_reference.h"
#include "../traits/negation.h"
#include "../traits/is_reference.h"

namespace hud
{

    /**
     * If object is an lvalue reference, the function returns object without modifying its type.
     * This is a helper function to allow perfect forwarding of arguments taken as rvalue references to deduced types, preserving any potential move semantics involved.
     * The need for this function stems from the fact that all named values (such as function parameters) always evaluate as lvalues (even those declared as rvalue references),
     * and this poses difficulties in preserving potential move semantics on template functions that forward arguments to other functions.
     * @tparam type_t The type of the object to forward
     * @param type The object to forward
     * @return An rvalue reference to object if object is not an lvalue reference.
     */
    template<typename type_t>
    constexpr type_t &&forward(hud::remove_reference_t<type_t> &object) noexcept
    {
        return static_cast<type_t &&>(object);
    }

    /**
     * If object is an lvalue reference, the function returns object without modifying its type.
     * This is a helper function to allow perfect forwarding of arguments taken as rvalue references to deduced types, preserving any potential move semantics involved.
     * The need for this function stems from the fact that all named values (such as function parameters) always evaluate as lvalues (even those declared as rvalue references),
     * and this poses difficulties in preserving potential move semantics on template functions that forward arguments to other functions.
     * @tparam type_t The type of the object to forward
     * @param type The object to forward
     * @return An rvalue reference to object if object is not an lvalue reference.
     */
    template<typename type_t>
    constexpr type_t &&forward(hud::remove_reference_t<type_t> &&object) noexcept
    {
        static_assert(negation_v<is_lvalue_reference<type_t>>, "can not forward an rvalue as an lvalue");
        return static_cast<type_t &&>(object);
    }

} // namespace hud

#endif // HD_INC_CORE_TEMPLATES_FORWARD_H
