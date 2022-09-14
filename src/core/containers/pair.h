#pragma once
#ifndef HD_INC_CORE_PAIR_H
#define HD_INC_CORE_PAIR_H
#include "../minimal.h"
#include "../traits/enable_if.h"
#include "../traits/conjunction.h"
#include "../traits/negation.h"
#include "../traits/disjunction.h"
#include "../traits/is_default_constructible.h"
#include "../traits/is_implicitly_default_constructible.h"
#include "../traits/is_explicitly_default_constructible.h"
#include "../traits/is_nothrow_default_constructible.h"
#include "../traits/is_nothrow_swappable.h"
#include "../traits/negation.h"
#include "../traits/is_copy_constructible.h"
#include "../traits/is_convertible.h"
#include "../traits/is_nothrow_copy_constructible.h"
#include "../traits/is_nothrow_constructible.h"
#include "../traits/is_move_constructible.h"
#include "../templates/forward.h"
#include "../traits/is_copy_assignable.h"
#include "../traits/is_nothrow_copy_assignable.h"
#include "../traits/is_assignable.h"
#include "../traits/is_nothrow_assignable.h"
#include "../traits/is_same.h"
#include "../traits/is_move_assignable.h"
#include "../traits/is_nothrow_move_assignable.h"
#include "../templates/swap.h"
#include "tuple_size.h"
#include "tuple_element.h"
#include "../traits/conditional.h"
#include "../traits/decay.h"

namespace hud {

    /**
    * This class couples together a pair of components, which may be of different types (type1_t and type2_t). The individual components can be accessed through its public members first and second.
    * pair is a particular case of Tuple.
    * @tparam type1_t Type of the first component
    * @tparam type2_t Type of the second component
    */
    template<typename type1_t, typename type2_t>
    struct pair {
        using type1_t = type1_t;
        using type2_t = type2_t;

        /** The first element */
        type1_t first;

        /** The second element */
        type2_t second;

        /**
        * Default constructor
        * This involves individually constructing its two component, with an initialization that depends on the constructor.
        * Constructs a pair object with its elements value-initialized.
        * If one of the component is explicitly default constructible, the pair is explicitly default constructible.
        * pair do not accept throwable default constructible components.
        */
        constexpr explicit(!(is_implicitly_default_constructible_v<type1_t>&& is_implicitly_default_constructible_v<type2_t>))
            pair() noexcept requires(is_default_constructible_v<type1_t>&& is_default_constructible_v<type2_t>)
            : first()
            , second() {
            static_assert(is_nothrow_default_constructible_v<type1_t>, "type1_t default constructor is throwable. pair is not designed to allow throwable default constructible components");
            static_assert(is_nothrow_default_constructible_v<type2_t>, "type2_t default constructor is throwable. pair is not designed to allow throwable default constructible components");
        }


        /**
        * Initialization copy constructor.
        * This involves individually constructing its two component, with an initialization that depends on the constructor.
        * Member first is constructed with f and member second with s.
        * pair do not accept throwable default constructible components.
        * @param f An object of the same type of first, or some other type implicitly convertible to it.
        * @param s An object of the same type of second, or some other type implicitly convertible to it.
        */
        constexpr explicit(!(is_convertible_v<const type1_t&, type1_t>&& is_convertible_v<const type2_t&, type2_t>))
            pair(const type1_t& f, const type2_t& s) noexcept requires(is_copy_constructible_v<type1_t>&& is_copy_constructible_v<type2_t>)
            : first(f)
            , second(s) {
            static_assert(is_nothrow_copy_constructible_v<type1_t>, "type1_t(const type1_t&) copy constructor is throwable. pair is not designed to allow throwable copy constructible components");
            static_assert(is_nothrow_copy_constructible_v<type2_t>, "type2_t(const type2_t&) copy constructor is throwable. pair is not designed to allow throwable copy constructible components");
        }


