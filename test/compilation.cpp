#include <core/compilation.h>
#include <core/cstring.h>

TEST(Compilation, get_compiler) {
#if defined(HD_COMPILER_CLANG_CL)
    ASSERT_EQ(hud::compilation::get_compiler(), hud::compiler_e::clang_cl);
#elif defined(HD_COMPILER_MSVC)
    ASSERT_EQ(hud::compilation::get_compiler(), hud::compiler_e::msvc);
#elif defined(HD_COMPILER_CLANG)
    ASSERT_EQ(hud::compilation::get_compiler(), hud::compiler_e::clang);
#else
    FAIL(); // "Not implemented"
#endif
}

TEST(Compilation, get_compiler_name) {
#if defined(HD_COMPILER_CLANG_CL)
    ASSERT_TRUE(hud::CString::equals(hud::compilation::get_compiler_name(), "Clang-cl"));
#elif defined(HD_COMPILER_MSVC)
    ASSERT_TRUE(hud::CString::equals(hud::compilation::get_compiler_name(), "Microsoft Visual Studio"));
#elif defined(HD_COMPILER_CLANG)
    ASSERT_TRUE(hud::CString::equals(hud::compilation::get_compiler_name(), "Clang"));
#else
    FAIL(); // "Not implemented"
#endif
}

TEST(Compilation, is_targeting_64bits) {
#if defined(HD_TARGET_X64)
    ASSERT_TRUE(hud::compilation::is_targeting_64bits());
#elif defined(HD_TARGET_X86)
    ASSERT_FALSE(hud::compilation::is_targeting_64bits());
#else
    FAIL(); // "Not implemented"
#endif
}

TEST(Compilation, get_cpu_instruction_set) {
#if defined(HD_TARGET_X64)
    ASSERT_EQ(hud::compilation::get_cpu_instruction_set(), hud::ECPUInstructionSet::x64);
#elif defined(HD_TARGET_X86)
    ASSERT_EQ(hud::compilation::get_cpu_instruction_set(), hud::ECPUInstructionSet::x86);
#elif defined(HD_TARGET_ARM32)
    ASSERT_EQ(hud::compilation::get_cpu_instruction_set(), hud::ECPUInstructionSet::arm32);
#elif defined(HD_TARGET_ARM64)
    ASSERT_EQ(hud::compilation::get_cpu_instruction_set(), hud::ECPUInstructionSet::arm64);
#else
    FAIL(); // "Not implemented"
#endif
}

TEST(Compilation, is_cpu_instruction_set) {
#if defined(HD_TARGET_X64)
    ASSERT_TRUE(hud::compilation::is_cpu_instruction_set(hud::ECPUInstructionSet::x64));
#elif defined(HD_TARGET_X86)
    ASSERT_TRUE(hud::compilation::is_cpu_instruction_set(hud::ECPUInstructionSet::x86));
#elif defined(HD_TARGET_ARM32)
    ASSERT_TRUE(hud::compilation::is_cpu_instruction_set(hud::ECPUInstructionSet::arm32));
#elif defined(HD_TARGET_ARM64)
    ASSERT_TRUE(hud::compilation::is_cpu_instruction_set(hud::ECPUInstructionSet::arm64));
#else
    FAIL(); // "Not implemented"
#endif
}

TEST(Compilation, get_os) {
#if defined(HD_OS_WINDOWS)
    ASSERT_EQ(hud::compilation::get_os(), hud::EOS::windows);
#elif defined(HD_OS_LINUX)
    ASSERT_EQ(hud::compilation::get_os(), hud::EOS::linux);
#else
    FAIL(); // "Not implemented"
#endif
}

TEST(Compilation, is_assertion_enabled) {
#if defined(HD_DEBUG) || defined(HD_DEBUGOPTIMIZED)
    ASSERT_TRUE(hud::compilation::is_assertion_enabled());
#elif defined(HD_RELEASE)
    ASSERT_FALSE(hud::compilation::is_assertion_enabled());
#else
    FAIL(); // "Not implemented"
#endif
}

TEST(Compilation, get_endianness) {
#if defined(HD_LITTLE_ENDIAN)
    ASSERT_EQ(hud::compilation::get_endianness(), hud::EEndianness::little);
#elif defined(HD_BIG_ENDIAN)
    ASSERT_EQ(hud::compilation::get_endianness(), hud::EEndianness::big);
#else
    FAIL();
#endif
}

TEST(Compilation, is_endianness) {
#if defined(HD_LITTLE_ENDIAN)
    ASSERT_TRUE(hud::compilation::is_endianness(hud::EEndianness::little));
#elif defined(HD_BIG_ENDIAN)
    ASSERT_TRUE(hud::compilation::is_endianness(hud::EEndianness::big));
#else
    FAIL();
#endif
}