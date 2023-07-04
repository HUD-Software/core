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

namespace hud
{

    /**
     * This class couples together a pair of components, which may be of different types (first_type and second_type). The individual components can be accessed through its public members first and second.
     * pair is a particular case of Tuple.
     * @tparam first_type Type of the first component
     * @tparam second_type Type of the second component
     */
    template<typename first_t, typename second_t>
    struct pair
    {
        using first_type = first_t;
        using second_type = second_t;

        /** The first element */
        first_type first;

        /** The second element */
        second_type second;

        /**
         * Default constructor
         * This involves individually constructing its two component, with an initialization that depends on the constructor.
         * Constructs a pair object with its elements value-initialized.
         * If one of the component is explicitly default constructible, the pair is explicitly default constructible.
         * pair do not accept throwable default constructible components.
         */
        constexpr explicit(!(hud::is_implicitly_default_constructible_v<first_type> && hud::is_implicitly_default_constructible_v<second_type>)) pair() noexcept
        requires(hud::is_default_constructible_v<first_type> && hud::is_default_constructible_v<second_type>)
            : first()
            , second()
        {
            static_assert(hud::is_nothrow_default_constructible_v<first_type>, "first_type default constructor is throwable. pair is not designed to allow throwable default constructible components");
            static_assert(hud::is_nothrow_default_constructible_v<second_type>, "second_type default constructor is throwable. pair is not designed to allow throwable default constructible components");
        }

