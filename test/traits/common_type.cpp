#include <core/traits/common_type.h>

namespace {
    struct Base {};
    struct Derived : Base {};

    template<typename = void, typename... type_t>
    struct has_common_type_impl 
        : hud::false_type {
    };

    template<typename... type_t>
    struct has_common_type_impl<hud::void_t<hud::common_type_t<type_t...>>, type_t...>
        : hud::true_type {
    };

    template<typename... type_t>
    struct has_common_type 
        : has_common_type_impl<void, type_t...> {
    };

    template<typename... type_t>
    inline constexpr bool has_common_type_v = has_common_type<type_t...>::value;
}


TEST(traits, common_type) {
    ASSERT_TRUE((hud::is_same_v<hud::common_type_t<i8, i16, i32>, i32>));
    ASSERT_TRUE((hud::is_same_v<hud::common_type_t<f32, f64>, f64>));
    ASSERT_TRUE((hud::is_same_v<hud::common_type_t<Derived, Base>, Base>));
    ASSERT_TRUE((hud::is_same_v<hud::common_type_t<const Derived, Base>, Base>));
    ASSERT_TRUE((hud::is_same_v<hud::common_type_t<const Derived, const Base>, Base>));
    ASSERT_TRUE((hud::is_same_v<hud::common_type_t<Derived*, Base*>, Base*>));
    ASSERT_TRUE((hud::is_same_v<hud::common_type_t<const Derived*, Base*>, const Base*>));
    ASSERT_TRUE((hud::is_same_v<hud::common_type_t<const Derived*, Derived*, const Base*>, const Base*>));
    ASSERT_TRUE((hud::is_same_v<hud::common_type_t<Derived*, const Base*>,const Base*>));
    ASSERT_TRUE((hud::is_same_v<hud::common_type_t<const i32, volatile i32>, i32>));
    ASSERT_TRUE((hud::is_same_v<hud::common_type_t<i32[], i32*>, i32*>));
    ASSERT_TRUE((hud::is_same_v<hud::common_type_t<i32[], const i32[32], i32*>, const i32*>));


    ASSERT_TRUE((has_common_type_v<i8, i16, i32>));
    ASSERT_TRUE((has_common_type_v<f32, f64>));
    ASSERT_TRUE((has_common_type_v<Derived, Base>));
    ASSERT_TRUE((has_common_type_v<const Derived, Base>));
    ASSERT_TRUE((has_common_type_v<const Derived, const Base>));
    ASSERT_TRUE((has_common_type_v<Derived*, Base*>));
    ASSERT_TRUE((has_common_type_v<const Derived*, Base*>));
    ASSERT_TRUE((has_common_type_v<const Derived*, Derived*, const Base*>));
    ASSERT_TRUE((has_common_type_v<Derived*, const Base*>));
    ASSERT_TRUE((has_common_type_v<const i32, volatile i32>));
    ASSERT_TRUE((has_common_type_v<i32[], i32*>));
    ASSERT_TRUE((has_common_type_v<i32[], const i32[32], i32*>));

    ASSERT_FALSE((has_common_type_v<i8, i16, i32, Derived>));
}