#pragma once
#ifndef HD_INC_CORE_TEMPLATES_LESS_EQUAL_H
#define HD_INC_CORE_TEMPLATES_LESS_EQUAL_H

namespace hud {

    /** Checks whether the first argument of type type_t compares less than or equal to the second (as returned by operator <=). */
    template<typename type_t>
    struct less_equal {
        /** Member function returning whether the first argument compares less than or equal to the second (a<=b). */ 
        [[nodiscard]]
        constexpr bool operator()(const type_t& a, const type_t& b) const noexcept {
            return a <= b;
        }
    };

    // /** Pointer specification for less_equal. */
    // template<typename type_t>
    // struct less_equal<type_t*> {
    //     /** Check if lhs <= rhs. */
    //     [[nodiscard]]
    //     bool operator()(type_t* lhs, type_t* rhs) const noexcept {
    //         return lhs <= rhs;
    //     }
    //     /** Check if lhs <= nullptr. */
    //     [[nodiscard]]
    //     constexpr bool operator()(type_t* lhs, hud::ptr::null) const noexcept {
    //         return lhs == nullptr; // Assume nullptr == 0x0;
    //     }
    //     /** Check if nullptr <= rhs. */
    //     [[nodiscard]]
    //     constexpr bool operator()(hud::ptr::null, type_t* rhs) const noexcept {
    //         return rhs != nullptr; // Assume nullptr == 0x0;
    //     }
    //     /** Check if nullptr <= nullptr. */
    //     [[nodiscard]]
    //     constexpr bool operator()(hud::ptr::null, hud::ptr::null) const noexcept {
    //         return true;
    //     }
    // };

} // namespace hud

#endif // HD_INC_CORE_TEMPLATES_LESS_EQUAL_H
