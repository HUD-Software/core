#ifndef HD_INC_CORE_TRAITS_DECAY_H
#define HD_INC_CORE_TRAITS_DECAY_H
#include "remove_reference.h"
#include "conditional.h"
#include "is_array.h"
#include "remove_extent.h"
#include "is_function.h"
#include "add_pointer.h"
#include "remove_cv.h"

namespace hud
{

    /**
     * Retrieves the decay type of type_t.
     * Provides a member typedef type decay type of type_t.
     * The decay type of type_t is the same type that results from the standard conversions that happen when an lvalue expression is used as an rvalue, with its cv-qualifier stripped:
     *   - If type_t is a function type, a function-to-pointer conversion is applied and the decay type is the same as: add_pointer_t<type_t>
     *   - If type_t is an array type, an array-to-pointer conversion is applied and the decay type is the same as: add_pointer_t<hud::remove_extent_t<hud::remove_reference_t<type_t>>>
     *   - Otherwise, a regular lvalue-to-rvalue conversion is applied and the decay type is the same as: remove_cv_t<hud::remove_reference_t<type_t>>.
     * This resembles the implicit conversions happening when an argument is passed by value to a function.
     */
    template<class type_t>
    struct decay
    {
    private:
        using NoRefType = hud::remove_reference_t<type_t>;

    public:
        using type = hud::conditional_t<is_array_v<NoRefType>, hud::remove_extent_t<NoRefType> *, hud::conditional_t<is_function_v<NoRefType>, add_pointer_t<NoRefType>, remove_cv_t<NoRefType>>>;
    };

    /** Equivalent of typename decay<type_t>::type. */
    template<typename type_t>
    using decay_t = typename decay<type_t>::type;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_DECAY_H