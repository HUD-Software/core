#ifndef HD_INC_CORE_COMPRESSED_TYPE_H
#define HD_INC_CORE_COMPRESSED_TYPE_H
#include "../traits/is_empty.h"
#include "../traits/is_same.h"
#include "../traits/remove_cv_ref.h"
#include "../traits/is_nothrow_constructible.h"
#include "../traits/is_nothrow_default_constructible.h"

namespace hud
{

    namespace details::compressed_pair
    {
        template<typename compressed_type_1_t, typename compressed_type_2_t, bool = hud::is_empty_v<compressed_type_1_t>, bool = hud::is_empty_v<compressed_type_2_t>>
        class compress_pair_impl;

        /*
         * When compressed_type_1_t != compressed_type_2_t, compressed_type_1_t is not empty and compressed_type_2_t is not empty
         * @tparam compressed_type_1_t
         * @tparam compressed_type_2_t
         */
        template<typename compressed_type_1_t, typename compressed_type_2_t>
        class compress_pair_impl<compressed_type_1_t, compressed_type_2_t, false, false>
        {
        public:
            constexpr compress_pair_impl() = default;
            constexpr compress_pair_impl(const compress_pair_impl &) = default;
            constexpr compress_pair_impl(compress_pair_impl &&) = default;
            constexpr compress_pair_impl &operator=(const compress_pair_impl &) = default;
            constexpr compress_pair_impl &operator=(compress_pair_impl &&) = default;

            template<typename u_compressed_type_1_t, typename u_compressed_type_2_t>
            explicit constexpr compress_pair_impl(u_compressed_type_1_t &&type_1, u_compressed_type_2_t &&type_2) noexcept
                : type_1_(hud::forward<u_compressed_type_1_t>(type_1))
                , type_2_(hud::forward<u_compressed_type_2_t>(type_2))
            {
                static_assert(hud::is_nothrow_constructible_v<compressed_type_1_t, u_compressed_type_1_t>);
                static_assert(hud::is_nothrow_constructible_v<compressed_type_2_t, u_compressed_type_2_t>);
            }

            template<typename u_compressed_type_1_t>
            explicit constexpr compress_pair_impl(u_compressed_type_1_t &&type_1) noexcept
            requires(hud::is_same_v<compressed_type_1_t, hud::remove_cv_ref_t<u_compressed_type_1_t>>)
                : type_1_(hud::forward<u_compressed_type_1_t>(type_1))
                , type_2_()
            {
                static_assert(hud::is_nothrow_constructible_v<compressed_type_1_t, u_compressed_type_1_t>);
                static_assert(hud::is_nothrow_default_constructible_v<compressed_type_2_t>);
            }

            template<typename u_compressed_type_2_t>
            explicit constexpr compress_pair_impl(u_compressed_type_2_t &&type_2) noexcept
            requires(hud::is_same_v<compressed_type_2_t, hud::remove_cv_ref_t<u_compressed_type_2_t>>)
                : type_1_()
                , type_2_(hud::forward<u_compressed_type_2_t>(type_2))
            {
                static_assert(hud::is_nothrow_default_constructible_v<compressed_type_1_t>);
                static_assert(hud::is_nothrow_constructible_v<compressed_type_2_t, u_compressed_type_2_t>);
            }

            [[nodiscard]] constexpr compressed_type_1_t &first() noexcept
            {
                return type_1_;
            }

            [[nodiscard]] constexpr const compressed_type_1_t &first() const noexcept
            {
                return type_1_;
            }

            [[nodiscard]] constexpr compressed_type_2_t &second() noexcept
            {
                return type_2_;
            }

            [[nodiscard]] constexpr const compressed_type_2_t &second() const noexcept
            {
                return type_2_;
            }

        private:
            compressed_type_1_t type_1_;
            compressed_type_2_t type_2_;
        };

