#
# Arithmetic Graph
#

# executables
add_executable(
  ArithmeticGraph
  source/main.cpp
  source/Node.cpp
)

target_include_directories(
  ArithmeticGraph
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)