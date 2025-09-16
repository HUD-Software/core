#ifndef HD_INC_CORE_MEMORY_MEMORY_H
#define HD_INC_CORE_MEMORY_MEMORY_H
#include "../assert.h"
#include <stdlib.h> // malloc, free, ...
#include <string.h> // memset
#include <memory>   // std::allocator
#include "../math.h"
#include "../traits/is_constant_evaluated.h"
#include "../traits/is_not_same.h"
#include "../traits/is_pointer.h"
#include "../traits/is_constant_evaluated.h"
#include "../traits/is_constructible.h"
#include "../traits/is_copy_constructible.h"
#include "../traits/is_move_constructible.h"
#include "../traits/is_default_constructible.h"
#include "../traits/is_destructible.h"
#include "../traits/is_copy_assignable.h"
#include "../traits/is_move_assignable.h"
#include "../traits/is_nothrow_constructible.h"
#include "../traits/is_nothrow_copy_constructible.h"
#include "../traits/is_nothrow_move_constructible.h"
#include "../traits/is_nothrow_default_constructible.h"
#include "../traits/is_nothrow_copy_assignable.h"
#include "../traits/is_nothrow_move_assignable.h"
#include "../traits/is_nothrow_destructible.h"
#include "../traits/is_trivially_default_constructible.h"
#include "../traits/is_trivially_destructible.h"
#include "../traits/is_trivially_copy_assignable.h"
#include "../traits/is_bitwise_copy_constructible.h"
#include "../traits/is_bitwise_move_constructible.h"
#include "../traits/is_bitwise_move_constructible.h"
#include "../traits/is_bitwise_copy_assignable.h"
#include "../traits/is_bitwise_move_assignable.h"
#include "../traits/is_bitwise_comparable.h"
#include "../traits/is_comparable_with_equal_operator.h"
#include "../traits/is_comparable_with_not_equal_operator.h"
#include "../templates/move.h"
#include "../templates/forward.h"

#if defined(HD_SSE2)
    #include <emmintrin.h>
#endif

