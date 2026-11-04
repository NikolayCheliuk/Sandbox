#
# DailyTemperatures
#

# executables
add_executable(
  DailyTemperatures
  source/main.cpp
)

target_include_directories(
  DailyTemperatures
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
