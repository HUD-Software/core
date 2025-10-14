#include <core/cstring.h>

namespace hud_test
{
    template<typename TCharType>
    i32 call_format_vargs(TCharType *buffer, u32 buffer_size, const TCharType *format, ...)
    {
        va_list args;
        va_start(args, format);
        i32 count = hud::cstring::format_vargs(buffer, buffer_size, format, args);
        va_end(args);
        return count;
    }
} // namespace hud_test

GTEST_TEST(cstring, is_ascii)
{
    const auto test = [](usize max) {
        if (hud::cstring::is_ascii((char8 *)nullptr)) {
            return false;
        }
        if (hud::cstring::is_ascii((wchar *)nullptr)) {
            return false;
        }
        for (char8 cur = 0; cur < max; cur++) {
            char8 text[2] = {cur, '\0'};
            if (!hud::cstring::is_ascii(text)) {
                return false;
            }
        }
        for (wchar cur = max / 2; cur < max; cur++) {
            wchar text[2] = {cur, L'\0'};
            if (hud::character::is_ascii(cur)) {
                if (!hud::cstring::is_ascii(text)) {
                    return false;
                }
            }
            else {
                if (hud::cstring::is_ascii(text)) {
                    return false;
                }
            }
        }
        return true;
    };

    // Non constant
    {
        const auto result = test(hud::char8_max + 20);
        hud_assert_true(result);
    }

    // Constant
    {
        constexpr auto result = test(hud::char8_max + 20);
        hud_assert_true(result);
    }
}

GTEST_TEST(cstring, is_ascii_safe)
{
    const auto test = [](usize max) {
        if (hud::cstring::is_ascii_safe((char8 *)nullptr, 1)) {
            return false;
        }
        if (hud::cstring::is_ascii_safe((wchar *)nullptr, 1)) {
            return false;
        }
        for (char8 cur = 0; cur < max; cur++) {
            char8 text[2] = {cur, '\0'};
            if (!hud::cstring::is_ascii_safe(text, 1)) {
                return false;
            }
        }
        for (wchar cur = max / 2; cur < max; cur++) {
            wchar text[2] = {cur, L'\0'};
            if (hud::character::is_ascii(cur)) {
                if (!hud::cstring::is_ascii_safe(text, 1)) {
                    return false;
                }
            }
            else if (hud::cstring::is_ascii_safe(text, 1)) {
                return false;
            }
        }
        return true;
    };

    // Non constant
    {
        const auto result = test(hud::char8_max + 20);
        hud_assert_true(result);
    }

    // Constant
    {
        constexpr auto result = test(hud::char8_max + 20);
        hud_assert_true(result);
    }
}

GTEST_TEST(cstring, is_null_or_empty)
{
    static constexpr const char8 *const no_null_empty = "abc";
    static constexpr const char8 *const empty = "";
    static constexpr const char8 *const null = nullptr;

    hud_assert_false(hud::cstring::is_null_or_empty(no_null_empty));
    hud_assert_true(hud::cstring::is_null_or_empty(empty));
    hud_assert_true(hud::cstring::is_null_or_empty(null));

    static constexpr const wchar *const wide_no_null_empty = L"abc";
    static constexpr const wchar *const wide_empty = L"";
    static constexpr const wchar *const wide_null = nullptr;

    hud_assert_false(hud::cstring::is_null_or_empty(wide_no_null_empty));
    hud_assert_true(hud::cstring::is_null_or_empty(wide_empty));
    hud_assert_true(hud::cstring::is_null_or_empty(wide_null));
}

GTEST_TEST(cstring, copy)
{
    static constexpr const char8 *const src = "abc";
    char8 dest[4];

    hud_assert_true(hud::cstring::copy(dest, src) == dest);
    hud_assert_true(dest[0] == 'a' && dest[1] == 'b' && dest[2] == 'c' && dest[3] == '\0');

    static constexpr const wchar *const wide_src = L"abc";
    wchar wide_dest[4];
    hud_assert_true(hud::cstring::copy(wide_dest, wide_src) == wide_dest);
    hud_assert_true(wide_dest[0] == L'a' && wide_dest[1] == L'b' && wide_dest[2] == L'c' && wide_dest[3] == L'\0');

    // Thoses crash
    // hud_assert_true(hud::cstring::copy((char8*)nullptr, nullptr) == nullptr);
    // hud_assert_true(hud::cstring::copy((wchar*)nullptr, nullptr) == nullptr);
}

// GTEST_TEST(cstring, copy_safe_with_nullptr_destination)
// {
//     FAIL();
// }
// GTEST_TEST(cstring, copy_safe_with_nullptr_source)
// {
//     FAIL();
// }
// GTEST_TEST(cstring, copy_safe_with_destination_size_zero_or_greater_than_isize_max)
// {
//     FAIL();
// }
// GTEST_TEST(cstring, copy_safe_with_destination_is_less_or_equal_lenght_safe_source_destination_size)
// {
//     FAIL();
// }
// GTEST_TEST(cstring, copy_safe_with_destination_and_source_overlap)
// {
//     FAIL();
// }

GTEST_TEST(cstring, copy_safe)
{
    static constexpr const char8 *const src = "abc";
    char8 dest[4];

    hud_assert_true(hud::cstring::copy_safe(dest, 4, src));
    hud_assert_true(hud::cstring::copy_safe(dest, 4, src));
    hud_assert_eq(dest[0], 'a');
    hud_assert_eq(dest[1], 'b');
    hud_assert_eq(dest[2], 'c');
    hud_assert_eq(dest[3], '\0');

    static constexpr const wchar *const wide_src = L"abc";
    wchar wide_dest[4];

    hud_assert_true(hud::cstring::copy_safe(wide_dest, 4, wide_src));

    hud_assert_true(hud::cstring::copy_safe(wide_dest, 4, wide_src));
    hud_assert_eq(wide_dest[0], L'a');
    hud_assert_eq(wide_dest[1], L'b');
    hud_assert_eq(wide_dest[2], L'c');
    hud_assert_eq(wide_dest[3], L'\0');

    // Thoses crash
    // hud_assert_false(hud::cstring::copy_safe(nullptr, 0, src));
    // hud_assert_false(hud::cstring::copy_safe(dest, 4, nullptr));
    // hud_assert_false(hud::cstring::copy_safe(dest, 0, src));
    // hud_assert_false(hud::cstring::copy_safe(nullptr, 0, wide_src));
    // hud_assert_false(hud::cstring::copy_safe(wide_dest, 4, nullptr));
    // hud_assert_false(hud::cstring::copy_safe(wide_dest, 0, wide_src));
    // hud_assert_false(hud::cstring::copy_safe((char8*)nullptr, 4, nullptr));
    // hud_assert_false(hud::cstring::copy_safe((wchar*)nullptr, 4, nullptr));
    // hud_assert_false(hud::cstring::copy_safe(wide_dest, 3, wide_src)); // No place for null-terminator character
}

GTEST_TEST(cstring, copy_partial)
{
    static constexpr const char8 *const src = "abc";
    char8 dest[4];

    hud_assert_true(hud::cstring::copy_partial(dest, src, 2) == dest);
    hud_assert_true(dest[0] == 'a' && dest[1] == 'b');

    hud_assert_true(hud::cstring::copy_partial(dest, src, 0) == dest);
    hud_assert_true(dest[0] == 'a' && dest[1] == 'b');

    hud_assert_true(hud::cstring::copy_partial(dest, src, 2) == dest);
    hud_assert_true(dest[0] == 'a' && dest[1] == 'b');

    static constexpr const wchar *const wide_src = L"abc";
    wchar wide_dest[4];

    hud_assert_true(hud::cstring::copy_partial(wide_dest, wide_src, 2) == wide_dest);
    hud_assert_true(wide_dest[0] == L'a' && wide_dest[1] == L'b');

    hud_assert_true(hud::cstring::copy_partial(wide_dest, wide_src, 0) == wide_dest);
    hud_assert_true(wide_dest[0] == L'a' && wide_dest[1] == L'b');

    hud_assert_true(hud::cstring::copy_partial(wide_dest, wide_src, 2) == wide_dest);
    hud_assert_true(wide_dest[0] == L'a' && wide_dest[1] == L'b');

    // Thoses crash
    // hud_assert_false(hud::cstring::copy_partial(nullptr, src, 2) == nullptr);
    // hud_assert_false(hud::cstring::copy_partial(dest, nullptr, 2) == nullptr);
    // hud_assert_false(hud::cstring::copy_partial(nullptr, wide_src, 2) == nullptr);
    // hud_assert_false(hud::cstring::copy_partial(wide_dest, nullptr, 2) == nullptr);
    // hud_assert_false(hud::cstring::copy_partial((char8*)nullptr, nullptr, 2) == nullptr);
    // hud_assert_false(hud::cstring::copy_partial((wchar*)nullptr, nullptr, 2) == nullptr);
}

