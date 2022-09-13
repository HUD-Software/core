#pragma once
#ifndef HD_INC_OSLAYER_OS_COMMON_MEMORY_H
#define HD_INC_OSLAYER_OS_COMMON_MEMORY_H
#include "../Assert.h"
#include <stdlib.h> // malloc, free, ...
#include <string.h> // memset
#include "../Math.h"
#include "../Traits/IsConstantEvaluated.h"
#include "../Traits/IsNotSame.h"

namespace hud::OS::Common{

    struct Memory {

        /**
        * Aligned allocation header size.
        * Aligned pointer memory follow this pattern : AlignmentPadding|AllocationSize|UnalignPointer|AlignPointer|UserData.
        */
        static constexpr const usize ALIGNED_MALLOC_HEADER_SIZE = sizeof(void*) + sizeof(uptr);

        /**
        * Align a given address.
        * @param address The address to align
        * @param alignment The alignment to set
        * @return The aligned address
        */
        [[nodiscard]]
        static constexpr uptr align_address(const uptr address, const u32 alignment) noexcept {
            return address + (alignment - 1) & ~uptr(alignment - 1);
        }

        /**
        * Check if address is aligned.
        * @param address The address to check
        * @param alignment Alignment in bytes
        * @return true if the address is aligned to the given alignment, false otherwise
        */
        [[nodiscard]]
        static constexpr bool is_address_aligned(const uptr address, const u32 alignment) noexcept {
            return (address & (alignment - 1)) == 0;
        }

        /**
        * Align the given pointer and add header before the align pointer.
        * This assume allocation is big enough. ( Size + Alignment + sizeof(size) + sizeof(uptr) )
        * Memory follow this pattern : AlignmentPadding|AllocationSize|UnalignPointer|AlignPointer
        * @param unaligned_pointer The unaligned pointer to align
        * @param size The size of the allocation of the unaligned_pointer
        * @param alignment Alignment in bytes
        * @return Aligned pointer with header information before the returned pointer, nullptr otherwise
        */
        [[nodiscard]]
        static HD_FORCEINLINE void* align_pointer(void* const unaligned_pointer, const usize size, const u32 alignment) noexcept {
            const uptr unaligned_address = (const uptr)unaligned_pointer;
            const uptr aligned_pointer = align_address(unaligned_address + ALIGNED_MALLOC_HEADER_SIZE, alignment);

            *((void**)(aligned_pointer - sizeof(uptr))) = unaligned_pointer; // Write unaligned pointer address
            *((usize*)(aligned_pointer - sizeof(uptr) - sizeof(usize))) = size; // Write size of the allocation

            return reinterpret_cast<void*>(aligned_pointer);
        }

        /**
        * Check if a pointer is aligned.
        * @param pointer The pointer to test
        * @param alignment Alignment in bytes
        * @return true if the pointer is aligned, false otherwise
        */
        template<typename T>
        [[nodiscard]]
        static constexpr bool is_pointer_aligned(const T* const pointer, const u32 alignment) noexcept {
            return is_address_aligned(reinterpret_cast<const uptr>(pointer), alignment);
        }

        /**
        * Retrieves the unaligned pointer from an aligned pointer.
        * @param aligned_pointer The aligned pointer of the allocation
        * @return The unaligned pointer
        */
        [[nodiscard]]
        static HD_FORCEINLINE void* get_unaligned_pointer(const void* const aligned_pointer) noexcept {
            return *(void**)((const uptr)aligned_pointer - sizeof(uptr));
        }

        /**
        * Allocate memory block with no alignment requirements. Must be freed with hud::Memory::free.
        * @param size Number of bytes to allocate
        * @return Pointer to the allocated memory block, nullptr if failed
        */
        [[nodiscard]]
        static void* allocate(const usize size) noexcept {
            check(size != 0);
            return ::malloc(size);
        }

