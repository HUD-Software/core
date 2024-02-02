<div align="center">
  <img width="80%" background-color="white" src="https://hud-software.github.io/bande.png" alt="HUD-Software"/>
  <h1>Core</h1>
</div>

**_Table of contents_**

1. [Status](#status)
1. [Description](#description)
<!-- 2. [Targets](#targets)
    1. [core](#core-target)
    2. [core_test](#core_test-target) -->
    
</br>

# Status

| | |
| --- | --- |
| Windows build and test | [![msvc_2022_x86-64](https://img.shields.io/github/actions/workflow/status/hud-software/core/windows_msvc_2022_x86-64.yml?label=MSVC%202022%20x86-64&logo=C%2B%2B&logoColor=lightgrey&style=flat-square)](https://github.com/HUD-Software/core/actions/workflows/windows_msvc_2022_x86-64.yml) [![clang-cl_2022_x86-64](https://img.shields.io/github/actions/workflow/status/hud-software/core/windows_clang_cl_2022_x86-64.yml?label=Clang-cl%202022%20x86-64&logo=C%2B%2B&logoColor=lightgrey&style=flat-square)](https://github.com/HUD-Software/core/actions/workflows/windows_clang_cl_2022_x86-64.yml) |
| Ubuntu build and test | [![clang_14_x86-64](https://img.shields.io/github/actions/workflow/status/hud-software/core/ubuntu_clang14_x86-64.yml?label=Clang%2014%20x86-64&logo=C%2B%2B&logoColor=lightgrey&style=flat-square)](https://github.com/HUD-Software/core/actions/workflows/ubuntu_clang14_x86-64.yml) [![gcc_12_x86-64](https://img.shields.io/github/actions/workflow/status/hud-software/core/ubuntu_gcc12_x86-64.yml?label=GCC%2012%20x86-64&logo=C%2B%2B&logoColor=lightgrey&style=flat-square)](https://github.com/HUD-Software/core/actions/workflows/ubuntu_gcc12_x86-64.yml) |
| Sanitizer | [![MSVC](https://img.shields.io/github/actions/workflow/status/hud-software/core/windows_msvc_2022_x86-64.yml?label=MSVC%202022%20x86-64&logo=C%2B%2B&logoColor=lightgrey&style=flat-square)](https://github.com/HUD-Software/core/actions/workflows/sanitizer_msvc.yml) [![GCC](https://img.shields.io/github/actions/workflow/status/hud-software/core/sanitizer_gcc12.yml?label=GCC%2012%20x86-64&logo=C%2B%2B&logoColor=lightgrey&style=flat-square)](https://github.com/HUD-Software/core/actions/workflows/sanitizer_gcc12.yml) [![Clang](https://img.shields.io/github/actions/workflow/status/hud-software/core/sanitizer_clang14.yml?label=Clang%2014%20x86-64&logo=C%2B%2B&logoColor=lightgrey&style=flat-square)](https://github.com/HUD-Software/core/actions/workflows/sanitizer_clang14.yml) |
| Coverage | [![codecov](https://img.shields.io/codecov/c/github/hud-software/core?label=Codecov&logo=Codecov&logoColor=lightgrey&style=flat-square)](https://app.codecov.io/gh/HUD-Software/core) |
| Quality | [![codeql](https://img.shields.io/github/actions/workflow/status/hud-software/core/codeQL.yml?label=CodeQL%20Quality&logo=C%2B%2B&logoColor=lightgrey&style=flat-square)](https://github.com/HUD-Software/core/actions/workflows/codeQL.yml) [![codacy](https://img.shields.io/codacy/grade/8014adeaff854f95b7688b8bed741964?label=Codacy%20Quality&logo=Codacy&logoColor=lightgrey&style=flat-square)](https://app.codacy.com/gh/HUD-Software/core/) |


</br>

# Description

**Core** is the heart of the HUD engine for [HUD-Software](https://github.com/HUD-Software).

It provides low-level C++ features that are close to the C++ STL implementation:

- **Containers:** array (std::vector), pair, tuple, optional, shared_pointer, unique_pointer, etc.
- **Debugging features:** conditional break, debugger attached checker, call stacks, etc.
- **Memory:** dynamic allocations, slicing, constexpr allocations/constructions/destructions, etc.
- **Strings:** UTF-8 strings, ANSI strings, platform-specific strings, etc.

**Core** follows the STL interface but permits making changes and additions to the STL specification to improve productivity, limit bugs, and enhance performance. It focuses on code quality, robustness, and performance.

**Core** follows the [HUD-Software](https://github.com/HUD-Software) project organization:

- The `src` directory contains the source and interface of the `Core`.
  - The `src/core` directory contains the interface of the `Core` library. This is the directory that will be included in C++ user code to use the library.

- The `test` directory contains the source of all tests of the `Core` in `src`. Code coverage checks ensure that `test` covers all `src` code.
<!-- 
## targets

### `core` target

This is the library target. It produce a library called `core` that can be used with the interfaces describes in `src/core` directory.

### `core_test` target

This is the test executable target. It produce a test executable that performs all `core` tests. -->
