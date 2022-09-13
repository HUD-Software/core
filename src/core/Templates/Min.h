#pragma once
#ifndef HD_INC_OSLAYER_TEMPLATES_MIN_H
#define HD_INC_OSLAYER_TEMPLATES_MIN_H
#include "Less.h"

namespace hud {

    /** Retrieves a if a < b, b otherwise. */
    template<typename U, typename V>
    constexpr auto min(const U& a, const V& b) noexcept {
        return Less<U,V>()(a, b) ? a : b;
    }

} // namespace hud 


#endif // HD_INC_OSLAYER_TEMPLATES_MIN_H