#ifndef HD_INC_CORE_HASHMAP_H
#define HD_INC_CORE_HASHMAP_H
#include "hashset.h"

namespace hud
{
    namespace details::hashmap
    {

        template<typename key_t, typename value_t>
        struct slot
            : hud::pair<key_t, value_t>
        {
            using super = hud::pair<key_t, value_t>;
            using super::super;

            using key_type = typename hud::pair<key_t, value_t>::first_type;
            using value_type = typename hud::pair<key_t, value_t>::second_type;

            [[nodiscard]] const key_t &key() const noexcept
            {
                return hud::get<0>(*this);
            }

            [[nodiscard]] const value_t &value() const & noexcept
            {
                return hud::get<1>(*this);
            }

            [[nodiscard]] value_t &value() & noexcept
            {
                return hud::get<1>(*this);
            }

            [[nodiscard]] const value_t &&value() const && noexcept
            {
                return hud::move(hud::get<1>(*this));
            }

            [[nodiscard]] value_t &&value() && noexcept
            {
                return hud::move(hud::get<1>(*this));
            }
        };

        template<typename key_t>
        struct default_hasher
            : hud::hasher_64
        {
            using key_type = key_t;
        };

        template<typename key_t>
        struct default_equal
            : hud::equal<key_t>
        {
        };

        struct default_allocator
            : hud::heap_allocator
        {
        };

    } // namespace details::hashmap

    template<typename key_t>
    using hashmap_default_hasher = details::hashmap::default_hasher<key_t>;

    template<typename key_t>
    using hashmap_default_key_equal = details::hashmap::default_equal<key_t>;

    using hashmap_default_allocator = details::hashmap::default_allocator;

    template<
        typename key_t,
        typename value_t,
        typename hasher_t = hashmap_default_hasher<key_t>,
        typename key_equal_t = hashmap_default_key_equal<key_t>,
        typename allocator_t = hashmap_default_allocator>
    class hashmap
        : public details::hashset::hashset_impl<details::hashmap::slot<key_t, value_t>, hasher_t, key_equal_t, allocator_t>

    {

    private:
        using super = details::hashset::hashset_impl<details::hashmap::slot<key_t, value_t>, hasher_t, key_equal_t, allocator_t>;

    public:
        /** Type of the hash function. */
        using typename super::hasher_type;
        /** Type of the key. */
        using typename super::key_type;
        /** Type of the value. */
        using typename super::value_type;
    };
} // namespace hud

#endif // HD_INC_CORE_HASHMAP_H