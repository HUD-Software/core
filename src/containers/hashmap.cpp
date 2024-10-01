#include <core/containers/hashmap.h>

namespace hud::details::hashmap
{

    alignas(16) constexpr const control_e EMPTY_GROUP[32] {
        details::hashmap::control_e {0}, // 0
        details::hashmap::control_e {0},
        details::hashmap::control_e {0},
        details::hashmap::control_e {0},
        details::hashmap::control_e {0}, // 4
        details::hashmap::control_e {0},
        details::hashmap::control_e {0},
        details::hashmap::control_e {0},
        details::hashmap::control_e {0}, // 8
        details::hashmap::control_e {0},
        details::hashmap::control_e {0},
        details::hashmap::control_e {0},
        details::hashmap::control_e {0}, // 12
        details::hashmap::control_e {0},
        details::hashmap::control_e {0},
        details::hashmap::control_e {0},
        details::hashmap::control_e::sentinel, // 16
        details::hashmap::control_e::empty,
        details::hashmap::control_e::empty,
        details::hashmap::control_e::empty,
        details::hashmap::control_e::empty, // 20
        details::hashmap::control_e::empty,
        details::hashmap::control_e::empty,
        details::hashmap::control_e::empty,
        details::hashmap::control_e::empty, // 24
        details::hashmap::control_e::empty,
        details::hashmap::control_e::empty,
        details::hashmap::control_e::empty,
        details::hashmap::control_e::empty, // 28
        details::hashmap::control_e::empty,
        details::hashmap::control_e::empty,
        details::hashmap::control_e::empty, // 31
    };
} // namespace hud::details::hashmap