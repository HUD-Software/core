#ifndef HD_INC_CORE_COMPRESSED_TYPE_H
#define HD_INC_CORE_COMPRESSED_TYPE_H
#include "../minimal.h"
#include "../traits/is_empty.h"
#include "../traits/is_same.h"
#include "../traits/remove_cv_ref.h"
#include "../traits/is_nothrow_constructible.h"
#include "../traits/is_nothrow_default_constructible.h"
#include "../traits/is_nothrow_copy_constructible.h"
#include "../traits/is_nothrow_move_constructible.h"
#include "../traits/is_convertible.h"
#include "../traits/is_implicitly_default_constructible.h"
#include "../traits/is_nothrow_swappable.h"
#include "../traits/decay.h"
#include "../templates/swap.h"
#include "tuple_size.h"
#include "tuple_element.h"

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
             *
             * @tparam u_type_t Type of the 'f' component.
             * @tparam v_type_t Type of the 's' component.
             * @param f An object of the same type as 'first', or some other type implicitly convertible to it.
             * @param s An object of the same type as 'second', or some other type implicitly convertible to it.
             */
            template<typename u_type_t, typename v_type_t>
            requires(hud::is_move_constructible_v<first_type, u_type_t> && hud::is_move_constructible_v<second_type, v_type_t>)
            constexpr explicit(!(hud::is_convertible_v<u_type_t &&, first_type> && hud::is_convertible_v<v_type_t &&, u_type_t>))
                compress_pair_impl(u_type_t &&type_1, v_type_t &&type_2) noexcept
                : type_1_(hud::forward<u_type_t>(type_1))
                , type_2_(hud::forward<v_type_t>(type_2))
            {
                static_assert(hud::is_nothrow_constructible_v<first_type, u_type_t>, "first_type(u_type_t&&) copy constructor is throwable. compressed_pair is not designed to allow throwable copy constructible components");
                static_assert(hud::is_nothrow_constructible_v<second_type, v_type_t>, "second_type(v_type_t&&) copy constructor is throwable. compressed_pair is not designed to allow throwable copy constructible components");
            }

            /**
             * Initialization move constructor.
             * This involves individually constructing its two components, with an initialization that depends on the constructor.
             * Member 'first' is constructed with 'f', and member 'second' is default-constructed.
             * compressed_pair does not accept throwable move-constructible components.
             * @tparam u_type_t Type of the 'f' component.
             * @param f An object of the same type as 'first', or some other type implicitly convertible to it.
             */
            template<typename u_type_t>
            constexpr explicit(!(hud::is_convertible_v<u_type_t &&, first_type> && hud::is_implicitly_default_constructible_v<second_type>))
                compress_pair_impl(u_type_t &&type_1) noexcept
            requires(hud::is_same_v<first_type, hud::remove_cv_ref_t<u_type_t>>)
                : type_1_(hud::forward<u_type_t>(type_1))
                , type_2_()
            {
                static_assert(hud::is_nothrow_constructible_v<first_type, u_type_t>, "first_type(const u_type_t&) copy constructor is throwable. compressed_pair is not designed to allow throwable copy constructible components");
                static_assert(hud::is_nothrow_default_constructible_v<second_type>, "second_type default constructor is throwable. compress_pair is not designed to allow throwable default constructible components");
            }

            /**
             * Initialization move constructor.
             * This involves individually constructing its two components, with an initialization that depends on the constructor.
             * Member 'first' is default-constructed, and member 'second' is constructed with 's'.
             * compressed_pair does not accept throwable move-constructible components.
             * @tparam v_type_t Type of the 's' component.
             * @param s An object of the same type as 'second', or some other type implicitly convertible to it.
             */
            template<typename v_type_t>
            constexpr explicit(!(hud::is_implicitly_default_constructible_v<first_type> && hud::is_convertible_v<v_type_t &&, second_type>))
                compress_pair_impl(v_type_t &&type_2) noexcept
            requires(hud::is_same_v<second_type, hud::remove_cv_ref_t<v_type_t>>)
                : type_1_()
                , type_2_(hud::forward<v_type_t>(type_2))
            {
                static_assert(hud::is_nothrow_default_constructible_v<first_type>, "first_type default constructor is throwable. compress_pair is not designed to allow throwable default constructible components");
                static_assert(hud::is_nothrow_constructible_v<second_type, v_type_t>, "second_type(const v_type_t&) copy constructor is throwable. compressed_pair is not designed to allow throwable copy constructible components");
            }

            // /** Copy assignment. */
            // constexpr compress_pair_impl &operator=(const compress_pair_impl &) = default;
            // /** Move assignment. */
            // constexpr compress_pair_impl &operator=(compress_pair_impl &&) = default;

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
                return hud::move(type_1_);
            }

            /** Retrieves the const l-value reference to the first element. */
            [[nodiscard]] constexpr first_type &&first() && noexcept
            {
                return hud::move(type_1_);
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
                return hud::move(type_2_);
            }

            /** Retrieves the const l-value reference to the first element. */
            [[nodiscard]] constexpr first_type &&second() && noexcept
            {
                return hud::move(type_2_);
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
            template<typename u_type_t, typename v_type_t>
            requires(hud::is_move_constructible_v<first_type, u_type_t> && hud::is_move_constructible_v<second_type, v_type_t>)
            constexpr explicit(!(hud::is_convertible_v<u_type_t &&, first_type> && hud::is_convertible_v<v_type_t &&, second_type>))
                compress_pair_impl(u_type_t &&type_1, v_type_t &&type_2) noexcept
                : first_type(hud::forward<u_type_t>(type_1))
                , type_2_(hud::forward<v_type_t>(type_2))
            {
                static_assert(hud::is_nothrow_constructible_v<first_type, u_type_t>, "first_type(const u_type_t&) copy constructor is throwable. compressed_pair is not designed to allow throwable copy constructible components");
                static_assert(hud::is_nothrow_constructible_v<second_type, v_type_t>, "second_type(const v_type_t&) copy constructor is throwable. compressed_pair is not designed to allow throwable copy constructible components");
            }

            /**
             * Initialization move constructor.
             * This involves individually constructing its two components, with an initialization that depends on the constructor.
             * Member 'first' is constructed with 'f', and member 'second' is default-constructed.
             * compressed_pair does not accept throwable move-constructible components.
             * @tparam u_type_t Type of the 'f' component.
             * @param f An object of the same type as 'first', or some other type implicitly convertible to it.
             */
            template<typename u_type_t>
            constexpr explicit(!(hud::is_convertible_v<u_type_t &&, first_type> && hud::is_implicitly_default_constructible_v<second_type>))
                compress_pair_impl(u_type_t &&type_1) noexcept
            requires(hud::is_same_v<first_type, hud::remove_cv_ref_t<u_type_t>>)
                : first_type(hud::forward<u_type_t>(type_1))
                , type_2_()
            {
                static_assert(hud::is_nothrow_constructible_v<first_type, u_type_t>, "first_type(const u_type_t&) copy constructor is throwable. compressed_pair is not designed to allow throwable copy constructible components");
                static_assert(hud::is_nothrow_default_constructible_v<second_type>, "second_type default constructor is throwable. compress_pair is not designed to allow throwable default constructible components");
            }

            /**
             * Initialization move constructor.
             * This involves individually constructing its two components, with an initialization that depends on the constructor.
             * Member 'first' is default-constructed, and member 'second' is constructed with 's'.
             * compressed_pair does not accept throwable move-constructible components.
             * @tparam v_type_t Type of the 's' component.
             * @param s An object of the same type as 'second', or some other type implicitly convertible to it.
             */
            template<typename v_type_t>
            constexpr explicit(!hud::is_implicitly_default_constructible_v<first_type> && (hud::is_convertible_v<v_type_t &&, second_type>))
                compress_pair_impl(v_type_t &&type_2) noexcept
            requires(hud::is_same_v<second_type, hud::remove_cv_ref_t<v_type_t>>)
                : first_type()
                , type_2_(hud::forward<v_type_t>(type_2))
            {
                static_assert(hud::is_nothrow_default_constructible_v<first_type>, "first_type default constructor is throwable. compress_pair is not designed to allow throwable default constructible components");
                static_assert(hud::is_nothrow_constructible_v<second_type, v_type_t>, "second_type(const v_type_t&) copy constructor is throwable. compressed_pair is not designed to allow throwable copy constructible components");
            }

            // /** Copy assignment. */
            // constexpr compress_pair_impl &operator=(const compress_pair_impl &) = default;
            // /** Move assignment. */
            // constexpr compress_pair_impl &operator=(compress_pair_impl &&) = default;

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
                return hud::move(*this);
            }

            /** Retrieves the const l-value reference to the first element. */
            [[nodiscard]] constexpr first_type &&first() && noexcept
            {
                return hud::move(*this);
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
                return hud::move(type_2_);
            }

            /** Retrieves the const l-value reference to the first element. */
            [[nodiscard]] constexpr first_type &&second() && noexcept
            {
                return hud::move(type_2_);
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

            // /** Copy assignment. */
            // constexpr compress_pair_impl &operator=(const compress_pair_impl &) = default;

            // /** Move assignment. */
            // constexpr compress_pair_impl &operator=(compress_pair_impl &&) = default;

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
            template<typename u_type_t, typename v_type_t>
            requires(hud::is_move_constructible_v<first_type, u_type_t> && hud::is_move_constructible_v<second_type, v_type_t>)
            constexpr explicit(!(hud::is_convertible_v<u_type_t &&, first_type> && hud::is_convertible_v<v_type_t &&, second_type>))
                compress_pair_impl(u_type_t &&type_1, v_type_t &&type_2) noexcept
                : second_type(hud::forward<v_type_t>(type_2))
                , type_1_(hud::forward<u_type_t>(type_1))
            {
                static_assert(hud::is_nothrow_constructible_v<first_type, u_type_t>, "first_type(const u_type_t&) copy constructor is throwable. compressed_pair is not designed to allow throwable copy constructible components");
                static_assert(hud::is_nothrow_constructible_v<second_type, v_type_t>, "second_type(const v_type_t&) copy constructor is throwable. compressed_pair is not designed to allow throwable copy constructible components");
            }

            /**
             * Initialization move constructor.
             * This involves individually constructing its two components, with an initialization that depends on the constructor.
             * Member 'first' is constructed with 'f', and member 'second' is default-constructed.
             * compressed_pair does not accept throwable move-constructible components.
             * @tparam u_type_t Type of the 'f' component.
             * @param f An object of the same type as 'first', or some other type implicitly convertible to it.
             */
            template<typename u_type_t>
            constexpr explicit(!(hud::is_convertible_v<u_type_t &&, first_type> && hud::is_implicitly_default_constructible_v<second_type>))
                compress_pair_impl(u_type_t &&type_1) noexcept
            requires(hud::is_same_v<first_type, hud::remove_cv_ref_t<u_type_t>>)
                : second_type()
                , type_1_(hud::forward<u_type_t>(type_1))
            {
                static_assert(hud::is_nothrow_constructible_v<first_type, u_type_t>, "first_type(const u_type_t&) copy constructor is throwable. compressed_pair is not designed to allow throwable copy constructible components");
                static_assert(hud::is_nothrow_default_constructible_v<second_type>, "second_type default constructor is throwable. compress_pair is not designed to allow throwable default constructible components");
            }

            /**
             * Initialization move constructor.
             * This involves individually constructing its two components, with an initialization that depends on the constructor.
             * Member 'first' is default-constructed, and member 'second' is constructed with 's'.
             * compressed_pair does not accept throwable move-constructible components.
             * @tparam v_type_t Type of the 's' component.
             * @param s An object of the same type as 'second', or some other type implicitly convertible to it.
             */
            template<typename v_type_t>
            constexpr explicit(!(hud::is_implicitly_default_constructible_v<first_type> && hud::is_convertible_v<v_type_t &&, second_type>))
                compress_pair_impl(v_type_t &&type_2) noexcept
            requires(hud::is_same_v<second_type, hud::remove_cv_ref_t<v_type_t>>)
                : second_type(hud::forward<v_type_t>(type_2))
                , first_type()
            {
                static_assert(hud::is_nothrow_default_constructible_v<first_type>, "first_type default constructor is throwable. compress_pair is not designed to allow throwable default constructible components");
                static_assert(hud::is_nothrow_constructible_v<second_type, v_type_t>, "second_type(const v_type_t&) copy constructor is throwable. compressed_pair is not designed to allow throwable copy constructible components");
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
                return hud::move(type_1_);
            }

            /** Retrieves the const l-value reference to the first element. */
            [[nodiscard]] constexpr first_type &&first() && noexcept
            {
                return hud::move(type_1_);
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
                return hud::move(*this);
            }

            /** Retrieves the const l-value reference to the first element. */
            [[nodiscard]] constexpr first_type &&second() && noexcept
            {
                return hud::move(*this);
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

            // /** Copy assignment. */
            // constexpr compress_pair_impl &operator=(const compress_pair_impl &) = default;

            // /** Move assignment. */
            // constexpr compress_pair_impl &operator=(compress_pair_impl &&) = default;

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
            template<typename u_type_t, typename v_type_t>
            requires(hud::is_move_constructible_v<first_type, u_type_t> && hud::is_move_constructible_v<second_type, v_type_t>)
            constexpr explicit(!(hud::is_convertible_v<u_type_t &&, first_type> && hud::is_convertible_v<v_type_t &&, second_type>))
                compress_pair_impl(u_type_t &&type_1, v_type_t &&type_2) noexcept
                : first_type(hud::forward<u_type_t>(type_1))
                , second_type(hud::forward<v_type_t>(type_2))
            {
                static_assert(hud::is_nothrow_constructible_v<first_type, u_type_t>, "first_type(const u_type_t&) copy constructor is throwable. compressed_pair is not designed to allow throwable copy constructible components");
                static_assert(hud::is_nothrow_constructible_v<second_type, v_type_t>, "second_type(const v_type_t&) copy constructor is throwable. compressed_pair is not designed to allow throwable copy constructible components");
            }

            /**
             * Initialization move constructor.
             * This involves individually constructing its two components, with an initialization that depends on the constructor.
             * Member 'first' is constructed with 'f', and member 'second' is default-constructed.
             * compressed_pair does not accept throwable move-constructible components.
             * @tparam u_type_t Type of the 'f' component.
             * @param f An object of the same type as 'first', or some other type implicitly convertible to it.
             */
            template<typename u_type_t>
            constexpr explicit(!(hud::is_convertible_v<u_type_t &&, first_type> && hud::is_implicitly_default_constructible_v<second_type>))
                compress_pair_impl(u_type_t &&type_1) noexcept
            requires(hud::is_same_v<first_type, hud::remove_cv_ref_t<u_type_t>>)
                : first_type(hud::forward<u_type_t>(type_1))
                , second_type()
            {
                static_assert(hud::is_nothrow_constructible_v<first_type, u_type_t>, "first_type(const u_type_t&) copy constructor is throwable. compressed_pair is not designed to allow throwable copy constructible components");
                static_assert(hud::is_nothrow_default_constructible_v<second_type>, "second_type default constructor is throwable. compress_pair is not designed to allow throwable default constructible components");
            }

            /**
             * Initialization move constructor.
             * This involves individually constructing its two components, with an initialization that depends on the constructor.
             * Member 'first' is default-constructed, and member 'second' is constructed with 's'.
             * compressed_pair does not accept throwable move-constructible components.
             * @tparam v_type_t Type of the 's' component.
             * @param s An object of the same type as 'second', or some other type implicitly convertible to it.
             */
            template<typename v_type_t>
            constexpr explicit(!(hud::is_implicitly_default_constructible_v<first_type> && hud::is_convertible_v<v_type_t &&, second_type>))
                compress_pair_impl(v_type_t &&type_2) noexcept
            requires(hud::is_same_v<second_type, hud::remove_cv_ref_t<v_type_t>>)
                : first_type()
                , second_type(hud::forward<v_type_t>(type_2))
            {
                static_assert(hud::is_nothrow_default_constructible_v<first_type>, "first_type default constructor is throwable. compress_pair is not designed to allow throwable default constructible components");
                static_assert(hud::is_nothrow_constructible_v<second_type, v_type_t>, "second_type(const v_type_t&) copy constructor is throwable. compressed_pair is not designed to allow throwable copy constructible components");
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
                return hud::move(*this);
            }

            /** Retrieves the const l-value reference to the first element. */
            [[nodiscard]] constexpr first_type &&first() && noexcept
            {
                return hud::move(*this);
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
                return hud::move(*this);
            }

            /** Retrieves the const l-value reference to the first element. */
            [[nodiscard]] constexpr first_type &&second() && noexcept
            {
                return hud::move(*this);
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

            // /** Copy assignment. */
            // constexpr compress_pair_impl &operator=(const compress_pair_impl &) = default;

            // /** Move assignment. */
            // constexpr compress_pair_impl &operator=(compress_pair_impl &&) = default;

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
            template<typename u_type_t, typename v_type_t>
            requires(hud::is_move_constructible_v<first_type, u_type_t> && hud::is_move_constructible_v<second_type, v_type_t>)
            constexpr explicit(!(hud::is_convertible_v<u_type_t &&, first_type> && hud::is_convertible_v<u_type_t &&, second_type>))
                compress_pair_impl(u_type_t &&type_1, v_type_t &&type_2) noexcept
                : type_1_(hud::forward<u_type_t>(type_1))
                , type_2_(hud::forward<v_type_t>(type_2))
            {
                static_assert(hud::is_nothrow_constructible_v<first_type, u_type_t>, "first_type(const u_type_t&) copy constructor is throwable. compressed_pair is not designed to allow throwable copy constructible components");
                static_assert(hud::is_nothrow_constructible_v<second_type, v_type_t>, "second_type(const v_type_t&) copy constructor is throwable. compressed_pair is not designed to allow throwable copy constructible components");
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
                return hud::move(type_1_);
            }

            /** Retrieves the const l-value reference to the first element. */
            [[nodiscard]] constexpr first_type &&first() && noexcept
            {
                return hud::move(type_1_);
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
                return hud::move(type_2_);
            }

            /** Retrieves the const l-value reference to the first element. */
            [[nodiscard]] constexpr first_type &&second() && noexcept
            {
                return hud::move(type_2_);
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
            : protected compress_pair_impl<compressed_type_t, compressed_type_t, false, false>
        {
        };

    } // namespace details::compressed_pair

    /**
     * `compressed_pair` is closely related to `hud::pair`.
     * However, if either of the template arguments represents an empty class, the Empty Base Class Optimization (EBCO) is applied to reduce the size of the compressed_pair.
     * The two members of the compressed_pair can be accessed using the member functions `first()` and `second()`.
     */
    template<typename compressed_type_1_t, typename compressed_type_2_t>
    class compressed_pair
        : details::compressed_pair::compress_pair_impl<hud::remove_cv_t<compressed_type_1_t>, hud::remove_cv_t<compressed_type_2_t>>
    {
        using super_type = details::compressed_pair::compress_pair_impl<hud::remove_cv_t<compressed_type_1_t>, hud::remove_cv_t<compressed_type_2_t>>;

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
         * @tparam u_type_t Type of the first component.
         * @tparam v_type_t Type of the second component.
         * @param other Another compressed_pair object.
         */
        template<typename u_type_t, typename v_type_t>
        requires(hud::is_copy_constructible_v<first_type, u_type_t> && hud::is_copy_constructible_v<second_type, v_type_t>)
        constexpr explicit(!(hud::is_convertible_v<const u_type_t &, first_type> && hud::is_convertible_v<const v_type_t &, second_type>))
            compressed_pair(const compressed_pair<u_type_t, v_type_t> &other) noexcept
            : super_type(other.first(), other.second())
        {
            static_assert(hud::is_nothrow_copy_constructible_v<first_type, u_type_t>, "first_type(const u_type_t&) copy constructor is throwable. compressed_pair is not designed to allow throwable copy constructible components");
            static_assert(hud::is_nothrow_copy_constructible_v<second_type, v_type_t>, "second_type(const v_type_t&) copy constructor is throwable. compressed_pair is not designed to allow throwable copy constructible components");
        }

        /**
         * Initialization copy constructor.
         * This involves individually constructing its two components, with an initialization that depends on the constructor.
         * Member 'first' is constructed with 'f', and member 'second' with 's'.
         * compressed_pair do not accept throwable move-constructible components.
         * @tparam u_type_t Type of the 'f' component.
         * @tparam v_type_t Type of the 's' component.
         * @param f An object of the same type as 'first', or some other type implicitly convertible to it.
         * @param s An object of the same type as 'second', or some other type implicitly convertible to it.
         */
        constexpr explicit(!(hud::is_convertible_v<const first_type &, first_type> && hud::is_convertible_v<const second_type &, second_type>))
            compressed_pair(const first_type &f, const second_type &s) noexcept
        requires(hud::is_copy_constructible_v<first_type> && hud::is_copy_constructible_v<second_type>)
            : super_type(f, s)
        {
            static_assert(hud::is_nothrow_copy_constructible_v<first_type>, "first_type(const first_type&) copy constructor is throwable. compressed_pair is not designed to allow throwable copy constructible components");
            static_assert(hud::is_nothrow_copy_constructible_v<second_type>, "second_type(const second_type&) copy constructor is throwable. compressed_pair is not designed to allow throwable copy constructible components");
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
         * @tparam u_type_t Type of the first component.
         * @tparam v_type_t Type of the second component.
         * @param other Another compressed_pair object.
         */
        template<typename u_type_t, typename v_type_t>
        requires(hud::is_move_constructible_v<first_type, u_type_t> && hud::is_move_constructible_v<second_type, v_type_t>)
        constexpr explicit(!(hud::is_convertible_v<u_type_t, first_type> && hud::is_convertible_v<v_type_t, second_type>))
            compressed_pair(compressed_pair<u_type_t, v_type_t> &&other) noexcept
            : super_type(hud::move(other.first()), hud::move(other.second()))
        {
            static_assert(hud::is_nothrow_move_constructible_v<first_type, u_type_t>, "first_type(u_type_t&&) move constructor is throwable. compressed_pair is not designed to allow throwable move constructible components");
            static_assert(hud::is_nothrow_move_constructible_v<second_type, v_type_t>, "second_type(v_type_t&&) move constructor is throwable. compressed_pair is not designed to allow throwable move constructible components");
        }

        /**
         * Assigns other as the new content for the pcompressed_pairair object.
         * Perform copy assignments, with the elements of its argument preserving their values after the call.
         * compressed_pair do not accept throwable copy assignable components.
         * @param other Another compressed_pair object.
         * @return *this
         */
        constexpr compressed_pair &operator=(const compressed_pair &other) noexcept
        requires(hud::is_copy_assignable_v<first_type> && hud::is_copy_assignable_v<second_type>)
        {
            static_assert(hud::is_nothrow_copy_assignable_v<first_type>, "first_type& first_type::operator=(const first_type&) is throwable. compressed_pair is not designed to allow throwable copy assignable components");
            static_assert(hud::is_nothrow_copy_assignable_v<second_type>, "second_type& second_type::operator=(const second_type&) is throwable. compressed_pair is not designed to allow throwable copy assignable components");
            first() = other.first();
            second() = other.second();
            return *this;
        }

        /**
         * Assigns other as the new content for the compressed_pair object.
         * Perform copy assignments, with the elements of its argument preserving their values after the call.
         * compressed_pair do not accept throwable copy assignable components.
         * @tparam u_type_t Type of the first component.
         * @tparam v_type_t Type of the second component.
         * @param other Another compressed_pair object.
         * @return *this
         */
        template<typename u_type_t, typename v_type_t>
        requires(hud::is_assignable_v<first_type &, const u_type_t &> && hud::is_copy_assignable_v<second_type, v_type_t>)
        constexpr compressed_pair &operator=(const compressed_pair<u_type_t, v_type_t> &other) noexcept
        {
            static_assert(hud::is_nothrow_copy_assignable_v<first_type, u_type_t>, "first_type& first_type::operator=(const u_type_t&) is throwable. compressed_pair is not designed to allow throwable copy assignable components");
            static_assert(hud::is_nothrow_copy_assignable_v<second_type, v_type_t>, "second_type& second_type::operator=(const v_type_t&) is throwable. compressed_pair is not designed to allow throwable copy assignable components");
            first() = other.first();
            second() = other.second();
            return *this;
        }

        /**
         * Assigns other as the new content for the compressed_pair object.
         * Perform move assignments, which, for elements of types supporting move semantics implies that these elements of other are left in an unspecified but valid state.
         * compressed_pair do not accept throwable move assignable components.
         * @param other Another pair object.
         * @return *this
         */
        constexpr compressed_pair &operator=(compressed_pair &&other) noexcept
        requires(hud::is_move_assignable_v<first_type> && hud::is_move_assignable_v<second_type>)
        {
            static_assert(hud::is_nothrow_move_assignable_v<first_type>, "first_type& first_type::operator=(first_type&&) is throwable. compressed_pair is not designed to allow throwable move assignable components");
            static_assert(hud::is_nothrow_move_assignable_v<second_type>, "second_type& second_type::operator=(second_type&&) is throwable. compressed_pair is not designed to allow throwable move assignable components");
            first() = hud::forward<first_type>(other.first());
            second() = hud::forward<second_type>(other.second());
            return *this;
        }

        /**
         * Assigns other as the new content for the compressed_pair object.
         * Perform move assignments, which, for elements of types supporting move semantics implies that these elements of other are left in an unspecified but valid state.
         * compressed_pair do not accept throwable move assignable components.
         * @tparam u_type_t Type of the first component.
         * @tparam v_type_t Type of the second component.
         * @param other Another compressed_pair object.
         * @return *this
         */
        template<typename u_type_t, typename v_type_t>
        requires(hud::is_assignable_v<first_type &, u_type_t &&> && hud::is_assignable_v<second_type &, v_type_t &&>)
        constexpr compressed_pair &operator=(compressed_pair<u_type_t, v_type_t> &&other) noexcept
        {
            static_assert(hud::is_nothrow_assignable_v<first_type &, u_type_t &&>, "first_type& first_type::operator=(u_type_t&&) is throwable. compressed_pair is not designed to allow throwable move assignable components");
            static_assert(hud::is_nothrow_assignable_v<second_type &, v_type_t &&>, "second_type& second_type::operator=(v_type_t&&) is throwable. compressed_pair is not designed to allow throwable move assignable components");
            first() = hud::forward<u_type_t>(other.first());
            second() = hud::forward<v_type_t>(other.second());
            return *this;
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
        template<typename u_type_t = first_type, typename v_type_t = second_type>
        requires(hud::is_move_constructible_v<first_type, u_type_t> && hud::is_move_constructible_v<second_type, v_type_t>)
        constexpr explicit(!(hud::is_convertible_v<u_type_t &&, first_type> && hud::is_convertible_v<v_type_t &&, second_type>))
            compressed_pair(u_type_t &&f, v_type_t &&s) noexcept
            : super_type(hud::forward<u_type_t>(f), hud::forward<v_type_t>(s))
        {
            static_assert(hud::is_nothrow_move_constructible_v<first_type, u_type_t>, "first_type(u_type_t&&) move constructor is throwable. compressed_pair is not designed to allow throwable move constructible components");
            static_assert(hud::is_nothrow_move_constructible_v<second_type, v_type_t>, "second_type(v_type_t&&) move constructor is throwable. compressed_pair is not designed to allow throwable move constructible components");
        }

        /**
         * Initialization move constructor.
         * This involves individually constructing its two components, with an initialization that depends on the constructor.
         * Member 'first' is constructed with 'f', and member 'second' is default-constructed.
         * compressed_pair does not accept throwable move-constructible components.
         * @tparam u_type_t Type of the 'f' component.
         * @param f An object of the same type as 'first', or some other type implicitly convertible to it.
         */
        template<typename u_type_t>
        requires(hud::is_same_v<first_type, hud::remove_cv_ref_t<u_type_t>>)
        constexpr explicit(!(hud::is_convertible_v<first_type, first_type>))
            compressed_pair(u_type_t &&f) noexcept
            : super_type(hud::forward<u_type_t>(f))
        {
            static_assert(hud::is_nothrow_constructible_v<first_type, u_type_t>, "first_type(const u_type_t&) copy constructor is throwable. compressed_pair is not designed to allow throwable copy constructible components");
            static_assert(hud::is_nothrow_default_constructible_v<second_type>, "second_type default constructor is throwable. compress_pair is not designed to allow throwable default constructible components");
        }

        /**
         * Initialization move constructor.
         * This involves individually constructing its two components, with an initialization that depends on the constructor.
         * Member 'first' is default-constructed, and member 'second' is constructed with 's'.
         * compressed_pair does not accept throwable move-constructible components.
         * @tparam v_type_t Type of the 's' component.
         * @param s An object of the same type as 'second', or some other type implicitly convertible to it.
         */
        template<typename v_type_t>
        requires(hud::is_same_v<second_type, hud::remove_cv_ref_t<v_type_t>>)
        constexpr explicit compressed_pair(v_type_t &&s) noexcept
            : super_type(hud::forward<v_type_t>(s))
        {
            static_assert(hud::is_nothrow_default_constructible_v<first_type>, "first_type default constructor is throwable. compress_pair is not designed to allow throwable default constructible components");
            static_assert(hud::is_nothrow_constructible_v<second_type, v_type_t>, "second_type(const v_type_t&) copy constructor is throwable. compressed_pair is not designed to allow throwable copy constructible components");
        }

        using super_type::first;
        using super_type::second;

        // /** Retrieves the reference to the first element. */
        // [[nodiscard]] constexpr first_type &first() & noexcept
        // {
        //     return super_type::first();
        // }

        // /** Retrieves the const reference to the first element. */
        // [[nodiscard]] constexpr const first_type &first() const & noexcept
        // {
        //     return super_type::first();
        // }

        // /** Retrieves the const l-value reference to the first element. */
        // [[nodiscard]] constexpr const first_type &&first() const && noexcept
        // {
        //     return hud::move(super_type::first());
        // }

        // /** Retrieves the const l-value reference to the first element. */
        // [[nodiscard]] constexpr first_type &&first() && noexcept
        // {
        //     return hud::move(super_type::first());
        // }

        // /** Retrieves the reference to the second element. */
        // [[nodiscard]] constexpr second_type &second() & noexcept
        // {
        //     return super_type::second();
        // }

        // /** Retrieves the const reference to the second element. */
        // [[nodiscard]] constexpr const second_type &second() const & noexcept
        // {
        //     return super_type::second();
        // }

        // /** Retrieves the const l-value reference to the first element. */
        // [[nodiscard]] constexpr const second_type &&second() const && noexcept
        // {
        //     return hud::move(super_type::second());
        // }

        // /** Retrieves the const l-value reference to the first element. */
        // [[nodiscard]] constexpr first_type &&second() && noexcept
        // {
        //     return hud::move(super_type::second());
        // }

        /**
         * Exchanges the contents of the compressed_pair object with the contents of other.
         * compressed_pair do not accept throwable swap components.
         * @param other The other compressed_pair to swap
         */
        constexpr void swap(compressed_pair &other) noexcept
        requires(hud::is_swappable_v<first_type> && hud::is_swappable_v<second_type>)
        {
            static_assert(hud::is_nothrow_swappable_v<first_type>, "swap(first_type,first_type) is throwable. compressed_pair is not designed to allow throwable swappable components");
            static_assert(hud::is_nothrow_swappable_v<second_type>, "swap(second_type,second_type) is throwable. compressed_pair is not designed to allow throwable swappable components");
            hud::swap(first(), other.first());
            hud::swap(second(), other.second());
        }

        /**
         * Tests if both elements of left and right are equal, that is, compares left.first() with right.first() and left.second() with right.second().
         * @param right The right compressed_pair of the comperand operator
         * @return true if both elements of left are equal both elements of right, false otherwise
         */
        [[nodiscard]] constexpr bool operator==(const compressed_pair &right) const noexcept
        {
            return first() == right.first() && second() == right.second();
        }

        /**
         * Compares this to left lexicographically by operator<, that is, compares the first elements and only if they are equivalent, compares the second elements.
         * @param right The right compressed_pair of the comperand operator
         * @return true if first<right.first. Otherwise, if right.first<left.first, returns false. Otherwise, if left.second<right.second, returns true. Otherwise, returns false.
         */
        [[nodiscard]] constexpr bool operator<(const compressed_pair &right) const noexcept
        {
            return first() < right.first() || (!(right.first() < first()) && second() < right.second());
        }
    };

    /**
     * `compressed_pair` is closely related to `hud::pair`.
     * However, if either of the template arguments represents an empty class, the Empty Base Class Optimization (EBCO) is applied to reduce the size of the pair.
     * The two members of the pair can be accessed using the member functions `first()` and `second()`.
     */
    template<typename compressed_type_t>
    class compressed_pair<compressed_type_t, compressed_type_t>
        : details::compressed_pair::compress_pair_impl<hud::remove_cv_t<compressed_type_t>, hud::remove_cv_t<compressed_type_t>>
    {
        using super_type = details::compressed_pair::compress_pair_impl<hud::remove_cv_t<compressed_type_t>, hud::remove_cv_t<compressed_type_t>>;

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
         * @tparam u_type_t Type of the first component.
         * @tparam v_type_t Type of the second component.
         * @param other Another compressed_pair object.
         */
        template<typename u_type_t, typename v_type_t>
        requires(hud::is_copy_constructible_v<first_type, u_type_t> && hud::is_copy_constructible_v<second_type, v_type_t>)
        constexpr explicit(!(hud::is_convertible_v<const u_type_t &, first_type> && hud::is_convertible_v<const v_type_t &, second_type>))
            compressed_pair(const compressed_pair<u_type_t, v_type_t> &other) noexcept
            : super_type(other.first(), other.second())
        {
            static_assert(hud::is_nothrow_copy_constructible_v<first_type, u_type_t>, "first_type(const u_type_t&) copy constructor is throwable. compressed_pair is not designed to allow throwable copy constructible components");
            static_assert(hud::is_nothrow_copy_constructible_v<second_type, v_type_t>, "second_type(const v_type_t&) copy constructor is throwable. compressed_pair is not designed to allow throwable copy constructible components");
        }

        /**
         * Initialization copy constructor.
         * This involves individually constructing its two components, with an initialization that depends on the constructor.
         * Member 'first' is constructed with 'f', and member 'second' with 's'.
         * compressed_pair do not accept throwable move-constructible components.
         * @tparam u_type_t Type of the 'f' component.
         * @tparam v_type_t Type of the 's' component.
         * @param f An object of the same type as 'first', or some other type implicitly convertible to it.
         * @param s An object of the same type as 'second', or some other type implicitly convertible to it.
         */
        constexpr explicit(!(hud::is_convertible_v<const first_type &, first_type> && hud::is_convertible_v<const second_type &, second_type>))
            compressed_pair(const first_type &f, const second_type &s) noexcept
        requires(hud::is_copy_constructible_v<first_type> && hud::is_copy_constructible_v<second_type>)
            : super_type(f, s)
        {
            static_assert(hud::is_nothrow_copy_constructible_v<first_type>, "first_type(const first_type&) copy constructor is throwable. compressed_pair is not designed to allow throwable copy constructible components");
            static_assert(hud::is_nothrow_copy_constructible_v<second_type>, "second_type(const second_type&) copy constructor is throwable. compressed_pair is not designed to allow throwable copy constructible components");
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
         * @tparam u_type_t Type of the first component.
         * @tparam v_type_t Type of the second component.
         * @param other Another compressed_pair object.
         */
        template<typename u_type_t, typename v_type_t>
        requires(hud::is_move_constructible_v<first_type, u_type_t> && hud::is_move_constructible_v<second_type, v_type_t>)
        constexpr explicit(!(hud::is_convertible_v<u_type_t, first_type> && hud::is_convertible_v<v_type_t, second_type>))
            compressed_pair(compressed_pair<u_type_t, v_type_t> &&other) noexcept
            : super_type(hud::move(other.first()), hud::move(other.second()))
        {
            static_assert(hud::is_nothrow_move_constructible_v<first_type, u_type_t>, "first_type(u_type_t&&) move constructor is throwable. compressed_pair is not designed to allow throwable move constructible components");
            static_assert(hud::is_nothrow_move_constructible_v<second_type, v_type_t>, "second_type(v_type_t&&) move constructor is throwable. compressed_pair is not designed to allow throwable move constructible components");
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
        template<typename u_type_t = first_type, typename v_type_t = second_type>
        requires(hud::is_move_constructible_v<first_type, u_type_t> && hud::is_move_constructible_v<second_type, v_type_t>)
        constexpr explicit(!(hud::is_convertible_v<u_type_t &&, first_type> && hud::is_convertible_v<v_type_t &&, second_type>))
            compressed_pair(u_type_t &&f, v_type_t &&s) noexcept
            : super_type(hud::forward<u_type_t>(f), hud::forward<v_type_t>(s))
        {
            static_assert(hud::is_nothrow_move_constructible_v<first_type, u_type_t>, "first_type(u_type_t&&) move constructor is throwable. compressed_pair is not designed to allow throwable move constructible components");
            static_assert(hud::is_nothrow_move_constructible_v<second_type, v_type_t>, "second_type(v_type_t&&) move constructor is throwable. compressed_pair is not designed to allow throwable move constructible components");
        }

        /**
         * Assigns other as the new content for the pcompressed_pairair object.
         * Perform copy assignments, with the elements of its argument preserving their values after the call.
         * compressed_pair do not accept throwable copy assignable components.
         * @param other Another compressed_pair object.
         * @return *this
         */
        constexpr compressed_pair &operator=(const compressed_pair &other) noexcept
        requires(hud::is_copy_assignable_v<first_type> && hud::is_copy_assignable_v<second_type>)
        {
            static_assert(hud::is_nothrow_copy_assignable_v<first_type>, "first_type& first_type::operator=(const first_type&) is throwable. compressed_pair is not designed to allow throwable copy assignable components");
            static_assert(hud::is_nothrow_copy_assignable_v<second_type>, "second_type& second_type::operator=(const second_type&) is throwable. compressed_pair is not designed to allow throwable copy assignable components");
            first() = other.first();
            second() = other.second();
            return *this;
        }

        /**
         * Assigns other as the new content for the compressed_pair object.
         * Perform copy assignments, with the elements of its argument preserving their values after the call.
         * compressed_pair do not accept throwable copy assignable components.
         * @tparam u_type_t Type of the first component.
         * @tparam v_type_t Type of the second component.
         * @param other Another compressed_pair object.
         * @return *this
         */
        template<typename u_type_t, typename v_type_t>
        requires(hud::is_assignable_v<first_type &, const u_type_t &> && hud::is_copy_assignable_v<second_type, v_type_t>)
        constexpr compressed_pair &operator=(const compressed_pair<u_type_t, v_type_t> &other) noexcept
        {
            static_assert(hud::is_nothrow_copy_assignable_v<first_type, u_type_t>, "first_type& first_type::operator=(const u_type_t&) is throwable. compressed_pair is not designed to allow throwable copy assignable components");
            static_assert(hud::is_nothrow_copy_assignable_v<second_type, v_type_t>, "second_type& second_type::operator=(const v_type_t&) is throwable. compressed_pair is not designed to allow throwable copy assignable components");
            first() = other.first();
            second() = other.second();
            return *this;
        }

        /**
         * Assigns other as the new content for the compressed_pair object.
         * Perform move assignments, which, for elements of types supporting move semantics implies that these elements of other are left in an unspecified but valid state.
         * compressed_pair do not accept throwable move assignable components.
         * @param other Another pair object.
         * @return *this
         */
        constexpr compressed_pair &operator=(compressed_pair &&other) noexcept
        requires(hud::is_move_assignable_v<first_type> && hud::is_move_assignable_v<second_type>)
        {
            static_assert(hud::is_nothrow_move_assignable_v<first_type>, "first_type& first_type::operator=(first_type&&) is throwable. compressed_pair is not designed to allow throwable move assignable components");
            static_assert(hud::is_nothrow_move_assignable_v<second_type>, "second_type& second_type::operator=(second_type&&) is throwable. compressed_pair is not designed to allow throwable move assignable components");
            first() = hud::forward<first_type>(other.first());
            second() = hud::forward<second_type>(other.second());
            return *this;
        }

        /**
         * Assigns other as the new content for the compressed_pair object.
         * Perform move assignments, which, for elements of types supporting move semantics implies that these elements of other are left in an unspecified but valid state.
         * compressed_pair do not accept throwable move assignable components.
         * @tparam u_type_t Type of the first component.
         * @tparam v_type_t Type of the second component.
         * @param other Another compressed_pair object.
         * @return *this
         */
        template<typename u_type_t, typename v_type_t>
        requires(hud::is_assignable_v<first_type &, u_type_t &&> && hud::is_assignable_v<second_type &, v_type_t &&>)
        constexpr compressed_pair &operator=(compressed_pair<u_type_t, v_type_t> &&other) noexcept
        {
            static_assert(hud::is_nothrow_assignable_v<first_type &, u_type_t &&>, "first_type& first_type::operator=(u_type_t&&) is throwable. compressed_pair is not designed to allow throwable move assignable components");
            static_assert(hud::is_nothrow_assignable_v<second_type &, v_type_t &&>, "second_type& second_type::operator=(v_type_t&&) is throwable. compressed_pair is not designed to allow throwable move assignable components");
            first() = hud::forward<u_type_t>(other.first());
            second() = hud::forward<v_type_t>(other.second());
            return *this;
        }

        using super_type::first;
        using super_type::second;

        /**
         * Exchanges the contents of the compressed_pair object with the contents of other.
         * compressed_pair do not accept throwable swap components.
         * @param other The other compressed_pair to swap
         */
        constexpr void swap(compressed_pair &other) noexcept
        requires(hud::is_swappable_v<first_type> && hud::is_swappable_v<second_type>)
        {
            static_assert(hud::is_nothrow_swappable_v<first_type>, "swap(first_type,first_type) is throwable. compressed_pair is not designed to allow throwable swappable components");
            static_assert(hud::is_nothrow_swappable_v<second_type>, "swap(second_type,second_type) is throwable. compressed_pair is not designed to allow throwable swappable components");
            hud::swap(first(), other.first);
            hud::swap(second(), other.second);
        }

        /**
         * Tests if both elements of left and right are equal, that is, compares left.first() with right.first() and left.second() with right.second().
         * @param right The right compressed_pair of the comperand operator
         * @return true if both elements of left are equal both elements of right, false otherwise
         */
        [[nodiscard]] constexpr bool operator==(const compressed_pair &right) const noexcept
        {
            return first() == right.first() && second() == right.second();
        }

        /**
         * Compares this to left lexicographically by operator<, that is, compares the first elements and only if they are equivalent, compares the second elements.
         * @param right The right compressed_pair of the comperand operator
         * @return true if first<right.first. Otherwise, if right.first<left.first, returns false. Otherwise, if left.second<right.second, returns true. Otherwise, returns false.
         */
        [[nodiscard]] constexpr bool operator<(const compressed_pair &right) const noexcept
        {
            return first() < right.first() || (!(right.first() < first()) && second() < right.second());
        }
    };

    /**
     * swap specialization for compressed_pair
     * @tparam first_type Type of the first component in pacompressed_pairir
     * @tparam second_type Type of the second component in compressed_pair
     * @param a The compressed_pair to swap with b
     * @param b The compressed_pair to swap with a
     */
    template<typename first_type, typename second_type>
    requires(hud::is_swappable_v<first_type> && hud::is_swappable_v<second_type>)
    static constexpr void swap(compressed_pair<first_type, second_type> &a, compressed_pair<first_type, second_type> &b) noexcept
    {
        a.swap(b);
    }

    /**
     * Creates a compressed_pair object, deducing the target type from the types of arguments.
     * @tparam first_type Type of the first component
     * @tparam second_type Type of the second component
     * @return compressed_pair<V1, V2> where V1 is hud::decay_t<first_type> and V2 is hud::decay_t<second_type>
     */
    template<typename first_type, typename second_type>
    [[nodiscard]] constexpr hud::compressed_pair<hud::decay_t<first_type>, hud::decay_t<second_type>> make_compressed_pair(first_type &&value_1, second_type &&value_2) noexcept
    {
        return hud::compressed_pair<hud::decay_t<first_type>, hud::decay_t<second_type>>(hud::forward<first_type>(value_1), hud::forward<second_type>(value_2));
    }

    /**
     * tuple_size specialization for compressed_pair
     * @tparam first_type Type of the first component
     * @tparam second_type Type of the second component
     */
    template<typename first_type, typename second_type>
    struct tuple_size<hud::compressed_pair<first_type, second_type>>
        : hud::integral_constant<usize, 2>
    {
    };

    /**
     * tuple_element specilization for compressed_pair
     * @tparam idx_to_reach Index of the type to reach
     * @tparam first_type Type of the first component
     * @tparam second_type Type of the second component
     */
    template<usize idx_to_reach, typename first_type, typename second_type>
    struct tuple_element<idx_to_reach, compressed_pair<first_type, second_type>>
    {
        static_assert(idx_to_reach < 2, "compressed_pair index out of bounds");
        using type = hud::conditional_t<idx_to_reach == 0, first_type, second_type>;
    };

    /**
     * Retrieves a lvalue reference to the member first if index is 0, second if index is 1.
     * @tparam element_index The index of the element
     * @tparam first_type Type of the first component
     * @tparam second_type Type of the second component
     * @param compressed_pair The compressed_pair to access
     * @return LValue reference to the member first if index is 0, second if index is 1.
     */
    template<usize element_index, typename first_type, typename second_type>
    [[nodiscard]] constexpr hud::tuple_element_t<element_index, compressed_pair<first_type, second_type>> &get(compressed_pair<first_type, second_type> &compressed_pair) noexcept
    {
        static_assert(element_index < 2, "compressed_pair index out of bounds");
        if constexpr (element_index == 0)
        {
            return compressed_pair.first();
        }
        else if constexpr (element_index == 1)
        {
            return compressed_pair.second();
        }
    }

    /**
     * Retrieves a lvalue reference to the member first if index is 0, second if index is 1.
     * @tparam element_index The index of the element
     * @tparam first_type Type of the first component
     * @tparam second_type Type of the second component
     * @param compressed_pair The compressed_pair to access
     * @return LValue reference to the member first if index is 0, second if index is 1.
     */
    template<usize element_index, typename first_type, typename second_type>
    [[nodiscard]] constexpr const hud::tuple_element_t<element_index, const compressed_pair<first_type, second_type>> &get(const compressed_pair<first_type, second_type> &compressed_pair) noexcept
    {
        static_assert(element_index < 2, "compressed_pair index out of bounds");
        if constexpr (element_index == 0)
        {
            return compressed_pair.first();
        }
        else if constexpr (element_index == 1)
        {
            return compressed_pair.second();
        }
    }

    /**
     * Retrieves an rvalue reference to the member first if index is 0, second if index is 1.
     * @tparam element_index The index of the element
     * @tparam first_type Type of the first component
     * @tparam second_type Type of the second component
     * @param compressed_pair The compressed_pair to access
     * @return RValue reference to the member first if index is 0, second if index is 1.
     */
    template<usize element_index, typename first_type, typename second_type>
    [[nodiscard]] constexpr hud::tuple_element_t<element_index, compressed_pair<first_type, second_type>> &&get(compressed_pair<first_type, second_type> &&compressed_pair) noexcept
    {
        static_assert(element_index < 2, "compressed_pair index out of bounds");
        if constexpr (element_index == 0)
        {
            return hud::forward<first_type &&>(compressed_pair.first());
        }
        else if constexpr (element_index == 1)
        {
            return hud::forward<second_type &&>(compressed_pair.second());
        }
    }

    /**
     * Retrieves an rvalue reference to the member first if index is 0, second if index is 1.
     * @tparam element_index The index of the element
     * @tparam first_type Type of the first component
     * @tparam second_type Type of the second component
     * @param compressed_pair The compressed_pair to access
     * @return RValue reference to the member first if index is 0, second if index is 1.
     */
    template<usize element_index, typename first_type, typename second_type>
    [[nodiscard]] constexpr const hud::tuple_element_t<element_index, compressed_pair<first_type, second_type>> &&get(const compressed_pair<first_type, second_type> &&compressed_pair) noexcept
    {
        static_assert(element_index < 2, "compressed_pair index out of bounds");
        if constexpr (element_index == 0)
        {
            return hud::forward<const first_type &&>(compressed_pair.first());
        }
        else if constexpr (element_index == 1)
        {
            return hud::forward<const second_type &&>(compressed_pair.second());
        }
    }

    /**
     * Tests if both elements of left and right are equal, that is, compares left.first() with right.first() and left.second() with right.second().
     * @tparam first_type Type of the first component
     * @tparam second_type Type of the second component
     * @param left The left compressed_pair of the comperand operator
     * @param right The right compressed_pair of the comperand operator
     * @return true if both elements of left are equal both elements of right, false otherwise
     */
    template<typename first_type, typename second_type>
    [[nodiscard]] constexpr bool operator==(const compressed_pair<first_type, second_type> &left, const compressed_pair<first_type, second_type> &right) noexcept
    {
        return left.operator==(right);
    }

    /**
     * Tests if elements of left or right are not equal, that is, compares left.first with right.first and left.second with right.second.
     * @tparam first_type Type of the first component
     * @tparam second_type Type of the second component
     * @param left The left compressed_pair of the comperand operator
     * @param right The right compressed_pair of the comperand operator
     * @return true if elements of left are not equals to elements of right, false otherwise
     */
    template<typename first_type, typename second_type>
    [[nodiscard]] constexpr bool operator!=(const compressed_pair<first_type, second_type> &left, const compressed_pair<first_type, second_type> &right) noexcept
    {
        return !(left == right);
    }

    /**
     * Compares right and left lexicographically by operator<, that is, compares the first elements and only if they are equivalent, compares the second elements.
     * @tparam first_type Type of the first component
     * @tparam second_type Type of the second component
     * @param left The left compressed_pair of the comperand operator
     * @param right The right compressed_pair of the comperand operator
     * @return true if left.first<right.first. Otherwise, if right.first<left.first, returns false. Otherwise, if left.second<right.second, returns true. Otherwise, returns false.
     */
    template<typename first_type, typename second_type>
    [[nodiscard]] constexpr bool operator<(const compressed_pair<first_type, second_type> &left, const compressed_pair<first_type, second_type> &right) noexcept
    {
        return left.operator==(right);
    }

    /**
     * Compares right and left lexicographically by operator<, that is, compares the first elements and only if they are equivalent, compares the second elements.
     * @tparam first_type Type of the first component
     * @tparam second_type Type of the second component
     * @param left The left compressed_pair of the comperand operator
     * @param right The right compressed_pair of the comperand operator
     * @return true if right.first<left.first. Otherwise, if left.first<right.first, returns false. Otherwise, if right.second<left.second, returns true. Otherwise, returns false.
     */
    template<typename first_type, typename second_type>
    [[nodiscard]] constexpr bool operator>(const compressed_pair<first_type, second_type> &left, const compressed_pair<first_type, second_type> &right) noexcept
    {
        return right < left;
    }

    /**
     * Compares right and left lexicographically by operator<, that is, compares the first elements and only if they are equivalent, compares the second elements.
     * @tparam first_type Type of the first component
     * @tparam second_type Type of the second component
     * @param left The left compressed_pair of the comperand operator
     * @param right The right compressed_pair of the comperand operator
     * @return false if right.first<left.first. Otherwise, if left.first<right.first, returns true. Otherwise, if right.second<left.second, returns false. Otherwise, returns true.
     */
    template<typename first_type, typename second_type>
    [[nodiscard]] constexpr bool operator<=(const compressed_pair<first_type, second_type> &left, const compressed_pair<first_type, second_type> &right) noexcept
    {
        return !(right < left);
    }

    /**
     * Compares right and left lexicographically by operator<, that is, compares the first elements and only if they are equivalent, compares the second elements.
     * @tparam first_type Type of the first component
     * @tparam second_type Type of the second component
     * @param left The left compressed_pair of the comperand operator
     * @param right The right compressed_pair of the comperand operator
     * @return false if left.first<right.first. Otherwise, if right.first<left.first, returns true. Otherwise, if left.second<right.second, returns false. Otherwise, returns true.
     */
    template<typename first_type, typename second_type>
    [[nodiscard]] constexpr bool operator>=(const compressed_pair<first_type, second_type> &left, const compressed_pair<first_type, second_type> &right) noexcept
    {
        return !(left < right);
    }

} // namespace hud

#endif // HD_INC_CORE_COMPRESSED_TYPE_H