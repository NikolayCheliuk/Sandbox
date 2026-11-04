#
# Two Sum
#

# executables
add_executable(
  TwoSum
  source/main.cpp
)

target_include_directories(
  TwoSum
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
