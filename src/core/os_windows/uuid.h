#pragma once
#ifndef HD_INC_CORE_OS_WINDOWS_UUID_H
#define HD_INC_CORE_OS_WINDOWS_UUID_H
#include "../os_common/uuid.h"
#include <combaseapi.h>

#if !defined(HD_OS_WINDOWS)
#error This file must be included only when targetting Windows OS
#endif

namespace hud::os::windows
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
        static HD_FORCEINLINE bool create(uuid &guid) noexcept
        {
            return CoCreateGuid(reinterpret_cast<::GUID *>(&guid)) == S_OK;
        }
    };

} // namespace hud::os::windows

#endif // HD_INC_CORE_OS_WINDOWS_UUID_H