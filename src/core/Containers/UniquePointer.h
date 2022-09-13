#pragma once
#ifndef HD_INC_OSLAYER_UNIQUE_POINTER_H
#define HD_INC_OSLAYER_UNIQUE_POINTER_H
#include "../Minimal.h"
#include "../Traits/VoidT.h"
#include "../Traits/RemoveReference.h"
#include "../Traits/EnableIf.h"
#include "../Traits/Or.h"
#include "../Traits/And.h"
#include "../Traits/Not.h"
#include "../Traits/IsPointer.h"
#include "../Traits/IsDefaultConstructible.h"
#include "../Traits/Conditional.h"
#include "../Traits/IsReference.h"
#include "../Traits/AddConst.h"
#include "../Traits/AddLValueReference.h"
#include "../Traits/IsConvertible.h"
#include "../Traits/IsArray.h"
#include "../Traits/IsSame.h"
#include "../Traits/IsMoveAssignable.h"
#include "../Traits/AddLValueReference.h"
#include "../Traits/CommonType.h"
#include "../Traits/IsArray.h"
#include "../Traits/IsBoundedArray.h"
#include "../Traits/RemoveAllExtents.h"
#include "../Traits/RemovePointer.h"
#include "../Traits/IsNullptr.h"
#include "../Traits/IsNothrowDefaultConstructible.h"
#include "../Traits/IsNothrowMoveConstructible.h"
#include "../Traits/IsNothrowCopyConstructible.h"
#include "../Traits/IsNothrowMoveAssignable.h"
#include "../Traits/IsUnboundedArray.h"

#include "../Templates/SelectDeleterPointerType.h"
#include "../Templates/DefaultDeleter.h"
#include "../Templates/Move.h"
#include "../Templates/Forward.h"
#include "../Templates/Swap.h"
#include "../Templates/Less.h"

#include "Tuple.h"
#include "../Hash.h"


namespace hud {

    template<typename T, typename Deleter = DefaultDeleter<T>>
    class UniquePointer;

    namespace details {

        /**
        * DeleterType parameter type.
        * If DeleterType is a reference, it's passed by value, else by const reference
        * @tparam DeleterType  The deleter type of the UniquePointer
        */
        template <typename DeleterType>
        using DeleterParamType = ConditionalT<IsReferenceV<DeleterType>, DeleterType, AddLValueReferenceT<AddConstT<DeleterType>>>;

        /**
        * Checks whether a UniquePointer<U, DeleterU> is convertible to UniquePointer<T, Deleter>.
        * Provides the member constant Value which is equal to true, if UniquePointer<U, DeleterU> is convertible to UniquePointer<T, Deleter>.
        * Otherwise, Value is equal to false.
        * @tparam UniquePointerT The UniquePointer<T>
        * @tparam UniquePointerU The UniquePointer<U>
        */
        template<typename UniquePointerT, typename UniquePointerU>
        struct IsUniquePointerConvertibleToUniquePointer
            : FalseType {
        };
        template<typename PtrT, typename DeleterT, typename U, typename DeleterU>
        struct IsUniquePointerConvertibleToUniquePointer<UniquePointer<PtrT, DeleterT>, UniquePointer<U, DeleterU>>
            : And <
            IsConvertible<typename UniquePointer<U, DeleterU>::PointerType, typename UniquePointer<PtrT, DeleterT>::PointerType >,
            Not<IsArray<U>>,
            ConditionalT<IsReferenceV<DeleterT>, IsSame<DeleterT, DeleterU>, IsConvertible<DeleterU, DeleterT>>
            > {
        };
        template<typename UniquePointerT, typename UniquePointerU>
        static constexpr bool IsUniquePointerConvertibleToUniquePointerV = IsUniquePointerConvertibleToUniquePointer<UniquePointerT, UniquePointerU>::Value;

        /**
        * Check whether a UniquePointer<U> is move assignable to UniquePointer<T> or not.
        * @tparam UniquePointerT The UniquePointer<T>
        * @tparam UniquePointerU The UniquePointer<U>
        */
        template<typename UniquePointerT, typename UniquePointerU>
        struct IsUniquePointerMoveAssignableToUniquePointer
            : FalseType {
        };
        template<typename PtrT, typename DeleterT, typename U, typename DeleterU>
        struct IsUniquePointerMoveAssignableToUniquePointer<UniquePointer<PtrT, DeleterT>, UniquePointer<U, DeleterU>>
            : And<
            IsConvertible<typename UniquePointer<U, DeleterU>::PointerType, typename UniquePointer<PtrT, DeleterT>::PointerType >,
            Not<IsArray<U>>,
            IsMoveAssignable<DeleterT, DeleterU>
            > {
        };
        template<typename UniquePointerT, typename UniquePointerU>
        static constexpr bool IsUniquePointerMoveAssignableToUniquePointerV = IsUniquePointerMoveAssignableToUniquePointer<UniquePointerT, UniquePointerU>::Value;

        /**
        * Check whether a U* can be used to reset a UniquePointer<T> or not.
        * @tparam UniquePointerT The UniquePointer<T[]>
        * @tparam PtrU The pointer U* used to reset UniquePointerT
        */
        template<typename UniquePointerT, typename PtrU>
        struct IsUniquePointerArrayCanBeResetWith
            : FalseType {
        };
        template<typename PtrT, typename DeleterT, typename U>
        struct IsUniquePointerArrayCanBeResetWith<UniquePointer<PtrT[], DeleterT>, U*>
            : Or <
            IsSame<U*, typename UniquePointer<PtrT[], DeleterT>::PointerType>,
            And<
            IsSame<typename UniquePointer<PtrT[], DeleterT>::PointerType, typename UniquePointer<PtrT[], DeleterT>::ElementType*>,
            IsConvertible<U(*)[], PtrT(*)[]>
            >
            > {
        };
        template<typename T, typename U>
        static constexpr bool IsUniquePointerArrayCanBeResetWithV = IsUniquePointerArrayCanBeResetWith<T, U>::Value;

