#include <core/traits/enable_if.h>

namespace {
    class a {};

    template <typename type_t>
    typename hud::enable_if_t<hud::is_integral_v<type_t>, bool> enable_if_fct_is_integral() { 
        return true; 
    }

    template <typename type_t>
    typename hud::enable_if_t<!hud::is_integral_v<type_t>, bool> enable_if_fct_is_integral() { 
        return false; 
    }
}

TEST(traits, enable_if)
{
    ASSERT_TRUE(enable_if_fct_is_integral<i32>());
    ASSERT_TRUE(enable_if_fct_is_integral<i8>());
    ASSERT_FALSE(enable_if_fct_is_integral<a>());
}