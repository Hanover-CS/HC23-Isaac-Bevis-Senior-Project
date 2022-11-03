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
    unsigned long correct = 32728353; // next code using seed 3425
    unsigned long temp = code.next();
    ASSERT_EQ(temp, correct);
}

TEST(RollingCode_Generator, Generator_Produces_Predictable_Next_Num_Multiple) {
    unsigned long s = 3425;
    RollingCode code = RollingCode(s);
    unsigned long correct = 3330697183; // next next next code using seed 3425
    unsigned long temp = code.next();
    temp = code.next();
    temp = code.next();
    ASSERT_EQ(temp, correct);
}

TEST(RollingCode_Generator, Generator_Produces_Predictable_Next_Num_When_s_is_Max) {
    unsigned long s = 4294967295; // max unsigned long 4294967295 (32 bits)
    RollingCode code = RollingCode(s);
    unsigned long correct = 2630751737; // next code using seed 4294967295
    unsigned long temp = code.next();
    ASSERT_EQ(temp, correct);
}

// NOTE: code1 represents the vehicles code and code2 represents code sent by key
//      code1 should never be ahead of code2 because the vehicles code only increments 
//      when it receives valid code from key (Program is okay to crash if this happens)
//      so I won't test this case

TEST(RollingCode_Comparer_Simple_Seed, Matches_No_Next_in_Sync) {
    unsigned long s = 100;
    RollingCode code1 = RollingCode(s);
    RollingCode code2 = RollingCode(s);
    bool result = code1.matches(code2.getSeed());
    EXPECT_TRUE(result);
}

TEST(RollingCode_Comparer_Simple_Seed, Matches_With_Next_in_Sync) {
    unsigned long s = 100;
    RollingCode code1 = RollingCode(s);
    RollingCode code2 = RollingCode(s);
    code1.next();
    code2.next();
    bool result = code1.matches(code2.getSeed());
    EXPECT_TRUE(result);
}

TEST(RollingCode_Comparer_Simple_Seed, Matches_With_Next_Code2_Desync) {
    unsigned long s = 100;
    RollingCode code1 = RollingCode(s);
    RollingCode code2 = RollingCode(s);
    code1.next();
    code2.next();
    code2.next();
    code2.next();
    bool result = code1.matches(code2.getSeed());
    EXPECT_TRUE(result);
}

// Numbers chosen so that the total would be more than 100 
TEST(RollingCode_Comparer_Simple_Seed, Multiple_Matches_First_Desync_50_Second_Desync_80) {
    unsigned long s = 100;
    RollingCode code1 = RollingCode(s);
    RollingCode code2 = RollingCode(s);
    for(int i=0; i<=50; i++){
        code2.next();
    }
    bool result1 = code1.matches(code2.getSeed());
    for(int i=0; i<=80; i++){
        code2.next();
    }
    bool result2 = code1.matches(code2.getSeed());
    EXPECT_TRUE(result1);
    EXPECT_TRUE(result2);
}

TEST(RollingCode_Comparer_Simple_Seed, No_Match_Code2_Desync_By_101) {
    unsigned long s = 100;
    RollingCode code1 = RollingCode(s);
    RollingCode code2 = RollingCode(s);
    for(int i=0; i<=101; i++){
        code2.next();
    }
    bool result = code1.matches(code2.getSeed());
    EXPECT_FALSE(result);
}

TEST(RollingCode_Comparer_Max_Seed, Matches_No_Next_in_Sync) {
    unsigned long s = 4294967295; // max unsigned long 4294967295 (32 bits)
    RollingCode code1 = RollingCode(s);
    RollingCode code2 = RollingCode(s);
    bool result = code1.matches(code2.getSeed());
    EXPECT_TRUE(result);
}

TEST(RollingCode_Comparer_Max_Seed, Matches_With_Next_in_Sync) {
    unsigned long s = 4294967295;
    RollingCode code1 = RollingCode(s);
    RollingCode code2 = RollingCode(s);
    code1.next();
    code2.next();
    bool result = code1.matches(code2.getSeed());
    EXPECT_TRUE(result);
}

TEST(RollingCode_Comparer_Max_Seed, Matches_With_Next_Code2_Desync) {
    unsigned long s = 4294967295;
    RollingCode code1 = RollingCode(s);
    RollingCode code2 = RollingCode(s);
    code1.next();
    code2.next();
    code2.next();
    code2.next();
    bool result = code1.matches(code2.getSeed());
    EXPECT_TRUE(result);
}

// Numbers chosen so that the total would be more than 100 
TEST(RollingCode_Comparer_Max_Seed, Multiple_Matches_First_Desync_50_Second_Desync_80) {
    unsigned long s = 4294967295;
    RollingCode code1 = RollingCode(s);
    RollingCode code2 = RollingCode(s);
    for(int i=0; i<=50; i++){
        code2.next();
    }
    bool result1 = code1.matches(code2.getSeed());
    for(int i=0; i<=80; i++){
        code2.next();
    }
    bool result2 = code1.matches(code2.getSeed());
    EXPECT_TRUE(result1);
    EXPECT_TRUE(result2);
}

TEST(RollingCode_Comparer_Max_Seed, No_Match_Code2_Desync_By_101) {
    unsigned long s = 4294967295;
    RollingCode code1 = RollingCode(s);
    RollingCode code2 = RollingCode(s);
    for(int i=0; i<=101; i++){
        code2.next();
    }
    bool result = code1.matches(code2.getSeed());
    EXPECT_FALSE(result);
}

TEST(RollingCode_Comparer_Custom_M_C_A_Values, Matches_With_Next_in_Sync) {
    unsigned long s = 100;
    RollingCode code1 = RollingCode(s, 1735814347, 5957, 2441);
    RollingCode code2 = RollingCode(s, 1735814347, 5957, 2441);
    code2.next();
    code2.next();
    bool result = code1.matches(code2.getSeed());
    EXPECT_TRUE(result);
}

TEST(RollingCode_Comparer_Custom_M_C_A_Values, No_Match_Code2_Different_Values) {
    unsigned long s = 3827;
    RollingCode code1 = RollingCode(s, 1735814347, 5957, 2441);
    RollingCode code2 = RollingCode(s, 1352817314, 8642, 5437);
    code2.next();
    bool result = code1.matches(code2.getSeed());
    EXPECT_FALSE(result);
}

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}