#
# WordSearch
#

# executables
add_executable(
  WordSearch
  source/main.cpp
)

target_include_directories(
  WordSearch
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
