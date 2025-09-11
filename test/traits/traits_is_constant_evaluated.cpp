#include <core/traits/is_constant_evaluated.h>

namespace hud_test
{
    constexpr bool evaluate()
    {
        if consteval
        {
            return true;
        }
        return false;
    }
} // namespace hud_test

GTEST_TEST(traits, is_constant_evaluated)
{
    hud_assert_false(hud_test::evaluate());

    constexpr bool const_evaluation = hud_test::evaluate();
    hud_assert_true(const_evaluation);
}