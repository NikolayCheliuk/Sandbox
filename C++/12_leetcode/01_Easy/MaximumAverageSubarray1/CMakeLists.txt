#
# MaximumAverageSubarray1
#

# executables
add_executable(
  MaximumAverageSubarray1
  source/main.cpp
)

target_include_directories(
  MaximumAverageSubarray1
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
