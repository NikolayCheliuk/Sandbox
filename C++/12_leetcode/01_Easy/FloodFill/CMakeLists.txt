#
# FloodFill
#

# executables
add_executable(
  FloodFill
  source/main.cpp
)

target_include_directories(
  FloodFill
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
