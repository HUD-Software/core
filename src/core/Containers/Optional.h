#pragma once
#ifndef HD_INC_OSLAYER_OPTIONAL_H
#define HD_INC_OSLAYER_OPTIONAL_H
#include "AlignedBuffer.h"
#include "../Memory.h"
#include "../Templates/Forward.h"
#include "../Templates/Move.h"
#include "../TagInPlace.h"
#include "../Templates/Swap.h"
#include "../Traits/IsConstructible.h"
#include "../Traits/IsCopyConstructible.h"
#include "../Traits/IsMoveConstructible.h"
#include "../Traits/IsAssignable.h"
#include "../Traits/IsTriviallyDestructible.h"
#include "../Traits/IsTriviallyCopyConstructible.h"
#include "../Traits/IsTriviallyMoveConstructible.h"
#include "../Traits/IsConvertible.h"
#include "../Traits/IsComparableWithEqual.h"
#include "../Traits/IsComparableWithNotEqual.h"
#include "../Traits/IsComparableWithLess.h"
#include "../Traits/IsComparableWithGreater.h"
#include "../Traits/IsComparableWithLessEqual.h"
#include "../Traits/IsComparableWithGreaterEqual.h"
#include "../Traits/IsNotSame.h"
#include "../Traits/IsScalar.h"
#include "../Traits/IsObject.h"
#include "../Traits/RemoveCVRef.h"
#include "../Traits/Decay.h"
#include "../Traits/And.h"
#include "../Traits/Not.h"

namespace hud {

    namespace details {

        /**
        * Contains the tagged union of the possible value.
        * @tparam T The OptionalDestructibleBase that inherit this class.
        * @tparam ValueType The type to store
        */
        template<typename T, typename ValueType>
        struct OptionalStorage {

            /** Retrieves this as a OptionalDestructibleBase */
            constexpr T* this_T() noexcept {
                return static_cast<T*>(this);
            }

            /** Retrieves this as a OptionalDestructibleBase */
            constexpr const T* this_T() const noexcept {
                return static_cast<const T*>(this);
            }

            /** Call destructor on the contained value and mark the storage without value */
            constexpr void reset() noexcept {
               if (this_T()->some) {
                    Memory::destroy(this_T()->some_value);
                    this_T()->some = false;
                }
            }

            /** Checks if it contains a value */
            [[nodiscard]]
            constexpr bool has_value() const noexcept {
                return this_T()->some;
            }

            /** Retrivies the contained value */
            [[nodiscard]]
            constexpr ValueType& value() & noexcept {
                check(has_value());
                return this_T()->some_value;
            }

            /** Retrivies the contained value */
            [[nodiscard]]
            constexpr const ValueType& value() const& noexcept {
                check(has_value());
                return this_T()->some_value;
            }

            /** Retrivies the contained value */
            [[nodiscard]]
            constexpr const ValueType&& value() const&& noexcept {
                check(has_value());
                return hud::move(this_T()->some_value);
            }

            /** Retrivies the contained value */
            [[nodiscard]]
            constexpr ValueType&& value() && noexcept {
                check(has_value());
                return hud::move(this_T()->some_value);
            }

            /**
            * Construct the value with the given parameter.
            * @tparam ...U Types of ...args
            * @param ...args Parameters forwarded to the value constructor
            */
            template<typename... U>
            constexpr void construct_in_place(U&&... args) noexcept {
                Memory::construct_at(&this_T()->some_value, hud::forward<U>(args)...);
                this_T()->some = true;
            }

            /** 
            * Assign another Optional to this. 
            * If *this and other does not contain a value, do nothing.
            * If *this and other contain a value, assign the value by forwarding the value of other in *this value.
            * If *this contains a value, but other does, the contained value is destroyed, and the Optional does not contains a value after this call.
            * If *this does not contain a value, but other do, the value is constructed by forwarding the value of other in *this value.
            * @tparam U Type of other
            * @param other The Optional to assign
            */
            template<typename U>
            constexpr void assign(U&& other) noexcept {
                if (has_value() == other.has_value()) {
                    if (has_value()) {
                        value() = hud::forward<U>(other).value();
                    }
                }
                else {
                    if (has_value()) {
                        reset();
                    }
                    else {
                        construct_in_place(hud::forward<U>(other).value());
                    }
                }
            }
        };

