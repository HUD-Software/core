#ifndef HD_INC_CORE_SHARED_POINTER_H
#define HD_INC_CORE_SHARED_POINTER_H
#include "../assert.h"
#include "../atomics.h"
#include "../minimal.h"
#include "aligned_buffer.h"
#include "pair.h"

#include "../traits/add_const.h"
#include "../traits/add_pointer.h"
#include "../traits/common_type.h"
#include "../traits/conjunction.h"
#include "../traits/disjunction.h"
#include "../traits/is_convertible.h"
#include "../traits/is_nothrow_default_constructible.h"
#include "../traits/is_pointer.h"
#include "../traits/is_same.h"
#include "../traits/is_unbounded_array.h"
#include "../traits/is_void.h"
#include "../traits/negation.h"
#include "../traits/remove_pointer.h"
#include "../traits/void_t.h"

#include "../allocators/allocation.h"
#include "../hash.h"
#include "../templates/declval.h"
#include "../templates/default_deleter.h"
#include "../templates/forward.h"
#include "../templates/less.h"
#include "../templates/select_deleter_pointer_type.h"

/*
@startuml
class reference_controller_base<thread_safety_e> {
u32 shared_count
u32 weak_count
}
class reference_controller_with_deleter<T, thread_safety_e, deleter> {
 T* pointer
}
class reference_controller_no_deleter<T, thread_safety_e, deleter> {
 [sizeof(T)] buffer
}
class shared_reference_controller<thread_safety_e>
class shared_pointer<T>

reference_controller_base <|-- reference_controller_with_deleter
reference_controller_base <|-- reference_controller_no_deleter

shared_reference_controller o- reference_controller_base
shared_pointer *- shared_reference_controller
@enduml

*/
namespace hud
{

    /**
     * shared_pointer thread safety enumeration.
     * Defines how a shared_pointer support thread safety.
     */
    enum class thread_safety_e
    {
        /** hud::atomic synchronisation is used by shared_pointer to modify counters. */
        safe,
        /** No thread synchronisation is used by shared_pointer to modify counters. */
        not_safe
    };

    /** Forward declaration, default thread safety is not safe. */
    template<typename type_t, thread_safety_e thread_safety = thread_safety_e::not_safe>
    class shared_pointer;

    namespace details
    {

        /**
         * The base class of the reference controller allocated on the heap shared with all shared_pointer and WeakPointer referencing the same pointer.
         * @tparam thread_safety The reference counting thread safety to use while counting shared and weak references
         */
        template<thread_safety_e thread_safety>
        class reference_controller_base;

        /** The not thread safe base class of the reference controller allocated on the heap shared with all shared_pointer and WeakPointer referencing the same pointer. */
        template<>
        class reference_controller_base<thread_safety_e::not_safe>
        {

        public:
            /** Default constructor. */
            constexpr reference_controller_base() noexcept = default;

            /** Destructor. */
            constexpr virtual ~reference_controller_base() = default;

            /** Override this function to destroyed the owned object. */
            constexpr virtual void destroy_object() = 0;

            /** Retrieves the shared counter. */
            [[nodiscard]] constexpr u32 get_shared_count() const noexcept
            {
                return shared_count;
            }

            /**
             * Increment the shared counter of a reference_controller_base by 1.
             * @param controller The reference_controller_base
             */
            static constexpr void acquire_sharedref(reference_controller_base *controller) noexcept
            {
                ++(controller->shared_count);
            }

            /**
             * Increment the weak counter of a reference_controller_base by 1.
             * @param controller The reference_controller_base
             */
            static constexpr void acquire_weakref(reference_controller_base *controller) noexcept
            {
                ++(controller->weak_count);
            }

            /**
             * Release a shared reference decrement the shared counter of a reference_controller_base by 1.
             * If the shared reference count is decremented to 0 the object is destroyed then the weak counter is released.
             * @param controller The reference_controller_base to decrement shared counter
             */
            static constexpr void release_sharedref(reference_controller_base *controller) noexcept
            {
                check(controller->shared_count > 0u);

                if (--(controller->shared_count) == 0u)
                {
                    // When reach 0 shared reference, destroy the object
                    controller->destroy_object();

                    // Release the weak count by 1, when the weak count will reach 0, the object will be destroyed
                    release_weakref(controller);
                }
            }

            /**
             * Release a weak reference decrement the weak counter of a reference_controller_base by 1.
             * If the weak reference count is decremented to 0 the controller is deleted.
             * @controller The reference_controller_base to decrement shared counter
             */
            static constexpr void release_weakref(reference_controller_base *controller) noexcept
            {
                check(controller->weak_count > 0u);

                if (--(controller->weak_count) == 0u)
                {
                    delete controller;
                }
            }

        private:
            /** Not copy constructible. */
            reference_controller_base(const reference_controller_base &) = delete;
            /** Not copy assignable. */
            reference_controller_base &operator=(const reference_controller_base &) = delete;

        private:
            /** Count of shared references. */
            u32 shared_count = 1u;
            /** Count of weak references. */
            u32 weak_count = 1u;
        };

        /**
         * The thread safe base class of the reference controller allocated on the heap.
         * Shared with all shared_pointer and WeakPointer referencing the same pointer.
         */
        template<>
        class reference_controller_base<thread_safety_e::safe>
        {

        public:
            /** Default constructor. */
            constexpr reference_controller_base() noexcept = default;

            /** Destructor. */
            constexpr virtual ~reference_controller_base() = default;

            /** Override this function to destroyed the owned object. */
            constexpr virtual void destroy_object() = 0;

            /** Retrieves the shared counter. */
            [[nodiscard]] u32 get_shared_count() const noexcept
            {
                return shared_count.load();
            }

            /**
             * Increment the shared counter of a reference_controller_base by 1.
             * @param controller The reference_controller_base
             */
            static void acquire_sharedref(reference_controller_base *controller) noexcept
            {
                controller->shared_count.increment();
            }

            /**
             * Increment the weak counter of a reference_controller_base by 1.
             * @param controller The reference_controller_base
             */
            static void acquire_weakref(reference_controller_base *controller) noexcept
            {
                controller->weak_count.increment();
            }

            /**
             * Release a shared reference decrement the shared counter of a reference_controller_base by 1.
             * If the shared reference count is decremented to 0 the object is destroyed then the weak counter is released.
             * @param controller The reference_controller_base to decrement shared counter
             */
            static void release_sharedref(reference_controller_base *controller) noexcept
            {
                check(controller->shared_count.load() > 0u);

                if (controller->shared_count.decrement() == 0u)
                {
                    // When reach 0 shared reference, destroy the object
                    controller->destroy_object();

                    // Release the weak count by 1, when the weak count will reach 0, the object will be destroyed
                    release_weakref(controller);
                }
            }

