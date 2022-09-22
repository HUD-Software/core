#pragma once
#ifndef HD_INC_CORE_ATOMICS_H
#define HD_INC_CORE_ATOMICS_H

#if defined(HD_OS_WINDOWS)
#include "os_windows/atomics.h"
#elif defined(HD_OS_LINUX)
#include "os_linux/atomics.h"
#else
#error Targeted OS not supported
#endif

namespace hud {

    /** Provides atomics operations. */
    using atomics = 
    #if defined(HD_OS_WINDOWS) 
    os::windows::atomics;
    #elif defined(HD_OS_LINUX)
    os::linux::atomics;
    #else
    #error atomics not implemented for the platform
    #endif
    
    namespace details
    {
        template<typename type_t>
        class atomic_impl;

        /**
        * atomic implementation when type_t is Integral.
        * @tparam type_t The Integral type
        */
        template<typename type_t> requires(is_integral_v<type_t>)
        class atomic_impl<type_t> {

        protected:
            /** Default constructor. Do not value construct the atomic value. */
            constexpr atomic_impl() noexcept {}

            /**
            * Initialization copy constructor.
            * @param value The value to assign
            */
            constexpr atomic_impl(const type_t value) noexcept
                : storage(value) {
            }

        public:
            /**
            * Add the value atomically and return result of the addition.
            * @param value The value to add
            * @param order Synchronization mode for the operation
            * @return Result of the addition
            */
            HD_FORCEINLINE type_t add(const type_t value, atomics::memory_order_e order = atomics::memory_order_e::seq_cst) noexcept {
                return atomics::fetch_add(storage, value, order) + value;
             }
             
            /**
            * Add the value atomically and return result of the addition.
            * @param value The value to add
            * @param order Synchronization mode for the operation
            * @return Result of the addition
            */
             HD_FORCEINLINE type_t add(const type_t value, atomics::memory_order_e order = atomics::memory_order_e::seq_cst) volatile noexcept {
                 return const_cast<atomic_impl*>(this)->add(value, order);
             }

             /**
             * Add the value atomically and return the value of the atomic before the addition.
             * @param value The value to add
             * @param order Synchronization mode for the operation
             * @return Value of the atomic before the addition
             */
             HD_FORCEINLINE type_t fetch_add(const type_t value, atomics::memory_order_e order = atomics::memory_order_e::seq_cst) noexcept {
                 return atomics::fetch_add(storage, value, order);
             }

             /**
             * Add the value atomically and return the value of the atomic before the addition.
             * @param value The value to add
             * @param order Synchronization mode for the operation
             * @return Value of the atomic before the addition
             */
             HD_FORCEINLINE type_t fetch_add(const type_t value, atomics::memory_order_e order = atomics::memory_order_e::seq_cst) volatile noexcept {
                 return const_cast<atomic_impl*>(this)->fetch_add(value, order);
             }

             /**
             * Subtract the value atomically and return result of the substraction.
             * @param value The value to subtract
             * @param order Synchronization mode for the operation
             * @return Result of the substraction
             */
             HD_FORCEINLINE type_t subtract(const type_t value, atomics::memory_order_e order = atomics::memory_order_e::seq_cst) noexcept {
                 return fetch_sub(value, order) - value;
             }

             /**
             * Subtract the value atomically and return result of the substraction.
             * @param value The value to subtract
             * @param order Synchronization mode for the operation
             * @return Result of the substraction
             */
             HD_FORCEINLINE type_t subtract(const type_t value, atomics::memory_order_e order = atomics::memory_order_e::seq_cst) volatile noexcept {
                 return const_cast<atomic_impl*>(this)->subtract(value, order);
             }

             /**
             * Subtract the value atomically and return the value of the atomic before the substraction.
             * @param value The value to subtract
             * @param order Synchronization mode for the operation
             * @return Value of the atomic before the substraction
             */
             HD_FORCEINLINE type_t fetch_sub(const type_t value, atomics::memory_order_e order = atomics::memory_order_e::seq_cst) noexcept {
                 return atomics::fetch_sub(storage, value, order);
             }

