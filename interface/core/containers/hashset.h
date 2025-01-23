#ifndef HD_INC_CORE_HASHSET_H
#define HD_INC_CORE_HASHSET_H
#include "../hash/city_hash.h"
#include "../allocators/heap_allocator.h"
#include "../traits/add_lvalue_reference.h"
#include "../traits/is_same_size.h"
#include "../templates/equal.h"
#include "optional.h"
#include "pair.h"
#include "../hash.h"
#include "../bits.h"
#include "../traits/is_comparable_with_equal.h"
#include "tuple_size.h"
#include "tuple_element.h"

namespace hud
{
    namespace details::hashset
    {
        template<typename key_t>
        struct slot
        {
            using key_type = key_t;

            constexpr explicit slot(const key_type &key) noexcept
                : element_(key)
            {
            }

            template<typename u_key_t>
            requires(hud::is_constructible_v<key_type, u_key_t>)
            constexpr explicit slot(u_key_t &&key) noexcept
                : element_(hud::forward<u_key_t>(key))
            {
            }

            [[nodiscard]] constexpr const key_type &get_key() const noexcept
            {
                return element_;
            }

            template<typename slot_t>
            [[nodiscard]] static constexpr decltype(auto) get_key(slot_t &&s) noexcept
            {
                return hud::forward<slot_t>(s).element_;
            }

            template<usize idx_to_reach>
            [[nodiscard]] friend constexpr decltype(auto) get(slot &s) noexcept
            {
                static_assert(idx_to_reach != 0, "Index out of bound");
                return s.element_;
            }

            template<usize idx_to_reach>
            [[nodiscard]] friend constexpr decltype(auto) get(const slot &s) noexcept
            {
                static_assert(idx_to_reach != 0, "Index out of bound");
                return s.element_;
            }

            template<usize idx_to_reach>
            [[nodiscard]] friend constexpr decltype(auto) get(slot &&s) noexcept
            {
                static_assert(idx_to_reach != 0, "Index out of bound");
                return hud::forward<slot>(s).element_;
            }

            template<usize idx_to_reach>
            [[nodiscard]] friend constexpr decltype(auto) get(const slot &&s) noexcept
            {
                static_assert(idx_to_reach != 0, "Index out of bound");
                return hud::forward<const slot>(s).element_;
            }

            key_type element_;
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

        // Metadata can be seen as a signed 8 bits value.
        // If the bit sign is set (value is negative), this means we don't have a value in the slot
        // Else this means we have a empty, deleted or sentinel
        using control_type = i8;

        // static constexpr control_type full = 0b0hhhhhhh;  // h represents the H2 hash 7 bits.
        static constexpr control_type empty_byte = 0b10000000;    // The slot is empty (0x80)
        static constexpr control_type deleted_byte = 0b11111110;  // The slot is deleted (0xFE)
        static constexpr control_type sentinel_byte = 0b11111111; // The slot is a sentinel, A sentinel is a special caracter that mark the end of the control for iteration (0xFF)

        /** Portable group used to analyze a group. */
        struct portable_group
        {
            static constexpr usize SLOT_PER_GROUP = 8;

            struct mask
            {
                constexpr mask(u64 mask_value) noexcept
                    : mask_value_(mask_value)
                {
                }

                [[nodiscard]] constexpr operator bool() const noexcept
                {
                    return mask_value_ != 0;
                }

                [[nodiscard]] constexpr u32 operator*() const noexcept
                {
                    return first_non_null_index();
                }

                constexpr mask &operator++() noexcept
                {
                    // Erase the last byte set to 0x80 by subtracting 1
                    // This will gives us 0x7F, applying AND 0x80 on it gives 0x00
                    // After that using first_non_null_byte()-> will gives us the next 0x80 set or 0 is non
                    mask_value_ &= 0x8080808080808080UL;
                    mask_value_ &= mask_value_ - 1;
                    return *this;
                }

                [[nodiscard]]
                constexpr mask begin() const noexcept
                {
                    return *this;
                }

                [[nodiscard]]
                constexpr mask end() const noexcept
                {
                    return mask(0);
                };

                /** Retrieves the index of the first non null byte set. 0 otherwise. */
                [[nodiscard]] constexpr u32 first_non_null_index() const noexcept
                {
                    // Get number of trailing zero and divide it by 8 (>>3) to get the insert offset of the byte
                    return hud::bits::trailing_zero(mask_value_) >> 3;
                }

                [[nodiscard]]
                friend constexpr bool operator==(const mask &a, const mask &b)
                {
                    return a.mask_value_ == b.mask_value_;
                }

                [[nodiscard]]
                friend constexpr bool operator!=(const mask &a, const mask &b)
                {
                    return !(a == b);
                }

                [[nodiscard]]
                constexpr operator u64() const noexcept
                {
                    return mask_value_;
                }

            private:
                u64 mask_value_;
            };

            struct empty_mask
                : mask
            {
                using mask::mask;

                [[nodiscard]] constexpr bool has_free_slot() const noexcept
                {
                    return *this;
                }

                [[nodiscard]] constexpr u32 first_free_index() const noexcept
                {
                    return first_non_null_index();
                }
            };

