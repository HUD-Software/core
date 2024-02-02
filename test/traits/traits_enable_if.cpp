#include <core/traits/enable_if.h>

namespace hud_test
{
    class a
    {
    };

    template<typename type_t>
    typename hud::enable_if_t<hud::is_integral_v<type_t>, bool> enable_if_fct_is_integral()
    {
        return true;
    }

    template<typename type_t>
    typename hud::enable_if_t<!hud::is_integral_v<type_t>, bool> enable_if_fct_is_integral()
    {
        return false;
    }
} // namespace hud_test

GTEST_TEST(traits, enable_if)
{
    hud_assert_true(hud_test::enable_if_fct_is_integral<i32>());
    hud_assert_true(hud_test::enable_if_fct_is_integral<i8>());
    hud_assert_false(hud_test::enable_if_fct_is_integral<hud_test::a>());
}