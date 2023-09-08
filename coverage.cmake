# Coverage configuration and build type
set(CMAKE_CXX_FLAGS_COVERAGE "${CMAKE_CXX_FLAGS_DEBUG}" CACHE STRING "Flags used by the C++ compiler during coverage builds.")
set(CMAKE_C_FLAGS_COVERAGE "${CMAKE_C_FLAGS_DEBUG}" CACHE STRING "Flags used by the C compiler during coverage builds.")
set(CMAKE_EXE_LINKER_FLAGS_COVERAGE "${CMAKE_EXE_LINKER_FLAGS_DEBUG}" CACHE STRING "Flags used for linking binaries during coverage builds.")
set(CMAKE_SHARED_LINKER_FLAGS_COVERAGE "${CMAKE_SHARED_LINKER_FLAGS_DEBUG}" CACHE STRING "Flags used by the shared libraries linker during coverage builds.")
mark_as_advanced(
	CMAKE_CXX_FLAGS_COVERAGE
	CMAKE_C_FLAGS_COVERAGE
	CMAKE_EXE_LINKER_FLAGS_COVERAGE
	CMAKE_SHARED_LINKER_FLAGS_COVERAGE)

set(CMAKE_BUILD_TYPE "${CMAKE_BUILD_TYPE}" CACHE STRING "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel Coverage.")
if(CMAKE_CONFIGURATION_TYPES)
	set(CMAKE_CONFIGURATION_TYPES Debug Release MinSizeRel RelWithDebInfo Coverage)
	set(CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}" CACHE STRING "Reset the configurations to what we need" FORCE)
endif()

