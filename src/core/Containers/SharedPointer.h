#pragma once
#ifndef HD_INC_OSLAYER_SHARED_POINTER_H
#define HD_INC_OSLAYER_SHARED_POINTER_H
#include "../Minimal.h"
#include "../Atomics.h"
#include "Pair.h"
#include "../Assert.h"
#include "AlignedBuffer.h"

#include "../Traits/And.h"
#include "../Traits/Or.h"
#include "../Traits/IsSame.h"
#include "../Traits/IsPointer.h"
#include "../Traits/IsConvertible.h"
#include "../Traits/RemovePointer.h"
#include "../Traits/VoidT.h"
#include "../Traits/AddPointer.h"
#include "../Traits/Not.h"
#include "../Traits/IsVoid.h"
#include "../Traits/AddConst.h"
#include "../Traits/IsNothrowDefaultConstructible.h"
#include "../Traits/CommonType.h"
#include "../Traits/IsUnboundedArray.h"

#include "../Templates/SelectDeleterPointerType.h"
#include "../Templates/DefaultDeleter.h"
#include "../Templates/Declval.h"
#include "../Templates/Less.h"
#include "../Templates/Forward.h"
#include "../Hash.h"
#include "../Allocators/Allocation.h"

namespace hud {

    /**
    * SharedPointer thread safety enumeration.
    * Defines how a SharedPointer support thread safety.
    */
    enum class EThreadSafety {
        /** Atomic synchronisation is used by SharedPointer to modify counters. */
        safe,
        /** No thread synchronisation is used by SharedPointer to modify counters. */
        not_safe
    };

    /** Forward declation, default thread safety is not safe. */
    template<typename T, EThreadSafety thread_safety = EThreadSafety::not_safe>
    class SharedPointer;


    namespace details {

        /**
        * The base class of the reference controller allocated on the heap shared with all SharedPointer and WeakPointer referencing the same pointer.
        * @tparam thread_safety The reference counting thread safety to use while counting shared and weak references
        */
        template<EThreadSafety thread_safety>
        class ReferenceControllerBase;

        /** The not thread safe base class of the reference controller allocated on the heap shared with all SharedPointer and WeakPointer referencing the same pointer. */
        template<>
        class ReferenceControllerBase<EThreadSafety::not_safe> {

        public:
            /** Default constructor. */
            constexpr ReferenceControllerBase() noexcept = default;

            /** Destructor. */
            constexpr virtual ~ReferenceControllerBase() = default;

            /** Override this function to destroyed the owned object. */
            constexpr virtual void destroy_object() = 0;

            /** Retrieves the shared counter. */
            [[nodiscard]]
            constexpr u32 get_shared_count() const noexcept {
                return shared_count;
            }

            /** Retrieves the weak counter. */
            [[nodiscard]]
            constexpr u32 get_weak_count() const noexcept {
                return weak_count;
            }

            /** 
            * Acquire a shared reference increment the shared counter of a ReferenceControllerBase by 1.
            * @controller The ReferenceControllerBase to decrement shared counter
            */
            static constexpr void acquire_sharedref(ReferenceControllerBase* controller) noexcept {
                ++(controller->shared_count);
            }

            /**
            * Release a shared reference decrement the shared counter of a ReferenceControllerBase by 1.
            * If the shared reference count is decremented to 0 the object is destroyed then the weak counter is released.
            * @controller The ReferenceControllerBase to decrement shared counter
            */
            static constexpr void release_sharedref(ReferenceControllerBase* controller) noexcept {
                check(controller->shared_count > 0u);

                if (--(controller->shared_count) == 0u) {
                    // When reach 0 shared reference, destroy the object
                    controller->destroy_object();

                    // Release the weak count by 1, when the weak count will reach 0, the object will be destroyed
                    release_weakref(controller);
                }
            }

            /**
            * Release a weak reference decrement the weak counter of a ReferenceControllerBase by 1.
            * If the weak reference count is decremented to 0 the controller is deleted.
            * @controller The ReferenceControllerBase to decrement shared counter
            */
            static constexpr void release_weakref(ReferenceControllerBase* controller) noexcept {
                check(controller->weak_count > 0u);

                if (--(controller->weak_count) == 0u) {
                    delete controller;
                }
            }

        private:
            /** Not copy constructible. */
            ReferenceControllerBase(const ReferenceControllerBase&) = delete;
            /** Not copy assignable. */
            ReferenceControllerBase& operator=(const ReferenceControllerBase&) = delete;

        private:
            /** Count of shared references. */
            u32 shared_count = 1u;
            /** Count of weak references. */
            u32 weak_count = 1u;
        };


        /** 
        * The thread safe base class of the reference controller allocated on the heap.
        * Shared with all SharedPointer and WeakPointer referencing the same pointer.
        */
        template<>
        class ReferenceControllerBase<EThreadSafety::safe> {

        public:
            /** Default constructor. */
            constexpr ReferenceControllerBase() noexcept = default;

