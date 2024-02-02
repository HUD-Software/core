#include <core/atomics.h>

GTEST_TEST(atomics, thread_fence)
{
    // Test nothing, just be sure it compile
    hud::atomics::thread_fence(hud::atomics::memory_order_e::relaxed);
    hud::atomics::thread_fence(hud::atomics::memory_order_e::consume);
    hud::atomics::thread_fence(hud::atomics::memory_order_e::acquire);
    hud::atomics::thread_fence(hud::atomics::memory_order_e::release);
    hud::atomics::thread_fence(hud::atomics::memory_order_e::acq_rel);
    hud::atomics::thread_fence(hud::atomics::memory_order_e::seq_cst);
}