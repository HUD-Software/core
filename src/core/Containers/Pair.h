#pragma once
#ifndef HD_INC_OSLAYER_PAIR_H
#define HD_INC_OSLAYER_PAIR_H
#include "../Minimal.h"
#include "../Traits/EnableIf.h"
#include "../Traits/And.h"
#include "../Traits/Not.h"
#include "../Traits/Or.h"
#include "../Traits/IsDefaultConstructible.h"
#include "../Traits/IsImplicitlyDefaultConstructible.h"
#include "../Traits/IsExplicitlyDefaultConstructible.h"
#include "../Traits/IsNothrowDefaultConstructible.h"
#include "../Traits/IsNothrowSwappable.h"
#include "../Traits/Not.h"
#include "../Traits/IsCopyConstructible.h"
#include "../Traits/IsConvertible.h"
#include "../Traits/IsNothrowCopyConstructible.h"
#include "../Traits/IsNothrowConstructible.h"
#include "../Traits/IsMoveConstructible.h"
#include "../Templates/Forward.h"
#include "../Traits/IsCopyAssignable.h"
#include "../Traits/IsNothrowCopyAssignable.h"
#include "../Traits/IsAssignable.h"
#include "../Traits/IsNothrowAssignable.h"
#include "../Traits/IsSame.h"
#include "../Traits/IsMoveAssignable.h"
#include "../Traits/IsNothrowMoveAssignable.h"
#include "../Templates/Swap.h"
#include "TupleSize.h"
#include "TupleElement.h"
#include "../Traits/Conditional.h"
#include "../Traits/Decay.h"

namespace hud {

    /**
    * This class couples together a pair of components, which may be of different types (T1 and T2). The individual components can be accessed through its public members first and second.
    * Pair is a particular case of Tuple.
    * @tparam T1 Type of the first component
    * @tparam T2 Type of the second component
    */
    template<typename T1, typename T2>
    struct Pair {
        using FirstType = T1;
        using SecondType = T2;

        /** The first element */
        T1 first;

        /** The second element */
        T2 second;

        /**
        * Default constructor
        * This involves individually constructing its two component, with an initialization that depends on the constructor.
        * Constructs a pair object with its elements value-initialized.
        * If one of the component is explicitly default constructible, the pair is explicitly default constructible.
        * Pair do not accept throwable default constructible components.
        */
        constexpr explicit(!(IsImplicitlyDefaultConstructibleV<T1>&& IsImplicitlyDefaultConstructibleV<T2>))
            Pair() noexcept requires(IsDefaultConstructibleV<T1>&& IsDefaultConstructibleV<T2>)
            : first()
            , second() {
            static_assert(IsNothrowDefaultConstructibleV<T1>, "T1 default constructor is throwable. Pair is not designed to allow throwable default constructible components");
            static_assert(IsNothrowDefaultConstructibleV<T2>, "T2 default constructor is throwable. Pair is not designed to allow throwable default constructible components");
        }


        /**
        * Initialization copy constructor.
        * This involves individually constructing its two component, with an initialization that depends on the constructor.
        * Member first is constructed with f and member second with s.
        * Pair do not accept throwable default constructible components.
        * @param f An object of the same type of first, or some other type implicitly convertible to it.
        * @param s An object of the same type of second, or some other type implicitly convertible to it.
        */
        constexpr explicit(!(IsConvertibleV<const T1&, T1>&& IsConvertibleV<const T2&, T2>))
            Pair(const T1& f, const T2& s) noexcept requires(IsCopyConstructibleV<T1>&& IsCopyConstructibleV<T2>)
            : first(f)
            , second(s) {
            static_assert(IsNothrowCopyConstructibleV<T1>, "T1(const T1&) copy constructor is throwable. Pair is not designed to allow throwable copy constructible components");
            static_assert(IsNothrowCopyConstructibleV<T2>, "T2(const T2&) copy constructor is throwable. Pair is not designed to allow throwable copy constructible components");
        }


