#ifndef HD_INC_CORE_HASHMAP_H
#define HD_INC_CORE_HASHMAP_H
#include "hashset.h"

namespace hud
{
    namespace details::hashmap
    {
        /**
         * Key-value storage for the hash map, offering controlled access with specific constraints:
         * - Keys are immutable to ensure consistent and reliable hash calculations.
         * - Values are mutable, allowing for updates and modifications.
         * - The storage is copyable but not movable to maintain the integrity and consistency of the keys.
         *
         * @tparam key_t   The type of the key
         * @tparam value_t The type of the value
         */
        template<typename key_t, typename value_t>
        class slot_storage
        {
        public:
            /** Type of the key. */
            using key_type = key_t;
            /** Type of the value. */
            using value_type = value_t;

        protected:
            using pair_type = hud::pair<key_type, value_type>;

        public:
            /** Retrieves non mutable reference to the key. */
            [[nodiscard]] constexpr const key_type &key() noexcept
            {
                return hud::get<0>(element_);
            }

            /** Retrieves non mutable reference to the key. */
            [[nodiscard]] constexpr const key_type &key() const noexcept
            {
                return hud::get<0>(element_);
            }

            /** Retrieves mutable reference to the key. */
            [[nodiscard]] constexpr value_type &value() noexcept
            {
                return hud::get<1>(element_);
            }

            /** Retrieves non mutable reference to the value. */
            [[nodiscard]] constexpr const value_type &value() const noexcept
            {
                return hud::get<1>(element_);
            }

            /**
             * Retrieves a reference to the element at the specified index from a non-const slot_storage object.
             * @tparam idx_to_reach The index of the element to retrieve.
             * @param s The slot_storage object.
             * @return A reference to the element at the specified index.
             */
            template<usize idx_to_reach>
            [[nodiscard]] friend constexpr decltype(auto) get(slot_storage &s) noexcept
            {
                return hud::get<idx_to_reach>(s.element_);
            }

            /**
             * Retrieves a const reference to the element at the specified index from a const slot_storage object.
             * @tparam idx_to_reach The index of the element to retrieve.
             * @param s The const slot_storage object.
             * @return A const reference to the element at the specified index.
             */
            template<usize idx_to_reach>
            [[nodiscard]] friend constexpr decltype(auto) get(const slot_storage &s) noexcept
            {
                return hud::get<idx_to_reach>(s.element_);
            }

            /**
             * Retrieves a reference to the element at the specified index from a slot_storage object.
             * @tparam idx_to_reach The index of the element to retrieve.
             * @param s The slot_storage object.
             * @return A reference to the element at the specified index.
             */
            template<usize idx_to_reach>
            [[nodiscard]] friend constexpr decltype(auto) get(slot_storage &&s) noexcept
            {
                return hud::get<idx_to_reach>(hud::forward<slot_storage>(s).element_);
            }

            /**
             * Retrieves a const reference to the element at the specified index from a const slot_storage object.
             * @tparam idx_to_reach The index of the element to retrieve.
             * @param s The const slot_storage object.
             * @return A const reference to the element at the specified index.
             */
            template<usize idx_to_reach>
            [[nodiscard]] friend constexpr decltype(auto) get(const slot_storage &&s) noexcept
            {
                return hud::get<idx_to_reach>(hud::forward<const slot_storage>(s).element_);
            }

            /**
             * Copy constructor.
             * Does not accept throwable copy constructible components.
             * @param other Another pair object.
             */
            constexpr explicit(!(hud::is_convertible_v<const pair_type &, pair_type>)) slot_storage(const slot_storage &other) noexcept = default;

        protected:
            /**
             * Constructor that initializes the slot_storage with a key and a value.
             * @tparam u_key_t Type of the key.
             * @tparam u_value_t Type of the value.
             * @param key The key to initialize with.
             * @param value The value to initialize with.
             */
            template<typename u_key_t, typename u_value_t>
            requires(hud::is_constructible_v<hud::pair<key_type, value_type>, u_key_t, u_value_t>)
            constexpr explicit slot_storage(const u_key_t &key, const u_value_t &value) noexcept
                : element_(key, value)
            {
            }