        /*
         * When compressed_type_1_t != compressed_type_2_t, compressed_type_1_t is empty and compressed_type_2_t is not empty
         * @tparam compressed_type_1_t
         * @tparam compressed_type_2_t
         */
        template<typename compressed_type_1_t, typename compressed_type_2_t>
        class compress_pair_impl<compressed_type_1_t, compressed_type_2_t, true, false>
            : compressed_type_1_t
        {
        public:
            constexpr compress_pair_impl() = default;
            constexpr compress_pair_impl(const compress_pair_impl &) = default;
            constexpr compress_pair_impl(compress_pair_impl &&) = default;
            constexpr compress_pair_impl &operator=(const compress_pair_impl &) = default;
            constexpr compress_pair_impl &operator=(compress_pair_impl &&) = default;

            template<typename u_compressed_type_1_t, typename u_compressed_type_2_t>
            explicit constexpr compress_pair_impl(u_compressed_type_1_t &&type_1, u_compressed_type_2_t &&type_2) noexcept
                : compressed_type_1_t(hud::forward<u_compressed_type_1_t>(type_1))
                , type_2_(hud::forward<u_compressed_type_2_t>(type_2))
            {
                static_assert(hud::is_nothrow_constructible_v<compressed_type_1_t, u_compressed_type_1_t>);
                static_assert(hud::is_nothrow_constructible_v<compressed_type_2_t, u_compressed_type_2_t>);
            }

            template<typename u_compressed_type_1_t>
            explicit constexpr compress_pair_impl(u_compressed_type_1_t &&type_1) noexcept
            requires(hud::is_same_v<compressed_type_1_t, hud::remove_cv_ref_t<u_compressed_type_1_t>>)
                : compressed_type_1_t(hud::forward<u_compressed_type_1_t>(type_1))
                , type_2_()
            {
                static_assert(hud::is_nothrow_constructible_v<compressed_type_1_t, u_compressed_type_1_t>);
                static_assert(hud::is_nothrow_default_constructible_v<compressed_type_2_t>);
            }

            template<typename u_compressed_type_2_t>
            explicit constexpr compress_pair_impl(u_compressed_type_2_t &&type_2) noexcept
            requires(hud::is_same_v<compressed_type_2_t, hud::remove_cv_ref_t<u_compressed_type_2_t>>)
                : compressed_type_1_t()
                , type_2_(hud::forward<u_compressed_type_2_t>(type_2))
            {
                static_assert(hud::is_nothrow_default_constructible_v<compressed_type_1_t>);
                static_assert(hud::is_nothrow_constructible_v<compressed_type_2_t, u_compressed_type_2_t>);
            }

            [[nodiscard]] constexpr compressed_type_1_t &first() noexcept
            {
                return *this;
            }

            [[nodiscard]] constexpr const compressed_type_1_t &first() const noexcept
            {
                return *this;
            }

            [[nodiscard]] constexpr compressed_type_2_t &second() noexcept
            {
                return type_2_;
            }

            [[nodiscard]] constexpr const compressed_type_2_t &second() const noexcept
            {
                return type_2_;
            }

        private:
            compressed_type_2_t type_2_;
        };

        /*
         * When compressed_type_1_t != compressed_type_2_t, compressed_type_1_t is not empty and compressed_type_2_t is empty
         * @tparam compressed_type_1_t
         * @tparam compressed_type_2_t
         */
        template<typename compressed_type_1_t, typename compressed_type_2_t>
        class compress_pair_impl<compressed_type_1_t, compressed_type_2_t, false, true>
            : compressed_type_2_t
        {
        public:
            constexpr compress_pair_impl() = default;
            constexpr compress_pair_impl(const compress_pair_impl &) = default;
            constexpr compress_pair_impl(compress_pair_impl &&) = default;
            constexpr compress_pair_impl &operator=(const compress_pair_impl &) = default;
            constexpr compress_pair_impl &operator=(compress_pair_impl &&) = default;

            template<typename u_compressed_type_1_t, typename u_compressed_type_2_t>
            explicit constexpr compress_pair_impl(u_compressed_type_1_t &&type_1, u_compressed_type_2_t &&type_2) noexcept
                : compressed_type_2_t(hud::forward<u_compressed_type_2_t>(type_2))
                , type_1_(hud::forward<u_compressed_type_1_t>(type_1))
            {
                static_assert(hud::is_nothrow_constructible_v<compressed_type_1_t, u_compressed_type_1_t>);
                static_assert(hud::is_nothrow_constructible_v<compressed_type_2_t, u_compressed_type_2_t>);
            }

            template<typename u_compressed_type_1_t>
            explicit constexpr compress_pair_impl(u_compressed_type_1_t &&type_1) noexcept
            requires(hud::is_same_v<compressed_type_1_t, hud::remove_cv_ref_t<u_compressed_type_1_t>>)
                : compressed_type_2_t()
                , type_1_(hud::forward<u_compressed_type_1_t>(type_1))
            {
                static_assert(hud::is_nothrow_constructible_v<compressed_type_1_t, u_compressed_type_1_t>);
                static_assert(hud::is_nothrow_default_constructible_v<compressed_type_2_t>);
            }

            template<typename u_compressed_type_2_t>
            explicit constexpr compress_pair_impl(u_compressed_type_2_t &&type_2) noexcept
            requires(hud::is_same_v<compressed_type_2_t, hud::remove_cv_ref_t<u_compressed_type_2_t>>)
                : compressed_type_2_t(hud::forward<u_compressed_type_2_t>(type_2))
                , compressed_type_1_t()
            {
                static_assert(hud::is_nothrow_default_constructible_v<compressed_type_1_t>);
                static_assert(hud::is_nothrow_constructible_v<compressed_type_2_t, u_compressed_type_2_t>);
            }

            [[nodiscard]] constexpr compressed_type_1_t &first() noexcept
            {
                return type_1_;
            }

            [[nodiscard]] constexpr const compressed_type_1_t &first() const noexcept
            {
                return type_1_;
            }

            [[nodiscard]] constexpr compressed_type_2_t &second() noexcept
            {
                return *this;
            }

            [[nodiscard]] constexpr const compressed_type_2_t &second() const noexcept
            {
                return *this;
            }

        private:
            compressed_type_1_t type_1_;
        };