             /**
             * Subtract the value atomically and return the value of the atomic before the substraction.
             * @param value The value to subtract
             * @param order Synchronization mode for the operation
             * @return Value of the atomic before the substraction
             */
             HD_FORCEINLINE type_t fetch_sub(const type_t value, atomics::memory_order_e order = atomics::memory_order_e::seq_cst) volatile noexcept {
                 return const_cast<atomic_impl*>(this)->fetch_sub(value, order);
             }

             /** Atomically increment the atomic value with a seq_cst memory order and return the incremented value of the atomic. */
             HD_FORCEINLINE type_t increment(atomics::memory_order_e order = atomics::memory_order_e::seq_cst) noexcept {
                 return add(static_cast<type_t>(1), order);
             }

             /** Atomically increment the atomic value with a seq_cst memory order and return the incremented value of the atomic. */
             HD_FORCEINLINE type_t increment(atomics::memory_order_e order = atomics::memory_order_e::seq_cst) volatile noexcept {
                 return const_cast<atomic_impl*>(this)->increment(order);
             }

             /** Atomically increment the atomic value with a seq_cst memory order and return the value of the atomic before the incrementation. */
             HD_FORCEINLINE type_t fetch_increment(atomics::memory_order_e order = atomics::memory_order_e::seq_cst) noexcept {
                 return fetch_add(static_cast<type_t>(1), order);
             }

             /** Atomically increment the atomic value with a seq_cst memory order and return the value of the atomic before the incrementation. */
             HD_FORCEINLINE type_t fetch_increment(atomics::memory_order_e order = atomics::memory_order_e::seq_cst) volatile noexcept {
                 return const_cast<atomic_impl*>(this)->fetch_increment(order);
             }

             /** Atomically decrement the atomic value with a seq_cst memory order and return the decremented value of the atomic. */
             HD_FORCEINLINE type_t decrement(atomics::memory_order_e order = atomics::memory_order_e::seq_cst) noexcept {
                 return subtract(static_cast<type_t>(1), order);
             }

             /** Atomically decrement the atomic value with a seq_cst memory order and return the decremented value of the atomic. */
             HD_FORCEINLINE type_t decrement(atomics::memory_order_e order = atomics::memory_order_e::seq_cst) volatile noexcept {
                 return const_cast<atomic_impl*>(this)->decrement(order);
             }

             /** Atomically decrement the atomic value with a seq_cst memory order and return the value of the atomic before the decrementation. */
             HD_FORCEINLINE type_t fetch_decrement(atomics::memory_order_e order = atomics::memory_order_e::seq_cst) noexcept {
                 return fetch_sub(static_cast<type_t>(1), order);
             }

             /** Atomically Decrement the atomic value with a seq_cst memory order and return the value of the atomic before the decrementation. */
             HD_FORCEINLINE type_t fetch_decrement(atomics::memory_order_e order = atomics::memory_order_e::seq_cst) volatile noexcept {
                 return const_cast<atomic_impl*>(this)->fetch_decrement(order);
             }

             /** Atomically increment the atomic value with a seq_cst memory order and return the incremented value of the atomic. */
             HD_FORCEINLINE type_t operator++() noexcept {
                 return increment();
             }

             /** Atomically increment the atomic value with a seq_cst memory order and return the incremented value of the atomic. */
             HD_FORCEINLINE type_t operator++() volatile noexcept {
                 return const_cast<atomic_impl*>(this)->operator++();
             }

             /** Atomically increment the atomic value with a seq_cst memory order and return the value of the atomic before the incrementation. */
             HD_FORCEINLINE type_t operator++(i32) noexcept {
                 return fetch_increment();
             }

             /** Atomically increment the atomic value with a seq_cst memory order and return the value of the atomic before the incrementation. */
             HD_FORCEINLINE type_t operator++(i32) volatile noexcept {
                 return const_cast<atomic_impl*>(this)->operator++(i32{});
             }

