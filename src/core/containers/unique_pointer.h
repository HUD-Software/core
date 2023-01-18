#ifndef HD_INC_CORE_UNIQUE_POINTER_H
#define HD_INC_CORE_UNIQUE_POINTER_H
#include "../minimal.h"
#include "../traits/void_t.h"
#include "../traits/remove_reference.h"
#include "../traits/enable_if.h"
#include "../traits/disjunction.h"
#include "../traits/conjunction.h"
#include "../traits/negation.h"
#include "../traits/is_pointer.h"
#include "../traits/is_default_constructible.h"
#include "../traits/conditional.h"
#include "../traits/is_reference.h"
#include "../traits/add_const.h"
#include "../traits/add_lvalue_reference.h"
#include "../traits/is_convertible.h"
#include "../traits/is_array.h"
#include "../traits/is_same.h"
#include "../traits/is_move_assignable.h"
#include "../traits/common_type.h"
#include "../traits/is_array.h"
#include "../traits/is_bounded_array.h"
#include "../traits/remove_all_extents.h"
#include "../traits/remove_pointer.h"
#include "../traits/is_nullptr.h"
#include "../traits/is_nothrow_default_constructible.h"
#include "../traits/is_nothrow_move_constructible.h"
#include "../traits/is_nothrow_copy_constructible.h"
#include "../traits/is_nothrow_move_assignable.h"
#include "../traits/is_unbounded_array.h"

#include "../templates/select_deleter_pointer_type.h"
#include "../templates/default_deleter.h"
#include "../templates/move.h"
#include "../templates/forward.h"
#include "../templates/swap.h"
#include "../templates/less.h"

#include "tuple.h"
#include "../hash.h"

namespace hud
{

    template<typename type_t, typename deleter_type = hud::default_deleter<type_t>>
    class unique_pointer;

    namespace details
    {

        /**
         * deleter_type parameter type.
         * If deleter_type is a reference, it's passed by value, else by const reference
         * @tparam deleter_type  The deleter type of the unique_pointer
         */
        template<typename deleter_type>
        using deleter_param_type = hud::conditional_t<hud::is_reference_v<deleter_type>, deleter_type, hud::add_lvalue_reference_t<hud::add_const_t<deleter_type>>>;

        /**
         * Checks whether a unique_pointer<u_ptr_t, u_deleter_t>::pointer is convertible to unique_pointer<type_t, deleter_type>::pointer and unique_pointer<u_ptr_t, u_deleter_t>::pointer is not an array.
         * @tparam unique_pointer_t The unique_pointer<type_t>
         * @tparam u_unique_pointer_t The unique_pointer<u_ptr_t>
         */
        template<typename unique_pointer_t, typename u_unique_pointer_t>
        struct is_pointer_convertible_and_not_array
            : hud::false_type
        {
        };

        template<typename ptr_t, typename deleter_type, typename u_ptr_t, typename u_deleter_t>
        struct is_pointer_convertible_and_not_array<unique_pointer<ptr_t, deleter_type>, unique_pointer<u_ptr_t, u_deleter_t>>
            : hud::conjunction<
                  hud::is_convertible<typename unique_pointer<u_ptr_t, u_deleter_t>::pointer_type, typename unique_pointer<ptr_t, deleter_type>::pointer_type>,
                  hud::negation<hud::is_array<u_ptr_t>>>
        {
        };

        /**
         * Checks whether a unique_pointer<u_ptr_t, u_deleter_t> is convertible to unique_pointer<type_t, deleter_type>.
         * Provides the member constant Value which is equal to true, if unique_pointer<u_ptr_t, u_deleter_t> is convertible to unique_pointer<type_t, deleter_type>.
         * Otherwise, Value is equal to false.
         * @tparam unique_pointer_t The unique_pointer<type_t>
         * @tparam u_unique_pointer_t The unique_pointer<u_ptr_t>
         */
        template<typename unique_pointer_t, typename u_unique_pointer_t>
        struct is_unique_pointer_convertible_to_unique_pointer
            : hud::false_type
        {
        };

        template<typename ptr_t, typename deleter_type, typename u_ptr_t, typename u_deleter_t>
        struct is_unique_pointer_convertible_to_unique_pointer<unique_pointer<ptr_t, deleter_type>, unique_pointer<u_ptr_t, u_deleter_t>>
            : hud::conjunction<
                  is_pointer_convertible_and_not_array<unique_pointer<ptr_t, deleter_type>, unique_pointer<u_ptr_t, u_deleter_t>>,
                  hud::conditional_t<hud::is_reference_v<deleter_type>, hud::is_same<deleter_type, u_deleter_t>, hud::is_convertible<u_deleter_t, deleter_type>>>
        {
        };

        template<typename unique_pointer_t, typename u_unique_pointer_t>
        static constexpr bool is_unique_pointer_convertible_to_unique_pointer_v = is_unique_pointer_convertible_to_unique_pointer<unique_pointer_t, u_unique_pointer_t>::value;

        /**
         * Check whether a unique_pointer<u_ptr_t> is move assignable to unique_pointer<type_t> or not.
         * @tparam unique_pointer_t The unique_pointer<type_t>
         * @tparam u_unique_pointer_t The unique_pointer<u_ptr_t>
         */
        template<typename unique_pointer_t, typename u_unique_pointer_t>
        struct is_unique_pointer_move_assignable_to_unique_pointer
            : hud::false_type
        {
        };

        template<typename ptr_t, typename deleter_type, typename u_ptr_t, typename u_deleter_t>
        struct is_unique_pointer_move_assignable_to_unique_pointer<unique_pointer<ptr_t, deleter_type>, unique_pointer<u_ptr_t, u_deleter_t>>
            : hud::conjunction<
                  is_pointer_convertible_and_not_array<unique_pointer<ptr_t, deleter_type>, unique_pointer<u_ptr_t, u_deleter_t>>,
                  hud::is_move_assignable<deleter_type, u_deleter_t>>
        {
        };

