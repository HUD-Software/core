#ifndef HD_INC_CORE_CHARACTER_CHARACTER_H
#define HD_INC_CORE_CHARACTER_CHARACTER_H

namespace hud
{
    struct character
    {
        static constexpr char8 ANSI_NULL_CHARACTER = '\0';
        static constexpr wchar WIDE_NULL_CHARACTER = L'\0';
        static constexpr char16 CHAR16_NULL_CHARACTER = u'\0';
        static constexpr char32 CHAR32_NULL_CHARACTER = U'\0';

        /** Check whether the character is a pure ansi character. */
        template<typename char_t>
        requires(hud::is_one_of_types_v<char_t, char8, wchar, char16, char32>)
        static HD_FORCEINLINE constexpr bool is_ascii(const char_t character) noexcept
        {
            return (character & ~0x7F) == 0;
        }

        /** Check whether the character is a null character '\0'. */
        template<typename char_t>
        requires hud::is_one_of_types_v<char_t, char8, wchar, char16, char32>
        static HD_FORCEINLINE constexpr bool is_null(const char_t character) noexcept
        {
            if constexpr (std::is_same_v<char_t, char8>) {
                return character == ANSI_NULL_CHARACTER;
            }
            else if constexpr (std::is_same_v<char_t, wchar>) {
                return character == WIDE_NULL_CHARACTER;
            }
            else if constexpr (std::is_same_v<char_t, char16>) {
                return character == CHAR16_NULL_CHARACTER;
            }
            else if constexpr (std::is_same_v<char_t, char32>) {
                return character == CHAR32_NULL_CHARACTER;
            }
        }

        /**
         * Check if the given character is an alphanumeric character as classified by the current C locale.
         * In the default locale, the following characters are alphanumeric:
         * - digits (0123456789)
         * - uppercase letters (ABCDEFGHIJKLMNOPQRSTUVWXYZ)
         * - lowercase letters (abcdefghijklmnopqrstuvwxyz)
         * @param character The character
         * @return true if the character is an alphanumeric character, false otherwise
         */
        static HD_FORCEINLINE bool is_alphanumeric(const char8 character) noexcept
        {
            return isalnum(character) != 0;
        }

        /**
         * Check if the given wide character is an alphanumeric character as classified by the current C locale.
         * In the default locale, the following characters are alphanumeric:
         * - digits (0123456789)
         * - uppercase letters (ABCDEFGHIJKLMNOPQRSTUVWXYZ)
         * - lowercase letters (abcdefghijklmnopqrstuvwxyz)
         * @param character The character
         * @return true if the character is an alphanumeric character, false otherwise
         */
        static HD_FORCEINLINE bool is_alphanumeric(const wchar character) noexcept
        {
            return iswalnum(static_cast<wint_t>(character)) != 0;
        }

        /**
         * Check if the given character is an alphabetic character as classified by the current C locale.
         * In the default locale, the following characters are alphabetic:
         * - uppercase letters (ABCDEFGHIJKLMNOPQRSTUVWXYZ)
         * - lowercase letters (abcdefghijklmnopqrstuvwxyz)
         * @param character The character
         * @return true if the character is an alphabetic character, false otherwise
         */
        static HD_FORCEINLINE bool is_alphabetic(const char8 character) noexcept
        {
            return isalpha(character) != 0;
        }

        /**
         * Check if the given wide character is an alphabetic character as classified by the current C locale.
         * In the default locale, the following characters are alphabetic:
         * - uppercase letters (ABCDEFGHIJKLMNOPQRSTUVWXYZ)
         * - lowercase letters (abcdefghijklmnopqrstuvwxyz)
         * @param character The character
         * @return true if the character is an alphabetic character, false otherwise
         */
        static HD_FORCEINLINE bool is_alphabetic(const wchar character) noexcept
        {
            return iswalpha(static_cast<wint_t>(character)) != 0;
        }

        /**
         * Check if the given character is a lowercase character according to the current C locale.
         * In the default locale, the following characters are lowercase letters (abcdefghijklmnopqrstuvwxyz).
         * @param character The character
         * @return true if the character is an lowercase character, false otherwise
         */
        static HD_FORCEINLINE bool is_lowercase(const char8 character) noexcept
        {
            return islower(character) != 0;
        }

        /**
         * Check if the given wide character is a lowercase character according to the current C locale.
         * In the default locale, the following characters are lowercase letters (abcdefghijklmnopqrstuvwxyz).
         * @param character The character
         * @return true if the character is an lowercase character, false otherwise
         */
        static HD_FORCEINLINE bool is_lowercase(const wchar character) noexcept
        {
            return iswlower(static_cast<wint_t>(character)) != 0;
        }