        /*
         * When compressed_type_1_t != compressed_type_2_t, compressed_type_1_t is empty and compressed_type_2_t is empty
         * @tparam compressed_type_1_t
         * @tparam compressed_type_2_t
         */
        template<typename compressed_type_1_t, typename compressed_type_2_t>
        class compress_pair_impl<compressed_type_1_t, compressed_type_2_t, true, true>
            : compressed_type_1_t
            , compressed_type_2_t
        {
        public:
            constexpr compress_pair_impl() = default;
            constexpr compress_pair_impl(const compress_pair_impl &) = default;
            constexpr compress_pair_impl(compress_pair_impl &&) = default;
            constexpr compress_pair_impl &operator=(const compress_pair_impl &) = default;
            constexpr compress_pair_impl &operator=(compress_pair_impl &&) = default;

            template<typename u_compressed_type_1_t, typename u_compressed_type_2_t>
            explicit constexpr compress_pair_impl(u_compressed_type_1_t &&type_1, u_compressed_type_2_t &&type_2) noexcept
                : compressed_type_1_t(hud::forward<u_compressed_type_1_t>(type_1))
                , compressed_type_2_t(hud::forward<u_compressed_type_2_t>(type_2))
            {
                static_assert(hud::is_nothrow_constructible_v<compressed_type_1_t, u_compressed_type_1_t>);
                static_assert(hud::is_nothrow_constructible_v<compressed_type_2_t, u_compressed_type_2_t>);
            }

            template<typename u_compressed_type_1_t>
            explicit constexpr compress_pair_impl(u_compressed_type_1_t &&type_1) noexcept
            requires(hud::is_same_v<compressed_type_1_t, hud::remove_cv_ref_t<u_compressed_type_1_t>>)
                : compressed_type_1_t(hud::forward<u_compressed_type_1_t>(type_1))
                , compressed_type_2_t()
            {
                static_assert(hud::is_nothrow_constructible_v<compressed_type_1_t, u_compressed_type_1_t>);
                static_assert(hud::is_nothrow_default_constructible_v<compressed_type_2_t>);
            }

            template<typename u_compressed_type_2_t>
            explicit constexpr compress_pair_impl(u_compressed_type_2_t &&type_2) noexcept
            requires(hud::is_same_v<compressed_type_2_t, hud::remove_cv_ref_t<u_compressed_type_2_t>>)
                : compressed_type_1_t()
                , compressed_type_2_t(hud::forward<u_compressed_type_2_t>(type_2))
            {
                static_assert(hud::is_nothrow_default_constructible_v<compressed_type_1_t>);
                static_assert(hud::is_nothrow_constructible_v<compressed_type_2_t, u_compressed_type_2_t>);
            }

            [[nodiscard]] constexpr compressed_type_1_t &first() noexcept
            {
                return *this;
            }

            [[nodiscard]] constexpr const compressed_type_1_t &first() const noexcept
            {
                return *this;
            }

            [[nodiscard]] constexpr compressed_type_2_t &second() noexcept
            {
                return *this;
            }

            [[nodiscard]] constexpr const compressed_type_2_t &second() const noexcept
            {
                return *this;
            }
        };

