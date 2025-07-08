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
        class hashmap_storage
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
             * Retrieves a reference to the element at the specified index from a non-const hashmap_storage object.
             * @tparam idx_to_reach The index of the element to retrieve.
             * @param s The hashmap_storage object.
             * @return A reference to the element at the specified index.
             */
            template<usize idx_to_reach>
            [[nodiscard]] friend constexpr decltype(auto) get(hashmap_storage &s) noexcept
            {
                return hud::get<idx_to_reach>(s.element_);
            }

            /**
             * Retrieves a const reference to the element at the specified index from a const hashmap_storage object.
             * @tparam idx_to_reach The index of the element to retrieve.
             * @param s The const hashmap_storage object.
             * @return A const reference to the element at the specified index.
             */
            template<usize idx_to_reach>
            [[nodiscard]] friend constexpr decltype(auto) get(const hashmap_storage &s) noexcept
            {
                return hud::get<idx_to_reach>(s.element_);
            }

            /**
             * Retrieves a reference to the element at the specified index from a hashmap_storage object.
             * @tparam idx_to_reach The index of the element to retrieve.
             * @param s The hashmap_storage object.
             * @return A reference to the element at the specified index.
             */
            template<usize idx_to_reach>
            [[nodiscard]] friend constexpr decltype(auto) get(hashmap_storage &&s) noexcept
            {
                return hud::get<idx_to_reach>(hud::forward<hashmap_storage>(s).element_);
            }

            /**
             * Retrieves a const reference to the element at the specified index from a const hashmap_storage object.
             * @tparam idx_to_reach The index of the element to retrieve.
             * @param s The const hashmap_storage object.
             * @return A const reference to the element at the specified index.
             */
            template<usize idx_to_reach>
            [[nodiscard]] friend constexpr decltype(auto) get(const hashmap_storage &&s) noexcept
            {
                return hud::get<idx_to_reach>(hud::forward<const hashmap_storage>(s).element_);
            }

            /**
             * Copy constructor.
             * Does not accept throwable copy constructible components.
             * @param other Another pair object.
             */
            constexpr explicit(!(hud::is_convertible_v<const pair_type &, pair_type>)) hashmap_storage(const hashmap_storage &other) noexcept
            requires(hud::is_nothrow_copy_constructible_v<pair_type>)
            = default;

            /**
             * Copy constructor for different key and value types.
             * @tparam u_key_t Type of the key in the other hashmap_storage.
             * @tparam u_value_t Type of the value in the other hashmap_storage.
             * @param other The other hashmap_storage object to copy from.
             */
            template<typename u_key_t, typename u_value_t>
            requires(hud::is_copy_constructible_v<pair_type, hud::pair<u_key_t, u_value_t>>)
            constexpr explicit(!hud::is_convertible_v<const pair_type &, hud::pair<u_key_t, u_value_t>>) hashmap_storage(const hashmap_storage<u_key_t, u_value_t> &other) noexcept
                : element_(other.element_)
            {
                static_assert(hud::is_nothrow_copy_constructible_v<key_t, u_key_t>, "key_t(const u_key_t&) copy constructor is throwable. hashmap_storage is not designed to allow throwable copy constructible components");
                static_assert(hud::is_nothrow_copy_constructible_v<value_t, u_value_t>, "value_t(const u_value_t&) copy constructor is throwable. hashmap_storage is not designed to allow throwable copy constructible components");
            }

            /**
             * Constructor that initializes the hashmap_storage with a key and a value.
             * @tparam u_key_t Type of the key.
             * @tparam u_value_t Type of the value.
             * @param key The key to initialize with.
             * @param value The value to initialize with.
             */
            template<typename u_key_t, typename u_value_t>
            requires(hud::is_constructible_v<pair_type, const u_key_t &, const u_value_t &>)
            constexpr explicit hashmap_storage(const u_key_t &key, const u_value_t &value) noexcept
                : element_(key, value)
            {
                static_assert(hud::is_nothrow_constructible_v<pair_type, const u_key_t &, const u_value_t &>);
            }

            /**
             * Constructor that initializes the hashmap_storage with a key and a value.
             * @tparam u_key_t Type of the key.
             * @tparam u_value_t Type of the value.
             * @param key The key to initialize with.
             * @param value The value to initialize with.
             */
            template<typename u_key_t, typename u_value_t>
            requires(hud::is_constructible_v<pair_type, u_key_t, u_value_t>)
            constexpr explicit hashmap_storage(u_key_t &&key, u_value_t &&value) noexcept
                : element_(hud::forward<u_key_t>(key), hud::forward<u_value_t>(value))
            {
                static_assert(hud::is_nothrow_constructible_v<pair_type, u_key_t, u_value_t>);
            }

            /**
             * Constructor that initializes the hashmap_storage with a key and a value.
             * @tparam u_key_t Type of the key.
             * @tparam u_value_t Type of the value.
             * @param key The key to initialize with.
             * @param value The value to initialize with.
             */
            template<typename key_tuple_t, typename value_tuple_t>
            constexpr explicit hashmap_storage(hud::tag_piecewise_construct_t, key_tuple_t &&key_tuple, value_tuple_t &&value_tuple) noexcept
                : element_(hud::tag_piecewise_construct, hud::forward<key_tuple_t>(key_tuple), hud::forward<value_tuple_t>(value_tuple))
            {
            }

            /**
             * Copy assign.
             * Does not accept throwable copy constructible components.
             * @param other Another pair object.
             */
            constexpr hashmap_storage &operator=(const hashmap_storage &other) noexcept
            requires(hud::is_nothrow_copy_assignable_v<pair_type>)
            = default;

        protected:
            /**
             * Move constructor.
             * Does not accept throwable copy constructible components.
             * @param other Another hashmap_storage object to move from.
             */
            constexpr hashmap_storage(hashmap_storage &&other) noexcept
            requires(hud::is_nothrow_move_constructible_v<pair_type>)
            = default;

            /**
             * Move assign.
             * Does not accept throwable move constructible components.
             * @param other Another pair object.
             */
            constexpr hashmap_storage &operator=(hashmap_storage &&other) noexcept
            requires(hud::is_nothrow_move_assignable_v<pair_type>)
            = default;

            /**
             * Move constructor for different key and value types.
             * @tparam u_key_t Type of the key in the other hashmap_storage.
             * @tparam u_value_t Type of the value in the other hashmap_storage.
             * @param other The other hashmap_storage object to move from.
             */
            template<typename u_key_t, typename u_value_t>
            requires(hud::is_move_constructible_v<pair_type, hud::pair<u_key_t, u_value_t>>)
            constexpr explicit(!hud::is_convertible_v<pair_type, pair_type>) hashmap_storage(hashmap_storage<u_key_t, u_value_t> &&other) noexcept
                : element_(hud::move(other.element_))
            {
                static_assert(hud::is_nothrow_move_constructible_v<key_t, u_key_t>, "key_t(u_key_t&&) move constructor is throwable. hashmap_storage is not designed to allow throwable move constructible components");
                static_assert(hud::is_nothrow_move_constructible_v<value_t, u_value_t>, "value_t(u_value_t&&) move constructor is throwable. hashmap_storage is not designed to allow throwable move constructible components");
            }

            /** hashmap_storage with other key or value can access private members of hashmap_storage. */
            template<typename u_key_t, typename u_value_t>
            friend class hashmap_storage;
            /** Only the slot can move construct storage. */
            template<typename u_storage>
            friend struct slot;

        protected:
            /** The pair containing the key and value. */
            pair_type element_;
        };

    } // namespace details::hashmap

    /** The default allocator type for hash maps. */
    using hashmap_default_allocator = hud::heap_allocator;

    /**
     * A hash map class that provides key-value storage with customizable hashing, equality comparison, and memory allocation.
     * This class is an implementation inspired by bseil flat_hash_set, leveraging efficient hash set operations.
     *
     * @tparam key_t The type of the keys in the hash map.
     * @tparam value_t The type of the values in the hash map.
     * @tparam hasher_t The type of the hash function to use (default is `hashmap_default_hasher`).
     * @tparam key_equal_t The type of the equality comparator to use (default is `hashmap_default_key_equal`).
     * @tparam allocator_t The type of the allocator to use (default is `hashmap_default_allocator`).
     */
    template<
        typename key_t,
        typename value_t,
        typename hasher_t = hud::hash_64<key_t>,
        typename key_equal_t = hud::equal<key_t>,
        typename allocator_t = hashmap_default_allocator>
    class hashmap
        : public details::hashset::hashset_impl<details::hashmap::hashmap_storage<key_t, value_t>, hasher_t, key_equal_t, allocator_t>
    {

    private:
        /** Alias for the base class to simplify access to its members. */
        using super = details::hashset::hashset_impl<details::hashmap::hashmap_storage<key_t, value_t>, hasher_t, key_equal_t, allocator_t>;

    public:
        /** Type of the hash function. */
        using typename super::hasher_type;
        /** Type of the storage used to store key-value pairs. */
        using storage_type = typename super::storage_type;
        /** Type of the key. */
        using key_type = typename storage_type::key_type;
        /** Type of the value. */
        using value_type = typename storage_type::value_type;

        /** Inherit constructors and methods from the base class. */
        // using super::add;
        using super::reserve;
        using super::super;
        using typename super::allocator_type;
        using typename super::const_iterator;
        using typename super::iterator;
        using super::operator=;

        /** Default constructor. */
        explicit constexpr hashmap() noexcept = default;

        /**
         * Constructor that initializes the hash map with a list of key-value pairs.
         * @tparam u_key_t The type of the keys in the initializer list.
         * @tparam u_value_t The type of the values in the initializer list.
         * @param list The initializer list of key-value pairs.
         * @param allocator The allocator to use for memory management.
         */
        template<typename u_key_t = key_t, typename u_value_t = value_t>
        requires(hud::is_copy_constructible_v<key_t, u_key_t> && hud::is_copy_constructible_v<value_t, u_value_t>)
        constexpr hashmap(std::initializer_list<hud::pair<u_key_t, u_value_t>> list, const allocator_type &allocator = allocator_type()) noexcept
            : super(allocator)
        {
            static_assert(hud::is_nothrow_copy_constructible_v<key_t, u_key_t>, "key_t(const u_key_t&) copy constructor is throwable. hashmap_storage is not designed to allow throwable copy constructible components");
            static_assert(hud::is_nothrow_copy_constructible_v<value_t, u_value_t>, "value_t(const u_value_t&) copy constructor is throwable. hashmap_storage is not designed to allow throwable copy constructible components");
            reserve(list.size());
            for (const auto &pair : list)
            {
                add(pair);
            }
        }

        /**
         * Constructor that initializes the hash map with a list of key-value pairs and additional capacity.
         * @tparam u_key_t The type of the keys in the initializer list.
         * @tparam u_value_t The type of the values in the initializer list.
         * @param list The initializer list of key-value pairs.
         * @param extra_element_count Additional capacity to reserve.
         * @param allocator The allocator to use for memory management.
         */

        template<typename u_key_t = key_t, typename u_value_t = value_t>
        requires(hud::is_copy_constructible_v<key_t, u_key_t> && hud::is_copy_constructible_v<value_t, u_value_t>)
        constexpr hashmap(std::initializer_list<hud::pair<u_key_t, u_value_t>> list, const usize extra_element_count, const allocator_type &allocator = allocator_type()) noexcept
            : super(allocator)
        {
            static_assert(hud::is_nothrow_copy_constructible_v<key_t, u_key_t>, "key_t(const u_key_t&) copy constructor is throwable. hashmap_storage is not designed to allow throwable copy constructible components");
            static_assert(hud::is_nothrow_copy_constructible_v<value_t, u_value_t>, "value_t(const u_value_t&) copy constructor is throwable. hashmap_storage is not designed to allow throwable copy constructible components");
            reserve(list.size() + extra_element_count);
            for (const auto &pair : list)
            {
                add(pair);
            }
        }

        /**
         * Insert a key-value pair into the hash map.
         * @tparam u_key_t The type of the key to insert.
         * @tparam u_value_t The type of the value to insert.
         * @param pair The key-value pair to insert.
         * @return An iterator to the inserted value.
         */
        template<typename u_key_t = key_t, typename u_value_t = value_t>
        constexpr iterator add(const hud::pair<u_key_t, u_value_t> &pair) noexcept
        {
            return add(pair.first, pair.second);
        }

        /**
         * Insert a key-value pair into the hash map (rvalue reference version).
         * @tparam u_key_t The type of the key to insert.
         * @tparam u_value_t The type of the value to insert.
         * @param pair The key-value pair to insert.
         * @return An iterator to the inserted value.
         */
        template<typename u_key_t = key_t, typename u_value_t = value_t>
        constexpr iterator add(hud::pair<u_key_t, u_value_t> &&pair) noexcept
        {
            return super::add(hud::forward<u_key_t &&>(pair.first), hud::forward<u_value_t &&>(pair.second));
        }

        /**
         * Insert a key in the hashset.
         * @param key The key associated with the `value`
         * @param args List of arguments pass to `value_type` constructor after the `key` itself
         * @return Iterator to the `value`
         */
        template<typename u_key_t = key_t, typename u_value_t = value_t>
        constexpr iterator add(u_key_t &&key, u_value_t &&value) noexcept
        requires(hud::is_constructible_v<storage_type, u_key_t, u_value_t>)
        {
            return super::add(hud::forward<u_key_t>(key), hud::forward<u_value_t>(value));
        }

        /**
         * Insert a key and value in the hashmap by piecewise construct them in place.
         * If the `key_type` is not hashable with the `key_tuple_t` a temporary key is created to find it in the hashmap
         * To make the `key_type` hashable with the `key_tuple_t` you must specialize the `hud::equal<key_type>` functor by adding the function
         * `[[nodiscard]] constexpr bool operator()(const hud::tuple<...&&> &rhs) const noexcept` or by
         */
        template<typename key_tuple_t, typename value_tuple_t>
        constexpr iterator add(hud::tag_piecewise_construct_t, key_tuple_t &&key_tuple, value_tuple_t &&value_tuple) noexcept
        {
            return super::add(hud::tag_piecewise_construct, hud::forward<key_tuple_t>(key_tuple), hud::forward<value_tuple_t>(value_tuple));
        }
    };

    /**
     * Specialization of `hud::tuple_size` for `details::hashmap::hashmap_storage`.
     * Defines the size of the tuple as 2, representing a key-value pair.
     *
     * @tparam key_t The type of the key.
     * @tparam value_t The type of the value.
     */

    template<typename key_t, typename value_t>
    struct tuple_size<details::hashmap::hashmap_storage<key_t, value_t>>
        : hud::integral_constant<usize, 2>
    {
    };

    /**
     * Specialization of `hud::tuple_element` for `details::hashmap::hashmap_storage`.
     * Provides access to the types of the elements in the tuple (key or value).
     *
     * @tparam idx_to_reach The index of the element to access (0 for key, 1 for value).
     * @tparam key_t The type of the key.
     * @tparam value_t The type of the value.
     */
    template<usize idx_to_reach, typename key_t, typename value_t>
    struct tuple_element<idx_to_reach, details::hashmap::hashmap_storage<key_t, value_t>>
    {
        static_assert(idx_to_reach < 2, "hashmap hashmap_storage index out of bounds");
        using type = hud::conditional_t<idx_to_reach == 0, const typename details::hashmap::hashmap_storage<key_t, value_t>::key_type, typename details::hashmap::hashmap_storage<key_t, value_t>::value_type>;
    };

    template<typename key_t, typename value_t, typename hasher_t, typename key_equal_t, typename allocator_t>
    constexpr void swap(hashmap<key_t, value_t, hasher_t, key_equal_t, allocator_t> &first, hashmap<key_t, value_t, hasher_t, key_equal_t, allocator_t> &second) noexcept
    {
        first.swap(second);
    }

    template<typename key_t, typename value_t, typename hasher_t, typename key_equal_t, typename allocator_t>
    [[nodiscard]] constexpr bool operator==(const hashmap<key_t, value_t, hasher_t, key_equal_t, allocator_t> &left, const hashmap<key_t, value_t, hasher_t, key_equal_t, allocator_t> &right) noexcept
    {
        // Map are not equal if the counts of elements differ
        if (left.count() != right.count())
        {
            return false;
        }

        // Speed of find is dependent of the max_slot_count_
        // We want to find in the smallest max_slot_count and iterate on the bigger only once
        const hashmap<key_t, value_t, hasher_t, key_equal_t, allocator_t> *biggest_capacity = &left;
        const hashmap<key_t, value_t, hasher_t, key_equal_t, allocator_t> *smallest_capacity = &right;
        if (smallest_capacity->max_count() > biggest_capacity->max_count())
        {
            hud::swap(biggest_capacity, smallest_capacity);
        }

        // Iterate over biggest capacity and find in the smallest each elements
        for (const auto &elem : *biggest_capacity)
        {
            const auto &it = smallest_capacity->find(elem.key());
            if (it == smallest_capacity->end() && !(it->value() == elem.value()))
            {
                return false;
            }
        }
        return true;
    }

} // namespace hud

