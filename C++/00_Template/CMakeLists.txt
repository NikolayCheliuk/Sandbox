#
# Template
#

# executables
add_executable(
  Template
  source/main.cpp
)

target_include_directories(
  Template
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
