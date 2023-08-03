# CMakeList.txt : CMake project for cmake-deneme, include source and define
# project specific logic here.
#

cmake_minimum_required (VERSION 3.14) # 3.8 ?

project ("iltren-staj")

add_subdirectory("Algorithm")
add_subdirectory("Utility")
add_subdirectory("UtilityUnitTest")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET cmake-deneme PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
