#include <core/containers/unique_pointer.h>

namespace hud_test
{
    template<typename type_t>
    struct custom_deleter
        : public hud::default_deleter<type_t>
        , hud_test::non_bitwise_type
    {

        constexpr custom_deleter() noexcept = default;
        constexpr custom_deleter(const custom_deleter &other) noexcept = default;
        constexpr custom_deleter(custom_deleter &&other) noexcept = default;

        constexpr custom_deleter(hud::default_deleter<type_t> &&other) noexcept
            : hud::default_deleter<type_t>(hud::move(other))
            , hud_test::non_bitwise_type(hud::move(other))
        {
        }

        template<typename U>
        constexpr custom_deleter(custom_deleter<U> &&other) noexcept
            : hud::default_deleter<type_t>(hud::move(other))
            , hud_test::non_bitwise_type(hud::move(other))
        {
        }

        constexpr custom_deleter &operator=(const custom_deleter &) noexcept
        {
            return *this;
        }

        constexpr custom_deleter &operator=(custom_deleter &&) noexcept
        {
            return *this;
        }

        ~custom_deleter() = default;
    };

    using deleter_type = hud_test::custom_deleter<hud_test::non_bitwise_type>;

    template<typename type_t>
    struct custom_deleter2 : public hud_test::custom_deleter<type_t>
    {
        ~custom_deleter2() = default;
    };

    using deleter_type2 = hud_test::custom_deleter<hud_test::non_bitwise_type>;
} // namespace hud_test

GTEST_TEST(unique_pointer, default_constructor)
{
    const auto test = []()
    {
        hud::unique_pointer<hud_test::non_bitwise_type> p;
        return p.pointer() == nullptr;
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(result);
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(result);
    }
}

GTEST_TEST(unique_pointer, constructor_with_same_pointer)
{

    const auto test = []()
    {
        hud_test::non_bitwise_type *pi = new hud_test::non_bitwise_type(123, nullptr);
        hud::unique_pointer<hud_test::non_bitwise_type> p(pi);
        return std::tuple {
            p.pointer() == pi,
            pi->constructor_count() == 1u,
            pi->copy_constructor_count() == 0u,
            pi->move_constructor_count() == 0u,
            pi->copy_assign_count() == 0u,
            pi->move_assign_count() == 0u};
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
    }
}

GTEST_TEST(unique_pointer, constructor_with_differnt_pointer)
{

    const auto test = []()
    {
        hud_test::non_bitwise_type *pi = new hud_test::non_bitwise_type(123, nullptr);
        hud::unique_pointer<const hud_test::non_bitwise_type> p(pi);
        return std::tuple {
            p.pointer() == pi,
            pi->constructor_count() == 1u,
            pi->copy_constructor_count() == 0u,
            pi->move_constructor_count() == 0u,
            pi->copy_assign_count() == 0u,
            pi->move_assign_count() == 0u};
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
    }
}

