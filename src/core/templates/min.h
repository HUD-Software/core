#pragma once
#ifndef HD_INC_CORE_TEMPLATES_MIN_H
#define HD_INC_CORE_TEMPLATES_MIN_H
#include "less.h"

namespace hud {

    /** Retrieves a if a < b, b otherwise. */
    template<typename u_t, typename v_t>
    constexpr auto min(const u_t& a, const v_t& b) noexcept {
        return less<u_t,v_t>()(a, b) ? a : b;
    }

} // namespace hud 


#endif // HD_INC_CORE_TEMPLATES_MIN_H