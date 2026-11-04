#
# Pizza
#

# executables
add_executable(
  Pizza
  source/main.cpp
  source/Pizza.cpp
)

target_include_directories(
  Pizza
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)