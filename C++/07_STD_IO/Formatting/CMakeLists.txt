#
# Formatting
#

# executables
add_executable(
  Formatting
  source/main.cpp
)

target_include_directories(
  Formatting
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