GTEST_TEST(cstring, copy_partial_safe)
{
    static constexpr const char8 *const src = "abc";
    char8 dest[4];

    hud_assert_true(hud::cstring::copy_partial_safe(dest, 4, src, 2));
    hud_assert_true(dest[0] == 'a' && dest[1] == 'b' && dest[2] == '\0');

    hud_assert_true(hud::cstring::copy_partial_safe(dest, 4, src, 0));
    hud_assert_true(dest[0] == '\0');

    static constexpr const wchar *const wide_src = L"abc";
    wchar wide_dest[4];

    hud_assert_true(hud::cstring::copy_partial_safe(wide_dest, 4, wide_src, 2));
    hud_assert_true(wide_dest[0] == L'a' && wide_dest[1] == L'b' && wide_dest[2] == L'\0');

    hud_assert_true(hud::cstring::copy_partial_safe(wide_dest, 4, wide_src, 0));
    hud_assert_true(dest[0] == L'\0');

    // Thoses crash
    // hud_assert_false(hud::cstring::copy_partial_safe(dest, 2, src, 2)); // Not enough place for null-terminator character
    // hud_assert_false(hud::cstring::copy_partial_safe(dest, 0, src, 2)); // destination is too small
    // hud_assert_false(hud::cstring::copy_partial_safe(wide_dest, 2, wide_src, 2)); // Not enough place for null-terminator character
    // hud_assert_false(hud::cstring::copy_partial_safe(wide_dest, 0, wide_src, 2)); // destination is too small
    // hud_assert_false(hud::cstring::copy_partial_safe(nullptr, 4, src, 2)); // destination is nullptr
    // hud_assert_false(hud::cstring::copy_partial_safe(dest, 4, nullptr, 2)); // source is nullptr
    // hud_assert_false(hud::cstring::copy_partial_safe(nullptr, 4, wide_src, 2)); // destination is nullptr
    // hud_assert_false(hud::cstring::copy_partial_safe(wide_dest, 4, nullptr, 2)); // source is nullptr
    // hud_assert_false(hud::cstring::copy_partial_safe((char8*)nullptr, 4, nullptr, 2)); // destination and source are nullptr
    // hud_assert_false(hud::cstring::copy_partial_safe((wchar*)nullptr, 4, nullptr, 2)); // destination and source are nullptr
}

GTEST_TEST(cstring, append)
{
    static constexpr const char8 *const src = "de";
    char8 dest[6] = {'a', 'b', 'c', '\0'};
    hud_assert_true(hud::cstring::append(dest, src) == dest);
    hud_assert_true(dest[0] == 'a' && dest[1] == 'b' && dest[2] == 'c' && dest[3] == 'd' && dest[4] == 'e' && dest[5] == '\0');

    static constexpr const wchar *const wide_src = L"de";
    wchar wide_dest[6] = {L'a', L'b', L'c', L'\0'};
    hud_assert_true(hud::cstring::append(wide_dest, wide_src) == wide_dest);
    hud_assert_true(wide_dest[0] == L'a' && wide_dest[1] == L'b' && wide_dest[2] == L'c' && wide_dest[3] == L'd' && wide_dest[4] == L'e' && wide_dest[5] == L'\0');

    // Thoses crash
    // hud_assert_false(hud::cstring::append(nullptr, src));
    // hud_assert_false(hud::cstring::append(dest, nullptr));
    // hud_assert_false(hud::cstring::append(nullptr, wide_src));
    // hud_assert_false(hud::cstring::append(wide_dest, nullptr));
    // hud_assert_false(hud::cstring::append((char8*)nullptr, nullptr));
    // hud_assert_false(hud::cstring::append((wchar*)nullptr, nullptr));
}

GTEST_TEST(cstring, append_safe)
{
    static constexpr const char8 *const src = "de";
    char8 dest[6] = {'a', 'b', 'c', '\0'};
    hud_assert_true(hud::cstring::append_safe(dest, 6, src));
    hud_assert_true(dest[0] == 'a' && dest[1] == 'b' && dest[2] == 'c' && dest[3] == 'd' && dest[4] == 'e' && dest[5] == '\0');

    static constexpr const wchar *const wide_src = L"de";
    wchar wide_dest[6] = {L'a', L'b', L'c', L'\0'};
    hud_assert_true(hud::cstring::append_safe(wide_dest, 6, wide_src));
    hud_assert_true(wide_dest[0] == L'a' && wide_dest[1] == L'b' && wide_dest[2] == L'c' && wide_dest[3] == L'd' && wide_dest[4] == L'e' && wide_dest[5] == L'\0');

    // Thoses crash
    // hud_assert_false(hud::cstring::append_safe(nullptr, 6, src));
    // hud_assert_false(hud::cstring::append_safe(dest, 6, nullptr));
    // hud_assert_false(hud::cstring::append_safe(nullptr, 6, wide_src));
    // hud_assert_false(hud::cstring::append_safe(wide_dest, 6, nullptr));
    // hud_assert_false(hud::cstring::append_safe((char8*)nullptr, 6, nullptr));
    // hud_assert_false(hud::cstring::append_safe((wchar*)nullptr, 6, nullptr));
    // hud_assert_false(hud::cstring::append_safe(dest, 1, src)); //  destination is not big enough
    // hud_assert_false(hud::cstring::append_safe(wide_dest, 1, wide_src)); //  destination is not big enough
}

GTEST_TEST(cstring, append_partial)
{
    static constexpr const char8 *const src = "de";
    char8 dest[5] = {'a', 'b', 'c', '\0'};
    hud_assert_true(hud::cstring::append_partial(dest, src, 1) == dest);
    hud_assert_true(dest[0] == 'a' && dest[1] == 'b' && dest[2] == 'c' && dest[3] == 'd' && dest[4] == '\0');

    static constexpr const wchar *const wide_src = L"de";
    wchar wide_dest[6] = {L'a', L'b', L'c', L'\0'};
    hud_assert_true(hud::cstring::append_partial(wide_dest, wide_src, 1) == wide_dest);
    hud_assert_true(wide_dest[0] == L'a' && wide_dest[1] == L'b' && wide_dest[2] == L'c' && wide_dest[3] == L'd' && wide_dest[4] == L'\0');

    // Thoses crash
    // hud_assert_false(hud::cstring::append_partial(nullptr, src, 1));
    // hud_assert_false(hud::cstring::append_partial(dest, nullptr, 1));
    // hud_assert_false(hud::cstring::append_partial(nullptr, wide_src, 1));
    // hud_assert_false(hud::cstring::append_partial(wide_dest, nullptr, 1));
    // hud_assert_false(hud::cstring::append_partial((char8*)nullptr, nullptr, 1));
    // hud_assert_false(hud::cstring::append_partial((wchar*)nullptr, nullptr, 1));
}

GTEST_TEST(cstring, append_partial_safe)
{
    static constexpr const char8 *const src = "de";
    char8 dest[6] = {'a', 'b', 'c', '\0'};
    hud_assert_true(hud::cstring::append_partial_safe(dest, 6, src, 1));
    hud_assert_true(dest[0] == 'a' && dest[1] == 'b' && dest[2] == 'c' && dest[3] == 'd' && dest[4] == '\0');

    static constexpr const wchar *const wide_src = L"de";
    wchar wide_dest[6] = {L'a', L'b', L'c', L'\0'};
    hud_assert_true(hud::cstring::append_partial_safe(wide_dest, 6, wide_src, 1));
    hud_assert_true(wide_dest[0] == L'a' && wide_dest[1] == L'b' && wide_dest[2] == L'c' && wide_dest[3] == L'd' && wide_dest[4] == L'\0');

    // hud_assert_false(hud::cstring::append_partial_safe(nullptr, 6, src, 1)); // destination is nullptr
    // hud_assert_false(hud::cstring::append_partial_safe(dest, 6, nullptr, 1)); // source is nullptr
    // hud_assert_false(hud::cstring::append_partial_safe(nullptr, 6, wide_src, 1)); // destination is nullptr
    // hud_assert_false(hud::cstring::append_partial_safe(wide_dest, 6, nullptr, 1)); // source is nullptr
    // hud_assert_false(hud::cstring::append_partial_safe((char8*)nullptr, 6, nullptr, 1)); // destination and source are nullptr
    // hud_assert_false(hud::cstring::append_partial_safe((wchar*)nullptr, 6, nullptr, 1)); // destination and source are nullptr
    // hud_assert_false(hud::cstring::append_partial_safe(dest, 0, src, 1)); // Buffer is too small
    // hud_assert_false(hud::cstring::append_partial_safe(dest, 1, src, 1)); // Buffer is too small
    // hud_assert_false(hud::cstring::append_partial_safe(wide_dest, 1, wide_src, 1)); // Buffer is too small
    // hud_assert_false(hud::cstring::append_partial_safe(wide_dest, 0, wide_src, 1)); // Buffer is too small
}

