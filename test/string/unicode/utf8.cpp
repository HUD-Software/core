#include <core/string/unicode/utf8.h>
#include <core/slice.h>
#include "../lipsum/latin_lipsum.h"
#include "../lipsum/russian_lipsum.h"
#include "../lipsum/korean_lipsum.h"
#include "../lipsum/japanese_lipsum.h"
#include "../lipsum/hindi_lipsum.h"
#include "../lipsum/hebrew_lipsum.h"
#include "../lipsum/emoji_lipsum.h"
#include "../lipsum/chinese_lipsum.h"
#include "../lipsum/arabic_lipsum.h"

GTEST_TEST(utf8, is_valid_utf8_portable)
{
    // Test with values from
    // https://github.com/lemire/unicode_lipsum
    const auto test_latin = []() {
        const char8 latin_lipsum[] = LATIN_LIPSUM;
        return hud::unicode::is_valid_utf8_portable(hud::slice {LATIN_LIPSUM, sizeof(latin_lipsum)});
    };
    const auto test_russian = []() {
        const char8 russian_lipsum[] = RUSSIAN_LIPSUM;
        return hud::unicode::is_valid_utf8_portable(hud::slice {RUSSIAN_LIPSUM, sizeof(russian_lipsum)});
    };
    const auto test_korean = []() {
        const char8 korean_lipsum[] = KOREAN_LIPSUM;
        return hud::unicode::is_valid_utf8_portable(hud::slice {KOREAN_LIPSUM, sizeof(korean_lipsum)});
    };
    const auto test_japanese = []() {
        const char8 japanese_lipsum[] = JAPANESE_LIPSUM;
        return hud::unicode::is_valid_utf8_portable(hud::slice {JAPANESE_LIPSUM, sizeof(japanese_lipsum)});
    };
    const auto test_hindi = []() {
        const char8 hindi_lipsum[] = HINDI_LIPSUM;
        return hud::unicode::is_valid_utf8_portable(hud::slice {HINDI_LIPSUM, sizeof(hindi_lipsum)});
    };
    const auto test_hebrew = []() {
        const char8 hebrew_lipsum[] = HEBREW_LIPSUM;
        return hud::unicode::is_valid_utf8_portable(hud::slice {HEBREW_LIPSUM, sizeof(hebrew_lipsum)});
    };
    const auto test_emoji = []() {
        const char8 emoji_lipsum[] = EMOJI_LIPSUM;
        return hud::unicode::is_valid_utf8_portable(hud::slice {EMOJI_LIPSUM, sizeof(emoji_lipsum)});
    };
    const auto test_chinese = []() {
        const char8 chinese_lipsum[] = CHINESE_LIPSUM;
        return hud::unicode::is_valid_utf8_portable(hud::slice {CHINESE_LIPSUM, sizeof(chinese_lipsum)});
    };
    const auto test_arabic = []() {
        const char8 arabic_lipsum[] = ARABIC_LIPSUM;
        return hud::unicode::is_valid_utf8_portable(hud::slice {ARABIC_LIPSUM, sizeof(arabic_lipsum)});
    };
    // Non constant
    {
        const auto result_latin = test_latin();
        hud_assert_true(result_latin);
        const auto result_russian = test_russian();
        hud_assert_true(result_russian);
        const auto result_korean = test_korean();
        hud_assert_true(result_korean);
        const auto result_japanese = test_japanese();
        hud_assert_true(result_japanese);
        const auto result_hindi = test_hindi();
        hud_assert_true(result_hindi);
        const auto result_hebrew = test_hebrew();
        hud_assert_true(result_hebrew);
        const auto result_emoji = test_emoji();
        hud_assert_true(result_emoji);
        const auto result_chinese = test_chinese();
        hud_assert_true(result_chinese);
        const auto result_arabic = test_arabic();
        hud_assert_true(result_arabic);
    }

    // Constant
    {
        const auto result_latin = test_latin();
        hud_assert_true(result_latin);
        const auto result_russian = test_russian();
        hud_assert_true(result_russian);
        constexpr auto result_korean = test_korean();
        hud_assert_true(result_korean);
        constexpr auto result_japanese = test_japanese();
        hud_assert_true(result_japanese);
        constexpr auto result_hindi = test_hindi();
        hud_assert_true(result_hindi);
        constexpr auto result_hebrew = test_hebrew();
        hud_assert_true(result_hebrew);
        constexpr auto result_emoji = test_emoji();
        hud_assert_true(result_emoji);
        constexpr auto result_chinese = test_chinese();
        hud_assert_true(result_chinese);
        constexpr auto result_arabic = test_arabic();
        hud_assert_true(result_arabic);
    }
}