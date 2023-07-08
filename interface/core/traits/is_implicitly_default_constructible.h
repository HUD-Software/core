#ifndef HD_INC_CORE_TRAITS_IS_IMPLICITLY_DEFAULT_CONSTRUCTIBLE_H
#define HD_INC_CORE_TRAITS_IS_IMPLICITLY_DEFAULT_CONSTRUCTIBLE_H
#include "integral_constant.h"
#include "void_t.h"

namespace hud
{
    namespace details
    {
        template<typename T>
        void test_implicitly_default_constructible(const T &);

        template<typename T, typename = void>
        struct is_implicitly_default_constructible
            : hud::false_type
        {
        };

        template<typename T>
        struct is_implicitly_default_constructible<T, hud::void_t<decltype(test_implicitly_default_constructible<T>({}))>>
            : hud::true_type
        {
        };
    } // namespace details

    /** Checks whether type_t is an implicitly default constructible type. */
    template<typename type_t>
    struct is_implicitly_default_constructible
        : details::is_implicitly_default_constructible<type_t>
    {
    };

    /** Equivalent of is_implicitly_default_constructible<type_t>::value */
    template<typename type_t>
    inline constexpr bool is_implicitly_default_constructible_v = is_implicitly_default_constructible<type_t>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_IMPLICITLY_DEFAULT_CONSTRUCTIBLE_H