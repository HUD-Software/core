#include <core/traits/is_constant_evaluated.h>

namespace hud_test
{
    constexpr bool evaluate()
    {
        if (hud::is_constant_evaluated())
        {
            return true;
        }
        return false;
    }
}

GTEST_TEST(traits, is_constant_evaluated)
{
    GTEST_ASSERT_FALSE(hud_test::evaluate());

    constexpr bool const_evaluation = hud_test::evaluate();
    GTEST_ASSERT_TRUE(const_evaluation);
}