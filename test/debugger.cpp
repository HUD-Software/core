#include <core/debugger.h>

TEST(debugger, is_present){
    ASSERT_FALSE(hud::debugger::is_present());
}