            /**
             * Release a weak reference decrement the weak counter of a reference_controller_base by 1.
             * If the weak reference count is decremented to 0 the controller is deleted.
             * @param controller The reference_controller_base to decrement shared counter
             */
            static void release_weakref(reference_controller_base *controller) noexcept
            {
                check(controller->weak_count.load() > 0u);

                if (controller->weak_count.decrement() == 0u)
                {
                    delete controller;
                }
            }

        private:
            /** Not copy constructible. */
            reference_controller_base(const reference_controller_base &) = delete;
            /** Not copy assignable. */
            reference_controller_base &operator=(const reference_controller_base &) = delete;

        private:
            /** Count of shared references. */
            hud::atomic<u32> shared_count = 1u;
            /** Count of weak references. */
            hud::atomic<u32> weak_count = 1u;
        };

        /**
         * The reference controller that keep the pointer to the heap allocated type_t.
         * @tparam type_t Type of the pointer to own
         * @tparam thread_safety The reference counting thread safety to use while counting shared and weak references
         * @tparam deleter_t Type of the deleter to use when the object is destroyed
         */
        template<typename type_t, thread_safety_e thread_safety, typename deleter_t>
        class reference_controller_with_deleter
            : public reference_controller_base<thread_safety>
        {

        public:
            /** Construct a reference_controller_with_deleter from a pointer to own. */
            explicit constexpr reference_controller_with_deleter(type_t *ptr) noexcept
                : pointer(ptr)
            {
            }

            /** Destroy the object by calling the deleter on the owned pointer. */
            constexpr void destroy_object() noexcept final
            {
                deleter_t deleter;
                deleter(pointer);
            }

        private:
            /** Not copy constructible. */
            reference_controller_with_deleter(const reference_controller_with_deleter &) = delete;
            /** Not copy assignable. */
            reference_controller_with_deleter operator=(const reference_controller_with_deleter &) = delete;

        private:
            /** The pointer to delete. */
            type_t *pointer;
        };

        /**
         * The reference controller that contains type_t instead of referencing a pointer.
         * @tparam type_t Type of the pointer to own
         * @tparam thread_safety The reference counting thread safety to use while counting shared and weak references
         */
        template<typename type_t, thread_safety_e thread_safety>
        class reference_controller_no_deleter
            : public reference_controller_base<thread_safety>
        {

        public:
            /**
             * Construct a reference_controller_no_deleter from a list or arguments forward to the type_t constructor.
             * In-place constructor type_t with the given arguments.
             * @tparam Args Types of arguments forward to the type_t constructor
             * @param args Arguments forward to the type_t constructor
             */
            template<typename... args_t>
            requires(!is_array_v<type_t>)
            explicit constexpr reference_controller_no_deleter(args_t &&...args) noexcept
            {
                static_assert(is_nothrow_constructible_v<type_t, args_t...>, "shared_pointer do not accept throwable constructible type");
                hud::memory::construct_at(pointer(), hud::forward<args_t>(args)...);
            }

            /** Retrieves a pointer to the type_t object. */
            HD_FORCEINLINE type_t *pointer() noexcept
            {
                return buffer.template pointer_as<type_t>();
            }

            /** Destroy the object by calling the destructor. */
            constexpr void destroy_object() noexcept final
            {
                hud::memory::destroy(*pointer());
            }

        private:
            /** Not copy constructible. */
            reference_controller_no_deleter(const reference_controller_no_deleter &) = delete;
            /** Not copy assignable. */
            reference_controller_no_deleter operator=(const reference_controller_no_deleter &) = delete;

        private:
            /** The aligned buffer containing type_t */
            aligned_buffer<sizeof(type_t), alignof(type_t)> buffer;
        };

        /**
         * The reference controller that contains an aligned pointer to an array of type_t.
         * @tparam type_t Type of the pointer to own
         * @tparam thread_safety The reference counting thread safety to use while counting shared and weak references
         */
        template<typename type_t, thread_safety_e thread_safety>
        class reference_controller_no_deleter<type_t[], thread_safety>
            : public reference_controller_base<thread_safety>
        {

        public:
            /** Construct a reference_controller_with_deleter from a pointer to own. */
            explicit constexpr reference_controller_no_deleter(usize count) noexcept
                : allocation(hud::memory::allocate_align<type_t>(count * sizeof(type_t), alignof(type_t)), count)
            {
                hud::memory::default_construct_array(allocation.data(), allocation.data_end());
            }

            /** Retrieves a pointer to the type_t object. */
            constexpr type_t *pointer() noexcept
            {
                return allocation.data();
            }

            /** Destroy the object by calling the destructor. */
            constexpr void destroy_object() noexcept final
            {
                hud::memory::destroy_array(pointer(), allocation.count());
                hud::memory::free_align(pointer());
            }

        private:
            /** Not copy constructible. */
            reference_controller_no_deleter(const reference_controller_no_deleter &) = delete;
            /** Not copy assignable. */
            reference_controller_no_deleter operator=(const reference_controller_no_deleter &) = delete;

        private:
            /** The allocation of the array. */
            hud::allocation<type_t> allocation;
        };

        template<thread_safety_e thread_safety>
        class weak_reference_controller;

        /**
         * The reference controller use to count reference of a shared_pointer.
         * @tparam thread_safety The reference counting thread safety to use while counting shared and weak references
         */
        template<thread_safety_e thread_safety>
        class shared_reference_controller
        {

        public:
            /**  reference_controller_base syntax sugar. */
            using reference_controller_base_type = reference_controller_base<thread_safety>;

        public:
            /** Default constrcutor. */
            constexpr shared_reference_controller() noexcept = default;

            /** Construct a shared_reference_controller by sharing ownership of the controller to control. */
            constexpr explicit shared_reference_controller(reference_controller_base_type *controller_base) noexcept
                : controller(controller_base)
            {
            }

            /** Construct a shared_reference_controller by sharing the ownership of the controller and acquire a shared reference on it. */
            constexpr shared_reference_controller(const shared_reference_controller &other) noexcept
                : controller(other.controller)
            {
                // Increment the counter if the copied reference controller have a associated object
                if (controller != nullptr)
                {
                    reference_controller_base_type::acquire_sharedref(controller);
                }
            }

            /** Construct a shared_reference_controller by stealing the controller. */
            constexpr shared_reference_controller(shared_reference_controller &&other) noexcept
                : controller(other.controller)
            {
                other.controller = nullptr;
            }