        template<typename unique_pointer_t, typename u_unique_pointer_t>
        static constexpr bool is_unique_pointer_move_assignable_to_unique_pointer_v = is_unique_pointer_move_assignable_to_unique_pointer<unique_pointer_t, u_unique_pointer_t>::value;

        /**
         * Check whether a u_ptr_t* can be used to reset a unique_pointer<type_t> or not.
         * @tparam unique_pointer_t The unique_pointer<type_t[]>
         * @tparam u_ptr_t The pointer u_ptr_t* used to reset unique_pointer_t
         */
        template<typename unique_pointer_t, typename u_ptr_t>
        struct is_unique_pointer_array_can_be_reset_with
            : hud::false_type
        {
        };

        template<typename ptr_t, typename deleter_type, typename u_ptr_t>
        struct is_unique_pointer_array_can_be_reset_with<unique_pointer<ptr_t[], deleter_type>, u_ptr_t *>
            : hud::disjunction<
                  hud::is_same<u_ptr_t *, typename unique_pointer<ptr_t[], deleter_type>::pointer_type>,
                  hud::conjunction<
                      hud::is_same<typename unique_pointer<ptr_t[], deleter_type>::pointer_type, typename unique_pointer<ptr_t[], deleter_type>::element_type *>,
                      hud::is_convertible<u_ptr_t (*)[], ptr_t (*)[]>>>
        {
        };

        template<typename type_t, typename u_ptr_t>
        static constexpr bool is_unique_pointer_array_can_be_reset_with_v = is_unique_pointer_array_can_be_reset_with<type_t, u_ptr_t>::value;

        /**
         * Checks whether a u_ptr_t* is convertible to unique_pointer<type_t[], deleter_type>.
         * Provides the member constant Value which is equal to true, if u_ptr_t* is convertible to unique_pointer<type_t, deleter_type>.
         * Otherwise, Value is equal to false.
         * @tparam unique_pointer_t The unique_pointer<type_t[]>
         * @tparam u_ptr_t The pointer u_ptr_t* used to reset unique_pointer_t
         */
        template<typename unique_pointer_t, typename u_ptr_t>
        struct is_pointer_convertible_to_unique_pointer_array
            : hud::disjunction<is_unique_pointer_array_can_be_reset_with<unique_pointer_t, u_ptr_t>, is_nullptr<u_ptr_t>>
        {
        };

        template<typename unique_pointer_t, typename u_ptr_t>
        static constexpr bool is_pointer_convertible_to_unique_pointer_array_v = is_pointer_convertible_to_unique_pointer_array<unique_pointer_t, u_ptr_t>::value;

        /**
         * Checks whether a unique_pointer<u_ptr_t> is convertible to an unique_pointer<type_t[]>
         * @tparam unique_pointer_t The unique_pointer<type_t[]>
         * @tparam u_unique_pointer_t The unique_pointer<u_ptr_t>
         */
        template<typename unique_pointer_t, typename u_unique_pointer_t>
        struct is_unique_pointer_convertible_to_unique_pointer_array
            : hud::false_type
        {
        };

        template<typename ptr_t, typename deleter_type, typename u_ptr_t, typename u_deleter_t>
        struct is_unique_pointer_convertible_to_unique_pointer_array<unique_pointer<ptr_t, deleter_type>, unique_pointer<u_ptr_t, u_deleter_t>>
            : hud::conjunction<
                  hud::is_array<u_ptr_t>,
                  hud::is_same<typename unique_pointer<ptr_t, deleter_type>::element_type *, typename unique_pointer<ptr_t, deleter_type>::pointer_type>,
                  hud::is_same<typename unique_pointer<u_ptr_t, u_deleter_t>::element_type *, typename unique_pointer<u_ptr_t, u_deleter_t>::pointer_type>,
                  hud::is_convertible<typename unique_pointer<u_ptr_t, u_deleter_t>::element_type (*)[], typename unique_pointer<ptr_t, deleter_type>::element_type (*)[]>>
        {
        };

        /**
         * Checks whether a unique_pointer<type_t[]> is move constructible from unique_pointer<u_ptr_t>
         * @tparam unique_pointer_t The unique_pointer<type_t[]>
         * @tparam u_unique_pointer_t The unique_pointer<u_ptr_t>
         */
        template<typename unique_pointer_t, typename u_unique_pointer_t>
        struct is_unique_pointer_array_move_constructible_from_unique_pointer
            : hud::false_type
        {
        };

        template<typename ptr_t, typename deleter_type, typename u_ptr_t, typename u_deleter_t>
        struct is_unique_pointer_array_move_constructible_from_unique_pointer<unique_pointer<ptr_t, deleter_type>, unique_pointer<u_ptr_t, u_deleter_t>>
            : hud::conjunction<
                  is_unique_pointer_convertible_to_unique_pointer_array<unique_pointer<ptr_t, deleter_type>, unique_pointer<u_ptr_t, u_deleter_t>>,
                  hud::conditional_t<hud::is_reference_v<deleter_type>, hud::is_same<deleter_type, u_deleter_t>, hud::is_convertible<u_deleter_t, deleter_type>>>
        {
        };

        template<typename unique_pointer_t, typename u_unique_pointer_t>
        static constexpr bool is_unique_pointer_array_move_constructible_from_unique_pointer_v = is_unique_pointer_array_move_constructible_from_unique_pointer<unique_pointer_t, u_unique_pointer_t>::value;

        /**
         * Checks whether a unique_pointer<u_ptr_t> is move assignable to a unique_pointer<type_t[]>
         * @tparam unique_pointer_t The unique_pointer<type_t[]>
         * @tparam u_unique_pointer_t The unique_pointer<u_ptr_t>
         */
        template<typename unique_pointer_t, typename u_unique_pointer_t>
        struct is_unique_pointer_move_assignable_to_unique_pointer_array
            : hud::false_type
        {
        };

