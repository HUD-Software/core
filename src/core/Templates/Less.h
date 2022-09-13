#pragma once
#ifndef HD_INC_OSLAYER_TEMPLATES_LESS_H
#define HD_INC_OSLAYER_TEMPLATES_LESS_H

namespace hud {

    /** Checks whether the first argument of type T compares less than the second (as returned by operator <). */
    template<typename U, typename V = U>
    struct Less {
        /** Member function returning whether the first argument compares less than the second (a<b). */
        [[nodiscard]]
        constexpr bool operator()(const U& a, const V& b) const noexcept {
            return a < b;
        }
    };

    /** Pointer specification for Less. */
    template<typename T>
    struct Less<T*, T*> {
        [[nodiscard]]
        bool operator()(T* a, T* b) const noexcept {
            return a < b;
        }
        [[nodiscard]]
        constexpr bool operator()(T*, hud::ptr::null) const noexcept {
            return false;
        }
        [[nodiscard]]
        constexpr bool operator()(hud::ptr::null, T* a) const noexcept {
            return a != nullptr;
        }
        [[nodiscard]]
        constexpr bool operator()(hud::ptr::null, hud::ptr::null) const noexcept {
            return false;
        }
    };


} // namespace hud

#endif // HD_INC_OSLAYER_TEMPLATES_LESS_H
