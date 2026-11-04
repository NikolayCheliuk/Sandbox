#
# Data Structure Utils
#

#
# Utils Object Library
#

add_library(
  UtilsObj OBJECT
  source/Utils.c
)

target_include_directories(
  UtilsObj
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(
  UtilsObj
  PUBLIC
  ErrorCodesLib
)

#
# Utils Library
#

add_library(
  UtilsLib STATIC
  $<TARGET_OBJECTS:UtilsObj>
)

target_link_libraries(
  UtilsLib
  PUBLIC
  UtilsObj
)

#
# Utils Test
#

if (BUILD_TESTING)
  add_executable(
    UtilsTest
    test/UtilsTest.c
  )

  target_link_libraries(
    UtilsTest
    PRIVATE
    UtilsLib
    TestUtilsLib
  )

  add_test(
    NAME UtilsTest
    COMMAND UtilsTest
  )
endif()
