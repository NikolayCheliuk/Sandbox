#
# C++11 Variadics
#

# executables
add_executable(
  Cpp11Variadics
  source/main.cpp
)

target_include_directories(
  Cpp11Variadics
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
