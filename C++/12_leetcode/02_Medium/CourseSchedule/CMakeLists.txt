#
# Course Schedule
#

# executables
add_executable(
  CourseSchedule
  source/main.cpp
)

target_include_directories(
  CourseSchedule
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
