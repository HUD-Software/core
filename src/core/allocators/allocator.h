#ifndef HD_INC_CORE_ALLOCATOR_H
#define HD_INC_CORE_ALLOCATOR_H
#include "allocation.h"

namespace hud
{
    struct allocator
    {

        /** The type of allocation done by this allocator. */
        template<typename type_t>
        using allocation_type = hud::allocation<type_t>;

        template<typename type_t = u8>
        [[nodiscard]] constexpr allocation_type<type_t> allocate(const usize count) noexcept;

        template<typename type_t = u8>
        constexpr void free(const allocation_type<type_t> &buffer) noexcept;
    };

} // namespace hud

#endif // HD_INC_CORE_ALLOCATOR_H