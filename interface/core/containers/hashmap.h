#ifndef HD_INC_CORE_HASHMAP_H
#define HD_INC_CORE_HASHMAP_H
#include "hashset.h"

namespace hud
{
    namespace details::hashmap
    {
        /**
         * Key-value storage for the hash map, offering controlled access with specific constraints:
         *
         * - Keys are immutable to ensure consistent and reliable hash calculations.
         *   Once a key is stored, it cannot be modified, guaranteeing that its hash value
         *   and placement in the hash map remain valid.
         *
         * - Values are mutable, allowing updates and modifications while keeping the key
         *   constant. This ensures that value updates do not invalidate hash map invariants.
         *
         * - The storage is copyable but not freely movable to maintain the integrity and
         *   consistency of keys and values. Move operations are carefully controlled and
         *   only allowed through the `slot` wrapper.
         *
         * @tparam key_t   The type of the key.
         * @tparam value_t The type of the value.
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
            /** Internal pair type holding key and value. */
            using pair_type = hud::pair<key_type, value_type>;

        public:
            /** Retrieves non-mutable reference to the key. */
            [[nodiscard]] constexpr const key_type &key() noexcept
            {
                return hud::get<0>(element_);
            }

            /** Retrieves non-mutable reference to the key. */
            [[nodiscard]] constexpr const key_type &key() const noexcept
            {
                return hud::get<0>(element_);
            }

            /** Retrieves mutable reference to the value. */
            [[nodiscard]] constexpr value_type &value() noexcept
            {
                return hud::get<1>(element_);
            }

            /** Retrieves non-mutable reference to the value. */
            [[nodiscard]] constexpr const value_type &value() const noexcept
            {
                return hud::get<1>(element_);
            }

            /**
             * Retrieves a reference to the element at the specified index from a non-const hashmap_storage object.
             * This function allows tuple-like access (structured binding) to the key/value pair.
             * @tparam idx_to_reach The index of the element to retrieve (0 = key, 1 = value).
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
             * This function allows tuple-like access (structured binding) to the key/value pair.
             * @tparam idx_to_reach The index of the element to retrieve (0 = key, 1 = value).
             * @param s The const hashmap_storage object.
             * @return A const reference to the element at the specified index.
             */
            template<usize idx_to_reach>
            [[nodiscard]] friend constexpr decltype(auto) get(const hashmap_storage &s) noexcept
            {
                return hud::get<idx_to_reach>(s.element_);
            }

            /**
             * Retrieves a reference to the element at the specified index from a non-const rvalue hashmap_storage object.
             * This function allows tuple-like access (structured binding) to the key/value pair and enables moving the element.
             * @tparam idx_to_reach The index of the element to retrieve (0 = key, 1 = value).
             * @param s The hashmap_storage rvalue object.
             * @return A reference to the element at the specified index, which can be moved.
             */
            template<usize idx_to_reach>
            [[nodiscard]] friend constexpr decltype(auto) get(hashmap_storage &&s) noexcept
            {
                return hud::get<idx_to_reach>(hud::forward<hashmap_storage>(s).element_);
            }

            /**
             * Retrieves a const reference to the element at the specified index from a const rvalue hashmap_storage object.
             * This function allows tuple-like access (structured binding) to the key/value pair.
             * @tparam idx_to_reach The index of the element to retrieve (0 = key, 1 = value).
             * @param s The const hashmap_storage rvalue object.
             * @return A const reference to the element at the specified index.
             */
            template<usize idx_to_reach>
            [[nodiscard]] friend constexpr decltype(auto) get(const hashmap_storage &&s) noexcept
            {
                return hud::get<idx_to_reach>(hud::forward<const hashmap_storage>(s).element_);
            }

            /**
             * Copy constructor from the same type.
             * Only enabled if the underlying pair is nothrow copy constructible.
             * @param other The hashmap_storage to copy from.
             */
            constexpr explicit(!(hud::is_convertible_v<const pair_type &, pair_type>)) hashmap_storage(const hashmap_storage &other) noexcept
            requires(hud::is_nothrow_copy_constructible_v<pair_type>)
            = default;

