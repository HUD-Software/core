#pragma once
#ifndef HD_INC_CORE_OS_WINDOWS_ATOMICS_INTEL_H
#define HD_INC_CORE_OS_WINDOWS_ATOMICS_INTEL_H
#include "../os_common/atomics.h"
#include "windows_api.h"
#include <intrin.h>
#include "../traits/make_unsigned.h"
#include "../traits/is_integral.h"
#include "../traits/is_pointer.h"
#include "../traits/remove_pointer.h"
#include "../memory.h"

namespace hud::os::windows {

    /** Limits the compiler optimizations that can reorder memory accesses across the point of the call. */
#if defined(HD_COMPILER_CLANG_CL)
#define compiler_barrier() \
    _Pragma("clang diagnostic push")\
    _Pragma("clang diagnostic ignored \"-Wdeprecated-declarations\"")\
    _ReadWriteBarrier(); \
    _Pragma("clang diagnostic pop")
#elif defined(HD_COMPILER_MSVC)
#define compiler_barrier() _ReadWriteBarrier();
#endif

    /** Implementation of atomic operations for Intel X86 and X86_64 */
    struct atomics_intel : public os::common::atomics {

        /**
        * Establishes a multi-thread fence. Prevents the compiler and CPU from reordering certain types of memory operations around it
        * @param order The memory ordering executed by this fence
        */
        static void thread_fence(const memory_order_e order) noexcept {
            if (order == memory_order_e::relaxed) {
                return;
            }
            compiler_barrier(); // Compiler barrier for x86/x64
            if (order == memory_order_e::seq_cst) {
                volatile long guard;
                (void)_InterlockedIncrement(&guard);
                compiler_barrier(); // Compiler barrier for x86/x64
            }
        }

        /**
        * Atomically loads the source and returns its value and follows the memory ordering specified.
        * @param source Reference to the source to load
        * @param order The memory ordering fence applied by this operation. Possible values are relaxed, seq_cst, consume and acquire
        * @return The loaded value of source
        */
        template<typename type_t>
        [[nodiscard]]
        static type_t load(const type_t& source, [[maybe_unused]] const memory_order_e order = memory_order_e::seq_cst) noexcept requires(is_integral_v<type_t> || is_pointer_v<type_t>) {
            type_t destination = load_no_barrier(source);

            // Load barrier
            if (order != memory_order_e::relaxed) {
                // atomic load do not support release and acq_rel memory fence
                check(order != memory_order_e::release);
                check(order != memory_order_e::acq_rel);
                compiler_barrier(); // Compiler barrier for x86/x64
            }
            return destination;
        }

        /**
        * Atomically store a integral value to a destination integral and follows the memory ordering specified.
        * @param destination Reference to the destination
        * @param value The value to store in destination
        * @param order The memory ordering fence applied by this operation. Possible values are relaxed, seq_cst and release
        */
        template<typename type_t>
        static void store(type_t& destination, const type_t value, [[maybe_unused]] const memory_order_e order = memory_order_e::seq_cst) noexcept requires(is_integral_v<type_t>) {
            switch (order) {
            case memory_order_e::relaxed:
                store_no_barrier(destination, value);
                return;
            case memory_order_e::release:
                compiler_barrier(); // Compiler barrier for x86/x64
                store_no_barrier(destination, value);
                return;
            default:
            case memory_order_e::consume:
            case memory_order_e::acquire:
            case memory_order_e::acq_rel:
                check(false); // Invalid memory order for store
                [[fallthrough]];
            case memory_order_e::seq_cst:
                check(hud::memory::is_pointer_aligned(&destination, sizeof(type_t)));
                if constexpr (sizeof(type_t) == 1) {
                    _InterlockedExchange8(reinterpret_cast<volatile char*>(&destination), static_cast<char>(value));
                }
                else if constexpr (sizeof(type_t) == 2) {
                    _InterlockedExchange16(reinterpret_cast<volatile short*>(&destination), static_cast<short>(value));
                }
                else if constexpr (sizeof(type_t) == 4) {
                    _InterlockedExchange(reinterpret_cast<volatile long*>(&destination), static_cast<long>(value));
                }
                else if constexpr (sizeof(type_t) == 8) {
#if defined (HD_TARGET_X86)
                    compiler_barrier(); // Compiler barrier for x86/x64
                    __iso_volatile_store64(reinterpret_cast<volatile __int64*>(&destination), static_cast<__int64>(value));
                    thread_fence(atomics::memory_order_e::seq_cst);
#else // HD_TARGET_X64
                    _InterlockedExchange64(reinterpret_cast<volatile long long*>(&destination), static_cast<long long>(value));
#endif
                }
                return;
            }
        }