GTEST_TEST(cstring, ascii_to_uppercase)
{
    char8 txt[] = "abc123,;:!";
    hud_assert_true(hud::cstring::ascii_to_uppercase(txt) == txt);
    hud_assert_true(txt[0] == 'A' && txt[1] == 'B' && txt[2] == 'C' && txt[3] == '1' && txt[4] == '2' && txt[5] == '3' && txt[6] == ',' && txt[7] == ';' && txt[8] == ':' && txt[9] == '!' && txt[10] == '\0');
    wchar wide_txt[] = L"abc123,;:!";
    hud_assert_true(hud::cstring::ascii_to_uppercase(wide_txt) == wide_txt);
    hud_assert_true(wide_txt[0] == 'A' && wide_txt[1] == 'B' && wide_txt[2] == 'C' && wide_txt[3] == '1' && wide_txt[4] == '2' && wide_txt[5] == '3' && wide_txt[6] == ',' && wide_txt[7] == ';' && wide_txt[8] == ':' && wide_txt[9] == '!' && wide_txt[10] == '\0');
}

GTEST_TEST(cstring, ascii_to_uppercase_safe)
{
    // Zero size
    {
        char8 txt[] = "abc123,;:!";
        hud_assert_false(hud::cstring::ascii_to_uppercase_safe(txt, 0));
        hud_assert_true(txt[0] == 'a' && txt[1] == 'b' && txt[2] == 'c' && txt[3] == '1' && txt[4] == '2' && txt[5] == '3' && txt[6] == ',' && txt[7] == ';' && txt[8] == ':' && txt[9] == '!' && txt[10] == '\0');
    }

    // Smaller size
    {
        char8 txt[] = "abc123,;:!";
        hud_assert_false(hud::cstring::ascii_to_uppercase_safe(txt, 2));
        hud_assert_true(txt[0] == 'A' && txt[1] == 'B' && txt[2] == 'c' && txt[3] == '1' && txt[4] == '2' && txt[5] == '3' && txt[6] == ',' && txt[7] == ';' && txt[8] == ':' && txt[9] == '!' && txt[10] == '\0');
    }

    // Correct size
    {
        char8 txt[] = "abc123,;:!";
        hud_assert_true(hud::cstring::ascii_to_uppercase_safe(txt, 10));
        hud_assert_true(txt[0] == 'A' && txt[1] == 'B' && txt[2] == 'C' && txt[3] == '1' && txt[4] == '2' && txt[5] == '3' && txt[6] == ',' && txt[7] == ';' && txt[8] == ':' && txt[9] == '!' && txt[10] == '\0');
    }

    // To big size
    {
        char8 txt[] = "abc123,;:!";
        hud_assert_false(hud::cstring::ascii_to_uppercase_safe(txt, 11));
        hud_assert_true(txt[0] == 'A' && txt[1] == 'B' && txt[2] == 'C' && txt[3] == '1' && txt[4] == '2' && txt[5] == '3' && txt[6] == ',' && txt[7] == ';' && txt[8] == ':' && txt[9] == '!' && txt[10] == '\0');
    }

    // With nullptr
    {
        hud_assert_false(hud::cstring::ascii_to_uppercase_safe((char8 *)nullptr, 11));
        hud_assert_false(hud::cstring::ascii_to_uppercase_safe((char8 *)nullptr, 0));
    }

    // Zero size
    {
        wchar txt[] = L"abc123,;:!";
        hud_assert_false(hud::cstring::ascii_to_uppercase_safe(txt, 0));
        hud_assert_true(txt[0] == L'a' && txt[1] == L'b' && txt[2] == L'c' && txt[3] == L'1' && txt[4] == L'2' && txt[5] == L'3' && txt[6] == L',' && txt[7] == L';' && txt[8] == L':' && txt[9] == L'!' && txt[10] == L'\0');
    }

    // Smaller size
    {
        wchar txt[] = L"abc123,;:!";
        hud_assert_false(hud::cstring::ascii_to_uppercase_safe(txt, 2));
        hud_assert_true(txt[0] == L'A' && txt[1] == L'B' && txt[2] == L'c' && txt[3] == L'1' && txt[4] == L'2' && txt[5] == L'3' && txt[6] == L',' && txt[7] == L';' && txt[8] == L':' && txt[9] == L'!' && txt[10] == L'\0');
    }

    // Correct size
    {
        wchar txt[] = L"abc123,;:!";
        hud_assert_true(hud::cstring::ascii_to_uppercase_safe(txt, 10));
        hud_assert_true(txt[0] == L'A' && txt[1] == L'B' && txt[2] == L'C' && txt[3] == L'1' && txt[4] == L'2' && txt[5] == L'3' && txt[6] == L',' && txt[7] == L';' && txt[8] == L':' && txt[9] == L'!' && txt[10] == L'\0');
    }

    // To big size
    {
        wchar txt[] = L"abc123,;:!";
        hud_assert_false(hud::cstring::ascii_to_uppercase_safe(txt, 11));
        hud_assert_true(txt[0] == L'A' && txt[1] == L'B' && txt[2] == L'C' && txt[3] == L'1' && txt[4] == L'2' && txt[5] == L'3' && txt[6] == L',' && txt[7] == L';' && txt[8] == L':' && txt[9] == L'!' && txt[10] == L'\0');
    }

    // With nullptr
    {
        hud_assert_false(hud::cstring::ascii_to_uppercase_safe((wchar *)nullptr, 11));
        hud_assert_false(hud::cstring::ascii_to_uppercase_safe((wchar *)nullptr, 0));
    }
}

GTEST_TEST(cstring, ascii_to_uppercase_partial)
{
    char8 txt[] = "abc123,;:!";
    hud_assert_true(hud::cstring::ascii_to_uppercase_partial(txt, 2) == txt);
    hud_assert_true(txt[0] == 'A' && txt[1] == 'B' && txt[2] == 'c' && txt[3] == '1' && txt[4] == '2' && txt[5] == '3' && txt[6] == ',' && txt[7] == ';' && txt[8] == ':' && txt[9] == '!' && txt[10] == '\0');
    wchar wide_txt[] = L"abc123,;:!";
    hud_assert_true(hud::cstring::ascii_to_uppercase_partial(wide_txt, 2) == wide_txt);
    hud_assert_true(wide_txt[0] == 'A' && wide_txt[1] == 'B' && wide_txt[2] == 'c' && wide_txt[3] == '1' && wide_txt[4] == '2' && wide_txt[5] == '3' && wide_txt[6] == ',' && wide_txt[7] == ';' && wide_txt[8] == ':' && wide_txt[9] == '!' && wide_txt[10] == '\0');
}