        /**
        * Allocate memory block of a defined type T with no alignment requirements. Must be freed with hud::Memory::free.
        * Never used if T is void because it's constexpr version of allocate.
        * Indeed, using void* return type can implies a cast that is not allowed in constexpr.
        * Sometimes, compiler can use this fonction in non constant evaluated expression because T is not void or provided by the user-defined code.
        * In this case, we forward the pointer to allocate and reinterpret_cast to T* the returned pointer.
        * This enforce the choise of std::allocate<T>.allocate() ONLY in constant evaluated expression because reinterpret_cast to T* will result in a compilation error in constant evaluated expression.
        * @tparam T The type to allocate
        * @param count Number of T to allocate
        * @return Pointer to the allocated memory block, nullptr if failed
        */
        template<typename T>
        [[nodiscard]]
        static constexpr T* allocate_array(const usize count) noexcept requires(IsNotSameV<T, void>) {
            const usize allocation_size = count * sizeof(T);
            if (is_constant_evaluated()) {
                // Usage of std::allocator.allocate is allowed in constexpr dynamic allocation. 
                // The allocation should be freed with std::allocator<T>().deallocate in the same constexpr expression
                return std::allocator<T>().allocate(count);
            }
            return reinterpret_cast<T*>(allocate(allocation_size));
        }

        /**
        * Allocate memory block with no alignment requirements and fille the buffer with 0. Must be freed with hud::Memory::free
        * @param size Number of bytes to allocate
        * @return Pointer to the allocated memory block, nullptr if failed
        */
        [[nodiscard]]
        static HD_FORCEINLINE void* allocate_zero(const usize size) noexcept {
            void* buffer = allocate(size);
            if (buffer != nullptr) {
                set_zero(buffer, size);
            }
            return buffer;
        }

        /**
        * Allocate aligned memory block. Must be freed with hud::Memory::free_align
        * @param size Number of bytes to allocate
        * @param alignment Required alignment in bytes
        * @return Pointer to the aligned allocated memory block, nullptr if failed
        */
        [[nodiscard]]
        static void* allocate_align(const usize size, const u32 alignment) noexcept {
            check(size != 0);
            check(size + alignment + ALIGNED_MALLOC_HEADER_SIZE >= size);
            void* aligned_pointer = nullptr;
            void* unaligned_pointer = allocate(size + alignment + ALIGNED_MALLOC_HEADER_SIZE);
            if (unaligned_pointer == nullptr) {
                /*assert(false);*/ // Out of memory
            }
            else {
                aligned_pointer = align_pointer(unaligned_pointer, size, alignment);
            }
            return aligned_pointer;
        }

        /**
        * Allocate aligned memory block in a constant evaluated expression.
        * In constant evaluated expression, this do only a simple allocation without alignment
        * To be used in a constant expression the type type of the allocation must be provided, 
        * void* is not usable in constant evaluated expression.
        * @param count Number of T to allocate
        * @param alignment Required alignment in bytes. Used only in non constant evaluated expression
        * @return Pointer to the aligned allocated memory block, nullptr if failed
        */
        template<typename T>
        static constexpr T* allocate_align(const usize count, const u32 alignment) noexcept {
            if (is_constant_evaluated()) {
                return allocate_array<T>(count);
            }
            return reinterpret_cast<T*>(allocate_align(count * sizeof(T), alignment));
        }

        /**
        * Allocate aligned memory block and fille the buffer with 0. Must be freed with hud::Memory::free_align
        * @param size Number of bytes to allocate
        * @param alignment Alignment in bytes
        * @return Pointer to the allocated memory block, nullptr if failed
        */
        [[nodiscard]]
        static HD_FORCEINLINE void* allocate_align_zero(const usize size, const u32 alignment) noexcept {
            void* buffer = allocate_align(size, alignment);
            if (buffer != nullptr) {
                set_zero(buffer, size);
            }
            return buffer;
        }

        /** Free memory block with no alignment requirements allocated with allocate */
        static HD_FORCEINLINE void free(const void* pointer) noexcept {
            ::free((void*)pointer);
        }

