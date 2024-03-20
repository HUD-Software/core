#ifndef HD_INC_CORE_TEMPLATES_EXCHANGE_H
#define HD_INC_CORE_TEMPLATES_EXCHANGE_H
#include "../traits/is_move_constructible.h"
#include "../traits/is_move_assignable.h"

namespace hud
{
    template<typename type_t, typename u_type_t = type_t>
    constexpr type_t exchange(type_t &obj, u_type_t &&new_value) noexcept
    {
        static_assert(hud::is_nothrow_move_constructible_v<type_t, u_type_t>, "type_t(const u_type_t&) or type_t(u_type_t&&) is throwable. exchange is not designed to allow throwable constructible components");
        static_assert(hud::is_nothrow_move_assignable_v<type_t, u_type_t>, "type_t& type_t::operator=(const u_type_t&) or type_t& type_t::operator=(u_type_t&&) is throwable. exchange is not designed to allow throwable assignable components");

        type_t old_value = std::move(obj);
        obj = std::forward<u_type_t>(new_value);
        return old_value;
    }
} // namespace hud

#endif // HD_INC_CORE_TEMPLATES_EXCHANGE_H