GTEST_TEST(cstring, ascii_to_uppercase_partial_safe)
{
    // Zero size, zero count
    {
        char8 txt[] = "abc123,;:!";
        hud_assert_true(hud::cstring::ascii_to_uppercase_partial_safe(txt, 0, 0));
        hud_assert_true(txt[0] == 'a' && txt[1] == 'b' && txt[2] == 'c' && txt[3] == '1' && txt[4] == '2' && txt[5] == '3' && txt[6] == ',' && txt[7] == ';' && txt[8] == ':' && txt[9] == '!' && txt[10] == '\0');
    }
    // Zero size, more count
    {
        char8 txt[] = "abc123,;:!";
        hud_assert_false(hud::cstring::ascii_to_uppercase_partial_safe(txt, 0, 1));
        hud_assert_true(txt[0] == 'a' && txt[1] == 'b' && txt[2] == 'c' && txt[3] == '1' && txt[4] == '2' && txt[5] == '3' && txt[6] == ',' && txt[7] == ';' && txt[8] == ':' && txt[9] == '!' && txt[10] == '\0');
    }

    // Smaller size, zero count
    {
        char8 txt[] = "abc123,;:!";
        hud_assert_true(hud::cstring::ascii_to_uppercase_partial_safe(txt, 2, 0));
        hud_assert_true(txt[0] == 'a' && txt[1] == 'b' && txt[2] == 'c' && txt[3] == '1' && txt[4] == '2' && txt[5] == '3' && txt[6] == ',' && txt[7] == ';' && txt[8] == ':' && txt[9] == '!' && txt[10] == '\0');
    }
    // Smaller size, smaller count
    {
        char8 txt[] = "abc123,;:!";
        hud_assert_true(hud::cstring::ascii_to_uppercase_partial_safe(txt, 2, 1));
        hud_assert_true(txt[0] == 'A' && txt[1] == 'b' && txt[2] == 'c' && txt[3] == '1' && txt[4] == '2' && txt[5] == '3' && txt[6] == ',' && txt[7] == ';' && txt[8] == ':' && txt[9] == '!' && txt[10] == '\0');
    }
    // Smaller size, exact count
    {
        char8 txt[] = "abc123,;:!";
        hud_assert_true(hud::cstring::ascii_to_uppercase_partial_safe(txt, 2, 2));
        hud_assert_true(txt[0] == 'A' && txt[1] == 'B' && txt[2] == 'c' && txt[3] == '1' && txt[4] == '2' && txt[5] == '3' && txt[6] == ',' && txt[7] == ';' && txt[8] == ':' && txt[9] == '!' && txt[10] == '\0');
    }
    // Smaller size, more count
    {
        char8 txt[] = "abc123,;:!";
        hud_assert_false(hud::cstring::ascii_to_uppercase_partial_safe(txt, 2, 3));
        hud_assert_true(txt[0] == 'a' && txt[1] == 'b' && txt[2] == 'c' && txt[3] == '1' && txt[4] == '2' && txt[5] == '3' && txt[6] == ',' && txt[7] == ';' && txt[8] == ':' && txt[9] == '!' && txt[10] == '\0');
    }

    // Correct size, zero count
    {
        char8 txt[] = "abc123,;:!";
        hud_assert_true(hud::cstring::ascii_to_uppercase_partial_safe(txt, 10, 0));
        hud_assert_true(txt[0] == 'a' && txt[1] == 'b' && txt[2] == 'c' && txt[3] == '1' && txt[4] == '2' && txt[5] == '3' && txt[6] == ',' && txt[7] == ';' && txt[8] == ':' && txt[9] == '!' && txt[10] == '\0');
    }
    // Correct size, smaller count
    {
        char8 txt[] = "abc123,;:!";
        hud_assert_true(hud::cstring::ascii_to_uppercase_partial_safe(txt, 10, 2));
        hud_assert_true(txt[0] == 'A' && txt[1] == 'B' && txt[2] == 'c' && txt[3] == '1' && txt[4] == '2' && txt[5] == '3' && txt[6] == ',' && txt[7] == ';' && txt[8] == ':' && txt[9] == '!' && txt[10] == '\0');
    }
    // Correct size, exact count
    {
        char8 txt[] = "abc123,;:!";
        hud_assert_true(hud::cstring::ascii_to_uppercase_partial_safe(txt, 10, 10));
        hud_assert_true(txt[0] == 'A' && txt[1] == 'B' && txt[2] == 'C' && txt[3] == '1' && txt[4] == '2' && txt[5] == '3' && txt[6] == ',' && txt[7] == ';' && txt[8] == ':' && txt[9] == '!' && txt[10] == '\0');
    }
    // Correct size, move count
    {
        char8 txt[] = "abc123,;:!";
        hud_assert_false(hud::cstring::ascii_to_uppercase_partial_safe(txt, 10, 11));
        hud_assert_true(txt[0] == 'a' && txt[1] == 'b' && txt[2] == 'c' && txt[3] == '1' && txt[4] == '2' && txt[5] == '3' && txt[6] == ',' && txt[7] == ';' && txt[8] == ':' && txt[9] == '!' && txt[10] == '\0');
    }

    // To big size, zero count
    {
        char8 txt[] = "abc123,;:!";
        hud_assert_true(hud::cstring::ascii_to_uppercase_partial_safe(txt, 11, 0));
        hud_assert_true(txt[0] == 'a' && txt[1] == 'b' && txt[2] == 'c' && txt[3] == '1' && txt[4] == '2' && txt[5] == '3' && txt[6] == ',' && txt[7] == ';' && txt[8] == ':' && txt[9] == '!' && txt[10] == '\0');
    }
    // To big size, smaller count
    {
        char8 txt[] = "abc123,;:!";
        hud_assert_true(hud::cstring::ascii_to_uppercase_partial_safe(txt, 11, 2));
        hud_assert_true(txt[0] == 'A' && txt[1] == 'B' && txt[2] == 'c' && txt[3] == '1' && txt[4] == '2' && txt[5] == '3' && txt[6] == ',' && txt[7] == ';' && txt[8] == ':' && txt[9] == '!' && txt[10] == '\0');
    }
    // To big size, exact count
    {
        char8 txt[] = "abc123,;:!";
        hud_assert_false(hud::cstring::ascii_to_uppercase_partial_safe(txt, 11, 11));
        hud_assert_true(txt[0] == 'A' && txt[1] == 'B' && txt[2] == 'C' && txt[3] == '1' && txt[4] == '2' && txt[5] == '3' && txt[6] == ',' && txt[7] == ';' && txt[8] == ':' && txt[9] == '!' && txt[10] == '\0');
    }
    // To big size, more count
    {
        char8 txt[] = "abc123,;:!";
        hud_assert_false(hud::cstring::ascii_to_uppercase_partial_safe(txt, 11, 12));
        hud_assert_true(txt[0] == 'a' && txt[1] == 'b' && txt[2] == 'c' && txt[3] == '1' && txt[4] == '2' && txt[5] == '3' && txt[6] == ',' && txt[7] == ';' && txt[8] == ':' && txt[9] == '!' && txt[10] == '\0');
    }

    // With nullptr, zero count
    {
        hud_assert_false(hud::cstring::ascii_to_uppercase_partial_safe((char8 *)nullptr, 11, 0));
        hud_assert_false(hud::cstring::ascii_to_uppercase_partial_safe((char8 *)nullptr, 0, 0));
    }
    // With nullptr, exact count
    {
        hud_assert_false(hud::cstring::ascii_to_uppercase_partial_safe((char8 *)nullptr, 11, 11));
        hud_assert_false(hud::cstring::ascii_to_uppercase_partial_safe((char8 *)nullptr, 0, 0));
    }
    // With nullptr, more count
    {
        hud_assert_false(hud::cstring::ascii_to_uppercase_partial_safe((char8 *)nullptr, 11, 12));
        hud_assert_false(hud::cstring::ascii_to_uppercase_partial_safe((char8 *)nullptr, 0, 1));
    }

    // Zero size, zero count
    {
        wchar txt[] = L"abc123,;:!";
        hud_assert_true(hud::cstring::ascii_to_uppercase_partial_safe(txt, 0, 0));
        hud_assert_true(txt[0] == L'a' && txt[1] == L'b' && txt[2] == L'c' && txt[3] == L'1' && txt[4] == L'2' && txt[5] == L'3' && txt[6] == L',' && txt[7] == L';' && txt[8] == L':' && txt[9] == L'!' && txt[10] == L'\0');
    }
    // Zero size, move count
    {
        wchar txt[] = L"abc123,;:!";
        hud_assert_false(hud::cstring::ascii_to_uppercase_partial_safe(txt, 0, 1));
        hud_assert_true(txt[0] == L'a' && txt[1] == L'b' && txt[2] == L'c' && txt[3] == L'1' && txt[4] == L'2' && txt[5] == L'3' && txt[6] == L',' && txt[7] == L';' && txt[8] == L':' && txt[9] == L'!' && txt[10] == L'\0');
    }

    // Smaller size, zero count
    {
        wchar txt[] = L"abc123,;:!";
        hud_assert_true(hud::cstring::ascii_to_uppercase_partial_safe(txt, 2, 0));
        hud_assert_true(txt[0] == L'a' && txt[1] == L'b' && txt[2] == L'c' && txt[3] == L'1' && txt[4] == L'2' && txt[5] == L'3' && txt[6] == L',' && txt[7] == L';' && txt[8] == L':' && txt[9] == L'!' && txt[10] == L'\0');
    }
    // Smaller size, smaller count
    {
        wchar txt[] = L"abc123,;:!";
        hud_assert_true(hud::cstring::ascii_to_uppercase_partial_safe(txt, 2, 1));
        hud_assert_true(txt[0] == L'A' && txt[1] == L'b' && txt[2] == L'c' && txt[3] == L'1' && txt[4] == L'2' && txt[5] == L'3' && txt[6] == L',' && txt[7] == L';' && txt[8] == L':' && txt[9] == L'!' && txt[10] == L'\0');
    }
    // Smaller size, exact count
    {
        wchar txt[] = L"abc123,;:!";
        hud_assert_true(hud::cstring::ascii_to_uppercase_partial_safe(txt, 2, 2));
        hud_assert_true(txt[0] == L'A' && txt[1] == L'B' && txt[2] == L'c' && txt[3] == L'1' && txt[4] == L'2' && txt[5] == L'3' && txt[6] == L',' && txt[7] == L';' && txt[8] == L':' && txt[9] == L'!' && txt[10] == L'\0');
    }
    // Smaller size, more count
    {
        wchar txt[] = L"abc123,;:!";
        hud_assert_false(hud::cstring::ascii_to_uppercase_partial_safe(txt, 2, 3));
        hud_assert_true(txt[0] == L'a' && txt[1] == L'b' && txt[2] == L'c' && txt[3] == L'1' && txt[4] == L'2' && txt[5] == L'3' && txt[6] == L',' && txt[7] == L';' && txt[8] == L':' && txt[9] == L'!' && txt[10] == L'\0');
    }

    // Correct size, zero count
    {
        wchar txt[] = L"abc123,;:!";
        hud_assert_true(hud::cstring::ascii_to_uppercase_partial_safe(txt, 10, 0));
        hud_assert_true(txt[0] == L'a' && txt[1] == L'b' && txt[2] == L'c' && txt[3] == L'1' && txt[4] == L'2' && txt[5] == L'3' && txt[6] == L',' && txt[7] == L';' && txt[8] == L':' && txt[9] == L'!' && txt[10] == L'\0');
    }
    // Correct size, smaller count
    {
        wchar txt[] = L"abc123,;:!";
        hud_assert_true(hud::cstring::ascii_to_uppercase_partial_safe(txt, 10, 2));
        hud_assert_true(txt[0] == L'A' && txt[1] == L'B' && txt[2] == L'c' && txt[3] == L'1' && txt[4] == L'2' && txt[5] == L'3' && txt[6] == L',' && txt[7] == L';' && txt[8] == L':' && txt[9] == L'!' && txt[10] == L'\0');
    }
    // Correct size, exact count
    {
        wchar txt[] = L"abc123,;:!";
        hud_assert_true(hud::cstring::ascii_to_uppercase_partial_safe(txt, 10, 10));
        hud_assert_true(txt[0] == L'A' && txt[1] == L'B' && txt[2] == L'C' && txt[3] == L'1' && txt[4] == L'2' && txt[5] == L'3' && txt[6] == L',' && txt[7] == L';' && txt[8] == L':' && txt[9] == L'!' && txt[10] == L'\0');
    }
    // Correct size, more count
    {
        wchar txt[] = L"abc123,;:!";
        hud_assert_false(hud::cstring::ascii_to_uppercase_partial_safe(txt, 10, 11));
        hud_assert_true(txt[0] == L'a' && txt[1] == L'b' && txt[2] == L'c' && txt[3] == L'1' && txt[4] == L'2' && txt[5] == L'3' && txt[6] == L',' && txt[7] == L';' && txt[8] == L':' && txt[9] == L'!' && txt[10] == L'\0');
    }

    // To big size, zero count
    {
        wchar txt[] = L"abc123,;:!";
        hud_assert_true(hud::cstring::ascii_to_uppercase_partial_safe(txt, 11, 0));
        hud_assert_true(txt[0] == L'a' && txt[1] == L'b' && txt[2] == L'c' && txt[3] == L'1' && txt[4] == L'2' && txt[5] == L'3' && txt[6] == L',' && txt[7] == L';' && txt[8] == L':' && txt[9] == L'!' && txt[10] == L'\0');
    }
    // To big size, smaller count
    {
        wchar txt[] = L"abc123,;:!";
        hud_assert_true(hud::cstring::ascii_to_uppercase_partial_safe(txt, 11, 2));
        hud_assert_true(txt[0] == L'A' && txt[1] == L'B' && txt[2] == L'c' && txt[3] == L'1' && txt[4] == L'2' && txt[5] == L'3' && txt[6] == L',' && txt[7] == L';' && txt[8] == L':' && txt[9] == L'!' && txt[10] == L'\0');
    }
    // To big size, exact count
    {
        wchar txt[] = L"abc123,;:!";
        hud_assert_false(hud::cstring::ascii_to_uppercase_partial_safe(txt, 11, 11));
        hud_assert_true(txt[0] == L'A' && txt[1] == L'B' && txt[2] == L'C' && txt[3] == L'1' && txt[4] == L'2' && txt[5] == L'3' && txt[6] == L',' && txt[7] == L';' && txt[8] == L':' && txt[9] == L'!' && txt[10] == L'\0');
    }
    // To big size, more count
    {
        wchar txt[] = L"abc123,;:!";
        hud_assert_false(hud::cstring::ascii_to_uppercase_partial_safe(txt, 11, 12));
        hud_assert_true(txt[0] == L'a' && txt[1] == L'b' && txt[2] == L'c' && txt[3] == L'1' && txt[4] == L'2' && txt[5] == L'3' && txt[6] == L',' && txt[7] == L';' && txt[8] == L':' && txt[9] == L'!' && txt[10] == L'\0');
    }

    // With nullptr, zero count
    {
        hud_assert_false(hud::cstring::ascii_to_uppercase_partial_safe((wchar *)nullptr, 11, 0));
        hud_assert_false(hud::cstring::ascii_to_uppercase_partial_safe((wchar *)nullptr, 0, 0));
    }
    // With nullptr, exact count
    {
        hud_assert_false(hud::cstring::ascii_to_uppercase_partial_safe((wchar *)nullptr, 11, 11));
        hud_assert_false(hud::cstring::ascii_to_uppercase_partial_safe((wchar *)nullptr, 0, 0));
    }
    // With nullptr, more count
    {
        hud_assert_false(hud::cstring::ascii_to_uppercase_partial_safe((wchar *)nullptr, 11, 12));
        hud_assert_false(hud::cstring::ascii_to_uppercase_partial_safe((wchar *)nullptr, 0, 1));
    }
}