        /**
        * Initialization move constructor.
        * This involves individually constructing its two component, with an initialization that depends on the constructor.
        * Member first is constructed with f and member second with s.
        * Pair do not accept throwable default constructible components.
        * @tparam U Type of the f component.
        * @tparam V Type of the s component.
        * @param f An object of the same type of first, or some other type implicitly convertible to it.
        * @param s An object of the same type of second, or some other type implicitly convertible to it.
        */
        template<typename U = T1, typename V = T2>
        constexpr explicit(!(IsConvertibleV<U&&, T1>&& IsConvertibleV<V&&, T2>))
            Pair(U&& f, V&& s) noexcept requires(IsMoveConstructibleV<T1, U>&& IsMoveConstructibleV<T2, V>)
            : first(forward<U>(f))
            , second(forward<V>(s)) {
            static_assert(IsNothrowMoveConstructibleV<T1, U>, "T1(U&&) move constructor is throwable. Pair is not designed to allow throwable move constructible components");
            static_assert(IsNothrowMoveConstructibleV<T2, V>, "T2(V&&) move constructor is throwable. Pair is not designed to allow throwable move constructible components");
        }

        /**
        * Copy constructor.
        * This involves individually constructing its two component, with an initialization that depends on the constructor.
        * The object is initialized with the contents of the other pair object.
        * The corresponding members of other is passed to the constructor of each of its members.
        * Pair do not accept throwable default constructible components.
        * @param other Another pair object.
        */
        constexpr explicit(!(IsConvertibleV<const T1&, T1>&& IsConvertibleV<const T2&, T2>)) Pair(const Pair& other) noexcept = default;

        /**
        * This involves individually constructing its two component, with an initialization that depends on the constructor.
        * The object is initialized with the contents of the other pair object.
        * The corresponding members of other is passed to the constructor of each of its members.
        * Pair do not accept throwable default constructible components.
        * @tparam U1 Type of the first component.
        * @tparam U2 Type of the second component.
        * @param other Another pair object.
        */
        template<typename U1, typename U2>
        constexpr explicit(!(IsConvertibleV<const U1&, T1>&& IsConvertibleV<const U2&, T2>))
            Pair(const Pair<U1, U2>& other) noexcept requires(IsCopyConstructibleV<T1, U1>&& IsCopyConstructibleV<T2, U2>)
            : first(other.first)
            , second(other.second) {
            static_assert(IsNothrowCopyConstructibleV<T1, U1>, "T1(const U1&) copy constructor is throwable. Pair is not designed to allow throwable copy constructible components");
            static_assert(IsNothrowCopyConstructibleV<T2, U2>, "T2(const U2&) copy constructor is throwable. Pair is not designed to allow throwable copy constructible components");
        }

        /**
        * Move constructor.
        * This involves individually constructing its two component, with an initialization that depends on the constructor.
        * The object is initialized with the contents of the other pair object.
        * The corresponding members of other is forward to the constructor of each of its members.
        * Pair do not accept throwable move constructible components.
        * @param other Another pair object.
        */
        constexpr explicit(!(IsConvertibleV<T1, T1>&& IsConvertibleV<T2, T2>)) Pair(Pair&& other) noexcept = default;

        /**
        * Move constructor.
        * This involves individually constructing its two component, with an initialization that depends on the constructor.
        * The object is initialized with the contents of the other pair object.
        * The corresponding members of other is forward to the constructor of each of its members.
        * Pair do not accept throwable default constructible components.
        * @tparam U1 Type of the first component.
        * @tparam U2 Type of the second component.
        * @param other Another pair object.
        */
        template<typename U1, typename U2>
        constexpr explicit(!(IsConvertibleV<U1, T1>&& IsConvertibleV<U2,T2>))
            Pair(Pair<U1, U2>&& other) noexcept requires(IsMoveConstructibleV<T1, U1>&& IsMoveConstructibleV<T2, U2>)
            : first(forward<U1>(other.first))
            , second(forward<U2>(other.second)) {
            static_assert(IsNothrowMoveConstructibleV<T1, U1>, "T1(U1&&) move constructor is throwable. Pair is not designed to allow throwable move constructible components");
            static_assert(IsNothrowMoveConstructibleV<T2, U2>, "T2(U2&&) move constructor is throwable. Pair is not designed to allow throwable move constructible components");
        }

