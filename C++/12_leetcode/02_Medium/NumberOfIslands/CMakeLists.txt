#
# Number Of Islands
#

# executables
add_executable(
  NumberOfIslands
  source/main.cpp
)

target_include_directories(
  NumberOfIslands
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