            struct empty_or_deleted_mask
                : mask
            {
                using mask::mask;

                [[nodiscard]] constexpr bool has_free_or_deleted_slot() const noexcept
                {
                    return *this;
                }

                [[nodiscard]] constexpr u32 first_free_or_deleted_index() const noexcept
                {
                    return first_non_null_index();
                }
            };

            struct full_mask
                : mask
            {
                using mask::mask;

                [[nodiscard]] constexpr bool has_full_slot() const noexcept
                {
                    return *this;
                }

                [[nodiscard]] constexpr u32 first_full_index() const noexcept
                {
                    return first_non_null_index();
                }
            };

            /** Load a 8 bytes control into the group. */
            constexpr portable_group(const control_type *control) noexcept
                : value_(hud::memory::unaligned_load64(control))

            {
            }

            /**Retrieve a mask where H2 matching control byte have value 0x80 and non matching have value 0x00. */
            constexpr mask match(u8 h2_hash) const noexcept
            {
                // Mix of  From http://graphics.stanford.edu/~seander/bithacks.html#ZeroInWord
                // And http://graphics.stanford.edu/~seander/bithacks.html#ValueInWord
                constexpr uint64_t lsbs = 0x0101010101010101ULL;
                auto x = value_ ^ (lsbs * h2_hash);
                return mask {(x - lsbs) & ~x & 0x8080808080808080ULL};
            }

            /** Retrieve a mask where empty control bytes have value 0x80 and others have value 0x00. */
            constexpr empty_mask mask_of_empty_slot() const noexcept
            {
                // controls are
                // empty : 0b10000000
                // deleted : 0b11111110
                // sentinel : 0b11111111
                // To filter 10000000_11111110_11111111_01001010 to have 10000000_00000000_00000000_00000000
                // We know that value that end with 0 can be empty, deleted or full but not sentinel
                // We know that value that start with 10 is empty
                // We know that value that start with 11 can be deleted and sentinel
                // We know that value that start with 0 is full
                //
                // We can retrieves the end bit with the following
                // 1 - We shift value by 6 bits to put the MSB 2 bit into MSB 2 bit -> 00111111_10111111_11010010_10000000
                // 2 - Negate the shift value -> 11000000_01000000_00101101_01111111
                // We have a mask where 0b11 is empty or full, 0x10 is deleted or full
                // If we mask this with the value we can filter values that start with 0b10, (empty) AND values that end with 0x00 (empty or full)
                // The result is (empty) AND (empty or full) = (empty)
                // 3 - Mask filtered with real value with AND
                //   10000000_11111110_11111111_01001010
                // & 11000000_01000000_00101101_01111111
                // = 10000000_01000000_00101101_01001010
                // - At this point, we never have 0x80 for values that are empty
                // 4 - Mask with 10000000_10000000_10000000_10000000
                // - At this point we have only empty values
                return (value_ & ~(value_ << 6)) & 0x8080808080808080UL;
            }

            /** Retrieve a mask where empty and deleted control bytes have value 0x80 and others have value 0x00. */
            constexpr empty_or_deleted_mask mask_of_empty_or_deleted_slot() const noexcept
            {
                // controls are
                // empty : 0b10000000
                // deleted : 0b11111110
                // sentinel : 0b11111111
                // To filter 10000000_11111110_11111111_01001010 to have 10000000_00000000_00000000_00000000
                // We know that value that end with 0 can be empty, deleted or full but not sentinel
                // We know that value that start with 1 can be empty, deleted and sentinel
                // We know that value that start with 0 is be full
                //
                // We can retrieves the end bit with the following
                // 1 - We shift value by 7 bits to put the MSB bit into MSB bit -> 01111111_01111111_10100101_00000000
                // 2 - Negate the shift value -> 10000000_10000000_01011010_11111111
                //  We have a mask where 0b1 is empty, deleted or full
                // If we mask this with the value we can filter values that start with 1 (empty, deleted or sentinel) AND values that end with 1 (empty, deleted or full)
                // The result is (empty, deleted or sentinel) AND (empty, deleted or full) = (empty, deleted)
                // 3 - Mask filtered with real value with AND
                //   10000000_11111110_11111111_01001010
                // & 10000000_10000000_01011010_11111111
                // = 10000000_10000000_01011010_01001010
                // - At this point, we never have 0x80 for values that are not empty or deleted
                // 4 - Mask with 10000000_10000000_10000000_10000000 to clean bits
                return (value_ & ~(value_ << 7)) & 0x8080808080808080UL;
            }

            /** Retrieve a mask where full control bytes have value 0x80 and others have value 0x00. */
            constexpr full_mask mask_of_full_slot() const noexcept
            {
                // controls are
                // empty : 0b10000000
                // deleted : 0b11111110
                // sentinel : 0b11111111
                // To filter 10000000_11111110_11111111_01001010 to have 0000000_00000000_00000000_10000000
                // We know that value that end with 0 can be empty, deleted or full but not sentinel
                // We know that value that start with 1 can be empty, deleted and sentinel
                // We know that value that start with 0 is be full
                //
                // We filter the value to set 1 if the MSB of the bit is 0
                // 1 - We OR the value with 10000000_10000000_10000000_10000000
                //   10000000_10000000_10000000_10000000
                // ^ 10000000_11111110_11111111_01001010
                // = 00000000_01111110_01111111_10000000
                // 2 - Mask with 10000000_10000000_10000000_10000000 to clean bits
                return (value_ ^ 0x8080808080808080UL) & 0x8080808080808080UL;
            }

