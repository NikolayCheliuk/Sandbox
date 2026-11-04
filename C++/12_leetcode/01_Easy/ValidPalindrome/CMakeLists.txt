#
# Template
#

# executables
add_executable(
  ValidPalindrome
  source/main.cpp
)

target_include_directories(
  ValidPalindrome
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
