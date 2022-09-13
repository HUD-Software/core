#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_IS_EXPLICITLY_CONSTRUCTIBLE_H
#define HD_INC_OSLAYER_TRAITS_IS_EXPLICITLY_CONSTRUCTIBLE_H
#include <OSLayer/Traits/IsConstructible.h>
#include <OSLayer/Traits/IsConvertible.h>
#include <OSLayer/Traits/VoidT.h>
#include <OSLayer/Templates/Declval.h>

namespace hud {

    namespace details {
        template <typename T>
        constexpr void ImplicitConstruct(const T&) noexcept;

        template<typename Void, bool is_constructible, typename T, typename ...TArgs>
        struct IsExplicitlyConstructibleImpl2
            : public BoolConstant<is_constructible> {
        };

        template<typename T, typename ...TArgs>
        struct IsExplicitlyConstructibleImpl2 < hud::VoidT<decltype(details::ImplicitConstruct<T>({ hud::declval<TArgs>()... })) > , true, T, TArgs... >
            : public FalseType {
        };

        template <typename T, typename... TArgs>
        struct IsExplicitlyConstructibleImpl
            : IsExplicitlyConstructibleImpl2<void, IsConstructibleV<T, TArgs...>, T, TArgs...> {
        };

        template <typename T>
        struct IsExplicitlyConstructibleImpl<T, const T&>
            : BoolConstant<!IsConvertibleV<const T&, T>> {
        };

        template <typename T>
        struct IsExplicitlyConstructibleImpl<T, T&&>
            : BoolConstant<!IsConvertibleV<T&&, T>> {
        };

    } // namespace hud::details

    /** Checks whether T is an explicitly constructible type with TArgs. */
    template< typename T, typename... TArgs>
    struct IsExplicitlyConstructible
        : details::IsExplicitlyConstructibleImpl < T, TArgs... > {
    };

    /** Equivalent of IsExplicitlyConstructible<T, TArgs...>::Value. */
    template<typename T, typename ...TArgs >
    inline constexpr bool IsExplicitlyConstructibleV = IsExplicitlyConstructible<T, TArgs...>::Value;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_IS_EXPLICITLY_CONSTRUCTIBLE_H