            /** Construct a shared_reference_controller from a weak_reference_controller and acquire a shared reference on it. */
            constexpr shared_reference_controller(const weak_reference_controller<thread_safety> &other) noexcept
                : controller(other.controller)
            {
                if (controller != nullptr)
                {
                    reference_controller_base_type::acquire_sharedref(controller);
                }
            }

            /**
             * Assigns the shared_reference_controller by sharing ownership the controller and acquire a shared reference on
             * the newly shared controller before releasing a shared reference on the old one.
             */
            constexpr shared_reference_controller &operator=(const shared_reference_controller &other) noexcept
            {
                // Do not take into account if we assign if the same controller
                if (controller != other.controller)
                {
                    // We take acquire a reference on the copied controller
                    if (other.controller != nullptr)
                    {
                        reference_controller_base_type::acquire_sharedref(other.controller);
                    }
                    // We release our controller
                    else if (controller != nullptr)
                    {
                        reference_controller_base_type::release_sharedref(controller);
                    }
                    // Keep the controller
                    controller = other.controller;
                }
                return *this;
            }

            /** Assigns the shared_reference_controller by stealing the controller before releasing a shared reference on the old one. */
            constexpr shared_reference_controller &operator=(shared_reference_controller &&other) noexcept
            {
                // Do not take into account if we assign if the same controller
                if (controller != other.controller)
                {
                    // Move the controller
                    reference_controller_base_type *old_controller = controller;
                    controller = other.controller;
                    other.controller = nullptr;

                    // Remove the reference on the old controller
                    if (old_controller != nullptr)
                    {
                        reference_controller_base_type::release_sharedref(old_controller);
                    }
                }
                return *this;
            }

            /** Destructor. Release a shared reference on the owned controller. */
            constexpr ~shared_reference_controller()
            {
                if (controller != nullptr)
                {
                    reference_controller_base_type::release_sharedref(controller);
                }
            }

            /** Retrieves the shared counter. */
            [[nodiscard]] constexpr u32 shared_count() const noexcept
            {
                return controller != nullptr ? controller->get_shared_count() : 0u;
            }

        private:
            template<thread_safety_e>
            friend class weak_reference_controller;

        private:
            /** Pointer to the owned reference_controller_base_type. */
            reference_controller_base_type *controller = nullptr;
        };

        /**
         * The reference controller use to count reference of a weak_pointer.
         * @tparam thread_safety The reference counting thread safety to use while counting shared and weak references
         */
        template<thread_safety_e thread_safety>
        class weak_reference_controller
        {
        public:
            /**  reference_controller_base syntax sugar. */
            using reference_controller_base_type = reference_controller_base<thread_safety>;

            /** Default constrcutor. */
            constexpr weak_reference_controller() noexcept = default;

            /** Construct a weak_reference_controller by sharing the controller and acquire a weak reference on it. */
            constexpr weak_reference_controller(const shared_reference_controller<thread_safety> &other) noexcept
                : controller(other.controller)
            {

                // Increment the counter if the copied reference controller have a associated object
                if (controller != nullptr)
                {
                    reference_controller_base_type::acquire_weakref(controller);
                }
            }

            /** Construct a weak_reference_controller by sharing the controller and acquire a weak reference on it. */
            constexpr weak_reference_controller(const weak_reference_controller &other) noexcept
                : controller(other.controller)
            {

                // Increment the counter if the copied reference controller have a associated object
                if (controller != nullptr)
                {
                    reference_controller_base_type::acquire_weakref(controller);
                }
            }

            /** Construct a weak_reference_controller by stealing the controller. */
            constexpr weak_reference_controller(weak_reference_controller &&other) noexcept
                : controller(other.controller)
            {
                other.controller = nullptr;
            }

            /** Destructor. Release a weak reference on the owned controller. */
            constexpr ~weak_reference_controller()
            {
                if (controller != nullptr)
                {
                    reference_controller_base_type::release_weakref(controller);
                }
            }

            /** Assign a weak controller by acquiring a weak reference. */
            constexpr weak_reference_controller &operator=(const weak_reference_controller &other) noexcept
            {
                // We take acquire a weak reference on the copied controller
                if (other.controller != nullptr)
                {
                    reference_controller_base_type::acquire_weakref(other.controller);
                }
                // We take release our controller
                if (controller != nullptr)
                {
                    reference_controller_base_type::release_weakref(controller);
                }
                // Keep the controller of the copied controller
                controller = other.controller;
                return *this;
            }

            /** Assign a weak controller by stealing the controller. */
            constexpr weak_reference_controller &operator=(weak_reference_controller &&other) noexcept
            {
                // We take release our controller
                if (controller != nullptr)
                {
                    reference_controller_base_type::release_weakref(controller);
                }
                // Keep the controller of the copied controller
                controller = other.controller;
                // Remove the controller from the moved one
                other.controller = nullptr;
                return *this;
            }

            /** Assign a shared_reference_controller by sharing the controller and acquire a weak reference on it. */
            constexpr weak_reference_controller &operator=(const shared_reference_controller<thread_safety> &shared_controller) noexcept
            {
                // We take acquire a weak reference on the copied controller
                if (shared_controller.controller != nullptr)
                {
                    reference_controller_base_type::acquire_weakref(shared_controller.controller);
                }
                // We take release our controller
                if (controller != nullptr)
                {
                    reference_controller_base_type::release_weakref(controller);
                }
                controller = shared_controller.controller;
                return *this;
            }

            /**
             * Retrieves the shared counter.
             * This function is available only for unsafe thread weak_pointer.
             * To aquire the shared_count in a thread safe manner take a lock() of the weak_pointer to access the
             */
            [[nodiscard]] constexpr u32 shared_count() const noexcept
            requires(thread_safety == hud::thread_safety_e::not_safe)
            {
                return controller != nullptr ? controller->get_shared_count() : 0u;
            }

        private:
            template<thread_safety_e>
            friend class shared_reference_controller;

        private:
            /** Pointer to the owned reference_controller_base_type. */
            reference_controller_base_type *controller = nullptr;
        };

        /**
         * Checks whether shared_pointer<u_type_t> is convertible to shared_pointer<type_t>
         * @tparam u_type_t Type of the shared_pointer<u_type_t>
         * @tparam type_t Type of the shared_pointer<type_t>
         */
        template<typename u_type_t, typename type_t>
        static constexpr bool is_convertible_v = hud::is_convertible_v<u_type_t *, type_t *>;
        template<typename u_type_t, typename type_t>
        static constexpr bool is_convertible_v<u_type_t, type_t[]> = hud::is_convertible_v<u_type_t (*)[], type_t (*)[]>;
        template<typename u_type_t, typename type_t, usize extent>
        static constexpr bool is_convertible_v<u_type_t, type_t[extent]> = hud::is_convertible_v<u_type_t (*)[extent], type_t (*)[extent]>;

