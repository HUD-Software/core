#pragma once
#ifndef HD_INC_OSLAYER_OS_WINDOWS_ATOMICS_INTEL_H
#define HD_INC_OSLAYER_OS_WINDOWS_ATOMICS_INTEL_H
#include "../OSCommon/CommonAtomics.h"
#include "WindowsAPI.h"
#include <intrin.h>
#include "../Traits/MakeUnsigned.h"
#include "../Traits/IsIntegral.h"
#include "../Traits/IsPointer.h"
#include "../Traits/RemovePointer.h"
#include "../Memory.h"

namespace hud::OS::Windows {

    /** Limits the compiler optimizations that can reorder memory accesses across the point of the call. */
#if defined(HD_COMPILER_CLANG_CL)
#define CompilerBarrier() \
    _Pragma("clang diagnostic push")\
    _Pragma("clang diagnostic ignored \"-Wdeprecated-declarations\"")\
    _ReadWriteBarrier(); \
    _Pragma("clang diagnostic pop")
#elif defined(HD_COMPILER_MSVC)
#define CompilerBarrier() _ReadWriteBarrier();
#endif

    /** Implementation of atomic operations for Intel X86 and X86_64 */
    struct AtomicsIntel : public OS::Common::Atomics {

        /**
        * Establishes a multi-thread fence. Prevents the compiler and CPU from reordering certain types of memory operations around it
        * @param order The memory ordering executed by this fence
        */
        static void thread_fence(const EMemoryOrder order) noexcept {
            if (order == EMemoryOrder::relaxed) {
                return;
            }
            CompilerBarrier(); // Compiler barrier for x86/x64
            if (order == EMemoryOrder::seq_cst) {
                volatile long guard;
                (void)_InterlockedIncrement(&guard);
                CompilerBarrier(); // Compiler barrier for x86/x64
            }
        }

        /**
        * Atomically loads the source and returns its value and follows the memory ordering specified.
        * @param source Reference to the source to load
        * @param order The memory ordering fence applied by this operation. Possible values are relaxed, seq_cst, consume and acquire
        * @return The loaded value of source
        */
        template<typename T>
        [[nodiscard]]
        static T load(const T& source, [[maybe_unused]] const EMemoryOrder order = EMemoryOrder::seq_cst) noexcept requires(IsIntegralV<T> || IsPointerV<T>) {
            T destination = load_no_barrier(source);

            // Load barrier
            if (order != EMemoryOrder::relaxed) {
                // Atomic load do not support release and acq_rel memory fence
                check(order != EMemoryOrder::release);
                check(order != EMemoryOrder::acq_rel);
                CompilerBarrier(); // Compiler barrier for x86/x64
            }
            return destination;
        }

