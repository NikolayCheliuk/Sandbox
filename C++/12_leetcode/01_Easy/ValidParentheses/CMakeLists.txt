#
# Valid Parentheses
#

# executables
add_executable(
  ValidParentheses
  source/main.cpp
)

target_include_directories(
  ValidParentheses
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