        /**
        * Assigns other as the new content for the pair object.
        * Perform copy assignments, with the elements of its argument preserving their values after the call.
        * Pair do not accept throwable copy assignable components.
        * @param other Another pair object.
        * @return *this
        */
        constexpr Pair& operator=(const Pair& other) noexcept requires(IsCopyAssignableV<T1>&& IsCopyAssignableV<T2>) {
            static_assert(IsNothrowCopyAssignableV<T1>, "T1& T1::operator=(const T1&) is throwable. Pair is not designed to allow throwable copy assignable components");
            static_assert(IsNothrowCopyAssignableV<T2>, "T2& T2::operator=(const T2&) is throwable. Pair is not designed to allow throwable copy assignable components");
            first = other.first;
            second = other.second;
            return *this;
        }

        /**
        * Assigns other as the new content for the pair object.
        * Perform copy assignments, with the elements of its argument preserving their values after the call.
        * Pair do not accept throwable copy assignable components.
        * @tparam U Type of the first component.
        * @tparam V Type of the second component.
        * @param other Another pair object.
        * @return *this
        */
        template<typename U, typename V>
        constexpr Pair& operator=(const Pair<U, V>& other) noexcept requires(IsAssignableV<T1&, const U&>&& IsCopyAssignableV<T2, V>) {
            static_assert(IsNothrowCopyAssignableV<T1, U>, "T1& T1::operator=(const U&) is throwable. Pair is not designed to allow throwable copy assignable components");
            static_assert(IsNothrowCopyAssignableV<T2, V>, "T2& T2::operator=(const V&) is throwable. Pair is not designed to allow throwable copy assignable components");
            first = other.first;
            second = other.second;
            return *this;
        }

        /**
        * Assigns other as the new content for the pair object.
        * Perform move assignments, which, for elements of types supporting move semantics implies that these elements of other are left in an unspecified but valid state.
        * Pair do not accept throwable move assignable components.
        * @param other Another pair object.
        * @return *this
        */
        constexpr Pair& operator=(Pair&& other) noexcept requires(IsMoveAssignableV<T1>&& IsMoveAssignableV<T2>) {
            static_assert(IsNothrowMoveAssignableV<T1>, "T1& T1::operator=(T1&&) is throwable. Pair is not designed to allow throwable move assignable components");
            static_assert(IsNothrowMoveAssignableV<T2>, "T2& T2::operator=(T2&&) is throwable. Pair is not designed to allow throwable move assignable components");
            first = forward<T1>(other.first);
            second = forward<T2>(other.second);
            return *this;
        }

        /**
        * Assigns other as the new content for the pair object.
        * Perform move assignments, which, for elements of types supporting move semantics implies that these elements of other are left in an unspecified but valid state.
        * Pair do not accept throwable move assignable components.
        * @tparam U Type of the first component.
        * @tparam V Type of the second component.
        * @param other Another pair object.
        * @return *this
        */
        template<typename U, typename V>
        constexpr Pair& operator=(Pair<U, V>&& other) noexcept requires(IsAssignableV<T1&, U&&> && IsAssignableV<T2&, V&&>) {
            static_assert(IsNothrowAssignableV<T1&, U&&>, "T1& T1::operator=(U&&) is throwable. Pair is not designed to allow throwable move assignable components");
            static_assert(IsNothrowAssignableV<T2&, V&&>, "T2& T2::operator=(V&&) is throwable. Pair is not designed to allow throwable move assignable components");
            first = forward<U>(other.first);
            second = forward<V>(other.second);
            return *this;
        }

        /**
        * Exchanges the contents of the Pair object with the contents of other.
        * Pair do not accept throwable swap components.
        * @param other The other pair to swap
        */
        constexpr void swap(Pair& other) noexcept requires(IsSwappableV<T1> && IsSwappableV<T2>) {
            static_assert(IsNothrowSwappableV<T1>, "swap(T1,T1) is throwable. Pair is not designed to allow throwable swappable components");
            static_assert(IsNothrowSwappableV<T2>, "swap(T2,T2) is throwable. Pair is not designed to allow throwable swappable components");
            hud::swap(first, other.first);
            hud::swap(second, other.second);
        }
    };