            /** Destructor. */
            constexpr virtual ~ReferenceControllerBase() = default;

            /** Override this function to destroyed the owned object. */
            constexpr virtual void destroy_object() = 0;

            /** Retrieves the shared counter. */
            [[nodiscard]]
            u32 get_shared_count() const noexcept {
                return shared_count.load();
            }

            /** Retrieves the weak counter. */
            [[nodiscard]]
            u32 get_weak_count() const noexcept {
                return weak_count.load();
            }

            /**
            * Acquire a shared reference increment the shared counter of a ReferenceControllerBase by 1.
            * @controller The ReferenceControllerBase to increment shared counter
            */
            static void acquire_sharedref(ReferenceControllerBase* controller) noexcept {
                controller->shared_count.increment();
            }

            /**
            * Release a shared reference decrement the shared counter of a ReferenceControllerBase by 1.
            * If the shared reference count is decremented to 0 the object is destroyed then the weak counter is released.
            * @controller The ReferenceControllerBase to decrement shared counter
            */
            static void release_sharedref(ReferenceControllerBase* controller) noexcept {
                check(controller->shared_count.load() > 0u);

                if (controller->shared_count.decrement() == 0u) {
                    // When reach 0 shared reference, destroy the object
                    controller->destroy_object();

                    // Release the weak count by 1, when the weak count will reach 0, the object will be destroyed
                    release_weakref(controller);
                }
            }

            /**
            * Release a weak reference decrement the weak counter of a ReferenceControllerBase by 1.
            * If the weak reference count is decremented to 0 the controller is deleted.
            * @controller The ReferenceControllerBase to decrement shared counter
            */
            static void release_weakref(ReferenceControllerBase* controller) noexcept {
                check(controller->weak_count.load() > 0u);

                if (controller->weak_count.decrement() == 0u) {
                    delete controller;
                }
            }

        private:
            /** Not copy constructible. */
            ReferenceControllerBase(const ReferenceControllerBase&) = delete;
            /** Not copy assignable. */
            ReferenceControllerBase& operator=(const ReferenceControllerBase&) = delete;

        private:
            /** Count of shared references. */
            Atomic<u32> shared_count = 1u;
            /** Count of weak references. */
            Atomic<u32> weak_count = 1u;

        };

        /**
        * The reference controller that keep the pointer to the heap allocated T.
        * @tparam T Type of the pointer to own
        * @tparam thread_safety The reference counting thread safety to use while counting shared and weak references
        * @tparam Deleter Type of the deleter to use when the object is destroyed
        */
        template<typename T, EThreadSafety thread_safety, typename Deleter>
        class ReferenceControllerWithDeleter 
            : public ReferenceControllerBase<thread_safety> {

        public:
            /** Construct a ReferenceControllerWithDeleter from a pointer to own. */
            explicit constexpr ReferenceControllerWithDeleter(T* ptr) noexcept
                : pointer(ptr) {
            }

            /** Destroy the object by calling the deleter on the owned pointer. */
            constexpr void destroy_object() noexcept final {
                Deleter deleter;
                deleter(pointer);
            }

        private:
            /** Not copy constructible. */
            ReferenceControllerWithDeleter(const ReferenceControllerWithDeleter&) = delete;
            /** Not copy assignable. */
            ReferenceControllerWithDeleter operator=(const ReferenceControllerWithDeleter&) = delete;

        private:
            /** The pointer to delete. */
            T* pointer;
        };

        /**
        * The reference controller that contains T instead of referencing a pointer.
        * @tparam T Type of the pointer to own
        * @tparam thread_safety The reference counting thread safety to use while counting shared and weak references
        */
        template<typename T, EThreadSafety thread_safety>
        class ReferenceControllerNoDeleter 
            : public ReferenceControllerBase<thread_safety> {

        public:
            /**
            * Construct a ReferenceControllerNoDeleter from a list or arguments forward to the T constructor.
            * In-place constructor T with the given arguments.
            * @tparam Args Types of arguments forward to the T constructor
            * @param args Arguments forward to the T constructor
            */
            template<typename... TArgs>
            explicit constexpr ReferenceControllerNoDeleter(TArgs&&... args) noexcept requires(!IsArrayV<T>) {
                static_assert(IsNothrowConstructibleV<T, TArgs...>, "SharedPointer do not accept throwable constructible type");
                Memory::construct_at(pointer(), hud::forward<TArgs>(args)...);
            }

            /** Retrieves a pointer to the T object. */
            HD_FORCEINLINE T* pointer() noexcept {
                return buffer.template pointer_as<T>();
            }

            /** Destroy the object by calling the destructor. */
            constexpr void destroy_object() noexcept final {
                Memory::destroy(*pointer());
            }

        private:
            /** Not copy constructible. */
            ReferenceControllerNoDeleter(const ReferenceControllerNoDeleter&) = delete;
            /** Not copy assignable. */
            ReferenceControllerNoDeleter operator=(const ReferenceControllerNoDeleter&) = delete;

        private:
            /** The aligned buffer containing T */
            AlignedBuffer<sizeof(T), alignof(T)> buffer;
        };

