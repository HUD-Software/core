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
#include "../traits/is_comparable_with_equal_operator.h"
#include "../traits/is_trivially_copy_constructible.h"
#include "tuple_size.h"
#include "tuple_element.h"
#include "../traits/is_transparent.h"
#include "../traits/conditional.h"
#include "compressed_tuple.h"

#if defined(HD_SSSE3)
    #include <tmmintrin.h>
#endif

// Difference with STL:
// - Don't use is_transparent because we don't have the nessecity to keep compatibility with existing code.
//   Comparaison and hasher is always transparent if the user provide a custom equal or hasher type.

namespace hud
{
    namespace details::hashset
    {

        /**
         * A slot struct that inherits from a given storage type.
         * This struct provides controlled construction and copy/move semantics.
         * A slot can be moved even if the underlying storage cannot.
         * To enable this, the move constructor and move assignment operator
         * of `storage_t` must be protected.
         *
         * @tparam storage_t The type of storage managed by this slot.
         */
        template<typename storage_t>
        struct slot
            : storage_t
        {
            /** The type of storage. */
            using storage_type = storage_t;

            /**
             * Constructs the slot by perfectly forwarding arguments to the base storage type constructor.
             * Enabled only if storage_type is constructible with the given arguments.
             *
             * @tparam types_t Variadic types forwarded to storage_type.
             * @param values Arguments to forward to storage_type's constructor.
             */
            template<typename... types_t>
            requires(hud::is_constructible_v<storage_type, types_t...>)
            constexpr explicit slot(types_t &&...values) noexcept
                : storage_type(hud::forward<types_t>(values)...)
            {
            }

            /**
             * Copy constructor from the same slot type.
             * Explicit if the copy constructor of storage_type is not implicitly convertible.
             * Enabled only if storage_type is nothrow copy constructible.
             *
             * @param other The slot to copy from.
             */
            constexpr explicit(!hud::is_convertible_v<const storage_type &, storage_type>) slot(const slot &other) noexcept
            requires(hud::is_nothrow_copy_constructible_v<storage_type>)
            = default;

            /**
             * Copy constructor from a slot of a different but compatible storage type.
             * Explicit if conversion from u_storage_t to storage_type is not implicit.
             * Enabled only if storage_type is copy constructible from u_storage_t.
             *
             * @tparam u_storage_t The source storage type to copy from.
             * @param other The slot with a compatible storage to copy from.
             */
            template<typename u_storage_t = storage_type>
            requires(hud::is_copy_constructible_v<storage_type, u_storage_t>)
            constexpr explicit(!hud::is_convertible_v<const storage_type &, u_storage_t>) slot(const slot<u_storage_t> &other) noexcept
                : storage_type(other)
            {
            }

            /**
             * Move constructor from the same slot type.
             * Explicit if the move constructor of storage_type is not implicitly convertible.
             * Enabled only if storage_type is nothrow move constructible.
             *
             * @param other The slot to move from.
             */
            constexpr explicit(!(hud::is_convertible_v<storage_type, storage_type>)) slot(slot &&other) noexcept
            requires(hud::is_nothrow_move_constructible_v<storage_type>)
            = default;

            /**
             * Move constructor from a slot of a different but compatible storage type.
             * Explicit if conversion from u_storage_t to storage_type is not implicit.
             * Enabled only if storage_type is move constructible from u_storage_t.
             *
             * @tparam u_storage_t The source storage type to move from.
             * @param other The slot with a compatible storage to move from.
             */
            template<typename u_storage_t = storage_type>
            // requires(hud::is_move_constructible_v<storage_type, u_storage_t>)
            constexpr explicit(!hud::is_convertible_v<storage_type, u_storage_t>) slot(slot<u_storage_t> &&other) noexcept
                : storage_type(hud::move(other))
            {
            }

            /**
             * Copy assign.
             * Does not accept throwable copy constructible components.
             * @param other Another pair object.
             */
            constexpr slot &operator=(const slot &other) noexcept
            requires(hud::is_nothrow_copy_assignable_v<storage_type>)
            = default;

            /**
             * Move assign.
             * Does not accept throwable move constructible components.
             * @param other Another pair object.
             */
            constexpr slot &operator=(slot &&other) noexcept
            requires(hud::is_nothrow_move_assignable_v<storage_type>)
            = default;
        };

        /**
         * Key storage for the set map, offering controlled access with specific constraints:
         * - Keys are immutable to ensure consistent and reliable hash calculations.
         * - The storage is copyable but not movable to maintain the integrity and consistency of the keys.
         *
         * @tparam key_t The type of the key
         */
        template<typename key_t>
        class hashset_storage
        {
        public:
            /** Type of the key. */
            using key_type = key_t;

        public:
            /** Retrieves non mutable reference to the key. */
            [[nodiscard]] constexpr const key_type &key() noexcept
            {
                return element_;
            }

            /** Retrieves non mutable reference to the key. */
            [[nodiscard]] constexpr const key_type &key() const noexcept
            {
                return element_;
            }

            /**
             * Retrieves a reference to the element at the specified index from a non-const hashset_storage object.
             * @tparam idx_to_reach The index of the element to retrieve.
             * @param s The hashset_storage object.
             * @return A reference to the element at the specified index.
             */
            template<usize idx_to_reach>
            [[nodiscard]] friend constexpr const key_type &get(hashset_storage &s) noexcept
            {
                return s.element_;
            }

            /**
             * Retrieves a const reference to the element at the specified index from a const hashset_storage object.
             * @tparam idx_to_reach The index of the element to retrieve.
             * @param s The const hashset_storage object.
             * @return A const reference to the element at the specified index.
             */
            template<usize idx_to_reach>
            [[nodiscard]] friend constexpr const key_type &get(const hashset_storage &s) noexcept
            {
                return s.element_;
            }

            /**
             * Retrieves a reference to the element at the specified index from a hashset_storage object.
             * @tparam idx_to_reach The index of the element to retrieve.
             * @param s The hashset_storage object.
             * @return A reference to the element at the specified index.
             */
            template<usize idx_to_reach>
            [[nodiscard]] friend constexpr decltype(auto) get(hashset_storage &&s) noexcept
            {
                return hud::forward<key_type &&>(s.element_);
            }

            /**
             * Retrieves a const reference to the element at the specified index from a const hashset_storage object.
             * @tparam idx_to_reach The index of the element to retrieve.
             * @param s The const hashset_storage object.
             * @return A const reference to the element at the specified index.
             */
            template<usize idx_to_reach>
            [[nodiscard]] friend constexpr decltype(auto) get(const hashset_storage &&s) noexcept
            {
                return hud::forward<const key_type &&>(s.element_);
            }

            /**
             * Copy constructor.
             * Does not accept throwable copy constructible components.
             * @param other Another pair object.
             */
            constexpr explicit(!(hud::is_convertible_v<const key_type &, key_type>)) hashset_storage(const hashset_storage &other) noexcept
            requires(hud::is_nothrow_copy_constructible_v<key_type>)
            = default;

            /**
             * Copy constructor for different key and value types.
             * @tparam u_key_t Type of the key in the other hashset_storage.
             * @tparam u_value_t Type of the value in the other hashset_storage.
             * @param other The other hashset_storage object to copy from.
             */
            template<typename u_key_t>
            requires(hud::is_copy_constructible_v<key_type, u_key_t>)
            constexpr explicit(!hud::is_convertible_v<const key_type &, key_type>) hashset_storage(const hashset_storage<u_key_t> &other) noexcept
                : element_(other.element_)
            {
                static_assert(hud::is_nothrow_copy_constructible_v<key_t, u_key_t>, "key_t(const u_key_t&) copy constructor is throwable. hashset_storage is not designed to allow throwable copy constructible components");
            }

            /**
             * Constructor that initializes the hashset_storage with a key and a default value.
             * @tparam u_key_t Type of the key.
             * @param key The key to initialize with.
             */
            template<typename u_key_t>
            requires(hud::is_constructible_v<key_type, const u_key_t &>)
            constexpr explicit hashset_storage(const u_key_t &key) noexcept
                : element_(key)
            {
                static_assert(hud::is_nothrow_constructible_v<key_type, const u_key_t &>);
            }

            /**
             * Constructor that initializes the hashset_storage with a key and a default value.
             * @tparam u_key_t Type of the key.
             * @param key The key to initialize with.
             */
            template<typename u_key_t>
            requires(hud::is_constructible_v<key_type, u_key_t>)
            constexpr explicit hashset_storage(u_key_t &&key) noexcept
                : element_(hud::forward<u_key_t>(key))
            {
                static_assert(hud::is_nothrow_constructible_v<key_type, u_key_t>);
            }

            /**
             * Constructor that initializes the hashset_storage with a key and a value.
             * @tparam u_key_t Type of the key.
             * @tparam u_value_t Type of the value.
             * @param key The key to initialize with.
             * @param value The value to initialize with.
             */
            template<typename... u_type_t>
            requires(hud::is_constructible_v<key_type, u_type_t...>)
            constexpr explicit hashset_storage(hud::tag_piecewise_construct_t, hud::tuple<u_type_t...> &&key_tuple) noexcept
                : hashset_storage(hud::forward<hud::tuple<u_type_t...>>(key_tuple), hud::make_index_sequence_for<u_type_t...> {})
            {
                static_assert(hud::is_nothrow_constructible_v<key_type, u_type_t...>, "key_type(u_type_t&&...) constructor is throwable. key_type is not designed to allow throwable constructible components");
            }

            /**
             * Copy assign.
             * Does not accept throwable copy constructible components.
             * @param other Another hashset_storage object.
             */
            constexpr hashset_storage &operator=(const hashset_storage &other) noexcept
            requires(hud::is_nothrow_copy_assignable_v<key_type>)
            = default;

        protected:
            /**
             * Move constructor.
             * Does not accept throwable copy constructible components.
             * @param other Another hashset_storage object to move from.
             */
            constexpr hashset_storage(hashset_storage &&other) noexcept
            requires(hud::is_nothrow_move_constructible_v<key_type>)
            = default;

            /**
             * Move assign.
             * Does not accept throwable move constructible components.
             * @param other Another pair object.
             */
            constexpr hashset_storage &operator=(hashset_storage &&other) noexcept
            requires(hud::is_nothrow_move_assignable_v<key_type>)
            = default;

            /**
             * Move constructor for different key and value types.
             * @tparam u_key_t Type of the key in the other hashset_storage.
             * @tparam u_value_t Type of the value in the other hashset_storage.
             * @param other The other hashset_storage object to move from.
             */
            template<typename u_key_t>
            requires(hud::is_move_constructible_v<key_type, u_key_t>)
            constexpr explicit(!hud::is_convertible_v<key_type, key_type>) hashset_storage(hashset_storage<u_key_t> &&other) noexcept
                : element_(hud::move(other.element_))
            {
                static_assert(hud::is_nothrow_move_constructible_v<key_t, u_key_t>, "key_t(u_key_t&&) move constructor is throwable. hashset_storage is not designed to allow throwable move constructible components");
            }

            /**
             * Constructs a `key_type` from a tuples by unpacking their elements.
             * @tparam tuple_first Type of the first tuple.
             * @tparam tuple_second Type of the second tuple.
             * @tparam indicies_first Index sequence for elements to extract from the first tuple.
             * @tparam indicies_second Index sequence for elements to extract from the second tuple.
             * @param first_tuple Reference to the first tuple to move from.
             * @param second_tuple Reference to the second tuple to move from.
             * @param ... Index sequences used to unpack the tuple elements.
             */
            template<typename key_tuple, usize... indicies>
            constexpr hashset_storage(key_tuple &&tuple, hud::index_sequence<indicies...>) noexcept
                : element_(hud::piecewise_get<indicies>(hud::forward<key_tuple>(tuple))...)
            {
            }

            /** hashset_storage with other key or value can access private members of hashset_storage. */
            template<typename u_key_t>
            friend class hashset_storage;

            /** Only the slot can move construct storage. */
            template<typename u_storage>
            friend struct slot;

        protected:
            /** The key. */
            key_type element_;
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

        /**
         * A "Group" is a small fixed-size array of control bytes used internally
         * by hashset_impl to efficiently manage slots. Each byte in the group
         * encodes the state of a corresponding slot (empty, deleted, or occupied)
         * and part of the hash of the key stored in that slot.
         *
         * This design enables:
         *   - Fast probing: Checking multiple slot states in a single vectorized
         *     operation.
         *   - Compact metadata: Only one byte per slot is used for control info.
         *
         * Implementation details:
         *   - hashset_impl provides two implementations of Groups:
         *       1. Portable: Works on all platforms without special CPU instructions.
         *       2. SSE2: Uses SIMD instructions (SSE2) for faster parallel comparisons
         *          of control bytes.
         *   - In constexpr contexts, the portable implementation is always used,
         *     because constexpr execution cannot use runtime SIMD instructions.
         *
         * Why both implementations:
         *   - SSE2 gives a performance boost on supported CPUs in non-constexpr code.
         *   - Portable ensures correctness and portability everywhere (constexpr,
         *     non-SSE2 CPUs, cross-platform builds).
         */

#if defined(HD_SSE2)

