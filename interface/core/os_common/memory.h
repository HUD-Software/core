#ifndef HD_INC_CORE_OS_COMMON_MEMORY_H
#define HD_INC_CORE_OS_COMMON_MEMORY_H
#include "../assert.h"
#include <stdlib.h> // malloc, free, ...
#include <string.h> // memset
#include <memory>   // std::allocator
#include "../math.h"
#include "../traits/is_constant_evaluated.h"
#include "../traits/is_not_same.h"
#include "../traits/is_pointer.h"
#include "../traits/is_constant_evaluated.h"

namespace hud::os::common
{

    struct memory
    {

        /**
         * Aligned allocation header size.
         * Aligned pointer memory follow this pattern : AlignmentPadding|AllocationSize|UnalignPointer|AlignPointer|UserData.
         */
        static constexpr const usize ALIGNED_MALLOC_HEADER_SIZE = sizeof(void *) + sizeof(uptr);

        /**
         * Align a given address.
         * @param address The address to align
         * @param alignment The alignment to set
         * @return The aligned address
         */
        [[nodiscard]] static constexpr uptr align_address(const uptr address, const u32 alignment) noexcept
        {
            return address + (alignment - 1) & ~uptr(alignment - 1);
        }

        /**
         * Check if address is aligned.
         * @param address The address to check
         * @param alignment Alignment in bytes
         * @return true if the address is aligned to the given alignment, false otherwise
         */
        [[nodiscard]] static constexpr bool is_address_aligned(const uptr address, const u32 alignment) noexcept
        {
            return (address & (alignment - 1)) == 0;
        }

        /**
         * Align the given pointer and add header before the align pointer.
         * This assume allocation is big enough. ( Size + Alignment + sizeof(size) + sizeof(uptr) )
         * memory follow this pattern : AlignmentPadding|AllocationSize|UnalignPointer|AlignPointer
         * @param unaligned_pointer The unaligned pointer to align
         * @param size The size of the allocation of the unaligned_pointer
         * @param alignment Alignment in bytes
         * @return Aligned pointer with header information before the returned pointer, nullptr otherwise
         */
        [[nodiscard]] static void *align_pointer(void *const unaligned_pointer, const usize size, const u32 alignment) noexcept
        {
            const uptr unaligned_address = (const uptr)unaligned_pointer;
            const uptr aligned_pointer = align_address(unaligned_address + ALIGNED_MALLOC_HEADER_SIZE, alignment);

            *((void **)(aligned_pointer - sizeof(uptr))) = unaligned_pointer;    // Write unaligned pointer address
            *((usize *)(aligned_pointer - sizeof(uptr) - sizeof(usize))) = size; // Write size of the allocation

            return reinterpret_cast<void *>(aligned_pointer);
        }

        /**
         * Check if a pointer is aligned.
         * @param pointer The pointer to test
         * @param alignment Alignment in bytes
         * @return true if the pointer is aligned, false otherwise
         */
        template<typename type_t>
        [[nodiscard]] static constexpr bool is_pointer_aligned(const type_t *const pointer, const u32 alignment) noexcept
        {
            return is_address_aligned(reinterpret_cast<const uptr>(pointer), alignment);
        }

        /**
         * Retrieves the unaligned pointer from an aligned pointer.
         * @param aligned_pointer The aligned pointer of the allocation
         * @return The unaligned pointer
         */
        [[nodiscard]] static HD_FORCEINLINE void *get_unaligned_pointer(const void *const aligned_pointer) noexcept
        {
            return *(void **)((const uptr)aligned_pointer - sizeof(uptr));
        }

        /**
         * Allocate memory block with no alignment requirements. Must be freed with hud::memory::free.
         * If Allocation failed or size is 0, nullptr is returned
         * @param size Number of bytes to allocate
         * @return Pointer to the allocated memory block, nullptr if failed
         */
        [[nodiscard]] static void *allocate(const usize size) noexcept
        {
            if (size == 0) [[unlikely]]
            {
                return nullptr;
            }
            void *unaligned_pointer = ::malloc(size);
            if (unaligned_pointer != nullptr) [[unlikely]]
            {
                // assert(false);// OOM
            }
            return unaligned_pointer;
        }

