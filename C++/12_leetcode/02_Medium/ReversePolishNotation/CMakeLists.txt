#
# ReversePolishNotation
#

# executables
add_executable(
  ReversePolishNotation
  source/main.cpp
)

target_include_directories(
  ReversePolishNotation
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
