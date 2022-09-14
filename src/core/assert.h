#pragma once
#ifndef HD_INC_CORE_ASSERT_H
#define HD_INC_CORE_ASSERT_H
#include "debugger.h"
#include "traits/is_constant_evaluated.h"

namespace hud {

        /**
        * Assert a condition is true. The condition is removed from code in release build
        * Break the debugger if present
        * @param condition The condition to assert
        */
        static constexpr void check([[maybe_unused]] const bool condition) noexcept {
            if (compilation::is_assertion_enabled() && !is_constant_evaluated()) {
                debugger::check(condition);
            }
        }

} // namespace hud

#endif // HD_INC_CORE_ASSERT_H