        /**
         * Checks whether a shared_pointer pointer u_type_t is compatible with shared_pointer pointer type_t
         * @tparam u_type_t Type pointer of the shared_pointer<u_type_t>
         * @tparam type_t Type of the shared_pointer<type_t>
         * */
        template<typename u_type_t, typename type_t>
        static constexpr bool is_pointer_compatible_v = hud::is_convertible_v<u_type_t *, type_t *>;
        template<typename u_type_t, usize extent>
        static constexpr bool is_pointer_compatible_v<u_type_t[extent], u_type_t[]> = true;
        template<typename u_type_t, usize extent>
        static constexpr bool is_pointer_compatible_v<u_type_t[extent], const u_type_t[]> = true;
        template<typename u_type_t, usize extent>
        static constexpr bool is_pointer_compatible_v<u_type_t[extent], volatile u_type_t[]> = true;
        template<typename u_type_t, usize extent>
        static constexpr bool is_pointer_compatible_v<u_type_t[extent], const volatile u_type_t[]> = true;
    } // namespace details

    template<typename type_t, hud::thread_safety_e thread_safety = hud::thread_safety_e::not_safe>
    class weak_pointer
    {
    public:
        /** Internal pointer type representation. */
        using pointer_type = hud::remove_extent_t<type_t> *;

        /** Default constructor. */
        constexpr weak_pointer() noexcept = default;

        /**
         * Construct a weak_pointer from a shared_pointer.
         * @tparam u_type_t Type of the shared_pointer to assign. Must be compatible with type_t
         * @param shared_ptr The shared_pointer to assign
         */
        template<typename u_type_t>
        requires(details::is_pointer_compatible_v<u_type_t, type_t>)
        constexpr weak_pointer(const hud::shared_pointer<u_type_t, thread_safety> &shared_ptr) noexcept
            : inner(shared_ptr.inner)
        {
        }

        /** Construct a weak_pointer by copying it. */
        constexpr weak_pointer(const weak_pointer &other) noexcept
            : inner(other.inner)
        {
        }

        /**
         * Construct a weak_pointer by copying it.
         * @tparam u_type_t Type of the other weak_pointer's pointer to copy. Must be compatible with type_t
         * @param other The weak_pointer to copy
         */
        template<typename u_type_t>
        requires(details::is_pointer_compatible_v<u_type_t, type_t>)
        constexpr weak_pointer(const weak_pointer<u_type_t, thread_safety> &other) noexcept
            : inner(other.inner)
        {
        }

        /** Construct a weak_pointer by stealing the pointer. */
        constexpr weak_pointer(weak_pointer &&other) noexcept
            : inner(hud::move(other.inner))
        {
            get<0>(other.inner) = nullptr;
        }

        /**
         * Construct a weak_pointer by moving another weak_pointer.
         * @tparam u_type_t Type of the other weak_pointer's pointer to move. Must be compatible with type_t
         * @param other The weak_pointer to move
         */
        template<typename u_type_t>
        requires(details::is_pointer_compatible_v<u_type_t, type_t>)
        constexpr weak_pointer(weak_pointer<u_type_t, thread_safety> &&other) noexcept
            : inner(hud::move(other.inner))
        {
            get<0>(other.inner) = nullptr;
        }

        /** Assign a weak_pointer by copying it. */
        constexpr weak_pointer &operator=(const weak_pointer &other) noexcept
        {
            if (this != &other)
            {
                inner = other.inner;
            }
            return *this;
        }

        /**
         * Assign a weak_pointer by copying it.
         * @tparam u_type_t Type of the other weak_pointer's pointer to copy. Must be compatible with type_t
         * @param other The weak_pointer to copy
         */
        template<typename u_type_t>
        requires(details::is_pointer_compatible_v<u_type_t, type_t>)
        constexpr weak_pointer &operator=(const weak_pointer<u_type_t, thread_safety> &other) noexcept
        {
            inner = other.inner;
            return *this;
        }

        /** Assign a weak_pointer by stealing the pointer. */
        constexpr weak_pointer &operator=(weak_pointer &&other) noexcept
        {
            if (this != &other)
            {
                inner = hud::move(other.inner);
                get<0>(other.inner) = nullptr;
            }
            return *this;
        }

        /**
         * Assign a weak_pointer by stealing it.
         * @tparam u_type_t Type of the other weak_pointer's pointer to move. Must be compatible with type_t
         * @param other The weak_pointer to copy
         */
        template<typename u_type_t>
        requires(details::is_pointer_compatible_v<u_type_t, type_t>)
        constexpr weak_pointer &operator=(weak_pointer<u_type_t, thread_safety> &&other) noexcept
        {
            inner = hud::move(other.inner);
            get<0>(other.inner) = nullptr;
            return *this;
        }

        /**
         * Assign a weak_pointer from a shared_pointer.
         * @tparam u_type_t Type of the shared_pointer to assign. Must be compatible with type_t
         * @param shared_ptr The shared_pointer to assign
         */
        template<typename u_type_t>
        requires(details::is_pointer_compatible_v<u_type_t, type_t>)
        constexpr weak_pointer &operator=(const hud::shared_pointer<u_type_t> &shared_ptr) noexcept
        {
            inner = shared_ptr.inner;
            return *this;
        }

        /**
         * Retrives a share_pointer that add the weak_pointer a owner.
         * eturns expired() ? shared_ptr<T>() : shared_ptr<T>(*this), executed atomically.
         * */
        [[nodiscard]] constexpr hud::shared_pointer<type_t, thread_safety> lock() const noexcept
        {
            return hud::shared_pointer<type_t, thread_safety> {*this};
        }

        /** Swap with another weak_pointer. */
        constexpr void swap(weak_pointer &other) noexcept
        {
            hud::swap(inner, other.inner);
        }

        /** Release the pointer. */
        constexpr void reset() noexcept
        {
            *this = weak_pointer {};
        }

        /**
         * Retrieves the number of time the pointer is shared with shared_pointer
         * This function is available only for unsafe thread weak_pointer.
         * To acquire the shared_count or access the pointer in a thread safe manner take a lock() of the weak_pointer to access the shared_count
         */
        [[nodiscard]] constexpr u32 shared_count() const noexcept
        requires(thread_safety == hud::thread_safety_e::not_safe)
        {
            return get<1>(inner).shared_count();
        }

