#!/bin/sh

#=========================
# Print the help function
#=========================
print_help() {
	echo
	echo "Syntax: generate_makefile.sh [dir] [arch] [compiler] [config]"
	echo
	echo " [dir] Path to the directory that contains the root CMakeLists.txt"
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

if [ "$2" != "x86_64" ] && [ "$2" != "x86" ]; then
echo "[arch] is not correct: $2"
print_help
exit 1
fi


if [ "$3" = "GCC" ]; then
export CXX="/usr/bin/g++"
export CC="/usr/bin/gcc"
elif [ "$3" = "Clang" ]; then
export CXX=/usr/bin/clang++
export CC=/usr/bin/clang
else
echo "[compiler] is not correct"
print_help
exit 1
fi

if [ "$4" = "Debug" ] || [ "$4" = "Release" ] || [ "$4" = "DebugOptimized" ]; then
	if ["$4" = "DebugOptimized" ]; then
		CMAKE_BUILD_TYPE=RelWithDebInfo
	else
		CMAKE_BUILD_TYPE=$4
	fi
else
echo "[config] is not correct"
print_help
exit 1
fi

#=========================
# Generate Unix Makefiles
#=========================
(
	cd "$build_dir"
	cmake "$1" -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=$CMAKE_BUILD_TYPE
)
exit $?
