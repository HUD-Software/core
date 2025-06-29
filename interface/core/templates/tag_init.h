#ifndef HD_INC_CORE_TEMPLATES_TAGINIT_H
#define HD_INC_CORE_TEMPLATES_TAGINIT_H

namespace hud
{

    /** Empty class type used to indicate to initialize an object. */
    struct tag_init_t
    {
        struct tag_t
        {
        };

        explicit constexpr tag_init_t(tag_t)
        {
        }
    };

    /** Constant used to indicate to initialize an object. */
    inline constexpr tag_init_t tag_init {tag_init_t::tag_t {}};

} // namespace hud

#endif // HD_INC_CORE_TEMPLATES_TAGINIT_H