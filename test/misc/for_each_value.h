#ifndef HD_INC_MISC_FOREACH_VALUE_H
#define HD_INC_MISC_FOREACH_VALUE_H
#include <utility> // std::integer_sequence

namespace hud_test
{
    template<typename type_t, type_t... seq>
    struct for_each_value;

    template<typename type_t, type_t current, type_t... seq>
    struct for_each_value<type_t, current, seq...>
    {
        template<typename Functor>
        void operator()(Functor functor)
        {
            functor.template operator()<current>();
            if constexpr (sizeof...(seq) > 0)
            {
                for_each_value<type_t, seq...>()(functor);
            }
        }
    };

    template<typename type_t, type_t current, type_t... seq>
    struct for_each_value<std::integer_sequence<type_t, current, seq...>> : public for_each_value<type_t, current, seq...>
    {
    };

} // namespace hud_test
#endif // HD_INC_MISC_FOREACH_VALUE_H