        /**
        * Atomically store a pointer to a destination pointer and follows the memory ordering specified.
        * @param destination Reference to the destination
        * @param value The value to store in destination
        * @param order The memory ordering fence applied by this operation. Possible values are relaxed, seq_cst and release
        */
        template<typename type_t>
        static void store(type_t& destination, const type_t value, [[maybe_unused]] const memory_order_e order = memory_order_e::seq_cst) noexcept requires(is_pointer_v<type_t>) {
            switch (order) {
            case memory_order_e::relaxed:
                store_no_barrier(destination, value);
                return;
            case memory_order_e::release:
                compiler_barrier(); // Compiler barrier for x86/x64
                store_no_barrier(destination, value);
                return;
            default:
            case memory_order_e::consume:
            case memory_order_e::acquire:
            case memory_order_e::acq_rel:
                check(false); // Invalid memory order for store
                [[fallthrough]];
            case memory_order_e::seq_cst:
                check(hud::memory::is_pointer_aligned(&destination, sizeof(type_t)));
                if constexpr (sizeof(type_t) == 4) {
                    _InterlockedExchange(reinterpret_cast<volatile long*>(&destination), reinterpret_cast<long>(value));
                }
                else if constexpr (sizeof(type_t) == 8) {
                    _InterlockedExchange64(reinterpret_cast<volatile long long*>(&destination), reinterpret_cast<long long>(value));
                }
                return;
            }
        }

        /**
        * Atomically compare an integral referenced by the destination integral parameter with an expected integral value and copy the value to destination if destination equal expected else nothing is done
        * This operation follows the memory ordering specified.
        * @param destination Reference to the destination integral
        * @param expected The integral value to compare to destination
        * @param value The integral value copied to destination if destination == expected
        * @param order The memory ordering fence applied by this operation
        * @return Initial value of the variable referenced by the destination parameter. Equal to expected if the operation is done, new value of destination otherwise.
        */
        template<typename type_t>
        [[nodiscard]]
        static type_t compare_and_swap(type_t& destination, const type_t& expected, const type_t& value, [[maybe_unused]] const memory_order_e order = memory_order_e::seq_cst) noexcept requires(is_integral_v<type_t>) {
            check(hud::memory::is_pointer_aligned(&destination, sizeof(type_t)));
            if constexpr (sizeof(type_t) == 1) {
                return static_cast<type_t>(_InterlockedCompareExchange8(reinterpret_cast<volatile char*>(&destination), static_cast<char>(value), static_cast<char>(expected)));
            }
            else if constexpr (sizeof(type_t) == 2) {
                return static_cast<type_t>(_InterlockedCompareExchange16(reinterpret_cast<volatile short*>(&destination), static_cast<short>(value), static_cast<short>(expected)));
            }
            else if constexpr (sizeof(type_t) == 4) {
                return static_cast<type_t>(_InterlockedCompareExchange(reinterpret_cast<volatile long*>(&destination), static_cast<long>(value), static_cast<long>(expected)));
            }
            else if constexpr (sizeof(type_t) == 8) {
                return static_cast<type_t>(_InterlockedCompareExchange64(reinterpret_cast<volatile long long*>(&destination), static_cast<long long>(value), static_cast<long long>(expected)));
            }
        }

