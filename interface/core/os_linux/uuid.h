#ifndef HD_INC_CORE_OS_LINUX_UUID_H
#define HD_INC_CORE_OS_LINUX_UUID_H
#include "../os_common/uuid.h"
#include <sys/random.h>

#if !defined(HD_OS_LINUX)
    #error This file must be included only when targetting Linux OS
#endif

namespace hud::os::linux
{

    struct uuid
        : public os::common::uuid
    {
        /** Inherit constructors */
        using os::common::uuid::uuid;

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
            i32 bytes_count_copied = getrandom(bytes, sizeof(bytes), GRND_RANDOM);

            // LCOV_EXCL_START ( Supposed never failed, else alternative is to read in /dev/{u}random )
            if (bytes_count_copied != sizeof(bytes)) [[unlikely]]
            {
                return false;
            }
            // LCOV_EXCL_STOP

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
} // namespace hud::os::linux

#endif /* HD_INC_CORE_OS_LINUX_UUID_H */