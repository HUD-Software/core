#pragma once
#ifndef HD_INC_CORE_TRAITS_IS_IMPLICITLY_CONSTRUCTIBLE_H
#define HD_INC_CORE_TRAITS_IS_IMPLICITLY_CONSTRUCTIBLE_H
#include "../traits/is_constructible.h"
#include "../traits/void_t.h"
#include "../templates/declval.h"
#include "../traits/is_convertible.h"

namespace hud {

    namespace details {
        template <typename type_t>
        constexpr void implicit_construct(const type_t&) noexcept;

        template<typename Void, bool, typename type_t, typename ...args_t>
        struct is_implicitly_constructible_impl_2
            : public hud::false_type {
        };

        template<typename type_t, typename ...args_t>
        struct is_implicitly_constructible_impl_2 < hud::void_t<decltype(details::implicit_construct<type_t>({ hud::declval<args_t>()... })) > , true, type_t, args_t... >
            : public hud::true_type {
        };

        template <typename type_t, typename... args_t>
        struct is_implicitly_constructible_impl
            : is_implicitly_constructible_impl_2<void, is_constructible_v<type_t, args_t...>, type_t, args_t...> {
        };

        template <typename type_t>
        struct is_implicitly_constructible_impl<type_t, const type_t&>
            : is_convertible<const type_t&, type_t> {
        };

        template <typename type_t>
        struct is_implicitly_constructible_impl<type_t, type_t&&>
            : is_convertible<type_t&&, type_t> {
        };

    }

    /** Checks whether type_t is an implicitly constructible type with args_t. */
    template <typename type_t, typename... args_t>
    struct is_implicitly_constructible
        : details::is_implicitly_constructible_impl < type_t, args_t... > {
    };

    /** Equivalent of is_implicitly_constructible<type_t, args_t...>::value. */
    template<typename type_t, typename ...args_t >
    inline constexpr bool is_implicitly_constructible_v = is_implicitly_constructible<type_t, args_t...>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_IMPLICITLY_CONSTRUCTIBLE_H