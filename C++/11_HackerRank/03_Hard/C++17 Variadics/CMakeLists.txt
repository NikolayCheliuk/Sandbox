#
# C++17 Variadics
#

# executables
add_executable(
  Cpp17Variadics
  source/main.cpp
)

target_include_directories(
  Cpp17Variadics
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
