cmake_minimum_required(VERSION 3.18)
# If operating system for which CMake to build is Windows set SDK version to 10
set(CMAKE_SYSTEM_VERSION 10.0)

# Set project name
project(Core
		DESCRIPTION "HUD Engine Core"
		LANGUAGES CXX
)

# Set the name of the target
set(target_name OSLayer)

set(CMAKE_CXX_STANDARD 20)

# Define a grouping for source files in IDE project generation
# source_files variable is a list all files in Implementation and Interface directory
set( src
	src/Hash/Crc32.cpp
)

#Linux implementations
if(UNIX AND NOT APPLE)
set( src ${implementations}
	src/OSLinux/LinuxDebugger.cpp
)
endif()

set( interface
	src/core/Allocators/AlignedHeapAllocator.h
	src/core/Allocators/Allocation.h
	src/core/Allocators/HeapAllocator.h
	src/core/Containers/AlignedBuffer.h
	src/core/Containers/Array.h
	src/core/Containers/Optional.h
	src/core/Containers/Pair.h
	src/core/Containers/SharedPointer.h
	src/core/Containers/Tuple.h
	src/core/Containers/TupleElement.h
	src/core/Containers/TupleSize.h
	src/core/Containers/UniquePointer.h
	src/core/Hash/CityHash.h
	src/core/Hash/Crc32.h
	src/core/Iterators/RandomAccessIterator.h
	src/core/OSCommon/CommonAtomics.h
	src/core/OSCommon/CommonChar.h
	src/core/OSCommon/CommonCString.h
	src/core/OSCommon/CommonMemory.h
	src/core/OSCommon/CommonTypes.h
	src/core/OSCommon/CommonUuid.h
	src/core/OSLinux/LinuxAtomics.h
	src/core/OSLinux/LinuxChar.h
	src/core/OSLinux/LinuxCString.h
	src/core/OSLinux/LinuxDebugger.h
	src/core/OSLinux/LinuxMemory.h
	src/core/OSLinux/LinuxTypes.h
	src/core/OSLinux/LinuxUuid.h
	src/core/OSWindows/WindowsAPI.h
	src/core/OSWindows/WindowsAtomics.h
	src/core/OSWindows/WindowsAtomicsIntel.h
	src/core/OSWindows/WindowsChar.h
	src/core/OSWindows/WindowsCString.h
	src/core/OSWindows/WindowsDebugger.h
	src/core/OSWindows/WindowsMemory.h
	src/core/OSWindows/WindowsTypes.h
	src/core/OSWindows/WindowsUuid.h
	src/core/Templates/BitCast.h
	src/core/Templates/Declval.h
	src/core/Templates/DefaultDeleter.h
	src/core/Templates/Equal.h
	src/core/Templates/Forward.h
	src/core/Templates/Greater.h
	src/core/Templates/GreaterEqual.h
	src/core/Templates/IntegerSequence.h
	src/core/Templates/Less.h
	src/core/Templates/LessEqual.h
	src/core/Templates/LexicographicalCompare.h
	src/core/Templates/Max.h
	src/core/Templates/Min.h
	src/core/Templates/Move.h
	src/core/Templates/SelectDeleterPointerType.h
	src/core/Templates/Swap.h
	src/core/Traits/AddConst.h
	src/core/Traits/AddCV.h
	src/core/Traits/AddLValueReference.h
	src/core/Traits/AddPointer.h
	src/core/Traits/AddRValueReference.h
	src/core/Traits/AddVolatile.h
	src/core/Traits/And.h
	src/core/Traits/ApplyCV.h
	src/core/Traits/CommonType.h
	src/core/Traits/Conditional.h
	src/core/Traits/Decay.h
	src/core/Traits/EnableIf.h
	src/core/Traits/Extent.h
	src/core/Traits/HasUniqueObjectRepresentations.h
	src/core/Traits/IntegralConstant.h
	src/core/Traits/IsAnyTypeIn.h
	src/core/Traits/IsArithmetic.h
	src/core/Traits/IsArray.h
	src/core/Traits/IsAssignable.h
	src/core/Traits/IsBaseOf.h
	src/core/Traits/IsBitwiseComparable.h
	src/core/Traits/IsBitwiseConvertible.h
	src/core/Traits/IsBitwiseCopyAssignable.h
	src/core/Traits/IsBitwiseCopyConstructible.h
	src/core/Traits/IsBitwiseMoveAssignable.h
	src/core/Traits/IsBitwiseMoveConstructible.h
	src/core/Traits/IsBool.h
	src/core/Traits/IsBoundedArray.h
	src/core/Traits/IsClass.h
	src/core/Traits/IsComparableWithEqual.h
	src/core/Traits/IsComparableWithGreater.h
	src/core/Traits/IsComparableWithGreaterEqual.h
	src/core/Traits/IsComparableWithLess.h
	src/core/Traits/IsComparableWithLessEqual.h
	src/core/Traits/IsComparableWithNotEqual.h
	src/core/Traits/IsConst.h
	src/core/Traits/IsConstantEvaluated.h
	src/core/Traits/IsConstructible.h
	src/core/Traits/IsConvertible.h
	src/core/Traits/IsCopyAssignable.h
	src/core/Traits/IsCopyConstructible.h
	src/core/Traits/IsDefaultConstructible.h
	src/core/Traits/IsDerivedFrom.h
	src/core/Traits/IsDestructible.h
	src/core/Traits/IsEnum.h
	src/core/Traits/IsExplicitlyConstructible.h
	src/core/Traits/IsExplicitlyCopyConstructible.h
	src/core/Traits/IsExplicitlyDefaultConstructible.h
	src/core/Traits/IsExplicitlyMoveConstructible.h
	src/core/Traits/IsFloatingPoint.h
	src/core/Traits/IsFunction.h
	src/core/Traits/IsImplicitlyConstructible.h
	src/core/Traits/IsImplicitlyCopyConstructible.h
	src/core/Traits/IsImplicitlyDefaultConstructible.h
	src/core/Traits/IsImplicitlyMoveConstructible.h
	src/core/Traits/IsIntegral.h
	src/core/Traits/IsLValueReference.h
	src/core/Traits/IsMemberFunctionPointer.h
	src/core/Traits/IsMemberObjectPointer.h
	src/core/Traits/IsMemberPointer.h
	src/core/Traits/IsMoveAssignable.h
	src/core/Traits/IsMoveConstructible.h
	src/core/Traits/IsNothrowAssignable.h
	src/core/Traits/IsNothrowConstructible.h
	src/core/Traits/IsNothrowCopyAssignable.h
	src/core/Traits/IsNothrowCopyConstructible.h
	src/core/Traits/IsNothrowDefaultConstructible.h
	src/core/Traits/IsNothrowDestructible.h
	src/core/Traits/IsNothrowMoveAssignable.h
	src/core/Traits/IsNothrowMoveConstructible.h
	src/core/Traits/IsNothrowSwappable.h
	src/core/Traits/IsNotSame.h
	src/core/Traits/IsNullptr.h
	src/core/Traits/IsObject.h
	src/core/Traits/IsPointer.h
	src/core/Traits/IsPowerOfTwo.h
	src/core/Traits/IsReference.h
	src/core/Traits/IsReferenceable.h
	src/core/Traits/IsRValueReference.h
	src/core/Traits/IsSame.h
	src/core/Traits/IsSameSize.h
	src/core/Traits/IsScalar.h
	src/core/Traits/IsSigned.h
	src/core/Traits/IsStandardLayout.h
	src/core/Traits/IsSwappable.h
	src/core/Traits/IsTrivial.h
	src/core/Traits/IsTriviallyAssignable.h
	src/core/Traits/IsTriviallyConstructible.h
	src/core/Traits/IsTriviallyCopyable.h
	src/core/Traits/IsTriviallyCopyAssignable.h
	src/core/Traits/IsTriviallyCopyConstructible.h
	src/core/Traits/IsTriviallyDefaultConstructible.h
	src/core/Traits/IsTriviallyDestructible.h
	src/core/Traits/IsTriviallyMoveAssignable.h
	src/core/Traits/IsTriviallyMoveConstructible.h
	src/core/Traits/IsTriviallySwappable.h
	src/core/Traits/IsUnboundedArray.h
	src/core/Traits/IsUnion.h
	src/core/Traits/IsUnsigned.h
	src/core/Traits/IsVoid.h
	src/core/Traits/IsVolatile.h
	src/core/Traits/MakeSigned.h
	src/core/Traits/MakeUnsigned.h
	src/core/Traits/Not.h
	src/core/Traits/Or.h
	src/core/Traits/RemoveAllExtents.h
	src/core/Traits/RemoveConst.h
	src/core/Traits/RemoveCV.h
	src/core/Traits/RemoveCVRef.h
	src/core/Traits/RemoveExtent.h
	src/core/Traits/RemovePointer.h
	src/core/Traits/RemoveReference.h
	src/core/Traits/RemoveVolatile.h
	src/core/Traits/UnderlyingType.h
	src/core/Traits/VoidT.h
	src/core/Assert.h
	src/core/Atomics.h
	src/core/Char.h
	src/core/Compilation.h
	src/core/CompilerDefines.h
	src/core/Containers
	src/core/CString.h
	src/core/Debugger.h
	src/core/Defines.h
	src/core/Hash
	src/core/Hash.h
	src/core/Iterators
	src/core/Math.h
	src/core/Memory.h
	src/core/Minimal.h
	src/core/OSCommon
	src/core/OSLinux
	src/core/OSWindows
	src/core/Slice.h
	src/core/String.h
	src/core/TagInit.h
	src/core/TagInPlace.h
	src/core/Templates
	src/core/Traits
	src/core/Types.h
	src/core/Uuid.h
)