        /**
         * Allocate memory block with no alignment requirements. Must be freed with hud::memory::free.
         * If Allocation failed or size is 0, nullptr is returned
         * @param size Number of bytes to allocate
         * @param extra Number of extra bytes to allocate
         * @return Pointer to the allocated memory block, nullptr if failed
         */
        [[nodiscard]] static void *allocate(const usize size, const usize extra) noexcept
        {
            if (size == 0) [[unlikely]]
            {
                return nullptr;
            }
            void *unaligned_pointer = ::malloc(size + extra);
            if (unaligned_pointer != nullptr) [[unlikely]]
            {
                // assert(false);// OOM
            }
            return unaligned_pointer;
        }

        /**
         * Allocate memory block of a defined type type_t with no alignment requirements. Must be freed with hud::memory::free.
         * Never used if type_t is void because it's constexpr version of allocate.
         * Indeed, using void* return type can implies a cast that is not allowed in constexpr.
         * Sometimes, compiler can use this fonction in non constant evaluated expression because type_t is not void or provided by the user-defined code.
         * In this case, we forward the pointer to allocate and reinterpret_cast to type_t* the returned pointer.
         * This enforce the choise of std::allocate<type_t>.allocate() ONLY in constant evaluated expression because reinterpret_cast to type_t* will result in a compilation error in constant evaluated expression.
         * @tparam type_t The type to allocate
         * @param count Number of type_t to allocate
         * @return Pointer to the allocated memory block, nullptr if failed
         */
        template<typename type_t>
        requires(hud::is_not_same_v<type_t, void>)
        [[nodiscard]] static constexpr type_t *allocate_array(const usize count) noexcept
        {
            const usize allocation_size = count * sizeof(type_t);
            if (hud::is_constant_evaluated())
            // LCOV_EXCL_START
            {
                // Usage of std::allocator.allocate is allowed in constexpr dynamic allocation.
                // The allocation should be freed with std::allocator<type_t>().deallocate in the same constexpr expression
                return (allocation_size > 0u) ? std::allocator<type_t>().allocate(count) : nullptr;
            }
            // LCOV_EXCL_STOP
            return reinterpret_cast<type_t *>(allocate(allocation_size));
        }

        /**
         * Allocate memory block with no alignment requirements and fille the buffer with 0. Must be freed with hud::memory::free
         * @param size Number of bytes to allocate
         * @return Pointer to the allocated memory block, nullptr if failed
         */
        [[nodiscard]] static HD_FORCEINLINE void *allocate_zero(const usize size) noexcept
        {
            void *buffer = allocate(size);
            if (buffer != nullptr) [[likely]]
            {
                set_zero(buffer, size);
            }
            return buffer;
        }

        /**
         * Allocate aligned memory block. Must be freed with hud::memory::free_align
         * @param size Number of bytes to allocate
         * @param alignment Required alignment in bytes
         * @return Pointer to the aligned allocated memory block, nullptr if failed
         */
        [[nodiscard]] static void *allocate_align(const usize size, const u32 alignment) noexcept
        {
            void *unaligned_pointer = allocate(size, alignment + ALIGNED_MALLOC_HEADER_SIZE);
            if (unaligned_pointer) [[likely]]
            {
                return align_pointer(unaligned_pointer, size, alignment);
            }
            return nullptr;
        }

        /**
         * Allocate aligned memory block in a constant evaluated expression.
         * In constant evaluated expression, this do only a simple allocation without alignment
         * To be used in a constant expression the type type of the allocation must be provided,
         * void* is not usable in constant evaluated expression.
         * @param count Number of type_t to allocate
         * @param alignment Required alignment in bytes. Used only in non constant evaluated expression
         * @return Pointer to the aligned allocated memory block, nullptr if failed
         */
        template<typename type_t>
        static constexpr type_t *allocate_align(const usize count, const u32 alignment) noexcept
        {
            if (hud::is_constant_evaluated())
            {
                return allocate_array<type_t>(count);
            }
            return reinterpret_cast<type_t *>(allocate_align(count * sizeof(type_t), alignment));
        }

        /**
         * Allocate aligned memory block and fille the buffer with 0. Must be freed with hud::memory::free_align
         * @param size Number of bytes to allocate
         * @param alignment Alignment in bytes
         * @return Pointer to the allocated memory block, nullptr if failed
         */
        [[nodiscard]] static HD_FORCEINLINE void *allocate_align_zero(const usize size, const u32 alignment) noexcept
        {
            void *buffer = allocate_align(size, alignment);
            if (buffer != nullptr) [[likely]]
            {
                set_zero(buffer, size);
            }
            return buffer;
        }

