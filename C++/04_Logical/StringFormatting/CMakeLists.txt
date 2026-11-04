#
# StringFormatting
#

# executables
add_executable(
  StringFormatting
  source/main.cpp
)

target_include_directories(
  StringFormatting
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
