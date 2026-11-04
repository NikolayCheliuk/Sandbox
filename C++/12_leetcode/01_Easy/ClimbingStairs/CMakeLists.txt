#
# Climbing Stairs
#

# executables
add_executable(
  ClimbingStairs
  source/main.cpp
)

target_include_directories(
  ClimbingStairs
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