        /**
         * SSE2-optimized Group for hashset_impl.
         *
         * A Group is a small fixed-size array of control bytes used internally
         * to efficiently manage slots. Each byte encodes the state of a slot
         * (empty, deleted, or occupied) and part of the hash of the stored key.
         *
         * This SSE2 version uses SIMD instructions to check multiple slot states
         * in parallel for faster probing. In constexpr contexts, the portable
         * implementation is always used instead.
         */
        struct sse2_group
        {
            /** Number of slots per Group when using SSE2. */
            static constexpr usize SLOT_PER_GROUP = 16;

            /**
             * A mask representing active slots within the Group.
             * Supports iteration, indexing, and utility functions.
             */
            struct mask
            {
                /** Construct the mask. */
                constexpr mask(u16 mask_value) noexcept
                    : mask_value_ {mask_value}
                {
                }

                /** True if any slot is active (bit set). */
                [[nodiscard]] constexpr operator bool() const noexcept
                {
                    return mask_value_ != 0;
                }

                /** Returns the index of the first active slot. */
                [[nodiscard]] constexpr u32 operator*() const noexcept
                {
                    return first_non_null_index();
                }

                /** Advances the mask by removing the least significant set bit. */
                constexpr mask &operator++() noexcept
                {
                    mask_value_ &= mask_value_ - 1;
                    return *this;
                }

                /** Begin iterator for range-based for loops. */
                [[nodiscard]]
                constexpr mask begin() const noexcept
                {
                    return *this;
                }

                /** End iterator (empty mask) for range-based for loops. */
                [[nodiscard]]
                constexpr mask end() const noexcept
                {
                    return mask(0);
                };

                /** Equality comparison. */
                [[nodiscard]]
                friend constexpr bool operator==(const mask &a, const mask &b) noexcept
                {
                    return a.mask_value_ == b.mask_value_;
                }

                /** Inequality comparison. */
                [[nodiscard]]
                friend constexpr bool operator!=(const mask &a, const mask &b) noexcept
                {
                    return !(a == b);
                }

                /** Index of the first active slot (least significant set bit). */
                [[nodiscard]] constexpr u32 first_non_null_index() const noexcept
                {
                    // Get number of trailing zero to get the insert offset of the byte
                    return hud::bits::trailing_zeros(mask_value_);
                }

                /** Convert mask to underlying 16-bit value. */
                [[nodiscard]]
                constexpr operator u16() const noexcept
                {
                    return mask_value_;
                }

            protected:
                /**
                 * The underlying 16-bit bitmask representing active slots within a Group.
                 * Each bit corresponds to a slot:
                 *   - 1 indicates the slot is active/matches the condition of the mask.
                 *   - 0 indicates the slot is inactive.
                 *
                 * This value is manipulated by mask operators (++, *, bool conversion)
                 * to iterate over active slots efficiently.
                 */
                u16 mask_value_;
            };

            /** Mask specialized for empty slots. */
            struct empty_mask
                : mask
            {
                using mask::mask;

                /** True if there is any empty slot. */
                [[nodiscard]] constexpr bool has_empty_slot() const noexcept
                {
                    return *this;
                }

                /** Index of the first empty slot. */
                [[nodiscard]] constexpr u32 first_empty_index() const noexcept
                {
                    return first_non_null_index();
                }

                /** Count trailing zeros in the mask. */
                [[nodiscard]] constexpr u32 trailing_zeros() const noexcept
                {
                    return hud::bits::trailing_zeros(mask_value_);
                }

                /** Count leading zeros in the mask. */
                [[nodiscard]] constexpr u32 leading_zeros() const noexcept
                {
                    return hud::bits::leading_zeros(mask_value_);
                }
            };

            /** Mask specialized for empty or deleted slots. */
            struct empty_or_deleted_mask
                : mask
            {
                using mask::mask;

                /** True if there is any empty or deleted slot. */
                [[nodiscard]] constexpr bool has_empty_or_deleted_slot() const noexcept
                {
                    return *this;
                }

                /** Index of the first empty or deleted slot. */
                [[nodiscard]] constexpr u32 first_empty_or_deleted_index() const noexcept
                {
                    return first_non_null_index();
                }
            };

            /** Mask specialized for full (occupied) slots. */
            struct full_mask
                : mask
            {
                using mask::mask;

                /** True if there is any occupied slot. */
                [[nodiscard]] constexpr bool has_full_slot() const noexcept
                {
                    return *this;
                }

                /** Index of the first occupied slot. */
                [[nodiscard]] constexpr u32 first_full_index() const noexcept
                {
                    return first_non_null_index();
                }
            };

            /**
             * Load a 16-byte control array into the group.
             * @param control Pointer to the control bytes.
             */
            constexpr sse2_group(const control_type *control) noexcept
                : value_(hud::memory::unaligned_load128(control))
            {
            }

            /**
             * Returns a mask where bits corresponding to slots matching the given h2_hash are set to 1,
             * and all other bits are cleared to 0.
             * Each bit in the returned mask corresponds to a slot in the group.
             *
             * @param h2_hash The hash fragment to match.
             */
            mask match(u8 h2_hash) const noexcept
            {
                __m128i match = _mm_set1_epi8(static_cast<char>(h2_hash));
                return mask(_mm_movemask_epi8(_mm_cmpeq_epi8(match, value_)));
            }

            /**
             * Returns a mask representing empty slots in the group.
             * Each bit in the mask corresponds to a slot:
             *   - 1 if the slot is empty
             *   - 0 if the slot is not empty
             *
             * Converts the 16 control bytes into a compact 16-bit bitmask for iteration.
             */
            empty_mask mask_of_empty_slot() const noexcept
            {
    #if defined(HD_SSSE3)
                return _mm_movemask_epi8(_mm_sign_epi8(value_, value_));
    #else
                __m128i match = _mm_set1_epi8(static_cast<char>(empty_byte));
                return _mm_movemask_epi8(_mm_cmpeq_epi8(match, value_));
    #endif
            };

            /**
             * Returns a mask representing empty or deleted slots in the group.
             * Each bit in the mask corresponds to a slot:
             *   - 1 if the slot is empty or deleted
             *   - 0 if the slot is full
             */
            empty_or_deleted_mask mask_of_empty_or_deleted_slot() const noexcept
            {
                __m128i special = _mm_set1_epi8(static_cast<char>(sentinel_byte));
                return _mm_movemask_epi8(_mm_cmpgt_epi8(special, value_));
            }

            /**
             * Returns a mask representing full (occupied) slots in the group.
             * Each bit in the mask corresponds to a slot:
             *   - 1 if the slot is full
             *   - 0 if the slot is empty or deleted
             */
            full_mask mask_of_full_slot() const noexcept
            {
                return _mm_movemask_epi8(value_) ^ 0xffff;
            }

            /**
             * Counts the number of leading empty or deleted slots in the group.
             * Returns the number of consecutive empty/deleted slots starting from the first slot.
             */
            u32 count_leading_empty_or_deleted() const noexcept
            {
                auto special = _mm_set1_epi8(static_cast<char>(sentinel_byte));
                u32 mask = _mm_movemask_epi8(_mm_cmpgt_epi8(special, value_)) + 1;
                return hud::bits::trailing_zeros(mask);
            }

        private:
            /** The 16-byte value representing the group of slots. */
            __m128i value_;
        };
#endif

        struct portable_group
        {
            /** Number of slots per Group in the portable implementation. */
            static constexpr usize SLOT_PER_GROUP = 8;

            /**
             * A mask representing active slots in the portable group.
             * Each byte in `mask_value_` represents a slot:
             *   - 1 in the bitmask indicates the slot matches the condition (active).
             *   - 0 indicates it does not match.
             * Used for iteration and slot state queries.
             */
            struct mask
            {
                /** Construct the mask. */
                constexpr mask(u64 mask_value) noexcept
                    : mask_value_ {mask_value}
                {
                }

                /** True if at least one slot matches (bit set). */
                [[nodiscard]] constexpr operator bool() const noexcept
                {
                    return mask_value_ != 0;
                }

                /** Returns the index of the first matching slot. */
                [[nodiscard]] constexpr u32 operator*() const noexcept
                {
                    return first_non_null_index();
                }

                /** Advances the mask to the next active slot. */
                constexpr mask &operator++() noexcept
                {
                    // Erase the last byte set to 0x80 by subtracting 1
                    // This will gives us 0x7F, applying AND 0x80 on it gives 0x00
                    // After that using first_non_null_byte()-> will gives us the next 0x80 set or 0 if not
                    mask_value_ &= 0x8080808080808080UL;
                    mask_value_ &= mask_value_ - 1;
                    return *this;
                }

                /** Begin iterator for range-based loops. */
                [[nodiscard]]
                constexpr mask begin() const noexcept
                {
                    return *this;
                }

                /** End iterator (empty mask). */
                [[nodiscard]]
                constexpr mask end() const noexcept
                {
                    return mask(0);
                };

                /** Index of the first active byte (slot) in the mask. 0 otherwise. */
                [[nodiscard]] constexpr u32 first_non_null_index() const noexcept
                {
                    // Get number of trailing zero and divide it by 8 (>>3) to get the insert offset of the byte
                    return hud::bits::trailing_zeros(mask_value_) >> 3;
                }

                /** Equality comparison. */
                [[nodiscard]]
                friend constexpr bool operator==(const mask &a, const mask &b) noexcept
                {
                    return a.mask_value_ == b.mask_value_;
                }

                /** Inequality comparison. */
                [[nodiscard]]
                friend constexpr bool operator!=(const mask &a, const mask &b) noexcept
                {
                    return !(a == b);
                }

                /** Convert mask to underlying 64-bit value. */
                [[nodiscard]]
                constexpr operator u64() const noexcept
                {
                    return mask_value_;
                }

            protected:
                /** Underlying 64-bit bitmask representing active slots. */
                u64 mask_value_;
            };

            struct empty_mask
                : mask
            {
                using mask::mask;

                /** True if any empty slot exists. */
                [[nodiscard]] constexpr bool has_empty_slot() const noexcept
                {
                    return *this;
                }

                /** Index of the first empty slot. */
                [[nodiscard]] constexpr u32 first_empty_index() const noexcept
                {
                    return first_non_null_index();
                }

                /** Trailing zeros in the mask. */
                [[nodiscard]] constexpr u32 trailing_zeros() const noexcept
                {
                    // We divide by 8 to get the slot index instead of bit index
                    return hud::bits::trailing_zeros(mask_value_) >> 3;
                }

                /** Leading zeros in the mask. */
                [[nodiscard]] constexpr u32 leading_zeros() const noexcept
                {
                    // We divide by 8 to get the slot index instead of bit index
                    return hud::bits::leading_zeros(mask_value_) >> 3;
                }
            };

            struct empty_or_deleted_mask
                : mask
            {
                using mask::mask;

                /** True if any empty or deleted slot exists. */
                [[nodiscard]] constexpr bool has_empty_or_deleted_slot() const noexcept
                {
                    return *this;
                }

                /** Index of the first empty or deleted slot. */
                [[nodiscard]] constexpr u32 first_empty_or_deleted_index() const noexcept
                {
                    return first_non_null_index();
                }
            };

            struct full_mask
                : mask
            {
                using mask::mask;

                /** True if any slot is full (occupied). */
                [[nodiscard]] constexpr bool has_full_slot() const noexcept
                {
                    return *this;
                }

                /** Index of the first full slot. */
                [[nodiscard]] constexpr u32 first_full_index() const noexcept
                {
                    return first_non_null_index();
                }
            };

            /** Load 8 bytes of control values into the group. */
            constexpr portable_group(const control_type *control) noexcept
                : value_ {hud::memory::unaligned_load64(control)}

            {
            }

            /**
             * Returns a mask where slots whose control byte matches the given h2_hash
             * have their MSB (0x80) set, and all other slots are cleared (0x00).
             *
             * Each byte in `value_` represents a slot:
             *   - If the byte equals `h2_hash`, the corresponding bit in the mask is set (0x80).
             *   - Otherwise, it is cleared (0x00).
             *
             * This uses a bit-hack to compare all 8 bytes in parallel and generate
             * a compact 64-bit mask for iteration.
             *
             * @param h2_hash The hash fragment to match against each slot's control byte.
             */
            constexpr mask match(u8 h2_hash) const noexcept
            {
                // Bit-hack to detect bytes equal to h2_hash
                // Mix of  From http://graphics.stanford.edu/~seander/bithacks.html#ZeroInWord
                // And http://graphics.stanford.edu/~seander/bithacks.html#ValueInWord
                constexpr u64 lsbs {0x0101010101010101ULL};
                u64 x = value_ ^ (lsbs * h2_hash);
                return mask {(x - lsbs) & ~x & 0x8080808080808080ULL};
            }

