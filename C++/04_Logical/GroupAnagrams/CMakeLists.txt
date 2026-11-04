#
# Group Anagrams
#

# executables
add_executable(
  GroupAnagrams
  source/main.cpp
)

target_include_directories(
  GroupAnagrams
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
