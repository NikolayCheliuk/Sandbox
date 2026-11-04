#
#
#

# executables
add_executable(
  SmallMemory2
  source/main.cpp
  source/SmallMemory2.cpp
)

target_include_directories(
  SmallMemory2
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