        /** Free memory block with no alignment requirements allocated with allocate */
        static HD_FORCEINLINE void free(const void *pointer) noexcept
        {
            ::free((void *)pointer);
        }

        /**
         * Free memory block of a defined type type_t.
         * Never used if type_t is void or not trivially destructible because it's constexpr version of free.
         * Indeed, using void* can implies a cast that is not allowed in constexpr.
         * Sometimes, compiler can use this fonction in non constant evaluated expression because type_t is not void.
         * In this case, we forward the pointer to free and reinterpret_cast to void* the given pointer.
         * This enforce the choise of std::allocator<type_t>::deallocate() ONLY in constant evaluated expression because reinterpret_cast to
         * void* will result in a compilation error in constant evaluated expression.
         * @tparam type_t The type to deallocate. Do not destroy the type.
         * @param alloc The allocation to free. Must be allocated with the constexpr allocate function in a constexpr expression.
         * @param count Number of type_t to deallocate
         */
        template<typename type_t>
        static constexpr void free_array(type_t *alloc, const usize count) noexcept
        {
            if (hud::is_constant_evaluated())
            // LCOV_EXCL_START
            {
                if (alloc != nullptr)
                {
                    // Usage of std::allocator.deallocate is allowed in constexpr dynamic allocation.
                    // The allocation should be freed after std::allocator<type_t>().allocate in the same constexpr expression
                    std::allocator<type_t>().deallocate(alloc, count);
                }
            }
            // LCOV_EXCL_STOP
            else
            {
                free(reinterpret_cast<void *>(alloc));
            }
        }

        /**
         * Free memory block of a defined type type_t in a constant expression
         * Never used if type_t is void or not trivially destructible because it's constexpr version of free.
         * Indeed, using void* can implies a cast that is not allowed in constexpr.
         * Sometimes, compiler can use this fonction in non constant evaluated expression because type_t is not void.
         * In this case, we forward the pointer to free and reinterpret_cast to void* the given pointer.
         * This enforce the choise of delete[] ONLY in constant evaluated expression because reinterpret_cast to
         * void* will result in a compilation error in constant evaluated expression.
         * @tparam type_t The type to allocate. Do not construct the type.
         * @param alloc The allocation to free. Must be allocated with the constexpr allocate function in a constexpr expression
         * @param count Number of type_t to allocate
         */
        template<typename type_t>
        static constexpr void free_align(type_t *pointer, const usize count) noexcept
        {
            if (hud::is_constant_evaluated())
            // LCOV_EXCL_START
            {
                free_array(pointer, count);
            }
            // LCOV_EXCL_STOP
            else
            {
                free_align(reinterpret_cast<const void *>(pointer));
            }
        }

        /** Free aligned memory block allocated with allocate_align */
        static HD_FORCEINLINE void free_align(const void *pointer) noexcept
        {
            if (pointer != nullptr)
            {
                free(get_unaligned_pointer(pointer));
            }
        }

        /**
         * Reallocate a memory block with no alignment requrements allocated with allocate.
         * If pointer is nullptr, the behaviour is the same has allocate.
         * If size is 0, the behaviour is the same has hud::memory::free and return value is nullptr.
         * After a reallocation, pointer is invalidated.
         * @param pointer Pointer to memory block to reallocate
         * @param size Number of bytes to reallocate
         * @return Pointer to the re-allocated memory block or nullptr
         */
        [[nodiscard]] static HD_FORCEINLINE void *reallocate(void *pointer, const usize size) noexcept
        {
            return ::realloc(pointer, size);
        }

        /**
         * Reallocate an aligned memory block allocated with allocate_align.
         * If pointer is nullptr, the behaviour is the same has allocate_align.
         * If size is 0, the behaviour is the same has hud::memory::free_align and return value is nullptr.
         * After a reallocation, pointer is invalidated.
         * @param pointer Pointer to memory block to reallocate
         * @param size Number of bytes to reallocate
         * @param alignment Alignment in bytes
         * @return Pointer to the re-allocated memory block or nullptr
         */
        [[nodiscard]] static void *reallocate_align(void *pointer, const usize size, const u32 alignment) noexcept
        {
            if (pointer && size)
            {
                usize old_size = *((usize *)((const uptr)pointer - sizeof(void *) - sizeof(uptr))); // Read the size of the allocation
                if (size == old_size)
                {
                    return pointer;
                }
                usize size_to_copy = size < old_size ? size : old_size;
                // Realloc alignment mean realloc and copy the data
                void *aligned_pointer = allocate_align(size, alignment);
                // Copy the current data to the new buffer
                copy(aligned_pointer, pointer, size_to_copy);
                free_align(pointer);
                return aligned_pointer;
            }
            else if (!pointer)
            {
                return allocate_align(size, alignment);
            }
            else
            {
                free_align(pointer);
                return nullptr;
            }
        }