        /**
        * Initialization move constructor.
        * This involves individually constructing its two component, with an initialization that depends on the constructor.
        * Member first is constructed with f and member second with s.
        * pair do not accept throwable default constructible components.
        * @tparam u_type_t Type of the f component.
        * @tparam v_type_t Type of the s component.
        * @param f An object of the same type of first, or some other type implicitly convertible to it.
        * @param s An object of the same type of second, or some other type implicitly convertible to it.
        */
        template<typename u_type_t = type1_t, typename v_type_t = type2_t>
        constexpr explicit(!(is_convertible_v<u_type_t&&, type1_t>&& is_convertible_v<v_type_t&&, type2_t>))
            pair(u_type_t&& f, v_type_t&& s) noexcept requires(is_move_constructible_v<type1_t, u_type_t>&& is_move_constructible_v<type2_t, v_type_t>)
            : first(hud::forward<u_type_t>(f))
            , second(hud::forward<v_type_t>(s)) {
            static_assert(is_nothrow_move_constructible_v<type1_t, u_type_t>, "type1_t(u_type_t&&) move constructor is throwable. pair is not designed to allow throwable move constructible components");
            static_assert(is_nothrow_move_constructible_v<type2_t, v_type_t>, "type2_t(v_type_t&&) move constructor is throwable. pair is not designed to allow throwable move constructible components");
        }

        /**
        * Copy constructor.
        * This involves individually constructing its two component, with an initialization that depends on the constructor.
        * The object is initialized with the contents of the other pair object.
        * The corresponding members of other is passed to the constructor of each of its members.
        * pair do not accept throwable default constructible components.
        * @param other Another pair object.
        */
        constexpr explicit(!(is_convertible_v<const type1_t&, type1_t>&& is_convertible_v<const type2_t&, type2_t>)) pair(const pair& other) noexcept = default;

        /**
        * This involves individually constructing its two component, with an initialization that depends on the constructor.
        * The object is initialized with the contents of the other pair object.
        * The corresponding members of other is passed to the constructor of each of its members.
        * pair do not accept throwable default constructible components.
        * @tparam u_type1_t Type of the first component.
        * @tparam u_type2_t Type of the second component.
        * @param other Another pair object.
        */
        template<typename u_type1_t, typename u_type2_t>
        constexpr explicit(!(is_convertible_v<const u_type1_t&, type1_t>&& is_convertible_v<const u_type2_t&, type2_t>))
            pair(const pair<u_type1_t, u_type2_t>& other) noexcept requires(is_copy_constructible_v<type1_t, u_type1_t>&& is_copy_constructible_v<type2_t, u_type2_t>)
            : first(other.first)
            , second(other.second) {
            static_assert(is_nothrow_copy_constructible_v<type1_t, u_type1_t>, "type1_t(const u_type1_t&) copy constructor is throwable. pair is not designed to allow throwable copy constructible components");
            static_assert(is_nothrow_copy_constructible_v<type2_t, u_type2_t>, "type2_t(const u_type2_t&) copy constructor is throwable. pair is not designed to allow throwable copy constructible components");
        }

        /**
        * Move constructor.
        * This involves individually constructing its two component, with an initialization that depends on the constructor.
        * The object is initialized with the contents of the other pair object.
        * The corresponding members of other is forward to the constructor of each of its members.
        * pair do not accept throwable move constructible components.
        * @param other Another pair object.
        */
        constexpr explicit(!(is_convertible_v<type1_t, type1_t>&& is_convertible_v<type2_t, type2_t>)) pair(pair&& other) noexcept = default;

        /**
        * Move constructor.
        * This involves individually constructing its two component, with an initialization that depends on the constructor.
        * The object is initialized with the contents of the other pair object.
        * The corresponding members of other is forward to the constructor of each of its members.
        * pair do not accept throwable default constructible components.
        * @tparam u_type1_t Type of the first component.
        * @tparam u_type2_t Type of the second component.
        * @param other Another pair object.
        */
        template<typename u_type1_t, typename u_type2_t>
        constexpr explicit(!(is_convertible_v<u_type1_t, type1_t>&& is_convertible_v<u_type2_t,type2_t>))
            pair(pair<u_type1_t, u_type2_t>&& other) noexcept requires(is_move_constructible_v<type1_t, u_type1_t>&& is_move_constructible_v<type2_t, u_type2_t>)
            : first(hud::forward<u_type1_t>(other.first))
            , second(hud::forward<u_type2_t>(other.second)) {
            static_assert(is_nothrow_move_constructible_v<type1_t, u_type1_t>, "type1_t(u_type1_t&&) move constructor is throwable. pair is not designed to allow throwable move constructible components");
            static_assert(is_nothrow_move_constructible_v<type2_t, u_type2_t>, "type2_t(u_type2_t&&) move constructor is throwable. pair is not designed to allow throwable move constructible components");
        }

