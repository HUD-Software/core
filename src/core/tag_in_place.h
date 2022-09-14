#pragma once
#ifndef HD_INC_OSLAYER_INPLACE_H
#define HD_INC_OSLAYER_INPLACE_H

namespace hud {

    /** Empty class type used to indicate to construct an object in-place. */
    struct TagInPlace {
        constexpr explicit TagInPlace() = default;
    };

    /** Constant used to indicate to construct an object in-place. */
    inline constexpr TagInPlace inplace{};
}

#endif // HD_INC_OSLAYER_INPLACE_H