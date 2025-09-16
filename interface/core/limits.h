#ifndef HD_INC_CORE_LIMITS_H
#define HD_INC_CORE_LIMITS_H

namespace hud
{

    /**
     * Defines limits of types
     * @tparam type_t The type to define
     */
    template<typename type_t>
    struct limits;
#define hud_limits_for(type_t)                                             \
    template<> struct limits<type_t>                                       \
    {                                                                      \
        static constexpr type_t min {hud::type_t##_min};                   \
        static constexpr type_t max {hud::type_t##_max};                   \
        static constexpr type_t min_positive {hud::type_t##_min_positive}; \
    };
    hud_limits_for(bool);
    hud_limits_for(i8);
    hud_limits_for(u8);
    hud_limits_for(i16);
    hud_limits_for(u16);
    hud_limits_for(i32);
    hud_limits_for(u32);
    hud_limits_for(i64);
    hud_limits_for(u64);
    hud_limits_for(f32);
    hud_limits_for(f64);
    hud_limits_for(char8);
    hud_limits_for(wchar);
    hud_limits_for(char16);
    hud_limits_for(char32);

#undef hud_limits_for

} // namespace hud

#endif // HD_INC_CORE_LIMITS_H