        /**
         * Copy block of memory. Do not support overlapped buffer. Use move to support overlapped buffer.
         * @param destination Pointer to the destination array where the content is to be copied
         * @param source Pointer to the source of data to be copied
         * @param size Number of bytes to copy
         * @return destination pointer
         */
        static void *copy(void *destination, const void *source, const usize size) noexcept
        {
            // The behavior is undefined if either destination or source is an invalid or null pointer.
            check(destination != nullptr);
            check(source != nullptr);

            // The behavior is undefined if the objects overlap
            if (size > 0) [[likely]]
            {
                check(destination != source);
                const volatile uptr dest_addr = reinterpret_cast<uptr>(destination);
                const volatile uptr src_addr = reinterpret_cast<uptr>(source);
                check(!(dest_addr >= src_addr && dest_addr < src_addr + size));
                check(!(src_addr >= dest_addr && src_addr < dest_addr + size));
            }

            return memcpy(destination, source, size);
        }

        template<typename type_t>
        requires(sizeof(type_t) == 1)
        static constexpr type_t *copy(type_t *destination, const type_t *source, const usize size) noexcept
        {
            if (hud::is_constant_evaluated())
            // LCOV_EXCL_START
            {
                type_t *dest = destination;
                for (usize position = 0; position < size; position++)
                {
                    std::construct_at(destination, *source);
                    destination++;
                    source++;
                }
                return dest;
            }
            // LCOV_EXCL_STOP
            else
            {
                return static_cast<type_t *>(copy(static_cast<void *>(destination), static_cast<const void *>(source), size));
            }
        }

        /**
         * Copy block of memory. Do not support overlapped buffer. Use move to support overlapped buffer.
         * @param destination Pointer to the destination array where the content is to be copied
         * @param source Pointer to the source of data to be copied
         * @param size Number of bytes to copy
         * @return destination pointer
         */
        template<typename type_t, usize buffer_size>
        static constexpr type_t *copy(type_t (&destination)[buffer_size], const type_t (&source)[buffer_size]) noexcept
        {
            return copy(destination, source, buffer_size * sizeof(type_t));
        }

        /**
         * Sets the first size bytes of the block of memory pointed by destination to the specified value.
         * @param destination Pointer to the buffer
         * @param size Number of bytes to set to the value
         * @param value The value to be set
         * @return Pointer to the buffer @p destination
         */
        static HD_FORCEINLINE void *set(void *destination, const usize size, const u8 value) noexcept
        {
            // The behavior is undefined if destination is a null pointer.
            check(destination != nullptr);
            return memset(destination, value, size);
        }

        static constexpr void set(u8 *destination, const usize size, const u8 value) noexcept
        {
            if (hud::is_constant_evaluated())
            // LCOV_EXCL_START
            {
                for (usize position = 0; position < size; position++)
                {
                    std::construct_at(destination + position, value);
                }
            }
            // LCOV_EXCL_STOP
            else
            {
                set(static_cast<void *>(destination), size, value);
            }
        }

        template<typename type_t>
        static constexpr void set(type_t *destination, const usize size, const u8 value) noexcept
        requires(is_integral_v<type_t>)
        {
            if (hud::is_constant_evaluated())
            // LCOV_EXCL_START
            {
                for (usize position = 0; position < size / sizeof(type_t); position++)
                {
                    std::construct_at(destination + position, value);
                }
            }
            // LCOV_EXCL_STOP
            else
            {
                set(static_cast<void *>(destination), size, value);
            }
        }

        /**
         * Sets the block of memory referenced by buffer to the specified value.
         * @param buffer The buffer memory to set to value
         * @param value The value to be set
         */
        template<typename type_t, usize buffer_size>
        static constexpr void set(type_t (&buffer)[buffer_size], const u8 value) noexcept
        {
            set(buffer, buffer_size * sizeof(type_t), value);
        }

