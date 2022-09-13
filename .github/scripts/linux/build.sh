#!/bin/sh


#=========================
# Print the help function
#=========================
print_help() {
	echo
	echo "Syntax: build.sh [generator] [arch] [compiler] [config]"
	echo
	echo " [generator] The generator used to build the project"
	echo "    * Makefile"
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
# Check and call the build script
#====================================
if [ "$1" = "Makefile" ]; then
./build_makefile.sh "$2" "$3" "$4"
else
echo "[generator] is not correct"
print_help
exit 1
fi

exit $?