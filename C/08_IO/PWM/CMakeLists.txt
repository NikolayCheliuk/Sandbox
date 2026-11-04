#
# PWM
#

# executables
add_executable(
  PWM
  source/main.c
)

target_include_directories(
  PWM
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