        /**
         * Check if the given character is a uppercase character according to the current C locale.
         * In the default locale, the following characters are uppercase letters (ABCDEFGHIJKLMNOPQRSTUVWXYZ).
         * @param character The character
         * @return true if the character is an uppercase character, false otherwise
         */
        static HD_FORCEINLINE bool is_uppercase(const char8 character) noexcept
        {
            return isupper(character) != 0;
        }

        /**
         * Check if the given wide character is a uppercase character according to the current C locale.
         * In the default locale, the following characters are uppercase letters (ABCDEFGHIJKLMNOPQRSTUVWXYZ).
         * @param character The character
         * @return true if the character is an uppercase character, false otherwise
         */
        static HD_FORCEINLINE bool is_uppercase(const wchar character) noexcept
        {
            return iswupper(static_cast<wint_t>(character)) != 0;
        }

        /** Check if the given character is a digit (0123456789). */
        static HD_FORCEINLINE bool is_digit(const char8 character) noexcept
        {
            return isdigit(character) != 0;
        }

        /** Check if the wide given character is a digit (0123456789). */
        static HD_FORCEINLINE bool is_digit(const wchar character) noexcept
        {
            return iswdigit(static_cast<wint_t>(character)) != 0;
        }

        /** Check if the given character is a hexadecimal (0123456789abcdefABCDEF). */
        static HD_FORCEINLINE bool is_hexa(const char8 character) noexcept
        {
            return isxdigit(character) != 0;
        }

        /** Check if the given wide character is a hexadecimal (0123456789abcdefABCDEF). */
        static HD_FORCEINLINE bool is_hexa(const wchar character) noexcept
        {
            return iswxdigit(static_cast<wint_t>(character)) != 0;
        }

        /** Check if the given character is a space or a horizontal tab according to the current C locale. */
        static HD_FORCEINLINE bool is_space_or_tab(const char8 character) noexcept
        {
            return isblank(character) != 0;
        }

        /** Check if the given wide character is a space or a horizontal tab according to the current C locale. */
        static HD_FORCEINLINE bool is_space_or_tab(const wchar character) noexcept
        {
            return iswblank(static_cast<wint_t>(character)) != 0;
        }

        /** Check if the given character is a control character according to the current C locale. */
        static HD_FORCEINLINE bool is_control(const char8 character) noexcept
        {
            return iscntrl(character) != 0;
        }

        /** Check if the given wide character is a control character according to the current C locale. */
        static HD_FORCEINLINE bool is_control(const wchar character) noexcept
        {
            return iswcntrl(static_cast<wint_t>(character)) != 0;
        }

        /** Check if the given character is a space. */
        static HD_FORCEINLINE bool is_space(const char8 character) noexcept
        {
            return isspace(character) != 0;
        }

        /** Check if the given wide character is a space. */
        static HD_FORCEINLINE bool is_space(const wchar character) noexcept
        {
            return iswspace(static_cast<wint_t>(character)) != 0;
        }

        /** Check if the given character is a punctuation (!"#$%&'()*+,-./:;<=>?@[\]^_`{|}~) according to the current C locale. */
        static HD_FORCEINLINE bool is_punctuation(const char8 character) noexcept
        {
            return ispunct(character) != 0;
        }

        /** Check if the given wide character is a punctuation (!"#$%&'()*+,-./:;<=>?@[\]^_`{|}~) according to the current C locale. */
        static HD_FORCEINLINE bool is_punctuation(const wchar character) noexcept
        {
            return iswpunct(static_cast<wint_t>(character)) != 0;
        }

        /** Converts the given character to lowercase according to the character conversion rules defined by the currently installed C locale. */
        static HD_FORCEINLINE char8 to_lowercase(const char8 character) noexcept
        {
            return static_cast<char8>(tolower(character));
        }

        /** Converts the given wide character to lowercase according to the wide character conversion rules defined by the currently installed C locale. */
        static HD_FORCEINLINE wchar to_lowercase(const wchar character) noexcept
        {
            return static_cast<wchar>(towlower(static_cast<wint_t>(character)));
        }

        /** Converts the given character to uppercase according to the character conversion rules defined by the currently installed C locale. */
        static HD_FORCEINLINE char8 to_uppercase(const char8 character) noexcept
        {
            return static_cast<char8>(toupper(character));
        }

        /** Converts the given wide character to uppercase according to the wide character conversion rules defined by the currently installed C locale. */
        static HD_FORCEINLINE wchar to_uppercase(const wchar character) noexcept
        {
            return static_cast<wchar>(towupper(static_cast<wint_t>(character)));
        }
    };
} // namespace hud

#endif // HD_INC_CORE_CHARACTER_CHARACTER_H