            /**
             * Copy constructor from a hashmap_storage with different key/value types.
             * Only enabled if pair_type is nothrow copy constructible from the other pair type.
             * @tparam u_key_t   Type of the key in the other hashmap_storage.
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
             * Constructor from a key only by copying the key.
             * Initializes the key from a const reference, value is default-constructed.
             * @tparam u_key_t Type of the key.
             * @param key The key to initialize with.
             */
            template<typename u_key_t>
            requires(hud::is_constructible_v<pair_type, const u_key_t &, const value_type &>)
            constexpr explicit hashmap_storage(const u_key_t &key) noexcept
                : element_(hud::tag_piecewise_construct, hud::forward_as_tuple(key), hud::forward_as_tuple())
            {
                static_assert(hud::is_nothrow_constructible_v<pair_type, const u_key_t &, const value_type &>);
            }

            /**
             * Constructor from a key only by moving the key.
             * Initializes the key using perfect forwarding, value is default-constructed.
             * @tparam u_key_t Type of the key.
             * @param key The key to initialize with.
             */
            template<typename u_key_t>
            requires(hud::is_constructible_v<pair_type, u_key_t, value_type>)
            constexpr explicit hashmap_storage(u_key_t &&key) noexcept
                : element_(hud::tag_piecewise_construct, hud::forward_as_tuple(hud::forward<u_key_t>(key)), hud::forward_as_tuple())
            {
                static_assert(hud::is_nothrow_constructible_v<pair_type, u_key_t, value_type>);
            }

            /**
             * Constructor from key and value by copying the key and the value.
             * Both key and value are copied from provided arguments.
             * @tparam u_key_t   Type of the key.
             * @tparam u_value_t Type of the value.
             * @param key   The key to initialize with.
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
             * Constructor from key and value by moving the key and the value.
             * Both key and value are constructed in-place using perfect forwarding.
             * @tparam u_key_t   Type of the key.
             * @tparam u_value_t Type of the value.
             * @param key   The key to initialize with.
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
             * Piecewise constructor from tuples for key and value.
             * Allows in-place construction of complex keys and values.
             * @tparam key_tuple_t   Tuple type containing key constructor arguments.
             * @tparam value_tuple_t Tuple type containing value constructor arguments.
             * @param key_tuple   Tuple to construct the key.
             * @param value_tuple Tuple to construct the value.
             */
            template<typename key_tuple_t, typename value_tuple_t>
            requires(hud::is_constructible_v<pair_type, hud::tag_piecewise_construct_t, key_tuple_t, value_tuple_t>)
            constexpr explicit hashmap_storage(hud::tag_piecewise_construct_t, key_tuple_t &&key_tuple, value_tuple_t &&value_tuple) noexcept
                : element_(hud::tag_piecewise_construct, hud::forward<key_tuple_t>(key_tuple), hud::forward<value_tuple_t>(value_tuple))
            {
            }

            /**
             * Copy assignment operator.
             * Only enabled if pair_type is nothrow copy assignable.
             * @param other Another hashmap_storage to copy from.
             * @return Reference to *this.
             */
            constexpr hashmap_storage &operator=(const hashmap_storage &other) noexcept
            requires(hud::is_nothrow_copy_assignable_v<pair_type>)
            = default;

        protected:
            /**
             * Move constructor.
             * Moves the content of another hashmap_storage into this one.
             * Only enabled if `pair_type` is nothrow move constructible.
             * Protected to prevent the user from moving out the key, which would break the container's invariants.
             * Only the `slot` struct can move the storage safely.
             *
             * @param other Another hashmap_storage to move from.
             */
            constexpr hashmap_storage(hashmap_storage &&other) noexcept
            requires(hud::is_nothrow_move_constructible_v<pair_type>)
            = default;

