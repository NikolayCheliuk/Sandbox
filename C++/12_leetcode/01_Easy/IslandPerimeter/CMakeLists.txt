#
# IslandPerimeter
#

# executables
add_executable(
  IslandPerimeter
  source/main.cpp
)

target_include_directories(
  IslandPerimeter
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
