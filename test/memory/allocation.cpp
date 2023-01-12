
#include <core/memory.h>
#include "../misc/leak_guard.h"

GTEST_TEST(memory, allocate)
{

    // Non constant
    {

        // Allocation should success and allocate a u32 on the heap
        constexpr usize allocation_size = sizeof(u32) * 2;
        void *ptr = hud::memory::allocate(allocation_size);
        hud_test::LeakGuard leak_guard(ptr);
        GTEST_ASSERT_NE(ptr, nullptr);

        // Allocation should failed (usize_max is too big)  and allocate should return nullptr
        void *ptr_2 = hud::memory::allocate(hud::usize_max);
        hud_test::LeakGuard leak_guard_2(ptr_2);
        GTEST_ASSERT_EQ(ptr_2, nullptr);
    }
}

GTEST_TEST(memory, allocate_and_free_are_usable_in_constexpr)
{

    constexpr bool allocate_const = []() -> bool
    {
        i32 *const_ptr = hud::memory::allocate_array<i32>(2);
        bool allocated = const_ptr != nullptr;
        hud::memory::free_array(const_ptr, 2);
        return allocated;
    }();
    GTEST_ASSERT_TRUE(allocate_const);
}

GTEST_TEST(memory, allocate_zero)
{

    // Allocation should success and allocate a 2*u32 on the heap and fill it with 0
    constexpr usize allocation_size = sizeof(u32) * 2;
    void *ptr = hud::memory::allocate_zero(allocation_size);
    hud_test::LeakGuard leak_guard(ptr);
    GTEST_ASSERT_NE(ptr, nullptr);
    GTEST_ASSERT_EQ(*static_cast<u32 *>(ptr), 0u);
    GTEST_ASSERT_EQ(*(static_cast<u32 *>(ptr) + 1), 0u);

    // Allocation should failed (usize_max is too big)  and allocate should return nullptr
    void *ptr_2 = hud::memory::allocate_zero(hud::usize_max);
    hud_test::LeakGuard leak_guard_2(ptr_2);
    GTEST_ASSERT_EQ(ptr_2, nullptr);
}

GTEST_TEST(memory, allocate_align)
{

    for (u32 aligment = 1; aligment <= 256; aligment <<= 1)
    {

        // Allocation should success and allocate a u32 on the heap
        void *ptr = hud::memory::allocate_align(sizeof(u32) * 2, aligment);
        hud_test::AlignLeakGuard leak_guard(ptr);
        GTEST_ASSERT_NE(ptr, nullptr);
        GTEST_ASSERT_TRUE(hud::memory::is_pointer_aligned(ptr, aligment));

        // Allocation should failed (usize_max is too big)  and allocate should return nullptr
        void *ptr_2 = hud::memory::allocate_align(hud::usize_max - (hud::memory::ALIGNED_MALLOC_HEADER_SIZE + aligment), aligment);
        hud_test::AlignLeakGuard leak_guard_2(ptr_2);
        GTEST_ASSERT_EQ(ptr_2, nullptr);
    }
}

GTEST_TEST(memory, allocate_align_zero)
{

    for (u32 aligment = 1; aligment <= 256; aligment <<= 1)
    {

        // Allocation should success and allocate a u32 on the heap
        void *ptr = hud::memory::allocate_align_zero(sizeof(u32) * 2, aligment);
        hud_test::AlignLeakGuard leak_guard(ptr);
        GTEST_ASSERT_NE(ptr, nullptr);
        GTEST_ASSERT_TRUE(hud::memory::is_pointer_aligned(ptr, aligment));
        GTEST_ASSERT_EQ(*static_cast<u32 *>(ptr), 0u);
        GTEST_ASSERT_EQ(*(static_cast<u32 *>(ptr) + 1), 0u);

        // Allocation should failed (usize_max is too big)  and allocate should return nullptr
        void *ptr_2 = hud::memory::allocate_align_zero(hud::usize_max - (hud::memory::ALIGNED_MALLOC_HEADER_SIZE + aligment), aligment);
        hud_test::AlignLeakGuard leak_guard_2(ptr_2);
        GTEST_ASSERT_EQ(ptr_2, nullptr);
    }
}

GTEST_TEST(memory, free)
{

    // Difficult to test, at least it should not crash
    hud::memory::free(hud::memory::allocate(sizeof(u32)));

    // Do nothing without crashing
    hud::memory::free(nullptr);
}

GTEST_TEST(memory, free_align)
{

    // Difficult to test, at least it should not crash
    hud::memory::free_align(hud::memory::allocate_align(sizeof(u32), 128));

    // Do nothing without crashing
    hud::memory::free_align(nullptr);
}

GTEST_TEST(memory, allocate_align_and_free_align_are_usable_in_consteval)
{

    constexpr bool allocate_const = []() -> bool
    {
        i32 *const_ptr = hud::memory::allocate_align<i32>(2, 128);
        bool allocated = const_ptr != nullptr;
        hud::memory::free_align(const_ptr, 2);
        return allocated;
    }();
    GTEST_ASSERT_TRUE(allocate_const);
}

GTEST_TEST(memory, reallocate)
{

    u32 *ptr = reinterpret_cast<u32 *>(hud::memory::reallocate(nullptr, sizeof(u32)));
    hud_test::LeakGuard guard(ptr);
    GTEST_ASSERT_NE(ptr, nullptr);
    *ptr = 2;
    ptr = reinterpret_cast<u32 *>(hud::memory::reallocate(ptr, sizeof(u32) * 2));
    GTEST_ASSERT_NE(ptr, nullptr);
    *(ptr + 1) = 3;
    GTEST_ASSERT_EQ(*ptr, 2u);
    GTEST_ASSERT_EQ(*(ptr + 1), 3u);
    GTEST_ASSERT_EQ(hud::memory::reallocate(ptr, 0), nullptr);

    // Do not free, it's already free
    guard.leak();
}

GTEST_TEST(memory, reallocate_align)
{

    struct a
    {
        a(a &&other) = default;
        u32 i;
    };
    for (u32 aligment = 1; aligment <= 256; aligment <<= 1)
    {

        a *ptr = reinterpret_cast<a *>(hud::memory::reallocate_align(nullptr, sizeof(a), aligment));
        hud_test::AlignLeakGuard guard(ptr);
        GTEST_ASSERT_NE(ptr, nullptr);
        GTEST_ASSERT_TRUE(hud::memory::is_pointer_aligned(ptr, aligment));
        ptr->i = 2;
        ptr = reinterpret_cast<a *>(hud::memory::reallocate_align(ptr, sizeof(a) * 2, aligment));
        GTEST_ASSERT_NE(ptr, nullptr);
        GTEST_ASSERT_TRUE(hud::memory::is_pointer_aligned(ptr, aligment));
        (ptr + 1)->i = 3;
        GTEST_ASSERT_EQ(ptr->i, 2u);
        GTEST_ASSERT_EQ((ptr + 1)->i, 3u);
        GTEST_ASSERT_EQ(hud::memory::reallocate_align(ptr, 0, 4), nullptr);
        guard.leak();
    }
}