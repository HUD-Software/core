#include <gtest/gtest.h>
#include <emscripten.h>

int main(int argc, char **argv)
{

    ::testing::InitGoogleTest(&argc, argv);
    int result = RUN_ALL_TESTS();

#if defined(HD_COMPILER_EMSCRIPTEN)
    EM_ASM({
        const fs = require('fs');
        const fileContent = UTF8ToString($0);
        const fileName = UTF8ToString($1);
        fs.writeFileSync(fileName, fileContent); }, "/test_core_report.xml", "./test_core_report.xml");
#endif
    return result;
}