            /**
             * Move assignment operator.
             * Moves the content of another hashmap_storage into this one.
             * Only enabled if `pair_type` is nothrow move assignable.
             * Assigning a moved-out storage by the user could break container invariants, hence protected.
             *
             * @param other Another hashmap_storage to move from.
             * @return Reference to *this after move assignment.
             */
            constexpr hashmap_storage &operator=(hashmap_storage &&other) noexcept
            requires(hud::is_nothrow_move_assignable_v<pair_type>)
            = default;

            /**
             * Move constructor from a hashmap_storage with potentially different key and value types.
             * Only enabled if `pair_type` is move constructible from `hud::pair<u_key_t, u_value_t>`.
             * Copies the key/value pair from `other` using move semantics.
             * Protected to maintain immutability of keys and container correctness.
             *
             * @tparam u_key_t   Key type in the other hashmap_storage.
             * @tparam u_value_t Value type in the other hashmap_storage.
             * @param other Another hashmap_storage to move from.
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

    /**
     * A high-performance hash map similar to Abseil's flat_hash_map.
     *
     * Template parameters:
     * --------------------
     * - `key_t`       : The type of the keys stored in the map.
     * - `value_t`     : The type of the values associated with keys.
     * - `hasher_t`    : Type providing a 64-bit hash function for `key_t`. Default is `hud::hash_64<key_t>`.
     * - `key_equal_t` : Type providing key equality comparisons. Default is `hud::equal<key_t>`.
     * - `allocator_t` : Allocator type for memory management. Default is `hud::heap_allocator`.
     *
     * Features:
     * ---------
     * 1. Open-addressing hash map with control bytes to track empty, deleted, and full slots.
     * 2. Supports constant-evaluated contexts (`consteval`) and runtime.
     * 3. Supports heterogeneous lookup: you can find, insert, or remove elements using types
     *    other than the stored `key_t` as long as `hasher_t` and `key_equal_t` are specialized
     *    to handle the alternative type `K`.
     * 4. Supports piecewise construction for keys and values. Piecewise construction allows in-place
     *    initialization of complex keys or values.
     * 5. Automatic resizing according to a 7/8 load factor.
     * 6. Keys are immutable to maintain consistent hash behavior.
     * 7. Values are mutable, allowing updates after insertion.
     *
     * Heterogeneous lookup with alternative key types `K`:
     * -----------------------------------------------------
     * You can hash and compare keys directly from their type `K`. To enable this for a user-defined type:
     *  - Specialize the map's `hasher_t` for your type `K` by implementing `operator()(const K&)`
     *    to compute a 64-bit hash. You can also provide overloads for alternative key representations
     *    (like an ID or a tuple) to avoid constructing the full key.
     *  - Specialize the map's `key_equal_t` to provide comparisons between the stored key type
     *    and the type `K` (or alternative key representations). Implement `operator()(const key_t&, const K&)`.
     *
     * Memory management and performance:
     * ----------------------------------
     * - `reserve(count)` allocates memory for at least `count` elements, avoiding unnecessary rehashes.
     * - `clear()` destroys all elements without releasing memory.
     * - `clear_shrink()` destroys all elements and frees memory.
     * - `rehash(max_count)` resizes the underlying table; if `max_count` is less than current capacity, nothing happens.
     * - `shrink_to_fit()` reduces the table to fit the current number of elements exactly.
     * - `free_slot_before_grow()` returns the number of free slots before the next resizing.
     *
     * Iterators:
     * ----------
     * Supports forward iteration over elements (key/value pairs):
     * - `begin()` / `end()` for mutable access
     * - `begin() const` / `end() const` for const access
     *
     * Structured bindings:
     * --------------------
     * You can use structured bindings to access key/value pairs:
     * ```cpp
     * hashmap<int, std::string> m;
     * m.add(42, "Hello");
     * for (auto &[key, value] : m) {
     *     // key is immutable, value is mutable
     *     value += " World";
     * }
     * ```
     *
     * Example:
     * ```cpp
     * struct MyType { int id; };
     *
     * // Hasher specialization
     * template<> struct hud::hash_64<MyType> {
     *     constexpr u64 operator()(const MyType& v) const noexcept { return hud::hash_64<int>{}(v.id); }
     *     constexpr u64 operator()(const int id) const noexcept { return hud::hash_64<int>{}(id); } // heterogeneous support
     * };
     *
     * // Equality specialization
     * template<> struct hud::equal<MyType> {
     *     constexpr bool operator()(const MyType& lhs, const MyType& rhs) const noexcept { return lhs.id == rhs.id; }
     *     constexpr bool operator()(const MyType& lhs, const int& rhs) const noexcept { return lhs.id == rhs; }
     * };
     *
     * hashmap<MyType, std::string> map;
     * map.add(MyType{42}, "Hello"); // insert using MyType key
     * map.add(56, "World");         // insert using alternative key (int)
     * bool has42 = map.contains(MyType{42});
     * bool has56 = map.contains(56);
     * ```
     */
    template<
        typename key_t,
        typename value_t,
        typename hasher_t = hud::hash_64<key_t>,
        typename key_equal_t = hud::equal<key_t>,
        typename allocator_t = hud::heap_allocator>
    class hashmap
        : public details::hashset::hashset_impl<details::hashmap::hashmap_storage<key_t, value_t>, hasher_t, key_equal_t, allocator_t>
    {

    private:
        /** Alias for the base class to simplify access to its members. */
        using super = details::hashset::hashset_impl<details::hashmap::hashmap_storage<key_t, value_t>, hasher_t, key_equal_t, allocator_t>;

    public:
        /** Type of the hash function. */
        using typename super::hasher_type;
        /** Type of the equal function. */
        using typename super::key_equal_type;
        /** Type of the iterator. */
        using typename super::iterator;
        /** Type of the const iterator. */
        using typename super::const_iterator;
        /**  Type of the allocator. */
        using typename super::allocator_type;
        /** Type of the storage used to store key-value pairs. */
        using storage_type = typename super::storage_type;
        /** Type of the key. */
        using key_type = typename storage_type::key_type;
        /** Type of the value. */
        using value_type = typename storage_type::value_type;

        /** Inherit constructors and methods from the base class. */
        using super::reserve;
        using super::super;
        using super::operator=;

        /**
         * Default constructor.
         * Constructs an empty hashmap with default-constructed allocator and other components.
         */
        explicit constexpr hashmap() noexcept = default;

        /**
         * Constructor that initializes the hash map with a list of key-value pairs.
         *
         * Copies all key-value pairs from the initializer list into the map.
         * Ensures that both key and value types are copy-constructible.
         *
         * @tparam u_key_t   Type of keys in the initializer list (default is `key_t`).
         * @tparam u_value_t Type of values in the initializer list (default is `value_t`).
         * @param list       The initializer list of key-value pairs.
         * @param allocator  Allocator to use for memory management (default-constructed if not provided).
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
         * Constructor that initializes the hash map with a list of key-value pairs and reserves extra capacity.
         *
         * Copies all key-value pairs from the initializer list and pre-allocates additional slots
         * to minimize rehashing when adding more elements.
         *
         * @tparam u_key_t   Type of keys in the initializer list (default is `key_t`).
         * @tparam u_value_t Type of values in the initializer list (default is `value_t`).
         * @param list               The initializer list of key-value pairs.
         * @param extra_element_count Extra number of elements to reserve space for.
         * @param allocator          Allocator to use for memory management (default-constructed if not provided).
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
         * Insert a key-value pair into the hash map by coping keys and values.
         *
         * Simply forwards to the `add(key, value)` function.
         *
         * @tparam u_key_t   Type of the key to insert (default is `key_t`).
         * @tparam u_value_t Type of the value to insert (default is `value_t`).
         * @param pair       The key-value pair to insert.
         * @return Iterator pointing to the inserted or existing element.
         */
        template<typename u_key_t = key_t, typename u_value_t = value_t>
        constexpr iterator add(const hud::pair<u_key_t, u_value_t> &pair) noexcept
        {
            return add(pair.first, pair.second);
        }

        /**
         * Insert a key-value pair into the hash map by forwarding pair first and pair second.
         *
         * Moves the key and value into the map to avoid unnecessary copies.
         *
         * @tparam u_key_t   Type of the key to insert (default is `key_t`).
         * @tparam u_value_t Type of the value to insert (default is `value_t`).
         * @param pair       The key-value pair to insert (rvalue).
         * @return Iterator pointing to the inserted or existing element.
         */
        template<typename u_key_t = key_t, typename u_value_t = value_t>
        constexpr iterator add(hud::pair<u_key_t, u_value_t> &&pair) noexcept
        {
            return super::add_impl(hud::forward<u_key_t &&>(pair.first), hud::forward<u_value_t &&>(pair.second));
        }

        /**
         * Insert a key-value pair into the hash map with perfect forwarding.
         *
         * Constructs the key and value in-place using the provided arguments.
         * The key is immutable after insertion; the value remains mutable.
         *
         * @tparam u_key_t   Type of the key to insert (deduced from arguments).
         * @tparam u_value_t Type of the value to insert (deduced from arguments).
         * @param key        The key to insert.
         * @param value      The value to insert.
         * @return Iterator pointing to the inserted or existing element.
         */
        template<typename u_key_t = key_t, typename u_value_t = value_t>
        constexpr iterator add(u_key_t &&key, u_value_t &&value) noexcept
        requires(hud::is_constructible_v<storage_type, u_key_t, u_value_t>)
        {
            return super::add_impl(hud::forward<u_key_t>(key), hud::forward<u_value_t>(value));
        }

        /**
         * Adds a new element to the container using piecewise construction of the key and value.
         *
         * If an element with the given key already exists, returns an iterator to it.
         * Otherwise, constructs a new element in-place using the provided key and value tuples.
         *
         * The key can be provided either as a fully constructed `key_type` or as a tuple of arguments
         * used to construct the key in-place. If the key tuple can't be used directly (e.g., it's not
         * hashable or comparable), it must be convertible into a valid `key_type`.
         *
         * To enable custom key lookup using a tuple of arguments, you can specialize the `hud::equal<key_type>`
         * and `hud::hash<key_type>` functors to support comparisons and hashes against a forwarding tuple
         * (i.e., `hud::tuple<Args&&...>&&`).
         *
         * @param key_tuple   Tuple of arguments used to identify or construct the key.
         * @param value_tuple Tuple of arguments used to construct the associated value.
         * @return An iterator to the existing or newly inserted element.
         */
        template<typename key_tuple_t, typename value_tuple_t>
        constexpr iterator add(hud::tag_piecewise_construct_t, key_tuple_t &&key_tuple, value_tuple_t &&value_tuple) noexcept
        {
            return super::add_impl(hud::tag_piecewise_construct, hud::forward<key_tuple_t>(key_tuple), hud::forward<value_tuple_t>(value_tuple));
        }

        /**
         * Access or insert a value by key.
         *
         * If the key exists, returns a reference to the existing value.
         * Otherwise, inserts a default-constructed value and returns a reference to it.
         *
         * @tparam KeyArgs  Arguments used to construct or find the key.
         * @param args      Arguments to forward to the key constructor or lookup.
         * @return Reference to the value associated with the key.
         */
        template<typename KeyArgs>
        constexpr value_type &operator[](KeyArgs &&args) noexcept
        {
            iterator it = super::add_impl(hud::forward<KeyArgs>(args));
            return it->value();
        }
    };