        /**
         * Sets the first size bytes of the block of memory pointed by destination to zero.
         * @param destination Pointer to the buffer
         * @param size Number of bytes to set to zero
         */
        static HD_FORCEINLINE void set_zero(void *destination, const usize size) noexcept
        {
            set(destination, size, 0);
        }

        static constexpr void set_zero(u8 *destination, const usize size) noexcept
        {
            set(destination, size, 0);
        }

        template<typename type_t>
        requires(is_integral_v<type_t>)
        static constexpr void set_zero(type_t *destination, const usize size) noexcept
        {
            set(destination, size, 0);
        }

        template<typename type_t>
        requires(hud::is_pointer_v<type_t>)
        static constexpr void set_zero(type_t *destination, const usize size) noexcept
        {
            if (hud::is_constant_evaluated())
            // LCOV_EXCL_START
            {
                for (usize position = 0; position < size / sizeof(type_t); position++)
                {
                    std::construct_at(destination + position, nullptr);
                }
            }
            // LCOV_EXCL_STOP
            else
            {
                set(static_cast<void *>(destination), size, 0);
            }
        }

        /**
         * Set a block of memory referenced by buffer to zero.
         * @param buffer The buffer memory to set to zero
         */
        template<typename type_t, usize buffer_size>
        static constexpr void set_zero(type_t (&buffer)[buffer_size]) noexcept
        {
            set_zero(buffer, buffer_size * sizeof(type_t));
        }

        /**
         * Move block of memory. Support overlapped buffer. Prefers copy if buffer do not overlapped.
         * @param destination Pointer to the destination array where the content is to be copied
         * @param source Pointer to the source of data to be copied
         * @param size Number of bytes to copy
         * @return destination pointer
         */
        static HD_FORCEINLINE void *move(void *destination, const void *source, const usize size) noexcept
        {
            // The behavior is undefined if either destination or source is an invalid or null pointer.
            check(destination != nullptr);
            check(source != nullptr);
            return memmove(destination, source, size);
        }

        static constexpr void *move(u8 *destination, const u8 *source, const usize size) noexcept
        {

            if (hud::is_constant_evaluated())
            // LCOV_EXCL_START
            {
                u8 *dest = destination;
                if (size == 0)
                {
                    return destination;
                }
                u8 *tmp = hud::os::common::memory::allocate_array<u8>(size);
                // copy src to tmp
                for (usize position = 0; position < size; position++)
                {
                    std::construct_at(tmp + position, *source);
                    source++;
                }

                // copy tmp to destination
                for (usize position = 0; position < size; position++)
                {
                    *destination = tmp[position];
                    destination++;
                }

                hud::os::common::memory::free_array(tmp, size);
                return dest;
            }
            // LCOV_EXCL_STOP
            else
            {
                return move(static_cast<void *>(destination), static_cast<const void *>(source), size);
            }
        }

        /**
         * Compares the first size butes of the block of memory pointed by buffer1 to the first size bytes pointed by buffer2
         * The comparison is done lexicographically
         * @param buffer1 The first buffer
         * @param buffer2 The second buffer
         * @param size Number of bytes to compare
         * @return - Negative value if the first differing byte in buffer1 is less than the corresponding byte in buffer2
         *         - 0 if bytes in buffer1 and buffer2 are equal
         *         - Positive value if the first differing byte in buffer1 is greater than the corresponding byte in buffer2
         */
        [[nodiscard]] static HD_FORCEINLINE i32 compare(const void *buffer1, const void *buffer2, const usize size) noexcept
        {
            // The behavior is undefined if either buffer1 or buffer2 is a null pointer.
            check(buffer1 != nullptr);
            check(buffer2 != nullptr);
            return memcmp(buffer1, buffer2, size);
        }

        [[nodiscard]] static constexpr i32 compare(const u8 *buffer1, const u8 *buffer2, const usize size) noexcept
        {
            if (hud::is_constant_evaluated())
            // LCOV_EXCL_START
            {
                const u8 *lhs = buffer1;
                const u8 *rhs = buffer2;
                for (usize position = 0; position < size; position++)
                {
                    i32 diff = *lhs - *rhs;
                    if (diff)
                    {
                        return diff;
                    }
                    lhs++;
                    rhs++;
                }
                return 0;
            }
            // LCOV_EXCL_STOP
            else
            {
                return compare(static_cast<const void *>(buffer1), static_cast<const void *>(buffer2), size);
            }
        }

