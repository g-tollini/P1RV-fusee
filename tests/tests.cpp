#include "gtest/gtest.h"

TEST(Config, default_conf)
{
    EXPECT_TRUE(true);
}

int main(int argc, char *argv[])
{
    int ret = RUN_ALL_TESTS();
    return ret;
}