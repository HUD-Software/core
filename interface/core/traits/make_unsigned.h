#ifndef HD_INC_CORE_TRAITS_MAKE_UNSIGNED_H
#define HD_INC_CORE_TRAITS_MAKE_UNSIGNED_H
#include "disjunction.h"
#include "conjunction.h"
#include "is_integral.h"
#include "negation.h"
#include "is_bool.h"
#include "is_enum.h"
#include "conditional.h"
#include "underlying_type.h"
#include "remove_cv.h"
#include "is_const.h"
#include "is_volatile.h"
#include "apply_cv.h"

namespace hud
{

    namespace details
    {

        /**  Retrieves the unsigned integral corresponding to u_type_t, keeping any cv-qualifiers. */
        template<typename u_type_t, usize>
        struct make_integral_unsigned;

        template<typename u_type_t>
        struct make_integral_unsigned<u_type_t, 1>
        {
            using type = unsigned char;
        };

        template<typename u_type_t>
        struct make_integral_unsigned<u_type_t, 2>
        {
            using type = unsigned short;
        };

        template<typename u_type_t>
        struct make_integral_unsigned<u_type_t, 4>
        {
            using type = unsigned int;
        };

        template<>
        struct make_integral_unsigned<long, 4>
        {
            using type = unsigned long;
        };

        template<>
        struct make_integral_unsigned<unsigned long, 4>
        {
            using type = unsigned long;
        };

        template<typename u_type_t>
        struct make_integral_unsigned<u_type_t, 8>
        {
            using type = unsigned long long;
        };

        template<>
        struct make_integral_unsigned<long, 8>
        {
            using type = unsigned long;
        };

        template<>
        struct make_integral_unsigned<unsigned long, 8>
        {
            using type = unsigned long;
        };

        template<typename u_type_t>
        using make_integral_unsigned_t = typename make_integral_unsigned<u_type_t, sizeof(u_type_t)>::type;

        /**  Retrieves the unsigned integral corresponding to u_type_t, keeping any cv-qualifiers if u_type_t is not an enum. */
        template<typename u_type_t, bool = is_enum_v<u_type_t>>
        struct make_unsigned_impl
            : make_unsigned_impl<underlying_type_t<u_type_t>>
        {
        };

        template<typename u_type_t>
        struct make_unsigned_impl<u_type_t, false>
            : apply_cv<make_integral_unsigned_t<remove_cv_t<u_type_t>>, is_const_v<u_type_t>, is_volatile_v<u_type_t>>
        {
        };

        template<typename u_type_t>
        using make_unsigned_impl_t = typename make_unsigned_impl<u_type_t>::type;

    } // namespace details

    /**
     * Retrieves the unsigned type corresponding to type_t, keeping any cv-qualifiers.
     * If type_t is an integral (except bool) or enumeration type, provides the member typedef type which is the unsigned integer type corresponding to type_t, with the same cv-qualifiers.
     * If type_t is signed or unsigned char, short, int, long, long long; the unsigned type from this list corresponding to type_t is provided.
     * The type_t is an enumeration type or char, wchar_t, char8_t, char16_t, char32_t; the unsigned integer type with the smallest rank having the same sizeof as type_t is provided.
     * Otherwise, the behavior is undefined.
     */
    template<typename type_t>
    struct make_unsigned
        : details::make_unsigned_impl<type_t>
    {
        static_assert(hud::disjunction_v<hud::conjunction<hud::is_integral<type_t>, hud::negation<hud::is_bool<type_t>>>, hud::is_enum<type_t>>, "make_unsigned<type_t> requires that type_t shall be a (possibly cv-qualified) integral type or enumeration but not a bool type.");
    };

    /** Equivalent of typename make_unsigned<type_t>::type. */
    template<typename type_t>
    using make_unsigned_t = typename make_unsigned<type_t>::type;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_MAKE_UNSIGNED_H