        /**
        * Checks whether a U* is convertible to UniquePointer<T[], Deleter>.
        * Provides the member constant Value which is equal to true, if U* is convertible to UniquePointer<T, Deleter>.
        * Otherwise, Value is equal to false.
        * @tparam UniquePointerT The UniquePointer<T[]>
        * @tparam PtrU The pointer U* used to reset UniquePointerT
        */
        template<typename UniquePointerT, typename PtrU>
        struct IsPointerConvertibleToUniquePointerArray
            : Or< IsUniquePointerArrayCanBeResetWith<UniquePointerT, PtrU>, IsNullptr<PtrU>> {
        };
        template<typename UniquePointerT, typename PtrU>
        static constexpr bool IsPointerConvertibleToUniquePointerArrayV = IsPointerConvertibleToUniquePointerArray<UniquePointerT, PtrU>::Value;

        /**
        * Checks whether a UniquePointer<U> is convertible to an UniquePointer<T[]>
        * @tparam UniquePointerT The UniquePointer<T[]>
        * @tparam UniquePointerU The UniquePointer<U>
        */
        template<typename UniquePointerT, typename UniquePointerU>
        struct IsUniquePointerConvertibleToUniquePointerArray
            : FalseType {
        };
        template<typename PtrT, typename DeleterT, typename U, typename DeleterU>
        struct IsUniquePointerConvertibleToUniquePointerArray<UniquePointer<PtrT, DeleterT>, UniquePointer<U, DeleterU>>
            : And<
            IsArray<U>,
            IsSame<typename UniquePointer<PtrT, DeleterT>::ElementType*, typename UniquePointer<PtrT, DeleterT>::PointerType>,
            IsSame<typename UniquePointer<U, DeleterU>::ElementType*, typename UniquePointer<U, DeleterU>::PointerType>,
            IsConvertible<typename UniquePointer<U, DeleterU>::ElementType(*)[], typename UniquePointer<PtrT, DeleterT>::ElementType(*)[]>
            > {
        };

        /**
        * Checks whether a UniquePointer<T[]> is move constructible from UniquePointer<U>
        * @tparam UniquePointerT The UniquePointer<T[]>
        * @tparam UniquePointerU The UniquePointer<U>
        */
        template<typename UniquePointerT, typename UniquePointerU>
        struct IsUniquePointerArrayMoveConstructibleFromUniquePointer
            : FalseType {
        };
        template<typename PtrT, typename DeleterT, typename U, typename DeleterU>
        struct IsUniquePointerArrayMoveConstructibleFromUniquePointer<UniquePointer<PtrT, DeleterT>, UniquePointer<U, DeleterU>>
            : And <
            IsUniquePointerConvertibleToUniquePointerArray<UniquePointer<PtrT, DeleterT>, UniquePointer<U, DeleterU>>,
            ConditionalT<IsReferenceV<DeleterT>, IsSame<DeleterT, DeleterU>, IsConvertible<DeleterU, DeleterT>>
            > {
        };
        template<typename UniquePointerT, typename UniquePointerU>
        static constexpr bool IsUniquePointerArrayMoveConstructibleFromUniquePointerV = IsUniquePointerArrayMoveConstructibleFromUniquePointer<UniquePointerT, UniquePointerU>::Value;

        /**
        * Checks whether a UniquePointer<U> is move assignable to a UniquePointer<T[]>
        * @tparam UniquePointerT The UniquePointer<T[]>
        * @tparam UniquePointerU The UniquePointer<U>
        */
        template<typename UniquePointerT, typename UniquePointerU>
        struct IsUniquePointerMoveAssignableToUniquePointerArray
            : FalseType {
        };
        template<typename PtrT, typename DeleterT, typename U, typename DeleterU>
        struct IsUniquePointerMoveAssignableToUniquePointerArray<UniquePointer<PtrT, DeleterT>, UniquePointer<U, DeleterU>>
            : And<
            IsUniquePointerConvertibleToUniquePointerArray<UniquePointer<PtrT, DeleterT>, UniquePointer<U, DeleterU>>,
            IsMoveAssignable<DeleterT, DeleterU>
            > {
        };
        template<typename UniquePointerT, typename UniquePointerU>
        static constexpr bool IsUniquePointerMoveAssignableToUniquePointerArrayV = IsUniquePointerMoveAssignableToUniquePointerArray<UniquePointerT, UniquePointerU>::Value;

    }

    /**
    * UniquePointer is a smart pointer that owns and manages another object through a pointer.
    * UniquePointer object automatically delete the object they manage (using a deleter) as soon as they themselves are destroyed, or as soon as their value changes either by an assignment operation or by an explicit call to UniquePointer::reset.
    * @tparam T The type of the pointer to own
    * @tparam Deleter the deleter used to delete the T pointer
    */
    template<typename T, typename Deleter>
    class UniquePointer {

    public:

        /** The deleter type. */
        using DeleterType = Deleter;
        /** The pointer type. Result of SelectDeleterPointerTypeT<T,DeleterType>*/
        using PointerType = SelectDeleterPointerTypeT<T, DeleterType>;
        /** The element pointed by the pointer. */
        using ElementType = T;

        static_assert(!hud::IsRValueReferenceV<DeleterType>, "the specified deleter type cannot be an rvalue reference");
        static_assert(hud::IsConvertibleV<T*, PointerType>, "T is no convertible to DeleterType::PointerType");

    public:

