#include <core/containers/hashmap.h>

namespace hud::details::hashmap
{

    alignas(16) constexpr const control_type EMPTY_GROUP[32] {
        details::hashmap::control_type {0}, // 0
        details::hashmap::control_type {0},
        details::hashmap::control_type {0},
        details::hashmap::control_type {0},
        details::hashmap::control_type {0}, // 4
        details::hashmap::control_type {0},
        details::hashmap::control_type {0},
        details::hashmap::control_type {0},
        details::hashmap::control_type {0}, // 8
        details::hashmap::control_type {0},
        details::hashmap::control_type {0},
        details::hashmap::control_type {0},
        details::hashmap::control_type {0}, // 12
        details::hashmap::control_type {0},
        details::hashmap::control_type {0},
        details::hashmap::control_type {0},
        details::hashmap::control_sentinel, // 16
        details::hashmap::control_empty,
        details::hashmap::control_empty,
        details::hashmap::control_empty,
        details::hashmap::control_empty, // 20
        details::hashmap::control_empty,
        details::hashmap::control_empty,
        details::hashmap::control_empty,
        details::hashmap::control_empty, // 24
        details::hashmap::control_empty,
        details::hashmap::control_empty,
        details::hashmap::control_empty,
        details::hashmap::control_empty, // 28
        details::hashmap::control_empty,
        details::hashmap::control_empty,
        details::hashmap::control_empty, // 31
    };
} // namespace hud::details::hashmap