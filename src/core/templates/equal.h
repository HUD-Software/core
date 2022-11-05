#pragma once
#ifndef HD_INC_CORE_TEMPLATES_EQUAL_H
#define HD_INC_CORE_TEMPLATES_EQUAL_H

namespace hud {

    /** Checks whether its two arguments of type type_t compare equal (as returned by operator ==). */
    template<typename type_t>
    struct equal {
        /** Member function returning whether the arguments compare equal (a==b). */
        [[nodiscard]]
        constexpr bool operator()(const type_t& a, const type_t& b) const noexcept {
            return a == b;
        }
    };

    /** Pointer specification for equal. */
    template<typename type_t>
    struct equal<type_t*> {
        /** Check if lhs == rhs. */
        [[nodiscard]]
        constexpr bool operator()(type_t* lhs, type_t* rhs) const noexcept {
            return  lhs == rhs;
        }
        /** Check if lhs == nullptr. */
        [[nodiscard]]
        constexpr bool operator()(type_t* lhs, hud::ptr::null) const noexcept {
            return lhs == nullptr;
        }
        /** Check if nullptr == rhs. */
        [[nodiscard]]
        constexpr bool operator()(hud::ptr::null, type_t* rhs) const noexcept {
            return rhs == nullptr;
        }
        /** Check if nullptr == nullptr. */
        [[nodiscard]]
        constexpr bool operator()(hud::ptr::null, hud::ptr::null) const noexcept {
            return true;
        }
    };

} // namespace hud

#endif // HD_INC_CORE_TEMPLATES_EQUAL_H