#file( GLOB_RECURSE source_files src/* interface/* )
source_group( TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${interface} ${src})

# Add OSLayer to the project
add_library(Core STATIC ${interface} ${src})

#Set the target under Runtime folder
set_target_properties(Core PROPERTIES FOLDER Runtime)

# Set output directory
set_target_properties(Core PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/target/${CMAKE_SYSTEM_NAME}_${CMAKE_CXX_COMPILER_ID}_${CMAKE_SYSTEM_PROCESSOR}/${CMAKE_BUILD_TYPE})

# Set MSVC PDB
if(MSVC)
	set_target_properties(Core PROPERTIES PDB_OUTPUT_DIRECTORY  ${CMAKE_CURRENT_SOURCE_DIR}/lib/${CMAKE_BUILD_TYPE})
	set_target_properties(Core PROPERTIES PDB_NAME "Core" PDB_NAME_DEBUG "Core" COMPILE_PDB_NAME "Core" COMPILE_PDB_NAME_DEBUG "Core")
endif()

# Include Interface directory to inclusion path
target_include_directories(Core PRIVATE src)

# Add defines
target_compile_definitions(
	Core
	PRIVATE
	_HAS_EXCEPTIONS=0
	_CRT_SECURE_NO_WARNINGS
	_UNICODE
	UNICODE
	HD_GLOBAL_NAMESPACE_TYPES
	# Debug specific compiler flags
	$<$<CONFIG:Debug>:HD_DEBUG>
	# Release specific compiler flags
	$<$<CONFIG:Release>:HD_RELEASE>
	# MinSizeRel specific compiler flags
	$<$<CONFIG:MinSizeRel>:HD_RELEASE>
	# DebugOptimized specific compiler flags
	$<$<CONFIG:RelWithDebInfo>:HD_DEBUGOPTIMIZED>
)
