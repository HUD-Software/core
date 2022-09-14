#pragma once
#ifndef HD_INC_CORE_TRAITS_REMOVE_REFERENCE_H
#define HD_INC_CORE_TRAITS_REMOVE_REFERENCE_H

namespace hud {

    /**
    * Remove the reference of a type.
    * If the type type_t is a reference type, provides the member typedef type which is the type referred to by type_t. Otherwise type is type_t.
    */
    template<typename type_t>
    struct remove_reference {
        using type = type_t;
    };

    template<typename type_t>
    struct remove_reference<type_t&> {
        using type = type_t;
    };
    template<typename type_t>
    struct remove_reference<type_t&&> {
        using type = type_t;
    };

    /** Equivalent of typename remove_reference<type_t>::type. */
    template <typename type_t>
    using remove_reference_t = typename remove_reference<type_t>::type;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_REMOVE_REFERENCE_H