        /**
        * The reference controller that contains an aligned pointer to an array of T.
        * @tparam T Type of the pointer to own
        * @tparam thread_safety The reference counting thread safety to use while counting shared and weak references
        */
        template<typename T, EThreadSafety thread_safety>
        class ReferenceControllerNoDeleter<T[], thread_safety>
            : public ReferenceControllerBase<thread_safety> {

        public:
            /** Construct a ReferenceControllerWithDeleter from a pointer to own. */
            explicit constexpr ReferenceControllerNoDeleter(usize count) noexcept
                : allocation(Memory::allocate_align<T>(count* sizeof(T), alignof(T)), count) {
                Memory::default_construct_array(allocation.data(), allocation.data_end());
            }

            /** Retrieves a pointer to the T object. */
            constexpr T* pointer() noexcept {
                return allocation.data();
            }

            /** Destroy the object by calling the destructor. */
            constexpr void destroy_object() noexcept final {
                Memory::destroy_array(pointer(), allocation.count());
                Memory::free_align(pointer());
            }

        private:
            /** Not copy constructible. */
            ReferenceControllerNoDeleter(const ReferenceControllerNoDeleter&) = delete;
            /** Not copy assignable. */
            ReferenceControllerNoDeleter operator=(const ReferenceControllerNoDeleter&) = delete;
        private:
            /** The allocation of the array. */
            Allocation<T> allocation;
        };

        /**
        * The reference controller use to count reference of a SharedPointer and WeakPointer.
        * @tparam thread_safety The reference counting thread safety to use while counting shared and weak references
        */
        template<EThreadSafety thread_safety>
        class ReferenceController {

        public:
            /** Default constrcutor. */
            constexpr ReferenceController() noexcept = default;

            /** Construct a ReferenceController by sharing ownership of the controller to control. */
            constexpr explicit ReferenceController(ReferenceControllerBase<thread_safety>* controller_base) noexcept
                : controller(controller_base) {
            }

            /**
            * Construct a ReferenceController by sharing the ownership the controller and acquire a shared reference on it.
            * @param other The other ReferenceController to copy
            */
            constexpr ReferenceController(const ReferenceController& other) noexcept
                : controller(other.controller) {

                // Increment the counter if the copied reference controller have a associated object
                if (controller != nullptr) {
                    ReferenceControllerBase::acquire_sharedref(controller);
                }
            }

            /**
            * Construct a ReferenceController by stealing the controller.
            * @param other The other ReferenceController to move
            */
            constexpr ReferenceController(ReferenceController&& other) noexcept
                : controller(other.controller) {
                other.controller = nullptr;
            }

            /**
            * Assigns the ReferenceController by sharing the controller and acquire a shared reference on 
            * the newly shared controller before releasing a shared reference on the old one.
            * @param other The other ReferenceController to copy
            * @return *this
            */
            constexpr ReferenceController& operator=(const ReferenceController& other) noexcept {
                // Do not take into account if we assign if the same controller
                if (controller != other.controller) {
                    // We take acquire a reference on the copied controller
                    if (other.controller != nullptr) {
                        ReferenceControllerBase::acquire_sharedref(other.controller);
                    }
                    // We release our controller
                    else if (controller != nullptr) {
                        ReferenceControllerBase::release_sharedref(controller);
                    }
                    // Then we borrow the controller
                    controller = other.controller;
                }
                return *this;
            }

            /**
            * Assigns the ReferenceController by stealing the controller before releasing a shared reference on the old one.
            * @param other The other ReferenceController to copy
            * @return *this
            */
            constexpr ReferenceController& operator=(ReferenceController&& other) noexcept {
                // Do not take into account if we assign if the same controller
                if (controller != other.controller) {

                    // Move the controller
                    ReferenceControllerBase* old_controller = controller;
                    controller = other.controller;
                    other.controller = nullptr;

                    // Remove the reference on the old controller
                    if (old_controller != nullptr) {
                        ReferenceControllerBase::release_sharedref(old_controller);
                    }
                }
                return *this;
            }

            /** Destructor. Release a shared reference on the owned controller. */
            constexpr ~ReferenceController() {
                if (controller != nullptr) {
                    ReferenceControllerBase::release_sharedref(controller);
                }
            }

            /** Retrieves the shared counter. */
            [[nodiscard]]
            constexpr u32 shared_count() const noexcept {
                return controller != nullptr ? controller->get_shared_count() : 0u;
            }

        private:
            /**  ReferenceControllerBase syntax sugar. */
            using ReferenceControllerBase = ReferenceControllerBase<thread_safety>;
            /** Pointer to the owned ReferenceControllerBase. */
            ReferenceControllerBase* controller = nullptr;
        };


