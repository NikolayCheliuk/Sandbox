#
# CloneGraph
#

# executables
add_executable(
  CloneGraph
  source/main.cpp
)

target_include_directories(
  CloneGraph
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