            constexpr u32 count_leading_empty_or_deleted() const noexcept
            {
                // ctrl | ~(ctrl >> 7) will have the lowest bit set to zero for kEmpty and
                // kDeleted. We lower all other bits and count number of trailing zeros.
                constexpr uint64_t bits = 0x0101010101010101ULL;
                return static_cast<u32>(hud::bits::trailing_zero((value_ | ~(value_ >> 7)) & bits) >> 3);
                // return static_cast<u32>(countr_zero((ctrl | ~(ctrl >> 7)) & bits) >> 3);
                // return 0;
            }

        private:
            /** The 8 bytes value of the group. */
            u64 value_;
        };

        /** Group type used to iterate over the control. */
        using group_type = portable_group;

        alignas(16) constexpr const control_type INIT_GROUP[32] {
            control_type {0}, // 0
            control_type {0},
            control_type {0},
            control_type {0},
            control_type {0}, // 4
            control_type {0},
            control_type {0},
            control_type {0},
            control_type {0}, // 8
            control_type {0},
            control_type {0},
            control_type {0},
            control_type {0}, // 12
            control_type {0},
            control_type {0},
            control_type {0},
            sentinel_byte, // 16 <- New empty hashmap point here
            empty_byte,
            empty_byte,
            empty_byte,
            empty_byte, // 20
            empty_byte,
            empty_byte,
            empty_byte,
            empty_byte, // 24
            empty_byte,
            empty_byte,
            empty_byte,
            empty_byte, // 28
            empty_byte,
            empty_byte,
            empty_byte, // 31
        };

        struct control
        {
            static constexpr usize COUNT_CLONED_BYTE = group_type::SLOT_PER_GROUP - 1;

            /** Save the H2 */
            static constexpr void set_h2(control_type *control_ptr, usize slot_index, control_type h2, usize max_slot_count) noexcept
            {
                // Save the h2 in the slot and also in the cloned byte
                control_ptr[slot_index] = h2;
                control_ptr[((slot_index - COUNT_CLONED_BYTE) & max_slot_count) + (COUNT_CLONED_BYTE & max_slot_count)] = h2;
            }

            /** Skip all empty or deleted control. */
            static constexpr control_type *skip_empty_or_deleted(control_type *control_ptr) noexcept
            {
                // skip all empty slot after the current one
                while (control::is_byte_empty_or_deleted(*control_ptr))
                {
                    control_ptr += group_type {control_ptr}.count_leading_empty_or_deleted();
                }
                return control_ptr;
            }

            /** Check if the control byte is empty. */
            [[nodiscard]]
            static constexpr bool is_byte_empty(control_type byte_value) noexcept
            {
                return byte_value == empty_byte;
            }

            /** Check if the control byte is deleted. */
            [[nodiscard]]
            static constexpr bool is_byte_deleted(control_type byte_value) noexcept
            {
                return byte_value == deleted_byte;
            }

            /** Check if the control byte is empty or deleted. */
            [[nodiscard]]
            static constexpr bool is_byte_empty_or_deleted(control_type byte_value) noexcept
            {
                return byte_value < sentinel_byte;
            }

            /** Check if the control byte is full. */
            [[nodiscard]]
            static constexpr bool is_byte_full(control_type byte_value) noexcept
            {
                return byte_value >= 0;
            }

            /** Check if the control byte is deleted. */
            [[nodiscard]]
            static constexpr bool is_byte_sentinel(control_type byte_value) noexcept
            {
                return byte_value == sentinel_byte;
            }
        };

        /** The hashmap iterator_impl that iterate over elements. */
        template<typename slot_t, bool is_const>
        class iterator
        {
        public:
            using slot_type = hud::conditional_t<is_const, const slot_t, slot_t>;
            // using element_type = hud::conditional_t<is_const, const typename slot_type::element_type, typename slot_type::element_type>;
            using key_type = hud::conditional_t<is_const, const typename slot_t::key_type, typename slot_t::key_type>;
            using value_type = hud::conditional_t<is_const, const typename slot_t::value_type, typename slot_t::value_type>;
            using pointer_type = hud::add_pointer_t<slot_type>;
            using reference_type = hud::add_lvalue_reference_t<slot_type>;

        public:
            constexpr iterator(control_type *control_ptr) noexcept
                : control_ptr_(control_ptr)
            {
                hud::check(control_ptr_ != nullptr);
                HD_ASSUME((control_ptr_ != nullptr));
            }

            constexpr iterator(control_type *control_ptr, slot_type *slot_ptr) noexcept
                : control_ptr_(control_ptr)
                , slot_ptr_(slot_ptr)
            {
                hud::check(control_ptr_ != nullptr);
                HD_ASSUME(control_ptr_ != nullptr);
                hud::check(slot_ptr_ != nullptr);
                HD_ASSUME(slot_ptr_ != nullptr);
            }

            /**
             * Create a constant iterator from a non constant iterator.
             */
            template<typename u_slot_t>
            constexpr iterator(iterator<u_slot_t, false> &&it) noexcept
                : iterator(hud::forward<iterator<u_slot_t, false>>(it).control_ptr_, hud::forward<iterator<u_slot_t, false>>(it).slot_ptr_)
            {
            }

