#
# Merge Intervals
#

# executables
add_executable(
  MergeIntervals
  source/main.cpp
)

target_include_directories(
  MergeIntervals
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
