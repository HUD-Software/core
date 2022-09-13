#pragma once
#ifndef HD_INC_OSLAYER_INITTAG_H
#define HD_INC_OSLAYER_INITTAG_H

namespace hud {

    /** Empty class type used to indicate to initialize an object. */
    struct TagInit {
        struct Tag {};
        explicit constexpr TagInit(Tag) {}
    };

    /** Constant used to indicate to initialize an object. */
    inline constexpr TagInit taginit{ TagInit::Tag{} };

}

#endif // HD_INC_OSLAYER_INITTAG_H