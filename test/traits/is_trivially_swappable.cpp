#include <core/traits/is_trivially_swappable.h>

namespace {
    struct a {
        void fn_a() {};
        void fn_a2() const {};
        void fn_a3() volatile {};
        void fn_a4() const volatile {};
        int a0 = 0;
        const int a1 = 0;
        volatile int a2 = 0;
        const volatile int a3 = 0;
    };
    [[maybe_unused]] void(a::* pt)() = &a::fn_a;
    [[maybe_unused]] void(a::* pt2)() const = &a::fn_a2;
    [[maybe_unused]] void(a::* pt3)() volatile = &a::fn_a3;
    [[maybe_unused]] void(a::* pt4)() const volatile = &a::fn_a4;
    enum E {};

    struct yes { };
    struct no_dtor {
        ~no_dtor() noexcept {}
    };

    struct no_move_ctor {
        no_move_ctor(no_move_ctor&&) noexcept {}

    };

    struct no_move_assign {
        no_move_assign& operator=(no_move_assign&&) noexcept { return *this; }

    };
      
    struct no {
        no(no&&)noexcept {}
        no& operator=(no&&) noexcept { return *this; }
        ~no() noexcept {}
    };
}

TEST(traits, is_trivially_swappable) {
    ASSERT_TRUE((hud::is_trivially_swappable_v<i8>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<u8>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<i16>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<u16>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<i32>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<u32>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<i64>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<u64>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<iptr>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<uptr>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<f32>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<f32>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<f64>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<f64>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<isize>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<usize>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<ansichar>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<wchar>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<char16>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<char32>));

    ASSERT_TRUE((hud::is_trivially_swappable_v<i8*>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<u8*>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<i16*>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<u16*>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<i32*>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<u32*>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<i64*>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<u64*>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<iptr*>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<uptr*>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<f32*>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<f32*>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<f64*>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<f64*>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<isize*>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<usize*>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<ansichar*>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<wchar*>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<char16*>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<char32*>));

    ASSERT_TRUE((hud::is_trivially_swappable_v<void(a::*)()>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<void(a::*)() const>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<void(a::*)() volatile>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<void(a::*)() const volatile>));

    ASSERT_TRUE((hud::is_trivially_swappable_v<decltype(pt)>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<decltype(pt2)>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<decltype(pt3)>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<decltype(pt4)>));

    
    ASSERT_TRUE((hud::is_trivially_swappable_v<E>));

    ASSERT_TRUE((hud::is_trivially_swappable_v<yes>));
    ASSERT_FALSE((hud::is_trivially_swappable_v<no_dtor>));
    ASSERT_FALSE((hud::is_trivially_swappable_v<no_move_ctor>));
    ASSERT_FALSE((hud::is_trivially_swappable_v<no_move_assign>));
    ASSERT_FALSE((hud::is_trivially_swappable_v<no>));
   


    ASSERT_TRUE((hud::is_trivially_swappable_v<i8, i8>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<i8, u8>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<i8, i16>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<i8, u16>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<i8, i32>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<i8, u32>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<i8, i64>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<i8, u64>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<u8, i8>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<u8, u8>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<u8, i16>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<u8, u16>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<u8, i32>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<u8, u32>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<u8, i64>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<u8, u64>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<i16, i8>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<i16, u8>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<i16, i16>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<i16, u16>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<i16, i32>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<i16, u32>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<i16, i64>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<i16, u64>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<u16, i8>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<u16, u8>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<u16, i16>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<u16, u16>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<u16, i32>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<u16, u32>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<u16, i64>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<u16, u64>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<i32, i8>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<i32, u8>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<i32, i16>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<i32, u16>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<i32, i32>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<i32, u32>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<i32, i64>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<i32, u64>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<u32, i8>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<u32, u8>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<u32, i16>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<u32, u16>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<u32, i32>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<u32, u32>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<u32, i64>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<u32, u64>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<i64, i8>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<i64, u8>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<i64, i16>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<i64, u16>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<i64, i32>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<i64, u32>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<i64, i64>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<i64, u64>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<u64, i8>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<u64, u8>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<u64, i16>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<u64, u16>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<u64, i32>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<u64, u32>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<u64, i64>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<u64, u64>));


    ASSERT_TRUE((hud::is_trivially_swappable_v<void(a::*)(), void(a::*)()>));
    ASSERT_FALSE((hud::is_trivially_swappable_v<void(a::*)(), void(a::*)() const>));
    ASSERT_FALSE((hud::is_trivially_swappable_v<void(a::*)(), void(a::*)() volatile>));
    ASSERT_FALSE((hud::is_trivially_swappable_v<void(a::*)(), void(a::*)() const volatile>));
    ASSERT_FALSE((hud::is_trivially_swappable_v<void(a::*)()const, void(a::*)()>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<void(a::*)()const, void(a::*)() const>));
    ASSERT_FALSE((hud::is_trivially_swappable_v<void(a::*)()const, void(a::*)() volatile>));
    ASSERT_FALSE((hud::is_trivially_swappable_v<void(a::*)()const, void(a::*)() const volatile>));
    ASSERT_FALSE((hud::is_trivially_swappable_v<void(a::*)()volatile, void(a::*)()>));
    ASSERT_FALSE((hud::is_trivially_swappable_v<void(a::*)()volatile, void(a::*)() const>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<void(a::*)()volatile, void(a::*)() volatile>));
    ASSERT_FALSE((hud::is_trivially_swappable_v<void(a::*)()volatile, void(a::*)() const volatile>));
    ASSERT_FALSE((hud::is_trivially_swappable_v<void(a::*)()const volatile, void(a::*)()>));
    ASSERT_FALSE((hud::is_trivially_swappable_v<void(a::*)()const volatile, void(a::*)() const>));
    ASSERT_FALSE((hud::is_trivially_swappable_v<void(a::*)()const volatile, void(a::*)() volatile>));
    ASSERT_TRUE((hud::is_trivially_swappable_v<void(a::*)()const volatile, void(a::*)() const volatile>));



    ASSERT_TRUE((hud::is_trivially_swappable_v<yes, yes>));
    ASSERT_FALSE((hud::is_trivially_swappable_v<yes, no>));
    ASSERT_FALSE((hud::is_trivially_swappable_v<no, yes>));
    ASSERT_FALSE((hud::is_trivially_swappable_v<no, no>));

    ASSERT_FALSE((hud::is_trivially_swappable_v<no_dtor, no_dtor>));
    ASSERT_FALSE((hud::is_trivially_swappable_v<no_dtor, yes>));
    ASSERT_FALSE((hud::is_trivially_swappable_v<yes, no_dtor>));

    ASSERT_FALSE((hud::is_trivially_swappable_v<no_move_ctor, no_move_ctor>));
    ASSERT_FALSE((hud::is_trivially_swappable_v<no_move_ctor, yes>));
    ASSERT_FALSE((hud::is_trivially_swappable_v<yes, no_move_ctor>));

    ASSERT_FALSE((hud::is_trivially_swappable_v<no_move_assign, no_move_assign>));
    ASSERT_FALSE((hud::is_trivially_swappable_v<no_move_assign, yes>));
    ASSERT_FALSE((hud::is_trivially_swappable_v<yes, no_move_assign>));
}