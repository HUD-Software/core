#pragma once
#ifndef HD_INC_OSLAYER_ATOMICS_H
#define HD_INC_OSLAYER_ATOMICS_H

#if defined(HD_OS_WINDOWS)
#include "OSWindows/WindowsAtomics.h"
#elif defined(HD_OS_LINUX)
#include "OSLinux/LinuxAtomics.h"
#else
#error Targeted OS not supported
#endif

namespace hud {

    /** Provides atomics operations. */
    using Atomics = 
    #if defined(HD_OS_WINDOWS) 
    OS::Windows::Atomics;
    #elif defined(HD_OS_LINUX)
    OS::Linux::Atomics;
    #else
    #error Atomics not implemented for the platform
    #endif
    
    namespace details
    {
        template<typename T>
        class AtomicImpl;

        /**
        * Atomic implementation when T is Integral.
        * @tparam T The Integral type
        */
        template<typename T> requires(IsIntegralV<T>)
        class AtomicImpl<T> {

        protected:
            /** Default constructor. Do not value construct the atomic value. */
            constexpr AtomicImpl() noexcept {}

            /**
            * Initialization copy constructor.
            * @param value The value to assign
            */
            constexpr AtomicImpl(const T value) noexcept
                : storage(value) {
            }

        public:
            /**
            * Add the value atomically and return result of the addition.
            * @param value The value to add
            * @param order Synchronization mode for the operation
            * @return Result of the addition
            */
            HD_FORCEINLINE T add(const T value, Atomics::EMemoryOrder order = Atomics::EMemoryOrder::seq_cst) noexcept {
                return Atomics::fetch_add(storage, value, order) + value;
             }
             
            /**
            * Add the value atomically and return result of the addition.
            * @param value The value to add
            * @param order Synchronization mode for the operation
            * @return Result of the addition
            */
             HD_FORCEINLINE T add(const T value, Atomics::EMemoryOrder order = Atomics::EMemoryOrder::seq_cst) volatile noexcept {
                 return const_cast<AtomicImpl*>(this)->add(value, order);
             }

             /**
             * Add the value atomically and return the value of the atomic before the addition.
             * @param value The value to add
             * @param order Synchronization mode for the operation
             * @return Value of the atomic before the addition
             */
             HD_FORCEINLINE T fetch_add(const T value, Atomics::EMemoryOrder order = Atomics::EMemoryOrder::seq_cst) noexcept {
                 return Atomics::fetch_add(storage, value, order);
             }

             /**
             * Add the value atomically and return the value of the atomic before the addition.
             * @param value The value to add
             * @param order Synchronization mode for the operation
             * @return Value of the atomic before the addition
             */
             HD_FORCEINLINE T fetch_add(const T value, Atomics::EMemoryOrder order = Atomics::EMemoryOrder::seq_cst) volatile noexcept {
                 return const_cast<AtomicImpl*>(this)->fetch_add(value, order);
             }

             /**
             * Subtract the value atomically and return result of the substraction.
             * @param value The value to subtract
             * @param order Synchronization mode for the operation
             * @return Result of the substraction
             */
             HD_FORCEINLINE T subtract(const T value, Atomics::EMemoryOrder order = Atomics::EMemoryOrder::seq_cst) noexcept {
                 return fetch_sub(value, order) - value;
             }

             /**
             * Subtract the value atomically and return result of the substraction.
             * @param value The value to subtract
             * @param order Synchronization mode for the operation
             * @return Result of the substraction
             */
             HD_FORCEINLINE T subtract(const T value, Atomics::EMemoryOrder order = Atomics::EMemoryOrder::seq_cst) volatile noexcept {
                 return const_cast<AtomicImpl*>(this)->subtract(value, order);
             }

             /**
             * Subtract the value atomically and return the value of the atomic before the substraction.
             * @param value The value to subtract
             * @param order Synchronization mode for the operation
             * @return Value of the atomic before the substraction
             */
             HD_FORCEINLINE T fetch_sub(const T value, Atomics::EMemoryOrder order = Atomics::EMemoryOrder::seq_cst) noexcept {
                 return Atomics::fetch_sub(storage, value, order);
             }