    /**
     * Provides a `hud::tuple_size` specialization for `hashmap_storage`.
     * This allows `hashmap_storage` to be treated as a tuple of size 2,
     * representing a key-value pair, primarily for structured bindings
     * or tuple-like interface compatibility.
     *
     * Example:
     * ```cpp
     * hashmap<int, hud::string>::storage_type storage{1, "one"};
     * auto [key, value] = storage; // structured binding works
     * ```
     *
     * @tparam key_t   The type of the key.
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

    /**
     * Swaps the contents of two hashmaps.
     *
     * Exchanges all key-value pairs and internal data between `first` and `second`.
     *
     * @tparam key_t       The type of the keys.
     * @tparam value_t     The type of the values.
     * @tparam hasher_t    The type of the hasher.
     * @tparam key_equal_t The type of the key equality comparator.
     * @tparam allocator_t The allocator type.
     * @param first  First hashmap to swap.
     * @param second Second hashmap to swap.
     *
     * Example:
     * ```cpp
     * hud::hashmap<int, std::string> a{{ {1,"one"}, {2,"two"} }};
     * hud::hashmap<int, std::string> b{{ {3,"three"} }};
     * hud::swap(a, b); // a now has key 3 ; b has keys 1 and 2
     * ```
     */
    template<typename key_t, typename value_t, typename hasher_t, typename key_equal_t, typename allocator_t>
    constexpr void swap(hashmap<key_t, value_t, hasher_t, key_equal_t, allocator_t> &first, hashmap<key_t, value_t, hasher_t, key_equal_t, allocator_t> &second) noexcept
    {
        first.swap(second);
    }