        /**
        * Destructor base of the Optional. If the inner type is trivially destructible, the Optional is trivially destructible.
        * @tparam T Inner type
        */
        template< typename T, bool = IsTriviallyDestructibleV<T>>
        struct OptionalDestructibleBase
            : OptionalStorage<OptionalDestructibleBase<T>, T>
        {
            /** By default Optional do not contains value */
            constexpr OptionalDestructibleBase() noexcept
                : none_value()
                , some(false) {
            }

            /**
            * Construct an Optional that contains a value. 
            * The Value is construct in place.
            * @tparam ...TArgs Types of args
            * @param TagInPlace tag used to select this constructor
            * @param args List of args forwarded to the constructed value
            */
            template<typename... TArgs>
            constexpr explicit OptionalDestructibleBase(TagInPlace, TArgs&&... args) noexcept requires(IsConstructibleV<T, TArgs...>)
                : some_value(hud::forward<TArgs>(args)...)
                , some(true) {
            }

            /** Aligned value in union that contains the value or nothing (Empty struct)*/
            struct Empty {};
            union alignas(alignof(T)) {
                Empty none_value;
                T some_value;
            };

            /**  True if storage contains a value, false otherwise */
            bool some = false;
        };

        /**
        * Destructor base of the Optional. If the inner type is trivially destructible, the Optional is trivially destructible.
        * @tparam T Inner type
        */
        template< typename T >
        struct OptionalDestructibleBase<T, false>
            : OptionalStorage<OptionalDestructibleBase<T>, T>
        {
            /** By default Optional do not contains value */
            constexpr OptionalDestructibleBase() noexcept
                : none_value()
                , some(false) {
            }

            /**
            * Construct an Optional that contains a value.
            * The Value is construct in place.
            * @tparam ...TArgs Types of args
            * @param TagInPlace tag used to select this constructor
            * @param args List of args forwarded to the constructed value
            */
            template<typename... TArgs>
            constexpr explicit OptionalDestructibleBase(TagInPlace, TArgs&&... args) noexcept requires(IsConstructibleV<T, TArgs...>)
                : some_value(hud::forward<TArgs>(args)...)
                , some(true) {
            }

            /** Call the destructor of inner value if this has value */
            constexpr ~OptionalDestructibleBase() noexcept {
                if (some) {
                    Memory::destroy(some_value);
                }
            }

            /** Aligned value in union that contains the value or nothing (Empty struct)*/
            struct Empty {};
            union alignas(alignof(T)) {
                Empty none_value;
                T some_value;
            };

            /**  True if storage contains a value, false otherwise */
            bool some = false;
        };

        /**
        * Copy constructor base of the Optional. If the inner type is trivially copy constructible, the Optional is trivially copy constructible.
        * @tparam T Inner type 
        */
        template< typename T, bool = IsTriviallyCopyConstructibleV<T>>
        struct OptionalCopyBase
            : OptionalDestructibleBase<T> {
            using Super = OptionalDestructibleBase<T>;
            using Super::Super;
        };
        template<typename T>
        struct OptionalCopyBase<T, false>
            : OptionalDestructibleBase<T> {

            using Super = OptionalDestructibleBase<T>;
            using Super::Super;

            constexpr OptionalCopyBase() = default;
            constexpr OptionalCopyBase(const OptionalCopyBase& other) noexcept {
                if (other.some) {
                    this->construct_in_place(other.some_value);
                }
            }
            constexpr OptionalCopyBase(OptionalCopyBase&&) = default;
            constexpr OptionalCopyBase& operator=(const OptionalCopyBase&) = default;
            constexpr OptionalCopyBase& operator=(OptionalCopyBase&&) = default;
        };


        /**
        * Move constructor base of the Optional. If the inner type is trivially move constructible, the Optional is trivially move constructible.
        * @tparam T Inner type
        */
        template<typename T, bool = IsTriviallyMoveConstructibleV<T>>
        struct OptionalMoveBase
            : OptionalCopyBase<T> {
            using Super = OptionalCopyBase<T>;
            using Super::Super;
        };

        template<typename T>
        struct OptionalMoveBase<T, false>
            : OptionalCopyBase<T> {
            using Super = OptionalCopyBase<T>;
            using Super::Super;

            constexpr OptionalMoveBase() = default;
            constexpr OptionalMoveBase(OptionalMoveBase&& other) noexcept {
                if (other.some) {
                    Memory::construct_at(&(this->some_value), hud::forward<T>(other.some_value));
                    this->some = true;
                }
            }
            constexpr OptionalMoveBase(const OptionalMoveBase&) = default;
            constexpr OptionalMoveBase& operator=(const OptionalMoveBase&) = default;
            constexpr OptionalMoveBase& operator=(OptionalMoveBase&&) = default;
        };

