#!/bin/sh

#=========================
# Print the help function
#=========================
print_help() {
	echo
	echo "Syntax: build_makefile.sh [arch] [compiler] [config]"
	echo
	echo " [arch] Set the architecture used to build the project"
	echo "   * x86_64"
	echo "   * x86"
	echo
	echo " [compiler] Set the compiler used to build the project"
	echo "   * GCC"
	echo "   * Clang"
	echo
	echo " [config] Configuration used to build the project"
	echo "   * Debug"
	echo "   * Release"
	echo "   * DebugOptimized"
	echo
	exit 1
}

#========================
# Check input parameters
#========================
if [ "$1" != "x86_64" ] && [ "$1" != "x86" ]; then
echo "[arch] is not correct: "$1""
print_help
exit 1
fi

if [ "$2" != "GCC" ] && [ "$2" != "Clang" ]; then
echo "[compiler] is not correct: "$2""
print_help
exit 1
fi

if [ "$3" != "Debug" ] && [ "$3" != "Release" ] && [ "$3" != "DebugOptimized" ]; then
echo "[config] is not correct: "$3""
print_help
exit 1
fi


#=========================
# Build Unix Makefiles
#=========================
export current_dir=$(pwd)
export build_dir=$(./setup_build_env.sh Makefile "$1" "$2" "$3")
if [ $? -eq 0 ]; then
(
	cd "$build_dir"
	make
)
fi

exit $?