        template<typename ptr_t, typename deleter_type, typename u_ptr_t, typename u_deleter_t>
        struct is_unique_pointer_move_assignable_to_unique_pointer_array<unique_pointer<ptr_t, deleter_type>, unique_pointer<u_ptr_t, u_deleter_t>>
            : hud::conjunction<
                  is_unique_pointer_convertible_to_unique_pointer_array<unique_pointer<ptr_t, deleter_type>, unique_pointer<u_ptr_t, u_deleter_t>>,
                  hud::is_move_assignable<deleter_type, u_deleter_t>>
        {
        };

        template<typename unique_pointer_t, typename u_unique_pointer_t>
        static constexpr bool is_unique_pointer_move_assignable_to_unique_pointer_array_v = is_unique_pointer_move_assignable_to_unique_pointer_array<unique_pointer_t, u_unique_pointer_t>::value;

        template<typename type_t, typename deleter_t>
        class unique_pointer_impl
        {
        public:
            /** The deleter type. */
            using deleter_type = deleter_t;
            /** The pointer type. Result of hud::select_deleter_pointer_type_t<<type_t,deleter_type>*/
            using pointer_type = hud::select_deleter_pointer_type_t<type_t, deleter_type>;
            /** The element pointed by the pointer. */
            using element_type = type_t;

            static_assert(!hud::is_rvalue_reference_v<deleter_type>, "the specified deleter type cannot be an rvalue reference");
            static_assert(hud::is_convertible_v<element_type *, pointer_type>, "element_type is no convertible to deleter_type::pointer_type");

            /**
             * Default constructor that value-initializes the stored pointer and the stored deleter.
             * unique_pointer do not accept throwable default constructible deleter.
             */
            HD_FORCEINLINE constexpr unique_pointer_impl() noexcept
            requires(hud::is_default_constructible_v<deleter_type> && !hud::is_pointer_v<deleter_type>)
                : inner(taginit)
            {
                static_assert(hud::is_nothrow_default_constructible_v<deleter_type>, "unique_pointer do not accept throwable default constructible deleter_type");
            }

            /**
             * Construct a unique_pointer form a raw pointer to own.
             * unique_pointer do not accept throwable default constructible deleter
             * @param pointer The poiner to own
             */
            HD_FORCEINLINE constexpr unique_pointer_impl(pointer_type pointer, tag_init) noexcept
            requires(hud::is_default_constructible_v<deleter_type> && !hud::is_pointer_v<deleter_type>)
                : inner(pointer, taginit)
            {
                static_assert(hud::is_nothrow_default_constructible_v<deleter_type>, "unique_pointer do not accept throwable default constructible deleter_type");
            }

            /**
             * Construct a unique_pointer from a pointer to own and a deleter.
             * @param pointer The poiner to own
             * @param deleter The deleter used to delete the owned pointer
             */
            HD_FORCEINLINE constexpr unique_pointer_impl(pointer_type pointer, const deleter_type &deleter) noexcept
            requires(hud::is_constructible_v<deleter_type, const deleter_type &>)
                : inner(pointer, deleter)
            {
                static_assert(hud::is_nothrow_copy_constructible_v<deleter_type>, "unique_pointer do not accept throwable copy constructible deleter_type");
            }

            /**
             * Construct a unique_pointer from a pointer to own and a deleter.
             * @param pointer The poiner to own
             * @param deleter The deleter used to delete the owned pointer
             */
            template<typename u_pointer_t, typename u_deleter_t>
            HD_FORCEINLINE constexpr unique_pointer_impl(u_pointer_t pointer, u_deleter_t &&deleter) noexcept
            requires(hud::is_move_constructible_v<deleter_type>)
                : inner(pointer, hud::forward<u_deleter_t>(deleter))
            {
                static_assert(hud::is_nothrow_move_constructible_v<deleter_type>, "unique_pointer do not accept throwable move constructible deleter_type");
            }

            /**
             * Construct a unique_pointer by stealing another unique_pointer ownership.
             * @param other The unique_pointer to transfert ownership from.
             */
            HD_FORCEINLINE constexpr unique_pointer_impl(unique_pointer_impl &&other) noexcept
            requires(hud::is_move_constructible_v<deleter_type>)
                : inner(other.leak(), hud::forward<deleter_type>(other.deleter()))
            {
                static_assert(hud::is_reference_v<deleter_type> ? hud::is_nothrow_move_constructible_v<deleter_type> : true, "unique_pointer do not accept throwable move constructible deleter_type");
            }

            /**
             * Assigns the unique_pointer by stealing the ownership of the pointer.
             * @tparam u_ptr_t Type of the other unique_pointer's pointer
             * @tparam u_deleter_t  Type of the other unique_pointer's deleter
             * @param other The unique_pointer to transfert ownership from.
             * @return *this
             */
            template<typename u_ptr_t, typename u_deleter_t>
            requires(hud::is_move_assignable_v<deleter_type, u_deleter_t>)
            constexpr unique_pointer_impl &operator=(unique_pointer_impl<u_ptr_t, u_deleter_t> &&other) noexcept
            {
                static_assert(!hud::is_reference_v<deleter_type> ? hud::is_nothrow_move_assignable_v<deleter_type> : true, "unique_pointer do not accept throwable move assignable deleter_type if deleter_type is no a reference");
                static_assert(hud::is_reference_v<deleter_type> ? hud::is_nothrow_copy_assignable_v<hud::remove_reference_t<deleter_type>> : true, "unique_pointer do not accept throwable copy assignable deleter_type if deleter_type is reference)");

                reset(other.leak());
                deleter() = hud::forward<u_deleter_t>(other.deleter());
                return *this;
            }

            /**
             * Assigns the unique_pointer with a nullptr. Same as reset().
             * @param A nullptr
             * @return *this
             */
            HD_FORCEINLINE constexpr unique_pointer_impl &operator=(hud::ptr::null) noexcept
            {
                reset();
                return *this;
            }

            /** Retrieves the owned pointer. */
            [[nodiscard]] HD_FORCEINLINE constexpr pointer_type pointer() const noexcept
            {
                return hud::get<0>(inner);
            }

            /** Retrieves the deleter. */
            [[nodiscard]] HD_FORCEINLINE constexpr deleter_type &deleter() noexcept
            {
                return hud::get<1>(inner);
            }