        /**
         * Initialization copy constructor.
         * This involves individually constructing its two component, with an initialization that depends on the constructor.
         * Member first is constructed with f and member second with s.
         * pair do not accept throwable default constructible components.
         * @param f An object of the same type of first, or some other type implicitly convertible to it.
         * @param s An object of the same type of second, or some other type implicitly convertible to it.
         */
        constexpr explicit(!(hud::is_convertible_v<const first_type &, first_type> && hud::is_convertible_v<const second_type &, second_type>)) pair(const first_type &f, const second_type &s) noexcept
        requires(hud::is_copy_constructible_v<first_type> && hud::is_copy_constructible_v<second_type>)
            : first(f)
            , second(s)
        {
            static_assert(hud::is_nothrow_copy_constructible_v<first_type>, "first_type(const first_type&) copy constructor is throwable. pair is not designed to allow throwable copy constructible components");
            static_assert(hud::is_nothrow_copy_constructible_v<second_type>, "second_type(const second_type&) copy constructor is throwable. pair is not designed to allow throwable copy constructible components");
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
        template<typename u_type_t = first_type, typename v_type_t = second_type>
        requires(hud::is_move_constructible_v<first_type, u_type_t> && hud::is_move_constructible_v<second_type, v_type_t>)
        constexpr explicit(!(hud::is_convertible_v<u_type_t &&, first_type> && hud::is_convertible_v<v_type_t &&, second_type>))
            pair(u_type_t &&f, v_type_t &&s) noexcept
            : first(hud::forward<u_type_t>(f))
            , second(hud::forward<v_type_t>(s))
        {
            static_assert(hud::is_nothrow_move_constructible_v<first_type, u_type_t>, "first_type(u_type_t&&) move constructor is throwable. pair is not designed to allow throwable move constructible components");
            static_assert(hud::is_nothrow_move_constructible_v<second_type, v_type_t>, "second_type(v_type_t&&) move constructor is throwable. pair is not designed to allow throwable move constructible components");
        }

        /**
         * Copy constructor.
         * This involves individually constructing its two component, with an initialization that depends on the constructor.
         * The object is initialized with the contents of the other pair object.
         * The corresponding members of other is passed to the constructor of each of its members.
         * pair do not accept throwable default constructible components.
         * @param other Another pair object.
         */
        constexpr explicit(!(hud::is_convertible_v<const first_type &, first_type> && hud::is_convertible_v<const second_type &, second_type>)) pair(const pair &other) noexcept = default;

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
        requires(hud::is_copy_constructible_v<first_type, u_type1_t> && hud::is_copy_constructible_v<second_type, u_type2_t>)
        constexpr explicit(!(hud::is_convertible_v<const u_type1_t &, first_type> && hud::is_convertible_v<const u_type2_t &, second_type>))
            pair(const pair<u_type1_t, u_type2_t> &other) noexcept
            : first(other.first)
            , second(other.second)
        {
            static_assert(hud::is_nothrow_copy_constructible_v<first_type, u_type1_t>, "first_type(const u_type1_t&) copy constructor is throwable. pair is not designed to allow throwable copy constructible components");
            static_assert(hud::is_nothrow_copy_constructible_v<second_type, u_type2_t>, "second_type(const u_type2_t&) copy constructor is throwable. pair is not designed to allow throwable copy constructible components");
        }

        /**
         * Move constructor.
         * This involves individually constructing its two component, with an initialization that depends on the constructor.
         * The object is initialized with the contents of the other pair object.
         * The corresponding members of other is forward to the constructor of each of its members.
         * pair do not accept throwable move constructible components.
         * @param other Another pair object.
         */
        constexpr explicit(!(hud::is_convertible_v<first_type, first_type> && hud::is_convertible_v<second_type, second_type>)) pair(pair &&other) noexcept = default;

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
        requires(hud::is_move_constructible_v<first_type, u_type1_t> && hud::is_move_constructible_v<second_type, u_type2_t>)
        constexpr explicit(!(hud::is_convertible_v<u_type1_t, first_type> && hud::is_convertible_v<u_type2_t, second_type>))
            pair(pair<u_type1_t, u_type2_t> &&other) noexcept
            : first(hud::forward<u_type1_t>(other.first))
            , second(hud::forward<u_type2_t>(other.second))
        {
            static_assert(hud::is_nothrow_move_constructible_v<first_type, u_type1_t>, "first_type(u_type1_t&&) move constructor is throwable. pair is not designed to allow throwable move constructible components");
            static_assert(hud::is_nothrow_move_constructible_v<second_type, u_type2_t>, "second_type(u_type2_t&&) move constructor is throwable. pair is not designed to allow throwable move constructible components");
        }

        /**
         * Assigns other as the new content for the pair object.
         * Perform copy assignments, with the elements of its argument preserving their values after the call.
         * pair do not accept throwable copy assignable components.
         * @param other Another pair object.
         * @return *this
         */
        constexpr pair &operator=(const pair &other) noexcept
        requires(hud::is_copy_assignable_v<first_type> && hud::is_copy_assignable_v<second_type>)
        {
            static_assert(hud::is_nothrow_copy_assignable_v<first_type>, "first_type& first_type::operator=(const first_type&) is throwable. pair is not designed to allow throwable copy assignable components");
            static_assert(hud::is_nothrow_copy_assignable_v<second_type>, "second_type& second_type::operator=(const second_type&) is throwable. pair is not designed to allow throwable copy assignable components");
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
        requires(hud::is_assignable_v<first_type &, const u_type_t &> && hud::is_copy_assignable_v<second_type, v_type_t>)
        constexpr pair &operator=(const pair<u_type_t, v_type_t> &other) noexcept
        {
            static_assert(hud::is_nothrow_copy_assignable_v<first_type, u_type_t>, "first_type& first_type::operator=(const u_type_t&) is throwable. pair is not designed to allow throwable copy assignable components");
            static_assert(hud::is_nothrow_copy_assignable_v<second_type, v_type_t>, "second_type& second_type::operator=(const v_type_t&) is throwable. pair is not designed to allow throwable copy assignable components");
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
        constexpr pair &operator=(pair &&other) noexcept
        requires(hud::is_move_assignable_v<first_type> && hud::is_move_assignable_v<second_type>)
        {
            static_assert(hud::is_nothrow_move_assignable_v<first_type>, "first_type& first_type::operator=(first_type&&) is throwable. pair is not designed to allow throwable move assignable components");
            static_assert(hud::is_nothrow_move_assignable_v<second_type>, "second_type& second_type::operator=(second_type&&) is throwable. pair is not designed to allow throwable move assignable components");
            first = hud::forward<first_type>(other.first);
            second = hud::forward<second_type>(other.second);
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
        requires(hud::is_assignable_v<first_type &, u_type_t &&> && hud::is_assignable_v<second_type &, v_type_t &&>)
        constexpr pair &operator=(pair<u_type_t, v_type_t> &&other) noexcept
        {
            static_assert(hud::is_nothrow_assignable_v<first_type &, u_type_t &&>, "first_type& first_type::operator=(u_type_t&&) is throwable. pair is not designed to allow throwable move assignable components");
            static_assert(hud::is_nothrow_assignable_v<second_type &, v_type_t &&>, "second_type& second_type::operator=(v_type_t&&) is throwable. pair is not designed to allow throwable move assignable components");
            first = hud::forward<u_type_t>(other.first);
            second = hud::forward<v_type_t>(other.second);
            return *this;
        }

        /**
         * Exchanges the contents of the pair object with the contents of other.
         * pair do not accept throwable swap components.
         * @param other The other pair to swap
         */
        constexpr void swap(pair &other) noexcept
        requires(hud::is_swappable_v<first_type> && hud::is_swappable_v<second_type>)
        {
            static_assert(hud::is_nothrow_swappable_v<first_type>, "swap(first_type,first_type) is throwable. pair is not designed to allow throwable swappable components");
            static_assert(hud::is_nothrow_swappable_v<second_type>, "swap(second_type,second_type) is throwable. pair is not designed to allow throwable swappable components");
            hud::swap(first, other.first);
            hud::swap(second, other.second);
        }
    };

    /**
     * swap specialization for pair
     * @tparam first_type Type of the first component in pair
     * @tparam second_type Type of the second component in pair
     * @param a The pair to swap with b
     * @param b The pair to swap with a
     */
    template<typename first_type, typename second_type>
    requires(hud::is_swappable_v<first_type> && hud::is_swappable_v<second_type>)
    static constexpr void swap(pair<first_type, second_type> &a, pair<first_type, second_type> &b) noexcept
    {
        a.swap(b);
    }

    /**
     * Tests if both elements of left and right are equal, that is, compares left.first with right.first and left.second with right.second.
     * @tparam first_type Type of the first component
     * @tparam second_type Type of the second component
     * @param left The left pair of the comperand operator
     * @param right The right pair of the comperand operator
     * @return true if both elements of left are equal both elements of right, false otherwise
     */
    template<typename first_type, typename second_type>
    [[nodiscard]] constexpr bool operator==(const pair<first_type, second_type> &left, const pair<first_type, second_type> &right) noexcept
    {
        return left.first == right.first && left.second == right.second;
    }

    /**
     * Tests if elements of left or right are not equal, that is, compares left.first with right.first and left.second with right.second.
     * @tparam first_type Type of the first component
     * @tparam second_type Type of the second component
     * @param left The left pair of the comperand operator
     * @param right The right pair of the comperand operator
     * @return true if elements of left are not equals to elements of right, false otherwise
     */
    template<typename first_type, typename second_type>
    [[nodiscard]] constexpr bool operator!=(const pair<first_type, second_type> &left, const pair<first_type, second_type> &right) noexcept
    {
        return !(left == right);
    }

    /**
     * Compares right and left lexicographically by operator<, that is, compares the first elements and only if they are equivalent, compares the second elements.
     * @tparam first_type Type of the first component
     * @tparam second_type Type of the second component
     * @param left The left pair of the comperand operator
     * @param right The right pair of the comperand operator
     * @return true if left.first<right.first. Otherwise, if right.first<left.first, returns false. Otherwise, if left.second<right.second, returns true. Otherwise, returns false.
     */
    template<typename first_type, typename second_type>
    [[nodiscard]] constexpr bool operator<(const pair<first_type, second_type> &left, const pair<first_type, second_type> &right) noexcept
    {
        return left.first < right.first || (!(right.first < left.first) && left.second < right.second);
    }

    /**
     * Compares right and left lexicographically by operator<, that is, compares the first elements and only if they are equivalent, compares the second elements.
     * @tparam first_type Type of the first component
     * @tparam second_type Type of the second component
     * @param left The left pair of the comperand operator
     * @param right The right pair of the comperand operator
     * @return true if right.first<left.first. Otherwise, if left.first<right.first, returns false. Otherwise, if right.second<left.second, returns true. Otherwise, returns false.
     */
    template<typename first_type, typename second_type>
    [[nodiscard]] constexpr bool operator>(const pair<first_type, second_type> &left, const pair<first_type, second_type> &right) noexcept
    {
        return right < left;
    }

    /**
     * Compares right and left lexicographically by operator<, that is, compares the first elements and only if they are equivalent, compares the second elements.
     * @tparam first_type Type of the first component
     * @tparam second_type Type of the second component
     * @param left The left pair of the comperand operator
     * @param right The right pair of the comperand operator
     * @return false if right.first<left.first. Otherwise, if left.first<right.first, returns true. Otherwise, if right.second<left.second, returns false. Otherwise, returns true.
     */
    template<typename first_type, typename second_type>
    [[nodiscard]] constexpr bool operator<=(const pair<first_type, second_type> &left, const pair<first_type, second_type> &right) noexcept
    {
        return !(right < left);
    }

    /**
     * Compares right and left lexicographically by operator<, that is, compares the first elements and only if they are equivalent, compares the second elements.
     * @tparam first_type Type of the first component
     * @tparam second_type Type of the second component
     * @param left The left pair of the comperand operator
     * @param right The right pair of the comperand operator
     * @return false if left.first<right.first. Otherwise, if right.first<left.first, returns true. Otherwise, if left.second<right.second, returns false. Otherwise, returns true.
     */
    template<typename first_type, typename second_type>
    [[nodiscard]] constexpr bool operator>=(const pair<first_type, second_type> &left, const pair<first_type, second_type> &right) noexcept
    {
        return !(left < right);
    }

    /**
     * Creates a pair object, deducing the target type from the types of arguments.
     * @tparam first_type Type of the first component
     * @tparam second_type Type of the second component
     * @return pair<V1, V2> where V1 is hud::decay_t<first_type> and V2 is hud::decay_t<second_type>
     */
    template<typename first_type, typename second_type>
    [[nodiscard]] constexpr hud::pair<hud::decay_t<first_type>, hud::decay_t<second_type>> make_pair(first_type &&value_1, second_type &&value_2) noexcept
    {
        return hud::pair<hud::decay_t<first_type>, hud::decay_t<second_type>>(hud::forward<first_type>(value_1), hud::forward<second_type>(value_2));
    }

    /**
     * tuple_size specialization for pair
     * @tparam first_type Type of the first component
     * @tparam second_type Type of the second component
     */
    template<typename first_type, typename second_type>
    struct tuple_size<hud::pair<first_type, second_type>>
        : hud::integral_constant<usize, 2>
    {
    };

    /**
     * tuple_element specilization for Tuple
     * @tparam idx_to_reach Index of the type to reach
     * @tparam first_type Type of the first component
     * @tparam second_type Type of the second component
     */
    template<usize idx_to_reach, typename first_type, typename second_type>
    struct tuple_element<idx_to_reach, pair<first_type, second_type>>
    {
        static_assert(idx_to_reach < 2, "pair index out of bounds");
        using type = hud::conditional_t<idx_to_reach == 0, first_type, second_type>;
    };

    /**
     * Retrieves a lvalue reference to the member first if index is 0, second if index is 1.
     * @tparam element_index The index of the element
     * @tparam first_type Type of the first component
     * @tparam second_type Type of the second component
     * @param pair The pair to access
     * @return LValue reference to the member first if index is 0, second if index is 1.
     */
    template<usize element_index, typename first_type, typename second_type>
    [[nodiscard]] constexpr hud::tuple_element_t<element_index, pair<first_type, second_type>> &get(pair<first_type, second_type> &pair) noexcept
    {
        static_assert(element_index < 2, "pair index out of bounds");
        if constexpr (element_index == 0)
        {
            return pair.first;
        }
        else if constexpr (element_index == 1)
        {
            return pair.second;
        }
    }

    /**
     * Retrieves a lvalue reference to the member first if index is 0, second if index is 1.
     * @tparam element_index The index of the element
     * @tparam first_type Type of the first component
     * @tparam second_type Type of the second component
     * @param pair The pair to access
     * @return LValue reference to the member first if index is 0, second if index is 1.
     */
    template<usize element_index, typename first_type, typename second_type>
    [[nodiscard]] constexpr const hud::tuple_element_t<element_index, const pair<first_type, second_type>> &get(const pair<first_type, second_type> &pair) noexcept
    {
        static_assert(element_index < 2, "pair index out of bounds");
        if constexpr (element_index == 0)
        {
            return pair.first;
        }
        else if constexpr (element_index == 1)
        {
            return pair.second;
        }
    }

    /**
     * Retrieves an rvalue reference to the member first if index is 0, second if index is 1.
     * @tparam element_index The index of the element
     * @tparam first_type Type of the first component
     * @tparam second_type Type of the second component
     * @param pair The pair to access
     * @return RValue reference to the member first if index is 0, second if index is 1.
     */
    template<usize element_index, typename first_type, typename second_type>
    [[nodiscard]] constexpr hud::tuple_element_t<element_index, pair<first_type, second_type>> &&get(pair<first_type, second_type> &&pair) noexcept
    {
        static_assert(element_index < 2, "pair index out of bounds");
        if constexpr (element_index == 0)
        {
            return hud::forward<first_type &&>(pair.first);
        }
        else if constexpr (element_index == 1)
        {
            return hud::forward<second_type &&>(pair.second);
        }
    }

    /**
     * Retrieves an rvalue reference to the member first if index is 0, second if index is 1.
     * @tparam element_index The index of the element
     * @tparam first_type Type of the first component
     * @tparam second_type Type of the second component
     * @param pair The pair to access
     * @return RValue reference to the member first if index is 0, second if index is 1.
     */
    template<usize element_index, typename first_type, typename second_type>
    [[nodiscard]] constexpr const hud::tuple_element_t<element_index, pair<first_type, second_type>> &&get(const pair<first_type, second_type> &&pair) noexcept
    {
        static_assert(element_index < 2, "pair index out of bounds");
        if constexpr (element_index == 0)
        {
            return hud::forward<const first_type &&>(pair.first);
        }
        else if constexpr (element_index == 1)
        {
            return hud::forward<const second_type &&>(pair.second);
        }
    }

} // namespace hud

#endif // HD_INC_CORE_PAIR_H