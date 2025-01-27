#ifndef HD_INC_CORE_HASHMAP_H
#define HD_INC_CORE_HASHMAP_H
#include "hashset.h"

namespace hud
{
    namespace details::hashmap
    {
        template<typename key_t, typename value_t>
        struct slot
        {
            using key_type = key_t;
            using value_type = value_t;

            constexpr explicit slot(const key_type &key, const value_type &value) noexcept
                : element_(key, value)
            {
            }

            template<typename u_key_t, typename u_value_t>
            requires(hud::is_constructible_v<hud::pair<key_type, value_type>, u_key_t, u_value_t>)
            constexpr explicit slot(u_key_t &&key, u_value_t &&value) noexcept
                : element_(hud::forward<u_key_t>(key), hud::forward<u_value_t>(value))
            {
            }

            [[nodiscard]] constexpr const key_type &key() noexcept
            {
                return hud::get<0>(element_);
            }

            [[nodiscard]] constexpr const key_type &key() const noexcept
            {
                return hud::get<0>(element_);
            }

            [[nodiscard]] constexpr value_type &value() noexcept
            {
                return hud::get<1>(element_);
            }

            [[nodiscard]] constexpr const value_type &value() const noexcept
            {
                return hud::get<1>(element_);
            }

            template<typename slot_t>
            [[nodiscard]] static constexpr decltype(auto) get_key(slot_t &&s) noexcept
            {
                return hud::get<0>(hud::forward<slot_t>(s).element_);
            }

            template<usize idx_to_reach>
            [[nodiscard]] friend constexpr decltype(auto) get(slot &s) noexcept
            {
                return hud::get<idx_to_reach>(s.element_);
            }

            template<usize idx_to_reach>
            [[nodiscard]] friend constexpr decltype(auto) get(const slot &s) noexcept
            {
                return hud::get<idx_to_reach>(s.element_);
            }

            template<usize idx_to_reach>
            [[nodiscard]] friend constexpr decltype(auto) get(slot &&s) noexcept
            {
                return hud::get<idx_to_reach>(hud::forward<slot>(s).element_);
            }

            template<usize idx_to_reach>
            [[nodiscard]] friend constexpr decltype(auto) get(const slot &&s) noexcept
            {
                return hud::get<idx_to_reach>(hud::forward<const slot>(s).element_);
            }

        private:
            hud::pair<key_type, value_type> element_;
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
        /** Type of the slot. */
        using slot_type = typename super::slot_type;
        /** Type of the key. */
        using key_type = typename super::key_type;
        /** Type of the value. */
        using value_type = typename slot_type::value_type;

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

    template<typename key_t, typename value_t>
    struct tuple_size<details::hashmap::slot<key_t, value_t>>
        : hud::integral_constant<usize, 2>
    {
    };

    template<usize idx_to_reach, typename key_t, typename value_t>
    struct tuple_element<idx_to_reach, details::hashmap::slot<key_t, value_t>>
    {
        static_assert(idx_to_reach < 2, "hashmap slot index out of bounds");
        using type = hud::conditional_t<idx_to_reach == 0, const typename details::hashmap::slot<key_t, value_t>::key_type, typename details::hashmap::slot<key_t, value_t>::value_type>;
    };

} // namespace hud

namespace std
{
    template<typename key_t, typename value_t>
    struct tuple_size<hud::details::hashmap::slot<key_t, value_t>>
        : hud::tuple_size<hud::details::hashmap::slot<key_t, value_t>>
    {
    };

    template<std::size_t idx_to_reach, typename key_t, typename value_t>
    struct tuple_element<idx_to_reach, hud::details::hashmap::slot<key_t, value_t>>
        : hud::tuple_element<idx_to_reach, hud::details::hashmap::slot<key_t, value_t>>
    {
    };
} // namespace std
#endif // HD_INC_CORE_HASHMAP_H