            /**
             * Constructor that initializes the slot_storage with a key and a value.
             * @tparam u_key_t Type of the key.
             * @tparam u_value_t Type of the value.
             * @param key The key to initialize with.
             * @param value The value to initialize with.
             */
            template<typename u_key_t, typename u_value_t>
            requires(hud::is_constructible_v<hud::pair<key_type, value_type>, u_key_t, u_value_t>)
            constexpr explicit slot_storage(u_key_t &&key, u_value_t &&value) noexcept
                : element_(hud::forward<u_key_t>(key), hud::forward<u_value_t>(value))
            {
            }

            /**
             * Move constructor.
             * Does not accept throwable copy constructible components.
             * @param other Another slot_storage object to move from.
             */
            constexpr slot_storage(slot_storage &&other) noexcept = default;

            /**
             * Copy constructor for different key and value types.
             * @tparam u_key_t Type of the key in the other slot_storage.
             * @tparam u_value_t Type of the value in the other slot_storage.
             * @param other The other slot_storage object to copy from.
             */

            template<typename u_key_t = key_t, typename u_value_t = value_t>
            requires(hud::is_copy_constructible_v<hud::pair<key_type, value_type>, hud::pair<u_key_t, u_value_t>>)
            constexpr explicit(!hud::is_convertible_v<const hud::pair<key_type, value_type> &, hud::pair<u_key_t, u_value_t>>) slot_storage(const slot_storage<u_key_t, u_value_t> &other) noexcept
                : element_(other.element_)
            {
                static_assert(hud::is_nothrow_copy_constructible_v<key_t, u_key_t>, "key_t(const u_key_t&) copy constructor is throwable. slot_storage is not designed to allow throwable copy constructible components");
                static_assert(hud::is_nothrow_copy_constructible_v<value_t, u_value_t>, "value_t(const u_value_t&) copy constructor is throwable. slot_storage is not designed to allow throwable copy constructible components");
            }

            /**
             * Move constructor for different key and value types.
             * @tparam u_key_t Type of the key in the other slot_storage.
             * @tparam u_value_t Type of the value in the other slot_storage.
             * @param other The other slot_storage object to move from.
             */
            template<typename u_key_t = key_t, typename u_value_t = value_t>
            requires(hud::is_move_constructible_v<hud::pair<key_type, value_type>, hud::pair<u_key_t, u_value_t>>)
            constexpr explicit(!hud::is_convertible_v<hud::pair<key_type, value_type>, hud::pair<key_type, value_type>>) slot_storage(slot_storage<u_key_t, u_value_t> &&other) noexcept
                : element_(hud::move(other.element_))
            {
                static_assert(hud::is_nothrow_copy_constructible_v<key_t, u_key_t>, "key_t(const u_key_t&) copy constructor is throwable. slot_storage is not designed to allow throwable copy constructible components");
                static_assert(hud::is_nothrow_copy_constructible_v<value_t, u_value_t>, "value_t(const u_value_t&) copy constructor is throwable. slot_storage is not designed to allow throwable copy constructible components");
            }

        private:
            /** slot_storage with other key or value can access private members of slot_storage. */
            template<typename u_key_t, typename u_value_t>
            friend class slot_storage;

        protected:
            /** The pair containing the key and value. */
            pair_type element_;
        };

        template<typename key_t, typename value_t>
        struct slot
            : slot_storage<key_t, value_t>
        {
            using storage = slot_storage<key_t, value_t>;

            template<typename... type_t>
            requires(hud::is_constructible_v<hud::pair<key_t, value_t>, type_t...>)
            constexpr explicit slot(type_t &&...values) noexcept
                : storage(hud::forward<type_t>(values)...)
            {
            }

            constexpr explicit(!hud::is_convertible_v<const hud::pair<key_t, value_t> &, hud::pair<key_t, value_t>>) slot(const slot &other) noexcept
            requires(hud::is_nothrow_copy_constructible_v<hud::pair<key_t, value_t>>)
            = default;

            template<typename u_key_t = key_t, typename u_value_t = value_t>
            requires(hud::is_copy_constructible_v<hud::pair<key_t, value_t>, hud::pair<u_key_t, u_value_t>>)
            constexpr explicit(!hud::is_convertible_v<const hud::pair<key_t, value_t> &, hud::pair<u_key_t, u_value_t>>) slot(const slot<u_key_t, u_value_t> &other) noexcept
                : storage(other)
            {
            }

            constexpr explicit(!(hud::is_convertible_v<hud::pair<key_t, value_t>, hud::pair<key_t, value_t>>)) slot(slot &&other) noexcept
            requires(hud::is_nothrow_move_constructible_v<hud::pair<key_t, value_t>>)
            = default;

            template<typename u_key_t = key_t, typename u_value_t = value_t>
            requires(hud::is_move_constructible_v<hud::pair<key_t, value_t>, hud::pair<u_key_t, u_value_t>>)
            constexpr explicit(!hud::is_convertible_v<hud::pair<key_t, value_t>, hud::pair<u_key_t, u_value_t>>) slot(slot<u_key_t, u_value_t> &&other) noexcept
                : storage(hud::move(other))
            {
            }
        };

