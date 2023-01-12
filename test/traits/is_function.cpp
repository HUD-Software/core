#include <core/traits/is_function.h>

// LCOV_EXCL_START
namespace hud_test
{
    int is_function_a(int i) { return i; }
    template <typename>
    struct is_function_pm
    {
    };
    template <class type_t, class u_type>
    struct is_function_pm<u_type type_t::*>
    {
        using type = u_type;
    };

    [[maybe_unused]] int (*b)(int) = is_function_a; // pointer to function
    [[maybe_unused]] struct c
    {
        int operator()(int i) { return i; }
    } fct_obj; // function-like class
    struct d
    {
        int fct(int i) { return i; }
        int fct2(int i) const { return i; }
        int fct3(int i) volatile { return i; }
        int fct4(int i) const volatile { return i; }
        int d5;
    };
}
// LCOV_EXCL_STOP

GTEST_TEST(traits, is_function)
{
    GTEST_ASSERT_TRUE((hud::is_function_v<decltype(hud_test::is_function_a)>));
    GTEST_ASSERT_FALSE((hud::is_function_v<decltype(hud_test::b)>));
    GTEST_ASSERT_FALSE((hud::is_function_v<decltype(hud_test::fct_obj)>));
    GTEST_ASSERT_FALSE((hud::is_function_v<hud_test::c>));
    GTEST_ASSERT_FALSE((hud::is_function_v<decltype(hud_test::c())>));
    GTEST_ASSERT_FALSE((hud::is_function_v<hud_test::d>));
    GTEST_ASSERT_TRUE((hud::is_function_v<hud_test::is_function_pm<decltype(&hud_test::d::fct)>::type>));
    GTEST_ASSERT_TRUE((hud::is_function_v<hud_test::is_function_pm<decltype(&hud_test::d::fct2)>::type>));
    GTEST_ASSERT_TRUE((hud::is_function_v<hud_test::is_function_pm<decltype(&hud_test::d::fct3)>::type>));
    GTEST_ASSERT_TRUE((hud::is_function_v<hud_test::is_function_pm<decltype(&hud_test::d::fct4)>::type>));
    GTEST_ASSERT_FALSE((hud::is_function_v<hud_test::is_function_pm<decltype(&hud_test::d::d5)>::type>));
}