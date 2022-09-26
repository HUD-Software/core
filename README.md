
<div align="center">
  <img width="200" height="200" background-color="white" src="https://avatars.githubusercontent.com/u/104791429"/>
  <h1>Core</h1>
</div>

![Windows Build and test](https://img.shields.io/github/workflow/status/HUD-Software/core/Windows?label=windows%20build%20%26%20test)

# Description

`Core` is the heart of the HUD-engine for [HUD-Software](https://github.com/HUD-Software).

It provides low level C++ features like that is close to C++ STL implmentation:
    - Containers: array (std::vector), pair, tuple, optional, shared_pointer, unique_pointer, etc...
    - Debugging features: conditional break, debugger attached checker, callstacks, etc...
    - Memory: dynamic allocations, slicing, constexpr allocations/constructions/destructions, etc...
    - Strings: UTF-8 strings, ANSI strings, platform specific strings, etc...

`Core` try to follow the STL interface but permit to make change and addition to the STL specification to improve productivity, limiting bugs and improve performances. It focus on code quality, robustness, and performance.

# Usage

`Core` follow the [HUD-Software](https://github.com/HUD-Software) project organisations:

- `src` directory contains the source and interface of the `Core`
  - `src/core` directroy contains the interface of the `Core` library. This is the directory that will be included in C++ user code to use the library.
- `test`directory contains the source of all tests of the core in `src`. Code coverage check that `test`cover all `src` code.

## targets

### `core` target

This is the library target. It produce a library called `core` that can be used with the interfaces describes in `src/core` directory.

### `core_test` target

This si the test executable target. It produce a test executable that performs all `core` tests.