        /**
        * Assigns other as the new content for the pair object.
        * Perform copy assignments, with the elements of its argument preserving their values after the call.
        * pair do not accept throwable copy assignable components.
        * @param other Another pair object.
        * @return *this
        */
        constexpr pair& operator=(const pair& other) noexcept requires(is_copy_assignable_v<type1_t>&& is_copy_assignable_v<type2_t>) {
            static_assert(IsNothrowCopyAssignableV<type1_t>, "type1_t& type1_t::operator=(const type1_t&) is throwable. pair is not designed to allow throwable copy assignable components");
            static_assert(IsNothrowCopyAssignableV<type2_t>, "type2_t& type2_t::operator=(const type2_t&) is throwable. pair is not designed to allow throwable copy assignable components");
            first = other.first;
            second = other.second;
            return *this;
        }

        /**
        * Assigns other as the new content for the pair object.
        * Perform copy assignments, with the elements of its argument preserving their values after the call.
        * pair do not accept throwable copy assignable components.
        * @tparam u_type_t Type of the first component.
        * @tparam v_type_t Type of the second component.
        * @param other Another pair object.
        * @return *this
        */
        template<typename u_type_t, typename v_type_t>
        constexpr pair& operator=(const pair<u_type_t, v_type_t>& other) noexcept requires(is_assignable_v<type1_t&, const u_type_t&>&& is_copy_assignable_v<type2_t, v_type_t>) {
            static_assert(IsNothrowCopyAssignableV<type1_t, u_type_t>, "type1_t& type1_t::operator=(const u_type_t&) is throwable. pair is not designed to allow throwable copy assignable components");
            static_assert(IsNothrowCopyAssignableV<type2_t, v_type_t>, "type2_t& type2_t::operator=(const v_type_t&) is throwable. pair is not designed to allow throwable copy assignable components");
            first = other.first;
            second = other.second;
            return *this;
        }

        /**
        * Assigns other as the new content for the pair object.
        * Perform move assignments, which, for elements of types supporting move semantics implies that these elements of other are left in an unspecified but valid state.
        * pair do not accept throwable move assignable components.
        * @param other Another pair object.
        * @return *this
        */
        constexpr pair& operator=(pair&& other) noexcept requires(is_move_assignable_v<type1_t>&& is_move_assignable_v<type2_t>) {
            static_assert(is_nothrow_move_assignable_v<type1_t>, "type1_t& type1_t::operator=(type1_t&&) is throwable. pair is not designed to allow throwable move assignable components");
            static_assert(is_nothrow_move_assignable_v<type2_t>, "type2_t& type2_t::operator=(type2_t&&) is throwable. pair is not designed to allow throwable move assignable components");
            first = hud::forward<type1_t>(other.first);
            second = hud::forward<type2_t>(other.second);
            return *this;
        }

        /**
        * Assigns other as the new content for the pair object.
        * Perform move assignments, which, for elements of types supporting move semantics implies that these elements of other are left in an unspecified but valid state.
        * pair do not accept throwable move assignable components.
        * @tparam u_type_t Type of the first component.
        * @tparam v_type_t Type of the second component.
        * @param other Another pair object.
        * @return *this
        */
        template<typename u_type_t, typename v_type_t>
        constexpr pair& operator=(pair<u_type_t, v_type_t>&& other) noexcept requires(is_assignable_v<type1_t&, u_type_t&&> && is_assignable_v<type2_t&, v_type_t&&>) {
            static_assert(is_nothrow_assignable_v<type1_t&, u_type_t&&>, "type1_t& type1_t::operator=(u_type_t&&) is throwable. pair is not designed to allow throwable move assignable components");
            static_assert(is_nothrow_assignable_v<type2_t&, v_type_t&&>, "type2_t& type2_t::operator=(v_type_t&&) is throwable. pair is not designed to allow throwable move assignable components");
            first = hud::forward<u_type_t>(other.first);
            second = hud::forward<v_type_t>(other.second);
            return *this;
        }

        /**
        * Exchanges the contents of the pair object with the contents of other.
        * pair do not accept throwable swap components.
        * @param other The other pair to swap
        */
        constexpr void swap(pair& other) noexcept requires(is_swappable_v<type1_t> && is_swappable_v<type2_t>) {
            static_assert(is_nothrow_swappable_v<type1_t>, "swap(type1_t,type1_t) is throwable. pair is not designed to allow throwable swappable components");
            static_assert(is_nothrow_swappable_v<type2_t>, "swap(type2_t,type2_t) is throwable. pair is not designed to allow throwable swappable components");
            hud::swap(first, other.first);
            hud::swap(second, other.second);
        }
    };