        /**
        * Copy assign operator base of the Optional. If the inner type is trivially copy assignable, the Optional is trivially copy assignable.
        * If the inner type is neither copy assignable, the Optional is explictly not copy assignable.
        * @tparam T Inner type
        */
        template<typename T,
            bool IsCopyAssignTrivial = IsTriviallyCopyConstructibleV<T> && IsTriviallyCopyAssignableV<T> && IsTriviallyDestructibleV<T>,
            bool IsCopyAssignDeleted = IsCopyConstructibleV<T>&& IsCopyAssignableV<T>>
        struct OptionalCopyAssignBase
            : OptionalMoveBase<T> {
            using Super = OptionalMoveBase<T>;
            using Super::Super;
        };
        template<typename T>
        struct OptionalCopyAssignBase<T, false, true> // Neither trivial or deleted
            : OptionalMoveBase<T> {
            using Super = OptionalMoveBase<T>;
            using Super::Super;
            constexpr OptionalCopyAssignBase() = default;
            constexpr OptionalCopyAssignBase(const OptionalCopyAssignBase&) = default;
            constexpr OptionalCopyAssignBase(OptionalCopyAssignBase&&) = default;
            constexpr OptionalCopyAssignBase& operator=(const OptionalCopyAssignBase& other) noexcept {
                this->assign(other);
                return *this;
            };
            constexpr OptionalCopyAssignBase& operator=(OptionalCopyAssignBase&&) = default;
        };
        template<typename T, bool IsCopyAssignTrivial>
        struct OptionalCopyAssignBase<T, IsCopyAssignTrivial, false> // deleted
            : OptionalMoveBase<T> {
            using Super = OptionalMoveBase<T>;
            using Super::Super;
            constexpr OptionalCopyAssignBase() = default;
            constexpr OptionalCopyAssignBase(const OptionalCopyAssignBase&) = default;
            constexpr OptionalCopyAssignBase(OptionalCopyAssignBase&&) = default;
            constexpr OptionalCopyAssignBase& operator=(const OptionalCopyAssignBase&) = delete;
            constexpr OptionalCopyAssignBase& operator=(OptionalCopyAssignBase&&) = default;
        };

        /**
        * Move assign operator base of the Optional. If the inner type is trivially move assignable, the Optional is trivially move assignable.
        * If the inner type is neither move assignable, the Optional is explictly not move assignable.
        * @tparam T Inner type
        */
        template<typename T,
            bool IsMoveAssignTrivial = IsTriviallyMoveAssignableV<T> && IsTriviallyMoveConstructibleV<T> && IsTriviallyDestructibleV<T>,
            bool IsMoveAssignDeleted = IsMoveConstructibleV<T>&& IsMoveAssignableV<T>>
        struct OptionalMoveAssignBase
            : OptionalCopyAssignBase<T> {
            using Super = OptionalCopyAssignBase<T>;
            using Super::Super;
        };
        template<typename T>
        struct OptionalMoveAssignBase<T, false, true> // Neither trivial or deleted
            : OptionalCopyAssignBase<T> {
            using Super = OptionalCopyAssignBase<T>;
            using Super::Super;
            constexpr OptionalMoveAssignBase() = default;
            constexpr OptionalMoveAssignBase(const OptionalMoveAssignBase&) = default;
            constexpr OptionalMoveAssignBase(OptionalMoveAssignBase&&) = default;
            constexpr OptionalMoveAssignBase& operator=(const OptionalMoveAssignBase&) = default;
            constexpr OptionalMoveAssignBase& operator=(OptionalMoveAssignBase&& other) noexcept {
                this->assign(hud::forward<OptionalMoveAssignBase>(other));
                return *this;
            };
        };
        template<typename T, bool IsMoveAssignTrivial>
        struct OptionalMoveAssignBase<T, IsMoveAssignTrivial, false> // deleted
            : OptionalCopyAssignBase<T> {
            using Super = OptionalCopyAssignBase<T>;
            using Super::Super;
            constexpr OptionalMoveAssignBase() = default;
            constexpr OptionalMoveAssignBase(const OptionalMoveAssignBase&) = default;
            constexpr OptionalMoveAssignBase(OptionalMoveAssignBase&&) = default;
            constexpr OptionalMoveAssignBase& operator=(const OptionalMoveAssignBase&) = default;
            constexpr OptionalMoveAssignBase& operator=(OptionalMoveAssignBase&&) = delete;
        };


        /** Hidden implementation of the Optional. */
        template<typename T>
        struct OptionalImpl : OptionalMoveAssignBase<T> {
            using Super = OptionalMoveAssignBase<T>;
            using Super::Super;

            constexpr OptionalImpl() = default;
            constexpr OptionalImpl(const OptionalImpl&) = default;
            constexpr OptionalImpl(OptionalImpl&&) = default;
            constexpr OptionalImpl& operator=(const OptionalImpl&) = default;
            constexpr OptionalImpl& operator=(OptionalImpl&&) = default;
        };

    } // details

    /** Empty class type used to indicate Optional type with uninitialized state */
    struct NullOpt {
        struct Tag {};
        explicit constexpr NullOpt(Tag) {}
    };

    /** Constant used to indicate Optional type with uninitialized state. */
    inline constexpr NullOpt nullopt{ NullOpt::Tag{} };