        /**
        * Atomically compare a ponter referenced by the destination parameter with an expected pointer value and copy the value to destination if destination equal expected else nothing is done
        * This operation follows the memory ordering specified.
        * @param destination Reference to the destination pointer
        * @param expected The pointer value to compare to destination
        * @param value The pointer value copied to destination if destination == expected
        * @param order The memory ordering fence applied by this operation
        * @return Initial value of the variable referenced by the destination parameter. Equal to expected if the operation is done, new value of destination otherwise.
        */
        template<typename type_t>
        [[nodiscard]]
        static type_t compare_and_swap(type_t& destination, const type_t& expected, const type_t& value, [[maybe_unused]] const memory_order_e order = memory_order_e::seq_cst) noexcept requires(is_pointer_v<type_t>) {
            check(hud::memory::is_pointer_aligned(&destination, sizeof(type_t)));
            if constexpr (sizeof(type_t) == 4) {
                return reinterpret_cast<type_t>(_InterlockedCompareExchange(reinterpret_cast<volatile long*>(&destination), reinterpret_cast<long>(value), reinterpret_cast<long>(expected)));
            }
            else if constexpr (sizeof(type_t) == 8) {
                return reinterpret_cast<type_t>(_InterlockedCompareExchange64(reinterpret_cast<volatile long long*>(&destination), reinterpret_cast<long long>(value), reinterpret_cast<long long>(expected)));
            }
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
        [[nodiscard]]
        static bool compare_and_set(type_t& destination, type_t& expected, const type_t& value, [[maybe_unused]] const memory_order_e order = memory_order_e::seq_cst) noexcept requires(is_integral_v<type_t>) {
            type_t old = compare_and_swap(destination, expected, value, order);
            if (old == expected) {
                return true;
            }
            expected = old;
            return false;
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
        [[nodiscard]]
        static bool compare_and_set(type_t& destination, type_t& expected, const type_t& value, [[maybe_unused]] const memory_order_e order = memory_order_e::seq_cst) noexcept requires(is_pointer_v<type_t>) {
            type_t old = compare_and_swap(destination, expected, value, order);
            if (old == expected) {
                return true;
            }
            expected = old;
            return false;
        }

        /**
        * Atomically copy the value to a destination. This operation follows the memory ordering specified.
        * @param destination Reference to the destination
        * @param value The value copied to destination
        * @param order The memory ordering fence applied by this operation
        * @return Initial value of the variable referenced by the destination parameter.
        */
        template<typename type_t>
        [[nodiscard]]
        static type_t exchange(type_t& destination, const type_t& value, [[maybe_unused]] const memory_order_e order = memory_order_e::seq_cst) noexcept requires(is_integral_v<type_t>) {
            check(hud::memory::is_pointer_aligned(&destination, sizeof(type_t)));
            if constexpr (sizeof(type_t) == 1) {
                return static_cast<type_t>(_InterlockedExchange8(reinterpret_cast<volatile char*>(&destination), static_cast<char>(value)));
            }
            else if constexpr (sizeof(type_t) == 2) {
                return static_cast<type_t>(_InterlockedExchange16(reinterpret_cast<volatile short*>(&destination), static_cast<short>(value)));
            }
            else if constexpr (sizeof(type_t) == 4) {
                return static_cast<type_t>(_InterlockedExchange(reinterpret_cast<volatile long*>(&destination), static_cast<long>(value)));
            }
            else if constexpr (sizeof(type_t) == 8) {
#if defined(HD_TARGET_X64)
                return static_cast<type_t>(_InterlockedExchange64(reinterpret_cast<volatile long long*>(&destination), static_cast<long long>(value)));
#else //HD_TARGET_X86
                type_t expected_value{ load(destination) };
                while (!compare_and_set(destination, expected_value, value, order)) {}
                return expected_value;
#endif 
            }
        }

        /**
        * Atomically copy the value to a destination. This operation follows the memory ordering specified.
        * @param destination Reference to the destination
        * @param value The value copied to destination
        * @param order The memory ordering fence applied by this operation
        * @return Initial value of the variable referenced by the destination parameter.
        */
        template<typename type_t>
        [[nodiscard]]
        static type_t exchange(type_t& destination, const type_t& value, [[maybe_unused]] const memory_order_e order = memory_order_e::seq_cst) noexcept requires(is_pointer_v<type_t>) {
            check(hud::memory::is_pointer_aligned(&destination, sizeof(type_t)));
            if constexpr (sizeof(type_t) == 4) {
                return reinterpret_cast<type_t>(_InterlockedExchange(reinterpret_cast<volatile long*>(&destination), reinterpret_cast<long>(value)));
            }
            else if constexpr (sizeof(type_t) == 8) {
                return reinterpret_cast<type_t>(_InterlockedExchange64(reinterpret_cast<volatile long long*>(&destination), reinterpret_cast<long long>(value)));
            }
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
        [[nodiscard]]
        static type_t fetch_add(type_t& addend, const type_t& to_add, [[maybe_unused]] const memory_order_e order = memory_order_e::seq_cst) noexcept requires(is_integral_v<type_t>) {
            check(hud::memory::is_pointer_aligned(&addend, sizeof(type_t)));
            if constexpr (sizeof(type_t) == 1) {
                return static_cast<type_t>(_InterlockedExchangeAdd8(reinterpret_cast<volatile char*>(&addend), static_cast<char>(to_add)));
            }
            else if constexpr (sizeof(type_t) == 2) {
                return static_cast<type_t>(_InterlockedExchangeAdd16(reinterpret_cast<volatile short*>(&addend), static_cast<short>(to_add)));
            }
            else if constexpr (sizeof(type_t) == 4) {
                return static_cast<type_t>(_InterlockedExchangeAdd(reinterpret_cast<volatile long*>(&addend), static_cast<long>(to_add)));
            }
            else if constexpr (sizeof(type_t) == 8) {
#if defined(HD_TARGET_X64)
                return static_cast<type_t>(_InterlockedExchangeAdd64(reinterpret_cast<volatile long long*>(&addend), static_cast<long long>(to_add)));
#else //HD_TARGET_X86
                type_t expected_value{ load(addend) };
                while (!compare_and_set(addend, expected_value, expected_value + to_add, order)) {}
                return expected_value;
#endif
            }
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
        [[nodiscard]]
        static type_t fetch_add(type_t& addend, const isize to_add, [[maybe_unused]] const memory_order_e order = memory_order_e::seq_cst) noexcept requires(is_pointer_v<type_t>) {
            check(hud::memory::is_pointer_aligned(&addend, sizeof(type_t)));
            const usize ptrdiff_to_add = static_cast<usize>(to_add) * sizeof(remove_pointer_t<type_t>);
            if constexpr (sizeof(type_t) == 4) {
                return reinterpret_cast<type_t>(_InterlockedExchangeAdd(reinterpret_cast<volatile long*>(&addend), static_cast<long>(ptrdiff_to_add)));
            }
            else if constexpr (sizeof(type_t) == 8) {
                return reinterpret_cast<type_t>(_InterlockedExchangeAdd64(reinterpret_cast<volatile long long*>(&addend), static_cast<long long>(ptrdiff_to_add)));
            }
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
        [[nodiscard]]
        static type_t fetch_sub(type_t& subtracted, const type_t& to_subtract, [[maybe_unused]] const memory_order_e order = memory_order_e::seq_cst) noexcept requires(is_integral_v<type_t>) {
            type_t negated = static_cast<type_t>(0U - static_cast<make_unsigned_t<type_t>>(to_subtract));
            return fetch_add(subtracted, negated, order);
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
        [[nodiscard]]
        static type_t fetch_sub(type_t& subtracted, const isize to_subtract, [[maybe_unused]] const memory_order_e order = memory_order_e::seq_cst) noexcept requires(is_pointer_v<type_t>) {
            return fetch_add(subtracted, -to_subtract, order);
        }

    private:
        /**
        * Atomically store an integral to a integral referenced by the destination reference without memory fence.
        * @param destination Reference to the destination
        * @param value The value to store in destination
        */
        template<typename type_t>
        static void store_no_barrier(type_t& destination, type_t value) noexcept requires(is_integral_v<type_t>)
        {
            check(hud::memory::is_pointer_aligned(&destination, sizeof(type_t)));
            if constexpr (sizeof(type_t) == 1) {
                __iso_volatile_store8(reinterpret_cast<volatile __int8*>(&destination), static_cast<__int8>(value));
            }
            else if constexpr (sizeof(type_t) == 2) {
                __iso_volatile_store16(reinterpret_cast<volatile __int16*>(&destination), static_cast<__int16>(value));
            }
            else if constexpr (sizeof(type_t) == 4) {
                __iso_volatile_store32(reinterpret_cast<volatile __int32*>(&destination), static_cast<__int32>(value));
            }
            else if constexpr (sizeof(type_t) == 8) {
                __iso_volatile_store64(reinterpret_cast<volatile __int64*>(&destination), static_cast<__int64>(value));
            }
        }

        /**
        * Atomically store a pointer address to a pointer referenced by the destination reference without memory fence.
        * @param destination Reference to the destination
        * @param value The pointer address to store in destination
        */
        template<typename type_t>
        static void store_no_barrier(type_t& destination, type_t value) noexcept requires(is_pointer_v<type_t>)
        {
            check(hud::memory::is_pointer_aligned(&destination, sizeof(type_t)));
            if constexpr (sizeof(type_t) == 4) {
                __iso_volatile_store32(reinterpret_cast<volatile __int32*>(&destination), reinterpret_cast<__int32>(value));
            }
            else if constexpr (sizeof(type_t) == 8) {
                __iso_volatile_store64(reinterpret_cast<volatile __int64*>(&destination), reinterpret_cast<__int64>(value));
            }
        }

        /**
        * Atomically loads an integral and returns its value. Do not apply memory barrier.
        * @param source Reference to the source to load
        * @return The loaded value of source
        */
        template<typename type_t>
        [[nodiscard]]
        static type_t load_no_barrier(const type_t& source) noexcept requires(is_integral_v<type_t>)
        {
            check(hud::memory::is_pointer_aligned(&source, sizeof(type_t)));
            if constexpr (sizeof(type_t) == 1) {
                return static_cast<type_t>(__iso_volatile_load8(reinterpret_cast<const volatile __int8*>(&source)));
            }
            else if constexpr (sizeof(type_t) == 2) {
                return static_cast<type_t>(__iso_volatile_load16(reinterpret_cast<const volatile __int16*>(&source)));
            }
            else if constexpr (sizeof(type_t) == 4) {
                return static_cast<type_t>(__iso_volatile_load32(reinterpret_cast<const volatile __int32*>(&source)));
            }
            else if constexpr (sizeof(type_t) == 8) {
                return static_cast<type_t>(__iso_volatile_load64(reinterpret_cast<const volatile __int64*>(&source)));
            }
        }

        /**
        * Atomically loads a pointer and returns its value. Do not apply memory barrier.
        * @param source Reference to the source to load
        * @return The loaded value of source
        */
        template<typename type_t>
        [[nodiscard]]
        static type_t load_no_barrier(const type_t& source) noexcept requires(is_pointer_v<type_t>)
        {
            check(hud::memory::is_pointer_aligned(&source, sizeof(type_t)));
            if constexpr (sizeof(type_t) == 4) {
                return reinterpret_cast<type_t>(__iso_volatile_load32(reinterpret_cast<const volatile __int32*>(&source)));
            }
            else if constexpr (sizeof(type_t) == 8) {
                return reinterpret_cast<type_t>(__iso_volatile_load64(reinterpret_cast<const volatile __int64*>(&source)));
            }
        }
    };

#undef compiler_barrier

} // namespace hud::os::windows 

#endif // HD_INC_CORE_OS_WINDOWS_ATOMICS_INTEL_H