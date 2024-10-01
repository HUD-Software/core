#ifndef HD_INC_CORE_OS_LINUX_MATH_H
#define HD_INC_CORE_OS_LINUX_MATH_H
#include "../os_common/math.h"
#include "../traits/is_floating_point.h"
#if !defined(HD_OS_LINUX)
    #error This file must be included only when targetting Linux OS
#endif
#include <cmath>

namespace hud::os::linux
{
    struct math
        : public os::common::math
    {
        /** Compute the natual logarithm of value. */
        template<typename type_t>
        [[nodiscard]] static type_t log(const type_t value) noexcept
        {
            return __builtin_log(value);
        }

        /** Compute the natual logarithm of value. */
        [[nodiscard]] static f32 log(const f32 value) noexcept
        {
            return __builtin_logf(value);
        }

        /** Compute the square root of value. */
        template<typename type_t>
        [[nodiscard]] static type_t sqrt(const type_t value) noexcept
        {
            return __builtin_sqrt(value);
        }

        /** Compute the square root of value. */
        [[nodiscard]] static f32 sqrt(const f32 value) noexcept
        {
            return __builtin_sqrtf(value);
        }
    };

} // namespace hud::os::linux

#endif /* HD_INC_CORE_OS_LINUX_MATH_H */