             /** Atomically decrement the atomic value with a seq_cst memory order and return the decremented value of the atomic. */
             HD_FORCEINLINE type_t operator--() noexcept {
                 return decrement();
             }

             /** Atomically decrement the atomic value with a seq_cst memory order and return the decremented value of the atomic. */
             HD_FORCEINLINE type_t operator--() volatile noexcept {
                 return const_cast<atomic_impl*>(this)->operator--();
             }

             /** Atomically decrement the atomic value with a seq_cst memory order and return the value of the atomic before the decrementation. */
             HD_FORCEINLINE type_t operator--(i32) noexcept {
                 return fetch_decrement();
             }

             /** Atomically decrement the atomic value with a seq_cst memory order and return the value of the atomic before the decrementation. */
             HD_FORCEINLINE type_t operator--(i32) volatile noexcept {
                 return const_cast<atomic_impl*>(this)->operator--(i32{});
             }

             /** Atomically add a value to the atomic value with a seq_cst memory order and return the result of the addition. */
             HD_FORCEINLINE type_t operator+=(const type_t value) noexcept {
                 return add(value);
             }

             /** Atomically add a value to the atomic value with a seq_cst memory order and return the result of the addition. */
             HD_FORCEINLINE type_t operator+=(const type_t value) volatile noexcept {
                 return const_cast<atomic_impl*>(this)->operator+=(value);
             }

             /** Atomically subtract a value to the atomic value with a seq_cst memory order and return the result of the substraction. */
             HD_FORCEINLINE type_t operator-=(const type_t value) noexcept {
                 return subtract(value);
             }

             /** Atomically subtract a value to the atomic value with a seq_cst memory order and return the result of the substraction. */
             HD_FORCEINLINE type_t operator-=(const type_t value) volatile noexcept {
                 return const_cast<atomic_impl*>(this)->operator-=(value);
             }

        protected:
            alignas(sizeof(type_t)) type_t storage;
        };

        template<typename type_t> requires(hud::is_pointer_v<type_t>)
        class atomic_impl<type_t> {

        protected:
            /** Default constructor. Do not value construct the atomic value. */
            constexpr atomic_impl() noexcept {}

            /**
            * Initialization copy constructor.
            * @param value The value to assign
            */
            constexpr atomic_impl(const type_t value) noexcept
                : storage(value) {
            }

        public:
            /**
            * Add the value atomically and return result of the addition.
            * @param value The value to add
            * @param order Synchronization mode for the operation
            * @return Result of the addition
            */
            HD_FORCEINLINE type_t add(const isize value, atomics::memory_order_e order = atomics::memory_order_e::seq_cst) noexcept {
                return atomics::fetch_add(storage, value, order) + value;
            }

            /**
            * Add the value atomically and return result of the addition.
            * @param value The value to add
            * @param order Synchronization mode for the operation
            * @return Result of the addition
            */
            HD_FORCEINLINE type_t add(const isize value, atomics::memory_order_e order = atomics::memory_order_e::seq_cst) volatile noexcept {
                return const_cast<atomic_impl*>(this)->add(value, order);
            }

            /**
            * Add the value atomically and return the value of the atomic before the addition.
            * @param value The value to add
            * @param order Synchronization mode for the operation
            * @return Value of the atomic before the addition
            */
            HD_FORCEINLINE type_t fetch_add(const isize value, atomics::memory_order_e order = atomics::memory_order_e::seq_cst) noexcept {
                return atomics::fetch_add(storage, value, order);
            }

            /**
            * Add the value atomically and return the value of the atomic before the addition.
            * @param value The value to add
            * @param order Synchronization mode for the operation
            * @return Value of the atomic before the addition
            */
            HD_FORCEINLINE type_t fetch_add(const isize value, atomics::memory_order_e order = atomics::memory_order_e::seq_cst) volatile noexcept {
                return const_cast<atomic_impl*>(this)->fetch_add(value, order);
            }