            /**
             * Returns a mask where empty slots have their MSB (0x80) set and all other slots cleared (0x00).
             * Uses the known control byte patterns for empty, deleted, sentinel, and full slots
             * to generate the mask.
             */
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

            /**
             * Returns a mask where empty or deleted slots have their MSB (0x80) set and all other slots cleared (0x00).
             * Filters control bytes using MSB patterns to isolate only empty and deleted slots.
             */
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

            /**
             * Returns a mask where full (occupied) slots have their MSB (0x80) set and all other slots cleared (0x00).
             * Filters control bytes using MSB patterns to isolate only full slots.
             */
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

            /**
             * Counts the number of leading empty or deleted slots in the group.
             * Uses the MSB of each byte to determine which slots are empty or deleted.
             * Returns the number of consecutive empty/deleted slots starting from the first slot.
             */
            constexpr u32 count_leading_empty_or_deleted() const noexcept
            {
                // ctrl | ~(ctrl >> 7) will have the lowest bit set to zero for kEmpty and
                // kDeleted. We lower all other bits and count number of trailing zeros.
                constexpr uint64_t bits {0x0101010101010101ULL};
                return static_cast<u32>(hud::bits::trailing_zeros((value_ | ~(value_ >> 7)) & bits) >> 3);
            }

        private:
            /** The 8 bytes value of the group. */
            u64 value_;
        };

#if defined(HD_SSE2)
        /**
         * Type of group used to iterate over the control bytes in a hashset_impl.
         *
         * - On SSE2-enabled platforms, `sse2_group` is used to leverage SIMD instructions
         *   for faster parallel comparisons of control bytes.
         */
        using group_type = sse2_group;
#else
        /**
         * Type of group used to iterate over the control bytes in a hashset_impl.
         *
         * - On platforms without SSE2 (or in constexpr contexts), `portable_group` is used.
         *   It provides a fully portable implementation that works on all CPUs.
         */
        using group_type = portable_group;
#endif

        /**
         * Returns the number of slots per group.
         *
         * - In `consteval` (compile-time) contexts, always returns the value from
         *   `portable_group`, because SIMD instructions cannot be used in constexpr.
         * - In runtime contexts, returns the value from `group_type`, which may be
         *   `sse2_group` on SSE2-enabled platforms for faster iteration, or
         *   `portable_group` otherwise.
         *
         * This allows code to generically query the group size while remaining
         * constexpr-friendly and platform-optimized.
         */
        [[nodiscard]]
        constexpr usize SLOT_PER_GROUP() noexcept
        {
            if consteval
            {
                return portable_group::SLOT_PER_GROUP;
            }
            else
            {
                return group_type::SLOT_PER_GROUP;
            }
        }

        /**
         * Initial control bytes for a newly created group in hashset_impl.
         *
         * - Aligned to 16 bytes for potential SIMD access (SSE2).
         * - Each entry is a `control_type` representing the state of a slot:
         *     - `0` (first 16 entries) : reserved / unused padding
         *     - `sentinel_byte` (index 16) : marks the "end" of the valid slots
         *     - `empty_byte` (indices 17–31) : indicates empty slots ready for insertion
         *
         * This array provides a known initial state for all groups, allowing the
         * hashset to safely iterate and insert elements without undefined behavior.
         *
         * Layout explanation:
         *   - The first 16 zeros are for padding or alignment (optional depending on implementation).
         *   - The sentinel marks the position beyond the last valid slot.
         *   - Remaining bytes are empty slots that can accept new elements.
         *
         * Using `alignas(16)` ensures proper alignment for SSE2 vectorized operations.
         */
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

        /**
         * Utility structure to manipulate control bytes in hashset_impl.
         *
         * Each slot in the hashset is associated with a control byte that encodes
         * its state (empty, deleted, full, sentinel) and part of the hash (H2).
         * This struct provides constexpr-safe operations to read, write, and skip
         * over control bytes efficiently.
         */
        struct control
        {
            /**
             * Set the control byte for a given slot and its cloned byte.
             * Each slot has a cloned byte at a fixed offset for easier SIMD/vectorized
             * access. This function writes the value both to the main slot and the cloned byte.
             * @param control_ptr Pointer to the start of the control array.
             * @param slot_index Index of the slot to set.
             * @param value The control value to write (H2, empty, deleted, etc.).
             * @param max_slot_count Maximum number of slots in the control array (used for wrapping the cloned byte position).
             */
            static constexpr void set(control_type *control_ptr, usize slot_index, control_type value, usize max_slot_count) noexcept
            {
                constexpr usize COUNT_CLONED_BYTE {SLOT_PER_GROUP() - 1};
                // Save the h2 in the slot and also in the cloned byte
                control_ptr[slot_index] = value;
                control_ptr[((slot_index - COUNT_CLONED_BYTE) & max_slot_count) + (COUNT_CLONED_BYTE & max_slot_count)] = value;
            }

            /**
             * Skip all empty or deleted control bytes starting from `control_ptr`.
             * Uses the group abstraction (`portable_group` or `group_type`) to count
             * consecutive empty/deleted slots efficiently.
             * Works in constexpr contexts (`portable_group`) and runtime (`group_type`).
             * @param control_ptr Pointer to the start of the control array.
             * @return Pointer to the first control byte that is full or sentinel.
             */
            static constexpr control_type *skip_empty_or_deleted(control_type *control_ptr) noexcept
            {
                // skip all empty slot after the current one
                while (control::is_byte_empty_or_deleted(*control_ptr))
                {
                    if consteval
                    {
                        control_ptr += portable_group {control_ptr}.count_leading_empty_or_deleted();
                    }
                    else
                    {
                        control_ptr += group_type {control_ptr}.count_leading_empty_or_deleted();
                    }
                }
                return control_ptr;
            }

            /**
             * Check if a control byte represents an empty slot.
             * @param byte_value The control byte to check.
             * @return True if the byte represents an empty slot, false otherwise.
             */
            [[nodiscard]]
            static constexpr bool is_byte_empty(control_type byte_value) noexcept
            {
                return byte_value == empty_byte;
            }

            /**
             * Check if a control byte represents a deleted slot.
             * @param byte_value The control byte to check.
             * @return True if the byte represents a deleted slot, false otherwise.
             */
            [[nodiscard]]
            static constexpr bool is_byte_deleted(control_type byte_value) noexcept
            {
                return byte_value == deleted_byte;
            }

            /**
             * Check if a control byte represents a deleted slot.
             * @param byte_value The control byte to check.
             * @return True if the byte represents a deleted slot, false otherwise.
             */
            [[nodiscard]]
            static constexpr bool is_byte_empty_or_deleted(control_type byte_value) noexcept
            {
                return byte_value < sentinel_byte;
            }

            /**
             * Check if a control byte represents a full (occupied) slot.
             * @param byte_value The control byte to check.
             * @return True if the byte represents a full slot, false otherwise.
             */
            [[nodiscard]]
            static constexpr bool is_byte_full(control_type byte_value) noexcept
            {
                return byte_value >= 0;
            }

            /**
             * Check if a control byte is a sentinel (special marker).
             * @param byte_value The control byte to check.
             * @return True if the byte is a sentinel, false otherwise.
             */
            [[nodiscard]]
            static constexpr bool is_byte_sentinel(control_type byte_value) noexcept
            {
                return byte_value == sentinel_byte;
            }
        };

        /**
         * Iterator over elements in hashset_impl.
         * Iterates only over slots marked as full (occupied). Skips empty and deleted slots.
         * @tparam slot_t Type of slot stored in the hashset.
         * @tparam is_const Whether the iterator is a const iterator.
         */
        template<typename slot_t, bool is_const>
        class iterator
        {
        public:
            using slot_type = hud::conditional_t<is_const, const slot_t, slot_t>;
            using storage_type = hud::conditional_t<is_const, const typename slot_type::storage_type, typename slot_type::storage_type>;
            using pointer_type = hud::add_pointer_t<storage_type>;
            using reference_type = hud::add_lvalue_reference_t<storage_type>;

        public:
            /**
             * Constructs an iterator starting at a control pointer.
             * @param control_ptr Pointer to the control byte to start iterating from.
             */
            constexpr iterator(control_type *control_ptr) noexcept
                : control_ptr_ {control_ptr}
            {
                HUD_CHECK(control_ptr_ != nullptr);
                HD_ASSUME((control_ptr_ != nullptr));
            }

            /**
             * Constructs an iterator starting at a control and slot pointer.
             * @param control_ptr Pointer to the control byte.
             * @param slot_ptr Pointer to the corresponding slot.
             */
            constexpr iterator(control_type *control_ptr, slot_type *slot_ptr) noexcept
                : control_ptr_ {control_ptr}
                , slot_ptr_ {slot_ptr}
            {
                HUD_CHECK(control_ptr_ != nullptr);
                HD_ASSUME(control_ptr_ != nullptr);
            }

            /**
             * Construct a const iterator from a non-const iterator.
             * @tparam u_slot_t Slot type of the non-const iterator.
             * @param it Non-const iterator to convert from.
             */
            template<typename u_slot_t>
            constexpr iterator(iterator<u_slot_t, false> &&it) noexcept
                : iterator {hud::forward<iterator<u_slot_t, false>>(it).control_ptr_, hud::forward<iterator<u_slot_t, false>>(it).slot_ptr_}
            {
            }

            /**
             * Dereference the iterator to access the element.
             * @return Reference to the element at the current slot.
             * @note Only valid if the control byte at the current position is full.
             */
            constexpr reference_type operator*() const noexcept
            {
                // Ensure we are in a full control
                HUD_CHECK(control::is_byte_full(*control_ptr_));
                return *static_cast<storage_type *>(slot_ptr_);
            }

            /**
             * Member access operator for the current element.
             * @return Pointer to the element at the current slot.
             * @note Only valid if the control byte at the current position is full.
             */
            constexpr pointer_type operator->() const noexcept
            {
                // Ensure we are in a full control
                HUD_CHECK(control::is_byte_full(*control_ptr_));
                return slot_ptr_;
            }

            /**
             * Pre-increment operator. Moves to the next full slot.
             * Skips empty and deleted slots automatically.
             * @return Reference to the incremented iterator.
             */
            constexpr iterator &operator++() noexcept
            {
                // Ensure we are in a full control
                HUD_CHECK(control::is_byte_full(*control_ptr_));
                // Skip all emptry or deleted
                control_type *full_or_sentinel {control::skip_empty_or_deleted(control_ptr_ + 1)};
                slot_ptr_ += full_or_sentinel - control_ptr_;
                control_ptr_ = full_or_sentinel;
                return *this;
            }

            /**
             * Post-increment operator. Moves to the next full slot.
             * @return Copy of the iterator before increment.
             */
            constexpr iterator operator++(int) noexcept
            {
                auto tmp {*this};
                ++*this;
                return tmp;
            }

            /**
             * Equality comparison.
             * @param other Another iterator to compare with.
             * @return True if both iterators point to the same control position.
             */
            [[nodiscard]] constexpr bool operator==(const iterator &other) const noexcept
            {
                return control_ptr_ == other.control_ptr_;
            }

            /**
             * Inequality comparison.
             * @param other Another iterator to compare with.
             * @return True if iterators point to different control positions.
             */
            [[nodiscard]] constexpr bool operator!=(const iterator &other) const noexcept
            {
                return control_ptr_ != other.control_ptr_;
            }

        private:
            template<typename u_slot_t, bool u_is_const>
            friend class iterator;
            template<
                typename storage_t,
                typename hasher_t,
                typename key_equal_t,
                typename allocator_t>
            friend class hashset_impl;

            /** Pointer to the current control byte being iterated. */
            control_type *control_ptr_;
            /** Pointer to the current slot corresponding to control_ptr_. */
            slot_type *slot_ptr_;
        };

        /**
         * A hash set implementation.
         * This implementation is inspired by Abseil's flat hash set but additionally
         * supports usage in `constexpr` contexts. It provides insertion, deletion,
         * lookup, and iteration over elements at compile-time or runtime.
         * Uses open addressing with H1/H2 hash scheme and groups for SIMD-friendly probing.
         * @tparam storage_t Type of storage used for the elements.
         * @tparam hasher_t Type of the hash function.
         * @tparam key_equal_t Type of the equality comparator.
         * @tparam allocator_t Type of the allocator.
         */
        template<
            typename storage_t,
            typename hasher_t,
            typename key_equal_t,
            typename allocator_t>
        class hashset_impl
        {
        protected:
            /** Type of the slot. */
            using slot_type = slot<storage_t>;
            /** Type of the slot. */
            using storage_type = typename slot_type::storage_type;
            /** Type of the key. */
            using key_type = typename slot_type::key_type;
            /** Type of the hash function. */
            using hasher_type = hasher_t;
            /** Type of the equal function. */
            using key_equal_type = key_equal_t;
            /** Type of the iterator. */
            using iterator = hud::details::hashset::iterator<slot_type, false>;
            /** Type of the const iterator. */
            using const_iterator = hud::details::hashset::iterator<slot_type, true>;
            /**  Type of the allocator. */
            using allocator_type = allocator_t;
            /** The type of allocation done by the allocator. */
            using memory_allocation_type = typename allocator_type::template memory_allocation_type<slot_type>;

