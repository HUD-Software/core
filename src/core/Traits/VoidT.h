#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_VOID_TYPE_H
#define HD_INC_OSLAYER_TRAITS_VOID_TYPE_H

namespace hud {

    /** Utility metafunction that maps a sequence of any types to the type void. */
    template<typename... TTypes>
    using VoidT = void;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_VOID_TYPE_H