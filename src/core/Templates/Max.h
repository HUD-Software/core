#pragma once
#ifndef HD_INC_OSLAYER_TEMPLATES_MAX_H
#define HD_INC_OSLAYER_TEMPLATES_MAX_H
#include "Less.h"

namespace hud {

    /** Retrieves a if b < a, b otherwise. */
    template<typename U, typename V>
    constexpr auto max(const U & a, const V & b) noexcept {
        return Less<V,U>()(b, a) ? a : b;
    }

} // namespace hud 


#endif // HD_INC_OSLAYER_TEMPLATES_MAX_H