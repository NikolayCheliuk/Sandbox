#
# LongestIncreasingSubsequence
#

# executables
add_executable(
  LongestIncreasingSubsequence
  source/main.cpp
)

target_include_directories(
  LongestIncreasingSubsequence
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
