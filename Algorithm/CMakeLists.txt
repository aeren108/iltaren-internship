project("Algorithm")

cmake_minimum_required (VERSION 3.14) # 3.8 ?

add_executable(maxtravel "src/maxtravel.cpp" "src/local_optima.cpp")
target_include_directories(maxtravel PRIVATE include "../Utility/include")

add_executable(bus_guesser "src/bus_guesser.cpp")
target_include_directories(bus_guesser PRIVATE include "../Utility/include")