            static_assert(hud::is_hashable_64_v<key_type>, "key_type is not hashable");
            static_assert(hud::is_comparable_with_equal_v<key_type, key_type>, "key_type is not comparable with equal");

            /**
             * HashableAndComparableArgType is used to select the type to be used by a fonction depending of the hashable and comparable possiblity.
             * If the type K est not hashable or comparable, the type is `key_type`, else the type is `K`
             */
            template<typename K>
            static constexpr bool is_hashable_and_comparable_v = hud::conjunction_v<hud::is_hashable_64<key_type, K>, hud::is_comparable_with_equal<key_type, K>>;

            /** Friend with other hashset_impl of other types. */
            template<typename u_storage_t, typename u_hasher_t, typename u_key_equal_t, typename u_allocator_t>
            friend class hashset_impl;

        public:
            /**  Default constructor. */
            explicit constexpr hashset_impl() noexcept = default;

            /**
             * Constructs a hashset_impl with a given allocator.
             * @param allocator Allocator to use.
             */
            constexpr explicit hashset_impl(const allocator_type &allocator) noexcept
                : compressed_(hud::tag_piecewise_construct, hud::forward_as_tuple(allocator), hud::forward_as_tuple(), hud::forward_as_tuple(), hud::forward_as_tuple())
            {
            }

            /**
             * Copy-constructs elements from another hashset_impl into this one.
             * Depending on the type and context, it either performs a bitwise copy of controls and slots or constructs each slot individually.
             * @param other The other array to move
             */
            constexpr explicit hashset_impl(const hashset_impl &other) noexcept
                : hashset_impl(other, other.allocator())
            {
            }

            /**
             * Copy-constructs elements from another hashset_impl into this one.
             * Depending on the type and context, it either performs a bitwise copy of controls and slots or constructs each slot individually.
             * @tparam u_storage_t Storage type of the other hashset_impl.
             * @tparam u_hasher_t Hash function type of the other hashset_impl.
             * @tparam u_key_equal_t Key equality comparator of the other hashset_impl.
             * @tparam u_allocator_t Allocator type of the other hashset_impl.
             * @param other The other array to move
             */
            template<typename u_storage_t, typename u_hasher_t, typename u_key_equal_t, typename u_allocator_t>
            constexpr explicit hashset_impl(const hashset_impl<u_storage_t, u_hasher_t, u_key_equal_t, u_allocator_t> &other) noexcept
                : hashset_impl(other, other.allocator())
            {
            }

            /**
             * Copy-constructs a hashset_impl from another hashset_impl using a specific allocator.
             * Depending on the type and context, it either performs a bitwise copy of controls and slots or constructs each slot individually.
             * @tparam u_storage_t Storage type of the other hashset_impl.
             * @tparam u_hasher_t Hash function type of the other hashset_impl.
             * @tparam u_key_equal_t Key equality comparator of the other hashset_impl.
             * @tparam u_allocator_t Allocator type of the other hashset_impl.
             * @param other The other hashset_impl to copy from.
             * @param allocator The allocator to use for this hashset_impl.
             */
            template<typename u_storage_t, typename u_hasher_t, typename u_key_equal_t, typename u_allocator_t>
            constexpr explicit hashset_impl(const hashset_impl<u_storage_t, u_hasher_t, u_key_equal_t, u_allocator_t> &other, const allocator_type &allocator) noexcept
                : max_slot_count_ {other.max_count()}
                , count_ {other.count()}
                , compressed_ {hud::tag_piecewise_construct, hud::forward_as_tuple(allocator), hud::forward_as_tuple(), hud::forward_as_tuple(), hud::forward_as_tuple(other.free_slot_before_grow_compressed())}
            {
                copy_construct(other);
            }

            /**
             * Copy-constructs elements from another hashset_impl with extra capacity.
             * Depending on the type and context, it either performs a bitwise copy of controls and slots or constructs each slot individually.
             * @tparam u_storage_t Storage type of the other hashset_impl.
             * @tparam u_hasher_t Hash function type of the other hashset_impl.
             * @tparam u_key_equal_t Key equality comparator of the other hashset_impl.
             * @tparam u_allocator_t Allocator type of the other hashset_impl.
             * @param other The other hashset_impl to copy from.
             * @param extra_max_count Optional extra slots to allocate beyond other.max_count().
             */
            template<typename u_storage_t, typename u_hasher_t, typename u_key_equal_t, typename u_allocator_t>
            constexpr explicit hashset_impl(const hashset_impl<u_storage_t, u_hasher_t, u_key_equal_t, u_allocator_t> &other, usize extra_max_count) noexcept
                : hashset_impl {other, extra_max_count, other.allocator()}
            {
            }

            /**
             * Copy-constructs elements from another hashset_impl with extra capacity using a specific allocator.
             * Depending on the type and context, it either performs a bitwise copy of controls and slots or constructs each slot individually.
             * @tparam u_storage_t Storage type of the other hashset_impl.
             * @tparam u_hasher_t Hash function type of the other hashset_impl.
             * @tparam u_key_equal_t Key equality comparator of the other hashset_impl.
             * @tparam u_allocator_t Allocator type of the other hashset_impl.
             * @param other The other hashset_impl to copy from.
             * @param extra_max_count Optional extra slots to allocate beyond other.max_count().
             */
            template<typename u_storage_t, typename u_hasher_t, typename u_key_equal_t, typename u_allocator_t>
            constexpr explicit hashset_impl(const hashset_impl<u_storage_t, u_hasher_t, u_key_equal_t, u_allocator_t> &other, usize extra_max_count, const allocator_type &allocator) noexcept
                : max_slot_count_ {normalize_max_count(other.max_count() + extra_max_count)}
                , count_ {other.count()}
                , compressed_(hud::tag_piecewise_construct, hud::forward_as_tuple(allocator), hud::forward_as_tuple(), hud::forward_as_tuple(), hud::forward_as_tuple(max_slot_before_grow(max_slot_count_) - count_))
            {
                copy_construct(other, extra_max_count);
            }

            /**
             * Move-constructs a hashset_impl from another hashset_impl, taking ownership of its storage.
             * Depending on the type and context, it either takes ownership of the other set's memory
             * or constructs each slot individually using move semantics.
             * @tparam u_storage_t Storage type of the other hashset_impl.
             * @tparam u_hasher_t Hash function type of the other hashset_impl.
             * @tparam u_key_equal_t Key equality comparator of the other hashset_impl.
             * @tparam u_allocator_t Allocator type of the other hashset_impl.
             * @param other The other hashset_impl to move from.
             */
            template<typename u_storage_t, typename u_hasher_t, typename u_key_equal_t, typename u_allocator_t>
            constexpr explicit hashset_impl(hashset_impl<u_storage_t, u_hasher_t, u_key_equal_t, u_allocator_t> &&other) noexcept
                : hashset_impl(hud::move(other), other.allocator())
            {
            }

            /**
             * Move-constructs a hashset_impl from another hashset_impl using a specific allocator, taking ownership of its storage
             * Depending on the type and context, it either takes ownership of the other set's memory
             * or constructs each slot individually using move semantics.
             * @tparam u_storage_t Storage type of the other hashset_impl.
             * @tparam u_hasher_t Hash function type of the other hashset_impl.
             * @tparam u_key_equal_t Key equality comparator of the other hashset_impl.
             * @tparam u_allocator_t Allocator type of the other hashset_impl.
             * @param other The other hashset_impl to move from.
             */
            template<typename u_storage_t, typename u_hasher_t, typename u_key_equal_t, typename u_allocator_t>
            constexpr explicit hashset_impl(hashset_impl<u_storage_t, u_hasher_t, u_key_equal_t, u_allocator_t> &&other, const allocator_type &allocator) noexcept
                : max_slot_count_ {other.max_count()}
                , count_ {other.count()}
                , compressed_(hud::tag_piecewise_construct, hud::forward_as_tuple(allocator), hud::forward_as_tuple(), hud::forward_as_tuple(), hud::forward_as_tuple(other.free_slot_before_grow_compressed()))
            {
                move_construct(hud::forward<hashset_impl<u_storage_t, u_hasher_t, u_key_equal_t, u_allocator_t>>(other));
            }

            /**
             * Move-constructs a hashset_impl from another hashset_impl with extra capacity, taking ownership of its storage
             * Depending on the type and context, it either takes ownership of the other set's memory
             * or constructs each slot individually using move semantics.
             * @tparam u_storage_t Storage type of the other hashset_impl.
             * @tparam u_hasher_t Hash function type of the other hashset_impl.
             * @tparam u_key_equal_t Key equality comparator of the other hashset_impl.
             * @tparam u_allocator_t Allocator type of the other hashset_impl.
             * @param other The other hashset_impl to move from.
             */
            template<typename u_storage_t, typename u_hasher_t, typename u_key_equal_t, typename u_allocator_t>
            constexpr explicit hashset_impl(hashset_impl<u_storage_t, u_hasher_t, u_key_equal_t, u_allocator_t> &&other, usize extra_max_count) noexcept
                : hashset_impl {hud::move(other), extra_max_count, other.allocator()}
            {
            }

            /**
             * Move-constructs a hashset_impl from another hashset_impl with extra capacity using a specific allocator, taking ownership of its storage
             * Depending on the type and context, it either takes ownership of the other set's memory
             * or constructs each slot individually using move semantics.
             * @tparam u_storage_t Storage type of the other hashset_impl.
             * @tparam u_hasher_t Hash function type of the other hashset_impl.
             * @tparam u_key_equal_t Key equality comparator of the other hashset_impl.
             * @tparam u_allocator_t Allocator type of the other hashset_impl.
             * @param other The other hashset_impl to move from.
             */
            template<typename u_storage_t, typename u_hasher_t, typename u_key_equal_t, typename u_allocator_t>
            constexpr explicit hashset_impl(hashset_impl<u_storage_t, u_hasher_t, u_key_equal_t, u_allocator_t> &&other, usize extra_max_count, const allocator_type &allocator) noexcept
                : max_slot_count_ {normalize_max_count(other.max_count() + extra_max_count)}
                , count_ {other.count()}
                , compressed_(hud::tag_piecewise_construct, hud::forward_as_tuple(allocator), hud::forward_as_tuple(), hud::forward_as_tuple(), hud::forward_as_tuple(max_slot_before_grow(max_slot_count_) - count_))
            {
                move_construct(hud::forward<hashset_impl<u_storage_t, u_hasher_t, u_key_equal_t, u_allocator_t>>(other), extra_max_count);
            }

            /**
             * Destroys the hashset_impl and releases all allocated resources.
             * Calls `clear_shrink()` to destroy all elements and free the underlying storage.
             */
            constexpr ~hashset_impl() noexcept
            {
                clear_shrink();
            }

            /**
             * Copy assign another hashset_impl.
             * The copy assignement only grow allocation and never shrink allocation.
             * No new allocation is done if the hashset_impl contains enough memory to copy all elements, in other words we don't copy the capacity of the copied hashset_impl.
             * @param other The other hashset_impl to copy
             * @return *this
             */
            constexpr hashset_impl &operator=(const hashset_impl &other) noexcept
            requires(hud::is_copy_constructible_v<slot_type>)
            {
                if (this != &other)
                {
                    copy_assign(other);
                }
                return *this;
            }

            template<typename u_storage_t, typename u_hasher_t, typename u_key_equal_t, typename u_allocator_t>
            requires(hud::is_copy_constructible_v<slot_type, typename hashset_impl<u_storage_t, u_hasher_t, u_key_equal_t, u_allocator_t>::slot_type>)
            constexpr hashset_impl &operator=(const hashset_impl<u_storage_t, u_hasher_t, u_key_equal_t, u_allocator_t> &other) noexcept
            {
                copy_assign(other);
                return *this;
            }

            /**
             * Move assign another hashset.
             * Never assume that the move assignement will keep the capacity of the moved hashset_impl.
             * Depending of the Type and the allocator the move operation can reallocate or not, this is by design and allow some move optimization
             * @param other The other hashset_impl to move
             */
            constexpr hashset_impl &operator=(hashset_impl &&other) noexcept
            requires(hud::is_move_constructible_v<slot_type>)
            {
                if (this != &other)
                {
                    move_assign(hud::move(other));
                }
                return *this;
            }

