#include <core/containers/aligned_buffer.h>

TEST(AlignedBuffer, default_constructor_should_aligned_buffer_correctly)
{

    using size = std::integer_sequence<int, 1, 2, 3, 4, 5, 6, 7, 8, 9>;
    using align = std::integer_sequence<int, 1, 2, 4, 8, 16, 32, 64, 128>;
    hud::test::for_each_value<size>()([]<int size>() {
        hud::test::for_each_value<align>()([]<int alignement>() {
            AlignedBuffer<size, alignement> buffer;
            ASSERT_TRUE(hud::memory::is_pointer_aligned(buffer.pointer(), alignement));
        });
    });
}