            /**
            * Subtract the value atomically and return result of the substraction.
            * @param value The value to subtract
            * @param order Synchronization mode for the operation
            * @return Result of the substraction
            */
            HD_FORCEINLINE type_t subtract(const isize value, atomics::memory_order_e order = atomics::memory_order_e::seq_cst) noexcept {
                return fetch_sub(value, order) - value;
            }

            /**
            * Subtract the value atomically and return result of the substraction.
            * @param value The value to subtract
            * @param order Synchronization mode for the operation
            * @return Result of the substraction
            */
            HD_FORCEINLINE type_t subtract(const isize value, atomics::memory_order_e order = atomics::memory_order_e::seq_cst) volatile noexcept {
                return const_cast<atomic_impl*>(this)->subtract(value, order);
            }

            /**
            * Subtract the value atomically and return the value of the atomic before the substraction.
            * @param value The value to subtract
            * @param order Synchronization mode for the operation
            * @return Value of the atomic before the substraction
            */
            HD_FORCEINLINE type_t fetch_sub(const isize value, atomics::memory_order_e order = atomics::memory_order_e::seq_cst) noexcept {
                return atomics::fetch_sub(storage, value, order);
            }

            /**
            * Subtract the value atomically and return the value of the atomic before the substraction.
            * @param value The value to subtract
            * @param order Synchronization mode for the operation
            * @return Value of the atomic before the substraction
            */
            HD_FORCEINLINE type_t fetch_sub(const isize value, atomics::memory_order_e order = atomics::memory_order_e::seq_cst) volatile noexcept {
                return const_cast<atomic_impl*>(this)->fetch_sub(value, order);
            }

            /** Atomically increment the atomic value with a seq_cst memory order and return the incremented value of the atomic. */
            HD_FORCEINLINE type_t increment(atomics::memory_order_e order = atomics::memory_order_e::seq_cst) noexcept {
                return add(1, order);
            }

            /** Atomically increment the atomic value with a seq_cst memory order and return the incremented value of the atomic. */
            HD_FORCEINLINE type_t increment(atomics::memory_order_e order = atomics::memory_order_e::seq_cst) volatile noexcept {
                return const_cast<atomic_impl*>(this)->increment(order);
            }

            /** Atomically increment the atomic value with a seq_cst memory order and return the value of the atomic before the incrementation. */
            HD_FORCEINLINE type_t fetch_increment(atomics::memory_order_e order = atomics::memory_order_e::seq_cst) noexcept {
                return fetch_add(1, order);
            }

            /** Atomically increment the atomic value with a seq_cst memory order and return the value of the atomic before the incrementation. */
            HD_FORCEINLINE type_t fetch_increment(atomics::memory_order_e order = atomics::memory_order_e::seq_cst) volatile noexcept {
                return const_cast<atomic_impl*>(this)->fetch_increment(order);
            }

            /** Atomically decrement the atomic value with a seq_cst memory order and return the decremented value of the atomic. */
            HD_FORCEINLINE type_t decrement(atomics::memory_order_e order = atomics::memory_order_e::seq_cst) noexcept {
                return subtract(1, order);
            }

            /** Atomically decrement the atomic value with a seq_cst memory order and return the decremented value of the atomic. */
            HD_FORCEINLINE type_t decrement(atomics::memory_order_e order = atomics::memory_order_e::seq_cst) volatile noexcept {
                return const_cast<atomic_impl*>(this)->decrement(order);
            }

            /** Atomically decrement the atomic value with a seq_cst memory order and return the value of the atomic before the decrementation. */
            HD_FORCEINLINE type_t fetch_decrement(atomics::memory_order_e order = atomics::memory_order_e::seq_cst) noexcept {
                return fetch_sub(1, order);
            }

            /** Atomically Decrement the atomic value with a seq_cst memory order and return the value of the atomic before the decrementation. */
            HD_FORCEINLINE type_t fetch_decrement(atomics::memory_order_e order = atomics::memory_order_e::seq_cst) volatile noexcept {
                return const_cast<atomic_impl*>(this)->fetch_decrement(order);
            }

