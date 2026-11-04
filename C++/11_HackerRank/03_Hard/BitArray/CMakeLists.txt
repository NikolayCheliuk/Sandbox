#
# Bit Array
#

# executables
add_executable(
  BitArray
  source/main.cpp
)

target_include_directories(
  BitArray
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
