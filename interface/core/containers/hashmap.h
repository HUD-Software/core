#ifndef HD_INC_CORE_HASHMAP_H
#define HD_INC_CORE_HASHMAP_H
#include "../hash/city_hash.h"
#include "../allocators/heap_allocator.h"
#include "../traits/add_lvalue_reference.h"
#include "optional.h"
#include "pair.h"
#include "../hash.h"

namespace hud
{
    namespace details::hashmap
    {
        template<typename key_t>
        struct default_hasher
        {
            using key_type = key_t;

            [[nodiscard]] u64 operator()(const key_type &key) const noexcept
            {
                return hud::hash_64(key);
            }
        };

        template<typename key_t> struct default_equal
        {
        };

        enum class control_e : u8
        {
            empty = 0b10000000,   // The slot is empty
            deleted = 0b11111110, // The slot is deleted
            sentinel = 0b11111111 // The slot is a sentinel, A sentinel is a special caracter that mark the end of the control for iteration
            // full: 0b0hhhhhhh where h represents the H2 hash bits.
        };

        extern const control_e EMPTY_GROUP[32];

        template<typename key_t, typename value_t>
        struct slot
            : hud::pair<key_t, value_t>
        {
            using key_type = hud::pair<key_t, value_t>::first_type;
            using value_type = hud::pair<key_t, value_t>::second_type;
        };

        template<typename slot_t>
        class iterator
        {
            using slot_type = slot_t;
            using key_type = slot_type::key_type;
            using value_type = slot_type::value_type;
            using pointer_type = slot_type *;
            using reference_type = hud::add_lvalue_reference<slot_type>;

            iterator()
            {
            }

            reference_type operator*() const
            {
                return *slot_;
            }

            pointer_type operator->() const
            {
                return &operator*();
            }

            // PRECONDITION: not an end() iterator.
            iterator &operator++()
            {
                // skip all empty slot after the current one
                // If we reach the sentinel set ctrl to nullptr
                return *this;
            }

            // PRECONDITION: not an end() iterator.
            iterator operator++(int)
            {
                auto tmp = *this;
                ++*this;
                return tmp;
            }

            friend bool operator==(const iterator &a, const iterator &b)
            {
                return a.ctrl_ == b.ctrl_;
            }

            friend bool operator!=(const iterator &a, const iterator &b)
            {
                return !(a == b);
            }

        private:
            // The current control we are iterating
            control_e *control_ {const_cast<control_e *>(&EMPTY_GROUP[16])};
            // The current slot we are iterating. Keep uninitialized.
            value_type *slot_;
        };

        template<
            typename slot_t,
            typename hash_t,
            typename key_equal_t,
            typename allocator_t>
        class hashmap_impl
        {
        protected:
            /** Type of the slot. */
            using slot_type = slot_t;
            /** Type of the key. */
            using key_type = slot_type::key_type;
            /** Type of the value. */
            using value_type = slot_type::value_type;
            /** Type of the hash function. */
            using hash_type = hash_t;
            /****/
            using iterator = iterator<slot_type>;

        public:
            constexpr hashmap_impl() noexcept
                : control_(const_cast<control_e *>(&EMPTY_GROUP[16])) // Point to the sentinel in the empty group, const_cast is ok, EMPTY_GROUP is used only when the table is empty
            {
            }

            constexpr hud::optional<iterator> insert(const key_type &key, const value_type &value) noexcept
            {
                return find_or_prepare_insert(key);
            }

        private:
            constexpr hud::optional<iterator> find_or_prepare_insert(const key_type &key)
            {
                // TODO: prefetch control bloc
                // Hash the key
                u64 hash = hash_type {}(key);
                return hud::nullopt;
            }

        private:
            /**
             * The capacity of the hashmap.
             * Refers to the number of slots available in the allocation, not the capacity of the
             * allocation itself.
             */
            usize capacity_ {0};

            /** The count of values in the hashmap. */
            usize count_ {0};

            /** Pointer to the control segment. */
            control_e *control_;

            /** Pointer to the slot segment. */
            value_type *slots_;
        };

    } // namespace details::hashmap

    template<
        typename key_t,
        typename value_t,
        typename hash_t = details::hashmap::default_hasher<key_t>,
        typename key_equal_t = details::hashmap::default_equal<key_t>,
        typename allocator_t = heap_allocator>
    class hashmap
        : details::hashmap::hashmap_impl<details::hashmap::slot<key_t, value_t>, hash_t, key_equal_t, allocator_t>

    {
    private:
        using super = details::hashmap::hashmap_impl<details::hashmap::slot<key_t, value_t>, hash_t, key_equal_t, allocator_t>;

    public:
        /** Type of the hash function. */
        using typename super::hash_type;
        /** Type of the key. */
        using typename super::key_type;
        /** Type of the value. */
        using typename super::value_type;

        using super::super;

        using super::insert;
    };
} // namespace hud

#endif // HD_INC_CORE_HASHMAP_H