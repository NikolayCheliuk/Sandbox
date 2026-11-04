#
# Maximum Subarray / Kadane’s Algorithm
#

# executables
add_executable(
  MaximumSubarray_KadanesAlgorithm
  source/main.cpp
)

target_include_directories(
  MaximumSubarray_KadanesAlgorithm
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
