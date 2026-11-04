#
# Video Player
#

# executables
add_executable(
  VideoPlayer
  source/main.cpp
  source/VideoPlayer.cpp
)

target_include_directories(
  VideoPlayer
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)