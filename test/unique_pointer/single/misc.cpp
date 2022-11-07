#include <core/containers/unique_pointer.h>

// LCOV_EXCL_START
namespace hud_test {
    template<typename type_t>
    struct custom_deleter
        : public hud::default_deleter<type_t>
        , hud_test::non_bitwise_type {

        constexpr custom_deleter() noexcept = default;
        constexpr custom_deleter(const custom_deleter& other) noexcept = default;
        constexpr custom_deleter(custom_deleter&& other) noexcept = default;
        constexpr custom_deleter(hud::default_deleter<type_t>&& other) noexcept
            : hud::default_deleter<type_t>(hud::move(other))
            , hud_test::non_bitwise_type(hud::move(other)) {
        }
        template<typename U>
        constexpr custom_deleter(custom_deleter<U>&& other) noexcept
            : hud::default_deleter<type_t>(hud::move(other))
            , hud_test::non_bitwise_type(hud::move(other)) {
        }
        constexpr custom_deleter& operator=(const custom_deleter&) noexcept {
            return *this;
        }
        constexpr custom_deleter& operator=(custom_deleter&&) noexcept {
            return *this;
        }

    };

    using deleter_type = hud_test::custom_deleter<hud_test::non_bitwise_type>;
}
// LCOV_EXCL_STOP

TEST(unique_pointer, pointer) {

    const auto test = []() {
        i32* ptr = new i32(35);
        hud::unique_pointer<i32> p(ptr);
        hud::unique_pointer<i32> p2;
        return std::tuple {
            p.pointer() == ptr,
            p2.pointer() == nullptr
        };
    };


    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
    }

    // constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
    }
}


TEST(unique_pointer, is_owning) {
    const auto test = []() {
        i32* ptr = new i32(35);
        hud::unique_pointer<i32> p(ptr);
        hud::unique_pointer<i32> p2;
        return std::tuple{
            p.is_owning(),
            p2.is_owning()
        };
    };


    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_FALSE(std::get<1>(result));
    }

    // constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_FALSE(std::get<1>(result));
    }
}

TEST(unique_pointer, cast_bool) {
    const auto test = []() {
        i32* ptr = new i32(35);
        hud::unique_pointer<i32> p(ptr);
        hud::unique_pointer<i32> p2;
        return std::tuple{
            static_cast<bool>(p),
            static_cast<bool>(p2)
        };
    };


    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_FALSE(std::get<1>(result));
    }

    // constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_FALSE(std::get<1>(result));
    }
}

TEST(unique_pointer, operator_dereference) {
    const auto test = []() {
        i32* ptr = new i32(35);
        hud::unique_pointer<i32> p(ptr);
        return *p == 35;
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(result);
    }

    // Constant
    {
        const auto result = test();
        ASSERT_TRUE(result);
    }
}

TEST(unique_pointer, operator_arrow) {
    const auto test = []() {
        i32* ptr = new i32(35);
        hud::unique_pointer<i32> p(ptr);
        return p.operator->() == ptr;
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(result);
    }

    // Constant
    {
        const auto result = test();
        ASSERT_TRUE(result);
    }
}

TEST(unique_pointer, deleter) {
    const auto test = []() {
        i32 dtor_counter = 0;
        hud_test::deleter_type deleter;
        deleter.set_dtor_counter_ptr(&dtor_counter);
        hud::unique_pointer<hud_test::non_bitwise_type, hud_test::deleter_type> p(new hud_test::non_bitwise_type(1, nullptr), deleter);
        const hud::unique_pointer<hud_test::non_bitwise_type, const hud_test::deleter_type> p_const(new hud_test::non_bitwise_type(2, nullptr), deleter);
        return std::tuple{
            !hud::is_const_v<hud::remove_reference_t<decltype(p.deleter())>>,
            hud::is_const_v<hud::remove_reference_t<decltype(p_const.deleter())>>,
            p.deleter().destructor_counter() == &dtor_counter,
            p_const.deleter().destructor_counter() == &dtor_counter
        };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
    }

    // constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
    }
}

TEST(unique_pointer, release) {
    const auto test = []() {
        hud_test::non_bitwise_type* ptr = new hud_test::non_bitwise_type(1, nullptr);
        hud::unique_pointer<hud_test::non_bitwise_type> p(ptr);
        auto released_ptr = p.leak();
        hud_test::non_bitwise_type* ptr_const = new hud_test::non_bitwise_type(1, nullptr);
        hud::unique_pointer<const hud_test::non_bitwise_type> p_const(ptr_const);
        auto const_released_ptr = p_const.leak();
        const auto result = std::tuple{
            hud::is_same_v<decltype(released_ptr), hud_test::non_bitwise_type*>,
            hud::is_same_v<decltype(const_released_ptr), const hud_test::non_bitwise_type*>,
            released_ptr == ptr,
            const_released_ptr == ptr_const
        };
        delete ptr;
        delete const_released_ptr;
        return result;
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
    }
}


TEST(unique_pointer, reset) {
    const auto test = []() {
        hud_test::non_bitwise_type* ptr = new hud_test::non_bitwise_type(1, nullptr);
        hud::unique_pointer<hud_test::non_bitwise_type> p(ptr);
        const bool is_not_null = p.pointer() == ptr;
        p.reset(nullptr);
        const bool is_null = p.pointer() == nullptr;
        ptr = new hud_test::non_bitwise_type(1, nullptr);
        p.reset(ptr);
        const bool is_not_null_2 = p.pointer() == ptr;
        p.reset();
        const bool is_null_2 = p.pointer() == nullptr;
        return std::tuple{
            is_not_null,
            is_null,
            is_not_null_2,
            is_null_2
        };
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(std::get<0>(result));
        ASSERT_TRUE(std::get<1>(result));
        ASSERT_TRUE(std::get<2>(result));
        ASSERT_TRUE(std::get<3>(result));
    }
}


TEST(unique_pointer, make_unique) {
    const auto test = []() {
        i32 dtor_count = 0;
        hud::unique_pointer<hud_test::non_bitwise_type> ptr = hud::make_unique<hud_test::non_bitwise_type>(123, &dtor_count);
        return ptr->id() == 123;
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(result);
    }

    // Constant
    {
        constexpr auto result = test();
        ASSERT_TRUE(result);
    }
}

TEST(unique_pointer, hash) {

    const auto test = []() {
        hud_test::non_bitwise_type* ptr = new hud_test::non_bitwise_type(1, nullptr);
        hud::unique_pointer<hud_test::non_bitwise_type> p(ptr);
        return hud::hash(p) == hud::hash(ptr);
    };

    // Non constant
    {
        const auto result = test();
        ASSERT_TRUE(result);
    }
}