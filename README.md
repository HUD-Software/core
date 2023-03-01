<div align="center">
  <img height="200" background-color="white" src="https://hud-software.github.io/bande.png" alt="HUD-Software"/>
  <h1>Core</h1>
</div>

**_Table of contents_**

1. [Status](#status)
1. [Description](#description)
2. [Targets](#targets)
    1. [core](#core-target)
    2. [core_test](#core_test-target)
    
</br>

# Status

***Windows build and test***

[![cl_x86-64](https://img.shields.io/github/actions/workflow/status/hud-software/core/windows_cl_x86-64.yml?label=CL%20compiler%20x86-64&logo=C%2B%2B&logoColor=lightgrey&style=flat-square)](https://github.com/HUD-Software/core/actions/workflows/windows_cl_x86-64.yml)
[![clang-cl_x86-64](https://img.shields.io/github/actions/workflow/status/hud-software/core/windows_clang_x86-64.yml?label=Clang-cl%20compiler%20x86-64&logo=C%2B%2B&logoColor=lightgrey&style=flat-square)](https://github.com/HUD-Software/core/actions/workflows/windows_clang_x86-64.yml)

***Ubuntu build and test***

[![clang_x86-64](https://img.shields.io/github/actions/workflow/status/hud-software/core/ubuntu_clang_x86-64.yml?label=Clang%20compiler%20x86-64&logo=C%2B%2B&logoColor=lightgrey&style=flat-square)](https://github.com/HUD-Software/core/actions/workflows/ubuntu_clang_x86-64.yml)

***Quality***

[![codecov](https://img.shields.io/codecov/c/github/hud-software/core?label=Codecov&logo=Codecov&logoColor=lightgrey&style=flat-square)](https://app.codecov.io/gh/HUD-Software/core)
[![codeql](https://img.shields.io/github/actions/workflow/status/hud-software/core/codeQL.yml?label=CodeQL%20Quality&logo=C%2B%2B&logoColor=lightgrey&style=flat-square)](https://github.com/HUD-Software/core/actions/workflows/codeQL.yml)
[![codacy](https://img.shields.io/codacy/grade/8014adeaff854f95b7688b8bed741964?label=Codacy%20Quality&logo=Codacy&logoColor=lightgrey&style=flat-square)](https://app.codacy.com/gh/HUD-Software/core/)

</br>

# Description

`Core` is the heart of the HUD-engine for [HUD-Software](https://github.com/HUD-Software).

It provides low level C++ features like that is close to C++ STL implmentation:

- Containers: array (std::vector), pair, tuple, optional, shared_pointer, unique_pointer, etc...
- Debugging features: conditional break, debugger attached checker, callstacks, etc...
- Memory: dynamic allocations, slicing, constexpr allocations/constructions/destructions, etc...
- Strings: UTF-8 strings, ANSI strings, platform specific strings, etc...

`Core` follows the STL interface but permit to make change and addition to the STL specification to improve productivity, limit bugs and improve performance. It focus on code quality, robustness, and performance.

`Core` follow the [HUD-Software](https://github.com/HUD-Software) project organisation:

- `src` directory contains the source and interface of the `Core`
  - `src/core` directory contains the interface of the `Core` library. This is the directory that will be included in C++ user code to use the library.

- `test`directory contains the source of all tests of the `Core` in `src`. Code coverage checks that `test` covers all `src` code.

## targets

### `core` target

This is the library target. It produce a library called `core` that can be used with the interfaces describes in `src/core` directory.

### `core_test` target

This is the test executable target. It produce a test executable that performs all `core` tests.
