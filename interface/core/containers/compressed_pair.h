#ifndef HD_INC_CORE_COMPRESSED_TYPE_H
#define HD_INC_CORE_COMPRESSED_TYPE_H
#include "../traits/is_empty.h"
#include "../traits/is_same.h"
#include "../traits/remove_cv_ref.h"
#include "../traits/is_nothrow_constructible.h"
#include "../traits/is_nothrow_default_constructible.h"
#include "../traits/is_nothrow_copy_constructible.h"
#include "../traits/is_nothrow_move_constructible.h"
#include "../traits/is_convertible.h"
#include "../traits/is_implicitly_default_constructible.h"

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
            using first_type = compressed_type_1_t;
            using second_type = compressed_type_2_t;

        public:
            /** Default constructor. */
            constexpr explicit(!(hud::is_implicitly_default_constructible_v<first_type> && hud::is_implicitly_default_constructible_v<second_type>))
                compress_pair_impl() noexcept
                : type_1_()
                , type_2_()
            {
                static_assert(hud::is_nothrow_default_constructible_v<first_type>, "first_type default constructor is throwable. compress_pair is not designed to allow throwable default constructible components");
                static_assert(hud::is_nothrow_default_constructible_v<second_type>, "second_type default constructor is throwable. compress_pair is not designed to allow throwable default constructible components");
            }

            /** Copy constructor. */
            constexpr explicit(!(hud::is_convertible_v<const first_type &, first_type> && hud::is_convertible_v<const second_type &, second_type>))
                compress_pair_impl(const compress_pair_impl &) = default;

            /** Move constructor. */
            constexpr explicit(!(hud::is_convertible_v<first_type, first_type> && hud::is_convertible_v<second_type, second_type>))
                compress_pair_impl(compress_pair_impl &&) = default;

            /**
             * Initialization move constructor.
             * This involves individually constructing its two components, with an initialization that depends on the constructor.
             * Member 'first' is constructed with 'f', and member 'second' with 's'.
             * compressed_pair do not accept throwable move-constructible components.
             * @tparam u_type_t Type of the 'f' component.
             * @tparam v_type_t Type of the 's' component.
             * @param f An object of the same type as 'first', or some other type implicitly convertible to it.
             * @param s An object of the same type as 'second', or some other type implicitly convertible to it.
             */
            template<typename u_compressed_type_1_t, typename u_compressed_type_2_t>
            requires(hud::is_move_constructible_v<first_type, u_compressed_type_1_t> && hud::is_move_constructible_v<second_type, u_compressed_type_2_t>)
            constexpr explicit(!(hud::is_convertible_v<u_compressed_type_1_t &&, first_type> && hud::is_convertible_v<u_compressed_type_2_t &&, u_compressed_type_1_t>))
                compress_pair_impl(u_compressed_type_1_t &&type_1, u_compressed_type_2_t &&type_2) noexcept
                : type_1_(hud::forward<u_compressed_type_1_t>(type_1))
                , type_2_(hud::forward<u_compressed_type_2_t>(type_2))
            {
                static_assert(hud::is_nothrow_constructible_v<first_type, u_compressed_type_1_t>, "first_type(const u_compressed_type_1_t&) copy constructor is throwable. compressed_pair is not designed to allow throwable copy constructible components");
                static_assert(hud::is_nothrow_constructible_v<second_type, u_compressed_type_2_t>, "second_type(const u_compressed_type_2_t&) copy constructor is throwable. compressed_pair is not designed to allow throwable copy constructible components");
            }

            /**
             * Initialization move constructor.
             * This involves individually constructing its two components, with an initialization that depends on the constructor.
             * Member 'first' is constructed with 'f', and member 'second' is default-constructed.
             * compressed_pair does not accept throwable move-constructible components.
             * @tparam u_compressed_type_1_t Type of the 'f' component.
             * @param f An object of the same type as 'first', or some other type implicitly convertible to it.
             */
            template<typename u_compressed_type_1_t>
            constexpr explicit(!(hud::is_convertible_v<u_compressed_type_1_t &&, first_type> && hud::is_implicitly_default_constructible_v<second_type>))
                compress_pair_impl(u_compressed_type_1_t &&type_1) noexcept
            requires(hud::is_same_v<first_type, hud::remove_cv_ref_t<u_compressed_type_1_t>>)
                : type_1_(hud::forward<u_compressed_type_1_t>(type_1))
                , type_2_()
            {
                static_assert(hud::is_nothrow_constructible_v<first_type, u_compressed_type_1_t>, "first_type(const u_compressed_type_1_t&) copy constructor is throwable. compressed_pair is not designed to allow throwable copy constructible components");
                static_assert(hud::is_nothrow_default_constructible_v<second_type>, "second_type default constructor is throwable. compress_pair is not designed to allow throwable default constructible components");
            }

            /**
             * Initialization move constructor.
             * This involves individually constructing its two components, with an initialization that depends on the constructor.
             * Member 'first' is default-constructed, and member 'second' is constructed with 's'.
             * compressed_pair does not accept throwable move-constructible components.
             * @tparam u_compressed_type_2_t Type of the 's' component.
             * @param s An object of the same type as 'second', or some other type implicitly convertible to it.
             */
            template<typename u_compressed_type_2_t>
            constexpr explicit(!(hud::is_implicitly_default_constructible_v<first_type> && hud::is_convertible_v<u_compressed_type_2_t &&, second_type>))
                compress_pair_impl(u_compressed_type_2_t &&type_2) noexcept
            requires(hud::is_same_v<second_type, hud::remove_cv_ref_t<u_compressed_type_2_t>>)
                : type_1_()
                , type_2_(hud::forward<u_compressed_type_2_t>(type_2))
            {
                static_assert(hud::is_nothrow_default_constructible_v<first_type>, "first_type default constructor is throwable. compress_pair is not designed to allow throwable default constructible components");
                static_assert(hud::is_nothrow_constructible_v<second_type, u_compressed_type_2_t>, "second_type(const u_compressed_type_2_t&) copy constructor is throwable. compressed_pair is not designed to allow throwable copy constructible components");
            }

            /** Copy assignment. */
            constexpr compress_pair_impl &operator=(const compress_pair_impl &) = default;
            /** Move assignment. */
            constexpr compress_pair_impl &operator=(compress_pair_impl &&) = default;

            /** Retrieves the reference to the first element. */
            [[nodiscard]] constexpr first_type &first() & noexcept
            {
                return type_1_;
            }

            /** Retrieves the const reference to the first element. */
            [[nodiscard]] constexpr const first_type &first() const & noexcept
            {
                return type_1_;
            }

            /** Retrieves the const l-value reference to the first element. */
            [[nodiscard]] constexpr const first_type &&first() const && noexcept
            {
                return std::move<first_type>(type_1_);
            }

            /** Retrieves the const l-value reference to the first element. */
            [[nodiscard]] constexpr first_type &&first() && noexcept
            {
                return std::move<first_type>(type_1_);
            }

            /** Retrieves the reference to the second element. */
            [[nodiscard]] constexpr second_type &second() & noexcept
            {
                return type_2_;
            }

            /** Retrieves the const reference to the second element. */
            [[nodiscard]] constexpr const second_type &second() const & noexcept
            {
                return type_2_;
            }

            /** Retrieves the const l-value reference to the first element. */
            [[nodiscard]] constexpr const second_type &&second() const && noexcept
            {
                return std::move<second_type>(type_2_);
            }

            /** Retrieves the const l-value reference to the first element. */
            [[nodiscard]] constexpr first_type &&second() && noexcept
            {
                return std::move<second_type>(type_2_);
            }

        private:
            first_type type_1_;
            second_type type_2_;
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
            using first_type = compressed_type_1_t;
            using second_type = compressed_type_2_t;

        public:
            /** Default constructor. */
            constexpr explicit(!(hud::is_implicitly_default_constructible_v<first_type> && hud::is_implicitly_default_constructible_v<second_type>))
                compress_pair_impl() noexcept
                : first_type()
                , type_2_()
            {
                static_assert(hud::is_nothrow_default_constructible_v<first_type>, "first_type default constructor is throwable. compress_pair is not designed to allow throwable default constructible components");
                static_assert(hud::is_nothrow_default_constructible_v<second_type>, "second_type default constructor is throwable. compress_pair is not designed to allow throwable default constructible components");
            }

            /** Copy constructor. */
            constexpr explicit(!(hud::is_convertible_v<const first_type &, first_type> && hud::is_convertible_v<const second_type &, second_type>))
                compress_pair_impl(const compress_pair_impl &) = default;

            /** Move constructor. */
            constexpr explicit(!(hud::is_convertible_v<first_type, first_type> && hud::is_convertible_v<second_type, second_type>))
                compress_pair_impl(compress_pair_impl &&) = default;

            /**
             * Initialization move constructor.
             * This involves individually constructing its two components, with an initialization that depends on the constructor.
             * Member 'first' is constructed with 'f', and member 'second' with 's'.
             * compressed_pair do not accept throwable move-constructible components.
             * @tparam u_type_t Type of the 'f' component.
             * @tparam v_type_t Type of the 's' component.
             * @param f An object of the same type as 'first', or some other type implicitly convertible to it.
             * @param s An object of the same type as 'second', or some other type implicitly convertible to it.
             */
            template<typename u_compressed_type_1_t, typename u_compressed_type_2_t>
            requires(hud::is_move_constructible_v<first_type, u_compressed_type_1_t> && hud::is_move_constructible_v<second_type, u_compressed_type_2_t>)
            constexpr explicit(!(hud::is_convertible_v<u_compressed_type_1_t &&, first_type> && hud::is_convertible_v<u_compressed_type_2_t &&, second_type>))
                compress_pair_impl(u_compressed_type_1_t &&type_1, u_compressed_type_2_t &&type_2) noexcept
                : first_type(hud::forward<u_compressed_type_1_t>(type_1))
                , type_2_(hud::forward<u_compressed_type_2_t>(type_2))
            {
                static_assert(hud::is_nothrow_constructible_v<first_type, u_compressed_type_1_t>, "first_type(const u_compressed_type_1_t&) copy constructor is throwable. compressed_pair is not designed to allow throwable copy constructible components");
                static_assert(hud::is_nothrow_constructible_v<second_type, u_compressed_type_2_t>, "second_type(const u_compressed_type_2_t&) copy constructor is throwable. compressed_pair is not designed to allow throwable copy constructible components");
            }

            /**
             * Initialization move constructor.
             * This involves individually constructing its two components, with an initialization that depends on the constructor.
             * Member 'first' is constructed with 'f', and member 'second' is default-constructed.
             * compressed_pair does not accept throwable move-constructible components.
             * @tparam u_compressed_type_1_t Type of the 'f' component.
             * @param f An object of the same type as 'first', or some other type implicitly convertible to it.
             */
            template<typename u_compressed_type_1_t>
            constexpr explicit(!(hud::is_convertible_v<u_compressed_type_1_t &&, first_type> && hud::is_implicitly_default_constructible_v<second_type>))
                compress_pair_impl(u_compressed_type_1_t &&type_1) noexcept
            requires(hud::is_same_v<first_type, hud::remove_cv_ref_t<u_compressed_type_1_t>>)
                : first_type(hud::forward<u_compressed_type_1_t>(type_1))
                , type_2_()
            {
                static_assert(hud::is_nothrow_constructible_v<first_type, u_compressed_type_1_t>, "first_type(const u_compressed_type_1_t&) copy constructor is throwable. compressed_pair is not designed to allow throwable copy constructible components");
                static_assert(hud::is_nothrow_default_constructible_v<second_type>, "second_type default constructor is throwable. compress_pair is not designed to allow throwable default constructible components");
            }

            /**
             * Initialization move constructor.
             * This involves individually constructing its two components, with an initialization that depends on the constructor.
             * Member 'first' is default-constructed, and member 'second' is constructed with 's'.
             * compressed_pair does not accept throwable move-constructible components.
             * @tparam u_compressed_type_2_t Type of the 's' component.
             * @param s An object of the same type as 'second', or some other type implicitly convertible to it.
             */
            template<typename u_compressed_type_2_t>
            constexpr explicit(!hud::is_implicitly_default_constructible_v<first_type> && (hud::is_convertible_v<u_compressed_type_2_t &&, second_type>))
                compress_pair_impl(u_compressed_type_2_t &&type_2) noexcept
            requires(hud::is_same_v<second_type, hud::remove_cv_ref_t<u_compressed_type_2_t>>)
                : first_type()
                , type_2_(hud::forward<u_compressed_type_2_t>(type_2))
            {
                static_assert(hud::is_nothrow_default_constructible_v<first_type>, "first_type default constructor is throwable. compress_pair is not designed to allow throwable default constructible components");
                static_assert(hud::is_nothrow_constructible_v<second_type, u_compressed_type_2_t>, "second_type(const u_compressed_type_2_t&) copy constructor is throwable. compressed_pair is not designed to allow throwable copy constructible components");
            }

            /** Copy assignment. */
            constexpr compress_pair_impl &operator=(const compress_pair_impl &) = default;
            /** Move assignment. */
            constexpr compress_pair_impl &operator=(compress_pair_impl &&) = default;

            /** Retrieves the reference to the first element. */
            [[nodiscard]] constexpr first_type &first() & noexcept
            {
                return *this;
            }

            /** Retrieves the const reference to the first element. */
            [[nodiscard]] constexpr const first_type &first() const & noexcept
            {
                return *this;
            }

            /** Retrieves the const l-value reference to the first element. */
            [[nodiscard]] constexpr const first_type &&first() const && noexcept
            {
                return std::move<first_type>(*this);
            }

            /** Retrieves the const l-value reference to the first element. */
            [[nodiscard]] constexpr first_type &&first() && noexcept
            {
                return std::move<first_type>(*this);
            }

            /** Retrieves the reference to the second element. */
            [[nodiscard]] constexpr second_type &second() & noexcept
            {
                return type_2_;
            }

            /** Retrieves the const reference to the second element. */
            [[nodiscard]] constexpr const second_type &second() const & noexcept
            {
                return type_2_;
            }

            /** Retrieves the const l-value reference to the first element. */
            [[nodiscard]] constexpr const second_type &&second() const && noexcept
            {
                return std::move<second_type>(type_2_);
            }

            /** Retrieves the const l-value reference to the first element. */
            [[nodiscard]] constexpr first_type &&second() && noexcept
            {
                return std::move<second_type>(type_2_);
            }

        private:
            second_type type_2_;
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
            using first_type = compressed_type_1_t;
            using second_type = compressed_type_2_t;

        public:
            /** Default constructor. */
            constexpr explicit(!(hud::is_implicitly_default_constructible_v<first_type> && hud::is_implicitly_default_constructible_v<second_type>))
                compress_pair_impl() noexcept
                : second_type()
                , type_1_()
            {
                static_assert(hud::is_nothrow_default_constructible_v<first_type>, "first_type default constructor is throwable. compress_pair is not designed to allow throwable default constructible components");
                static_assert(hud::is_nothrow_default_constructible_v<second_type>, "second_type default constructor is throwable. compress_pair is not designed to allow throwable default constructible components");
            }

            /** Copy constructor. */
            constexpr explicit(!(hud::is_convertible_v<const first_type &, first_type> && hud::is_convertible_v<const second_type &, second_type>))
                compress_pair_impl(const compress_pair_impl &) = default;

            /** Move constructor. */
            constexpr explicit(!(hud::is_convertible_v<first_type, first_type> && hud::is_convertible_v<second_type, second_type>))
                compress_pair_impl(compress_pair_impl &&) = default;

            /** Copy assignment. */
            constexpr compress_pair_impl &operator=(const compress_pair_impl &) = default;

            /** Move assignment. */
            constexpr compress_pair_impl &operator=(compress_pair_impl &&) = default;

            /**
             * Initialization move constructor.
             * This involves individually constructing its two components, with an initialization that depends on the constructor.
             * Member 'first' is constructed with 'f', and member 'second' with 's'.
             * compressed_pair do not accept throwable move-constructible components.
             * @tparam u_type_t Type of the 'f' component.
             * @tparam v_type_t Type of the 's' component.
             * @param f An object of the same type as 'first', or some other type implicitly convertible to it.
             * @param s An object of the same type as 'second', or some other type implicitly convertible to it.
             */
            template<typename u_compressed_type_1_t, typename u_compressed_type_2_t>
            requires(hud::is_move_constructible_v<first_type, u_compressed_type_1_t> && hud::is_move_constructible_v<second_type, u_compressed_type_2_t>)
            constexpr explicit(!(hud::is_convertible_v<u_compressed_type_1_t &&, first_type> && hud::is_convertible_v<u_compressed_type_2_t &&, second_type>))
                compress_pair_impl(u_compressed_type_1_t &&type_1, u_compressed_type_2_t &&type_2) noexcept
                : second_type(hud::forward<u_compressed_type_2_t>(type_2))
                , type_1_(hud::forward<u_compressed_type_1_t>(type_1))
            {
                static_assert(hud::is_nothrow_constructible_v<first_type, u_compressed_type_1_t>, "first_type(const u_compressed_type_1_t&) copy constructor is throwable. compressed_pair is not designed to allow throwable copy constructible components");
                static_assert(hud::is_nothrow_constructible_v<second_type, u_compressed_type_2_t>, "second_type(const u_compressed_type_2_t&) copy constructor is throwable. compressed_pair is not designed to allow throwable copy constructible components");
            }

            /**
             * Initialization move constructor.
             * This involves individually constructing its two components, with an initialization that depends on the constructor.
             * Member 'first' is constructed with 'f', and member 'second' is default-constructed.
             * compressed_pair does not accept throwable move-constructible components.
             * @tparam u_compressed_type_1_t Type of the 'f' component.
             * @param f An object of the same type as 'first', or some other type implicitly convertible to it.
             */
            template<typename u_compressed_type_1_t>
            constexpr explicit(!(hud::is_convertible_v<u_compressed_type_1_t &&, first_type> && hud::is_implicitly_default_constructible_v<second_type>))
                compress_pair_impl(u_compressed_type_1_t &&type_1) noexcept
            requires(hud::is_same_v<first_type, hud::remove_cv_ref_t<u_compressed_type_1_t>>)
                : second_type()
                , type_1_(hud::forward<u_compressed_type_1_t>(type_1))
            {
                static_assert(hud::is_nothrow_constructible_v<first_type, u_compressed_type_1_t>, "first_type(const u_compressed_type_1_t&) copy constructor is throwable. compressed_pair is not designed to allow throwable copy constructible components");
                static_assert(hud::is_nothrow_default_constructible_v<second_type>, "second_type default constructor is throwable. compress_pair is not designed to allow throwable default constructible components");
            }

            /**
             * Initialization move constructor.
             * This involves individually constructing its two components, with an initialization that depends on the constructor.
             * Member 'first' is default-constructed, and member 'second' is constructed with 's'.
             * compressed_pair does not accept throwable move-constructible components.
             * @tparam u_compressed_type_2_t Type of the 's' component.
             * @param s An object of the same type as 'second', or some other type implicitly convertible to it.
             */
            template<typename u_compressed_type_2_t>
            constexpr explicit(!(hud::is_implicitly_default_constructible_v<first_type> && hud::is_convertible_v<u_compressed_type_2_t &&, second_type>))
                compress_pair_impl(u_compressed_type_2_t &&type_2) noexcept
            requires(hud::is_same_v<second_type, hud::remove_cv_ref_t<u_compressed_type_2_t>>)
                : second_type(hud::forward<u_compressed_type_2_t>(type_2))
                , first_type()
            {
                static_assert(hud::is_nothrow_default_constructible_v<first_type>, "first_type default constructor is throwable. compress_pair is not designed to allow throwable default constructible components");
                static_assert(hud::is_nothrow_constructible_v<second_type, u_compressed_type_2_t>, "second_type(const u_compressed_type_2_t&) copy constructor is throwable. compressed_pair is not designed to allow throwable copy constructible components");
            }

            /** Retrieves the reference to the first element. */
            [[nodiscard]] constexpr first_type &first() & noexcept
            {
                return type_1_;
            }

            /** Retrieves the const reference to the first element. */
            [[nodiscard]] constexpr const first_type &first() const & noexcept
            {
                return type_1_;
            }

            /** Retrieves the const l-value reference to the first element. */
            [[nodiscard]] constexpr const first_type &&first() const && noexcept
            {
                return std::move<first_type>(type_1_);
            }

            /** Retrieves the const l-value reference to the first element. */
            [[nodiscard]] constexpr first_type &&first() && noexcept
            {
                return std::move<first_type>(type_1_);
            }

            /** Retrieves the reference to the second element. */
            [[nodiscard]] constexpr second_type &second() & noexcept
            {
                return *this;
            }

            /** Retrieves the const reference to the second element. */
            [[nodiscard]] constexpr const second_type &second() const & noexcept
            {
                return *this;
            }

            /** Retrieves the const l-value reference to the first element. */
            [[nodiscard]] constexpr const second_type &&second() const && noexcept
            {
                return std::move<second_type>(*this);
            }

            /** Retrieves the const l-value reference to the first element. */
            [[nodiscard]] constexpr first_type &&second() && noexcept
            {
                return std::move<second_type>(*this);
            }

        private:
            first_type type_1_;
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
            using first_type = compressed_type_1_t;
            using second_type = compressed_type_2_t;

        public:
            /** Default constructor. */
            constexpr explicit(!(hud::is_implicitly_default_constructible_v<first_type> && hud::is_implicitly_default_constructible_v<second_type>))
                compress_pair_impl() noexcept
                : first_type()
                , second_type()
            {
                static_assert(hud::is_nothrow_default_constructible_v<first_type>, "first_type default constructor is throwable. compress_pair is not designed to allow throwable default constructible components");
                static_assert(hud::is_nothrow_default_constructible_v<second_type>, "second_type default constructor is throwable. compress_pair is not designed to allow throwable default constructible components");
            }

            /** Copy constructor. */
            constexpr explicit(!(hud::is_convertible_v<const first_type &, first_type> && hud::is_convertible_v<const second_type &, second_type>))
                compress_pair_impl(const compress_pair_impl &) = default;

            /** Move constructor. */
            constexpr explicit(!(hud::is_convertible_v<first_type, first_type> && hud::is_convertible_v<second_type, second_type>))
                compress_pair_impl(compress_pair_impl &&) = default;

            /** Copy assignment. */
            constexpr compress_pair_impl &operator=(const compress_pair_impl &) = default;

            /** Move assignment. */
            constexpr compress_pair_impl &operator=(compress_pair_impl &&) = default;

            /**
             * Initialization move constructor.
             * This involves individually constructing its two components, with an initialization that depends on the constructor.
             * Member 'first' is constructed with 'f', and member 'second' with 's'.
             * compressed_pair do not accept throwable move-constructible components.
             * @tparam u_type_t Type of the 'f' component.
             * @tparam v_type_t Type of the 's' component.
             * @param f An object of the same type as 'first', or some other type implicitly convertible to it.
             * @param s An object of the same type as 'second', or some other type implicitly convertible to it.
             */
            template<typename u_compressed_type_1_t, typename u_compressed_type_2_t>
            requires(hud::is_move_constructible_v<first_type, u_compressed_type_1_t> && hud::is_move_constructible_v<second_type, u_compressed_type_2_t>)
            constexpr explicit(!(hud::is_convertible_v<u_compressed_type_1_t &&, first_type> && hud::is_convertible_v<u_compressed_type_2_t &&, second_type>))
                compress_pair_impl(u_compressed_type_1_t &&type_1, u_compressed_type_2_t &&type_2) noexcept
                : first_type(hud::forward<u_compressed_type_1_t>(type_1))
                , second_type(hud::forward<u_compressed_type_2_t>(type_2))
            {
                static_assert(hud::is_nothrow_constructible_v<first_type, u_compressed_type_1_t>, "first_type(const u_compressed_type_1_t&) copy constructor is throwable. compressed_pair is not designed to allow throwable copy constructible components");
                static_assert(hud::is_nothrow_constructible_v<second_type, u_compressed_type_2_t>, "second_type(const u_compressed_type_2_t&) copy constructor is throwable. compressed_pair is not designed to allow throwable copy constructible components");
            }

            /**
             * Initialization move constructor.
             * This involves individually constructing its two components, with an initialization that depends on the constructor.
             * Member 'first' is constructed with 'f', and member 'second' is default-constructed.
             * compressed_pair does not accept throwable move-constructible components.
             * @tparam u_compressed_type_1_t Type of the 'f' component.
             * @param f An object of the same type as 'first', or some other type implicitly convertible to it.
             */
            template<typename u_compressed_type_1_t>
            constexpr explicit(!(hud::is_convertible_v<u_compressed_type_1_t &&, first_type> && hud::is_implicitly_default_constructible_v<second_type>))
                compress_pair_impl(u_compressed_type_1_t &&type_1) noexcept
            requires(hud::is_same_v<first_type, hud::remove_cv_ref_t<u_compressed_type_1_t>>)
                : first_type(hud::forward<u_compressed_type_1_t>(type_1))
                , second_type()
            {
                static_assert(hud::is_nothrow_constructible_v<first_type, u_compressed_type_1_t>, "first_type(const u_compressed_type_1_t&) copy constructor is throwable. compressed_pair is not designed to allow throwable copy constructible components");
                static_assert(hud::is_nothrow_default_constructible_v<second_type>, "second_type default constructor is throwable. compress_pair is not designed to allow throwable default constructible components");
            }

            /**
             * Initialization move constructor.
             * This involves individually constructing its two components, with an initialization that depends on the constructor.
             * Member 'first' is default-constructed, and member 'second' is constructed with 's'.
             * compressed_pair does not accept throwable move-constructible components.
             * @tparam u_compressed_type_2_t Type of the 's' component.
             * @param s An object of the same type as 'second', or some other type implicitly convertible to it.
             */
            template<typename u_compressed_type_2_t>
            constexpr explicit(!(hud::is_implicitly_default_constructible_v<first_type> && hud::is_convertible_v<u_compressed_type_2_t &&, second_type>))
                compress_pair_impl(u_compressed_type_2_t &&type_2) noexcept
            requires(hud::is_same_v<second_type, hud::remove_cv_ref_t<u_compressed_type_2_t>>)
                : first_type()
                , second_type(hud::forward<u_compressed_type_2_t>(type_2))
            {
                static_assert(hud::is_nothrow_default_constructible_v<first_type>, "first_type default constructor is throwable. compress_pair is not designed to allow throwable default constructible components");
                static_assert(hud::is_nothrow_constructible_v<second_type, u_compressed_type_2_t>, "second_type(const u_compressed_type_2_t&) copy constructor is throwable. compressed_pair is not designed to allow throwable copy constructible components");
            }

            /** Retrieves the reference to the first element. */
            [[nodiscard]] constexpr first_type &first() & noexcept
            {
                return *this;
            }

            /** Retrieves the const reference to the first element. */
            [[nodiscard]] constexpr const first_type &first() const & noexcept
            {
                return *this;
            }

            /** Retrieves the const l-value reference to the first element. */
            [[nodiscard]] constexpr const first_type &&first() const && noexcept
            {
                return std::move<first_type>(*this);
            }

            /** Retrieves the const l-value reference to the first element. */
            [[nodiscard]] constexpr first_type &&first() && noexcept
            {
                return std::move<first_type>(*this);
            }

            /** Retrieves the reference to the second element. */
            [[nodiscard]] constexpr second_type &second() & noexcept
            {
                return *this;
            }

            /** Retrieves the const reference to the second element. */
            [[nodiscard]] constexpr const second_type &second() const & noexcept
            {
                return *this;
            }

            /** Retrieves the const l-value reference to the first element. */
            [[nodiscard]] constexpr const second_type &&second() const && noexcept
            {
                return std::move<second_type>(*this);
            }

            /** Retrieves the const l-value reference to the first element. */
            [[nodiscard]] constexpr first_type &&second() && noexcept
            {
                return std::move<second_type>(*this);
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
            using first_type = compressed_type_t;
            using second_type = compressed_type_t;

        public:
            /** Default constructor. */
            constexpr explicit(!(hud::is_implicitly_default_constructible_v<first_type> && hud::is_implicitly_default_constructible_v<second_type>))
                compress_pair_impl() noexcept
                : type_1_()
                , type_2_()
            {
                static_assert(hud::is_nothrow_default_constructible_v<first_type>, "first_type default constructor is throwable. compress_pair is not designed to allow throwable default constructible components");
                static_assert(hud::is_nothrow_default_constructible_v<second_type>, "second_type default constructor is throwable. compress_pair is not designed to allow throwable default constructible components");
            }

            /** Copy constructor. */
            constexpr explicit(!(hud::is_convertible_v<const first_type &, first_type> && hud::is_convertible_v<const second_type &, second_type>))
                compress_pair_impl(const compress_pair_impl &) = default;

            /** Move constructor. */
            constexpr explicit(!(hud::is_convertible_v<first_type, first_type> && hud::is_convertible_v<second_type, second_type>))
                compress_pair_impl(compress_pair_impl &&) = default;

            /** Copy assignment. */
            constexpr compress_pair_impl &operator=(const compress_pair_impl &) = default;

            /** Move assignment. */
            constexpr compress_pair_impl &operator=(compress_pair_impl &&) = default;

            /**
             * Initialization move constructor.
             * This involves individually constructing its two components, with an initialization that depends on the constructor.
             * Member 'first' is constructed with 'f', and member 'second' with 's'.
             * compressed_pair do not accept throwable move-constructible components.
             * @tparam u_type_t Type of the 'f' component.
             * @tparam v_type_t Type of the 's' component.
             * @param f An object of the same type as 'first', or some other type implicitly convertible to it.
             * @param s An object of the same type as 'second', or some other type implicitly convertible to it.
             */
            template<typename u_compressed_type_1_t, typename u_compressed_type_2_t>
            requires(hud::is_move_constructible_v<first_type, u_compressed_type_1_t> && hud::is_move_constructible_v<second_type, u_compressed_type_2_t>)
            constexpr explicit(!(hud::is_convertible_v<u_compressed_type_1_t &&, first_type> && hud::is_convertible_v<u_compressed_type_1_t &&, second_type>))
                compress_pair_impl(u_compressed_type_1_t &&type_1, u_compressed_type_2_t &&type_2) noexcept
                : type_1_(hud::forward<u_compressed_type_1_t>(type_1))
                , type_2_(hud::forward<u_compressed_type_2_t>(type_2))
            {
                static_assert(hud::is_nothrow_constructible_v<first_type, u_compressed_type_1_t>, "first_type(const u_compressed_type_1_t&) copy constructor is throwable. compressed_pair is not designed to allow throwable copy constructible components");
                static_assert(hud::is_nothrow_constructible_v<second_type, u_compressed_type_2_t>, "second_type(const u_compressed_type_2_t&) copy constructor is throwable. compressed_pair is not designed to allow throwable copy constructible components");
            }

            /** Retrieves the reference to the first element. */
            [[nodiscard]] constexpr first_type &first() & noexcept
            {
                return type_1_;
            }

            /** Retrieves the const reference to the first element. */
            [[nodiscard]] constexpr const first_type &first() const & noexcept
            {
                return type_1_;
            }

            /** Retrieves the const l-value reference to the first element. */
            [[nodiscard]] constexpr const first_type &&first() const && noexcept
            {
                return std::move<first_type>(type_1_);
            }

            /** Retrieves the const l-value reference to the first element. */
            [[nodiscard]] constexpr first_type &&first() && noexcept
            {
                return std::move<first_type>(type_1_);
            }

            /** Retrieves the reference to the second element. */
            [[nodiscard]] constexpr second_type &second() & noexcept
            {
                return type_2_;
            }

            /** Retrieves the const reference to the second element. */
            [[nodiscard]] constexpr const second_type &second() const & noexcept
            {
                return type_2_;
            }

            /** Retrieves the const l-value reference to the first element. */
            [[nodiscard]] constexpr const second_type &&second() const && noexcept
            {
                return std::move<second_type>(type_2_);
            }

            /** Retrieves the const l-value reference to the first element. */
            [[nodiscard]] constexpr first_type &&second() && noexcept
            {
                return std::move<second_type>(type_2_);
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
            using first_type = compressed_type_t;
            using second_type = compressed_type_t;

        public:
            /** Default constructor. */
            constexpr explicit(!(hud::is_implicitly_default_constructible_v<first_type> && hud::is_implicitly_default_constructible_v<second_type>))
                compress_pair_impl() noexcept
                : first_type()
                , type_2_()
            {
                static_assert(hud::is_nothrow_default_constructible_v<first_type>, "first_type default constructor is throwable. compress_pair is not designed to allow throwable default constructible components");
                static_assert(hud::is_nothrow_default_constructible_v<second_type>, "second_type default constructor is throwable. compress_pair is not designed to allow throwable default constructible components");
            }

            /** Copy constructor. */
            constexpr explicit(!(hud::is_convertible_v<const first_type &, first_type> && hud::is_convertible_v<const second_type &, second_type>))
                compress_pair_impl(const compress_pair_impl &) = default;

            /** Move constructor. */
            constexpr explicit(!(hud::is_convertible_v<first_type, first_type> && hud::is_convertible_v<second_type, second_type>))
                compress_pair_impl(compress_pair_impl &&) = default;

            /** Copy assignment. */
            constexpr compress_pair_impl &operator=(const compress_pair_impl &) = default;

            /** Move assignment. */
            constexpr compress_pair_impl &operator=(compress_pair_impl &&) = default;

            /**
             * Initialization move constructor.
             * This involves individually constructing its two components, with an initialization that depends on the constructor.
             * Member 'first' is constructed with 'f', and member 'second' with 's'.
             * compressed_pair do not accept throwable move-constructible components.
             * @tparam u_type_t Type of the 'f' component.
             * @tparam v_type_t Type of the 's' component.
             * @param f An object of the same type as 'first', or some other type implicitly convertible to it.
             * @param s An object of the same type as 'second', or some other type implicitly convertible to it.
             */
            template<typename u_compressed_type_1_t, typename u_compressed_type_2_t>
            requires(hud::is_move_constructible_v<first_type, u_compressed_type_1_t> && hud::is_move_constructible_v<second_type, u_compressed_type_2_t>)
            constexpr explicit(!(hud::is_convertible_v<u_compressed_type_1_t &&, first_type> && hud::is_convertible_v<u_compressed_type_2_t &&, second_type>))
                compress_pair_impl(u_compressed_type_1_t &&type_1, u_compressed_type_2_t &&type_2) noexcept
                : compressed_type_t(hud::forward<u_compressed_type_1_t>(type_1))
                , type_2_(hud::forward<u_compressed_type_2_t>(type_2))
            {
                static_assert(hud::is_nothrow_constructible_v<first_type, u_compressed_type_1_t>, "first_type(const u_compressed_type_1_t&) copy constructor is throwable. compressed_pair is not designed to allow throwable copy constructible components");
                static_assert(hud::is_nothrow_constructible_v<second_type, u_compressed_type_2_t>, "second_type(const u_compressed_type_2_t&) copy constructor is throwable. compressed_pair is not designed to allow throwable copy constructible components");
            }

            /** Retrieves the reference to the first element. */
            [[nodiscard]] constexpr first_type &first() & noexcept
            {
                return *this;
            }

            /** Retrieves the const reference to the first element. */
            [[nodiscard]] constexpr const first_type &first() const & noexcept
            {
                return *this;
            }

            /** Retrieves the const l-value reference to the first element. */
            [[nodiscard]] constexpr const first_type &&first() const && noexcept
            {
                return std::move<first_type>(*this);
            }

            /** Retrieves the const l-value reference to the first element. */
            [[nodiscard]] constexpr first_type &&first() && noexcept
            {
                return std::move<first_type>(*this);
            }

            /** Retrieves the reference to the second element. */
            [[nodiscard]] constexpr second_type &second() & noexcept
            {
                return type_2_;
            }

            /** Retrieves the const reference to the second element. */
            [[nodiscard]] constexpr const second_type &second() const & noexcept
            {
                return type_2_;
            }

            /** Retrieves the const l-value reference to the first element. */
            [[nodiscard]] constexpr const second_type &&second() const && noexcept
            {
                return std::move<second_type>(type_2_);
            }

            /** Retrieves the const l-value reference to the first element. */
            [[nodiscard]] constexpr first_type &&second() && noexcept
            {
                return std::move<second_type>(type_2_);
            }

        private:
            compressed_type_t type_2_;
        };

    } // namespace details::compressed_pair

    /**
     * `compressed_pair` is closely related to `hud::pair`.
     * However, if either of the template arguments represents an empty class, the Empty Base Class Optimization (EBCO) is applied to reduce the size of the pair.
     * The two members of the pair can be accessed using the member functions `first()` and `second()`.
     */
    template<typename compressed_type_1_t, typename compressed_type_2_t>
    class compressed_pair
        : details::compressed_pair::compress_pair_impl<compressed_type_1_t, compressed_type_2_t>
    {
        using super_type = details::compressed_pair::compress_pair_impl<compressed_type_1_t, compressed_type_2_t>;

    public:
        /** The type of 'first' */
        using first_type = typename super_type::first_type;
        /** The type of 'second' */
        using second_type = typename super_type::second_type;

    public:
        /** Default construct first and second */
        constexpr explicit(!(hud::is_implicitly_default_constructible_v<first_type> && hud::is_implicitly_default_constructible_v<second_type>))
            compressed_pair() = default;

        /**
         * Copy constructor.
         * This involves individually constructing its two components, with an initialization that depends on the constructor.
         * The object is initialized with the contents of the other compressed_pair object.
         * The corresponding members of 'other' are passed to the constructor of each of its members.
         * compressed_pair does not accept throwable default constructible components.
         * @param other Another compressed_pair object.
         */
        constexpr explicit(!(hud::is_convertible_v<const first_type &, first_type> && hud::is_convertible_v<const second_type &, second_type>))
            compressed_pair(const compressed_pair &other)
        requires(hud::is_nothrow_copy_constructible_v<first_type> && hud::is_nothrow_copy_constructible_v<second_type>)
        = default;

        /**
         * This involves individually constructing its two components, with an initialization that depends on the constructor.
         * The object is initialized with the contents of the other compressed_pair object.
         * The corresponding members of 'other' are passed to the constructor of each of its members.
         * Compressed pairs do not accept throwable copy-constructible components.
         * @tparam u_compressed_type_1_t Type of the first component.
         * @tparam u_compressed_type_2_t Type of the second component.
         * @param other Another compressed_pair object.
         */
        template<typename u_compressed_type_1_t, typename u_compressed_type_2_t>
        requires(hud::is_copy_constructible_v<first_type, u_compressed_type_1_t> && hud::is_copy_constructible_v<second_type, u_compressed_type_2_t>)
        constexpr explicit(!(hud::is_convertible_v<const u_compressed_type_1_t &, first_type> && hud::is_convertible_v<const u_compressed_type_2_t &, second_type>))
            compressed_pair(const compressed_pair<u_compressed_type_1_t, u_compressed_type_2_t> &other) noexcept
            : super_type(other)
        {
            static_assert(hud::is_nothrow_copy_constructible_v<first_type, u_compressed_type_1_t>, "first_type(const u_compressed_type_1_t&) copy constructor is throwable. compressed_pair is not designed to allow throwable copy constructible components");
            static_assert(hud::is_nothrow_copy_constructible_v<second_type, u_compressed_type_2_t>, "second_type(const u_compressed_type_2_t&) copy constructor is throwable. compressed_pair is not designed to allow throwable copy constructible components");
        }

        /**
         * Move constructor.
         * This involves individually constructing its two components, with an initialization that depends on the constructor.
         * The object is initialized with the contents of the other compressed_pair object.
         * The corresponding members of 'other' are forwarded to the constructor of each of its members.
         * Pairs do not accept throwable move-constructible components.
         * @param other Another compressed_pair object.
         */
        constexpr explicit(!(hud::is_convertible_v<first_type, first_type> && hud::is_convertible_v<second_type, second_type>))
            compressed_pair(compressed_pair &&other)
        requires(hud::is_nothrow_move_constructible_v<second_type> && hud::is_nothrow_move_constructible_v<second_type>)
        = default;

        /**
         * Move constructor.
         * This involves individually constructing its two components, with an initialization that depends on the constructor.
         * The object is initialized with the contents of the other compressed_pair object.
         * The corresponding members of 'other' are forwarded to the constructor of each of its members.
         * Pairs do not accept throwable move-constructible components.
         * @tparam u_type1_t Type of the first component.
         * @tparam u_type2_t Type of the second component.
         * @param other Another compressed_pair object.
         */
        template<typename u_compressed_type_1_t, typename u_compressed_type_2_t>
        requires(hud::is_move_constructible_v<first_type, u_compressed_type_1_t> && hud::is_move_constructible_v<second_type, u_compressed_type_2_t>)
        constexpr explicit(!(hud::is_convertible_v<u_compressed_type_1_t, first_type> && hud::is_convertible_v<u_compressed_type_2_t, second_type>))
            compressed_pair(compressed_pair<u_compressed_type_1_t, u_compressed_type_2_t> &&other) noexcept
            : super_type(hud::forward<compressed_pair<u_compressed_type_1_t, u_compressed_type_2_t>>(other))
        {
            static_assert(hud::is_nothrow_move_constructible_v<first_type, u_compressed_type_1_t>, "first_type(u_type1_t&&) move constructor is throwable. pair is not designed to allow throwable move constructible components");
            static_assert(hud::is_nothrow_move_constructible_v<second_type, u_compressed_type_2_t>, "second_type(u_compressed_type_2_t&&) move constructor is throwable. pair is not designed to allow throwable move constructible components");
        }

        /**
         * Assigns other as the new content for the compressed_pair object.
         * Perform copy assignments, with the elements of its argument preserving their values after the call.
         * compressed_pair do not accept throwable copy assignable components.
         * @param other Another compressed_pair object.
         * @return *this
         */
        constexpr compressed_pair &operator=(const compressed_pair &)
        requires(hud::is_nothrow_copy_assignable_v<first_type> && hud::is_nothrow_copy_assignable_v<second_type>)
        = default;

        /**
         * Assigns other as the new content for the compressed_pair object.
         * Perform move assignments, which, for elements of types supporting move semantics implies that these elements of other are left in an unspecified but valid state.
         * compressed_pair do not accept throwable move assignable components.
         * @param other Another compressed_pair object.
         * @return *this
         */
        constexpr compressed_pair &operator=(compressed_pair &&)
        requires(hud::is_nothrow_move_assignable_v<first_type> && hud::is_nothrow_move_assignable_v<second_type>)
        = default;

        /**
         * Initialization move constructor.
         * This involves individually constructing its two components, with an initialization that depends on the constructor.
         * Member 'first' is constructed with 'f', and member 'second' with 's'.
         * compressed_pair do not accept throwable move-constructible components.
         * @tparam u_type_t Type of the 'f' component.
         * @tparam v_type_t Type of the 's' component.
         * @param f An object of the same type as 'first', or some other type implicitly convertible to it.
         * @param s An object of the same type as 'second', or some other type implicitly convertible to it.
         */
        template<typename u_compressed_type_1_t, typename u_compressed_type_2_t>
        requires(hud::is_move_constructible_v<first_type, u_compressed_type_1_t> && hud::is_move_constructible_v<second_type, u_compressed_type_2_t>)
        constexpr explicit(!(hud::is_convertible_v<u_compressed_type_1_t &&, first_type> && hud::is_convertible_v<u_compressed_type_2_t &&, second_type>))
            compressed_pair(u_compressed_type_1_t &&f, u_compressed_type_2_t &&s) noexcept
            : super_type(hud::forward<u_compressed_type_1_t>(f), hud::forward<u_compressed_type_2_t>(s))
        {
            static_assert(hud::is_nothrow_move_constructible_v<first_type, u_compressed_type_1_t>, "first_type(u_compressed_type_1_t&&) move constructor is throwable. compressed_pair is not designed to allow throwable move constructible components");
            static_assert(hud::is_nothrow_move_constructible_v<second_type, u_compressed_type_2_t>, "second_type(u_compressed_type_2_t&&) move constructor is throwable. compressed_pair is not designed to allow throwable move constructible components");
        }

        /**
         * Initialization move constructor.
         * This involves individually constructing its two components, with an initialization that depends on the constructor.
         * Member 'first' is constructed with 'f', and member 'second' is default-constructed.
         * compressed_pair does not accept throwable move-constructible components.
         * @tparam u_compressed_type_1_t Type of the 'f' component.
         * @param f An object of the same type as 'first', or some other type implicitly convertible to it.
         */
        template<typename u_compressed_type_1_t>
        requires(hud::is_same_v<first_type, hud::remove_cv_ref_t<u_compressed_type_1_t>>)
        constexpr explicit(!(hud::is_convertible_v<first_type, first_type>))
            compressed_pair(u_compressed_type_1_t &&f) noexcept
            : super_type(hud::forward<u_compressed_type_1_t>(f))
        {
            static_assert(hud::is_nothrow_constructible_v<first_type, u_compressed_type_1_t>, "first_type(const u_compressed_type_1_t&) copy constructor is throwable. compressed_pair is not designed to allow throwable copy constructible components");
            static_assert(hud::is_nothrow_default_constructible_v<second_type>, "second_type default constructor is throwable. compress_pair is not designed to allow throwable default constructible components");
        }

        /**
         * Initialization move constructor.
         * This involves individually constructing its two components, with an initialization that depends on the constructor.
         * Member 'first' is default-constructed, and member 'second' is constructed with 's'.
         * compressed_pair does not accept throwable move-constructible components.
         * @tparam u_compressed_type_2_t Type of the 's' component.
         * @param s An object of the same type as 'second', or some other type implicitly convertible to it.
         */
        template<typename u_compressed_type_2_t>
        requires(hud::is_same_v<second_type, hud::remove_cv_ref_t<u_compressed_type_2_t>>)
        constexpr explicit compressed_pair(u_compressed_type_2_t &&s) noexcept
            : super_type(hud::forward<u_compressed_type_2_t>(s))
        {
            static_assert(hud::is_nothrow_default_constructible_v<first_type>, "first_type default constructor is throwable. compress_pair is not designed to allow throwable default constructible components");
            static_assert(hud::is_nothrow_constructible_v<second_type, u_compressed_type_2_t>, "second_type(const u_compressed_type_2_t&) copy constructor is throwable. compressed_pair is not designed to allow throwable copy constructible components");
        }

        /** Retrieves the reference to the first element. */
        [[nodiscard]] constexpr first_type &first() noexcept
        {
            return super_type::first();
        }

        /** Retrieves the const reference to the first element. */
        [[nodiscard]] constexpr const first_type &first() const noexcept
        {
            return super_type::first();
        }

        /** Retrieves the reference to the second element. */
        [[nodiscard]] constexpr second_type &second() noexcept
        {
            return super_type::second();
        }

        /** Retrieves the const reference to the second element. */
        [[nodiscard]] constexpr const second_type &second() const noexcept
        {
            return super_type::second();
        }
    };

    /**
     * `compressed_pair` is closely related to `hud::pair`.
     * However, if either of the template arguments represents an empty class, the Empty Base Class Optimization (EBCO) is applied to reduce the size of the pair.
     * The two members of the pair can be accessed using the member functions `first()` and `second()`.
     */
    template<typename compressed_type_t>
    class compressed_pair<compressed_type_t, compressed_type_t>
        : details::compressed_pair::compress_pair_impl<compressed_type_t, compressed_type_t>
    {
        using super_type = details::compressed_pair::compress_pair_impl<compressed_type_t, compressed_type_t>;

    public:
        /** The type of 'first' */
        using first_type = typename super_type::first_type;
        /** The type of 'second' */
        using second_type = typename super_type::second_type;

    public:
        /** Default construct first and second */
        constexpr explicit(!(hud::is_implicitly_default_constructible_v<first_type> && hud::is_implicitly_default_constructible_v<second_type>))
            compressed_pair() = default;

        /**
         * Copy constructor.
         * This involves individually constructing its two components, with an initialization that depends on the constructor.
         * The object is initialized with the contents of the other compressed_pair object.
         * The corresponding members of 'other' are passed to the constructor of each of its members.
         * compressed_pair does not accept throwable default constructible components.
         * @param other Another compressed_pair object.
         */
        constexpr explicit(!(hud::is_convertible_v<const first_type &, first_type> && hud::is_convertible_v<const second_type &, second_type>))
            compressed_pair(const compressed_pair &other)
        requires(hud::is_nothrow_copy_constructible_v<first_type> && hud::is_nothrow_copy_constructible_v<second_type>)
        = default;

        /**
         * This involves individually constructing its two components, with an initialization that depends on the constructor.
         * The object is initialized with the contents of the other compressed_pair object.
         * The corresponding members of 'other' are passed to the constructor of each of its members.
         * Compressed pairs do not accept throwable copy-constructible components.
         * @tparam u_compressed_type_1_t Type of the first component.
         * @tparam u_compressed_type_2_t Type of the second component.
         * @param other Another compressed_pair object.
         */
        template<typename u_compressed_type_1_t, typename u_compressed_type_2_t>
        requires(hud::is_copy_constructible_v<first_type, u_compressed_type_1_t> && hud::is_copy_constructible_v<second_type, u_compressed_type_2_t>)
        constexpr explicit(!(hud::is_convertible_v<const u_compressed_type_1_t &, first_type> && hud::is_convertible_v<const u_compressed_type_2_t &, second_type>))
            compressed_pair(const compressed_pair<u_compressed_type_1_t, u_compressed_type_2_t> &other) noexcept
            : super_type(other.first(), other.second())
        {
            static_assert(hud::is_nothrow_copy_constructible_v<first_type, u_compressed_type_1_t>, "first_type(const u_compressed_type_1_t&) copy constructor is throwable. compressed_pair is not designed to allow throwable copy constructible components");
            static_assert(hud::is_nothrow_copy_constructible_v<second_type, u_compressed_type_2_t>, "second_type(const u_compressed_type_2_t&) copy constructor is throwable. compressed_pair is not designed to allow throwable copy constructible components");
        }

        /**
         * Move constructor.
         * This involves individually constructing its two components, with an initialization that depends on the constructor.
         * The object is initialized with the contents of the other compressed_pair object.
         * The corresponding members of 'other' are forwarded to the constructor of each of its members.
         * Pairs do not accept throwable move-constructible components.
         * @param other Another compressed_pair object.
         */
        constexpr explicit(!(hud::is_convertible_v<first_type, first_type> && hud::is_convertible_v<second_type, second_type>))
            compressed_pair(compressed_pair &&other)
        requires(hud::is_nothrow_move_constructible_v<second_type> && hud::is_nothrow_move_constructible_v<second_type>)
        = default;

        /**
         * Move constructor.
         * This involves individually constructing its two components, with an initialization that depends on the constructor.
         * The object is initialized with the contents of the other compressed_pair object.
         * The corresponding members of 'other' are forwarded to the constructor of each of its members.
         * Pairs do not accept throwable move-constructible components.
         * @tparam u_type1_t Type of the first component.
         * @tparam u_type2_t Type of the second component.
         * @param other Another compressed_pair object.
         */
        template<typename u_compressed_type_1_t, typename u_compressed_type_2_t>
        requires(hud::is_move_constructible_v<first_type, u_compressed_type_1_t> && hud::is_move_constructible_v<second_type, u_compressed_type_2_t>)
        constexpr explicit(!(hud::is_convertible_v<u_compressed_type_1_t, first_type> && hud::is_convertible_v<u_compressed_type_2_t, second_type>))
            compressed_pair(compressed_pair<u_compressed_type_1_t, u_compressed_type_2_t> &&other) noexcept
            : super_type(hud::forward<u_compressed_type_1_t>(other.first()), hud::forward<u_compressed_type_2_t>(other.second()))
        {
            static_assert(hud::is_nothrow_move_constructible_v<first_type, u_compressed_type_1_t>, "first_type(u_type1_t&&) move constructor is throwable. pair is not designed to allow throwable move constructible components");
            static_assert(hud::is_nothrow_move_constructible_v<second_type, u_compressed_type_2_t>, "second_type(u_compressed_type_2_t&&) move constructor is throwable. pair is not designed to allow throwable move constructible components");
        }

        /**
         * Initialization move constructor.
         * This involves individually constructing its two components, with an initialization that depends on the constructor.
         * Member 'first' is constructed with 'f', and member 'second' with 's'.
         * compressed_pair do not accept throwable move-constructible components.
         * @tparam u_type_t Type of the 'f' component.
         * @tparam v_type_t Type of the 's' component.
         * @param f An object of the same type as 'first', or some other type implicitly convertible to it.
         * @param s An object of the same type as 'second', or some other type implicitly convertible to it.
         */
        template<typename u_compressed_type_t, typename u_compressed_type_2_t>
        constexpr explicit(!(hud::is_convertible_v<u_compressed_type_t &&, first_type> && hud::is_convertible_v<u_compressed_type_2_t &&, second_type>))
            compressed_pair(u_compressed_type_t &&type_1, u_compressed_type_2_t &&type_2) noexcept
            : super_type(hud::forward<u_compressed_type_t>(type_1), hud::forward<u_compressed_type_2_t>(type_2))
        {
            static_assert(hud::is_nothrow_move_constructible_v<first_type, u_compressed_type_t>, "first_type(u_compressed_type_t&&) move constructor is throwable. compressed_pair is not designed to allow throwable move constructible components");
            static_assert(hud::is_nothrow_move_constructible_v<second_type, u_compressed_type_2_t>, "second_type(u_compressed_type_t&&) move constructor is throwable. compressed_pair is not designed to allow throwable move constructible components");
        }

        /**
         * Assigns other as the new content for the compressed_pair object.
         * Perform copy assignments, with the elements of its argument preserving their values after the call.
         * compressed_pair do not accept throwable copy assignable components.
         * @param other Another compressed_pair object.
         * @return *this
         */
        constexpr compressed_pair &operator=(const compressed_pair &)
        requires(hud::is_nothrow_copy_assignable_v<first_type> && hud::is_nothrow_copy_assignable_v<second_type>)
        = default;

        /**
         * Assigns other as the new content for the compressed_pair object.
         * Perform move assignments, which, for elements of types supporting move semantics implies that these elements of other are left in an unspecified but valid state.
         * compressed_pair do not accept throwable move assignable components.
         * @param other Another compressed_pair object.
         * @return *this
         */
        constexpr compressed_pair &operator=(compressed_pair &&)
        requires(hud::is_nothrow_move_assignable_v<first_type> && hud::is_nothrow_move_assignable_v<second_type>)
        = default;

        /** Retrieves the reference to the first element. */
        [[nodiscard]] constexpr compressed_type_t &first() & noexcept
        {
            return super_type::first();
        }

        /** Retrieves the const reference to the first element. */
        [[nodiscard]] constexpr const compressed_type_t &first() const & noexcept
        {
            return super_type::first();
        }

        /** Retrieves the reference to the second element. */
        [[nodiscard]] constexpr compressed_type_t &second() & noexcept
        {
            return super_type::second();
        }

        /** Retrieves the const reference to the second element. */
        [[nodiscard]] constexpr const compressed_type_t &second() const & noexcept
        {
            return super_type::second();
        }
    };

} // namespace hud

#endif // HD_INC_CORE_COMPRESSED_TYPE_H