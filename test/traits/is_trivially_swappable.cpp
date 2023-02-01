#include <core/traits/is_trivially_swappable.h>

namespace hud_test
{
    namespace
    {
        struct a
        {
            void fn_a() {};
            void fn_a2() const {};
            void fn_a3() volatile {};
            void fn_a4() const volatile {};
            int a0 = 0;
            const int a1 = 0;
            volatile int a2 = 0;
            const volatile int a3 = 0;
        };

        [[maybe_unused]] void (a::*pt)() = &a::fn_a;
        [[maybe_unused]] void (a::*pt2)() const = &a::fn_a2;
        [[maybe_unused]] void (a::*pt3)() volatile = &a::fn_a3;
        [[maybe_unused]] void (a::*pt4)() const volatile = &a::fn_a4;

        enum E
        {
        };

        struct yes
        {
        };

        struct no_dtor
        {
            ~no_dtor() noexcept
            {
            }
        };

        struct no_move_ctor
        {
            no_move_ctor(no_move_ctor &&) noexcept
            {
            }
        };

        struct no_move_assign
        {
            no_move_assign &operator=(no_move_assign &&) noexcept
            {
                return *this;
            }
        };

        struct no
        {
            no(no &&) noexcept
            {
            }

            no &operator=(no &&) noexcept
            {
                return *this;
            }

            ~no() noexcept
            {
            }
        };
    } // namespace
} // namespace hud_test

