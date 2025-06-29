#ifndef HD_INC_CORE_TEMPLATES_TAGINPLACE_H
#define HD_INC_CORE_TEMPLATES_TAGINPLACE_H

namespace hud
{

    /** Empty class type used to indicate to construct an object in-place. */
    struct tag_in_place_t
    {
        constexpr explicit tag_in_place_t() = default;
    };

    /** Constant used to indicate to construct an object in-place. */
    inline constexpr tag_in_place_t tag_in_place {};

} // namespace hud

#endif // HD_INC_CORE_TEMPLATES_TAGINPLACE_H