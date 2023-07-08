#ifndef HD_INC_CORE_TUPLE_ELEMENT_H
#define HD_INC_CORE_TUPLE_ELEMENT_H
#include "../traits/add_const.h"
#include "../traits/add_volatile.h"
#include "../traits/add_cv.h"

namespace hud
{

    /**
     * Class template designed to access the type of an elements in a tuple-like type at a given index.
     * The class is itself undefined for the generic types, but a specialization for tuple-like types ( Tuple, Pair, hdFixedArray )
     * in their respective headers, also having a Type member defined to the appropriate type.
     * For const and/or volatile-qualified tuple-like objects, the class is specialized so that its Type member is itself non cv-qualified.
     * Provides the member constant Type which the type of the elements in a tuple-like type at a given index.
     * @tparam index The index to get
     * @tparam TupleLike The tuple-like type
     */
    template<const usize index, typename tuple_t>
    struct tuple_element;

    /**
     * Specialization for const qualified tuple-like objects.
     * @tparam index The index to get
     * @tparam TupleLike The tuple-like type
     */
    template<const usize index, typename tuple_t>
    struct tuple_element<index, const tuple_t>
        : add_const<typename tuple_element<index, tuple_t>::type>
    {
    };

    /**
     * Specialization for volatile qualified tuple-like objects.
     * @tparam index The index to get
     * @tparam TupleLike The tuple-like type
     */
    template<const usize index, typename tuple_t>
    struct tuple_element<index, volatile tuple_t>
        : add_volatile<typename tuple_element<index, tuple_t>::type>
    {
    };

    /**
     * Specialization for const volatile qualified tuple-like objects.
     * @tparam at The index to get
     * @tparam TupleLike The tuple-like type
     */
    template<const usize index, typename tuple_t>
    struct tuple_element<index, const volatile tuple_t>
        : add_cv<typename tuple_element<index, tuple_t>::type>
    {
    };

    /** Equalivent of typename tuple_element<at, tuple_t>::type. */
    template<const usize index, typename tuple_t>
    using tuple_element_t = typename tuple_element<index, tuple_t>::type;

} // namespace hud

#endif // HD_INC_CORE_TUPLE_ELEMENT_H