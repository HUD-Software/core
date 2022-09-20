#include <core/char.h>

static constexpr ansichar Digits[] = { '0' , '1' ,'2', '3', '4', '5', '6', '7', '8', '9' };
static constexpr wchar WideDigits[] = { L'0' , L'1' , L'2', L'3', L'4', L'5', L'6', L'7', L'8', L'9' };
static constexpr ansichar LowerLetter[] = { 'a' , 'b' ,'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z' };
static constexpr wchar WideLowerLetter[] = { L'a' , L'b' , L'c', L'd', L'e', L'f', L'g', L'h', L'i', L'j', L'k', L'l', L'm', L'n', L'o', L'p', L'q', L'r', L's', L't', L'u', L'v', L'w', L'x', L'y', L'z' };
static constexpr ansichar HigherLetter[] = { 'A' , 'B' ,'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'type_t', 'U', 'V', 'W', 'X', 'Y', 'Z' };
static constexpr wchar WideHigherLetter[] = { L'A' , L'B' , L'C', L'D', L'E', L'F', L'G', L'H', L'I', L'J', L'K', L'L', L'M', L'N', L'O', L'P', L'Q', L'R', L'S', L'type_t', L'U', L'V', L'W', L'X', L'Y', L'Z' };
static constexpr ansichar Hexa[] = { '0' , '1' ,'2', '3', '4', '5', '6', '7', '8', '9', 'a' , 'b' ,'c', 'd', 'e', 'f', 'A' , 'B' ,'C', 'D', 'E', 'F' };
static constexpr wchar WideHexa[] = { L'0' , L'1' , L'2', L'3', L'4', L'5', L'6', L'7', L'8', L'9', L'a' , L'b' , L'c', L'd', L'e', L'f', L'A' , L'B' , L'C', L'D', L'E', L'F' };
static constexpr ansichar SpaceAndTab[] = { ' ', '\t' };
static constexpr wchar WideSpaceAndTab[] = { L' ', L'\t' };
static constexpr ansichar Control[] = { '\x0', '\x1' , '\x2', '\x3', '\x4', '\x5', '\x6', '\x7', '\x8', '\x9', '\xA','\xB', '\xC', '\xD', '\xE', '\xF', '\x7F', '\x10', '\x11', '\x12' , '\x13', '\x14', '\x15', '\x16', '\x17', '\x18', '\x19', '\x1A', '\x1B', '\x1C', '\x1D', '\x1E', '\x1F' };
static constexpr wchar WideControl[] = { L'\x0', L'\x1' , L'\x2', L'\x3', L'\x4', L'\x5', L'\x6', L'\x7', L'\x8', L'\x9', L'\xA', L'\xB', L'\xC', L'\xD', L'\xE', L'\xF', L'\x7F', L'\x10', L'\x11', L'\x12' , L'\x13', L'\x14', L'\x15', L'\x16', L'\x17', L'\x18', L'\x19', L'\x1A', L'\x1B', L'\x1C', L'\x1D', L'\x1E', L'\x1F' };
static constexpr ansichar Punc[] = { '!', '\"', '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',', '-', '.', '/', ':', ';' ,'<', '=', '>', '?', '@', '[', '\\', ']', '^', '_', '`', '{', '|', '}', '~' };
static constexpr ansichar WidePunc[] = { L'!', L'\"', L'#', L'$', L'%', L'&', L'\'', L'(', L')', L'*', L'+', L',', L'-', L'.', L'/', L':', L';' , L'<', L'=', L'>', L'?', L'@', L'[', L'\\', L']', L'^', L'_', L'`', L'{', L'|', L'}', L'~' };


TEST(Char, is_pure_ansi)
{


    for (ansichar cur = 0; cur < hud::ansichar_max; cur++) {
        ASSERT_TRUE(Char::is_pure_ansi(cur));
    }
    for (wchar cur = 0; cur < hud::wchar_max; cur++) {
        if (cur <= hud::ansichar_max) {
            ASSERT_TRUE(Char::is_pure_ansi(cur));
        }
        else {
            ASSERT_FALSE(Char::is_pure_ansi(cur));
        }
    }
}

TEST(Char, is_null)
{


    ASSERT_TRUE(Char::is_null('\0'));
    ASSERT_FALSE(Char::is_null(' '));
    ASSERT_TRUE(Char::is_null(L'\0'));
    ASSERT_FALSE(Char::is_null(L' '));
}

TEST(Char, is_alphanumeric) 
{


    for (const auto& alphanumeric : Digits) {
        ASSERT_TRUE(Char::is_alphanumeric(alphanumeric));
    }
    for (const auto& alphanumeric : WideDigits) {
        ASSERT_TRUE(Char::is_alphanumeric(alphanumeric));
    }

    for (const auto& alphanumeric : LowerLetter) {
        ASSERT_TRUE(Char::is_alphanumeric(alphanumeric));
    }
    for (const auto& alphanumeric : WideLowerLetter) {
        ASSERT_TRUE(Char::is_alphanumeric(alphanumeric));
    }

    for (const auto& alphanumeric : WideHigherLetter) {
        ASSERT_TRUE(Char::is_alphanumeric(alphanumeric));
    }
    for (const auto& alphanumeric : HigherLetter) {
        ASSERT_TRUE(Char::is_alphanumeric(alphanumeric));
    }

    for (const auto& no_alphanumeric : SpaceAndTab) {
        ASSERT_FALSE(Char::is_alphanumeric(no_alphanumeric));
    }
    for (const auto& no_alphanumeric : WideSpaceAndTab) {
        ASSERT_FALSE(Char::is_alphanumeric(no_alphanumeric));
    }

    for (const auto& no_alphanumeric : Control) {
        ASSERT_FALSE(Char::is_alphanumeric(no_alphanumeric));
    }
    for (const auto& no_alphanumeric : WideControl) {
        ASSERT_FALSE(Char::is_alphanumeric(no_alphanumeric));
    }

    for (const auto& no_alphanumeric : Punc) {
        ASSERT_FALSE(Char::is_alphanumeric(no_alphanumeric));
    }
    for (const auto& no_alphanumeric : WidePunc) {
        ASSERT_FALSE(Char::is_alphanumeric(no_alphanumeric));
    }
}   

TEST(Char, is_alphabetic)
{


    for (const auto& no_alphabetic : Digits) {
        ASSERT_FALSE(Char::is_alphabetic(no_alphabetic));
    }
    for (const auto& no_alphabetic : WideDigits) {
        ASSERT_FALSE(Char::is_alphabetic(no_alphabetic));
    }

    for (const auto& alphabetic : LowerLetter) {
        ASSERT_TRUE(Char::is_alphabetic(alphabetic));
    }
    for (const auto& alphabetic : HigherLetter) {
        ASSERT_TRUE(Char::is_alphabetic(alphabetic));
    }

    for (const auto& alphabetic : WideLowerLetter) {
        ASSERT_TRUE(Char::is_alphabetic(alphabetic));
    }
    for (const auto& alphabetic : WideHigherLetter) {
        ASSERT_TRUE(Char::is_alphabetic(alphabetic));
    }

    for (const auto& no_alphabetic : SpaceAndTab) {
        ASSERT_FALSE(Char::is_alphabetic(no_alphabetic));
    }
    for (const auto& no_alphabetic : WideSpaceAndTab) {
        ASSERT_FALSE(Char::is_alphabetic(no_alphabetic));
    }

    for (const auto& no_alphabetic : Control) {
        ASSERT_FALSE(Char::is_alphabetic(no_alphabetic));
    }
    for (const auto& no_alphabetic : WideControl) {
        ASSERT_FALSE(Char::is_alphabetic(no_alphabetic));
    }

    for (const auto& no_alphabetic : Punc) {
        ASSERT_FALSE(Char::is_alphabetic(no_alphabetic));
    }
    for (const auto& no_alphabetic : WidePunc) {
        ASSERT_FALSE(Char::is_alphabetic(no_alphabetic));
    }
}


TEST(Char, is_lowercase)
{


    for (const auto& no_lowercase : Digits) {
        ASSERT_FALSE(Char::is_lowercase(no_lowercase));
    }
    for (const auto& no_lowercase : WideDigits) {
        ASSERT_FALSE(Char::is_lowercase(no_lowercase));
    }

    for (const auto& lowercase : LowerLetter) {
        ASSERT_TRUE(Char::is_lowercase(lowercase));
    }
    for (const auto& no_lowercase : HigherLetter) {
        ASSERT_FALSE(Char::is_lowercase(no_lowercase));
    }

    for (const auto& lowercase : WideLowerLetter) {
        ASSERT_TRUE(Char::is_lowercase(lowercase));
    }
    for (const auto& no_lowercase : WideHigherLetter) {
        ASSERT_FALSE(Char::is_lowercase(no_lowercase));
    }

    for (const auto& no_lowercase : SpaceAndTab) {
        ASSERT_FALSE(Char::is_lowercase(no_lowercase));
    }
    for (const auto& no_lowercase : WideSpaceAndTab) {
        ASSERT_FALSE(Char::is_lowercase(no_lowercase));
    }

    for (const auto& no_lowercase : Control) {
        ASSERT_FALSE(Char::is_lowercase(no_lowercase));
    }
    for (const auto& no_lowercase : WideControl) {
        ASSERT_FALSE(Char::is_lowercase(no_lowercase));
    }

    for (const auto& no_lowercase : Punc) {
        ASSERT_FALSE(Char::is_lowercase(no_lowercase));
    }
    for (const auto& no_lowercase : WidePunc) {
        ASSERT_FALSE(Char::is_lowercase(no_lowercase));
    }
}


TEST(Char, is_uppercase)
{


    for (const auto& no_uppercase : Digits) {
        ASSERT_FALSE(Char::is_uppercase(no_uppercase));
    }
    for (const auto& no_uppercase : WideDigits) {
        ASSERT_FALSE(Char::is_uppercase(no_uppercase));
    }

    for (const auto& no_uppercase : LowerLetter) {
        ASSERT_FALSE(Char::is_uppercase(no_uppercase));
    }
    for (const auto& uppercase : HigherLetter) {
        ASSERT_TRUE(Char::is_uppercase(uppercase));
    }

    for (const auto& no_uppercase : WideLowerLetter) {
        ASSERT_FALSE(Char::is_uppercase(no_uppercase));
    }
    for (const auto& uppercase : WideHigherLetter) {
        ASSERT_TRUE(Char::is_uppercase(uppercase));
    }

    for (const auto& no_uppercase : SpaceAndTab) {
        ASSERT_FALSE(Char::is_uppercase(no_uppercase));
    }
    for (const auto& no_uppercase : WideSpaceAndTab) {
        ASSERT_FALSE(Char::is_uppercase(no_uppercase));
    }

    for (const auto& no_uppercase : Control) {
        ASSERT_FALSE(Char::is_uppercase(no_uppercase));
    }
    for (const auto& no_uppercase : WideControl) {
        ASSERT_FALSE(Char::is_uppercase(no_uppercase));
    }

    for (const auto& no_uppercase : Punc) {
        ASSERT_FALSE(Char::is_uppercase(no_uppercase));
    }
    for (const auto& no_uppercase : WidePunc) {
        ASSERT_FALSE(Char::is_uppercase(no_uppercase));
    }
}

TEST(Char, is_digit)
{


    for (const auto& digit : Digits) {
        ASSERT_TRUE(Char::is_digit(digit));
    }
    for (const auto& digit : WideDigits) {
        ASSERT_TRUE(Char::is_digit(digit));
    }

    for (const auto& no_digit : LowerLetter) {
        ASSERT_FALSE(Char::is_digit(no_digit));
    }
    for (const auto& no_digit : HigherLetter) {
        ASSERT_FALSE(Char::is_digit(no_digit));
    }

    for (const auto& no_digit : WideLowerLetter) {
        ASSERT_FALSE(Char::is_digit(no_digit));
    }
    for (const auto& no_digit : WideHigherLetter) {
        ASSERT_FALSE(Char::is_digit(no_digit));
    }

    for (const auto& no_digit : SpaceAndTab) {
        ASSERT_FALSE(Char::is_digit(no_digit));
    }
    for (const auto& no_digit : WideSpaceAndTab) {
        ASSERT_FALSE(Char::is_digit(no_digit));
    }

    for (const auto& no_digit : Control) {
        ASSERT_FALSE(Char::is_digit(no_digit));
    }
    for (const auto& no_digit : WideControl) {
        ASSERT_FALSE(Char::is_digit(no_digit));
    }

    for (const auto& no_digit : Punc) {
        ASSERT_FALSE(Char::is_digit(no_digit));
    }
    for (const auto& no_digit : WidePunc) {
        ASSERT_FALSE(Char::is_digit(no_digit));
    }
}

TEST(Char, is_hexa)
{


    static constexpr ansichar NoHexa[] = { 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z' };
    static constexpr wchar WideNoHexa[] = { L'G', L'H', L'I', L'J', L'K', L'L', L'M', L'N', L'O', L'P', L'Q', L'R', L'S', L'type_t', L'U', L'V', L'W', L'X', L'Y', L'Z' };

    for (const auto& hexa : Hexa) {
        ASSERT_TRUE(Char::is_hexa(hexa));
    }
    for (const auto& hexa : WideHexa) {
        ASSERT_TRUE(Char::is_hexa(hexa));
    }

    for (const auto& no_hexa : NoHexa) {
        ASSERT_FALSE(Char::is_hexa(no_hexa));
    }
    for (const auto& no_hexa : WideNoHexa) {
        ASSERT_FALSE(Char::is_hexa(no_hexa));
    }

    for (const auto& no_hexa : SpaceAndTab) {
        ASSERT_FALSE(Char::is_hexa(no_hexa));
    }
    for (const auto& no_hexa : WideSpaceAndTab) {
        ASSERT_FALSE(Char::is_hexa(no_hexa));
    }

    for (const auto& no_hexa : Control) {
        ASSERT_FALSE(Char::is_hexa(no_hexa));
    }
    for (const auto& no_hexa : WideControl) {
        ASSERT_FALSE(Char::is_hexa(no_hexa));
    }

    for (const auto& no_hexa : Punc) {
        ASSERT_FALSE(Char::is_hexa(no_hexa));
    }
    for (const auto& no_hexa : WidePunc) {
        ASSERT_FALSE(Char::is_hexa(no_hexa));
    }
}

TEST(Char, is_space_or_tab)
{


    for (const auto& no_space_or_tab : Digits) {
        ASSERT_FALSE(Char::is_space_or_tab(no_space_or_tab));
    }
    for (const auto& no_space_or_tab : WideDigits) {
        ASSERT_FALSE(Char::is_space_or_tab(no_space_or_tab));
    }

    for (const auto& no_space_or_tab : LowerLetter) {
        ASSERT_FALSE(Char::is_space_or_tab(no_space_or_tab));
    }
    for (const auto& no_space_or_tab : WideLowerLetter) {
        ASSERT_FALSE(Char::is_space_or_tab(no_space_or_tab));
    }

    for (const auto& no_space_or_tab : HigherLetter) {
        ASSERT_FALSE(Char::is_space_or_tab(no_space_or_tab));
    }
    for (const auto& no_space_or_tab : WideHigherLetter) {
        ASSERT_FALSE(Char::is_space_or_tab(no_space_or_tab));
    }

    for (const auto& space_or_tab : SpaceAndTab) {
        ASSERT_TRUE(Char::is_space_or_tab(space_or_tab));
    }
    for (const auto& space_or_tab : WideSpaceAndTab) {
        ASSERT_TRUE(Char::is_space_or_tab(space_or_tab));
    }

    for (const auto& no_space_or_tab : Control) {
        if (no_space_or_tab == ' ' || no_space_or_tab == '\t') {
            ASSERT_TRUE(Char::is_space_or_tab(no_space_or_tab));
        }
        else {
            ASSERT_FALSE(Char::is_space_or_tab(no_space_or_tab));
        }
    }
    for (const auto& no_space_or_tab : WideControl) {
        if (no_space_or_tab == L' ' || no_space_or_tab == L'\t') {
            ASSERT_TRUE(Char::is_space_or_tab(no_space_or_tab));
        }
        else {
            ASSERT_FALSE(Char::is_space_or_tab(no_space_or_tab));
        }
    }

    for (const auto& no_space_or_tab : Punc) {
        ASSERT_FALSE(Char::is_space_or_tab(no_space_or_tab));
    }
    for (const auto& no_space_or_tab : WidePunc) {
        ASSERT_FALSE(Char::is_space_or_tab(no_space_or_tab));
    }

}


TEST(Char, is_control)
{


    for (const auto& no_control : Digits) {
        ASSERT_FALSE(Char::is_control(no_control));
    }
    for (const auto& no_control : WideDigits) {
        ASSERT_FALSE(Char::is_control(no_control));
    }

    for (const auto& no_control : LowerLetter) {
        ASSERT_FALSE(Char::is_control(no_control));
    }
    for (const auto& no_control : WideLowerLetter) {
        ASSERT_FALSE(Char::is_control(no_control));
    }

    for (const auto& no_control : HigherLetter) {
        ASSERT_FALSE(Char::is_control(no_control));
    }
    for (const auto& no_control : WideHigherLetter) {
        ASSERT_FALSE(Char::is_control(no_control));
    }

    for (const auto& control : Control) {
        ASSERT_TRUE(Char::is_control(control));
    }
    for (const auto& control : WideControl) {
        ASSERT_TRUE(Char::is_control(control));
    }

    for (const auto& no_control : Punc) {
        ASSERT_FALSE(Char::is_control(no_control));
    }
    for (const auto& no_control : WidePunc) {
        ASSERT_FALSE(Char::is_control(no_control));
    }
}

TEST(Char, is_space)
{


    static constexpr ansichar Space[] = { ' ', '\x20', '\x09', '\xA', '\xB', '\xC', '\xD' };
    static constexpr wchar WideSpace[] = { L' ', L'\x20', L'\x09', L'\xA', L'\xB', L'\xC', L'\xD' };

    for (const auto& no_space : Digits) {
        ASSERT_FALSE(Char::is_space(no_space));
    }
    for (const auto& no_space : WideDigits) {
        ASSERT_FALSE(Char::is_space(no_space));
    }

    for (const auto& no_space : LowerLetter) {
        ASSERT_FALSE(Char::is_space(no_space));
    }
    for (const auto& no_space : WideLowerLetter) {
        ASSERT_FALSE(Char::is_space(no_space));
    }

    for (const auto& no_space : HigherLetter) {
        ASSERT_FALSE(Char::is_space(no_space));
    }
    for (const auto& no_space : WideHigherLetter) {
        ASSERT_FALSE(Char::is_space(no_space));
    }

    for (const auto& no_space : Control) {
        bool in_both = false;
        for (const auto& space : Space) {
            if (space == no_space) {
                in_both = true;
                break;
            }
        }
        if (in_both) {
            ASSERT_TRUE(Char::is_space(no_space));
        }
        else {
            ASSERT_FALSE(Char::is_space(no_space));
        }
    }
    for (const auto& no_space : WideControl) {
        bool in_both = false;
        for (const auto& space : WideSpace) {
            if (space == no_space) {
                in_both = true;
                break;
            }
        }
        if (in_both) {
            ASSERT_TRUE(Char::is_space(no_space));
        }
        else {
            ASSERT_FALSE(Char::is_space(no_space));
        }
    }

    for (const auto& space : Space) {
        ASSERT_TRUE(Char::is_space(space));
    }
    for (const auto& space : WideSpace) {
        ASSERT_TRUE(Char::is_space(space));
    }
    for (const auto& no_space : Punc) {
        ASSERT_FALSE(Char::is_space(no_space));
    }
    for (const auto& no_space : WidePunc) {
        ASSERT_FALSE(Char::is_space(no_space));
    }
}

TEST(Char, is_punctuation)
{


    for (const auto& no_punctuation : Digits) {
        ASSERT_FALSE(Char::is_punctuation(no_punctuation));
    }
    for (const auto& no_punctuation : WideDigits) {
        ASSERT_FALSE(Char::is_punctuation(no_punctuation));
    }

    for (const auto& no_punctuation : LowerLetter) {
        ASSERT_FALSE(Char::is_punctuation(no_punctuation));
    }
    for (const auto& no_punctuation : WideLowerLetter) {
        ASSERT_FALSE(Char::is_punctuation(no_punctuation));
    }

    for (const auto& no_punctuation : HigherLetter) {
        ASSERT_FALSE(Char::is_punctuation(no_punctuation));
    }
    for (const auto& no_punctuation : WideHigherLetter) {
        ASSERT_FALSE(Char::is_punctuation(no_punctuation));
    }

    for (const auto& no_punctuation : SpaceAndTab) {
        ASSERT_FALSE(Char::is_hexa(no_punctuation));
    }
    for (const auto& no_punctuation : WideSpaceAndTab) {
        ASSERT_FALSE(Char::is_hexa(no_punctuation));
    }

    for (const auto& no_punctuation : Control) {
        ASSERT_FALSE(Char::is_hexa(no_punctuation));
    }
    for (const auto& no_punctuation : WideControl) {
        ASSERT_FALSE(Char::is_hexa(no_punctuation));
    }

    for (const auto& punctuation : Punc) {
        ASSERT_TRUE(Char::is_punctuation(punctuation));
    }
    for (const auto& punctuation : WidePunc) {
        ASSERT_TRUE(Char::is_punctuation(punctuation));
    }
}

TEST(Char, to_lowercase)
{


    for (const auto& no_tolower : Digits) {
        ASSERT_TRUE(Char::to_lowercase(no_tolower) == no_tolower);
    }

    const usize count = sizeof(LowerLetter) / sizeof(LowerLetter[0]);
    for (usize at = 0; at < count; at++) {
        ASSERT_TRUE(Char::to_lowercase(LowerLetter[at]) == LowerLetter[at]);
    }
    for (usize at = 0; at < count; at++) {
        ASSERT_TRUE(Char::to_lowercase(HigherLetter[at]) == LowerLetter[at]);
    }

    for (usize at = 0; at < count; at++) {
        ASSERT_TRUE(Char::to_lowercase(WideLowerLetter[at]) == WideLowerLetter[at]);
    }
    for (usize at = 0; at < count; at++) {
        ASSERT_TRUE(Char::to_lowercase(WideHigherLetter[at]) == WideLowerLetter[at]);
    }

    for (const auto& no_tolower : Control) {
        ASSERT_TRUE(Char::to_lowercase(no_tolower) == no_tolower);
    }
    for (const auto& no_tolower : WideControl) {
        ASSERT_TRUE(Char::to_lowercase(no_tolower) == no_tolower);
    }

    for (const auto& no_tolower : SpaceAndTab) {
        ASSERT_TRUE(Char::to_lowercase(no_tolower) == no_tolower);
    }
    for (const auto& no_tolower : WideSpaceAndTab) {
        ASSERT_TRUE(Char::to_lowercase(no_tolower) == no_tolower);
    }

    for (const auto& no_tolower : Punc) {
        ASSERT_TRUE(Char::to_lowercase(no_tolower) == no_tolower);
    }
    for (const auto& no_tolower : WidePunc) {
        ASSERT_TRUE(Char::to_lowercase(no_tolower) == no_tolower);
    }
}

TEST(Char, to_uppercase)
{


    for (const auto& no_tolower : Digits) {
        ASSERT_TRUE(Char::to_uppercase(no_tolower) == no_tolower);
    }

    const usize count = sizeof(LowerLetter) / sizeof(LowerLetter[0]);
    for (usize at = 0; at < count; at++) {
        ASSERT_TRUE(Char::to_uppercase(LowerLetter[at]) == HigherLetter[at]);
    }
    for (usize at = 0; at < count; at++) {
        ASSERT_TRUE(Char::to_uppercase(HigherLetter[at]) == HigherLetter[at]);
    }

    for (usize at = 0; at < count; at++) {
        ASSERT_TRUE(Char::to_uppercase(WideLowerLetter[at]) == WideHigherLetter[at]);
    }
    for (usize at = 0; at < count; at++) {
        ASSERT_TRUE(Char::to_uppercase(WideHigherLetter[at]) == WideHigherLetter[at]);
    }

    for (const auto& no_tolower : Control) {
        ASSERT_TRUE(Char::to_uppercase(no_tolower) == no_tolower);
    }
    for (const auto& no_tolower : WideControl) {
        ASSERT_TRUE(Char::to_uppercase(no_tolower) == no_tolower);
    }

    for (const auto& no_tolower : SpaceAndTab) {
        ASSERT_TRUE(Char::to_uppercase(no_tolower) == no_tolower);
    }
    for (const auto& no_tolower : WideSpaceAndTab) {
        ASSERT_TRUE(Char::to_uppercase(no_tolower) == no_tolower);
    }

    for (const auto& no_tolower : Punc) {
        ASSERT_TRUE(Char::to_uppercase(no_tolower) == no_tolower);
    }
    for (const auto& no_tolower : WidePunc) {
        ASSERT_TRUE(Char::to_uppercase(no_tolower) == no_tolower);
    }
}