        /*
         * When compressed_type_1_t == compressed_type_2_t, compressed_type_1_t is not empty and compressed_type_2_t is not empty
         * @tparam compressed_type_1_t
         * @tparam compressed_type_2_t
         */
        template<typename compressed_type_t>
        class compress_pair_impl<compressed_type_t, compressed_type_t, false, false>
        {
        public:
            constexpr compress_pair_impl() = default;
            constexpr compress_pair_impl(const compress_pair_impl &) = default;
            constexpr compress_pair_impl(compress_pair_impl &&) = default;
            constexpr compress_pair_impl &operator=(const compress_pair_impl &) = default;
            constexpr compress_pair_impl &operator=(compress_pair_impl &&) = default;

            template<typename u_compressed_type_t>
            explicit constexpr compress_pair_impl(u_compressed_type_t &&type_1, u_compressed_type_t &&type_2) noexcept
                : type_1_(hud::forward<u_compressed_type_t>(type_1))
                , type_2_(hud::forward<u_compressed_type_t>(type_2))
            {
                static_assert(hud::is_nothrow_constructible_v<compressed_type_t, u_compressed_type_t>);
                static_assert(hud::is_nothrow_constructible_v<compressed_type_t, u_compressed_type_t>);
            }

            [[nodiscard]] constexpr compressed_type_t &first() noexcept
            {
                return type_1_;
            }

            [[nodiscard]] constexpr const compressed_type_t &first() const noexcept
            {
                return type_1_;
            }

            [[nodiscard]] constexpr compressed_type_t &second() noexcept
            {
                return type_2_;
            }

            [[nodiscard]] constexpr const compressed_type_t &second() const noexcept
            {
                return type_2_;
            }

        private:
            compressed_type_t type_1_;
            compressed_type_t type_2_;
        };

        /*
         * When compressed_type_1_t == compressed_type_2_t, compressed_type_1_t is empty and compressed_type_2_t is not empty
         * This is not possible, if compressed_type_1_t == compressed_type_2_t there are both empty or not
         * @tparam compressed_type_1_t
         * @tparam compressed_type_2_t
         */
        template<typename compressed_type_t>
        class compress_pair_impl<compressed_type_t, compressed_type_t, true, false>;

        /*
         * When compressed_type_1_t == compressed_type_2_t, compressed_type_1_t is not empty and compressed_type_2_t is empty
         * This is not possible, if compressed_type_1_t == compressed_type_2_t there are both empty or not
         * @tparam compressed_type_1_t
         * @tparam compressed_type_2_t
         */
        template<typename compressed_type_t>
        class compress_pair_impl<compressed_type_t, compressed_type_t, false, true>;

        /*
         * When compressed_type_1_t == compressed_type_2_t, compressed_type_1_t is empty and compressed_type_2_t is empty
         * If both are compressed, &first() == &second(), to avoid this, compress only the first element, so &first() != &second().
         * @tparam compressed_type_1_t
         * @tparam compressed_type_2_t
         */
        template<typename compressed_type_t>
        class compress_pair_impl<compressed_type_t, compressed_type_t, true, true>
            : compressed_type_t
        {
        public:
            constexpr compress_pair_impl() = default;
            constexpr compress_pair_impl(const compress_pair_impl &) = default;
            constexpr compress_pair_impl(compress_pair_impl &&) = default;
            constexpr compress_pair_impl &operator=(const compress_pair_impl &) = default;
            constexpr compress_pair_impl &operator=(compress_pair_impl &&) = default;

            template<typename u_compressed_type_t>
            explicit constexpr compress_pair_impl(u_compressed_type_t &&type_1, u_compressed_type_t &&type_2) noexcept
                : compressed_type_t(hud::forward<u_compressed_type_t>(type_1))
                , type_2_(hud::forward<u_compressed_type_t>(type_2))
            {
                static_assert(hud::is_nothrow_constructible_v<compressed_type_t, u_compressed_type_t>);
                static_assert(hud::is_nothrow_constructible_v<compressed_type_t, u_compressed_type_t>);
            }

            [[nodiscard]] constexpr compressed_type_t &first() noexcept
            {
                return *this;
            }

            [[nodiscard]] constexpr const compressed_type_t &first() const noexcept
            {
                return *this;
            }

            [[nodiscard]] constexpr compressed_type_t &second() noexcept
            {
                return type_2_;
            }

            [[nodiscard]] constexpr const compressed_type_t &second() const noexcept
            {
                return type_2_;
            }

        private:
            compressed_type_t type_2_;
        };

    } // namespace details::compressed_pair

