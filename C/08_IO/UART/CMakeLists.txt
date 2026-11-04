#
# UART
#

# executables
add_executable(
  UART
  source/main.c
)

target_include_directories(
  UART
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
