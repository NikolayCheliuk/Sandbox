#
# TwoSumII - Sorted
#

# executables
add_executable(
  TwoSumII
  source/main.cpp
)

target_include_directories(
  TwoSumII
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
