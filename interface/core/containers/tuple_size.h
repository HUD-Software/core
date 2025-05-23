#ifndef HD_INC_CORE_TUPLE_SIZE_H
#define HD_INC_CORE_TUPLE_SIZE_H
#include "../traits/integral_constant.h"
#include "../traits/void_t.h"

namespace hud
{

    /**
     * Class template designed to access the number of elements in a tuple-like type (as a constexpr).
     * The class is itself undefined for the generic types, but a specialization for tuple-like types ( Tuple, Pair, hdFixedArray )
     * in their respective headers, also having a Value member defined to the appropriate constexpr value.
     * For const and/or volatile-qualified tuple-like objects, the class is specialized so that its Value member is itself non cv-qualified.
     * Provides the member constant Value which is equal to the number of elements in the tuple-like type.
     * @tparam tuple_like The tuple-like type
     */
    template<typename tuple_like>
    struct tuple_size;

    namespace details
    {

        /**
         * Selected when tuple_size<tuple_like>::value is ill-formed
         * @tparam tuple_like The tuple-like type
         */
        template<typename tuple_like, typename = void>
        struct tuple_size_impl
        {
        };

        /**
         * Selected when tuple_size<tuple_like>::value is well-formed
         * @tparam tuple_like The tuple-like type
         */
        template<typename tuple_like>
        struct tuple_size_impl<tuple_like, void_t<decltype(tuple_size<tuple_like>::value)>>
            : hud::integral_constant<usize, tuple_size<tuple_like>::value>
        {
        };
    } // namespace details

    /**
     * Specialization for const qualified tuple-like objects
     * @tparam tuple_like The tuple-like type
     */
    template<typename tuple_like>
    struct tuple_size<const tuple_like>
        : details::tuple_size_impl<tuple_like>
    {
    };

    /**
     * Specialization for volatile-qualified tuple-like objects
     * @tparam tuple_like The tuple-like type
     */
    template<typename tuple_like>
    struct tuple_size<volatile tuple_like>
        : details::tuple_size_impl<tuple_like>
    {
    };

    /**
     * Specialization for constvolatile-qualified tuple-like objects
     * @tparam tuple_like The tuple-like type
     */
    template<typename tuple_like>
    struct tuple_size<const volatile tuple_like>
        : details::tuple_size_impl<tuple_like>
    {
    };

    /** Equalivent of tuple_size<tuple_like>::value. */
    template<typename tuple_like>
    inline constexpr usize tuple_size_v = tuple_size<tuple_like>::value;

} // namespace hud

#endif // HD_INC_CORE_TUPLE_SIZE_H