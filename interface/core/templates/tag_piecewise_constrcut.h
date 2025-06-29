#ifndef HD_INC_CORE_TEMPLATES_TAG_PIECEWISE_CONSTRUCT_H
#define HD_INC_CORE_TEMPLATES_TAG_PIECEWISE_CONSTRUCT_H

namespace hud
{

    /** Empty class type used to indicate to construct an object in-place. */
    struct tag_piecewise_construct_t
    {
        constexpr explicit tag_piecewise_construct_t() = default;
    };

    /** Constant used to indicate to construct an object in-place. */
    inline constexpr tag_piecewise_construct_t tag_piecewise_construct {};

} // namespace hud

#endif // HD_INC_CORE_TEMPLATES_TAG_PIECEWISE_CONSTRUCT_H