namespace std
{
    /**
     * Specialization of `std::tuple_size` for `hud::details::hashmap::hashmap_storage`.
     * This specialization defines the size of the tuple as 2, representing a key-value pair.
     * It inherits from `hud::tuple_size` to leverage its implementation.
     *
     * @tparam key_t The type of the key.
     * @tparam value_t The type of the value.
     */
    template<typename key_t, typename value_t>
    struct tuple_size<hud::details::hashmap::hashmap_storage<key_t, value_t>>
        : hud::tuple_size<hud::details::hashmap::hashmap_storage<key_t, value_t>>
    {
    };

    /**
     * Specialization of `std::tuple_element` for `hud::details::hashmap::hashmap_storage`.
     * This specialization provides access to the types of the elements in the tuple (key or value).
     * It inherits from `hud::tuple_element` to leverage its implementation.
     *
     * @tparam idx_to_reach The index of the element to access (0 for key, 1 for value).
     * @tparam key_t The type of the key.
     * @tparam value_t The type of the value.
     */
    template<std::size_t idx_to_reach, typename key_t, typename value_t>
    struct tuple_element<idx_to_reach, hud::details::hashmap::hashmap_storage<key_t, value_t>>
        : hud::tuple_element<idx_to_reach, hud::details::hashmap::hashmap_storage<key_t, value_t>>
    {
    };
} // namespace std
#endif // HD_INC_CORE_HASHMAP_H