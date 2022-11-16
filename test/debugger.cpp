#include <core/debugger.h>

GTEST_TEST(debugger, is_present){
    ASSERT_FALSE(hud::debugger::is_present());
}