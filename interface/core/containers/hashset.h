#ifndef HD_INC_CORE_HASHSET_H
#define HD_INC_CORE_HASHSET_H
#include "../hash/city_hash.h"
#include "../allocators/heap_allocator.h"
#include "../traits/add_lvalue_reference.h"
#include "../templates/equal.h"
#include "optional.h"
#include "pair.h"
#include "../hash.h"
#include "../bits.h"

namespace hud
{
    namespace details::hashset
    {
        template<typename value_t>
        struct slot
        {
            using key_type = value_t;
            using value_type = value_t;

            [[nodiscard]] const key_type &key() const noexcept
            {
                return value_;
            }

            [[nodiscard]] const value_type &value() const noexcept
            {
                return value_;
            }

            [[nodiscard]] value_type &value() noexcept
            {
                return value_;
            }

            value_t value_;
        };

        // template<typename slot_t, typename... args_t>
        // constexpr slot_t::key_type &&get_key(args_t &&...args) noexcept;

        // template<typename slot_t, typename slot_t::key_type key_t, typename... args_t>
        // constexpr slot_t::key_type &&get_key(slot_t::template key_type &&key, args_t &&...args) noexcept
        // {
        //     return hud::forward<slot_t::key_type>(key);
        // }

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

        template<typename slot_t>
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

        struct metadata
        {

            // Metadata can be seen as a signed 8 bits value.
            // If the bit sign is set (value is negative), this means we don't have a value in the slot
            // Else this means we have a empty, deleted or sentinel
            using byte_type = i8;
            using slot_index = usize;
            static const byte_type INIT_GROUP[32];

            // static constexpr byte_type full = 0b0hhhhhhh;  // h represents the H2 hash 7 bits.
            static constexpr byte_type empty_byte = 0b10000000;    // The slot is empty (0x80)
            static constexpr byte_type deleted_byte = 0b11111110;  // The slot is deleted (0xFE)
            static constexpr byte_type sentinel_byte = 0b11111111; // The slot is a sentinel, A sentinel is a special caracter that mark the end of the control for iteration (0xFF)

            constexpr metadata() = default;

            /** Construct a metadata that start from the given pointer. */
            constexpr metadata(byte_type *metadata_ptr) noexcept
                : metadata_ptr_(metadata_ptr)
            {
            }

            /**
             * Save the H2
             */
            [[nodiscard]] constexpr void set_h2(usize slot_index, byte_type h2, usize max_slot_count) noexcept
            {
                // Save the h2 in the slot and also in the cloned byte
                metadata_ptr_[slot_index] = h2;
                metadata_ptr_[((slot_index - COUNT_CLONED_BYTE) & max_slot_count) + (COUNT_CLONED_BYTE & max_slot_count)] = h2;
            }

            /** Check if the metadata byte is empty. */
            [[nodiscard]]
            static constexpr bool is_byte_empty(byte_type byte_value) noexcept
            {
                return byte_value == empty_byte;
            }

            /** Check if the metadata byte is deleted. */
            [[nodiscard]]
            static constexpr bool is_byte_deleted(byte_type byte_value) noexcept
            {
                return byte_value == deleted_byte;
            }

            /** Check if the metadata byte is empty or deleted. */
            [[nodiscard]]
            static constexpr bool is_byte_empty_or_deleted(byte_type byte_value) noexcept
            {
                return byte_value < sentinel_byte;
            }

            /** Check if the metadata byte is full. */
            [[nodiscard]]
            static constexpr bool is_byte_full(byte_type byte_value) noexcept
            {
                return byte_value >= 0;
            }

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

                    [[nodiscard]] u32 operator*() const noexcept
                    {
                        return first_non_null_index();
                    }

                    mask &operator++() noexcept
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

                /** Load a 8 bytes metadata into the group. */
                constexpr portable_group(const byte_type *metadata) noexcept
                    : value_(hud::memory::unaligned_load64(metadata))
                {
                }

                /**Retrieve a mask where H2 matching metadata byte have value 0x80 and non matching have value 0x00. */
                constexpr mask match(u8 h2_hash) const noexcept
                {
                    return mask {hud::bits::has_value_byte(value_, h2_hash)};
                }

                /** Retrieve a mask where empty metadata bytes have value 0x80 and others have value 0x00. */
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

                /** Retrieve a mask where empty and deleted metadata bytes have value 0x80 and others have value 0x00. */
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