function(enable_coverage project_name lib_name)
if(MSVC)
	string(
		APPEND VS_CONFIG
		"$<IF:$<CONFIG:Debug>,"
		"Debug,"
		"$<IF:$<CONFIG:Release>,"
		"Release,"
		"$<IF:$<CONFIG:MinSizeRel>,"
		"MinSizeRel,"
		"$<IF:$<CONFIG:RelWithDebInfo>,"
		"RelWithDebInfo,>>>>"
	)

	if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
		set(MSVC_CODECOVERAGE_CONSOLE_PATH "C:\\Program Files\\Microsoft Visual Studio\\2022\\Enterprise\\Common7\\IDE\\Extensions\\Microsoft\\CodeCoverage.Console\\Microsoft.CodeCoverage.Console.exe" CACHE STRING "Path to Microsoft.CodeCoverage.Console.exe")
		find_program(MSVC_CODECOVERAGE_CONSOLE_EXE ${MSVC_CODECOVERAGE_CONSOLE_PATH})
		if(NOT MSVC_CODECOVERAGE_CONSOLE_EXE)
			message(FATAL_ERROR "Code coverage on Windows need Microsoft.CodeCoverage.Console.exe available in Visual Studio 2022 17.3 Enterprise Edition")
		endif()

		target_link_options(${project_name} PRIVATE /PROFILE)
		
		add_custom_command(
			TARGET ${project_name} POST_BUILD
			COMMENT "Instrument and Collect ${project_name}.exe"
			COMMAND ${MSVC_CODECOVERAGE_CONSOLE_EXE} instrument Release\\${project_name}.exe -s ..\\..\\test\\coverage.runsettings
			COMMAND ${MSVC_CODECOVERAGE_CONSOLE_EXE} collect Release\\${project_name}.exe -o Release\\coverage.msvc -f cobertura -s ..\\..\\test\\coverage.runsettings
		)
	elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
		target_compile_options(${project_name} PRIVATE -fprofile-instr-generate -fcoverage-mapping)
		target_compile_options(${lib_name} PRIVATE -fprofile-instr-generate -fcoverage-mapping)
		
		# Add clang lib path to libraries paths
		get_filename_component(CMAKE_CXX_COMPILER_PATH ${CMAKE_CXX_COMPILER} DIRECTORY)
		target_link_directories(${project_name} PRIVATE "${CMAKE_CXX_COMPILER_PATH}\\..\\lib\\clang\\${CMAKE_CXX_COMPILER_VERSION}\\lib\\windows\\")

		add_custom_command( 
		 	TARGET ${project_name} POST_BUILD
			COMMAND echo Download Grcov...
			COMMAND Powershell.exe Invoke-WebRequest -Uri https://github.com/mozilla/grcov/releases/download/v0.8.13/grcov-x86_64-pc-windows-msvc.zip -OutFile ./grcov-x86_64-pc-windows-msvc.zip
			COMMAND Powershell.exe Expand-Archive -Path ./grcov-x86_64-pc-windows-msvc.zip -DestinationPath . -F
		)

    	add_custom_command( 
		 	TARGET ${project_name} POST_BUILD
			COMMAND echo Start coverage...
			COMMAND ${CMAKE_COMMAND} -E env ./${VS_CONFIG}/${project_name}.exe
		 	#COMMAND ${CMAKE_COMMAND} -E env LLVM_PROFILE_FILE="${lib_name}.profraw" ./${VS_CONFIG}/${project_name}.exe
		)

		add_custom_command( 
			TARGET ${project_name} POST_BUILD
			COMMAND echo Merge coverage info...
			COMMAND ${CMAKE_CXX_COMPILER_PATH}/llvm-profdata merge -sparse default.profraw -o default.profdata
			#COMMAND ${CMAKE_CXX_COMPILER_PATH}/llvm-profdata merge -sparse ${lib_name}.profraw -o ${lib_name}.profdata
		)

		# add_custom_command( 
		# 	TARGET ${project_name} POST_BUILD
		# 	COMMAND echo Show coverage info...
		# 	COMMAND ${CMAKE_CXX_COMPILER_PATH}/llvm-cov report ./${VS_CONFIG}/${project_name}.exe -instr-profile=default.profdata
		# 	#COMMAND ${CMAKE_CXX_COMPILER_PATH}/llvm-cov report ./${VS_CONFIG}/${project_name}.exe -instr-profile=${lib_name}.profdata
		# )

		add_custom_command( 
			TARGET ${project_name} POST_BUILD
			COMMAND echo Show coverage info...
			COMMAND ${CMAKE_CXX_COMPILER_PATH}/llvm-cov show ./${VS_CONFIG}/${project_name}.exe -instr-profile=default.profdata --show-branches=count --show-expansions >> show.txt
		)

		add_custom_command( 
			TARGET ${project_name} POST_BUILD
			COMMAND echo Generate HTML report...
			COMMAND del /s /q coverage.windows.lcov.info # It appears that coverage.windows.lcov.info impact this generation...
			COMMAND ./grcov.exe --llvm -t html -b ./${VS_CONFIG}/ -s ./../../
					--llvm-path ${CMAKE_CXX_COMPILER_PATH}
					--branch
					# --keep-only "src/**" 
					# --keep-only "interface/**"
					# --excl-start "^.*LCOV_EXCL_START.*" 
					# --excl-stop "^.*LCOV_EXCL_STOP.*" 
					# --excl-line "\"(\\s*^.*GTEST_TEST\\.*)|(^.*LCOV_EXCL_LINE.*)\"" 
					# --excl-br-start "^.*LCOV_EXCL_START.*" 
					# --excl-br-stop "^.*LCOV_EXCL_STOP.*" 
					# --excl-br-line "\"(\\s*^.*GTEST_TEST\\.*)|(^.*LCOV_EXCL_BR_LINE.*)\"" 
					-o windows
					..
		)

		# add_custom_command( 
		# 	TARGET ${project_name} POST_BUILD
		# 	COMMAND echo Generate LCOV report...
		# 	COMMAND del /s /q coverage.windows.lcov.info
		# 	COMMAND ./grcov.exe --llvm -t lcov -b ./${VS_CONFIG}/ -s ./../../
		# 			--llvm-path ${CMAKE_CXX_COMPILER_PATH}
		# 			--branch
		# 			--keep-only "src/*"
		# 			--keep-only "interface/*"
		# 			--excl-start "^.*LCOV_EXCL_START.*" 
		# 			--excl-stop "^.*LCOV_EXCL_STOP.*" 
		# 			--excl-line "\"(\\s*^.*GTEST_TEST\\.*)|(^.*LCOV_EXCL_LINE.*)\"" 
		# 			--excl-br-start "^.*LCOV_EXCL_START.*" 
		# 			--excl-br-stop "^.*LCOV_EXCL_STOP.*" 
		# 			--excl-br-line "\"(\\s*^.*GTEST_TEST\\.*)|(^.*LCOV_EXCL_BR_LINE.*)\"" 
		# 			-o coverage.windows.lcov.info
		# 			..
		# )

		# add_custom_command( 
		#  	TARGET ${project_name} POST_BUILD
		# 	COMMAND echo "Download Grcov..."
		# 	COMMAND Powershell.exe Invoke-WebRequest -Uri https://github.com/mozilla/grcov/releases/download/v0.8.13/grcov-x86_64-pc-windows-msvc.zip -OutFile ./grcov-x86_64-pc-windows-msvc.zip
		# 	COMMAND Powershell.exe Expand-Archive -Path ./grcov-x86_64-pc-windows-msvc.zip -DestinationPath . -F
		# 	COMMAND echo "Delete existing covering files..."
		# 	COMMAND del /S /Q ${lib_name}.profraw
		# 	COMMAND echo "Start coverage..."
		#  	COMMAND ${CMAKE_COMMAND} -E env LLVM_PROFILE_FILE="${lib_name}.profraw" ./${VS_CONFIG}/${project_name}.exe
		# 	COMMAND echo "Merge coverage info..."
		# 	COMMAND ${CMAKE_CXX_COMPILER_PATH}/llvm-profdata merge -sparse ${lib_name}.profraw -o ${lib_name}.profdata
		# 	COMMAND echo "Generate HTML report..."
		# 	COMMAND ./grcov.exe --llvm -t html -b ./${VS_CONFIG}/ -s ./../../
		# 			--llvm-path ${CMAKE_CXX_COMPILER_PATH}
		# 			--branch
		# 			--keep-only "src/*" 
		# 			--keep-only "interface/*"
		# 			--excl-start "^.*LCOV_EXCL_START.*" 
		# 			--excl-stop "^.*LCOV_EXCL_STOP.*" 
		# 			--excl-line "\"(\\s*^.*GTEST_TEST\\.*)|(^.*LCOV_EXCL_LINE.*)\"" 
		# 			--excl-br-start "^.*LCOV_EXCL_START.*" 
		# 			--excl-br-stop "^.*LCOV_EXCL_STOP.*" 
		# 			--excl-br-line "\"(\\s*^.*GTEST_TEST\\.*)|(^.*LCOV_EXCL_BR_LINE.*)\"" 
		# 			-o windows
		# 			..
		# 	COMMAND echo "Generate LCOV report..."
		# 	COMMAND ./grcov.exe --llvm -t lcov -b ./${VS_CONFIG}/ -s ./../../
		# 			--llvm-path ${CMAKE_CXX_COMPILER_PATH}
		# 			--branch
		# 			--keep-only "src/*"
		# 			--keep-only "interface/*"
		# 			--excl-start "^.*LCOV_EXCL_START.*" 
		# 			--excl-stop "^.*LCOV_EXCL_STOP.*" 
		# 			--excl-line "\"(\\s*^.*GTEST_TEST\\.*)|(^.*LCOV_EXCL_LINE.*)\"" 
		# 			--excl-br-start "^.*LCOV_EXCL_START.*" 
		# 			--excl-br-stop "^.*LCOV_EXCL_STOP.*" 
		# 			--excl-br-line "\"(\\s*^.*GTEST_TEST\\.*)|(^.*LCOV_EXCL_BR_LINE.*)\"" 
		# 			-o coverage.windows.lcov.info
		# 			..
			
		# )
	endif()
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	target_compile_options(${project_name} PRIVATE -fprofile-instr-generate -fcoverage-mapping)
	target_link_options(${project_name} PRIVATE -fprofile-instr-generate -fcoverage-mapping)
	target_compile_options(${lib_name} PRIVATE -fprofile-instr-generate -fcoverage-mapping)
	target_link_options(${lib_name} PRIVATE -fprofile-instr-generate -fcoverage-mapping)
	
	add_custom_command( 
		 	TARGET ${project_name} POST_BUILD
			COMMAND echo Download Grcov...
			COMMAND curl -L https://github.com/mozilla/grcov/releases/latest/download/grcov-x86_64-unknown-linux-gnu.tar.bz2 | tar jxf -
		)

    	add_custom_command( 
		 	TARGET ${project_name} POST_BUILD
			COMMAND echo Start coverage...
			COMMAND ${CMAKE_COMMAND} -E env LLVM_PROFILE_FILE="${lib_name}.profraw" ./${project_name}
		)

		add_custom_command( 
			TARGET ${project_name} POST_BUILD
			COMMAND echo Merge coverage info...
			COMMAND ${CMAKE_CXX_COMPILER_PATH}/llvm-profdata merge ${lib_name}.profraw -o ${lib_name}.profdata
		)

		add_custom_command( 
			TARGET ${project_name} POST_BUILD
			COMMAND echo Show profraw...
			COMMAND ${CMAKE_CXX_COMPILER_PATH}/llvm-profdata show --all-functions ${lib_name}.profdata >> profraw.info.txt
		)

		add_custom_command( 
			TARGET ${project_name} POST_BUILD
			COMMAND echo Show coverage info...
			COMMAND rm -f show.txt
			COMMAND ${CMAKE_CXX_COMPILER_PATH}/llvm-cov show ./${project_name} -instr-profile=${lib_name}.profdata -debuginfod >> show.txt
			#COMMAND ${CMAKE_CXX_COMPILER_PATH}/llvm-cov show ./${VS_CONFIG}/${project_name}.exe -instr-profile=default.profdata --show-branches=count --show-expansions >> show.txt
		)


		add_custom_command( 
			TARGET ${project_name} POST_BUILD
			COMMAND echo Generate HTML report...
			COMMAND rm -f coverage.ubuntu.lcov.info
			COMMAND ./grcov --llvm -t html -b . -s ./../../
					--llvm-path /usr/bin/
					--guess-directory-when-missing
					--branch
					--keep-only "src/*" 
					--keep-only "interface/*"
					--excl-start "^.*LCOV_EXCL_START.*" 
					--excl-stop "^.*LCOV_EXCL_STOP.*" 
					--excl-line "\"(\\s*^.*GTEST_TEST\\.*)|(^.*LCOV_EXCL_LINE.*)\"" 
					--excl-br-start "^.*LCOV_EXCL_START.*" 
					--excl-br-stop "^.*LCOV_EXCL_STOP.*" 
					--excl-br-line "\"(\\s*^.*GTEST_TEST\\.*)|(^.*LCOV_EXCL_BR_LINE.*)\"" 
					-o ubuntu
					..
		)

		add_custom_command( 
			TARGET ${project_name} POST_BUILD
			COMMAND echo Generate LCOV report...
			COMMAND rm -f coverage.ubuntu.lcov.info
			COMMAND ./grcov --llvm -t lcov -b . -s ./../../
					--llvm-path /usr/bin/
					--branch
					--keep-only "src/*"
					--keep-only "interface/*"
					--excl-start "^.*LCOV_EXCL_START.*" 
					--excl-stop "^.*LCOV_EXCL_STOP.*" 
					--excl-line "\"(\\s*^.*GTEST_TEST\\.*)|(^.*LCOV_EXCL_LINE.*)\"" 
					--excl-br-start "^.*LCOV_EXCL_START.*" 
					--excl-br-stop "^.*LCOV_EXCL_STOP.*" 
					--excl-br-line "\"(\\s*^.*GTEST_TEST\\.*)|(^.*LCOV_EXCL_BR_LINE.*)\"" 
					-o coverage.ubuntu.lcov.info
					..
		)


	# add_custom_command(
	# 	TARGET ${project_name} POST_BUILD
	# 	COMMENT "Run ${project_name}.exe"
	# 	COMMAND curl -L https://github.com/mozilla/grcov/releases/latest/download/grcov-x86_64-unknown-linux-gnu.tar.bz2 | tar jxf -
	# 	COMMAND ${CMAKE_COMMAND} -E env LLVM_PROFILE_FILE="${lib_name}.profraw" ./${project_name}
	# 	COMMAND llvm-profdata merge -sparse ${lib_name}.profraw -o ${lib_name}.profdata
	# 	COMMAND ./grcov --llvm -t html -b . -s ./../../
	# 			--llvm-path /usr/bin/
	# 			--branch
	# 			--keep-only "src/*" 
	# 			--keep-only "interface/*"
	# 			--excl-start "^.*LCOV_EXCL_START.*" 
	# 			--excl-stop "^.*LCOV_EXCL_STOP.*" 
	# 			--excl-line "\"(\\s*^.*GTEST_TEST\\.*)|(^.*LCOV_EXCL_LINE.*)\"" 
	# 			--excl-br-start "^.*LCOV_EXCL_START.*" 
	# 			--excl-br-stop "^.*LCOV_EXCL_STOP.*" 
	# 			--excl-br-line "\"(\\s*^.*GTEST_TEST\\.*)|(^.*LCOV_EXCL_BR_LINE.*)\"" 
	# 			-o ubuntu
	# 			..
	# 	COMMAND ./grcov --llvm -t lcov -b . -s ./../../
	# 			--llvm-path /usr/bin/
	# 			--branch
	# 			--keep-only "src/*"
	# 			--keep-only "interface/*"
	# 			--excl-start "^.*LCOV_EXCL_START.*" 
	# 			--excl-stop "^.*LCOV_EXCL_STOP.*" 
	# 			--excl-line "\"(\\s*^.*GTEST_TEST\\.*)|(^.*LCOV_EXCL_LINE.*)\"" 
	# 			--excl-br-start "^.*LCOV_EXCL_START.*" 
	# 			--excl-br-stop "^.*LCOV_EXCL_STOP.*" 
	# 			--excl-br-line "\"(\\s*^.*GTEST_TEST\\.*)|(^.*LCOV_EXCL_BR_LINE.*)\"" 
	# 			-o coverage.ubuntu.lcov.info
	# 			..
	# )
endif()
endfunction()