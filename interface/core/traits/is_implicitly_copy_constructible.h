#ifndef HD_INC_CORE_TRAITS_IS_IMPLICITLY_COPY_CONSTRUCTIBLE_H
#define HD_INC_CORE_TRAITS_IS_IMPLICITLY_COPY_CONSTRUCTIBLE_H
#include "add_lvalue_reference.h"
#include "add_const.h"
#include "void_t.h"

namespace hud
{

    namespace details
    {
        template<typename T>
        void test_implicitly_copy_constructible(T);

        template<typename T, typename U, typename = void>
        struct is_implicitly_copy_constructible
            : hud::false_type
        {
        };

        template<typename T, typename U>
        struct is_implicitly_copy_constructible<T, const U &, hud::void_t<decltype(test_implicitly_copy_constructible<T>(static_cast<const U &>(hud::declval<U>())))>>
            : hud::true_type
        {
        };

    } // namespace details

    /** Checks whether type_t is an implicitly copy constructible type with u_type_t. */
    template<typename type_t, typename u_type_t = type_t>
    struct is_implicitly_copy_constructible
        : details::is_implicitly_copy_constructible<type_t, hud::add_lvalue_reference_t<hud::add_const_t<u_type_t>>>
    {
    };

    /** Equivalent of is_implicitly_copy_constructible<type_t, u_type_t>::value. */
    template<typename type_t, typename u_type_t = type_t>
    constexpr bool is_implicitly_copy_constructible_v = is_implicitly_copy_constructible<type_t, u_type_t>::value;
} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_IMPLICITLY_COPY_CONSTRUCTIBLE_H