#
# SearchSort
#

# executables
add_executable(
  SearchSort
  source/main.cpp
)

target_include_directories(
  SearchSort
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
