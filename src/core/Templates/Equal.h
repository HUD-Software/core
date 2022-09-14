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
        [[nodiscard]]
        constexpr bool operator()(type_t* a, type_t* b) const noexcept {
            return  a == b;
        }
        [[nodiscard]]
        constexpr bool operator()(type_t* a, hud::ptr::null) const noexcept {
            return a == nullptr;
        }
        [[nodiscard]]
        constexpr bool operator()(hud::ptr::null, type_t* a) const noexcept {
            return a == nullptr;
        }
        [[nodiscard]]
        constexpr bool operator()(hud::ptr::null, hud::ptr::null) const noexcept {
            return true;
        }
    };

} // namespace hud

#endif // HD_INC_CORE_TEMPLATES_EQUAL_H