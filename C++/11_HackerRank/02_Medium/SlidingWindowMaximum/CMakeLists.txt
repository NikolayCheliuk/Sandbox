#
# SlidingWindowMaximum
#

# executables
add_executable(
  SlidingWindowMaximum
  source/main.cpp
)

target_include_directories(
  SlidingWindowMaximum
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