            /** Retrieves a reference to the deleter. */
            [[nodiscard]] HD_FORCEINLINE constexpr const deleter_type &deleter() const noexcept
            {
                return hud::get<1>(inner);
            }

            /** Release the ownership of the pointer and return it without calling the deleter on the owned pointer. */
            [[nodiscard]] constexpr pointer_type leak() noexcept
            {
                pointer_type old = pointer();
                hud::get<0>(inner) = pointer_type();
                return old;
            }

            /** Check whether the unique_pointer own a pointer. */
            [[nodiscard]] HD_FORCEINLINE constexpr bool is_owning() const noexcept
            {
                return pointer() != pointer_type();
            }

            /** Check whether the unique_pointer own a pointer. */
            [[nodiscard]] HD_FORCEINLINE constexpr explicit operator bool() const noexcept
            {
                return is_owning();
            }

            /** Call the deleter on the owned pointer and optionally take ownership of a new pointer. */
            constexpr void reset(pointer_type ptr) noexcept
            {
                pointer_type old_ptr = pointer();
                hud::get<0>(inner) = ptr;
                deleter()(old_ptr);
            }

            /** Call the deleter on the owned pointer and taking no ownership. */
            HD_FORCEINLINE constexpr void reset(hud::ptr::null) noexcept
            {
                deleter()(pointer());
                hud::get<0>(inner) = pointer_type();
            }

            /** Call the deleter on the owned pointer and taking no ownership. */
            HD_FORCEINLINE constexpr void reset() noexcept
            {
                reset(nullptr);
            }

            /** Swap with another unique_pointer. */
            HD_FORCEINLINE constexpr void swap(unique_pointer_impl &other) noexcept
            {
                hud::swap(inner, other.inner);
            }

            /** Call the deleter on the pointer if any owned. */
            constexpr ~unique_pointer_impl() noexcept
            {
                if (is_owning())
                {
                    deleter()(pointer());
                }
            }

        protected:
            /** hud::tuple containing the pointer and the deleter */
            hud::tuple<pointer_type, deleter_type> inner;
        };
    } // namespace details

    /**
     * unique_pointer is a smart pointer that owns and manages another object through a pointer.
     * unique_pointer object automatically delete the object they manage (using a deleter) as soon as they themselves are destroyed, or as soon as their value changes either by an assignment operation or by an explicit call to unique_pointer::reset.
     * @tparam type_t The type of the pointer to own
     * @tparam deleter_t the deleter used to delete the type_t pointer
     */
    template<typename type_t, typename deleter_t>
    class unique_pointer : public details::unique_pointer_impl<type_t, deleter_t>
    {
    private:
        using base_type = details::unique_pointer_impl<type_t, deleter_t>;

    public:
        /** The deleter type. */
        using typename base_type::deleter_type;
        /** The pointer type. Result of hud::select_deleter_pointer_type_t<<type_t,deleter_type>*/
        using typename base_type::pointer_type;
        /** The element pointed by the pointer. */
        using typename base_type::element_type;

        /**
         * Default constructor that value-initializes the stored pointer and the stored deleter.
         * unique_pointer do not accept throwable default constructible deleter.
         */
        HD_FORCEINLINE constexpr unique_pointer() noexcept
            : base_type()
        {
        }

        /**
         * Construct a unique_pointer form a raw pointer to own.
         * unique_pointer do not accept throwable default constructible deleter
         * @param pointer The poiner to own
         */
        HD_FORCEINLINE constexpr explicit unique_pointer(pointer_type pointer) noexcept
            : base_type(pointer, taginit)
        {
        }

        /**
         * Construct a unique_pointer from a pointer to own and a deleter.
         * @param pointer The poiner to own
         * @param deleter The deleter used to delete the owned pointer
         */
        HD_FORCEINLINE constexpr unique_pointer(pointer_type pointer, const deleter_type &deleter) noexcept
            : base_type(pointer, deleter)
        {
        }

        /**
         * Construct a unique_pointer from a pointer to own and a deleter.
         * @param pointer The poiner to own
         * @param deleter The deleter used to delete the owned pointer
         */
        HD_FORCEINLINE constexpr unique_pointer(pointer_type pointer, deleter_type &&deleter) noexcept
        requires(!hud::is_reference_v<deleter_type>)
            : base_type(pointer, hud::move(deleter))
        {
        }

        /**
         * Construct a unique_pointer from a raw pointer to own and a reference to a deleter.
         * @param pointer The poiner to own
         * @param deleter A reference to a deleter
         */
        unique_pointer(pointer_type pointer, hud::remove_reference_t<deleter_type> &&deleter) noexcept
        requires(hud::is_reference_v<deleter_type> && hud::is_constructible_v<deleter_type, hud::remove_reference_t<deleter_type>>)
        = delete;

        /** Construct a unique_pointer from a nullptr */
        HD_FORCEINLINE constexpr unique_pointer(hud::ptr::null) noexcept
            : base_type()
        {
        }

        /**
         * Construct a unique_pointer by stealing another unique_pointer ownership.
         * @param other The unique_pointer to transfert ownership from.
         */
        HD_FORCEINLINE constexpr unique_pointer(unique_pointer &&other) noexcept
            : base_type(other.leak(), hud::forward<deleter_type>(other.deleter()))
        {
        }

        /**
         * Construct a unique_pointer by stealing another unique_pointer ownership.
         * @tparam u_ptr_t Type of the unique_pointer's pointer
         * @tparam u_deleter_t  Type of the unique_pointer's deleter
         * @param other The unique_pointer to transfert ownership from.
         */
        template<typename u_ptr_t, typename u_deleter_t>
        requires(details::is_unique_pointer_convertible_to_unique_pointer_v<unique_pointer, unique_pointer<u_ptr_t, u_deleter_t>>)
        HD_FORCEINLINE constexpr unique_pointer(unique_pointer<u_ptr_t, u_deleter_t> &&other) noexcept
            : base_type(other.leak(), hud::forward<u_deleter_t>(other.deleter()))
        {
            static_assert(hud::is_nothrow_move_constructible_v<deleter_type, u_deleter_t>, "unique_pointer do not accept throwable move constructible deleter_type");
        }