namespace hud
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
        [[nodiscard]] static bool is_pointer_aligned(const type_t *const pointer, const u32 alignment) noexcept
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
            if (size == 0) [[unlikely]] {
                return nullptr;
            }
            void *unaligned_pointer = ::malloc(size);
            if (unaligned_pointer != nullptr) [[unlikely]] {
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
            if (size == 0) [[unlikely]] {
                return nullptr;
            }
            void *unaligned_pointer = ::malloc(size + extra);
            if (unaligned_pointer != nullptr) [[unlikely]] {
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
            if consteval
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
            if (buffer != nullptr) [[likely]] {
                set_memory_zero(buffer, size);
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
            if (unaligned_pointer) [[likely]] {
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
            if consteval
            // LCOV_EXCL_START
            {
                return allocate_array<type_t>(count);
            }
            // LCOV_EXCL_STOP
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
            if (buffer != nullptr) [[likely]] {
                set_memory_zero(buffer, size);
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
            if consteval
            // LCOV_EXCL_START
            {
                if (alloc != nullptr) {
                    // Usage of std::allocator.deallocate is allowed in constexpr dynamic allocation.
                    // The allocation should be freed after std::allocator<type_t>().allocate in the same constexpr expression
                    std::allocator<type_t>().deallocate(alloc, count);
                }
            }
            // LCOV_EXCL_STOP
            else {
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
            if consteval
            // LCOV_EXCL_START
            {
                free_array(pointer, count);
            }
            // LCOV_EXCL_STOP
            else {
                free_align(reinterpret_cast<const void *>(pointer));
            }
        }

        /** Free aligned memory block allocated with allocate_align */
        static HD_FORCEINLINE void free_align(const void *pointer) noexcept
        {
            if (pointer != nullptr) {
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
            if (size == 0) {
                free(pointer);
                return nullptr;
            }
            else {
                return ::realloc(pointer, size);
            }
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
            if (pointer && size) {
                usize old_size = *((usize *)((const uptr)pointer - sizeof(void *) - sizeof(uptr))); // Read the size of the allocation
                if (size == old_size) {
                    return pointer;
                }
                usize size_to_copy = size < old_size ? size : old_size;
                // Realloc alignment mean realloc and copy the data
                void *aligned_pointer = allocate_align(size, alignment);
                // Copy the current data to the new buffer
                copy_memory(aligned_pointer, pointer, size_to_copy);
                free_align(pointer);
                return aligned_pointer;
            }
            else if (!pointer) {
                return allocate_align(size, alignment);
            }
            else {
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
        static void *copy_memory(void *destination, const void *source, const usize size) noexcept
        {
            // The behavior is undefined if either destination or source is an invalid or null pointer.
            check(destination != nullptr);
            check(source != nullptr);

            // The behavior is undefined if the objects overlap
            if (!hud::compilation::is_compilation_mode(hud::compilation_mode::release) && size > 0) [[likely]] {
                check(destination != source);
                const volatile uptr dest_addr = reinterpret_cast<uptr>(destination);
                const volatile uptr src_addr = reinterpret_cast<uptr>(source);
                check(!(dest_addr >= src_addr && dest_addr < src_addr + size));
                check(!(src_addr >= dest_addr && src_addr < dest_addr + size));
            }

            return ::memcpy(destination, source, size);
        }

        /**
         * Copy block of memory. Do not support overlapped buffer. Use move to support overlapped buffer.
         * @tparam type_t An integral
         * @param destination Pointer to the destination array where the content is to be copied
         * @param source Pointer to the source of data to be copied
         * @param size Number of bytes to copy
         * @return destination pointer
         */
        template<typename type_t>
        requires(hud::is_integral_v<type_t>)
        static constexpr type_t *copy_memory(type_t *destination, const type_t *source, const usize size) noexcept
        {
            static_assert(hud::is_bitwise_copy_constructible_v<type_t> && hud::is_bitwise_copy_assignable_v<type_t>, "copy_memory should be called only on type that is bitwise copy constructible and assignable");
            if consteval
            // LCOV_EXCL_START
            {
                for (usize position = 0; position < size; ++position) {
                    destination[position] = source[position];
                }
                return destination;
            }
            // LCOV_EXCL_STOP
            else {
                return static_cast<type_t *>(copy_memory(static_cast<void *>(destination), static_cast<const void *>(source), size));
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
        static constexpr type_t *copy_memory(type_t (&destination)[buffer_size], const type_t (&source)[buffer_size]) noexcept
        {
            return copy_memory(destination, source, buffer_size * sizeof(type_t));
        }

        /**
         * Sets the first size bytes of the block of memory pointed by destination to the specified value.
         * @param destination Pointer to the memory buffer. Must not be null.
         * @param size Number of bytes to set to the specified `value`.
         * @param value The value to be set in the buffer `destination`.
         * @return Pointer to the buffer `destination`.
         */
        static HD_FORCEINLINE void *set_memory(void *destination, const usize size, const u8 value) noexcept
        {
            // The behavior is undefined if destination is a null pointer.
            check(destination != nullptr);
            return memset(destination, value, size);
        }

        /**
         * Sets the first size bytes of the block of memory pointed by destination to the specified value.
         * @param destination Pointer to the memory buffer. Must not be null.
         * @param size Number of u8 to set to the specified `value`.
         * @param value The value to be set in the buffer `destination`.
         * @return Pointer to the buffer `destination`.
         */
        static constexpr u8 *set_memory(u8 *destination, const usize size, const u8 value) noexcept
        {
            if consteval
            // LCOV_EXCL_START
            {
                for (usize position = 0; position < size; ++position) {
                    destination[position] = value;
                }
                return destination;
            }
            // LCOV_EXCL_STOP
            else {
                return static_cast<u8 *>(set_memory(static_cast<void *>(destination), size, value));
            }
        }

        /**
         * Sets the first `size` `type_t` that is not a integral type of the block of memory pointed to by `destination` to the specified `value`.
         * @param destination Pointer to the memory buffer. Must not be null.
         * @param size Number of type_t to set to the specified `value`.
         * @param value The value to be set in the buffer `destination`.
         * @return Pointer to the buffer `destination`.
         */
        template<typename type_t>
        requires(!hud::is_integral_v<type_t>)
        static type_t *set_memory(type_t *destination, const usize size, const u8 value) noexcept
        {
            return static_cast<type_t *>(set_memory(static_cast<void *>(destination), size, value));
        }

        /**
         * Sets the first `size` integral `type_t` of the block of memory pointed by `destination` to the specified `value`.
         * @tparam type_t An integral type of destination
         * @param destination Pointer to the memory buffer. Must not be null.
         * @param size Number of type_t to set to the specified `value`.
         * @param value The value to be set in the buffer `destination`.
         * @return Pointer to the buffer `destination`.
         */
        template<typename type_t>
        requires(hud::is_integral_v<type_t>)
        static constexpr type_t *set_memory(type_t *destination, const usize size, const u8 value) noexcept
        {
            if consteval
            // LCOV_EXCL_START
            {
                for (usize position = 0; position < size / sizeof(type_t); position++) {
                    std::construct_at(destination + position, value);
                }
                return destination;
            }
            // LCOV_EXCL_STOP
            else {
                return static_cast<type_t *>(set_memory(static_cast<void *>(destination), size, value));
            }
        }

        /**
         * Sets the block of memory referenced by buffer to the specified value.
         * @tparam type_t An integral type of `buffer`
         * @tparam buffer_size Size of the `buffer`. Must not be null.
         * @param buffer The buffer memory to set to `value`
         * @param value The value to be set in the `buffer`.
         * @return Pointer to the `buffer`.
         */
        template<typename type_t, usize buffer_size>
        requires(hud::is_integral_v<type_t>)
        static constexpr void *set_memory(type_t (&buffer)[buffer_size], const u8 value) noexcept
        {
            return set_memory(buffer, buffer_size * sizeof(type_t), value);
        }

    private:
        /**
         * Sets the first `size` bytes of the block of memory pointed to by `destination` to the specified `value`.
         * This safe version ensures that the operation is not removed by the compiler due to optimization.
         * `destination` is volatile to prevent memset to be remove by the compiler optimisation
         * @param destination Pointer to the memory buffer. Must not be null.
         * @param size Number of bytes to set to the specified `value`.
         * @param value The value to be set in the buffer `destination`.
         * @return Pointer to the buffer `destination`.
         */
        static HD_FORCEINLINE void *set_memory_safe(void *destination, const usize size, const u8 value) noexcept
        {
            // The behavior is undefined if destination is a null pointer.
            check(destination != nullptr);
            void *(*volatile memset_volatile)(void *, int, size_t) = memset;
            return memset_volatile(destination, value, size);
            // Prevent compiler from removing the memset
            // Add a volatile pointer to the memory and reaffect it to destination
            // This prevent memset to be remove if the user don't use return value
            // volatile unsigned char *p = (volatile unsigned char *)destination;
            // destination = (void *)p;
            // return destination;
        }

    public:
        /**
         * Sets the first `size` bytes of the block of memory pointed to by `destination` to the specified `value`.
         * This safe version ensures that the operation is not removed by the compiler due to optimization.
         * `destination` is volatile to prevent memset to be remove by the compiler optimisation
         * @param destination Pointer to the memory buffer. Must not be null.
         * @param size Number of u8 to set to the specified `value`.
         * @param value The value to be set in the buffer `destination`.
         * @return Pointer to the buffer `destination`.
         */
        static constexpr u8 *set_memory_safe(u8 *destination, const usize size, const u8 value) noexcept
        {
            if consteval
            // LCOV_EXCL_START
            {
                for (usize position = 0; position < size; position++) {
                    destination[position] = value;
                }
                return destination;
            }
            // LCOV_EXCL_STOP
            else {
                return static_cast<u8 *>(set_memory_safe(static_cast<void *>(destination), size, value));
            }
        }

        /**
         * Sets the first `size` `type_t` that is not a integral type of the block of memory pointed to by `destination` to the specified `value`.
         * This safe version ensures that the operation is not removed by the compiler due to optimization.
         * `destination` is volatile to prevent memset to be remove by the compiler optimisation
         * @param destination Pointer to the memory buffer. Must not be null.
         * @param size Number of type_t to set to the specified `value`.
         * @param value The value to be set in the buffer `destination`.
         * @return Pointer to the buffer `destination`.
         */
        template<typename type_t>
        requires(!hud::is_integral_v<type_t>)
        static type_t *set_memory_safe(type_t *destination, const usize size, const u8 value) noexcept
        {
            return static_cast<type_t *>(set_memory_safe(static_cast<void *>(destination), size, value));
        }

        /**
         * Sets the first `size` integral `type_t` of the block of memory pointed to by `destination` to the specified `value`.
         * This safe version ensures that the operation is not removed by the compiler due to optimization.
         * `destination` is volatile to prevent memset to be remove by the compiler optimisation
         * @tparam type_t An integral type of destination
         * @param destination Pointer to the memory buffer. Must not be null.
         * @param size Number of type_t to set to the specified `value`.
         * @param value The value to be set in the buffer `destination`.
         * @return Pointer to the buffer `destination`.
         */
        template<typename type_t>
        requires(hud::is_integral_v<type_t>)
        static constexpr type_t *set_memory_safe(type_t *destination, const usize size, const u8 value) noexcept
        {
            if consteval
            // LCOV_EXCL_START
            {
                for (usize position = 0; position < size / sizeof(type_t); position++) {
                    std::construct_at(destination + position, value);
                }
                return destination;
            }
            // LCOV_EXCL_STOP
            else {
                return static_cast<type_t *>(set_memory_safe(static_cast<void *>(destination), size, value));
            }
        }

        /**
         * Sets the block of memory referenced by `buffer` to the specified `value`.
         * This safe version ensures that the operation is not removed by the compiler due to optimization.
         * `destination` is volatile to prevent memset to be remove by the compiler optimisation
         * @tparam type_t An integral type of `buffer`
         * @tparam buffer_size Size of the `buffer`
         * @param buffer The buffer memory to set to `value`. Must not be null.
         * @param value The value to be set in the `buffer`.
         * @return Pointer to the `buffer`.
         */
        template<typename type_t, usize buffer_size>
        static HD_FORCEINLINE constexpr type_t *set_memory_safe(type_t (&buffer)[buffer_size], const u8 value) noexcept
        {
            return set_memory_safe(buffer, buffer_size * sizeof(type_t), value);
        }

        /**
         * Sets the first `size` bytes of the block of memory pointed by `destination` to zero.
         * @param destination Pointer to the buffer. Must not be null.
         * @param size Number of bytes to set to zero
         * @return Pointer to the buffer `destination`.
         */
        static HD_FORCEINLINE void *set_memory_zero(void *destination, const usize size) noexcept
        {
            return set_memory(destination, size, 0);
        }

        /**
         * Sets the first `size` integral `type_t` of the block of memory pointed by `destination` to zero.
         * @tparam type_t An integral type of destination
         * @param destination Pointer to the buffer. Must not be null.
         * @param size Number of bytes to set to zero
         * @return Pointer to the buffer `destination`.
         */
        template<typename type_t>
        requires(hud::is_integral_v<type_t>)
        static HD_FORCEINLINE constexpr type_t *set_memory_zero(type_t *destination, const usize size) noexcept
        {
            return set_memory(destination, size, 0);
        }

        /**
         * Sets the first `size` pointer to `type_t` of the block of memory pointed to by `destination` to zero.
         * @tparam type_t An pointer type of destination
         * @param destination Pointer to the memory buffer. Must not be null.
         * @param size Number of type_t to set to zero.
         * @return Pointer to the buffer `destination`.
         */
        template<typename type_t>
        requires(hud::is_pointer_v<type_t>)
        static HD_FORCEINLINE constexpr type_t *set_memory_zero(type_t *destination, const usize size) noexcept
        {
            if consteval
            // LCOV_EXCL_START
            {
                for (usize position = 0; position < size / sizeof(type_t); position++) {
                    std::construct_at(destination + position, nullptr);
                }
                return destination;
            }
            // LCOV_EXCL_STOP
            else {
                return static_cast<type_t *>(set_memory_zero(static_cast<void *>(destination), size));
            }
        }

        /**
         * Sets the block of memory referenced by `buffer` to the specified `value`.
         * @tparam type_t An integral type of `buffer`
         * @tparam buffer_size Size of the `buffer`
         * @param buffer The buffer memory to set to `value`. Must not be null.
         * @return Pointer to the `buffer`.
         */
        template<typename type_t, usize buffer_size>
        static HD_FORCEINLINE constexpr type_t *set_memory_zero(type_t (&buffer)[buffer_size]) noexcept
        {
            return static_cast<type_t *>(set_memory_zero(buffer, buffer_size * sizeof(type_t)));
        }

        /**
         * Sets the first `size` bytes of the block of memory pointed by `destination` to zero.
         * This safe version ensures that the operation is not removed by the compiler due to optimization.
         * `destination` is volatile to prevent memset to be remove by the compiler optimisation
         * @param destination Pointer to the buffer. Must not be null.
         * @param size Number of bytes to set to zero
         * @return Pointer to the buffer `destination`.
         */
        static HD_FORCEINLINE constexpr u8 *set_memory_zero_safe(u8 *destination, const usize size) noexcept
        {
            return set_memory_safe(destination, size, 0);
        }

        /**
         * Sets the first `size` bytes of the block of memory pointed to by `destination` where `type_t` is not an integral or pointer type to zero.
         * This safe version ensures that the operation is not removed by the compiler due to optimization.
         * `destination` is volatile to prevent memset to be remove by the compiler optimisation
         * @tparam type_t An integral type of destination
         * @param destination Pointer to the buffer. Must not be null.
         * @param size Number of bytes to set to zero
         * @return Pointer to the buffer `destination`.
         */
        template<typename type_t>
        requires(!hud::is_integral_v<type_t> && !hud::is_pointer_v<type_t> && hud::is_trivially_copyable_v<type_t>)
        static HD_FORCEINLINE type_t *set_memory_zero_safe(type_t *destination, const usize size) noexcept
        {
            return set_memory_safe(destination, size, 0);
        }

        /**
         * Sets the first `size` integral `type_t` of the block of memory pointed to by `destination` to zero.
         * This safe version ensures that the operation is not removed by the compiler due to optimization.
         * `destination` is volatile to prevent memset to be remove by the compiler optimisation
         * @tparam type_t An pointer type of destination
         * @param destination Pointer to the memory buffer. Must not be null.
         * @param size Number of type_t to set to zero.
         * @return Pointer to the buffer `destination`.
         */
        template<typename type_t>
        requires(hud::is_integral_v<type_t>)
        static HD_FORCEINLINE constexpr type_t *set_memory_zero_safe(type_t *destination, const usize size) noexcept
        {
            if consteval
            // LCOV_EXCL_START
            {
                for (usize position = 0; position < size / sizeof(type_t); position++) {
                    std::construct_at(destination + position, 0);
                }
                return destination;
            }
            // LCOV_EXCL_STOP
            else {
                return static_cast<type_t *>(set_memory_safe(destination, size, 0));
            }
        }

        /**
         * Sets the first `size` pointer to `type_t` of the block of memory pointed to by `destination` to zero.
         * This safe version ensures that the operation is not removed by the compiler due to optimization.
         * `destination` is volatile to prevent memset to be remove by the compiler optimisation
         * @tparam type_t An pointer type of destination
         * @param destination Pointer to the memory buffer. Must not be null.
         * @param size Number of type_t to set to zero.
         * @return Pointer to the buffer `destination`.
         */
        template<typename type_t>
        requires(hud::is_pointer_v<type_t>)
        static HD_FORCEINLINE constexpr type_t *set_memory_zero_safe(type_t *destination, const usize size) noexcept
        {
            if consteval
            // LCOV_EXCL_START
            {
                for (usize position = 0; position < size / sizeof(type_t); position++) {
                    std::construct_at(destination + position, nullptr);
                }
                return destination;
            }
            // LCOV_EXCL_STOP
            else {
                return static_cast<type_t *>(set_memory_safe(destination, size, 0));
            }
        }

        /**
         * Sets the block of memory referenced by `buffer` to the specified `value`.
         * This safe version ensures that the operation is not removed by the compiler due to optimization.
         * `destination` is volatile to prevent memset to be remove by the compiler optimisation
         * @tparam type_t An integral type of `buffer`
         * @tparam buffer_size Size of the `buffer`
         * @param buffer The buffer memory to set to `value`. Must not be null.
         * @return Pointer to the `buffer`.
         */
        template<typename type_t, usize buffer_size>
        requires(hud::is_trivially_copyable_v<type_t>)
        static HD_FORCEINLINE constexpr type_t *set_memory_zero_safe(type_t (&buffer)[buffer_size]) noexcept
        {
            return static_cast<type_t *>(set_memory_safe(buffer, buffer_size * sizeof(type_t), 0));
        }

        /**
         * Move block of memory. Support overlapped buffer. Prefers copy if buffer do not overlapped.
         * @param destination Pointer to the destination array where the content is to be copied
         * @param source Pointer to the source of data to be copied
         * @param size Number of bytes to copy
         * @return destination pointer
         */
        static HD_FORCEINLINE void *move_memory(void *destination, const void *source, const usize size) noexcept
        {
            // The behavior is undefined if either destination or source is an invalid or null pointer.
            check(destination != nullptr);
            check(source != nullptr);
            return memmove(destination, source, size);
        }

        /**
         * Move block of memory. Support overlapped buffer. Prefers copy if buffer do not overlapped.
         * @param destination Pointer to the destination array where the content is to be copied
         * @param source Pointer to the source of data to be copied
         * @param size Number of bytes to copy
         * @return destination pointer
         */
        static constexpr HD_FORCEINLINE void *move_memory(u8 *destination, const u8 *source, const usize size) noexcept
        {

            if consteval
            // LCOV_EXCL_START
            {
                u8 *dest = destination;
                if (size == 0) {
                    return destination;
                }
                u8 *tmp = hud::memory::allocate_array<u8>(size);
                // copy src to tmp
                for (usize position = 0; position < size; position++) {
                    std::construct_at(tmp + position, *source);
                    source++;
                }

                // copy tmp to destination
                for (usize position = 0; position < size; position++) {
                    *destination = tmp[position];
                    destination++;
                }

                hud::memory::free_array(tmp, size);
                return dest;
            }
            // LCOV_EXCL_STOP
            else {
                return move_memory(static_cast<void *>(destination), static_cast<const void *>(source), size);
            }
        }

        /**
         * Compares the first size bytes of the block of memory pointed by buffer1 to the first size bytes pointed by buffer2
         * The comparison is done lexicographically
         * @param buffer1 The first buffer
         * @param buffer2 The second buffer
         * @param size Number of bytes to compare
         * @return - Negative value if the first differing byte in buffer1 is less than the corresponding byte in buffer2
         *         - 0 if bytes in buffer1 and buffer2 are equal
         *         - Positive value if the first differing byte in buffer1 is greater than the corresponding byte in buffer2
         */
        [[nodiscard]] static HD_FORCEINLINE i32 compare_memory(const void *buffer1, const void *buffer2, const usize size) noexcept
        {
            // The behavior is undefined if either buffer1 or buffer2 is a null pointer.
            check(buffer1 != nullptr);
            check(buffer2 != nullptr);
            return memcmp(buffer1, buffer2, size);
        }

        /**
         * Compares the first size bytes of the block of memory pointed by buffer1 to the first size bytes pointed by buffer2
         * The comparison is done lexicographically
         * @param buffer1 The first buffer
         * @param buffer2 The second buffer
         * @param size Number of bytes to compare
         * @return - Negative value if the first differing byte in buffer1 is less than the corresponding byte in buffer2
         *         - 0 if bytes in buffer1 and buffer2 are equal
         *         - Positive value if the first differing byte in buffer1 is greater than the corresponding byte in buffer2
         */
        [[nodiscard]] static HD_FORCEINLINE constexpr i32 compare_memory(const u8 *buffer1, const u8 *buffer2, const usize size) noexcept
        {
            if consteval
            // LCOV_EXCL_START
            {
                const u8 *lhs = buffer1;
                const u8 *rhs = buffer2;
                for (usize position = 0; position < size; position++) {
                    i32 diff = *lhs - *rhs;
                    if (diff) {
                        return diff;
                    }
                    lhs++;
                    rhs++;
                }
                return 0;
            }
            // LCOV_EXCL_STOP
            else {
                return compare_memory(static_cast<const void *>(buffer1), static_cast<const void *>(buffer2), size);
            }
        }

        /**
         * Compares bytes of buffer1 to the bytes of buffer2
         * The comparison is done lexicographically
         * @param buffer1 The first buffer
         * @param buffer2 The second buffer
         * @return - Negative value if the first differing byte in buffer1 is less than the corresponding byte in buffer2
         *         - 0 if bytes in buffer1 and buffer2 are equal
         *         - Positive value if the first differing byte in buffer1 is greater than the corresponding byte in buffer2
         */
        template<typename type_t, usize buffer_size>
        [[nodiscard]] static HD_FORCEINLINE constexpr i32 compare_memory(const type_t (&buffer1)[buffer_size], const type_t (&buffer2)[buffer_size]) noexcept
        {
            return compare_memory(buffer1, buffer2, buffer_size);
        }

        /**
         * Compares the first size bytes of the block of memory pointed by buffer1 to the first size bytes pointed by buffer2
         * The comparison is done lexicographically
         * @param buffer1 The first buffer
         * @param buffer2 The second buffer
         * @param size Number of bytes to compare
         * @return true if both buffers are equal, false otherwise
         */
        [[nodiscard]] static HD_FORCEINLINE bool is_memory_compare_equal(const void *buffer1, const void *buffer2, const usize size) noexcept
        {
            return compare_memory(buffer1, buffer2, size) == 0;
        }

        /**
         * Compares the first size bytes of the block of memory pointed by buffer1 to the first size bytes pointed by buffer2
         * The comparison is done lexicographically
         * @param buffer1 The first buffer
         * @param buffer2 The second buffer
         * @param size Number of bytes to compare
         * @return true if both buffers are equal, false otherwise
         */
        [[nodiscard]] static HD_FORCEINLINE constexpr bool is_memory_compare_equal(const u8 *buffer1, const u8 *buffer2, const usize size) noexcept
        {
            return compare_memory(buffer1, buffer2, size) == 0;
        }

        /**
         * Compares bytes of buffer1 to the bytes of buffer2
         * The comparison is done lexicographically
         * @param buffer1 The first buffer
         * @param buffer2 The second buffer
         * @return true if both buffers are equal, false otherwise
         */
        template<typename type_t, usize buffer_size>
        [[nodiscard]] static HD_FORCEINLINE constexpr bool is_memory_compare_equal(const type_t (&buffer1)[buffer_size], const type_t (&buffer2)[buffer_size]) noexcept
        {
            return is_memory_compare_equal(buffer1, buffer2, buffer_size * sizeof(type_t));
        }

        /**
         * Compares the first size bytes of the block of memory pointed by buffer1 to the first size bytes pointed by buffer2
         * The comparison is done lexicographically
         * @param buffer1 The first buffer
         * @param buffer2 The second buffer
         * @param size Number of bytes to compare
         * @return true if buffer1 is less that buffer2, false otherwise
         */
        [[nodiscard]] static HD_FORCEINLINE bool is_memory_compare_less(const void *buffer1, const void *buffer2, const usize size) noexcept
        {
            return compare_memory(buffer1, buffer2, size) < 0;
        }

        /**
         * Compares the first size bytes of the block of memory pointed by buffer1 to the first size bytes pointed by buffer2
         * The comparison is done lexicographically
         * @param buffer1 The first buffer
         * @param buffer2 The second buffer
         * @param size Number of bytes to compare
         * @return true if buffer1 is less that buffer2, false otherwise
         */
        [[nodiscard]] static HD_FORCEINLINE constexpr bool is_memory_compare_less(const u8 *buffer1, const u8 *buffer2, const usize size) noexcept
        {
            return compare_memory(buffer1, buffer2, size) < 0;
        }

        /**
         * Compares bytes of buffer1 to the bytes of buffer2
         * The comparison is done lexicographically
         * @param buffer1 The first buffer
         * @param buffer2 The second buffer
         * @return true if buffer1 is less that buffer2, false otherwise
         */
        template<typename type_t, usize buffer_size>
        [[nodiscard]] static HD_FORCEINLINE constexpr bool is_memory_compare_less(const type_t (&buffer1)[buffer_size], const type_t (&buffer2)[buffer_size]) noexcept
        {
            return is_memory_compare_less(buffer1, buffer2, buffer_size);
        }

        /**
         * Compares the first size bytes of the block of memory pointed by buffer1 to the first size bytes pointed by buffer2
         * The comparison is done lexicographically
         * @param buffer1 The first buffer
         * @param buffer2 The second buffer
         * @param size Number of bytes to compare
         * @return true if buffer1 is greater that buffer2, false otherwise
         */
        [[nodiscard]] static HD_FORCEINLINE bool is_memory_compare_greater(const void *buffer1, const void *buffer2, const usize size) noexcept
        {
            return compare_memory(buffer1, buffer2, size) > 0;
        }

        /**
         * Compares the first size bytes of the block of memory pointed by buffer1 to the first size bytes pointed by buffer2
         * The comparison is done lexicographically
         * @param buffer1 The first buffer
         * @param buffer2 The second buffer
         * @param size Number of bytes to compare
         * @return true if buffer1 is greater that buffer2, false otherwise
         */
        [[nodiscard]] static HD_FORCEINLINE constexpr bool is_memory_compare_greater(const u8 *buffer1, const u8 *buffer2, const usize size) noexcept
        {
            return compare_memory(buffer1, buffer2, size) > 0;
        }

        /**
         * Compares bytes of buffer1 to the bytes of buffer2
         * The comparison is done lexicographically
         * @param buffer1 The first buffer
         * @param buffer2 The second buffer
         * @return true if buffer1 is greater that buffer2, false otherwise
         */
        template<typename type_t, usize buffer_size>
        [[nodiscard]] static HD_FORCEINLINE constexpr bool is_memory_compare_greater(const type_t (&buffer1)[buffer_size], const type_t (&buffer2)[buffer_size]) noexcept
        {
            return is_memory_compare_greater(buffer1, buffer2, buffer_size);
        }

        /** Performs a load of 32 bits into an aligned memory from a unaligned memory */
        [[nodiscard]] static constexpr u32 unaligned_load32(const char8 *buffer) noexcept
        {
            char8 result[sizeof(u32)];
            copy_memory(result, buffer, sizeof(u32));
            return hud::bit_cast<u32>(result);
        }

        /** Performs a load of 32 bits into an aligned memory from a unaligned memory */
        [[nodiscard]] static constexpr u64 unaligned_load64(const char8 *buffer) noexcept
        {
            char8 result[sizeof(u64)];
            copy_memory(result, buffer, sizeof(u64));
            return hud::bit_cast<u64>(result);
        }

        /** Load 64 bits value and return it. */
        [[nodiscard]] static constexpr u64 unaligned_load64(const u8 *buffer) noexcept
        {
            u8 result[sizeof(u64)];
            copy_memory(result, buffer, sizeof(u64));
            return hud::bit_cast<u64>(result);
        }

        /** Load 64 bits value and return it. */
        [[nodiscard]] static constexpr u64 unaligned_load64(const i8 *buffer) noexcept
        {
            i8 result[sizeof(u64)];
            copy_memory(result, buffer, sizeof(u64));
            return hud::bit_cast<u64>(result);
        }

#if defined(HD_SSE2)
        /** Load 128 bits value and return it. */
        [[nodiscard]] static constexpr __m128i unaligned_load128(const i8 *buffer) noexcept
        {
            if consteval {
                i8 result[sizeof(__m128i)];
                copy_memory(result, buffer, sizeof(__m128i));
                return hud::bit_cast<__m128i>(result);
            }
            else {
                return _mm_loadu_si128(reinterpret_cast<const __m128i *>(buffer));
            }
        }
#endif
        /**
         * Call constructor of type.
         * @tparam type_t Type to construct
         * @param address The address of the type type_t to construct
         * @param args Arguments forward to the type_t constructor
         */
        template<typename type_t, typename... args_t>
        requires(hud::is_constructible_v<type_t, args_t...>)
        static constexpr type_t *construct_object_at(type_t *const address, args_t &&...args) noexcept
        {
            static_assert(hud::is_nothrow_constructible_v<type_t, args_t...>, "type_t constructor is throwable.hud::memory::construct_object_at is not designed to allow throwable constructible type");
            return std::construct_at(address, hud::forward<args_t>(args)...);
        }

        /**
         * Call copy constructor of type of u_type_t is copy constructible type to type_t.
         * @tparam type_t Type to copy construct
         * @tparam u_type_t Type to copy
         * @param address The address of the type type_t to copy construct
         * @param other The objet to copy to address
         */
        template<typename type_t, typename u_type_t>
        requires(hud::is_copy_constructible_v<type_t, u_type_t>)
        static constexpr type_t *construct_object_at(type_t *const address, const u_type_t &other) noexcept
        {
            static_assert(hud::is_nothrow_copy_constructible_v<type_t, u_type_t>, "type_t(const u_type_t&) constructor is throwable.hud::memory::construct_object_at is not designed to allow throwable constructible type");
            return std::construct_at(address, other);
        }

        /**
         * Call move constructor of type of u_type_t is move constructible type to type_t.
         * @tparam type_t Type to move construct
         * @tparam u_type_t Type to move
         * @param address The address of the type type_t to move construct
         * @param other The objet to move to address
         */
        template<typename type_t, typename u_type_t>
        requires(hud::is_move_constructible_v<type_t, u_type_t>)
        static constexpr type_t *construct_object_at(type_t *const address, u_type_t &&other) noexcept
        {
            static_assert(hud::is_nothrow_move_constructible_v<type_t, u_type_t>, "type_t(u_type_t&&) constructor is throwable.hud::memory::construct_object_at is not designed to allow throwable constructible type");
            return std::construct_at(address, hud::forward<u_type_t>(other));
        }

        /**
         * Call constructor of first count contiguously stored elements of type type_t.
         * @tparam type_t Type to construct
         * @param address The address of the type type_t to default construct
         * @param count Number of element to construct
         * @param args Arguments forward to the type_t constructor
         */
        template<typename type_t, typename... args_t>
        requires(hud::is_constructible_v<type_t, args_t...>)
        static constexpr void construct_array_at(type_t *HD_RESTRICT begin, const type_t *HD_RESTRICT const end, args_t &&...args) noexcept
        {
            static_assert(hud::is_nothrow_constructible_v<type_t, args_t...>, "type_t constructor is throwable.hud::memory::construct_array_at is not designed to allow throwable constructible type");
            while (begin < end) {
                hud::memory::construct_object_at<type_t, args_t...>(begin++, hud::forward<args_t>(args)...);
            }
        }

        /**
         * Call default constructor of type type_t.
         * @tparam type_t Type to default construct
         * @param address The address of the type type_t to default construct
         */
        template<typename type_t>
        requires(hud::is_default_constructible_v<type_t>)
        static constexpr void default_construct(type_t *address) noexcept
        {
            static_assert(hud::is_nothrow_default_constructible_v<type_t>, "type_t default constructor is throwable.hud::memory::default_construct is not designed to allow throwable default constructible type");
            hud::memory::construct_object_at<type_t>(address);
        }

        /**
        Call default constructor of first count contiguously stored elements of type type_t
        @tparam type_t Type to default construct
        @param address Address of the first element to default construct
        @param count Number of element to construct
        */
        template<typename type_t>
        requires(hud::is_default_constructible_v<type_t>)
        static constexpr void default_construct_array(type_t *HD_RESTRICT begin, type_t *HD_RESTRICT end) noexcept
        {
            static_assert(hud::is_nothrow_default_constructible_v<type_t>, "type_t default constructor is throwable.hud::memory::default_construct is not designed to allow throwable default constructible type");
            if (!hud::is_constant_evaluated() && hud::is_trivially_default_constructible_v<type_t>) {
                hud::memory::set_memory_zero(begin, (end - begin) * sizeof(type_t));
            }
            else {
                hud::memory::construct_array_at<type_t>(begin, end);
            }
        }

        /**
        Call destructor of type type_t ( Only for type with non trivial destructible type )
        @tparam type_t Type to default destroy
        @param address The address of the type type_t to destroy
        */
        template<typename type_t>
        requires(is_destructible_v<type_t> && !hud::is_pointer_v<type_t>)
        static constexpr void destroy_object(type_t *obj) noexcept
        {
            if constexpr (!hud::is_trivially_destructible_v<type_t>) {
                static_assert(hud::is_nothrow_destructible_v<type_t>, "type_t destructor is throwable.hud::memory::destroy_object is not designed to allow throwable destructible type");
                obj->~type_t();
            }
        }

        /**
         * Call destructor of first count contiguously stored elements of type type_t ( Only for type with non trivial destructible type ).
         * Do nothing if address is nullptr.
         * @tparam type_t Type to default destroy
         * @param address Address of the first element to destroy
         * @param count Number of element to destroy
         */
        template<typename type_t>
        requires(is_destructible_v<type_t>)
        static constexpr void destroy_object_array([[maybe_unused]] type_t *address, [[maybe_unused]] usize count) noexcept
        {
            if constexpr (!hud::is_trivially_destructible_v<type_t>) {
                while (count) {
                    hud::memory::destroy_object<type_t>(address);
                    address++;
                    count--;
                }
            }
        }

        /**
         * Call copy constructor of first count contiguously stored elements of type type_t by passing corresponding element in array u_type_t has parameter if the type is not bitwise copy constructible.
         * If the type is bitwise copy constructible just copy dest_source to dest_addr.
         * @tparam type_t Type to construct
         * @tparam u_type_t Type of parameter passed to the constructor
         * @param destination Address of first the element to copy construct
         * @param source Address of fisrt the element to pass to destination
         * @param count Number of element to copy construct
         */
        template<typename type_t, typename u_type_t>
        requires(hud::is_copy_constructible_v<type_t, u_type_t>)
        static constexpr void copy_construct_array(type_t *destination, const u_type_t *source, usize count) noexcept
        {
            static_assert(hud::is_nothrow_copy_constructible_v<type_t, u_type_t>, "type_t(const u_type_t&) copy constructor is throwable.hud::memory::copy_construct_array is not designed to allow throwable copy constructible type");
            if (!hud::is_constant_evaluated() && hud::is_bitwise_copy_constructible_v<type_t, u_type_t> && count > 0u) {
                hud::memory::copy_memory(destination, source, count * sizeof(type_t));
            }
            else {
                while (count) {
                    hud::memory::construct_object_at<type_t, u_type_t>(destination, *source);
                    destination++;
                    source++;
                    count--;
                }
            }
        }

        /**
         * Call move constructor of first count contiguously stored elements of type type_t by passing corresponding element in array u_type_t has parameter if the type is not bitwise move constructible.
         * If the type is bitwise move constructible just move dest_source to dest_addr by callinghud::memory::move
         * Support overlapped destination and source
         * @tparam type_t Type to construct
         * @tparam u_type_t Type of parameter forward to the constructor
         * @param destination Address of the first element to move construct
         * @param source Address of the first element to pass to destination
         * @param count Number of element to copy construct
         */
        template<typename type_t, typename u_type_t>
        requires(hud::is_move_constructible_v<type_t, u_type_t> || hud::is_copy_constructible_v<type_t, u_type_t>)
        static constexpr void move_or_copy_construct_array(type_t *destination, u_type_t *source, usize count) noexcept
        {
            if constexpr (hud::is_move_constructible_v<type_t, u_type_t>) {
                static_assert(hud::is_nothrow_move_constructible_v<type_t, u_type_t>, "type_t(const u_type_t&) constructor is throwable.hud::memory::construct_object_at is not designed to allow throwable constructible type");
            }
            if constexpr (hud::is_copy_constructible_v<type_t, u_type_t>) {
                static_assert(hud::is_nothrow_copy_constructible_v<type_t, u_type_t>, "type_t(u_type_t&&) constructor is throwable.hud::memory::construct_object_at is not designed to allow throwable constructible type");
            }

            if (!hud::is_constant_evaluated() && hud::is_bitwise_move_constructible_v<type_t, u_type_t> && count > 0u) {
                hud::memory::move_memory(destination, source, count * sizeof(type_t));
            }
            else {
                while (count) {
                    hud::memory::construct_object_at<type_t, u_type_t>(destination, hud::move(*source));
                    destination++;
                    source++;
                    count--;
                }
            }
        }

        /**
         * Copy assign of first count contiguously stored elements of type u_type_t to first count contiguously stored elements of type type_t
         * @tparam type_t Type of the copied element
         * @tparam u_type_t Type to copy
         * @param destination Address of the first element to copy assign
         * @param source Address of the first element to copy assign
         * @param count Number of elements to copy assign
         */
        template<typename type_t, typename u_type_t>
        requires(hud::is_copy_assignable_v<type_t, u_type_t>)
        static constexpr void copy_assign_object_array(type_t *destination, const u_type_t *source, usize count) noexcept
        {
            if (hud::is_bitwise_copy_assignable_v<type_t, u_type_t> && !hud::is_constant_evaluated()) {
                hud::memory::copy_memory(destination, source, count * sizeof(type_t));
            }
            else {
                while (count) {
                    static_assert(hud::is_nothrow_copy_assignable_v<type_t, u_type_t>, "type_t operator=(const u_type_t&) copy assign is throwable.hud::memory::copy_assign is not designed to allow throwable copy assignable type");
                    *destination = *source;
                    destination++;
                    source++;
                    count--;
                }
            }
        }

        /**
         * Move assign of first count contiguously stored elements of type u_type_t to first count contiguously stored elements of type type_t
         * Support overlapped destination and source
         * @tparam type_t Type of the moved element
         * @tparam u_type_t Type to move
         * @param destination Address of the first element to move assign
         * @param source Address of the first element to move assign
         * @param count Number of elements to move assign
         */
        template<typename type_t, typename u_type_t>
        requires(hud::is_move_assignable_v<type_t, u_type_t> || hud::is_copy_assignable_v<type_t, u_type_t>)
        static constexpr void move_or_copy_assign_object_array(type_t *destination, u_type_t *source, u_type_t const *const HD_RESTRICT end_source) noexcept
        {
            if constexpr (hud::is_move_assignable_v<type_t, u_type_t>) {
                static_assert(hud::is_nothrow_move_assignable_v<type_t, u_type_t>, "type_t operator=(u_type_t&&) move assign is throwable. hud::memory::move_or_copy_assign_object_array is not designed to allow throwable move assignable type");
            }
            if constexpr (hud::is_copy_assignable_v<type_t, u_type_t>) {
                static_assert(hud::is_nothrow_copy_assignable_v<type_t, u_type_t>, "type_t operator=(const u_type_t&) copy assign is throwable. hud::memory::move_or_copy_assign_object_array is not designed to allow throwable copy assignable type");
            }

            if (hud::is_bitwise_move_assignable_v<type_t, u_type_t> && hud::is_same_size_v<type_t, u_type_t> && !hud::is_constant_evaluated()) {
                hud::memory::move_memory(destination, source, (end_source - source) * sizeof(type_t));
            }
            else {
                while (source < end_source) {
                    *destination = hud::move(*source);
                    destination++;
                    source++;
                }
            }
        }

        /**
         * Move or copy construct an element of type u_type_t to a type type_t
         * The move constructor is called if u_type_t is a move constructible type to type_t, else the copy constructor is called if u_type_t is a copy constructible type to type_t.
         * After the construction, the destructor of source if called
         * @tparam type_t Type of the relocated element
         * @tparam u_type_t Type to relocate
         * @param destination Address of the element type_t
         * @param source Address of the element u_type_t to relocate to destination
         */
        template<typename type_t, typename u_type_t>
        requires(hud::is_move_constructible_v<type_t, u_type_t> || hud::is_copy_constructible_v<type_t, u_type_t>)
        static constexpr void move_or_copy_construct_object_then_destroy(type_t *destination, u_type_t &&source) noexcept
        {
            hud::memory::construct_object_at<type_t, u_type_t>(destination, hud::forward<u_type_t>(source));
            hud::memory::destroy_object<u_type_t>(&source);
        }

        /**
         * Move or copy construct the first count of contiguously stored elements of type u_type_t to the first count of contiguously stored elements of type type_t.
         * The move constructor is called if u_type_t is a move constructible type to type_t, else the copy constructor is called if u_type_t is a copy constructible type to type_t.
         * If type_t is bitwise copy constructible the source is copied by simply copy the source memory to destination with hud::memory::copy.
         * This implies that source and destination are not overlapping.
         * After the construction of all elements, destructor's are called for count elements in the contiguously stored source
         * @tparam type_t Type of the relocated element
         * @tparam u_type_t Type to relocate
         * @param destination Address of the first element type_t
         * @param source Address of the first element u_type_t to relocate to destination
         * @param count Number of element to relocate
         */
        template<typename type_t, typename u_type_t>
        requires((hud::is_move_constructible_v<type_t, u_type_t> || hud::is_copy_constructible_v<type_t, u_type_t>) && is_destructible_v<u_type_t>)
        static constexpr void fast_move_or_copy_construct_object_array_then_destroy(type_t *destination, u_type_t *source, usize count) noexcept
        {
            // If the source is bitwise copyable and bitwise movable to destination then we make a copy instead of a move semantic
            // This performs better that using move semantic that we do a memory move instead
            if (!hud::is_constant_evaluated() && hud::is_bitwise_copy_constructible_v<type_t, u_type_t>) {
                hud::memory::copy_memory(destination, source, count * sizeof(type_t));
            }
            else {
                hud::memory::move_or_copy_construct_array<type_t, u_type_t>(destination, source, count);
            }
            hud::memory::destroy_object_array<u_type_t>(source, count);
        }

        /**
         * Move or copy construct the first count of contiguously stored elements of type u_type_t to the first count of contiguously stored elements of type type_t in backward order
         * The move constructor is called if u_type_t is a move constructible type to type_t, else the copy constructor is called if u_type_t is a copy constructible type to type_t.
         * After the construction of all elements, destructor's are called for count elements in the contiguously stored source
         * This version is slightly faster than relocate_array if element u_type_t is bitwise move constructible to type_t by callinghud::memory::copy instead ofhud::memory::move
         * but this must be use carefully, it do not support overlapped buffer. If you want to support overlapped buffer, use relocate_array instead.
         * @tparam type_t Type of the relocated element
         * @tparam u_type_t Type to relocate
         * @param destination Address of the first element type_t
         * @param source Address of the first element u_type_t to relocate to destination
         * @param count Number of element to relocate
         */
        template<typename type_t, typename u_type_t>
        requires((hud::is_move_constructible_v<type_t, u_type_t> || hud::is_copy_constructible_v<type_t, u_type_t>) && is_destructible_v<u_type_t>)
        static constexpr void move_or_copy_construct_object_array_then_destroy_backward(type_t *destination, u_type_t *source, const usize count) noexcept
        {
            if (!hud::is_constant_evaluated() && hud::is_bitwise_move_constructible_v<type_t, u_type_t>) {
                hud::memory::move_memory(destination, source, count * sizeof(u_type_t));
                hud::memory::destroy_object_array<u_type_t>(source, count);
            }
            else {
                type_t *last_destination = destination + count;
                u_type_t *last_source = source + count;
                while (last_source > source) {
                    last_destination--;
                    last_source--;
                    hud::memory::move_or_copy_construct_object_then_destroy<type_t, u_type_t>(last_destination, hud::move(*last_source));
                }
            }
        }

        /**
         * Compare an element of type lhs_t with an element of type rhs_t
         * @tparam lhs_t Type of the left element to compare
         * @tparam rhs_t Type of the right element to compare
         * @param left Address of the left element to compare
         * @param right Address of the right element to compare
         * @return true if both elements are the equal, false otherwise
         */
        template<typename lhs_t, typename rhs_t>
        static HD_FORCEINLINE bool is_object_equal(const lhs_t *left, const rhs_t *right) noexcept
        {
            static_assert(hud::is_comparable_with_equal_operator_v<lhs_t, rhs_t>, "Types lhs_t and rhs_t are not comparable");
            return *left == *right;
        }

        /**
         * Compare first count contiguously stored elements of type lhs_t with first count contiguously stored elements of type rhs_t
         * @tparam lhs_t Type of the left element to compare
         * @tparam rhs_t Type of the right element to compare
         * @param left Address of the first left element to compare
         * @param right Address of the first right element to compare
         * @param count Number of element to compare
         * @return true if all elements are the equal, false otherwise
         */
        template<typename lhs_t, typename rhs_t>
        static HD_FORCEINLINE bool is_object_array_equal(const lhs_t *left, const rhs_t *right, usize count) noexcept
        {
            if constexpr (hud::is_bitwise_comparable_v<lhs_t, rhs_t> && hud::is_same_size_v<lhs_t, rhs_t>) {
                return hud::memory::is_memory_compare_equal(left, right, count * sizeof(lhs_t));
            }
            else {
                static_assert(hud::is_comparable_with_equal_operator_v<lhs_t, rhs_t>, "Types lhs_t and rhs_t are not comparable");

                while (count) {
                    if (!hud::memory::is_object_equal<lhs_t, rhs_t>(left, right)) {
                        return false;
                    }
                    left++;
                    right++;
                    count--;
                }
                return true;
            }
        }

        /**
         * Compare an element of type lhs_t with an element of type rhs_t
         * @tparam lhs_t Type of the left element to compare
         * @tparam rhs_t Type of the right element to compare
         * @param left Address of the left element to compare
         * @param right Address of the right element to compare
         * @return true if elements are not the equal, false otherwise
         */
        template<typename lhs_t, typename rhs_t>
        static HD_FORCEINLINE bool is_object_not_equal(const lhs_t *left, const rhs_t *right) noexcept
        {
            static_assert(hud::is_comparable_with_not_equal_operator_v<lhs_t, rhs_t>, "Types lhs_t and rhs_t are not comparable");
            return *left != *right;
        }

        /**
         * Compare first count contiguously stored elements of type lhs_t with first count contiguously stored elements of type rhs_t
         * @tparam lhs_t Type of the left element to compare
         * @tparam rhs_t Type of the right element to compare
         * @param left Address of the first left element to compare
         * @param right Address of the first right element to compare
         * @param count Number of element to compare
         * @return true if at least one element is not equal, false otherwise
         */
        template<typename lhs_t, typename rhs_t>
        static HD_FORCEINLINE bool is_object_array_not_equal(const lhs_t *left, const rhs_t *right, usize count) noexcept
        {
            if constexpr (hud::is_bitwise_comparable_v<lhs_t, rhs_t> && hud::is_same_size_v<lhs_t, rhs_t>) {
                return !hud::memory::is_memory_compare_equal(left, right, count * sizeof(lhs_t));
            }
            else {
                static_assert(hud::is_comparable_with_not_equal_operator_v<lhs_t, rhs_t>, "Types lhs_t and rhs_t are not comparable");

                while (count) {
                    if (hud::memory::is_object_not_equal<lhs_t, rhs_t>(left, right)) {
                        return true;
                    }
                    left++;
                    right++;
                    count--;
                }
                return false;
            }
        }
    };

} // namespace hud

#endif // HD_INC_CORE_MEMORY_MEMORY_H