#
# Max Area Of Island
#

# executables
add_executable(
  MaxAreaOfIsland
  source/main.cpp
)

target_include_directories(
  MaxAreaOfIsland
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
