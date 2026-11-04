#
# C
#

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_subdirectory(00_ErrorCodes)

if (BUILD_TESTING)
  add_subdirectory(00_TestUtils)
endif()

add_subdirectory(01_Allocators)
add_subdirectory(02_DataStructures)
add_subdirectory(03_Algorithms)

add_subdirectory(08_IO)
add_subdirectory(09_StateMachine)