    /**
    * Manages an optional contained value, i.e. a value that may or may not be present. 
    * If an Optional<T> contains a value, the value is guaranteed to be allocated as part of the optional object footprint, i.e. no dynamic memory allocation ever takes place.
    * The optional object contains a value in the following conditions:
    *  - The object is initialized with/assigned from a value of type T or another optional that contains a value. 
    * The object does not contain a value in the following conditions:
    *  - The object is default-initialized.
    *  - The object is initialized with/assigned from a value of type hud::nullopt or an optional object that does not contain a value.
    *  - The member function reset() is called. 
    */
    template<typename T>
    class Optional : details::OptionalImpl<T> {
        using Super = details::OptionalImpl<T>;

        static_assert(!IsReferenceV<T>, "Optional<T&> is ill-formed");
        static_assert(IsDestructibleV<T>, "Optional<T> must meet the requirements of destructible type (t.~T() should be valid.");

    public:
        /** Default construct an Optional that does not contain a value */
        constexpr Optional() noexcept = default;

        /** Copy construct the Optional. Copy the inner value if presents. */
        constexpr Optional(const Optional&) noexcept = default;

        /** Move construct the Optional. Move the inner value if presents. Does not make other empty. */
        constexpr Optional(Optional&&) noexcept = default;

        /**
        * Construct an Optional that does not contain a value
        * @param NullOptT nullopt tag that indicate Optional type with uninitialized state
        */
        constexpr Optional(NullOpt) noexcept
        {};

        /**
        * Construct an Optional that contain a in-place constructed T value
        * @param TagInPlace tag used to select this constructor
        * @tparam TArgs The T constructor arguments
        * @param args Arguments to forward to the constructor of the value
        */
        template<typename... TArgs>
        constexpr explicit Optional(TagInPlace, TArgs&&... args) noexcept requires(IsConstructibleV<T, TArgs...>)
            : Super(TagInPlace{}, hud::forward<TArgs>(args)...) {
        }

        /**
        * Constructs an optional object that contains a value
        * @tparam U Type of value
        * @param value The value to move
        */
        template<typename U = T>
        explicit(!IsConvertibleV<U&&, T>) constexpr Optional(U&& value) noexcept requires(IsConstructibleV<T, U&&>&& IsNotSameV<RemoveCVRefT<U>, TagInPlace>&& IsNotSameV<U, Optional<T>>)
            : Super(TagInPlace{}, hud::move(value)) {
        }

        /**
        * If the Optional contains a value, destroy that value.
        * Otherwise do nothing.
        * This does not contain a value after this call.
        * @param NullOpt The nullopt tag
        */
        constexpr Optional& operator=(NullOpt) noexcept {
            reset();
            return *this;
        }

        /** 
        * If the Optional contains a value, copy assign of the contained value.
        * Otherwise do nothing.
        */
        constexpr Optional& operator=(const Optional&) noexcept = default;

        /**
        * If the Optional contains a value, move assign of the contained value.
        * Otherwise do nothing.
        * Does not make other empty. 
        */
        constexpr Optional& operator=(Optional&&) noexcept = default;
    

        /*
        * Depending on whether *this contains a value before the call, the contained value is either direct-initialized from hud::forward<U>(value) or assigned from hud::forward<U>(value).
        * @param value The value to perfect-forward assign
        * @return *this
        */
        template<typename U = T>
        constexpr Optional& operator=(U&& value) noexcept
            requires(AndV<
                IsNotSame<RemoveCVRefT<U>, Optional<T>>,
                IsConstructible<T, U>,
                IsAssignable<T&, U>,
                Not<And<IsScalar<T>,IsSame<DecayT<U>, T>>>
                >) {
            if (has_value()) {
                this->some_value = hud::forward<U>(value);
            }
            else {
                this->construct_in_place(hud::forward<U>(value));
            }
            return *this;
        }

        /**
        * Copy assign another Optional to this.
        * If *this and other does not contain a value, do nothing.
        * If *this and other contain a value, assign *this value with other values.
        * If *this contains a value, but other does, the contained value is destroyed, and the Optional does not contains a value after this call.
        * If *this does not contain a value, but other do, the value is constructed *this value with other values.
        * @tparam U Type of other
        * @param other The Optional to assign
        */
        template<typename U>
        constexpr Optional& operator=(const Optional<U>&other) noexcept
            requires((IsCopyConstructibleV<T, U> && IsCopyAssignableV<T, U>) &&
                !(
                    IsConstructibleV<T, Optional<U>&> ||
                    IsConstructibleV<T, const Optional<U>&> ||
                    IsConstructibleV<T, Optional<U>&&> ||
                    IsConstructibleV < T, const Optional<U>&&> ||
                    IsConvertibleV< Optional<U>&, T> ||
                    IsConvertibleV<const Optional<U>&, T> ||
                    IsConvertibleV<Optional<U>&&, T> ||
                    IsConvertibleV<const Optional<U>&&, T> ||
                    IsAssignableV<T&, Optional<U>&> ||
                    IsAssignableV<T&, const Optional<U>&> ||
                    IsAssignableV<T&, Optional<U>&&> ||
                    IsAssignableV<T&, const Optional<U>&&>
                    )) {
            this->assign(other);
            return *this;
        }

