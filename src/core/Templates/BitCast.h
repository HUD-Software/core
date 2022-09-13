#pragma once
#ifndef HD_INC_OSLAYER_TEMPLATES_BITCAST_H
#define HD_INC_OSLAYER_TEMPLATES_BITCAST_H
#include "../Traits/IsSameSize.h"
#include "../Traits/IsTriviallyCopyable.h"

namespace hud {

        /**
        * Obtain a value of type To by reinterpreting the object representation of From. 
        * Every bit in the value representation of the returned To object is equal to the corresponding bit in the object representation of from.
        * The values of padding bits in the returned To object are unspecified. 
        * @tparam To The reinterpreted output type
        * @tparam From The type of the source of bits to reinterpret
        */
        template<typename To, typename From>
        [[nodiscard]]
        constexpr To bit_cast(const From& value) noexcept requires(sizeof(To) == sizeof(From) && IsTriviallyCopyableV<To> && IsTriviallyCopyableV<From>){
            return __builtin_bit_cast(To, value);
        }

} // namespace hud

#endif // HD_INC_OSLAYER_TEMPLATES_BITCAST_H