        /**
        * Checks whether SharedPointer<U> is convertible to SharedPointer<T>
        * @tparam U Type of the SharedPointer<U>
        * @tparam T Type of the SharedPointer<T>
        */
        template<typename U, typename T>
        static constexpr bool IsConvertibleV = hud::IsConvertibleV<U*, T*>;
        template<typename U, typename T>
        static constexpr bool IsConvertibleV<U,T[]> = hud::IsConvertibleV<U(*)[], T(*)[]>;
        template<typename U, typename T, usize extent>
        static constexpr bool IsConvertibleV<U, T[extent]> = hud::IsConvertibleV<U(*)[extent], T(*)[extent]>;

        /**
        * Checks whether a SharedPointer pointer U is compatible with SharedPointer pointer T
        * @tparam U Type pointer of the SharedPointer<U>
        * @tparam T Type of the SharedPointer<T>
        * */
        template<typename U, typename T>
        static constexpr bool IsPointerCompatibleV = hud::IsConvertibleV<U*, T*>;
        template<typename U, usize extent>
        static constexpr bool IsPointerCompatibleV<U[extent], U[]> = true;
        template<typename U, usize extent>
        static constexpr bool IsPointerCompatibleV<U[extent], const U[]> = true;
        template<typename U, usize extent>
        static constexpr bool IsPointerCompatibleV<U[extent], volatile U[]> = true;
        template<typename U, usize extent>
        static constexpr bool IsPointerCompatibleV<U[extent], const volatile U[]> = true;
}

    /**
    * SharedPointer is a smart pointer that share a owning pointer with other SharedPointer.
    * SharedPointer object is automatically delete when no other SharedPointer or WeakPointer share the owned pointer (either by using deleter or just calling destructor) as soon as they themselves are destroyed, or as soon as their value changes either by an assignment operation or by an explicit call to UniquePointer::reset.
    * @tparam T The type of the pointer to own
    * @tparam thread_safety The thread safty of the reference counting
    */
    template<typename T, EThreadSafety thread_safety>
    class SharedPointer {

    public:
        /** Internal pointer type representation. */
        using PointerType = RemoveExtentT<T>*;
        /** Internal Deleter type used to delete the internal pointer. */
        template<typename U>
        using DeleterType = ConditionalT<IsArrayV<T>, DefaultDeleter<U[]>, DefaultDeleter<U>>;

    public:
        /** Default constructor. Do not own pointer. */
        constexpr SharedPointer() noexcept = default;

        /**
        * Construct a SharedPointer form a raw pointer to own.
        * The given pointer must be convertible to T*.
        * @param pointer The poiner to own
        */
        template<typename U>
        constexpr explicit SharedPointer(U* pointer) noexcept requires(details::IsConvertibleV<U,T>)
            : inner(pointer, new (std::nothrow)details::ReferenceControllerWithDeleter<U, thread_safety, DeleterType<U>>(pointer)) {
        }

        /** Construct a SharedPointer from a nullptr. Do not own pointer. */
        constexpr SharedPointer(hud::ptr::null) noexcept {
        }

        /** Construct a SharedPointer by sharing another SharedPointer ownership. */
        constexpr SharedPointer(const SharedPointer& other) noexcept
            : inner(other.inner) {
        }

        /**
        * Construct a SharedPointer by sharing another SharedPointer ownership.
        * @tparam U Type of the other SharedPointer's pointer to share. Must be compatible with T
        * @param other The SharedPointer to share ownership with
        */
        template<typename U>
        constexpr SharedPointer(const SharedPointer<U, thread_safety>& other) noexcept requires(details::IsPointerCompatibleV<U, T>)
            : inner(other.inner) {
        }

        /** Construct a SharedPointer by stealing another SharedPointer ownership. */
        constexpr SharedPointer(SharedPointer&& other) noexcept
            : inner(hud::move(other.inner)) {
            get<0>(other.inner) = nullptr;
        }

        /**
        * Construct a SharedPointer by stealing another SharedPointer ownership.
        * @tparam U Type of the SharedPointer's pointer to steal. Must be compatible with T
        * @param other The SharedPointer to transfert ownership from.
        */
        template<typename U>
        constexpr SharedPointer(SharedPointer<U, thread_safety>&& other) noexcept requires(details::IsPointerCompatibleV<U, T>)
            : inner(hud::move(other.inner)) {
            get<0>(other.inner) = nullptr;
        }

        /** Assign the SharedPointer by stealing the ownership of the other SharedPointer. */
        constexpr SharedPointer& operator=(const SharedPointer& other) noexcept {
            if (this != &other) {
                inner = other.inner;
            }
            return *this;
        }

        /**
        * Assigns the SharedPointer by sharing the ownership of the other SharedPointer.
        * @tparam U Type of the other SharedPointer's pointer to share. Must be compatible with T
        * @param other The SharedPointer to share ownership
        * @return *this
        */
        template<typename U>
        constexpr SharedPointer& operator=(const SharedPointer<U, thread_safety>& other) noexcept requires(details::IsPointerCompatibleV<U, T>) {
            inner = other.inner;
            return *this;
        }

