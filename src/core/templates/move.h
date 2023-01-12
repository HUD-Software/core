#pragma once
#ifndef HD_INC_CORE_TEMPLATES_MOVE_H
#define HD_INC_CORE_TEMPLATES_MOVE_H
#include "../traits/remove_reference.h"

namespace hud
{

    /**
     * This is a helper function to force move semantics on values, even if they have a name: Directly using the returned value causes object to be considered an rvalue.
     * Generally, rvalues are values whose address cannot be obtained by dereferencing them, either because they are literals or because they are temporary in nature (such as values returned by functions or explicit constructor calls).
     * By passing an object to this function, an rvalue that refers to it is obtained.
     * @tparam type_t The type of the object to move
     * @param object The object to move
     * @return An rvalue reference to object.
     */
    template <typename type_t>
    constexpr hud::remove_reference_t<type_t> &&move(type_t &&object) noexcept
    {
        return static_cast<hud::remove_reference_t<type_t> &&>(object);
    }

} // namespace hud

#endif // HD_INC_CORE_TEMPLATES_MOVE_H