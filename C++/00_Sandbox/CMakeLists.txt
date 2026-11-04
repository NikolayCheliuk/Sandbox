#
# Sandbox
#

# executables
add_executable(
  Sandbox
  source/main.cpp
)

target_include_directories(
  Sandbox
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