        /**
         * Check if the pointer is still valid to be locked
         * This function is available only for unsafe thread weak_pointer.
         * To access the pointer in a thread safe manner take a lock() of the weak_pointer.
         */
        [[nodiscard]] constexpr bool expired() const noexcept
        requires(thread_safety == hud::thread_safety_e::not_safe)
        {
            return shared_count() == 0u;
        }

    private:
        /** Friend with other owning pointer types */
        template<typename u_type_t, thread_safety_e>
        friend class weak_pointer;
        template<typename u_type_t, thread_safety_e>
        friend class shared_pointer;

    private:
        /** Pair containing pointer, deleter and shared reference counter. */
        using inner_type = hud::pair<pointer_type, details::weak_reference_controller<thread_safety>>;
        inner_type inner;
    };

    /**
     * Swap weak_pointer.
     * Same as first.swap(second).
     * @tparam type_t Type of the first weak_pointer's pointer
     * @tparam thread_safety The thread safety of pointers
     * @param first The first to swap
     * @param second The second to swap
     */
    template<typename type_t, thread_safety_e thread_safety>
    HD_FORCEINLINE constexpr void swap(weak_pointer<type_t, thread_safety> &first, weak_pointer<type_t, thread_safety> &second) noexcept
    {
        first.swap(second);
    }

    /**
     * shared_pointer is a smart pointer that share a owning pointer with other shared_pointer.
     * shared_pointer object is automatically delete when no other shared_pointer or WeakPointer share the owned pointer (either by using deleter or just calling destructor) as soon as they themselves are destroyed, or as soon as their value changes either by an assignment operation or by an explicit call to UniquePointer::reset.
     * @tparam type_t The type of the pointer to own
     * @tparam thread_safety The thread safty of the reference counting
     */
    template<typename type_t, thread_safety_e thread_safety>
    class shared_pointer
    {

    public:
        /** Internal pointer type representation. */
        using pointer_type = hud::remove_extent_t<type_t> *;
        /** Internal deleter_t type used to delete the internal pointer. */
        template<typename u_type_t>
        using deleter_type = hud::conditional_t<is_array_v<type_t>, hud::default_deleter<u_type_t[]>, hud::default_deleter<u_type_t>>;

    public:
        /** Default constructor. Do not own pointer. */
        constexpr shared_pointer() noexcept = default;

        /**
         * Construct a shared_pointer form a raw pointer to own.
         * The given pointer must be convertible to type_t*.
         * @param pointer The poiner to own
         */
        template<typename u_type_t>
        requires(details::is_convertible_v<u_type_t, type_t>)
        constexpr explicit shared_pointer(u_type_t *pointer) noexcept
            : inner(pointer, new(std::nothrow) details::reference_controller_with_deleter<u_type_t, thread_safety, deleter_type<u_type_t>>(pointer))
        {
        }

        /** Construct a shared_pointer from a nullptr. Do not own pointer. */
        constexpr shared_pointer(hud::ptr::null) noexcept
        {
        }

        /** Construct a shared_pointer by sharing another shared_pointer ownership. */
        constexpr shared_pointer(const shared_pointer &other) noexcept
            : inner(other.inner)
        {
        }

        /**
         * Construct a shared_pointer by sharing another shared_pointer ownership.
         * @tparam u_type_t Type of the other shared_pointer's pointer to share. Must be compatible with type_t
         * @param other The shared_pointer to share ownership with
         */
        template<typename u_type_t>
        requires(details::is_pointer_compatible_v<u_type_t, type_t>)
        constexpr shared_pointer(const shared_pointer<u_type_t, thread_safety> &other) noexcept
            : inner(other.inner)
        {
        }

        /** Construct a shared_pointer by stealing another shared_pointer ownership. */
        constexpr shared_pointer(shared_pointer &&other) noexcept
            : inner(hud::move(other.inner))
        {
            get<0>(other.inner) = nullptr;
        }

        /**
         * Construct a shared_pointer by stealing another shared_pointer ownership.
         * @tparam u_type_t Type of the shared_pointer's pointer to steal. Must be compatible with type_t
         * @param other The shared_pointer to transfert ownership from.
         */
        template<typename u_type_t>
        requires(details::is_pointer_compatible_v<u_type_t, type_t>)
        constexpr shared_pointer(shared_pointer<u_type_t, thread_safety> &&other) noexcept
            : inner(hud::move(other.inner))
        {
            get<0>(other.inner) = nullptr;
        }

        /**
         * Construct a shared_pointer by adding a owner to a weak_pointer
         * @tparam u_type_t Type of the weak_pointer
         * @param weak The weak_pointer
         */
        template<typename u_type_t>
        constexpr shared_pointer(const hud::weak_pointer<u_type_t, thread_safety> &weak) noexcept
            : inner(weak.inner)
        {
        }

        /** Assign the shared_pointer by stealing the ownership of the other shared_pointer. */
        constexpr shared_pointer &operator=(const shared_pointer &other) noexcept
        {
            if (this != &other)
            {
                inner = other.inner;
            }
            return *this;
        }

        /**
         * Assigns the shared_pointer by sharing the ownership of the other shared_pointer.
         * @tparam u_type_t Type of the other shared_pointer's pointer to share. Must be compatible with type_t
         * @param other The shared_pointer to share ownership
         * @return *this
         */
        template<typename u_type_t>
        requires(details::is_pointer_compatible_v<u_type_t, type_t>)
        constexpr shared_pointer &operator=(const shared_pointer<u_type_t, thread_safety> &other) noexcept
        {
            inner = other.inner;
            return *this;
        }

        /** Assigns the shared_pointer by stealing the ownership of the other pointer. */
        constexpr shared_pointer &operator=(shared_pointer &&other) noexcept
        {
            if (this != &other)
            {
                inner = hud::move(other.inner);
                get<0>(other.inner) = nullptr;
            }
            return *this;
        }

        /**
         * Assigns the shared_pointer by stealing the ownership of the other pointer.
         * @tparam u_type_t Type of the other shared_pointer's pointer to steal. Must be compatible with type_t
         * @param other The shared_pointer to steal
         * @return *this
         */
        template<typename u_type_t>
        requires(details::is_pointer_compatible_v<u_type_t, type_t>)
        constexpr shared_pointer &operator=(shared_pointer<u_type_t, thread_safety> &&other) noexcept
        {
            inner = hud::move(other.inner);
            get<0>(other.inner) = nullptr;
            return *this;
        }

        /** Assigns the shared_pointer with a nullptr. This will no more own a pointer. */
        constexpr shared_pointer &operator=(hud::ptr::null) noexcept
        {
            reset();
            return *this;
        }