            template<typename u_storage_t, typename u_hasher_t, typename u_key_equal_t, typename u_allocator_t>
            requires(hud::is_move_constructible_v<slot_type, typename hashset_impl<u_storage_t, u_hasher_t, u_key_equal_t, u_allocator_t>::slot_type>)
            constexpr hashset_impl &operator=(hashset_impl<u_storage_t, u_hasher_t, u_key_equal_t, u_allocator_t> &&other) noexcept
            {
                move_assign(hud::forward<hashset_impl<u_storage_t, u_hasher_t, u_key_equal_t, u_allocator_t>>(other));
                return *this;
            }

            /** Reserve memory for at least `count` element and regenerates the hash table if needed. */
            constexpr void reserve(usize count) noexcept
            {
                const usize max_count = normalize_max_count(min_capacity_for_count(count));
                if (max_count > max_slot_count_)
                {
                    resize(max_count);
                }
            }

            /**
             * Remove all elements.
             * Calls the destructor of each element if they are not trivially destructible,
             * but does not release the allocated memory.
             */
            constexpr void clear() noexcept
            {
                if (count() > 0)
                {
                    destroy_all_slots();
                    hud::memory::set_memory(control_ptr_, control_size_for_max_count(max_slot_count_), empty_byte);
                    control_ptr_[max_slot_count_] = sentinel_byte;
                    count_ = 0;
                }
            }

            /**
             * Remove all elements.
             * Calls the destructor of each element if they are not trivially destructible,
             * and then releases the allocated memory.
             */
            constexpr void clear_shrink() noexcept
            {
                destroy_all_slots();
                reset_control_and_slot();
            }

            /**
             * Finds an element with the given key in the hashset.
             * Important: constructing a `slot_type` (the actual key object) is not required to perform the search.
             * You can hash and compare the key directly from its type `K`. To do this for a user-defined type:
             *   - Specialize the hashset's `hasher_t` for your type `K` by implementing `operator()(const K&)` to compute a 64-bit hash.
             *     You can also provide overloads for alternative key representations (like an ID or a tuple) to avoid constructing
             *     the full key.
             *   - Specialize the hashset's `key_equal_t` to provide comparisons between the stored key type and the type `K` (or
             *     alternative key representations). Implement `operator()(const key_type&, const K&)` for this purpose.
             *
             * If `hasher_t` and `key_equal_t` are not specialized for `K`, a temporary `slot_type` key will be constructed
             * in order to compute the hash and perform the comparison, which may be expensive..
             *
             * This design allows `find(key)` to search for a key without constructing a full key object, which is especially
             * useful for types that are expensive to constructs.
             *
             * @tparam K Type of the key to search for.
             * @param key The key to search for.
             * @return An iterator pointing to the found element, or the end iterator if not found.
             */
            template<typename K>
            [[nodiscard]]
            constexpr iterator find(K &&key) noexcept
            {
                if consteval
                {
                    return find_impl<portable_group>(forward_key(hud::forward<K>(key)));
                }
                else
                {
                    return find_impl<group_type>(forward_key(hud::forward<K>(key)));
                }
            }

            constexpr void rehash(i32 count) noexcept
            {
                // If we request 0
                // and :
                // - we have no allocation done, just return
                // - we have allocation but no element, free allocation and reset internal state
                if (count == 0)
                {
                    if (max_slot_count_ == 0)
                        return;
                    if (is_empty())
                    {
                        reset_control_and_slot();
                        return;
                    }
                }

                // We request 0 or more and we have allocation and elements
                // bitor is a faster way of doing `max` here. We will round up to the next
                // power-of-2-minus-1, so bitor is good enough.
                usize max_count = normalize_max_count(count | min_capacity_for_count(count_));
                if (count == 0 || max_count > max_slot_count_)
                    resize(max_count);
            }

            constexpr void shrink_to_fit() noexcept
            {
                rehash(0);
            }

            template<typename K>
            [[nodiscard]]
            constexpr const_iterator find(K &&key) const noexcept
            {
                return const_cast<hashset_impl *>(this)->find(hud::forward<K>(key));
            }

            template<typename K>
            [[nodiscard]]
            constexpr bool contains(K &&key) noexcept
            {
                return find(hud::forward<K>(key)) != end();
            }

            template<typename K>
            [[nodiscard]]
            constexpr bool contains(K &&key) const noexcept
            {
                return const_cast<hashset_impl *>(this)->contains(hud::forward<K>(key));
            }

            constexpr void swap(hashset_impl &other) noexcept
            requires(hud::is_swappable_v<slot_type>)
            {
                static_assert(hud::is_nothrow_swappable_v<allocator_type>, "swap(hashmap<type_t>&) is throwable. hashmap is not designed to allow throwable swappable components");

                if constexpr (hud::allocator_traits<allocator_type>::swap_when_container_swap::value)
                {
                    hud::swap(allocator_mut(), other.allocator_mut());
                }

                hud::swap(other.count_, count_);
                hud::swap(other.control_ptr_, control_ptr_);
                hud::swap(other.slot_ptr_, slot_ptr_);
                hud::swap(other.max_slot_count_, max_slot_count_);
                hud::swap(other.free_slot_before_grow_compressed(), free_slot_before_grow_compressed());
            }

            template<typename K>
            constexpr void remove(K &&key) noexcept
            {
                iterator it = find(hud::forward<K>(key));
                if (it != end())
                {
                    remove_iterator(it);
                }
            }

            /** Retrieves the allocator. */
            [[nodiscard]] HD_FORCEINLINE constexpr const allocator_type &allocator() const noexcept
            {
                return hud::get<0>(compressed_);
            }

            /** Return the slack in number of elements. */
            [[nodiscard]] HD_FORCEINLINE constexpr usize slack() const noexcept
            {
                return free_slot_before_grow();
            }

            /** Checks whether the array is empty of not. */
            [[nodiscard]] HD_FORCEINLINE constexpr bool is_empty() const noexcept
            {
                return count_ == 0;
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
            [[nodiscard]] constexpr iterator
            begin() noexcept
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
                return iterator {control_ptr_sentinel()};
            }

            /** Retrieves an iterator to the end of the array. */
            [[nodiscard]] constexpr const_iterator end() const noexcept
            {
                return const_iterator {control_ptr_sentinel()};
            }

