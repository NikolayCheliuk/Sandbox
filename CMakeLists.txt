#
# Sandbox
#

cmake_minimum_required(VERSION 3.20)
project(Sandbox LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)

include(CTest)

add_subdirectory(C)
add_subdirectory(C++)