        /**
         * Assigns the unique_pointer by stealing the ownership of the pointer.
         * @param other The unique_pointer to transfert ownership from.
         * @return *this
         */
        constexpr unique_pointer &operator=(unique_pointer &&other) noexcept
        {
            base_type::operator=(hud::move(other));
            return *this;
        }

        /**
         * Assigns the unique_pointer by stealing the ownership of the pointer.
         * @tparam u_ptr_t Type of the other unique_pointer's pointer
         * @tparam u_deleter_t  Type of the other unique_pointer's deleter
         * @param other The unique_pointer to transfert ownership from.
         * @return *this
         */
        template<typename u_ptr_t, typename u_deleter_t>
        requires(details::is_unique_pointer_move_assignable_to_unique_pointer_v<unique_pointer, unique_pointer<u_ptr_t, u_deleter_t>>)
        constexpr unique_pointer &operator=(unique_pointer<u_ptr_t, u_deleter_t> &&other) noexcept
        {
            base_type::operator=(hud::move(other));
            return *this;
        }

        /**
         * Assigns the unique_pointer with a nullptr. Same as reset().
         * @param A nullptr
         * @return *this
         */
        HD_FORCEINLINE constexpr unique_pointer &operator=(hud::ptr::null) noexcept
        {
            base_type::operator=(nullptr);
            return *this;
        }

        /** Dereference owned pointer. */
        [[nodiscard]] HD_FORCEINLINE constexpr hud::add_lvalue_reference_t<element_type> operator*() const noexcept
        {
            return *base_type::pointer();
        }

        /** Retrieves the owned pointer. */
        [[nodiscard]] HD_FORCEINLINE constexpr pointer_type operator->() const noexcept
        {
            return base_type::pointer();
        }

    private:
        /** Not copy constructible. */
        unique_pointer(const unique_pointer &) = delete;
        /** Not copyable. */
        unique_pointer &operator=(const unique_pointer &) = delete;
    };

    /**
     * unique_pointer is a smart pointer that owns and manages another object through a pointer.
     * unique_pointer object automatically delete the object they manage (using a deleter) as soon as they themselves are destroyed, or as soon as their value changes either by an assignment operation or by an explicit call to unique_pointer::reset.
     */
    template<typename type_t, typename deleter_t>
    class unique_pointer<type_t[], deleter_t>
        : public details::unique_pointer_impl<type_t, deleter_t>
    {
    private:
        using base_type = details::unique_pointer_impl<type_t, deleter_t>;

    public:
        /** The deleter type. */
        using typename base_type::deleter_type;
        /** The pointer type. Result of hud::select_deleter_pointer_type_t<<type_t,deleter_type>*/
        using typename base_type::pointer_type;
        /** The element pointed by the pointer. */
        using typename base_type::element_type;

        /**
         * Default constructor that value-initializes the stored pointer and the stored deleter.
         * unique_pointer do not accept throwable default constructible deleter.
         */
        HD_FORCEINLINE constexpr unique_pointer() noexcept
            : base_type()
        {
        }

        /**
         * Construct a unique_pointer form a raw pointer to own.
         * unique_pointer do not accept throwable default constructible deleter
         * @param pointer The poiner to own
         */
        template<typename u_ptr_t>
        requires(details::is_pointer_convertible_to_unique_pointer_array_v<unique_pointer, u_ptr_t>)
        HD_FORCEINLINE constexpr explicit unique_pointer(u_ptr_t pointer) noexcept
            : base_type(pointer, taginit)
        {
        }

        /**
         * Construct a unique_pointer from a pointer to own and a deleter.
         * @param pointer The poiner to own
         * @param deleter The deleter used to delete the owned pointer
         */
        template<typename u_ptr_t>
        requires(details::is_pointer_convertible_to_unique_pointer_array_v<unique_pointer, u_ptr_t>)
        HD_FORCEINLINE constexpr unique_pointer(u_ptr_t pointer, const deleter_type &deleter) noexcept
            : base_type(pointer, deleter)
        {
        }

        /**
         * Construct a unique_pointer from a pointer to own and a deleter.
         * @param pointer The poiner to own
         * @param deleter The deleter used to delete the owned pointer
         */
        template<typename u_ptr_t>
        requires(!hud::is_reference_v<deleter_type> && details::is_pointer_convertible_to_unique_pointer_array_v<unique_pointer, u_ptr_t>)
        HD_FORCEINLINE constexpr unique_pointer(u_ptr_t pointer, deleter_type &&deleter) noexcept
            : base_type(pointer, hud::move(deleter))
        {
        }

        /** Do not accept deleter_type is a reference will allow the move of it. */
        unique_pointer(pointer_type pointer, hud::remove_reference_t<deleter_type> &&deleter) noexcept
        requires(hud::is_reference_v<deleter_type> && hud::is_constructible_v<deleter_type, hud::remove_reference_t<deleter_type>>)
        = delete;

        /** Construct a unique_pointer from a nullptr. */
        HD_FORCEINLINE constexpr unique_pointer(hud::ptr::null) noexcept
            : base_type()
        {
        }

        /**
         * Construct a unique_pointer by stealing another unique_pointer ownership.
         * @param other The unique_pointer to transfert ownership from.
         */
        HD_FORCEINLINE constexpr unique_pointer(unique_pointer &&other) noexcept
            : base_type(other.leak(), hud::forward<deleter_type>(other.deleter()))
        {
        }

        /**
         * Construct a unique_pointer by stealing another unique_pointer ownership.
         * @tparam u_ptr_t Type of the unique_pointer's pointer
         * @tparam u_deleter_t  Type of the unique_pointer's deleter
         * @param other The unique_pointer to transfert ownership from.
         */
        template<typename u_ptr_t, typename u_deleter_t>
        requires(details::is_unique_pointer_array_move_constructible_from_unique_pointer_v<unique_pointer, unique_pointer<u_ptr_t, u_deleter_t>>)
        HD_FORCEINLINE constexpr unique_pointer(unique_pointer<u_ptr_t, u_deleter_t> &&other) noexcept
            : base_type(other.leak(), hud::forward<u_deleter_t>(other.deleter()))
        {
            static_assert(hud::is_nothrow_move_constructible_v<deleter_type, u_deleter_t>, "unique_pointer do not accept throwable move constructible deleter_type");
        }

        /**
         * Assigns the unique_pointer by stealing the ownership of the pointer.
         * @param other The unique_pointer to transfert ownership from.
         * @return *this
         */
        constexpr unique_pointer &operator=(unique_pointer &&other) noexcept
        requires(hud::is_move_assignable_v<deleter_type>)
        {
            base_type::operator=(hud::move(other));
            return *this;
        }

        /**
         * Assigns the unique_pointer by stealing the ownership of the pointer.
         * @tparam u_ptr_t Type of the other unique_pointer's pointer
         * @tparam u_deleter_t  Type of the other unique_pointer's deleter
         * @param other The unique_pointer to transfert ownership from.
         * @return *this
         */
        template<typename u_ptr_t, typename u_deleter_t>
        requires(details::is_unique_pointer_move_assignable_to_unique_pointer_array_v<unique_pointer, unique_pointer<u_ptr_t, u_deleter_t>>)
        constexpr unique_pointer &operator=(unique_pointer<u_ptr_t, u_deleter_t> &&other) noexcept
        {
            base_type::operator=(hud::move(other));
            return *this;
        }

        /**
         * Assigns the unique_pointer with a nullptr. Same as reset().
         * @param A nullptr
         * @return *this
         */
        HD_FORCEINLINE constexpr unique_pointer &operator=(hud::ptr::null) noexcept
        {
            base_type::operator=(nullptr);
            return *this;
        }

        /** Retrieves a reference to the element at the given index. */
        [[nodiscard]] HD_FORCEINLINE constexpr hud::add_lvalue_reference_t<type_t> operator[](const usize at) const noexcept
        {
            return base_type::pointer()[at];
        }

        /**  Call the deleter on the owned pointer and optionally take ownership of a new pointer. */
        template<typename u_ptr_t>
        requires(details::is_unique_pointer_array_can_be_reset_with_v<unique_pointer, u_ptr_t>)
        constexpr void reset(u_ptr_t ptr) noexcept
        {
            base_type::reset(ptr);
        }

        /** Inherit unique_pointer_impl::reset(hud::ptr::null) and unique_pointer_impl::reset(). */
        using base_type::reset;

    private:
        /** Not copy constructible. */
        unique_pointer(const unique_pointer &) = delete;
        /** Not copyable. */
        unique_pointer &operator=(const unique_pointer &) = delete;
    };