             /**
             * Subtract the value atomically and return the value of the atomic before the substraction.
             * @param value The value to subtract
             * @param order Synchronization mode for the operation
             * @return Value of the atomic before the substraction
             */
             HD_FORCEINLINE T fetch_sub(const T value, Atomics::EMemoryOrder order = Atomics::EMemoryOrder::seq_cst) volatile noexcept {
                 return const_cast<AtomicImpl*>(this)->fetch_sub(value, order);
             }

             /** Atomically increment the atomic value with a seq_cst memory order and return the incremented value of the atomic. */
             HD_FORCEINLINE T increment(Atomics::EMemoryOrder order = Atomics::EMemoryOrder::seq_cst) noexcept {
                 return add(static_cast<T>(1), order);
             }

             /** Atomically increment the atomic value with a seq_cst memory order and return the incremented value of the atomic. */
             HD_FORCEINLINE T increment(Atomics::EMemoryOrder order = Atomics::EMemoryOrder::seq_cst) volatile noexcept {
                 return const_cast<AtomicImpl*>(this)->increment(order);
             }

             /** Atomically increment the atomic value with a seq_cst memory order and return the value of the atomic before the incrementation. */
             HD_FORCEINLINE T fetch_increment(Atomics::EMemoryOrder order = Atomics::EMemoryOrder::seq_cst) noexcept {
                 return fetch_add(static_cast<T>(1), order);
             }

             /** Atomically increment the atomic value with a seq_cst memory order and return the value of the atomic before the incrementation. */
             HD_FORCEINLINE T fetch_increment(Atomics::EMemoryOrder order = Atomics::EMemoryOrder::seq_cst) volatile noexcept {
                 return const_cast<AtomicImpl*>(this)->fetch_increment(order);
             }

             /** Atomically decrement the atomic value with a seq_cst memory order and return the decremented value of the atomic. */
             HD_FORCEINLINE T decrement(Atomics::EMemoryOrder order = Atomics::EMemoryOrder::seq_cst) noexcept {
                 return subtract(static_cast<T>(1), order);
             }

             /** Atomically decrement the atomic value with a seq_cst memory order and return the decremented value of the atomic. */
             HD_FORCEINLINE T decrement(Atomics::EMemoryOrder order = Atomics::EMemoryOrder::seq_cst) volatile noexcept {
                 return const_cast<AtomicImpl*>(this)->decrement(order);
             }

             /** Atomically decrement the atomic value with a seq_cst memory order and return the value of the atomic before the decrementation. */
             HD_FORCEINLINE T fetch_decrement(Atomics::EMemoryOrder order = Atomics::EMemoryOrder::seq_cst) noexcept {
                 return fetch_sub(static_cast<T>(1), order);
             }

             /** Atomically Decrement the atomic value with a seq_cst memory order and return the value of the atomic before the decrementation. */
             HD_FORCEINLINE T fetch_decrement(Atomics::EMemoryOrder order = Atomics::EMemoryOrder::seq_cst) volatile noexcept {
                 return const_cast<AtomicImpl*>(this)->fetch_decrement(order);
             }

             /** Atomically increment the atomic value with a seq_cst memory order and return the incremented value of the atomic. */
             HD_FORCEINLINE T operator++() noexcept {
                 return increment();
             }

             /** Atomically increment the atomic value with a seq_cst memory order and return the incremented value of the atomic. */
             HD_FORCEINLINE T operator++() volatile noexcept {
                 return const_cast<AtomicImpl*>(this)->operator++();
             }

             /** Atomically increment the atomic value with a seq_cst memory order and return the value of the atomic before the incrementation. */
             HD_FORCEINLINE T operator++(i32) noexcept {
                 return fetch_increment();
             }

             /** Atomically increment the atomic value with a seq_cst memory order and return the value of the atomic before the incrementation. */
             HD_FORCEINLINE T operator++(i32) volatile noexcept {
                 return const_cast<AtomicImpl*>(this)->operator++(i32{});
             }

