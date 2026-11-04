#
# RottingOranges
#

# executables
add_executable(
  RottingOranges
  source/main.cpp
)

target_include_directories(
  RottingOranges
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