                /** Retrieve a mask where full metadata bytes have value 0x80 and others have value 0x00. */
                constexpr u64 mask_of_full_slot() const noexcept
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

            private:
                /** The 8 bytes value of the group. */
                u64 value_;
            };

            /** Group type used to iterate over the metadata. */
            using group_type = portable_group;
            /**
             * Number of byte that are cloned at the end of the metadata.
             * We cloned size of a group - 1 because we never reach the last cloned bytes.
             * */
            static constexpr usize COUNT_CLONED_BYTE = group_type::SLOT_PER_GROUP - 1;

            [[nodiscard]] group_type group_of_slot_index(u32 index) const noexcept
            {
                return group_type {metadata_ptr_ + index};
            }

            [[nodiscard]] metadata metadata_that_start_at_slot_index(usize index) const noexcept
            {
                return metadata {metadata_ptr_ + index};
            }

            [[nodiscard]] const byte_type *data() const noexcept
            {
                return metadata_ptr_;
            }

            [[nodiscard]] byte_type *data() noexcept
            {
                return metadata_ptr_;
            }

        private:
            byte_type *metadata_ptr_ {const_cast<metadata::byte_type *>(&INIT_GROUP[16])};
        };

        /** The hashmap iterator that iterate over elements. */
        template<typename slot_t>
        class iterator
        {
            using slot_type = slot_t;
            using key_type = typename slot_type::key_type;
            using value_type = typename slot_type::value_type;
            using pointer_type = slot_type *;
            using reference_type = hud::add_lvalue_reference_t<slot_type>;

        public:
            iterator(metadata metadata, slot_type *slot)
                : metadata_(metadata)
                , slot_(slot)
            {
            }

            reference_type operator*() const
            {
                return *slot_;
            }

            pointer_type operator->() const
            {
                return slot_;
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
                return a.control_ == b.control_;
            }

            friend bool operator!=(const iterator &a, const iterator &b)
            {
                return !(a == b);
            }

        private:
            // The metadata to iterate over
            metadata metadata_;
            // The current slot we are iterating. Keep uninitialized.
            slot_type *slot_;
        };

        struct find_result
        {
            usize metadata_index_;
            usize slot_index_;
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
            /** Type of the value. */
            using value_type = typename slot_type::value_type;
            /** Type of the hash function. */
            using hasher_type = hasher_t;
            /** Type of the iterator/ */
            using iterator_type = hud::details::hashset::iterator<slot_type>;
            /**  Type of the allocator. */
            using allocator_type = allocator_t;
            /** The type of allocation done by the allocator. */
            using memory_allocation_type = typename allocator_type::template memory_allocation_type<slot_type>;

        public:
            /**
             * Insert a key/pair values in the hashset.
             * @param key The key associated with the `value`
             * @param value The value associated with the `key`
             * @return Reference to the value
             */
            template<typename... args_t>
            requires(hud::is_constructible_v<value_type, args_t...>)
            constexpr value_type &insert_to_ref(key_type &&key, args_t &&...args) noexcept
            {
                hud::pair<usize, bool> res = find_or_insert_no_construct(key);
                slot_type *slot_ptr = slots_ + res.first;
                if (res.second)
                {
                    hud::memory::template construct_at(slot_ptr, key, hud::forward<args_t>(args)...);
                }
                return slot_ptr->value();
            }

            /**
             * Insert a key/pair values in the hashmap.
             * @param key The key associated with the `value`
             * @param value The value associated with the `key`
             * @return Reference to the value
             */
            constexpr iterator_type insert(key_type &&key, value_type &&value) noexcept
            {
                hud::pair<usize, bool> res = find_or_insert_no_construct(key);
                slot_type *slot_ptr = slots_ + res.first;
                if (res.second)
                {
                    hud::memory::template construct_at(slot_ptr, key, value);
                }
                return iterator_type(metadata_.metadata_start_at_slot_index(res.first), slot_ptr);
            }

            /**
             * Insert a key/pair values in the hashmap.
             * @param key The key associated with the `value`
             * @param value The value associated with the `key`
             * @return Reference to the value
             */
            template<typename... args_t>
            requires(hud::is_constructible_v<slot_type, args_t...>)
            constexpr iterator_type insert(key_type &&key, args_t &&...args) noexcept
            {
                hud::pair<usize, bool> res = find_or_insert_no_construct(key);
                slot_type *slot_ptr = slots_ + res.first;
                if (res.second)
                {
                    hud::memory::template construct_at(slot_ptr, key, hud::forward<args_t>(args)...);
                }
                return iterator_type(metadata_.metadata_start_at_slot_index(res.first), slot_ptr);
            }