        /** Assigns the SharedPointer by stealing the ownership of the other pointer. */
        constexpr SharedPointer& operator=(SharedPointer&& other) noexcept {
            if (this != &other) {
                inner = hud::move(other.inner);
                get<0>(other.inner) = nullptr;
            }
            return *this;
        }

        /**
        * Assigns the SharedPointer by stealing the ownership of the other pointer.
        * @tparam U Type of the other SharedPointer's pointer to steal. Must be compatible with T
        * @param other The SharedPointer to steal
        * @return *this
        */
        template<typename U>
        constexpr SharedPointer& operator=(SharedPointer<U, thread_safety>&& other) noexcept requires(details::IsPointerCompatibleV<U, T>) {
            inner = hud::move(other.inner);
            get<0>(other.inner) = nullptr;
            return *this;
        }

        /** Assigns the SharedPointer with a nullptr. This will no more own a pointer. */
        constexpr SharedPointer& operator=(hud::ptr::null) noexcept {
            reset();
            return *this;
        }

        /** Retrieves the own pointer. */
        [[nodiscard]]
        constexpr PointerType pointer() const noexcept {
            return get<0>(inner);
        }

        /** Check whether the SharedPointer own a pointer. */
        [[nodiscard]]
        constexpr bool is_owning() const noexcept {
            return pointer() != PointerType();
        }

        /** Check whether the SharedPointer own a pointer. */
        [[nodiscard]]
        constexpr explicit operator bool() const noexcept {
            return is_owning();
        }

        /** Dereference owned pointer. */
        [[nodiscard]]
        constexpr AddLValueReferenceT<RemoveExtentT<T>> operator*() const noexcept requires(!IsVoidV<T>) {
            return *pointer();
        }

        /** Retrieves the owned pointer. */
        [[nodiscard]]
        constexpr PointerType operator->() const noexcept {
            return pointer();
        }

        /** Retrieves a reference to the element at the given index. */
        [[nodiscard]]
        constexpr AddConstT<AddLValueReferenceT<RemoveExtentT<T>>> operator[](const usize at) const noexcept requires(IsArrayV<T>){
            return pointer()[at];
        } 
        
        /** Retrieves a reference to the element at the given index. */
        [[nodiscard]]
        constexpr AddLValueReferenceT<RemoveExtentT<T>> operator[](const usize at) noexcept requires(IsArrayV<T>) {
            return pointer()[at];
        }

        /**
        * Retrieves the number of time the pointer is shared
        * This function return an approximation when thread_safety is EThreadSafety::safe
        * @return Number of time the pointer is shared
        */
        [[nodiscard]]
        constexpr u32 shared_count() const noexcept {
            return get<1>(inner).shared_count();
        }

        /** Destroy the owned pointer and optionally take ownership of a new pointer. */
        constexpr void reset(PointerType ptr) noexcept {
            *this = SharedPointer(ptr);
        }

        /** Destroy the owned pointer and taking no ownership. */
        constexpr void reset(hud::ptr::null) noexcept {
            *this = SharedPointer();
        }

        /** Destroy the owned pointer and taking no ownership. */
        constexpr void reset() noexcept {
            reset(nullptr);
        }

        /**Swap with another SharedPointer. */
        constexpr void swap(SharedPointer& other) noexcept {
           hud::swap(inner, other.inner);
        }

    private:
        /** Friend with other owing pointer types */
        template<typename U, EThreadSafety> friend class SharedPointer;
        template<typename U, EThreadSafety thread_safety_1, typename... TArgs> friend SharedPointer<U, thread_safety_1> make_shared(TArgs&&... args) noexcept requires(!IsArrayV<U>);
        template<typename U, EThreadSafety thread_safety_1> friend SharedPointer<U, thread_safety_1> make_shared(usize count) noexcept requires(IsUnboundedArrayV<U>);

        /**
        * Construct a SharedPointer form a ReferenceControllerNoDeleter.
        * Only used by make_shared(...).
        * @param controller The controller to use
        */
        HD_FORCEINLINE explicit SharedPointer(details::ReferenceControllerNoDeleter<T, thread_safety>* controller) noexcept
            : inner(controller->pointer(), controller) {
        }

    private:
        /** Pair containing pointer, deleter and shared reference counter. */
        using InnerType = Pair< PointerType, details::ReferenceController<thread_safety>>;
        InnerType inner;
    };


    /**
    * Swap SharedPointer.
    * Same as first.swap(second).
    * @tparam T Type of the first SharedPointer's pointer
    * @tparam thread_safety The thread safety of pointers
    * @param first The first to swap
    * @param second The second to swap
    */
    template<typename T, EThreadSafety thread_safety>
    HD_FORCEINLINE constexpr void swap(SharedPointer<T, thread_safety>& first, SharedPointer<T, thread_safety>& second) noexcept {
        first.swap(second);
    }