    /**
     * compressed_pair is very similar to hud::pair.
     * However, if either of the template arguments are empty classes, then the empty base-class optimisation is applied to compress the size of the pair.
     * The two members of the pair can be accessed using the member functions first() and second()
     */
    template<typename compressed_type_1_t, typename compressed_type_2_t>
    class compressed_pair
        : details::compressed_pair::compress_pair_impl<compressed_type_1_t, compressed_type_2_t>
    {
        using super_type = details::compressed_pair::compress_pair_impl<compressed_type_1_t, compressed_type_2_t>;

    public:
        constexpr compressed_pair() = default;
        constexpr compressed_pair(const compressed_pair &) = default;
        constexpr compressed_pair(compressed_pair &&) = default;
        constexpr compressed_pair &operator=(const compressed_pair &) = default;
        constexpr compressed_pair &operator=(compressed_pair &&) = default;

        template<typename u_compressed_type_1_t, typename u_compressed_type_2_t>
        explicit constexpr compressed_pair(u_compressed_type_1_t &&type_1, u_compressed_type_2_t &&type_2) noexcept
            : super_type(hud::forward<u_compressed_type_1_t>(type_1), hud::forward<u_compressed_type_2_t>(type_2))
        {
        }

        template<typename u_compressed_type_1_t>
        explicit constexpr compressed_pair(u_compressed_type_1_t &&type_1) noexcept
        requires(hud::is_same_v<compressed_type_1_t, hud::remove_cv_ref_t<u_compressed_type_1_t>>)
            : super_type(hud::forward<u_compressed_type_1_t>(type_1))
        {
        }

        template<typename u_compressed_type_2_t>
        explicit constexpr compressed_pair(u_compressed_type_2_t &&type_2) noexcept
        requires(hud::is_same_v<compressed_type_2_t, hud::remove_cv_ref_t<u_compressed_type_2_t>>)
            : super_type(hud::forward<u_compressed_type_2_t>(type_2))
        {
        }

        [[nodiscard]] constexpr compressed_type_1_t &first() noexcept
        {
            return super_type::first();
        }

        [[nodiscard]] constexpr const compressed_type_1_t &first() const noexcept
        {
            return super_type::first();
        }

        [[nodiscard]] constexpr compressed_type_2_t &second() noexcept
        {
            return super_type::second();
        }

        [[nodiscard]] constexpr const compressed_type_2_t &second() const noexcept
        {
            return super_type::second();
        }
    };

    template<typename compressed_type_t>
    class compressed_pair<compressed_type_t, compressed_type_t>
        : details::compressed_pair::compress_pair_impl<compressed_type_t, compressed_type_t>
    {
        using super_type = details::compressed_pair::compress_pair_impl<compressed_type_t, compressed_type_t>;

    public:
        constexpr compressed_pair() = default;

        template<typename u_compressed_type_t>
        explicit constexpr compressed_pair(u_compressed_type_t &&type_1, u_compressed_type_t &&type_2) noexcept
            : super_type(hud::forward<u_compressed_type_t>(type_1), hud::forward<u_compressed_type_t>(type_2))
        {
        }

        [[nodiscard]] constexpr compressed_type_t &first() noexcept
        {
            return super_type::first();
        }

        [[nodiscard]] constexpr const compressed_type_t &first() const noexcept
        {
            return super_type::first();
        }

        [[nodiscard]] constexpr compressed_type_t &second() noexcept
        {
            return super_type::second();
        }

        [[nodiscard]] constexpr const compressed_type_t &second() const noexcept
        {
            return super_type::second();
        }
    };

} // namespace hud

#endif // HD_INC_CORE_COMPRESSED_TYPE_H