
#include <core/string.h>
#include <core/memory.h>
#include "lipsum/arabic_lipsum.h"
#include "lipsum/chinese_lipsum.h"
#include "lipsum/emoji_lipsum.h"
#include "lipsum/hebrew_lipsum.h"
#include "lipsum/hindi_lipsum.h"
#include "lipsum/japanese_lipsum.h"
#include "lipsum/korean_lipsum.h"
#include "lipsum/latin_lipsum.h"
#include "lipsum/russian_lipsum.h"
GTEST_TEST(string, default_constructor_should_allocate_no_memory)
{
    auto test = []() {
        hud::string s;
        return std::tuple {
            s.bytes() == nullptr,
            s.byte_count() == 0u,
            s.max_byte_count() == 0u
        };
    };

    // Non Constant
    {
        auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
    }
}

GTEST_TEST(string, constructor_from_char8)
{
    auto test = []() {
        // Supported null character
        const char8 txt[] = "é&='\0/1234564789";
        hud::optional<hud::string> str_opt = hud::make_string(txt, sizeof(txt));
        // Arabic
        const char8 txt_arabic[] = ARABIC_LIPSUM;
        hud::optional<hud::string> str_arabic_opt = hud::make_string(txt_arabic, sizeof(txt_arabic));
        // Chinese
        const char8 txt_chinese[] = CHINESE_LIPSUM;
        hud::optional<hud::string> str_chinese_opt = hud::make_string(txt_chinese, sizeof(txt_chinese));
        // Emoji
        const char8 txt_emoji[] = EMOJI_LIPSUM;
        hud::optional<hud::string> str_emoji_opt = hud::make_string(txt_emoji, sizeof(txt_emoji));
        // Hebrew
        const char8 txt_hebrew[] = HEBREW_LIPSUM;
        hud::optional<hud::string> str_hebrew_opt = hud::make_string(txt_hebrew, sizeof(txt_hebrew));
        // Hindi
        const char8 txt_hindi[] = HINDI_LIPSUM;
        hud::optional<hud::string> str_hindi_opt = hud::make_string(txt_hindi, sizeof(txt_hindi));
        // Japanese
        const char8 txt_japanese[] = JAPANESE_LIPSUM;
        hud::optional<hud::string> str_japanese_opt = hud::make_string(txt_japanese, sizeof(txt_japanese));
        // Korean
        const char8 txt_korean[] = KOREAN_LIPSUM;
        hud::optional<hud::string> str_korean_opt = hud::make_string(txt_korean, sizeof(txt_korean));
        // Latin
        const char8 txt_latin[] = LATIN_LIPSUM;
        hud::optional<hud::string> str_latin_opt = hud::make_string(txt_latin, sizeof(txt_latin));
        // Russian
        const char8 txt_russian[] = RUSSIAN_LIPSUM;
        hud::optional<hud::string> str_russian_opt = hud::make_string(txt_russian, sizeof(txt_russian));
        return std::tuple {
            str_opt.has_value(),                                                                                       // 0
            str_opt->byte_count() == sizeof(txt),                                                                      // 1
            str_opt->max_byte_count() == sizeof(txt),                                                                  // 2
            hud::memory::is_memory_compare_equal(str_opt->bytes(), txt, sizeof(txt)),                                  // 3
            str_arabic_opt.has_value(),                                                                                // 4
            str_arabic_opt->byte_count() == sizeof(ARABIC_LIPSUM),                                                     // 5
            str_arabic_opt->max_byte_count() == sizeof(ARABIC_LIPSUM),                                                 // 6
            hud::memory::is_memory_compare_equal(str_arabic_opt->bytes(), ARABIC_LIPSUM, sizeof(ARABIC_LIPSUM)),       // 7
            str_chinese_opt.has_value(),                                                                               // 8
            str_chinese_opt->byte_count() == sizeof(CHINESE_LIPSUM),                                                   // 9
            str_chinese_opt->max_byte_count() == sizeof(CHINESE_LIPSUM),                                               // 10
            hud::memory::is_memory_compare_equal(str_chinese_opt->bytes(), CHINESE_LIPSUM, sizeof(CHINESE_LIPSUM)),    // 11
            str_emoji_opt.has_value(),                                                                                 // 12
            str_emoji_opt->byte_count() == sizeof(EMOJI_LIPSUM),                                                       // 13
            str_emoji_opt->max_byte_count() == sizeof(EMOJI_LIPSUM),                                                   // 14
            hud::memory::is_memory_compare_equal(str_emoji_opt->bytes(), EMOJI_LIPSUM, sizeof(EMOJI_LIPSUM)),          // 15
            str_hebrew_opt.has_value(),                                                                                // 16
            str_hebrew_opt->byte_count() == sizeof(HEBREW_LIPSUM),                                                     // 17
            str_hebrew_opt->max_byte_count() == sizeof(HEBREW_LIPSUM),                                                 // 18
            hud::memory::is_memory_compare_equal(str_hebrew_opt->bytes(), HEBREW_LIPSUM, sizeof(HEBREW_LIPSUM)),       // 19
            str_hindi_opt.has_value(),                                                                                 // 20
            str_hindi_opt->byte_count() == sizeof(HINDI_LIPSUM),                                                       // 21
            str_hindi_opt->max_byte_count() == sizeof(HINDI_LIPSUM),                                                   // 22
            hud::memory::is_memory_compare_equal(str_hindi_opt->bytes(), HINDI_LIPSUM, sizeof(HINDI_LIPSUM)),          // 23
            str_japanese_opt.has_value(),                                                                              // 24
            str_japanese_opt->byte_count() == sizeof(JAPANESE_LIPSUM),                                                 // 25
            str_japanese_opt->max_byte_count() == sizeof(JAPANESE_LIPSUM),                                             // 26
            hud::memory::is_memory_compare_equal(str_japanese_opt->bytes(), JAPANESE_LIPSUM, sizeof(JAPANESE_LIPSUM)), // 27
            str_korean_opt.has_value(),                                                                                // 28
            str_korean_opt->byte_count() == sizeof(KOREAN_LIPSUM),                                                     // 29
            str_korean_opt->max_byte_count() == sizeof(KOREAN_LIPSUM),                                                 // 30
            hud::memory::is_memory_compare_equal(str_korean_opt->bytes(), KOREAN_LIPSUM, sizeof(KOREAN_LIPSUM)),       // 31
            str_latin_opt.has_value(),                                                                                 // 32
            str_latin_opt->byte_count() == sizeof(LATIN_LIPSUM),                                                       // 33
            str_latin_opt->max_byte_count() == sizeof(LATIN_LIPSUM),                                                   // 34
            hud::memory::is_memory_compare_equal(str_latin_opt->bytes(), LATIN_LIPSUM, sizeof(LATIN_LIPSUM)),          // 35
            str_russian_opt.has_value(),                                                                               // 36
            str_russian_opt->byte_count() == sizeof(RUSSIAN_LIPSUM),                                                   // 37
            str_russian_opt->max_byte_count() == sizeof(RUSSIAN_LIPSUM),                                               // 38
            hud::memory::is_memory_compare_equal(str_russian_opt->bytes(), RUSSIAN_LIPSUM, sizeof(RUSSIAN_LIPSUM)),    // 39

        };
    };

    // Non Constant
    {
        auto result = test();
        hud_test::for_each_value(std::make_index_sequence<std::tuple_size_v<decltype(result)>>(), [&]<usize value>() {
            hud_assert_true(std::get<value>(result));
        });
    }

    // Constant
    {
        constexpr auto result = test();
        hud_test::for_each_value(std::make_index_sequence<std::tuple_size_v<decltype(result)>>(), [&]<usize value>() {
            hud_assert_true(std::get<value>(result));
        });
    }
}