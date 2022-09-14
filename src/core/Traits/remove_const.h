#pragma once
#ifndef HD_INC_CORE_TRAITS_REMOVE_CONST_H
#define HD_INC_CORE_TRAITS_REMOVE_CONST_H

namespace hud {

    /** Remove const qualifier of a type if type is const qualified, else do nothing. */
    template<typename type_t>
    struct remove_const {
        using type = type_t;
    };
    template<typename type_t>
    struct remove_const<const type_t> {
        using type = type_t;
    };

    /** Equivalent of typename remove_const<type_t>::type. */
    template<typename type_t>
    using remove_const_t = typename remove_const<type_t>::type;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_REMOVE_CONST_H