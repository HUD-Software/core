#pragma once
#ifndef HD_INC_CORE_TEMPLATES_LESS_H
#define HD_INC_CORE_TEMPLATES_LESS_H

namespace hud {

    /** Checks whether the first argument of type type_t compares less than the second (as returned by operator <). */
    template<typename type_t, typename u_type_t = type_t>
    struct less {
        /** Member function returning whether the first argument compares less than the second (a<b). */
        [[nodiscard]]
        constexpr bool operator()(const type_t& lhs, const u_type_t& rhs) const noexcept {
            return lhs < rhs;
        }
    };

    // /** Pointer specification for less. */
    // template<typename type_t>
    // struct less<type_t*, type_t*> {
    //     /** Check if lhs < rhs. */
    //     [[nodiscard]]
    //     bool operator()(type_t* lhs, type_t* rhs) const noexcept {
    //         return lhs < rhs;
    //     }
    //     /** Check if lhs < nullptr. */
    //     [[nodiscard]]
    //     constexpr bool operator()(type_t*, hud::ptr::null) const noexcept {
    //         return false; // Assume nullptr == 0x0;
    //     }
    //     /** Check if nullptr < rhs. */
    //     [[nodiscard]]
    //     constexpr bool operator()(hud::ptr::null, type_t* rhs) const noexcept {
    //         return rhs != nullptr; // Assume nullptr == 0x0;
    //     }
    //     /** Check if nullptr < nullptr. */
    //     [[nodiscard]]
    //     constexpr bool operator()(hud::ptr::null, hud::ptr::null) const noexcept {
    //         return false;
    //     }
    // };


} // namespace hud

#endif // HD_INC_CORE_TEMPLATES_LESS_H