    /**
     * Compares two hashmaps for equality.
     *
     * Two hashmaps are considered equal if:
     * 1. They contain the same number of elements.
     * 2. Each key-value pair in one hashmap exists in the other hashmap with an equal value.
     *
     * The comparison is optimized by iterating over the hashmap with the larger capacity
     * and looking up elements in the smaller hashmap.
     *
     * @tparam key_t       The type of keys stored.
     * @tparam value_t     The type of values stored.
     * @tparam hasher_t    The type of the hasher.
     * @tparam key_equal_t The type of the key equality comparator.
     * @tparam allocator_t The allocator type.
     * @param left  First hashmap.
     * @param right Second hashmap.
     * @return `true` if both hashmaps contain the same key-value pairs; `false` otherwise.
     *
     * Example:
     * ```cpp
     * hud::hashmap<int, std::string> a{{ {1,"one"}, {2,"two"} }};
     * hud::hashmap<int, std::string> b{{ {2,"two"}, {1,"one"} }};
     * bool are_equal = (a == b); // true
     * ```
     */
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
     *
     * Enables structured bindings with `hashmap_storage`, allowing syntax like:
     * ```cpp
     * hashmap<int, hud::string>::storage_type storage;
     * auto [key, value] = storage; // Structured binding works, key = storage.key(), value = storage.value()
     * ```
     *
     * The tuple size is defined as 2, representing the key-value pair stored in the hashmap.
     *
     * @tparam key_t   The type of the key.
     * @tparam value_t The type of the value.
     */
    template<typename key_t, typename value_t>
    struct tuple_size<hud::details::hashmap::hashmap_storage<key_t, value_t>>
        : hud::tuple_size<hud::details::hashmap::hashmap_storage<key_t, value_t>>
    {
    };

    /**
     * Specialization of `std::tuple_element` for `hud::details::hashmap::hashmap_storage`.
     *
     * Allows access to the types of the elements in structured bindings or tuple-like access:
     * - index 0 corresponds to the key type (`key_type`).
     * - index 1 corresponds to the value type (`value_type`).
     *
     * Example:
     * ```cpp
     * hashmap<int, hud::string>::storage_type storage{1, "one"};
     * using key_type   = std::tuple_element<0, decltype(storage)>::type; // int
     * using value_type = std::tuple_element<1, decltype(storage)>::type; // hud::string
     * ```
     *
     * @tparam idx_to_reach The index of the element to access (0 = key, 1 = value).
     * @tparam key_t       The type of the key.
     * @tparam value_t     The type of the value.
     */
    template<std::size_t idx_to_reach, typename key_t, typename value_t>
    struct tuple_element<idx_to_reach, hud::details::hashmap::hashmap_storage<key_t, value_t>>
        : hud::tuple_element<idx_to_reach, hud::details::hashmap::hashmap_storage<key_t, value_t>>
    {
    };
} // namespace std
#endif // HD_INC_CORE_HASHMAP_H