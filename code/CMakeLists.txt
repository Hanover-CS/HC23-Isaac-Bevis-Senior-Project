cmake_minimum_required(VERSION 3.5) # version can be different

set(GOOGLETEST_VERSION 1.12)

project(Rolling_Codes_Tests) #name of your project

add_subdirectory(googletest) # add googletest subdirectory

include_directories(googletest/include) # this is so we can #include <gtest/gtest.h>

add_executable(rollingCodeTests rollingCodeTests.cpp) # add this executable

target_link_libraries(rollingCodeTests PRIVATE gtest) # link google test to this executable

