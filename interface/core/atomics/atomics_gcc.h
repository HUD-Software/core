#ifndef HD_INC_CORE_ATOMICS_ATOMICS_GCC_H
#define HD_INC_CORE_ATOMICS_ATOMICS_GCC_H
#include "atomics_common.h"
#include "../traits/remove_pointer.h"

namespace hud::gcc
{
    struct atomics
        : public hud::common::atomics
    {

        /**
         * Establishes a multi-thread fence. Prevents the compiler and CPU from reordering certain types of memory operations around it
         * @param order The memory ordering executed by this fence
         */
        static void thread_fence(const memory_order_e order) noexcept
        {
#if HD_HAS_BUILTIN_ATOMIC_THREAD_FENCE
            __atomic_thread_fence(to_gcc_order(order));
#else
    #error hud::os::linux::atomics::thread_fence(memory_order_e) not defined
#endif
        }

        /**
         * Atomically loads the source and returns its value and follows the memory ordering specified.
         * @param source Reference to the source to load
         * @param order The memory ordering fence applied by this operation. Possible values are relaxed, seq_cst, consume and acquire
         * @return The loaded value of source
         */
        template<typename type_t>
        requires(is_integral_v<type_t> || hud::is_pointer_v<type_t>)
        [[nodiscard]] static type_t load(const type_t &source, [[maybe_unused]] const memory_order_e order = memory_order_e::seq_cst) noexcept
        {
            // atomic load do not support release and acq_rel memory fence
            check(order != memory_order_e::release);
            check(order != memory_order_e::acq_rel);
#if HD_HAS_BUILTIN_ATOMIC_LOAD_N
            return __atomic_load_n(&source, to_gcc_order(order));
#else
    #error hud::os::linux::atomics::load(type_t, memory_order_e) not defined
#endif
        }

        /**
         * Atomically store a integral or pointer value to a destination integral or pointer and follows the memory ordering specified.
         * @param destination Reference to the destination
         * @param value The value to store in destination
         * @param order The memory ordering fence applied by this operation. Possible values are relaxed, seq_cst and release
         */
        template<typename type_t>
        requires(is_integral_v<type_t> || hud::is_pointer_v<type_t>)
        static void store(type_t &destination, const type_t value, [[maybe_unused]] const memory_order_e order = memory_order_e::seq_cst) noexcept
        {
            // atomic store do not support consume, acquire and and acq_rel memory fence
            check(order != memory_order_e::consume);
            check(order != memory_order_e::acquire);
            check(order != memory_order_e::acq_rel);
#if HD_HAS_BUILTIN_ATOMIC_STORE_N
            __atomic_store_n(&destination, value, to_gcc_order(order));
#else
    #error hud::os::linux::atomics::store(type_t, type_t, memory_order_e) not defined
#endif
        }

        /**
         * Atomically compare an integral or pointer referenced by the destination integral or pointer parameter with an expected integral or pointer value and copy the value to destination if destination equal expected else nothing is done
         * This operation is a strong compare and exchange that follows the memory ordering specified.
         * @param destination Reference to the destination integral
         * @param expected The integral value to compare to destination
         * @param value The integral value copied to destination if destination == expected
         * @param order The memory ordering fence applied by this operation
         * @return Initial value of the variable referenced by the destination parameter. Equal to expected if the operation is done, new value of destination otherwise.
         */
        template<typename type_t>
        requires(is_integral_v<type_t> || hud::is_pointer_v<type_t>)
        [[nodiscard]] static type_t compare_and_swap(type_t &destination, const type_t &expected, const type_t &value, [[maybe_unused]] const memory_order_e order = memory_order_e::seq_cst) noexcept
        {
            check(hud::memory::is_pointer_aligned(&destination, sizeof(type_t)));
#if HD_HAS_BUILTIN_SYNC_VAL_COMPARE_AND_SWAP
            return __sync_val_compare_and_swap(&destination, expected, value);
#else
    #error hud::os::linux::atomics::compare_and_swap(type_t, type_t, type_t, memory_order_e) not defined
#endif
        }

        /**
         * Atomically compare the value referenced by the destination parameter with an expected value and copy the value to destination if destination equal expected else nothing is done
         * expected is set to the value of destination if the compare_and_set fail
         * This operation follows the memory ordering specified.
         * @param destination Reference to the destination
         * @param expected The value to compare to destination
         * @param value The value copied to destination if destination == expected
         * @param order The memory ordering fence applied by this operation
         * @return true if the value is copied to destination, false otherwise.
         */
        template<typename type_t>
        requires(is_integral_v<type_t> || hud::is_pointer_v<type_t>)
        [[nodiscard]] static bool compare_and_set(type_t &destination, type_t &expected, const type_t &value, [[maybe_unused]] const memory_order_e order = memory_order_e::seq_cst) noexcept
        {
            check(hud::memory::is_pointer_aligned(&destination, sizeof(type_t)));
#if HD_HAS_BUILTIN_ATOMIC_COMPARE_EXCHANGE_N
            return __atomic_compare_exchange_n(&destination, &expected, value, false, to_gcc_order(order), to_gcc_failure_order(order));
#else
    #error hud::os::linux::atomics::compare_and_set(type_t, type_t, type_t, memory_order_e) not defined
#endif
        }

        /**
         * Atomically copy the value to a destination. This operation follows the memory ordering specified.
         * @param destination Reference to the destination
         * @param value The value copied to destination
         * @param order The memory ordering fence applied by this operation
         * @return Initial value of the variable referenced by the destination parameter.
         */
        template<typename type_t>
        requires(is_integral_v<type_t> || hud::is_pointer_v<type_t>)
        [[nodiscard]] static type_t exchange(type_t &destination, const type_t &value, [[maybe_unused]] const memory_order_e order = memory_order_e::seq_cst) noexcept
        {
            check(hud::memory::is_pointer_aligned(&destination, sizeof(type_t)));
#if HD_HAS_BUILTIN_ATOMIC_EXCHANGE_N
            return __atomic_exchange_n(&destination, value, to_gcc_order(order));
#else
    #error hud::os::linux::atomics::exchange(type_t, type_t, memory_order_e) not defined
#endif
        }

        /**
         * Atomically add a value to a value referenced by the addend parameter.
         * This operation follows the memory ordering specified.
         * @param addend The value to be added to; replaced by the result of the addition.
         * @param to_add The value to add
         * @param order The memory ordering fence applied by this operation
         * @return Initial value of the variable referenced by the addend parameter.
         */
        template<typename type_t>
        requires(is_integral_v<type_t>)
        [[nodiscard]] static type_t fetch_add(type_t &addend, const type_t &to_add, [[maybe_unused]] const memory_order_e order = memory_order_e::seq_cst) noexcept
        {
            check(hud::memory::is_pointer_aligned(&addend, sizeof(type_t)));
#if HD_HAS_BUILTIN_ATOMIC_FETCH_ADD
            return __atomic_fetch_add(&addend, to_add, to_gcc_order(order));
#else
    #error hud::os::linux::atomics::fetch_add(type_t, type_t, memory_order_e) not defined
#endif
        }

        /**
         * Atomically add a value to a value referenced by the addend parameter.
         * This operation follows the memory ordering specified.
         * @param addend The value to be added to; replaced by the result of the addition.
         * @param to_add The value to add
         * @param order The memory ordering fence applied by this operation
         * @return Initial value of the variable referenced by the addend parameter.
         */
        template<typename type_t>
        requires(hud::is_pointer_v<type_t>)
        [[nodiscard]] static type_t fetch_add(type_t &addend, const isize to_add, [[maybe_unused]] const memory_order_e order = memory_order_e::seq_cst) noexcept
        {
            check(hud::memory::is_pointer_aligned(&addend, sizeof(type_t)));
#if HD_HAS_BUILTIN_ATOMIC_FETCH_ADD
            const usize ptrdiff_to_add = static_cast<usize>(to_add) * sizeof(hud::remove_pointer_t<type_t>);
            return __atomic_fetch_add(&addend, static_cast<isize>(ptrdiff_to_add), to_gcc_order(order));
#else
    #error hud::os::linux::atomics::fetch_add(type_t, isize, memory_order_e) not defined
#endif
        }

        /**
         * Atomically subtract a value to a value referenced by the subtracted parameter.
         * This operation follows the memory ordering specified.
         * @param subtracted The value to be subtracted to; replaced by the result of the substraction.
         * @param to_subtract The value to subtract
         * @param order The memory ordering fence applied by this operation
         * @return Initial value of the variable referenced by the subtracted parameter.
         */
        template<typename type_t>
        requires(is_integral_v<type_t>)
        [[nodiscard]] static type_t fetch_sub(type_t &subtracted, const type_t &to_subtract, [[maybe_unused]] const memory_order_e order = memory_order_e::seq_cst) noexcept
        {
            check(hud::memory::is_pointer_aligned(&subtracted, sizeof(type_t)));
#if HD_HAS_BUILTIN_ATOMIC_FETCH_SUB
            return __atomic_fetch_sub(&subtracted, to_subtract, to_gcc_order(order));
#else
    #error hud::os::linux::atomics::fetch_sub(type_t, type_t, memory_order_e) not defined
#endif
        }

        /**
         * Atomically subtract a value to a value referenced by the subtracted parameter.
         * This operation follows the memory ordering specified.
         * @param subtracted The value to be subtracted to; replaced by the result of the substraction.
         * @param to_subtract The value to subtract
         * @param order The memory ordering fence applied by this operation
         * @return Initial value of the variable referenced by the subtracted parameter.
         */
        template<typename type_t>
        requires(hud::is_pointer_v<type_t>)
        [[nodiscard]] static type_t fetch_sub(type_t &subtracted, const isize to_subtract, [[maybe_unused]] const memory_order_e order = memory_order_e::seq_cst) noexcept
        {
            check(hud::memory::is_pointer_aligned(&subtracted, sizeof(type_t)));
#if HD_HAS_BUILTIN_ATOMIC_FETCH_SUB
            const usize ptrdiff_to_subtract = static_cast<usize>(to_subtract) * sizeof(hud::remove_pointer_t<type_t>);
            return __atomic_fetch_sub(&subtracted, static_cast<isize>(ptrdiff_to_subtract), to_gcc_order(order));
#else
    #error hud::os::linux::atomics::fetch_sub(type_t, isize, memory_order_e) not defined
#endif
        }

    private:

    private:
        static inline constexpr i32 to_gcc_order(memory_order_e order)
        {
            // Avoid switch statement to make this a constexpr.
            return order == memory_order_e::relaxed ? __ATOMIC_RELAXED : (order == memory_order_e::acquire ? __ATOMIC_ACQUIRE : (order == memory_order_e::release ? __ATOMIC_RELEASE : (order == memory_order_e::seq_cst ? __ATOMIC_SEQ_CST : (order == memory_order_e::acq_rel ? __ATOMIC_ACQ_REL : __ATOMIC_CONSUME))));
        }

        static inline constexpr i32 to_gcc_failure_order(memory_order_e order)
        {
            // Avoid switch statement to make this a constexpr.
            // This memory order cannot be __ATOMIC_RELEASE nor __ATOMIC_ACQ_REL. It also cannot be a stronger order than that specified by success_memorder.
            return order == memory_order_e::relaxed ? __ATOMIC_RELAXED : (order == memory_order_e::acquire ? __ATOMIC_ACQUIRE : (order == memory_order_e::release ? __ATOMIC_RELAXED :                                                                                            // failure order cannot be __ATOMIC_RELEASE
                                                                                                                                                                    (order == memory_order_e::seq_cst ? __ATOMIC_SEQ_CST : (order == memory_order_e::acq_rel ? __ATOMIC_ACQUIRE : // failure order cannot be __ATOMIC_ACQ_REL
                                                                                                                                                                                                                                __ATOMIC_CONSUME))));
        }
    };

} // namespace hud::gcc

#endif // HD_INC_CORE_ATOMICS_ATOMICS_GCC_H