    /**
    * swap specialization for pair
    * @tparam type1_t Type of the first component in pair
    * @tparam type2_t Type of the second component in pair
    * @param a The pair to swap with b
    * @param b The pair to swap with a
    */
    template<typename type1_t, typename type2_t>
    static constexpr void swap(pair<type1_t, type2_t>& a, pair<type1_t, type2_t>& b) noexcept requires(is_swappable_v<type1_t>&& is_swappable_v<type2_t>) {
        a.swap(b);
    }

    /**
    * Tests if both elements of left and right are equal, that is, compares left.first with right.first and left.second with right.second.
    * @tparam type1_t Type of the first component
    * @tparam type2_t Type of the second component
    * @param left The left pair of the comperand operator
    * @param right The right pair of the comperand operator
    * @return true if both elements of left are equals both elements of right, false otherwise
    */
    template<typename type1_t, typename type2_t>
    [[nodiscard]]
    constexpr bool operator==(const pair<type1_t, type2_t>& left, const pair<type1_t, type2_t>& right) noexcept {
        return left.first == right.first && left.second == right.second;
    }


    /**
    * Tests if elements of left or right are not equal, that is, compares left.first with right.first and left.second with right.second.
    * @tparam type1_t Type of the first component
    * @tparam type2_t Type of the second component
    * @param left The left pair of the comperand operator
    * @param right The right pair of the comperand operator
    * @return true if elements of left are not equals to elements of right, false otherwise
    */
    template<typename type1_t, typename type2_t>
    [[nodiscard]]
    constexpr bool operator!=(const pair<type1_t, type2_t>& left, const pair<type1_t, type2_t>& right) noexcept {
        return !(left == right);
    }

    /**
    * Compares right and left lexicographically by operator<, that is, compares the first elements and only if they are equivalent, compares the second elements.
    * @tparam type1_t Type of the first component
    * @tparam type2_t Type of the second component
    * @param left The left pair of the comperand operator
    * @param right The right pair of the comperand operator
    * @return true if left.first<right.first. Otherwise, if right.first<left.first, returns false. Otherwise, if left.second<right.second, returns true. Otherwise, returns false.
    */
    template<typename type1_t, typename type2_t>
    [[nodiscard]]
    constexpr bool operator<(const pair<type1_t, type2_t>& left, const pair<type1_t, type2_t>& right) noexcept {
        return left.first < right.first || (!(right.first < left.first) && left.second < right.second);
    }

    /**
    * Compares right and left lexicographically by operator<, that is, compares the first elements and only if they are equivalent, compares the second elements.
    * @tparam type1_t Type of the first component
    * @tparam type2_t Type of the second component
    * @param left The left pair of the comperand operator
    * @param right The right pair of the comperand operator
    * @return true if right.first<left.first. Otherwise, if left.first<right.first, returns false. Otherwise, if right.second<left.second, returns true. Otherwise, returns false.
    */
    template<typename type1_t, typename type2_t>
    [[nodiscard]]
    constexpr bool operator>(const pair<type1_t, type2_t>& left, const pair<type1_t, type2_t>& right) noexcept {
        return right < left;
    }

    /**
    * Compares right and left lexicographically by operator<, that is, compares the first elements and only if they are equivalent, compares the second elements.
    * @tparam type1_t Type of the first component
    * @tparam type2_t Type of the second component
    * @param left The left pair of the comperand operator
    * @param right The right pair of the comperand operator
    * @return false if right.first<left.first. Otherwise, if left.first<right.first, returns true. Otherwise, if right.second<left.second, returns false. Otherwise, returns true.
    */
    template<typename type1_t, typename type2_t>
    [[nodiscard]]
    constexpr bool operator<=(const pair<type1_t, type2_t>& left, const pair<type1_t, type2_t>& right) noexcept {
        return !(right < left);
    }

    /**
    * Compares right and left lexicographically by operator<, that is, compares the first elements and only if they are equivalent, compares the second elements.
    * @tparam type1_t Type of the first component
    * @tparam type2_t Type of the second component
    * @param left The left pair of the comperand operator
    * @param right The right pair of the comperand operator
    * @return false if left.first<right.first. Otherwise, if right.first<left.first, returns true. Otherwise, if left.second<right.second, returns false. Otherwise, returns true.
    */
    template<typename type1_t, typename type2_t>
    [[nodiscard]]
    constexpr bool operator>=(const pair<type1_t, type2_t>& left, const pair<type1_t, type2_t>& right) noexcept {
        return !(left < right);
    }