             /** Atomically decrement the atomic value with a seq_cst memory order and return the decremented value of the atomic. */
             HD_FORCEINLINE T operator--() noexcept {
                 return decrement();
             }

             /** Atomically decrement the atomic value with a seq_cst memory order and return the decremented value of the atomic. */
             HD_FORCEINLINE T operator--() volatile noexcept {
                 return const_cast<AtomicImpl*>(this)->operator--();
             }

             /** Atomically decrement the atomic value with a seq_cst memory order and return the value of the atomic before the decrementation. */
             HD_FORCEINLINE T operator--(i32) noexcept {
                 return fetch_decrement();
             }

             /** Atomically decrement the atomic value with a seq_cst memory order and return the value of the atomic before the decrementation. */
             HD_FORCEINLINE T operator--(i32) volatile noexcept {
                 return const_cast<AtomicImpl*>(this)->operator--(i32{});
             }

             /** Atomically add a value to the atomic value with a seq_cst memory order and return the result of the addition. */
             HD_FORCEINLINE T operator+=(const T value) noexcept {
                 return add(value);
             }

             /** Atomically add a value to the atomic value with a seq_cst memory order and return the result of the addition. */
             HD_FORCEINLINE T operator+=(const T value) volatile noexcept {
                 return const_cast<AtomicImpl*>(this)->operator+=(value);
             }

             /** Atomically subtract a value to the atomic value with a seq_cst memory order and return the result of the substraction. */
             HD_FORCEINLINE T operator-=(const T value) noexcept {
                 return subtract(value);
             }

             /** Atomically subtract a value to the atomic value with a seq_cst memory order and return the result of the substraction. */
             HD_FORCEINLINE T operator-=(const T value) volatile noexcept {
                 return const_cast<AtomicImpl*>(this)->operator-=(value);
             }

        protected:
            alignas(sizeof(T)) T storage;
        };

        template<typename T> requires(IsPointerV<T>)
        class AtomicImpl<T> {

        protected:
            /** Default constructor. Do not value construct the atomic value. */
            constexpr AtomicImpl() noexcept {}

            /**
            * Initialization copy constructor.
            * @param value The value to assign
            */
            constexpr AtomicImpl(const T value) noexcept
                : storage(value) {
            }

        public:
            /**
            * Add the value atomically and return result of the addition.
            * @param value The value to add
            * @param order Synchronization mode for the operation
            * @return Result of the addition
            */
            HD_FORCEINLINE T add(const isize value, Atomics::EMemoryOrder order = Atomics::EMemoryOrder::seq_cst) noexcept {
                return Atomics::fetch_add(storage, value, order) + value;
            }

            /**
            * Add the value atomically and return result of the addition.
            * @param value The value to add
            * @param order Synchronization mode for the operation
            * @return Result of the addition
            */
            HD_FORCEINLINE T add(const isize value, Atomics::EMemoryOrder order = Atomics::EMemoryOrder::seq_cst) volatile noexcept {
                return const_cast<AtomicImpl*>(this)->add(value, order);
            }

            /**
            * Add the value atomically and return the value of the atomic before the addition.
            * @param value The value to add
            * @param order Synchronization mode for the operation
            * @return Value of the atomic before the addition
            */
            HD_FORCEINLINE T fetch_add(const isize value, Atomics::EMemoryOrder order = Atomics::EMemoryOrder::seq_cst) noexcept {
                return Atomics::fetch_add(storage, value, order);
            }

            /**
            * Add the value atomically and return the value of the atomic before the addition.
            * @param value The value to add
            * @param order Synchronization mode for the operation
            * @return Value of the atomic before the addition
            */
            HD_FORCEINLINE T fetch_add(const isize value, Atomics::EMemoryOrder order = Atomics::EMemoryOrder::seq_cst) volatile noexcept {
                return const_cast<AtomicImpl*>(this)->fetch_add(value, order);
            }

