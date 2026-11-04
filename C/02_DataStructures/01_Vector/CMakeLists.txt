#
# Vector
#

#
# Vector Object Library
#

add_library(
  VectorObj OBJECT
  source/Vector.c
)

target_include_directories(
  VectorObj
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(
  VectorObj
  PUBLIC
  UtilsLib
  ErrorCodesLib
)

#
# Vector Library
#

add_library(
  VectorLib STATIC
  $<TARGET_OBJECTS:VectorObj>
)

target_link_libraries(
  VectorLib
  PUBLIC
  VectorObj
)

#
# Vector Test
#

if (BUILD_TESTING)
  add_executable(
    VectorTest
    test/VectorTest.c
  )

  target_link_libraries(
    VectorTest
    PRIVATE
    VectorLib
    TestUtilsLib
  )

  add_test(
    NAME VectorTest
    COMMAND VectorTest
  )
endif()
