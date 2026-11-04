#
# StringStream
#

# executables
add_executable(
  StringStream
  source/main.cpp
)

target_include_directories(
  StringStream
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
