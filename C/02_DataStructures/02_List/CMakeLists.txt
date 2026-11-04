#
# List
#

#
# List Object Library
#

add_library(
  ListObj OBJECT
  source/List.c
)

target_include_directories(
  ListObj
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(
  ListObj
  PUBLIC
  UtilsLib
  ErrorCodesLib
)

#
# List Library
#

add_library(
  ListLib STATIC
  $<TARGET_OBJECTS:ListObj>
)

target_link_libraries(
  ListLib
  PUBLIC
  ListObj
)

#
# List Test
#

if (BUILD_TESTING)
  add_executable(
    ListTest
    test/ListTest.c
  )

  target_link_libraries(
    ListTest
    PRIVATE
    ListLib
    TestUtilsLib
  )

  add_test(
    NAME ListTest
    COMMAND ListTest
  )
endif()