    /**
    * swap specialization for Pair
    * @tparam T1 Type of the first component in Pair
    * @tparam T2 Type of the second component in Pair
    * @param a The pair to swap with b
    * @param b The pair to swap with a
    */
    template<typename T1, typename T2>
    static constexpr void swap(Pair<T1, T2>& a, Pair<T1, T2>& b) noexcept requires(IsSwappableV<T1>&& IsSwappableV<T2>) {
        a.swap(b);
    }

    /**
    * Tests if both elements of left and right are equal, that is, compares left.first with right.first and left.second with right.second.
    * @tparam T1 Type of the first component
    * @tparam T2 Type of the second component
    * @param left The left pair of the comperand operator
    * @param right The right pair of the comperand operator
    * @return true if both elements of left are equals both elements of right, false otherwise
    */
    template<typename T1, typename T2>
    [[nodiscard]]
    constexpr bool operator==(const Pair<T1, T2>& left, const Pair<T1, T2>& right) noexcept {
        return left.first == right.first && left.second == right.second;
    }


    /**
    * Tests if elements of left or right are not equal, that is, compares left.first with right.first and left.second with right.second.
    * @tparam T1 Type of the first component
    * @tparam T2 Type of the second component
    * @param left The left pair of the comperand operator
    * @param right The right pair of the comperand operator
    * @return true if elements of left are not equals to elements of right, false otherwise
    */
    template<typename T1, typename T2>
    [[nodiscard]]
    constexpr bool operator!=(const Pair<T1, T2>& left, const Pair<T1, T2>& right) noexcept {
        return !(left == right);
    }

    /**
    * Compares right and left lexicographically by operator<, that is, compares the first elements and only if they are equivalent, compares the second elements.
    * @tparam T1 Type of the first component
    * @tparam T2 Type of the second component
    * @param left The left pair of the comperand operator
    * @param right The right pair of the comperand operator
    * @return true if left.first<right.first. Otherwise, if right.first<left.first, returns false. Otherwise, if left.second<right.second, returns true. Otherwise, returns false.
    */
    template<typename T1, typename T2>
    [[nodiscard]]
    constexpr bool operator<(const Pair<T1, T2>& left, const Pair<T1, T2>& right) noexcept {
        return left.first < right.first || (!(right.first < left.first) && left.second < right.second);
    }

    /**
    * Compares right and left lexicographically by operator<, that is, compares the first elements and only if they are equivalent, compares the second elements.
    * @tparam T1 Type of the first component
    * @tparam T2 Type of the second component
    * @param left The left pair of the comperand operator
    * @param right The right pair of the comperand operator
    * @return true if right.first<left.first. Otherwise, if left.first<right.first, returns false. Otherwise, if right.second<left.second, returns true. Otherwise, returns false.
    */
    template<typename T1, typename T2>
    [[nodiscard]]
    constexpr bool operator>(const Pair<T1, T2>& left, const Pair<T1, T2>& right) noexcept {
        return right < left;
    }

    /**
    * Compares right and left lexicographically by operator<, that is, compares the first elements and only if they are equivalent, compares the second elements.
    * @tparam T1 Type of the first component
    * @tparam T2 Type of the second component
    * @param left The left pair of the comperand operator
    * @param right The right pair of the comperand operator
    * @return false if right.first<left.first. Otherwise, if left.first<right.first, returns true. Otherwise, if right.second<left.second, returns false. Otherwise, returns true.
    */
    template<typename T1, typename T2>
    [[nodiscard]]
    constexpr bool operator<=(const Pair<T1, T2>& left, const Pair<T1, T2>& right) noexcept {
        return !(right < left);
    }

    /**
    * Compares right and left lexicographically by operator<, that is, compares the first elements and only if they are equivalent, compares the second elements.
    * @tparam T1 Type of the first component
    * @tparam T2 Type of the second component
    * @param left The left pair of the comperand operator
    * @param right The right pair of the comperand operator
    * @return false if left.first<right.first. Otherwise, if right.first<left.first, returns true. Otherwise, if left.second<right.second, returns false. Otherwise, returns true.
    */
    template<typename T1, typename T2>
    [[nodiscard]]
    constexpr bool operator>=(const Pair<T1, T2>& left, const Pair<T1, T2>& right) noexcept {
        return !(left < right);
    }

