#ifndef HD_INC_CORE_HASHSET_H
#define HD_INC_CORE_HASHSET_H
#include "hashmap.h"

namespace hud
{
    namespace details::hashset
    {
        template<typename value_t>
        class slot
        {
        public:
            using key_type = value_t;
            using value_type = value_t;
        };

    } // namespace details::hashset

    template<
        typename value_t,
        typename hash_t = details::hashmap::default_hasher<value_t>,
        typename key_equal_t = details::hashmap::default_equal<value_t>,
        typename allocator_t = heap_allocator>
    class hashset
        : details::hashmap::hashmap_impl<details::hashset::slot<value_t>, hash_t, key_equal_t, allocator_t>
    {
    private:
        using super = details::hashmap::hashmap_impl<details::hashset::slot<value_t>, hash_t, key_equal_t, allocator_t>;

    public:
        /** Type of the hash function. */
        using typename super::hash_type;
        /** Type of the value. */
        using typename super::value_type;

        using super::super;
    };
} // namespace hud

#endif // HD_INC_CORE_HASHSET_H