        /**
        * Move sassign another Optional to this.
        * If *this and other does not contain a value, do nothing.
        * If *this and other contain a value, assign the value by forwarding the value of other in *this value.
        * If *this contains a value, but other does, the contained value is destroyed, and the Optional does not contains a value after this call.
        * If *this does not contain a value, but other do, the value is constructed by forwarding the value of other in *this value.
        * @tparam U Type of other
        * @param other The Optional to assign
        */
        template<typename U>
        constexpr Optional& operator=(Optional<U> && other) noexcept
            requires(
                AndV<
                    IsConstructible<T, U>, IsAssignable<T&, U>,
                    Not<Or<
                        IsConstructible<T, Optional<U>&>,
                        IsConstructible<T, const Optional<U>&>,
                        IsConstructible<T, Optional<U>&&>,
                        IsConstructible < T, const Optional<U>&&>,
                        IsConvertible< Optional<U>&, T>,
                        IsConvertible<const Optional<U>&, T>,
                        IsConvertible<Optional<U>&&, T>,
                        IsConvertible<const Optional<U>&&, T>,
                        IsAssignable<T&, Optional<U>&>,
                        IsAssignable<T&, const Optional<U>&>,
                        IsAssignable<T&, Optional<U>&&>,
                        IsAssignable<T&, const Optional<U>&&>
                    >>
                >) {
            this->assign(hud::forward<Optional<U>>(other));
            return *this;
        }

        /** Checks whether the Optional contains a value or not */
        [[nodiscard]]
        constexpr bool has_value() const noexcept {
            return Super::has_value();
        }

        /** Retrieves a const l-value reference to the contained value */
        [[nodiscard]]
        constexpr T& value() & noexcept {
            return Super::value();
        }

        /** Retrieves a const l-value reference to the contained value */
        [[nodiscard]]
        constexpr const T& value() const& noexcept {
            return Super::value();
        }

        /** Retrieves a const l-value reference to the contained value */
        [[nodiscard]]
        constexpr const T&& value() const&& noexcept {
            return move(Super::value());
        }

        /** Retrieves a const l-value reference to the contained value */
        [[nodiscard]]
        constexpr T&& value() && noexcept {
            return move(Super::value());
        }

        /** 
        * If the Optional contains a value, destroy that value.
        * Otherwise do nothing. 
        * This does not contain a value after this call. 
        */
        constexpr void reset() noexcept {
            Super::reset();
        }

        /**
        Construct the contained value in-place.
        If this contain a value the contained value is destroyed by calling its destructor before constructing the value.
        @tparam ...TArgs Types of args
        @param args Arguments to forward to the constructor of the value
        */
        template<typename... TArgs>
        constexpr T& emplace(TArgs&&... args) noexcept {
            reset();
            this->construct_in_place(hud::forward<TArgs>(args)...);
            return value();
        }

        /** Checks whether the Optional contains a value or not */
        [[nodiscard]]
        constexpr explicit operator bool() const noexcept {
            return has_value();
        }

        /** Retrieves a reference to the contained value if Optional contains a value, return the default_value otherwise */
        template<typename U>
        [[nodiscard]]
        constexpr T value_or(U&& default_value) const& noexcept {
            static_assert(IsCopyConstructibleV<T>, "Optional<T>::value_or: T must be copy constructible");
            static_assert(IsConvertibleV<U, T>, "Optional<T>::value_or: U must be convertible to T");
            return has_value() ? value() : static_cast<T>(hud::forward<U>(default_value));
        }

        /** Retrieve a reference to the contained value if Optional contains a value, return the default_value otherwise */
        template<typename U>
        [[nodiscard]]
        constexpr T value_or(U&& default_value) && noexcept {
            static_assert(IsCopyConstructibleV<T>, "Optional<T>::value_or: T must be copy constructible");
            static_assert(IsConvertibleV<U, T>, "Optional<T>::value_or: U must be convertible to T");
            return has_value() ? move(value()) : static_cast<T>(hud::forward<U>(default_value));
        }

        /** Retrieves a pointer to the contained value */
        [[nodiscard]]
        constexpr T* operator->() noexcept {
            check(has_value());
            return &value();
        }

        /** Retrieves a pointer to the contained value */
        [[nodiscard]]
        constexpr const T* operator->() const noexcept {
            check(has_value());
            return &value();
        }

