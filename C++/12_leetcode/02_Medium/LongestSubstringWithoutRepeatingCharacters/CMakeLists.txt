#
# Longest Substring Without Repeating Characters
#

# executables
add_executable(
  LongestSubstringWithoutRepeatingCharacters
  source/main.cpp
)

target_include_directories(
  LongestSubstringWithoutRepeatingCharacters
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
