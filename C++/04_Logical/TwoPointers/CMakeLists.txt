#
# Two Pointers (Sorted Array / Target Sum)
#

# executables
add_executable(
  TwoPointers
  source/main.cpp
)

target_include_directories(
  TwoPointers
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
