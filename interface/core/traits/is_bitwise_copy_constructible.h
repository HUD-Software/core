#ifndef HD_INC_CORE_TRAITS_IS_BITWISE_COPY_CONSTRUCTIBLE_H
#define HD_INC_CORE_TRAITS_IS_BITWISE_COPY_CONSTRUCTIBLE_H
#include "disjunction.h"
#include "is_bitwise_convertible.h"
#include "conjunction.h"
#include "is_same.h"
#include "is_trivially_copy_constructible.h"

namespace hud
{

    /** Checks whether type_t is a bitwise copy constructible type ( Constructible with a bitwise memory copy ) with the argument type u_type_t. */
    template<typename type_t, typename u_type_t>
    struct is_bitwise_copy_constructible
        : hud::disjunction<
              hud::is_bitwise_convertible<type_t, u_type_t>,
              hud::conjunction< // or if same type trivially copy constructible
                  hud::is_same<type_t, u_type_t>,
                  is_trivially_copy_constructible<type_t, u_type_t>>>
    {
    };

    /** Equivalent of is_bitwise_copy_constructible<type_t, u_type_t = type_t>::value. */
    template<typename type_t, typename u_type_t = type_t>
    constexpr bool is_bitwise_copy_constructible_v = is_bitwise_copy_constructible<type_t, u_type_t>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_BITWISE_COPY_CONSTRUCTIBLE_H