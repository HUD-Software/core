#pragma once
#ifndef HD_INC_MISC_FOREACH_TYPE_H
#define HD_INC_MISC_FOREACH_TYPE_H
#include <utility> // std::integer_sequence

namespace hud_test
{
    template <typename... Types>
    struct for_each_type;

    template <typename type_t, typename... Rest>
    struct for_each_type<type_t, Rest...>
    {
        template <typename Functor, typename... Args>
        void operator()(Functor functor, Args &&...args)
        {
            functor.template operator()<type_t>(std::forward<Args>(args)...);
            if constexpr (sizeof...(Rest) > 0)
            {
                for_each_type<Rest...>()(functor, std::forward<Args>(args)...);
            }
        }
    };

    template <typename... Types, typename... Rest>
    struct for_each_type<std::tuple<Types...>, Rest...>
    {
        template <typename Functor, typename... Args>
        void operator()(Functor functor, Args &&...args)
        {
            // Iterate over each type of the tuple
            for_each_type<Types...>()(functor, std::forward<Args>(args)...);

            if constexpr (sizeof...(Rest) > 0)
            {
                for_each_type<Rest...>()(functor, std::forward<Args>(args)...);
            }
        }
    };

} // namespace hud_test

#endif // HD_INC_MISC_FOREACH_TYPE_H