        /** Retrieves the own pointer. */
        [[nodiscard]] constexpr pointer_type pointer() const noexcept
        {
            return get<0>(inner);
        }

        /** Check whether the shared_pointer own a pointer. */
        [[nodiscard]] constexpr bool is_owning() const noexcept
        {
            return pointer() != pointer_type {};
        }

        /** Check whether the shared_pointer own a pointer. */
        [[nodiscard]] constexpr explicit operator bool() const noexcept
        {
            return is_owning();
        }

        /** Dereference owned pointer. */
        [[nodiscard]] constexpr hud::add_lvalue_reference_t<hud::remove_extent_t<type_t>> operator*() const noexcept
        requires(!is_void_v<type_t>)
        {
            return *pointer();
        }

        /** Retrieves the owned pointer. */
        [[nodiscard]] constexpr pointer_type operator->() const noexcept
        {
            return pointer();
        }

        /** Retrieves a reference to the element at the given index. */
        [[nodiscard]] constexpr hud::add_const_t<hud::add_lvalue_reference_t<hud::remove_extent_t<type_t>>> operator[](const usize at) const noexcept
        requires(is_array_v<type_t>)
        {
            return pointer()[at];
        }

        /** Retrieves a reference to the element at the given index. */
        [[nodiscard]] constexpr hud::add_lvalue_reference_t<hud::remove_extent_t<type_t>> operator[](const usize at) noexcept
        requires(is_array_v<type_t>)
        {
            return pointer()[at];
        }

        /**
         * Retrieves the number of time the pointer is shared with shared_pointer
         * This function return an approximation when thread_safety is thread_safety_e::safe
         */
        [[nodiscard]] constexpr u32 shared_count() const noexcept
        {
            return get<1>(inner).shared_count();
        }

        /** Destroy the owned pointer and optionally take ownership of a new pointer. */
        constexpr void reset(pointer_type ptr) noexcept
        {
            *this = shared_pointer {ptr};
        }

        /** Destroy the owned pointer and taking no ownership. */
        constexpr void reset(hud::ptr::null) noexcept
        {
            *this = shared_pointer {};
        }

        /** Destroy the owned pointer and taking no ownership. */
        constexpr void reset() noexcept
        {
            reset(nullptr);
        }

        /** Swap with another shared_pointer. */
        constexpr void swap(shared_pointer &other) noexcept
        {
            hud::swap(inner, other.inner);
        }

    private:
        /** Friend with other owning pointer types */
        template<typename u_type_t, thread_safety_e>
        friend class shared_pointer;
        template<typename u_type_t, thread_safety_e>
        friend class weak_pointer;
        template<typename u_type_t, thread_safety_e thread_safety_1, typename... args_t>
        requires(!is_array_v<u_type_t>)
        friend shared_pointer<u_type_t, thread_safety_1> make_shared(args_t &&...args) noexcept;
        template<typename u_type_t, thread_safety_e thread_safety_1>
        requires(hud::is_unbounded_array_v<u_type_t>)
        friend shared_pointer<u_type_t, thread_safety_1> make_shared(usize count) noexcept;
        template<typename u_type_t, thread_safety_e thread_safety_1>
        requires(hud::is_bounded_array_v<u_type_t>)
        friend shared_pointer<u_type_t, thread_safety_1> make_shared(std::initializer_list<hud::remove_extent_t<u_type_t>> init_list) noexcept;

        /**
         * Construct a shared_pointer form a reference_controller_no_deleter.
         * Only used by hud::make_shared(...).
         * @param controller The controller to use
         */
        HD_FORCEINLINE explicit shared_pointer(details::reference_controller_no_deleter<type_t, thread_safety> *controller) noexcept
            : inner(controller->pointer(), controller)
        {
        }

    private:
        /** Pair containing pointer, deleter and shared reference counter. */
        using inner_type = hud::pair<pointer_type, details::shared_reference_controller<thread_safety>>;
        inner_type inner;
    };

    /**
     * Swap shared_pointer.
     * Same as first.swap(second).
     * @tparam type_t Type of the first shared_pointer's pointer
     * @tparam thread_safety The thread safety of pointers
     * @param first The first to swap
     * @param second The second to swap
     */
    template<typename type_t, thread_safety_e thread_safety>
    HD_FORCEINLINE constexpr void swap(shared_pointer<type_t, thread_safety> &first, shared_pointer<type_t, thread_safety> &second) noexcept
    {
        first.swap(second);
    }

    /**
     * Checks whether two shared_pointer owned the same pointer;
     * @tparam type_t Type of the first shared_pointer's pointer
     * @tparam thread_safety The thread safety of first SharePointer
     * @tparam u_type_t Type of the second shared_pointer's pointer
     * @tparam thread_safety_u The thread safety of second SharePointer
     * @param first The first to compare
     * @param second The second to compare
     * @param true if both pointer are equals, false otherwise
     */
    template<typename type_t, thread_safety_e thread_safety, typename u_type_t, thread_safety_e thread_safety_u>
    [[nodiscard]] HD_FORCEINLINE constexpr bool operator==(const shared_pointer<type_t, thread_safety> &first, const shared_pointer<u_type_t, thread_safety_u> &second) noexcept
    {
        return first.pointer() == second.pointer();
    }

    /**
     * Checks whether the shared_pointer don't own pointer.
     * @tparam type_t Type of the shared_pointer's pointer
     * @tparam thread_safety The thread safety of SharePointer
     * @param pointer The shared_pointer to compare with nullptr
     * @param hud::ptr::null
     * @param true if shared_pointer don't own a pointer, false otherwise
     */
    template<typename type_t, thread_safety_e thread_safety>
    [[nodiscard]] HD_FORCEINLINE constexpr bool operator==(const shared_pointer<type_t, thread_safety> &pointer, hud::ptr::null) noexcept
    {
        return !pointer;
    }

    /**
     * Checks whether the shared_pointer don't own pointer.
     * @tparam type_t Type of the shared_pointer's pointer
     * @tparam thread_safety The thread safety of SharePointer
     * @param pointer The shared_pointer to compare with nullptr
     * @param hud::ptr::null
     * @param true if shared_pointer don't own a pointer, false otherwise
     */
    template<typename type_t, thread_safety_e thread_safety>
    [[nodiscard]] HD_FORCEINLINE constexpr bool operator==(hud::ptr::null, const shared_pointer<type_t, thread_safety> &pointer) noexcept
    {
        return !pointer;
    }