        /**
        * Default constructor that value-initializes the stored pointer and the stored deleter.
        * UniquePointer do not accept throwable default constructible deleter.
        */
        HD_FORCEINLINE constexpr UniquePointer() noexcept requires(IsDefaultConstructibleV<DeleterType> && !IsPointerV<DeleterType>)
            : inner(taginit) {
            static_assert(IsNothrowDefaultConstructibleV<DeleterType>, "UniquePointer do not accept throwable default constructible Deleter");
        }

        /**
        * Construct a UniquePointer form a raw pointer to own.
        * UniquePointer do not accept throwable default constructible deleter
        * @param pointer The poiner to own
        */
        HD_FORCEINLINE constexpr explicit UniquePointer(PointerType pointer) noexcept requires(IsDefaultConstructibleV<DeleterType> && !IsPointerV<DeleterType>)
            : inner(pointer, taginit) {
            static_assert(IsNothrowDefaultConstructibleV<DeleterType>, "UniquePointer do not accept throwable default constructible Deleter");
        }

        /**
        * Construct a UniquePointer from a pointer to own and a deleter.
        * @param pointer The poiner to own
        * @param deleter The deleter used to delete the owned pointer
        */
        HD_FORCEINLINE constexpr UniquePointer(PointerType pointer, const DeleterType& deleter) noexcept requires(IsConstructibleV<DeleterType, const DeleterType& >)
            : inner(pointer, deleter) {
            static_assert(IsNothrowCopyConstructibleV<DeleterType>, "UniquePointer do not accept throwable copy constructible Deleter");
        }

        /**
        * Construct a UniquePointer from a pointer to own and a deleter.
        * @param pointer The poiner to own
        * @param deleter The deleter used to delete the owned pointer
        */
        HD_FORCEINLINE constexpr UniquePointer(PointerType pointer, DeleterType&& deleter) noexcept requires(!IsReferenceV<DeleterType>&& IsMoveConstructibleV<DeleterType>)
            : inner(pointer, move(deleter)) {
            static_assert(IsNothrowMoveConstructibleV<DeleterType>, "UniquePointer do not accept throwable move constructible Deleter");
        }

        /**
        * Construct a UniquePointer from a raw pointer to own and a reference to a deleter.
        * @param pointer The poiner to own
        * @param deleter A reference to a deleter
        */
        UniquePointer(PointerType pointer, RemoveReferenceT<DeleterType>&& deleter) noexcept requires(IsReferenceV<DeleterType>&& IsConstructibleV<DeleterType, RemoveReferenceT<DeleterType>>) = delete;


        /** Construct a UniquePointer from a nullptr */
        HD_FORCEINLINE constexpr UniquePointer(hud::ptr::null) noexcept requires(IsDefaultConstructibleV<DeleterType> && !IsPointerV<DeleterType>)
            : inner(taginit) {
            static_assert(IsNothrowDefaultConstructibleV<DeleterType>, "UniquePointer do not accept throwable default constructible Deleter");
        }

        /**
        * Construct a UniquePointer by stealing another UniquePointer ownership.
        * @param other The UniquePointer to transfert ownership from.
        */
        HD_FORCEINLINE constexpr UniquePointer(UniquePointer&& other) noexcept requires(IsMoveConstructibleV<DeleterType>)
            : inner(other.leak(), forward<DeleterType>(other.deleter())) {
            static_assert(IsReferenceV<DeleterType> ? IsNothrowMoveConstructibleV<DeleterType> : true, "UniquePointer do not accept throwable move constructible Deleter");
        }

        /**
        * Construct a UniquePointer by stealing another UniquePointer ownership.
        * @tparam U Type of the UniquePointer's pointer
        * @tparam DeleterU  Type of the UniquePointer's deleter
        * @param other The UniquePointer to transfert ownership from.
        */
        template<typename U, typename DeleterU>
        HD_FORCEINLINE constexpr UniquePointer(UniquePointer<U, DeleterU>&& other) noexcept requires(details::IsUniquePointerConvertibleToUniquePointerV<UniquePointer, UniquePointer<U, DeleterU>>)
            : inner(other.leak(), forward<DeleterU>(other.deleter())) {
            static_assert(IsNothrowMoveConstructibleV<DeleterType, DeleterU>, "UniquePointer do not accept throwable move constructible Deleter");
        }

        /**
        * Assigns the UniquePointer by stealing the ownership of the pointer.
        * @param other The UniquePointer to transfert ownership from.
        * @return *this
        */
        constexpr UniquePointer& operator=(UniquePointer&& other) noexcept requires(IsMoveAssignableV<DeleterType>) {
            static_assert(!IsReferenceV<DeleterType> ? IsNothrowMoveAssignableV<DeleterType> : true, "UniquePointer do not accept throwable move assignable Deleter if DeleterType is no a reference");
            static_assert(IsReferenceV<DeleterType> ? IsNothrowCopyAssignableV<RemoveReferenceT<DeleterType>> : true, "UniquePointer do not accept throwable copy assignable Deleter if DeleterType is reference)");

            reset(other.leak());
            deleter() = forward<DeleterType>(other.deleter());
            return *this;
        }

        /**
        * Assigns the UniquePointer by stealing the ownership of the pointer.
        * @tparam U Type of the other UniquePointer's pointer
        * @tparam DeleterU  Type of the other UniquePointer's deleter
        * @param other The UniquePointer to transfert ownership from.
        * @return *this
        */
        template<typename U, typename DeleterU>
        constexpr UniquePointer& operator=(UniquePointer<U, DeleterU>&& other) noexcept requires(details::IsUniquePointerMoveAssignableToUniquePointerV<UniquePointer, UniquePointer<U, DeleterU>>) {
            static_assert(!IsReferenceV<DeleterType> ? IsNothrowMoveAssignableV<DeleterType> : true, "UniquePointer do not accept throwable move assignable Deleter if DeleterType is no a reference");
            static_assert(IsReferenceV<DeleterType> ? IsNothrowCopyAssignableV<RemoveReferenceT<DeleterType>> : true, "UniquePointer do not accept throwable copy assignable Deleter if DeleterType is reference)");

            reset(other.leak());
            deleter() = forward<DeleterU>(other.deleter());
            return *this;
        }