            /**
            * Subtract the value atomically and return result of the substraction.
            * @param value The value to subtract
            * @param order Synchronization mode for the operation
            * @return Result of the substraction
            */
            HD_FORCEINLINE T subtract(const isize value, Atomics::EMemoryOrder order = Atomics::EMemoryOrder::seq_cst) noexcept {
                return fetch_sub(value, order) - value;
            }

            /**
            * Subtract the value atomically and return result of the substraction.
            * @param value The value to subtract
            * @param order Synchronization mode for the operation
            * @return Result of the substraction
            */
            HD_FORCEINLINE T subtract(const isize value, Atomics::EMemoryOrder order = Atomics::EMemoryOrder::seq_cst) volatile noexcept {
                return const_cast<AtomicImpl*>(this)->subtract(value, order);
            }

            /**
            * Subtract the value atomically and return the value of the atomic before the substraction.
            * @param value The value to subtract
            * @param order Synchronization mode for the operation
            * @return Value of the atomic before the substraction
            */
            HD_FORCEINLINE T fetch_sub(const isize value, Atomics::EMemoryOrder order = Atomics::EMemoryOrder::seq_cst) noexcept {
                return Atomics::fetch_sub(storage, value, order);
            }

            /**
            * Subtract the value atomically and return the value of the atomic before the substraction.
            * @param value The value to subtract
            * @param order Synchronization mode for the operation
            * @return Value of the atomic before the substraction
            */
            HD_FORCEINLINE T fetch_sub(const isize value, Atomics::EMemoryOrder order = Atomics::EMemoryOrder::seq_cst) volatile noexcept {
                return const_cast<AtomicImpl*>(this)->fetch_sub(value, order);
            }

            /** Atomically increment the atomic value with a seq_cst memory order and return the incremented value of the atomic. */
            HD_FORCEINLINE T increment(Atomics::EMemoryOrder order = Atomics::EMemoryOrder::seq_cst) noexcept {
                return add(1, order);
            }

            /** Atomically increment the atomic value with a seq_cst memory order and return the incremented value of the atomic. */
            HD_FORCEINLINE T increment(Atomics::EMemoryOrder order = Atomics::EMemoryOrder::seq_cst) volatile noexcept {
                return const_cast<AtomicImpl*>(this)->increment(order);
            }

            /** Atomically increment the atomic value with a seq_cst memory order and return the value of the atomic before the incrementation. */
            HD_FORCEINLINE T fetch_increment(Atomics::EMemoryOrder order = Atomics::EMemoryOrder::seq_cst) noexcept {
                return fetch_add(1, order);
            }

            /** Atomically increment the atomic value with a seq_cst memory order and return the value of the atomic before the incrementation. */
            HD_FORCEINLINE T fetch_increment(Atomics::EMemoryOrder order = Atomics::EMemoryOrder::seq_cst) volatile noexcept {
                return const_cast<AtomicImpl*>(this)->fetch_increment(order);
            }

            /** Atomically decrement the atomic value with a seq_cst memory order and return the decremented value of the atomic. */
            HD_FORCEINLINE T decrement(Atomics::EMemoryOrder order = Atomics::EMemoryOrder::seq_cst) noexcept {
                return subtract(1, order);
            }

            /** Atomically decrement the atomic value with a seq_cst memory order and return the decremented value of the atomic. */
            HD_FORCEINLINE T decrement(Atomics::EMemoryOrder order = Atomics::EMemoryOrder::seq_cst) volatile noexcept {
                return const_cast<AtomicImpl*>(this)->decrement(order);
            }

            /** Atomically decrement the atomic value with a seq_cst memory order and return the value of the atomic before the decrementation. */
            HD_FORCEINLINE T fetch_decrement(Atomics::EMemoryOrder order = Atomics::EMemoryOrder::seq_cst) noexcept {
                return fetch_sub(1, order);
            }

            /** Atomically Decrement the atomic value with a seq_cst memory order and return the value of the atomic before the decrementation. */
            HD_FORCEINLINE T fetch_decrement(Atomics::EMemoryOrder order = Atomics::EMemoryOrder::seq_cst) volatile noexcept {
                return const_cast<AtomicImpl*>(this)->fetch_decrement(order);
            }