    /**
     * Checks whether two shared_pointer owned the different pointers.
     * @tparam type_t Type of the first shared_pointer's pointer
     * @tparam thread_safety The thread safety of first SharePointer
     * @tparam u_type_t Type of the second shared_pointer's pointer
     * @tparam thread_safety_u The thread safety of second SharePointer
     * @param first The first to compare
     * @param second The second to compare
     * @param true if both pointer are not equals, false otherwise
     */
    template<typename type_t, thread_safety_e thread_safety, typename u_type_t, thread_safety_e thread_safety_u>
    [[nodiscard]] HD_FORCEINLINE constexpr bool operator!=(const shared_pointer<type_t, thread_safety> &first, const shared_pointer<u_type_t, thread_safety_u> &second) noexcept
    {
        return first.pointer() != second.pointer();
    }

    /**
    Checks whether the shared_pointer own pointer
    @tparam type_t Type of the shared_pointer's pointer
    @tparam thread_safety The thread safety of SharePointer
    @param pointer The pointer to check
    @param hud::ptr::null
    @param true if shared_pointer own a pointer, false otherwise
    */
    template<typename type_t, thread_safety_e thread_safety>
    [[nodiscard]] HD_FORCEINLINE constexpr bool operator!=(const shared_pointer<type_t, thread_safety> &pointer, hud::ptr::null) noexcept
    {
        return static_cast<bool>(pointer);
    }

    /**
     * Checks whether the shared_pointer own pointer.
     * @tparam type_t Type of the shared_pointer's pointer
     * @tparam thread_safety The thread safety of SharePointer
     * @param hud::ptr::null
     * @param pointer The pointer to check
     * @param true if shared_pointer own a pointer, false otherwise
     */
    template<typename type_t, thread_safety_e thread_safety>
    [[nodiscard]] HD_FORCEINLINE constexpr bool operator!=(hud::ptr::null, const shared_pointer<type_t, thread_safety> &pointer) noexcept
    {
        return static_cast<bool>(pointer);
    }

    /**
     * Checks whether the first shared_pointer owned pointer address less than the second shared_pointer owned pointer address.
     * @tparam type_t Type of the first shared_pointer's pointer
     * @tparam thread_safety The thread safety of first SharePointer
     * @tparam u_type_t Type of the second shared_pointer's pointer
     * @tparam thread_safety_u The thread safety of second SharePointer
     * @param first The first to compare
     * @param second The second to compare
     * @return true if the first shared_pointer owned pointer address less than the second shared_pointer owned pointer address, false otherwise
     */
    template<typename type_t, thread_safety_e thread_safety, typename u_type_t, thread_safety_e thread_safety_u>
    [[nodiscard]] HD_FORCEINLINE constexpr bool operator<(const shared_pointer<type_t, thread_safety> &first, const shared_pointer<u_type_t, thread_safety_u> &second) noexcept
    {
        using pointer_type = common_type_t<typename shared_pointer<type_t, thread_safety>::pointer_type, typename shared_pointer<u_type_t, thread_safety>::pointer_type>;
        return less<pointer_type>()(first.pointer(), second.pointer());
    }

    /**
     * Checks whether the shared_pointer owned pointer address is less than nullptr.
     * @tparam type_t Type of the shared_pointer's pointer
     * @tparam thread_safety The thread safety of SharePointer
     * @param pointer The pointer to compare
     * @param nullptr
     * @return true if the shared_pointer owned pointer address is less than nullptr, false otherwise
     */
    template<typename type_t, thread_safety_e thread_safety>
    [[nodiscard]] HD_FORCEINLINE constexpr bool operator<(const shared_pointer<type_t, thread_safety> &pointer, hud::ptr::null) noexcept
    {
        return less<typename shared_pointer<type_t, thread_safety>::pointer_type>()(pointer.pointer(), nullptr);
    }

    /**
     * Checks whether nullptr is less than the shared_pointer owned pointer address.
     * @tparam type_t Type of the shared_pointer's pointer
     * @tparam thread_safety The thread safety of SharePointer
     * @param nullptr
     * @param pointer The pointer to compare
     * @return true if nullptr is less than the shared_pointer owned pointer address, false otherwise
     */
    template<typename type_t, thread_safety_e thread_safety>
    [[nodiscard]] HD_FORCEINLINE constexpr bool operator<(hud::ptr::null, const shared_pointer<type_t, thread_safety> &pointer) noexcept
    {
        return less<typename shared_pointer<type_t, thread_safety>::pointer_type>()(nullptr, pointer.pointer());
    }

    /**
     * Checks whether the first shared_pointer owned pointer address greater than the second shared_pointer owned pointer address.
     * @tparam type_t Type of the first shared_pointer's pointer
     * @tparam thread_safety The thread safety of first SharePointer
     * @tparam u_type_t Type of the second shared_pointer's pointer
     * @tparam thread_safety_u The thread safety of second SharePointer
     * @param first The first to compare
     * @param second The second to compare
     * @return true if the first shared_pointer owned pointer address greater than the second shared_pointer owned pointer address, false otherwise
     */
    template<typename type_t, thread_safety_e thread_safety, typename u_type_t, thread_safety_e thread_safety_u>
    [[nodiscard]] HD_FORCEINLINE constexpr bool operator>(const shared_pointer<type_t, thread_safety> &first, const shared_pointer<u_type_t, thread_safety_u> &second) noexcept
    {
        return second < first;
    }

    /**
     * Checks whether the shared_pointer owned pointer address is greater than nullptr.
     * @tparam type_t Type of the shared_pointer's pointer
     * @tparam thread_safety The thread safety of SharePointer
     * @param pointer The pointer to compare
     * @param nullptr
     * @return true if the shared_pointer owned pointer address is greater than nullptr, false otherwise
     */
    template<typename type_t, thread_safety_e thread_safety>
    [[nodiscard]] HD_FORCEINLINE constexpr bool operator>(const shared_pointer<type_t, thread_safety> &pointer, hud::ptr::null) noexcept
    {
        return nullptr < pointer;
    }

    /**
     * Checks whether nullptr is greater than the shared_pointer owned pointer address.
     * @tparam type_t Type of the shared_pointer's pointer
     * @tparam thread_safety The thread safety of SharePointer
     * @param nullptr
     * @param pointer The pointer to compare
     * @return true if nullptr is greater than the shared_pointer owned pointer address, false otherwise
     */
    template<typename type_t, thread_safety_e thread_safety>
    [[nodiscard]] HD_FORCEINLINE constexpr bool operator>(hud::ptr::null, const shared_pointer<type_t, thread_safety> &pointer) noexcept
    {
        return pointer < nullptr;
    }