        /**
        * Assigns the UniquePointer with a nullptr. Same as reset().
        * @param A nullptr
        * @return *this
        */
        HD_FORCEINLINE constexpr UniquePointer& operator=(hud::ptr::null) noexcept {
            reset();
            return *this;
        }

        /** Call the deleter on the pointer if any owned. */
        constexpr ~UniquePointer() noexcept {
            if (is_owning()) {
                deleter()(pointer());
            }
        }

        /** Retrieves the owned pointer. */
        [[nodiscard]]
        HD_FORCEINLINE constexpr PointerType pointer() const noexcept {
            return get<0>(inner);
        }

        /** Check whether the UniquePointer own a pointer. */
        [[nodiscard]]
        HD_FORCEINLINE constexpr bool is_owning() const noexcept {
            return pointer() != PointerType();
        }

        /** Check whether the UniquePointer own a pointer. */
        [[nodiscard]]
        HD_FORCEINLINE constexpr explicit operator bool() const noexcept {
            return is_owning();
        }

        /** Dereference owned pointer. */
        [[nodiscard]]
        HD_FORCEINLINE constexpr AddLValueReferenceT<ElementType> operator*() const noexcept {
            return *pointer();
        }

        /** Retrieves the owned pointer. */
        [[nodiscard]]
        HD_FORCEINLINE constexpr PointerType operator->() const noexcept {
            return pointer();
        }

        /** Retrieves the deleter. */
        [[nodiscard]]
        HD_FORCEINLINE constexpr DeleterType& deleter() noexcept {
            return get<1>(inner);
        }

        /** Retrieves a reference to the deleter. */
        [[nodiscard]]
        HD_FORCEINLINE constexpr const DeleterType& deleter() const noexcept {
            return get<1>(inner);
        }

        /** Release the ownership of the pointer and return it without calling the deleter on the owned pointer. */
        [[nodiscard]]
        constexpr PointerType leak() noexcept {
            PointerType old = pointer();
            get<0>(inner) = PointerType();
            return old;
        }

        /** Call the deleter on the owned pointer and optionally take ownership of a new pointer. */
        constexpr void reset(PointerType ptr) noexcept {
            PointerType old_ptr = pointer();
            get<0>(inner) = ptr;
            deleter()(old_ptr);
        }

        /** Call the deleter on the owned pointer and taking no ownership. */
        HD_FORCEINLINE constexpr void reset(hud::ptr::null) noexcept {
            deleter()(pointer());
            get<0>(inner) = PointerType();
        }

        /** Call the deleter on the owned pointer and taking no ownership. */
        HD_FORCEINLINE constexpr void reset() noexcept {
            reset(nullptr);
        }

        /** Swap with another UniquePointer. */
        HD_FORCEINLINE constexpr void swap(UniquePointer& other) noexcept {
            hud::swap(inner, other.inner);
        }

    private:
        /** Not copy constructible. */
        UniquePointer(const UniquePointer&) = delete;
        /** Not copyable. */
        UniquePointer& operator = (const UniquePointer&) = delete;

    private:
        /** Tuple containing the pointerand the deleter */
        Tuple< PointerType, DeleterType> inner;
    };

    /**
    * UniquePointer is a smart pointer that owns and manages another object through a pointer.
    * UniquePointer object automatically delete the object they manage (using a deleter) as soon as they themselves are destroyed, or as soon as their value changes either by an assignment operation or by an explicit call to UniquePointer::reset.
    */
    template<typename T, typename Deleter>
    class UniquePointer<T[], Deleter> {

    public:
        /** The deleter type. */
        using DeleterType = Deleter;
        /** The pointer type. Result of SelectDeleterPointerTypeT<T,DeleterType>*/
        using PointerType = SelectDeleterPointerTypeT<T, DeleterType>;
        /** The element pointed by the pointer. */
        using ElementType = T;

        static_assert(!hud::IsRValueReferenceV<DeleterType>, "the specified deleter type cannot be an rvalue reference");
        static_assert(hud::IsConvertibleV<T*, PointerType>, "T is no convertible to DeleterType::PointerType");

    public:

        /**
        * Default constructor that value-initializes the stored pointer and the stored deleter.
        * UniquePointer do not accept throwable default constructible deleter.
        */
        HD_FORCEINLINE constexpr UniquePointer() noexcept requires(IsDefaultConstructibleV<DeleterType> && !IsPointerV<DeleterType>)
            : inner(taginit) {
            static_assert(IsNothrowDefaultConstructibleV<DeleterType>, "UniquePointer do not accept throwable default constructible Deleter");
        }

        /**
        * Construct a UniquePointer form a raw pointer to own.
        * UniquePointer do not accept throwable default constructible deleter
        * @param pointer The poiner to own
        */
        template<typename U>
        HD_FORCEINLINE constexpr explicit UniquePointer(U pointer) noexcept requires(IsDefaultConstructibleV<DeleterType> && !IsPointerV<DeleterType> && details::IsPointerConvertibleToUniquePointerArrayV<UniquePointer, U>)
            : inner(pointer, taginit) {
            static_assert(IsNothrowDefaultConstructibleV<DeleterType>, "UniquePointer do not accept throwable default constructible Deleter");
        }

