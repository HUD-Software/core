#ifndef HD_INC_TEST_MISC_LEAKGUARD_H
#define HD_INC_TEST_MISC_LEAKGUARD_H
#include <core/memory.h>

namespace hud_test
{

    template<typename type_t = void>
    class LeakGuard
    {
    public:
        constexpr LeakGuard(type_t *allocation) noexcept
            : ptr(allocation)
        {
        }

        constexpr ~LeakGuard()
        {
            hud::memory::free(ptr);
        }

        constexpr void leak() noexcept
        {
            ptr = nullptr;
        }

    private:
        type_t *ptr;
    };

    template<typename type_t = void>
    class LeakArrayGuard
    {
    public:
        constexpr LeakArrayGuard(type_t *allocation, const usize count) noexcept
            : ptr(allocation)
            , count_of_T(count)
        {
        }

        constexpr ~LeakArrayGuard()
        {
            hud::memory::free_array(ptr, count_of_T);
        }

        constexpr void leak() noexcept
        {
            ptr = nullptr;
            count_of_T = 0;
        }

    private:
        type_t *ptr;
        usize count_of_T;
    };

    class AlignLeakGuard
    {
    public:
        AlignLeakGuard(void *allocation) noexcept
            : ptr(allocation)
        {
        }

        ~AlignLeakGuard()
        {
            hud::memory::free_align(ptr);
        }

        void leak() noexcept
        {
            ptr = nullptr;
        }

    private:
        void *ptr;
    };

} // namespace hud_test

#endif // HD_INC_TEST_MISC_LEAKGUARD_H