            constexpr reference_type operator*() const noexcept
            {
                // Ensure we are in a full control
                hud::check(control::is_byte_full(*control_ptr_));
                return *slot_ptr_;
            }

            constexpr pointer_type operator->() const noexcept
            {
                // Ensure we are in a full control
                hud::check(control::is_byte_full(*control_ptr_));
                return slot_ptr_;
            }

            constexpr iterator &operator++() noexcept
            {
                // Ensure we are in a full control
                hud::check(control::is_byte_full(*control_ptr_));
                // Skip all emptry or deleted
                control_type *full_or_sentinel = control::skip_empty_or_deleted(control_ptr_ + 1);
                slot_ptr_ += full_or_sentinel - control_ptr_;
                control_ptr_ = full_or_sentinel;
                return *this;
            }

            constexpr iterator operator++(int) noexcept
            {
                auto tmp = *this;
                ++*this;
                return tmp;
            }

            /** Checks if 2 iterator are equal. */
            [[nodiscard]] constexpr bool operator==(const iterator &other) const noexcept
            {
                return control_ptr_ == other.control_ptr_;
            }

            /** Checks if 2 iterator are not equals. */
            [[nodiscard]] constexpr bool operator!=(const iterator &other) const noexcept
            {
                return control_ptr_ != other.control_ptr_;
            }

            template<typename iterator_t>
            [[nodiscard]] static constexpr decltype(auto) get_key(iterator_t &&s) noexcept
            {
                return slot_t::get_key(*(hud::forward<iterator_t>(s).slot_ptr_));
            }

        private:
            template<usize idx_to_reach>
            [[nodiscard]] friend constexpr decltype(auto) get(iterator &s) noexcept
            {
                return get<idx_to_reach>(*(s.slot_ptr_));
            }

            template<usize idx_to_reach>
            [[nodiscard]] friend constexpr decltype(auto) get(const iterator &s) noexcept
            {
                return get<idx_to_reach>(*(s.slot_ptr_));
            }

            template<usize idx_to_reach>
            [[nodiscard]] friend constexpr auto get(iterator &&s) noexcept
            {
                return get<idx_to_reach>(*(hud::forward<iterator>(s).slot_ptr_));
            }

            template<usize idx_to_reach>
            [[nodiscard]] friend constexpr auto get(const iterator &&s) noexcept
            {
                return get<idx_to_reach>(*(hud::forward<const iterator>(s).slot_ptr_));
            }

        private:
            template<typename u_slot_t, bool u_is_const>
            friend class iterator;

            // The control to iterate over
            control_type *control_ptr_;
            // The current slot we are iterating. Keep uninitialized.
            slot_type *slot_ptr_;
        };

        template<
            typename slot_t,
            typename hasher_t,
            typename key_equal_t,
            typename allocator_t>
        class hashset_impl
        {
        protected:
            /** Type of the slot. */
            using slot_type = slot_t;
            /** Type of the key. */
            using key_type = typename slot_type::key_type;
            /** Type of the hash function. */
            using hasher_type = hasher_t;
            /** Type of the iterator. */
            using iterator = hud::details::hashset::iterator<slot_t, false>;
            /** Type of the const iterator. */
            using const_iterator = hud::details::hashset::iterator<slot_t, true>;
            /**  Type of the allocator. */
            using allocator_type = allocator_t;
            /** The type of allocation done by the allocator. */
            using memory_allocation_type = typename allocator_type::template memory_allocation_type<slot_type>;

            // static_assert(alignof(slot_type) == alignof(element_type), "slot and element_type must have same alignement");
            // static_assert(hud::is_same_size_v<slot_type, element_type>, "slot and element_type must have same size");

        public:
            /**  Default constructor. */
            explicit constexpr hashset_impl() noexcept = default;

            constexpr explicit hashset_impl(const allocator_type &allocator) noexcept
                : allocator_(allocator)
            {
            }

            constexpr ~hashset_impl() noexcept
            {
                clear_shrink();
            }

            /** Reserve memory for at least `count` element and regenerates the hash table. */
            constexpr void reserve(usize count) noexcept
            {
                if (count > max_slot_count_)
                {
                    grow_capacity(hud::math::next_power_of_two(count + 1) - 1);
                }
            }

            /**
             * Remove all elements.
             * Calls the destructor of each element if they are not trivially destructible,
             * but does not release the allocated memory.
             */
            constexpr void clear() noexcept
            {
                if (!hud::is_trivially_destructible_v<slot_type>)
                {
                    if (max_slot_count_ > 0)
                    {
                        size_t remaining_slots = count();
                        control_type *ctrl = control_ptr_;
                        slot_type *slot = slot_ptr_;
                        while (remaining_slots != 0)
                        {
                            group_type group {ctrl};
                            for (u32 full_index : group.mask_of_full_slot())
                            {
                                hud::memory::destroy(slot_ptr_ + full_index);
                                --remaining_slots;
                            }
                            ctrl += group_type::SLOT_PER_GROUP;
                            slot += group_type::SLOT_PER_GROUP;
                        }
                    }
                }
            }

            /**
             * Remove all elements.
             * Calls the destructor of each element if they are not trivially destructible,
             * and then releases the allocated memory.
             */
            constexpr void clear_shrink() noexcept
            {
                if (max_slot_count_ > 0)
                {
                    clear();
                    free_control_and_slot(control_ptr_, slot_ptr_, max_slot_count_);
                }
            }