    /**
    * Checks whether two SharedPointer owned the same pointer;
    * @tparam T Type of the first SharedPointer's pointer
    * @tparam thread_safety The thread safety of first SharePointer
    * @tparam U Type of the second SharedPointer's pointer
    * @tparam thread_safety_u The thread safety of second SharePointer
    * @param first The first to compare
    * @param second The second to compare
    * @param true if both pointer are equals, false otherwise
    */
    template<typename T, EThreadSafety thread_safety, typename U, EThreadSafety thread_safety_u>
    [[nodiscard]]
    HD_FORCEINLINE constexpr bool operator==(const SharedPointer<T, thread_safety>& first, const SharedPointer<U, thread_safety_u>& second) noexcept {
        return first.pointer() == second.pointer();
    }

    /**
    * Checks whether the SharedPointer don't own pointer.
    * @tparam T Type of the SharedPointer's pointer
    * @tparam thread_safety The thread safety of SharePointer
    * @param pointer The SharedPointer to compare with nullptr
    * @param hud::ptr::null
    * @param true if SharedPointer don't own a pointer, false otherwise
    */
    template<typename T, EThreadSafety thread_safety>
    [[nodiscard]]
    HD_FORCEINLINE constexpr bool operator==(const SharedPointer<T, thread_safety>& pointer, hud::ptr::null) noexcept {
        return !pointer;
    }

    /**
    * Checks whether the SharedPointer don't own pointer.
    * @tparam T Type of the SharedPointer's pointer
    * @tparam thread_safety The thread safety of SharePointer
    * @param pointer The SharedPointer to compare with nullptr
    * @param hud::ptr::null
    * @param true if SharedPointer don't own a pointer, false otherwise
    */
    template<typename T, EThreadSafety thread_safety>
    [[nodiscard]]
    HD_FORCEINLINE constexpr bool operator==(hud::ptr::null, const SharedPointer<T, thread_safety>& pointer) noexcept {
        return !pointer;
    }

    /**
    * Checks whether two SharedPointer owned the different pointers.
    * @tparam T Type of the first SharedPointer's pointer
    * @tparam thread_safety The thread safety of first SharePointer
    * @tparam U Type of the second SharedPointer's pointer
    * @tparam thread_safety_u The thread safety of second SharePointer
    * @param first The first to compare
    * @param second The second to compare
    * @param true if both pointer are not equals, false otherwise
    */
    template<typename T, EThreadSafety thread_safety, typename U, EThreadSafety thread_safety_u>
    [[nodiscard]]
    HD_FORCEINLINE constexpr bool operator!=(const SharedPointer<T, thread_safety>& first, const SharedPointer<U, thread_safety_u>& second) noexcept {
        return first.pointer() != second.pointer();
    }

    /**
    Checks whether the SharedPointer own pointer
    @tparam T Type of the SharedPointer's pointer
    @tparam thread_safety The thread safety of SharePointer
    @param pointer The pointer to check
    @param hud::ptr::null
    @param true if SharedPointer own a pointer, false otherwise
    */
    template<typename T, EThreadSafety thread_safety>
    [[nodiscard]]
    HD_FORCEINLINE constexpr bool operator!=(const SharedPointer<T, thread_safety>& pointer, hud::ptr::null) noexcept {
        return static_cast<bool>(pointer);
    }

    /**
    * Checks whether the SharedPointer own pointer.
    * @tparam T Type of the SharedPointer's pointer
    * @tparam thread_safety The thread safety of SharePointer
    * @param hud::ptr::null
    * @param pointer The pointer to check
    * @param true if SharedPointer own a pointer, false otherwise
    */
    template<typename T, EThreadSafety thread_safety>
    [[nodiscard]]
    HD_FORCEINLINE constexpr bool operator!=(hud::ptr::null, const SharedPointer<T, thread_safety>& pointer) noexcept {
        return static_cast<bool>(pointer);
    }

    /**
    * Checks whether the first SharedPointer owned pointer address less than the second SharedPointer owned pointer address.
    * @tparam T Type of the first SharedPointer's pointer
    * @tparam thread_safety The thread safety of first SharePointer
    * @tparam U Type of the second SharedPointer's pointer
    * @tparam thread_safety_u The thread safety of second SharePointer
    * @param first The first to compare
    * @param second The second to compare
    * @return true if the first SharedPointer owned pointer address less than the second SharedPointer owned pointer address, false otherwise
    */
    template<typename T, EThreadSafety thread_safety, typename U, EThreadSafety thread_safety_u>
    [[nodiscard]]
    HD_FORCEINLINE constexpr bool operator<(const SharedPointer<T, thread_safety>& first, const SharedPointer<U, thread_safety_u>& second) noexcept {
        using PointerType = CommonTypeT<typename SharedPointer<T, thread_safety>::PointerType, typename SharedPointer<U, thread_safety>::PointerType>;
        return Less<PointerType>()(first.pointer(), second.pointer());
    }