        /**
        * Construct a UniquePointer from a pointer to own and a deleter.
        * @param pointer The poiner to own
        * @param deleter The deleter used to delete the owned pointer
        */
        template<typename U>
        HD_FORCEINLINE constexpr UniquePointer(U pointer, const DeleterType& deleter) noexcept requires(IsConstructibleV<DeleterType, const DeleterType& > && details::IsPointerConvertibleToUniquePointerArrayV<UniquePointer, U>)
            : inner(pointer, deleter) {
            static_assert(IsNothrowCopyConstructibleV<DeleterType>, "UniquePointer do not accept throwable copy constructible Deleter");
        }

        /**
        * Construct a UniquePointer from a pointer to own and a deleter.
        * @param pointer The poiner to own
        * @param deleter The deleter used to delete the owned pointer
        */
        template<typename U>
        HD_FORCEINLINE constexpr UniquePointer(U pointer, DeleterType&& deleter) noexcept requires(!IsReferenceV<DeleterType> && IsMoveConstructibleV<DeleterType>&& details::IsPointerConvertibleToUniquePointerArrayV<UniquePointer, U>)
            : inner(pointer, move(deleter)) {
            static_assert(IsNothrowMoveConstructibleV<DeleterType>, "UniquePointer do not accept throwable move constructible Deleter");
        }

        /** Do not accept DeleterType is a reference will allow the move of it. */
        UniquePointer(PointerType pointer, RemoveReferenceT<DeleterType>&& deleter) noexcept requires(IsReferenceV<DeleterType>&& IsConstructibleV<DeleterType, RemoveReferenceT<DeleterType>>) = delete;


        /** Construct a UniquePointer from a nullptr. */ 
        HD_FORCEINLINE constexpr UniquePointer(hud::ptr::null) noexcept requires(IsDefaultConstructibleV<DeleterType> && !IsPointerV<DeleterType>)
            : inner(taginit) {
            static_assert(IsNothrowDefaultConstructibleV<DeleterType>, "UniquePointer do not accept throwable default constructible Deleter");
        }

        /**
        * Construct a UniquePointer by stealing another UniquePointer ownership.
        * @param other The UniquePointer to transfert ownership from.
        */
        HD_FORCEINLINE constexpr UniquePointer(UniquePointer&& other) noexcept requires(IsMoveConstructibleV<DeleterType>)
            : inner(other.leak(), forward<DeleterType>(other.deleter())) {
            static_assert(IsReferenceV<DeleterType> ? IsNothrowMoveConstructibleV<DeleterType> : true, "UniquePointer do not accept throwable move constructible Deleter");
        }

        /**
        * Construct a UniquePointer by stealing another UniquePointer ownership.
        * @tparam U Type of the UniquePointer's pointer
        * @tparam DeleterU  Type of the UniquePointer's deleter
        * @param other The UniquePointer to transfert ownership from.
        */
        template<typename U, typename DeleterU>
        HD_FORCEINLINE constexpr UniquePointer(UniquePointer<U, DeleterU>&& other) noexcept requires(details::IsUniquePointerArrayMoveConstructibleFromUniquePointerV<UniquePointer, UniquePointer<U, DeleterU>>)
            : inner(other.leak(), forward<DeleterU>(other.deleter())) {
            static_assert(IsNothrowMoveConstructibleV<DeleterType, DeleterU>, "UniquePointer do not accept throwable move constructible Deleter");
        }

        /**
        * Assigns the UniquePointer by stealing the ownership of the pointer.
        * @param other The UniquePointer to transfert ownership from.
        * @return *this
        */
        constexpr UniquePointer& operator=(UniquePointer&& other) noexcept requires(IsMoveAssignableV<DeleterType>) {
            static_assert(!IsReferenceV<DeleterType> ? IsNothrowMoveAssignableV<DeleterType> : true, "UniquePointer do not accept throwable move assignable Deleter if DeleterType is no a reference");
            static_assert(IsReferenceV<DeleterType> ? IsNothrowCopyAssignableV<RemoveReferenceT<DeleterType>> : true, "UniquePointer do not accept throwable copy assignable Deleter if DeleterType is reference)");

            reset(other.leak());
            deleter() = forward<DeleterType>(other.deleter());
            return *this;
        }

        /**
        * Assigns the UniquePointer by stealing the ownership of the pointer.
        * @tparam U Type of the other UniquePointer's pointer
        * @tparam DeleterU  Type of the other UniquePointer's deleter
        * @param other The UniquePointer to transfert ownership from.
        * @return *this
        */
        template<typename U, typename DeleterU>
        constexpr UniquePointer& operator=(UniquePointer<U, DeleterU>&& other) noexcept requires(details::IsUniquePointerMoveAssignableToUniquePointerArrayV<UniquePointer, UniquePointer<U, DeleterU>>) {
            static_assert(!IsReferenceV<DeleterType> ? IsNothrowMoveAssignableV<DeleterType> : true, "UniquePointer do not accept throwable move assignable Deleter if DeleterType is no a reference");
            static_assert(IsReferenceV<DeleterType> ? IsNothrowCopyAssignableV<RemoveReferenceT<DeleterType>> : true, "UniquePointer do not accept throwable copy assignable Deleter if DeleterType is reference)");

            reset(other.leak());
            deleter() = forward<DeleterU>(other.deleter());
            return *this;
        }

        /**
        * Assigns the UniquePointer with a nullptr. Same as reset().
        * @param A nullptr
        * @return *this
        */
        HD_FORCEINLINE constexpr UniquePointer& operator=(hud::ptr::null) noexcept {
            reset();
            return *this;
        }

        /** Call the deleter on the pointer if any owned. */
        constexpr ~UniquePointer() noexcept {
            if (is_owning()) {
                deleter()(pointer());
            }
        }