            /**
             *
             */
            [[nodiscard]]
            constexpr hud::optional<iterator_type> find(key_type &&key) const noexcept
            {
                u64 hash = hasher_type {}(key);
                u64 h1 = H1(hash);
                hud::check(hud::bits::is_valid_power_of_two_mask(max_slot_count_) && "Not a mask");
                usize slot_index = h1 & max_slot_count_;
                while (true)
                {
                    metadata::group_type group = metadata_.group_of_slot_index(slot_index);
                    metadata::group_type::mask group_mask_that_match_h2 = group.match(H2(hash));
                    for (u32 group_index_that_match_h2 : group_mask_that_match_h2)
                    {
                        usize slot_index_that_match_h2 = slot_index + group_index_that_match_h2 & max_slot_count_;
                        slot_type *slot_that_match_h2 = slots_ + slot_index_that_match_h2;
                        if (key_equal_t {}(slot_that_match_h2->key(), key)) [[likely]]
                        {
                            return iterator_type(metadata_.metadata_start_at_slot_index(slot_index_that_match_h2), slot_that_match_h2);
                        }
                    }

                    // If we have free slot, we don't find it
                    if (group.mask_of_empty_slot().has_free_slot())
                        return hud::nullopt;

                    // Advance to next group (Maybe a metadata iterator taht iterate over groups can be better alternative)
                    slot_index += metadata::group_type::SLOT_PER_GROUP;
                    slot_index &= max_slot_count_;
                }
            }