        protected:
            /**
             * Finds or inserts a slot corresponding to the given key.
             * If the key is not found, a new slot is created by constructing it with the key followed by `args`.
             * @param key The key used to find or insert the slot.
             * @param args The arguments forwarded to the `slot_type` constructor after the key.
             * @return An iterator to the inserted or existing value.
             */
            template<typename K, typename... args_t>
            constexpr iterator add_impl(K &&key, args_t &&...args) noexcept
            {
                hud::pair<iterator, bool> res {find_or_insert_no_construct(hud::forward<K>(key))};
                if (res.second)
                {
                    hud::memory::construct_object_at(res.first.slot_ptr_, hud::forward<K>(key), hud::forward<args_t>(args)...);
                }
                return res.first;
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
            template<typename key_tuple_t, typename... u_tuple_t>
            constexpr iterator add_impl(hud::tag_piecewise_construct_t, key_tuple_t &&key_tuple, u_tuple_t &&...tuples) noexcept
            {
                hud::pair<iterator, bool> res = find_or_insert_no_construct(hud::forward<key_tuple_t>(key_tuple));
                if (res.second)
                {
                    hud::memory::construct_object_at(res.first.slot_ptr_, hud::tag_piecewise_construct, hud::forward<key_tuple_t>(key_tuple), hud::forward<u_tuple_t>(tuples)...);
                }
                return res.first;
            }

            template<typename K>
            [[nodiscard]] constexpr decltype(auto) forward_key(K &&k) noexcept
            {
                if constexpr (is_hashable_and_comparable_v<K>)
                {
                    return hud::forward<K>(k);
                }
                else
                {
                    return key_type(hud::forward<K>(k));
                }
            }

            template<typename... Args>
            [[nodiscard]] constexpr decltype(auto) forward_key(hud::tuple<Args...> &&tuple) noexcept
            {
                constexpr auto forward_key_tuple_impl = []<usize... indices_key>(
                                                            hud::tuple<Args...> &&key_tuple,
                                                            hud::index_sequence<indices_key...>
                                                        ) -> decltype(auto)
                {
                    if constexpr (is_hashable_and_comparable_v<hud::tuple<Args...>>)
                    {
                        static_assert(hud::is_constructible_v<key_type, decltype(hud::get<indices_key>(key_tuple))...>, "key_type is hashable and comparable with the given tuple but cannot be constructed from its values. ");
                        return hud::forward<hud::tuple<Args...>>(key_tuple);
                    }
                    else
                    {
                        static_assert(hud::is_constructible_v<key_type, decltype(hud::get<indices_key>(key_tuple))...>, "key_type is neither hashable nor comparable with the given tuple, and cannot be constructed from its values. "
                                                                                                                        "Ensure that hud::equal and hud::hash support hud::tuple<...&&>&&, or provide a constructor for key_type that accepts the tuple elements.");
                        return key_type(hud::get<indices_key>(key_tuple)...);
                    }
                };

                return forward_key_tuple_impl(hud::forward<hud::tuple<Args...>>(tuple), hud::make_index_sequence<hud::tuple_size_v<hud::tuple<Args...>>> {});
            }

        private:
            /**
             * Searches for a key in the hashset using a given group type.
             * Implements the probing logic of the hashset:
             *  - Computes the hash of the key and splits it into H1 and H2 components.
             *  - Starts from the slot determined by H1 and iterates over groups of slots.
             *  - Within each group, checks which slots match H2 and then compares keys using `key_equal()`.
             *  - Returns an iterator pointing to the matching slot if found.
             *  - If an empty slot is encountered during probing, the key is not present, and returns the end iterator.
             * @tparam group_t Type representing a group of slots/controls in the hashset.
             * @tparam K Type of the key to search for.
             * @param key The key to search for.
             * @return An iterator pointing to the found slot, or the end iterator if not found.
             */
            template<typename group_t, typename K>
            [[nodiscard]]
            constexpr iterator find_impl(K &&key) noexcept
            {
                u64 hash {hasher()(hud::forward<K>(key))};
                u64 h1(H1(hash));
                HUD_CHECK(hud::bits::is_valid_power_of_two_mask(max_slot_count_) && "Not a mask");
                usize slot_index(h1 & max_slot_count_);

                while (true)
                {
                    const group_t group {control_ptr_ + slot_index};
                    const typename group_t::mask group_mask_that_match_h2 {group.match(H2(hash))};
                    for (u32 group_index_that_match_h2 : group_mask_that_match_h2)
                    {
                        usize slot_index_that_match_h2 {slot_index + group_index_that_match_h2 & max_slot_count_};
                        slot_type *slot_that_match_h2 {slot_ptr_ + slot_index_that_match_h2};
                        if (key_equal()(slot_that_match_h2->key(), hud::forward<K>(key))) [[likely]]
                        {
                            return iterator {control_ptr_ + slot_index_that_match_h2, slot_that_match_h2};
                        }
                    }

                    // If we have free slot, we don't find it
                    if (group.mask_of_empty_slot().has_empty_slot())
                    {
                        return iterator {control_ptr_sentinel()};
                    }

                    // Advance to next group (Maybe a control iterator that iterate over groups can be better alternative)
                    slot_index += group_t::SLOT_PER_GROUP;
                    slot_index &= max_slot_count_;
                }
            }

            template<typename K>
            [[nodiscard]] constexpr u64 compute_hash(K &&key) noexcept
            {
                if constexpr (hud::is_hashable_64_v<key_type, K>)
                {
                    return hasher()(hud::forward<K>(key));
                }
                else
                {
                    return hasher()(key_type(hud::forward<K>(key)));
                }
            }

            constexpr bool should_be_mark_as_empty_if_deleted(usize index) const noexcept
            {
                if consteval
                {
                    return should_be_mark_as_empty_if_deleted_impl<portable_group>(index);
                }
                else
                {
                    return should_be_mark_as_empty_if_deleted_impl<group_type>(index);
                }
            }

            template<typename group_t>
            constexpr bool should_be_mark_as_empty_if_deleted_impl(usize index) const noexcept
            {
                // If map fits entirely into a probing group.
                if (max_slot_count_ <= group_t::SLOT_PER_GROUP)
                {
                    return true;
                }

                // Mask of empty slot of the group after index
                const typename group_t::empty_mask empty_after = group_t {control_ptr_ + index}.mask_of_empty_slot();
                // Mask of empty slot of the group before index
                const usize index_before = (index - group_t::SLOT_PER_GROUP) & max_slot_count_;
                const typename group_t::empty_mask empty_before = group_t {control_ptr_ + index_before}.mask_of_empty_slot();
                // If both groups are not fully empty (i.e., contain at least one empty slot each),
                // and the number of consecutive empty slots before and after the index
                // doesn’t span the whole group (i.e., < SLOT_PER_GROUP),
                // then a probing sequence might have crossed this position → must keep as 'deleted'.
                return empty_before && empty_after && static_cast<usize>(empty_after.trailing_zeros() + empty_before.leading_zeros()) < group_t::SLOT_PER_GROUP;
            }

            constexpr void remove_iterator(iterator it) noexcept
            {
                // Destroy the slot then mark the contral as empty if 'it' in not in probing sequence,
                // else mark the control as deleted to not break the probing sequence
                const usize index = it.control_ptr_ - control_ptr_;
                hud::memory::destroy_object(it.slot_ptr_);
                if (should_be_mark_as_empty_if_deleted(index))
                {
                    control::set(control_ptr_, index, empty_byte, max_slot_count_);
                    free_slot_before_grow_compressed()++;
                }
                else
                {
                    control::set(control_ptr_, index, deleted_byte, max_slot_count_);
                    free_slot_before_grow_compressed() |= ~((~usize {}) >> 1); // Set the sign bit that represent the presence of delete slots
                }
                count_--;
            }

            /**
             * Copy-constructs elements from another hashset_impl into this one.
             * Depending on the type and context, it either performs a bitwise copy of controls and slots or constructs each slot individually.
             * @tparam u_storage_t Storage type of the other hashset_impl.
             * @tparam u_hasher_t Hash function type of the other hashset_impl.
             * @tparam u_key_equal_t Key equality comparator of the other hashset_impl.
             * @tparam u_allocator_t Allocator type of the other hashset_impl.
             * @param other The other hashset_impl to copy from.
             * @param extra_max_count Optional extra slots to allocate beyond `other.max_count()`.
             */
            template<typename u_storage_t, typename u_hasher_t, typename u_key_equal_t, typename u_allocator_t>
            constexpr void copy_construct(const hashset_impl<u_storage_t, u_hasher_t, u_key_equal_t, u_allocator_t> &other, usize extra_max_count = 0) noexcept
            {
                // If `max_slot_count_` is zero, it returns immediately (nothing to copy).
                if (max_slot_count_ == 0)
                    return;

                // Allocate the buffer that will contain controls and aligned slots
                // In a constant-evaluated context, bit_cast cannot be used with pointers
                // To satisfy the compiler, allocate controls and slots in two separate allocations
                usize control_size {allocate_control_and_slot(max_slot_count_)};

                // If `extra_max_count > 0`, `hud::is_constant_evaluated()`, or `slot_type` is not bitwise copy-constructible:
                //        - Initializes controls to empty with sentinel.
                //        - Iterates over `other` slots and constructs each slot individually.
                //        - Computes the hash for each slot and finds the appropriate H1/H2 index.
                //        - Sets control byte and constructs slot in place.
                //  Else (trivial types in runtime context):
                //        - Uses `fast_move_or_copy_construct_object_array_then_destroy` to copy
                //          the control array.
                //        - If there are elements, copies the slot array directly.
                if (extra_max_count > 0 || hud::is_constant_evaluated() || !hud::is_bitwise_copy_constructible_v<slot_type, typename hashset_impl<u_storage_t, u_hasher_t, u_key_equal_t, u_allocator_t>::slot_type>)
                {
                    // Set control to empty ending with sentinel
                    hud::memory::set_memory(control_ptr_, control_size, empty_byte);
                    control_ptr_[max_slot_count_] = sentinel_byte;

                    // Nothing to copy, stop here
                    if (other.count() != 0)
                    {
                        auto insert_slot_by_copy = [this](control_type *control_ptr, auto *slot_ptr)
                        {
                            u64 hash {hasher()(slot_ptr->key())};
                            u64 h1 {H1(hash)};
                            usize slot_index {find_first_empty_or_deleted(control_ptr_, max_slot_count_, h1)};
                            control::set(control_ptr_, slot_index, H2(hash), max_slot_count_);
                            hud::memory::construct_object_at(slot_ptr_ + slot_index, *slot_ptr);
                        };
                        iterate_over_full_slots(other.control_ptr_, other.slot_ptr_, other.count_, other.max_slot_count_, insert_slot_by_copy);
                    }
                }
                else
                {
                    hud::memory::fast_move_or_copy_construct_object_array_then_destroy(control_ptr_, other.control_ptr_, control_size);
                    if (other.count() > 0)
                    {
                        hud::memory::fast_move_or_copy_construct_object_array_then_destroy(slot_ptr_, other.slot_ptr_, other.max_count());
                    }
                }
            }

            /**
             * Move-constructs elements from another hashset_impl into this one.
             * Depending on the type and context, it either takes ownership of the other set's memory
             * or constructs each slot individually using move semantics.
             * @tparam u_storage_t Storage type of the other hashset_impl.
             * @tparam u_hasher_t Hash function type of the other hashset_impl.
             * @tparam u_key_equal_t Key equality comparator of the other hashset_impl.
             * @tparam u_allocator_t Allocator type of the other hashset_impl.
             * @param other The other hashset_impl to move from.
             * @param extra_max_count Optional extra slots to allocate beyond `other.max_count()`.
             */
            template<typename u_storage_t, typename u_hasher_t, typename u_key_equal_t, typename u_allocator_t>
            constexpr void move_construct(hashset_impl<u_storage_t, u_hasher_t, u_key_equal_t, u_allocator_t> &&other, usize extra_max_count = 0) noexcept
            {
                // Nothing to move if max_slot_count_ is zero
                if (max_slot_count_ == 0)
                    return;

                // Allocate the buffer that will contain controls and aligned slots
                // In a constant-evaluated context, bit_cast cannot be used with pointers
                // To satisfy the compiler, allocate controls and slots in two separate allocations
                usize control_size {allocate_control_and_slot(max_slot_count_)};

                // If `extra_max_count > 0`, constant-evaluated context, or slot_type is not bitwise move-constructible:
                //      - Initialize control array to empty + sentinel.
                //      - Iterate over `other` slots, move-construct each slot individually.
                //      - Compute hash for each slot and find H1/H2 index.
                //      - Set control byte and move-construct slot in place.
                // Else (trivial types in runtime context):
                //      - Take ownership of the other set's control and slot arrays.
                if (extra_max_count > 0 || hud::is_constant_evaluated() || !hud::is_bitwise_move_constructible_v<slot_type, typename hashset_impl<u_storage_t, u_hasher_t, u_key_equal_t, u_allocator_t>::slot_type>)
                {
                    // Set control to empty ending with sentinel
                    hud::memory::set_memory(control_ptr_, control_size, empty_byte);
                    control_ptr_[max_slot_count_] = sentinel_byte;

                    // Nothing to copy, stop here
                    if (other.count() != 0)
                    {
                        auto insert_slot_by_copy = [this](control_type *control_ptr, auto *slot_ptr)
                        {
                            u64 hash {hasher()(slot_ptr->key())};
                            u64 h1 {H1(hash)};
                            usize slot_index {find_first_empty_or_deleted(control_ptr_, max_slot_count_, h1)};
                            control::set(control_ptr_, slot_index, H2(hash), max_slot_count_);
                            hud::memory::construct_object_at(slot_ptr_ + slot_index, hud::move(*slot_ptr));
                        };
                        iterate_over_full_slots(other.control_ptr_, other.slot_ptr_, other.count_, other.max_slot_count_, insert_slot_by_copy);
                    }
                    other.reset_control_and_slot();
                }
                else
                {
                    // Take ownership of other set's memory (fast path for trivial types)
                    control_ptr_ = other.control_ptr_;
                    other.control_ptr_ = const_cast<control_type *>(&INIT_GROUP[16]);
                    slot_ptr_ = reinterpret_cast<slot_type *>(other.slot_ptr_);
                    other.slot_ptr_ = nullptr;
                    other.max_slot_count_ = 0;
                    other.count_ = 0;
                    other.free_slot_before_grow_compressed() = 0;
                }
            }

            template<typename u_storage_t, typename u_hasher_t, typename u_key_equal_t, typename u_allocator_t>
            constexpr void copy_assign(const hashset_impl<u_storage_t, u_hasher_t, u_key_equal_t, u_allocator_t> &other) noexcept
            {
                // Destroy all slots
                destroy_all_slots();

                // We don't keep the max count of the copied array
                // The requested memory is the number of element in the copied array, not the max slot count.
                const usize max_count_requested = other.count_;

                // If we have enough allocated memory
                if (max_count_requested <= max_slot_count_)
                {
                    // Reset the control to empty if we have allocated memory ( Do nothing if we have nothing because control_ptr_ points to static INIT_GROUP)
                    if (max_slot_count_ > 0)
                    {
                        hud::memory::set_memory(control_ptr_, control_size_for_max_count(max_slot_count_), empty_byte);
                        control_ptr_[max_slot_count_] = sentinel_byte;
                    }
                    // Copy the allocator if copy_when_container_copy_assigned is true
                    if constexpr (hud::is_not_same_v<u_allocator_t, allocator_type> || hud::allocator_traits<allocator_t>::copy_when_container_copy_assigned::value)
                    {
                        allocator_mut() = other.allocator();
                    }
                    // Copy the number of element
                    count_ = other.count_;
                    // Compute the free slot count before growing
                    free_slot_before_grow_compressed() = max_slot_before_grow(max_slot_count_) - count_;
                }
                else // If we don't have enough memory
                {
                    // Free the control and slot allocation
                    free_control_and_slot(control_ptr_, slot_ptr_, max_slot_count_);

                    // Copy the allocator if copy_when_container_copy_assigned is true
                    if constexpr (hud::is_not_same_v<u_allocator_t, allocator_type> || hud::allocator_traits<allocator_t>::copy_when_container_copy_assigned::value)
                    {
                        allocator_mut() = other.allocator();
                    }
                    // Copy the number of element
                    count_ = other.count_;
                    // Copy the max number of element
                    max_slot_count_ = normalize_max_count(count_);
                    // Compute the free slot count before growing
                    free_slot_before_grow_compressed() = max_slot_before_grow(max_slot_count_) - count_;
                    // Allocate the control and slot
                    usize control_size {allocate_control_and_slot(max_slot_count_)};

                    // Set control to empty ending with sentinel
                    hud::memory::set_memory(control_ptr_, control_size, empty_byte);
                    control_ptr_[max_slot_count_] = sentinel_byte;
                }

                // If we have elements to copy, copy them
                if (count_ > 0)
                {
                    auto insert_slot_by_copy = [this](control_type *control_ptr, auto *slot_ptr)
                    {
                        u64 hash {compute_hash(slot_ptr->key())};
                        // Find H1 slot index
                        u64 h1 {H1(hash)};
                        usize slot_index {find_first_empty_or_deleted(control_ptr_, max_slot_count_, h1)};
                        // Save h2 in control h1 index
                        control::set(control_ptr_, slot_index, H2(hash), max_slot_count_);
                        // Copy slot
                        hud::memory::construct_object_at(slot_ptr_ + slot_index, *slot_ptr);
                    };
                    iterate_over_full_slots(other.control_ptr_, other.slot_ptr_, count_, other.max_slot_count_, insert_slot_by_copy);
                }
            }

            template<typename u_storage_t, typename u_hasher_t, typename u_key_equal_t, typename u_allocator_t>
            constexpr void move_assign(hashset_impl<u_storage_t, u_hasher_t, u_key_equal_t, u_allocator_t> &&other) noexcept
            {
                // Destroy all slots but don't touch the allocated memory now
                // If we can just move control and slot pointers we do it
                // If we can't just move control and slot pointers, we deal with 2 scenarios
                // First, we have enough memory and just add elements by moving them one by one
                // Second, we don't have enough memory and we reallacte memory and add elements by moving them one by one
                destroy_all_slots();

                if (hud::is_constant_evaluated() || !hud::is_bitwise_move_constructible_v<slot_type, typename hashset_impl<u_storage_t, u_hasher_t, u_key_equal_t, u_allocator_t>::slot_type>)
                {
                    // We don't keep the max count of the copied array
                    // The requested memory is the number of element in the copied array, not the max slot count.
                    const usize max_count_requested = other.count_;
                    // If we have enough allocated memory
                    if (max_count_requested <= max_slot_count_)
                    {
                        // Reset the control to empty if we have allocated memory ( Do nothing if we have nothing because control_ptr_ points to static INIT_GROUP)
                        if (max_slot_count_ > 0)
                        {
                            hud::memory::set_memory(control_ptr_, control_size_for_max_count(max_slot_count_), empty_byte);
                            control_ptr_[max_slot_count_] = sentinel_byte;
                        }
                        // Copy the allocator if copy_when_container_copy_assigned is true
                        if constexpr (hud::is_not_same_v<u_allocator_t, allocator_type> || hud::allocator_traits<allocator_t>::copy_when_container_copy_assigned::value)
                        {
                            allocator_mut() = hud::move(other.allocator_mut());
                        }
                        // Copy the number of element
                        count_ = other.count_;
                        // Compute the free slot count before growing
                        free_slot_before_grow_compressed() = max_slot_before_grow(max_slot_count_) - count_;
                    }
                    else // If we don't have enough memory
                    {
                        // Free the control and slot allocation
                        free_control_and_slot(control_ptr_, slot_ptr_, max_slot_count_);

                        // Copy the allocator if copy_when_container_copy_assigned is true
                        if constexpr (hud::is_not_same_v<u_allocator_t, allocator_type> || hud::allocator_traits<allocator_t>::copy_when_container_copy_assigned::value)
                        {
                            allocator_mut() = hud::move(other.allocator_mut());
                        }
                        // Copy the number of element
                        count_ = other.count_;
                        // Copy the max number of element
                        max_slot_count_ = normalize_max_count(count_);
                        // Compute the free slot count before growing
                        free_slot_before_grow_compressed() = max_slot_before_grow(max_slot_count_) - count_;
                        // Allocate the control and slot
                        usize control_size {allocate_control_and_slot(max_slot_count_)};

                        // Set control to empty ending with sentinel
                        hud::memory::set_memory(control_ptr_, control_size, empty_byte);
                        control_ptr_[max_slot_count_] = sentinel_byte;
                    }
                    // If we have elements to move, move them
                    if (count_ > 0)
                    {
                        auto insert_slot_by_copy = [this](control_type *control_ptr, auto *slot_ptr)
                        {
                            u64 hash {compute_hash(slot_ptr->key())};
                            // Find H1 slot index
                            u64 h1 {H1(hash)};
                            usize slot_index {find_first_empty_or_deleted(control_ptr_, max_slot_count_, h1)};
                            // Save h2 in control h1 index
                            control::set(control_ptr_, slot_index, H2(hash), max_slot_count_);
                            // Copy slot
                            hud::memory::construct_object_at(slot_ptr_ + slot_index, hud::move(*slot_ptr));
                        };
                        iterate_over_full_slots(other.control_ptr_, other.slot_ptr_, count_, other.max_slot_count_, insert_slot_by_copy);
                    }

                    other.reset_control_and_slot();
                }
                // If we are not in constant expression and the slot_type are bitwise moveable, move it.
                else
                {
                    // Free the allocated buffers and take ownership of other buffers
                    free_control_and_slot(control_ptr_, slot_ptr_, max_slot_count_);

                    // Move allocator and move members
                    if constexpr (hud::is_not_same_v<u_allocator_t, allocator_type> || hud::allocator_traits<allocator_t>::move_when_container_move_assigned::value)
                    {
                        allocator_mut() = hud::move(other.allocator_mut());
                    }
                    control_ptr_ = other.control_ptr_;
                    other.control_ptr_ = const_cast<control_type *>(&INIT_GROUP[16]);
                    slot_ptr_ = reinterpret_cast<slot_type *>(other.slot_ptr_);
                    other.slot_ptr_ = nullptr;
                    count_ = other.count_;
                    other.count_ = 0;
                    max_slot_count_ = other.max_slot_count_;
                    other.max_slot_count_ = 0;
                    free_slot_before_grow_compressed() = other.free_slot_before_grow_compressed();
                    other.free_slot_before_grow_compressed() = 0;
                }
            }

            /**
             * Find the key and add the H2 hash in the control
             * If the key is found, return the iterator and false
             * If not found insert the key but do not construct the value, retrun the iterator and true
             */
            template<typename K>
            [[nodiscard]]
            constexpr hud::pair<iterator, bool> find_or_insert_no_construct(K &&key) noexcept
            {
                if consteval
                {
                    return find_or_insert_no_construct_impl<portable_group>(forward_key(hud::forward<K>(key)));
                }
                else
                {
                    return find_or_insert_no_construct_impl<group_type>(forward_key(hud::forward<K>(key)));
                }
            }

            /**
             * Find the key and add the H2 hash in the control
             * If the key is found, return the iterator
             * If not found insert the key but do not construct the value.
             */
            template<typename group_t, typename K>
            [[nodiscard]] constexpr hud::pair<iterator, bool> find_or_insert_no_construct_impl(K &&key) noexcept
            {
                u64 hash {compute_hash(key)};
                u64 h1(H1(hash));
                HUD_CHECK(hud::bits::is_valid_power_of_two_mask(max_slot_count_) && "Not a mask");
                usize slot_index(h1 & max_slot_count_);
                while (true)
                {
                    // Find the key if present, return iterator to it if not prepare for insertion
                    const group_t group {control_ptr_ + slot_index};
                    const typename group_t::mask group_mask_that_match_h2 {group.match(H2(hash))};
                    for (u32 group_index_that_match_h2 : group_mask_that_match_h2)
                    {
                        usize slot_index_that_match_h2 {slot_index + group_index_that_match_h2 & max_slot_count_};
                        slot_type *slot_that_match_h2 {slot_ptr_ + slot_index_that_match_h2};
                        if (key_equal()(slot_that_match_h2->key(), key)) [[likely]]
                        {
                            return {
                                iterator {control_ptr_ + slot_index_that_match_h2, slot_that_match_h2},
                                false
                            };
                        }
                    }

                    // Don't find the key
                    auto empty_mask_of_group {group.mask_of_empty_slot()};
                    if (empty_mask_of_group.has_empty_slot()) [[likely]]
                    {
                        return {insert_no_construct(h1, H2(hash)), true};
                    }

                    // Advance to next group (Maybe a control iterator taht iterate over groups can be better alternative)
                    slot_index += group_t::SLOT_PER_GROUP;
                    slot_index &= max_slot_count_;
                }
            }

            /** Insert a slot index associated with the given h2 hash. */
            [[nodiscard]] constexpr iterator insert_no_construct(u64 h1, u8 h2) noexcept
            {
                // If we reach the load factor grow the table and retrieves the new slot, else use the given slot
                if (free_slot_before_grow() == 0)
                {
                    resize(next_capacity());
                }

                // Find the first empty of deleted slot that can be used for this h1 hash
                usize slot_index {find_first_empty_or_deleted(control_ptr_, max_slot_count_, h1)};
                count_++;
                control::set(control_ptr_, slot_index, h2, max_slot_count_);

                free_slot_before_grow_compressed()--;
                return iterator {control_ptr_ + slot_index, slot_ptr_ + slot_index}; // slot_index;
            }

            constexpr void resize(usize new_max_slot_count) noexcept
            {
                // HUD_CHECK(new_max_slot_count > max_slot_count_ && "Grow need a bigger value");
                HUD_CHECK(hud::bits::is_valid_power_of_two_mask(new_max_slot_count) && "Not a mask");

                // Create the buffer with control and slots
                // Slots are aligned based on alignof(slot_type)
                // In the case of a constant-evaluated context, slot_ptr_ may be uninitialized when the map is created
                // To satisfy the compiler, initialize it to nullptr in this case
                if (hud::is_constant_evaluated() ? (control_ptr_ == &INIT_GROUP[16]) : false)
                {
                    slot_ptr_ = nullptr;
                }
                control_type *old_control_ptr {control_ptr_};
                slot_type *old_slot_ptr {slot_ptr_};
                usize old_max_slot_count {max_slot_count_};

                max_slot_count_ = new_max_slot_count;

                // Allocate the buffer that will contain controls and aligned slots
                // In a constant-evaluated context, to satisfy the compiler, allocate controls and slots in two separate allocations
                usize control_size {allocate_control_and_slot(max_slot_count_)};

                // Update number of slot we should put into the table before a resizing rehash
                free_slot_before_grow_compressed() = max_slot_before_grow(max_slot_count_) - count_;

                // Set control to empty ending with sentinel
                hud::memory::set_memory(control_ptr_, control_size, empty_byte);
                control_ptr_[max_slot_count_] = sentinel_byte;

                // If we have elements, insert them to the new buffer
                if (count_ > 0)
                {
                    // Move elements to new buffer if any
                    // Relocate slots to newly allocated buffer
                    auto insert_slot_by_copy = [this](control_type *control_ptr, auto *slot_ptr)
                    {
                        // Compute the hash
                        u64 hash {compute_hash(slot_ptr->key())};
                        // Find H1 slot index
                        u64 h1 {H1(hash)};
                        usize slot_index {find_first_empty_or_deleted(control_ptr_, max_slot_count_, h1)};
                        // Save h2 in control h1 index
                        control::set(control_ptr_, slot_index, H2(hash), max_slot_count_);
                        // Move old slot to new slot
                        hud::memory::move_or_copy_construct_object_then_destroy(slot_ptr_ + slot_index, hud::move(*slot_ptr));
                    };
                    iterate_over_full_slots(old_control_ptr, old_slot_ptr, count_, old_max_slot_count, insert_slot_by_copy);
                    free_control_and_slot(old_control_ptr, old_slot_ptr, old_max_slot_count);
                }
            }

            [[nodiscard]] constexpr usize free_slot_before_grow() const noexcept
            {
                // Remove the sign bit that represent if the map contains deleted slots
                return free_slot_before_grow_compressed() & ((~usize {}) >> 1);
            }

            /** Retrieves the next capacity after a grow. */
            [[nodiscard]] constexpr usize next_capacity() const noexcept
            {
                // Value are always power of two mask 0,1,3,7,15,31,63, etc...
                return max_slot_count_ * 2 + 1;
            }

            /** Retrieves a correct max slot count that is applicable. */
            [[nodiscard]] constexpr usize normalize_max_count(usize max_slot_count) const noexcept
            {
                return max_slot_count ? ~usize {} >> hud::bits::leading_zeros(max_slot_count) : 0;
            }

            /** Compute the size of the allocation needed for the given slot count. */
            [[nodiscard]] constexpr usize current_allocation_size() const noexcept
            {
                const usize control_size {control_size_for_max_count(max_slot_count_)};
                const uptr aligned_control_size {hud::memory::align_address(control_size, sizeof(slot_type))};
                return aligned_control_size + max_slot_count_ * sizeof(slot_type);
            }

            /** Find the first empty or deleted slot for the given h1.
             * WARNING: This function works only if there is free or deleted slot available
             * @param h1 The H1 hash
             * @return The first empty or deleted slot that can be used for h1 hash
             */
            [[nodiscard]] static constexpr usize find_first_empty_or_deleted(control_type *control_ptr, u64 max_slot_count, u64 h1) noexcept
            {
                if consteval
                {
                    return find_first_empty_or_deleted_impl<portable_group>(control_ptr, max_slot_count, h1);
                }
                else
                {
                    return find_first_empty_or_deleted_impl<group_type>(control_ptr, max_slot_count, h1);
                }
            }

            template<typename group_t>
            [[nodiscard]] static constexpr usize find_first_empty_or_deleted_impl(control_type *control_ptr, u64 max_slot_count, u64 h1) noexcept
            {
                HUD_CHECK(hud::bits::is_valid_power_of_two_mask(max_slot_count) && "Not a mask");
                usize slot_index(h1 & max_slot_count);
                while (true)
                {
                    const group_t group {control_ptr + slot_index};
                    const typename group_t::empty_or_deleted_mask group_mask_that_match_h2 {group.mask_of_empty_or_deleted_slot()};
                    if (group_mask_that_match_h2.has_empty_or_deleted_slot())
                    {
                        u32 free_index {group_mask_that_match_h2.first_empty_or_deleted_index()};
                        usize slot_index_that_is_free_or_deleted(slot_index + free_index & max_slot_count);
                        return slot_index_that_is_free_or_deleted;
                    }

                    slot_index += group_t::SLOT_PER_GROUP;
                    slot_index &= max_slot_count;
                }
            }

            [[nodiscard]]
            constexpr hud::pair<control_type *, slot_type *> find_first_full() const noexcept
            {
                if consteval
                {
                    return find_first_full_impl<portable_group>();
                }
                else
                {
                    return find_first_full_impl<group_type>();
                }
            }

            template<typename group_t>
            [[nodiscard]] constexpr hud::pair<control_type *, slot_type *> find_first_full_impl() const noexcept
            {
                HUD_CHECK(hud::bits::is_valid_power_of_two_mask(max_slot_count_) && "Not a mask");
                usize slot_index {0};
                while (slot_index < max_slot_count_)
                {
                    const group_t group {control_ptr_ + slot_index};
                    const typename group_t::full_mask group_mask {group.mask_of_full_slot()};
                    if (group_mask.has_full_slot())
                    {
                        u32 first_full_index {group_mask.first_full_index()};
                        return {control_ptr_ + first_full_index, slot_ptr_ + first_full_index};
                    }

                    slot_index += group_t::SLOT_PER_GROUP;
                }
                return {control_ptr_sentinel(), nullptr};
            }

            /**
             * Compute the maximum number of slots we should put into the table before a resizing rehash.
             * Subtract the returned value with the number of slots `count()` to obtains the number of slots we can currently use before a resizing rehash.
             */
            [[nodiscard]] constexpr usize max_slot_before_grow(usize capacity) const noexcept
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
                return SLOT_PER_GROUP() == 8 && capacity == 7 ? 6 :
                                                                capacity - capacity / 8;
            }

