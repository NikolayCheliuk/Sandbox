#
# Heap
#

#
# Heap Object Library
#

add_library(
  HeapObj OBJECT
  source/Heap.c
)

target_include_directories(
  HeapObj
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(
  HeapObj
  PUBLIC
  VectorLib
  UtilsLib
  ErrorCodesLib
)

#
# Heap Library
#

add_library(
  HeapLib STATIC
  $<TARGET_OBJECTS:HeapObj>
)

target_link_libraries(
  HeapLib
  PUBLIC
  HeapObj
)

#
# Heap Test
#

if (BUILD_TESTING)
  add_executable(
    HeapTest
    test/HeapTest.c
  )

  target_link_libraries(
    HeapTest
    PRIVATE
    HeapLib
    TestUtilsLib
  )

  add_test(
    NAME HeapTest
    COMMAND HeapTest
  )
endif()