        /**
        * Free memory block of a defined type T.
        * Never used if T is void or not trivially destructible because it's constexpr version of free.
        * Indeed, using void* can implies a cast that is not allowed in constexpr.
        * Sometimes, compiler can use this fonction in non constant evaluated expression because T is not void.
        * In this case, we forward the pointer to free and reinterpret_cast to void* the given pointer.
        * This enforce the choise of std::allocator<T>::deallocate() ONLY in constant evaluated expression because reinterpret_cast to
        * void* will result in a compilation error in constant evaluated expression.
        * @tparam T The type to deallocate. Do not destroy the type.
        * @param alloc The allocation to free. Must be allocated with the constexpr allocate function in a constexpr expression.
        * @param count Number of T to deallocate
        */
        template<typename T>
        static constexpr void free_array(T* alloc, const usize count) noexcept {
            if (is_constant_evaluated()) {
                if (alloc != nullptr) {
                    // Usage of std::allocator.deallocate is allowed in constexpr dynamic allocation. 
                    // The allocation should be freed after std::allocator<T>().allocate in the same constexpr expression
                    std::allocator<T>().deallocate(alloc, count);
                }
            }
            else {
                free(reinterpret_cast<void*>(alloc));
            }
        }

        /**
        * Free memory block of a defined type T in a constant expression
        * Never used if T is void or not trivially destructible because it's constexpr version of free.
        * Indeed, using void* can implies a cast that is not allowed in constexpr.
        * Sometimes, compiler can use this fonction in non constant evaluated expression because T is not void.
        * In this case, we forward the pointer to free and reinterpret_cast to void* the given pointer.
        * This enforce the choise of delete[] ONLY in constant evaluated expression because reinterpret_cast to
        * void* will result in a compilation error in constant evaluated expression.
        * @tparam T The type to allocate. Do not construct the type.
        * @param alloc The allocation to free. Must be allocated with the constexpr allocate function in a constexpr expression
        * @param count Number of T to allocate
        */
        template<typename T>
        static constexpr void free_align(T* pointer, const usize count) noexcept {
            if (is_constant_evaluated()) {
                free_array(pointer, count);
            }
            else {
                free_align(reinterpret_cast<const void*>(pointer));
            }
        }

        /** Free aligned memory block allocated with allocate_align */
        static HD_FORCEINLINE void free_align(const void* pointer) noexcept {
            if (pointer != nullptr) {
                free(get_unaligned_pointer(pointer));
            }
        }

        /**
        * Reallocate a memory block with no alignment requrements allocated with allocate.
        * If pointer is nullptr, the behaviour is the same has allocate.
        * If size is 0, the behaviour is the same has hud::Memory::free and return value is nullptr.
        * After a reallocation, pointer is invalidated.
        * @param pointer Pointer to memory block to reallocate
        * @param size Number of bytes to reallocate
        * @return Pointer to the re-allocated memory block or nullptr
        */
        [[nodiscard]]
        static HD_FORCEINLINE void* reallocate(void* pointer, const usize size) noexcept {
            return ::realloc(pointer, size);
        }

