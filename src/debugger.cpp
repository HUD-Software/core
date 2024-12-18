#if defined(HD_COMPILER_CLANG) || defined(HD_COMPILER_GCC)
    #include "debugger/debugger_linux.cpp"
#elif defined(HD_COMPILER_EMSCRIPTEN)
    #include "debugger/debugger_browser.cpp"
#endif