        /** Retrieves the owned pointer. */
        [[nodiscard]]
        HD_FORCEINLINE constexpr PointerType pointer() const noexcept {
            return get<0>(inner);
        }

        /** Retrieves a reference to the element at the given index. */
        [[nodiscard]]
        HD_FORCEINLINE constexpr AddLValueReferenceT<T> operator[](const usize at) const noexcept {
            return pointer()[at];
        }

        /** Check whether the UniquePointer own a pointer. */
        [[nodiscard]]
        HD_FORCEINLINE constexpr bool is_owning() const noexcept {
            return pointer() != PointerType();
        }

        /** Check whether the UniquePointer own a pointer. */
        [[nodiscard]]
        HD_FORCEINLINE constexpr explicit operator bool() const noexcept {
            return is_owning();
        }

        /** Retrieves the deleter. */
        [[nodiscard]]
        HD_FORCEINLINE constexpr DeleterType& deleter() noexcept {
            return get<1>(inner);
        }

        /** Retrieves the const deleter. */
        [[nodiscard]]
        HD_FORCEINLINE constexpr const DeleterType& deleter() const noexcept {
            return get<1>(inner);
        }

        /** Release the ownership of the pointer and return it without calling the deleter on the owned pointer. */
        [[nodiscard]]
        constexpr PointerType leak() noexcept {
            PointerType old = pointer();
            get<0>(inner) = PointerType();
            return old;
        }

        /**  Call the deleter on the owned pointer and optionally take ownership of a new pointer. */
        template<typename U>
        constexpr void reset(U ptr) noexcept requires(details::IsUniquePointerArrayCanBeResetWithV<UniquePointer, U>) {
            PointerType old_ptr = pointer();
            get<0>(inner) = ptr;
            deleter()(old_ptr);
        }

        /** Call the deleter on the owned pointer and taking ownership of nullptr. */
        HD_FORCEINLINE constexpr void reset(hud::ptr::null) noexcept {
            deleter()(pointer());
            get<0>(inner) = PointerType();
        }

        /** Call the deleter on the owned pointer and taking no ownership. */
        HD_FORCEINLINE constexpr void reset() noexcept {
            reset(nullptr);
        }

        /** Swap with another UniquePointer. */
        HD_FORCEINLINE constexpr void swap(UniquePointer& other) noexcept {
            hud::swap(inner, other.inner);
        }

    private:
        /** Not copy constructible. */
        UniquePointer(const UniquePointer&) = delete;
        /** Not copyable. */
        UniquePointer& operator = (const UniquePointer&) = delete;

    private:
        /** Tuple containing the pointer and the deleter */
        Tuple< PointerType, DeleterType> inner;
    };

    /**
    * Swap UniquePointer.
    * Same as first.swap(second).
    * @tparam T Type of the first UniquePointer's pointer
    * @tparam DeleterT Type of the first UniquePointer's deleter
    * @tparam U Type of the second UniquePointer's pointer
    * @tparam DeleterU Type of the second UniquePointer's deleter
    * @param first The first to swap
    * @param second The second to swap
    */
    template<typename T, typename DeleterT, typename U, typename DeleterU>
    constexpr void swap(UniquePointer<T, DeleterT>& first, UniquePointer<U, DeleterU>& second) noexcept {
        first.swap(second);
    }

    /**
    * Swap UniquePointer.
    * Same as first.swap(second)
    * @tparam T Type of both UniquePointer's pointer
    * @tparam Deleter Type of the both UniquePointer's deleter
    * @param first The first to swap
    * @param second The second to swap
    */
    template<typename T, typename Deleter>
    constexpr void swap(UniquePointer<T, Deleter>& first, UniquePointer<T, Deleter>& second) noexcept {
        first.swap(second);
    }

    /**
    * Checks whether two UniquePointer owned the same pointer
    * @tparam T Type of the first UniquePointer's pointer
    * @tparam DeleterT Type of the first UniquePointer's deleter
    * @tparam U Type of the second UniquePointer's pointer
    * @tparam DeleterU Type of the second UniquePointer's deleter
    * @param first The first to compare
    * @param second The second to compare
    * @param true if both pointer are equals, false otherwise
    */
    template<typename T, typename DeleterT, typename U, typename DeleterU>
    [[nodiscard]]
    constexpr bool operator==(const UniquePointer<T, DeleterT>& first, const UniquePointer<U, DeleterU>& second) noexcept {
        return first.pointer() == second.pointer();
    }

    /**
    * Checks whether the UniquePointer don't own pointer.
    * @tparam T Type of the UniquePointer's pointer
    * @tparam Deleter Type of the UniquePointer's deleter
    * @param pointer The pointer to compare with nullptr
    * @param hud::ptr::null
    * @param true if UniquePointer don't own a pointer, false otherwise
    */
    template<typename T, typename Deleter>
    [[nodiscard]]
    constexpr bool operator==(const UniquePointer<T, Deleter>& pointer, hud::ptr::null) noexcept {
        return !pointer;
    }

    /**
    * Checks whether the UniquePointer don't own pointer.
    * @tparam T Type of the UniquePointer's pointer
    * @tparam Deleter Type of the UniquePointer's deleter
    * @param hud::ptr::null
    * @param pointer The pointer to compare with nullptr
    * @param true if UniquePointer don't own a pointer, false otherwise
    */
    template<typename T, typename Deleter>
    [[nodiscard]]
    constexpr bool operator==(hud::ptr::null, const UniquePointer<T, Deleter>& pointer) noexcept {
        return !pointer;
    }

