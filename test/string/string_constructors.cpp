
#include <core/string.h>

GTEST_TEST(string, default_constructor_should_allocate_no_memory)
{
    auto test = []() -> std::tuple<bool, usize, usize> {
        hud::string s;
        return std::tuple {
            s.data() == nullptr,
            s.count() == 0u,
            s.max_count() == 0u
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