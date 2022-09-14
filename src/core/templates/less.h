#pragma once
#ifndef HD_INC_CORE_TEMPLATES_LESS_H
#define HD_INC_CORE_TEMPLATES_LESS_H

namespace hud {

    /** Checks whether the first argument of type type_t compares less than the second (as returned by operator <). */
    template<typename u_type_t, typename V = u_type_t>
    struct less {
        /** Member function returning whether the first argument compares less than the second (a<b). */
        [[nodiscard]]
        constexpr bool operator()(const u_type_t& a, const V& b) const noexcept {
            return a < b;
        }
    };

    /** Pointer specification for less. */
    template<typename type_t>
    struct less<type_t*, type_t*> {
        [[nodiscard]]
        bool operator()(type_t* a, type_t* b) const noexcept {
            return a < b;
        }
        [[nodiscard]]
        constexpr bool operator()(type_t*, hud::ptr::null) const noexcept {
            return false;
        }
        [[nodiscard]]
        constexpr bool operator()(hud::ptr::null, type_t* a) const noexcept {
            return a != nullptr;
        }
        [[nodiscard]]
        constexpr bool operator()(hud::ptr::null, hud::ptr::null) const noexcept {
            return false;
        }
    };


} // namespace hud

#endif // HD_INC_CORE_TEMPLATES_LESS_H
