#include <gtest/gtest.h>
#include "RollingCode.h"

using namespace bevis_FinalProject;

TEST(RollingCode, Default_Constructor){
    RollingCode code = RollingCode();
    int temp = code.getSeed();
    ASSERT_EQ(temp, -1);
}

TEST(RollingCode, Constructor_With_seed){
    int s = 1827;
    RollingCode code = RollingCode(s);
    int temp = code.getSeed();
    ASSERT_EQ(temp, s);
}

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}