        template<typename type_t, usize buffer_size>
        [[nodiscard]] static constexpr i32 compare(const type_t (&buffer1)[buffer_size], const type_t (&buffer2)[buffer_size]) noexcept
        {
            return compare(buffer1, buffer2, buffer_size);
        }

        /**
         * Compares the first size bytes of the block of memory pointed by buffer1 to the first size bytes pointed by buffer2
         * The comparison is done lexicographically
         * @param buffer1 The first buffer
         * @param buffer2 The second buffer
         * @param size Number of bytes to compare
         * @return true if both buffers are equal, false otherwise
         */
        [[nodiscard]] static HD_FORCEINLINE bool compare_equal(const void *buffer1, const void *buffer2, const usize size) noexcept
        {
            return compare(buffer1, buffer2, size) == 0;
        }

        [[nodiscard]] static constexpr bool compare_equal(const u8 *buffer1, const u8 *buffer2, const usize size) noexcept
        {
            return compare(buffer1, buffer2, size) == 0;
        }

        template<typename type_t, usize buffer_size>
        [[nodiscard]] static constexpr bool compare_equal(const type_t (&buffer1)[buffer_size], const type_t (&buffer2)[buffer_size]) noexcept
        {
            return compare_equal(buffer1, buffer2, buffer_size * sizeof(type_t));
        }

        /**
         * Compares the first size bytes of the block of memory pointed by buffer1 to the first size bytes pointed by buffer2
         * The comparison is done lexicographically
         * @param buffer1 The first buffer
         * @param buffer2 The second buffer
         * @param size Number of bytes to compare
         * @return true if buffer1 is less that buffer2, false otherwise
         */
        [[nodiscard]] static HD_FORCEINLINE bool compare_less(const void *buffer1, const void *buffer2, const usize size) noexcept
        {
            return compare(buffer1, buffer2, size) < 0;
        }

        [[nodiscard]] static constexpr bool compare_less(const u8 *buffer1, const u8 *buffer2, const usize size) noexcept
        {
            return compare(buffer1, buffer2, size) < 0;
        }

        template<typename type_t, usize buffer_size>
        [[nodiscard]] static constexpr bool compare_less(const type_t (&buffer1)[buffer_size], const type_t (&buffer2)[buffer_size]) noexcept
        {
            return compare_less(buffer1, buffer2, buffer_size);
        }

        /**
         * Compares the first size bytes of the block of memory pointed by buffer1 to the first size bytes pointed by buffer2
         * The comparison is done lexicographically
         * @param buffer1 The first buffer
         * @param buffer2 The second buffer
         * @param size Number of bytes to compare
         * @return true if buffer1 is greater that buffer2, false otherwise
         */
        [[nodiscard]] static HD_FORCEINLINE bool compare_greater(const void *buffer1, const void *buffer2, const usize size) noexcept
        {
            return compare(buffer1, buffer2, size) > 0;
        }

        [[nodiscard]] static constexpr bool compare_greater(const u8 *buffer1, const u8 *buffer2, const usize size) noexcept
        {
            return compare(buffer1, buffer2, size) > 0;
        }

        template<typename type_t, usize buffer_size>
        [[nodiscard]] static constexpr bool compare_greater(const type_t (&buffer1)[buffer_size], const type_t (&buffer2)[buffer_size]) noexcept
        {
            return compare_greater(buffer1, buffer2, buffer_size);
        }

        /** Reverse bits of a 32 bits integral type */
        [[nodiscard]] static constexpr u32 reverse_bits(u32 integral) noexcept
        {
            integral = (integral << 16) | (integral >> 16);
            integral = ((integral & 0x00ff00ff) << 8) | ((integral & 0xff00ff00) >> 8);
            integral = ((integral & 0x0f0f0f0f) << 4) | ((integral & 0xf0f0f0f0) >> 4);
            integral = ((integral & 0x33333333) << 2) | ((integral & 0xcccccccc) >> 2);
            integral = ((integral & 0x55555555) << 1) | ((integral & 0xaaaaaaaa) >> 1);
            return integral;
        }
    };

} // namespace hud::os::common

#endif // HD_INC_CORE_OS_COMMON_MEMORY_H