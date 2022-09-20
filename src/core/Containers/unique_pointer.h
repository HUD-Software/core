#pragma once
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
#include "../templates/hud::forward.h"
#include "../templates/swap.h"
#include "../templates/less.h"

#include "tuple.h"
#include "../hash.h"


namespace hud {

    template<typename type_t, typename deleter_t = hud::default_deleter<type_t>>
    class unique_pointer;

    namespace details {

        /**
        * deleter_t parameter type.
        * If deleter_t is a reference, it's passed by value, else by const reference
        * @tparam deleter_t  The deleter type of the unique_pointer
        */
        template <typename deleter_t>
        using deleter_param_type = conditional_t<is_reference_v<deleter_t>, deleter_t, add_lvalue_reference_t<add_const_t<deleter_t>>>;

        /**
        * Checks whether a unique_pointer<u_ptr_t, u_deleter_t> is convertible to unique_pointer<type_t, deleter_t>.
        * Provides the member constant Value which is equal to true, if unique_pointer<u_ptr_t, u_deleter_t> is convertible to unique_pointer<type_t, deleter_t>.
        * Otherwise, Value is equal to false.
        * @tparam unique_pointer_t The unique_pointer<type_t>
        * @tparam u_unique_pointer_t The unique_pointer<u_ptr_t>
        */
        template<typename unique_pointer_t, typename u_unique_pointer_t>
        struct is_unique_pointer_convertible_to_unique_pointer
            : hud::false_type {
        };
        template<typename ptr_t, typename deleter_t, typename u_ptr_t, typename u_deleter_t>
        struct is_unique_pointer_convertible_to_unique_pointer<unique_pointer<ptr_t, deleter_t>, unique_pointer<u_ptr_t, u_deleter_t>>
            : conjunction <
            is_convertible<typename unique_pointer<u_ptr_t, u_deleter_t>::pointer_type, typename unique_pointer<ptr_t, deleter_t>::pointer_type >,
            negation<is_array<u_ptr_t>>,
            conditional_t<is_reference_v<deleter_t>, is_same<deleter_t, u_deleter_t>, is_convertible<u_deleter_t, deleter_t>>
            > {
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
            : hud::false_type {
        };
        template<typename ptr_t, typename deleter_t, typename u_ptr_t, typename u_deleter_t>
        struct is_unique_pointer_move_assignable_to_unique_pointer<unique_pointer<ptr_t, deleter_t>, unique_pointer<u_ptr_t, u_deleter_t>>
            : conjunction<
            is_convertible<typename unique_pointer<u_ptr_t, u_deleter_t>::pointer_type, typename unique_pointer<ptr_t, deleter_t>::pointer_type >,
            negation<is_array<u_ptr_t>>,
            is_move_assignable<deleter_t, u_deleter_t>
            > {
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
            : hud::false_type {
        };
        template<typename ptr_t, typename deleter_t, typename u_ptr_t>
        struct is_unique_pointer_array_can_be_reset_with<unique_pointer<ptr_t[], deleter_t>, u_ptr_t*>
            : disjunction <
            is_same<u_ptr_t*, typename unique_pointer<ptr_t[], deleter_t>::pointer_type>,
            conjunction<
            is_same<typename unique_pointer<ptr_t[], deleter_t>::pointer_type, typename unique_pointer<ptr_t[], deleter_t>::element_type*>,
            is_convertible<u_ptr_t(*)[], ptr_t(*)[]>
            >
            > {
        };
        template<typename type_t, typename u_ptr_t>
        static constexpr bool is_unique_pointer_array_can_be_reset_with_v = is_unique_pointer_array_can_be_reset_with<type_t, u_ptr_t>::value;

        /**
        * Checks whether a u_ptr_t* is convertible to unique_pointer<type_t[], deleter_t>.
        * Provides the member constant Value which is equal to true, if u_ptr_t* is convertible to unique_pointer<type_t, deleter_t>.
        * Otherwise, Value is equal to false.
        * @tparam unique_pointer_t The unique_pointer<type_t[]>
        * @tparam u_ptr_t The pointer u_ptr_t* used to reset unique_pointer_t
        */
        template<typename unique_pointer_t, typename u_ptr_t>
        struct is_pointer_convertible_to_unique_pointer_array
            : disjunction< is_unique_pointer_array_can_be_reset_with<unique_pointer_t, u_ptr_t>, is_nullptr<u_ptr_t>> {
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
            : hud::false_type {
        };
        template<typename ptr_t, typename deleter_t, typename u_ptr_t, typename u_deleter_t>
        struct is_unique_pointer_convertible_to_unique_pointer_array<unique_pointer<ptr_t, deleter_t>, unique_pointer<u_ptr_t, u_deleter_t>>
            : conjunction<
            is_array<u_ptr_t>,
            is_same<typename unique_pointer<ptr_t, deleter_t>::element_type*, typename unique_pointer<ptr_t, deleter_t>::pointer_type>,
            is_same<typename unique_pointer<u_ptr_t, u_deleter_t>::element_type*, typename unique_pointer<u_ptr_t, u_deleter_t>::pointer_type>,
            is_convertible<typename unique_pointer<u_ptr_t, u_deleter_t>::element_type(*)[], typename unique_pointer<ptr_t, deleter_t>::element_type(*)[]>
            > {
        };

        /**
        * Checks whether a unique_pointer<type_t[]> is move constructible from unique_pointer<u_ptr_t>
        * @tparam unique_pointer_t The unique_pointer<type_t[]>
        * @tparam u_unique_pointer_t The unique_pointer<u_ptr_t>
        */
        template<typename unique_pointer_t, typename u_unique_pointer_t>
        struct is_unique_pointer_array_move_constructible_from_unique_pointer
            : hud::false_type {
        };
        template<typename ptr_t, typename deleter_t, typename u_ptr_t, typename u_deleter_t>
        struct is_unique_pointer_array_move_constructible_from_unique_pointer<unique_pointer<ptr_t, deleter_t>, unique_pointer<u_ptr_t, u_deleter_t>>
            : conjunction <
            is_unique_pointer_convertible_to_unique_pointer_array<unique_pointer<ptr_t, deleter_t>, unique_pointer<u_ptr_t, u_deleter_t>>,
            conditional_t<is_reference_v<deleter_t>, is_same<deleter_t, u_deleter_t>, is_convertible<u_deleter_t, deleter_t>>
            > {
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
            : hud::false_type {
        };
        template<typename ptr_t, typename deleter_t, typename u_ptr_t, typename u_deleter_t>
        struct is_unique_pointer_move_assignable_to_unique_pointer_array<unique_pointer<ptr_t, deleter_t>, unique_pointer<u_ptr_t, u_deleter_t>>
            : conjunction<
            is_unique_pointer_convertible_to_unique_pointer_array<unique_pointer<ptr_t, deleter_t>, unique_pointer<u_ptr_t, u_deleter_t>>,
            is_move_assignable<deleter_t, u_deleter_t>
            > {
        };
        template<typename unique_pointer_t, typename u_unique_pointer_t>
        static constexpr bool is_unique_pointer_move_assignable_to_unique_pointer_array_v = is_unique_pointer_move_assignable_to_unique_pointer_array<unique_pointer_t, u_unique_pointer_t>::value;

    }

    /**
    * unique_pointer is a smart pointer that owns and manages another object through a pointer.
    * unique_pointer object automatically delete the object they manage (using a deleter) as soon as they themselves are destroyed, or as soon as their value changes either by an assignment operation or by an explicit call to unique_pointer::reset.
    * @tparam type_t The type of the pointer to own
    * @tparam deleter_t the deleter used to delete the type_t pointer
    */
    template<typename type_t, typename deleter_t>
    class unique_pointer {

    public:

        /** The deleter type. */
        using deleter_t = deleter_t;
        /** The pointer type. Result of select_deleter_pointer_type_t<type_t,deleter_t>*/
        using pointer_type = select_deleter_pointer_type_t<type_t, deleter_t>;
        /** The element pointed by the pointer. */
        using element_type = type_t;

        static_assert(!hud::is_rvalue_reference_v<deleter_t>, "the specified deleter type cannot be an rvalue reference");
        static_assert(hud::is_convertible_v<type_t*, pointer_type>, "type_t is no convertible to deleter_t::pointer_type");

    public:

        /**
        * Default constructor that value-initializes the stored pointer and the stored deleter.
        * unique_pointer do not accept throwable default constructible deleter.
        */
        HD_FORCEINLINE constexpr unique_pointer() noexcept requires(hud::is_default_constructible_v<deleter_t> && !is_pointer_v<deleter_t>)
            : inner(taginit) {
            static_assert(is_nothrow_default_constructible_v<deleter_t>, "unique_pointer do not accept throwable default constructible deleter_t");
        }

        /**
        * Construct a unique_pointer form a raw pointer to own.
        * unique_pointer do not accept throwable default constructible deleter
        * @param pointer The poiner to own
        */
        HD_FORCEINLINE constexpr explicit unique_pointer(pointer_type pointer) noexcept requires(hud::is_default_constructible_v<deleter_t> && !is_pointer_v<deleter_t>)
            : inner(pointer, taginit) {
            static_assert(is_nothrow_default_constructible_v<deleter_t>, "unique_pointer do not accept throwable default constructible deleter_t");
        }

        /**
        * Construct a unique_pointer from a pointer to own and a deleter.
        * @param pointer The poiner to own
        * @param deleter The deleter used to delete the owned pointer
        */
        HD_FORCEINLINE constexpr unique_pointer(pointer_type pointer, const deleter_t& deleter) noexcept requires(is_constructible_v<deleter_t, const deleter_t& >)
            : inner(pointer, deleter) {
            static_assert(is_nothrow_copy_constructible_v<deleter_t>, "unique_pointer do not accept throwable copy constructible deleter_t");
        }

        /**
        * Construct a unique_pointer from a pointer to own and a deleter.
        * @param pointer The poiner to own
        * @param deleter The deleter used to delete the owned pointer
        */
        HD_FORCEINLINE constexpr unique_pointer(pointer_type pointer, deleter_t&& deleter) noexcept requires(!is_reference_v<deleter_t>&& is_move_constructible_v<deleter_t>)
            : inner(pointer, hud::move(deleter)) {
            static_assert(is_nothrow_move_constructible_v<deleter_t>, "unique_pointer do not accept throwable move constructible deleter_t");
        }

        /**
        * Construct a unique_pointer from a raw pointer to own and a reference to a deleter.
        * @param pointer The poiner to own
        * @param deleter A reference to a deleter
        */
        unique_pointer(pointer_type pointer, remove_reference_t<deleter_t>&& deleter) noexcept requires(is_reference_v<deleter_t>&& is_constructible_v<deleter_t, remove_reference_t<deleter_t>>) = delete;


        /** Construct a unique_pointer from a nullptr */
        HD_FORCEINLINE constexpr unique_pointer(hud::ptr::null) noexcept requires(hud::is_default_constructible_v<deleter_t> && !is_pointer_v<deleter_t>)
            : inner(taginit) {
            static_assert(is_nothrow_default_constructible_v<deleter_t>, "unique_pointer do not accept throwable default constructible deleter_t");
        }

        /**
        * Construct a unique_pointer by stealing another unique_pointer ownership.
        * @param other The unique_pointer to transfert ownership from.
        */
        HD_FORCEINLINE constexpr unique_pointer(unique_pointer&& other) noexcept requires(is_move_constructible_v<deleter_t>)
            : inner(other.leak(), hud::forward<deleter_t>(other.deleter())) {
            static_assert(is_reference_v<deleter_t> ? is_nothrow_move_constructible_v<deleter_t> : true, "unique_pointer do not accept throwable move constructible deleter_t");
        }

        /**
        * Construct a unique_pointer by stealing another unique_pointer ownership.
        * @tparam u_ptr_t Type of the unique_pointer's pointer
        * @tparam u_deleter_t  Type of the unique_pointer's deleter
        * @param other The unique_pointer to transfert ownership from.
        */
        template<typename u_ptr_t, typename u_deleter_t>
        HD_FORCEINLINE constexpr unique_pointer(unique_pointer<u_ptr_t, u_deleter_t>&& other) noexcept requires(details::is_unique_pointer_convertible_to_unique_pointer_v<unique_pointer, unique_pointer<u_ptr_t, u_deleter_t>>)
            : inner(other.leak(), hud::forward<u_deleter_t>(other.deleter())) {
            static_assert(is_nothrow_move_constructible_v<deleter_t, u_deleter_t>, "unique_pointer do not accept throwable move constructible deleter_t");
        }

        /**
        * Assigns the unique_pointer by stealing the ownership of the pointer.
        * @param other The unique_pointer to transfert ownership from.
        * @return *this
        */
        constexpr unique_pointer& operator=(unique_pointer&& other) noexcept requires(hud::is_move_assignable_v<deleter_t>) {
            static_assert(!is_reference_v<deleter_t> ? is_nothrow_move_assignable_v<deleter_t> : true, "unique_pointer do not accept throwable move assignable deleter_t if deleter_t is no a reference");
            static_assert(is_reference_v<deleter_t> ? IsNothrowCopyAssignableV<remove_reference_t<deleter_t>> : true, "unique_pointer do not accept throwable copy assignable deleter_t if deleter_t is reference)");

            reset(other.leak());
            deleter() = hud::forward<deleter_t>(other.deleter());
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
        constexpr unique_pointer& operator=(unique_pointer<u_ptr_t, u_deleter_t>&& other) noexcept requires(details::is_unique_pointer_move_assignable_to_unique_pointer_v<unique_pointer, unique_pointer<u_ptr_t, u_deleter_t>>) {
            static_assert(!is_reference_v<deleter_t> ? is_nothrow_move_assignable_v<deleter_t> : true, "unique_pointer do not accept throwable move assignable deleter_t if deleter_t is no a reference");
            static_assert(is_reference_v<deleter_t> ? IsNothrowCopyAssignableV<remove_reference_t<deleter_t>> : true, "unique_pointer do not accept throwable copy assignable deleter_t if deleter_t is reference)");

            reset(other.leak());
            deleter() = hud::forward<u_deleter_t>(other.deleter());
            return *this;
        }

        /**
        * Assigns the unique_pointer with a nullptr. Same as reset().
        * @param A nullptr
        * @return *this
        */
        HD_FORCEINLINE constexpr unique_pointer& operator=(hud::ptr::null) noexcept {
            reset();
            return *this;
        }

        /** Call the deleter on the pointer if any owned. */
        constexpr ~unique_pointer() noexcept {
            if (is_owning()) {
                deleter()(pointer());
            }
        }

        /** Retrieves the owned pointer. */
        [[nodiscard]]
        HD_FORCEINLINE constexpr pointer_type pointer() const noexcept {
            return get<0>(inner);
        }

        /** Check whether the unique_pointer own a pointer. */
        [[nodiscard]]
        HD_FORCEINLINE constexpr bool is_owning() const noexcept {
            return pointer() != pointer_type();
        }

        /** Check whether the unique_pointer own a pointer. */
        [[nodiscard]]
        HD_FORCEINLINE constexpr explicit operator bool() const noexcept {
            return is_owning();
        }

        /** Dereference owned pointer. */
        [[nodiscard]]
        HD_FORCEINLINE constexpr add_lvalue_reference_t<element_type> operator*() const noexcept {
            return *pointer();
        }

        /** Retrieves the owned pointer. */
        [[nodiscard]]
        HD_FORCEINLINE constexpr pointer_type operator->() const noexcept {
            return pointer();
        }

        /** Retrieves the deleter. */
        [[nodiscard]]
        HD_FORCEINLINE constexpr deleter_t& deleter() noexcept {
            return get<1>(inner);
        }

        /** Retrieves a reference to the deleter. */
        [[nodiscard]]
        HD_FORCEINLINE constexpr const deleter_t& deleter() const noexcept {
            return get<1>(inner);
        }

        /** Release the ownership of the pointer and return it without calling the deleter on the owned pointer. */
        [[nodiscard]]
        constexpr pointer_type leak() noexcept {
            pointer_type old = pointer();
            get<0>(inner) = pointer_type();
            return old;
        }

        /** Call the deleter on the owned pointer and optionally take ownership of a new pointer. */
        constexpr void reset(pointer_type ptr) noexcept {
            pointer_type old_ptr = pointer();
            get<0>(inner) = ptr;
            deleter()(old_ptr);
        }

        /** Call the deleter on the owned pointer and taking no ownership. */
        HD_FORCEINLINE constexpr void reset(hud::ptr::null) noexcept {
            deleter()(pointer());
            get<0>(inner) = pointer_type();
        }

        /** Call the deleter on the owned pointer and taking no ownership. */
        HD_FORCEINLINE constexpr void reset() noexcept {
            reset(nullptr);
        }

        /** Swap with another unique_pointer. */
        HD_FORCEINLINE constexpr void swap(unique_pointer& other) noexcept {
            hud::swap(inner, other.inner);
        }

    private:
        /** Not copy constructible. */
        unique_pointer(const unique_pointer&) = delete;
        /** Not copyable. */
        unique_pointer& operator = (const unique_pointer&) = delete;

    private:
        /** hud::tuple containing the pointerand the deleter */
        hud::tuple< pointer_type, deleter_t> inner;
    };

    /**
    * unique_pointer is a smart pointer that owns and manages another object through a pointer.
    * unique_pointer object automatically delete the object they manage (using a deleter) as soon as they themselves are destroyed, or as soon as their value changes either by an assignment operation or by an explicit call to unique_pointer::reset.
    */
    template<typename type_t, typename deleter_t>
    class unique_pointer<type_t[], deleter_t> {

    public:
        /** The deleter type. */
        using deleter_t = deleter_t;
        /** The pointer type. Result of select_deleter_pointer_type_t<type_t,deleter_t>*/
        using pointer_type = select_deleter_pointer_type_t<type_t, deleter_t>;
        /** The element pointed by the pointer. */
        using element_type = type_t;

        static_assert(!hud::is_rvalue_reference_v<deleter_t>, "the specified deleter type cannot be an rvalue reference");
        static_assert(hud::is_convertible_v<type_t*, pointer_type>, "type_t is no convertible to deleter_t::pointer_type");

    public:

        /**
        * Default constructor that value-initializes the stored pointer and the stored deleter.
        * unique_pointer do not accept throwable default constructible deleter.
        */
        HD_FORCEINLINE constexpr unique_pointer() noexcept requires(hud::is_default_constructible_v<deleter_t> && !is_pointer_v<deleter_t>)
            : inner(taginit) {
            static_assert(is_nothrow_default_constructible_v<deleter_t>, "unique_pointer do not accept throwable default constructible deleter_t");
        }

        /**
        * Construct a unique_pointer form a raw pointer to own.
        * unique_pointer do not accept throwable default constructible deleter
        * @param pointer The poiner to own
        */
        template<typename u_ptr_t>
        HD_FORCEINLINE constexpr explicit unique_pointer(u_ptr_t pointer) noexcept requires(hud::is_default_constructible_v<deleter_t> && !is_pointer_v<deleter_t> && details::is_pointer_convertible_to_unique_pointer_array_v<unique_pointer, u_ptr_t>)
            : inner(pointer, taginit) {
            static_assert(is_nothrow_default_constructible_v<deleter_t>, "unique_pointer do not accept throwable default constructible deleter_t");
        }

        /**
        * Construct a unique_pointer from a pointer to own and a deleter.
        * @param pointer The poiner to own
        * @param deleter The deleter used to delete the owned pointer
        */
        template<typename u_ptr_t>
        HD_FORCEINLINE constexpr unique_pointer(u_ptr_t pointer, const deleter_t& deleter) noexcept requires(is_constructible_v<deleter_t, const deleter_t& > && details::is_pointer_convertible_to_unique_pointer_array_v<unique_pointer, u_ptr_t>)
            : inner(pointer, deleter) {
            static_assert(is_nothrow_copy_constructible_v<deleter_t>, "unique_pointer do not accept throwable copy constructible deleter_t");
        }

        /**
        * Construct a unique_pointer from a pointer to own and a deleter.
        * @param pointer The poiner to own
        * @param deleter The deleter used to delete the owned pointer
        */
        template<typename u_ptr_t>
        HD_FORCEINLINE constexpr unique_pointer(u_ptr_t pointer, deleter_t&& deleter) noexcept requires(!is_reference_v<deleter_t> && is_move_constructible_v<deleter_t>&& details::is_pointer_convertible_to_unique_pointer_array_v<unique_pointer, u_ptr_t>)
            : inner(pointer, hud::move(deleter)) {
            static_assert(is_nothrow_move_constructible_v<deleter_t>, "unique_pointer do not accept throwable move constructible deleter_t");
        }

        /** Do not accept deleter_t is a reference will allow the move of it. */
        unique_pointer(pointer_type pointer, remove_reference_t<deleter_t>&& deleter) noexcept requires(is_reference_v<deleter_t>&& is_constructible_v<deleter_t, remove_reference_t<deleter_t>>) = delete;


        /** Construct a unique_pointer from a nullptr. */ 
        HD_FORCEINLINE constexpr unique_pointer(hud::ptr::null) noexcept requires(hud::is_default_constructible_v<deleter_t> && !is_pointer_v<deleter_t>)
            : inner(taginit) {
            static_assert(is_nothrow_default_constructible_v<deleter_t>, "unique_pointer do not accept throwable default constructible deleter_t");
        }

        /**
        * Construct a unique_pointer by stealing another unique_pointer ownership.
        * @param other The unique_pointer to transfert ownership from.
        */
        HD_FORCEINLINE constexpr unique_pointer(unique_pointer&& other) noexcept requires(is_move_constructible_v<deleter_t>)
            : inner(other.leak(), hud::forward<deleter_t>(other.deleter())) {
            static_assert(is_reference_v<deleter_t> ? is_nothrow_move_constructible_v<deleter_t> : true, "unique_pointer do not accept throwable move constructible deleter_t");
        }

        /**
        * Construct a unique_pointer by stealing another unique_pointer ownership.
        * @tparam u_ptr_t Type of the unique_pointer's pointer
        * @tparam u_deleter_t  Type of the unique_pointer's deleter
        * @param other The unique_pointer to transfert ownership from.
        */
        template<typename u_ptr_t, typename u_deleter_t>
        HD_FORCEINLINE constexpr unique_pointer(unique_pointer<u_ptr_t, u_deleter_t>&& other) noexcept requires(details::is_unique_pointer_array_move_constructible_from_unique_pointer_v<unique_pointer, unique_pointer<u_ptr_t, u_deleter_t>>)
            : inner(other.leak(), hud::forward<u_deleter_t>(other.deleter())) {
            static_assert(is_nothrow_move_constructible_v<deleter_t, u_deleter_t>, "unique_pointer do not accept throwable move constructible deleter_t");
        }

        /**
        * Assigns the unique_pointer by stealing the ownership of the pointer.
        * @param other The unique_pointer to transfert ownership from.
        * @return *this
        */
        constexpr unique_pointer& operator=(unique_pointer&& other) noexcept requires(hud::is_move_assignable_v<deleter_t>) {
            static_assert(!is_reference_v<deleter_t> ? is_nothrow_move_assignable_v<deleter_t> : true, "unique_pointer do not accept throwable move assignable deleter_t if deleter_t is no a reference");
            static_assert(is_reference_v<deleter_t> ? IsNothrowCopyAssignableV<remove_reference_t<deleter_t>> : true, "unique_pointer do not accept throwable copy assignable deleter_t if deleter_t is reference)");

            reset(other.leak());
            deleter() = hud::forward<deleter_t>(other.deleter());
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
        constexpr unique_pointer& operator=(unique_pointer<u_ptr_t, u_deleter_t>&& other) noexcept requires(details::is_unique_pointer_move_assignable_to_unique_pointer_array_v<unique_pointer, unique_pointer<u_ptr_t, u_deleter_t>>) {
            static_assert(!is_reference_v<deleter_t> ? is_nothrow_move_assignable_v<deleter_t> : true, "unique_pointer do not accept throwable move assignable deleter_t if deleter_t is no a reference");
            static_assert(is_reference_v<deleter_t> ? IsNothrowCopyAssignableV<remove_reference_t<deleter_t>> : true, "unique_pointer do not accept throwable copy assignable deleter_t if deleter_t is reference)");

            reset(other.leak());
            deleter() = hud::forward<u_deleter_t>(other.deleter());
            return *this;
        }

        /**
        * Assigns the unique_pointer with a nullptr. Same as reset().
        * @param A nullptr
        * @return *this
        */
        HD_FORCEINLINE constexpr unique_pointer& operator=(hud::ptr::null) noexcept {
            reset();
            return *this;
        }

        /** Call the deleter on the pointer if any owned. */
        constexpr ~unique_pointer() noexcept {
            if (is_owning()) {
                deleter()(pointer());
            }
        }

        /** Retrieves the owned pointer. */
        [[nodiscard]]
        HD_FORCEINLINE constexpr pointer_type pointer() const noexcept {
            return get<0>(inner);
        }

        /** Retrieves a reference to the element at the given index. */
        [[nodiscard]]
        HD_FORCEINLINE constexpr add_lvalue_reference_t<type_t> operator[](const usize at) const noexcept {
            return pointer()[at];
        }

        /** Check whether the unique_pointer own a pointer. */
        [[nodiscard]]
        HD_FORCEINLINE constexpr bool is_owning() const noexcept {
            return pointer() != pointer_type();
        }

        /** Check whether the unique_pointer own a pointer. */
        [[nodiscard]]
        HD_FORCEINLINE constexpr explicit operator bool() const noexcept {
            return is_owning();
        }

        /** Retrieves the deleter. */
        [[nodiscard]]
        HD_FORCEINLINE constexpr deleter_t& deleter() noexcept {
            return get<1>(inner);
        }

        /** Retrieves the const deleter. */
        [[nodiscard]]
        HD_FORCEINLINE constexpr const deleter_t& deleter() const noexcept {
            return get<1>(inner);
        }

        /** Release the ownership of the pointer and return it without calling the deleter on the owned pointer. */
        [[nodiscard]]
        constexpr pointer_type leak() noexcept {
            pointer_type old = pointer();
            get<0>(inner) = pointer_type();
            return old;
        }

        /**  Call the deleter on the owned pointer and optionally take ownership of a new pointer. */
        template<typename u_ptr_t>
        constexpr void reset(u_ptr_t ptr) noexcept requires(details::is_unique_pointer_array_can_be_reset_with_v<unique_pointer, u_ptr_t>) {
            pointer_type old_ptr = pointer();
            get<0>(inner) = ptr;
            deleter()(old_ptr);
        }

        /** Call the deleter on the owned pointer and taking ownership of nullptr. */
        HD_FORCEINLINE constexpr void reset(hud::ptr::null) noexcept {
            deleter()(pointer());
            get<0>(inner) = pointer_type();
        }

        /** Call the deleter on the owned pointer and taking no ownership. */
        HD_FORCEINLINE constexpr void reset() noexcept {
            reset(nullptr);
        }

        /** Swap with another unique_pointer. */
        HD_FORCEINLINE constexpr void swap(unique_pointer& other) noexcept {
            hud::swap(inner, other.inner);
        }

    private:
        /** Not copy constructible. */
        unique_pointer(const unique_pointer&) = delete;
        /** Not copyable. */
        unique_pointer& operator = (const unique_pointer&) = delete;

    private:
        /** hud::tuple containing the pointer and the deleter */
        hud::tuple< pointer_type, deleter_t> inner;
    };

    /**
    * Swap unique_pointer.
    * Same as first.swap(second).
    * @tparam type_t Type of the first unique_pointer's pointer
    * @tparam deleter_t Type of the first unique_pointer's deleter
    * @tparam u_ptr_t Type of the second unique_pointer's pointer
    * @tparam u_deleter_t Type of the second unique_pointer's deleter
    * @param first The first to swap
    * @param second The second to swap
    */
    template<typename type_t, typename deleter_t, typename u_ptr_t, typename u_deleter_t>
    constexpr void swap(unique_pointer<type_t, deleter_t>& first, unique_pointer<u_ptr_t, u_deleter_t>& second) noexcept {
        first.swap(second);
    }

    /**
    * Swap unique_pointer.
    * Same as first.swap(second)
    * @tparam type_t Type of both unique_pointer's pointer
    * @tparam deleter_t Type of the both unique_pointer's deleter
    * @param first The first to swap
    * @param second The second to swap
    */
    template<typename type_t, typename deleter_t>
    constexpr void swap(unique_pointer<type_t, deleter_t>& first, unique_pointer<type_t, deleter_t>& second) noexcept {
        first.swap(second);
    }

    /**
    * Checks whether two unique_pointer owned the same pointer
    * @tparam type_t Type of the first unique_pointer's pointer
    * @tparam deleter_t Type of the first unique_pointer's deleter
    * @tparam u_ptr_t Type of the second unique_pointer's pointer
    * @tparam u_deleter_t Type of the second unique_pointer's deleter
    * @param first The first to compare
    * @param second The second to compare
    * @param true if both pointer are equals, false otherwise
    */
    template<typename type_t, typename deleter_t, typename u_ptr_t, typename u_deleter_t>
    [[nodiscard]]
    constexpr bool operator==(const unique_pointer<type_t, deleter_t>& first, const unique_pointer<u_ptr_t, u_deleter_t>& second) noexcept {
        return first.pointer() == second.pointer();
    }

    /**
    * Checks whether the unique_pointer don't own pointer.
    * @tparam type_t Type of the unique_pointer's pointer
    * @tparam deleter_t Type of the unique_pointer's deleter
    * @param pointer The pointer to compare with nullptr
    * @param hud::ptr::null
    * @param true if unique_pointer don't own a pointer, false otherwise
    */
    template<typename type_t, typename deleter_t>
    [[nodiscard]]
    constexpr bool operator==(const unique_pointer<type_t, deleter_t>& pointer, hud::ptr::null) noexcept {
        return !pointer;
    }

    /**
    * Checks whether the unique_pointer don't own pointer.
    * @tparam type_t Type of the unique_pointer's pointer
    * @tparam deleter_t Type of the unique_pointer's deleter
    * @param hud::ptr::null
    * @param pointer The pointer to compare with nullptr
    * @param true if unique_pointer don't own a pointer, false otherwise
    */
    template<typename type_t, typename deleter_t>
    [[nodiscard]]
    constexpr bool operator==(hud::ptr::null, const unique_pointer<type_t, deleter_t>& pointer) noexcept {
        return !pointer;
    }

    /**
    * Checks whether two unique_pointer owned the different pointers.
    * @tparam type_t Type of the first unique_pointer's pointer
    * @tparam deleter_t Type of the first unique_pointer's deleter
    * @tparam u_ptr_t Type of the second unique_pointer's pointer
    * @tparam u_deleter_t Type of the second unique_pointer's deleter
    * @param first The first to compare
    * @param second The second to compare
    * @param true if both pointer are not equals, false otherwise
    */
    template<typename type_t, typename deleter_t, typename u_ptr_t, typename u_deleter_t>
    [[nodiscard]]
    constexpr bool operator!=(const unique_pointer<type_t, deleter_t>& first, const unique_pointer<u_ptr_t, u_deleter_t>& second) noexcept {
        return first.pointer() != second.pointer();
    }

    /**
    * Checks whether the unique_pointer own pointer.
    * @tparam type_t Type of the unique_pointer's pointer
    * @tparam deleter_t Type of the unique_pointer's deleter
    * @param pointer The pointer to check
    * @param hud::ptr::null
    * @param true if unique_pointer own a pointer, false otherwise
    */
    template<typename type_t, typename deleter_t>
    [[nodiscard]]
    constexpr bool operator!=(const unique_pointer<type_t, deleter_t>& pointer, hud::ptr::null) noexcept {
        return static_cast<bool>(pointer);
    }

    /**
    * Checks whether the unique_pointer own pointer.
    * @tparam type_t Type of the unique_pointer's pointer
    * @tparam deleter_t Type of the unique_pointer's deleter
    * @param hud::ptr::null
    * @param pointer The pointer to check
    * @param true if unique_pointer own a pointer, false otherwise
    */
    template<typename type_t, typename deleter_t>
    [[nodiscard]]
    constexpr bool operator!=(hud::ptr::null, const unique_pointer<type_t, deleter_t>& pointer) noexcept {
        return static_cast<bool>(pointer);
    }

    /**
    * Checks whether the first owned unique_pointer address less than the second owned pointer address.
    * @tparam type_t Type of the first unique_pointer's pointer
    * @tparam deleter_t Type of the first unique_pointer's deleter
    * @tparam u_ptr_t Type of the second unique_pointer's pointer
    * @tparam u_deleter_t Type of the second unique_pointer's deleter
    * @param first The first to compare
    * @param second The second to compare
    * @return true if first owned pointer address is less than second owned pointer address, false otherwise
    */
    template<typename type_t, typename deleter_t, typename u_ptr_t, typename u_deleter_t>
    [[nodiscard]]
    bool operator<(const unique_pointer<type_t, deleter_t>& first, const unique_pointer<u_ptr_t, u_deleter_t>& second) noexcept {
        using pointer_type = common_type_t<typename unique_pointer<type_t, deleter_t>::pointer_type, typename unique_pointer<u_ptr_t, u_deleter_t>::pointer_type>;
        return less<pointer_type>()(first.pointer(), second.pointer());
    }

    /**
    * Checks whether the owned pointer address is less than nullptr.
    * @tparam type_t Type of the unique_pointer's pointer
    * @tparam deleter_t Type of the unique_pointer's deleter
    * @param pointer The pointer to compare
    * @param nullptr
    * @return true if the owned pointer address is less than nullptr, false otherwise
    */
    template<typename type_t, typename deleter_t>
    [[nodiscard]]
    constexpr bool operator<(const unique_pointer<type_t, deleter_t>& pointer, hud::ptr::null) noexcept {
        using pointer_type = typename unique_pointer<type_t, deleter_t>::pointer_type;
        return less<pointer_type>()(pointer.pointer(), nullptr);
    }

    /**
    * Checks whether nullptr is less than the owned pointer address.
    * @tparam type_t Type of the unique_pointer's pointer
    * @tparam deleter_t Type of the unique_pointer's deleter
    * @param nullptr
    * @param pointer The pointer to compare
    * @return true if nullptr is less than the owned pointer address, false otherwise
    */
    template<typename type_t, typename deleter_t>
    [[nodiscard]]
    constexpr bool operator<(hud::ptr::null, const unique_pointer<type_t, deleter_t>& pointer) noexcept {
        using pointer_type = typename unique_pointer<type_t, deleter_t>::pointer_type;
        return less<pointer_type>()(nullptr, pointer.pointer());
    }

    /**
    * Checks whether the first owned unique_pointer address greater than the second owned pointer address.
    * @tparam type_t Type of the first unique_pointer's pointer
    * @tparam deleter_t Type of the first unique_pointer's deleter
    * @tparam u_ptr_t Type of the second unique_pointer's pointer
    * @tparam u_deleter_t Type of the second unique_pointer's deleter
    * @param first The first to compare
    * @param second The second to compare
    * @return true if first owned pointer address is greater than second owned pointer address, false otherwise
    */
    template<typename type_t, typename deleter_t, typename u_ptr_t, typename u_deleter_t>
    [[nodiscard]]
    bool operator>(const unique_pointer<type_t, deleter_t>& first, const unique_pointer<u_ptr_t, u_deleter_t>& second) noexcept {
        return second < first;
    }

    /**
    * Checks whether the owned pointer address is greater than nullptr.
    * @tparam type_t Type of the unique_pointer's pointer
    * @tparam deleter_t Type of the unique_pointer's deleter
    * @param pointer The pointer to compare
    * @param nullptr
    * @return true if the owned pointer address is greater than nullptr, false otherwise
    */
    template<typename type_t, typename deleter_t>
    [[nodiscard]]
    constexpr bool operator>(const unique_pointer<type_t, deleter_t>& pointer, hud::ptr::null) noexcept {
        return nullptr < pointer;
    }

    /**
    * Checks whether nullptr is greater than the owned pointer address.
    * @tparam type_t Type of the unique_pointer's pointer
    * @tparam deleter_t Type of the unique_pointer's deleter
    * @param nullptr
    * @param pointer The pointer to compare
    * @return true if nullptr is greater than the owned pointer address, false otherwise
    */
    template<typename type_t, typename deleter_t>
    [[nodiscard]]
    constexpr bool operator>(hud::ptr::null, const unique_pointer<type_t, deleter_t>& pointer) noexcept {
        return pointer < nullptr;
    }

    /**
    * Checks whether the first owned unique_pointer address less or equal the second owned pointer address.
    * @tparam type_t Type of the first unique_pointer's pointer
    * @tparam deleter_t Type of the first unique_pointer's deleter
    * @tparam u_ptr_t Type of the second unique_pointer's pointer
    * @tparam u_deleter_t Type of the second unique_pointer's deleter
    * @param first The first to compare
    * @param second The second to compare
    * @return true if first owned pointer address is less or equal the second owned pointer address, false otherwise
    */
    template<typename type_t, typename deleter_t, typename u_ptr_t, typename u_deleter_t>
    [[nodiscard]]
    bool operator<=(const unique_pointer<type_t, deleter_t>& first, const unique_pointer<u_ptr_t, u_deleter_t>& second) noexcept {
        return !(second < first);
    }

    /**
    * Checks whether the owned pointer address is less or equal nullptr.
    * @tparam type_t Type of the unique_pointer's pointer
    * @tparam deleter_t Type of the unique_pointer's deleter
    * @param pointer The pointer to compare
    * @param nullptr
    * @return true if the owned pointer address is less or equal nullptr, false otherwise
    */
    template<typename type_t, typename deleter_t>
    [[nodiscard]]
    constexpr bool operator<=(const unique_pointer<type_t, deleter_t>& pointer, hud::ptr::null) noexcept {
        return !(nullptr < pointer);
    }

    /**
    * Checks whether nullptr is less or equal the owned pointer address.
    * @tparam type_t Type of the unique_pointer's pointer
    * @tparam deleter_t Type of the unique_pointer's deleter
    * @param nullptr
    * @param pointer The pointer to compare
    * @return true if nullptr is less or equal the owned pointer address, false otherwise
    */
    template<typename type_t, typename deleter_t>
    [[nodiscard]]
    constexpr bool operator<=(hud::ptr::null, const unique_pointer<type_t, deleter_t>& pointer) noexcept {
        return !(pointer < nullptr);
    }

    /**
    * Checks whether the first owned unique pointer address compares greater or equal the second owned pointer address.
    * @tparam type_t Type of the first unique_pointer's pointer
    * @tparam deleter_t Type of the first unique_pointer's deleter
    * @tparam u_ptr_t Type of the second unique_pointer's pointer
    * @tparam u_deleter_t Type of the second unique_pointer's deleter
    * @param first The first to compare
    * @param second The second to compare
    * @return true if first owned unique pointer address compares greater or equal the second owned pointer address, false otherwise
    */
    template<typename type_t, typename deleter_t, typename u_ptr_t, typename u_deleter_t>
    [[nodiscard]]
    bool operator>=(const unique_pointer<type_t, deleter_t>& first, const unique_pointer<u_ptr_t, u_deleter_t>& second) noexcept {
        return !(first < second);
    }

    /**
    * Checks whether the owned pointer address is greater or equal nullptr.
    * @tparam type_t Type of the unique_pointer's pointer
    * @tparam deleter_t Type of the unique_pointer's deleter
    * @param pointer The pointer to compare
    * @param nullptr
    * @return True if the owned pointer address is greater or equal nullptr, false otherwise
    */
    template<typename type_t, typename deleter_t>
    [[nodiscard]]
    constexpr bool operator>=(const unique_pointer<type_t, deleter_t>& pointer, hud::ptr::null) noexcept {
        return !(pointer < nullptr);
    }

    /**
    * Checks whether nullptr is greater or equal the owned pointer address.
    * @tparam type_t Type of the unique_pointer's pointer
    * @tparam deleter_t Type of the unique_pointer's deleter
    * @param nullptr
    * @param pointer The pointer to compare
    * @return true if nullptr is greater or equal the owned pointer address, false otherwise
    */
    template<typename type_t, typename deleter_t>
    [[nodiscard]]
    constexpr bool operator>=(hud::ptr::null, const unique_pointer<type_t, deleter_t>& pointer) noexcept {
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
    template<typename type_t, typename... args_t, enable_if_t<negation_v<is_array<type_t>>, i32> = 0 >
    [[nodiscard]]
    constexpr unique_pointer<type_t> make_unique(args_t&&... args) noexcept {
        return unique_pointer<type_t>(new (std::nothrow) type_t(hud::forward<args_t>(args)...));
    }

    /**
    * Constructs a unique_pointer that owns a pointer to an array of unknown bound type_t.
    * This overload only participates in overload resolution if type_t is an array of unknown bound.
    * @tparam type_t Type of the unique_pointer's pointer
    * @param size Number of type_t to allocate
    * @return unique_pointer<type_t> pointer to an array of type type_t
    */
    template<typename type_t, enable_if_t<is_unbounded_array_v<type_t>, i32> = 0>
    [[nodiscard]]
    unique_pointer<type_t> make_unique(const usize size) noexcept {
        return unique_pointer<type_t>(new (std::nothrow) remove_extent_t<type_t>[size]());
    }

    /** Construction of arrays of known bound is disallowed. */
    template<typename type_t, typename... args_t, enable_if_t<is_bounded_array_v<type_t>> = 0>
    void make_unique(args_t&&...) = delete;

    /**
    * Hash function for unique_pointer<type_t>
    * @tparam type_t Type of the unique_pointer's pointer
    * @param ptr The unique_pointer<type_t> to hash
    * @return The hash of ptr
    */
    template<typename type_t>
    constexpr u32 hash(const unique_pointer<type_t>& ptr) noexcept {
        return hud::hash(ptr.pointer());
    }

} // namespace hud

#endif // HD_INC_CORE_UNIQUE_POINTER_H