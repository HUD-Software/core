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

        template<typename slot_t>
        struct default_allocator
            : hud::heap_allocator
        {
        };

        using control_type = u8;

        static constexpr control_type control_empty = 0b10000000;    // The slot is empty
        static constexpr control_type control_deleted = 0b11111110;  // The slot is deleted
        static constexpr control_type control_sentinel = 0b11111111; // The slot is a sentinel, A sentinel is a special caracter that mark the end of the control for iteration
        // full: 0b0hhhhhhh where h represents the H2 hash bits.

        extern const control_type EMPTY_GROUP[32];

        template<typename key_t, typename value_t>
        struct slot
            : hud::pair<key_t, value_t>
        {
            using key_type = typename hud::pair<key_t, value_t>::first_type;
            using value_type = typename hud::pair<key_t, value_t>::second_type;
        };

        struct group_portable_iterator
        {
            constexpr group_portable_iterator(u64 mask) noexcept
                : mask_(mask)
            {
            }

            [[nodiscard]] constexpr explicit operator bool() const noexcept
            {
                return mask_ != 0;
            }

            [[nodiscard]] constexpr u32 insertion_byte_offset() const noexcept
            {
                // Get number of trailing zero and divide it by 8 (>>3) to get the insert offset of the byte
                return hud::bits::trailing_zero(mask_) >> 3;
            }

            u64 mask_;
        };

        struct group_portable
        {
            using iterator_type = group_portable_iterator;
            static constexpr u64 slot_per_group_ = 8;

            constexpr group_portable(const control_type *control) noexcept
                : value_(hud::memory::unaligned_load64(static_cast<const u8 *>(control)))
            {
            }

            constexpr iterator_type match(u8 h2_hash) const noexcept
            {
                return iterator_type {hud::bits::has_zero_byte(hud::bits::has_value_byte(value_, h2_hash))};
            }

            constexpr iterator_type mask_of_empty_slot() const noexcept
            {
                return hud::bits::has_value_byte(value_, control_empty);
            }

            [[nodiscard]] constexpr bool
            operator==(const group_portable &other) const noexcept
            {
                return value_ == other.value_;
            }

            [[nodiscard]] constexpr bool operator!=(const group_portable &other) const noexcept
            {
                return value_ == other.value_;
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
                byte_offset_ = h1_hash & mask_;
            }

            u64 byte_offset() const
            {
                return byte_offset_;
            }

            u64 byte_offset(u64 i) const
            {
                return (byte_offset_ + i) & mask_;
            }

            void next_group()
            {
                // Next group index
                index_ += group::slot_per_group_;
                byte_offset_ += index_;
                byte_offset_ &= mask_;
            }

            // 0-based probe index, a multiple of `Width`.
            u64 index() const
            {
                return index_;
            }

        private:
            /** Mask used to iterate over. */
            u64 mask_;
            /** Byte offset */
            u64 byte_offset_;

            u64 index_ {0};
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
            control_type *control_;
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
            /** Type of the iterator/ */
            using iterator_type = hud::details::hashmap::iterator<slot_type>;
            /**  Type of the allocator. */
            using allocator_type = allocator_t;
            /** The type of allocation done by the allocator. */
            using memory_allocation_type = typename allocator_type::template memory_allocation_type<slot_type>;

        public:
            constexpr hashmap_impl() noexcept
                : control_(const_cast<control_type *>(&EMPTY_GROUP[16])) // Point to the sentinel in the empty group, const_cast is ok, EMPTY_GROUP is used only when the table is empty
            {
            }

            constexpr hud::optional<iterator_type> insert(const key_type &key, const value_type &value) noexcept
            {
                return find_or_prepare_insert(key);
            }

            constexpr void resize(usize new_max_slot_count) noexcept
            {
                max_slot_count_ = new_max_slot_count;

                // Perform allocation of the buffer
                // |-------------------|----------------/
                // | control metadata  | Aligned slots  /
                // |-------------------|----------------/
                constexpr const usize num_cloned_bytes = group::slot_per_group_ - 1;
                const usize control_size = max_count() + 1 + num_cloned_bytes;
                // Compute the control_size that is a multiple of the slot_type alignement
                // We are shure to allocate enough memory for control_size and we ensure to be align on alignof(slot_type) after this allocation size
                // control_size is multiple of sizeof(slot_type)
                const uptr aligned_control_size = hud::memory::align_address(control_size, sizeof(slot_type));
                const usize aligned_allocation_size = aligned_control_size + new_max_slot_count * sizeof(slot_type);

                // Allocate the buffer that will contains control metadata and aligned slots
                memory_allocation_type allocation = allocator_.template allocate<slot_type>(aligned_allocation_size);

                // Save control metadata and slot pointers
                control_ = hud::bit_cast<control_type *>(allocation.data());
                slot_type *slot_ptr = hud::bit_cast<slot_type *>(control_ + aligned_control_size);
                hud::check(hud::memory::is_pointer_aligned(slot_ptr, alignof(slot_type)));

                // Update number of slot we should put into the table before a resizing rehash
                growth_left_ = max_growth_left() - count();

                // Reset control metadata
                hud::memory::set(control_, control_empty, control_size);
                control_[max_slot_count_] = control_sentinel;

                // 
                if (old)
            }

            [[nodiscard]] constexpr usize max_count() const noexcept
            {
                return max_slot_count_;
            }

            [[nodiscard]] constexpr usize count() const noexcept
            {
                return count_;
            }

        private:
            constexpr hud::optional<iterator_type> find_or_prepare_insert(const key_type &key)
            {
                // TODO: prefetch control bloc
                // Hash the key
                u64 hash = hasher_type {}(key);
                control_iterator probe = control_iterator(H1(hash), max_slot_count_);
                const control_type *control = control_;
                while (true)
                {
                    group g {control + probe.byte_offset()};
                    // for (u32 i : g.match(H2(hash)))
                    // {
                    // }
                    group::iterator_type mask_of_empty_slot = g.mask_of_empty_slot();
                    if (mask_of_empty_slot) [[likely]]
                    {
                        u64 insertion_byte_offset = mask_of_empty_slot.insertion_byte_offset();
                        u64 target = probe.byte_offset(insertion_byte_offset);
                        prepare_insert(hash, target, probe.index());
                    }
                }
                return hud::nullopt;
            }

            constexpr u64 prepare_insert(u64 hash, u64 target, u64 probe_lenght)
            {
                if (growth_left_ == 0)
                {
                    // const usize old_capacity = max_count();
                    rehash_and_grow_if_necessary();
                }
                return 0;
            }

            constexpr void rehash_and_grow_if_necessary()
            {
                const usize capacity = max_count();

                // Rehash in place if the current size is <= 25/32 of capacity.
                if (capacity > group::slot_per_group_ && count_ * uint64_t {32} <= capacity * uint64_t {25})
                {
                }
                else
                {
                    resize(next_capacity(capacity));
                }
            }

            [[nodiscard]] constexpr usize next_capacity(usize capacity) const noexcept
            {
                return capacity * 2 + 1;
            }

            /**
             * Compute the maximum number of slots we should put into the table before a resizing rehash.
             * Subtract the returned value with the number of slots `count()` to obtains the number of slots we can currently before a resizing rehash.
             */
            [[nodiscard]] constexpr usize max_growth_left() noexcept
            {
                // Current load factor is 7/8, this means we can resize when 7/8 slots are occupied
                // A special case appear when group are 8 bytes width and `max_count()` is 7 : 7−7/8=7, in this case, we return 6
                //
                // | Capacity (capacity) | capacity - capacity / 8 | result (max_growth_left) |
                // |  7 (special case)   |            -            |             6            |
                // |          8          |       8−8/8             |             7            |
                // |         16          |      16−16/8            |            14            |
                // |         32          |      32−32/8            |            28            |
                // |         64          |      64−64/8            |            56            |
                return group::slot_per_group_ == 8 && max_count() == 7 ? 6 : max_count() - max_count() / 8;
            }

        private:
            /** The max slot count of the hashmap. */
            usize max_slot_count_ {0};

            /** The count of values in the hashmap. */
            usize count_ {0};

            /** Pointer to the control segment. */
            control_type *control_;

            /** Pointer to the slot segment. */
            value_type *slots_;

            /** Number of slot we should put into the table before a resizing rehash. */
            usize growth_left_ {0};

            /** The allocator. */
            allocator_type allocator_;
        };

    } // namespace details::hashmap

    template<
        typename key_t,
        typename value_t,
        typename hasher_t = details::hashmap::default_hasher<key_t>,
        typename key_equal_t = details::hashmap::default_equal<key_t>,
        typename allocator_t = details::hashmap::default_allocator<details::hashmap::slot<key_t, value_t>>>
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