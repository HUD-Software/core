#ifndef HD_INC_CORE_STRING_STRING_H
#define HD_INC_CORE_STRING_STRING_H
#include "../containers/vector.h"

namespace hud
{
    class string
    {
        constexpr string() noexcept = default;
        constexpr string(const string &) noexcept = default;
        constexpr string(string &&) noexcept = default;
        constexpr string &operator=(const string &) noexcept = default;
        constexpr string &operator=(string &&) noexcept = default;

    private:
        hud::vector<char8> data_;
    };
} // namespace hud

#endif HD_INC_CORE_STRING_STRING_H