        /** Retrieves a reference to the contained value */
        [[nodiscard]]
        constexpr T& operator*() noexcept {
            check(has_value());
            return value();
        }

        /** Retrieves a reference to the contained value */
        [[nodiscard]]
        constexpr const T& operator*() const noexcept {
            check(has_value());
            return value();
        }


        /** Swap with another Optional */
        constexpr void swap(Optional& other) noexcept {
            if (has_value() == other.has_value()) {
                if (has_value()) {
                    hud::swap(value(), other.value());
                }
            }
            else {
                const bool this_has_value = has_value();
                // Move the Optional that contains value to the Optional that do not contains value then destroy the first one
                Optional& empty = this_has_value ? other : *this;
                Optional& non_empty = this_has_value ? *this : other;
                empty.construct_in_place(move(non_empty.value()));
                non_empty.reset();
            }
        }
    };

    /**
    * Swap first Optional with the second Optional
    * Same as first.swap(second)
    * @tparam T The element type of both Optional
    * @param first The first Optional to swap
    * @param second The second Optional to swap
    */
    template<typename T>
    constexpr void swap(Optional<T>& first, Optional<T>& second) noexcept {
        first.swap(second);
    }

    /**
    * Checks whether right and left Optional are equals.
    * Optionals are equals if both contained or not a value and if contained values are equals.
    * Value types must be comparable with operator==()
    * @tparam Left Value type of the left Optional
    * @tparam Right Value type of the right Optional
    * @param left The left Optional to compare
    * @param right The right Optional to compare
    * @param true if right and left Optional are equals, false otherwise
    */
    template<typename Left, typename Right>
    [[nodiscard]]
    constexpr bool operator==(const Optional<Left>& left, const Optional<Right>& right) noexcept requires(IsComparableWithEqualV<Left, Right>) {
        const bool left_has_value = left.has_value();
        return left_has_value == right.has_value() && (!left_has_value || left.value() == right.value());
    }


    /**
    * Checks whether right and left Optional are not equals.
    * Optionals are not equals if at least one Optional don't contains a value or if both contains a value, checks whether both values are not equals
    * Value types must be comparable with operator!=()
    * @tparam Left Value type of the left Optional
    * @tparam Right Value type of the right Optional
    * @param left The left Optional to compare
    * @param right The right Optional to compare
    * @param true if right and left Optional are not equals, false otherwise
    */
    template<typename Left, typename Right>
    [[nodiscard]]
    constexpr bool operator!=(const Optional<Left>& left, const Optional<Right>& right) noexcept requires(IsComparableWithNotEqualV<Left, Right>) {
        const bool left_has_value = left.has_value();
        return left_has_value != right.has_value() || (left_has_value && left.value() != right.value());
    }

    /**
    * Checks whether right is less than left Optional.
    * An right is less than left if :
    *     - right don't contains a value while left do, or,
    *     - both contains a value and right value is less than left value.
    * Value types must be comparable with operator<()
    * @tparam Left Value type of the left Optional
    * @tparam Right Value type of the right Optional
    * @param left The left Optional to compare
    * @param right The right Optional to compare
    * @param true if right is less than left Optional, false otherwise
    */
    template<typename Left, typename Right>
    [[nodiscard]]
    constexpr bool operator<(const Optional<Left>& left, const Optional<Right>& right) noexcept requires(IsComparableWithLessV<Left, Right>) {
        return right.has_value() && (!left.has_value() || left.value() < right.value());
    }

    /**
    * Checks whether right is greater than left Optional.
    * An right is greater than left if :
    *     - right contains a value while left don't, or,
    *     - both contains a value and right value is greater than left value.
    * Value types must be comparable with operator>()
    * @tparam Left Value type of the left Optional
    * @tparam Right Value type of the right Optional
    * @param left The left Optional to compare
    * @param right The right Optional to compare
    * @param true if right is greater than left Optional, false otherwise
    */
    template<typename Left, typename Right>
    [[nodiscard]]
    constexpr bool operator>(const Optional<Left>& left, const Optional<Right>& right) noexcept requires(IsComparableWithGreaterV<Left, Right>) {
        return left.has_value() && (!right.has_value() || left.value() > right.value());
    }

    /**
    * Checks whether right is less or equal than left Optional.
    * An right is less or equal than left if :
    *     - right don't contains a value while left do, or,
    *     - both contains a value and right value is less or equal than left value.
    * Value types must be comparable with operator<=()
    * @tparam Left Value type of the left Optional
    * @tparam Right Value type of the right Optional
    * @param left The left Optional to compare
    * @param right The right Optional to compare
    * @param true if right is less or equal than left Optional, false otherwise
    */
    template<typename Left, typename Right>
    [[nodiscard]]
    constexpr bool operator<=(const Optional<Left>& left, const Optional<Right>& right) noexcept requires(IsComparableWithLessEqualV<Left, Right>) {
        return !left.has_value() || (right.has_value() && left.value() <= right.value());
    }

