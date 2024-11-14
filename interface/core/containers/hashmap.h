#ifndef HD_INC_CORE_HASHMAP_H
#define HD_INC_CORE_HASHMAP_H
#include "../hash/city_hash.h"
#include "../allocators/heap_allocator.h"
#include "../traits/add_lvalue_reference.h"
#include "optional.h"
#include "pair.h"
#include "../hash.h"
#include "../bits.h"

namespace hud
{
    namespace details::hashmap
    {
        template<typename key_t>
        struct default_hasher
            : hud::hasher_64
        {
            using key_type = key_t;

            /** The hasher hash ansichar and wchar with their size. */
            template<typename... type_t>
            [[nodiscard]] constexpr hasher_64 &operator()(const ansichar *value, const usize length) noexcept
            {
                return hud::hasher_64::hash(value, length).hash(reinterpret_cast<const ansichar *>(&length), sizeof(length));
            }

            /** The hasher hash ansichar and wchar with their size. */
            template<typename... type_t>
            [[nodiscard]] constexpr hasher_64 &operator()(const ansichar *value) noexcept
            {
                return (*this)(value, hud::cstring::length(value));
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
            using key_type = typename hud::pair<key_t, value_t>::first_type;
            using value_type = typename hud::pair<key_t, value_t>::second_type;
        };

        class bitmask
        {
        };

        struct group_portable
        {
            static constexpr u64 slot_per_group_ = 8;

            constexpr group_portable(const control_e *control) noexcept
                : value_(hud::memory::unaligned_load64(control))
            {
            }

            u64 match(u8 h2_hash)
            {
                return hud::bits::has_zero_byte(hud::bits::has_value_byte(value_, h2_hash));
            }

            u64 mask_of_empty_slot()
            {
                return hud::bits::has_zero_byte(hud::bits::has_value_byte(value_, static_cast<u8>(control_e::empty)));
            }

        private:
            u64 value_;
        };

        using group = group_portable;

        template<typename group_t = group>
        class control_iterator
        {
        public:
            constexpr control_iterator(u64 h1_hash, u64 mask) noexcept
            {
                hud::check(((mask + 1) & mask) == 0 && "not a mask");
                mask_ = mask;
                offset_ = h1_hash & mask_;
            }

            u64 offset() const
            {
                return offset_;
            }

            u64 offset(u64 i) const
            {
                return (offset_ + i) & mask_;
            }

            void next()
            {
                index_ += Width;
                offset_ += index_;
                offset_ &= mask_;
            }

            // 0-based probe index, a multiple of `Width`.
            u64 index() const
            {
                return index_;
            }

        private:
            /** Mask used to iterate over. */
            u64 mask_;
            /** */
            u64 offset_;
            u64 index_ = 0;
        };

        /** The hashmap iterator that iterate over elements. */
        template<typename slot_t>
        class iterator
        {
            using slot_type = slot_t;
            using key_type = typename slot_type::key_type;
            using value_type = typename slot_type::value_type;
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
            // The current control we are iterating. For the moment set to the global init control
            control_e *control_ {const_cast<control_e *>(&EMPTY_GROUP[32])};
            // The current slot we are iterating. Keep uninitialized.
            value_type *slot_;
        };

        /** Retrives the H1 (57 first bit) of a hash. */
        [[nodiscard]]
        static constexpr u64 H1(u64 hash) noexcept
        {
            return hash >> 7;
        }

        /** Retrives the H2 (7 last bit) of a hash. */
        [[nodiscard]]
        static constexpr u8 H2(u64 hash) noexcept
        {
            return hash & 0x7F;
        }

        template<
            typename slot_t,
            typename hasher_t,
            typename key_equal_t,
            typename allocator_t>
        class hashmap_impl
        {
        protected:
            /** Type of the slot. */
            using slot_type = slot_t;
            /** Type of the key. */
            using key_type = typename slot_type::key_type;
            /** Type of the value. */
            using value_type = typename slot_type::value_type;
            /** Type of the hash function. */
            using hasher_type = hasher_t;
            /** Type of the iterator */
            using iterator = hud::details::hashmap::iterator<slot_type>;

        public:
            constexpr hashmap_impl() noexcept
                : control_(const_cast<control_e *>(&EMPTY_GROUP[32])) // Point to the sentinel in the empty group, const_cast is ok, EMPTY_GROUP is used only when the table is empty
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
                u64 hash = hasher_type {}(key);
                control_iterator probe = control_iterator(H1(hash), max_slot_count_);
                const control_e *control = control_;
                while (true)
                {
                    group g {control + probe.offset()};
                    // for (u32 i : g.match(H2(hash)))
                    // {
                    // }
                    u64 mask_of_empty_slot = g.mask_of_empty_slot();
                    if (mask_of_empty_slot != 0) [[likely]]
                    {
                    }
                }
                return hud::nullopt;
            }

        private:
            /** The max slot count of the hashmap. */
            usize max_slot_count_ {0};

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
        typename hasher_t = details::hashmap::default_hasher<key_t>,
        typename key_equal_t = details::hashmap::default_equal<key_t>,
        typename allocator_t = heap_allocator>
    class hashmap
        : details::hashmap::hashmap_impl<details::hashmap::slot<key_t, value_t>, hasher_t, key_equal_t, allocator_t>

    {
    private:
        using super = details::hashmap::hashmap_impl<details::hashmap::slot<key_t, value_t>, hasher_t, key_equal_t, allocator_t>;

    public:
        /** Type of the hash function. */
        using typename super::hasher_type;
        /** Type of the key. */
        using typename super::key_type;
        /** Type of the value. */
        using typename super::value_type;

        using super::super;

        using super::insert;
    };
} // namespace hud

#endif // HD_INC_CORE_HASHMAP_H