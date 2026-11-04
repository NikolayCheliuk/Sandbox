#
# HashTable
#

#
# HashTable Object Library
#

add_library(
  HashTableObj OBJECT
  source/HashTable.c
)

target_include_directories(
  HashTableObj
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(
  HashTableObj
  PUBLIC
  UtilsLib
  ErrorCodesLib
)

#
# HashTable Library
#

add_library(
  HashTableLib STATIC
  $<TARGET_OBJECTS:HashTableObj>
)

target_link_libraries(
  HashTableLib
  PUBLIC
  HashTableObj
)

#
# HashTable Test
#

if (BUILD_TESTING)

  add_executable(
    HashTableTest
    test/HashTableTest.c
  )

  target_link_libraries(
    HashTableTest
    PRIVATE
    HashTableLib
    TestUtilsLib
  )

  add_test(
    NAME HashTableTest
    COMMAND HashTableTest
  )
endif()
