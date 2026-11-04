#
# Number Of Provinces
#

# executables
add_executable(
  NumberOfProvinces
  source/main.cpp
)

target_include_directories(
  NumberOfProvinces
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
