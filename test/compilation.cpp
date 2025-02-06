#include <core/compilation.h>
#include <core/cstring.h>

GTEST_TEST(compilation, get_compiler)
{
#if defined(HD_COMPILER_CLANG_CL)
    hud_assert_eq(hud::compilation::get_compiler(), hud::compiler::clang_cl);
#elif defined(HD_COMPILER_MSVC)
    hud_assert_eq(hud::compilation::get_compiler(), hud::compiler::msvc);
#elif defined(HD_COMPILER_CLANG)
    hud_assert_eq(hud::compilation::get_compiler(), hud::compiler::clang);
#elif defined(HD_COMPILER_GCC)
    hud_assert_eq(hud::compilation::get_compiler(), hud::compiler::gcc);
#elif defined(HD_COMPILER_EMSCRIPTEN)
    hud_assert_eq(hud::compilation::get_compiler(), hud::compiler::emscripten);
#else
    FAIL(); // "Not implemented"
#endif
}

GTEST_TEST(compilation, get_compiler_name)
{
#if defined(HD_COMPILER_CLANG_CL)
    hud_assert_true(hud::cstring::equals(hud::compilation::get_compiler_name(), "Clang-cl"));
#elif defined(HD_COMPILER_MSVC)
    hud_assert_true(hud::cstring::equals(hud::compilation::get_compiler_name(), "Microsoft Visual Studio"));
#elif defined(HD_COMPILER_CLANG)
    hud_assert_true(hud::cstring::equals(hud::compilation::get_compiler_name(), "Clang"));
#elif defined(HD_COMPILER_GCC)
    hud_assert_true(hud::cstring::equals(hud::compilation::get_compiler_name(), "GNU gcc/G++"));
#elif defined(HD_COMPILER_EMSCRIPTEN)
    hud_assert_true(hud::cstring::equals(hud::compilation::get_compiler_name(), "Emscripten"));
#else
    FAIL(); // "Not implemented"
#endif
}

GTEST_TEST(compilation, is_targeting_64bits)
{
#if defined(HD_TARGET_64_BITS)
    hud_assert_true(hud::compilation::is_targeting_64bits());
#elif defined(HD_TARGET_32_BITS)
    hud_assert_false(hud::compilation::is_targeting_64bits());
#else
    FAIL(); // "Not implemented"
#endif
}

GTEST_TEST(compilation, get_cpu_instruction_set)
{
#if defined(HD_TARGET_X64)
    hud_assert_eq(hud::compilation::get_cpu_instruction_set(), hud::cpu_instruction_set::x64);
#elif defined(HD_TARGET_X86)
    hud_assert_eq(hud::compilation::get_cpu_instruction_set(), hud::cpu_instruction_set::x86);
#elif defined(HD_TARGET_ARM32)
    hud_assert_eq(hud::compilation::get_cpu_instruction_set(), hud::cpu_instruction_set::arm32);
#elif defined(HD_TARGET_ARM64)
    hud_assert_eq(hud::compilation::get_cpu_instruction_set(), hud::cpu_instruction_set::arm64);
#elif defined(HD_TARGET_WASM32)
    hud_assert_eq(hud::compilation::get_cpu_instruction_set(), hud::cpu_instruction_set::wasm32);
#elif defined(HD_TARGET_WASM64)
    hud_assert_eq(hud::compilation::get_cpu_instruction_set(), hud::cpu_instruction_set::wasm64);
#else
    FAIL(); // "Not implemented"
#endif
}

GTEST_TEST(compilation, is_cpu_instruction_set)
{
#if defined(HD_TARGET_X64)
    hud_assert_true(hud::compilation::is_cpu_instruction_set(hud::cpu_instruction_set::x64));
#elif defined(HD_TARGET_X86)
    hud_assert_true(hud::compilation::is_cpu_instruction_set(hud::cpu_instruction_set::x86));
#elif defined(HD_TARGET_ARM32)
    hud_assert_true(hud::compilation::is_cpu_instruction_set(hud::cpu_instruction_set::arm32));
#elif defined(HD_TARGET_ARM64)
    hud_assert_true(hud::compilation::is_cpu_instruction_set(hud::cpu_instruction_set::arm64));
#elif defined(HD_TARGET_WASM32)
    hud_assert_true(hud::compilation::is_cpu_instruction_set(hud::cpu_instruction_set::wasm32));
#elif defined(HD_TARGET_WASM64)
    hud_assert_true(hud::compilation::is_cpu_instruction_set(hud::cpu_instruction_set::wasm64));
#else
    FAIL(); // "Not implemented"
#endif
}

GTEST_TEST(compilation, get_host)
{
#if defined(HD_HOST_WINDOWS)
    hud_assert_eq(hud::compilation::get_host(), hud::host::windows);
#elif defined(HD_HOST_LINUX)
    hud_assert_eq(hud::compilation::get_host(), hud::host::linux);
#elif defined(HD_HOST_EMSCRIPTEN)
    hud_assert_eq(hud::compilation::get_host(), hud::host::emscripten);
#else
    FAIL(); // "Not implemented"
#endif
}

GTEST_TEST(compilation, is_assertion_enabled)
{
#if defined(HD_DEBUG) || defined(HD_DEBUGOPTIMIZED)
    hud_assert_true(hud::compilation::is_assertion_enabled());
#elif defined(HD_RELEASE)
    hud_assert_false(hud::compilation::is_assertion_enabled());
#else
    FAIL(); // "Not implemented"
#endif
}

GTEST_TEST(compilation, get_endianness)
{
#if defined(HD_LITTLE_ENDIAN)
    hud_assert_eq(hud::compilation::get_endianness(), hud::endianness::little);
#elif defined(HD_BIG_ENDIAN)
    hud_assert_eq(hud::compilation::get_endianness(), hud::endianness::big);
#else
    FAIL();
#endif
}

GTEST_TEST(compilation, is_endianness)
{
#if defined(HD_LITTLE_ENDIAN)
    hud_assert_true(hud::compilation::is_endianness(hud::endianness::little));
#elif defined(HD_BIG_ENDIAN)
    hud_assert_true(hud::compilation::is_endianness(hud::endianness::big));
#else
    FAIL();
#endif
}