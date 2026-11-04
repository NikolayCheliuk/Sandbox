#
# SmallMemory1
#

# executables
add_executable(
  SmallMemory1
  source/main.cpp
  source/SmallMemory1.cpp
)

target_include_directories(
  SmallMemory1
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
