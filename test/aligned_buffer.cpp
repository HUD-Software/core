#include <core/containers/aligned_buffer.h>

GTEST_TEST(aligned_buffer, default_constructor_should_aligned_buffer_correctly)
{

    using size = std::integer_sequence<usize, 1, 2, 3, 4, 5, 6, 7, 8, 9>;
    using align = std::integer_sequence<u32, 1, 2, 4, 8, 16, 32>;
    hud_test::for_each_value<size>()([]<usize size>()
                                     { hud_test::for_each_value<align>()([]<u32 alignement>()
                                                                         {
            hud::aligned_buffer<size, alignement> buffer;
            hud_assert_true(hud::memory::is_pointer_aligned(buffer.pointer(), alignement)); }); });
}