            /**
             * Compute the minimum capacity needed for the given `count` element that respect the load factor.
             * The capacity can be invalid, you should call
             */
            [[nodiscard]] constexpr usize min_capacity_for_count(usize count) const noexcept
            {
                // `count*8/7`
                if (SLOT_PER_GROUP() == 8 && count == 7)
                {
                    // x+(x-1)/7 does not work when x==7.
                    return 8;
                }
                return count + static_cast<usize>((static_cast<i64>(count) - 1) / 7);
            }

            [[nodiscard]]
            constexpr control_type *control_ptr_sentinel() const noexcept
            {
                HUD_CHECK(control::is_byte_sentinel(control_ptr_[max_slot_count_]));
                return control_ptr_ + max_slot_count_;
            }

            constexpr usize control_size_for_max_count(usize max_slot_count) const noexcept
            {
                // We cloned size of a group - 1 because we never reach the last cloned bytes
                // Control size is the number of slot + sentinel + number of cloned bytes
                return max_slot_count + 1 + SLOT_PER_GROUP();
            }

            constexpr usize allocate_control_and_slot(usize max_slot_count) noexcept
            {
                /**
                 *  Allocation layout is :
                 *
                 *  |      aligned_control_size      | slots_size |
                 *  | control_size  |                |
                 *  |  controls     |  slot padding  |    slots   |
                 *  ^                                ^
                 *  control_ptr_                      slot_ptr_
                 */
                const usize control_size {control_size_for_max_count(max_slot_count)};
                const usize slots_size {max_slot_count * sizeof(slot_type)};

                if consteval
                {
                    control_ptr_ = allocator_mut().template allocate<control_type>(control_size).data();
                    slot_ptr_ = allocator_mut().template allocate<slot_type>(slots_size).data();
                }
                else
                {
                    // Allocate control, slot, and slot size to satisfy slot_ptr_ alignment requirements
                    const usize aligned_allocation_size {control_size + sizeof(slot_type) + slots_size};
                    control_ptr_ = allocator_mut().template allocate<control_type>(aligned_allocation_size).data();
                    slot_ptr_ = reinterpret_cast<slot_type *>(hud::memory::align_address(reinterpret_cast<const uptr>(control_ptr_ + control_size), sizeof(slot_type)));
                    HUD_CHECK(hud::memory::is_pointer_aligned(slot_ptr_, alignof(slot_type)));
                }
                return control_size;
            }