            /**
             * Insert a key in the hashset.
             * @param key The key associated with the `value`
             * @param args List of arguments pass to `value_type` constructor after the `key` itself
             * @return Iterator to the `value`
             */
            template<typename... args_t>
            requires(hud::is_constructible_v<slot_type, key_type, args_t...>)
            constexpr iterator add(key_type &&key, args_t &&...args) noexcept
            {
                hud::pair<usize, bool> res = find_or_insert_no_construct(key);
                slot_type *slot_ptr = slot_ptr_ + res.first;
                if (res.second)
                {
                    hud::memory::construct_at(slot_ptr, hud::move(key), hud::forward<args_t>(args)...);
                }
                return {control_ptr_ + res.first, slot_ptr};
            }

            /**
             * Insert a key in the hashset.
             * @param key The key associated with the `value`
             * @param args List of arguments pass to `value_type` constructor after the `key` itself
             * @return Iterator to the `value`
             */
            template<typename... args_t>
            requires(hud::is_constructible_v<slot_type, const key_type &, args_t...>)
            constexpr iterator add(const key_type &key, args_t &&...args) noexcept
            {
                hud::pair<usize, bool> res = find_or_insert_no_construct(key);
                slot_type *slot_ptr = slot_ptr_ + res.first;
                if (res.second)
                {
                    hud::memory::construct_at(slot_ptr, key, hud::forward<args_t>(args)...);
                }
                return {control_ptr_ + res.first, slot_ptr};
            }

            /** Find a key and return an iterator to the value. */
            [[nodiscard]]
            constexpr iterator find(const key_type &key) noexcept
            {
                u64 hash = hasher_type {}(key);
                u64 h1 = H1(hash);
                hud::check(hud::bits::is_valid_power_of_two_mask(max_slot_count_) && "Not a mask");
                usize slot_index = h1 & max_slot_count_;
                while (true)
                {

                    group_type group {control_ptr_ + slot_index};
                    group_type::mask group_mask_that_match_h2 = group.match(H2(hash));
                    for (u32 group_index_that_match_h2 : group_mask_that_match_h2)
                    {
                        usize slot_index_that_match_h2 = slot_index + group_index_that_match_h2 & max_slot_count_;
                        slot_type *slot_that_match_h2 = slot_ptr_ + slot_index_that_match_h2;
                        if (key_equal_t {}(slot_t::get_key(*slot_that_match_h2), key)) [[likely]]
                        {
                            return {control_ptr_ + slot_index_that_match_h2, slot_that_match_h2};
                        }
                    }

                    // If we have free slot, we don't find it
                    if (group.mask_of_empty_slot().has_free_slot())
                    {
                        return iterator(control_ptr_sentinel());
                    }

                    // Advance to next group (Maybe a control iterator taht iterate over groups can be better alternative)
                    slot_index += group_type::SLOT_PER_GROUP;
                    slot_index &= max_slot_count_;
                }
            }

            [[nodiscard]]
            constexpr const_iterator find(const key_type &key) const noexcept
            {
                return const_cast<hashset_impl *>(this)->find(key);
            }

            /** Retrieves the allocator. */
            [[nodiscard]] HD_FORCEINLINE constexpr const allocator_type &allocator() const noexcept
            {
                return allocator_;
            }

            /** Return the slack in number of elements. */
            [[nodiscard]] HD_FORCEINLINE constexpr usize slack() const noexcept
            {
                return free_slot_before_grow_;
            }

            /** Checks whether the array is empty of not. */
            [[nodiscard]] HD_FORCEINLINE constexpr bool is_empty() const noexcept
            {
                return count_ != 0;
            }

            /** Retreives number of elements in the array. */
            [[nodiscard]] HD_FORCEINLINE constexpr usize count() const noexcept
            {
                return count_;
            }

            /** Retreives maximum number of elements the array can contains. */
            [[nodiscard]] HD_FORCEINLINE constexpr usize max_count() const noexcept
            {
                return max_slot_count_;
            }

            /** Retrieves an iterator to the end of the array. */
            [[nodiscard]] constexpr iterator begin() noexcept
            {
                auto [control_ptr, slot_ptr] = find_first_full();
                return iterator {control_ptr, slot_ptr};
            }

            /** Retrieves an iterator to the end of the array. */
            [[nodiscard]] constexpr const_iterator begin() const noexcept
            {
                auto [control_ptr, slot_ptr] = find_first_full();
                return const_iterator {control_ptr, slot_ptr};
            }

            /** Retrieves an iterator to the end of the array. */
            [[nodiscard]] constexpr iterator end() noexcept
            {
                return iterator(control_ptr_sentinel());
            }

            /** Retrieves an iterator to the end of the array. */
            [[nodiscard]] constexpr const_iterator end() const noexcept
            {
                return const_iterator(control_ptr_sentinel());
            }

