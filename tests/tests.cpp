#include "gtest/gtest.h"

int main(int argc, char *argv[])
{
    int ret = RUN_ALL_TESTS();
    return ret;
}

TEST(Config, default_conf)
{
    EXPECT_TRUE(true);
}