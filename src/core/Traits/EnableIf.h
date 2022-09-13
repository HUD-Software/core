#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_ENABLE_IF_H
#define HD_INC_OSLAYER_TRAITS_ENABLE_IF_H

namespace hud {

    /**
    * If enabled is true, EnableIf has a public member typedef Type, equal to T; otherwise, there is no member typedef.
    * This metafunction is a convenient way to leverage SFINAE to conditionally remove functions from overload resolution
    * based on type traits and to provide separate function overloads and specializations for different type traits.
    * EnableIf can be used as:
    * - An additional function argument (not applicable to operator overloads),
    * - A return type (not applicable to constructors and destructors),
    * - A class template or function template parameter
    */
    template<bool enabled, typename T = void>
    struct EnableIf
    {};

    template<typename T>
    struct EnableIf<true, T> {
        using Type = T;
    };

    /** Equivalent of typename EnableIf<enabled, T>::Type. */
    template< bool enabled, typename T = void >
    using EnableIfT = typename EnableIf<enabled, T>::Type;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_ENABLE_IF_H