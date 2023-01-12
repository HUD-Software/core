#pragma once
#ifndef HD_INC_CORE_INITTAG_H
#define HD_INC_CORE_INITTAG_H

namespace hud
{

    /** Empty class type used to indicate to initialize an object. */
    struct tag_init
    {
        struct tag
        {
        };
        explicit constexpr tag_init(tag) {}
    };

    /** Constant used to indicate to initialize an object. */
    inline constexpr tag_init taginit{tag_init::tag{}};

}

#endif // HD_INC_CORE_INITTAG_H