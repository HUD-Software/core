#include <core/compilation.h>
#include <core/cstring.h>

GTEST_TEST(compilation, get_compiler)
{
#if defined(HD_COMPILER_CLANG_CL)
    hud_assert_eq(hud::compilation::get_compiler(), hud::compiler_e::clang_cl);
#elif defined(HD_COMPILER_MSVC)
    hud_assert_eq(hud::compilation::get_compiler(), hud::compiler_e::msvc);
#elif defined(HD_COMPILER_CLANG)
    hud_assert_eq(hud::compilation::get_compiler(), hud::compiler_e::clang);
#elif defined(HD_COMPILER_GCC)
    hud_assert_eq(hud::compilation::get_compiler(), hud::compiler_e::gcc);
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
#else
    FAIL(); // "Not implemented"
#endif
}

GTEST_TEST(compilation, is_targeting_64bits)
{
#if defined(HD_TARGET_X64)
    hud_assert_true(hud::compilation::is_targeting_64bits());
#elif defined(HD_TARGET_X86)
    hud_assert_false(hud::compilation::is_targeting_64bits());
#else
    FAIL(); // "Not implemented"
#endif
}

GTEST_TEST(compilation, get_cpu_instruction_set)
{
#if defined(HD_TARGET_X64)
    hud_assert_eq(hud::compilation::get_cpu_instruction_set(), hud::cpu_instruction_set_e::x64);
#elif defined(HD_TARGET_X86)
    hud_assert_eq(hud::compilation::get_cpu_instruction_set(), hud::cpu_instruction_set_e::x86);
#elif defined(HD_TARGET_ARM32)
    hud_assert_eq(hud::compilation::get_cpu_instruction_set(), hud::cpu_instruction_set_e::arm32);
#elif defined(HD_TARGET_ARM64)
    hud_assert_eq(hud::compilation::get_cpu_instruction_set(), hud::cpu_instruction_set_e::arm64);
#else
    FAIL(); // "Not implemented"
#endif
}

GTEST_TEST(compilation, is_cpu_instruction_set)
{
#if defined(HD_TARGET_X64)
    hud_assert_true(hud::compilation::is_cpu_instruction_set(hud::cpu_instruction_set_e::x64));
#elif defined(HD_TARGET_X86)
    hud_assert_true(hud::compilation::is_cpu_instruction_set(hud::cpu_instruction_set_e::x86));
#elif defined(HD_TARGET_ARM32)
    hud_assert_true(hud::compilation::is_cpu_instruction_set(hud::cpu_instruction_set_e::arm32));
#elif defined(HD_TARGET_ARM64)
    hud_assert_true(hud::compilation::is_cpu_instruction_set(hud::cpu_instruction_set_e::arm64));
#else
    FAIL(); // "Not implemented"
#endif
}

GTEST_TEST(compilation, get_os)
{
#if defined(HD_OS_WINDOWS)
    hud_assert_eq(hud::compilation::get_os(), hud::os_e::windows);
#elif defined(HD_OS_LINUX)
    hud_assert_eq(hud::compilation::get_os(), hud::os_e::linux);
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
    hud_assert_eq(hud::compilation::get_endianness(), hud::endianness_e::little);
#elif defined(HD_BIG_ENDIAN)
    hud_assert_eq(hud::compilation::get_endianness(), hud::endianness_e::big);
#else
    FAIL();
#endif
}

GTEST_TEST(compilation, is_endianness)
{
#if defined(HD_LITTLE_ENDIAN)
    hud_assert_true(hud::compilation::is_endianness(hud::endianness_e::little));
#elif defined(HD_BIG_ENDIAN)
    hud_assert_true(hud::compilation::is_endianness(hud::endianness_e::big));
#else
    FAIL();
#endif
}