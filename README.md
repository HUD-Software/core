<div align="center">
  <img width="200" height="200" background-color="white" src="https://avatars.githubusercontent.com/u/104791429"/>
  <h1>Core</h1>
</div>

| Build & test | Quality | Coverage |
|:--------------|---------|--------------|
|[![Windows cl build & test](https://github.com/HUD-Software/core/actions/workflows/windows_cl_x64.yml/badge.svg?branch=main)](https://github.com/HUD-Software/core/actions/workflows/windows_cl_x64.yml?label=Windows%20cl%20build%20%26%20test) <br> [![Windows clang-cl build & test](https://github.com/HUD-Software/core/actions/workflows/windows_clang_x64.yml/badge.svg?branch=main)](https://github.com/HUD-Software/core/actions/workflows/windows_clang_x64.yml?label=Windows%20clang-cl%20build%20%26%20test) <br> [![Ubuntu clang build & test](https://github.com/HUD-Software/core/actions/workflows/ubuntu.yml/badge.svg?branch=main)](https://github.com/HUD-Software/core/actions/workflows/ubuntu.yml?label=Ubuntu%20clang%20build%20%26%20test) | [![Codacy Badge](https://app.codacy.com/project/badge/Grade/8014adeaff854f95b7688b8bed741964)](https://www.codacy.com/gh/HUD-Software/core/dashboard?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=HUD-Software/core&amp;utm_campaign=Badge_Grade) <br> [![CodeQL Analysis](https://github.com/HUD-Software/core/actions/workflows/codeQL.yml/badge.svg?branch=main)](https://github.com/HUD-Software/core/actions/workflows/codeQL.yml) | [![codecov](https://codecov.io/gh/HUD-Software/core/branch/main/graph/badge.svg?token=KDDSR2NG0M)](https://codecov.io/gh/HUD-Software/core)|

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
