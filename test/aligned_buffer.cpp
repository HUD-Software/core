#include <core/containers/aligned_buffer.h>

GTEST_TEST(aligned_buffer, default_constructor_should_aligned_buffer_correctly)
{

    auto for_each_size = []<usize size>() {
        hud_test::for_each_value(std::integer_sequence<u32, 1, 2, 4, 8, 16, 32>(), []<u32 alignement>() {
                                     hud::aligned_buffer<size, alignement> buffer;
                                     hud_assert_true(hud::memory::is_pointer_aligned(buffer.pointer(), alignement)); });
    };
    hud_test::for_each_value(std::make_index_sequence<10>(), for_each_size);
}