        private:
            /**
             * Find the key and add the H2 hash in the control
             * If the key is found, return the iterator
             * If not found insert the key but do not construct the value.
             */
            [[nodiscard]] constexpr hud::pair<usize, bool> find_or_insert_no_construct(const key_type &key) noexcept
            {
                static_assert(hud::is_hashable_64_v<key_type>, "key_type is not hashable");
                static_assert(hud::is_comparable_with_equal_v<key_type, key_type>, "key_type is not comparable with equal");

                u64 hash = hasher_type {}(key);
                u64 h1 = H1(hash);
                hud::check(hud::bits::is_valid_power_of_two_mask(max_slot_count_) && "Not a mask");
                usize slot_index = h1 & max_slot_count_;
                while (true)
                {
                    // Find the key if present, return iterator to it if not prepare for insertion
                    group_type group {control_ptr_ + slot_index};
                    group_type::mask group_mask_that_match_h2 = group.match(H2(hash));
                    for (u32 group_index_that_match_h2 : group_mask_that_match_h2)
                    {
                        usize slot_index_that_match_h2 = slot_index + group_index_that_match_h2 & max_slot_count_;
                        slot_type *slot_that_match_h2 = slot_ptr_ + slot_index_that_match_h2;
                        if (key_equal_t {}(slot_t::get_key(*slot_that_match_h2), key)) [[likely]]
                        {
                            return {slot_index_that_match_h2, false};
                        }
                    }

                    // Don't find the key
                    group_type::empty_mask empty_mask_of_group = group.mask_of_empty_slot();
                    if (empty_mask_of_group.has_free_slot()) [[likely]]
                    {
                        usize inserted_slot_index = insert_no_construct(h1, H2(hash));
                        return {inserted_slot_index, true};
                    }

                    // Advance to next group (Maybe a control iterator taht iterate over groups can be better alternative)
                    slot_index += group_type::SLOT_PER_GROUP;
                    slot_index &= max_slot_count_;
                }
            }

            /** Insert a slot index associated with the given h2 hash. */
            [[nodiscard]] constexpr usize insert_no_construct(u64 h1, u64 h2) noexcept
            {
                // If we reach the load factor grow the table and retrieves the new slot, else use the given slot
                if (free_slot_before_grow() == 0)
                {
                    grow_capacity(next_capacity());
                }

                // Find the first empty of deleted slot that can be used for this h1 hash
                usize slot_index = find_first_empty_or_deleted(control_ptr_, max_slot_count_, h1);
                count_++;
                control::set_h2(control_ptr_, slot_index, h2, max_slot_count_);

                free_slot_before_grow_--;

                return slot_index;
            }

            constexpr void grow_capacity(usize new_max_slot_count) noexcept
            {
                hud::check(new_max_slot_count > max_slot_count_ && "Grow need a bigger value");
                hud::check(hud::bits::is_valid_power_of_two_mask(max_slot_count_) && "Not a mask");

                // Create the buffer with control and slots
                // Slots are aligned based on alignof(slot_type)
                // In the case of a constant-evaluated context, slot_ptr_ may be uninitialized when the map is created
                // To satisfy the compiler, initialize it to nullptr in this case
                if (hud::is_constant_evaluated() && control_ptr_ == &INIT_GROUP[16])
                {
                    slot_ptr_ = nullptr;
                }
                control_type *old_control_ptr = control_ptr_;
                slot_type *old_slot_ptr = slot_ptr_;
                usize old_max_slot_count = max_slot_count_;
                max_slot_count_ = new_max_slot_count;

                // Allocate the buffer that will contain controls and aligned slots
                // In a constant-evaluated context, bit_cast cannot be used with pointers
                // To satisfy the compiler, allocate controls and slots in two separate allocations
                usize control_size = allocate_control_and_slot(max_slot_count_);

                // Update number of slot we should put into the table before a resizing rehash
                free_slot_before_grow_ = max_slot_before_grow(max_slot_count_) - count_;

                // Set control to empty ending with sentinel
                hud::memory::set(control_ptr_, control_size, empty_byte);
                control_ptr_[max_slot_count_] = sentinel_byte;

                // If we have elements, insert them to the new buffer
                if (count_ > 0)
                {
                    // Move elements to new buffer if any
                    // Relocate slots to newly allocated buffer
                    for (auto it = iterator {old_control_ptr, old_slot_ptr};
                         it != iterator(old_control_ptr + old_max_slot_count);
                         ++it)
                    {
                        // Compute the hash
                        u64 hash = hasher_type {}(iterator::get_key(it));
                        // Find H1 slot index
                        u64 h1 = H1(hash);
                        usize slot_index = find_first_empty_or_deleted(control_ptr_, old_max_slot_count, h1);
                        // Save h2 in control h1 index
                        control::set_h2(control_ptr_, slot_index, H2(hash), old_max_slot_count);
                        // Move old slot to new slot
                        hud::memory::move_or_copy_construct_then_destroy(slot_ptr_ + slot_index, hud::move(*it));
                    }

                    free_control_and_slot(old_control_ptr, old_slot_ptr, old_max_slot_count);
                }
            }

            [[nodiscard]] constexpr usize free_slot_before_grow() const noexcept
            {
                return free_slot_before_grow_;
            }

            /** Retrieves the next capacity after a grow. */
            [[nodiscard]] constexpr usize next_capacity() const noexcept
            {
                // Value are always power of two mask 0,1,3,7,15,31,63, etc...
                return max_slot_count_ * 2 + 1;
            }

