#
# LRUCache
#

# executables
add_executable(
  LRUCache
  source/main.cpp
)

target_include_directories(
  LRUCache
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