GTEST_TEST(unique_pointer, constructor_with_pointer_and_same_deleter_by_copy)
{

    const auto test = []()
    {
        hud_test::non_bitwise_type *pi = new hud_test::non_bitwise_type(123, nullptr);
        hud_test::deleter_type deleter;
        hud::unique_pointer<hud_test::non_bitwise_type, hud_test::deleter_type> p(pi, deleter);
        return std::tuple {
            p.pointer() == pi,
            hud::is_same_v<decltype(p.deleter()), hud_test::deleter_type &>,
            pi->constructor_count() == 1u,
            pi->copy_constructor_count() == 0u,
            pi->move_constructor_count() == 0u,
            pi->copy_assign_count() == 0u,
            pi->move_assign_count() == 0u,
            p.deleter().constructor_count() == 0u,
            p.deleter().copy_constructor_count() == 1u,
            p.deleter().move_constructor_count() == 0u,
            p.deleter().copy_assign_count() == 0u,
            p.deleter().move_assign_count() == 0u,
        };
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_true(std::get<6>(result));
        hud_assert_true(std::get<7>(result));
        hud_assert_true(std::get<8>(result));
        hud_assert_true(std::get<9>(result));
        hud_assert_true(std::get<10>(result));
        hud_assert_true(std::get<11>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_true(std::get<6>(result));
        hud_assert_true(std::get<7>(result));
        hud_assert_true(std::get<8>(result));
        hud_assert_true(std::get<9>(result));
        hud_assert_true(std::get<10>(result));
        hud_assert_true(std::get<11>(result));
    }
}

GTEST_TEST(unique_pointer, constructor_with_pointer_and_different_deleter_by_copy)
{

    const auto test = []()
    {
        hud_test::non_bitwise_type *pi = new hud_test::non_bitwise_type(123, nullptr);
        hud_test::deleter_type2 deleter;
        hud::unique_pointer<hud_test::non_bitwise_type, hud_test::deleter_type> p(pi, deleter);
        return std::tuple {
            p.pointer() == pi,
            hud::is_same_v<decltype(p.deleter()), hud_test::deleter_type &>,
            pi->constructor_count() == 1u,
            pi->copy_constructor_count() == 0u,
            pi->move_constructor_count() == 0u,
            pi->copy_assign_count() == 0u,
            pi->move_assign_count() == 0u,
            p.deleter().constructor_count() == 0u,
            p.deleter().copy_constructor_count() == 1u,
            p.deleter().move_constructor_count() == 0u,
            p.deleter().copy_assign_count() == 0u,
            p.deleter().move_assign_count() == 0u,
        };
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_true(std::get<6>(result));
        hud_assert_true(std::get<7>(result));
        hud_assert_true(std::get<8>(result));
        hud_assert_true(std::get<9>(result));
        hud_assert_true(std::get<10>(result));
        hud_assert_true(std::get<11>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_true(std::get<6>(result));
        hud_assert_true(std::get<7>(result));
        hud_assert_true(std::get<8>(result));
        hud_assert_true(std::get<9>(result));
        hud_assert_true(std::get<10>(result));
        hud_assert_true(std::get<11>(result));
    }
}

GTEST_TEST(unique_pointer, constructor_with_pointer_and_same_deleter_by_move)
{
    const auto test = []()
    {
        static_assert(hud::is_move_constructible_v<hud_test::deleter_type>);
        hud_test::non_bitwise_type *pi = new hud_test::non_bitwise_type(123, nullptr);
        hud::unique_pointer<hud_test::non_bitwise_type, hud_test::deleter_type> p(pi, hud_test::deleter_type {});
        return std::tuple {
            p.pointer() == pi,
            hud::is_same_v<decltype(p.deleter()), hud_test::deleter_type &>,
            pi->constructor_count() == 1u,
            pi->copy_constructor_count() == 0u,
            pi->move_constructor_count() == 0u,
            pi->copy_assign_count() == 0u,
            pi->move_assign_count() == 0u,
            p.deleter().constructor_count() == 0u,
            p.deleter().copy_constructor_count() == 0u,
            p.deleter().move_constructor_count() == 1u,
            p.deleter().copy_assign_count() == 0u,
            p.deleter().move_assign_count() == 0u,
        };
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_true(std::get<6>(result));
        hud_assert_true(std::get<7>(result));
        hud_assert_true(std::get<8>(result));
        hud_assert_true(std::get<9>(result));
        hud_assert_true(std::get<10>(result));
        hud_assert_true(std::get<11>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_true(std::get<6>(result));
        hud_assert_true(std::get<7>(result));
        hud_assert_true(std::get<8>(result));
        hud_assert_true(std::get<9>(result));
        hud_assert_true(std::get<10>(result));
        hud_assert_true(std::get<11>(result));
    }
}

GTEST_TEST(unique_pointer, constructor_with_pointer_and_different_deleter_by_move)
{
    const auto test = []()
    {
        static_assert(hud::is_move_constructible_v<hud_test::deleter_type>);
        hud_test::non_bitwise_type *pi = new hud_test::non_bitwise_type(123, nullptr);
        hud::unique_pointer<hud_test::non_bitwise_type, hud_test::deleter_type> p(pi, hud_test::deleter_type2 {});
        return std::tuple {
            p.pointer() == pi,
            hud::is_same_v<decltype(p.deleter()), hud_test::deleter_type &>,
            pi->constructor_count() == 1u,
            pi->copy_constructor_count() == 0u,
            pi->move_constructor_count() == 0u,
            pi->copy_assign_count() == 0u,
            pi->move_assign_count() == 0u,
            p.deleter().constructor_count() == 0u,
            p.deleter().copy_constructor_count() == 0u,
            p.deleter().move_constructor_count() == 1u,
            p.deleter().copy_assign_count() == 0u,
            p.deleter().move_assign_count() == 0u,
        };
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_true(std::get<6>(result));
        hud_assert_true(std::get<7>(result));
        hud_assert_true(std::get<8>(result));
        hud_assert_true(std::get<9>(result));
        hud_assert_true(std::get<10>(result));
        hud_assert_true(std::get<11>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_true(std::get<6>(result));
        hud_assert_true(std::get<7>(result));
        hud_assert_true(std::get<8>(result));
        hud_assert_true(std::get<9>(result));
        hud_assert_true(std::get<10>(result));
        hud_assert_true(std::get<11>(result));
    }
}

GTEST_TEST(unique_pointer, constructor_with_pointer_and_ref_same_deleter)
{
    const auto test = []()
    {
        hud_test::non_bitwise_type *pi = new hud_test::non_bitwise_type(123, nullptr);
        hud_test::deleter_type deleter;
        hud::unique_pointer<hud_test::non_bitwise_type, hud_test::deleter_type &> p(pi, deleter);
        return std::tuple {
            p.pointer() == pi,
            &(p.deleter()) == &deleter,
            pi->constructor_count() == 1u,
            pi->copy_constructor_count() == 0u,
            pi->move_constructor_count() == 0u,
            pi->copy_assign_count() == 0u,
            pi->move_assign_count() == 0u,
            p.deleter().constructor_count() == 0u,
            p.deleter().copy_constructor_count() == 0u,
            p.deleter().move_constructor_count() == 0u,
            p.deleter().copy_assign_count() == 0u,
            p.deleter().move_assign_count() == 0u,
            deleter.constructor_count() == 0u,
            deleter.copy_constructor_count() == 0u,
            deleter.move_constructor_count() == 0u,
            deleter.copy_assign_count() == 0u,
            deleter.move_assign_count() == 0u,
        };
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_true(std::get<6>(result));
        hud_assert_true(std::get<7>(result));
        hud_assert_true(std::get<8>(result));
        hud_assert_true(std::get<9>(result));
        hud_assert_true(std::get<10>(result));
        hud_assert_true(std::get<11>(result));
        hud_assert_true(std::get<12>(result));
        hud_assert_true(std::get<13>(result));
        hud_assert_true(std::get<14>(result));
        hud_assert_true(std::get<15>(result));
        hud_assert_true(std::get<16>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_true(std::get<6>(result));
        hud_assert_true(std::get<7>(result));
        hud_assert_true(std::get<8>(result));
        hud_assert_true(std::get<9>(result));
        hud_assert_true(std::get<10>(result));
        hud_assert_true(std::get<11>(result));
        hud_assert_true(std::get<12>(result));
        hud_assert_true(std::get<13>(result));
        hud_assert_true(std::get<14>(result));
        hud_assert_true(std::get<15>(result));
        hud_assert_true(std::get<16>(result));
    }
}

GTEST_TEST(unique_pointer, constructor_with_pointer_and_ref_different_deleter)
{
    const auto test = []()
    {
        hud_test::non_bitwise_type *pi = new hud_test::non_bitwise_type(123, nullptr);
        hud_test::deleter_type2 deleter;
        hud::unique_pointer<hud_test::non_bitwise_type, hud_test::deleter_type &> p(pi, deleter);
        return std::tuple {
            p.pointer() == pi,
            &(p.deleter()) == &deleter,
            pi->constructor_count() == 1u,
            pi->copy_constructor_count() == 0u,
            pi->move_constructor_count() == 0u,
            pi->copy_assign_count() == 0u,
            pi->move_assign_count() == 0u,
            p.deleter().constructor_count() == 0u,
            p.deleter().copy_constructor_count() == 0u,
            p.deleter().move_constructor_count() == 0u,
            p.deleter().copy_assign_count() == 0u,
            p.deleter().move_assign_count() == 0u,
            deleter.constructor_count() == 0u,
            deleter.copy_constructor_count() == 0u,
            deleter.move_constructor_count() == 0u,
            deleter.copy_assign_count() == 0u,
            deleter.move_assign_count() == 0u,
        };
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_true(std::get<6>(result));
        hud_assert_true(std::get<7>(result));
        hud_assert_true(std::get<8>(result));
        hud_assert_true(std::get<9>(result));
        hud_assert_true(std::get<10>(result));
        hud_assert_true(std::get<11>(result));
        hud_assert_true(std::get<12>(result));
        hud_assert_true(std::get<13>(result));
        hud_assert_true(std::get<14>(result));
        hud_assert_true(std::get<15>(result));
        hud_assert_true(std::get<16>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_true(std::get<6>(result));
        hud_assert_true(std::get<7>(result));
        hud_assert_true(std::get<8>(result));
        hud_assert_true(std::get<9>(result));
        hud_assert_true(std::get<10>(result));
        hud_assert_true(std::get<11>(result));
        hud_assert_true(std::get<12>(result));
        hud_assert_true(std::get<13>(result));
        hud_assert_true(std::get<14>(result));
        hud_assert_true(std::get<15>(result));
        hud_assert_true(std::get<16>(result));
    }
}

GTEST_TEST(unique_pointer, constructor_with_nullptr)
{
    const auto test = []()
    {
        hud::unique_pointer<hud_test::non_bitwise_type> p(nullptr);
        return p.pointer() == nullptr;
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(result);
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(result);
    }
}

GTEST_TEST(unique_pointer, constructor_with_nullptr_with_deleter)
{
    const auto test = []()
    {
        hud_test::deleter_type deleter;
        hud::unique_pointer<hud_test::non_bitwise_type, hud_test::deleter_type> p(nullptr, deleter);
        return p.pointer() == nullptr;
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(result);
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(result);
    }
}

GTEST_TEST(unique_pointer, constructor_with_nullptr_with_deleter_ref)
{
    const auto test = []()
    {
        hud_test::deleter_type deleter;
        hud::unique_pointer<hud_test::non_bitwise_type, hud_test::deleter_type &> p(nullptr, deleter);
        return p.pointer() == nullptr;
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(result);
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(result);
    }
}

GTEST_TEST(unique_pointer, move_constructor_same_type)
{
    const auto test = []()
    {
        hud_test::non_bitwise_type *pi = new hud_test::non_bitwise_type(123, nullptr);
        hud::unique_pointer<hud_test::non_bitwise_type, hud_test::deleter_type> other(pi);
        hud::unique_pointer<hud_test::non_bitwise_type, hud_test::deleter_type> p(hud::move(other));
        return std::tuple {
            other.pointer() == nullptr,
            p.pointer() == pi,
            p.deleter().constructor_count() == 0u,
            p.deleter().copy_constructor_count() == 0u,
            p.deleter().move_constructor_count() == 1u,
            p.deleter().copy_assign_count() == 0u,
            p.deleter().move_assign_count() == 0u,
            other.deleter().constructor_count() == 0u,
            other.deleter().copy_constructor_count() == 0u,
            other.deleter().move_constructor_count() == 0u,
            other.deleter().copy_assign_count() == 0u,
            other.deleter().move_assign_count() == 0u,
        };
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_true(std::get<6>(result));
        hud_assert_true(std::get<7>(result));
        hud_assert_true(std::get<8>(result));
        hud_assert_true(std::get<9>(result));
        hud_assert_true(std::get<10>(result));
        hud_assert_true(std::get<11>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_true(std::get<6>(result));
        hud_assert_true(std::get<7>(result));
        hud_assert_true(std::get<8>(result));
        hud_assert_true(std::get<9>(result));
        hud_assert_true(std::get<10>(result));
        hud_assert_true(std::get<11>(result));
    }
}

GTEST_TEST(unique_pointer, move_constructor_same_type_different_deleter)
{
    const auto test = []()
    {
        hud_test::non_bitwise_type *pi = new hud_test::non_bitwise_type(123, nullptr);
        hud::unique_pointer<hud_test::non_bitwise_type, hud_test::deleter_type2> other(pi);
        hud::unique_pointer<hud_test::non_bitwise_type, hud_test::deleter_type> p(hud::move(other));
        return std::tuple {
            other.pointer() == nullptr,
            p.pointer() == pi,
            p.deleter().constructor_count() == 0u,
            p.deleter().copy_constructor_count() == 0u,
            p.deleter().move_constructor_count() == 1u,
            p.deleter().copy_assign_count() == 0u,
            p.deleter().move_assign_count() == 0u,
            other.deleter().constructor_count() == 0u,
            other.deleter().copy_constructor_count() == 0u,
            other.deleter().move_constructor_count() == 0u,
            other.deleter().copy_assign_count() == 0u,
            other.deleter().move_assign_count() == 0u,
        };
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_true(std::get<6>(result));
        hud_assert_true(std::get<7>(result));
        hud_assert_true(std::get<8>(result));
        hud_assert_true(std::get<9>(result));
        hud_assert_true(std::get<10>(result));
        hud_assert_true(std::get<11>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_true(std::get<6>(result));
        hud_assert_true(std::get<7>(result));
        hud_assert_true(std::get<8>(result));
        hud_assert_true(std::get<9>(result));
        hud_assert_true(std::get<10>(result));
        hud_assert_true(std::get<11>(result));
    }
}

GTEST_TEST(unique_pointer, move_constructor_same_type_with_same_deleter_ref)
{
    const auto test = []()
    {
        hud_test::non_bitwise_type *pi = new hud_test::non_bitwise_type(123, nullptr);
        hud_test::deleter_type deleter;
        hud::unique_pointer<hud_test::non_bitwise_type, hud_test::deleter_type &> other(pi, deleter);
        hud::unique_pointer<hud_test::non_bitwise_type, hud_test::deleter_type &> p(hud::move(other));
        return std::tuple {
            other.pointer() == nullptr,
            p.pointer() == pi,
            &(p.deleter()) == &deleter,
            p.deleter().constructor_count() == 0u,
            p.deleter().copy_constructor_count() == 0u,
            p.deleter().move_constructor_count() == 0u,
            p.deleter().copy_assign_count() == 0u,
            p.deleter().move_assign_count() == 0u,
            other.deleter().constructor_count() == 0u,
            other.deleter().copy_constructor_count() == 0u,
            other.deleter().move_constructor_count() == 0u,
            other.deleter().copy_assign_count() == 0u,
            other.deleter().move_assign_count() == 0u,
        };
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_true(std::get<6>(result));
        hud_assert_true(std::get<7>(result));
        hud_assert_true(std::get<8>(result));
        hud_assert_true(std::get<9>(result));
        hud_assert_true(std::get<10>(result));
        hud_assert_true(std::get<11>(result));
        hud_assert_true(std::get<12>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_true(std::get<6>(result));
        hud_assert_true(std::get<7>(result));
        hud_assert_true(std::get<8>(result));
        hud_assert_true(std::get<9>(result));
        hud_assert_true(std::get<10>(result));
        hud_assert_true(std::get<11>(result));
        hud_assert_true(std::get<12>(result));
    }
}

GTEST_TEST(unique_pointer, move_constructor_same_type_with_different_deleter_ref)
{
    const auto test = []()
    {
        hud_test::non_bitwise_type *pi = new hud_test::non_bitwise_type(123, nullptr);
        hud_test::deleter_type2 deleter;
        hud::unique_pointer<hud_test::non_bitwise_type, hud_test::deleter_type2 &> other(pi, deleter);
        hud::unique_pointer<hud_test::non_bitwise_type, hud_test::deleter_type &> p(hud::move(other));
        return std::tuple {
            other.pointer() == nullptr,
            p.pointer() == pi,
            &(p.deleter()) == &deleter,
            p.deleter().constructor_count() == 0u,
            p.deleter().copy_constructor_count() == 0u,
            p.deleter().move_constructor_count() == 0u,
            p.deleter().copy_assign_count() == 0u,
            p.deleter().move_assign_count() == 0u,
            other.deleter().constructor_count() == 0u,
            other.deleter().copy_constructor_count() == 0u,
            other.deleter().move_constructor_count() == 0u,
            other.deleter().copy_assign_count() == 0u,
            other.deleter().move_assign_count() == 0u,
        };
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_true(std::get<6>(result));
        hud_assert_true(std::get<7>(result));
        hud_assert_true(std::get<8>(result));
        hud_assert_true(std::get<9>(result));
        hud_assert_true(std::get<10>(result));
        hud_assert_true(std::get<11>(result));
        hud_assert_true(std::get<12>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_true(std::get<6>(result));
        hud_assert_true(std::get<7>(result));
        hud_assert_true(std::get<8>(result));
        hud_assert_true(std::get<9>(result));
        hud_assert_true(std::get<10>(result));
        hud_assert_true(std::get<11>(result));
        hud_assert_true(std::get<12>(result));
    }
}

GTEST_TEST(unique_pointer, move_constructor_different_type_same_deleter)
{
    const auto test = []()
    {
        hud_test::non_bitwise_type2 *pi = new hud_test::non_bitwise_type2(123, nullptr);
        hud::unique_pointer<hud_test::non_bitwise_type2, hud_test::deleter_type> other(pi);
        hud::unique_pointer<hud_test::non_bitwise_type, hud_test::deleter_type> p(hud::move(other));
        return std::tuple {
            other.pointer() == nullptr,
            p.pointer() == pi,
            p.deleter().constructor_count() == 0u,
            p.deleter().copy_constructor_count() == 0u,
            p.deleter().move_constructor_count() == 1u,
            p.deleter().copy_assign_count() == 0u,
            p.deleter().move_assign_count() == 0u,
            other.deleter().constructor_count() == 0u,
            other.deleter().copy_constructor_count() == 0u,
            other.deleter().move_constructor_count() == 0u,
            other.deleter().copy_assign_count() == 0u,
            other.deleter().move_assign_count() == 0u,
        };
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_true(std::get<6>(result));
        hud_assert_true(std::get<7>(result));
        hud_assert_true(std::get<8>(result));
        hud_assert_true(std::get<9>(result));
        hud_assert_true(std::get<10>(result));
        hud_assert_true(std::get<11>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_true(std::get<6>(result));
        hud_assert_true(std::get<7>(result));
        hud_assert_true(std::get<8>(result));
        hud_assert_true(std::get<9>(result));
        hud_assert_true(std::get<10>(result));
        hud_assert_true(std::get<11>(result));
    }
}

GTEST_TEST(unique_pointer, move_constructor_different_type_different_deleter)
{
    const auto test = []()
    {
        hud_test::non_bitwise_type2 *pi = new hud_test::non_bitwise_type2(123, nullptr);
        hud::unique_pointer<hud_test::non_bitwise_type2, hud_test::deleter_type2> other(pi);
        hud::unique_pointer<hud_test::non_bitwise_type, hud_test::deleter_type> p(hud::move(other));
        return std::tuple {
            other.pointer() == nullptr,
            p.pointer() == pi,
            p.deleter().constructor_count() == 0u,
            p.deleter().copy_constructor_count() == 0u,
            p.deleter().move_constructor_count() == 1u,
            p.deleter().copy_assign_count() == 0u,
            p.deleter().move_assign_count() == 0u,
            other.deleter().constructor_count() == 0u,
            other.deleter().copy_constructor_count() == 0u,
            other.deleter().move_constructor_count() == 0u,
            other.deleter().copy_assign_count() == 0u,
            other.deleter().move_assign_count() == 0u,
        };
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_true(std::get<6>(result));
        hud_assert_true(std::get<7>(result));
        hud_assert_true(std::get<8>(result));
        hud_assert_true(std::get<9>(result));
        hud_assert_true(std::get<10>(result));
        hud_assert_true(std::get<11>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_true(std::get<6>(result));
        hud_assert_true(std::get<7>(result));
        hud_assert_true(std::get<8>(result));
        hud_assert_true(std::get<9>(result));
        hud_assert_true(std::get<10>(result));
        hud_assert_true(std::get<11>(result));
    }
}

GTEST_TEST(unique_pointer, move_constructor_different_type_with_same_deleter_ref)
{
    const auto test = []()
    {
        hud_test::non_bitwise_type2 *pi = new hud_test::non_bitwise_type2(123, nullptr);
        hud_test::deleter_type deleter;
        hud::unique_pointer<hud_test::non_bitwise_type2, hud_test::deleter_type &> other(pi, deleter);
        hud::unique_pointer<hud_test::non_bitwise_type, hud_test::deleter_type &> p(hud::move(other));
        return std::tuple {
            other.pointer() == nullptr,
            p.pointer() == pi,
            p.deleter().constructor_count() == 0u,
            p.deleter().copy_constructor_count() == 0u,
            p.deleter().move_constructor_count() == 0u,
            p.deleter().copy_assign_count() == 0u,
            p.deleter().move_assign_count() == 0u,
            other.deleter().constructor_count() == 0u,
            other.deleter().copy_constructor_count() == 0u,
            other.deleter().move_constructor_count() == 0u,
            other.deleter().copy_assign_count() == 0u,
            other.deleter().move_assign_count() == 0u,
        };
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_true(std::get<6>(result));
        hud_assert_true(std::get<7>(result));
        hud_assert_true(std::get<8>(result));
        hud_assert_true(std::get<9>(result));
        hud_assert_true(std::get<10>(result));
        hud_assert_true(std::get<11>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_true(std::get<6>(result));
        hud_assert_true(std::get<7>(result));
        hud_assert_true(std::get<8>(result));
        hud_assert_true(std::get<9>(result));
        hud_assert_true(std::get<10>(result));
        hud_assert_true(std::get<11>(result));
    }
}

GTEST_TEST(unique_pointer, move_constructor_different_type_with_dfferent_deleter_ref)
{
    const auto test = []()
    {
        hud_test::non_bitwise_type2 *pi = new hud_test::non_bitwise_type2(123, nullptr);
        hud_test::deleter_type2 deleter;
        hud::unique_pointer<hud_test::non_bitwise_type2, hud_test::deleter_type2 &> other(pi, deleter);
        hud::unique_pointer<hud_test::non_bitwise_type, hud_test::deleter_type &> p(hud::move(other));
        return std::tuple {
            other.pointer() == nullptr,
            p.pointer() == pi,
            p.deleter().constructor_count() == 0u,
            p.deleter().copy_constructor_count() == 0u,
            p.deleter().move_constructor_count() == 0u,
            p.deleter().copy_assign_count() == 0u,
            p.deleter().move_assign_count() == 0u,
            other.deleter().constructor_count() == 0u,
            other.deleter().copy_constructor_count() == 0u,
            other.deleter().move_constructor_count() == 0u,
            other.deleter().copy_assign_count() == 0u,
            other.deleter().move_assign_count() == 0u,
        };
    };

    // Non constant
    {
        const auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_true(std::get<6>(result));
        hud_assert_true(std::get<7>(result));
        hud_assert_true(std::get<8>(result));
        hud_assert_true(std::get<9>(result));
        hud_assert_true(std::get<10>(result));
        hud_assert_true(std::get<11>(result));
    }

    // Constant
    {
        constexpr auto result = test();
        hud_assert_true(std::get<0>(result));
        hud_assert_true(std::get<1>(result));
        hud_assert_true(std::get<2>(result));
        hud_assert_true(std::get<3>(result));
        hud_assert_true(std::get<4>(result));
        hud_assert_true(std::get<5>(result));
        hud_assert_true(std::get<6>(result));
        hud_assert_true(std::get<7>(result));
        hud_assert_true(std::get<8>(result));
        hud_assert_true(std::get<9>(result));
        hud_assert_true(std::get<10>(result));
        hud_assert_true(std::get<11>(result));
    }
}