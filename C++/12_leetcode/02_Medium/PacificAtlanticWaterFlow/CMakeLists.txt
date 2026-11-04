#
# PacificAtlanticWaterFlow
#

# executables
add_executable(
  PacificAtlanticWaterFlow
  source/main.cpp
)

target_include_directories(
  PacificAtlanticWaterFlow
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
