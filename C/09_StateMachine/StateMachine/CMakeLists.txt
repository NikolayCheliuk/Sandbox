#
# State
#

# library
add_library(
  StateMachineLib
  source/StateMachine.c
)

target_include_directories(
  StateMachineLib
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(
  StateMachineLib
  PUBLIC
)

# executables
add_executable(
  StateMachineTest
  test/StateMachineTest.c
)

target_link_libraries(
  StateMachineTest
  PRIVATE
  StateMachineLib
)
