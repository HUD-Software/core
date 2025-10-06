#ifndef HD_INC_CORE_STRING_STRING_H
#define HD_INC_CORE_STRING_STRING_H
#include "../containers/vector.h"
#include "../containers/optional.h"
#include "unicode/utf8.h"
#include "cstring_view.h"
#include "../slice.h"

constexpr const char8 *const check_utf8 = "é";
static_assert(check_utf8[0] == char8(0xC3) && check_utf8[1] == char8(0xA9), "Compiler did not interpret source as UTF-8!");

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
        constexpr ~string() noexcept = default;

        [[nodiscard]] constexpr usize byte_count() const noexcept
        {
            return bytes_.byte_count();
        }
        [[nodiscard]] constexpr usize max_byte_count() const noexcept
        {
            return bytes_.max_count();
        }
        [[nodiscard]] constexpr const char8 *bytes() const noexcept
        {
            return bytes_.data();
        }

        [[nodiscard]] constexpr hud::slice<const char8> as_bytes() const noexcept
        {
            return bytes_.as_slice();
        }
        [[nodiscard]] constexpr hud::slice<char8> as_bytes() noexcept
        {
            return bytes_.as_slice();
        }

    private:
        template<typename char_t>
        requires(hud::is_same_v<hud::remove_cv_t<char_t>, char8>)
        constexpr string(const hud::slice<char_t> slice) noexcept
            : bytes_ {slice.data(), slice.count()}
        {
        }

        template<typename char_t>
        requires(hud::is_same_v<hud::remove_cv_t<char_t>, char8>)
        friend constexpr hud::optional<hud::string> make_string(const hud::slice<char_t> slice) noexcept;

    private:
        hud::vector<char8> bytes_;
    };

    template<typename char_t>
    requires(hud::is_same_v<hud::remove_cv_t<char_t>, char8>)
    constexpr hud::optional<hud::string> make_string(const char_t *ptr, usize len) noexcept
    {
        return make_string(hud::slice {ptr, len});
    }

    template<typename char_t>
    requires(hud::is_same_v<hud::remove_cv_t<char_t>, char8>)
    constexpr hud::optional<hud::string> make_string(const hud::slice<char_t> slice) noexcept
    {
        if (hud::unicode::is_valid_utf8(slice))
            return hud::string {slice};
        return hud::nullopt;
    }

    template<typename char_t>
    requires(hud::is_same_v<hud::remove_cv_t<char_t>, char8>)
    [[nodiscard]] constexpr hud::optional<hud::string> make_string(const hud::cstring_view<char_t> view) noexcept
    {
        return make_string(view.as_slice());
    }
} // namespace hud

#endif // HD_INC_CORE_STRING_STRING_H