    /**
    * Checks whether two UniquePointer owned the different pointers.
    * @tparam T Type of the first UniquePointer's pointer
    * @tparam DeleterT Type of the first UniquePointer's deleter
    * @tparam U Type of the second UniquePointer's pointer
    * @tparam DeleterU Type of the second UniquePointer's deleter
    * @param first The first to compare
    * @param second The second to compare
    * @param true if both pointer are not equals, false otherwise
    */
    template<typename T, typename DeleterT, typename U, typename DeleterU>
    [[nodiscard]]
    constexpr bool operator!=(const UniquePointer<T, DeleterT>& first, const UniquePointer<U, DeleterU>& second) noexcept {
        return first.pointer() != second.pointer();
    }

    /**
    * Checks whether the UniquePointer own pointer.
    * @tparam T Type of the UniquePointer's pointer
    * @tparam Deleter Type of the UniquePointer's deleter
    * @param pointer The pointer to check
    * @param hud::ptr::null
    * @param true if UniquePointer own a pointer, false otherwise
    */
    template<typename T, typename Deleter>
    [[nodiscard]]
    constexpr bool operator!=(const UniquePointer<T, Deleter>& pointer, hud::ptr::null) noexcept {
        return static_cast<bool>(pointer);
    }

    /**
    * Checks whether the UniquePointer own pointer.
    * @tparam T Type of the UniquePointer's pointer
    * @tparam DeleterT Type of the UniquePointer's deleter
    * @param hud::ptr::null
    * @param pointer The pointer to check
    * @param true if UniquePointer own a pointer, false otherwise
    */
    template<typename T, typename Deleter>
    [[nodiscard]]
    constexpr bool operator!=(hud::ptr::null, const UniquePointer<T, Deleter>& pointer) noexcept {
        return static_cast<bool>(pointer);
    }

    /**
    * Checks whether the first owned UniquePointer address less than the second owned pointer address.
    * @tparam T Type of the first UniquePointer's pointer
    * @tparam DeleterT Type of the first UniquePointer's deleter
    * @tparam U Type of the second UniquePointer's pointer
    * @tparam DeleterU Type of the second UniquePointer's deleter
    * @param first The first to compare
    * @param second The second to compare
    * @return true if first owned pointer address is less than second owned pointer address, false otherwise
    */
    template<typename T, typename DeleterT, typename U, typename DeleterU>
    [[nodiscard]]
    bool operator<(const UniquePointer<T, DeleterT>& first, const UniquePointer<U, DeleterU>& second) noexcept {
        using PointerType = CommonTypeT<typename UniquePointer<T, DeleterT>::PointerType, typename UniquePointer<U, DeleterU>::PointerType>;
        return Less<PointerType>()(first.pointer(), second.pointer());
    }

    /**
    * Checks whether the owned pointer address is less than nullptr.
    * @tparam T Type of the UniquePointer's pointer
    * @tparam Deleter Type of the UniquePointer's deleter
    * @param pointer The pointer to compare
    * @param nullptr
    * @return true if the owned pointer address is less than nullptr, false otherwise
    */
    template<typename T, typename Deleter>
    [[nodiscard]]
    constexpr bool operator<(const UniquePointer<T, Deleter>& pointer, hud::ptr::null) noexcept {
        using PointerType = typename UniquePointer<T, Deleter>::PointerType;
        return Less<PointerType>()(pointer.pointer(), nullptr);
    }

    /**
    * Checks whether nullptr is less than the owned pointer address.
    * @tparam T Type of the UniquePointer's pointer
    * @tparam Deleter Type of the UniquePointer's deleter
    * @param nullptr
    * @param pointer The pointer to compare
    * @return true if nullptr is less than the owned pointer address, false otherwise
    */
    template<typename TType, typename TDeleterType>
    [[nodiscard]]
    constexpr bool operator<(hud::ptr::null, const UniquePointer<TType, TDeleterType>& pointer) noexcept {
        using PointerType = typename UniquePointer<TType, TDeleterType>::PointerType;
        return Less<PointerType>()(nullptr, pointer.pointer());
    }

    /**
    * Checks whether the first owned UniquePointer address greater than the second owned pointer address.
    * @tparam T Type of the first UniquePointer's pointer
    * @tparam DeleterT Type of the first UniquePointer's deleter
    * @tparam U Type of the second UniquePointer's pointer
    * @tparam DeleterU Type of the second UniquePointer's deleter
    * @param first The first to compare
    * @param second The second to compare
    * @return true if first owned pointer address is greater than second owned pointer address, false otherwise
    */
    template<typename T, typename DeleterT, typename U, typename DeleterU>
    [[nodiscard]]
    bool operator>(const UniquePointer<T, DeleterT>& first, const UniquePointer<U, DeleterU>& second) noexcept {
        return second < first;
    }

    /**
    * Checks whether the owned pointer address is greater than nullptr.
    * @tparam T Type of the UniquePointer's pointer
    * @tparam Deleter Type of the UniquePointer's deleter
    * @param pointer The pointer to compare
    * @param nullptr
    * @return true if the owned pointer address is greater than nullptr, false otherwise
    */
    template<typename T, typename Deleter>
    [[nodiscard]]
    constexpr bool operator>(const UniquePointer<T, Deleter>& pointer, hud::ptr::null) noexcept {
        return nullptr < pointer;
    }

    /**
    * Checks whether nullptr is greater than the owned pointer address.
    * @tparam T Type of the UniquePointer's pointer
    * @tparam Deleter Type of the UniquePointer's deleter
    * @param nullptr
    * @param pointer The pointer to compare
    * @return true if nullptr is greater than the owned pointer address, false otherwise
    */
    template<typename T, typename Deleter>
    [[nodiscard]]
    constexpr bool operator>(hud::ptr::null, const UniquePointer<T, Deleter>& pointer) noexcept {
        return pointer < nullptr;
    }

