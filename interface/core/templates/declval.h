#ifndef HD_INC_CORE_TEMPLATES_DECLVAL_H
#define HD_INC_CORE_TEMPLATES_DECLVAL_H
#include "../traits/add_rvalue_reference.h"

namespace hud
{

    /**
     * Converts any type type_t to a reference type, making it possible to use member functions in decltype expressions without the need to go through constructors.
     * declval is commonly used in templates where acceptable template parameters may have no constructor in common, but have the same member function whose return type is needed.
     * Note that because no definition exists for declval, it can only be used in unevaluated contexts;
     * It is an error to evaluate an expression that contains this function. Formally, the program is ill-formed if this function is odr-used.
     */
    template<class type_t>
    constexpr add_rvalue_reference_t<type_t> declval() noexcept;

} // namespace hud

#endif // HD_INC_CORE_TEMPLATES_DECLVAL_H