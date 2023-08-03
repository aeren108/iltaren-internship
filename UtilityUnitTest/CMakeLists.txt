project("UtilityUnitTest")

cmake_minimum_required (VERSION 3.14) # 3.8 ?

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
  list_test
  src/list_test.cpp
)

add_executable(
  vector_test
  src/vector_test.cpp
)

target_link_libraries(
  vector_test
  GTest::gtest_main
)

target_link_libraries(
  list_test
  GTest::gtest_main
)

target_include_directories(list_test PRIVATE include "../Utility/include")
target_include_directories(vector_test PRIVATE include "../Utility/include")

include(GoogleTest)
gtest_discover_tests(list_test)