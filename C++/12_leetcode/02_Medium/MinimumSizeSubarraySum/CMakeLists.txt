#
# MinimumSizeSubarraySum
#

# executables
add_executable(
  MinimumSizeSubarraySum
  source/main.cpp
)

target_include_directories(
  MinimumSizeSubarraySum
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