            /** Atomically increment the atomic value with a seq_cst memory order and return the incremented value of the atomic. */
            HD_FORCEINLINE T operator++() noexcept {
                return increment();
            }

            /** Atomically increment the atomic value with a seq_cst memory order and return the incremented value of the atomic. */
            HD_FORCEINLINE T operator++() volatile noexcept {
                return const_cast<AtomicImpl*>(this)->operator++();
            }

            /** Atomically increment the atomic value with a seq_cst memory order and return the value of the atomic before the incrementation. */
            HD_FORCEINLINE T operator++(i32) noexcept {
                return fetch_increment();
            }

            /** Atomically increment the atomic value with a seq_cst memory order and return the value of the atomic before the incrementation. */
            HD_FORCEINLINE T operator++(i32) volatile noexcept {
                return const_cast<AtomicImpl*>(this)->operator++(i32{});
            }

            /** Atomically decrement the atomic value with a seq_cst memory order and return the decremented value of the atomic. */
            HD_FORCEINLINE T operator--() noexcept {
                return decrement();
            }

            /** Atomically decrement the atomic value with a seq_cst memory order and return the decremented value of the atomic. */
            HD_FORCEINLINE T operator--() volatile noexcept {
                return const_cast<AtomicImpl*>(this)->operator--();
            }

            /** Atomically decrement the atomic value with a seq_cst memory order and return the value of the atomic before the decrementation. */
            HD_FORCEINLINE T operator--(i32) noexcept {
                return fetch_decrement();
            }

            /** Atomically decrement the atomic value with a seq_cst memory order and return the value of the atomic before the decrementation. */
            HD_FORCEINLINE T operator--(i32) volatile noexcept {
                return const_cast<AtomicImpl*>(this)->operator--(i32{});
            }

            /** Atomically add a value to the atomic value with a seq_cst memory order and return the result of the addition. */
            HD_FORCEINLINE T operator+=(const isize value) noexcept {
                return add(value);
            }

            /** Atomically add a value to the atomic value with a seq_cst memory order and return the result of the addition. */
            HD_FORCEINLINE T operator+=(const isize value) volatile noexcept {
                return const_cast<AtomicImpl*>(this)->operator+=(value);
            }

            /** Atomically subtract a value to the atomic value with a seq_cst memory order and return the result of the substraction. */
            HD_FORCEINLINE T operator-=(const isize value) noexcept {
                return subtract(value);
            }

            /** Atomically subtract a value to the atomic value with a seq_cst memory order and return the result of the substraction. */
            HD_FORCEINLINE T operator-=(const isize value) volatile noexcept {
                return const_cast<AtomicImpl*>(this)->operator-=(value);
            }

        protected:
            alignas(T) T storage;
        };
    }

    template<typename T>
    class Atomic : public details::AtomicImpl<T> {
       
    private:
        using Base = details::AtomicImpl<T>;

    public:
        /** Default constructor. Do not value construct the atomic value. */
        constexpr Atomic() noexcept : Base() {}

        /** 
        * Constructor that initialize the atomic with a given value.
        * @param value The initialization value
        */
        constexpr Atomic(const T value) noexcept
            : Base(value) {
        }
    
        /** Loads the atomic value and return it. Equivalent to load(). */
        operator T() const noexcept {
            return load();
        }

        /** Loads the atomic value and return it. Equivalent to load(). */
        operator T() const volatile noexcept {
            return const_cast<Atomic*>(this)->operator T();
        }

        /** Atomically assign a value with a seq_cst memory order and return the assigned value. */
        HD_FORCEINLINE T operator=(const T value) noexcept {
            store(value);
            return this->storage;
        }

        /** Atomically assign a value with a seq_cst memory order and return the assigned value. */
        HD_FORCEINLINE T operator=(const T value) volatile noexcept {
            return const_cast<Atomic*>(this)->operator=(value);
        }

