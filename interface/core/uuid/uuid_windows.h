#ifndef HD_INC_CORE_UUID_UUID_WINDOWS_H
#define HD_INC_CORE_UUID_UUID_WINDOWS_H
#include "uuid_common.h"
#include <combaseapi.h>

#if !defined(HD_OS_WINDOWS)
    #error This file must be included only when targetting Windows OS
#endif

namespace hud::windows
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
        static HD_FORCEINLINE bool create(uuid &guid) noexcept
        {
            return CoCreateGuid(reinterpret_cast<::GUID *>(&guid)) == S_OK;
        }
    };

} // namespace hud::windows

#endif // HD_INC_CORE_UUID_UUID_WINDOWS_H