    /**
    * Checks whether right is greater or equal than left Optional.
    * An right is greater or equal than left if :
    *     - right contains a value while left don't, or,
    *     - both contains a value and right value is greater or equal than left value.
    * Value types must be comparable with operator>=()
    * @tparam Left Value type of the left Optional
    * @tparam Right Value type of the right Optional
    * @param left The left Optional to compare
    * @param right The right Optional to compare
    * @param true if right is greater or equal than left Optional, false otherwise
    */
    template<typename Left, typename Right>
    [[nodiscard]]
    constexpr bool operator>=(const Optional<Left>& left, const Optional<Right>& right) noexcept requires(IsComparableWithGreaterEqualV<Left, Right>) {
        return !right.has_value() || (left.has_value() && left.value() >= right.value());
    }


    /**
    * Checks whether an Optional value is equal to a value
    * Optional is equal to a value if :
    *     - the Optional contain a value and the contained value is equal the compared value.
    * Value types must be comparable with operator==()
    * @tparam TOption Value type of the Optional
    * @tparam TValue Type of the value
    * @param option The Optional to compare
    * @param value The value to compare
    * @param true if an Optional value is equal to a value, false otherwise
    */
    template<typename TOption, typename TValue>
    [[nodiscard]]
    constexpr bool operator==(const Optional<TOption>& option, const TValue& value) noexcept requires(IsComparableWithEqualV<TOption, TValue>) {
        return option.has_value() ? option.value() == value : false;
    }

    /**
    * Checks whether a value is equal to an Optional
    * A value is equal to a Optional if :
    *     - the Optional contain a value and the value is equal the Optional value.
    * Value types must be comparable with operator==()
    * @tparam TOption Value type of the Optional
    * @tparam TValue Type of the value
    * @param option The Optional to compare
    * @param value The value to compare
    * @param true if a value is equal to an Optional, false otherwise
    */
    template<typename TOption, typename TValue>
    [[nodiscard]]
    constexpr bool operator==(const TValue& value, const Optional<TOption>& option) noexcept requires(IsComparableWithEqualV<TValue, TOption>) {
        return option.has_value() ? value == option.value() : false;
    }


    /**
    * Checks whether an Optional value is not equal to a value
    * Optional is not equal to a value if:
    *     - the Optional do not contain a value and the Optional value is not equal the compared value, or,
    *     - the Optional do not have a value
    * Value types must be comparable with operator!=()
    * @tparam TOption Value type of the Optional
    * @tparam TValue Type of the value
    * @param option The Optional to compare
    * @param value The value to compare
    * @param true if an Optional value is not equal to a value, false otherwise
    */
    template<typename TOption, typename TValue>
    [[nodiscard]]
    constexpr bool operator!=(const Optional<TOption>& option, const TValue& value) noexcept requires(IsComparableWithNotEqualV<TOption, TValue>) {
        return option.has_value() ? option.value() != value : true;
    }

    /**
    * Checks whether a value is not equal an Optional
    * A value is not equal to an Optional if:
    *     - the Optional do not contain a value and the Optional value is not equal the compared value, or,
    *     - the Optional do not have a value
    * Value types must be comparable with operator!=()
    * @tparam TOption Value type of the Optional
    * @tparam TValue Type of the value
    * @param option The Optional to compare
    * @param value The value to compare
    * @param true if a value is not equal an Optional, false otherwise
    */
    template<typename TOption, typename TValue>
    [[nodiscard]]
    constexpr bool operator!=(const TValue& value, const Optional<TOption>& option) noexcept requires(IsComparableWithNotEqualV<TValue, TOption>) {
        return option.has_value() ? value != option.value() : true;
    }

    /**
    * Checks whether an Optional value is less than a value
    * Optional is less to a value if:
    *     - the Optional contains a value and the Optional value is less than the compared value, or,
    *     - the Optional do not contain a value
    * Value types must be comparable with operator<()
    * @tparam TOption Value type of the Optional
    * @tparam TValue Type of the value
    * @param option The Optional to compare
    * @param value The value to compare
    * @param true if an Optional value is less than a value, false otherwise
    */
    template<typename TOption, typename TValue>
    [[nodiscard]]
    constexpr bool operator<(const Optional<TOption>& option, const TValue& value) noexcept requires(IsComparableWithLessV<TOption, TValue>) {
        return option.has_value() ? option.value() < value : true;
    }