    /**
     * Swap unique_pointer.
     * Same as first.swap(second).
     * @tparam type_t Type of the first unique_pointer's pointer
     * @tparam deleter_type Type of the first unique_pointer's deleter
     * @tparam u_ptr_t Type of the second unique_pointer's pointer
     * @tparam u_deleter_t Type of the second unique_pointer's deleter
     * @param first The first to swap
     * @param second The second to swap
     */
    template<typename type_t, typename deleter_type, typename u_ptr_t, typename u_deleter_t>
    constexpr void swap(unique_pointer<type_t, deleter_type> &first, unique_pointer<u_ptr_t, u_deleter_t> &second) noexcept
    {
        first.swap(second);
    }

    /**
     * Swap unique_pointer.
     * Same as first.swap(second)
     * @tparam type_t Type of both unique_pointer's pointer
     * @tparam deleter_type Type of the both unique_pointer's deleter
     * @param first The first to swap
     * @param second The second to swap
     */
    template<typename type_t, typename deleter_type>
    constexpr void swap(unique_pointer<type_t, deleter_type> &first, unique_pointer<type_t, deleter_type> &second) noexcept
    {
        first.swap(second);
    }

    /**
     * Checks whether two unique_pointer owned the same pointer
     * @tparam type_t Type of the first unique_pointer's pointer
     * @tparam deleter_type Type of the first unique_pointer's deleter
     * @tparam u_ptr_t Type of the second unique_pointer's pointer
     * @tparam u_deleter_t Type of the second unique_pointer's deleter
     * @param first The first to compare
     * @param second The second to compare
     * @param true if both pointer are equals, false otherwise
     */
    template<typename type_t, typename deleter_type, typename u_ptr_t, typename u_deleter_t>
    [[nodiscard]] constexpr bool operator==(const unique_pointer<type_t, deleter_type> &first, const unique_pointer<u_ptr_t, u_deleter_t> &second) noexcept
    {
        return first.pointer() == second.pointer();
    }

    /**
     * Checks whether the unique_pointer don't own pointer.
     * @tparam type_t Type of the unique_pointer's pointer
     * @tparam deleter_type Type of the unique_pointer's deleter
     * @param pointer The pointer to compare with nullptr
     * @param hud::ptr::null
     * @param true if unique_pointer don't own a pointer, false otherwise
     */
    template<typename type_t, typename deleter_type>
    [[nodiscard]] constexpr bool operator==(const unique_pointer<type_t, deleter_type> &pointer, hud::ptr::null) noexcept
    {
        return !pointer;
    }

    /**
     * Checks whether the unique_pointer don't own pointer.
     * @tparam type_t Type of the unique_pointer's pointer
     * @tparam deleter_type Type of the unique_pointer's deleter
     * @param hud::ptr::null
     * @param pointer The pointer to compare with nullptr
     * @param true if unique_pointer don't own a pointer, false otherwise
     */
    template<typename type_t, typename deleter_type>
    [[nodiscard]] constexpr bool operator==(hud::ptr::null, const unique_pointer<type_t, deleter_type> &pointer) noexcept
    {
        return !pointer;
    }