            /** Atomically increment the atomic value with a seq_cst memory order and return the incremented value of the atomic. */
            HD_FORCEINLINE type_t operator++() noexcept {
                return increment();
            }

            /** Atomically increment the atomic value with a seq_cst memory order and return the incremented value of the atomic. */
            HD_FORCEINLINE type_t operator++() volatile noexcept {
                return const_cast<atomic_impl*>(this)->operator++();
            }

            /** Atomically increment the atomic value with a seq_cst memory order and return the value of the atomic before the incrementation. */
            HD_FORCEINLINE type_t operator++(i32) noexcept {
                return fetch_increment();
            }

            /** Atomically increment the atomic value with a seq_cst memory order and return the value of the atomic before the incrementation. */
            HD_FORCEINLINE type_t operator++(i32) volatile noexcept {
                return const_cast<atomic_impl*>(this)->operator++(i32{});
            }

            /** Atomically decrement the atomic value with a seq_cst memory order and return the decremented value of the atomic. */
            HD_FORCEINLINE type_t operator--() noexcept {
                return decrement();
            }

            /** Atomically decrement the atomic value with a seq_cst memory order and return the decremented value of the atomic. */
            HD_FORCEINLINE type_t operator--() volatile noexcept {
                return const_cast<atomic_impl*>(this)->operator--();
            }

            /** Atomically decrement the atomic value with a seq_cst memory order and return the value of the atomic before the decrementation. */
            HD_FORCEINLINE type_t operator--(i32) noexcept {
                return fetch_decrement();
            }

            /** Atomically decrement the atomic value with a seq_cst memory order and return the value of the atomic before the decrementation. */
            HD_FORCEINLINE type_t operator--(i32) volatile noexcept {
                return const_cast<atomic_impl*>(this)->operator--(i32{});
            }

            /** Atomically add a value to the atomic value with a seq_cst memory order and return the result of the addition. */
            HD_FORCEINLINE type_t operator+=(const isize value) noexcept {
                return add(value);
            }

            /** Atomically add a value to the atomic value with a seq_cst memory order and return the result of the addition. */
            HD_FORCEINLINE type_t operator+=(const isize value) volatile noexcept {
                return const_cast<atomic_impl*>(this)->operator+=(value);
            }

            /** Atomically subtract a value to the atomic value with a seq_cst memory order and return the result of the substraction. */
            HD_FORCEINLINE type_t operator-=(const isize value) noexcept {
                return subtract(value);
            }

            /** Atomically subtract a value to the atomic value with a seq_cst memory order and return the result of the substraction. */
            HD_FORCEINLINE type_t operator-=(const isize value) volatile noexcept {
                return const_cast<atomic_impl*>(this)->operator-=(value);
            }

        protected:
            alignas(type_t) type_t storage;
        };
    }

    template<typename type_t>
    class atomic : public details::atomic_impl<type_t> {
       
    private:
        using base_type = details::atomic_impl<type_t>;

    public:
        /** Default constructor. Do not value construct the atomic value. */
        constexpr atomic() noexcept : base_type() {}

        /** 
        * Constructor that initialize the atomic with a given value.
        * @param value The initialization value
        */
        constexpr atomic(const type_t value) noexcept
            : base_type(value) {
        }
    
        /** Loads the atomic value and return it. Equivalent to load(). */
        operator type_t() const noexcept {
            return load();
        }

        /** Loads the atomic value and return it. Equivalent to load(). */
        operator type_t() const volatile noexcept {
            return const_cast<atomic*>(this)->operator type_t();
        }

        /** Atomically assign a value with a seq_cst memory order and return the assigned value. */
        HD_FORCEINLINE type_t operator=(const type_t value) noexcept {
            store(value);
            return this->storage;
        }

        /** Atomically assign a value with a seq_cst memory order and return the assigned value. */
        HD_FORCEINLINE type_t operator=(const type_t value) volatile noexcept {
            return const_cast<atomic*>(this)->operator=(value);
        }

