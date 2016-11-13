cmake_minimum_required(VERSION 2.6)

# Enable c++11
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Link runTests with what we want to test, GTest, and pthread library
add_executable(run trie.cpp tests.cpp)
target_link_libraries(run ${GTEST_LIBRARIES} pthread)
