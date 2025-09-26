#ifndef HD_INC_CORE_STRING_STRING_H
#define HD_INC_CORE_STRING_STRING_H
#include "../containers/vector.h"

namespace hud
{
    class string
    {
    public:
        constexpr string() noexcept = default;
        constexpr string(const string &) noexcept = default;
        constexpr string(string &&) noexcept = default;
        constexpr string &operator=(const string &) noexcept = default;
        constexpr string &operator=(string &&) noexcept = default;
        [[nodiscard]] constexpr usize count() const noexcept
        {
            return data_.count();
        }
        [[nodiscard]] constexpr usize max_count() const noexcept
        {
            return data_.max_count();
        }
        [[nodiscard]] constexpr const char8 *data() const noexcept
        {
            return data_.data();
        }

    private:
        hud::vector<char8> data_;
    };
} // namespace hud

#endif // HD_INC_CORE_STRING_STRING_H