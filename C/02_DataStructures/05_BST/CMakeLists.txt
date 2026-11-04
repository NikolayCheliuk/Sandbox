#
# BST
#

#
# BST Object Library
#

add_library(
  BSTObj OBJECT
  source/BST.c
)

target_include_directories(
  BSTObj
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(
  BSTObj
  PUBLIC
  UtilsLib
  ErrorCodesLib
)

#
# BST Library
#

add_library(
  BSTLib STATIC
  $<TARGET_OBJECTS:BSTObj>
)

target_link_libraries(
  BSTLib
  PUBLIC
  BSTObj
)

#
# BST Test
#

if (BUILD_TESTING)
  add_executable(
    BSTTest
    test/BSTTest.c
  )

  target_link_libraries(
    BSTTest
    PRIVATE
    BSTLib
    TestUtilsLib
  )

  add_test(
    NAME BSTTest
    COMMAND BSTTest
  )
endif()