        /**
        * Reallocate an aligned memory block allocated with allocate_align.
        * If pointer is nullptr, the behaviour is the same has allocate_align.
        * If size is 0, the behaviour is the same has hud::Memory::free_align and return value is nullptr.
        * After a reallocation, pointer is invalidated.
        * @param pointer Pointer to memory block to reallocate
        * @param size Number of bytes to reallocate
        * @param alignment Alignment in bytes
        * @return Pointer to the re-allocated memory block or nullptr
        */
        [[nodiscard]]
        static void* reallocate_align(void* pointer, const usize size, const u32 alignment) noexcept {
            if (pointer && size) {
                //Realloc alignment mean realloc and copy the data
                void* aligned_pointer = allocate_align(size, alignment);
                usize old_size = *((usize*)((const uptr)pointer - sizeof(void*) - sizeof(uptr))); // Read the size of the allocation
                usize size_to_copy = size <= old_size ? size : old_size;
                //Copy the current data to the new buffer
                copy(aligned_pointer, pointer, size_to_copy);
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
        static void* copy(void* destination, const void* source, const usize size) noexcept {
            // Check that buffers are not overlapping
            if (size > 0 ) {
                check(destination != source);
                const volatile uptr dest_addr = reinterpret_cast<uptr>(destination);
                const volatile uptr src_addr = reinterpret_cast<uptr>(source);
                check(!(dest_addr >= src_addr && dest_addr < src_addr+size));
                check(!(src_addr >= dest_addr && src_addr < dest_addr+size));
            }
            
            return memcpy(destination, source, size);
        }
        template<typename T>
        static constexpr T* copy(T* destination, const T* source, const usize size) noexcept requires(sizeof(T) == 1)  {
            if (is_constant_evaluated()) {
                T* dest = destination;
                for (usize position = 0; position < size; position++) {
                    std::construct_at(destination, *source);
                    destination++;
                    source++;
                }
                return dest;
            }
            else {
                return static_cast<T*>(copy(static_cast<void*>(destination), static_cast<const void*>(source), size));
            }
        }
        /**
        * Copy block of memory. Do not support overlapped buffer. Use move to support overlapped buffer.
        * @param destination Pointer to the destination array where the content is to be copied
        * @param source Pointer to the source of data to be copied
        * @param size Number of bytes to copy
        * @return destination pointer
        */
        template<typename T, usize buffer_size>
        static constexpr T* copy(T(&destination)[buffer_size], const T(&source)[buffer_size]) noexcept {
            return copy(destination, source, buffer_size * sizeof(T));
        }

        /**
        * Sets the first size bytes of the block of memory pointed by destination to the specified value.
        * @param destination Pointer to the buffer
        * @param value The value to be set
        * @param size Number of bytes to set to the value
        */
        static HD_FORCEINLINE void set(void* destination, const usize size, const u8 value) noexcept {
            memset(destination, value, size);
        }
        static constexpr void set(u8* destination, const usize size, const u8 value) noexcept {
            if (is_constant_evaluated()) {
                for (usize position = 0; position < size; position++) {
                    std::construct_at(destination + position, value);
                }
            } 
            else {
                set(static_cast<void*>(destination), size, value);
            }
        }
        template<typename T>
        static constexpr void set(T* destination, const usize size, const u8 value) noexcept requires(IsIntegralV<T> ) {
            if (is_constant_evaluated()) {
                for (usize position = 0; position < size/sizeof(T); position++) {
                    std::construct_at(destination + position, value);
                }
            }
            else {
                set(static_cast<void*>(destination), size, value);
            }
        }

        /**
        * Sets the block of memory referenced by buffer to the specified value.
        * @param buffer The buffer memory to set to value
        * @param value The value to be set
        */
        template<typename T, usize buffer_size>
        static constexpr void set(T(&buffer)[buffer_size], const u8 value) noexcept {
            set(buffer, buffer_size * sizeof(T), value);
        }

        /**
        * Sets the first size bytes of the block of memory pointed by destination to zero.
        * @param destination Pointer to the buffer
        * @param size Number of bytes to set to zero
        */
        static HD_FORCEINLINE void set_zero(void* destination, const usize size) noexcept {
            set(destination, size, 0);
        }
        static constexpr void set_zero(u8* destination, const usize size) noexcept {
            set(destination, size, 0);
        }
        template<typename T>
        static constexpr void set_zero(T* destination, const usize size) noexcept requires(IsIntegralV<T>){
            set(destination, size, 0);
        }
        template<typename T>
        static constexpr void set_zero(T* destination, const usize size) noexcept requires(IsPointerV<T>) {
            if (is_constant_evaluated()) {
                for (usize position = 0; position < size / sizeof(T); position++) {
                    std::construct_at(destination + position, nullptr);
                }
            }
            else {
                set(static_cast<void*>(destination), size, 0);
            }
        }

        /**
        * Set a block of memory referenced by buffer to zero.
        * @param buffer The buffer memory to set to zero
        */
        template<typename T, usize buffer_size>
        static constexpr void set_zero(T(&buffer)[buffer_size]) noexcept {
            set_zero(buffer, buffer_size * sizeof(T));
        }

        /**
        * Move block of memory. Support overlapped buffer. Prefers copy if buffer do not overlapped.
        * @param destination Pointer to the destination array where the content is to be copied
        * @param source Pointer to the source of data to be copied
        * @param size Number of bytes to copy
        * @return destination pointer
        */
        static HD_FORCEINLINE void* move(void* destination, const void* source, const usize size) noexcept {
            return memmove(destination, source, size);
        }
        static constexpr void* move(u8* destination, const u8* source, const usize size) noexcept {
            if (is_constant_evaluated()) {
                u8* dest = destination;
                if (size == 0) {
                    return destination;
                }
                u8* tmp = Memory::allocate_array<u8>(size);
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

                Memory::free_array(tmp, size);
                return dest;
            }
            else {
                return move(static_cast<void*>(destination), static_cast<const void*>(source), size);
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
        [[nodiscard]]
        static HD_FORCEINLINE i32 compare(const void* buffer1, const void* buffer2, const usize size) noexcept {
            return memcmp(buffer1, buffer2, size);
        }
        [[nodiscard]]
        static constexpr i32 compare(const u8* buffer1, const u8* buffer2, const usize size) noexcept {
            if (is_constant_evaluated()) {
                const u8* lhs = buffer1;
                const u8* rhs = buffer2;
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
            else {
                return compare(static_cast<const void*>(buffer1), static_cast<const void*>(buffer2), size);
            }
        }
        template<typename T, usize buffer_size>
        [[nodiscard]]
        static constexpr i32 compare(const T(&buffer1)[buffer_size], const T(&buffer2)[buffer_size]) noexcept {
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
        [[nodiscard]]
        static HD_FORCEINLINE bool compare_equal(const void* buffer1, const void* buffer2, const usize size) noexcept {
            return compare(buffer1, buffer2, size) == 0;
        }
        [[nodiscard]]
        static constexpr bool compare_equal(const u8* buffer1, const u8* buffer2, const usize size) noexcept {
            return compare(buffer1, buffer2, size) == 0;
        }
        template<typename T, usize buffer_size>
        [[nodiscard]]
        static constexpr bool compare_equal(const T(&buffer1)[buffer_size], const T(&buffer2)[buffer_size]) noexcept {
            return compare_equal(buffer1, buffer2, buffer_size * sizeof(T));
        }

        /**
        * Compares the first size bytes of the block of memory pointed by buffer1 to the first size bytes pointed by buffer2
        * The comparison is done lexicographically
        * @param buffer1 The first buffer
        * @param buffer2 The second buffer
        * @param size Number of bytes to compare
        * @return true if buffer1 is less that buffer2, false otherwise
        */
        [[nodiscard]]
        static HD_FORCEINLINE bool compare_less(const void* buffer1, const void* buffer2, const usize size) noexcept {
            return compare(buffer1, buffer2, size) < 0;
        }
        [[nodiscard]]
        static constexpr bool compare_less(const u8* buffer1, const u8* buffer2, const usize size) noexcept {
            return compare(buffer1, buffer2, size) < 0;
        }
        template<typename T, usize buffer_size>
        [[nodiscard]]
        static constexpr bool compare_less(const T(&buffer1)[buffer_size], const T(&buffer2)[buffer_size]) noexcept {
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
        [[nodiscard]]
        static HD_FORCEINLINE bool compare_greater(const void* buffer1, const void* buffer2, const usize size) noexcept {
            return compare(buffer1, buffer2, size) > 0;
        }
        [[nodiscard]]
        static constexpr bool compare_greater(const u8* buffer1, const u8* buffer2, const usize size) noexcept {
            return compare(buffer1, buffer2, size) > 0;
        }
        template<typename T, usize buffer_size>
        [[nodiscard]]
        static constexpr bool compare_greater(const T(&buffer1)[buffer_size], const T(&buffer2)[buffer_size]) noexcept {
            return compare_greater(buffer1, buffer2, buffer_size);
        }

        /** Reverse bits of a 32 bits integral type */
        [[nodiscard]]
        static constexpr u32 reverse_bits(u32 integral) noexcept {
            integral = (integral << 16) | (integral >> 16);
            integral = ((integral & 0x00ff00ff) << 8) | ((integral & 0xff00ff00) >> 8);
            integral = ((integral & 0x0f0f0f0f) << 4) | ((integral & 0xf0f0f0f0) >> 4);
            integral = ((integral & 0x33333333) << 2) | ((integral & 0xcccccccc) >> 2);
            integral = ((integral & 0x55555555) << 1) | ((integral & 0xaaaaaaaa) >> 1);
            return integral;
        }
    };

} // namespace hud

#endif // HD_INC_OSLAYER_OS_COMMON_MEMORY_H