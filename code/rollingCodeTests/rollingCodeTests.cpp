#include <gtest/gtest.h>
#include "RollingCode.h"

using namespace bevis_FinalProject;


TEST(RollingCode_Constructors, Constructor_With_seed) {
    unsigned long s = 2000000;
    RollingCode code = RollingCode(s);
    unsigned long temp = code.getSeed();
    ASSERT_EQ(temp, s);
}

TEST(RollingCode_Constructors, Constructor_With_seed_At_Max) {
    unsigned long s = 4294967295; // max unsigned long 4294967295 (32 bits)
    RollingCode code = RollingCode(s);
    unsigned long temp = code.getSeed();
    ASSERT_EQ(temp, s);
}

TEST(RollingCode_Generator, Generator_Produces_Predictable_Next_Num) {
    unsigned long s = 3425;
    RollingCode code = RollingCode(s);
    unsigned long correct = 4693926; // next code using seed 3425
    unsigned long temp = code.next();
    ASSERT_EQ(temp, correct);
}

TEST(RollingCode_Generator, Generator_Produces_Predictable_Next_Num_Multiple) {
    unsigned long s = 3425;
    RollingCode code = RollingCode(s);
    unsigned long correct = 629613765; // next next next code using seed 3425
    unsigned long temp = code.next();
    temp = code.next();
    temp = code.next();
    ASSERT_EQ(temp, correct);
}

TEST(RollingCode_Generator, Generator_Produces_Predictable_Next_Num_When_s_is_Max) {
    unsigned long s = 4294967295; // max unsigned long 4294967295 (32 bits)
    RollingCode code = RollingCode(s);
    unsigned long correct = 2623497498; // next code using seed 4294967295
    unsigned long temp = code.next();
    ASSERT_EQ(temp, correct);
}

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}