        /**
        * Store a value to the atomic. Memory is affected according to the value of order.
        * @param value The value to store in the atomic
        * @param order The memory ordering fence applied by this operation. Possible values are relaxed, seq_cst and release
        */
        HD_FORCEINLINE void store(const T value, Atomics::EMemoryOrder order = Atomics::EMemoryOrder::seq_cst) noexcept {
            Atomics::store(this->storage, value, order);
        }

        /**
        * Store a value to the atomic.Memory is affected according to the value of order.
        * @param value The value to store in the atomic
        * @param order The memory ordering fence applied by this operation. Possible values are relaxed, seq_cst and release
        */
        HD_FORCEINLINE void store(const T value, Atomics::EMemoryOrder order = Atomics::EMemoryOrder::seq_cst) volatile noexcept {
            const_cast<Atomic*>(this)->store(value, order);
        }

        /**
        * Loads the atomic value and return it. Memory is affected according to the value of order.
        * @param order The memory ordering fence applied by this operation.Possible values are relaxed, seq_cst, consumeand acquire
        * @return The loaded value of the atomic
        */
        HD_FORCEINLINE T load(Atomics::EMemoryOrder order = Atomics::EMemoryOrder::seq_cst) const noexcept {
            return Atomics::load(this->storage, order);
        }

        /**
        * Loads the atomic value and return it. Memory is affected according to the value of order.
        * @param order The memory ordering fence applied by this operation.Possible values are relaxed, seq_cst, consumeand acquire
        * @return The loaded value of the atomic
        */
        HD_FORCEINLINE T load(Atomics::EMemoryOrder order = Atomics::EMemoryOrder::seq_cst) const volatile noexcept {
            return const_cast<Atomic*>(this)->load(order);
        }

        /**
        * Replaces the atomic value with another value. Memory is affected according to the value of order.
        * @param value The replacing value
        * @param order Synchronization mode for the operation
        * @return The value of the atomic before exchange
        */
        HD_FORCEINLINE T exchange(const T value, Atomics::EMemoryOrder order = Atomics::EMemoryOrder::seq_cst) noexcept {
            return Atomics::exchange(this->storage, value, order);
        }

        /**
        * Replaces the atomic value with another value. Memory is affected according to the value of order.
        * @param value The replacing value
        * @param order Synchronization mode for the operation
        * @return The value of the atomic before exchange
        */
        HD_FORCEINLINE T exchange(const T value, Atomics::EMemoryOrder order = Atomics::EMemoryOrder::seq_cst) volatile noexcept {
            return const_cast<Atomic*>(this)->exchange(value, order);
        }

        /**
        * Replace the atomic value with another value only if the atomic value equals the expected value.
        * If the atomic value change during this operation resulting of a failure during the comparison this function return false.
        * Memory is affected according to the value of order.
        * @param expected The value to compare to destination
        * @param value The value copied to destination if destination == expected
        * @param order The memory ordering fence applied by this operation
        * @return true if the value is replaced, false otherwise.
        */
        HD_FORCEINLINE bool compare_exchange(T& expected, const T desired, Atomics::EMemoryOrder order = Atomics::EMemoryOrder::seq_cst) noexcept {
            return Atomics::compare_and_set(this->storage, expected, desired, order);
        } 
        
        /**
        * Replace the atomic value with another value only if the atomic value equals the expected value.
        * If the atomic value change during this operation resulting of a failure during the comparison this function return false.
        * Memory is affected according to the value of order.
        * @param expected The value to compare to destination
        * @param value The value copied to destination if destination == expected
        * @param order The memory ordering fence applied by this operation
        * @return true if the value is replaced, false otherwise.
        */
        HD_FORCEINLINE bool compare_exchange(T& expected, const T desired, Atomics::EMemoryOrder order = Atomics::EMemoryOrder::seq_cst) volatile noexcept {
            return const_cast<Atomic*>(this)->compare_exchange(expected, desired, order);
        }
        
    private:
        /** Not copyable */
        Atomic(const Atomic&) = delete;
        Atomic& operator=(const Atomic&) = delete;
        Atomic& operator=(const Atomic&) volatile = delete;

    };

} // namespace hud

#endif // HD_INC_OSLAYER_ATOMICS_H