            /** Compute the size of the allocation needed for the given slot count. */
            [[nodiscard]] constexpr usize current_allocation_size() const noexcept
            {
                const usize control_size = max_slot_count_ + 1 + control::COUNT_CLONED_BYTE;
                const uptr aligned_control_size = hud::memory::align_address(control_size, sizeof(slot_type));
                return aligned_control_size + max_slot_count_ * sizeof(slot_type);
            }

            /** Find the first empty or deleted slot for the given h1.
             * WARNING: This function works only if there is free or deleted slot available
             * @param h1 The H1 hash
             * @return The first empty or deleted slot that can be used for h1 hash
             */
            [[nodiscard]] static constexpr usize find_first_empty_or_deleted(control_type *control_ptr, u64 max_slot_count, u64 h1) noexcept
            {
                hud::check(hud::bits::is_valid_power_of_two_mask(max_slot_count) && "Not a mask");
                usize slot_index = h1 & max_slot_count;
                while (true)
                {
                    group_type group {control_ptr + slot_index};
                    group_type::empty_or_deleted_mask group_mask_that_match_h2 = group.mask_of_empty_or_deleted_slot();
                    if (group_mask_that_match_h2.has_free_or_deleted_slot())
                    {
                        u32 free_index = group_mask_that_match_h2.first_free_or_deleted_index();
                        usize slot_index_that_is_free_or_deleted = slot_index + free_index & max_slot_count;
                        return slot_index_that_is_free_or_deleted;
                    }

                    // Advance to next group (Maybe a control iterator taht iterate over groups can be better alternative)
                    slot_index += group_type::SLOT_PER_GROUP;
                    slot_index &= max_slot_count;
                }
            }

            [[nodiscard]]
            constexpr hud::pair<control_type *, slot_type *> find_first_full() const noexcept
            {
                hud::check(hud::bits::is_valid_power_of_two_mask(max_slot_count_) && "Not a mask");
                usize slot_index = 0;
                while (slot_index < max_slot_count_)
                {
                    group_type group {control_ptr_ + slot_index};
                    group_type::full_mask group_mask = group.mask_of_full_slot();
                    if (group_mask.has_full_slot())
                    {
                        u32 first_full_index = group_mask.first_full_index();
                        return {control_ptr_ + first_full_index, slot_ptr_ + first_full_index};
                    }

                    // Advance to next group (Maybe a control iterator that iterate over groups can be better alternative)
                    slot_index += group_type::SLOT_PER_GROUP;
                }
                return {control_ptr_sentinel(), nullptr};
            }

            /**
             * Compute the maximum number of slots we should put into the table before a resizing rehash.
             * Subtract the returned value with the number of slots `count()` to obtains the number of slots we can currently use before a resizing rehash.
             */
            [[nodiscard]] constexpr usize max_slot_before_grow(usize capacity) noexcept
            {
                // Current load factor is 7/8, this means we can resize when 7/8 slots are occupied
                // A special case appear when group are 8 bytes width and `capacity` is 7 : 7−7/8=7, in this case, we return 6
                //
                // | Capacity (capacity) | capacity - capacity / 8 | result (max_growth_left) |
                // |  7 (special case)   |            -            |             6            |
                // |          8          |       8−8/8             |             7            |
                // |         16          |      16−16/8            |            14            |
                // |         32          |      32−32/8            |            28            |
                // |         64          |      64−64/8            |            56            |
                return group_type::SLOT_PER_GROUP == 8 && capacity == 7 ? 6 : capacity - capacity / 8;
            }

            [[nodiscard]]
            constexpr control_type *control_ptr_sentinel() const noexcept
            {
                hud::check(control::is_byte_sentinel(control_ptr_[max_slot_count_]));
                return control_ptr_ + max_slot_count_;
            }

            constexpr usize allocate_control_and_slot(usize max_count) noexcept
            {
                // We cloned size of a group - 1 because we never reach the last cloned bytes
                constexpr const usize num_cloned_bytes = control::COUNT_CLONED_BYTE;
                // Control size is the number of slot + sentinel + number of cloned bytes
                const usize control_size = max_count + 1 + num_cloned_bytes;
                const usize slot_size = max_count * sizeof(slot_type);

                const uptr aligned_control_size = hud::memory::align_address(control_size, sizeof(slot_type));
                const usize aligned_allocation_size = aligned_control_size + slot_size;

                if (hud::is_constant_evaluated())
                {
                    control_ptr_ = allocator_.template allocate<control_type>(aligned_control_size).data();
                    slot_ptr_ = allocator_.template allocate<slot_type>(slot_size).data();
                }
                else
                {
                    control_ptr_ = hud::bit_cast<control_type *>(allocator_.template allocate<slot_type>(aligned_allocation_size).data());
                    slot_ptr_ = hud::bit_cast<slot_type *>(control_ptr_ + aligned_control_size);
                    hud::check(hud::memory::is_pointer_aligned(slot_ptr_, alignof(slot_type)));
                }
                return control_size;
            }

