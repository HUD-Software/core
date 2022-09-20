#include <core/traits/is_constant_evaluated.h>

namespace {
    constexpr bool evaluate() {
        if (hud::is_constant_evaluated()) {
            return true;
        }
        return false;
    }
}

TEST(traits, is_constant_evaluated) {
    ASSERT_FALSE(evaluate());

    constexpr bool const_evaluation = evaluate();
    ASSERT_TRUE(const_evaluation);
}