            constexpr void free_control_and_slot(control_type *control_ptr, slot_type *slot_ptr, usize max_slot_count) noexcept
            {
                if (max_slot_count > 0)
                {
                    const usize control_size {control_size_for_max_count(max_slot_count)};
                    const usize slots_size {max_slot_count * sizeof(slot_type)};

                    // In a constant-evaluated context, bit_cast cannot be used with pointers
                    // and allocation is done in two separate allocation
                    if consteval
                    {
                        allocator_mut().template free<control_type>({control_ptr, control_size});
                        allocator_mut().template free<slot_type>({slot_ptr, slots_size});
                    }
                    else
                    {
                        const usize aligned_allocation_size {control_size + sizeof(slot_type) + slots_size};
                        allocator_mut().template free<control_type>({control_ptr, aligned_allocation_size});
                    }
                }
            }

            /**
             * Resets the hashset control and slot arrays.
             * Frees the memory used by controls and slots, then resets internal state to default.
             */
            constexpr void reset_control_and_slot() noexcept
            {
                free_control_and_slot(control_ptr_, slot_ptr_, max_slot_count_);
                control_ptr_ = const_cast<control_type *>(&INIT_GROUP[16]);
                max_slot_count_ = 0;
                count_ = 0;
                free_slot_before_grow_compressed() = 0;
            }

            /**
             * Destroys all slots in the hashset.
             * For non-trivially destructible slot types, calls the destructor for each element.
             * Trivial types are ignored for efficiency.
             */
            constexpr void destroy_all_slots() noexcept
            {
                if constexpr (!hud::is_trivially_destructible_v<slot_type>)
                {
                    auto destroy_slot = [](control_type *control_ptr, slot_type *slot_ptr)
                    {
                        hud::memory::destroy_object(slot_ptr);
                    };
                    iterate_over_full_slots(control_ptr_, slot_ptr_, count_, max_slot_count_, destroy_slot);
                }
            }

            constexpr void iterate_over_full_slots(control_type *control_ptr, auto *slot_ptr, usize slot_count, usize max_slot_count, auto callback) noexcept
            {
                if consteval
                {
                    return iterate_over_full_slots_impl<portable_group>(control_ptr, slot_ptr, slot_count, max_slot_count, callback);
                }
                else
                {
                    return iterate_over_full_slots_impl<group_type>(control_ptr, slot_ptr, slot_count, max_slot_count, callback);
                }
            }

            /**
             * Iterate through full slot.
             * Caution : Do not erase value during iteration
             */
            template<typename group_t>
            constexpr void iterate_over_full_slots_impl(control_type *control_ptr, auto *slot_ptr, usize slot_count, usize max_slot_count, auto callback) noexcept
            {
                // When max slot count is less than the probing group
                // We have cloned control in the group
                // In this case, we start probing at the sentinel instead of 0
                if (max_slot_count < group_t::SLOT_PER_GROUP - 1)
                {
                    // In the case of constant expression
                    // If the hashmap is empty, slot_ptr is nullptr, we don't want to decrement the pointer in the case
                    // In a non constant expression slot_ptr is located after control in the same memory layout,
                    // we can safely decrement as soon as we don't read the value
                    const group_t group {control_ptr + max_slot_count};

                    // Iterate over cloned control bytes
                    for (u32 full_index : group.mask_of_full_slot())
                    {
                        u32 real_index = (full_index - 1);
                        callback(control_ptr + real_index, slot_ptr + real_index);
                    }
                }
                else
                {
                    while (slot_count != 0)
                    {
                        const group_t group {control_ptr};
                        for (u32 full_index : group.mask_of_full_slot())
                        {
                            callback(control_ptr + full_index, slot_ptr + full_index);
                            --slot_count;
                        }
                        control_ptr += group_t::SLOT_PER_GROUP;
                        slot_ptr += group_t::SLOT_PER_GROUP;
                    }
                }
            }

            [[nodiscard]] constexpr usize &free_slot_before_grow_compressed() noexcept
            {
                return hud::get<3>(compressed_);
            }

            [[nodiscard]] constexpr const usize &free_slot_before_grow_compressed() const noexcept
            {
                return hud::get<3>(compressed_);
            }

            [[nodiscard]] constexpr allocator_type &allocator_mut() noexcept
            {
                return hud::get<0>(compressed_);
            }

            [[nodiscard]] constexpr hasher_type &hasher() noexcept
            {
                return hud::get<1>(compressed_);
            }

            [[nodiscard]] constexpr const hasher_type &hasher() const noexcept
            {
                return hud::get<1>(compressed_);
            }

            [[nodiscard]] constexpr key_equal_type &key_equal() noexcept
            {
                return hud::get<2>(compressed_);
            }

            [[nodiscard]] constexpr const key_equal_type &key_equal() const noexcept
            {
                return hud::get<2>(compressed_);
            }

        private:
            /** Max count of slot in the map. Always a power of two mask value. */
            usize max_slot_count_ {0};

            /** The count of values in the hashmap. */
            usize count_ {0};

            /**
             * 0 - The allocator
             * 1 - The hasher
             * 2 - The key equal
             * 3 -  Number of slot we should put into the table before a resizing rehash
             */
            hud::compressed_tuple<allocator_type, hasher_type, key_equal_type, usize> compressed_ {hud::tag_init};

            /** The control of the hashmap. Initialized to sentinel. */
            control_type *control_ptr_ {const_cast<control_type *>(&INIT_GROUP[16])};

            /** Pointer to the slot segment. */
            slot_type *slot_ptr_ {nullptr};
        };

    } // namespace details::hashset

    template<
        typename element_t,
        typename hasher_t = hud::hash_64<element_t>,
        typename key_equal_t = hud::equal<element_t>,
        typename allocator_t = hud::heap_allocator>
    class hashset
        : public details::hashset::hashset_impl<details::hashset::hashset_storage<element_t>, hasher_t, key_equal_t, allocator_t>
    {
    private:
        using super = details::hashset::hashset_impl<details::hashset::hashset_storage<element_t>, hasher_t, key_equal_t, allocator_t>;

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
        /** Type of the storage used to store key. */
        using storage_type = typename super::storage_type;
        /** Type of the key. */
        using key_type = typename storage_type::key_type;

        /** Inherit constructors and methods from the base class. */
        using super::reserve;
        using super::super;
        using super::operator=;

        explicit constexpr hashset() noexcept = default;

        constexpr explicit hashset(const allocator_type &allocator) noexcept
            : super {allocator}
        {
        }

        template<typename u_element_t>
        requires(hud::is_constructible_v<storage_type, u_element_t>)
        constexpr hashset(std::initializer_list<u_element_t> list, const allocator_type &allocator = allocator_type()) noexcept
            : super {allocator}
        {
            reserve(list.size());
            for (auto &value : list)
            {
                add(hud::move(value));
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

        template<typename u_key_t = key_type>
        requires(hud::is_copy_constructible_v<key_type, u_key_t>)
        constexpr hashset(std::initializer_list<u_key_t> list, const usize extra_element_count, const allocator_type &allocator = allocator_type()) noexcept
            : super(allocator)
        {
            static_assert(hud::is_nothrow_copy_constructible_v<key_type, u_key_t>, "key_type(const u_key_t&) copy constructor is throwable. hashset_storage is not designed to allow throwable copy constructible components");
            reserve(list.size() + extra_element_count);
            for (const auto &element : list)
            {
                add(element);
            }
        }

        /**
         * Insert a key in the hashset.
         * @param key The key
         * @return Iterator to the storage containing the `key`
         */
        template<typename u_key_t = key_type>
        constexpr iterator add(u_key_t &&key) noexcept
        requires(hud::is_constructible_v<storage_type, u_key_t>)
        {
            return super::add_impl(hud::forward<u_key_t>(key));
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
        template<typename key_tuple_t>
        constexpr iterator add(hud::tag_piecewise_construct_t, key_tuple_t &&key_tuple) noexcept
        {
            return super::add_impl(hud::tag_piecewise_construct, hud::forward<key_tuple_t>(key_tuple));
        }
    };

    template<typename key_t>
    struct tuple_size<details::hashset::hashset_storage<key_t>>
        : hud::integral_constant<usize, 1>
    {
    };

    template<usize idx_to_reach, typename key_t>
    struct tuple_element<idx_to_reach, details::hashset::hashset_storage<key_t>>
    {
        static_assert(idx_to_reach < 1, "hashset hashset_storage index out of bounds");
        using type = const typename details::hashset::hashset_storage<key_t>::key_type;
    };

    template<typename key_t, typename hasher_t, typename key_equal_t, typename allocator_t>
    constexpr void swap(hashset<key_t, hasher_t, key_equal_t, allocator_t> &first, hashset<key_t, hasher_t, key_equal_t, allocator_t> &second) noexcept
    {
        first.swap(second);
    }

    template<typename key_t, typename hasher_t, typename key_equal_t, typename allocator_t>
    [[nodiscard]] constexpr bool operator==(const hashset<key_t, hasher_t, key_equal_t, allocator_t> &left, const hashset<key_t, hasher_t, key_equal_t, allocator_t> &right) noexcept
    {
        // Map are not equal if the counts of elements differ
        if (left.count() != right.count())
        {
            return false;
        }

        // Speed of find is dependent of the max_slot_count_
        // We want to find in the smallest max_slot_count and iterate on the bigger only once
        const hashset<key_t, hasher_t, key_equal_t, allocator_t> *biggest_capacity = &left;
        const hashset<key_t, hasher_t, key_equal_t, allocator_t> *smallest_capacity = &right;
        if (smallest_capacity->max_count() > biggest_capacity->max_count())
        {
            hud::swap(biggest_capacity, smallest_capacity);
        }

        // Iterate over biggest capacity and find in the smallest each elements
        for (const auto &elem : *biggest_capacity)
        {
            const auto &it = smallest_capacity->find(elem.key());
            if (it == smallest_capacity->end())
            {
                return false;
            }
        }
        return true;
    }

} // namespace hud

namespace std
{
    template<typename key_t>
    struct tuple_size<hud::details::hashset::hashset_storage<key_t>>
        : hud::tuple_size<hud::details::hashset::hashset_storage<key_t>>
    {
    };

    template<std::size_t idx_to_reach, typename key_t>
    struct tuple_element<idx_to_reach, hud::details::hashset::hashset_storage<key_t>>
        : hud::tuple_element<idx_to_reach, hud::details::hashset::hashset_storage<key_t>>
    {
    };

} // namespace std
#endif // HD_INC_CORE_HASHSET_H