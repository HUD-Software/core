if(NOT UNIX)
    message(FATAL_ERROR "Windows coverage.cmake should not be used if not Windows OS")
endif()

function(enable_unix_coverage project_name lib_name)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        message("Enable Clang coverage")
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
            COMMAND llvm-profdata merge ${lib_name}.profraw -o ${lib_name}.profdata
        )

        add_custom_command( 
            TARGET ${project_name} POST_BUILD
            COMMAND echo Show profraw...
            COMMAND llvm-profdata show --all-functions ${lib_name}.profdata >> profraw.info.txt
        )

        add_custom_command( 
            TARGET ${project_name} POST_BUILD
            COMMAND echo Show coverage info...
            COMMAND rm -f show.txt
            COMMAND llvm-cov show ./${project_name} -instr-profile=${lib_name}.profdata --show-expansions >> show.txt
        )


        add_custom_command( 
            TARGET ${project_name} POST_BUILD
            COMMAND echo Generate HTML report...
            COMMAND rm -f coverage.linux.clang.lcov.info
            COMMAND ./grcov --llvm -t html -b . -s ./../../
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
                    -o ubuntu
                    ..
        )

        add_custom_command( 
            TARGET ${project_name} POST_BUILD
            COMMAND echo Generate LCOV report...
            COMMAND rm -f coverage.linux.clang.lcov.info
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
                    -o coverage.linux.clang.lcov.info
                    ..
        )
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        message("Enable GCC coverage")
        target_compile_options(${project_name} PRIVATE --coverage)
        target_link_options(${project_name} PRIVATE --coverage)
        target_compile_options(${lib_name} PRIVATE --coverage)
        
        add_custom_command( 
            TARGET ${project_name} POST_BUILD
            COMMAND echo Download Grcov...
            COMMAND if [ ! -e grcov ];then (curl -L https://github.com/mozilla/grcov/releases/latest/download/grcov-x86_64-unknown-linux-gnu.tar.bz2 | tar jxf -) fi
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
                    -o linux.gcc
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
                    -o coverage.linux.gcc.lcov.info
                    ..
        )
    else()
        message(STATUS "Unsupported compiler")
    endif()
endfunction()