            constexpr void free_control_and_slot(control_type *control_ptr, slot_type *slot_ptr, usize max_slot_count) noexcept
            {
                // In a constant-evaluated context, bit_cast cannot be used with pointers
                // and allocation is done in two separate allocation
                if (hud::is_constant_evaluated())
                {
                    // We cloned size of a group - 1 because we never reach the last cloned bytes
                    constexpr const usize num_cloned_bytes = control::COUNT_CLONED_BYTE;
                    // Control size is the number of slot + sentinel + number of cloned bytes
                    const usize control_size = max_slot_count + 1 + num_cloned_bytes;
                    const usize slot_size = max_slot_count * sizeof(slot_type);
                    const uptr aligned_control_size = hud::memory::align_address(control_size, sizeof(slot_type));
                    // const usize aligned_allocation_size = aligned_control_size + slot_size;
                    allocator_.template free<control_type>({control_ptr, aligned_control_size});
                    allocator_.template free<slot_type>({slot_ptr, slot_size});
                }
                else
                {
                    allocator_.template free<slot_type>({hud::bit_cast<slot_type *>(control_ptr), current_allocation_size()});
                }
            }

        private:
            /** The control of the hashmap. */
            control_type *control_ptr_ {const_cast<control_type *>(&INIT_GROUP[16])};

            /** Pointer to the slot segment. */
            slot_type *slot_ptr_;

            /** Max count of slot in the map. Always a power of two mask value. */
            usize max_slot_count_ {0};

            /** The count of values in the hashmap. */
            usize count_ {0};

            /** Number of slot we should put into the table before a resizing rehash. */
            usize free_slot_before_grow_ {0};

            /** The allocator. */
            allocator_type allocator_;
        };

    } // namespace details::hashset

    template<typename element_t>
    using hashset_default_hasher = details::hashset::default_hasher<element_t>;

    template<typename element_t>
    using hashset_default_key_equal = details::hashset::default_equal<element_t>;

    using hashset_default_allocator = details::hashset::default_allocator;

    template<
        typename element_t,
        typename hasher_t = hashset_default_hasher<element_t>,
        typename key_equal_t = hashset_default_key_equal<element_t>,
        typename allocator_t = hashset_default_allocator>
    class hashset
        : public details::hashset::hashset_impl<details::hashset::slot<element_t>, hasher_t, key_equal_t, allocator_t>
    {
    private:
        using super = details::hashset::hashset_impl<details::hashset::slot<element_t>, hasher_t, key_equal_t, allocator_t>;

    public:
        /** Type of the hash function. */
        using typename super::hasher_type;
        /** Type of the slot. */
        using slot_type = typename super::slot_type;
        /** Type of the key. */
        using key_type = typename slot_type::key_type;
        /** Type of the value. */
        using super::add;
        using super::reserve;
        using super::super;
        using typename super::allocator_type;
        using typename super::const_iterator;
        using typename super::iterator;
        explicit constexpr hashset() noexcept = default;

        constexpr explicit hashset(const allocator_type &allocator) noexcept
            : super(allocator)
        {
        }

        constexpr hashset(std::initializer_list<element_t> list, const allocator_type &allocator = allocator_type()) noexcept
            : super(allocator)
        {
            reserve(list.size());
            for (auto &value : list)
            {
                add(value);
            }
        }
    };

    template<typename value_t>
    struct tuple_size<details::hashset::slot<value_t>>
        : hud::integral_constant<usize, 1>
    {
    };

    template<typename value_t>
    struct tuple_size<const details::hashset::slot<value_t>>
        : hud::integral_constant<usize, 1>
    {
    };

    template<usize idx_to_reach, typename value_t>
    struct tuple_element<idx_to_reach, details::hashset::slot<value_t>>
    {
        static_assert(idx_to_reach < 1, "hashset slot index out of bounds");
        using type = const details::hashset::slot<value_t>::key_type;
    };

    template<usize idx_to_reach, typename value_t>
    struct tuple_element<idx_to_reach, const details::hashset::slot<value_t>>
        : tuple_element<idx_to_reach, details::hashset::slot<value_t>>
    {
    };

    template<typename slot_t, bool is_const>
    struct tuple_size<hud::details::hashset::iterator<slot_t, is_const>>
        : tuple_size<typename hud::details::hashset::iterator<slot_t, is_const>::slot_type>
    {
    };

    /** Specialize tuple_element for iterator that permit structured binding. */
    template<usize idx_to_reach, typename slot_t, bool is_const>
    struct tuple_element<idx_to_reach, hud::details::hashset::iterator<slot_t, is_const>>
        : tuple_element<idx_to_reach, typename hud::details::hashset::iterator<slot_t, is_const>::slot_type>
    {
    };

} // namespace hud

namespace std
{
    template<typename key_t>
    struct tuple_size<hud::details::hashset::slot<key_t>>
        : hud::tuple_size<hud::details::hashset::slot<key_t>>
    {
    };

    template<std::size_t idx_to_reach, typename key_t>
    struct tuple_element<idx_to_reach, hud::details::hashset::slot<key_t>>
        : hud::tuple_element<idx_to_reach, hud::details::hashset::slot<key_t>>
    {
    };

    template<typename slot_t, bool is_const>
    struct tuple_size<hud::details::hashset::iterator<slot_t, is_const>>
        : hud::tuple_size<hud::details::hashset::iterator<slot_t, is_const>>
    {
    };

    template<std::size_t idx_to_reach, typename slot_t, bool is_const>
    struct tuple_element<idx_to_reach, hud::details::hashset::iterator<slot_t, is_const>>
        : hud::tuple_element<idx_to_reach, hud::details::hashset::iterator<slot_t, is_const>>
    {
    };

} // namespace std
#endif // HD_INC_CORE_HASHSET_H