#
# Filmography
#

# executables
add_executable(
  Filmography
  source/main.cpp
  source/Filmography.cpp
)

target_include_directories(
  Filmography
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)