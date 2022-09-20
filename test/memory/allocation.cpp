
#include <core/memory.h>
#include "../misc/leak_guard.h"

TEST(Memory, allocate)
{


    // Non constant
    {

        // Allocation should success and allocate a u32 on the heap
        constexpr usize allocation_size = sizeof(u32) * 2;
        void* ptr = hud::memory::allocate(allocation_size);
        hud::test::LeakGuard leak_guard(ptr);
        ASSERT_NE(ptr, nullptr);

        // Allocation should failed (usize_max is too big)  and allocate should return nullptr
        void* ptr_2 = hud::memory::allocate(usize_max);
        hud::test::LeakGuard leak_guard_2(ptr_2);
        ASSERT_EQ(ptr_2, nullptr);
    }

}

TEST(Memory, allocate_and_free_are_usable_in_constexpr)
{


    constexpr bool allocate_const = []() -> bool {
        i32* const_ptr = hud::memory::allocate_array<i32>(2);
        bool allocated = const_ptr != nullptr;
        hud::memory::free_array(const_ptr, 2);
        return allocated;
    }();
    ASSERT_TRUE(allocate_const);
}

TEST(Memory, allocate_zero)
{


    // Allocation should success and allocate a 2*u32 on the heap and fill it with 0
    constexpr usize allocation_size = sizeof(u32) * 2;
    void* ptr = hud::memory::allocate_zero(allocation_size);
    hud::test::LeakGuard leak_guard(ptr);
    ASSERT_NE(ptr, nullptr);
    ASSERT_EQ(*static_cast<u32*>(ptr), 0u);
    ASSERT_EQ(*(static_cast<u32*>(ptr) + 1), 0u);

    // Allocation should failed (usize_max is too big)  and allocate should return nullptr
    void* ptr_2 = hud::memory::allocate_zero(usize_max);
    hud::test::LeakGuard leak_guard_2(ptr_2);
    ASSERT_EQ(ptr_2, nullptr);
}

TEST(Memory, allocate_align)
{


    for (u32 aligment = 1; aligment <= 256; aligment <<= 1) {

        // Allocation should success and allocate a u32 on the heap
        void* ptr = hud::memory::allocate_align(sizeof(u32) * 2, aligment);
        hud::test::AlignLeakGuard leak_guard(ptr);
        ASSERT_NE(ptr, nullptr);
        ASSERT_TRUE(hud::memory::is_pointer_aligned(ptr, aligment));

        // Allocation should failed (usize_max is too big)  and allocate should return nullptr
        void* ptr_2 = hud::memory::allocate_align(usize_max - (hud::memory::ALIGNED_MALLOC_HEADER_SIZE + aligment), aligment);
        hud::test::AlignLeakGuard leak_guard_2(ptr_2);
        ASSERT_EQ(ptr_2, nullptr);
    }
}

TEST(Memory, allocate_align_zero)
{


    for (u32 aligment = 1; aligment <= 256; aligment <<= 1) {

        // Allocation should success and allocate a u32 on the heap
        void* ptr = hud::memory::allocate_align_zero(sizeof(u32) * 2, aligment);
        hud::test::AlignLeakGuard leak_guard(ptr);
        ASSERT_NE(ptr, nullptr);
        ASSERT_TRUE(hud::memory::is_pointer_aligned(ptr, aligment));
        ASSERT_EQ(*static_cast<u32*>(ptr), 0u);
        ASSERT_EQ(*(static_cast<u32*>(ptr) + 1), 0u);

        // Allocation should failed (usize_max is too big)  and allocate should return nullptr
        void* ptr_2 = hud::memory::allocate_align_zero(usize_max - (hud::memory::ALIGNED_MALLOC_HEADER_SIZE + aligment), aligment);
        hud::test::AlignLeakGuard leak_guard_2(ptr_2);
        ASSERT_EQ(ptr_2, nullptr);
    }
}

TEST(Memory, free)
{


    // Difficult to test, at least it should not crash
    hud::memory::free(hud::memory::allocate(sizeof(u32)));

    // Do nothing without crashing
    hud::memory::free(nullptr);
}


TEST(Memory, free_align)
{


    // Difficult to test, at least it should not crash
    hud::memory::free_align(hud::memory::allocate_align(sizeof(u32), 128));

    // Do nothing without crashing
    hud::memory::free_align(nullptr);
}

TEST(Memory, allocate_align_and_free_align_are_usable_in_consteval)
{


    constexpr bool allocate_const = []() -> bool {
        i32* const_ptr = hud::memory::allocate_align<i32>(2, 128);
        bool allocated = const_ptr != nullptr;
        hud::memory::free_align(const_ptr, 2);
        return allocated;
    }();
    ASSERT_TRUE(allocate_const);
}

TEST(Memory, reallocate)
{


    u32* ptr = reinterpret_cast<u32*>(hud::memory::reallocate(nullptr, sizeof(u32)));
    hud::test::LeakGuard guard(ptr);
    ASSERT_NE(ptr, nullptr);
    *ptr = 2;
    ptr = reinterpret_cast<u32*>(hud::memory::reallocate(ptr, sizeof(u32) * 2));
    ASSERT_NE(ptr, nullptr);
    *(ptr + 1) = 3;
    ASSERT_EQ(*ptr, 2u);
    ASSERT_EQ(*(ptr + 1), 3u  );
    ASSERT_EQ(hud::memory::reallocate(ptr, 0), nullptr);

    //Do not free, it's already free
    guard.leak();
}

TEST(Memory, reallocate_align)
{


    struct a {
        A(A&& other) = default;
        u32 i;
    };
    for (u32 aligment = 1; aligment <= 256; aligment <<= 1) {

        A* ptr = reinterpret_cast<A*>(hud::memory::reallocate_align(nullptr, sizeof(A), aligment));
        hud::test::AlignLeakGuard guard(ptr);
        ASSERT_NE(ptr, nullptr);
        ASSERT_TRUE(hud::memory::is_pointer_aligned(ptr, aligment));
        ptr->i = 2;
        ptr = reinterpret_cast<A*>(hud::memory::reallocate_align(ptr, sizeof(A) * 2, aligment));
        ASSERT_NE(ptr, nullptr);
        ASSERT_TRUE(hud::memory::is_pointer_aligned(ptr, aligment));
        (ptr + 1)->i = 3;
        ASSERT_EQ(ptr->i, 2u);
        ASSERT_EQ((ptr + 1)->i, 3u);
        ASSERT_EQ(hud::memory::reallocate_align(ptr, 0, 4), nullptr);
        guard.leak();
    }
}