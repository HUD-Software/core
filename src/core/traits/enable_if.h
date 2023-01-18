#ifndef HD_INC_CORE_TRAITS_ENABLE_IF_H
#define HD_INC_CORE_TRAITS_ENABLE_IF_H

namespace hud
{

    /**
     * If enabled is true, enable_if has a public member typedef type, equal to type_t; otherwise, there is no member typedef.
     * This metafunction is a convenient way to leverage SFINAE to conditionally remove functions from overload resolution
     * based on type traits and to provide separate function overloads and specializations for different type traits.
     * enable_if can be used as:
     * - An additional function argument (not applicable to operator overloads),
     * - A return type (not applicable to constructors and destructors),
     * - A class template or function template parameter
     */
    template<bool enabled, typename type_t = void>
    struct enable_if
    {
    };

    template<typename type_t>
    struct enable_if<true, type_t>
    {
        using type = type_t;
    };

    /** Equivalent of typename enable_if<enabled, type_t>::type. */
    template<bool enabled, typename type_t = void>
    using enable_if_t = typename enable_if<enabled, type_t>::type;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_ENABLE_IF_H