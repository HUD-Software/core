#include <core/debugger.h>

GTEST_TEST(debugger, is_present)
{
    hud_assert_false(hud::debugger::is_present());
}

GTEST_TEST(debugger, break_here_if)
{
    hud::debugger::break_here_if(true);
    hud::debugger::break_here_if(false);
}

GTEST_TEST(debugger, break_here)
{
    hud::debugger::break_here();
}