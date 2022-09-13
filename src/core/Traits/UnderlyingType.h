#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_UNDERLYING_TYPE_H
#define HD_INC_OSLAYER_TRAITS_UNDERLYING_TYPE_H

namespace hud {

    /*
    * Retrieves the underlying type of enum type T without const volatile qualifiers.
    * The underlying type of an enum declared with enum class is int unless a different type is specified on declaration.
    */
    template<typename T>
    struct UnderlyingType {
        using Type = __underlying_type(T);
    };

    /* Equivalent of typename UnderlyingType<T>::Type. */
    template<typename T>
    using UnderlyingTypeT = typename UnderlyingType<T>::Type;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_UNDERLYING_TYPE_H