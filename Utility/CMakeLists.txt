project("Utility")

cmake_minimum_required (VERSION 3.14) # 3.8 ?

add_executable (cmake-deneme "src/cmake-deneme.cpp")
target_include_directories(cmake-deneme PRIVATE include)