#ifndef HD_INC_CORE_TRAITS_REMOVE_CVREF_H
#define HD_INC_CORE_TRAITS_REMOVE_CVREF_H
#include "remove_cv.h"
#include "remove_reference.h"

namespace hud
{

    /** Remove the reference and the topmost const and volatile qualifier of a type */
    template<typename type_t>
    struct remove_cv_ref
        : remove_cv<hud::remove_reference_t<type_t>>
    {
    };

    /** Equivalent of typename remove_cv_ref<type_t>::type. */
    template<typename type_t>
    using remove_cv_ref_t = typename remove_cv_ref<type_t>::type;
} // namespace hud

#endif // HD_INC_CORE_TRAITS_REMOVE_CVREF_H