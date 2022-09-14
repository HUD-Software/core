#pragma once
#ifndef HD_INC_CORE_TEMPLATES_GREATER_H
#define HD_INC_CORE_TEMPLATES_GREATER_H

namespace hud {

    /** Checks whether the first argument of type type_t compares greater than the second (as returned by operator >). */
    template<typename type_t>
    struct greater {
        /** Member function returning whether the first argument compares greater than the second (a>b). */ 
        [[nodiscard]]
        constexpr bool operator()(const type_t& a, const type_t& b) const noexcept {
            return a > b;
        }
    };

    /** Pointer specification for greater. */
    template<typename type_t>
    struct greater<type_t*> {
        [[nodiscard]]
        bool operator()(type_t* a, type_t* b) const noexcept {
            return  a > b;
        }
        [[nodiscard]]
        constexpr bool operator()(type_t* a, hud::ptr::null) const noexcept {
            return a != nullptr;
        }
        [[nodiscard]]
        constexpr bool operator()(hud::ptr::null, type_t*) const noexcept {
            return false;
        }
        [[nodiscard]]
        constexpr bool operator()(hud::ptr::null, hud::ptr::null) const noexcept {
            return false;
        }

    };

} // namespace hud

#endif // HD_INC_CORE_TEMPLATES_GREATER_H
