#
# NextGreaterElement1
#

# executables
add_executable(
  NextGreaterElement1
  source/main.cpp
)

target_include_directories(
  NextGreaterElement1
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
