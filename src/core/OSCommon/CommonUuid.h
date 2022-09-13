#pragma once
#ifndef HD_INC_OSLAYER_OS_COMMON_UUID_H
#define HD_INC_OSLAYER_OS_COMMON_UUID_H
#include "../Memory.h"
#include "../Traits/IsConstantEvaluated.h"

namespace hud::OS::Common{

    enum class EGuidFormat
    {
        /** 00000000000000000000000000000000 */
        Digits,

        /** 00000000-0000-0000-0000-000000000000 */
        URN,

        /** {00000000-0000-0000-0000-000000000000} */
        URNInBraces,

        /** (00000000-0000-0000-0000-000000000000) */
        URNInParentheses,

        /** {0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */
        HexInBraces,

        /** 00000000-00000000-00000000-00000000 */
        Integer
    };


/** Universally Unique Identifiers (UUIDs). A UUID is a unique 128-bit number, stored as 16 bytes */
struct alignas(16) Uuid {

    /** Default constructor. Initialize all components to 0. */
    constexpr Uuid() noexcept = default;

    /**
    * Constructor. Initialise component with paramters.
    * @param comp_a The a component. 
    * @param comp_b The b component.
    * @param comp_c The c component.
    * @param comp_d The d component.
    */
    constexpr Uuid(const u32 comp_a, const u32 comp_b, const u32 comp_c, const u32 comp_d) noexcept
        :a(comp_a), b(comp_b), c(comp_c), d(comp_d) {
    }

    /** Check whether the Uuid is valid or not. */
    [[nodiscard]]
    bool is_valid() const noexcept {
        constexpr const Uuid invalid;
        return !Memory::compare_equal(this, &invalid, sizeof(Uuid));
    }

    /** Reset the Uuid with all components to zero. Making it invalid*/
    void reset() noexcept {
        Memory::set_zero(this, sizeof(Uuid));
    }

    /** 
    * Assign another guid to this.
    * @param other The Uuid to assign to this
    * @return *this
    */
    Uuid& operator=(const Uuid& other) noexcept {
        Memory::copy(this, &other, sizeof(Uuid));
        return *this;
    }

    /** First component */
    u32 a = 0u;
    /** Second component */
    u32 b = 0u;
    /** Third component */
    u32 c = 0u;
    /** Fourth component */
    u32 d = 0u;

};

/**
* Check if two Uuid are equal.
* @param left Left-hand side Uuid
* @param right Right-hand side Uuid
* @return true if left equal right, false otherwise
*/
[[nodiscard]]
bool operator==(const Uuid& left, const Uuid& right) noexcept {
    return Memory::compare_equal(&left, &right, sizeof(Uuid));
}

/**
* Check if two Uuid are not equal.
* @param left Left-hand side Uuid
* @param right Right-hand side Uuid
* @return true if left is not equal right, false otherwise
*/
[[nodiscard]]
bool operator!=(const Uuid& left, const Uuid& right) noexcept {
    return !(left == right);
}
} // namespace hud::OS::GUID

#endif // HD_INC_OSLAYER_OS_COMMON_UUID_H
