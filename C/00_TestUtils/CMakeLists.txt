#
# Test Utils
#

#
# TestUtils Object Library
#

add_library(
  TestUtilsObj OBJECT
  source/TestUtils.c
)

target_include_directories(
  TestUtilsObj
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(
  TestUtilsObj
  PUBLIC
  ErrorCodesLib
)

#
# TestUtils Library
#

add_library(
  TestUtilsLib STATIC
  $<TARGET_OBJECTS:TestUtilsObj>
)

target_link_libraries(
  TestUtilsLib
  PUBLIC
  TestUtilsObj
)

#
# TestUtils Self Test
#

if (BUILD_TESTING)
  add_executable(
    TestUtilsTest
    test/TestUtilsTest.c
  )

  target_link_libraries(
    TestUtilsTest
    PRIVATE
    TestUtilsLib
  )

  add_test(
    NAME TestUtilsTest
    COMMAND TestUtilsTest
  )
endif()