GTEST_TEST(cstring, ascii_to_lowercase)
{
    char8 txt[] = "ABC123,;:!";
    hud_assert_true(hud::cstring::ascii_to_lowercase(txt) == txt);
    hud_assert_true(txt[0] == 'a' && txt[1] == 'b' && txt[2] == 'c' && txt[3] == '1' && txt[4] == '2' && txt[5] == '3' && txt[6] == ',' && txt[7] == ';' && txt[8] == ':' && txt[9] == '!' && txt[10] == '\0');
    wchar wide_txt[] = L"ABC123,;:!";
    hud_assert_true(hud::cstring::ascii_to_lowercase(wide_txt) == wide_txt);
    hud_assert_true(wide_txt[0] == 'a' && wide_txt[1] == 'b' && wide_txt[2] == 'c' && wide_txt[3] == '1' && wide_txt[4] == '2' && wide_txt[5] == '3' && wide_txt[6] == ',' && wide_txt[7] == ';' && wide_txt[8] == ':' && wide_txt[9] == '!' && wide_txt[10] == '\0');
}

GTEST_TEST(cstring, ascii_to_lowercase_safe)
{
    // Zero size
    {
        char8 txt[] = "ABC123,;:!";
        hud_assert_false(hud::cstring::ascii_to_lowercase_safe(txt, 0));
        hud_assert_true(txt[0] == 'A' && txt[1] == 'B' && txt[2] == 'C' && txt[3] == '1' && txt[4] == '2' && txt[5] == '3' && txt[6] == ',' && txt[7] == ';' && txt[8] == ':' && txt[9] == '!' && txt[10] == '\0');
    }

    // Smaller size
    {
        char8 txt[] = "ABC123,;:!";
        hud_assert_false(hud::cstring::ascii_to_lowercase_safe(txt, 2));
        hud_assert_true(txt[0] == 'a' && txt[1] == 'b' && txt[2] == 'C' && txt[3] == '1' && txt[4] == '2' && txt[5] == '3' && txt[6] == ',' && txt[7] == ';' && txt[8] == ':' && txt[9] == '!' && txt[10] == '\0');
    }

    // Correct size
    {
        char8 txt[] = "ABC123,;:!";
        hud_assert_true(hud::cstring::ascii_to_lowercase_safe(txt, 10));
        hud_assert_true(txt[0] == 'a' && txt[1] == 'b' && txt[2] == 'c' && txt[3] == '1' && txt[4] == '2' && txt[5] == '3' && txt[6] == ',' && txt[7] == ';' && txt[8] == ':' && txt[9] == '!' && txt[10] == '\0');
    }

    // To big size
    {
        char8 txt[] = "ABC123,;:!";
        hud_assert_false(hud::cstring::ascii_to_lowercase_safe(txt, 11));
        hud_assert_true(txt[0] == 'a' && txt[1] == 'b' && txt[2] == 'c' && txt[3] == '1' && txt[4] == '2' && txt[5] == '3' && txt[6] == ',' && txt[7] == ';' && txt[8] == ':' && txt[9] == '!' && txt[10] == '\0');
    }

    // With nullptr
    {
        hud_assert_false(hud::cstring::ascii_to_lowercase_safe((char8 *)nullptr, 11));
        hud_assert_false(hud::cstring::ascii_to_lowercase_safe((char8 *)nullptr, 0));
    }

    // Zero size
    {
        wchar txt[] = L"ABC123,;:!";
        hud_assert_false(hud::cstring::ascii_to_lowercase_safe(txt, 0));
        hud_assert_true(txt[0] == L'A' && txt[1] == L'B' && txt[2] == L'C' && txt[3] == L'1' && txt[4] == L'2' && txt[5] == L'3' && txt[6] == L',' && txt[7] == L';' && txt[8] == L':' && txt[9] == L'!' && txt[10] == L'\0');
    }

    // Smaller size
    {
        wchar txt[] = L"ABC123,;:!";
        hud_assert_false(hud::cstring::ascii_to_lowercase_safe(txt, 2));
        hud_assert_true(txt[0] == L'a' && txt[1] == L'b' && txt[2] == L'C' && txt[3] == L'1' && txt[4] == L'2' && txt[5] == L'3' && txt[6] == L',' && txt[7] == L';' && txt[8] == L':' && txt[9] == L'!' && txt[10] == L'\0');
    }

    // Correct size
    {
        wchar txt[] = L"ABC123,;:!";
        hud_assert_true(hud::cstring::ascii_to_lowercase_safe(txt, 10));
        hud_assert_true(txt[0] == L'a' && txt[1] == L'b' && txt[2] == L'c' && txt[3] == L'1' && txt[4] == L'2' && txt[5] == L'3' && txt[6] == L',' && txt[7] == L';' && txt[8] == L':' && txt[9] == L'!' && txt[10] == L'\0');
    }

    // To big size
    {
        wchar txt[] = L"ABC123,;:!";
        hud_assert_false(hud::cstring::ascii_to_lowercase_safe(txt, 11));
        hud_assert_true(txt[0] == L'a' && txt[1] == L'b' && txt[2] == L'c' && txt[3] == L'1' && txt[4] == L'2' && txt[5] == L'3' && txt[6] == L',' && txt[7] == L';' && txt[8] == L':' && txt[9] == L'!' && txt[10] == L'\0');
    }

    // With nullptr
    {
        hud_assert_false(hud::cstring::ascii_to_lowercase_safe((wchar *)nullptr, 11));
        hud_assert_false(hud::cstring::ascii_to_lowercase_safe((wchar *)nullptr, 0));
    }
}

