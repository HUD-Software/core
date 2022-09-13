#pragma once
#ifndef HD_INC_OSLAYER_OS_COMMON_ATOMICS_H
#define HD_INC_OSLAYER_OS_COMMON_ATOMICS_H

namespace hud::OS::Common{

    struct Atomics {
    /** Specifies how memory accesses, including regular, non-atomic memory accesses, are to be ordered around an atomic operation. */
    enum class EMemoryOrder {
        /** No synchronization or ordering constraints imposed on other reads or writes, only this operation's atomicity is guaranteed. */
        relaxed,
        /**
        * No reads or writes in the current thread dependent on the value currently loaded can be reordered before this load.
        * Writes to data-dependent variables in other threads that release the same atomic variable are visible in the current thread.
        * On most platforms, this affects compiler optimizations only.
        */
        consume,
        /**
        * No reads or writes in the current thread can be reordered before this load.
        * All writes in other threads that release the same atomic variable are visible in the current thread.
        */
        acquire,
        /**
        * No reads or writes in the current thread can be reordered after this store.
        * All writes in the current thread are visible in other threads that acquire the same atomic variable.
        * and writes that carry a dependency into the atomic variable become visible in other threads that consume the same atomic .
        */
        release,
        /**
        * A read - modify - write operation with this memory order is both an acquire operation and a release operation.
        * No memory reads or writes in the current thread can be reordered before or after this store.
        * All writes in other threads that release the same atomic variable are visible before the modification and the modification
        * is visible in other threads that acquire the same atomic variable.
        */
        acq_rel,
        /**
        * A load operation with this memory order performs an acquire operation, a store performs a release operation,
        * and read - modify - write performs both an acquire operation and a release operation,
        * plus a single total order exists in which all threads observe all modifications in the same order.
        */
        seq_cst
    };
};
}

#endif // HD_INC_OSLAYER_OS_COMMON_ATOMICS_H