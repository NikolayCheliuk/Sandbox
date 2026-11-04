#
# Kth Largest Element in an Array
#

# executables
add_executable(
  KthLargestElementInAnArray
  source/main.cpp
)

target_include_directories(
  KthLargestElementInAnArray
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
