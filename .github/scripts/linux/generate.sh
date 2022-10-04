#!/bin/sh

#========================
# Check CMake is install
#========================
if ! test cmake
then
echo "CMake not found"
fi

#=========================
# Print the help function
#=========================
print_help() {
	echo
	echo "Syntax: generate.sh [dir] [generator] [arch] [compiler] [config]"
	echo
	echo " [dir] Path to the directory that contains the root CMakeLists.txt"
	echo
	echo " [generator] The generator used to generate the project"
	echo "    * Makefile"
	echo 
	echo " [arch] Set the architecture used to generate the project"
	echo "   * x86_64"
	echo "   * x86"
	echo
	echo " [compiler] Set the compiler used to generate the project"
	echo "   * GCC"
	echo "   * Clang"
	echo
	echo " [config] Configuration used to generate the project"
	echo "   * Debug"
	echo "   * Release"
	echo "   * DebugOptimized"
	echo
	exit 1
}

#========================
# Check input parameters
#========================
if [ ! -d "$1" ]; then
echo "[dir] $1 do not exist"
print_help
exit 1
fi

if [ ! -f "$1/CMakeLists.txt" ]; then
echo "[dir] do not contains CMakeLists.txt"
echo "$1/CMakeLists.txt"
print_help
exit 1
fi

if [ "$3" != "x86_64" ] && [ "$3" != "x86" ]; then
echo "[arch] is not correct: $3"
print_help
exit 1
fi

if [ "$4" != "GCC" ] && [ "$4" != "Clang" ]; then
echo "[compiler] is not correct: $4"
print_help
exit 1
fi

if [ "$5" != "Debug" ] && [ "$5" != "Release" ] && [ "$5" != "DebugOptimized" ]; then
echo "[config] is not correct: $5"
print_help
exit 1
fi

#====================================
# Check and call the generate script
#====================================
if [ "$2" = "Makefile" ]; then
"$(dirname "$(realpath "$0")")"/generate_makefile.sh "$1" "$3" "$4" "$5"
else
echo "[generator] is not correct"
print_help
exit 1
fi


exit $?