    /**
     * Checks whether two unique_pointer owned the different pointers.
     * @tparam type_t Type of the first unique_pointer's pointer
     * @tparam deleter_type Type of the first unique_pointer's deleter
     * @tparam u_ptr_t Type of the second unique_pointer's pointer
     * @tparam u_deleter_t Type of the second unique_pointer's deleter
     * @param first The first to compare
     * @param second The second to compare
     * @param true if both pointer are not equals, false otherwise
     */
    template<typename type_t, typename deleter_type, typename u_ptr_t, typename u_deleter_t>
    [[nodiscard]] constexpr bool operator!=(const unique_pointer<type_t, deleter_type> &first, const unique_pointer<u_ptr_t, u_deleter_t> &second) noexcept
    {
        return first.pointer() != second.pointer();
    }

    /**
     * Checks whether the unique_pointer own pointer.
     * @tparam type_t Type of the unique_pointer's pointer
     * @tparam deleter_type Type of the unique_pointer's deleter
     * @param pointer The pointer to check
     * @param hud::ptr::null
     * @param true if unique_pointer own a pointer, false otherwise
     */
    template<typename type_t, typename deleter_type>
    [[nodiscard]] constexpr bool operator!=(const unique_pointer<type_t, deleter_type> &pointer, hud::ptr::null) noexcept
    {
        return static_cast<bool>(pointer);
    }

    /**
     * Checks whether the unique_pointer own pointer.
     * @tparam type_t Type of the unique_pointer's pointer
     * @tparam deleter_type Type of the unique_pointer's deleter
     * @param hud::ptr::null
     * @param pointer The pointer to check
     * @param true if unique_pointer own a pointer, false otherwise
     */
    template<typename type_t, typename deleter_type>
    [[nodiscard]] constexpr bool operator!=(hud::ptr::null, const unique_pointer<type_t, deleter_type> &pointer) noexcept
    {
        return static_cast<bool>(pointer);
    }

    /**
     * Checks whether the first owned unique_pointer address less than the second owned pointer address.
     * @tparam type_t Type of the first unique_pointer's pointer
     * @tparam deleter_type Type of the first unique_pointer's deleter
     * @tparam u_ptr_t Type of the second unique_pointer's pointer
     * @tparam u_deleter_t Type of the second unique_pointer's deleter
     * @param first The first to compare
     * @param second The second to compare
     * @return true if first owned pointer address is less than second owned pointer address, false otherwise
     */
    template<typename type_t, typename deleter_type, typename u_ptr_t, typename u_deleter_t>
    [[nodiscard]] bool operator<(const unique_pointer<type_t, deleter_type> &first, const unique_pointer<u_ptr_t, u_deleter_t> &second) noexcept
    {
        using pointer_type = common_type_t<typename unique_pointer<type_t, deleter_type>::pointer_type, typename unique_pointer<u_ptr_t, u_deleter_t>::pointer_type>;
        return less<pointer_type>()(first.pointer(), second.pointer());
    }

    /**
     * Checks whether the owned pointer address is less than nullptr.
     * @tparam type_t Type of the unique_pointer's pointer
     * @tparam deleter_type Type of the unique_pointer's deleter
     * @param pointer The pointer to compare
     * @param nullptr
     * @return true if the owned pointer address is less than nullptr, false otherwise
     */
    template<typename type_t, typename deleter_type>
    [[nodiscard]] bool operator<(const unique_pointer<type_t, deleter_type> &pointer, hud::ptr::null) noexcept
    {
        using pointer_type = typename unique_pointer<type_t, deleter_type>::pointer_type;
        return less<pointer_type>()(pointer.pointer(), nullptr);
    }

    /**
     * Checks whether nullptr is less than the owned pointer address.
     * @tparam type_t Type of the unique_pointer's pointer
     * @tparam deleter_type Type of the unique_pointer's deleter
     * @param nullptr
     * @param pointer The pointer to compare
     * @return true if nullptr is less than the owned pointer address, false otherwise
     */
    template<typename type_t, typename deleter_type>
    [[nodiscard]] bool operator<(hud::ptr::null, const unique_pointer<type_t, deleter_type> &pointer) noexcept
    {
        using pointer_type = typename unique_pointer<type_t, deleter_type>::pointer_type;
        return less<pointer_type>()(nullptr, pointer.pointer());
    }

    /**
     * Checks whether the first owned unique_pointer address greater than the second owned pointer address.
     * @tparam type_t Type of the first unique_pointer's pointer
     * @tparam deleter_type Type of the first unique_pointer's deleter
     * @tparam u_ptr_t Type of the second unique_pointer's pointer
     * @tparam u_deleter_t Type of the second unique_pointer's deleter
     * @param first The first to compare
     * @param second The second to compare
     * @return true if first owned pointer address is greater than second owned pointer address, false otherwise
     */
    template<typename type_t, typename deleter_type, typename u_ptr_t, typename u_deleter_t>
    [[nodiscard]] bool operator>(const unique_pointer<type_t, deleter_type> &first, const unique_pointer<u_ptr_t, u_deleter_t> &second) noexcept
    {
        return second < first;
    }

    /**
     * Checks whether the owned pointer address is greater than nullptr.
     * @tparam type_t Type of the unique_pointer's pointer
     * @tparam deleter_type Type of the unique_pointer's deleter
     * @param pointer The pointer to compare
     * @param nullptr
     * @return true if the owned pointer address is greater than nullptr, false otherwise
     */
    template<typename type_t, typename deleter_type>
    [[nodiscard]] bool operator>(const unique_pointer<type_t, deleter_type> &pointer, hud::ptr::null) noexcept
    {
        return nullptr < pointer;
    }

    /**
     * Checks whether nullptr is greater than the owned pointer address.
     * @tparam type_t Type of the unique_pointer's pointer
     * @tparam deleter_type Type of the unique_pointer's deleter
     * @param nullptr
     * @param pointer The pointer to compare
     * @return true if nullptr is greater than the owned pointer address, false otherwise
     */
    template<typename type_t, typename deleter_type>
    [[nodiscard]] bool operator>(hud::ptr::null, const unique_pointer<type_t, deleter_type> &pointer) noexcept
    {
        return pointer < nullptr;
    }

