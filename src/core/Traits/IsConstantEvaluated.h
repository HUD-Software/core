#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_IS_CONSTANT_EVALUATED
#define HD_INC_OSLAYER_TRAITS_IS_CONSTANT_EVALUATED

namespace hud {

    /** Detects whether the function call occurs within a constant-evaluated context. */
    [[nodiscard]]
    constexpr bool is_constant_evaluated() noexcept {
        return __builtin_is_constant_evaluated();
    }

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_IS_CONSTANT_EVALUATED