#
# Data Structures
#

add_subdirectory(00_Utils)

add_subdirectory(01_Vector)
add_subdirectory(02_List)
add_subdirectory(03_RingBuffer)

add_subdirectory(04_HashTable)
add_subdirectory(05_BST)
add_subdirectory(06_Heap)

add_subdirectory(11_Stack)
add_subdirectory(12_Deque)
add_subdirectory(13_Queue)

#
# Data Structures Library
#

add_library(
  DataStructuresLib STATIC

  $<TARGET_OBJECTS:VectorObj>
  $<TARGET_OBJECTS:ListObj>
  $<TARGET_OBJECTS:RingBufferObj>

  $<TARGET_OBJECTS:HeapObj>
  $<TARGET_OBJECTS:HashTableObj>
  $<TARGET_OBJECTS:BSTObj>

  $<TARGET_OBJECTS:StackObj>
  $<TARGET_OBJECTS:DequeObj>
  $<TARGET_OBJECTS:QueueObj>

  $<TARGET_OBJECTS:UtilsObj>
)

target_include_directories(
  DataStructuresLib
  PUBLIC

  $<TARGET_PROPERTY:VectorObj,INTERFACE_INCLUDE_DIRECTORIES>
  $<TARGET_PROPERTY:ListObj,INTERFACE_INCLUDE_DIRECTORIES>
  $<TARGET_PROPERTY:RingBufferObj,INTERFACE_INCLUDE_DIRECTORIES>

  $<TARGET_PROPERTY:HeapObj,INTERFACE_INCLUDE_DIRECTORIES>
  $<TARGET_PROPERTY:HashTableObj,INTERFACE_INCLUDE_DIRECTORIES>
  $<TARGET_PROPERTY:BSTObj,INTERFACE_INCLUDE_DIRECTORIES>

  $<TARGET_PROPERTY:StackObj,INTERFACE_INCLUDE_DIRECTORIES>
  $<TARGET_PROPERTY:DequeObj,INTERFACE_INCLUDE_DIRECTORIES>
  $<TARGET_PROPERTY:QueueObj,INTERFACE_INCLUDE_DIRECTORIES>

  $<TARGET_PROPERTY:UtilsObj,INTERFACE_INCLUDE_DIRECTORIES>
)