    /**
     * Checks whether the first owned shared_pointer address less or equal the second shared_pointer owned pointer address.
     * @tparam type_t Type of the first shared_pointer's pointer
     * @tparam thread_safety The thread safety of first SharePointer
     * @tparam u_type_t Type of the second shared_pointer's pointer
     * @tparam thread_safety_u The thread safety of second SharePointer
     * @param first The first to compare
     * @param second The second to compare
     * @return true if the first owned shared_pointer address less or equal the second shared_pointer owned pointer address, false otherwise
     */
    template<typename type_t, thread_safety_e thread_safety, typename u_type_t, thread_safety_e thread_safety_u>
    [[nodiscard]] HD_FORCEINLINE constexpr bool operator<=(const shared_pointer<type_t, thread_safety> &first, const shared_pointer<u_type_t, thread_safety_u> &second) noexcept
    {
        return !(second < first);
    }

    /**
     * Checks whether the shared_pointer owned pointer address is less or equal nullptr.
     * @tparam type_t Type of the shared_pointer's pointer
     * @tparam thread_safety The thread safety of SharePointer
     * @param pointer The pointer to compare
     * @param nullptr
     * @return true if the shared_pointer owned pointer address is less or equal nullptr, false otherwise
     */
    template<typename type_t, thread_safety_e thread_safety>
    [[nodiscard]] HD_FORCEINLINE constexpr bool operator<=(const shared_pointer<type_t, thread_safety> &pointer, hud::ptr::null) noexcept
    {
        return !(nullptr < pointer);
    }

    /**
     * Checks whether nullptr is less or equal the shared_pointer owned pointer address.
     * @tparam type_t Type of the shared_pointer's pointer
     * @tparam thread_safety The thread safety of SharePointer
     * @param nullptr
     * @param pointer The pointer to compare
     * @return true if nullptr is less or equal the shared_pointer owned pointer address, false otherwise
     */
    template<typename type_t, thread_safety_e thread_safety>
    [[nodiscard]] HD_FORCEINLINE constexpr bool operator<=(hud::ptr::null, const shared_pointer<type_t, thread_safety> &pointer) noexcept
    {
        return !(pointer < nullptr);
    }

    /**
     * Checks whether the first shared_pointer owned pointer address compares greater or equal the second shared_pointer owned pointer address.
     * @tparam type_t Type of the first shared_pointer's pointer
     * @tparam thread_safety The thread safety of first SharePointer
     * @tparam u_type_t Type of the second shared_pointer's pointer
     * @tparam thread_safety_u The thread safety of second SharePointer
     * @param first The first to compare
     * @param second The second to compare
     * @return true if the first shared_pointer owned pointer address compares greater or equal the second shared_pointer owned pointer address, false otherwise
     */
    template<typename type_t, thread_safety_e thread_safety, typename u_type_t, thread_safety_e thread_safety_u>
    [[nodiscard]] HD_FORCEINLINE constexpr bool operator>=(const shared_pointer<type_t, thread_safety> &first, const shared_pointer<u_type_t, thread_safety_u> &second) noexcept
    {
        return !(first < second);
    }

    /**
     * Checks whether the shared_pointer owned pointer address is greater or equal nullptr.
     * @tparam type_t Type of the shared_pointer's pointer
     * @tparam thread_safety The thread safety of SharePointer
     * @param pointer The pointer to compare
     * @param nullptr
     * @return True if the shared_pointer owned pointer address is greater or equal nullptr, false otherwise
     */
    template<typename type_t, thread_safety_e thread_safety>
    [[nodiscard]] HD_FORCEINLINE constexpr bool operator>=(const shared_pointer<type_t, thread_safety> &pointer, hud::ptr::null) noexcept
    {
        return !(pointer < nullptr);
    }

    /**
     * Checks whether nullptr is greater or equal the shared_pointer owned pointer address.
     * @tparam type_t Type of the shared_pointer's pointer
     * @tparam thread_safety The thread safety of SharePointer
     * @param nullptr
     * @param pointer The pointer to compare
     * @return true if nullptr is greater or equal the shared_pointer owned pointer address, false otherwise
     */
    template<typename type_t, thread_safety_e thread_safety>
    [[nodiscard]] HD_FORCEINLINE constexpr bool operator>=(hud::ptr::null, const shared_pointer<type_t, thread_safety> &pointer) noexcept
    {
        return !(nullptr < pointer);
    }

    /**
     * Constructs a shared_pointer that owns a pointer of type type_t. The arguments are forward to the constructor of type_t.
     * This overload only participates in overload resolution if type_t is not an array type
     * @tparam type_t Type of the shared_pointer's pointer
     * @tparam thread_safety The thread safety of SharePointer
     * @tparam args_t Types of arguments forward to the type_t constructor
     * @param args Arguments forward to the type_t constructor
     * @return shared_pointer<type_t, thread_safety> pointing to a object of type type_t construct by passing args arguments to its constructor
     */
    template<typename type_t, thread_safety_e thread_safety = thread_safety_e::not_safe, typename... args_t>
    requires(!is_array_v<type_t>)
    [[nodiscard]] HD_FORCEINLINE shared_pointer<type_t, thread_safety> make_shared(args_t &&...args) noexcept
    {
        return shared_pointer<type_t, thread_safety>(new (std::nothrow) details::reference_controller_no_deleter<type_t, thread_safety>(hud::forward<args_t>(args)...));
    }

    /**
     * Constructs a shared_pointer that owns a pointer to an array of unknown bound type_t.
     * This overload only participates in overload resolution if type_t is an array of unknown bound.
     * @tparam type_t Type of the shared_pointer's pointer
     * @tparam thread_safety The thread safety of SharePointer
     * @param size Number of type_t to allocate
     * @return shared_pointer<type_t, thread_safety> pointer to an array of type type_t
     */
    template<typename type_t, thread_safety_e thread_safety = thread_safety_e::not_safe>
    requires(hud::is_unbounded_array_v<type_t>)
    [[nodiscard]] HD_FORCEINLINE shared_pointer<type_t, thread_safety> make_shared(const usize count) noexcept
    {
        return shared_pointer<type_t, thread_safety>(new (std::nothrow) details::reference_controller_no_deleter<type_t, thread_safety>(count));
    }

    /** Specialization of the hash function for shared_pointer */
    template<typename type_t, thread_safety_e thread_safety>
    HD_FORCEINLINE u32 hash(const shared_pointer<type_t, thread_safety> &ptr) noexcept
    {
        // Simply hash the pointer
        return hash(ptr.pointer());
    }

} // namespace hud

#endif // HD_INC_CORE_SHARED_POINTER_H