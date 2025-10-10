#ifndef HD_INC_CORE_STRING_UTF16_STRING_VIEW_H
#define HD_INC_CORE_STRING_UTF16_STRING_VIEW_H
#include "cstring.h"
#include "../containers/optional.h"
#include "../slice.h"
#include "encoding/utf16.h"

namespace hud
{
    template<typename char_t, usize char_size = sizeof(char_t)>
    struct utf16_string_view;

    template<typename char_t>
    struct utf16_string_view<char_t, 2>
    {

        /** Type of the underlying character. */
        using char_type = char_t;

        /**
         * Returns the length of the string (number of characters before the null terminator).
         * @return The length of the string in characters.
         */
        [[nodiscard]]
        constexpr usize length() const noexcept
        {
            return hud::cstring::length(ptr_);
        }

        /**
         * Returns a pointer to the underlying C-style string.
         * @return A pointer to the string's first character (null-terminated).
         */
        [[nodiscard]]
        constexpr char_type *data() const noexcept
        {
            return ptr_;
        }

        /**
         * Checks if the string is empty.
         * @return true if the string is empty, false otherwise.
         */
        [[nodiscard]]
        constexpr bool is_empty() const noexcept
        {
            return hud::character::is_null(ptr_[0]);
        }

        /**
         * Checks if this string is equal to another utf16_string_view.
         * @param v Another utf16_string_view to compare.
         * @return true if both strings are identical, false otherwise.
         */
        [[nodiscard]]
        constexpr bool equals(const utf16_string_view<char_type, 1> &v) const noexcept
        {
            return hud::cstring::equals(ptr_, v.ptr_);
        }

        /**
         * Checks if the first n characters of this string match another utf16_string_view.
         * @param v Another utf16_string_view to compare.
         * @param n Number of characters to compare.
         * @return true if the first n characters match, false otherwise.
         */
        [[nodiscard]]
        constexpr bool equals_partial(const utf16_string_view<char_type, 1> &v, const usize n) const noexcept
        {
            return hud::cstring::equals_partial(ptr_, v.ptr_, n);
        }

        /**
         * Finds the first occurrence of a substring given as a C-style string.
         * @param to_find_ptr Pointer to a null-terminated string to search for.
         * @return Index of the first occurrence, or -1 if not found.
         */
        [[nodiscard]]
        constexpr isize find_first(const char_type *to_find_ptr) const noexcept
        {
            const char_type *result = hud::cstring::find_string(ptr_, to_find_ptr);
            return result == nullptr ? -1 : result - ptr_;
        }

        /**
         * Finds the first occurrence of a substring given as another utf16_string_view.
         * @param to_find Substring to search for.
         * @return Index of the first occurrence, or -1 if not found.
         */
        [[nodiscard]]
        constexpr isize find_first(const utf16_string_view<char_type, 1> &to_find) const noexcept
        {
            return find_first(to_find.data());
        }

        /**
         * Finds the first occurrence of a character.
         * @param character_to_find The character to search for.
         * @return Index of the first occurrence, or -1 if not found.
         */
        [[nodiscard]]
        constexpr isize find_first_character(char_type character_to_find) const noexcept
        {
            const char_type *result = hud::cstring::find_character(ptr_, character_to_find);
            return result == nullptr ? -1 : result - ptr_;
        }

        /**
         * Checks if this string contains a given substring (C-style string).
         * @param to_find_str Substring to search for.
         * @return true if the substring is found, false otherwise.
         */
        [[nodiscard]]
        constexpr bool contains(const char_type *to_find_str) const noexcept
        {
            return hud::cstring::find_string(ptr_, to_find_str) != nullptr;
        }

        /**
         * Checks if this string contains a given substring (utf16_string_view).
         * @param to_find Substring to search for.
         * @return true if the substring is found, false otherwise.
         */
        [[nodiscard]]
        constexpr bool contains(const utf16_string_view<char_type, 1> &to_find) const noexcept
        {
            return contains(to_find.data());
        }

        /**
         * Checks if this string contains a given character.
         * @param character_to_find The character to search for.
         * @return true if the character is found, false otherwise.
         */
        [[nodiscard]]
        constexpr bool contains(const char_type character_to_find) const noexcept
        {
            return hud::cstring::find_character(ptr_, character_to_find) != nullptr;
        }

