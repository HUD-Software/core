#include <core/allocators/aligned_heap_allocator.h>

GTEST_TEST(aligned_heap_allocator, allocate_zero_do_not_allocate)
{
    hud_test::for_each_type<i8, i16, i32, i64, u8, u16, u32, u64, f32, f64, uptr, iptr, usize, isize>()([]<typename type_t>() {
        hud::aligned_heap_allocator<alignof(type_t)> heap_allocator;
        const auto buffer = heap_allocator.template allocate<type_t>(0);
        hud_assert_eq(buffer.data(), nullptr);
        hud_assert_eq(buffer.count(), 0u); });
}

GTEST_TEST(aligned_heap_allocator, correctly_allocate_and_free_aligned_requested_amount_of_memory)
{
    auto lambda_for_each_type = []<typename type_t>() {
        hud_test::for_each_value(std::integer_sequence<u32, 1, 2, 4, 8, 16, 32, 64, 128, 256, 1024> {}, []<u32 alignement>() {
            for (u32 count = 1; count < hud::i8_max; count++) {
                hud::aligned_heap_allocator<alignement> heap_allocator;
                auto buffer = heap_allocator.template allocate<type_t>(count);
                hud_assert_ne(buffer.data(), nullptr);
                hud_assert_eq(buffer.count(), count);
                hud_assert_eq(buffer.end() - buffer.begin(), static_cast<iptr>(count));
                hud_assert_true(hud::memory::is_pointer_aligned(buffer.data(), alignement));

                // Write in the allocated memory
                // If memory is not allocated, write access violation should happend
                isize write_index = 0;
                for (type_t& value : buffer) {
                    value = static_cast<type_t>(write_index);
                    write_index++;
                }

                // Ensure we correctly write values of index in the correct memory
                for (usize read_index = 0; read_index < buffer.count(); read_index++) {
                    hud_assert_eq(buffer[read_index], static_cast<type_t>(read_index));
                }

                heap_allocator.free(buffer);
            } });
    };
    hud_test::for_each_type<i8, i16, i32, i64, u8, u16, u32, u64, f32, f64, uptr, iptr, usize, isize>()(lambda_for_each_type);
}