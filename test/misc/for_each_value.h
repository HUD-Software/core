#ifndef HD_INC_MISC_FOREACH_VALUE_H
#define HD_INC_MISC_FOREACH_VALUE_H
#include <utility> // std::integer_sequence

namespace hud_test
{
    template<typename T, T... seq, typename Functor>
    constexpr void for_each_value(std::integer_sequence<T, seq...>, Functor &&functor)
    {
        (functor.template operator()<seq>(), ...);
    }

} // namespace hud_test
#endif // HD_INC_MISC_FOREACH_VALUE_H