    /**
    * Checks whether the first owned UniquePointer address less or equal the second owned pointer address.
    * @tparam T Type of the first UniquePointer's pointer
    * @tparam DeleterT Type of the first UniquePointer's deleter
    * @tparam U Type of the second UniquePointer's pointer
    * @tparam DeleterU Type of the second UniquePointer's deleter
    * @param first The first to compare
    * @param second The second to compare
    * @return true if first owned pointer address is less or equal the second owned pointer address, false otherwise
    */
    template<typename T, typename DeleterT, typename U, typename DeleterU>
    [[nodiscard]]
    bool operator<=(const UniquePointer<T, DeleterT>& first, const UniquePointer<U, DeleterU>& second) noexcept {
        return !(second < first);
    }

    /**
    * Checks whether the owned pointer address is less or equal nullptr.
    * @tparam T Type of the UniquePointer's pointer
    * @tparam Deleter Type of the UniquePointer's deleter
    * @param pointer The pointer to compare
    * @param nullptr
    * @return true if the owned pointer address is less or equal nullptr, false otherwise
    */
    template<typename T, typename Deleter>
    [[nodiscard]]
    constexpr bool operator<=(const UniquePointer<T, Deleter>& pointer, hud::ptr::null) noexcept {
        return !(nullptr < pointer);
    }

    /**
    * Checks whether nullptr is less or equal the owned pointer address.
    * @tparam T Type of the UniquePointer's pointer
    * @tparam Deleter Type of the UniquePointer's deleter
    * @param nullptr
    * @param pointer The pointer to compare
    * @return true if nullptr is less or equal the owned pointer address, false otherwise
    */
    template<typename T, typename Deleter>
    [[nodiscard]]
    constexpr bool operator<=(hud::ptr::null, const UniquePointer<T, Deleter>& pointer) noexcept {
        return !(pointer < nullptr);
    }

    /**
    * Checks whether the first owned unique pointer address compares greater or equal the second owned pointer address.
    * @tparam T Type of the first UniquePointer's pointer
    * @tparam DeleterT Type of the first UniquePointer's deleter
    * @tparam U Type of the second UniquePointer's pointer
    * @tparam DeleterU Type of the second UniquePointer's deleter
    * @param first The first to compare
    * @param second The second to compare
    * @return true if first owned unique pointer address compares greater or equal the second owned pointer address, false otherwise
    */
    template<typename T, typename DeleterT, typename U, typename DeleterU>
    [[nodiscard]]
    bool operator>=(const UniquePointer<T, DeleterT>& first, const UniquePointer<U, DeleterU>& second) noexcept {
        return !(first < second);
    }

    /**
    * Checks whether the owned pointer address is greater or equal nullptr.
    * @tparam T Type of the UniquePointer's pointer
    * @tparam Deleter Type of the UniquePointer's deleter
    * @param pointer The pointer to compare
    * @param nullptr
    * @return True if the owned pointer address is greater or equal nullptr, false otherwise
    */
    template<typename T, typename Deleter>
    [[nodiscard]]
    constexpr bool operator>=(const UniquePointer<T, Deleter>& pointer, hud::ptr::null) noexcept {
        return !(pointer < nullptr);
    }

    /**
    * Checks whether nullptr is greater or equal the owned pointer address.
    * @tparam T Type of the UniquePointer's pointer
    * @tparam Deleter Type of the UniquePointer's deleter
    * @param nullptr
    * @param pointer The pointer to compare
    * @return true if nullptr is greater or equal the owned pointer address, false otherwise
    */
    template<typename T, typename Deleter>
    [[nodiscard]]
    constexpr bool operator>=(hud::ptr::null, const UniquePointer<T, Deleter>& pointer) noexcept {
        return !(nullptr < pointer);
    }

    /**
    * Constructs a UniquePointer that owns a pointer of type T. The arguments are forward to the constructor of T.
    * This overload only participates in overload resolution if T is not an array type
    * @tparam T Type of the UniquePointer's pointer
    * @tparam TArgs The T constructor arguments
    * @param args Arguments forward to the T constructor
    * @return UniquePointer<T> pointing to a object of type T construct by passing args arguments to its constructor
    */
    template<typename T, typename... TArgs, EnableIfT<NotV<IsArray<T>>, i32> = 0 >
    [[nodiscard]]
    constexpr UniquePointer<T> make_unique(TArgs&&... args) noexcept {
        return UniquePointer<T>(new (std::nothrow) T(forward<TArgs>(args)...));
    }

    /**
    * Constructs a UniquePointer that owns a pointer to an array of unknown bound T.
    * This overload only participates in overload resolution if T is an array of unknown bound.
    * @tparam T Type of the UniquePointer's pointer
    * @param size Number of T to allocate
    * @return UniquePointer<T> pointer to an array of type T
    */
    template<typename T, EnableIfT<IsUnboundedArrayV<T>, i32> = 0>
    [[nodiscard]]
    UniquePointer<T> make_unique(const usize size) noexcept {
        return UniquePointer<T>(new (std::nothrow) RemoveExtentT<T>[size]());
    }

    /** Construction of arrays of known bound is disallowed. */
    template<typename T, typename... TArgs, EnableIfT<IsBoundedArrayV<T>> = 0>
    void make_unique(TArgs&&...) = delete;

    /**
    * Hash function for UniquePointer<T>
    * @tparam T Type of the UniquePointer's pointer
    * @param ptr The UniquePointer<T> to hash
    * @return The hash of ptr
    */
    template<typename T>
    constexpr u32 hash(const UniquePointer<T>& ptr) noexcept {
        return hud::hash(ptr.pointer());
    }

} // namespace hud

#endif // HD_INC_OSLAYER_UNIQUE_POINTER_H