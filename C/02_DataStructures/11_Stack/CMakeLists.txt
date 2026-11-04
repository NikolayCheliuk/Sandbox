#
# Stack
#

#
# Stack Object Library
#

add_library(
  StackObj OBJECT
  source/Stack.c
)

target_include_directories(
  StackObj
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(
  StackObj
  PUBLIC
  VectorLib
  UtilsLib
  ErrorCodesLib
)

#
# Stack Library
#

add_library(
  StackLib STATIC
  $<TARGET_OBJECTS:StackObj>
)

target_link_libraries(
  StackLib
  PUBLIC
  StackObj
)

#
# Stack Test
#

if (BUILD_TESTING)
  add_executable(
    StackTest
    test/StackTest.c
  )

  target_link_libraries(
    StackTest
    PRIVATE
    StackLib
    TestUtilsLib
  )

  add_test(
    NAME StackTest
    COMMAND StackTest
  )
endif()
