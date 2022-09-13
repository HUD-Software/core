#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_COMMON_TYPE_H
#define HD_INC_OSLAYER_TRAITS_COMMON_TYPE_H
#include "Decay.h"
#include "../Templates/Declval.h"
#include "RemoveReference.h"
#include "VoidT.h"

namespace hud {

    /** Retrieves the common type for the types in the Types list to which all of them can be converted. */
    template<typename... T>
    struct CommonType;

    namespace details {

        template<typename T0, typename T1>
        using CondT = decltype(false ? hud::declval<T0>() : hud::declval<T1>());

        template<typename T0, typename T1, typename CRT0 = const RemoveReferenceT<T0>&, typename CRT1 = const RemoveReferenceT<T1>&, typename = void>
        struct CommonType2Impl2 {
        };

        template<typename T0, typename T1, typename CRT0, typename CRT1 >
        struct CommonType2Impl2<T0,T1, CRT0, CRT1, VoidT<DecayT<CondT<CRT0, CRT1>>>> {
            // If DecayT<CondT<CRT0, CRT1>> is valid, the Type is DecayT<CondT<CRT0, CRT1>>
            using Type = DecayT<CondT<CRT0, CRT1>>;
        };

        template<typename T0, typename T1, typename = void>
        struct CommonType2Impl
            : CommonType2Impl2<T0, T1> {
        };

        template<typename T0, typename T1>
        struct CommonType2Impl<T0,T1, VoidT<DecayT<CondT<T0,T1>>>> {
            // If DecayT<CondT<T0,T1>> is valid, the Type is DecayT<CondT<T0, T1>>
            using Type = DecayT<CondT<T0, T1>>;
        };

        template<typename T0, typename T1, typename DecayedT0 = DecayT<T0>, typename DecayedT1 = DecayT<T1>>
        struct CommonType2
            : CommonType < DecayedT0, DecayedT1> {
            // If DecayT<T0> or DecayT<T1> produces different type, the Type is the same as CommonType<DecayedT0, DecayedT1>
        };
        template<typename T0, typename T1>
        struct CommonType2<T0,T1,T0,T1>
            : CommonType2Impl<T0, T1> { // T0 is DecayedT0, T1 is DecayedT1
            // If DecayT<T0> or DecayT<T1> produces same type, the Type is the same as CommonType<DecayedT0, DecayedT1>
        };
        template<typename T0, typename T1>
        struct CommonType2<T0, T1, VoidT<CondT<T0, T1>>> {
            using Type = DecayT<CondT<T0, T1>>;
        };

        template<typename Void, typename T0, typename T1, typename... Rest>
        struct CommonType3 {
        };

        template<typename T0, typename T1, typename... Rest>
        struct CommonType3< VoidT<typename CommonType<T0, T1>::Type>, T0, T1, Rest... >
            : CommonType<typename CommonType<T0, T1>::Type, Rest...> {
            // If CommonType<T0,T1>::Type exist, the Type is CommonType<typename CommonType<T0, T1>::Type, Rest...>
        };
    }


    /**
    * Determines the common type among all types T..., that is the type all T... can be implicitly converted to.
    * If such a type exists (as determined according to the rules below), the member type names that type. Otherwise, there is no member type.
    * - If sizeof...(T) is zero, there is no member type. 
    * - If sizeof...(T) is one (i.e., T... contains only one type T0), the member type names the same type as CommonType<T0, T0>::Type if it exists; otherwise there is no member type.
    * - If sizeof...(T) is two (i.e., T... contains exactly two types T0 and T1), 
    *   - If applying Decay to at least one of T1 and T2 produces a different type, the member type names the same type as CommonTypeT<DecayT<T0>, DecayT<T1>>, if it exists; if not, there is no member type.
    *   - Otherwise, if there is a user specialization for CommonType<T0, T1>, that specialization is used;
    *   - Otherwise, if DecayT<decltype(false ? hud::declval<T0>() : hud::declval<T1>())> is a valid type, the member type denotes that type; 
    *   - Otherwise, if Decayt<decltype(false ? hud::declval<CR0>() : hud::declval<CR1>())> is a valid type, where CR0 and CR1 are const RemoveReferenceT<T0>& and const RemoveReferenceT<T1>& respectively, the member type denotes that type; 
    *   - Otherwise, there is no member type.
    * - If sizeof...(T) is greater than two (i.e., T... consists of the types T0, T1, R...), then if CommonTypeT<T0, T1> exists, the member Type denotes CommonTypeT<typename CommonTypeT<T0, T1>, R...> if such a type exists. In all other cases, there is no member type.
    */
    template <>
    struct CommonType<> {
        // sizeof...(T) == 0
    };

    template <typename T>
    struct CommonType<T>
        : CommonType<T, T> {
        // sizeof...(T) == 1
    };


    template <typename T0, typename T1>
    struct CommonType<T0, T1>
        : details::CommonType2<DecayT<T0>, DecayT<T1>> {
        // sizeof...(T) == 2
    };


    template <typename T0, typename T1, typename... Rest>
    struct CommonType<T0, T1, Rest...>
        : details::CommonType3<void, T0, T1, Rest...> {
        // sizeof...(T) > 2
    };

    /** Equivalent of typename CommonType<TTypes...>::Type. */
    template<typename... T >
    using CommonTypeT = typename CommonType<T...>::Type;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_COMMON_TYPE_H