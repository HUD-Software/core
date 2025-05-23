#ifndef HD_INC_CORE_TRAITS_IS_OBJECT_H
#define HD_INC_CORE_TRAITS_IS_OBJECT_H
#include "negation.h"
#include "disjunction.h"
#include "is_function.h"
#include "is_reference.h"
#include "is_void.h"

namespace hud
{

    /** Checks whether type_t is an object type (that is any possibly cv-qualified type other than function, reference, or void types). */
    template<typename type_t>
    struct is_object
        : hud::negation<hud::disjunction<is_function<type_t>, is_reference<type_t>, is_void<type_t>>>
    {
    };

    /** Equivalent of is_object<type_t>::value. */
    template<typename type_t>
    constexpr bool is_object_v = is_object<type_t>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_OBJECT_H
