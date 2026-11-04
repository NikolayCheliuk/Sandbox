#
# Attribute Parser
#

# executables
add_executable(
  AttributeParser
  source/main.cpp
)

target_include_directories(
  AttributeParser
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
