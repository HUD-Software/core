#pragma once
#ifndef HD_INC_OSLAYER_ASSERT_H
#define HD_INC_OSLAYER_ASSERT_H
#include "Debugger.h"
#include "Traits/IsConstantEvaluated.h"

namespace hud {

        /**
        * Assert a condition is true. The condition is removed from code in release build
        * Break the debugger if present
        * @param condition The condition to assert
        */
        static constexpr void check([[maybe_unused]] const bool condition) noexcept {
            if (Compilation::is_assertion_enabled() && !is_constant_evaluated()) {
                Debugger::check(condition);
            }
        }

} // namespace hud

#endif // HD_INC_OSLAYER_ASSERT_H