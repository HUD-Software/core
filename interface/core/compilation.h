#ifndef HD_INC_CORE_COMPILATION_H
#define HD_INC_CORE_COMPILATION_H
#include "defines.h"

#ifndef HUD_ASSERTION_ENABLED
    #if defined(HD_DEBUG) || defined(HD_DEBUGOPTIMIZED)
        #define HUD_ASSERTION_ENABLED 1
    #else
        #define HUD_ASSERTION_ENABLED 0
    #endif
#endif

namespace hud
{

    /** List of compiler supported compiler. */
    enum class compiler
    {
        msvc,
        clang,
        clang_cl,
        gcc,
        emscripten,
        unknown,
    };

    /** List of supported OS target. */
    enum class host
    {
        windows,
        linux,
        emscripten,
        unknown,
    };

    /** List of supported architecture. */
    enum class cpu_instruction_set
    {
        x86,
        x64,
        arm32,
        arm64,
        wasm32,
        wasm64,
        unknown,
    };

    /** List of all supported endianness. */
    enum class endianness
    {
        little,
        big,
        unknown
    };

    /** List of all compilation modes. */
    enum class compilation_mode
    {
        debug,
        debug_optimized,
        release,
        unknown
    };

    struct compilation
    {

        /** Retrieves the current compiler used when compiling the current file. */
        static constexpr compiler get_compiler() noexcept
        {
#if defined(HD_COMPILER_CLANG)
            return compiler::clang;
#elif defined(HD_COMPILER_CLANG_CL)
            return compiler::clang_cl;
#elif defined(HD_COMPILER_MSVC)
            return compiler::msvc;
#elif defined(HD_COMPILER_GCC)
            return compiler::gcc;
#elif defined(HD_COMPILER_EMSCRIPTEN)
            return compiler::emscripten;
#else
            return compiler::unknown;
#endif
        }

        /** Retrieves the name of the compiler used when compiling the current file, nullptr if compiler is not supported. */
        static constexpr const char *get_compiler_name() noexcept
        {
            static_assert(get_compiler() != compiler::unknown, "Not implemented");
            if constexpr (get_compiler() == compiler::msvc)
            {
                return "Microsoft Visual Studio";
            }
            else if constexpr (get_compiler() == compiler::clang_cl)
            {
                return "Clang-cl";
            }
            else if constexpr (get_compiler() == compiler::clang)
            {
                return "Clang";
            }
            else if constexpr (get_compiler() == compiler::gcc)
            {
                return "GNU gcc/G++";
            }
            else if constexpr (get_compiler() == compiler::emscripten)
            {
                return "Emscripten";
            }
            else
            {
                return nullptr;
            }
        }

        /** Check if for the current compiler. */
        static constexpr bool is_compiler(compiler compiler) noexcept
        {
            return get_compiler() == compiler;
        }

        /** Retrieves if the compiler used when compiling the current file target 64 bits processors. */
        static constexpr bool is_targeting_64bits() noexcept
        {
            static_assert(get_compiler() != compiler::unknown, "Compiler is unkown");
#if defined(HD_TARGET_64_BITS)
            return true;
#elif defined(HD_TARGET_32_BITS)
            return false;
#else
    #error Neither 32 or 64 bits
#endif
        }

        /** Retrieves the processor target architecture used when compiling the current file. */
        static constexpr cpu_instruction_set get_cpu_instruction_set() noexcept
        {
            static_assert(get_compiler() != compiler::unknown, "Compiler is unkown");
#if defined(HD_TARGET_X64)
            return cpu_instruction_set::x64;
#elif defined(HD_TARGET_X86)
            return cpu_instruction_set::x86;
#elif defined(HD_TARGET_ARM32)
            return cpu_instruction_set::arm32;
#elif defined(HD_TARGET_ARM64)
            return cpu_instruction_set::arm64;
#elif defined(HD_TARGET_WASM32)
            return cpu_instruction_set::wasm32;
#elif defined(HD_TARGET_WASM64)
            return cpu_instruction_set::wasm64;
#else
            return cpu_instruction_set::unknown;
#endif
        }

        /** Check if the CPU instruction set is currently the processor target architecture used. */
        static constexpr bool is_cpu_instruction_set(cpu_instruction_set set) noexcept
        {
            return get_cpu_instruction_set() == set;
        }

        /** Retrieves the compiler host target used when compiling the current file. */
        static constexpr host get_host() noexcept
        {
#if defined(HD_HOST_WINDOWS)
            return host::windows;
#elif defined(HD_HOST_LINUX)
            return host::linux;
#elif defined(HD_HOST_EMSCRIPTEN)
            return host::emscripten;
#else
            return host::unknown;
#endif
        }

        /** Check the target host. */
        static constexpr bool is_host(const host os) noexcept
        {
            return get_host() == os;
        }

        /** Retrieves if the assertion are enabled. */
        static constexpr bool is_assertion_enabled() noexcept
        {
            return HUD_ASSERTION_ENABLED == 1;
        }

        /** Retrieves the endianness of scalar types. */
        static constexpr endianness get_endianness() noexcept
        {
#if defined(HD_LITTLE_ENDIAN)
            return endianness::little;
#elif defined(HD_BIG_ENDIAN)
            return endianness::big;
#else
            return endianness::unknown;
#endif
        }

        /** Check the endianness of scalar types. */
        static constexpr bool is_endianness(endianness endianness) noexcept
        {
            return get_endianness() == endianness;
        }

        /** Retrieves the compilation mode. */
        static constexpr compilation_mode get_compilation_mode() noexcept
        {
#if defined(HD_DEBUG)
            return compilation_mode::debug;
#elif defined(HD_DEBUGOPTIMIZED)
            return compilation_mode::debug_optimized;
#elif defined(HD_RELEASE)
            return compilation_mode::release;
#else
            return compilation_mode::unknown;
#endif
        }

        /** Check the compilation mode. */
        static constexpr bool is_compilation_mode(const hud::compilation_mode compilation_mode) noexcept
        {
            return get_compilation_mode() == compilation_mode;
        }
    };

} // namespace hud

#endif // HD_INC_CORE_COMPILATION_H