GTEST_TEST(traits, is_trivially_swappable)
{
    hud_assert_true((hud::is_trivially_swappable_v<i8>));
    hud_assert_true((hud::is_trivially_swappable_v<u8>));
    hud_assert_true((hud::is_trivially_swappable_v<i16>));
    hud_assert_true((hud::is_trivially_swappable_v<u16>));
    hud_assert_true((hud::is_trivially_swappable_v<i32>));
    hud_assert_true((hud::is_trivially_swappable_v<u32>));
    hud_assert_true((hud::is_trivially_swappable_v<i64>));
    hud_assert_true((hud::is_trivially_swappable_v<u64>));
    hud_assert_true((hud::is_trivially_swappable_v<iptr>));
    hud_assert_true((hud::is_trivially_swappable_v<uptr>));
    hud_assert_true((hud::is_trivially_swappable_v<f32>));
    hud_assert_true((hud::is_trivially_swappable_v<f32>));
    hud_assert_true((hud::is_trivially_swappable_v<f64>));
    hud_assert_true((hud::is_trivially_swappable_v<f64>));
    hud_assert_true((hud::is_trivially_swappable_v<isize>));
    hud_assert_true((hud::is_trivially_swappable_v<usize>));
    hud_assert_true((hud::is_trivially_swappable_v<ansichar>));
    hud_assert_true((hud::is_trivially_swappable_v<wchar>));
    hud_assert_true((hud::is_trivially_swappable_v<char16>));
    hud_assert_true((hud::is_trivially_swappable_v<char32>));

    hud_assert_true((hud::is_trivially_swappable_v<i8 *>));
    hud_assert_true((hud::is_trivially_swappable_v<u8 *>));
    hud_assert_true((hud::is_trivially_swappable_v<i16 *>));
    hud_assert_true((hud::is_trivially_swappable_v<u16 *>));
    hud_assert_true((hud::is_trivially_swappable_v<i32 *>));
    hud_assert_true((hud::is_trivially_swappable_v<u32 *>));
    hud_assert_true((hud::is_trivially_swappable_v<i64 *>));
    hud_assert_true((hud::is_trivially_swappable_v<u64 *>));
    hud_assert_true((hud::is_trivially_swappable_v<iptr *>));
    hud_assert_true((hud::is_trivially_swappable_v<uptr *>));
    hud_assert_true((hud::is_trivially_swappable_v<f32 *>));
    hud_assert_true((hud::is_trivially_swappable_v<f32 *>));
    hud_assert_true((hud::is_trivially_swappable_v<f64 *>));
    hud_assert_true((hud::is_trivially_swappable_v<f64 *>));
    hud_assert_true((hud::is_trivially_swappable_v<isize *>));
    hud_assert_true((hud::is_trivially_swappable_v<usize *>));
    hud_assert_true((hud::is_trivially_swappable_v<ansichar *>));
    hud_assert_true((hud::is_trivially_swappable_v<wchar *>));
    hud_assert_true((hud::is_trivially_swappable_v<char16 *>));
    hud_assert_true((hud::is_trivially_swappable_v<char32 *>));

    hud_assert_true((hud::is_trivially_swappable_v<void (hud_test::a::*)()>));
    hud_assert_true((hud::is_trivially_swappable_v<void (hud_test::a::*)() const>));
    hud_assert_true((hud::is_trivially_swappable_v<void (hud_test::a::*)() volatile>));
    hud_assert_true((hud::is_trivially_swappable_v<void (hud_test::a::*)() const volatile>));

    hud_assert_true((hud::is_trivially_swappable_v<decltype(hud_test::pt)>));
    hud_assert_true((hud::is_trivially_swappable_v<decltype(hud_test::pt2)>));
    hud_assert_true((hud::is_trivially_swappable_v<decltype(hud_test::pt3)>));
    hud_assert_true((hud::is_trivially_swappable_v<decltype(hud_test::pt4)>));

    hud_assert_true((hud::is_trivially_swappable_v<hud_test::E>));

    hud_assert_true((hud::is_trivially_swappable_v<hud_test::yes>));
    hud_assert_false((hud::is_trivially_swappable_v<hud_test::no_dtor>));
    hud_assert_false((hud::is_trivially_swappable_v<hud_test::no_move_ctor>));
    hud_assert_false((hud::is_trivially_swappable_v<hud_test::no_move_assign>));
    hud_assert_false((hud::is_trivially_swappable_v<hud_test::no>));

    hud_assert_true((hud::is_trivially_swappable_v<i8, i8>));
    hud_assert_true((hud::is_trivially_swappable_v<i8, u8>));
    hud_assert_true((hud::is_trivially_swappable_v<i8, i16>));
    hud_assert_true((hud::is_trivially_swappable_v<i8, u16>));
    hud_assert_true((hud::is_trivially_swappable_v<i8, i32>));
    hud_assert_true((hud::is_trivially_swappable_v<i8, u32>));
    hud_assert_true((hud::is_trivially_swappable_v<i8, i64>));
    hud_assert_true((hud::is_trivially_swappable_v<i8, u64>));
    hud_assert_true((hud::is_trivially_swappable_v<u8, i8>));
    hud_assert_true((hud::is_trivially_swappable_v<u8, u8>));
    hud_assert_true((hud::is_trivially_swappable_v<u8, i16>));
    hud_assert_true((hud::is_trivially_swappable_v<u8, u16>));
    hud_assert_true((hud::is_trivially_swappable_v<u8, i32>));
    hud_assert_true((hud::is_trivially_swappable_v<u8, u32>));
    hud_assert_true((hud::is_trivially_swappable_v<u8, i64>));
    hud_assert_true((hud::is_trivially_swappable_v<u8, u64>));
    hud_assert_true((hud::is_trivially_swappable_v<i16, i8>));
    hud_assert_true((hud::is_trivially_swappable_v<i16, u8>));
    hud_assert_true((hud::is_trivially_swappable_v<i16, i16>));
    hud_assert_true((hud::is_trivially_swappable_v<i16, u16>));
    hud_assert_true((hud::is_trivially_swappable_v<i16, i32>));
    hud_assert_true((hud::is_trivially_swappable_v<i16, u32>));
    hud_assert_true((hud::is_trivially_swappable_v<i16, i64>));
    hud_assert_true((hud::is_trivially_swappable_v<i16, u64>));
    hud_assert_true((hud::is_trivially_swappable_v<u16, i8>));
    hud_assert_true((hud::is_trivially_swappable_v<u16, u8>));
    hud_assert_true((hud::is_trivially_swappable_v<u16, i16>));
    hud_assert_true((hud::is_trivially_swappable_v<u16, u16>));
    hud_assert_true((hud::is_trivially_swappable_v<u16, i32>));
    hud_assert_true((hud::is_trivially_swappable_v<u16, u32>));
    hud_assert_true((hud::is_trivially_swappable_v<u16, i64>));
    hud_assert_true((hud::is_trivially_swappable_v<u16, u64>));
    hud_assert_true((hud::is_trivially_swappable_v<i32, i8>));
    hud_assert_true((hud::is_trivially_swappable_v<i32, u8>));
    hud_assert_true((hud::is_trivially_swappable_v<i32, i16>));
    hud_assert_true((hud::is_trivially_swappable_v<i32, u16>));
    hud_assert_true((hud::is_trivially_swappable_v<i32, i32>));
    hud_assert_true((hud::is_trivially_swappable_v<i32, u32>));
    hud_assert_true((hud::is_trivially_swappable_v<i32, i64>));
    hud_assert_true((hud::is_trivially_swappable_v<i32, u64>));
    hud_assert_true((hud::is_trivially_swappable_v<u32, i8>));
    hud_assert_true((hud::is_trivially_swappable_v<u32, u8>));
    hud_assert_true((hud::is_trivially_swappable_v<u32, i16>));
    hud_assert_true((hud::is_trivially_swappable_v<u32, u16>));
    hud_assert_true((hud::is_trivially_swappable_v<u32, i32>));
    hud_assert_true((hud::is_trivially_swappable_v<u32, u32>));
    hud_assert_true((hud::is_trivially_swappable_v<u32, i64>));
    hud_assert_true((hud::is_trivially_swappable_v<u32, u64>));
    hud_assert_true((hud::is_trivially_swappable_v<i64, i8>));
    hud_assert_true((hud::is_trivially_swappable_v<i64, u8>));
    hud_assert_true((hud::is_trivially_swappable_v<i64, i16>));
    hud_assert_true((hud::is_trivially_swappable_v<i64, u16>));
    hud_assert_true((hud::is_trivially_swappable_v<i64, i32>));
    hud_assert_true((hud::is_trivially_swappable_v<i64, u32>));
    hud_assert_true((hud::is_trivially_swappable_v<i64, i64>));
    hud_assert_true((hud::is_trivially_swappable_v<i64, u64>));
    hud_assert_true((hud::is_trivially_swappable_v<u64, i8>));
    hud_assert_true((hud::is_trivially_swappable_v<u64, u8>));
    hud_assert_true((hud::is_trivially_swappable_v<u64, i16>));
    hud_assert_true((hud::is_trivially_swappable_v<u64, u16>));
    hud_assert_true((hud::is_trivially_swappable_v<u64, i32>));
    hud_assert_true((hud::is_trivially_swappable_v<u64, u32>));
    hud_assert_true((hud::is_trivially_swappable_v<u64, i64>));
    hud_assert_true((hud::is_trivially_swappable_v<u64, u64>));

    hud_assert_true((hud::is_trivially_swappable_v<void (hud_test::a::*)(), void (hud_test::a::*)()>));
    hud_assert_false((hud::is_trivially_swappable_v<void (hud_test::a::*)(), void (hud_test::a::*)() const>));
    hud_assert_false((hud::is_trivially_swappable_v<void (hud_test::a::*)(), void (hud_test::a::*)() volatile>));
    hud_assert_false((hud::is_trivially_swappable_v<void (hud_test::a::*)(), void (hud_test::a::*)() const volatile>));
    hud_assert_false((hud::is_trivially_swappable_v<void (hud_test::a::*)() const, void (hud_test::a::*)()>));
    hud_assert_true((hud::is_trivially_swappable_v<void (hud_test::a::*)() const, void (hud_test::a::*)() const>));
    hud_assert_false((hud::is_trivially_swappable_v<void (hud_test::a::*)() const, void (hud_test::a::*)() volatile>));
    hud_assert_false((hud::is_trivially_swappable_v<void (hud_test::a::*)() const, void (hud_test::a::*)() const volatile>));
    hud_assert_false((hud::is_trivially_swappable_v<void (hud_test::a::*)() volatile, void (hud_test::a::*)()>));
    hud_assert_false((hud::is_trivially_swappable_v<void (hud_test::a::*)() volatile, void (hud_test::a::*)() const>));
    hud_assert_true((hud::is_trivially_swappable_v<void (hud_test::a::*)() volatile, void (hud_test::a::*)() volatile>));
    hud_assert_false((hud::is_trivially_swappable_v<void (hud_test::a::*)() volatile, void (hud_test::a::*)() const volatile>));
    hud_assert_false((hud::is_trivially_swappable_v<void (hud_test::a::*)() const volatile, void (hud_test::a::*)()>));
    hud_assert_false((hud::is_trivially_swappable_v<void (hud_test::a::*)() const volatile, void (hud_test::a::*)() const>));
    hud_assert_false((hud::is_trivially_swappable_v<void (hud_test::a::*)() const volatile, void (hud_test::a::*)() volatile>));
    hud_assert_true((hud::is_trivially_swappable_v<void (hud_test::a::*)() const volatile, void (hud_test::a::*)() const volatile>));

    hud_assert_true((hud::is_trivially_swappable_v<hud_test::yes, hud_test::yes>));
    hud_assert_false((hud::is_trivially_swappable_v<hud_test::yes, hud_test::no>));
    hud_assert_false((hud::is_trivially_swappable_v<hud_test::no, hud_test::yes>));
    hud_assert_false((hud::is_trivially_swappable_v<hud_test::no, hud_test::no>));

    hud_assert_false((hud::is_trivially_swappable_v<hud_test::no_dtor, hud_test::no_dtor>));
    hud_assert_false((hud::is_trivially_swappable_v<hud_test::no_dtor, hud_test::yes>));
    hud_assert_false((hud::is_trivially_swappable_v<hud_test::yes, hud_test::no_dtor>));

    hud_assert_false((hud::is_trivially_swappable_v<hud_test::no_move_ctor, hud_test::no_move_ctor>));
    hud_assert_false((hud::is_trivially_swappable_v<hud_test::no_move_ctor, hud_test::yes>));
    hud_assert_false((hud::is_trivially_swappable_v<hud_test::yes, hud_test::no_move_ctor>));

    hud_assert_false((hud::is_trivially_swappable_v<hud_test::no_move_assign, hud_test::no_move_assign>));
    hud_assert_false((hud::is_trivially_swappable_v<hud_test::no_move_assign, hud_test::yes>));
    hud_assert_false((hud::is_trivially_swappable_v<hud_test::yes, hud_test::no_move_assign>));
}