#ifndef HD_INC_CORE_TRAITS_IS_DERIVABLE_FROM_H
#define HD_INC_CORE_TRAITS_IS_DERIVABLE_FROM_H
#include "is_base_of.h"
#include "conjunction.h"
#include "is_not_same.h"
#include "remove_cv.h"

namespace hud
{

    /**
     * Checks whether derived_t is a derived class of base_t and not the same class, without regard to their const and/or volatile qualification.
     * Only classes that are not unions are considered.
     */
    template<typename derived_t, typename base_t>
    struct is_derived_from
        : hud::conjunction<
              is_base_of<base_t, derived_t>,
              is_not_same<remove_cv_t<base_t>, remove_cv_t<derived_t>>>
    {
    };

    /** Equivalent of is_derived_from<derived_t,base_t>::value. */
    template<typename derived_t, typename base_t>
    inline constexpr bool is_derived_from_v = is_derived_from<derived_t, base_t>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_DERIVABLE_FROM_H