        /**
        * Store a value to the atomic. memory is affected according to the value of order.
        * @param value The value to store in the atomic
        * @param order The memory ordering fence applied by this operation. Possible values are relaxed, seq_cst and release
        */
        HD_FORCEINLINE void store(const type_t value, atomics::memory_order_e order = atomics::memory_order_e::seq_cst) noexcept {
            atomics::store(this->storage, value, order);
        }

        /**
        * Store a value to the atomic.memory is affected according to the value of order.
        * @param value The value to store in the atomic
        * @param order The memory ordering fence applied by this operation. Possible values are relaxed, seq_cst and release
        */
        HD_FORCEINLINE void store(const type_t value, atomics::memory_order_e order = atomics::memory_order_e::seq_cst) volatile noexcept {
            const_cast<atomic*>(this)->store(value, order);
        }

        /**
        * Loads the atomic value and return it. memory is affected according to the value of order.
        * @param order The memory ordering fence applied by this operation.Possible values are relaxed, seq_cst, consumeand acquire
        * @return The loaded value of the atomic
        */
        HD_FORCEINLINE type_t load(atomics::memory_order_e order = atomics::memory_order_e::seq_cst) const noexcept {
            return atomics::load(this->storage, order);
        }

        /**
        * Loads the atomic value and return it. memory is affected according to the value of order.
        * @param order The memory ordering fence applied by this operation.Possible values are relaxed, seq_cst, consumeand acquire
        * @return The loaded value of the atomic
        */
        HD_FORCEINLINE type_t load(atomics::memory_order_e order = atomics::memory_order_e::seq_cst) const volatile noexcept {
            return const_cast<atomic*>(this)->load(order);
        }

        /**
        * Replaces the atomic value with another value. memory is affected according to the value of order.
        * @param value The replacing value
        * @param order Synchronization mode for the operation
        * @return The value of the atomic before exchange
        */
        HD_FORCEINLINE type_t exchange(const type_t value, atomics::memory_order_e order = atomics::memory_order_e::seq_cst) noexcept {
            return atomics::exchange(this->storage, value, order);
        }

        /**
        * Replaces the atomic value with another value. memory is affected according to the value of order.
        * @param value The replacing value
        * @param order Synchronization mode for the operation
        * @return The value of the atomic before exchange
        */
        HD_FORCEINLINE type_t exchange(const type_t value, atomics::memory_order_e order = atomics::memory_order_e::seq_cst) volatile noexcept {
            return const_cast<atomic*>(this)->exchange(value, order);
        }

        /**
        * Replace the atomic value with another value only if the atomic value equals the expected value.
        * If the atomic value change during this operation resulting of a failure during the comparison this function return false.
        * memory is affected according to the value of order.
        * @param expected The value to compare to destination
        * @param value The value copied to destination if destination == expected
        * @param order The memory ordering fence applied by this operation
        * @return true if the value is replaced, false otherwise.
        */
        HD_FORCEINLINE bool compare_exchange(type_t& expected, const type_t desired, atomics::memory_order_e order = atomics::memory_order_e::seq_cst) noexcept {
            return atomics::compare_and_set(this->storage, expected, desired, order);
        } 
        
        /**
        * Replace the atomic value with another value only if the atomic value equals the expected value.
        * If the atomic value change during this operation resulting of a failure during the comparison this function return false.
        * memory is affected according to the value of order.
        * @param expected The value to compare to destination
        * @param value The value copied to destination if destination == expected
        * @param order The memory ordering fence applied by this operation
        * @return true if the value is replaced, false otherwise.
        */
        HD_FORCEINLINE bool compare_exchange(type_t& expected, const type_t desired, atomics::memory_order_e order = atomics::memory_order_e::seq_cst) volatile noexcept {
            return const_cast<atomic*>(this)->compare_exchange(expected, desired, order);
        }
        
    private:
        /** Not copyable */
        atomic(const atomic&) = delete;
        atomic& operator=(const atomic&) = delete;
        atomic& operator=(const atomic&) volatile = delete;

    };

} // namespace hud

#endif // HD_INC_CORE_ATOMICS_H