    /**
     * Checks whether the first owned unique_pointer address less or equal the second owned pointer address.
     * @tparam type_t Type of the first unique_pointer's pointer
     * @tparam deleter_type Type of the first unique_pointer's deleter
     * @tparam u_ptr_t Type of the second unique_pointer's pointer
     * @tparam u_deleter_t Type of the second unique_pointer's deleter
     * @param first The first to compare
     * @param second The second to compare
     * @return true if first owned pointer address is less or equal the second owned pointer address, false otherwise
     */
    template<typename type_t, typename deleter_type, typename u_ptr_t, typename u_deleter_t>
    [[nodiscard]] bool operator<=(const unique_pointer<type_t, deleter_type> &first, const unique_pointer<u_ptr_t, u_deleter_t> &second) noexcept
    {
        return !(second < first);
    }

    /**
     * Checks whether the owned pointer address is less or equal nullptr.
     * @tparam type_t Type of the unique_pointer's pointer
     * @tparam deleter_type Type of the unique_pointer's deleter
     * @param pointer The pointer to compare
     * @param nullptr
     * @return true if the owned pointer address is less or equal nullptr, false otherwise
     */
    template<typename type_t, typename deleter_type>
    [[nodiscard]] bool operator<=(const unique_pointer<type_t, deleter_type> &pointer, hud::ptr::null) noexcept
    {
        return !(nullptr < pointer);
    }

    /**
     * Checks whether nullptr is less or equal the owned pointer address.
     * @tparam type_t Type of the unique_pointer's pointer
     * @tparam deleter_type Type of the unique_pointer's deleter
     * @param nullptr
     * @param pointer The pointer to compare
     * @return true if nullptr is less or equal the owned pointer address, false otherwise
     */
    template<typename type_t, typename deleter_type>
    [[nodiscard]] bool operator<=(hud::ptr::null, const unique_pointer<type_t, deleter_type> &pointer) noexcept
    {
        return !(pointer < nullptr);
    }

    /**
     * Checks whether the first owned unique pointer address compares greater or equal the second owned pointer address.
     * @tparam type_t Type of the first unique_pointer's pointer
     * @tparam deleter_type Type of the first unique_pointer's deleter
     * @tparam u_ptr_t Type of the second unique_pointer's pointer
     * @tparam u_deleter_t Type of the second unique_pointer's deleter
     * @param first The first to compare
     * @param second The second to compare
     * @return true if first owned unique pointer address compares greater or equal the second owned pointer address, false otherwise
     */
    template<typename type_t, typename deleter_type, typename u_ptr_t, typename u_deleter_t>
    [[nodiscard]] bool operator>=(const unique_pointer<type_t, deleter_type> &first, const unique_pointer<u_ptr_t, u_deleter_t> &second) noexcept
    {
        return !(first < second);
    }

    /**
     * Checks whether the owned pointer address is greater or equal nullptr.
     * @tparam type_t Type of the unique_pointer's pointer
     * @tparam deleter_type Type of the unique_pointer's deleter
     * @param pointer The pointer to compare
     * @param nullptr
     * @return True if the owned pointer address is greater or equal nullptr, false otherwise
     */
    template<typename type_t, typename deleter_type>
    [[nodiscard]] bool operator>=(const unique_pointer<type_t, deleter_type> &pointer, hud::ptr::null) noexcept
    {
        return !(pointer < nullptr);
    }

    /**
     * Checks whether nullptr is greater or equal the owned pointer address.
     * @tparam type_t Type of the unique_pointer's pointer
     * @tparam deleter_type Type of the unique_pointer's deleter
     * @param nullptr
     * @param pointer The pointer to compare
     * @return true if nullptr is greater or equal the owned pointer address, false otherwise
     */
    template<typename type_t, typename deleter_type>
    [[nodiscard]] bool operator>=(hud::ptr::null, const unique_pointer<type_t, deleter_type> &pointer) noexcept
    {
        return !(nullptr < pointer);
    }

    /**
     * Constructs a unique_pointer that owns a pointer of type type_t. The arguments are hud::forward to the constructor of type_t.
     * This overload only participates in overload resolution if type_t is not an array type
     * @tparam type_t Type of the unique_pointer's pointer
     * @tparam args_t The type_t constructor arguments
     * @param args Arguments hud::forward to the type_t constructor
     * @return unique_pointer<type_t> pointing to a object of type type_t construct by passing args arguments to its constructor
     */
    template<typename type_t, typename... args_t>
    requires(hud::negation_v<hud::is_array<type_t>>)
    [[nodiscard]] constexpr unique_pointer<type_t> make_unique(args_t &&...args) noexcept
    {
        return unique_pointer<type_t>(new type_t(hud::forward<args_t>(args)...));
    }

    /**
     * Constructs a unique_pointer that owns a pointer to an array of unknown bound type_t.
     * This overload only participates in overload resolution if type_t is an array of unknown bound.
     * @tparam type_t Type of the unique_pointer's pointer
     * @param size Number of type_t to allocate
     * @return unique_pointer<type_t> pointer to an array of type type_t
     */
    template<typename type_t>
    requires(hud::is_unbounded_array_v<type_t>)
    [[nodiscard]] constexpr unique_pointer<type_t> make_unique(const usize size) noexcept
    {
        return unique_pointer<type_t>(new (std::nothrow) hud::remove_extent_t<type_t>[size]());
    }

    /** Construction of arrays of known bound is disallowed. */
    template<typename type_t, typename... args_t>
    requires(hud::is_bounded_array_v<type_t>)
    void make_unique(args_t &&...) = delete;

    /**
     * Hash function for unique_pointer<type_t>
     * @tparam type_t Type of the unique_pointer's pointer
     * @param ptr The unique_pointer<type_t> to hash
     * @return The hash of ptr
     */
    template<typename type_t>
    constexpr u32 hash(const unique_pointer<type_t> &ptr) noexcept
    {
        return hud::hash(ptr.pointer());
    }

} // namespace hud

#endif // HD_INC_CORE_UNIQUE_POINTER_H