cmake_minimum_required(VERSION 3.18)

# !add_hudengine_c_compile_options : Add HUD Engine compiler options to a target
# 
# \arg:target The target
#
function(add_hudengine_c_compile_options target)
		# # Win32 specific compiler options
		# $<$<AND:$<STREQUAL:${CMAKE_GENERATOR_PLATFORM},Win32>,$<STREQUAL:${CMAKE_GENERATOR_PLATFORM},Win64>>:
		# 	-arch:AVX2 # Enables the use of Intel Advanced Vector Extensions 2 instructions
		# >
		# # ARM specific compiler options
		# $<$<AND:$<STREQUAL:${CMAKE_GENERATOR_PLATFORM},ARM32>,$<STREQUAL:${CMAKE_GENERATOR_PLATFORM},ARM64>>:
		# 	-arch:VFPv4
		# >
		# # Debug specific compiler options
		# $<$<CONFIG:Debug>:
		# 	-MDd  # Defines _DEBUG, _MT, and _DLL and causes the application to use the debug multithread-specific and DLL-specific version of the run-time library. It also causes the compiler to place the library name MSVCRTD.lib into the .obj file
		# 	-Zi # Produces a separate PDB file that contains all the symbolic debugging information for use with the debugger. The debugging information isn't included in the object files or executable, which makes them much smaller
		# 	-Od # Turns off all optimizations in the program and speeds compilation
		# 	-Oi # Replaces some function calls with intrinsic or otherwise special forms of the function that help your application run faster
		# 	-RTC1 # Enable the run-time error checks feature, in conjunction with the runtime_checks pragma. ( Enables stack frame run-time error checking and Reports when a variable is used without having been initialized )
		# 	-sdl # Enables recommended Security Development Lifecycle (SDL) checks. These checks change security-relevant warnings into errors, and set additional secure code-generation features
		# 	-GS # Detects some buffer overruns that overwrite a function's return address, exception handler address, or certain types of parameters
		# 	-guard:cf # enable security flags
		# >
		# # DebugOptimized specific compiler options
		# $<$<CONFIG:DebugOptimized>:
		# 	-MD # Causes the application to use the multithread-specific and DLL-specific version of the run-time library. Defines _MT and _DLL and causes the compiler to place the library name MSVCRT.lib into the .obj file
		# 	-Zi  # Produces a separate PDB file that contains all the symbolic debugging information for use with the debugger. The debugging information isn't included in the object files or executable, which makes them much smaller
		# 	-Ox # Enable Most Speed Optimizations. Same as using the following options in combination /Ob2 /Oi /Ot /Oy
		# 	-sdl # Enables recommended Security Development Lifecycle (SDL) checks. These checks change security-relevant warnings into errors, and set additional secure code-generation features
		# 	-GS # Disables detection of some buffer overruns that overwrite a function's return address, exception handler address, or certain types of parameters
		# 	-guard:cf # enable security flags
		# >
		# # Release specific compiler options
		# $<$<CONFIG:Release>:
		# 	-MD # Causes the application to use the multithread-specific and DLL-specific version of the run-time library. Defines _MT and _DLL and causes the compiler to place the library name MSVCRT.lib into the .obj file
		# 	-Ox # Enable Most Speed Optimizations. Same as using the following options in combination /Ob2 /Oi /Ot /Oy
		# 	-flto=thin # Enables whole program optimization.
		# 	-sdl- # Disables recommended Security Development Lifecycle (SDL) checks. These checks change security-relevant warnings into errors, and set additional secure code-generation features
		# 	-GS- # Disables detection of some buffer overruns that overwrite a function's return address, exception handler address, or certain types of parameters
		# 	-guard:cf- # disable security flags
		# >
		
	target_compile_options(
		${target}
		PRIVATE
		-std=c2x # ISO C2x
		-Wall -Wextra -Wpedantic # Enable all warnings
		-Werror #Treat all warnings as error
		-Wshadow -Wshadow-field -Wshadow-field-in-constructor # Enable warnings when variable shadows another variable
		-Wdouble-promotion # Enable warning : implicit conversion increases floating-point precision: A to B
		-Wformat=2 # Enable warning : format string is not a string literal, format string is not a string literal (potentially insecure), 
		-Wfloat-conversion # Enable warning : implicit conversion turns floating-point number into integer: A to B
		-Wsign-compare # comparison of integers of different signs: A and B
		-Wsign-conversion # implicit conversion changes signedness: A to B
		-Wno-format-nonliteral # format string is not a string literal
		-fno-rtti # disable RTTI
		-fno-openmp # Disable all Clang extensions for OpenMP directives and clauses	 
		-ffast-math # Enable fast floating point behaviour
	)
	
endfunction()



# !add_hudengine_c_compile_definitions : Add HUD Engine compiler definition to a target
# 
# \arg:target The target
#
function(add_hudengine_c_compile_definitions target)
	target_compile_definitions(
		${target}
		PRIVATE
		HD_GLOBAL_NAMESPACE_TYPES # HUDEngine types are globals ( i8, i16, etc...)
		# Debug specific compiler flags
		$<$<CONFIG:Debug>:HD_DEBUG>
		# Release specific compiler flags
		$<$<CONFIG:Release>:HD_RELEASE>
		# DebugOptimized specific compiler flags
		$<$<CONFIG:DebugOptimized>:HD_DEBUGOPTIMIZED>
	)
endfunction()