        private:
            /**
             * Find the key and add the H2 hash in the metadata
             * If the key is found, return the iterator
             * If not found insert the key but do not construct the value.
             * If the key*/
            [[nodiscard]] constexpr hud::pair<usize, bool> find_or_insert_no_construct(const key_type &key) noexcept
            {
                u64 hash = hasher_type {}(key);
                u64 h1 = H1(hash);
                hud::check(hud::bits::is_valid_power_of_two_mask(max_slot_count_) && "Not a mask");
                usize slot_index = h1 & max_slot_count_;
                while (true)
                {
                    // Find the key if present, return iterator to it if not prepare for insertion
                    metadata::group_type group = metadata_.group_of_slot_index(slot_index);
                    metadata::group_type::mask group_mask_that_match_h2 = group.match(H2(hash));
                    for (u32 group_index_that_match_h2 : group_mask_that_match_h2)
                    {
                        usize slot_index_that_match_h2 = slot_index + group_index_that_match_h2 & max_slot_count_;
                        slot_type *slot_that_match_h2 = slots_ + slot_index_that_match_h2;
                        if (key_equal_t {}(slot_that_match_h2->key(), key)) [[likely]]
                        {
                            return {slot_index_that_match_h2, false};
                        }
                    }

                    // Don't find the key
                    metadata::group_type::empty_mask empty_mask_of_group = group.mask_of_empty_slot();
                    if (empty_mask_of_group.has_free_slot()) [[likely]]
                    {
                        usize inserted_slot_index = insert_no_construct(h1, H2(hash));
                        return {inserted_slot_index, true};
                    }

                    // Advance to next group (Maybe a metadata iterator taht iterate over groups can be better alternative)
                    slot_index += metadata::group_type::SLOT_PER_GROUP;
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
                usize slot_index = find_first_empty_or_deleted(h1);
                count_++;
                metadata_.set_h2(slot_index, h2, max_slot_count_);

                free_slot_before_grow_--;

                return slot_index;
            }

            constexpr void grow_capacity(usize slot_count) noexcept
            {
                hud::check(slot_count > max_slot_count_ && "Grow need a bigger value");
                hud::check(hud::bits::is_valid_power_of_two_mask(max_slot_count_) && "Not a mask");

                usize new_max_slot_count_ = slot_count;

                // Create the buffer with metadata and slots
                // Slots are aligned on alignof(slot_type)

                // We cloned size of a group - 1 because we never reach the last cloned bytes
                constexpr const usize num_cloned_bytes = metadata::COUNT_CLONED_BYTE;
                // Control size is the number of slot + sentinel + number of cloned bytes
                const usize control_size = new_max_slot_count_ + 1 + num_cloned_bytes;
                const uptr aligned_control_size = hud::memory::align_address(control_size, sizeof(slot_type));
                const usize aligned_allocation_size = aligned_control_size + new_max_slot_count_ * sizeof(slot_type);

                // Allocate the buffer that will contains control metadata and aligned slots
                memory_allocation_type allocation = allocator_.template allocate<slot_type>(aligned_allocation_size);

                // Save control metadata and slot pointers
                metadata::byte_type *metadata_ptr = hud::bit_cast<metadata::byte_type *>(allocation.data());
                slot_type *slot_ptr = hud::bit_cast<slot_type *>(metadata_ptr + aligned_control_size);
                hud::check(hud::memory::is_pointer_aligned(slot_ptr, alignof(slot_type)));

                // Update number of slot we should put into the table before a resizing rehash
                free_slot_before_grow_ = max_slot_before_grow(new_max_slot_count_) - count_;

                // Reset control metadata
                hud::memory::set(metadata_ptr, control_size, metadata::empty_byte);
                metadata_ptr[new_max_slot_count_] = metadata::sentinel_byte;

                if (count_ > 0)
                {
                    // Move elements to new buffer if any
                    // Relocate slots to newly allocated buffer
                    // Free old buffer

                    allocator_.template free<slot_type>({hud::bit_cast<slot_type *>(metadata_.data()), current_allocation_size()});
                }

                metadata_ = metadata {metadata_ptr};
                slots_ = slot_ptr;
                max_slot_count_ = new_max_slot_count_;
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
                const usize control_size = max_slot_count_ + 1 + metadata::COUNT_CLONED_BYTE;
                const uptr aligned_control_size = hud::memory::align_address(control_size, sizeof(slot_type));
                return aligned_control_size + max_slot_count_ * sizeof(slot_type);
            }

            /** Find the first empty or deleted slot for the given h1.
             * WARNING: This function works only if there is free or deleted slot available
             * @param h1 The H1 hash
             * @return The first empty or deleted slot that can be used for h1 hash
             */
            [[nodiscard]] constexpr usize find_first_empty_or_deleted(u64 h1) const noexcept
            {
                hud::check(hud::bits::is_valid_power_of_two_mask(max_slot_count_) && "Not a mask");
                usize slot_index = h1 & max_slot_count_;
                while (true)
                {
                    metadata::group_type group = metadata_.group_of_slot_index(slot_index);
                    metadata::group_type::empty_or_deleted_mask group_mask_that_match_h2 = group.mask_of_empty_or_deleted_slot();
                    if (group_mask_that_match_h2.has_free_or_deleted_slot())
                    {
                        u32 free_index = group_mask_that_match_h2.first_free_or_deleted_index();
                        usize slot_index_that_is_free_or_deleted = slot_index + free_index & max_slot_count_;
                        return slot_index_that_is_free_or_deleted;
                    }

                    // Advance to next group (Maybe a metadata iterator taht iterate over groups can be better alternative)
                    slot_index += metadata::group_type::SLOT_PER_GROUP;
                    slot_index &= max_slot_count_;
                }
            }

            /**
             * Compute the maximum number of slots we should put into the table before a resizing rehash.
             * Subtract the returned value with the number of slots `count()` to obtains the number of slots we can currently before a resizing rehash.
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
                return metadata::group_type::SLOT_PER_GROUP == 8 && capacity == 7 ? 6 : capacity - capacity / 8;
            }

        private:
            /** The metadata of the hashmap. */
            metadata metadata_;

            /** Pointer to the slot segment. */
            slot_type *slots_;

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

    template<
        typename value_t,
        typename hasher_t = details::hashset::default_hasher<value_t>,
        typename key_equal_t = details::hashset::default_equal<value_t>,
        typename allocator_t = details::hashset::default_allocator<value_t>>
    class hashset
        : details::hashset::hashset_impl<details::hashset::slot<value_t>, hasher_t, key_equal_t, allocator_t>
    {
    private:
        using super = details::hashset::hashset_impl<details::hashset::slot<value_t>, hasher_t, key_equal_t, allocator_t>;

    public:
        /** Type of the hash function. */
        using hasher_type = typename super::hasher_type;
        /** Type of the value. */
        using value_type = typename super::value_type;

        /** Import super functions. */
        using super::insert_to_ref;
        using super::super;
    };
} // namespace hud

#endif // HD_INC_CORE_HASHSET_H