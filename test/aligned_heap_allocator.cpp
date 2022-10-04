#include <core/allocators/aligned_heap_allocator.h>

TEST(aligned_heap_allocator, allocate_zero_do_not_allocate)
{
    hud_test::for_each_type<i8, i16, i32, i64, u8, u16, u32, u64, f32, f64, uptr, iptr, usize, isize>()([]<typename type_t>() {
        hud::aligned_heap_allocator<alignof(type_t)> heap_allocator;
        const auto buffer = heap_allocator.template allocate<type_t>(0);
        ASSERT_EQ(buffer.data(), nullptr);
        ASSERT_EQ(buffer.count(), 0u);
    });
}

TEST(aligned_heap_allocator, correctly_allocate_and_free_aligned_requested_amount_of_memory)
{
    hud_test::for_each_type<i8, i16, i32, i64, u8, u16, u32, u64, f32, f64, uptr, iptr, usize, isize>()([]<typename type_t>() {
        hud_test::for_each_value<u32, 1, 2, 4, 8, 16, 32, 64, 128, 256, 1024>()([]<u32 alignement>() {
            for (u32 count = 1; count < hud::i8_max; count++) {
                hud::aligned_heap_allocator<alignement> heap_allocator;
                const auto buffer = heap_allocator.template allocate<type_t>(count);
                ASSERT_NE(buffer.data(), nullptr);
                ASSERT_EQ(buffer.count(), count);
                ASSERT_EQ(buffer.end() - buffer.begin(), static_cast<iptr>(count));
                ASSERT_TRUE(hud::memory::is_pointer_aligned(buffer.data(), alignement));

                // Write in the allocated memory
                // If memory is not allocated, write access violation should happend
                isize write_index = 0;
                for (type_t& value : buffer) {
                    value = static_cast<type_t>(write_index);
                    write_index++;
                }

                // Ensure we correctly write values of index in the correct memory
                for (usize read_index = 0; read_index < buffer.count(); read_index++) {
                    ASSERT_EQ(buffer[read_index], static_cast<type_t>(read_index));
                }

                heap_allocator.free(buffer);
            }
        });
    });
}