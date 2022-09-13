#pragma once
#ifndef HD_INC_OSLAYER_TEMPLATES_EQUAL_H
#define HD_INC_OSLAYER_TEMPLATES_EQUAL_H

namespace hud {

    /** Checks whether its two arguments of type T compare equal (as returned by operator ==). */
    template<typename T>
    struct Equal {
        /** Member function returning whether the arguments compare equal (a==b). */
        [[nodiscard]]
        constexpr bool operator()(const T& a, const T& b) const noexcept {
            return a == b;
        }
    };

    /** Pointer specification for Equal. */
    template<typename T>
    struct Equal<T*> {
        [[nodiscard]]
        constexpr bool operator()(T* a, T* b) const noexcept {
            return  a == b;
        }
        [[nodiscard]]
        constexpr bool operator()(T* a, hud::ptr::null) const noexcept {
            return a == nullptr;
        }
        [[nodiscard]]
        constexpr bool operator()(hud::ptr::null, T* a) const noexcept {
            return a == nullptr;
        }
        [[nodiscard]]
        constexpr bool operator()(hud::ptr::null, hud::ptr::null) const noexcept {
            return true;
        }
    };

} // namespace hud

#endif // HD_INC_OSLAYER_TEMPLATES_EQUAL_H