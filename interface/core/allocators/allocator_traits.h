#ifndef HD_INC_CORE_ALLOCATOR_TRAITS_H
#define HD_INC_CORE_ALLOCATOR_TRAITS_H
#include "../traits/integral_constant.h"
#include "../traits/is_bitwise_copyable.h"
#include "../traits/is_empty.h"
#include "../traits/disjunction.h"

namespace hud
{
    /** `allocator_traits` is used for allocator aware containers. */
    template<typename allocator_t>
    struct allocator_traits
    {
        /**
         * Indicates whether `allocator_t` should never be copy/move constructed and copy/move assigned.
         * If `hud::true_type`, the allocator should never be copied or moved.
         * If `hud::false_type`, the allocator can be copy/move constructed and assigned.
         */
        using is_always_equal = hud::disjunction<hud::is_empty<allocator_t>>;

        /**
         * Indicates whether `allocator_t` should never be copy assigned when the container is copy assigned.
         * If `hud::true_type`, the allocator should never be copy assigned.
         * If `hud::false_type`, the allocator can be copy assigned when the container is copy assigned.
         */
        using copy_when_container_copy = hud::false_type;

        /**
         * Indicates whether `allocator_t` should never be move assigned when the container is move assigned.
         * If `hud::true_type`, the allocator should never be move assigned.
         * If `hud::false_type`, the allocator can be move assigned when the container is move assigned.
         */
        using move_when_container_move = hud::false_type;
    };

} // namespace hud

#endif // HD_INC_CORE_ALLOCATOR_TRAITS_H