    /**
    * Checks whether a value is less than an Optional
    * A value is less than an Optional if:
    *     - the Optional contain a value and the value is less than the Optional value
    * Value types must be comparable with operator<()
    * @tparam TOption Value type of the Optional
    * @tparam TValue Type of the value
    * @param option The Optional to compare
    * @param value The value to compare
    * @param true if an Optional value is less than a value, false otherwise
    */
    template<typename TOption, typename TValue>
    [[nodiscard]]
    constexpr bool operator<(const TValue& value, const Optional<TOption>& option) noexcept requires(IsComparableWithLessV<TValue, TOption>) {
        return option.has_value() ? value < option.value() : false;
    }

    /**
    * Checks whether an Optional value is greater than a value
    * Optional is greater than a value if:
    *     - the Optional contain a value and the contained value is greater than the compared value
    * Value types must be comparable with operator>()
    * @tparam TOption Value type of the Optional
    * @tparam TValue Type of the value
    * @param option The Optional to compare
    * @param value The value to compare
    * @param true if an Optional value is greater than a value, false otherwise
    */
    template<typename TOption, typename TValue>
    [[nodiscard]]
    constexpr bool operator>(const Optional<TOption>& option, const TValue& value) noexcept requires(IsComparableWithGreaterV<TOption, TValue>) {
        return option.has_value() ? option.value() > value : false;
    }

    /**
    * Checks whether a value is greater than an Optional
    * A value is greater than an Optional if:
    *     - the Optional contain a value and the contained value is greater than the compared value, or,
    *     - the Optional do not contain a value
    * Value types must be comparable with operator>()
    * @tparam TOption Value type of the Optional
    * @tparam TValue Type of the value
    * @param option The Optional to compare
    * @param value The value to compare
    * @param true if a value is greater than an Optional, false otherwise
    */
    template<typename TOption, typename TValue>
    [[nodiscard]]
    constexpr bool operator>(const TValue& value, const Optional<TOption>& option) noexcept requires(IsComparableWithGreaterV<TValue, TOption>) {
        return option.has_value() ? value > option.value() : true;
    }


    /**
    * Checks whether an Optional value is less or equal a value
    * Optional is less or equal to a value if:
    *     - the Optional contains a value and the Optional value is less or equal the compared value, or,
    *     - the Optional do not contain a value
    * Value types must be comparable with operator<=()
    * @tparam TOption Value type of the Optional
    * @tparam TValue Type of the value
    * @param option The Optional to compare
    * @param value The value to compare
    * @param true if an Optional value is less or equal to a value, false otherwise
    */
    template<typename TOption, typename TValue>
    [[nodiscard]]
    constexpr bool operator<=(const Optional<TOption>& option, const TValue& value) noexcept requires(IsComparableWithLessV<TOption, TValue>) {
        return option.has_value() ? option.value() <= value : true;
    }

    /**
    * Checks whether a value is less or equal an Optional
    * A value is less or equal than an Optional if:
    *     - the Optional contain a value and the value is less or equal the Optional value
    * Value types must be comparable with operator<=()
    * @tparam TOption Value type of the Optional
    * @tparam TValue Type of the value
    * @param option The Optional to compare
    * @param value The value to compare
    * @param true if an Optional value is less or equal to a value, false otherwise
    */
    template<typename TOption, typename TValue>
    [[nodiscard]]
    constexpr bool operator<=(const TValue& value, const Optional<TOption>& option) noexcept requires(IsComparableWithLessV<TValue, TOption>) {
        return option.has_value() ? value <= option.value() : false;
    }

    /**
    * Checks whether an Optional value is greater or equal a value
    * Optional is greater or equal to a value if:
    *     - the Optional contains a value and the Optional value is greater or equal the compared value
    * Value types must be comparable with operator>=()
    * @tparam TOption Value type of the Optional
    * @tparam TValue Type of the value
    * @param option The Optional to compare
    * @param value The value to compare
    * @param true if an Optional value is greater or equal to a value, false otherwise
    */
    template<typename TOption, typename TValue>
    [[nodiscard]]
    constexpr bool operator>=(const Optional<TOption>& option, const TValue& value) noexcept requires(IsComparableWithGreaterEqualV<TOption, TValue>) {
        return option.has_value() ? option.value() >= value : false;
    }

    /**
    * Checks whether a value is greater or equal an Optional
    * A value is less than an Optional if:
    *     - the Optional contain a value and the value is greater or equal the Optional value, or,
    *     - the Optional do not contain a value
    * Value types must be comparable with operator>=()
    * @tparam TOption Value type of the Optional
    * @tparam TValue Type of the value
    * @param option The Optional to compare
    * @param value The value to compare
    * @param true if an Optional value is greater or equal to a value, false otherwise
    */
    template<typename TOption, typename TValue>
    [[nodiscard]]
    constexpr bool operator>=(const TValue& value, const Optional<TOption>& option) noexcept requires(IsComparableWithGreaterEqualV<TValue, TOption>) {
        return option.has_value() ? value >= option.value() : true;
    }

} // namespace hud

#endif // HD_INC_OSLAYER_OPTIONAL_H
