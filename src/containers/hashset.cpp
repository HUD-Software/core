#include <core/containers/hashset.h>

namespace hud::details::hashset
{
    // When the map is empty, we threat it as a full empty hashmap with capacity that is 0 in the hashmap.
    // This trick removed a if branch for empty hashmap
    // So when insert a new element we trigger a new allocation of 1 element
    alignas(16) constexpr const control_type INIT_GROUP[32] {
        control_type {0}, // 0
        control_type {0},
        control_type {0},
        control_type {0},
        control_type {0}, // 4
        control_type {0},
        control_type {0},
        control_type {0},
        control_type {0}, // 8
        control_type {0},
        control_type {0},
        control_type {0},
        control_type {0}, // 12
        control_type {0},
        control_type {0},
        control_type {0},
        sentinel_byte, // 16 <- New empty hashmap point here
        empty_byte,
        empty_byte,
        empty_byte,
        empty_byte, // 20
        empty_byte,
        empty_byte,
        empty_byte,
        empty_byte, // 24
        empty_byte,
        empty_byte,
        empty_byte,
        empty_byte, // 28
        empty_byte,
        empty_byte,
        empty_byte, // 31
    };

    // alignas(16) constexpr const control_type EMPTY_GROUP[32] {
    //     details::hashmap::control_type {0}, // 0
    //     details::hashmap::control_type {0},
    //     details::hashmap::control_type {0},
    //     details::hashmap::control_type {0},
    //     details::hashmap::control_type {0}, // 4
    //     details::hashmap::control_type {0},
    //     details::hashmap::control_type {0},
    //     details::hashmap::control_type {0},
    //     details::hashmap::control_type {0}, // 8
    //     details::hashmap::control_type {0},
    //     details::hashmap::control_type {0},
    //     details::hashmap::control_type {0},
    //     details::hashmap::control_type {0}, // 12
    //     details::hashmap::control_type {0},
    //     details::hashmap::control_type {0},
    //     details::hashmap::control_type {0},
    //     details::hashmap::control_sentinel, // 16
    //     details::hashmap::control_empty,
    //     details::hashmap::control_empty,
    //     details::hashmap::control_empty,
    //     details::hashmap::control_empty, // 20
    //     details::hashmap::control_empty,
    //     details::hashmap::control_empty,
    //     details::hashmap::control_empty,
    //     details::hashmap::control_empty, // 24
    //     details::hashmap::control_empty,
    //     details::hashmap::control_empty,
    //     details::hashmap::control_empty,
    //     details::hashmap::control_empty, // 28
    //     details::hashmap::control_empty,
    //     details::hashmap::control_empty,
    //     details::hashmap::control_empty, // 31
    // };

} // namespace hud::details::hashset