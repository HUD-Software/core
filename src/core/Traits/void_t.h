#pragma once
#ifndef HD_INC_CORE_TRAITS_VOID_TYPE_H
#define HD_INC_CORE_TRAITS_VOID_TYPE_H

namespace hud {

    /** Utility metafunction that maps a sequence of any types to the type void. */
    template<typename... args_t>
    using void_t = void;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_VOID_TYPE_H