#ifndef HD_INC_CORE_UUID_UUID_LINUX_H
#define HD_INC_CORE_UUID_UUID_LINUX_H
#include "uuid_common.h"
#include <sys/random.h>

#if defined(HD_COMPILER_EMSCRIPTEN)
    #include <emscripten.h>
#endif

#if !defined(HD_HOST_LINUX) && !defined(HD_HOST_EMSCRIPTEN)
    #error This file must be included only when targetting Linux OS
#endif

namespace hud::linux
{

    struct uuid
        : public hud::common::uuid
    {
        /** Inherit constructors */
        using hud::common::uuid::uuid;

        /**
         * Create a Universally Unique Identifiers.
         * @param guid reference to the uuid that received the cerated uuid
         * @return true if the uuid is created, false otherwise
         */
        static bool create(uuid &guid) noexcept
        {
            // Generate a UUID from Truly Random or Pseudo-Random Numbers as defined in RFC-4122
            // See : https://datatracker.ietf.org/doc/html/rfc4122#section-4.4
            //       https://www.cryptosys.net/pki/uuid-rfc4122.html

            // 1. Generate 16 random bytes (=128 bits)
            alignas(16) u8 bytes[16];
#if defined(HD_HOST_LINUX)
            i32 bytes_count_copied = getrandom(bytes, sizeof(bytes), GRND_RANDOM);

            // LCOV_EXCL_START ( Supposed never failed, else alternative is to read in /dev/{u}random )
            if (bytes_count_copied != sizeof(bytes)) [[unlikely]]
            {
                return false;
            }
                // LCOV_EXCL_STOP
#elif defined(HD_HOST_EMSCRIPTEN)
            EM_ASM({
            const crypto = require('crypto');
            var bytes = new Uint8Array(16);
            crypto.getRandomValues(bytes);
            for (var i = 0; i < 16; i++) {
                HEAPU8[$0 + i] = bytes[i];
            } }, &bytes[0]);
#endif
            // 2. Set the four most significant bits of the 7th byte to 0100, so the high nibble is "4"
            bytes[6] = (bytes[6] & 0x0F) | 0x40;

            // 3. Set the two most significant bits of the 9th byte to 10, so the high nibble will be one of "8", "9", "A", or "B"
            bytes[8] = (bytes[8] & 0x3F) | 0x80;

            // Save in the uuid
            const u32 *as_u32 = reinterpret_cast<const u32 *>(&bytes[0]);
            guid = uuid {as_u32[0], as_u32[1], as_u32[2], as_u32[3]};
            return true;
        }
    };
} // namespace hud::linux

#endif /* HD_INC_CORE_UUID_UUID_LINUX_H */