#pragma once
#ifndef HD_INC_CORE_COMPILATION_H
#define HD_INC_CORE_COMPILATION_H
#include "defines.h"

namespace hud {

    /** List of compiler supported compiler. */
    enum class compiler_e {
        msvc,
        clang,
        clang_cl,
        gcc,
        unknown,
    };

    /** List of supported OS target. */
    enum class os_e {
        windows,
        gnu_linux,
        unknown,
    };

    /** List of supported architecture. */
    enum class cpu_instruction_set_e {
        x86,
        x64,
        arm32,
        arm64,
        unknown,
    };

    /** List of all supported endianness */
    enum class endianness_e {
        little,
        big,
        unknown
    };

    struct compilation {

        /** Retrieves the current compiler used when compiling the current file. */
        static constexpr compiler_e get_compiler() noexcept {
            #if defined(HD_COMPILER_CLANG)
                return compiler_e::clang;
            #elif defined(HD_COMPILER_CLANG_CL)
                return compiler_e::clang_cl;
            #elif defined(HD_COMPILER_MSVC)
                return compiler_e::msvc;
            #elif defined(HD_COMPILER_GCC)
                return compiler_e::gcc;
            #else
                return compiler_e::unknown;
            #endif
        }

        /** Retrieves the name of the compiler used when compiling the current file, nullptr if compiler is not supported. */
        static constexpr const char* get_compiler_name() noexcept {
            static_assert(get_compiler() != compiler_e::unknown, "Not implemented");
            if constexpr (get_compiler() == compiler_e::msvc) {
                return "Microsoft Visual Studio";
            }
            else if constexpr (get_compiler() == compiler_e::clang_cl) {
                return "Clang-cl";
            }
            else if constexpr (get_compiler() == compiler_e::clang) {
                return "Clang";
            }
            else if constexpr (get_compiler() == compiler_e::gcc) {
                return "GNU gcc/G++";
            }
            else {
                return nullptr;
            }
        }

        /** Check if for the current compiler */
        static constexpr bool is_compiler(compiler_e compiler) noexcept {
            return get_compiler() == compiler;
        }

        /** Retrieves if the compiler used when compiling the current file target 64 bits processors. */
        static constexpr bool is_targeting_64bits() noexcept {
            static_assert(get_compiler() != compiler_e::unknown, "Compiler is unkown");
            #if defined(HD_TARGET_64_BITS)
                return true;
            #else
                return false;
            #endif
        }

        /** Retrieves the processor target architecture used when compiling the current file. */
        static constexpr cpu_instruction_set_e get_cpu_instruction_set() noexcept {
            static_assert(get_compiler() != compiler_e::unknown, "Compiler is unkown");
            #if defined(HD_TARGET_X64)
                return cpu_instruction_set_e::x64;
            #elif defined(HD_TARGET_X86)
                return cpu_instruction_set_e::x86;
            #elif defined(HD_TARGET_ARM32)
                return cpu_instruction_set_e::arm32;
            #elif defined(HD_TARGET_ARM64)
                return cpu_instruction_set_e::arm64;
            #else
                return cpu_instruction_set_e::unknown;
            #endif
        }

        /** Check if the CPU instruction set is currently the processor target architecture used. */
        static constexpr bool is_cpu_instruction_set(cpu_instruction_set_e set) noexcept {
            return get_cpu_instruction_set() == set;
        }

        /** Retrieves the compiler OS target used when compiling the current file. */
        static constexpr os_e get_os() noexcept {
            #if defined(HD_OS_WINDOWS)
                return os_e::windows;
            #elif defined(HD_OS_LINUX)
                return os_e::gnu_linux;
            #else
                return os_e::unknown;
            #endif
        }

        /** Check the target OS */
        static constexpr bool is_os(const os_e os) noexcept {
            return get_os() == os;
        }

        /** Retrieves if the assertion are enabled. */
        static constexpr bool is_assertion_enabled() noexcept {
            #if defined(HD_DEBUG) || defined(HD_DEBUGOPTIMIZED)
                return true;
            #else
                return false;
            #endif
        }

        /** Retrives the endianness of scalar types */
        static constexpr endianness_e get_endianness() noexcept {
            #if defined(HD_LITTLE_ENDIAN)
                return endianness_e::little;
            #elif defined(HD_BIG_ENDIAN)
                return endianness_e::big;
            #else
                return endianness_e::unknown;
            #endif
        }

        /** Check the endianness of scalar types*/
        static constexpr bool is_endianness(endianness_e endianness) noexcept {
            return get_endianness() == endianness;
        }
    };

} // namespace hud

#endif // HD_INC_CORE_COMPILATION_H