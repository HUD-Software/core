#pragma once
#ifndef HD_INC_OSLAYER_OS_LINUX_UUID_H
#define HD_INC_OSLAYER_OS_LINUX_UUID_H
#include "../OSCommon/CommonUuid.h"
#include <sys/random.h>

#if !defined(HD_OS_LINUX)
#error This file must be included only when targetting Linux OS
#endif 


namespace hud::OS::Linux{

    struct Uuid 
        : public OS::Common::Uuid {
       
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
            : OS::Common::Uuid(comp_a, comp_b, comp_c, comp_d) {
        }

        /**
        * Create a Universally Unique Identifiers.
        * @param guid reference to the Uuid that received the cerated Uuid
        * @return true if the Uuid is created, false otherwise
        */
        static bool create(Uuid& guid) noexcept {
            // Generate a UUID from Truly Random or Pseudo-Random Numbers as defined in RFC-4122
            // See : https://datatracker.ietf.org/doc/html/rfc4122#section-4.4
            //       https://www.cryptosys.net/pki/uuid-rfc4122.html

            // 1. Generate 16 random bytes (=128 bits)
            alignas(16) u8 bytes[16];
            if(getrandom(bytes, sizeof(bytes), GRND_RANDOM) != sizeof(bytes)) {
                return false;
            }

            // 2. Set the four most significant bits of the 7th byte to 0100, so the high nibble is "4"
            bytes[6] = (bytes[6] & 0x0F) | 0x40;

            // 3. Set the two most significant bits of the 9th byte to 10, so the high nibble will be one of "8", "9", "A", or "B" 
            bytes[8] = (bytes[8] & 0x3F) | 0x80;

            // Save in the Uuid
            const u32* as_u32 = reinterpret_cast<const u32*>(&bytes[0]);
            guid = Uuid{as_u32[0], as_u32[1], as_u32[2], as_u32[3]};
            return true;
        }
    };
}

#endif /* HD_INC_OSLAYER_OS_LINUX_UUID_H */