        /** Convert string to uppercase. */
        constexpr void to_uppercase() noexcept
        {
            hud::cstring::ascii_to_uppercase(ptr_);
        }

        /**
         * Convert string to uppercase.
         * @param count Number of character to capitalize
         */
        constexpr void to_uppercase_partial(usize count) noexcept
        {
            hud::cstring::ascii_to_uppercase_partial(ptr_, count);
        }

        /** Convert string to lowercase. */
        constexpr void to_lowercase() noexcept
        {
            hud::cstring::ascii_to_lowercase(ptr_);
        }

        /**
         * Convert string to lowercase.
         * @param count Number of character to minimize
         * @return string pointer
         */
        constexpr void to_lowercase_partial(usize count) noexcept
        {
            hud::cstring::ascii_to_lowercase_partial(ptr_, count);
        }

        /**
         * Checks whether the slice contains only valid ASCII characters.
         *
         * This method performs a runtime validation of the string content.
         * It is generally discouraged to use it after constructing an utf16_string_view.
         *
         * Instead, prefer using `make_ascii_string_view_checked()` when creating the view,
         * which validates the content upfront and guarantees that the resulting utf16_string_view
         * only contains ASCII characters.
         */
        [[nodiscard]] constexpr bool is_valid() const noexcept
        {
            return hud::encoding::is_valid_utf16(as_slice());
        }

        /**
         * Returns a slice view of the string.
         * @return A slice representing all characters of this string.
         */
        [[nodiscard]]
        constexpr const hud::slice<char_type> as_slice() const noexcept
        {
            return hud::slice<char_type> {ptr_, length()};
        }

        /**
         * Returns a slice view of the string.
         * @return A slice representing all characters of this string.
         */
        [[nodiscard]]
        constexpr hud::slice<char_type> as_slice() noexcept
        {
            return hud::slice<char_type> {ptr_, length()};
        }

        /**
         * Provides read-only access to the code unit at a given index.
         * @param i Index of the code unit to access.
         * @return A const reference to the code unit at position i.
         */
        [[nodiscard]]
        constexpr char_type &operator[](const usize i) const noexcept
        {
            return ptr_[i];
        }

        /**
         * Constructs an utf16_string_view from a C-style null-terminated string pointer.
         *
         * @param str Pointer to a null-terminated string. Must not be null.
         *
         * This constructor does **not** validate that the provided string contains only ASCII characters.
         * It is the caller’s responsibility to ensure that `str` is valid ASCII data.
         *
         * If validation is required, use `is_valid()` after construction to check the content,
         * or preferably use `make_ascii_string_view_checked()` to create a validated utf16_string_view
         * that guarantees the string contains only ASCII characters.
         */
        constexpr utf16_string_view(char_type *str) noexcept
            : ptr_(str)
        {
            HUD_CHECK(ptr_ != nullptr && "Invalid null pointer");
        }

    private:
        /**Pointer to the ascii null-terminated C-style string. */
        char_type *ptr_;
    };

    template<typename char_t, size_t N>
    utf16_string_view(const char_t (&)[N]) -> utf16_string_view<const char_t>;

    template<typename char_t, size_t N>
    utf16_string_view(char_t (&)[N]) -> utf16_string_view<char_t>;

    /**
     * Create a string view over a null-terminated ASCII string.
     *
     * This function checks that all characters in the given string are valid ASCII.
     * If the string contains any non-ASCII character, it returns hud::nullopt.
     *
     * @tparam char_t  Type of characters in the string (must be hud::char8).
     * @param ptr      Pointer to a null-terminated string.
     * @return         hud::optional containing a hud::utf16_string_view if valid,
     *                 hud::nullopt otherwise.
     */
    template<typename char_t>
    requires(hud::is_same_v<hud::remove_cv_t<char_t>, char16>)
    constexpr hud::optional<hud::utf16_string_view<char_t>> make_utf16_string_view_checked(char_t *ptr) noexcept
    {
        if (hud::encoding::is_valid_utf16(hud::slice {ptr, hud::cstring::length(ptr)})) {
            return hud::utf16_string_view<char_t>(ptr);
        }
        return hud::nullopt;
    }

} // namespace hud

#endif // HD_INC_CORE_STRING_UTF16_STRING_VIEW_H