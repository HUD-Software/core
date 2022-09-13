#pragma once
#ifndef HD_INC_OSLAYER_TEMPLATES_LESS_EQUAL_H
#define HD_INC_OSLAYER_TEMPLATES_LESS_EQUAL_H

namespace hud {

    /** Checks whether the first argument of type T compares less than or equal to the second (as returned by operator <=). */
    template<typename T>
    struct LessEqual {
        /** Member function returning whether the first argument compares less than or equal to the second (a<=b). */ 
        [[nodiscard]]
        constexpr bool operator()(const T& a, const T& b) const noexcept {
            return a <= b;
        }
    };

    /** Pointer specification for LessEqual. */
    template<typename T>
    struct LessEqual<T*> {
        [[nodiscard]]
        bool operator()(T* a, T* b) const noexcept {
            return a <= b;
        }
        [[nodiscard]]
        constexpr bool operator()(T* a, hud::ptr::null) const noexcept {
            return a == nullptr;
        }
        [[nodiscard]]
        constexpr bool operator()(hud::ptr::null, T* a) const noexcept {
            return a != nullptr;
        }
        [[nodiscard]]
        constexpr bool operator()(hud::ptr::null, hud::ptr::null) const noexcept {
            return true;
        }
    };

} // namespace hud

#endif // HD_INC_OSLAYER_TEMPLATES_LESS_EQUAL_H
