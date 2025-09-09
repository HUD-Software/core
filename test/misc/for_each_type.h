#ifndef HD_INC_MISC_FOREACH_TYPE_H
#define HD_INC_MISC_FOREACH_TYPE_H
#include <utility> // std::integer_sequence

namespace hud_test
{
    namespace detail::for_each_type
    {
        template<typename T>
        struct invoke
        {
            template<typename Functor, typename... Args>
            static void call(Functor &&f, Args &&...args)
            {
                f.template operator()<T>(std::forward<Args>(args)...);
            }
        };

        template<typename... Ts>
        struct invoke<std::tuple<Ts...>>
        {
            template<typename Functor, typename... Args>
            static void call(Functor &&f, Args &&...args)
            {
                (f.template operator()<Ts>(std::forward<Args>(args)...), ...);
            }
        };

    } // namespace detail::for_each_type

    template<typename... Types>
    struct for_each_type
    {
        template<typename Functor, typename... Args>
        void operator()(Functor functor, Args &&...args) const
        {
            (detail::for_each_type::invoke<Types>::call(functor, std::forward<Args>(args)...), ...);
        }
    };
} // namespace hud_test

#endif // HD_INC_MISC_FOREACH_TYPE_H