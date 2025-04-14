#ifndef HD_INC_CORE_UUID_UUID_COMMON_H
#define HD_INC_CORE_UUID_UUID_COMMON_H
#include "../memory.h"
#include "../traits/is_constant_evaluated.h"

namespace hud::common
{

    enum class uuid_format_e
    {
        /** 00000000000000000000000000000000 */
        digits,

        /** 00000000-0000-0000-0000-000000000000 */
        urn,

        /** {00000000-0000-0000-0000-000000000000} */
        urn_in_braces,

        /** (00000000-0000-0000-0000-000000000000) */
        urn_in_parentheses,

        /** {0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */
        hex_in_braces,

        /** 00000000-00000000-00000000-00000000 */
        integer
    };

    /** Universally Unique Identifiers (UUIDs). A UUID is a unique 128-bit number, stored as 16 bytes */
    struct alignas(16) uuid
    {

        /** Default constructor. Initialize all components to 0. */
        constexpr uuid() noexcept = default;

        /**
         * Constructor. Initialise component with paramters.
         * @param comp_a The a component.
         * @param comp_b The b component.
         * @param comp_c The c component.
         * @param comp_d The d component.
         */
        constexpr uuid(const u32 comp_a, const u32 comp_b, const u32 comp_c, const u32 comp_d) noexcept
            : a(comp_a)
            , b(comp_b)
            , c(comp_c)
            , d(comp_d)
        {
        }

        /** Check whether the uuid is valid or not. */
        [[nodiscard]] bool is_valid() const noexcept
        {
            constexpr const uuid invalid;
            return !hud::memory::compare_equal(this, &invalid, sizeof(uuid));
        }

        /** Reset the uuid with all components to zero. Making it invalid*/
        void reset() noexcept
        {
            hud::memory::set_memory_zero(this, sizeof(uuid));
        }

        /**
         * Assign another guid to this.
         * @param other The uuid to assign to this
         * @return *this
         */
        uuid &operator=(const uuid &other) noexcept
        {
            hud::memory::copy_memory(this, &other, sizeof(uuid));
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
     * Check if two uuid are equal.
     * @param left Left-hand side uuid
     * @param right Right-hand side uuid
     * @return true if left equal right, false otherwise
     */
    [[nodiscard]] bool operator==(const uuid &left, const uuid &right) noexcept
    {
        return hud::memory::compare_equal(&left, &right, sizeof(uuid));
    }

    /**
     * Check if two uuid are not equal.
     * @param left Left-hand side uuid
     * @param right Right-hand side uuid
     * @return true if left is not equal right, false otherwise
     */
    [[nodiscard]] bool operator!=(const uuid &left, const uuid &right) noexcept
    {
        return !(left == right);
    }
} // namespace hud::common

#endif // HD_INC_CORE_UUID_UUID_COMMON_H
