
#include <core/string.h>
#include <core/memory.h>

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
        const char8 txt[] = "é&='\0/1234564789";
        hud::optional<hud::string> str_opt = hud::make_string(hud::slice {txt, sizeof(txt)});
        return std::tuple {
            str_opt.has_value(),
            str_opt->byte_count() == sizeof(txt),
            str_opt->max_byte_count() == sizeof(txt),
            hud::memory::is_memory_compare_equal(str_opt->bytes(), txt, sizeof(txt))
        };
    };

    // Non Constant
    {
        auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
    }
}