    /**
    * Checks whether the SharedPointer owned pointer address is less than nullptr.
    * @tparam T Type of the SharedPointer's pointer
    * @tparam thread_safety The thread safety of SharePointer
    * @param pointer The pointer to compare
    * @param nullptr
    * @return true if the SharedPointer owned pointer address is less than nullptr, false otherwise
    */
    template<typename T, EThreadSafety thread_safety>
    [[nodiscard]]
    HD_FORCEINLINE constexpr bool operator<(const SharedPointer<T, thread_safety>& pointer, hud::ptr::null) noexcept {
        return Less<typename SharedPointer<T, thread_safety>::PointerType>()(pointer.pointer(), nullptr);
    }

    /**
    * Checks whether nullptr is less than the SharedPointer owned pointer address.
    * @tparam T Type of the SharedPointer's pointer
    * @tparam thread_safety The thread safety of SharePointer
    * @param nullptr
    * @param pointer The pointer to compare
    * @return true if nullptr is less than the SharedPointer owned pointer address, false otherwise
    */
    template<typename T, EThreadSafety thread_safety>
    [[nodiscard]]
    HD_FORCEINLINE constexpr bool operator<(hud::ptr::null, const SharedPointer<T, thread_safety>& pointer) noexcept {
        return Less<typename SharedPointer<T, thread_safety>::PointerType>()(nullptr, pointer.pointer());
    }

    /**
    * Checks whether the first SharedPointer owned pointer address greater than the second SharedPointer owned pointer address.
    * @tparam T Type of the first SharedPointer's pointer
    * @tparam thread_safety The thread safety of first SharePointer
    * @tparam U Type of the second SharedPointer's pointer
    * @tparam thread_safety_u The thread safety of second SharePointer
    * @param first The first to compare
    * @param second The second to compare
    * @return true if the first SharedPointer owned pointer address greater than the second SharedPointer owned pointer address, false otherwise
    */
    template<typename T, EThreadSafety thread_safety, typename U, EThreadSafety thread_safety_u>
    [[nodiscard]]
    HD_FORCEINLINE constexpr bool operator>(const SharedPointer<T, thread_safety>& first, const SharedPointer<U, thread_safety_u>& second) noexcept {
        return second < first;
    }

    /**
    * Checks whether the SharedPointer owned pointer address is greater than nullptr.
    * @tparam T Type of the SharedPointer's pointer
    * @tparam thread_safety The thread safety of SharePointer
    * @param pointer The pointer to compare
    * @param nullptr
    * @return true if the SharedPointer owned pointer address is greater than nullptr, false otherwise
    */
    template<typename T, EThreadSafety thread_safety>
    [[nodiscard]]
    HD_FORCEINLINE constexpr bool operator>(const SharedPointer<T, thread_safety>& pointer, hud::ptr::null) noexcept {
        return nullptr < pointer;
    }

    /**
    * Checks whether nullptr is greater than the SharedPointer owned pointer address.
    * @tparam T Type of the SharedPointer's pointer
    * @tparam thread_safety The thread safety of SharePointer
    * @param nullptr
    * @param pointer The pointer to compare
    * @return true if nullptr is greater than the SharedPointer owned pointer address, false otherwise
    */
    template<typename T, EThreadSafety thread_safety>
    [[nodiscard]]
    HD_FORCEINLINE constexpr bool operator>(hud::ptr::null, const SharedPointer<T, thread_safety>& pointer) noexcept {
        return pointer < nullptr;
    }

    /**
    * Checks whether the first owned SharedPointer address less or equal the second SharedPointer owned pointer address.
    * @tparam T Type of the first SharedPointer's pointer
    * @tparam thread_safety The thread safety of first SharePointer
    * @tparam U Type of the second SharedPointer's pointer
    * @tparam thread_safety_u The thread safety of second SharePointer
    * @param first The first to compare
    * @param second The second to compare
    * @return true if the first owned SharedPointer address less or equal the second SharedPointer owned pointer address, false otherwise
    */
    template<typename T, EThreadSafety thread_safety, typename U, EThreadSafety thread_safety_u>
    [[nodiscard]]
    HD_FORCEINLINE constexpr bool operator<=(const SharedPointer<T, thread_safety>& first, const SharedPointer<U, thread_safety_u>& second) noexcept {
        return !(second < first);
    }

    /**
    * Checks whether the SharedPointer owned pointer address is less or equal nullptr.
    * @tparam T Type of the SharedPointer's pointer
    * @tparam thread_safety The thread safety of SharePointer
    * @param pointer The pointer to compare
    * @param nullptr
    * @return true if the SharedPointer owned pointer address is less or equal nullptr, false otherwise
    */
    template<typename T, EThreadSafety thread_safety>
    [[nodiscard]]
    HD_FORCEINLINE constexpr bool operator<=(const SharedPointer<T, thread_safety>& pointer, hud::ptr::null) noexcept {
        return !(nullptr < pointer);
    }

