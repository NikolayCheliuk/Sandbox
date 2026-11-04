#
# Queue
#

#
# Queue Object Library
#

add_library(
  QueueObj OBJECT
  source/Queue.c
)

target_include_directories(
  QueueObj
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(
  QueueObj
  PUBLIC
  RingBufferLib
  UtilsLib
  ErrorCodesLib
)

#
# Queue Library
#

add_library(
  QueueLib STATIC
  $<TARGET_OBJECTS:QueueObj>
)

target_link_libraries(
  QueueLib
  PUBLIC
  QueueObj
)

#
# Queue Test
#

if (BUILD_TESTING)
  add_executable(
    QueueTest
    test/QueueTest.c
  )

  target_link_libraries(
    QueueTest
    PRIVATE
    QueueLib
    TestUtilsLib
  )

  add_test(
    NAME QueueTest
    COMMAND QueueTest
  )
endif()
