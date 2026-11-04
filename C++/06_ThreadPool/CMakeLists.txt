#
# Thread Pool
#

# executables
add_executable(
  ThreadPool
  source/main.cpp
  source/ThreadPool.cpp
)

target_include_directories(
  ThreadPool
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
