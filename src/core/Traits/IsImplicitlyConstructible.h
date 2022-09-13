#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_IS_IMPLICITLY_CONSTRUCTIBLE_H
#define HD_INC_OSLAYER_TRAITS_IS_IMPLICITLY_CONSTRUCTIBLE_H
#include <OSLayer/Traits/IsConstructible.h>
#include <OSLayer/Traits/VoidT.h>
#include <OSLayer/Templates/Declval.h>
#include <OSLayer/Traits/IsConvertible.h>

namespace hud {

    namespace details {
        template <typename T>
        constexpr void ImplicitConstruct(const T&) noexcept;

        template<typename Void, bool, typename T, typename ...TArgs>
        struct IsImplicitlyConstructibleImpl2
            : public FalseType {
        };

        template<typename T, typename ...TArgs>
        struct IsImplicitlyConstructibleImpl2 < hud::VoidT<decltype(details::ImplicitConstruct<T>({ hud::declval<TArgs>()... })) > , true, T, TArgs... >
            : public TrueType {
        };

        template <typename T, typename... TArgs>
        struct IsImplicitlyConstructibleImpl
            : IsImplicitlyConstructibleImpl2<void, IsConstructibleV<T, TArgs...>, T, TArgs...> {
        };

        template <typename T>
        struct IsImplicitlyConstructibleImpl<T, const T&>
            : IsConvertible<const T&, T> {
        };

        template <typename T>
        struct IsImplicitlyConstructibleImpl<T, T&&>
            : IsConvertible<T&&, T> {
        };

    }

    /** Checks whether T is an implicitly constructible type with TArgs. */
    template <typename T, typename... TArgs>
    struct IsImplicitlyConstructible
        : details::IsImplicitlyConstructibleImpl < T, TArgs... > {
    };

    /** Equivalent of IsImplicitlyConstructible<T, TArgs...>::Value. */
    template<typename T, typename ...TArgs >
    inline constexpr bool IsImplicitlyConstructibleV = IsImplicitlyConstructible<T, TArgs...>::Value;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_IS_IMPLICITLY_CONSTRUCTIBLE_H