    /**
    * Creates a Pair object, deducing the target type from the types of arguments.
    * @tparam T1 Type of the first component
    * @tparam T2 Type of the second component
    * @return Pair<V1, V2> where V1 is DecayT<T1> and V2 is DecayT<T2>
    */
    template<typename T1, typename T2>
    [[nodiscard]]
    constexpr Pair<DecayT<T1>, DecayT<T2>> make_pair(T1&& value_1, T2&& value_2) noexcept {
        return Pair<DecayT<T1>, DecayT<T2>>(forward<T1>(value_1), forward<T2>(value_2));
    }

    /**
    * TupleSize specialization for Pair
    * @tparam T1 Type of the first component
    * @tparam T2 Type of the second component
    */
    template<typename T1, typename T2>
    struct TupleSize <Pair<T1, T2>> 
        : IntegralConstant<usize, 2> {
    };

    /**
    * TupleElement specilization for Tuple
    * @tparam idx_to_reach Index of the type to reach
    * @tparam T1 Type of the first component
    * @tparam T2 Type of the second component
    */
    template<usize idx_to_reach, typename T1, typename T2>
    struct TupleElement<idx_to_reach, Pair<T1, T2>> {
        static_assert(idx_to_reach < 2, "Pair index out of bounds");
        using Type = ConditionalT<idx_to_reach == 0, T1, T2>;
    };

    /**
    * Retrieves a lvalue reference to the member first if index is 0, second if index is 1.
    * @tparam element_index The index of the element
    * @tparam T1 Type of the first component
    * @tparam T2 Type of the second component
    * @param pair The pair to access
    * @return LValue reference to the member first if index is 0, second if index is 1.
    */
    template<usize element_index, typename T1, typename T2>
    [[nodiscard]]
    constexpr TupleElementT<element_index, Pair<T1, T2>>& get(Pair<T1, T2>& pair) noexcept {
        static_assert(element_index < 2, "Pair index out of bounds");
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
    * @tparam T1 Type of the first component
    * @tparam T2 Type of the second component
    * @param pair The pair to access
    * @return LValue reference to the member first if index is 0, second if index is 1.
    */
    template<usize element_index, typename T1, typename T2>
    [[nodiscard]]
    constexpr const TupleElementT<element_index, const Pair<T1, T2>>& get(const Pair<T1, T2>& pair) noexcept {
        static_assert(element_index < 2, "Pair index out of bounds");
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
    * @tparam T1 Type of the first component
    * @tparam T2 Type of the second component
    * @param pair The pair to access
    * @return RValue reference to the member first if index is 0, second if index is 1.
    */
    template<usize element_index, typename T1, typename T2>
    [[nodiscard]]
    constexpr TupleElementT<element_index, Pair<T1, T2>>&& get(Pair<T1, T2>&& pair) noexcept {
        static_assert(element_index < 2, "Pair index out of bounds");
        if constexpr (element_index == 0) {
            return forward<T1&&>(pair.first);
        }
        else if constexpr (element_index == 1) {
            return forward<T2&&>(pair.second);
        }
    }

    /**
    * Retrieves an rvalue reference to the member first if index is 0, second if index is 1.
    * @tparam element_index The index of the element
    * @tparam T1 Type of the first component
    * @tparam T2 Type of the second component
    * @param pair The pair to access
    * @return RValue reference to the member first if index is 0, second if index is 1.
    */
    template<usize element_index, typename T1, typename T2>
    [[nodiscard]]
    constexpr const TupleElementT<element_index, Pair<T1, T2>>&& get(const Pair<T1, T2>&& pair) noexcept {
        static_assert(element_index < 2, "Pair index out of bounds");
        if constexpr (element_index == 0) {
            return forward<const T1&&>(pair.first);
        }
        else if constexpr (element_index == 1) {
            return forward<const T2&&>(pair.second);
        }
    }

} // namespace hud

#endif // HD_INC_OSLAYER_PAIR_H