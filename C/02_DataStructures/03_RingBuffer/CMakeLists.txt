#
# RingBuffer
#

#
# RingBuffer Object Library
#

add_library(
  RingBufferObj OBJECT
  source/RingBuffer.c
)

target_include_directories(
  RingBufferObj
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(
  RingBufferObj
  PUBLIC
  UtilsLib
  ErrorCodesLib
)

#
# RingBuffer Library
#

add_library(
  RingBufferLib STATIC
  $<TARGET_OBJECTS:RingBufferObj>
)

target_link_libraries(
  RingBufferLib
  PUBLIC
  RingBufferObj
)

#
# RingBuffer Test
#

if (BUILD_TESTING)
  add_executable(
    RingBufferTest
    test/RingBufferTest.c
  )

  target_link_libraries(
    RingBufferTest
    PRIVATE
    RingBufferLib
    TestUtilsLib
  )

  add_test(
    NAME RingBufferTest
    COMMAND RingBufferTest
  )
endif()