        /**
        * Atomically store a integral value to a destination integral and follows the memory ordering specified.
        * @param destination Reference to the destination
        * @param value The value to store in destination
        * @param order The memory ordering fence applied by this operation. Possible values are relaxed, seq_cst and release
        */
        template<typename T>
        static void store(T& destination, const T value, [[maybe_unused]] const EMemoryOrder order = EMemoryOrder::seq_cst) noexcept requires(IsIntegralV<T>) {
            switch (order) {
            case EMemoryOrder::relaxed:
                store_no_barrier(destination, value);
                return;
            case EMemoryOrder::release:
                CompilerBarrier(); // Compiler barrier for x86/x64
                store_no_barrier(destination, value);
                return;
            default:
            case EMemoryOrder::consume:
            case EMemoryOrder::acquire:
            case EMemoryOrder::acq_rel:
                check(false); // Invalid memory order for store
                [[fallthrough]];
            case EMemoryOrder::seq_cst:
                check(hud::Memory::is_pointer_aligned(&destination, sizeof(T)));
                if constexpr (sizeof(T) == 1) {
                    _InterlockedExchange8(reinterpret_cast<volatile char*>(&destination), static_cast<char>(value));
                }
                else if constexpr (sizeof(T) == 2) {
                    _InterlockedExchange16(reinterpret_cast<volatile short*>(&destination), static_cast<short>(value));
                }
                else if constexpr (sizeof(T) == 4) {
                    _InterlockedExchange(reinterpret_cast<volatile long*>(&destination), static_cast<long>(value));
                }
                else if constexpr (sizeof(T) == 8) {
#if defined (HD_TARGET_X86)
                    CompilerBarrier(); // Compiler barrier for x86/x64
                    __iso_volatile_store64(reinterpret_cast<volatile __int64*>(&destination), static_cast<__int64>(value));
                    thread_fence(Atomics::EMemoryOrder::seq_cst);
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
        template<typename T>
        static void store(T& destination, const T value, [[maybe_unused]] const EMemoryOrder order = EMemoryOrder::seq_cst) noexcept requires(IsPointerV<T>) {
            switch (order) {
            case EMemoryOrder::relaxed:
                store_no_barrier(destination, value);
                return;
            case EMemoryOrder::release:
                CompilerBarrier(); // Compiler barrier for x86/x64
                store_no_barrier(destination, value);
                return;
            default:
            case EMemoryOrder::consume:
            case EMemoryOrder::acquire:
            case EMemoryOrder::acq_rel:
                check(false); // Invalid memory order for store
                [[fallthrough]];
            case EMemoryOrder::seq_cst:
                check(hud::Memory::is_pointer_aligned(&destination, sizeof(T)));
                if constexpr (sizeof(T) == 4) {
                    _InterlockedExchange(reinterpret_cast<volatile long*>(&destination), reinterpret_cast<long>(value));
                }
                else if constexpr (sizeof(T) == 8) {
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
        template<typename T>
        [[nodiscard]]
        static T compare_and_swap(T& destination, const T& expected, const T& value, [[maybe_unused]] const EMemoryOrder order = EMemoryOrder::seq_cst) noexcept requires(IsIntegralV<T>) {
            check(hud::Memory::is_pointer_aligned(&destination, sizeof(T)));
            if constexpr (sizeof(T) == 1) {
                return static_cast<T>(_InterlockedCompareExchange8(reinterpret_cast<volatile char*>(&destination), static_cast<char>(value), static_cast<char>(expected)));
            }
            else if constexpr (sizeof(T) == 2) {
                return static_cast<T>(_InterlockedCompareExchange16(reinterpret_cast<volatile short*>(&destination), static_cast<short>(value), static_cast<short>(expected)));
            }
            else if constexpr (sizeof(T) == 4) {
                return static_cast<T>(_InterlockedCompareExchange(reinterpret_cast<volatile long*>(&destination), static_cast<long>(value), static_cast<long>(expected)));
            }
            else if constexpr (sizeof(T) == 8) {
                return static_cast<T>(_InterlockedCompareExchange64(reinterpret_cast<volatile long long*>(&destination), static_cast<long long>(value), static_cast<long long>(expected)));
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
        template<typename T>
        [[nodiscard]]
        static T compare_and_swap(T& destination, const T& expected, const T& value, [[maybe_unused]] const EMemoryOrder order = EMemoryOrder::seq_cst) noexcept requires(IsPointerV<T>) {
            check(hud::Memory::is_pointer_aligned(&destination, sizeof(T)));
            if constexpr (sizeof(T) == 4) {
                return reinterpret_cast<T>(_InterlockedCompareExchange(reinterpret_cast<volatile long*>(&destination), reinterpret_cast<long>(value), reinterpret_cast<long>(expected)));
            }
            else if constexpr (sizeof(T) == 8) {
                return reinterpret_cast<T>(_InterlockedCompareExchange64(reinterpret_cast<volatile long long*>(&destination), reinterpret_cast<long long>(value), reinterpret_cast<long long>(expected)));
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
        template<typename T>
        [[nodiscard]]
        static bool compare_and_set(T& destination, T& expected, const T& value, [[maybe_unused]] const EMemoryOrder order = EMemoryOrder::seq_cst) noexcept requires(IsIntegralV<T>) {
            T old = compare_and_swap(destination, expected, value, order);
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
        template<typename T>
        [[nodiscard]]
        static bool compare_and_set(T& destination, T& expected, const T& value, [[maybe_unused]] const EMemoryOrder order = EMemoryOrder::seq_cst) noexcept requires(IsPointerV<T>) {
            T old = compare_and_swap(destination, expected, value, order);
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
        template<typename T>
        [[nodiscard]]
        static T exchange(T& destination, const T& value, [[maybe_unused]] const EMemoryOrder order = EMemoryOrder::seq_cst) noexcept requires(IsIntegralV<T>) {
            check(hud::Memory::is_pointer_aligned(&destination, sizeof(T)));
            if constexpr (sizeof(T) == 1) {
                return static_cast<T>(_InterlockedExchange8(reinterpret_cast<volatile char*>(&destination), static_cast<char>(value)));
            }
            else if constexpr (sizeof(T) == 2) {
                return static_cast<T>(_InterlockedExchange16(reinterpret_cast<volatile short*>(&destination), static_cast<short>(value)));
            }
            else if constexpr (sizeof(T) == 4) {
                return static_cast<T>(_InterlockedExchange(reinterpret_cast<volatile long*>(&destination), static_cast<long>(value)));
            }
            else if constexpr (sizeof(T) == 8) {
#if defined(HD_TARGET_X64)
                return static_cast<T>(_InterlockedExchange64(reinterpret_cast<volatile long long*>(&destination), static_cast<long long>(value)));
#else //HD_TARGET_X86
                T expected_value{ load(destination) };
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
        template<typename T>
        [[nodiscard]]
        static T exchange(T& destination, const T& value, [[maybe_unused]] const EMemoryOrder order = EMemoryOrder::seq_cst) noexcept requires(IsPointerV<T>) {
            check(hud::Memory::is_pointer_aligned(&destination, sizeof(T)));
            if constexpr (sizeof(T) == 4) {
                return reinterpret_cast<T>(_InterlockedExchange(reinterpret_cast<volatile long*>(&destination), reinterpret_cast<long>(value)));
            }
            else if constexpr (sizeof(T) == 8) {
                return reinterpret_cast<T>(_InterlockedExchange64(reinterpret_cast<volatile long long*>(&destination), reinterpret_cast<long long>(value)));
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
        template<typename T>
        [[nodiscard]]
        static T fetch_add(T& addend, const T& to_add, [[maybe_unused]] const EMemoryOrder order = EMemoryOrder::seq_cst) noexcept requires(IsIntegralV<T>) {
            check(hud::Memory::is_pointer_aligned(&addend, sizeof(T)));
            if constexpr (sizeof(T) == 1) {
                return static_cast<T>(_InterlockedExchangeAdd8(reinterpret_cast<volatile char*>(&addend), static_cast<char>(to_add)));
            }
            else if constexpr (sizeof(T) == 2) {
                return static_cast<T>(_InterlockedExchangeAdd16(reinterpret_cast<volatile short*>(&addend), static_cast<short>(to_add)));
            }
            else if constexpr (sizeof(T) == 4) {
                return static_cast<T>(_InterlockedExchangeAdd(reinterpret_cast<volatile long*>(&addend), static_cast<long>(to_add)));
            }
            else if constexpr (sizeof(T) == 8) {
#if defined(HD_TARGET_X64)
                return static_cast<T>(_InterlockedExchangeAdd64(reinterpret_cast<volatile long long*>(&addend), static_cast<long long>(to_add)));
#else //HD_TARGET_X86
                T expected_value{ load(addend) };
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
        template<typename T>
        [[nodiscard]]
        static T fetch_add(T& addend, const isize to_add, [[maybe_unused]] const EMemoryOrder order = EMemoryOrder::seq_cst) noexcept requires(IsPointerV<T>) {
            check(hud::Memory::is_pointer_aligned(&addend, sizeof(T)));
            const usize ptrdiff_to_add = static_cast<usize>(to_add) * sizeof(RemovePointerT<T>);
            if constexpr (sizeof(T) == 4) {
                return reinterpret_cast<T>(_InterlockedExchangeAdd(reinterpret_cast<volatile long*>(&addend), static_cast<long>(ptrdiff_to_add)));
            }
            else if constexpr (sizeof(T) == 8) {
                return reinterpret_cast<T>(_InterlockedExchangeAdd64(reinterpret_cast<volatile long long*>(&addend), static_cast<long long>(ptrdiff_to_add)));
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
        template<typename T>
        [[nodiscard]]
        static T fetch_sub(T& subtracted, const T& to_subtract, [[maybe_unused]] const EMemoryOrder order = EMemoryOrder::seq_cst) noexcept requires(IsIntegralV<T>) {
            T negated = static_cast<T>(0U - static_cast<MakeUnsignedT<T>>(to_subtract));
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
        template<typename T>
        [[nodiscard]]
        static T fetch_sub(T& subtracted, const isize to_subtract, [[maybe_unused]] const EMemoryOrder order = EMemoryOrder::seq_cst) noexcept requires(IsPointerV<T>) {
            return fetch_add(subtracted, -to_subtract, order);
        }

    private:
        /**
        * Atomically store an integral to a integral referenced by the destination reference without memory fence.
        * @param destination Reference to the destination
        * @param value The value to store in destination
        */
        template<typename T>
        static void store_no_barrier(T& destination, T value) noexcept requires(IsIntegralV<T>)
        {
            check(hud::Memory::is_pointer_aligned(&destination, sizeof(T)));
            if constexpr (sizeof(T) == 1) {
                __iso_volatile_store8(reinterpret_cast<volatile __int8*>(&destination), static_cast<__int8>(value));
            }
            else if constexpr (sizeof(T) == 2) {
                __iso_volatile_store16(reinterpret_cast<volatile __int16*>(&destination), static_cast<__int16>(value));
            }
            else if constexpr (sizeof(T) == 4) {
                __iso_volatile_store32(reinterpret_cast<volatile __int32*>(&destination), static_cast<__int32>(value));
            }
            else if constexpr (sizeof(T) == 8) {
                __iso_volatile_store64(reinterpret_cast<volatile __int64*>(&destination), static_cast<__int64>(value));
            }
        }

        /**
        * Atomically store a pointer address to a pointer referenced by the destination reference without memory fence.
        * @param destination Reference to the destination
        * @param value The pointer address to store in destination
        */
        template<typename T>
        static void store_no_barrier(T& destination, T value) noexcept requires(IsPointerV<T>)
        {
            check(hud::Memory::is_pointer_aligned(&destination, sizeof(T)));
            if constexpr (sizeof(T) == 4) {
                __iso_volatile_store32(reinterpret_cast<volatile __int32*>(&destination), reinterpret_cast<__int32>(value));
            }
            else if constexpr (sizeof(T) == 8) {
                __iso_volatile_store64(reinterpret_cast<volatile __int64*>(&destination), reinterpret_cast<__int64>(value));
            }
        }

        /**
        * Atomically loads an integral and returns its value. Do not apply memory barrier.
        * @param source Reference to the source to load
        * @return The loaded value of source
        */
        template<typename T>
        [[nodiscard]]
        static T load_no_barrier(const T& source) noexcept requires(IsIntegralV<T>)
        {
            check(hud::Memory::is_pointer_aligned(&source, sizeof(T)));
            if constexpr (sizeof(T) == 1) {
                return static_cast<T>(__iso_volatile_load8(reinterpret_cast<const volatile __int8*>(&source)));
            }
            else if constexpr (sizeof(T) == 2) {
                return static_cast<T>(__iso_volatile_load16(reinterpret_cast<const volatile __int16*>(&source)));
            }
            else if constexpr (sizeof(T) == 4) {
                return static_cast<T>(__iso_volatile_load32(reinterpret_cast<const volatile __int32*>(&source)));
            }
            else if constexpr (sizeof(T) == 8) {
                return static_cast<T>(__iso_volatile_load64(reinterpret_cast<const volatile __int64*>(&source)));
            }
        }

        /**
        * Atomically loads a pointer and returns its value. Do not apply memory barrier.
        * @param source Reference to the source to load
        * @return The loaded value of source
        */
        template<typename T>
        [[nodiscard]]
        static T load_no_barrier(const T& source) noexcept requires(IsPointerV<T>)
        {
            check(hud::Memory::is_pointer_aligned(&source, sizeof(T)));
            if constexpr (sizeof(T) == 4) {
                return reinterpret_cast<T>(__iso_volatile_load32(reinterpret_cast<const volatile __int32*>(&source)));
            }
            else if constexpr (sizeof(T) == 8) {
                return reinterpret_cast<T>(__iso_volatile_load64(reinterpret_cast<const volatile __int64*>(&source)));
            }
        }
    };

#undef CompilerBarrier

} // namespace hud::OS::Windows 

#endif // HD_INC_OSLAYER_OS_WINDOWS_ATOMICS_INTEL_H