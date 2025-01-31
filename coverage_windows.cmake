
if(NOT WIN32)
    message(FATAL_ERROR "Windows coverage.cmake should not be used if not Windows OS")
endif()

message("Enable GCC coverage")

function(enable_windows_coverage project_name lib_name)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        message("Enable MSCV coverage with Clang-cl")

        target_compile_options(${project_name} PRIVATE -fprofile-instr-generate -fcoverage-mapping)
        target_compile_options(${lib_name} PRIVATE -fprofile-instr-generate -fcoverage-mapping)
        target_link_options(${project_name} PRIVATE --coverage)
        # Add clang lib path to libraries paths
        get_filename_component(CMAKE_CXX_COMPILER_PATH ${CMAKE_CXX_COMPILER} DIRECTORY)
        target_link_directories(${project_name} PRIVATE "${CMAKE_CXX_COMPILER_PATH}\\..\\lib\\clang\\${CMAKE_CXX_COMPILER_VERSION}\\lib\\windows\\")

        add_custom_command( 
            TARGET ${project_name} POST_BUILD
            COMMAND echo Download Grcov...
            COMMAND Powershell.exe Invoke-WebRequest -Uri https://github.com/mozilla/grcov/releases/latest/download/grcov-x86_64-pc-windows-msvc.zip -OutFile ./grcov-x86_64-pc-windows-msvc.zip
            COMMAND Powershell.exe Expand-Archive -Path ./grcov-x86_64-pc-windows-msvc.zip -DestinationPath . -F
        )

        add_custom_command( 
            TARGET ${project_name} POST_BUILD
            COMMAND echo Start coverage...
            COMMAND ${CMAKE_COMMAND} -E env LLVM_PROFILE_FILE="${project_name}.profraw" ./${VS_CONFIG}/${project_name}.exe
        )

        add_custom_command( 
            TARGET ${project_name} POST_BUILD
            COMMAND echo Merge coverage info...
            COMMAND ${CMAKE_CXX_COMPILER_PATH}/llvm-profdata merge -sparse ${project_name}.profraw -o ${project_name}.profdata
        )

        add_custom_command( 
            TARGET ${project_name} POST_BUILD
            COMMAND echo Show coverage info...
            COMMAND ${CMAKE_CXX_COMPILER_PATH}/llvm-cov report ./${VS_CONFIG}/${project_name}.exe -instr-profile=${project_name}.profdata -dump
        )

        add_custom_command( 
            TARGET ${project_name} POST_BUILD
            COMMAND echo Show coverage info...
            COMMAND ${CMAKE_CXX_COMPILER_PATH}/llvm-cov show ./${VS_CONFIG}/${project_name}.exe -instr-profile=${project_name}.profdata --show-expansions >> show.txt
        )

        file(REMOVE coverage.windows.clang.lcov.info)
        add_custom_command( 
            TARGET ${project_name} POST_BUILD
            COMMAND echo Generate HTML report...
            COMMAND ./grcov.exe --llvm -t html -b ./${VS_CONFIG}/ -s ./../../
                    --llvm-path ${CMAKE_CXX_COMPILER_PATH}
                    --branch
                    --keep-only "src/*" 
                    --keep-only "interface/*"
                    --excl-start "^.*LCOV_EXCL_START.*" 
                    --excl-stop "^.*LCOV_EXCL_STOP.*" 
                    --excl-line "\"(\\s*^.*GTEST_TEST\\.*)|(^.*LCOV_EXCL_LINE.*)\"" 
                    --excl-br-start "^.*LCOV_EXCL_START.*" 
                    --excl-br-stop "^.*LCOV_EXCL_STOP.*" 
                    --excl-br-line "\"(\\s*^.*GTEST_TEST\\.*)|(^.*LCOV_EXCL_BR_LINE.*)\"" 
                    -o windows.clang
                    ..
        )

        add_custom_command( 
            TARGET ${project_name} POST_BUILD
            COMMAND echo Generate LCOV report...
            COMMAND ./grcov.exe --llvm -t lcov -b ./${VS_CONFIG}/ -s ./../../
                    --llvm-path ${CMAKE_CXX_COMPILER_PATH}
                    --branch
                    --keep-only "src/*"
                    --keep-only "interface/*"
                    --excl-start "^.*LCOV_EXCL_START.*" 
                    --excl-stop "^.*LCOV_EXCL_STOP.*" 
                    --excl-line "\"(\\s*^.*GTEST_TEST\\.*)|(^.*LCOV_EXCL_LINE.*)\"" 
                    --excl-br-start "^.*LCOV_EXCL_START.*" 
                    --excl-br-stop "^.*LCOV_EXCL_STOP.*" 
                    --excl-br-line "\"(\\s*^.*GTEST_TEST\\.*)|(^.*LCOV_EXCL_BR_LINE.*)\"" 
                    -o coverage.windows.clang.lcov.info
                    ..
        )
    elseif(MSVC)
        set(MSVC_CODECOVERAGE_CONSOLE_PATH "C:\\Program Files\\Microsoft Visual Studio\\2022\\Enterprise\\Common7\\IDE\\Extensions\\Microsoft\\CodeCoverage.Console\\Microsoft.CodeCoverage.Console.exe" CACHE STRING "Path to Microsoft.CodeCoverage.Console.exe")
        find_program(MSVC_CODECOVERAGE_CONSOLE_EXE ${MSVC_CODECOVERAGE_CONSOLE_PATH})
        if(NOT MSVC_CODECOVERAGE_CONSOLE_EXE)
            message(FATAL_ERROR "Code coverage on Windows need Microsoft.CodeCoverage.Console.exe available in Visual Studio 2022 17.3 Enterprise Edition")
        endif()
        message("Enable MSCV coverage with Microsoft.CodeCoverage.Console.exe")

        target_link_options(${project_name} PRIVATE /PROFILE)

        add_custom_command(
            TARGET ${project_name} POST_BUILD
            COMMAND echo Instrument ${project_name}.exe
            COMMAND ${MSVC_CODECOVERAGE_CONSOLE_EXE} instrument ${VS_CONFIG}/${project_name}.exe 
                    --settings ../../coverage.runsettings
        )
        add_custom_command(
            TARGET ${project_name} POST_BUILD
            COMMAND echo Collect ${project_name}.exe
            COMMAND ${MSVC_CODECOVERAGE_CONSOLE_EXE} collect ${VS_CONFIG}/${project_name}.exe 
                    --output ${VS_CONFIG}/coverage.windows.msvc.cobertura 
                    --output-format cobertura 
                    --settings ../../coverage.runsettings
        )
    elseif(MINGW)
        if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
            message("Enable GCC coverage")
            target_compile_options(${project_name} PRIVATE --coverage)
            target_link_options(${project_name} PRIVATE --coverage)
            target_compile_options(${lib_name} PRIVATE --coverage)
            
            add_custom_command( 
                TARGET ${project_name} POST_BUILD
                COMMAND echo Download Grcov...
                COMMAND Powershell.exe Invoke-WebRequest -Uri https://github.com/mozilla/grcov/releases/latest/download/grcov-x86_64-pc-windows-msvc.zip -OutFile ./grcov-x86_64-pc-windows-msvc.zip
                COMMAND Powershell.exe Expand-Archive -Path ./grcov-x86_64-pc-windows-msvc.zip -DestinationPath . -F
            )
            
            add_custom_command( 
                TARGET ${project_name} POST_BUILD
                COMMAND echo Start coverage...
                COMMAND ./${project_name}
            )

            add_custom_command( 
                TARGET ${project_name} POST_BUILD
                COMMAND echo Generate HTML report...
                COMMAND ./grcov -t html -b . -s ./../../
                        --llvm-path /usr/bin/
                        #--branch
                        --keep-only "src/*" 
                        --keep-only "interface/*"
                        --excl-start "^.*LCOV_EXCL_START.*" 
                        --excl-stop "^.*LCOV_EXCL_STOP.*" 
                        --excl-line "\"(\\s*^.*GTEST_TEST\\.*)|(^.*LCOV_EXCL_LINE.*)\"" 
                        --excl-br-start "^.*LCOV_EXCL_START.*" 
                        --excl-br-stop "^.*LCOV_EXCL_STOP.*" 
                        --excl-br-line "\"(\\s*^.*GTEST_TEST\\.*)|(^.*LCOV_EXCL_BR_LINE.*)\"" 
                        -o windows.mingw
                        ..
            )
            add_custom_command( 
                TARGET ${project_name} POST_BUILD
                COMMAND echo Generate LCOV report...
                COMMAND ./grcov -t lcov -b . -s ./../../
                        --llvm-path /usr/bin/
                        #--branch
                        --keep-only "src/*"
                        --keep-only "interface/*"
                        --excl-start "^.*LCOV_EXCL_START.*" 
                        --excl-stop "^.*LCOV_EXCL_STOP.*" 
                        --excl-line "\"(\\s*^.*GTEST_TEST\\.*)|(^.*LCOV_EXCL_LINE.*)\"" 
                        --excl-br-start "^.*LCOV_EXCL_START.*" 
                        --excl-br-stop "^.*LCOV_EXCL_STOP.*" 
                        --excl-br-line "\"(\\s*^.*GTEST_TEST\\.*)|(^.*LCOV_EXCL_BR_LINE.*)\"" 
                        -o coverage.windows.mingw.lcov.info
                        ..
            )
        else()
            message(FATAL_ERROR "Unsupported compiler for MinGW")
        endif()
    else()
        message(STATUS "Unsupported compiler")
    endif()
endfunction()