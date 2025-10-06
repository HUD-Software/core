#ifndef HD_INC_CORE_STRING_ASCII_STRING_VIEW_H
#define HD_INC_CORE_STRING_ASCII_STRING_VIEW_H
#include "cstring.h"
#include "../containers/optional.h"
#include "../slice.h"
#include "encoding/ascii.h"

namespace hud
{
    /**
     * An view of a C-style, ASCII null-terminated string.
     * This view does not own the underlying string and does not allow modification
     * of its length. It provides utility functions for querying and accessing
     * the string content.
     */
    template<typename char_t>
    struct ascii_string_view
    {
        static_assert(sizeof(char_t) == 1);

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
        constexpr char_t *data() const noexcept
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
         * Checks if this string is equal to another ascii_string_view.
         * @param v Another ascii_string_view to compare.
         * @return true if both strings are identical, false otherwise.
         */
        [[nodiscard]]
        constexpr bool equals(const ascii_string_view<char_t> &v) const noexcept
        {
            return hud::cstring::equals(ptr_, v.ptr_);
        }

        /**
         * Checks if the first n characters of this string match another ascii_string_view.
         * @param v Another ascii_string_view to compare.
         * @param n Number of characters to compare.
         * @return true if the first n characters match, false otherwise.
         */
        [[nodiscard]]
        constexpr bool equals_partial(const ascii_string_view<char_t> &v, const usize n) const noexcept
        {
            return hud::cstring::equals_partial(ptr_, v.ptr_, n);
        }

        /**
         * Finds the first occurrence of a substring given as a C-style string.
         * @param to_find_ptr Pointer to a null-terminated string to search for.
         * @return Index of the first occurrence, or -1 if not found.
         */
        [[nodiscard]]
        constexpr isize find_first(const char_t *to_find_ptr) const noexcept
        {
            const char_t *result = hud::cstring::find_string(ptr_, to_find_ptr);
            return result == nullptr ? -1 : result - ptr_;
        }

        /**
         * Finds the first occurrence of a substring given as another ascii_string_view.
         * @param to_find Substring to search for.
         * @return Index of the first occurrence, or -1 if not found.
         */
        [[nodiscard]]
        constexpr isize find_first(const ascii_string_view<char_t> &to_find) const noexcept
        {
            return find_first(to_find.data());
        }

        /**
         * Finds the first occurrence of a character.
         * @param character_to_find The character to search for.
         * @return Index of the first occurrence, or -1 if not found.
         */
        [[nodiscard]]
        constexpr isize find_first_character(char_t character_to_find) const noexcept
        {
            const char_t *result = hud::cstring::find_character(ptr_, character_to_find);
            return result == nullptr ? -1 : result - ptr_;
        }

        /**
         * Checks if this string contains a given substring (C-style string).
         * @param to_find_str Substring to search for.
         * @return true if the substring is found, false otherwise.
         */
        [[nodiscard]]
        constexpr bool contains(const char_t *to_find_str) const noexcept
        {
            return hud::cstring::find_string(ptr_, to_find_str) != nullptr;
        }

        /**
         * Checks if this string contains a given substring (ascii_string_view).
         * @param to_find Substring to search for.
         * @return true if the substring is found, false otherwise.
         */
        [[nodiscard]]
        constexpr bool contains(const ascii_string_view<char_t> &to_find) const noexcept
        {
            return contains(to_find.data());
        }

        /**
         * Checks if this string contains a given character.
         * @param character_to_find The character to search for.
         * @return true if the character is found, false otherwise.
         */
        [[nodiscard]]
        constexpr bool contains(const char_t character_to_find) const noexcept
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
         * Returns a slice view of the string.
         * @return A slice representing all characters of this string.
         */
        [[nodiscard]]
        constexpr const hud::slice<char_t> as_slice() const noexcept
        {
            return hud::slice<char_t> {ptr_, length()};
        }

        /**
         * Returns a slice view of the string.
         * @return A slice representing all characters of this string.
         */
        [[nodiscard]]
        constexpr hud::slice<char_t> as_slice() noexcept
        {
            return hud::slice<char_t> {ptr_, length()};
        }

        /**
         * Provides read-only access to the code unit at a given index.
         * @param i Index of the code unit to access.
         * @return A const reference to the code unit at position i.
         */
        [[nodiscard]]
        constexpr char_t &operator[](const usize i) const noexcept
        {
            return ptr_[i];
        }

        /**
         * Constructs a ascii_string_view from a C-style string pointer.
         * @param str Pointer to a null-terminated string. Must not be null.
         */
        constexpr ascii_string_view(char_t *str) noexcept
            : ptr_(str)
        {
            HUD_CHECK(ptr_ != nullptr && "Invalid null pointer");
        }

    private:
        /**Pointer to the ascii null-terminated C-style string. */
        char_type *ptr_;
    };

    /**
     * Create a string view over a full ascii string.
     * If the given string is not ascii, the function return a nullopt.
     */
    template<typename char_t>
    requires(hud::is_same_v<hud::remove_cv_t<char_t>, char8>)
    constexpr hud::optional<hud::ascii_string_view<char_t>> make_ascii_string_view(char_t *ptr) noexcept
    {
        if (hud::encoding::is_valid_ascii(hud::slice {ptr, hud::cstring::length(ptr)})) {
            return hud::ascii_string_view(ptr);
        }
        return hud::nullopt;
    }

} // namespace hud

#endif // HD_INC_CORE_STRING_ASCII_STRING_VIEW_H