    /**
    * Creates a pair object, deducing the target type from the types of arguments.
    * @tparam type1_t Type of the first component
    * @tparam type2_t Type of the second component
    * @return pair<V1, V2> where V1 is decay_t<type1_t> and V2 is decay_t<type2_t>
    */
    template<typename type1_t, typename type2_t>
    [[nodiscard]]
    constexpr pair<decay_t<type1_t>, decay_t<type2_t>> make_pair(type1_t&& value_1, type2_t&& value_2) noexcept {
        return pair<decay_t<type1_t>, decay_t<type2_t>>(hud::forward<type1_t>(value_1), hud::forward<type2_t>(value_2));
    }

    /**
    * tuple_size specialization for pair
    * @tparam type1_t Type of the first component
    * @tparam type2_t Type of the second component
    */
    template<typename type1_t, typename type2_t>
    struct tuple_size <pair<type1_t, type2_t>> 
        : integral_constant<usize, 2> {
    };

    /**
    * tuple_element specilization for Tuple
    * @tparam idx_to_reach Index of the type to reach
    * @tparam type1_t Type of the first component
    * @tparam type2_t Type of the second component
    */
    template<usize idx_to_reach, typename type1_t, typename type2_t>
    struct tuple_element<idx_to_reach, pair<type1_t, type2_t>> {
        static_assert(idx_to_reach < 2, "pair index out of bounds");
        using Type = conditional_t<idx_to_reach == 0, type1_t, type2_t>;
    };

    /**
    * Retrieves a lvalue reference to the member first if index is 0, second if index is 1.
    * @tparam element_index The index of the element
    * @tparam type1_t Type of the first component
    * @tparam type2_t Type of the second component
    * @param pair The pair to access
    * @return LValue reference to the member first if index is 0, second if index is 1.
    */
    template<usize element_index, typename type1_t, typename type2_t>
    [[nodiscard]]
    constexpr tuple_element_t<element_index, pair<type1_t, type2_t>>& get(pair<type1_t, type2_t>& pair) noexcept {
        static_assert(element_index < 2, "pair index out of bounds");
        if constexpr (element_index == 0) {
            return pair.first;
        }
        else if constexpr (element_index == 1) {
            return pair.second;
        }
    }

    /**
    * Retrieves a lvalue reference to the member first if index is 0, second if index is 1.
    * @tparam element_index The index of the element
    * @tparam type1_t Type of the first component
    * @tparam type2_t Type of the second component
    * @param pair The pair to access
    * @return LValue reference to the member first if index is 0, second if index is 1.
    */
    template<usize element_index, typename type1_t, typename type2_t>
    [[nodiscard]]
    constexpr const tuple_element_t<element_index, const pair<type1_t, type2_t>>& get(const pair<type1_t, type2_t>& pair) noexcept {
        static_assert(element_index < 2, "pair index out of bounds");
        if constexpr (element_index == 0) {
            return pair.first;
        }
        else if constexpr (element_index == 1) {
            return pair.second;
        }
    }

    /**
    * Retrieves an rvalue reference to the member first if index is 0, second if index is 1.
    * @tparam element_index The index of the element
    * @tparam type1_t Type of the first component
    * @tparam type2_t Type of the second component
    * @param pair The pair to access
    * @return RValue reference to the member first if index is 0, second if index is 1.
    */
    template<usize element_index, typename type1_t, typename type2_t>
    [[nodiscard]]
    constexpr tuple_element_t<element_index, pair<type1_t, type2_t>>&& get(pair<type1_t, type2_t>&& pair) noexcept {
        static_assert(element_index < 2, "pair index out of bounds");
        if constexpr (element_index == 0) {
            return hud::forward<type1_t&&>(pair.first);
        }
        else if constexpr (element_index == 1) {
            return hud::forward<type2_t&&>(pair.second);
        }
    }

    /**
    * Retrieves an rvalue reference to the member first if index is 0, second if index is 1.
    * @tparam element_index The index of the element
    * @tparam type1_t Type of the first component
    * @tparam type2_t Type of the second component
    * @param pair The pair to access
    * @return RValue reference to the member first if index is 0, second if index is 1.
    */
    template<usize element_index, typename type1_t, typename type2_t>
    [[nodiscard]]
    constexpr const tuple_element_t<element_index, pair<type1_t, type2_t>>&& get(const pair<type1_t, type2_t>&& pair) noexcept {
        static_assert(element_index < 2, "pair index out of bounds");
        if constexpr (element_index == 0) {
            return hud::forward<const type1_t&&>(pair.first);
        }
        else if constexpr (element_index == 1) {
            return hud::forward<const type2_t&&>(pair.second);
        }
    }

} // namespace hud

#endif // HD_INC_CORE_PAIR_H