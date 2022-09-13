#pragma once
#ifndef HD_INC_OSLAYER_COMPILATION_H
#define HD_INC_OSLAYER_COMPILATION_H
#include "Defines.h"

namespace hud {

    /** List of compiler supported compiler. */
    enum class ECompiler {
        msvc,
        clang,
        clang_cl,
        gcc,
        unknown,
    };

    /** List of supported OS target. */
    enum class EOS {
        windows,
        gnu_linux,
        unknown,
    };

    /** List of supported architecture. */
    enum class ECPUInstructionSet {
        x86,
        x64,
        arm32,
        arm64,
        unknown,
    };

    /** List of all supported endianness */
    enum class EEndianness {
        little,
        big,
        unknown
    };

    struct Compilation {

        /** Retrieves the current compiler used when compiling the current file. */
        static constexpr ECompiler get_compiler() noexcept {
            #if defined(HD_COMPILER_CLANG)
                return ECompiler::clang;
            #elif defined(HD_COMPILER_CLANG_CL)
                return ECompiler::clang_cl;
            #elif defined(HD_COMPILER_MSVC)
                return ECompiler::msvc;
            #elif defined(HD_COMPILER_GCC)
                return ECompiler::gcc;
            #else
                return ECompiler::unknown;
            #endif
        }

        /** Retrieves the name of the compiler used when compiling the current file, nullptr if compiler is not supported. */
        static constexpr const char* get_compiler_name() noexcept {
            static_assert(get_compiler() != ECompiler::unknown, "Not implemented");
            if constexpr (get_compiler() == ECompiler::msvc) {
                return "Microsoft Visual Studio";
            }
            else if constexpr (get_compiler() == ECompiler::clang_cl) {
                return "Clang-cl";
            }
            else if constexpr (get_compiler() == ECompiler::clang) {
                return "Clang";
            }
            else if constexpr (get_compiler() == ECompiler::gcc) {
                return "GNU gcc/G++";
            }
            else {
                return nullptr;
            }
        }

        /** Check if for the current compiler */
        static constexpr bool is_compiler(ECompiler compiler) noexcept {
            return get_compiler() == compiler;
        }

        /** Retrieves if the compiler used when compiling the current file target 64 bits processors. */
        static constexpr bool is_targeting_64bits() noexcept {
            static_assert(get_compiler() != ECompiler::unknown, "Compiler is unkown");
            #if defined(HD_TARGET_64_BITS)
                return true;
            #else
                return false;
            #endif
        }

        /** Retrieves the processor target architecture used when compiling the current file. */
        static constexpr ECPUInstructionSet get_cpu_instruction_set() noexcept {
            static_assert(get_compiler() != ECompiler::unknown, "Compiler is unkown");
            #if defined(HD_TARGET_X64)
                return ECPUInstructionSet::x64;
            #elif defined(HD_TARGET_X86)
                return ECPUInstructionSet::x86;
            #elif defined(HD_TARGET_ARM32)
                return ECPUInstructionSet::arm32;
            #elif defined(HD_TARGET_ARM64)
                return ECPUInstructionSet::arm64;
            #else
                return ECPUInstructionSet::unknown;
            #endif
        }

        /** Check if the CPU instruction set is currently the processor target architecture used. */
        static constexpr bool is_cpu_instruction_set(ECPUInstructionSet set) noexcept {
            return get_cpu_instruction_set() == set;
        }

        /** Retrieves the compiler OS target used when compiling the current file. */
        static constexpr EOS get_os() noexcept {
            #if defined(HD_OS_WINDOWS)
                return EOS::windows;
            #elif defined(HD_OS_LINUX)
                return EOS::gnu_linux;
            #else
                return EOS::unknown;
            #endif
        }

        /** Check the target OS */
        static constexpr bool is_os(const EOS os) noexcept {
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
        static constexpr EEndianness get_endianness() noexcept {
            #if defined(HD_LITTLE_ENDIAN)
                return EEndianness::little;
            #elif defined(HD_BIG_ENDIAN)
                return EEndianness::big;
            #else
                return EEndianness::unknown;
            #endif
        }

        /** Check the endianness of scalar types*/
        static constexpr bool is_endianness(EEndianness endianness) noexcept {
            return get_endianness() == endianness;
        }
    };

} // namespace hud

#endif // HD_INC_OSLAYER_COMPILATION_H