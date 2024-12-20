// #include <gtest/gtest.h>

// int main(int argc, char **argv)
// {
//     ::testing::InitGoogleTest(&argc, argv);
//     return RUN_ALL_TESTS();
// }

#include <catch2/catch_session.hpp>

int main(int argc, char *argv[])
{
    return Catch::Session().run(argc, argv);
}