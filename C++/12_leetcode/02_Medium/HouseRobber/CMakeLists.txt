#
# House Robber
#

# executables
add_executable(
  HouseRobber
  source/main.cpp
)

target_include_directories(
  HouseRobber
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
