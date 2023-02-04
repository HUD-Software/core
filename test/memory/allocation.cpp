
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
        hud_assert_ne(ptr, nullptr);

        // Allocation should failed (usize_max is too big)  and allocate should return nullptr
        void *ptr_2 = hud::memory::allocate(hud::usize_max);
        hud_test::LeakGuard leak_guard_2(ptr_2);
        hud_assert_eq(ptr_2, nullptr);
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
    hud_assert_true(allocate_const);
}

GTEST_TEST(memory, allocate_zero)
{

    // Allocation should success and allocate a 2*u32 on the heap and fill it with 0
    constexpr usize allocation_size = sizeof(u32) * 2;
    void *ptr = hud::memory::allocate_zero(allocation_size);
    hud_test::LeakGuard leak_guard(ptr);
    hud_assert_ne(ptr, nullptr);
    hud_assert_eq(*static_cast<u32 *>(ptr), 0u);
    hud_assert_eq(*(static_cast<u32 *>(ptr) + 1), 0u);

    // Allocation should failed (usize_max is too big)  and allocate should return nullptr
    void *ptr_2 = hud::memory::allocate_zero(hud::usize_max);
    hud_test::LeakGuard leak_guard_2(ptr_2);
    hud_assert_eq(ptr_2, nullptr);
}

GTEST_TEST(memory, allocate_align)
{

    for (u32 aligment = 1; aligment <= 256; aligment <<= 1)
    {

        // Allocation should success and allocate a u32 on the heap
        void *ptr = hud::memory::allocate_align(sizeof(u32) * 2, aligment);
        hud_test::AlignLeakGuard leak_guard(ptr);
        hud_assert_ne(ptr, nullptr);
        hud_assert_true(hud::memory::is_pointer_aligned(ptr, aligment));

        // Allocation should failed (usize_max is too big)  and allocate should return nullptr
        void *ptr_2 = hud::memory::allocate_align(hud::usize_max - (hud::memory::ALIGNED_MALLOC_HEADER_SIZE + aligment), aligment);
        hud_test::AlignLeakGuard leak_guard_2(ptr_2);
        hud_assert_eq(ptr_2, nullptr);
    }
}

GTEST_TEST(memory, allocate_align_zero)
{

    for (u32 aligment = 1; aligment <= 256; aligment <<= 1)
    {

        // Allocation should success and allocate a u32 on the heap
        void *ptr = hud::memory::allocate_align_zero(sizeof(u32) * 2, aligment);
        hud_test::AlignLeakGuard leak_guard(ptr);
        hud_assert_ne(ptr, nullptr);
        hud_assert_true(hud::memory::is_pointer_aligned(ptr, aligment));
        hud_assert_eq(*static_cast<u32 *>(ptr), 0u);
        hud_assert_eq(*(static_cast<u32 *>(ptr) + 1), 0u);

        // Allocation should failed (usize_max is too big)  and allocate should return nullptr
        void *ptr_2 = hud::memory::allocate_align_zero(hud::usize_max - (hud::memory::ALIGNED_MALLOC_HEADER_SIZE + aligment), aligment);
        hud_test::AlignLeakGuard leak_guard_2(ptr_2);
        hud_assert_eq(ptr_2, nullptr);
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
    hud_assert_true(allocate_const);
}

GTEST_TEST(memory, reallocate)
{

    u32 *ptr = reinterpret_cast<u32 *>(hud::memory::reallocate(nullptr, sizeof(u32)));
    hud_test::LeakGuard guard(ptr);
    hud_assert_ne(ptr, nullptr);
    *ptr = 2;
    ptr = reinterpret_cast<u32 *>(hud::memory::reallocate(ptr, sizeof(u32) * 2));
    hud_assert_ne(ptr, nullptr);
    *(ptr + 1) = 3;
    hud_assert_eq(*ptr, 2u);
    hud_assert_eq(*(ptr + 1), 3u);
    hud_assert_eq(hud::memory::reallocate(ptr, 0), nullptr);

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

    for (u32 alignment = 1; alignment <= 256; alignment <<= 1)
    {
        // Reallocate a null pointer to sizeof(a)
        a *ptr = reinterpret_cast<a *>(hud::memory::reallocate_align(nullptr, sizeof(a), alignment));
        hud_test::AlignLeakGuard guard(ptr);
        hud_assert_ne(ptr, nullptr);
        hud_assert_true(hud::memory::is_pointer_aligned(ptr, alignment));
        ptr->i = 1u;

        // Reallocate a sizeof(a) to 2*sizeof(a)
        a *ptr_2 = reinterpret_cast<a *>(hud::memory::reallocate_align(ptr, sizeof(a) * 2, alignment));
        hud_assert_ne(ptr_2, nullptr);
        hud_assert_ne(ptr, ptr_2);
        hud_assert_true(hud::memory::is_pointer_aligned(ptr_2, alignment));
        (ptr_2 + 1)->i = 2u;
        hud_assert_eq(ptr_2->i, 1u);
        hud_assert_eq((ptr_2 + 1)->i, 2u);

        // Reallocate a 2*sizeof(a) to sizeof(a)
        a *ptr_3 = reinterpret_cast<a *>(hud::memory::reallocate_align(ptr_2, sizeof(a), alignment));
        hud_assert_ne(ptr_3, nullptr);
        hud_assert_ne(ptr_2, ptr_3);
        hud_assert_true(hud::memory::is_pointer_aligned(ptr_3, alignment));
        hud_assert_eq(ptr_3->i, 1u);

        // Reallocate a sizeof(a) to sizeof(a)
        a *ptr_4 = reinterpret_cast<a *>(hud::memory::reallocate_align(ptr_3, sizeof(a), alignment));
        hud_assert_ne(ptr_4, nullptr);
        hud_assert_eq(ptr_3, ptr_4); // same same should not be reallocated and return the same pointer
        hud_assert_true(hud::memory::is_pointer_aligned(ptr_4, alignment));
        hud_assert_eq(ptr_4->i, 1u);

        // Reallocate a sizeof(a) to 0
        hud_assert_eq(hud::memory::reallocate_align(ptr_4, 0, alignment), nullptr);
        guard.leak();
    }
}