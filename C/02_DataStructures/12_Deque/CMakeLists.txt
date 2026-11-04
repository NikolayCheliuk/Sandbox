#
# Deque
#

#
# Deque Object Library
#

add_library(
  DequeObj OBJECT
  source/Deque.c
)

target_include_directories(
  DequeObj
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(
  DequeObj
  PUBLIC
  ListLib
  UtilsLib
  ErrorCodesLib
)

#
# Deque Library
#

add_library(
  DequeLib STATIC
  $<TARGET_OBJECTS:DequeObj>
)

target_link_libraries(
  DequeLib
  PUBLIC
  DequeObj
)

#
# Deque Test
#

if (BUILD_TESTING)
  add_executable(
    DequeTest
    test/DequeTest.c
  )

  target_link_libraries(
    DequeTest
    PRIVATE
    DequeLib
    TestUtilsLib
  )

  add_test(
    NAME DequeTest
    COMMAND DequeTest
  )
endif()
