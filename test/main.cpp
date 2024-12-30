
#define CATCH_CONFIG_RUNNER
#include <catch2/catch_all.hpp>

int main(int argc, char *argv[])
{
    Catch::Session session;

    // session.configData().showSuccessfulTests = true;
    session.configData().abortAfter = 1;

    int result = session.run(argc, argv);

    return result;
}