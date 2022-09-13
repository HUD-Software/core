#!/bin/sh

#=========================
# Print the help function
#=========================
print_help() {
	echo
	echo "Syntax: test.sh [generator] [arch] [compiler] [config]"
	echo
	echo " [generator] The generator used to test the project"
	echo "    * Makefile"
	echo 
	echo " [arch] Set the architecture used to test the project"
	echo "   * x86_64"
	echo "   * x86"
	echo
	echo " [compiler] Set the compiler used to test the project"
	echo "   * GCC"
	echo "   * Clang"
	echo
	echo " [config] Configuration used to test the project"
	echo "   * Debug"
	echo "   * Release"
	echo "   * DebugOptimized"
    echo
    echo " [test] Run tests matching regular expression. Run all test if empty."
	echo
	exit 1
}

#========================
# Check input parameters
#========================
if [ "$2" != "x86_64" ] && [ "$2" != "x86" ]; then
echo "[arch] is not correct: "$2""
print_help
exit 1
fi

if [ "$3" != "GCC" ] && [ "$3" != "Clang" ]; then
echo "[compiler] is not correct: "$3""
print_help
exit 1
fi

if [ "$4" != "Debug" ] && [ "$4" != "Release" ] && [ "$4" != "DebugOptimized" ]; then
echo "[config] is not correct: "$4""
print_help
exit 1
fi

#====================================
# Check and call CTest
#====================================

if [ "$1" = "Makefile" ]; then

export current_dir=$(pwd)
export build_dir=$(./setup_build_env.sh Makefile "$2" "$3" "$4")
if [ $? -eq 0 ]; then
(
	cd "$build_dir"
    if [ -z "$5" ]; then
    ctest -C %config% --verbose
    else
    ctest -C %config% -R %test% --verbose
    fi
)
fi
else
echo "[generator] is not correct"
print_help
exit 1
fi

exit $?