#include <core/traits/is_function.h>

namespace {
    int is_function_a(int i) { return i; }
    template<typename>
    struct is_function_pm {};
    template<class type_t, class u_type>
    struct is_function_pm<u_type type_t::*> {
        using type = u_type;
    };

    [[maybe_unused]] int(*b)(int) = is_function_a; // pointer to function
    [[maybe_unused]] struct c { int operator()(int i) { return i; } } fct_obj;  // function-like class
    struct d {
        int fct(int i) { return i; }
        int fct2(int i) const { return i; }
        int fct3(int i) volatile { return i; }
        int fct4(int i) const volatile { return i; }
        int d5;
    };
}

TEST(traits, is_function) {
    ASSERT_TRUE((hud::is_function_v<decltype(is_function_a)>));
    ASSERT_FALSE((hud::is_function_v<decltype(b)>));
    ASSERT_FALSE((hud::is_function_v<decltype(fct_obj)>));
    ASSERT_FALSE((hud::is_function_v<c>));
    ASSERT_FALSE((hud::is_function_v<decltype(c())>));
    ASSERT_FALSE((hud::is_function_v<d>));
    ASSERT_TRUE((hud::is_function_v<is_function_pm<decltype(&d::fct)>::type>));
    ASSERT_TRUE((hud::is_function_v<is_function_pm<decltype(&d::fct2)>::type>));
    ASSERT_TRUE((hud::is_function_v<is_function_pm<decltype(&d::fct3)>::type>));
    ASSERT_TRUE((hud::is_function_v<is_function_pm<decltype(&d::fct4)>::type>));
    ASSERT_FALSE((hud::is_function_v<is_function_pm<decltype(&d::d5)>::type>));
}