GTEST_TEST(cstring, ascii_to_lowercase_partial)
{
    char8 txt[] = "ABC123,;:!";
    hud_assert_true(hud::cstring::ascii_to_lowercase_partial(txt, 2) == txt);
    hud_assert_true(txt[0] == 'a' && txt[1] == 'b' && txt[2] == 'C' && txt[3] == '1' && txt[4] == '2' && txt[5] == '3' && txt[6] == ',' && txt[7] == ';' && txt[8] == ':' && txt[9] == '!' && txt[10] == '\0');

    wchar wide_txt[] = L"ABC123,;:!";
    hud_assert_true(hud::cstring::ascii_to_lowercase_partial(wide_txt, 2) == wide_txt);
    hud_assert_true(wide_txt[0] == 'a' && wide_txt[1] == 'b' && wide_txt[2] == 'C' && wide_txt[3] == '1' && wide_txt[4] == '2' && wide_txt[5] == '3' && wide_txt[6] == ',' && wide_txt[7] == ';' && wide_txt[8] == ':' && wide_txt[9] == '!' && wide_txt[10] == '\0');
}

GTEST_TEST(cstring, ascii_to_lowercase_partial_safe)
{
    // Zero size, zero count
    {
        char8 txt[] = "ABC123,;:!";
        hud_assert_true(hud::cstring::ascii_to_lowercase_partial_safe(txt, 0, 0));
        hud_assert_true(txt[0] == 'A' && txt[1] == 'B' && txt[2] == 'C' && txt[3] == '1' && txt[4] == '2' && txt[5] == '3' && txt[6] == ',' && txt[7] == ';' && txt[8] == ':' && txt[9] == '!' && txt[10] == '\0');
    }
    // Zero size, more count
    {
        char8 txt[] = "ABC123,;:!";
        hud_assert_false(hud::cstring::ascii_to_lowercase_partial_safe(txt, 0, 1));
        hud_assert_true(txt[0] == 'A' && txt[1] == 'B' && txt[2] == 'C' && txt[3] == '1' && txt[4] == '2' && txt[5] == '3' && txt[6] == ',' && txt[7] == ';' && txt[8] == ':' && txt[9] == '!' && txt[10] == '\0');
    }

    // Smaller size, zero count
    {
        char8 txt[] = "ABC123,;:!";
        hud_assert_true(hud::cstring::ascii_to_lowercase_partial_safe(txt, 2, 0));
        hud_assert_true(txt[0] == 'A' && txt[1] == 'B' && txt[2] == 'C' && txt[3] == '1' && txt[4] == '2' && txt[5] == '3' && txt[6] == ',' && txt[7] == ';' && txt[8] == ':' && txt[9] == '!' && txt[10] == '\0');
    }
    // Smaller size, smaller count
    {
        char8 txt[] = "ABC123,;:!";
        hud_assert_true(hud::cstring::ascii_to_lowercase_partial_safe(txt, 2, 1));
        hud_assert_true(txt[0] == 'a' && txt[1] == 'B' && txt[2] == 'C' && txt[3] == '1' && txt[4] == '2' && txt[5] == '3' && txt[6] == ',' && txt[7] == ';' && txt[8] == ':' && txt[9] == '!' && txt[10] == '\0');
    }
    // Smaller size, exact count
    {
        char8 txt[] = "ABC123,;:!";
        hud_assert_true(hud::cstring::ascii_to_lowercase_partial_safe(txt, 2, 2));
        hud_assert_true(txt[0] == 'a' && txt[1] == 'b' && txt[2] == 'C' && txt[3] == '1' && txt[4] == '2' && txt[5] == '3' && txt[6] == ',' && txt[7] == ';' && txt[8] == ':' && txt[9] == '!' && txt[10] == '\0');
    }
    // Smaller size, more count
    {
        char8 txt[] = "ABC123,;:!";
        hud_assert_false(hud::cstring::ascii_to_lowercase_partial_safe(txt, 2, 3));
        hud_assert_true(txt[0] == 'A' && txt[1] == 'B' && txt[2] == 'C' && txt[3] == '1' && txt[4] == '2' && txt[5] == '3' && txt[6] == ',' && txt[7] == ';' && txt[8] == ':' && txt[9] == '!' && txt[10] == '\0');
    }

    // Correct size, zero count
    {
        char8 txt[] = "ABC123,;:!";
        hud_assert_true(hud::cstring::ascii_to_lowercase_partial_safe(txt, 10, 0));
        hud_assert_true(txt[0] == 'A' && txt[1] == 'B' && txt[2] == 'C' && txt[3] == '1' && txt[4] == '2' && txt[5] == '3' && txt[6] == ',' && txt[7] == ';' && txt[8] == ':' && txt[9] == '!' && txt[10] == '\0');
    }
    // Correct size, smaller count
    {
        char8 txt[] = "ABC123,;:!";
        hud_assert_true(hud::cstring::ascii_to_lowercase_partial_safe(txt, 10, 2));
        hud_assert_true(txt[0] == 'a' && txt[1] == 'b' && txt[2] == 'C' && txt[3] == '1' && txt[4] == '2' && txt[5] == '3' && txt[6] == ',' && txt[7] == ';' && txt[8] == ':' && txt[9] == '!' && txt[10] == '\0');
    }
    // Correct size, exact count
    {
        char8 txt[] = "ABC123,;:!";
        hud_assert_true(hud::cstring::ascii_to_lowercase_partial_safe(txt, 10, 10));
        hud_assert_true(txt[0] == 'a' && txt[1] == 'b' && txt[2] == 'c' && txt[3] == '1' && txt[4] == '2' && txt[5] == '3' && txt[6] == ',' && txt[7] == ';' && txt[8] == ':' && txt[9] == '!' && txt[10] == '\0');
    }
    // Correct size, move count
    {
        char8 txt[] = "ABC123,;:!";
        hud_assert_false(hud::cstring::ascii_to_lowercase_partial_safe(txt, 10, 11));
        hud_assert_true(txt[0] == 'A' && txt[1] == 'B' && txt[2] == 'C' && txt[3] == '1' && txt[4] == '2' && txt[5] == '3' && txt[6] == ',' && txt[7] == ';' && txt[8] == ':' && txt[9] == '!' && txt[10] == '\0');
    }

    // To big size, zero count
    {
        char8 txt[] = "ABC123,;:!";
        hud_assert_true(hud::cstring::ascii_to_lowercase_partial_safe(txt, 11, 0));
        hud_assert_true(txt[0] == 'A' && txt[1] == 'B' && txt[2] == 'C' && txt[3] == '1' && txt[4] == '2' && txt[5] == '3' && txt[6] == ',' && txt[7] == ';' && txt[8] == ':' && txt[9] == '!' && txt[10] == '\0');
    }
    // To big size, smaller count
    {
        char8 txt[] = "ABC123,;:!";
        hud_assert_true(hud::cstring::ascii_to_lowercase_partial_safe(txt, 11, 2));
        hud_assert_true(txt[0] == 'a' && txt[1] == 'b' && txt[2] == 'C' && txt[3] == '1' && txt[4] == '2' && txt[5] == '3' && txt[6] == ',' && txt[7] == ';' && txt[8] == ':' && txt[9] == '!' && txt[10] == '\0');
    }
    // To big size, exact count
    {
        char8 txt[] = "ABC123,;:!";
        hud_assert_false(hud::cstring::ascii_to_lowercase_partial_safe(txt, 11, 11));
        hud_assert_true(txt[0] == 'a' && txt[1] == 'b' && txt[2] == 'c' && txt[3] == '1' && txt[4] == '2' && txt[5] == '3' && txt[6] == ',' && txt[7] == ';' && txt[8] == ':' && txt[9] == '!' && txt[10] == '\0');
    }
    // To big size, more count
    {
        char8 txt[] = "ABC123,;:!";
        hud_assert_false(hud::cstring::ascii_to_lowercase_partial_safe(txt, 11, 12));
        hud_assert_true(txt[0] == 'A' && txt[1] == 'B' && txt[2] == 'C' && txt[3] == '1' && txt[4] == '2' && txt[5] == '3' && txt[6] == ',' && txt[7] == ';' && txt[8] == ':' && txt[9] == '!' && txt[10] == '\0');
    }

    // With nullptr, zero count
    {
        hud_assert_false(hud::cstring::ascii_to_lowercase_partial_safe((char8 *)nullptr, 11, 0));
        hud_assert_false(hud::cstring::ascii_to_lowercase_partial_safe((char8 *)nullptr, 0, 0));
    }
    // With nullptr, exact count
    {
        hud_assert_false(hud::cstring::ascii_to_lowercase_partial_safe((char8 *)nullptr, 11, 11));
        hud_assert_false(hud::cstring::ascii_to_lowercase_partial_safe((char8 *)nullptr, 0, 0));
    }
    // With nullptr, more count
    {
        hud_assert_false(hud::cstring::ascii_to_lowercase_partial_safe((char8 *)nullptr, 11, 12));
        hud_assert_false(hud::cstring::ascii_to_lowercase_partial_safe((char8 *)nullptr, 0, 1));
    }

    // Zero size, zero count
    {
        wchar txt[] = L"ABC123,;:!";
        hud_assert_true(hud::cstring::ascii_to_lowercase_partial_safe(txt, 0, 0));
        hud_assert_true(txt[0] == L'A' && txt[1] == L'B' && txt[2] == L'C' && txt[3] == L'1' && txt[4] == L'2' && txt[5] == L'3' && txt[6] == L',' && txt[7] == L';' && txt[8] == L':' && txt[9] == L'!' && txt[10] == L'\0');
    }
    // Zero size, move count
    {
        wchar txt[] = L"ABC123,;:!";
        hud_assert_false(hud::cstring::ascii_to_lowercase_partial_safe(txt, 0, 1));
        hud_assert_true(txt[0] == L'A' && txt[1] == L'B' && txt[2] == L'C' && txt[3] == L'1' && txt[4] == L'2' && txt[5] == L'3' && txt[6] == L',' && txt[7] == L';' && txt[8] == L':' && txt[9] == L'!' && txt[10] == L'\0');
    }

    // Smaller size, zero count
    {
        wchar txt[] = L"ABC123,;:!";
        hud_assert_true(hud::cstring::ascii_to_lowercase_partial_safe(txt, 2, 0));
        hud_assert_true(txt[0] == L'A' && txt[1] == L'B' && txt[2] == L'C' && txt[3] == L'1' && txt[4] == L'2' && txt[5] == L'3' && txt[6] == L',' && txt[7] == L';' && txt[8] == L':' && txt[9] == L'!' && txt[10] == L'\0');
    }
    // Smaller size, smaller count
    {
        wchar txt[] = L"ABC123,;:!";
        hud_assert_true(hud::cstring::ascii_to_lowercase_partial_safe(txt, 2, 1));
        hud_assert_true(txt[0] == L'a' && txt[1] == L'B' && txt[2] == L'C' && txt[3] == L'1' && txt[4] == L'2' && txt[5] == L'3' && txt[6] == L',' && txt[7] == L';' && txt[8] == L':' && txt[9] == L'!' && txt[10] == L'\0');
    }
    // Smaller size, exact count
    {
        wchar txt[] = L"ABC123,;:!";
        hud_assert_true(hud::cstring::ascii_to_lowercase_partial_safe(txt, 2, 2));
        hud_assert_true(txt[0] == L'a' && txt[1] == L'b' && txt[2] == L'C' && txt[3] == L'1' && txt[4] == L'2' && txt[5] == L'3' && txt[6] == L',' && txt[7] == L';' && txt[8] == L':' && txt[9] == L'!' && txt[10] == L'\0');
    }
    // Smaller size, more count
    {
        wchar txt[] = L"ABC123,;:!";
        hud_assert_false(hud::cstring::ascii_to_lowercase_partial_safe(txt, 2, 3));
        hud_assert_true(txt[0] == L'A' && txt[1] == L'B' && txt[2] == L'C' && txt[3] == L'1' && txt[4] == L'2' && txt[5] == L'3' && txt[6] == L',' && txt[7] == L';' && txt[8] == L':' && txt[9] == L'!' && txt[10] == L'\0');
    }

    // Correct size, zero count
    {
        wchar txt[] = L"ABC123,;:!";
        hud_assert_true(hud::cstring::ascii_to_lowercase_partial_safe(txt, 10, 0));
        hud_assert_true(txt[0] == L'A' && txt[1] == L'B' && txt[2] == L'C' && txt[3] == L'1' && txt[4] == L'2' && txt[5] == L'3' && txt[6] == L',' && txt[7] == L';' && txt[8] == L':' && txt[9] == L'!' && txt[10] == L'\0');
    }
    // Correct size, smaller count
    {
        wchar txt[] = L"ABC123,;:!";
        hud_assert_true(hud::cstring::ascii_to_lowercase_partial_safe(txt, 10, 2));
        hud_assert_true(txt[0] == L'a' && txt[1] == L'b' && txt[2] == L'C' && txt[3] == L'1' && txt[4] == L'2' && txt[5] == L'3' && txt[6] == L',' && txt[7] == L';' && txt[8] == L':' && txt[9] == L'!' && txt[10] == L'\0');
    }
    // Correct size, exact count
    {
        wchar txt[] = L"ABC123,;:!";
        hud_assert_true(hud::cstring::ascii_to_lowercase_partial_safe(txt, 10, 10));
        hud_assert_true(txt[0] == L'a' && txt[1] == L'b' && txt[2] == L'c' && txt[3] == L'1' && txt[4] == L'2' && txt[5] == L'3' && txt[6] == L',' && txt[7] == L';' && txt[8] == L':' && txt[9] == L'!' && txt[10] == L'\0');
    }
    // Correct size, more count
    {
        wchar txt[] = L"ABC123,;:!";
        hud_assert_false(hud::cstring::ascii_to_lowercase_partial_safe(txt, 10, 11));
        hud_assert_true(txt[0] == L'A' && txt[1] == L'B' && txt[2] == L'C' && txt[3] == L'1' && txt[4] == L'2' && txt[5] == L'3' && txt[6] == L',' && txt[7] == L';' && txt[8] == L':' && txt[9] == L'!' && txt[10] == L'\0');
    }

    // To big size, zero count
    {
        wchar txt[] = L"ABC123,;:!";
        hud_assert_true(hud::cstring::ascii_to_lowercase_partial_safe(txt, 11, 0));
        hud_assert_true(txt[0] == L'A' && txt[1] == L'B' && txt[2] == L'C' && txt[3] == L'1' && txt[4] == L'2' && txt[5] == L'3' && txt[6] == L',' && txt[7] == L';' && txt[8] == L':' && txt[9] == L'!' && txt[10] == L'\0');
    }
    // To big size, smaller count
    {
        wchar txt[] = L"ABC123,;:!";
        hud_assert_true(hud::cstring::ascii_to_lowercase_partial_safe(txt, 11, 2));
        hud_assert_true(txt[0] == L'a' && txt[1] == L'b' && txt[2] == L'C' && txt[3] == L'1' && txt[4] == L'2' && txt[5] == L'3' && txt[6] == L',' && txt[7] == L';' && txt[8] == L':' && txt[9] == L'!' && txt[10] == L'\0');
    }
    // To big size, exact count
    {
        wchar txt[] = L"ABC123,;:!";
        hud_assert_false(hud::cstring::ascii_to_lowercase_partial_safe(txt, 11, 11));
        hud_assert_true(txt[0] == L'a' && txt[1] == L'b' && txt[2] == L'c' && txt[3] == L'1' && txt[4] == L'2' && txt[5] == L'3' && txt[6] == L',' && txt[7] == L';' && txt[8] == L':' && txt[9] == L'!' && txt[10] == L'\0');
    }
    // To big size, more count
    {
        wchar txt[] = L"ABC123,;:!";
        hud_assert_false(hud::cstring::ascii_to_lowercase_partial_safe(txt, 11, 12));
        hud_assert_true(txt[0] == L'A' && txt[1] == L'B' && txt[2] == L'C' && txt[3] == L'1' && txt[4] == L'2' && txt[5] == L'3' && txt[6] == L',' && txt[7] == L';' && txt[8] == L':' && txt[9] == L'!' && txt[10] == L'\0');
    }

    // With nullptr, zero count
    {
        hud_assert_false(hud::cstring::ascii_to_lowercase_partial_safe((wchar *)nullptr, 11, 0));
        hud_assert_false(hud::cstring::ascii_to_lowercase_partial_safe((wchar *)nullptr, 0, 0));
    }
    // With nullptr, exact count
    {
        hud_assert_false(hud::cstring::ascii_to_lowercase_partial_safe((wchar *)nullptr, 11, 11));
        hud_assert_false(hud::cstring::ascii_to_lowercase_partial_safe((wchar *)nullptr, 0, 0));
    }
    // With nullptr, more count
    {
        hud_assert_false(hud::cstring::ascii_to_lowercase_partial_safe((wchar *)nullptr, 11, 12));
        hud_assert_false(hud::cstring::ascii_to_lowercase_partial_safe((wchar *)nullptr, 0, 1));
    }
}

