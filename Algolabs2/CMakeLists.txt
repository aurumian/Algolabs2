# CMakeList.txt : CMake project for Algolabs2, include source and define
# project specific logic here.
#
cmake_minimum_required(VERSION 3.14)
project(my_project)

# GoogleTest requires at least C++11
set(CMAKE_CXX_STANDARD 14)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
  Algolab2
  Algolabs2.cpp
)
target_link_libraries(
  Algolab2
  gtest_main
)

include(GoogleTest)
gtest_discover_tests(Algolab2)