        template<typename key_t> struct default_hasher
        {
            /** Hash the value and combine the value with the current hasher value. */
            template<typename... type_t>
            [[nodiscard]] constexpr u64 operator()(type_t &&...values) noexcept
            {
                return hud::hash_64<hud::decay_t<type_t>...> {}(hud::forward<type_t>(values)...);
            }

            /** Hash the value and combine the value with the current hasher value. */
            template<typename... type_t>
            [[nodiscard]] constexpr u64 hash(type_t &&...values) noexcept
            {
                return (*this)(hud::forward<type_t>(values)...);
            }
        };

        template<typename key_t>
        using default_equal = hud::equal<key_t>;

        using default_allocator = hud::heap_allocator;

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

        template<typename u_key_t = key_t, typename u_value_t = value_t>
        requires(hud::is_copy_constructible_v<key_t, u_key_t> && hud::is_copy_constructible_v<value_t, u_value_t>)
        constexpr hashmap(std::initializer_list<hud::pair<u_key_t, u_value_t>> list, const allocator_type &allocator = allocator_type()) noexcept
            : super(allocator)
        {
            reserve(list.size());
            for (const auto &pair : list)
            {
                add(pair);
            }
        }

        template<typename u_key_t = key_t, typename u_value_t = value_t>
        requires(hud::is_copy_constructible_v<key_t, u_key_t> && hud::is_copy_constructible_v<value_t, u_value_t>)
        constexpr hashmap(std::initializer_list<hud::pair<u_key_t, u_value_t>> list, const usize extra_element_count, const allocator_type &allocator = allocator_type()) noexcept
            : super(allocator)
        {
            reserve(list.size() + extra_element_count);
            for (const auto &pair : list)
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
        template<typename u_key_t = key_t, typename u_value_t = value_t>
        constexpr iterator add(const hud::pair<u_key_t, u_value_t> &pair) noexcept
        {
            return add(pair.first, pair.second);
        }

        /**
         * Insert a key in the hashset.
         * @param key The key associated with the `value`
         * @param args List of arguments pass to `value_type` constructor after the `key` itself
         * @return Iterator to the `value`
         */
        template<typename u_key_t = key_t, typename u_value_t = value_t>
        constexpr iterator add(hud::pair<u_key_t, u_value_t> &&pair) noexcept
        {
            return add(hud::get<0>(hud::forward<hud::pair<u_key_t, u_value_t>>(pair)), hud::get<1>(hud::forward<hud::pair<u_key_t, u_value_t>>(pair)));
        }
    };

    template<typename key_t, typename value_t>
    struct tuple_size<details::hashmap::slot_storage<key_t, value_t>>
        : hud::integral_constant<usize, 2>
    {
    };

    template<usize idx_to_reach, typename key_t, typename value_t>
    struct tuple_element<idx_to_reach, details::hashmap::slot_storage<key_t, value_t>>
    {
        static_assert(idx_to_reach < 2, "hashmap slot_storage index out of bounds");
        using type = hud::conditional_t<idx_to_reach == 0, const typename details::hashmap::slot_storage<key_t, value_t>::key_type, typename details::hashmap::slot_storage<key_t, value_t>::value_type>;
    };

} // namespace hud

namespace std
{
    template<typename key_t, typename value_t>
    struct tuple_size<hud::details::hashmap::slot_storage<key_t, value_t>>
        : hud::tuple_size<hud::details::hashmap::slot_storage<key_t, value_t>>
    {
    };

    template<std::size_t idx_to_reach, typename key_t, typename value_t>
    struct tuple_element<idx_to_reach, hud::details::hashmap::slot_storage<key_t, value_t>>
        : hud::tuple_element<idx_to_reach, hud::details::hashmap::slot_storage<key_t, value_t>>
    {
    };
} // namespace std
#endif // HD_INC_CORE_HASHMAP_H