GTEST_TEST(cstring, equals)
{
    const auto test = []() {
        return std::tuple {
            hud::cstring::equals("abc", "abc"),   // 0
            hud::cstring::equals("aBc", "abc"),   // 1
            hud::cstring::equals("abc", "ab"),    // 2
            hud::cstring::equals(L"abc", L"abc"), // 3
            hud::cstring::equals(L"aBc", L"abc"), // 4
            hud::cstring::equals(L"abc", L"ab"),  // 5
            hud::cstring::equals("abc", ""),      // 6
            hud::cstring::equals("", "abc")       // 7
        };
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_false(std::get<1>(result));
        hud_assert_false(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_false(std::get<4>(result));
        hud_assert_false(std::get<5>(result));
        hud_assert_false(std::get<6>(result));
        hud_assert_false(std::get<7>(result));
    }
    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_false(std::get<1>(result));
        hud_assert_false(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_false(std::get<4>(result));
        hud_assert_false(std::get<5>(result));
        hud_assert_false(std::get<6>(result));
        hud_assert_false(std::get<7>(result));
    }
}

GTEST_TEST(cstring, equals_partial)
{
    hud_assert_true(hud::cstring::equals_partial("abc", "abc", 1));
    hud_assert_false(hud::cstring::equals_partial("abc", "bc", 2));
    hud_assert_false(hud::cstring::equals_partial("aBc", "abc", 3));
    hud_assert_true(hud::cstring::equals_partial("aBc", "abc", 1));

    hud_assert_true(hud::cstring::equals_partial(L"abc", L"abc", 1));
    hud_assert_false(hud::cstring::equals_partial(L"abc", L"bc", 2));
    hud_assert_false(hud::cstring::equals_partial(L"aBc", L"abc", 3));
    hud_assert_true(hud::cstring::equals_partial(L"aBc", L"abc", 1));
}

GTEST_TEST(cstring, length)
{
    hud_assert_true(hud::cstring::length("aBc") == 3);
    hud_assert_true(hud::cstring::length(L"aBc") == 3);
    hud_assert_false(hud::cstring::length("aBcd") == 3);
    hud_assert_false(hud::cstring::length(L"aBcd") == 3);
}

GTEST_TEST(cstring, length_safe_with_string_nullptr)
{
    constexpr char8 *null_string = nullptr;
    hud_assert_eq(hud::cstring::length_safe(null_string, 0u), 0u);
    hud_assert_eq(hud::cstring::length_safe(null_string, 1u), 0u);
    hud_assert_eq(hud::cstring::length_safe(null_string, hud::cstring::RSIZE_MAX_STR), 0u);
    hud_assert_eq(hud::cstring::length_safe(null_string, hud::cstring::RSIZE_MAX_STR + 1u), 0u);

    constexpr wchar *null_wstring = nullptr;
    hud_assert_eq(hud::cstring::length_safe(null_wstring, 0u), 0u);
    hud_assert_eq(hud::cstring::length_safe(null_wstring, 1u), 0u);
    hud_assert_eq(hud::cstring::length_safe(null_wstring, hud::cstring::RSIZE_MAX_STR), 0u);
    hud_assert_eq(hud::cstring::length_safe(null_wstring, hud::cstring::RSIZE_MAX_STR + 1u), 0u);
}

GTEST_TEST(cstring, length_safe_with_max_length_zero)
{
    hud_assert_eq(hud::cstring::length_safe("", 0u), 0u);
    hud_assert_eq(hud::cstring::length_safe("a", 0u), 0u);
    hud_assert_eq(hud::cstring::length_safe("aBc", 0u), 0u);

    hud_assert_eq(hud::cstring::length_safe(L"", 0u), 0u);
    hud_assert_eq(hud::cstring::length_safe(L"a", 0u), 0u);
    hud_assert_eq(hud::cstring::length_safe(L"aBc", 0u), 0u);
}

GTEST_TEST(cstring, length_safe_with_max_length_greater_than_RSIZE_MAX_STR)
{
    hud_assert_eq(hud::cstring::length_safe("", hud::cstring::RSIZE_MAX_STR + 1u), 0u);
    hud_assert_eq(hud::cstring::length_safe("a", hud::cstring::RSIZE_MAX_STR + 1u), 1u);
    hud_assert_eq(hud::cstring::length_safe("aBc", hud::cstring::RSIZE_MAX_STR + 1u), 3u);

    hud_assert_eq(hud::cstring::length_safe(L"", hud::cstring::RSIZE_MAX_STR + 1u), 0u);
    hud_assert_eq(hud::cstring::length_safe(L"a", hud::cstring::RSIZE_MAX_STR + 1u), 1u);
    hud_assert_eq(hud::cstring::length_safe(L"aBc", hud::cstring::RSIZE_MAX_STR + 2u), 3u);
}

GTEST_TEST(cstring, length_safe_without_null_terminated_character)
{
    char8 MAX_STR_WITHOUT_NULL_TERMINATED[hud::cstring::RSIZE_MAX_STR + 1u];
    hud::memory::set_memory_safe(MAX_STR_WITHOUT_NULL_TERMINATED, 'a');
    hud_assert_eq(hud::cstring::length_safe(MAX_STR_WITHOUT_NULL_TERMINATED, 3), 3);
    hud_assert_eq(hud::cstring::length_safe(MAX_STR_WITHOUT_NULL_TERMINATED, hud::cstring::RSIZE_MAX_STR), hud::cstring::RSIZE_MAX_STR);
    hud_assert_eq(hud::cstring::length_safe(MAX_STR_WITHOUT_NULL_TERMINATED, hud::cstring::RSIZE_MAX_STR + 1u), hud::cstring::RSIZE_MAX_STR);
}

GTEST_TEST(cstring, length_safe)
{
    hud_assert_true(hud::cstring::length_safe("aBc", 3) == 3);
    hud_assert_true(hud::cstring::length_safe("aBc", 2) == 2);
    hud_assert_true(hud::cstring::length_safe("aBc", 4) == 3);
    hud_assert_true(hud::cstring::length_safe((char8 *)nullptr, 2) == 0);

    hud_assert_true(hud::cstring::length_safe(L"aBc", 3) == 3);
    hud_assert_true(hud::cstring::length_safe(L"aBc", 2) == 2);
    hud_assert_true(hud::cstring::length_safe(L"aBc", 4) == 3);
    hud_assert_true(hud::cstring::length_safe((wchar *)nullptr, 2) == 0);
}

GTEST_TEST(cstring, find_string)
{
    const char8 *str = "abcdefcd";
    hud_assert_true(hud::cstring::find_string(str, "cd") == str + 2);
    hud_assert_true(hud::cstring::find_string(str, "fe") == nullptr);

    const wchar *wide_str = L"abcdef";
    hud_assert_true(hud::cstring::find_string(wide_str, L"cd") == wide_str + 2);
    hud_assert_true(hud::cstring::find_string(wide_str, L"fe") == nullptr);
}

GTEST_TEST(cstring, find_character)
{
    const char8 *str = "abcdefcd";
    hud_assert_true(hud::cstring::find_character(str, 'c') == str + 2);
    hud_assert_true(hud::cstring::find_character(str, 'g') == nullptr);

    const wchar *wide_str = L"abcdef";
    hud_assert_true(hud::cstring::find_character(wide_str, 'c') == wide_str + 2);
    hud_assert_true(hud::cstring::find_character(wide_str, 'g') == nullptr);
}

GTEST_TEST(cstring, format_vargs)
{
    const char8 *fmt = "Hello %s! %ls time";
    char8 buffer[256];
    i32 character_count = hud_test::call_format_vargs(buffer, 256, fmt, "World", L"Hammer");
    hud_assert_true(hud::cstring::equals(buffer, "Hello World! Hammer time"));
    hud_assert_true(character_count == 24);

    const wchar *wide_fmt = L"Hello %ls! %ls time";
    wchar wide_buffer[256];
    character_count = hud_test::call_format_vargs(wide_buffer, 256, wide_fmt, L"World", L"Hammer");
    hud_assert_true(hud::cstring::equals(wide_buffer, L"Hello World! Hammer time"));
    hud_assert_true(character_count == 24);
}

GTEST_TEST(cstring, format)
{
    const char8 *fmt = "Hello %s! %ls time";
    char8 buffer[256];
    i32 character_count = hud::cstring::format(buffer, 256, fmt, "World", L"Hammer");
    hud_assert_true(hud::cstring::equals(buffer, "Hello World! Hammer time"));
    hud_assert_true(character_count == 24);

    const wchar *wide_fmt = L"Hello %ls! %ls time";
    wchar wide_buffer[256];
    character_count = hud::cstring::format(wide_buffer, 256, wide_fmt, L"World", L"Hammer");
    hud_assert_true(hud::cstring::equals(wide_buffer, L"Hello World! Hammer time"));
    hud_assert_true(character_count == 24);
}