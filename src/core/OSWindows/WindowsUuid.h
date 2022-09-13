#pragma once
#ifndef HD_INC_OSLAYER_OS_WINDOWS_UUID_H
#define HD_INC_OSLAYER_OS_WINDOWS_UUID_H
#include "../OSCommon/CommonUuid.h"
#include <combaseapi.h>

#if !defined(HD_OS_WINDOWS)
#error This file must be included only when targetting Windows OS
#endif 

namespace hud::OS::Windows{

    struct Uuid : public OS::Common::Uuid {
       
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
        static HD_FORCEINLINE bool create(Uuid& guid) noexcept {
            return CoCreateGuid(reinterpret_cast<::GUID*>(&guid)) == S_OK;
        }
    };

} // namespace hud::OS::Windows


#endif // HD_INC_OSLAYER_OS_WINDOWS_UUID_H