    /**
    * Checks whether nullptr is less or equal the SharedPointer owned pointer address.
    * @tparam T Type of the SharedPointer's pointer
    * @tparam thread_safety The thread safety of SharePointer
    * @param nullptr
    * @param pointer The pointer to compare
    * @return true if nullptr is less or equal the SharedPointer owned pointer address, false otherwise
    */
    template<typename T, EThreadSafety thread_safety>
    [[nodiscard]]
    HD_FORCEINLINE constexpr bool operator<=(hud::ptr::null, const SharedPointer<T, thread_safety>& pointer) noexcept {
        return !(pointer < nullptr);
    }

    /**
    * Checks whether the first SharedPointer owned pointer address compares greater or equal the second SharedPointer owned pointer address.
    * @tparam T Type of the first SharedPointer's pointer
    * @tparam thread_safety The thread safety of first SharePointer
    * @tparam U Type of the second SharedPointer's pointer
    * @tparam thread_safety_u The thread safety of second SharePointer
    * @param first The first to compare
    * @param second The second to compare
    * @return true if the first SharedPointer owned pointer address compares greater or equal the second SharedPointer owned pointer address, false otherwise
    */
    template<typename T, EThreadSafety thread_safety, typename U, EThreadSafety thread_safety_u>
    [[nodiscard]]
    HD_FORCEINLINE constexpr bool operator>=(const SharedPointer<T, thread_safety>& first, const SharedPointer<U, thread_safety_u>& second) noexcept {
        return !(first < second);
    }

    /**
    * Checks whether the SharedPointer owned pointer address is greater or equal nullptr.
    * @tparam T Type of the SharedPointer's pointer
    * @tparam thread_safety The thread safety of SharePointer
    * @param pointer The pointer to compare
    * @param nullptr
    * @return True if the SharedPointer owned pointer address is greater or equal nullptr, false otherwise
    */
    template<typename T, EThreadSafety thread_safety>
    [[nodiscard]]
    HD_FORCEINLINE constexpr bool operator>=(const SharedPointer<T, thread_safety>& pointer, hud::ptr::null) noexcept {
        return !(pointer < nullptr);
    }

    /**
    * Checks whether nullptr is greater or equal the SharedPointer owned pointer address.
    * @tparam T Type of the SharedPointer's pointer
    * @tparam thread_safety The thread safety of SharePointer
    * @param nullptr
    * @param pointer The pointer to compare
    * @return true if nullptr is greater or equal the SharedPointer owned pointer address, false otherwise
    */
    template<typename T, EThreadSafety thread_safety>
    [[nodiscard]]
    HD_FORCEINLINE constexpr bool operator>=(hud::ptr::null, const SharedPointer<T, thread_safety>& pointer) noexcept {
        return !(nullptr < pointer);
    }

    /**
    * Constructs a SharedPointer that owns a pointer of type T. The arguments are forward to the constructor of T.
    * This overload only participates in overload resolution if T is not an array type
    * @tparam T Type of the SharedPointer's pointer
    * @tparam thread_safety The thread safety of SharePointer
    * @tparam TArgs Types of arguments forward to the T constructor
    * @param args Arguments forward to the T constructor
    * @return SharedPointer<T, thread_safety> pointing to a object of type T construct by passing args arguments to its constructor
    */
    template<typename T, EThreadSafety thread_safety = EThreadSafety::not_safe, typename... TArgs>
    [[nodiscard]]
    HD_FORCEINLINE SharedPointer<T, thread_safety> make_shared(TArgs&&... args) noexcept requires(!IsArrayV<T>) {
        return SharedPointer<T, thread_safety>(new (std::nothrow) details::ReferenceControllerNoDeleter<T, thread_safety>(hud::forward<TArgs>(args)...));
    }

    /**
    * Constructs a SharedPointer that owns a pointer to an array of unknown bound T.
    * This overload only participates in overload resolution if T is an array of unknown bound.
    * @tparam T Type of the SharedPointer's pointer
    * @tparam thread_safety The thread safety of SharePointer
    * @param size Number of T to allocate 
    * @return SharedPointer<T, thread_safety> pointer to an array of type T
    */
    template<typename T, EThreadSafety thread_safety = EThreadSafety::not_safe>
    [[nodiscard]]
    HD_FORCEINLINE SharedPointer<T, thread_safety> make_shared(const usize count) noexcept requires(IsUnboundedArrayV<T>) {
        return SharedPointer<T, thread_safety>(new (std::nothrow) details::ReferenceControllerNoDeleter<T, thread_safety>(count));
    }

    /** Specialization of the hash function for SharedPointer */
    template<typename T, EThreadSafety thread_safety>
    HD_FORCEINLINE u32 hash(const SharedPointer<T, thread_safety>& ptr) noexcept {
        // Simply hash the pointer
        return hash(ptr.pointer());
    }
}





#endif // HD_INC_OSLAYER_SHARED_POINTER_H