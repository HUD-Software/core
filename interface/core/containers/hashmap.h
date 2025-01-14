#ifndef HD_INC_CORE_HASHMAP_H
#define HD_INC_CORE_HASHMAP_H
#include "hashset.h"

namespace hud
{
    namespace details::hashmap
    {
        template<typename element_t>
        struct hashmap_slot_func
        {
            using element_type = element_t;
            using const_element_type = const element_type;
            using key_type = typename element_type::first_type;
            using value_type = typename element_type::second_type;

            [[nodiscard]] static constexpr key_type &get_key(element_type &pair) noexcept
            {
                return pair.first;
            }

            [[nodiscard]] static constexpr const key_type &get_key(const element_type &pair) noexcept
            {
                return pair.first;
            }

            [[nodiscard]] static constexpr key_type &&get_key(element_type &&pair) noexcept
            {
                return hud::forward<key_type &&>(pair.first);
            }

            [[nodiscard]] static constexpr const key_type &&get_key(const element_type &&pair) noexcept
            {
                return hud::forward<const key_type &&>(pair.first);
            }

            // template<usize index, typename slot_t, typename key_t, typename value_t>
            // requires(hud::is_same_v<element_type<key_t, value_t>, hud::decay_t<slot_t>>)
            // friend constexpr decltype(auto) get(slot_t &&slot) noexcept
            // {
            //     if constexpr (index == 0)
            //     {
            //         return hud::forward<slot_t>(slot).first;
            //     }
            //     else if constexpr (index == 1)
            //     {
            //         return hud::forward<slot_t>(slot).second;
            //     }
            // }
        };

        // template<typename key_t, typename value_t>
        // class slot
        //     : hud::pair<key_t, value_t>
        // {
        // public:
        //     using super = hud::pair<key_t, value_t>;
        //     using type = super;
        //     using key_type = typename hud::pair<key_t, value_t>::first_type;
        //     using value_type = typename hud::pair<key_t, value_t>::second_type;

        // using super::super;

        // explicit constexpr slot(const key_type &key) noexcept
        //     : super(key, value_type {})
        // {
        // }

        // explicit constexpr slot(key_type &&key) noexcept
        //     : super(std::move(key), value_type {})
        // {
        // }

        // [[nodiscard]] constexpr const key_t &key() const noexcept
        // {
        //     return hud::get<0>(*this);
        // }

        // [[nodiscard]] constexpr const value_t &value() const & noexcept
        // {
        //     return hud::get<1>(*this);
        // }

        // [[nodiscard]] constexpr value_t &value() & noexcept
        // {
        //     return hud::get<1>(*this);
        // }

        // [[nodiscard]] constexpr const value_t &&value() const && noexcept
        // {
        //     return hud::move(hud::get<1>(*this));
        // }

        // [[nodiscard]] constexpr value_t &&value() && noexcept
        // {
        //     return hud::move(hud::get<1>(*this));
        // }
        // };

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
        : public details::hashset::hashset_impl<details::hashmap::hashmap_slot_func<hud::pair<key_t, value_t>>, hasher_t, key_equal_t, allocator_t>
    {

    private:
        using super = details::hashset::hashset_impl<details::hashmap::hashmap_slot_func<hud::pair<key_t, value_t>>, hasher_t, key_equal_t, allocator_t>;

    public:
        /** Type of the hash function. */
        using typename super::hasher_type;
        /** Type of the key. */
        using typename super::key_type;
        /** Type of the value. */
        using typename super::value_type;
        /** Type of the key, value pair. */
        using type = typename super::slot_type;

        /** Type of the value. */
        using super::add;
        using super::reserve;
        using super::super;
        using typename super::allocator_type;
        using typename super::const_iterator;
        using typename super::iterator;
        explicit constexpr hashmap() noexcept = default;

        constexpr explicit hashmap(const allocator_type &allocator) noexcept
            : super(allocator)
        {
        }

        constexpr hashmap(std::initializer_list<hud::pair<key_t, value_t>> list, const allocator_type &allocator = allocator_type()) noexcept
            : super(allocator)
        {
            reserve(list.size());
            for (auto &pair : list)
            {
                add(pair);
            }
        }

        /**
         * Insert a key in the hashset.
         * @param key The key associated with the `value`
         * @param args List of arguments pass to `value_type` constructor after the `key` itself
         * @return Iterator to the `value`
         */
        constexpr iterator add(const hud::pair<key_t, value_t> &pair) noexcept
        {
            return add(pair.first, pair.second);
        }

        /**
         * Insert a key in the hashset.
         * @param key The key associated with the `value`
         * @param args List of arguments pass to `value_type` constructor after the `key` itself
         * @return Iterator to the `value`
         */
        constexpr iterator add(hud::pair<key_t, value_t> &&pair) noexcept
        {
            return add(hud::move(pair.first), hud::move(pair.second));
        }
    };

    // /** Specialize tuple_size for slot that permit structured binding. */
    // template<typename key_t, typename value_t>
    // struct tuple_size<hud::pair<key_t, value_t>>
    //     : hud::integral_constant<usize, 2>
    // {
    // };

    // /** Specialize tuple_element for slot that permit structured binding. */
    // template<std::size_t index, typename key_t, typename value_t>
    // struct tuple_element<index, hud::details::hashmap::slot<key_t, value_t>>
    // {
    //     using type = std::conditional_t<index == 0, key_t, value_t>;
    // };

} // namespace hud

namespace std
{

    template<std::size_t index, typename key_t, typename value_t>
    struct tuple_element<index, hud::pair<key_t, value_t>>
        : hud::tuple_element<index, hud::pair<key_t, value_t>>
    {
    };

} // namespace std

#endif // HD_INC_CORE_HASHMAP_H