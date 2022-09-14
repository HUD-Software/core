#pragma once
#ifndef HD_INC_CORE_INPLACE_H
#define HD_INC_CORE_INPLACE_H

namespace hud {

    /** Empty class type used to indicate to construct an object in-place. */
    struct tag_in_place {
        constexpr explicit tag_in_place() = default;
    };

    /** Constant used to indicate to construct an object in-place. */
    inline constexpr tag_in_place in_place{};
}

#endif // HD_INC_CORE_INPLACE_H